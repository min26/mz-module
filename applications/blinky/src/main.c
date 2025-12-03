/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include "m_gpio.h"


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

// /*
//  * A build error on this line means your board is unsupported.
//  * See the sample documentation for information on how to fix this.
//  */
#define DEV_LED0	DT_ALIAS(my_led0)
static const struct device *dev_led0 = DEVICE_DT_GET(DEV_LED0);
#define DEV_SW0		DT_ALIAS(my_sw0)
static const struct device *dev_sw0 = DEVICE_DT_GET(DEV_SW0);

int main(void)
{
	int ret;
	int led_onoff = 0;
	int sw_state = 0;

	if (!(device_is_ready(dev_led0) && device_is_ready(dev_sw0))) {
		printk("ERROR: devices are not ready\n");
		return 0;
	}

	const struct m_gpio_api *sw0_api = (const struct m_gpio_api*)dev_sw0->api;
	//const struct switch_data *sw0_data = (const struct switch_data*)dev_sw0->data;
	const struct m_gpio_api *led0_api = (const struct m_gpio_api*)dev_led0->api;
	//const struct led_data *led0_data = (const struct led_data)dev_led0->data;

	while(1) {
		ret = sw0_api->get(dev_led0, &sw_state);
		if (ret < 0) {
			printk("ERROR(%d) failed to ready sw0\n", ret);
			continue;
		}
		printk("switch get: %d\n", sw_state);
		
		ret = led0_api->toggle(dev_sw0);
		if (ret < 0) {
			printk("ERROR(%d) failed to toggle led0\n", ret);
			continue;
		}
		printk("led toggle: %d\n", led0_data->status);

		// Sleep
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

