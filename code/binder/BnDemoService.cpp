#include <utils/log>
#define LOG_TAG "BnDemoService"


#include "IDemoService.h"



namespace android {

	void BnDemoService::func1(void) {
		ALOGI("func1:\n");
	}
	
	int BnDemoService::func2(const char *str) {
		static int ret = 0;
		ALOGI("func2: str=%s   ret_static_int=%d\n", str, ret++);
		return ret;
	}
	status_t BnDemoService::onTransace(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
		switch (code) {
			case FUNC1: {
				//checks to see if the interface exists and you have required permission
				CHECK_INTERFACE(IDemoService, data, reply);
				
				//不需要参数和返回值直接执行FUNC1函数编号对应的函数
				func1();
				
				return NO_ERROR;
			} break;
			case FUNC2: {
				//checks to see if the interface exists and you have required permission
				CHECK_INTERFACE(IDemoService, data, reply);
				
				//从data取出参数
				String8  str8 = data.readString8();
				
				//执行FUNC2函数编号对应的函数
				int ret = func2(str8.string());
				//通过reply传回去
				reply->writeInt32(ret);

				return NO_ERROR;
			} break;
			default:
				return BBinder::onTransact(code, data, reply, flags);
		}
	}
}