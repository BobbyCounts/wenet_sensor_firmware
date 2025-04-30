#include <zephyr/kernel.h>
#include <zephyr/bluetooth/gatt.h>
#include "notify.h"

extern const struct bt_gatt_service_static wenet_service;

// Message queue for Wenet notification packets
char msgq_buffer[NOTIFY_MESGQ_ENTRIES * sizeof(struct wenet_packet)];
struct k_msgq notify_mesgq;

void wenet_send_notify(struct wenet_packet *packet)
{
    // Send the packet to the message queue
    k_msgq_put(&notify_mesgq, packet, K_NO_WAIT);
}

// Thread loop for Wenet notification packets
void wenet_notify_thread(void *p1, void *p2, void *p3)
{
    k_msgq_init(&notify_mesgq, msgq_buffer, sizeof(struct wenet_packet), NOTIFY_MESGQ_ENTRIES);
    while (1) {
        struct wenet_packet packet;
        k_msgq_get(&notify_mesgq, &packet, K_FOREVER);

        // Build packet
        int ble_packet_size = WENET_PACKET_HEADER_SIZE + packet.payload_size;
        uint8_t ble_packet[WENET_PACKET_HEADER_SIZE + WENET_MAX_PAYLOAD_SIZE];
        ble_packet[0] = packet.packet_id;
        memcpy(&ble_packet[1], &packet.sequence_number, sizeof(packet.sequence_number));
        memcpy(&ble_packet[3], packet.payload, packet.payload_size);
        bt_gatt_notify(NULL, &wenet_service.attrs[1], &ble_packet, ble_packet_size);
    }
}