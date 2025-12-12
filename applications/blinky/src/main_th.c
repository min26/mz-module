
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



K_SEM_DEFINE(sem_led, 0, 1); //name, init-count, limit)


void thread_entry_led(void *arg1, void *arg2, void *arg3)
{
    while(1) {

        k_sem_take(&sem_led, K_FOREVER); //Wait
        print("do something...\n");
        k_sem_give(&my_sem); // Release semaphore        

        k_msleep(TIME_1000MS);
    }
}

int main(void)
{
    K_THREAD_DEFINE(tid_sw0,
                    MY_STACK_SIZE,
                    thread_entry_led, NULL, NULL, NULL,
                    MY_HIGH_PRIORITY,
                    0,  /* option */
                    0); /* no delay */

    print("Hello blinky\n");
    while(1) {
                
        k_sem_give(&sem_led); //Signal to 

        k_msleep(TIME_1000MS);
    }
}