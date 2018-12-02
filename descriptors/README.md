# Overview

This is a test to see how easy it is to manually read descriptors from devices with libusb.

I don't use the built-in libusb APIs for this, just because I wanted practice doing it myself. In other projects, like the keyboard example, I use the built-in API.

# Example output

```
initializing libusb...
listing devices...
analyzing devices...
Device: 
 - manufacturer: 
 - serial: 
 - class: 9
 - subclass: 0
   | Configuration: 
     - max power: 0
     - attributes: 224
     | Interface: 0
       - name: 
       - alternate: 0
       - class: 9
       - subclass: 0
       - protocol: 0
       | Endpoint: 129
         - max packet size: 2
         - interval: 12
         - attributes: 3
Device: EHCI Host Controller
 - manufacturer: Linux 4.15.0-39-generic ehci_hcd
 - serial: 0000:00:1d.0
 - class: 9
 - subclass: 0
   | Configuration: 
     - max power: 0
     - attributes: 224
     | Interface: 0
       - name: 
       - alternate: 0
       - class: 9
       - subclass: 0
       - protocol: 0
       | Endpoint: 129
         - max packet size: 4
         - interval: 12
         - attributes: 3
Device: xHCI Host Controller
 - manufacturer: Linux 4.15.0-39-generic xhci-hcd
 - serial: 0000:00:14.0
 - class: 9
 - subclass: 0
   | Configuration: 
     - max power: 0
     - attributes: 224
     | Interface: 0
       - name: 
       - alternate: 0
       - class: 9
       - subclass: 0
       - protocol: 0
       | Endpoint: 129
         - max packet size: 4
         - interval: 12
         - attributes: 3
       | (Unknown descriptor type: 48)
Device: CSR8510 A10
 - manufacturer: 
 - serial: 
 - class: 224
 - subclass: 1
   | Configuration: 
     - max power: 50
     - attributes: 224
     | Interface: 0
       - name: 
       - alternate: 0
       - class: 224
       - subclass: 1
       - protocol: 1
       | Endpoint: 129
         - max packet size: 16
         - interval: 1
         - attributes: 3
       | Endpoint: 2
         - max packet size: 64
         - interval: 1
         - attributes: 2
       | Endpoint: 130
         - max packet size: 64
         - interval: 1
         - attributes: 2
     | Interface: 1
       - name: 
       - alternate: 0
       - class: 224
       - subclass: 1
       - protocol: 1
       | Endpoint: 3
         - max packet size: 0
         - interval: 1
         - attributes: 1
       | Endpoint: 131
         - max packet size: 0
         - interval: 1
         - attributes: 1
     | Interface: 1
       - name: 
       - alternate: 1
       - class: 224
       - subclass: 1
       - protocol: 1
       | Endpoint: 3
         - max packet size: 9
         - interval: 1
         - attributes: 1
       | Endpoint: 131
         - max packet size: 9
         - interval: 1
         - attributes: 1
     | Interface: 1
       - name: 
       - alternate: 2
       - class: 224
       - subclass: 1
       - protocol: 1
       | Endpoint: 3
         - max packet size: 17
         - interval: 1
         - attributes: 1
       | Endpoint: 131
         - max packet size: 17
         - interval: 1
         - attributes: 1
     | Interface: 1
       - name: 
       - alternate: 3
       - class: 224
       - subclass: 1
       - protocol: 1
       | Endpoint: 3
         - max packet size: 25
         - interval: 1
         - attributes: 1
       | Endpoint: 131
         - max packet size: 25
         - interval: 1
         - attributes: 1
     | Interface: 1
       - name: 
       - alternate: 4
       - class: 224
       - subclass: 1
       - protocol: 1
       | Endpoint: 3
         - max packet size: 33
         - interval: 1
         - attributes: 1
       | Endpoint: 131
         - max packet size: 33
         - interval: 1
         - attributes: 1
     | Interface: 1
       - name: 
       - alternate: 5
       - class: 224
       - subclass: 1
       - protocol: 1
       | Endpoint: 3
         - max packet size: 49
         - interval: 1
         - attributes: 1
       | Endpoint: 131
         - max packet size: 49
         - interval: 1
         - attributes: 1
Device: Wireless Device
 - manufacturer: Chicony
 - serial: 
 - class: 0
 - subclass: 0
   | Configuration: 
     - max power: 50
     - attributes: 160
     | Interface: 0
       - name: 
       - alternate: 0
       - class: 3
       - subclass: 1
       - protocol: 1
       | (Unknown descriptor type: 33)
       | Endpoint: 129
         - max packet size: 8
         - interval: 10
         - attributes: 3
     | Interface: 1
       - name: 
       - alternate: 0
       - class: 3
       - subclass: 0
       - protocol: 0
       | (Unknown descriptor type: 33)
       | Endpoint: 130
         - max packet size: 8
         - interval: 10
         - attributes: 3
     | Interface: 2
       - name: 
       - alternate: 0
       - class: 3
       - subclass: 1
       - protocol: 2
       | (Unknown descriptor type: 33)
       | Endpoint: 131
         - max packet size: 8
         - interval: 4
         - attributes: 3
Device: USB2.0 Hub
 - manufacturer: 
 - serial: 
 - class: 9
 - subclass: 0
   | Configuration: 
     - max power: 50
     - attributes: 224
     | Interface: 0
       - name: 
       - alternate: 0
       - class: 9
       - subclass: 0
       - protocol: 0
       | Endpoint: 129
         - max packet size: 1
         - interval: 12
         - attributes: 3
Device: DasKeyboard
 - manufacturer: 
 - serial: 
 - class: 0
 - subclass: 0
   | Configuration: 
     - max power: 50
     - attributes: 160
     | Interface: 0
       - name: 
       - alternate: 0
       - class: 3
       - subclass: 1
       - protocol: 1
       | (Unknown descriptor type: 33)
       | Endpoint: 129
         - max packet size: 8
         - interval: 10
         - attributes: 3
     | Interface: 1
       - name: 
       - alternate: 0
       - class: 3
       - subclass: 0
       - protocol: 0
       | (Unknown descriptor type: 33)
       | Endpoint: 130
         - max packet size: 1
         - interval: 10
         - attributes: 3
Device: xHCI Host Controller
 - manufacturer: Linux 4.15.0-39-generic xhci-hcd
 - serial: 0000:00:14.0
 - class: 9
 - subclass: 0
   | Configuration: 
     - max power: 0
     - attributes: 224
     | Interface: 0
       - name: 
       - alternate: 0
       - class: 9
       - subclass: 0
       - protocol: 0
       | Endpoint: 129
         - max packet size: 4
         - interval: 12
         - attributes: 3
```