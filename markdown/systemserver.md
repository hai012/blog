## systemserver

### 1. 启动过程

frameworks/base/services/java/com/android/server/SystemServer.java

```
66  import android.os.ServiceManager;
...
201  public final class SystemServer {
.....
429      public static void main(String[] args) {
430          new SystemServer().run();
431      }
......
452      private void run() {
......
557              // Initialize native services.
558              System.loadLibrary("android_servers");
......
624              startOtherServices(t);

......
658      }

1055      private void startOtherServices(@NonNull TimingsTraceAndSlog t) {
......
1073          ConsumerIrService consumerIr = null;
......
1185                  t.traceBegin("StartConsumerIrService");
1186                  consumerIr = new ConsumerIrService(context);
1187                  ServiceManager.addService(Context.CONSUMER_IR_SERVICE, consumerIr);
1188                  t.traceEnd();
......
2563      }
...
2775  }
```



### 2. 加载libandroid_servers.so库

frameworks/base/services/Android.bp

```
100 cc_library_shared {
101     name: "libandroid_servers",
102     defaults: ["libservices.core-libs"],
103     whole_static_libs: ["libservices.core"],
104 }
```



frameworks/base/services/core/jni/Android.bp

```
1 cc_library_static {
2     name: "libservices.core",
3     defaults: ["libservices.core-libs"],
4 
5     cpp_std: "c++2a",
6     cflags: [
7         "-Wall",
8         "-Werror",
9         "-Wno-unused-parameter",
10         "-Wthread-safety",
11 
12         "-DEGL_EGLEXT_PROTOTYPES",
13         "-DGL_GLEXT_PROTOTYPES",
14     ],
15 
16     srcs: [
......
30         "com_android_server_ConsumerIrService.cpp",
31         "com_android_server_devicepolicy_CryptoTestHelper.cpp",
......
62         "onload.cpp",
63         ":lib_networkStatsFactory_native",#使用：开头是引用其他模块中定义的一组文件
64     ],
65 
66     include_dirs: [
67         "frameworks/base/libs",
68         "frameworks/native/services",
69         "system/gatekeeper/include",
70     ],
71 
72     header_libs: [
73         "bionic_libc_platform_headers",
74     ],
...
86 }
87 
88 cc_defaults {
89     name: "libservices.core-libs",
90     shared_libs: [
......
125         "libEGL",
126         "libGLESv2",
......
148         "android.hardware.ir@1.0",
149         "android.hardware.light@2.0",
......
169     ],
170 
171     static_libs: [
172         "android.hardware.broadcastradio@common-utils-1x-lib",
173     ],
...
188 }
...
```



frameworks/base/services/core/jni/onload.cpp

```
...
26  namespace android {
27  int register_android_server_AlarmManagerService(JNIEnv* env);
28  int register_android_server_BatteryStatsService(JNIEnv* env);
29  int register_android_server_ConsumerIrService(JNIEnv *env);
30  int register_android_server_InputManager(JNIEnv* env);
31  int register_android_server_LightsService(JNIEnv* env);
32  int register_android_server_PowerManagerService(JNIEnv* env);
...
67  };
68
69  using namespace android;
70  
71  extern "C" jint JNI_OnLoad(JavaVM* vm, void* /* reserved */)
72  {
73      JNIEnv* env = NULL;
74      jint result = -1;
75  
76      if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
77          ALOGE("GetEnv failed!");
78          return result;
79      }
80      ALOG_ASSERT(env, "Could not retrieve the env!");
81  
......
99      register_android_server_devicepolicy_CryptoTestHelper(env);
100     register_android_server_ConsumerIrService(env);
......
124      return JNI_VERSION_1_4;
125  }
```







frameworks/base/services/core/jni/com_android_server_ConsumerIrService.cpp

