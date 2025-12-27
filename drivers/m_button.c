/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * NOTE: If you are looking into an implementation of button events with
 * debouncing, check out `input` subsystem and `samples/subsys/input/input_dump`
 * example instead.
 */


#include "m_button.h"
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

// #include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <errno.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(m_button);



static struct gpio_dt_spec s_btn = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});

static struct gpio_dt_spec s_led = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0});

static struct gpio_callback button_handler;

void (*s_handler)(void);

/**************************
 * cbutton pressed callback 
 * */
void s_button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	LOG_DBG("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
	
	/* do something */
	s_handler();
	
	// /* If we have an LED, match its state to the button's. */
	// if (s_led.port) {		
	// 	int val = gpio_pin_get_dt(&s_btn);
	// 	if (val >= 0) { 
	// 		gpio_pin_set_dt(&s_led, val);
	// 	}
	// }
}


int m_button_init(void(*handler)(void))
{
	int ret;
	s_handler = handler; 

	if (!gpio_is_ready_dt(&s_btn)) {
		LOG_DBG("Error: button device %s is not ready\n", s_btn.port->name);
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&s_btn, GPIO_INPUT);
	if (ret != 0) {
		LOG_DBG("Error %d: failed to configure %s pin %d\n",
		       ret, s_btn.port->name, s_btn.pin);
		return -ENODEV;
	}

	ret = gpio_pin_interrupt_configure_dt(&s_btn, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		LOG_DBG("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, s_btn.port->name, s_btn.pin);
		return -EINTR;
	}

	gpio_init_callback(&button_handler, s_button_pressed, BIT(s_btn.pin));
	gpio_add_callback(s_btn.port, &button_handler);
	LOG_DBG("Set up button at %s pin %d\n", s_btn.port->name, s_btn.pin);	

	return 0;
}

int m_led_init(void)
{
	int ret;
	if (s_led.port && !gpio_is_ready_dt(&s_led)) {
		LOG_DBG("Error: LED device %s is not ready; ignoring it\n", s_led.port->name);
		s_led.port = NULL;
		return -ENODEV;
	}
	if (s_led.port) {
		ret = gpio_pin_configure_dt(&s_led, GPIO_OUTPUT);
		if (ret != 0) {
			LOG_DBG("Error %d: failed to configure LED device %s pin %d\n",
			       ret, s_led.port->name, s_led.pin);
			s_led.port = NULL;
			return -ENODEV;
		} else {
			LOG_DBG("Set up LED at %s pin %d\n", s_led.port->name, s_led.pin);
		}
	}
	return 0;
}

int m_led_set(int val)
{
	int ret;
	LOG_DBG("Set LED to %d\n", val);
	ret = gpio_pin_set_dt(&s_led, val);
	return 0;
}

int m_led_toggle(void)
{
	int ret;
	LOG_DBG("Set LED toggle\n");
	ret = gpio_pin_toggle_dt(&s_led);
	return 0;

}