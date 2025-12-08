
#include "m_usb.h"

#include <zephyr/device.h>
#include <errno.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(m_usbd);

#include <zephyr/sys/ring_buffer.h>
#define RING_BUF_SIZE 1024
uint8_t ring_buffer[RING_BUF_SIZE];
struct ring_buf ringbuf;

#include <zephyr/usb/usbd.h>
#include <zephyr/usb/bos.h>
static struct usbd_context *m_usbd;
K_SEM_DEFINE(dtr_sem, 0, 1);

static const char *const blocklist[] {
    "dfu_dfu",
    NULL,
};

static bool rx_throttled;

/****************************************
 *  doc configuration instantiation start 
 * */
/*
 * Create a 'usbd_context' named 'm_usbd' using the default USB device
 * controller, the Zephyr project vendor ID, and the sample product ID.
 */
USBD_DEVICE_DEFINE(m_usbd,
		   DEVICE_DT_GET(DT_NODELABEL(zephyr_udc0)),
		   CONFIG_USB_DEVICE_VID, CONFIG_USB_DEVICE_PID);

USBD_DESC_LANG_DEFINE(m_usbd_lang);
USBD_DESC_MANUFACTURER_DEFINE(m_usbd_mfg, CONFIG_USB_DEVICE_MANUFACTURER);
USBD_DESC_PRODUCT_DEFINE(m_usbd_prod, CONFIG_USB_DEVICE_PRODUCT);
IF_ENABLED(CONFIG_HWINFO, (USBD_DESC_SERIAL_NUMBER_DEFINE(m_usbd_sn)));

USBD_DESC_CONFIG_DEFINE(fs_cfg_desc, "FS Configuration");
USBD_DESC_CONFIG_DEFINE(hs_cfg_desc, "HS Configuration");

/** USB required one configuration per speed */
static const uint8_t attributes = 
                (IS_ENABLED(CONFIG_USB_DEVICE_SELF_POWERED) ? USB_SCD_SELF_POWERED : 0) |
				  (IS_ENABLED(CONFIG_USB_DEVICE_REMOTE_WAKEUP) ? USB_SCD_REMOTE_WAKEUP : 0);

/* -- Full speed configuration */
USBD_CONFIGURATION_DEFINE(m_fs_config,
			            attributes,
			            CONFIG_USB_DEVICE_MAX_POWER, &fs_cfg_desc);

/* -- High speed configuration */
USBD_CONFIGURATION_DEFINE(m_hs_config,
			            attributes,
			            CONFIG_USB_DEVICE_MAX_POWER, &hs_cfg_desc);

#if CONFIG_USB_DEVICE_20_EXTENSION_DESC
/*
 * This does not yet provide valuable information, but rather serves as an
 * example, and will be improved in the future.
 */
static const struct usb_bos_capability_lpm bos_cap_lpm = {
	.bLength = sizeof(struct usb_bos_capability_lpm),
	.bDescriptorType = USB_DESC_DEVICE_CAPABILITY,
	.bDevCapabilityType = USB_BOS_CAPABILITY_EXTENSION,
	.bmAttributes = 0UL,
};
USBD_DESC_BOS_DEFINE(m_usbext, sizeof(bos_cap_lpm), &bos_cap_lpm);
#endif

/**
 * doc configuration instantiation end 
 * ************************************/

static void m_fix_code_triple(struct usbd_context *uds_ctx,
				            const enum usbd_speed speed)
{
	/* Always use class code information from Interface Descriptors */
	if (IS_ENABLED(CONFIG_USBD_CDC_ACM_CLASS)) {
		/*
		 * Class with multiple interfaces have an Interface
		 * Association Descriptor available, use an appropriate triple
		 * to indicate it.
		 */
		usbd_device_set_code_triple(uds_ctx, speed,
					    USB_BCC_MISCELLANEOUS, 0x02, 0x01);
	} else {
		usbd_device_set_code_triple(uds_ctx, speed, 0, 0, 0);
	}
}


struct usbd_context *m_usbd_setup_device(usbd_msg_cb_t msg_cb)
{
	int err;

	/* Required string descriptor *******************************/
	err = usbd_add_descriptor(&m_usbd, &m_usbd_lang);
	if (err) {
		LOG_ERR("Failed to initialize language descriptor (%d)", err);
		return NULL;
	}

	err = usbd_add_descriptor(&m_usbd, &m_usbd_mfg);
	if (err) {
		LOG_ERR("Failed to initialize manufacturer descriptor (%d)", err);
		return NULL;
	}

