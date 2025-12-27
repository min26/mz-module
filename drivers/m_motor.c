
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>

#include "m_motor.h"
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

#include <errno.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(m_motor);


static const struct pwm_dt_spec motor = PWM_DT_SPEC_GET(MOTOR_NODE);
static const uint32_t min_pulse = DT_PROP(MOTOR_NODE, min_pulse);
static const uint32_t max_pulse = DT_PROP(MOTOR_NODE, max_pulse);

enum direction {
	DOWN,
	UP,
};

int m_motor_init(void)
{
	return 0;
}

int m_motor_up(void)
{
	return 0;
}