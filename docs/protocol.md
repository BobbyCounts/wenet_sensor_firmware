# Requirements

## Data Packet Specifications

### Wenet Telemetry Total Frame

| Position | Data Type                                              | Description        |
| -------- | ------------------------------------------------------ | ------------------ |
| 0        | uint8_t                                                | Number for packets |
| 1        | Wenet Telemetry Single Packet (0-11) (253 bytes total) |                    |

### Wenet Telemetry Single Packet 

| Position | Data Type           | Description                            |
| -------- | ------------------- | -------------------------------------- |
| 0        | uint8_t             | Sensor ID from BLE Notify Packet       |
| 1        | uint16_t            | Sequence Number from BLE Notify Packet |
| 3        | uint32_t            | Timestamp - Time of day in ms          |
| 7        | uint8_t payload[16] | Payload from BLE Notify Packet         |

Total Packet Size: 23 bytes

### BLE Notify Packet

| Position | Data Type           | Description                 |
| -------- | ------------------- | --------------------------- |
| 0        | uint8_t             | Sensor ID                   |
| 1        | uint16_t            | Sequence Number             |
| 3        | uint8_t payload[16] | Payload - User defined data |

## Packet Processing Flow

1. BLE Sensor Hub will send a notify packet to the RPi. The notify packet will always contain a sensor ID and sequence number. The payload length will vary depending on the individual payload specification. The payload will be padded out to 16 bytes after processing.
2. RPi receives the BLE notify packet. The RPi will convert the notify packet to the Wenet Telemetry Single Packet format.
3. The processed packet is added to the transmit queue. Once 11 packets are queued or the packet a JSON object, and transmitted to the Wenet listener over UDP.
4. After Wenet transmission, a ground station receives the telemetry frame over UDP. The processing script will read the first byte of the frame to determine how many individual packets to process in the frame. Each packet is then decoded using the JSON sensor ID for payload specific information.