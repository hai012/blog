
#define LOG_TAG "demo_client"


#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>
#include "DemoService.h"



using namespace android;


int main(int argc argc, char **argv argv)
{
	int ret;
	
	if(argc > 2) {
		return -1;
	}
	
	//打开打开驱动,mmap等
    sp<ProcessState> proc(ProcessState::self());
	
	//获得BpServiceManager
    sp<IServiceManager> sm(defaultServiceManager());
	
	//调用 getService 获得binder节点
	sp<IBinder> binder = sm->getService(String16("Demo"));
	
	if(binder == 0) {
		ALOGI("can't get Demo service");
		return -1; 
	}
	
	//将binder节点转化成Demo  service的接口类
	sp<IDemoService> service = interface_cast<IDemoService>(binder);
	
	//使用服务
	if(argc == 1) {
		service->func1();
		ALOGI("client call func1 OK\n");
	} else {
		ret = service->func2(argv[2]);
		ALOGI("client call func2 OK, ret=%d\n",ret);
	}
	
	return 0;
}
