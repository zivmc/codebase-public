#ifndef ANDROID_EBBCHAR_INTERFACE_H
#define ANDROID_EBBCHAR_INTERFACE_H

#include <hardware/hardware.h>

__BEGIN_DECLS

/**
 * The id of this module
 */
#define EBBCHAR_HARDWARE_MODULE_ID "ebbchar"

/**
 * The id of this device
 */
#define EBBCHAR_HARDWARE_DEVICE_ID "ebbchar"

struct ebbchar_module_t {
	struct hw_module_t common;
};

struct ebbchar_device_t {
	struct hw_device_t common;
	int fd;
	int (*set_input)(struct ebbchar_device_t* dev, const char* input);
	int (*get_output)(struct ebbchar_device_t* dev, char** output);
};

__END_DECLS

#endif