```
17  #define LOG_TAG "ConsumerIrService"
18  
19  #include "jni.h"
20  #include <nativehelper/JNIHelp.h>
21  #include "android_runtime/AndroidRuntime.h"
22  
23  #include <stdlib.h>
24  #include <utils/misc.h>
25  #include <utils/Log.h>
26  #include <android/hardware/ir/1.0/IConsumerIr.h>
27  #include <nativehelper/ScopedPrimitiveArray.h>
28  
29  using ::android::hardware::ir::V1_0::IConsumerIr;
30  using ::android::hardware::ir::V1_0::ConsumerIrFreqRange;
31  using ::android::hardware::hidl_vec;
32  
33  namespace android {
34  
35  static sp<IConsumerIr> mHal;
36  
37  static jboolean halOpen(JNIEnv* /* env */, jobject /* obj */) {
38      // TODO(b/31632518)
39      mHal = IConsumerIr::getService();//获得BpHwIConsumerIr对象
40      return mHal != nullptr;
41  }
42  
43  static jint halTransmit(JNIEnv *env, jobject /* obj */, jint carrierFrequency,
44     jintArray pattern) {
45      ScopedIntArrayRO cPattern(env, pattern);
46      if (cPattern.get() == NULL) {
47          return -EINVAL;
48      }
49      hidl_vec<int32_t> patternVec;
50      patternVec.setToExternal(const_cast<int32_t*>(cPattern.get()), cPattern.size());
51      //通过hwbinder通信让hal层的android.hardware.ir@1.0-service进程调用transmit接口，然后返回结果过来
52      bool success = mHal->transmit(carrierFrequency, patternVec);
53      return success ? 0 : -1;
54  }
55  
56  static jintArray halGetCarrierFrequencies(JNIEnv *env, jobject /* obj */) {
57      int len;
58      hidl_vec<ConsumerIrFreqRange> ranges;
59      bool success;
60  
61      auto cb = [&](bool s, hidl_vec<ConsumerIrFreqRange> vec) {
62              ranges = vec;
63              success = s;
64      };
65      mHal->getCarrierFreqs(cb);//通过hwbinder通信让hal层的android.hardware.ir@1.0-service进程调用transmit接口，然后返回结果过来
66  
67      if (!success) {
68          return NULL;
69      }
70      len = ranges.size();
71  
72      int i;
73      ScopedIntArrayRW freqsOut(env, env->NewIntArray(len*2));
74      jint *arr = freqsOut.get();
75      if (arr == NULL) {
76          return NULL;
77      }
78      for (i = 0; i < len; i++) {
79          arr[i*2] = static_cast<jint>(ranges[i].min);
80          arr[i*2+1] = static_cast<jint>(ranges[i].max);
81      }
82  
83      return freqsOut.getJavaArray();
84  }
85  
86  static const JNINativeMethod method_table[] = {
87      { "halOpen", "()Z", (void *)halOpen },
88      { "halTransmit", "(I[I)I", (void *)halTransmit },
89      { "halGetCarrierFrequencies", "()[I", (void *)halGetCarrierFrequencies},
90  };
91  
92  int register_android_server_ConsumerIrService(JNIEnv *env) {
93      return jniRegisterNativeMethods(env, "com/android/server/ConsumerIrService",
94              method_table, NELEM(method_table));
95  }
96  
97  };
```



给JAVA虚拟机中的的com.android.server.ConsumerIrService类中注册native方法，在halOpen中获得BpHwIConsumerIr C++对象。在halTransmit中使用BpHwIConsumerIr的transmit方法，transmit方法通过hwbinder通信让hal层的android.hardware.ir@1.0-service进程调用transmit接口，然后返回结果过来。在halGetCarrierFrequencies中使用BpHwIConsumerIr的getCarrierFreqs方法，getCarrierFreqs方法通过hwbinder通信让hal层的android.hardware.ir@1.0-service进程调用getCarrierFreqs接口，然后返回结果过来。





### 3. ConsumerIrService类的实现



frameworks/base/core/java/android/hardware/IConsumerIrService.aidl

```
17  package android.hardware;
18  
19  /** {@hide} */
20  interface IConsumerIrService
21  {
22      boolean hasIrEmitter();
23      void transmit(String packageName, int carrierFrequency, in int[] pattern);
24      int[] getCarrierFrequencies();
25  }
```





frameworks/base/services/core/java/com/android/server/ConsumerIrService.java

