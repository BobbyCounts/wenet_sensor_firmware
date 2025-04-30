#include "wenet_service.h"

BT_GATT_SERVICE_DEFINE(wenet_service,
    BT_GATT_PRIMARY_SERVICE(WENET_SERVICE_UUID),
    BT_GATT_CHARACTERISTIC(WENET_SENSOR_CHAR_UUID,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, NULL),
    BT_GATT_CUD("Wenet Packet Interface", BT_GATT_PERM_READ),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

// Define the advertising data
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
    BT_DATA_BYTES(BT_DATA_UUID128_SOME, WENET_SERVICE_ENCODE)
};

// Implement the advertising start function as a workqueue
// This is to prevent an error since an exsiting connect is not fully 
// closed until the disconnect callback is finished
static void adv_start_work(struct k_work *work)
{
    int err;
    err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
        // Retry after a delay
        k_work_schedule(k_work_delayable_from_work(work), K_SECONDS(5));
        return;
    }
    printk("Advertising started\n");
}
K_WORK_DELAYABLE_DEFINE(adv_start, adv_start_work);

// Connection callbacks
static void connected(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (err) {
        printk("Connection failed (err %d)\n", err);
        return;
    }

    printk("Connected: %s\n", addr);
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected (reason %u)\n", reason);
    k_work_schedule(&adv_start, K_MSEC(500));
}

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

void wenet_uart_init(void)
{
    // Initialize UART here if needed
    // This is a placeholder for UART initialization

}

void wenet_service_init(void)
{
    int err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }
    // Register the connection callbacks
    bt_conn_cb_register(&conn_callbacks);
    
    // Start advertising
    k_work_schedule(&adv_start, K_NO_WAIT);
}