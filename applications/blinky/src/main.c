/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

#include "m_gpio.h"
#include "m_usbd.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);


// Multi-thread define -------------------------------
#define SLEEP_TIME_MS   1000
#define MY_THREAD_STACK_SIZE	256
// Define Stack areas for threads
K_THREAD_STACK_DEFINE(usbd_stack, MY_THREAD_STACK_SIZE);
static struct k_thread usbd_thread;

K_THREAD_STACK_DEFINE(sw0_stack, MY_THREAD_STACK_SIZE);
static struct k_thread sw0_thread;

// USB Device -----------------------------------------------
#define DEV_USB_CDC		DT_CHOSEN(zephyr_console)
BUILD_ASSERT(DT_NODE_HAS_COMPAT(DEV_USB_CDC, zephyr_cdc_acm),
			"Console device is not ACM CDC");
static const struct device *const dev_usb = DEVICE_DT_GET(DEV_USB_CDC);

// LED, Switch Device ---------------------------------------
#define DEV_LED0	DT_ALIAS(myled0)
static const struct device *dev_led0 = DEVICE_DT_GET(DEV_LED0);
#define DEV_SW0		DT_ALIAS(mysw0)
static const struct device *dev_sw0 = DEVICE_DT_GET(DEV_SW0);



void m_sw0_task(void arg1, void arg2, void arg3)
{
	int ret;
	int state_sw = 0;
	
	if (!(device_is_ready(dev_sw0))) {
		printk("ERROR:SW0 device is not ready\n");
		return;
	}
	const struct m_gpio_api *sw0_api = (const struct m_gpio_api*)dev_sw0->api;
	//const struct switch_data *sw0_data = (const struct switch_data*)dev_sw0->data;
	
	while(1) {
		ret = sw0_api->get(dev_sw0, &state_sw);
		if (ret < 0) {
			printk("ERROR:SW0(%d) failed to get\n", ret);
		} else {
			printk("Switch-get: %d\n", state_sw);
		}
		k_msleep(SLEEP_TIME_MS);
	}
}



int main(void)
{
	int ret;
	k_tid_t usbd_tid, sw0_tid;

	int state_led = 0;
	if (!(device_is_ready(dev_led0))) {
		printk("ERROR:LED0 devices is not ready\n");
		return 0;
	}	
	const struct m_gpio_api *led0_api = (const struct m_gpio_api*)dev_led0->api;
	//const struct led_data *led0_data = (const struct led_data)dev_led0->data;
	
	// Start thread
	usbd_tid = k_thread_create(&usbd_thread, 	// Thread struct
							usbd_stack,			// stack
							K_THREAD_STACK_SIZE(usbd_stack),
							m_usbd_task,		// Entry function
							NULL, NULL, NULL,	// arg1, arg2, arg3
							7,					// priority
							0, 					// options)
							K_NO_WAIT);			// Delay

	sw0_tid = k_thread_create(&sw0_thread,		// Thread struct
							sw0_stack,			// stack
							K_THREAD_STACK_SIZE(sw0_stack),
							m_sw0_task,			// Entry function
							NULL, NULL, NULL,	// arg1, arg2, arg3
							7,					// priority
							0, 					// options)
							K_NO_WAIT);			// Delay
	while(1) {
		printk("Hello blinky\n");
		

				
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

