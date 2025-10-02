import usb.core
import usb.util
import usb.backend.libusb1

# Define the Vendor ID (VID) and Product ID (PID) of your USB device
# You can find these using tools like 'lsusb' on Linux or Device Manager on Windows
VENDOR_ID = 0x0416  # Replace with your device's Vendor ID
PRODUCT_ID = 0x3F00  # Replace with your device's Product ID

# Find the device
#dev = usb.core.find(find_all=1)
dev = usb.core.find(idVendor=0x0416, idProduct=0x3F00)


if dev is None:
    raise ValueError('Device not found')
else:
    print(f"Device found: {dev}")

# If the device is already in use by a kernel driver, detach it
#if dev.is_kernel_driver_active(0): # Check interface 0
#    try:
#        dev.detach_kernel_driver(0)
#        print("Kernel driver detached.")
#    except usb.core.USBError as e:
#        print(f"Could not detach kernel driver: {e}")

# Set the active configuration (usually the first one)
#try:
#    dev.set_configuration()
#    print("Configuration set.")
#except usb.core.USBError as e:
#    print(f"Could not set configuration: {e}")

# Perform a control transfer (example: a simple write operation)
# This sends a command to the device. The parameters depend on your device's protocol.
# bmRequestType: The type of request (e.g., 0x40 for host-to-device, vendor-specific)
# bRequest: The specific request code
# wValue, wIndex: Additional parameters for the request
# data_or_wLength: Data to send or expected length of data to receive
#try:
    # Example: Writing a single byte 'A' to a vendor-specific control endpoint
    # (replace with your actual device's control transfer details)
#    bytes_written = dev.ctrl_transfer(
#        bmRequestType=0x40,  # Host-to-device, Vendor, Interface
#        bRequest=0x01,       # Example request code
 #       wValue=0x00,         # Example wValue
 #       wIndex=0x00,         # Example wIndex
 #       data_or_wLength=b'A' # Data to send
 #   )
 #   print(f"Control transfer completed. Bytes written: {bytes_written}")

    # Example: Reading 4 bytes from a vendor-specific control endpoint
    # (replace with your actual device's control transfer details)
  #  read_data = dev.ctrl_transfer(
   #     bmRequestType=0xC0,  # Device-to-host, Vendor, Interface
    #    bRequest=0x02,       # Example request code
     #   wValue=0x00,
     #   wIndex=0x00,
     #   data_or_wLength=4    # Expected length of data to receive
    #)
    #print(f"Data read from device: {read_data}")

#except usb.core.USBError as e:
 #   print(f"Control transfer failed: {e}")

# Release the device (important when you're done)
#usb.util.release_interface(dev, 0) # Release interface 0
#dev.attach_kernel_driver(0) # Reattach kernel driver if detached