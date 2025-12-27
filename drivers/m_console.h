
#ifndef _M_CONSOLE_H_
#define _M_CONSOLE_H_


#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>

#define CONSOLE_MODE	DT_CHOSEN(zephyr_console)
BUILD_ASSERT(DT_NODE_HAS_COMPAT(CONSOLE_MODE, zephyr_cdc_acm_uart),
	"Console device is not ACM CDC UART");


int m_console_init(void);

#endif /* _M_CONSPLE_H_ */