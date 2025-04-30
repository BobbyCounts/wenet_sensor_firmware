#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/bluetooth/gatt.h>
#include "param_stack.h"

#define MAX_ITEMS 50

static struct bt_gatt_notify_params notify_params[MAX_ITEMS];

K_STACK_DEFINE(notify_stack, MAX_ITEMS);

static int param_stack_init(void)
{
    // Push to stack
    int i;
    for(i = 0; i < MAX_ITEMS; i++){
        k_stack_push(&notify_stack, (stack_data_t)&notify_params[i]);
    }
    return 0;
}

int param_stack_get_notify(struct bt_gatt_notify_params **data)
{
    return k_stack_pop(&notify_stack, (stack_data_t*)data, K_NO_WAIT);
}

int param_stack_free_notify(struct bt_gatt_notify_params *data)
{
    return k_stack_push(&notify_stack, (stack_data_t)data);
}

SYS_INIT(param_stack_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);