

#include "m_timer.h"

#include <zephyr/device.h>
#include <errno.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(m_timer);





static void example_work_handler(struct k_work *id)
{
    struct m_timer_driver *drv = 
                CONTAINER_OF(id, struct m_timer_driver, work);
    k_mutex_lock(&drv->mutex, K_FOREVER);
    printk("dev name:%s\n", &drv->name);
    k_mutex_unlock(&drv->mutex);
}

static void timer_handler(struct k_timer *id)
{    
    struct m_timer_driver *drv = 
                CONTAINER_OF(id, struct m_timer_driver, work);
    // add to system workqueue
    k_work_submit(&drv->work);    
}

/** public function */
void m_timer_init(m_timer_driver *drv)
{
    LOG_DBG("m_timer_init()\n");

    k_mutex_init(&drv->mutex);
    k_work_init(&drv->work, drv->work_handler);
    k_timer_init(&drv->timer, timer_handler, NULL);
    // start timer
    k_timer_start(&drv->timer.timer, drv->duration, drv->period);
}