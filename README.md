#zephyr_module
min's zephyr-rtos module, NXP kineties K66f 2MB 

# Workspace Tree

zephyr_module/
    ..CMakeLists.txt[1]
    ..Kconfig[2]
    ..zephyr/
        ....module.yaml[3]
    ..dts/bindings/
            ......mg,button.yaml
    ..drivers/
        ....CMakeLists.txt[4]
        ....Kconfig[5]
        ....button/
            ......CMakeLists.txt[6]
            ......Kconfig[7]
            ......button.c[8]
            ......button.h[9]
    ..applications/
        ....ap_motor/
    
