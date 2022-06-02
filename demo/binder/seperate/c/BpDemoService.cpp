
//以下是安卓系统提供的头文件
#include <binder/Parcel.h>  //提供   Parcel 类 的声明
#include <binder/Binder.h>//提供   remote  inline函数的声明和定义
#include <utils/String8.h>//提供 class String8 类 的声明

#include "BpDemoService.h"

namespace android {
	
	class BpDemoService: public BpInterface<IDemoService>
	{
		
		public BpDemoService(const sp<IBinder> &impl) : BpInterface<IDemoService>(impl)
		{
			
		}
		void func1(void)
		{
			Parcel data, reply;
			data.writeInterfaceToken(IDemoService::getInterfaceDescriptor());
			remote()->transact(FUNC1, data, &reply);
		}
		int func2(const char *str)
		{
			Parcel data, reply;
			data.writeInterfaceToken(IDemoService::getInterfaceDescriptor());
			data.writeString8(String8(str));
			remote()->transact(FUNC1, data, &reply);
			return reply.readInt32();
		}
	}
}