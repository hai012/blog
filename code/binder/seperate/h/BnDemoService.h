
#ifndef ANDROID_BNDEMOSERVICE_H
#define ANDROID_BNDEMOSERVICE_H


//以下头文件来自标准C库，位于交叉编译工具链路径中
#include <stdint.h>//提供uint32_t的定义

//以下是安卓系统提供的头文件
#include <binder/IInterface.h>//提供 class BnInterface<>  模板类 的声明
#include <utils/Errors.h>     //提供   status_t 的typedef 声明 和  NO_ERROR 的enum声明
#include <binder/Parcel.h>  //提供   Parcel 类 的声明

//以下是本模块内部提供的头文件
#include "IDemoService.h"//提供 class IDemoService 类 的声明



namespace android {
	//BnInterface<IDemoService> 模板类由 IDemoService 和 BBinder 类public派生而出，且加入了一些属性和方法
	//class BBinder : public IBinder
	//class IBinder : public virtual RefBase
	class BnDemoService: public BnInterface<IDemoService>
	{
	public:
		virtual void func1(void);
		virtual int  func2(const char *str);

		virtual status_t    onTransact( uint32_t code,
										const Parcel& data,
										Parcel* reply,
										uint32_t flags = 0);
	};
#endif