#include <zephyr/kernel.h>
#include "wenet_service.h"
#include "notify.h"

#define WENET_NOTIFY_STACK_SIZE 1024

extern void wenet_notify_thread(void *p1, void *p2, void *p3);
K_THREAD_STACK_DEFINE(wenet_notify_stack, WENET_NOTIFY_STACK_SIZE);
struct k_thread wenet_notify;

int main(void)
{
    wenet_service_init();
    // Wenet Nofication Thread
    k_thread_create(&wenet_notify, wenet_notify_stack, WENET_NOTIFY_STACK_SIZE,
                    wenet_notify_thread, NULL, NULL, NULL,
                    7, 0, K_NO_WAIT);
    int i = 0;
    while(1)
    {
        // Main loop
        k_sleep(K_MSEC(100));
        struct wenet_packet packet = {
            .packet_id = 0x01,
            .sequence_number = i,
            .payload_size = 4,
            .payload = {0xDE, 0xAD, 0xBE, 0xEF}
        };
        wenet_send_notify(&packet);
        i++;
    }
}