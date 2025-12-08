/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>

#include <zephyr/sys/printk.h>
#include <zephyr/sys/ring_buffer.h>

#include <zephyr/usb/usb_device.h>

#include "m_gpio.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);
/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000



#define DEV_USB_CDC		DT_CHOSEN(zephyr_console)
BUILD_ASSERT(DT_NODE_HAS_COMPAT(DEV_USB_CDC, zephyr_cdc_acm),
			"Console device is not ACM CDC");
static const struct device *const dev_usb = DEVICE_DT_GET(DEV_USB_CDC);
// /*
//  * A build error on this line means your board is unsupported.
//  * See the sample documentation for information on how to fix this.
//  */

// Alias allows only characters
#define DEV_LED0	DT_ALIAS(myled0)
static const struct device *dev_led0 = DEVICE_DT_GET(DEV_LED0);
#define DEV_SW0		DT_ALIAS(mysw0)
static const struct device *dev_sw0 = DEVICE_DT_GET(DEV_SW0);





int main(void)
{
	int ret;
	int state_led = 0;
	int state_sw = 0;

	if (!(device_is_ready(dev_led0) && device_is_ready(dev_sw0))) {
		printk("ERROR: devices are not ready\n");
		return 0;
	}

	const struct m_gpio_api *sw0_api = (const struct m_gpio_api*)dev_sw0->api;
	//const struct switch_data *sw0_data = (const struct switch_data*)dev_sw0->data;
	const struct m_gpio_api *led0_api = (const struct m_gpio_api*)dev_led0->api;
	//const struct led_data *led0_data = (const struct led_data)dev_led0->data;

	
	const stuct

	while(1) {
		printk("Hello USB CDC\n");
		
		ret = sw0_api->get(dev_sw0, &state_sw);
		if (ret < 0) {
			printk("ERROR(%d) failed to get sw0\n", ret);
		} else {
			printk("Switch-get: %d\n", state_sw);
		}
		
		
		ret = led0_api->toggle(dev_sw0);
		if (ret < 0) {
			printk("ERROR(%d) failed to toggle led0\n", ret);
		} else {
			printk("led-toggle\n");
		}

		// Sleep
		k_msleep(SLEEP_TIME_MS);
	}
	
	return 0;
}

