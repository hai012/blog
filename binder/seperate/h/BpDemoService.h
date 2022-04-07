

#ifndef ANDROID_BPDEMOSERVICE_H
#define ANDROID_BPDEMOSERVICE_H

//以下是安卓系统提供的头文件
#include <binder/IInterface.h>//提供 class BpInterface<>  模板类 的声明
#include <utils/StrongPointer.h>//提供 class sp<> 模板类 的声明
#include <binder/IBinder.h>//提供 class IBinder 类 的声明

//以下是本模块内部提供的头文件
#include "IDemoService.h"//提供 class IDemoService 类 的声明


//IDemoService由handle(handle对应着binder底层ipc通信时的进程)生成


namespace android {

	//BpInterface<IDemoService> 由 IDemoService 和 BpRefBase 类public派生而出，且加入了一些属性和方法
	//class BpRefBase : public virtual RefBase
	class BpDemoService: public BpInterface<IDemoService>
	{

		public:
			BpDemoService(const sp<IBinder> &impl);
			virtual void func1(void);
			virtual int  func2(const char *str);
	}
}

#endif