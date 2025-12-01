
// #define DT_DRV_COMPAT mz_button


#include <errno.h>
#include <zephyr/logging/log.h>
#include "m_switch.h"


LOG_MODULE_REGISTER(button);

// #define MY_SWITCH DT_ALIAS(mz_switch)
// #if DT_NODE_HAS_STATUS(MY_SWITCH, okay)
//    const struct device *const dev_sw = DEVICE_DT_GET(MY_SWITCH);
//    if (!device_is_ready(dev_sw)) {
//        LOG_DBG("Device not ready\n");
//        return 0;
//    }
// #else
//     #error "Node is disabled"
// #endif

static int switch_init(const struct device *dev);
static int switch_get(const struct device *dev, int *state);


static int switch_init(const struct device *dev)
{
    int ret;
    // Cast (void*)device->config & (void*)device->data 
    const struct switch_conf *cfg = (const struct switch_conf *)dev->config;
    const struct gpio_dt_spec *btn = &cfg->btn;

    LOG_DBG("Initialize Button Driver ID: %u\n",cfg->id);

    // Check button device is ready
    if (!gpio_is_ready_dt(btn)) {
        LOG_ERR("GPIO is not ready\n");
        return -ENODEV;
    }
    // Set button as input
    ret = gpio_pin_configure_dt(btn, GPIO_INPUT);
    if (ret != 0) {
        LOG_ERR("Could not configure GPIO as input\n");
        return -ENODEV;
    }

    return 0; 
}

static int switch_get(const struct device *dev, int *state)
{
    int ret;
    // Cast (void*)device->config & (void*)device->data 
    const struct switch_conf *cfg = (const struct switch_conf *)dev->config;
    const struct gpio_dt_spec *btn = &cfg->btn;
    const struct switch_data *data = (const struct switch_data *)dev->data;
    const int status = &data->status;

    //poll button state
    ret = gpio_pin_get_dt(btn);
    data->status = ret;
    if (ret < 0) {
        LOG_ERR("ERROR(%d) failed to get pin\n");
        return ret;
    }  
    
    *state = ret;
    return 0;
}


/*********************************************************
 *  Device Tree Handling
 */

static const struct switch_api switch_api_func = {
    .get = switch_get,
}

#define SWITCH_DEFINE(inst)                                 \
    /* Create instance of config struct */                  \
    static const struct switch_conf switch_conf_##inst = {  \
        .btn = GPIO_DT_SPEC_GET(DT_PHANDLE(DT_INST(inst,mz_switch),pin),gpios),\
        .id = inst                                          \
    };                                                      \
    /* Create value of data struct */                       \
    static const struct switch_data switch_data_##inst = {  \
        .status = 0,                                        \
    };                                                      \
    /* Create device instance from DeviceTree node */       \
    DEVICE_DT_INST_DEFINE(inst,                             \
                    switch_init,                            \
                    NULL,                                   \
                    &switch_data_##inst,                    \
                    &switch_config_##inst,                  \
                    POST_KERNEL,                            \
                    CONFIG_GPIO_INIT_PRIORITY,              \
                    &switch_api_func);                  

// Devicetree build calls this to create an instance for each button device
DT_INST_FOREACH_STATUS_OKAY(SWITCH_DEFINE)

/****************************************************** */