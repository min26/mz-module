
#include <stdio.h>
//#include <inttypes.h>
#include <zephyr/kernel.h>
//#include <zephyr/device.h>
//#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include "m_console.h"
#include "m_button.h"
#include "m_timer.h"

#define MYTIME_1000MS	1000



void timer_work_handler(struct k_work *item)
{
	struct m_timer_info *drv = 
		CONTAINER_OF(item, struct m_timer_info, work);
	k_mutex_lock(&drv->mutex, K_FOREVER);
	printk("timer_handler()\n");
	k_mutex_unlock(&drv->mutex);
}

void button_handler(void)
{
	printk("button_handler()\n");
	m_led_toggle();
}


int main(void)
{
	struct m_timer_info mytimer = {
		.handler = timer_work_handler,
		.duration = K_SECONDS(1),
		.period = K_SECONDS(1) };

	m_console_init();
	printk("main() start\n");

	m_button_init(button_handler);
	m_led_init();

	m_timer_init(&mytimer);
	
	while(1){
		k_msleep(MYTIME_1000MS);
	}
	return 0;
}