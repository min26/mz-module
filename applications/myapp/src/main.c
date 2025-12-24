/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "m_gpio.h"
// #include "m_usbd.h"

// Multi-thread define -------------------------------
#define TIME_1000MS				1000
#define MY_TASK_STACK			256
#define MY_TASK_PRIORITY		5


// // Binary semaphore(name, init-count, limit)
// K_SEM_DEFINE(my_sem, 0, 1);
// // Define task-stack
// K_THREAD_STACK_DEFINE(my_stack, MY_TASK_STACK);
// struct k_thread my_task;

// struct device_info {
// 	struct k_mutex mutex;
// 	struct k_work work;
// 	struct k_timer timer;
// 	char name[16];
// } my_device;

// USB Device -----------------------------------------------
// #define DEV_USB_CDC		DT_CHOSEN(zephyr_console)
// BUILD_ASSERT(DT_NODE_HAS_COMPAT(DEV_USB_CDC, zephyr_cdc_acm),
// 			"Console device is not ACM CDC");
// static const struct device *const dev_usb = DEVICE_DT_GET(DEV_USB_CDC);

// LED, Switch Device ---------------------------------------
// #define DEV_SW0		DT_ALIAS(mysw0)
// static const struct device *dev_sw0 = DEVICE_DT_GET(DEV_SW0);
#define DEV_LED0	DT_ALIAS(myled0)
static const struct device *dev_led0 = DEVICE_DT_GET(DEV_LED0);


// void thread_sw0_func(void arg1, void arg2, void arg3)
// {
// 	int ret;
// 	int sw_state = 0;
// 	if (!device_is_ready(dev_sw0)) {
// 		printk("ERROR:SW0 is not ready\n");
// 		return;
// 	}
// 	const struct m_gpio_api *sw0_api = (const struct m_gpio_api*)dev_sw0->api;
// 	//const struct switch_data *sw0_data = (const struct switch_data*)dev_sw0->data;
// 	while(1) {
// 		ret = sw0_api->get(dev_sw0, &sw_state);
// 		if (ret < 0) {
// 			printk("ERROR:SW0(%d) failed to get\n", ret);
// 		} else {
// 			printk("Switch-get: %d\n", sw_state);
// 			if (sw_state) led_state_set(TOGGLE);
// 		}
// 		k_msleep(TIME_1000MS);
// 	}
// }

// void thread_led0_func(void arg1, void arg2, void arg3)
// {
// 	int ret;
// 	if (!device_is_ready(dev_led0)) {
// 		printk("ERROR:LED0 is not ready\n)");
// 		return;
// 	}
// 	const struct m_gpio_api *led0_api = (const struct m_gpio_api*)dev_led0->api;
// 	//const struct led_data *led0_data = (const struct led_data)dev_led0->data;
// 	while(1) {
// 		printk("Hello blinky\n");
// 		switch (led0_state) {
// 			case OFF:
// 				ret = led0_api->set(dev_led0, M_GPIO_OFF):
// 				if (ret < 0) {
// 					printk("ERROR(%d) failed to set led0\n", ret);
// 				}				
// 				break;
// 			case ON:
// 				ret = led0_api->set(dev_led0, M_GPIO_ON);
// 				if (ret < 0) {
// 					printk("ERROR(%d) failed to clear led0\n", ret);
// 				}
// 				break;
// 			case TOGGLE:
// 				ret = led0_api->toggle(dev_sw0);
// 				if (ret < 0) {
// 					printk("ERROR(%d) failed to toggle led0\n", ret);
// 				}
// 				break;
// 			default:								
// 		}
// 		led0_state = NULL;
// 		// Sleep
// 		k_msleep(TIME_1000MS);
// 	}
// }


int main(void)
{
	int ret;
	int my_state = 0;		
	printk("Hello myapp\n");

	// k_tid_t tid_led0, tid_sw0;
	// // Start thread
	// tid_led0 = k_thread_create(&thread_led0, 	// Thread struct
	// 						stack_led0,			// stack
	// 						K_THREAD_STACK_SIZE(stack_led0),
	// 						thread_led0_func,	// Entry function
	// 						NULL, NULL, NULL,	// arg1, arg2, arg3
	// 						7,					// high priority
	// 						0, 					// options
	// 						K_NO_WAIT);			// Delay

	// tid_sw0 = k_thread_create(&thread_sw0,		// Thread struct
	// 						stack_sw0,			// stack
	// 						K_THREAD_STACK_SIZE(stack_sw0),
	// 						thread_sw0_func,	// Entry function
	// 						NULL, NULL, NULL,	// arg1, arg2, arg3
	// 						8,					// low priority
	// 						0, 					// options
	// 						K_NO_WAIT);			// Delay

	/* testing code */
	if (!device_is_ready(dev_led0)) {
		printk("ERROR:LED0 is not ready\n");
		return -1;
	}
	const struct m_gpio_api *led0_api = (const struct m_gpio_api*)dev_led0->api;
	//const struct m_gpio_data *led0_data = (const struct m_gpio_data*)dev_led0->data;
	
	while(1) {
		ret = led0_api->toggle(dev_led0);
		//
		ret = led0_api->get(dev_led0, &my_state);
		if (ret < 0) {
			printk("ERROR:LED0(%d) failed to get\n", ret);
		} else {
			printk("LED0 get: %d\n", my_state);			
		}
		k_msleep(TIME_1000MS);
	}

	return 0;
}

