
// #define DT_DRV_COMPAT mz_led

#include <errno.h>
#include <zephyr/logging/log.h>
#include "m_led.h"


LOG_MODULE_REGISTER(led);

/** declarations */
static int led_init(const struct device *dev);
static int led_set(const struct device *dev, int onoff);
static int led_toggle(const struct device *dev);


static int led_init(const struct device *dev)
{
    int ret;
    const struct led_conf *cfg = (const struct led_conf *)dev->confg;
    const struct gpio_dt_spec *led = &cfg->led;

    LOG_DBG("Initialize LED driver ID: %u\n",cfg->id);

    if(!gpio_is_ready_dt(led)) {
        LOG_ERR("GPIO is not ready\n");
        return -ENODEV;        
    }
    ret = gpio_pin_configure_dt(led, GPIO_OUTPUT_INACTIVE);
    if (ret != 0) {
        LOG_ERR("Could not configure GPIO as output\n");
        return -ENODEV;
    }
    return 0;
}

static int led_set(const struct device *dev, int onoff)
{
    int ret;
    const struct led_conf *cfg = (const struct led_conf *)dev->confg;
    const struct gpio_dt_spec *led = &cfg->led;
    const struct led_data *data = (const struct led_data *)dev->data;
    //const int status = &data->status;

    ret = gpio_pin_set_dt(led, onoff);    
    if (ret < 0) {
        LOG_ERR("ERROR(%d) failed to set led %d\n", ret, onoff);
        data->status = ret;
        return ret;
    } else {
        data->status = onoff;
    }

    return 0;
}

static int led_toggle(const struct device *dev)
{
    int ret;
    const struct led_conf *cfg = (const struct led_conf *)dev->confg;
    const struct gpio_dt_spec *led = &cfg->led;
    const struct led_data *data = (const struct led_data *)dev->data;
    //const int status = &data->status;

    ret = gpio_pin_toggle_dt(led);
    if (ret < 0) {
        LOG_ERR("ERROR(%d) failed to toggle %d\n", ret);
        data->status = ret;
        return ret;
    } 
    
    ret = gpio_pin_get_dt(btn);
    data->status = ret;
    if (ret < 0) {
        LOG_ERR("ERROR(%d) failed to get pin\n");
        return ret;
    }  
    
    return 0;
}

/******************************************************
 * DEvice Tree Handling
 */

static const struct led_api led_api_func = {
    .set = led_set,
    .toggle = led_toggle,
};

#define LED_DEFINE(inst)                                    \
    /* Create instance of config struct */                  \
    static const struct led_conf led_conf_##inst = {        \
        .led = GPIO_DT_SPEC_GET(DT_PHANDLE(DT_INST(inst,mz_led),pin),gpios),\
        .id = inst                                          \
    };                                                      \
    /* Create value of data struct */                       \
    static const struct led_data led_data_##inst = {  \
        .status = 0,                                        \
    };                                                      \
    /* Create device instance from DeviceTree node */       \
    DEVICE_DT_INST_DEFINE(inst,                             \
                    led_init,                               \
                    NULL,                                   \
                    &led_data_##inst,                       \
                    &led_config_##inst,                     \
                    POST_KERNEL,                            \
                    CONFIG_GPIO_INIT_PRIORITY,              \
                    &led_api_func);                  

// Devicetree build calls this to create an instance for each button device
DT_INST_FOREACH_STATUS_OKAY(LED_DEFINE)

/****************************************************** */
                