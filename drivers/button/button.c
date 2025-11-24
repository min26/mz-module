#include <errno.h>
#include <zephyr/logging/log.h>
#include "button.h"

LOG_MODULE_REGISTER(button);

/** declarations */
static int button_init(const struct device *dev);
static int button_state_get(const struct device *dev, uint8_t *state);




static int button_init(const struct device *dev)
{
    int ret;
    // Cast device->config (decleard const void *) to my button_config*
    const struct button_config *cfg = (const struct button_config *)dev->config;
    // Get the button struct from the config
    const struct gpio_dt_spec *btn = &cfg->button;

    LOG_DBG("Initialize Driver,button (ID: %u)\n");

    // Check button device is ready
    if (!gpio_is_ready_dt(btn)) {
        LOG_ERR("GPIO is not ready\n");
        return -ENODEV;
    }

    // Set button as input
    ret gpio_pin_configure_dt(btn, GPIO_INPUT);
    if (ret != 0) {
        LOG_ERR("Could not configure GPIO as input\n");
        return -ENODEV;
    }

    return 0; 
}

static int button_state_get(const struct device *dev, uint8_t *state)
{
    int ret;

    // Cast device->config (decleard const void *) to my button_config*
    const struct button_config *cfg = (const struct button_config *)dev->config;
    //Get button struct 
    const struct gpio_dt_spec *btn = &cfg->button;

    //poll button state
    ret = gpio_pin_get_dt(btn);
    if (ret < 0) {
        LOG_ERR("ERROR(%d) failed to read button pin\n");
        return ret;
    } else {
        *state = (uint8_t)ret;
    }

    return 0;
}


/*********************************************************
 *  Device Tree Handling
 */

static const struct button_api button_api_funcs = {
    .get = button_state_get,
}

// Create device instance from DeviceTree node
DEVICE_DT_INST_DEFINE(inst,
                    button_init,
                    NULL, NULL,
                    &button_config_##inst,
                    POST_KERNEL,
                    CONFIG_GPIO_INIT_PRIORITY,
                    &button_api_funcs);

// Devicetree build calls this to create an instance for each button device
DT_INST_FOREACH_STATUS_OKAY(BUTTON_DEFINE)