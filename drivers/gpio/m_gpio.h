
#ifndef _DRIVER_M_GPIO_H_
#define _DRIVER_M_GPIO_H_

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define M_PIN_INPUT         1
#define M_PIN_OUTPUT        0


struct m_gpio_api {
    int (*get)(const struct device *dev, int *state);
    int (*set)(const struct device *dev, int state);
    int (*toggle)(const struct device *dev);
};

struct m_gpio_conf {
    struct gpio_dt_spec dt;
    int io;
    int id;
};

struct m_gpio_data {
    int state;  
};

/*************************************** */


#endif /* _DRIVER_M_GPIO_H_*/