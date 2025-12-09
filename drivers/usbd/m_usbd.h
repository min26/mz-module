/*
 * Copyright (c) 2023 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _M_USBD_INIT_H_
#define _M_USBD_INIT_H_

#include <stdint.h>
#include <zephyr/usb/usbd.h>

/*
 * This function uses Kconfig.usbd options to configure and initialize a
 * USB device. It configures sample's device context, default string descriptors,
 * USB device configuration, registers any available class instances, and
 * finally initializes USB device. It is limited to a single device with a
 * single configuration instantiated in m_usbd_init.c, which should be
 * enough for a simple USB device.
 *
 * It returns the configured and initialized USB device context on success,
 * otherwise it returns NULL.
 */
struct usbd_context *m_usbd_init_device(usbd_msg_cb_t msg_cb);

/*
 * This function is similar to m_usbd_init_device(), but does not
 * initialize the device. It allows the application to set additional features,
 * such as additional descriptors.
 */
struct usbd_context *m_usbd_setup_device(usbd_msg_cb_t msg_cb);

#endif /* _M_USBD_INIT_H_ */
