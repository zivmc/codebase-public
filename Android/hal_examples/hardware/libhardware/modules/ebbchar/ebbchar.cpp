#define LOG_TAG "EbbcharHALStub"
#define LOG_NDEBUG 0

#include <hardware/hardware.h>
#include <hardware/ebbchar.h>

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string>

#include <log/log.h>
#include <malloc.h>
#include <cutils/atomic.h>

#define DEVICE_NAME "/dev/ebbchar"
#define MODULE_NAME "Ebbchar"
#define MODULE_AUTHOR "shyluo@gmail.com"

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];

static int ebbchar_device_open(const struct hw_module_t* module, const char* id, struct hw_device_t** device);
static int ebbchar_device_close(struct hw_device_t* device);
static int ebbchar_set_input(struct ebbchar_device_t* dev, const char* input);
static int ebbchar_get_output(struct ebbchar_device_t* dev, char** output);

static struct hw_module_methods_t ebbchar_module_methods = {
	.open = ebbchar_device_open
};

struct ebbchar_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.version_major = 1,
		.version_minor = 0,
		.id = EBBCHAR_HARDWARE_MODULE_ID,
		.name = MODULE_NAME,
		.author = MODULE_AUTHOR,
		.methods = &ebbchar_module_methods,
	}
};

static int ebbchar_device_open(const struct hw_module_t* module, const char* id, struct hw_device_t** device) {
	if(!strcmp(id, EBBCHAR_HARDWARE_DEVICE_ID)) {
		struct ebbchar_device_t* dev;

		dev = (struct ebbchar_device_t*)malloc(sizeof(struct ebbchar_device_t));
		if(!dev) {
			ALOGE("Failed to alloc space for ebbchar_device_t.");
			return -EFAULT;
		}

		memset(dev, 0, sizeof(struct ebbchar_device_t));

		dev->common.tag = HARDWARE_DEVICE_TAG;
		dev->common.version = 0;
		dev->common.module = (hw_module_t*)module;
		dev->common.close = ebbchar_device_close;
		dev->set_input = ebbchar_set_input;
		dev->get_output = ebbchar_get_output;

		if((dev->fd = open(DEVICE_NAME, O_RDWR)) == -1) {
			ALOGE("Failed to open device file /dev/ebbchar -- %s.", strerror(errno));
			free(dev);
			return -EFAULT;
		}

		*device = &(dev->common);
		ALOGI("Open device file /dev/ebbchar successfully.");


		return 0;
	}

	return -EFAULT;
}

static int ebbchar_device_close(struct hw_device_t* device) {
	struct ebbchar_device_t* ebbchar_device = (struct ebbchar_device_t*)device;
	if(ebbchar_device) {
		close(ebbchar_device->fd);
		free(ebbchar_device);
	}

	return 0;
}

static int ebbchar_set_input(struct ebbchar_device_t* dev, const char* input) {
	if(!dev) {
		ALOGE("Null dev pointer.");
		return -EFAULT;
	}

	ALOGI("Set input %s to device file /dev/ebbchar.", input);
	write(dev->fd, input, sizeof(input));

	return 0;
}

static int ebbchar_get_output(struct ebbchar_device_t* dev, char** output) {
	if(!dev) {
		ALOGE("Null dev pointer.");
		return -EFAULT;
	}

	if(!output) {
		ALOGE("Null output pointer.");
		return -EFAULT;
	}

	read(dev->fd, receive, sizeof(receive));

    *output = receive;

	ALOGI("Get output %s from device file /dev/ebbchar.", *output);

	return 0;
}
