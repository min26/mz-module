
#ifndef _DRIVER_LED_H_
#define _DRIVER_LED_H_

#include <zephyr/drivers/gpio.h>

struct led_api {
    int (*set)(const struct device *dev, int onoff);
    int (*toggle)(const struct device *dev);
};

struct led_conf {
    struct gpio_dt_spec led;
    uint32_t id;
};

struct led_data {
    int status;  
};

#endif /* _DRIVER_LED_H_ */