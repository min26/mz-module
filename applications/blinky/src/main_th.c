
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define TIME_1000MS             1000
#define MY_STACK_SIZE           256
#define MY_TASK_PRIORITY        5

#define MY_DEV_IRQ              24
#define MY_DEV_PRIO             2

/// @brief define semaphore  
K_SEM_DEFINE(led_sem, 0, 1); //name, init-count, limit)

/// @brief define stack 
K_THREAD_STACK_DEFINE(led_stack, MY_STACK_SIZE);
struct k_thread led_thread;

struct device_info {
    struct k_mutex mutex;    
    struct k_work work;
    struct k_timer timer;
    char name[16];
} mydevice;

/// gpio from device tree
static const struct gpio_dt_spec = GPIO_DT_SPEC_GET(DT_ALIAS(my_button), gpios);
static struct gpio_callback btn_cb_data;

void gpio_isr_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if(BIT(btn.pin) & pins) {
        printk("button %d pressed\n",btn.pin);
    }
}

/// @brief working thread handler
/// @param item 
void work_handler(struct k_work *item)
{
    struct device_info *mydev = CONTAINER_OF(item, struct devcie_info, work);
    
    k_mutex_lock(&mydev->mutex, K_FOREVER);
    printk("device name:%s\n",mydev->name);
    k_mutex_unlock(&mydev->mutex);
}

/// @brief timer ISR, call work thread
/// @param item 
void timer_handler(struct k_timer *item)
{
    if (item == &mydevice.timer) {
        // dump to working thread
        k_work_submit(&mydevice->work);
    }
}
///

void led_task(void *arg1, void *arg2, void *arg3)
{
    while(1) {
        k_sem_take(&led_sem, K_FOREVER);
        print("do something...\n");
        k_sem_give(&led_sem); // Release semaphore        

        // k_msleep(TIME_1000MS);
    }
}

/// @brief 
/// @param  
/// @return 
int main(void)
{
    int ret;
    if (!gpio_is_ready_dt(&btn)){
        return 0;
    }
    ret = gpio_pin_configure_dt(&btn, GPIO_INPUT);
    ret = gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&btn_cb_data, gpio_isr_handler, BIT(btn.pin));
    gpio_add_callback(btn.port, &btn_cb_data);


    strcpy(mydevice.name, "led-switch");
    k_mutex_init(&mydevice.mutex);
    k_work_init(&mydevice.work, work_handler);
    k_timer_init(&mydevice.timer, timer_handler, NULL);
    
    
    k_tid_t led_tid = k_thread__create(&led_thread,
                                    led_stack,
                                    MY_STACK_SIZE,
                                    led_task, NULL, NULL, NULL,
                                    MY_TASK_PRIORITY,
                                    0, 0); /* option, no-delay */

    /// start periodic timer (timer, duration(before-calling), period(every 1 sec))    
    k_timer_start(&mydevice.timer, K_SECOND(1), k_SECONDS(1));
    
    print("Hello blinky\n");
    while(1) {
                
        k_sem_give(&led_sem); // Signal to 

        k_msleep(TIME_1000MS);
    }
    return 0;
}