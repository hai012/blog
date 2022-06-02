// FIXME: your file license if you have one

#pragma once

#include <android/hardware/led/1.0/IPowerLed.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android::hardware::led::implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct PowerLed : public V1_0::IPowerLed {
    //
    PowerLed::PowerLed(void);

    // Methods from ::android::hardware::led::V1_0::IPowerLed follow.
    Return<void> readState(int32_t which, readState_cb _hidl_cb) override;
    Return<bool> writeState(int32_t which, bool state) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IPowerLed* HIDL_FETCH_IPowerLed(const char* name);

}  // namespace android::hardware::led::implementation
