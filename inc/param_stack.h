#include <zephyr/bluetooth/gatt.h>
int param_stack_get_notify(struct bt_gatt_notify_params **data);
int param_stack_free_notify(struct bt_gatt_notify_params *data);
