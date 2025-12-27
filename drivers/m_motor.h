
#ifndef _M_MOTOR_H_
#define _M_MOTOR_H_

#include <stdio.h>
#include <zephyr/kernel.h>


/*
 * Get motor configuration from the devicetree servo. This is mandatory.
 */
#define MOTOR0_NODE	DT_NODELABLE(servo0)
#if !DT_NODE_HAS_STATUS_OKAY(MOTOR0_NODE)
#error "Unsupported board: servo0 devicetree alias is not defined"
#endif

#define MOTOR_STEP	PWM_USEC(100)




#endif /* _M_MOTOR_H_ */