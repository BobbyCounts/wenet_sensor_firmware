import sys

from micropython import const

import asyncio
import aioble
import bluetooth

import random
import struct

_WENET_SERVICE_UUID = bluetooth.UUID('fb63feb8-31ad-451d-a587-9fc20f9c8add')
_WENET_CHAR_UUID = bluetooth.UUID('3d235f0e-61f8-4455-89c6-2f7d73c33178')

# How frequently to send advertising beacons.
_ADV_INTERVAL_MS = 250_000

# Register GATT server.
temp_service = aioble.Service(_WENET_SERVICE_UUID)
temp_characteristic = aioble.Characteristic(
    temp_service, _WENET_CHAR_UUID, read=True, notify=True
)
aioble.register_services(temp_service)

def create_wenet_packet(sensor_id, sequence_num, data):
    header = struct.pack('<BH', sensor_id, sequence_num)
    return header + data

# This would be periodically polling a hardware sensor.
async def sensor_task():
    sequence_num = 0
    while True:
        data = b'\x11\x22\x33\x44\xaf\x55\xfa\xcc'
        temp_characteristic.write(create_wenet_packet(0x34, sequence_num, data), send_update=True)
        await asyncio.sleep_ms(500)
        sequence_num = sequence_num + 1
        
# Serially wait for connections. Don't advertise while a central is
# connected.
async def peripheral_task():
    while True:
        async with await aioble.advertise(
            _ADV_INTERVAL_MS,
            name="WenetPico",
            services=[_WENET_SERVICE_UUID],
        ) as connection:
            print("Connection from", connection.device)
            await connection.disconnected(timeout_ms=None)

# Run both tasks.
async def main():
    t1 = asyncio.create_task(sensor_task())
    t2 = asyncio.create_task(peripheral_task())
    await asyncio.gather(t1, t2)

asyncio.run(main())