```
17  package com.android.server;
18  
19  import android.content.Context;
20  import android.content.pm.PackageManager;
21  import android.hardware.IConsumerIrService;
22  import android.os.PowerManager;
23  import android.util.Slog;
24  
25  import java.lang.RuntimeException;
26  
27  public class ConsumerIrService extends IConsumerIrService.Stub {
28      private static final String TAG = "ConsumerIrService";
29  
30      private static final int MAX_XMIT_TIME = 2000000; /* in microseconds */
31 //native方法在frameworks/base/services/java/com/android/server/SystemServer.java的558行System.loadLibrary("android_servers")注册
32      private static native boolean halOpen();
33      private static native int halTransmit(int carrierFrequency, int[] pattern);
34      private static native int[] halGetCarrierFrequencies();
35  
36      private final Context mContext;
37      private final PowerManager.WakeLock mWakeLock;
38      private final boolean mHasNativeHal;
39      private final Object mHalLock = new Object();
40  
41      ConsumerIrService(Context context) {
42          mContext = context;
43          PowerManager pm = (PowerManager)context.getSystemService(
44                  Context.POWER_SERVICE);
45          mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);
46          mWakeLock.setReferenceCounted(true);
47  
48          mHasNativeHal = halOpen();
49          if (mContext.getPackageManager().hasSystemFeature(PackageManager.FEATURE_CONSUMER_IR)) {
50              if (!mHasNativeHal) {
51                  throw new RuntimeException("FEATURE_CONSUMER_IR present, but no IR HAL loaded!");
52              }
53          } else if (mHasNativeHal) {
54              throw new RuntimeException("IR HAL present, but FEATURE_CONSUMER_IR is not set!");
55          }
56      }
57  
58      @Override
59      public boolean hasIrEmitter() {
60          return mHasNativeHal;
61      }
62  
63      private void throwIfNoIrEmitter() {
64          if (!mHasNativeHal) {
65              throw new UnsupportedOperationException("IR emitter not available");
66          }
67      }
68  
69  
70      @Override
71      public void transmit(String packageName, int carrierFrequency, int[] pattern) {
72          if (mContext.checkCallingOrSelfPermission(android.Manifest.permission.TRANSMIT_IR)
73                  != PackageManager.PERMISSION_GRANTED) {
74              throw new SecurityException("Requires TRANSMIT_IR permission");
75          }
76  
77          long totalXmitTime = 0;
78  
79          for (int slice : pattern) {
80              if (slice <= 0) {
81                  throw new IllegalArgumentException("Non-positive IR slice");
82              }
83              totalXmitTime += slice;
84          }
85  
86          if (totalXmitTime > MAX_XMIT_TIME ) {
87              throw new IllegalArgumentException("IR pattern too long");
88          }
89  
90          throwIfNoIrEmitter();
91  
92          // Right now there is no mechanism to ensure fair queing of IR requests
93          synchronized (mHalLock) {
94              int err = halTransmit(carrierFrequency, pattern);
95  
96              if (err < 0) {
97                  Slog.e(TAG, "Error transmitting: " + err);
98              }
99          }
100      }
101  
102      @Override
103      public int[] getCarrierFrequencies() {
104          if (mContext.checkCallingOrSelfPermission(android.Manifest.permission.TRANSMIT_IR)
105                  != PackageManager.PERMISSION_GRANTED) {
106              throw new SecurityException("Requires TRANSMIT_IR permission");
107          }
108  
109          throwIfNoIrEmitter();
110  
111          synchronized(mHalLock) {
112              return halGetCarrierFrequencies();
113          }
114      }
115  }
```



out/soong/.intermediates/frameworks/base/framework-doc-stubs/android_common/srcjars/frameworks/base/core/java/android/hardware/IConsumerIrService.java



