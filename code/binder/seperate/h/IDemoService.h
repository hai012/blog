

#ifndef ANDROID_IDEMOSERVICE_H
#define ANDROID_IDEMOSERVICE_H

//以下头文件来自C++标准库，头文件放在交叉编译工具链的路径中
#include <bits/unique_ptr.h>//提供 class unique_ptr<>  模板类 的声明

//以下是安卓系统提供的头文件
#include <binder/IInterface.h>//提供  IInterface 类 的声明
#include <utils/String16.h>//提供 class String16 类 的声明
#include <utils/StrongPointer.h>//提供 class sp<> 模板类 的声明
#include <binder/IBinder.h>//提供 class IBinder 类 的声明


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
		//使用宏声明一些方法
		DECLARE_META_INTERFACE(DemoService);

		virtual void func1(void) = 0;
		virtual int  func2(const char *str) = 0;
	}
}
#endif


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