	err = usbd_add_descriptor(&m_usbd, &m_usbd_prod);
	if (err) {
		LOG_ERR("Failed to initialize product descriptor (%d)", err);
		return NULL;
	}

	IF_ENABLED(CONFIG_HWINFO,(err = usbd_add_descriptor(&m_usbd, &m_usbd_sn);))
	if (err) {
		LOG_ERR("Failed to initialize SN descriptor (%d)", err);
		return NULL;
	}

	if (USBD_SUPPORTS_HIGH_SPEED && usbd_caps_speed(&m_usbd) == USBD_SPEED_HS) {
		err = usbd_add_configuration(&m_usbd, USBD_SPEED_HS,&sample_hs_config);
		if (err) {
			LOG_ERR("Failed to add High-Speed configuration");
			return NULL;
		}

		err = usbd_register_all_classes(&m_usbd, USBD_SPEED_HS, 1, blocklist);
		if (err) {
			LOG_ERR("Failed to add register classes");
			return NULL;
		}

		m_fix_code_triple(&m_usbd, USBD_SPEED_HS);
	}

	/* Required to add speed configuration to the device-context 
        before initialized USBD */
	err = usbd_add_configuration(&m_usbd, USBD_SPEED_FS, &m_fs_config);
	if (err) {
		LOG_ERR("Failed to add Full-Speed configuration");
		return NULL;
	}
	err = usbd_register_all_classes(&m_usbd, USBD_SPEED_FS, 1, blocklist);
	if (err) {
		LOG_ERR("Failed to add register classes");
		return NULL;
	}

	m_fix_code_triple(&m_usbd, USBD_SPEED_FS);
	usbd_self_powered(&m_usbd, attributes & USB_SCD_SELF_POWERED);

	if (msg_cb != NULL) {
		/* doc device init-and-msg */
		err = usbd_msg_register_cb(&m_usbd, msg_cb);
		if (err) {
			LOG_ERR("Failed to register message callback");
			return NULL;
		}
	}

#if CONFIG_SAMPLE_USBD_20_EXTENSION_DESC
	(void)usbd_device_set_bcd_usb(&m_usbd, USBD_SPEED_FS, 0x0201);
	(void)usbd_device_set_bcd_usb(&m_usbd, USBD_SPEED_HS, 0x0201);

	err = usbd_add_descriptor(&m_usbd, &m_usbext);
	if (err) {
		LOG_ERR("Failed to add USB 2.0 Extension Descriptor");
		return NULL;
	}
#endif

	return &m_usbd;
}


struct usbd_context *m_usbd_init_device(usbd_msg_cb_t msg_cb)
{
	int err;

	if (m_usbd_setup_device(msg_cb) == NULL) {
		return NULL;
	}

	/* doc device init start */
	err = usbd_init(&m_usbd);
	if (err) {
		LOG_ERR("Failed to initialize device support");
		return NULL;
	}
	/* doc device init end */

	return &m_usbd;
}


static int enable_usb_device_next(void)
{
	int err;

	sample_usbd = sample_usbd_init_device(sample_msg_cb);
	if (sample_usbd == NULL) {
		LOG_ERR("Failed to initialize USB device");
		return -ENODEV;
	}

	if (!usbd_can_detect_vbus(sample_usbd)) {
		err = usbd_enable(sample_usbd);
		if (err) {
			LOG_ERR("Failed to enable device support");
			return err;
		}
	}

	LOG_INF("USB device support enabled");

	return 0;
}

int m_usbd_task(const struct device *dev)
{
    int ret;
    uint32_t dtr = 0;
    if (!device_is_ready(dev)) {
        LOG_ERR("USB DEVICE is not ready\n");
        return -ENODEV;
    }

	ret = enable_usb_device_next();
	if (ret != 0) {
		LOG_ERR("Failed to enable USB device support");
		return 0;
	}

	ring_buf_init(&ringbuf, sizeof(ring_buffer), ring_buffer);

    /* Configure the UART interrupt handler */
    uart_irq_callback_set(dev, interrupt_handler);

    /* Wait for the DTR line to be set */
    LOG_INF("Waiting for DTR...\n");
    while(1) {
        uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
        if (dtr) { break; }
        k_msleep(1000);
    }
    LOG_INF("DTR set, USB connected\n");

    /* Enable RX interrupt */
    uart_irq_rx_enable(dev);

    while(1) {
        // add data transfer
        k_msleep(1000);
    }    
    return 0;
}
