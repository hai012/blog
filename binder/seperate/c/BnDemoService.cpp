#define LOG_TAG "BnDemoService"



#include <utils/Log.h>

//以下头文件来自标准C库，位于交叉编译工具链路径中
#include <stdint.h>//提供 uint32_t 的定义
//以下头文件来自C++标准库，头文件放在交叉编译工具链的路径中
#include <bits/unique_ptr.h>//提供  unique_ptr<>  模板类 的声明

//以下是安卓系统提供的头文件
#include <binder/IInterface.h>//提供  CHECK_INTERFACE 宏 的定义

#include <binder/Binder.h>//提供  BBinder 类 的声明
#include <utils/String8.h>//提供  String8 类 的声明


//以下是本模块内部提供的头文件
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