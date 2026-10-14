/**************************************************************************//**
 * @file     nvt_eth.c
 * @version  V3.00
 * @brief    Nuvoton EMAC driver source file
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2022 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#ifndef __NVT_ETH_H__
#define __NVT_ETH_H__

/* Includes ------------------------------------------------------------------*/
#include "m460_emac.h"
#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/opt.h"

/* Exported types ------------------------------------------------------------*/
/* TCP connection state */
typedef enum
{
    TCP_NONE = 0,
    TCP_CONNECTED,
    TCP_RECEIVED,
    TCP_SENT,
    TCP_ACCEPTED,
    TCP_CLOSING,
} tcp_client_states;

/* Struct to store received data */
struct pbuf_data
{
    struct pbuf *p;     // the packet buffer that was received
    uint16_t available; // number of data
};

/* UDP structure */
struct udp_struct
{
    struct udp_pcb *pcb; /* pointer on the current udp_pcb */
    struct pbuf_data data;
    ip_addr_t ip;       // the remote IP address from which the packet was received
    u16_t port;         // the remote port from which the packet was received
#if LWIP_UDP
    void (*onDataArrival)(void);
#endif
};

/* TCP structure */
struct tcp_struct
{
    struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
    struct pbuf_data data;
    tcp_client_states state;      /* current connection state */
};

/* Exported constants --------------------------------------------------------*/
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   (uint8_t) 192
#define IP_ADDR1   (uint8_t) 168
#define IP_ADDR2   (uint8_t) 0
#define IP_ADDR3   (uint8_t) 10

/*NETMASK*/
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/*Gateway Address*/
#define GW_ADDR0   (uint8_t) 192
#define GW_ADDR1   (uint8_t) 168
#define GW_ADDR2   (uint8_t) 0
#define GW_ADDR3   (uint8_t) 1

/* DHCP process states */
#define DHCP_OFF                   (uint8_t) 0
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5
#define DHCP_ASK_RELEASE           (uint8_t) 6

/* Maximum number of client per server */
#define MAX_CLIENT  32

/* Exported functions ------------------------------------------------------- */
#if LWIP_DHCP
    void nvt_DHCP_Process(struct netif *netif);
    void nvt_DHCP_Periodic_Handle(struct netif *netif);
    void nvt_DHCP_manual_config(void);
    uint8_t nvt_get_DHCP_lease_state(void);
    void nvt_set_DHCP_state(uint8_t state);
    uint8_t nvt_get_DHCP_state(void);
    uint8_t nvt_dhcp_started(void);
#else
    #error "LWIP_DHCP must be enabled in lwipopts.h"
#endif

#if LWIP_DNS
    void nvt_dns_init(const uint8_t *dnsaddr);
    int8_t nvt_dns_gethostbyname(const char *hostname, uint32_t *ipaddr);
#else
    #error "LWIP_DNS must be enabled in lwipopts.h"
#endif

#if LWIP_UDP
void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                          const ip_addr_t *addr, u16_t port);
#else
#error "LWIP_UDP must be enabled in lwipopts.h"
#endif
/* Exported functions NVT override------------------------------------------------- */
void nvt_eth_init(const uint8_t *mac, const uint8_t *ip, const uint8_t *gw, const uint8_t *netmask);
uint8_t nvt_eth_is_init(void);
void nvt_eth_get_macaddr(uint8_t *mac);
void nvt_eth_set_macaddr(const uint8_t *mac);
uint8_t nvt_eth_link_up(void);
void nvt_eth_scheduler(void);
void nvt_User_notification(struct netif *netif);
/*Eth Auxillary function NVT override----*/
//[2024-10-21]
uint32_t nvt_eth_get_ipaddr(void);
uint32_t nvt_eth_get_gwaddr(void);
uint32_t nvt_eth_get_netmaskaddr(void);
uint32_t nvt_eth_get_dnsaddr(void);
uint32_t nvt_eth_get_dhcpaddr(void);

struct pbuf *nvt_new_data(struct pbuf *p, const uint8_t *buffer, size_t size);
struct pbuf *nvt_free_data(struct pbuf *p);
uint16_t nvt_get_data(struct pbuf_data *data, uint8_t *buffer, size_t size);


ip_addr_t *u8_to_ip_addr(uint8_t *ipu8, ip_addr_t *ipaddr);
uint32_t ip_addr_to_u32(ip_addr_t *ipaddr);

#if LWIP_TCP
    err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
    err_t tcp_accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err);
    void tcp_connection_close(struct tcp_pcb *tpcb, struct tcp_struct *tcp);
#else
    #error "LWIP_TCP must be enabled in lwipopts.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void TMR0_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __NVT_ETH_H__ */
