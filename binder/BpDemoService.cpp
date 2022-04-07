#define LOG_TAG "BpDemoService"

#include <utils/log>
#include "IDemoService.h"

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

	IMPLEMENT_META_INTERFACE(DemoService, "android.media.IDemoService");//IDemoService类 的这些方法的实现依赖于  BpDemoService

}