
// Matching compatible binding-yaml, devicetree-dts, header
// compatible="m,gpio"
// #define DT_DRV_COMPAT m_gpio

#include "m_gpio.h"

#include <errno.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(m_gpio);

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


/**
 * Call by zephyr kernel
 */
static int m_gpio_init(const struct device *dev)
{
    int ret, io;
    const struct m_gpio_conf *cfg = (const struct m_gpio_conf *)dev->config;
    const struct gpio_dt_spec *gpio_dt = &cfg->dt;

    LOG_DBG("Initialize M-GPIO Driver ID: %u\n",cfg->id);

    // Check button device is ready
    if (!gpio_is_ready_dt(gpio_dt)) {
        LOG_ERR("GPIO is not ready\n");
        return -ENODEV;
    }
    // Set pin configuration
    io = cfg->io;
    if (io == PIN_OUTPUT) {
        ret = gpio_pin_configure_dt(gpio_dt, GPIO_OUTPUT_INACTIVE);
    } else {
        ret = gpio_pin_configure_dt(gpio_dt, GPIO_INPUT);
    }    
    if (ret != 0) {
        LOG_ERR("ERROR(%d) Failed to config\n", ret);
        return -ENODEV;
    }

    return 0; 
}

static int m_gpio_get(const struct device *dev, int *state)
{
    int ret;
    // Cast (void*)device->config & (void*)device->data 
    const struct m_gpio_conf *cfg = (const struct m_gpio_conf *)dev->config;
    const struct gpio_dt_spec *gpio_dt = &cfg->dt;
    // const struct m_gpio_data *data = (const struct m_gpio_data *)dev->data;
    //
    ret = gpio_pin_get_dt(gpio_dt);
    // data->state = ret;
    if (ret < 0) {
        // LOG_ERR("ERROR(%d) failed to get pin\n", ret);
        return ret;
    } 
    *state = ret;    
    return 0;
}

static int m_gpio_set(const struct device *dev, int state)
{
    int ret;
    const struct m_gpio_conf *cfg = (const struct m_gpio_conf *)dev->config;
    const struct gpio_dt_spec *gpio_dt = &cfg->dt;
    // const struct m_gpio_data *data = (const struct m_gpio_data *)dev->data;
    // 
    ret = gpio_pin_set_dt(gpio_dt, state);
    // data->state = ret;
    if (ret < 0) {
        // LOG_ERR("ERROR(%d) failed to set pin\n", ret);
        return ret;
    }

    return 0;
}

static int m_gpio_toggle(const struct device *dev)
{
    int ret;
    const struct m_gpio_conf *cfg = (const struct m_gpio_conf *)dev->config;
    const struct gpio_dt_spec *gpio_dt = &cfg->dt;
    // const struct m_gpio_data *data = (const struct m_gpio_data *)dev->data;
    // 
    ret = gpio_pin_toggle_dt(gpio_dt);
    // data->state = ret;
    if (ret < 0) {
        LOG_ERR("ERROR(%d) failed to toggle pin\n", ret);
        return ret;
    }

    return 0;
}
/*********************************************************
 * DeviceDriver / DeviceTree Handling
 * struct device {.name, .config, .api, .data}
 */

static const struct m_gpio_api m_gpio_api_func = {
    .get = m_gpio_get,
    .set = m_gpio_set,
    .toggle = m_gpio_toggle,
};

#define M_GPIO_DEFINE(inst)                                 \
    /* Create instance of config struct */                  \
    static const struct m_gpio_conf m_gpio_conf_##inst = {  \
        .dt = GPIO_DT_SPEC_GET(DT_PHANDLE(DT_INST(inst,m_gpio),pin),gpios),\
        .io = DT_INSDT_PROP(DT_INST(inst,m_gpio), io),      \
        .id = inst,                                         \
    };                                                      \
    /* Create value of data struct */                       \
    static const struct m_gpio_data m_gpio_data_##inst = {  \
        .state = 0,                                         \
    };                                                      \
    /* Create device instance from DeviceTree node */       \
    DEVICE_DT_INST_DEFINE(inst,                             \
                    m_gpio_init,                            \
                    NULL,                                   \
                    &m_gpio_data_##inst,                    \
                    &m_gpio_conf_##inst,                    \
                    POST_KERNEL,                            \
                    CONFIG_GPIO_INIT_PRIORITY,              \
                    &m_gpio_api_func);                  

// Devicetree-build calls this to create an instance for each device
DT_INST_FOREACH_STATUS_OKAY(M_GPIO_DEFINE)

/****************************************************** */

mgpio_timer_init()
