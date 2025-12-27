
#ifndef _M_TIMER_H_
#define _M_TIMER_H_


#include <stdio.h>
#include <zephyr/kernel.h>
/// container_of()
#include <zephyr/sys/util.h>



struct m_timer_info {
    struct k_work work; 
    struct k_mutex mutex;
    struct k_timer timer;    
    /* user define below */
    void (*handler)(struct k_work*);
    k_timeout_t duration;
    k_timeout_t period;
};

/** main() code example *******************
static void work_handler(struct k_work *item)
{
	struct m_timer_info *drv = 
			CONTAINER_OF(item, struct m_timer_info, work);
	k_mutex_lock(&drv->mutex, K_FOREVER);
    // do something //
    k_mutex_unlock(&drv->mutex);
}
struct m_timer_info timer = {
	.handler = work_handler,
	.duration = K_SECOND(1),
	.period = K_SECOND(1),
}
m_timer_init(&timer);
******************************************/

void m_timer_init(struct m_timer_info *timer);



#endif /* _M_TIMER_H_ */