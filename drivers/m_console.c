

#include "m_console.h"
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(m_console);



const struct device *const dev = DEVICE_DT_GET(CONSOLE_MODE);

int m_console_init(void)
{
	uint32_t dtr = 0;
	LOG_DBG("m_console_init()\n");

	/* Poll if the DTR flag was set */
	while (!dtr) {
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
		/* Give CPU resources to low priority threads. */
		k_sleep(K_MSEC(100));
	}

	printk("console is ready\n");

	return 0;
}