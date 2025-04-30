#include <zephyr/kernel.h>
#include "wenet_service.h"

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
    return 0;
}