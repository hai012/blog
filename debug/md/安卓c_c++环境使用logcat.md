```
1、#include <cutils/log.h>
可以使用ALOGX，通过define LOG_TAG xxx 来指定标识
Android.mk:
LOCAL_C_INCLUDE += system/core/libcutils/include \
                   system/core/liblog/include \
				   system/core/include
LOCAL_SHARED_LIBRARY += liblog



2、#include <utils/Log.h>
可以使用ALOGX，通过define LOG_TAG xxx 来指定标识
Android.mk:
LOCAL_C_INCLUDE += system/core/libutils/include \
                   system/core/liblog/include \
				   system/core/include
LOCAL_SHARED_LIBRARY += liblog


3、#include <log/log.h>
可以使用ALOGX，通过define LOG_TAG xxx 来指定标识
Android.mk:
LOCAL_C_INCLUDE += system/core/liblog/include \
				   system/core/include
LOCAL_SHARED_LIBRARY += liblog

4、#include <android/log.h>
ALOGX 和 LOG_TAG  需要自己define声明
#define LOG_TAG "hardCtrl"
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG ,__VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG ,__VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG ,__VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG ,__VA_ARGS__)
#define ALOGF(...) __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG ,__VA_ARGS__)
#define ALOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG ,__VA_ARGS__)
Android.mk:
LOCAL_C_INCLUDE += system/core/include
LOCAL_SHARED_LIBRARY += liblog
```

