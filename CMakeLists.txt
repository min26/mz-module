# This CMake file is picked by zephyr.
# It is defined as module CMake entry point (zephyr/module.yml)

add_subdirectory(drivers)
zephyr_include_directories(drivers)

