
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define TIME_1000MS             1000
#define MY_STACK_SIZE           256
#define MY_HIGH_PRIORITY        5

#define MY_DEV_IRQ              24
#define MY_DEV_PRIO             2

/// @brief semaphore  
K_SEM_DEFINE(led_sem, 0, 1); //name, init-count, limit)


/// @brief working thread handler
/// @param work 
void work_handler(struct k_work *work)
{
    // do something
}
K_WORK_DEFINE(mywork, work_handler);

/// @brief timer ISR, call work thread
/// @param timer_id 
void timer_callback(struct k_timer *timer_id)
{
    // dump to working thread
    k_work_submit(&mywork);
}

void led_task(void *arg1, void *arg2, void *arg3)
{
    while(1) {

        k_sem_take(&led_sem, K_FOREVER);
        print("do something...\n");
        k_sem_give(&led_sem); // Release semaphore        

        k_msleep(TIME_1000MS);
    }
}

/// @brief 
/// @param  
/// @return 
int main(void)
{
    K_TIMER_DEFINE(my_timer, timer_callback, NULL);
    
    K_THREAD_DEFINE(sw0_tid,
                    MY_STACK_SIZE,
                    led_task, NULL, NULL, NULL,
                    MY_HIGH_PRIORITY,
                    0,  /* option */
                    0); /* no delay */

    print("Hello blinky\n");
    while(1) {
                
        k_sem_give(&led_sem); // Signal to 

        k_msleep(TIME_1000MS);
    }
}