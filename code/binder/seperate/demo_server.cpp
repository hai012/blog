#define LOG_TAG "demo_server" 

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>
#include "DemoService.h"



using namespace android;

int main(int argc __unused, char **argv __unused)
{
	//打开打开驱动,mmap等
    sp<ProcessState> proc(ProcessState::self());
	
	/*
	*	获得 BpServiceManager (IServiceManager是BpServiceManager的一个基) 对象的指针
	*	defaultServiceManager 是一个android命名空间的一个普通函数不是方法在frameworks/native/libs/binder/IServiceManager.cpp实现:
	*	sp<IServiceManager> defaultServiceManager()
	*	{
	*		std::call_once(gSmOnce, []() {
	*			sp<IServiceManager> sm = nullptr;
	*			while (sm == nullptr) {
	*				sm = interface_cast<IServiceManager>(ProcessState::self()->getContextObject(nullptr));
	*				if (sm == nullptr) {
	*					ALOGE("Waiting 1s on context object on %s.", ProcessState::self()->getDriverName().c_str());
	*					sleep(1);
	*				}
	*			}
	*	
	*			gDefaultServiceManager = new ServiceManagerShim(sm);
	*		});
	*		return gDefaultServiceManager;
	*	}
	*	
	*	在ProcessState类中，getContextObject方法调用getStrongProxyForHandle方法并传入参数0
	*	getStrongProxyForHandle方法中使用handle参数(此时等于0)创建 BpBinder 对象(class BpBinder : public IBinder)，
	*	getContextObject(nullptr)
	*		getStrongProxyForHandle(0);
    *               BpBinder::create(handle);
	*	最后getContextObject返回的sp<IBinder>其实指向的是一个 BpBinder(handle=0) 的对象
	*	
	*	
	*	interface_cast负责将 IBinder 接口的 BpBinder(handle=0)对象 转换成 IServiceManager接口的 BpServiceManager 对象，
	*	
	*	template<typename INTERFACE>
    *   inline sp<INTERFACE> interface_cast(const sp<IBinder>& obj)
    *   {
    *       return INTERFACE::asInterface(obj);
    *   }
	*   INTERFACE实例化后就是IServiceManager
	*	asInterface方法通过  DECLARE_META_INTERFACE(ServiceManager) 宏来声明
	*   通过 IMPLEMENT_META_INTERFACE(ServiceManager, "android.xxx")宏来实现
	*   IMPLEMENT_META_INTERFACE宏实现的asInterface方法中new一个 BpServiceManager 并返回其智能指针。
	*
	*
	*   最终  defaultServiceManager  返回的是一个 sp<IServiceManager> 智能指针对象，智能指针指向 BpServiceManager 对象
	*   ProcessState::self()->getContextObject(nullptr)返回的是  BpBinder(handle=0) 对象，
	*   通过 interface_cast 等后续转化成  BpServiceManager  对象的
	*   通过 BpServiceManager 对象的 getService 方法 得到  BpBinder(handle=目标服务所在进程对应的handle编号) 对象
	
	*  BpServiceManager 对象的 getService 方法 和 ProcessState::self()->getContextObject(nullptr) 底层都调用了getStrongProxyForHandle(handle)来
    *  创建BpBinder(handle=xxx) 对象并返回其智能指针，在 getService 方法中调用getStrongProxyForHandle(handle)传入的handle来自service_manager的回复，
	*  在 getContextObject 方法中调用getStrongProxyForHandle(handle)传入handle的值为0。然后 通过 interface_cast 使用BpBinder(handle=xxx) 对象创建
	*  目标服务的 BpXXXService 代理对象，BpDemoService和BpServiceManager都派生自 BpRefBase,BpRefBase类中有IBinder * mRemote 属性，interface_cast在
	*  创建BpDemoService和BpServiceManager时将 mRemote 指向传进来的 BpBinder(handle=xxx)。BpBinder 是 IBinder 的派生类
	*  
	*  BpXXXService 代理对象的 getService 首先构造好了数据，再调用remote()->transact(....)  remote()返回的是 IBinder * mRemote，
	*  然后调用 IBinder 类中实现的 transact 方法。
	*
	*/
    sp<IServiceManager> sm = defaultServiceManager();  //sp<IServiceManager> sm(defaultServiceManager());是同样的效果
	
	
	//调用 addService 注册service
	sm->addService(String16("Demo"), new BnDemoService);
	
	///while(1){ read data, parse data, call funcx}
	ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
	
}