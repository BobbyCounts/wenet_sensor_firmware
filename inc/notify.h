#include <stdint.h>

#define WENET_PACKET_HEADER_SIZE 3
// Maximum payload size in bytes
#define WENET_MAX_PAYLOAD_SIZE 16
// Number of entries in thread message queue
#define NOTIFY_MESGQ_ENTRIES 32
// Maximum number of outbound notifications
#define NOTIFY_MAX_QUEUED 32

struct wenet_packet {
    uint8_t packet_id;
    uint16_t sequence_number;
    uint8_t payload_size;
    uint8_t payload[WENET_MAX_PAYLOAD_SIZE];
};

void wenet_send_notify(struct wenet_packet *packet);