#include <android/hardware/led/1.0/IPowerLed.h>
#include <hidl/LegacySupport.h>

using android::hardware::led::V1_0::IPowerLed;
using android::hardware::defaultPassthroughServiceImplementation;

int main() {
    configureRpcThreadpool (2,true);//避免主线程registerAsService后joinRpcThreadpool之前来了请求没有线程进行处理

    PowerLed powerledObj();

    auto status = powerledObj.registerAsService();
    CHECK_EQ(status, android::OK) << "Failed to register powerledObj HAL implementation";

    joinRpcThreadpool();
    return  -1;
}
