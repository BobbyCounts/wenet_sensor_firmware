# Requirements

## Data Packet Specifications

### Wenet Telemetry Total Frame

| Position | Data Type                                              | Description        |
| -------- | ------------------------------------------------------ | ------------------ |
| 0        | uint8_t                                                | Number for packets |
| 1        | Wenet Telemetry Single Packet (0-11) (253 bytes total) |                    |

### Wenet Telemetry Single Packet 

| Position | Data Type           | Description       |
| -------- | ------------------- | ----------------- |
| 0        | uint8_t             | Packet ID         |
| 1        | uint16_t            | Sequence Number   |
| 3        | uint32_t            | Time of Day in ms |
| 7        | uint8_t payload[16] | Payload           |

Total Packet Size: 23 bytes

### BLE Notify Packet

We will rely on the balloon RPi to provide the timestamp for the data. This packet can be a maximum of 20 bytes.

| Position | Data Type           | Description     |
| -------- | ------------------- | --------------- |
| 0        | uint8_t             | Packet ID       |
| 1        | uint16_t            | Sequence Number |
| 3        | uint8_t payload[16] | Payload         |

## Packet Processing Flow

1. BLE Sensor Hub will send a notify packet to the RPi. The notify packet will always contain a packet ID and sequence number. The payload length will vary depending on the individual payload specification.
2. RPi receives the BLE notify packet. The RPi will convert the notify packet to the Wenet Telemetry Single Packet format.
3. The processed packet is added to the transmit queue. Once 11 packets are queue or the packet timeout is reached, the packets are formatted into a Wenet Telemetry frame and send over UDP for transmission.
4. After Wenet transmission, a ground station receives the telemetry frame over UDP. The processing script will read the first byte of the frame to determine how many individual packets to process in the frame. Each packet is then decoded using the JSON packet ID for payload specific information.