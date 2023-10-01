<!--<img src="https://github.com/miloserdev/msx/assets/37951044/8c669d7f-e6a0-4e9b-beab-816d857a1b46"/>  -->
<img src="https://github.com/miloserdev/msx/assets/37951044/ae03f2e9-750f-48a7-a5f1-f123ce8c2b7a"/>  

# MSX ESP8266  
## ESP WiFi & ESP-NOW Networking  
### A pure FreeRTOS-SDK version  
  
### Features  
 * WiFi  
 * ESP-NOW  
 * OTA Updates  
 * JSON Command Parser  

In this case we can setup peer-to-peer connecion throuth 2.4GHz and send packets with size of 250 bytes  
ESP-NOW protocol can be used without wifi connection  
ESP-NOW Features  
 * Multicast (non-encrypted / encrypted data)  
 * Broadcast (non-encrypted data)  
 * Peer add / remove | up to 20 peers  

# <br>
<br>

## ESP-NOW example  
### Basics  
// Read pin state on board with MAC AB:CD:EF:A1:B2:C3;
```json
[{ "to": "AB:CD:EF:A1:B2:C3" "digitalRead": { "pin": 32 } }]
```

<br>

// Turn on 32 pin on board with MAC AB:CD:EF:A1:B2:C3;
```json
[{ "to": "AB:CD:EF:A1:B2:C3" "digitalWrite": { "pin": 32, "value": 0 } }]
```

<br>

### Support double commands  
> **Note:** only if JSON data length less than 200 bytes
  
```json
[{ "to": "AB:CD:EF:A1:B2:C3" "digitalWrite": { "pin": 33, "value": 0 } }, { "to": "AB:CD:EF:A1:B2:C3" "digitalWrite": { "pin": 32, "value": 0 } }]
```  
```json
[{ "to": "AB:CD:EF:A1:B2:C3" "digitalWrite": { "pin": 33, "value": 1 } }, { "to": "AB:CD:EF:A1:B2:C3" "digitalWrite": { "pin": 32, "value": 1 } }]
```
   
# <br>
<br>

### OTA  
> **Note:** OTA Updates possible only if your partitions setup correctly   
> Make sure that your **[partitions.csv](https://github.com/miloserdev/msx/blob/master/partitions.csv)** contains OTA.  
`make menuconfig` > `Partition Tables`  


| Name | Type | SubType | Offset      | Size         | Flags |
|------|------|-----------|-------------|-------------|------|
| *    |      |           |             |         |          |
| ota_0| 0    | ota_0     | 0x10000     | 0xF0000 |          |
|      |      |           |             |         |          |

Update example  
```console
curl 192.168.1.101:8066/update --no-buffer --data-binary @./build/msx.bin --output -
```

# <br>
<br>

### Errata  
    1. Print cJSON number variables causes Guru Meditation :D;  
        FIX: Need to disable "nano" formatting in menuconfig;  
        make menuconfig -> Component config -> Newlib -> "nano" formatting  

    2. UART data sending only in non separated format;  
        i will make a buffer concatenator (maybe later XD);  
        echo -en '\x12\x02[{"to":"34:94:54:62:9f:74","digitalWrite":{"pin":2,"value":2}}]' > /dev/ttyUSB1  




