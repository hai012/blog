/*


frameworks/native/libs/binder/

相关头文件
system/core/libutils/include/utils/RefBase.h                         RefBase
frameworks/native/include/binder/IBinder.h                           IBinder
frameworks/native/libs/binder/include/binder/Binder.h                BBinder     BpRefBase    
frameworks/native/libs/binder/include/binder/IInterface.h            IInterface  BnInterface  BpInterface
frameworks/native/include/binder/Parcel.h                            Parcel
frameworks/native/libs/binder/include/binder/BpBinder.h              BpBinder


//参考  MediaPlayerService 不使用AIDL，直接 h C++ 文件声明 定义
frameworks/av/media/libmedia/include/media/IMediaPlayerService.h
参考frameworks/av/media/libmedia/IMediaPlayerService.cpp
参考frameworks/av/media/libmediaplayerservice/MediaPlayerService.cpp
参考frameworks/av/media/libmediaplayerservice/MediaPlayerService.h
参考frameworks/av/media/mediaserver/main_mediaserver.cpp


//参考 ServiceManager
1、不使用AIDL，直接 h C++ 文件声明 定义
frameworks/native/libs/binder/IServiceManager.cpp
frameworks/native/libs/binder/include/binder/IServiceManager.h
2、使用AIDL，构建时生成
frameworks/native/libs/binder/aidl/android/os/IServiceManager.aidl
out/soong/.intermediates/frameworks/native/libs/binder/libbinder/android_vendor.30_arm64_armv8-2a_cortex-a55_shared/gen/aidl/android/os/BnServiceManager.h
out/soong/.intermediates/frameworks/native/libs/binder/libbinder/android_vendor.30_arm64_armv8-2a_cortex-a55_shared/gen/aidl/android/os/BpServiceManager.h
out/soong/.intermediates/frameworks/native/libs/binder/libbinder/android_vendor.30_arm64_armv8-2a_cortex-a55_shared/gen/aidl/android/os/IServiceManager.h
out/soong/.intermediates/frameworks/native/libs/binder/libbinder/android_vendor.30_arm64_armv8-2a_cortex-a55_shared/gen/aidl/frameworks/native/libs/binder/aidl/android/os/IServiceManager.cpp
*/


#ifndef ANDROID_IDEMOSERVICE_H
#define ANDROID_IDEMOSERVICE_H

#include <utils/Errors.h>  // for status_t
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <utils/String8.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>


//IDemoService由handle(handle对应着binder底层ipc通信时的进程)生成


namespace android {
	enum {
		//CREATE = IBinder::FIRST_CALL_TRANSACTION,
		//CREATE_MEDIA_RECORDER,
		//ENABLE_FAST_REMOTE_DISPLAY
		
		FUNC1 = IBinder::FIRST_CALL_TRANSACTION,
		FUNC2
	};
	
	class IDemoService: public IInterface
	{
	public:
		DECLARE_META_INTERFACE(DemoService);

#if 0
	#define DECLARE_META_INTERFACE(INTERFACE)                               \
99  public:                                                                 \
100      static const ::android::String16 descriptor;                        \
101      static ::android::sp<I##INTERFACE> asInterface(                     \
102              const ::android::sp<::android::IBinder>& obj);              \
103      virtual const ::android::String16& getInterfaceDescriptor() const;  \
104      I##INTERFACE();                                                     \
105      virtual ~I##INTERFACE();                                            \
106      static bool setDefaultImpl(std::unique_ptr<I##INTERFACE> impl);     \
107      static const std::unique_ptr<I##INTERFACE>& getDefaultImpl();       \
108  private:                                                                \
109      static std::unique_ptr<I##INTERFACE> default_impl;                  \
110  public:                                                                 \
111  
#endif

		virtual void func1(void) = 0;
		virtual int  func2(const char *str) = 0;
	}
	
	
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
	

	//BpInterface<IDemoService> 由 IDemoService 和 BpRefBase 类public派生而出，且加入了一些属性和方法
	//class BpRefBase : public virtual RefBase
	/*class BpDemoService: public BpInterface<IDemoService>
	{
		
		public:
			BpDemoService(const sp<IBinder> &impl);
			virtual void func1(void);
			virtual int  func2(const char *str);
	}*/
}





//class IBinder : public virtual RefBase
//class BpBinder : public IBinder






#endif