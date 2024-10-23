/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
//#include "netif/ppp_oe.h"
#include "m460_emac.h"
#include "string.h"
#include "ethernetif.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

//[2024-10-16]for emac_m460.c to access lwip ethernetif object.
struct netif *_netif;
//[2024-10-21]to record m460 emac status(compa. to EthHandle.State)
static u8 nvtEthState = 0;
/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif
{
    struct eth_addr *ethaddr;
    /* Add whatever per-interface state that is needed here. */
};


/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
#ifdef LWIP_OS 
//sys_sem_t xRxSemaphore = NULL;
//sys_thread_t xRxThread = NULL;
static void eth_rx_thread_entry(void *parameter);
#endif
//void ethernetif_input(u16_t len, u8_t *buf, u32_t s, u32_t ns);
extern u8 my_mac_addr[6];
static void TIMER0_Init(void);

static void
low_level_init(struct netif *netif)
{
    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] = my_mac_addr[0];
    netif->hwaddr[1] = my_mac_addr[1];
    netif->hwaddr[2] = my_mac_addr[2];
    netif->hwaddr[3] = my_mac_addr[3];
    netif->hwaddr[4] = my_mac_addr[4];
    netif->hwaddr[5] = my_mac_addr[5];

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
#ifdef LWIP_IGMP
    netif->flags |= NETIF_FLAG_IGMP;
#endif

    //[2024-10-22]Add timer to do lwip scheduler
    TIMER0_Init();
    
    nvtEthState = EMAC_Open(&my_mac_addr[0]);

#ifdef LWIP_OS
    /*
    To do: Add notification mechanism for EMAC rx
    */

    if (sys_sem_new(&xRxSemaphore, 0) != ERR_OK)
    {
        while (1);
    }
    else if ((xRxThread = sys_thread_new("eth_rx", eth_rx_thread_entry, NULL, RX_THREAD_STACKSIZE, RX_THREAD_PRIO)) == NULL)
    {
        while (1);
    }
		    
    sys_arch_sem_wait(&xRxSemaphore, 0);
#endif    
}

// Implement NAPI
#ifdef LWIP_OS
static void eth_rx_thread_entry(void *parameter)
{
    int len;

    sys_sem_signal(&xRxSemaphore);

    while (1)
    {
        if (sys_arch_sem_wait(&xRxSemaphore, 0) != SYS_ARCH_TIMEOUT)    // Wait forever
        {
            while (1)
            {
                if ((len = EMAC_ReceivePkt()) <= 0)
                {
                    // Enable RX interrupt.
                    break;
                }
            }
        }
    }
}
#endif
/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
 //[2024-10-14]This function could be kept.
static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;
    u8_t *buf = NULL;
    u16_t len = 0;

    buf = EMAC_AllocatePktBuf();

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    for (q = p; q != NULL; q = q->next)
    {
        memcpy((u8_t *)&buf[len], q->payload, q->len);
        len = len + q->len;
    }
#ifdef TIME_STAMPING
    ETH_trigger_tx(len, p->flags & PBUF_FLAG_GET_TXTS ? p : NULL);
#else
    EMAC_TransmitPkt(buf, len);
#endif

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
 //[2024-10-15] The paramater are not coincident
//static struct pbuf *
//low_level_input(struct netif *netif, u16_t len, u8_t *buf)
//Fit STM32: 
static struct pbuf *low_level_input(struct netif *netif)
{
    /*
    To do: find missing len, *buf
    */
    struct pbuf *p, *q;
    //[2024-10-16] create dummy len, *buf.
    //In m460 implementation, 
    //data Rx is handled in EMAC_ISR, this function may not be called.
    u16_t len;
    u8_t *buf = NULL;

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (p != NULL)
    {

#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

        len = 0;
        /* We iterate over the pbuf chain until we have read the entire
        * packet into the pbuf. */
        for (q = p; q != NULL; q = q->next)
        {
            memcpy((u8_t *)q->payload, (u8_t *)&buf[len], q->len);
            len = len + q->len;
        }


#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.recv);
    }
    else
    {
        // do nothing. drop the packet
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
    }

