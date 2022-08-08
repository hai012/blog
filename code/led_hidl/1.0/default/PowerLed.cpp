// FIXME: your file license if you have one
#define TAG "PowerLed"

#include <utils/Log.h>
#include "PowerLed.h"

namespace android::hardware::led::implementation {

PowerLed::PowerLed(void) {
    ALOGI("PowerLed constructor, which can open /dev /proc /sys\n");
}



// Methods from ::android::hardware::led::V1_0::IPowerLed follow.
Return<void> PowerLed::readState(int32_t which, readState_cb _hidl_cb) {
    // TODO implement

    ALOGI("readState led%d=%s", which, "on");

    _hidl_cb(true, true);
    return Void();
}

Return<bool> PowerLed::writeState(int32_t which, bool state) {
    // TODO implement

    if(state)
        ALOGI("writeState led%d=%s", which, "on");
    else
        ALOGI("writeState led%d=%s", which, "off");

    return true;
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IPowerLed* HIDL_FETCH_IPowerLed(const char* /* name */) {
//    return new PowerLed();
//}
//
}  // namespace android::hardware::led::implementation
