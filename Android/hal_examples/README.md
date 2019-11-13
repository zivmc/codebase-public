## 将HAL模块安装到vendor分区

需要重新挂载 /vendor 分区，所以需要先把 Android 源码中以下文件中的 `ro.secure=1` 修改为 `ro.secure=0`:

1. build/core/build-system.html
2. build/core/main.mk

重新编译、烧录到设备之后：

```
adb shell setenforce 0
adb shell mount -o rw,remount /vendor
```

这时可以把编译出的 ebbchar.default.so 推到设备中去：

```
adb push $OUT/vendor/lib/hw/ebbchar.default.so /vendor/lib/hw/
adb push $OUT/vendor/lib64/hw/ebbchar.default.so /vendor/lib64/hw/
```

## sepolicy

sepolicy 的修改有两部分，一部分是修改 /dev/ebbchar 设备权限相关的 sepolicy，另一部分是修改 Ebbchar Service 权限相关的 sepolicy。两部分的修改分别参照 ashmem_device 的 sepolicy 和 vibrator_sevice 的 sepolicy 即可。

这里的 0001-add-sepolicy-for-ebbchar.patch 是适用于 android-8.1.0_r1 的一个补丁文件，如果版本正确，可以通过 git apply 的方式直接将修改导入源码。

