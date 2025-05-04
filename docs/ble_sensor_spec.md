# Wenet BLE Sensor Hub Specification

## Design Goals

- Ultra-low power
- Smallest footprint
- Two transmission modes:
  - Wenet sensor profile: Custom GATT profile for sensors
  - Wenet UART Direct Packet: Transmit a message packet to be sent over UDP through BLE UART
- Breakout for SPI, 2 UARTs, I2C
- MAYBE: SD Card
- MAYBE: QSPI NOR Flash

## Wenet Sensor Service
- UUID: fb63feb8-31ad-451d-a587-9fc20f9c8add

### BLE Sensor Hub Advertisement

- Must advertise the 128-bit Wenet Sensor Service UUID

### Wenet Sensor Characteristic

- UUID: 3d235f0e-61f8-4455-89c6-2f7d73c33178
- Interface for reading sensor data through notification

### Wenet UART Characteristic

- UUID: 64a96749-ab48-434d-8d31-77c07e6e352f
- BLE UART interface for Wenet text messages
