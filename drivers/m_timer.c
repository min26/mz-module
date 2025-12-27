

#include "m_timer.h"

#include <zephyr/device.h>
#include <errno.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(m_timer);



void s_timer_handler(struct k_timer *item)
{    
    struct m_timer_info *drv = 
                CONTAINER_OF(item, struct m_timer_info, timer);
    // add to system workqueue
    k_work_submit(&drv->work);    
}

/** public function */
void m_timer_init(struct m_timer_info *timer)
{
    LOG_DBG("m_timer_init()\n");

    k_mutex_init(&timer->mutex);
    k_work_init(&timer->work, timer->handler);
    k_timer_init(&timer->timer, s_timer_handler, NULL);
    // start timer
    k_timer_start(&timer->timer, timer->duration, timer->period);
}