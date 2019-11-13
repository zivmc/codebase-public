#define LOG_TAG "EbbcharServiceJNI"
#define LOG_NDEBUG 0

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/hardware.h>
#include <hardware/ebbchar.h>

#include <stdio.h>


namespace android
{
	static void ebbchar_setInput(JNIEnv* env, jobject clazz, jlong ptr, jstring input) {
		ebbchar_device_t* device = (ebbchar_device_t*)ptr;
		if(!device) {
			ALOGE("Device ebbchar is not open.");
			return;
		}

    const char* str = env->GetStringUTFChars(input, NULL);
    if(str == NULL) {
       return; /* OutOfMemoryError already thrown */
    }

		ALOGI("Set input %s to device ebbchar.", str);

		device->set_input(device, str);
	}

	static jstring ebbchar_getOutput(JNIEnv* env, jobject clazz, jlong ptr) {
		ebbchar_device_t* device = (ebbchar_device_t*)ptr;
		if(!device) {
			ALOGE("Device ebbchar is not open.");
			return env->NewStringUTF("");
		}

		char* output;

		device->get_output(device, &output);

		ALOGI("Get output %s from device ebbchar.", output);

		return env->NewStringUTF(output);
	}

	static inline int ebbchar_device_open(const hw_module_t* module, struct ebbchar_device_t** device) {
		return module->methods->open(module, EBBCHAR_HARDWARE_DEVICE_ID, (struct hw_device_t**)device);
	}

	static jlong ebbchar_init(JNIEnv* env, jclass clazz) {
		ebbchar_module_t* module;
		ebbchar_device_t* device;

		ALOGI("Initializing HAL stub ebbchar......");

		if(hw_get_module(EBBCHAR_HARDWARE_MODULE_ID, (const struct hw_module_t**)&module) == 0) {
			ALOGI("Device ebbchar found.");
			if(ebbchar_device_open(&(module->common), &device) == 0) {
				ALOGI("Device ebbchar is open.");
				return (jlong)device;
			}

			ALOGE("Failed to open device ebbchar.");
			return 0;
		}

		ALOGE("Failed to get HAL stub ebbchar.");

		return 0;
	}

	static const JNINativeMethod method_table[] = {
		{"init_native", "()J", (void*)ebbchar_init},
		{"setInput_native", "(JLjava/lang/String;)V", (void*)ebbchar_setInput},
		{"getOutput_native", "(J)Ljava/lang/String;", (void*)ebbchar_getOutput},
	};

	int register_android_server_EbbcharService(JNIEnv *env) {
    		return jniRegisterNativeMethods(env, "com/android/server/EbbcharService", method_table, NELEM(method_table));
	}
};

