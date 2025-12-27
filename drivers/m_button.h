
#ifndef _M_BUTTON_H_
#define _M_BUTTON_H_

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>


/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE	DT_ALIAS(mysw0)
#if !DT_NODE_HAS_STATUS_OKAY(SW0_NODE)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
/*
 * The led0 devicetree alias is optional. If present, we'll use it
 * to turn on the LED whenever the button is pressed.
 */
#define LED0_NODE	DT_ALIAS(myled0)
#if !DT_NODE_HAS_STATUS_OKAY(LED0_NODE)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif



int m_button_init(void(*handler)(void));

int m_led_init(void);

int m_led_set(int val);

int m_led_toggle(void);

#endif /* M_BUTTON_H_ */