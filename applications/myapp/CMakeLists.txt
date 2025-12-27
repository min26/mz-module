# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)


### adding drivers root (location of module's CMakeList)
set(ZEPHYR_EXTRA_MODULES ${CMAKE_SOURCE_DIR}/../..)

### same setting as zephyr/module.yml ###
### adding board root (with boards folder) - set on zephyr/module.yml as well.
set(BOARD_ROOT ${CMAKE_SOURCE_DIR}/../..)
set(BOARD fmuk64f)
### adding dts bindings root
list(APPEND DTS_ROOT ${CMAKE_SOURCE_DIR}/../..)


find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(myapp)

target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_sources(app PRIVATE 
		src/main.c)


