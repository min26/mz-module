#mz_module
min's zephyr-rtos module, NXP kineties 
    - K66f 2MB
    - K64f 1MB 

# Workspace Tree

    
## DTS 
## LABEL: NODE-NAME@ADDRESS
aliases { my_serial = &serial0; }
chosen { zephyr,console = &serial0; }
/{
    soc {
        serial0: serial@400200 {
            status = "okay";
            speed = <6900>;
        };
    };
};

## C/C++ 
#define MY_SERIAL DT_NODELABEL(serial0)
#define MY_SERIAL DT_ALIAS(my_serial)
#define MY_SERIAL DT_CHOSEN(zephyr_console)
#define MY_SERIAL DT_PATH(soc,serial_400200)

#if DT_NODE_HAS_STATUS(MY_SERIAL, okay)
    <option#1>
    const struct device *const uart_dev = DEVICE_DT_GET(MY_SERIAL)
    if (!device_is_ready(uart_dev)) return -ENODEV;
    <option#2>
    const char *dev_name = /* run time binding name */
    const struct device *const uart_dev = device_get_binding(dev_name);
#else
    #ERROR "Node is disabled"
#endif




## C/C++ example
struct dev-data { ... };
struct dev-cfg { uint32_t freq; ... };

static int dev_api_func(const struct device *dev, uint32_t *foo)
{ ... }

static struct myapi my_api_func = {.func1 = dev_api_func, ... }






## CMake: if found it, stop research 
1. DT_OVERLAY_FILE 
2. boards/<board>.overlay
3. <board>.overlay
4. app.overlay
- extra file
    EXTRA_DT_OVERLAY_FILE

## 


