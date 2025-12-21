
#ifndef _M_TIMER_H_
#define _M_TIMER_H_

#include <stdio.h>
#include <zephyr/kernel.h>

struct m_timer_driver {
	struct k_mutex mutex;
	struct k_timer timer;
	struct k_work work;	
	/* user define below */
	void (*work_handler)(struct k_work *id);
	char name[16];
	int32_t duration;
	int32_t period;	
};
/** main() code example *******************
static void work_handler(struct k_work *id)
{
	struct m_timer_driver *drv = 
			CONTAINER_OF(id, struct m_timer_driver, work);
	k_mutex_lock(&drv->mutex, K_FOREVER);
    // printk("dev name:%s\n", &drv->name);
    k_mutex_unlock(&drv->mutex);
}
struct m_timer_driver driver ={
	.work_handler = work_handler,
	.duration = K_SECOND(1),
	.period = K_SECOND(1),
}
strcpy(driver.name, "MyTimer");

m_timer_init(&driver);
******************************************/


void m_timer_init(m_timer_driver *drv);



#endif /* _M_TIMER_H_ */