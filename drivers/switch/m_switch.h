
#ifndef _DRIVER_SWITCH_H_
#define _DRIVER_SWITCH_H_

#include <zephyr/drivers/gpio.h>


struct switch_api {
    int (*get)(const struct device *dev, int *state);
};

struct switch_conf {
    struct gpio_dt_spec btn;
    uint32_t id;
};

struct switch_data {
    int status;  
};

#endif /* _DRIVER_SWITCH_H_*/