```

/*
 * This file is auto-generated.  DO NOT MODIFY.
 */
package android.hardware;
/** {@hide} */
public interface IConsumerIrService extends android.os.IInterface
{
  /** Default implementation for IConsumerIrService. */
  public static class Default implements android.hardware.IConsumerIrService
  {
    @Override public boolean hasIrEmitter() throws android.os.RemoteException
    {
      return false;
    }
    @Override public void transmit(java.lang.String packageName, int carrierFrequency, int[] pattern) throws android.os.RemoteException
    {
    }
    @Override public int[] getCarrierFrequencies() throws android.os.RemoteException
    {
      return null;
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements android.hardware.IConsumerIrService
  {
    private static final java.lang.String DESCRIPTOR = "android.hardware.IConsumerIrService";
    /** Construct the stub at attach it to the interface. */
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an android.hardware.IConsumerIrService interface,
     * generating a proxy if needed.
     */
    public static android.hardware.IConsumerIrService asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof android.hardware.IConsumerIrService))) {
        return ((android.hardware.IConsumerIrService)iin);
      }
      return new android.hardware.IConsumerIrService.Stub.Proxy(obj);
    }
    @Override public android.os.IBinder asBinder()
    {
      return this;
    }
    @Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
    {
      java.lang.String descriptor = DESCRIPTOR;
      switch (code)
      {
        case INTERFACE_TRANSACTION:
        {
          reply.writeString(descriptor);
          return true;
        }
        case TRANSACTION_hasIrEmitter:
        {
          data.enforceInterface(descriptor);
          boolean _result = this.hasIrEmitter();
          reply.writeNoException();
          reply.writeInt(((_result)?(1):(0)));
          return true;
        }
        case TRANSACTION_transmit:
        {
          data.enforceInterface(descriptor);
          java.lang.String _arg0;
          _arg0 = data.readString();
          int _arg1;
          _arg1 = data.readInt();
          int[] _arg2;
          _arg2 = data.createIntArray();
          this.transmit(_arg0, _arg1, _arg2);
          reply.writeNoException();
          return true;
        }
        case TRANSACTION_getCarrierFrequencies:
        {
          data.enforceInterface(descriptor);
          int[] _result = this.getCarrierFrequencies();
          reply.writeNoException();
          reply.writeIntArray(_result);
          return true;
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
    }
    private static class Proxy implements android.hardware.IConsumerIrService
    {
      private android.os.IBinder mRemote;
      Proxy(android.os.IBinder remote)
      {
        mRemote = remote;
      }
      @Override public android.os.IBinder asBinder()
      {
        return mRemote;
      }
      public java.lang.String getInterfaceDescriptor()
      {
        return DESCRIPTOR;
      }
      @Override public boolean hasIrEmitter() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_hasIrEmitter, _data, _reply, 0);
          if (!_status && getDefaultImpl() != null) {
            return getDefaultImpl().hasIrEmitter();
          }
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      @Override public void transmit(java.lang.String packageName, int carrierFrequency, int[] pattern) throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          _data.writeString(packageName);
          _data.writeInt(carrierFrequency);
          _data.writeIntArray(pattern);
          boolean _status = mRemote.transact(Stub.TRANSACTION_transmit, _data, _reply, 0);
          if (!_status && getDefaultImpl() != null) {
            getDefaultImpl().transmit(packageName, carrierFrequency, pattern);
            return;
          }
          _reply.readException();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
      }
      @Override public int[] getCarrierFrequencies() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        int[] _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_getCarrierFrequencies, _data, _reply, 0);
          if (!_status && getDefaultImpl() != null) {
            return getDefaultImpl().getCarrierFrequencies();
          }
          _reply.readException();
          _result = _reply.createIntArray();
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
      public static android.hardware.IConsumerIrService sDefaultImpl;
    }
    static final int TRANSACTION_hasIrEmitter = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
    static final int TRANSACTION_transmit = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
    static final int TRANSACTION_getCarrierFrequencies = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
    public static boolean setDefaultImpl(android.hardware.IConsumerIrService impl) {
      // Only one user of this interface can use this function
      // at a time. This is a heuristic to detect if two different
      // users in the same process use this function.
      if (Stub.Proxy.sDefaultImpl != null) {
        throw new IllegalStateException("setDefaultImpl() called twice");
      }
      if (impl != null) {
        Stub.Proxy.sDefaultImpl = impl;
        return true;
      }
      return false;
    }
    public static android.hardware.IConsumerIrService getDefaultImpl() {
      return Stub.Proxy.sDefaultImpl;
    }
  }
  public boolean hasIrEmitter() throws android.os.RemoteException;
  public void transmit(java.lang.String packageName, int carrierFrequency, int[] pattern) throws android.os.RemoteException;
  public int[] getCarrierFrequencies() throws android.os.RemoteException;
}
```

