
#ifndef _DRIVER_M_GPIO_H_
#define _DRIVER_M_GPIO_H_

#include <stdint.h>
#include <zephyr/drivers/gpio.h>


struct m_gpio_api {
    int (*config)(const struct device *dev, int state);
    int (*get)(const struct device *dev, int *state);
    int (*set)(const struct device *dev, int state);
    int (*toggle)(const struct device *dev);
};

struct m_gpio_conf {
    struct gpio_dt_spec dt;
    uint32_t id;
};

struct m_gpio_data {
    int state;  
};

#endif /* _DRIVER_M_GPIO_H_*/