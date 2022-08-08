
//以下是安卓系统提供的头文件
#include <binder/IInterface.h>//提供  IMPLEMENT_META_INTERFACE  的定义


/*
#include <utils/String16.h>//提供  String16 类 StaticString16 的声明
#include <utils/StrongPointer.h>//提供  sp<> 模板类 的声明
#include <binder/IBinder.h>//提供  IBinder 类 的声明

static_cast<> 是C++中的强制转换
把一个类型的变量a强制赋值给另外类型的变量b，如果能成功，变量b就具有一个值，如果不能成功就抛出一个错误。
C++共有四种转换:static_cast,dynamic_cast,reinterpret_cast,const_cast
*/


//以下是本模块内部提供的头文件
#include "BpDemoService.h"//IDemoService类的这些方法的实现依赖于  BpDemoService 类的声明
#include "IDemoService.h"

namespace android {

	//使用宏来实现IDemoService类的一些方法
	IMPLEMENT_META_INTERFACE(DemoService, "android.media.IDemoService");

}


#if 0
115  #ifndef DO_NOT_CHECK_MANUAL_BINDER_INTERFACES
116  
117  #define IMPLEMENT_META_INTERFACE(INTERFACE, NAME)                       \
118      static_assert(internal::allowedManualInterface(NAME),               \
119                    "b/64223827: Manually written binder interfaces are " \
120                    "considered error prone and frequently have bugs. "   \
121                    "The preferred way to add interfaces is to define "   \
122                    "an .aidl file to auto-generate the interface. If "   \
123                    "an interface must be manually written, add its "     \
124                    "name to the whitelist.");                            \
125      DO_NOT_DIRECTLY_USE_ME_IMPLEMENT_META_INTERFACE(INTERFACE, NAME)    \
126  
127  #else
128  
129  #define IMPLEMENT_META_INTERFACE(INTERFACE, NAME)                       \
130      DO_NOT_DIRECTLY_USE_ME_IMPLEMENT_META_INTERFACE(INTERFACE, NAME)    \
131  
132  #endif

     #define DO_NOT_DIRECTLY_USE_ME_IMPLEMENT_META_INTERFACE(INTERFACE, NAME)\
135      const ::android::StaticString16                                     \
136          IDemoService_descriptor_static_str16(__IINTF_CONCAT(u, NAME));\
137      const ::android::String16 IDemoService::descriptor(                 \
138          IDemoService_descriptor_static_str16);                        \
139      const ::android::String16&                                          \
140              IDemoService::getInterfaceDescriptor() const {              \
141          return IDemoService::descriptor;                                \
142      }                                                                   \
143      ::android::sp<IDemoService> IDemoService::asInterface(              \
144              const ::android::sp<::android::IBinder>& obj)               \
145      {                                                                   \
146          ::android::sp<IDemoService> intr;                               \
147          if (obj != nullptr) {                                           \
148              intr = static_cast<IDemoService*>(                          \
149                  obj->queryLocalInterface(                               \
150                          IDemoService::descriptor).get());               \
151              if (intr == nullptr) {                                      \
152                  intr = new BpDemoService(obj);                          \
153              }                                                           \
154          }                                                               \
155          return intr;                                                    \
156      }                                                                   \
157      std::unique_ptr<IDemoService> IDemoService::default_impl;           \
158      bool IDemoService::setDefaultImpl(std::unique_ptr<IDemoService> impl)\
159      {                                                                   \
160          /* Only one user of this interface can use this function     */ \
161          /* at a time. This is a heuristic to detect if two different */ \
162          /* users in the same process use this function.              */ \
163          assert(!IDemoService::default_impl);                            \
164          if (impl) {                                                     \
165              IDemoService::default_impl = std::move(impl);               \
166              return true;                                                \
167          }                                                               \
168          return false;                                                   \
169      }                                                                   \
170      const std::unique_ptr<IDemoService>& IDemoService::getDefaultImpl() \
171      {                                                                   \
172          return IDemoService::default_impl;                              \
173      }                                                                   \
174      IDemoService::IDemoService() { }                                    \
175      IDemoService::~IDemoService() { }                                   \
176  
177  

#endif

