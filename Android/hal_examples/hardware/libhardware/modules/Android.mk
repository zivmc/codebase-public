hardware_modules := \
    audio_remote_submix \
    camera \
    gralloc \
    hwcomposer \
    input \
    radio \
    sensors \
    thermal \
    usbaudio \
    usbcamera \
    vehicle \
    vr \
    ebbchar
include $(call all-named-subdir-makefiles,$(hardware_modules))