    return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */

//[2024-10-15] The paramater are not coincident
//void
//ethernetif_input(u16_t len, u8_t *buf, u32_t s, u32_t ns)
//STM32: 
void ethernetif_input(struct netif *netif)
{
    /*
    To do: find missing len, s, ns
    s and ns may be optional
    */
    struct eth_hdr *ethhdr;
    struct pbuf *p;

    //[2024-10-16] create dummy len, *buf.
    //In m460 implementation, 
    //data Rx is handled in EMAC_ISR, this function may not be called.
    u16_t len;
    u8_t *buf = NULL;
    
    
    /* move received packet into a new pbuf */
    p = low_level_input(_netif);//[2024-10-22]use global netif 
    /* no packet could be read, silently ignore this */
    if (p == NULL) return;
#ifdef TIME_STAMPING
    p->ts_sec = s;
    p->ts_nsec = ns;
#endif
 
    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = (eth_hdr *)(p->payload);//[2024-10-16]Fix build error

    switch (htons(ethhdr->type))
    {
    /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
#if PPPOE_SUPPORT
    /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        /* full packet send to tcpip_thread to process */
        if (netif->input(p, netif) != ERR_OK)//[2024-10-15]use local netif 
        {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
            pbuf_free(p);
            p = NULL;
        }
        break;

    default:
        pbuf_free(p);
        p = NULL;
        break;
    }
}

#ifdef    TIME_STAMPING
void
ethernetif_loopback_input(struct pbuf *p)           // TODO: make sure packet not drop in input()
{
    struct eth_hdr *ethhdr;

    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;

    switch (htons(ethhdr->type))
    {
    /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
#if PPPOE_SUPPORT
    /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        /* full packet send to tcpip_thread to process */
        if (_netif->input(p, _netif) != ERR_OK)
        {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
            pbuf_free(p);
            p = NULL;
        }
        break;

    default:
        pbuf_free(p);
        p = NULL;
        break;
    }
}

#endif

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
    struct ethernetif *ethernetif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    _netif = netif;
    
    void * vp = mem_malloc(sizeof(struct ethernetif));//[2024-10-15]use local netif 
    //ethernetif = (ethernetif* )(vp);//[2024-10-16]Not known. To be solved.
    if (ethernetif == NULL)
    {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "nuvoton";
#endif /* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state = ethernetif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;

    ethernetif->ethaddr = (struct eth_addr *) & (netif->hwaddr[0]);

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}


//----------STM32--------//
/**
  * @brief  This function set a custom MAC address. This function must be called
  *         before ethernetif_init().
  * @param  mac: mac address
  * @retval None
  */
/*
to do: MAC_ADDR0
*/  
#define MAC_ADDR0 0x00
#define MAC_ADDR1 0x11
#define MAC_ADDR2 0x22
#define MAC_ADDR3 0x33
#define MAC_ADDR4 0x44
#define MAC_ADDR5 0x55
static uint8_t macaddress[6] = { MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5 }; 
void ethernetif_set_mac_addr(const uint8_t *mac)
{
  if ((mac != NULL) && !(ethernetif_is_init())) {
    memcpy(macaddress, mac, 6);
  }
}

/**
  * @brief  This function get the current MAC address.
  * @param  mac: mac address
  * @retval None
  */
void ethernetif_get_mac_addr(uint8_t *mac)
{
  if (mac != NULL) {
    memcpy(mac, macaddress, 6);
  }
}

/**
  * @brief Returns the current state
  *
  * @param None
  * @return 0 not initialized else 1
  */
uint8_t ethernetif_is_init(void)
{
  //STM32:
  //return (EthHandle.State != HAL_ETH_STATE_RESET);
  /*To do*/
  //This value is update in m460_emc-EMAC_Open
  //return 1: means is_init
  //return 0: means not_init
  return nvtEthState;
  
}

//u32_t sys_now(void)
//{
//  return 1;//HAL_GetTick();
//}

static volatile uint32_t u32Jiffies = 0;

void TMR0_IRQHandler(void)
{
    u32Jiffies += 10;
    TIMER_ClearIntFlag(TIMER0);
}

u32_t sys_now(void)
{

    return u32Jiffies;

}

static void TIMER0_Init(void)
{
    CLK_EnableModuleClock(TMR0_MODULE);

    // Select module clock source
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HIRC, 0);

    // Set timer frequency to 100HZ
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 100);

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);

    // Start Timer 0
    TIMER_Start(TIMER0);
}

#ifdef __cplusplus
}
#endif