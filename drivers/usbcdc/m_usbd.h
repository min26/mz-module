
#ifndef _DRIVER_M_USBD_H_
#define _DRIVER_M_USBD_H_

#include <stdint.h>
#include <zephyr/usb/usbd.h>


struct usbd_context *m_usbd_setup_device(usbd_msg_cb_t msg_cb);

struct usbd_context *m_usbd_init_device(ussbd_msg_cb_t msg_cb);

int m_usbd_task(const struct device *dev);

#endif /* _DRIVER_M_USBD_H_ */