1. app进程

### 1.1使用WifiManager



```
WifiManager工具类：
android包中自带有WifiManager工具类，专门用于wifi管理：

import android.net.wifi.WifiManager;

取得WifiManager对象：
WifiManager mWifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
打开WIFI：
if (!mWifiManager.isWifiEnabled()) {
	mWifiManager.setWifiEnabled(true);
}
创建一个WIFI信息：
public WifiConfiguration CreateWifiInfo(String SSID, String Password,
			int Type) {
		WifiConfiguration config = new WifiConfiguration();
		config.allowedAuthAlgorithms.clear();
		config.allowedGroupCiphers.clear();
		config.allowedKeyManagement.clear();
		config.allowedPairwiseCiphers.clear();
		config.allowedProtocols.clear();
		config.SSID = "\"" + SSID + "\"";

		WifiConfiguration tempConfig = this.IsExsits(SSID);
		if (tempConfig != null) {
			mWifiManager.removeNetwork(tempConfig.networkId);
		}

		if (Type == 1) // WIFICIPHER_NOPASS
		{
			config.wepKeys[0] = "";
			config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
			config.wepTxKeyIndex = 0;
		}
		if (Type == 2) // WIFICIPHER_WEP
		{
			config.hiddenSSID = true;
			config.wepKeys[0] = "\"" + Password + "\"";
			config.allowedAuthAlgorithms
					.set(WifiConfiguration.AuthAlgorithm.SHARED);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
			config.allowedGroupCiphers
					.set(WifiConfiguration.GroupCipher.WEP104);
			config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
			config.wepTxKeyIndex = 0;
		}
		if (Type == 3) // WIFICIPHER_WPA
		{
			config.preSharedKey = "\"" + Password + "\"";
			config.hiddenSSID = true;
			config.allowedAuthAlgorithms
					.set(WifiConfiguration.AuthAlgorithm.OPEN);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
			config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
			config.allowedPairwiseCiphers
					.set(WifiConfiguration.PairwiseCipher.TKIP);
			// config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
			config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
			config.allowedPairwiseCiphers
					.set(WifiConfiguration.PairwiseCipher.CCMP);
			config.status = WifiConfiguration.Status.ENABLED;
		}
		return config;
	}
这里只介绍第三个参数：Type。从代码中可以看出，Type有三个值，分别为1，2，3。WIFI热点是有加密的，加密方式包括：不加密，WEP，WPA三种，1、2、3就分别对应这三种加密方式，方法返回一个WIFI热点信息。

添加一个网络并连接：
public void addNetwork(WifiConfiguration wcg) {
		int wcgID = mWifiManager.addNetwork(wcg);
		boolean b = mWifiManager.enableNetwork(wcgID, true);
		System.out.println("a--" + wcgID);
		System.out.println("b--" + b);
	}
    具体的使用方式如下：

mWifiAdmin.addNetwork(mWifiAdmin.CreateWifiInfo(SSID, password, 3));

判断WIFI是否连接成功：
public int isWifiContected(Context context) {
		ConnectivityManager connectivityManager = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo wifiNetworkInfo = connectivityManager
				.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
		
		Log.v(TAG, "isConnectedOrConnecting = " + wifiNetworkInfo.isConnectedOrConnecting());
		Log.d(TAG, "wifiNetworkInfo.getDetailedState() = " + wifiNetworkInfo.getDetailedState());
		if (wifiNetworkInfo.getDetailedState() == DetailedState.OBTAINING_IPADDR
				|| wifiNetworkInfo.getDetailedState() == DetailedState.CONNECTING) {
			return WIFI_CONNECTING;
		} else if (wifiNetworkInfo.getDetailedState() == DetailedState.CONNECTED) {
			return WIFI_CONNECTED;
		} else {
			Log.d(TAG, "getDetailedState() == " + wifiNetworkInfo.getDetailedState());
			return WIFI_CONNECT_FAILED;
		}
	}
关闭WIFI连接：
if (mWifiManager.isWifiEnabled()) {
	mWifiManager.setWifiEnabled(false);
}



开始WIFI扫描
mWifiManager.startScan()
获取WIFI扫描结果
mWifiManager.getWifiState()


获取当前已连接的wifi信息

WifiInfo info = mWifiManager.getConnectionInfo();
几乎和WifiConfiguration一样的，但是如果当前没有连接wifi的话，就会返回Null，它包括了SSID、networkId、BSSID的，切记一个问题：它的SSID是带双引号的，这点和ScanResult对象不一样


在高版本的安卓，普通APP无权限通过
mWifiManager.setWifiEnabled(true/false)来开启关闭wifi

也无权限在wifi开启的状态通过如下连接网络
WifiConfiguration config
config.xxx=
int networkid = mWifiManager.addNetwork(config);
mWifiManager.enableNetwork(networkid, true);

普通APP应该在通过使用intent打开Settings.Panel.ACTION_WIFI activity,
Intent panelIntent = new Intent(android.provider.Settings.Panel.ACTION_WIFI);
startActivityForResult(panelIntent,1);
参考https://developer.android.google.cn/about/versions/10/features#settings-panels
然后待用户在Settings.Panel.ACTION_WIFI activity中来打开关闭连接wifi
```







Activity  Service   BroadcastReceiver 



一个BroadcastReceiver 可注册多个intent action，action对应的每种intent 里面具有不同的extra data



WifiManager.WIFI_STATE_CHANGED_ACTION

```
"android.net.wifi.WIFI_STATE_CHANGED"
```





WifiManager.NETWORK_STATE_CHANGED_ACTION

```
"android.net.wifi.STATE_CHANGE"
此action的广播携带两个额外数据:
public class NetworkConnectChangedReceiver extends BroadcastReceiver{  
    @Override  
    public void onReceive(Context context, Intent intent) {  
    if (WifiManager.WIFI_STATE_CHANGED_ACTION.equals(intent.getAction())) {//这个监听wifi的打开与关闭，与wifi的连接无关  
            int wifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, 0);   
            LogTag.showTAG_e("WIFI状态", "wifiState"+wifiState);  
            switch (wifiState) {   
            case WifiManager.WIFI_STATE_DISABLED:   
                break;   
            case WifiManager.WIFI_STATE_DISABLING:   
                break;   
           //  
            }   
        }  
    // 这个监听wifi的连接状态即是否连上了一个有效无线路由，当上边广播的状态是WifiManager.WIFI_STATE_DISABLING，和WIFI_STATE_DISABLED的时候，根本不会接到这个广播。  
    // 在上边广播接到广播是WifiManager.WIFI_STATE_ENABLED状态的同时也会接到这个广播，当然刚打开wifi肯定还没有连接到有效的无线  
    if (WifiManager.NETWORK_STATE_CHANGED_ACTION.equals(intent.getAction())) {  
            Parcelable parcelableExtra = intent.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);    
            if (null != parcelableExtra) {    
                NetworkInfo networkInfo = (NetworkInfo) parcelableExtra;   
                
                
                State          state         = networkInfo.getState();  
                boolean isConnected = state==networkInfo.State.CONNECTED;//当然，这边可以更精确的确定状态  
                
                
                DetailedState  detailedState = networkInfo.getDetailedState();
                boolean isConnected = detailedState==networkInfo.DetailedState.CONNECTED;
                
                
            }    
        }  
}
state和DetailedState是networkInfo中的枚举变量，具体取值如下：


75      public enum State {
76          CONNECTING, CONNECTED, SUSPENDED, DISCONNECTING, DISCONNECTED, UNKNOWN
77      }
78  
79      /**
80       * The fine-grained state of a network connection. This level of detail
81       * is probably of interest to few applications. Most should use
82       * {@link android.net.NetworkInfo.State State} instead.
83       *
84       * @deprecated See {@link NetworkInfo}.
85       */
86      @Deprecated
87      public enum DetailedState {
88          /** Ready to start data connection setup. */
89          IDLE,
90          /** Searching for an available access point. */
91          SCANNING,
92          /** Currently setting up data connection. */
93          CONNECTING,
94          /** Network link established, performing authentication. */
95          AUTHENTICATING,
96          /** Awaiting response from DHCP server in order to assign IP address information. */
97          OBTAINING_IPADDR,
98          /** IP traffic should be available. */
99          CONNECTED,
100          /** IP traffic is suspended */
101          SUSPENDED,
102          /** Currently tearing down data connection. */
103          DISCONNECTING,
104          /** IP traffic not available. */
105          DISCONNECTED,
106          /** Attempt to connect failed. */
107          FAILED,
108          /** Access to this network is blocked. */
109          BLOCKED,
110          /** Link has poor connectivity. */
111          VERIFYING_POOR_LINK,
112          /** Checking if network is a captive portal */
113          CAPTIVE_PORTAL_CHECK
114      }



networkinfo还有gettype方法判断当前网络是数据流量还是wifi
```



API29(android10)以后NetworkInfo被废弃，使用ConnectivityManager.NetworkCallback来监听网络变化

https://blog.csdn.net/u012834390/article/details/110136322

https://developer.android.google.cn/reference/android/net/ConnectivityManager.NetworkCallback?hl=en

使用 `ConnectivityManager#getNetworkCapabilities` or `ConnectivityManager#getLinkProperties`来判断是数据流量还是wifi。







WifiManager.SUPPLICANT_STATE_CHANGED_ACTION

```
"android.net.wifi.supplicant.STATE_CHANGE"
```



WifiManager.WIFI_CREDENTIAL_CHANGED_ACTION

```
"android.net.wifi.WIFI_CREDENTIAL_CHANGED"
```



WifiManager.WIFI_AP_STATE_CHANGED_ACTION

```
"android.net.wifi.WIFI_AP_STATE_CHANGED"
```



WifiManager.CONFIGURED_NETWORKS_CHANGED_ACTION

```
"android.net.wifi.CONFIGURED_NETWORKS_CHANGE"
```



WifiManager.RSSI_CHANGED_ACTION

```
"android.net.wifi.RSSI_CHANGED"
```





WifiManager.LINK_CONFIGURATION_CHANGED_ACTION

```
"android.net.wifi.LINK_CONFIGURATION_CHANGED"
```



NETWORK_IDS_CHANGED_ACTION

```
"android.net.wifi.NETWORK_IDS_CHANGED"
```





### 2.获取IWifiManager的AIDL proxy端接口



![image-20220105174458632](wifi.assets/image-20220105174458632.png)



APP 进程创建完成后进程中有一个Application对象，若干个Activity和Service对象。他们都直接或间接派生自ContextWrapper类。

ContextWrapper类和ContextIml类都实现了Context接口。在ContextWrapper对象(Application对象、Activity和Service对象都是ContextWrapper对象)中有一个ContextIml对象。调用ContextWrapper对象Context接口中的各种方法时，这些方法内部就调用ContextWrapper对象中的ContextIml对象相应的Context接口。



ContextWrapper对象与ContextIml对象是一对一还是多对一还是多对多还需后续看代码。目前还不确定



ContextIml类中使用SystemServiceRegistry类，SystemServiceRegistry类在静态初始化块中调用了WifiFrameworkInitializer.registerServiceWrappers():

frameworks/base/core/java/android/app/SystemServiceRegistry.java

```
public final class SystemServiceRegistry {
...
static {
...
WifiFrameworkInitializer.registerServiceWrappers();
...
}
...
}
```





在WifiFrameworkInitializer.registerServiceWrappers方法中调用SystemServiceRegistry.registerContextAwareService将new出来的各种WifiXXXManager对象注册到SystemServiceRegistry类的哈希表中，new各种WifiXXXManager对象时需要传入一个对应的IWifiXXXManager对象，即binder通信的 proxy 对象：

frameworks/base/wifi/java/android/net/wifi/WifiFrameworkInitializer.java

![image-20220105160644640](wifi.assets/image-20220105160644640.png)





ContextWrapper对象的getSystemService方法直接调用其内部ContextIml对象的getSystemService方法，参考

frameworks/base/core/java/android/app/ContextImpl.java



![image-20220105184950143](wifi.assets/image-20220105184950143.png)



ContextIml对象的getSystemService中调用了SystemServiceRegistry类的getSystemService方法将先前注册到哈希表中的WifiXXXManager对象取出，之后使用WifiXXXManager对象就能进行wifi相关的操作。



取出的WifiXXXManager对象是其内部IWifiXXXManager类的wrapper，调用WifiXXXManager的各种方法最终都会调用到其内部IWifiXXXManager类的方法，而IWifiXXXManager类是AIDL的proxy端，最终通过binder通信调到framework中具体负责实现的服务。





```

72          SystemServiceRegistry.registerContextAwareService(
73                  Context.WIFI_SERVICE,
74                  WifiManager.class,
75                  (context, serviceBinder) -> {
76                      IWifiManager service = IWifiManager.Stub.asInterface(serviceBinder);
77                      return new WifiManager(context, service, getInstanceLooper());
78                  }
79          );
80          SystemServiceRegistry.registerStaticService(
81                  Context.WIFI_P2P_SERVICE,
82                  WifiP2pManager.class,
83                  serviceBinder -> {
84                      IWifiP2pManager service = IWifiP2pManager.Stub.asInterface(serviceBinder);
85                      return new WifiP2pManager(service);
86                  }
87          );
88          SystemServiceRegistry.registerContextAwareService(
89                  Context.WIFI_AWARE_SERVICE,
90                  WifiAwareManager.class,
91                  (context, serviceBinder) -> {
92                      IWifiAwareManager service = IWifiAwareManager.Stub.asInterface(serviceBinder);
93                      return new WifiAwareManager(context, service);
94                  }
95          );
96          SystemServiceRegistry.registerContextAwareService(
97                  Context.WIFI_SCANNING_SERVICE,
98                  WifiScanner.class,
99                  (context, serviceBinder) -> {
100                      IWifiScanner service = IWifiScanner.Stub.asInterface(serviceBinder);
101                      return new WifiScanner(context, service, getInstanceLooper());
102                  }
103          );
104          SystemServiceRegistry.registerContextAwareService(
105                  Context.WIFI_RTT_RANGING_SERVICE,
106                  WifiRttManager.class,
107                  (context, serviceBinder) -> {
108                      IWifiRttManager service = IWifiRttManager.Stub.asInterface(serviceBinder);
109                      return new WifiRttManager(context, service);
110                  }
111          );
112          SystemServiceRegistry.registerContextAwareService(
113                  Context.WIFI_RTT_SERVICE,
114                  RttManager.class,
115                  context -> {
116                      WifiRttManager wifiRttManager = context.getSystemService(WifiRttManager.class);
117                      return new RttManager(context, wifiRttManager);
118                  }
119          );


RttManager使用wifiRttManager，wifiRttManager使用IWifiRttManager，IWifiRttManager是binder proxy端



wifi涉及的主要AIDL接口
IWifiManager
IWifiP2pManager
IWifiAwareManager
IWifiRttManager
```



## 2. framework层服务进程

### WifiService服务注册

system_server调用startOtherServices创建AIDL服务时会创建WifiService服务对象并注册到systemManager。

```
private void startOtherServices(@NonNull TimingsTraceAndSlog t) {
......
248      private static final String WIFI_SERVICE_CLASS =
249              "com.android.server.wifi.WifiService";
......
1182              if (!isWatch) {
1183                  t.traceBegin("StartConsumerIrService");
1184                  consumerIr = new ConsumerIrService(context);
1185                  ServiceManager.addService(Context.CONSUMER_IR_SERVICE, consumerIr);
1186                  t.traceEnd();
1187              }
......
1535              if (context.getPackageManager().hasSystemFeature(
1536                      PackageManager.FEATURE_WIFI)) {
1537                  // Wifi Service must be started first for wifi-related services.
1538                  t.traceBegin("StartWifi");
1539                  mSystemServiceManager.startService(WIFI_SERVICE_CLASS);
1540                  t.traceEnd();
1541                  t.traceBegin("StartWifiScanning");
1542                  mSystemServiceManager.startService(
1543                          "com.android.server.wifi.scanner.WifiScanningService");
1544                  t.traceEnd();
1545              }
......
}
```



***WifiService对象在构造时首先分别new出来WifiContext、WifiInjector、WifiAsyncChannel对象，然后把三个对象作为参数new 了一个WifiServiceImpl对象***，其实现如下：

frameworks/opt/net/wifi/service/java/com/android/server/wifi/WifiService.java

```
34  public final class WifiService extends SystemService {
35  
36      private static final String TAG = "WifiService";
37      // Notification channels used by the wifi service.
38      public static final String NOTIFICATION_NETWORK_STATUS = "NETWORK_STATUS";
39      public static final String NOTIFICATION_NETWORK_ALERTS = "NETWORK_ALERTS";
40      public static final String NOTIFICATION_NETWORK_AVAILABLE = "NETWORK_AVAILABLE";
41  
42      private final WifiServiceImpl mImpl;
43      private final WifiContext mWifiContext;
44  
45      public WifiService(Context contextBase) {
46          super(contextBase);
47          mWifiContext = new WifiContext(contextBase);
48          WifiInjector injector = new WifiInjector(mWifiContext);
49          WifiAsyncChannel channel =  new WifiAsyncChannel(TAG);
50          mImpl = new WifiServiceImpl(mWifiContext, injector, channel);
51      }
       ...
}
```



### WifiInjector对象构造



new 一个SupplicantStaIfaceHal对象，然后将其作为参数new一个WifiNative对象。

```
public class WifiInjector {

99      private final WifiNative mWifiNative;

103      private final SupplicantStaIfaceHal mSupplicantStaIfaceHal;

172      public WifiInjector(WifiContext context) {

232          mSupplicantStaIfaceHal = new SupplicantStaIfaceHal(
233                  mContext, mWifiMonitor, mFrameworkFacade, wifiHandler, mClock, mWifiMetrics);
234          mHostapdHal = new HostapdHal(mContext, wifiHandler);
235          mWifiCondManager = (WifiNl80211Manager) mContext.getSystemService(
236                  Context.WIFI_NL80211_SERVICE);
237          mWifiNative = new WifiNative(
238                  mWifiVendorHal, mSupplicantStaIfaceHal, mHostapdHal, mWifiCondManager,
239                  mWifiMonitor, mPropertyService, mWifiMetrics,
240                  wifiHandler, new Random(), this);

389      }

}
```



### WifiNative对象

构造时将传入的SupplicantStaIfaceHal对象赋给内部成员变量mSupplicantStaIfaceHal。

![image-20220304142117993](wifi.assets/image-20220304142117993.png)



WifiNative的setupInterfaceForClientInScanMode方法

frameworks/opt/net/wifi/service/java/com/android/server/wifi/WifiNative.java



![image-20220304142638288](wifi.assets/image-20220304142638288.png)



WifiNative的startHal方法



WifiNative的startSupplicant方法



SupplicantStaIfaceHal的setupIface方法

frameworks/opt/net/wifi/service/java/com/android/server/wifi/SupplicantStaIfaceHal.java

![image-20220304143459565](wifi.assets/image-20220304143459565.png)

SupplicantStaIfaceHal的setupIface方法



![image-20220304143916342](wifi.assets/image-20220304143916342.png)



### WifiServiceImpl对象



frameworks/opt/net/wifi/service/java/com/android/server/wifi/WifiServiceImpl.java

```
public class WifiServiceImpl extends BaseWifiService {
...
294      public WifiServiceImpl(Context context, WifiInjector wifiInjector, AsyncChannel asyncChannel) {
295          mContext = context;
296          mWifiInjector = wifiInjector;
297          mClock = wifiInjector.getClock();
298  
299          mFacade = mWifiInjector.getFrameworkFacade();
300          mWifiMetrics = mWifiInjector.getWifiMetrics();
301          mWifiTrafficPoller = mWifiInjector.getWifiTrafficPoller();
302          mUserManager = mWifiInjector.getUserManager();
303          mCountryCode = mWifiInjector.getWifiCountryCode();
304          mClientModeImpl = mWifiInjector.getClientModeImpl();
305          mActiveModeWarden = mWifiInjector.getActiveModeWarden();
306          mScanRequestProxy = mWifiInjector.getScanRequestProxy();
307          mSettingsStore = mWifiInjector.getWifiSettingsStore();
308          mPowerManager = mContext.getSystemService(PowerManager.class);
309          mAppOps = (AppOpsManager) mContext.getSystemService(Context.APP_OPS_SERVICE);
310          mWifiLockManager = mWifiInjector.getWifiLockManager();
311          mWifiMulticastLockManager = mWifiInjector.getWifiMulticastLockManager();
312          mClientModeImplHandler = new ClientModeImplHandler(TAG,
313                  mWifiInjector.getAsyncChannelHandlerThread().getLooper(), asyncChannel);
314          mWifiBackupRestore = mWifiInjector.getWifiBackupRestore();
315          mSoftApBackupRestore = mWifiInjector.getSoftApBackupRestore();
316          mWifiApConfigStore = mWifiInjector.getWifiApConfigStore();
317          mWifiPermissionsUtil = mWifiInjector.getWifiPermissionsUtil();
318          mLog = mWifiInjector.makeLog(TAG);
319          mFrameworkFacade = wifiInjector.getFrameworkFacade();
320          mTetheredSoftApTracker = new TetheredSoftApTracker();
321          mActiveModeWarden.registerSoftApCallback(mTetheredSoftApTracker);
322          mLohsSoftApTracker = new LohsSoftApTracker();
323          mActiveModeWarden.registerLohsCallback(mLohsSoftApTracker);
324          mWifiNetworkSuggestionsManager = mWifiInjector.getWifiNetworkSuggestionsManager();
325          mDppManager = mWifiInjector.getDppManager();
326          mWifiThreadRunner = mWifiInjector.getWifiThreadRunner();
327          mWifiConfigManager = mWifiInjector.getWifiConfigManager();
328          mPasspointManager = mWifiInjector.getPasspointManager();
329          mWifiScoreCard = mWifiInjector.getWifiScoreCard();
330          mMemoryStoreImpl = new MemoryStoreImpl(mContext, mWifiInjector,
331                  mWifiScoreCard,  mWifiInjector.getWifiHealthMonitor());
332      }
...
}
```



frameworks/opt/net/wifi/service/java/com/android/server/wifi/BaseWifiService.java

```
public class BaseWifiService extends IWifiManager.Stub {

74      public long getSupportedFeatures() {
75          throw new UnsupportedOperationException();
76      }  
...
687      public void stopReconnectAndScan(int index, int period) {
688          throw new UnsupportedOperationException();
689      }
690
}
 

```



WifiService 中new出来的 WifiServiceImpl 对象是IWifiManager  AIDL接口的实现端 。WifiServiceImpl派生自BaseWifiService，BaseWifiService派生自IWifiManager.Stub，在BaseWifiService类中只负责调用各个接口后抛出异常，WifiServiceImpl继承并重写这些方法后才有真正的实现。



从WifiServiceImpl的构造函数来看WifiServiceImpl其实最终通过构造时传进来的WifiInjector干活。



frameworks/opt/net/wifi/service/java/com/android/server/wifi/WifiInjector.java

```
75  public class WifiInjector {
...
172      public WifiInjector(WifiContext context) {
...
232          mSupplicantStaIfaceHal = new SupplicantStaIfaceHal(
233                  mContext, mWifiMonitor, mFrameworkFacade, wifiHandler, mClock, mWifiMetrics);
234          mHostapdHal = new HostapdHal(mContext, wifiHandler);
235          mWifiCondManager = (WifiNl80211Manager) mContext.getSystemService(
236                  Context.WIFI_NL80211_SERVICE);
237          mWifiNative = new WifiNative(
238                  mWifiVendorHal, mSupplicantStaIfaceHal, mHostapdHal, mWifiCondManager,
239                  mWifiMonitor, mPropertyService, mWifiMetrics,
240                  wifiHandler, new Random(), this);
...
287          mWifiConnectivityHelper = new WifiConnectivityHelper(mWifiNative);
...


}
```





在WifiInjector的构造函数中new 了SupplicantStaIfaceHal对象和HostapdHal对象，并将其作为构造参数来new WifiNative对象，然后使用WifiNative对象作为构造参数new出其他各个wfi管理对象：WifiConnectivityHelper、WifiNetworkSelector、PasspointManager、SarManager、WifiDiagnostics、LinkProbeManager、MboOceController、WifiHealthMonitor、ClientModeImpl、ActiveModeWarden、WifiLockManager、DppManager，之后WifiInjector对象的很多方法在WifiServiceImpl中被调用时就在方法中使用各种wifi管理对象，wifi管理对象最终还是通过WifiNative对象使用来使用SupplicantStaIfaceHal对象来跟底层wpa_supplicant交互。





在WifiNative使用SupplicantStaIfaceHal对象将wifi设置成连接模式时调用到了mSupplicantStaIfaceHal的setupIface方法:

frameworks/opt/net/wifi/service/java/com/android/server/wifi/WifiNative.java

```
public class WifiNative {
...
1097      public String setupInterfaceForClientInScanMode(
...
1131              if (!mSupplicantStaIfaceHal.setupIface(iface.name)) {
1132                  Log.e(TAG, "Failed to setup iface in supplicant on " + iface);
1133                  teardownInterface(iface.name);
1134                  mWifiMetrics.incrementNumSetupClientInterfaceFailureDueToSupplicant();
1135                  return null;
1136              }
...
1152      }
...
```



mSupplicantStaIfaceHal的setupIface方法调用addIfaceV1_1(ifaceName)，mSupplicantStaIfaceHal的addIfaceV1_1方法调用getSupplicantMockableV1_1().addInterface()，getSupplicantMockableV1_1调用getSupplicantMockable()，getSupplicantMockable方法调用了ISupplicant.getService()。

frameworks/opt/net/wifi/service/java/com/android/server/wifi/SupplicantStaIfaceHal.java

```
public class SupplicantStaIfaceHal {
...
392      private ISupplicantStaIface setupStaIface(@NonNull String ifaceName,
393              @NonNull ISupplicantIface ifaceHwBinder) throws RemoteException {
394          /* Prepare base type for later cast. */
395          ISupplicantStaIface iface = getStaIfaceMockable(ifaceHwBinder);
396  
397          /* try newer version first. */
398          if (trySetupStaIfaceV1_1(ifaceName, iface)) {
399              logd("Newer HAL is found, skip V1_0 remaining init flow.");
400              return iface;
401          }
402  
403          SupplicantStaIfaceHalCallback callback = new SupplicantStaIfaceHalCallback(ifaceName);
404          if (!registerCallback(iface, callback)) {
405              throw new RemoteException("Init StaIface V1_0 failed.");
406          }
407          /* keep this in a store to avoid recycling by garbage collector. */
408          mISupplicantStaIfaceCallbacks.put(ifaceName, callback);
409          return iface;
410      }



418      public boolean setupIface(@NonNull String ifaceName) {
419          final String methodStr = "setupIface";
420          if (checkSupplicantStaIfaceAndLogFailure(ifaceName, methodStr) != null) return false;
421          ISupplicantIface ifaceHwBinder;
422  
423          if (isV1_1()) {
424              ifaceHwBinder = addIfaceV1_1(ifaceName);
425          } else {
426              ifaceHwBinder = getIfaceV1_0(ifaceName);
427          }
428          if (ifaceHwBinder == null) {
429              Log.e(TAG, "setupIface got null iface");
430              return false;
431          }
432  
433          try {
434              ISupplicantStaIface iface = setupStaIface(ifaceName, ifaceHwBinder);
435              mISupplicantStaIfaces.put(ifaceName, iface);
436              com.mediatek.server.wifi.MtkWapi.setupMtkIface(ifaceName);
437              com.mediatek.server.wifi.MtkSupplicantStaIfaceHal.setupMtkIface(ifaceName);
438          } catch (RemoteException e) {
439              loge("setup StaIface failed: " + e.toString());
440              return false;
441          }
442  
443          return true;
444      }


508      private ISupplicantIface addIfaceV1_1(@NonNull String ifaceName) {
509          synchronized (mLock) {
510              ISupplicant.IfaceInfo ifaceInfo = new ISupplicant.IfaceInfo();
511              ifaceInfo.name = ifaceName;
512              ifaceInfo.type = IfaceType.STA;
513              Mutable<ISupplicantIface> supplicantIface = new Mutable<>();
514              try {
515                  getSupplicantMockableV1_1().addInterface(ifaceInfo,
516                          (SupplicantStatus status, ISupplicantIface iface) -> {
517                              if (status.code != SupplicantStatusCode.SUCCESS
518                                      && status.code != SupplicantStatusCode.FAILURE_IFACE_EXISTS) {
519                                  Log.e(TAG, "Failed to create ISupplicantIface " + status.code);
520                                  return;
521                              }
522                              supplicantIface.value = iface;
523                          });
524              } catch (RemoteException e) {
525                  Log.e(TAG, "ISupplicant.addInterface exception: " + e);
526                  handleRemoteException(e, "addInterface");
527                  return null;
528              } catch (NoSuchElementException e) {
529                  Log.e(TAG, "ISupplicant.addInterface exception: " + e);
530                  handleNoSuchElementException(e, "addInterface");
531                  return null;
532              }
533              return supplicantIface.value;
534          }
535      }



761      protected ISupplicant getSupplicantMockable() throws RemoteException, NoSuchElementException {
762          synchronized (mLock) {
763              ISupplicant iSupplicant = ISupplicant.getService();
764              if (iSupplicant == null) {
765                  throw new NoSuchElementException("Cannot get root service.");
766              }
767              return iSupplicant;
768          }
769      }
770  
771      protected android.hardware.wifi.supplicant.V1_1.ISupplicant getSupplicantMockableV1_1()
772              throws RemoteException, NoSuchElementException {
773          synchronized (mLock) {
774              android.hardware.wifi.supplicant.V1_1.ISupplicant iSupplicantDerived =
775                      android.hardware.wifi.supplicant.V1_1.ISupplicant.castFrom(
776                              getSupplicantMockable());
777              if (iSupplicantDerived == null) {
778                  throw new NoSuchElementException("Cannot cast to V1.1 service.");
779              }
780              return iSupplicantDerived;
781          }
782      }


797      protected ISupplicantStaIface getStaIfaceMockable(ISupplicantIface iface) {
798          synchronized (mLock) {
799              return ISupplicantStaIface.asInterface(iface.asBinder());
800          }
801      }
}
```



getSupplicantMockableV1_1()返回的是一个ISupplicant.Stub.Proxy对象，使用该hidl代理对象的addInterface方法相当于调用到了wpa_supplicant进程中Supplicant对象的addInterface方法。具体实现参考external/wpa_supplicant_8/wpa_supplicant/hidl/1.3/supplicant.cpp：

```
171  Return<void> Supplicant::addInterface(
172      const IfaceInfo& iface_info, addInterface_cb _hidl_cb)
173  {
174  	return validateAndCall(
175  	    this, SupplicantStatusCode::FAILURE_IFACE_INVALID,
176  	    &Supplicant::addInterfaceInternal, _hidl_cb, iface_info);
177  }
....
256  std::pair<SupplicantStatus, sp<ISupplicantIface>>
257  Supplicant::addInterfaceInternal(const IfaceInfo& iface_info)
258  {
....
307  	iface_params.ifname = iface_info.name.c_str();
308  	struct wpa_supplicant* wpa_s =
309  	    wpa_supplicant_add_iface(wpa_global_, &iface_params, NULL);
310  	if (!wpa_s) {
311  		return {{SupplicantStatusCode::FAILURE_UNKNOWN, ""}, {}};
312  	}
313  	// The supplicant core creates a corresponding hidl object via
314  	// HidlManager when |wpa_supplicant_add_iface| is called.
315  	return getInterfaceInternal(iface_info);
316  }
```

addInterface方法最终调用到了addInterfaceInternal方法，addInterfaceInternal方法调用wpa_supplicant_add_iface函数。wpa_supplicant_add_iface函数在ctrl_interface指定的目录下创建了一个unix socket套接字文件，并间接调用到HidlManager::registerInterface



external/wpa_supplicant_8/wpa_supplicant/hidl/1.3/hidl_manager.cpp

```
440  int HidlManager::registerInterface(struct wpa_supplicant *wpa_s)...
460  		if (addHidlObjectToMap<StaIface>(
461  			wpa_s->ifname,
462  			new StaIface(wpa_s->global, wpa_s->ifname),
463  			sta_iface_object_map_)) {
464  			wpa_printf(
465  			    MSG_ERROR,
466  			    "Failed to register STA interface with HIDL "
467  			    "control: %s",
468  			    wpa_s->ifname);
469  			return 1;
470  		}...}
```

HidlManager::registerInterface方法通过调用addHidlObjectToMap将new出来的一个StaIface对象放入Map中。

这一系列搞完后返回到addInterfaceInternal的return处调用getInterfaceInternal，getInterfaceInternal间接调用到HidlManager::getStaIfaceHidlObjectByIfname()从map中将StaIface对象取回并返回给JAVA。JAVA端调用getSupplicantMockableV1_1().addInterface()其实拿到的是StaIface对象的代理对象。



StaIface类派生自ISupplicantStaIface类，

external/wpa_supplicant_8/wpa_supplicant/hidl/1.3/sta_iface.h

```
class StaIface : public V1_3::ISupplicantStaIface
```

ISupplicantStaIface类派生自ISupplicantIface类，hardware/interfaces/wifi/supplicant/1.0/ISupplicantStaIface.hal

```
interface ISupplicantStaIface extends ISupplicantIface 
```







在SupplicantStaIfaceHal类的的setupIface方法中调完addIfaceV1_1，addIfaceV1_1首先通过hwServiceManager拿到ISupplicant代理对象，再通过ISupplicant代理对象调用addInterface拿到ISupplicantIface的代理对象（其实是ISupplicantStaIface的代理对象）后返回，setupIface方法接着再调用setupStaIface方法，setupStaIface方法调用getStaIfaceMockable将ISupplicantIface的代理对象转换成ISupplicantStaIface的代理对象，即wpa_supplicant中sta_iface对象的代理对象。







## 3 wificond

参考后续章节  打开wifi和扫描AP的描述







## 4.android.hardware.wifi@1.x-service









## 5. wpa_supplicant



### 5.0 wpa_supplicant  hal文件中interface的关系



![wpa-wpa_hal_interface_relations](wifi.assets/wpa-wpa_hal_interface_relations.jpg)





### 5.1 基本命令行参数及配置文件



iw

iwconfig

iwlist

iwpriv







wpa_cli -i wlan0 -p -g@android:wpa_wlan0 IFNAME=wlan0 status





```
1|console:/ #  cat /vendor/etc/init/android.hardware.wifi.supplicant-service.rc
service wpa_supplicant /vendor/bin/hw/wpa_supplicant \
    -O/data/vendor/wifi/wpa/sockets -dd \
    -g@android:wpa_wlan0
    #   we will start as root and wpa_supplicant will switch to user wifi
    #   after setting up the capabilities required for WEXT
    #   user wifi
    #   group wifi inet keystore
    interface android.hardware.wifi.supplicant@1.0::ISupplicant default
    interface android.hardware.wifi.supplicant@1.1::ISupplicant default
    interface android.hardware.wifi.supplicant@1.2::ISupplicant default
    interface android.hardware.wifi.supplicant@1.3::ISupplicant default
    interface vendor.mediatek.hardware.wifi.supplicant@2.0::ISupplicant default
    class main
    socket wpa_wlan0 dgram 660 wifi wifi
    disabled
    oneshot
```









```
console:/ #  cat /vendor/etc/wifi/wpa_supplicant.conf
ctrl_interface=wlan0
update_config=1
manufacturer=MediaTek Inc.
device_name=Wireless Client
model_name=MTK Wireless Model
model_number=1.0
serial_number=2.0
device_type=10-0050F204-5
os_version=01020300
config_methods=display push_button keypad
p2p_no_group_iface=1
driver_param=use_p2p_group_interface=1
hs20=1
pmf=1
wowlan_disconnect_on_deinit=1
```



```
console:/ # /vendor/bin/hw/wpa_supplicant -h
wpa_supplicant v2.10-devel-11
Copyright (c) 2003-2019, Jouni Malinen <j@w1.fi> and contributors

This software may be distributed under the terms of the BSD license.
See README for more details.

This product includes software developed by the OpenSSL Project
for use in the OpenSSL Toolkit (http://www.openssl.org/)

usage:
  wpa_supplicant [-BddhKLqqtvW] [-P<pid file>] [-g<global ctrl>] \
        [-G<group>] \
        -i<ifname> -c<config file> [-C<ctrl>] [-D<driver>] [-p<driver_param>] \
        [-b<br_ifname>] [-e<entropy file>] \
        [-o<override driver>] [-O<override ctrl>] \
        [-N -i<ifname> -c<conf> [-C<ctrl>] [-D<driver>] \
        [-m<P2P Device config file>] \
        [-p<driver_param>] [-b<br_ifname>] [-I<config file>] ...]

drivers:
  nl80211 = Linux nl80211/cfg80211
options:
  -b = optional bridge interface name
  -B = run daemon in the background
  -c = Configuration file
  -C = ctrl_interface parameter (only used if -c is not)
  -d = increase debugging verbosity (-dd even more)
  -D = driver name (can be multiple drivers: nl80211,wext)
  -e = entropy file
  -g = global ctrl_interface
  -G = global ctrl_interface group
  -h = show this help text
  -i = interface name
  -I = additional configuration file
  -K = include keys (passwords, etc.) in debug output
  -L = show license (BSD)
  -m = Configuration file for the P2P Device interface
  -N = start describing new interface
  -o = override driver parameter for new interfaces
  -O = override ctrl_interface parameter for new interfaces
  -p = driver parameters
  -P = PID file
  -q = decrease debugging verbosity (-qq even less)
  -t = include timestamp in debug messages
  -v = show version
  -W = wait for a control interface monitor before starting
example:
  wpa_supplicant -Dnl80211 -iwlan0 -c/etc/wpa_supplicant.conf

```



ls /data/vendor/wifi/wpa/sockets/



setprop  wifi.interface  wlan0 指定默认套接字/网卡接口，在-i参数未指定时生效



直接使用-g参数指定/dev/socket/wpa_wlan0 套接字文件路径及文件名，即socket的sun_path，sun_path = ctrl_interface + ifname

```
#file /dev/socket/wpa_wlan0
/dev/socket/wpa_wlan0: no such file or directory

#wpa_supplicant -g@android:wpa_wlan0 IFNAME=wlan0

#file /dev/socket/wpa_wlan0
/dev/socket/wpa_wlan0: socket

#wpa_cli  -g@android:wpa_wlan0 IFNAME=wlan0
```



以下四个操作等效，都指定wlan0这个interface

wpa_cli  -g@android:wpa_wlan0        IFNAME=wlan0   #通过全局控制套接字，并指定interface为wlan0

wpa_cli  -p  /data/vendor/wifi/wpa/sockets  -i wlan0    #通过-p指定interface的套接字所在路径，-i指定interface的套接字名

wpa_cli  -O /data/vendor/wifi/wpa/sockets  -i wlan0     #-O可以重写-p的配置。

wpa_cli  -i wlan0                                                                    #使用默认路径/data/vendor/wifi/wpa/sockets





使用ctrl_interface属性指定socket套接字所在目录，使用ifname属性指定socket套接字文件名

```
//通过-C参数指定socket套接字所在目录的路径（使用-C了就不能使用-c指定配置文件），-i参数指定该目录下的socket套接字文件名
/vendor/bin/hw/wpa_supplicant -C /data/vendor/wifi/wpa/sockets -i wlan0

//通过-c指定wpa_supplicant.conf配置文件，配置文件中有ctrl_interface=/data/vendor/wifi/wpa/sockets  ifname=wlan0
wpa_supplicant -c /data/vendor/wifi/wpa/wpa_supplicant.conf

//使用-c指定wpa_supplicant.conf配置文件，配置文件中有ctrl_interface=/data/vendor/wifi/wpa/sockets，使用-i参数指定ifname套接字文件名
wpa_supplicant -c /data/vendor/wifi/wpa/wpa_supplicant.conf -i wlan0


wpa_cli  -p /data/vendor/wifi/wpa/sockets  -i wlan0



注意，-O选项可以覆盖-C指定的ctrl_interface或在-c指定的conf文件中的ctrl_interface，即可以通过-O指定ctrl_interface
```





```
wpa_cli   <-xx   <yyy>>   zzz
wpa_cli 将除-xx及-xx选项所需的yyy之外的剩余参数zzz都会通过unix套接字原封不动地通过ctrl_interface指定的目录下指定的unix套接字文件发给wpa_supplicant, wpa_supplicant监听这些套件字文件并做后续相应事件处理。
```



装载wifi驱动后得到的wlan0网卡要经过如下步骤后才能正常使用：

insmod  wifi-driver.ko   #执行成功后就能看到网卡名wlan0 

ip link set dev  wlan0 up  #或 ifconfig wlan0 up

wpa_cli   add_inface  wlan0  #成功后ctrl_interface指定的目录下就有wlan0（与网卡同名）套接字文件了

wpa_cli   -i wlan0  scan

wpa_cli   -i wlan0  scan_result

wpa_cli    -i wlan0 add_network  #在wlan0接口中添加网络编号,得到networkID  

wpa_cli    -i wlan0 set_network  \<networkID\>xxxxx  #wifi热点名ssid、key_mgmt加密方式wpa/psk认证等，相当于配置文件中network={ ssid=xxx   psk=xxx    key_mgnt=xxx  ...}字段

wpa_cli    -i wlan0 select_network \<networkID\>   #select_network 会把指定的networkid enable，把其他的networkid disalbe

wpa_cli    -i wlan0 enable_network   \<networkID\>  #enable_network   会把指定的networkid enable

busybox udhcpc  -i wlan0 -q   #使用hdcp协议让DNS服务器分配ip，dhclient -i wlan0 功能与其类似

ip addr add XXX.XXX.XXX.XXX/24 dev wlan0 #将wlan0网卡的ip设置成DNS服务器分配的ip，ifconfig也行

#查询当前局域网的网关

#添加路由表

ip route <add/del/change/show>

ip route add default via 192.168.1.2 dev eth0

ip route add 192.168.10.0/24 via 192.168.5.100 dev eth0

ip route add  192.168.42.0/24   dev  eth0

ip route add  192.168.43.0/24   dev  wlan0

ip route add  < default  或者 0.0.0.0/0 >  via 192.168.43.1 dev  wlan0

ip route add  192.22.31.22/32  via  192.168.43.1 dev eth0  

ip route add  192.22.33.0/24  via  192.168.43.1 dev eth0  

ip route  del  < ip/mask >

ip route change 

配置DNS



https://blog.csdn.net/weixin_30682043/article/details/116870468

**当wpa_supplicant触发定时扫描并获得扫描结果后，如果扫描结果中有某个已经enable的networkid对应的SSID，那么就会触发连接的流程。扫描过程可以通过指定当前需要连接的BSSID、信道等信息加快扫描过程，从而更快地获得扫描结果。**



netsh wlan show interfaces

wpa_cli -iwlan0  dump

 wpa_cli -iwlan0   log_level

wpa_cli -iwlan0  signal_poll

wpa_cli -iwlan0  blacklist

wpa_cli -iwlan0   status

wpa_cli -iwlan0   scan

wpa_cli -iwlan0  scan_result

wpa_cli -iwlan0  bss <SSID/BSSID>

wpa_cli -i wlan0 get_capability <eap/pairwise/group/key_mgmt/proto/auth_alg/channels/freq/modes>



iw dev

```
184  	struct wpa_interface *ifaces, *iface;
185  	int iface_count, exitcode = -1;
186  	struct wpa_params params;
187  	struct wpa_global *global;

iface->confname =""/.../wpa_supplicnat.conf"
iface->ctrl_interface=
iface->ifname =


global->params.ctrl_interface = "@android:wpa_wlan0"
global->params.override_ctrl_interface="/data/vendor/wifi/wpa/sockets"
```



```
 echo ctrl_interface=/data/vendor/wifi/wpa/sockets > /data/meta_wpa_supplicant.conf
/vendor/bin/hw/wpa_supplicant -iwlan0 -Dnl80211 -c /data/meta_wpa_supplicant.conf &

	wpa_cli -i wlan0 add_network //增加网络号
	wpa_cli -i wlan0 set_network 1 ssid '"ZTE123"' //设置TP-LINK_5G    TP-LINK_E41CB0
	wpa_cli -i wlan0 set_network 1 key_mgmt "WPA-PSK" //设置加密方式
	wpa_cli -i wlan0 set_network 1 psk '"88888888"' //设置密码
	wpa_cli -i wlan0 enable_network 1 //使能该网络号
	wpa_cli -i wlan0 status //查看状态
	wpa_cli -i wlan0 add_network //获得新的网络编号
	
	wpa_cli -i wlan0 set_network 1 ssid '"ZTE123"' 
	wpa_cli -i wlan0 set_network 1 key_mgmt "WPA-PSK" 
	wpa_cli -i wlan0 set_network 1 psk '"88888888"' 
	wpa_cli -i wlan0 enable_network 1 

	wpa_cli -i wlan0 status
	wpa_cli -i wlan0 list_networks
	wpa_cli -i wlan0 select_network <网络编号>
	wpa_cli -i wlan0 add_network
	wpa_cli -i wlan0 remove_network <网络编号>
	wpa_cli -i wlan0 disable_network <网络编号>
	wpa_cli -i wlan0 enable_network <网络编号>
	wpa_cli -i wlan0 save_config
	wpa_cli -i wlan0 disconnect
	wpa_cli -i wlan0 reconnect
	
	
	std::string sUsername = "6418000265";
	std::string sPassword = "yzf2019**";
	std::string sSSID     = "zte_wlanap_uds_test118";
    int iNetworkID = 0;
	std::string sKeymgmt = "WPA-EAP";	// WPA2-GTC
	std::string sEapType = "PEAP_90";	// ZTE无线特别定义的
	std::string sCACert = "/usr/local/wpa_server/cert_ca.crt";
	std::string sClientCert = "/usr/local/wpa_server/cert_server.crt";
	std::string sPhase1 = "peaplabel=0";
	std::string sPhase2 = "auth=MSCHAPV2";
	std::string sPriority = "10";







/vendor/bin/hw/wpa_supplicant -C /data/vendor/wifi/wpa/sockets -iwlan0 -Dnl80211 &
    wpa_cli -i wlan0 add_network
    对应StaIface::addNetwork
    
	wpa_cli -i wlan0 set_network 0 ssid          '"zte_wlanap_sn_CS_h3c"' 
	对应StaNetwork::setSsid
	
	wpa_cli -i wlan0 set_network 0 key_mgmt        WPA-EAP 
	对应StaNetwork::setKeyMgmt    StaNetwork::setKeyMgmt_1_2   StaNetwork::setKeyMgmt_1_3
	
	wpa_cli -i wlan0 set_network 0 eap            PEAP_90
    对应StaNetwork::setEapMethod
    
    
    identity为二维码qrKey
	wpa_cli -i wlan0 set_network 0 identity       '"   "'
	对应StaNetwork::setEapIdentity
	
	wpa_cli -i wlan0 set_network 0 password      '"   "'
	对应StaNetwork::setEapPassword
	
	wpa_cli -i wlan0 set_network 0 ca_cert        '"/usr/local/wpa_server/cert_ca.crt"'
	对应StaNetwork::setEapCACert 
	
	wpa_cli -i wlan0 set_network 0 client_cert     '"/usr/local/wpa_server/cert_server.crt"'
	对应StaNetwork::setEapClientCert
	
	wpa_cli -i wlan0 set_network 0 phase1          '"peaplabel=0"'
	不完全对应StaNetwork::enableTlsSuiteBEapPhase1Param，都是设置的wpa_ssid->eap.phase1但值不同
	
	wpa_cli -i wlan0 set_network 0 phase2          '"auth=MSCHAPV2"'
	对应StaNetwork::setEapPhase2Method
	
    wpa_cli -i wlan0 set_network 0 priority        10
    没有对应的hidl接口
    
    wpa_cli -i wlan0 select_network 0
    对应StaNetwork::select
    
    wpa_cli -i wlan0 enable_network 0
    对应StaNetwork::enable
    
    wpa_cli -i wlan0 save_config
    sta没有对应的hidl接口，但p2p对应P2pIface::saveConfig
```



1、通过全局控unix套接字连接wlan0接口

wpa_cli  -g  /dev/socket/wpa_wlan0                   -i wlan0

wpa_cli  -g@android:wpa_wlan0                         -i wlan0

2、通过某个路径下的wlan0 unix套接字连接wlan0接口

wpa_cli -p  /data/vendor/wifi/wpa/sockets      -i wlan0



### 5.2 wpa_supplicant代码分析



全局控制unix套接字接口

/dev/socket/wpa_wlan0

可以通过该unix套接字发送INTERFACE_ADD命令来在ctrl_interface下创建ifname，即在/data/vendor/wifi/wpa/sockets/目录下创建wlan0  unix套接字文件，如果只是通过start wpa_supplicant来启动wpa_supplicant服务，在/data/vendor/wifi/wpa/sockets/下没有任何文件，如果通过UI打开wifi，来启动wpa_supplicant服务，在wpa_supplicant服务启动后发送INTERFACE_ADD在/data/vendor/wifi/wpa/sockets/下创建wlan0套接字接口文件。该接口主要用于增加或删除网络接口。





单一网卡(例如wlan0)的unix套接字控制接口

/data/vendor/wifi/wpa/sockets/wlan0

向该套接字发送



eap_peer_peap_register函数调用eap_peer_method_alloc函数分配一个struct eap_method结构体，并经过初始化后调用 eap_peer_method_register函数将struct eap_method结构体组成一个链表， 静态全局指针eap_methods 指向这个链表。





android.hardware.wifi.supplicant-service.rc中：

```
service wpa_supplicant /vendor/bin/hw/wpa_supplicant \
    -O/data/vendor/wifi/wpa/sockets -dd \
    -g@android:wpa_wlan0
    #   we will start as root and wpa_supplicant will switch to user wifi
    #   after setting up the capabilities required for WEXT
    #   user wifi
    #   group wifi inet keystore
    interface android.hardware.wifi.supplicant@1.0::ISupplicant default
    interface android.hardware.wifi.supplicant@1.1::ISupplicant default
    interface android.hardware.wifi.supplicant@1.2::ISupplicant default
    interface android.hardware.wifi.supplicant@1.3::ISupplicant default
    interface vendor.mediatek.hardware.wifi.supplicant@2.0::ISupplicant default
    class main
    socket wpa_wlan0 dgram 660 wifi wifi
    disabled
    oneshot
```

init进程在fork-exec  wpa_supplicant进程之前会在/dev/socket目录下创建wpa_wlan0    unix套接字文件，然后把套接字fd放到一个ANDROID_SOCKET_wpa_wlan0 环境变量中，fork-exec出的wpa_supplicant进程将继承fd和套接字。



wpa_supplicant_global_ctrl_iface_init函数中调用wpas_global_ctrl_iface_open_sock函数，wpas_global_ctrl_iface_open_sock函数调用android_get_control_socket函数。android_get_control_socket函数首先调用__android_get_control_from_env，使用-g命令行参数传进来的套接字名组合成ANDROID_SOCKET_wpa_wlan0字符串，再使用该字符串调用getenv从环境变量中拿到fd。拿到fd后在 android_get_control_socket函数中调用getsockname使用fd拿到套接字的sun_path，再查看与"/dev/socket"前缀和 -g传进来的"wpa_wlan0"套接字文件名是否匹配，如果成功返回到wpas_global_ctrl_iface_open_sock函数中将fd赋给priv->socks后再使用fcntl将套接字设为非阻塞模式，然后调用eloop_register_read_sock将套接字加入epoll事件队列，后续执行eloop_run时进行事件监听。当触发fd的可读事件时调用eloop_register_read_sock注册的wpa_supplicant_global_ctrl_iface_receive函数。



将套接字fd从ANDROID_SOCKET_wpa_wlan0 环境变量中取出，然后与-g命令行参数传进来的套接字名



tr.cook是BnHwSupplicant的智能指针，



```
main函数中的局部变量
struct wpa_supplicant *wpa_s 
struct wpa_interface  *ifaces
struct wpa_global     *global;
```



```
static struct eap_method *eap_methods;
external/wpa_supplicant_8/src/eap_peer/eap_methods.c
```



相关数据结构

struct wpa_supplicant   a

struct wpa_ssid               b
struct eap_peer_config  c


struct eap_sm                 d


struct eapol_sm              e



a->current_ssid  = b
b.eap = c

d->eapol_ctx = e
e->config = c


d->last_config = c







#### 5.2.1 初始化



![wpa-wpa_main_init](wifi.assets/wpa-wpa_main_init.jpg)



#### 5.2.2 全局控制unix套接字事件处理

wpa_supplicant_global_ctrl_iface_receive

![wpa-wpa_global_socket](wifi.assets/wpa-wpa_global_socket.jpg)



#### 5.2.3 hwbinder通信事件处理

##### 5.2.3.1 wpas_hidl_sock_handler



![wpa-wpa_hidl_call_Bnxxx](wifi.assets/wpa-wpa_hidl_call_Bnxxx.jpg)



##### 5.2.3.2 oneway方法接口



hal文件  hardware/interfaces/wifi/supplicant/1.1/ISupplicant.hal

```
28 interface ISupplicant extends @1.0::ISupplicant {
...
58     /**
59      * Terminate the service.
60      * This must de-register the service and clear all state. If this HAL
61      * supports the lazy HAL protocol, then this may trigger daemon to exit and
62      * wait to be restarted.
63      */
64     oneway terminate();
65 };
```



代理端

out/soong/.intermediates/hardware/interfaces/wifi/supplicant/1.1/android.hardware.wifi.supplicant@1.1_genc++/gen/android/hardware/wifi/supplicant/1.1/SupplicantAll.cpp

```
::android::hardware::Return<void> BpHwSupplicant::terminate(){
    ::android::hardware::Return<void>  _hidl_out = ::android::hardware::wifi::supplicant::V1_1::BpHwSupplicant::_hidl_terminate(this, this);

    return _hidl_out;
}


::android::hardware::Return<void> BpHwSupplicant::_hidl_terminate(::android::hardware::IInterface *_hidl_this, ::android::hardware::details::HidlInstrumentor *_hidl_this_instrumentor) {
    #ifdef __ANDROID_DEBUGGABLE__
    bool mEnableInstrumentation = _hidl_this_instrumentor->isInstrumentationEnabled();
    const auto &mInstrumentationCallbacks = _hidl_this_instrumentor->getInstrumentationCallbacks();
    #else
    (void) _hidl_this_instrumentor;
    #endif // __ANDROID_DEBUGGABLE__
    ::android::ScopedTrace PASTE(___tracer, __LINE__) (ATRACE_TAG_HAL, "HIDL::ISupplicant::terminate::client");
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::CLIENT_API_ENTRY, "android.hardware.wifi.supplicant", "1.1", "ISupplicant", "terminate", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    ::android::hardware::Parcel _hidl_data;
    ::android::hardware::Parcel _hidl_reply;
    ::android::status_t _hidl_err;
    ::android::status_t _hidl_transact_err;
    ::android::hardware::Status _hidl_status;

    _hidl_err = _hidl_data.writeInterfaceToken(BpHwSupplicant::descriptor);
    if (_hidl_err != ::android::OK) { goto _hidl_error; }

    _hidl_transact_err = ::android::hardware::IInterface::asBinder(_hidl_this)->transact(11 /* terminate */, _hidl_data, &_hidl_reply, 1u /* oneway */);
    if (_hidl_transact_err != ::android::OK)
    {
        _hidl_err = _hidl_transact_err;
        goto _hidl_error;
    }

    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::CLIENT_API_EXIT, "android.hardware.wifi.supplicant", "1.1", "ISupplicant", "terminate", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    return ::android::hardware::Return<void>();

_hidl_error:
    _hidl_status.setFromStatusT(_hidl_err);
    return ::android::hardware::Return<void>(_hidl_status);
}
```



```
代理端填充_hidl_data后调用
::android::hardware::IInterface::asBinder(_hidl_this)->transact(11 /* terminate */, _hidl_data, &_hidl_reply, 1u /* oneway */);
通过binder驱动发送出去之后并不读取 _hidl_reply直接返回
```

服务实现端

out/soong/.intermediates/hardware/interfaces/wifi/supplicant/1.1/android.hardware.wifi.supplicant@1.1_genc++/gen/android/hardware/wifi/supplicant/1.1/SupplicantAll.cpp

```
::android::status_t BnHwSupplicant::_hidl_terminate(
        ::android::hidl::base::V1_0::BnHwBase* _hidl_this,
        const ::android::hardware::Parcel &_hidl_data,
        ::android::hardware::Parcel *_hidl_reply,
        TransactCallback _hidl_cb) {
    #ifdef __ANDROID_DEBUGGABLE__
    bool mEnableInstrumentation = _hidl_this->isInstrumentationEnabled();
    const auto &mInstrumentationCallbacks = _hidl_this->getInstrumentationCallbacks();
    #endif // __ANDROID_DEBUGGABLE__

    ::android::status_t _hidl_err = ::android::OK;
    if (!_hidl_data.enforceInterface(BnHwSupplicant::Pure::descriptor)) {
        _hidl_err = ::android::BAD_TYPE;
        return _hidl_err;
    }

    atrace_begin(ATRACE_TAG_HAL, "HIDL::ISupplicant::terminate::server");
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::SERVER_API_ENTRY, "android.hardware.wifi.supplicant", "1.1", "ISupplicant", "terminate", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    ::android::hardware::Return<void> _hidl_ret = static_cast<ISupplicant*>(_hidl_this->getImpl().get())->terminate();

    (void) _hidl_cb;

    atrace_end(ATRACE_TAG_HAL);
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::SERVER_API_EXIT, "android.hardware.wifi.supplicant", "1.1", "ISupplicant", "terminate", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    _hidl_ret.assertOk();
    ::android::hardware::writeToParcel(::android::hardware::Status::ok(), _hidl_reply);

    return _hidl_err;
}
```



```
在服务端执行::android::hardware::Return<void> _hidl_ret = static_cast<ISupplicant*>(_hidl_this->getImpl().get())->terminate()调用具体实现的函数后 不需要填充_hidl_reply并通过调用_hidl_cb函数将_hidl_reply返回给代理端
::android::hardware::Return<void> _hidl_ret = static_cast<ISupplicant*>(_hidl_this->getImpl().get())->terminate();

    (void) _hidl_cb;
```

具体实现函数如下：

![image-20220117191613672](wifi.assets/image-20220117191613672.png)



##### 5.2.3.3 generates(单个参数且能映射成C++基本类型，包括void类型)

hal文件

hardware/interfaces/wifi/supplicant/1.0/ISupplicant.hal

```
27 interface ISupplicant {
...
33   enum DebugLevel : uint32_t {
34     EXCESSIVE = 0,
35     MSGDUMP = 1,
36     DEBUG = 2,
37     INFO = 3,
38     WARNING = 4,
39     ERROR = 5
40   };
...
129   getDebugLevel() generates (DebugLevel level);
...
159 };
```

代理端

out/soong/.intermediates/hardware/interfaces/wifi/supplicant/1.0/android.hardware.wifi.supplicant@1.0_genc++/gen/android/hardware/wifi/supplicant/1.0/SupplicantAll.cpp

```
::android::hardware::Return<::android::hardware::wifi::supplicant::V1_0::ISupplicant::DebugLevel> BpHwSupplicant::getDebugLevel(){
    ::android::hardware::Return<::android::hardware::wifi::supplicant::V1_0::ISupplicant::DebugLevel>  _hidl_out = ::android::hardware::wifi::supplicant::V1_0::BpHwSupplicant::_hidl_getDebugLevel(this, this);

    return _hidl_out;
}



::android::hardware::Return<::android::hardware::wifi::supplicant::V1_0::ISupplicant::DebugLevel> BpHwSupplicant::_hidl_getDebugLevel(::android::hardware::IInterface *_hidl_this, ::android::hardware::details::HidlInstrumentor *_hidl_this_instrumentor) {
    #ifdef __ANDROID_DEBUGGABLE__
    bool mEnableInstrumentation = _hidl_this_instrumentor->isInstrumentationEnabled();
    const auto &mInstrumentationCallbacks = _hidl_this_instrumentor->getInstrumentationCallbacks();
    #else
    (void) _hidl_this_instrumentor;
    #endif // __ANDROID_DEBUGGABLE__
    ::android::ScopedTrace PASTE(___tracer, __LINE__) (ATRACE_TAG_HAL, "HIDL::ISupplicant::getDebugLevel::client");
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::CLIENT_API_ENTRY, "android.hardware.wifi.supplicant", "1.0", "ISupplicant", "getDebugLevel", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    ::android::hardware::Parcel _hidl_data;
    ::android::hardware::Parcel _hidl_reply;
    ::android::status_t _hidl_err;
    ::android::status_t _hidl_transact_err;
    ::android::hardware::Status _hidl_status;

    ::android::hardware::wifi::supplicant::V1_0::ISupplicant::DebugLevel _hidl_out_level;

    _hidl_err = _hidl_data.writeInterfaceToken(BpHwSupplicant::descriptor);
    if (_hidl_err != ::android::OK) { goto _hidl_error; }

    _hidl_transact_err = ::android::hardware::IInterface::asBinder(_hidl_this)->transact(5 /* getDebugLevel */, _hidl_data, &_hidl_reply, 0);
    if (_hidl_transact_err != ::android::OK)
    {
        _hidl_err = _hidl_transact_err;
        goto _hidl_error;
    }

    _hidl_err = ::android::hardware::readFromParcel(&_hidl_status, _hidl_reply);
    if (_hidl_err != ::android::OK) { goto _hidl_error; }

    if (!_hidl_status.isOk()) { return _hidl_status; }

    _hidl_err = _hidl_reply.readUint32((uint32_t *)&_hidl_out_level);
    if (_hidl_err != ::android::OK) { goto _hidl_error; }

    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        _hidl_args.push_back((void *)&_hidl_out_level);
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::CLIENT_API_EXIT, "android.hardware.wifi.supplicant", "1.0", "ISupplicant", "getDebugLevel", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    return ::android::hardware::Return<::android::hardware::wifi::supplicant::V1_0::ISupplicant::DebugLevel>(_hidl_out_level);

_hidl_error:
    _hidl_status.setFromStatusT(_hidl_err);
    return ::android::hardware::Return<::android::hardware::wifi::supplicant::V1_0::ISupplicant::DebugLevel>(_hidl_status);
}
```



```
代理端填充_hidl_data后调用
_hidl_transact_err = ::android::hardware::IInterface::asBinder(_hidl_this)->transact(5 /* getDebugLevel */, _hidl_data, &_hidl_reply, 0);
将数据发送给服务端，之后再调用
 _hidl_err = ::android::hardware::readFromParcel(&_hidl_status, _hidl_reply);
 阻塞等待服务端将_hidl_reply发回来，最后读取里面的值作为BpHwSupplicant::_hidl_getDebugLevel方法的返回值。
```



服务实现端

out/soong/.intermediates/hardware/interfaces/wifi/supplicant/1.0/android.hardware.wifi.supplicant@1.0_genc++/gen/android/hardware/wifi/supplicant/1.0/SupplicantAll.cpp

```
 ::android::status_t BnHwSupplicant::_hidl_getDebugLevel(
        ::android::hidl::base::V1_0::BnHwBase* _hidl_this,
        const ::android::hardware::Parcel &_hidl_data,
        ::android::hardware::Parcel *_hidl_reply,
        TransactCallback _hidl_cb) {
    #ifdef __ANDROID_DEBUGGABLE__
    bool mEnableInstrumentation = _hidl_this->isInstrumentationEnabled();
    const auto &mInstrumentationCallbacks = _hidl_this->getInstrumentationCallbacks();
    #endif // __ANDROID_DEBUGGABLE__

    ::android::status_t _hidl_err = ::android::OK;
    if (!_hidl_data.enforceInterface(BnHwSupplicant::Pure::descriptor)) {
        _hidl_err = ::android::BAD_TYPE;
        return _hidl_err;
    }

    atrace_begin(ATRACE_TAG_HAL, "HIDL::ISupplicant::getDebugLevel::server");
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::SERVER_API_ENTRY, "android.hardware.wifi.supplicant", "1.0", "ISupplicant", "getDebugLevel", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    ::android::hardware::wifi::supplicant::V1_0::ISupplicant::DebugLevel _hidl_out_level = static_cast<ISupplicant*>(_hidl_this->getImpl().get())->getDebugLevel();

    ::android::hardware::writeToParcel(::android::hardware::Status::ok(), _hidl_reply);

    _hidl_err = _hidl_reply->writeUint32((uint32_t)_hidl_out_level);
    if (_hidl_err != ::android::OK) { goto _hidl_error; }

_hidl_error:
    atrace_end(ATRACE_TAG_HAL);
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        _hidl_args.push_back((void *)&_hidl_out_level);
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::SERVER_API_EXIT, "android.hardware.wifi.supplicant", "1.0", "ISupplicant", "getDebugLevel", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    if (_hidl_err != ::android::OK) { return _hidl_err; }
    _hidl_cb(*_hidl_reply);
    return _hidl_err;
}
```



```
调用具体实现函数
::android::hardware::wifi::supplicant::V1_0::ISupplicant::DebugLevel _hidl_out_level = static_cast<ISupplicant*>(_hidl_this->getImpl().get())->getDebugLevel();
将得到的返回值_hidl_out_level写入_hidl_reply
_hidl_err = _hidl_reply->writeUint32((uint32_t)_hidl_out_level);
调用_hidl_cb函数把_hidl_reply发送给代理端。这里的_hidl_cb函数类型是TransactCallback，它指向调用BnHwSupplicant::_hidl_getDebugLevel()时传入的匿名函数，即reply_callback
```

![image-20220117185710778](wifi.assets/image-20220117185710778.png)



具体实现函数如下：

![image-20220117191843169](wifi.assets/image-20220117191843169.png)

##### 5.2.3.4 generates(多个参数  或 不能映射成C++基本类型的单个参数)

hal文件

hardware/interfaces/wifi/supplicant/1.1/ISupplicant.hal

```
28 interface ISupplicant extends @1.0::ISupplicant {
29     /**
30      * Registers a wireless interface in supplicant.
31      *
32      * @param ifaceInfo Combination of the interface type and name(e.g wlan0).
33      * @return status Status of the operation.
34      *         Possible status codes:
35      *         |SupplicantStatusCode.SUCCESS|,
36      *         |SupplicantStatusCode.FAILURE_ARGS_INVALID|,
37      *         |SupplicantStatusCode.FAILURE_UNKNOWN|,
38      *         |SupplicantStatusCode.FAILURE_IFACE_EXISTS|
39      * @return iface HIDL interface object representing the interface if
40      *         successful, null otherwise.
41      */
42     addInterface(IfaceInfo ifaceInfo)
43         generates (SupplicantStatus status, ISupplicantIface iface);
...
65 };
```



代理端

out/soong/.intermediates/hardware/interfaces/wifi/supplicant/1.1/android.hardware.wifi.supplicant@1.1_genc++/gen/android/hardware/wifi/supplicant/1.1/SupplicantAll.cpp

```
::android::hardware::Return<void> BpHwSupplicant::addInterface(const ::android::hardware::wifi::supplicant::V1_0::ISupplicant::IfaceInfo& ifaceInfo, addInterface_cb _hidl_cb){
    ::android::hardware::Return<void>  _hidl_out = ::android::hardware::wifi::supplicant::V1_1::BpHwSupplicant::_hidl_addInterface(this, this, ifaceInfo, _hidl_cb);

    return _hidl_out;
}


::android::hardware::Return<void> BpHwSupplicant::_hidl_addInterface(::android::hardware::IInterface *_hidl_this, ::android::hardware::details::HidlInstrumentor *_hidl_this_instrumentor, const ::android::hardware::wifi::supplicant::V1_0::ISupplicant::IfaceInfo& ifaceInfo, addInterface_cb _hidl_cb) {
    #ifdef __ANDROID_DEBUGGABLE__
    bool mEnableInstrumentation = _hidl_this_instrumentor->isInstrumentationEnabled();
    const auto &mInstrumentationCallbacks = _hidl_this_instrumentor->getInstrumentationCallbacks();
    #else
    (void) _hidl_this_instrumentor;
    #endif // __ANDROID_DEBUGGABLE__
    ::android::ScopedTrace PASTE(___tracer, __LINE__) (ATRACE_TAG_HAL, "HIDL::ISupplicant::addInterface::client");
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        _hidl_args.push_back((void *)&ifaceInfo);
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::CLIENT_API_ENTRY, "android.hardware.wifi.supplicant", "1.1", "ISupplicant", "addInterface", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    ::android::hardware::Parcel _hidl_data;
    ::android::hardware::Parcel _hidl_reply;
    ::android::status_t _hidl_err;
    ::android::status_t _hidl_transact_err;
    ::android::hardware::Status _hidl_status;

    _hidl_err = _hidl_data.writeInterfaceToken(BpHwSupplicant::descriptor);
    if (_hidl_err != ::android::OK) { goto _hidl_error; }

    size_t _hidl_ifaceInfo_parent;

    _hidl_err = _hidl_data.writeBuffer(&ifaceInfo, sizeof(ifaceInfo), &_hidl_ifaceInfo_parent);
    if (_hidl_err != ::android::OK) { goto _hidl_error; }

    _hidl_err = writeEmbeddedToParcel(
            ifaceInfo,
            &_hidl_data,
            _hidl_ifaceInfo_parent,
            0 /* parentOffset */);

    if (_hidl_err != ::android::OK) { goto _hidl_error; }

    _hidl_transact_err = ::android::hardware::IInterface::asBinder(_hidl_this)->transact(9 /* addInterface */, _hidl_data, &_hidl_reply, 0, [&] (::android::hardware::Parcel& _hidl_reply) {
        ::android::hardware::wifi::supplicant::V1_0::SupplicantStatus* _hidl_out_status;
        ::android::sp<::android::hardware::wifi::supplicant::V1_0::ISupplicantIface> _hidl_out_iface;


        _hidl_err = ::android::hardware::readFromParcel(&_hidl_status, _hidl_reply);
        if (_hidl_err != ::android::OK) { return; }

        if (!_hidl_status.isOk()) { return; }

        size_t _hidl__hidl_out_status_parent;

        _hidl_err = _hidl_reply.readBuffer(sizeof(*_hidl_out_status), &_hidl__hidl_out_status_parent,  const_cast<const void**>(reinterpret_cast<void **>(&_hidl_out_status)));
        if (_hidl_err != ::android::OK) { return; }

        _hidl_err = readEmbeddedFromParcel(
                const_cast<::android::hardware::wifi::supplicant::V1_0::SupplicantStatus &>(*_hidl_out_status),
                _hidl_reply,
                _hidl__hidl_out_status_parent,
                0 /* parentOffset */);

        if (_hidl_err != ::android::OK) { return; }

        {
            ::android::sp<::android::hardware::IBinder> _hidl_binder;
            _hidl_err = _hidl_reply.readNullableStrongBinder(&_hidl_binder);
            if (_hidl_err != ::android::OK) { return; }

            _hidl_out_iface = ::android::hardware::fromBinder<::android::hardware::wifi::supplicant::V1_0::ISupplicantIface,::android::hardware::wifi::supplicant::V1_0::BpHwSupplicantIface,::android::hardware::wifi::supplicant::V1_0::BnHwSupplicantIface>(_hidl_binder);
        }

        _hidl_cb(*_hidl_out_status, _hidl_out_iface);

        #ifdef __ANDROID_DEBUGGABLE__
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)_hidl_out_status);
            _hidl_args.push_back((void *)&_hidl_out_iface);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::CLIENT_API_EXIT, "android.hardware.wifi.supplicant", "1.1", "ISupplicant", "addInterface", &_hidl_args);
            }
        }
        #endif // __ANDROID_DEBUGGABLE__

    });
    if (_hidl_transact_err != ::android::OK) {
        _hidl_err = _hidl_transact_err;
        goto _hidl_error;
    }

    if (!_hidl_status.isOk()) { return _hidl_status; }
    return ::android::hardware::Return<void>();

_hidl_error:
    _hidl_status.setFromStatusT(_hidl_err);
    return ::android::hardware::Return<void>(_hidl_status);
}
```



```
代理端在填充_hidl_data后调用_hidl_transact_err = ::android::hardware::IInterface::asBinder(_hidl_this)->transact(9 /* addInterface */, _hidl_data, &_hidl_reply, 0, [&] (::android::hardware::Parcel& _hidl_reply) { ... });额外传入了一个匿名函数指针记作A，_hidl_reply是A函数的参数，在A函数里面调用_hidl_err = ::android::hardware::readFromParcel(&_hidl_status, _hidl_reply);等待服务实现端将_hidl_reply发送过来，然后读取里面的数据尝试将其转换成SupplicantStatus类型的数据，再读取_hidl_reply的数据并尝试将其转换成BpHwSupplicantIface类型的数据，然后调用函数指针_hidl_cb(*_hidl_out_status, _hidl_out_iface)将二者返回。这里的_hidl_cb是addInterface_cb类型的函数指针，指向了调用BpHwSupplicant::_hidl_addInterface时传入的回调函数(记作B函数)。在B中可以将从服务实现端传入的_hidl_out_status, _hidl_out_iface参数进行保存。
```





服务实现端

out/soong/.intermediates/hardware/interfaces/wifi/supplicant/1.1/android.hardware.wifi.supplicant@1.1_genc++/gen/android/hardware/wifi/supplicant/1.1/SupplicantAll.cpp

```
::android::status_t BnHwSupplicant::_hidl_addInterface(
        ::android::hidl::base::V1_0::BnHwBase* _hidl_this,
        const ::android::hardware::Parcel &_hidl_data,
        ::android::hardware::Parcel *_hidl_reply,
        TransactCallback _hidl_cb) {
    #ifdef __ANDROID_DEBUGGABLE__
    bool mEnableInstrumentation = _hidl_this->isInstrumentationEnabled();
    const auto &mInstrumentationCallbacks = _hidl_this->getInstrumentationCallbacks();
    #endif // __ANDROID_DEBUGGABLE__

    ::android::status_t _hidl_err = ::android::OK;
    if (!_hidl_data.enforceInterface(BnHwSupplicant::Pure::descriptor)) {
        _hidl_err = ::android::BAD_TYPE;
        return _hidl_err;
    }

    ::android::hardware::wifi::supplicant::V1_0::ISupplicant::IfaceInfo* ifaceInfo;

    size_t _hidl_ifaceInfo_parent;

    _hidl_err = _hidl_data.readBuffer(sizeof(*ifaceInfo), &_hidl_ifaceInfo_parent,  const_cast<const void**>(reinterpret_cast<void **>(&ifaceInfo)));
    if (_hidl_err != ::android::OK) { return _hidl_err; }

    _hidl_err = readEmbeddedFromParcel(
            const_cast<::android::hardware::wifi::supplicant::V1_0::ISupplicant::IfaceInfo &>(*ifaceInfo),
            _hidl_data,
            _hidl_ifaceInfo_parent,
            0 /* parentOffset */);

    if (_hidl_err != ::android::OK) { return _hidl_err; }

    atrace_begin(ATRACE_TAG_HAL, "HIDL::ISupplicant::addInterface::server");
    #ifdef __ANDROID_DEBUGGABLE__
    if (UNLIKELY(mEnableInstrumentation)) {
        std::vector<void *> _hidl_args;
        _hidl_args.push_back((void *)ifaceInfo);
        for (const auto &callback: mInstrumentationCallbacks) {
            callback(InstrumentationEvent::SERVER_API_ENTRY, "android.hardware.wifi.supplicant", "1.1", "ISupplicant", "addInterface", &_hidl_args);
        }
    }
    #endif // __ANDROID_DEBUGGABLE__

    bool _hidl_callbackCalled = false;

    ::android::hardware::Return<void> _hidl_ret = static_cast<ISupplicant*>(_hidl_this->getImpl().get())->addInterface(*ifaceInfo, [&](const auto &_hidl_out_status, const auto &_hidl_out_iface) {
        if (_hidl_callbackCalled) {
            LOG_ALWAYS_FATAL("addInterface: _hidl_cb called a second time, but must be called once.");
        }
        _hidl_callbackCalled = true;

        ::android::hardware::writeToParcel(::android::hardware::Status::ok(), _hidl_reply);

        size_t _hidl__hidl_out_status_parent;

        _hidl_err = _hidl_reply->writeBuffer(&_hidl_out_status, sizeof(_hidl_out_status), &_hidl__hidl_out_status_parent);
        if (_hidl_err != ::android::OK) { goto _hidl_error; }

        _hidl_err = writeEmbeddedToParcel(
                _hidl_out_status,
                _hidl_reply,
                _hidl__hidl_out_status_parent,
                0 /* parentOffset */);

        if (_hidl_err != ::android::OK) { goto _hidl_error; }

        if (_hidl_out_iface == nullptr) {
            _hidl_err = _hidl_reply->writeStrongBinder(nullptr);
        } else {
            ::android::sp<::android::hardware::IBinder> _hidl_binder = ::android::hardware::getOrCreateCachedBinder(_hidl_out_iface.get());
            if (_hidl_binder.get() != nullptr) {
                _hidl_err = _hidl_reply->writeStrongBinder(_hidl_binder);
            } else {
                _hidl_err = ::android::UNKNOWN_ERROR;
            }
        }
        if (_hidl_err != ::android::OK) { goto _hidl_error; }

    _hidl_error:
        atrace_end(ATRACE_TAG_HAL);
        #ifdef __ANDROID_DEBUGGABLE__
        if (UNLIKELY(mEnableInstrumentation)) {
            std::vector<void *> _hidl_args;
            _hidl_args.push_back((void *)&_hidl_out_status);
            _hidl_args.push_back((void *)&_hidl_out_iface);
            for (const auto &callback: mInstrumentationCallbacks) {
                callback(InstrumentationEvent::SERVER_API_EXIT, "android.hardware.wifi.supplicant", "1.1", "ISupplicant", "addInterface", &_hidl_args);
            }
        }
        #endif // __ANDROID_DEBUGGABLE__

        if (_hidl_err != ::android::OK) { return; }
        _hidl_cb(*_hidl_reply);
    });

    _hidl_ret.assertOk();
    if (!_hidl_callbackCalled) {
        LOG_ALWAYS_FATAL("addInterface: _hidl_cb not called, but must be called once.");
    }

    return _hidl_err;
}
```



```
服务端调用
::android::hardware::Return<void> _hidl_ret = static_cast<ISupplicant*>(_hidl_this->getImpl().get())->addInterface(*ifaceInfo, [&](const auto &_hidl_out_status, const auto &_hidl_out_iface) { ... });额外传入了一个匿名函数记作C。
```





![image-20220117191339730](wifi.assets/image-20220117191339730.png)







![image-20220117192141273](wifi.assets/image-20220117192141273.png)







![image-20220117191423017](wifi.assets/image-20220117191423017.png)





```
const auto& ret_pair = (obj->*work)(std::forward<Args>(args)...); 相当于调用了具体的实现函数Supplicant::addInterfaceInternal
接着从调用具体实现函数返回的ret_pair中取出SupplicantStatus类型的status对象和sp<ISupplicantIface>类型的ret_value对象(实际上ret_value指向的是一个ISupplicantStaIface对象，ISupplicantStaIface派生自ISupplicantIface)，然后将二者作为参数调用hidl_cb函数指针。hidl_cb其实指向了前面传入的匿名函数C，在匿名函数C中将传入的status写入_hidl_reply，然后使用传入的ret_value对象调用::android::sp<::android::hardware::IBinder> _hidl_binder = ::android::hardware::getOrCreateCachedBinder(_hidl_out_iface.get())得到一个sp<BnHwSupplicantIface>对象(实际指向的的是一个BnHwSupplicantStaIface对象)，然后将得到的sp<BnHwSupplicantIface>对象写入_hidl_reply，最后调用_hidl_cb(*_hidl_reply);将_hidl_reply返回给代理端，与前面一样，_hidl_cb的类型是TransactCallback，它指向调用BnHwSupplicant::_hidl_getDebugLevel()时传入的匿名函数，即在reply_callback：
```



![image-20220117185710778](wifi.assets/image-20220117185710778.png)





![wpa-Supplicant__addInterface](wifi.assets/wpa-Supplicant__addInterface-16425729479171.jpg)



#### 5.2.4 部分关键的hidl接口具体实现函数



##### 5.2.3.3 Supplicant::addInterfaceInternal

Supplicant::addInterfaceInternal判断传入的iface_info.name(virtual interface name ,也即网卡名)不为空后调用getInterfaceInternal从map中获取SupplicantIface接口的对象，如果没有则根据iface_info.type调用ensureConfigFileExists函数，然后使用相关信息填充struct wpa_interface类型的iface_params对象，然后将iface_params等信息作为参数调用wpa_supplicant_add_iface函数来创建一个SupplicantIface接口的对象并放入map中，最后再次调用getInterfaceInternal从map中取出该SupplicantIface接口的对象。



**注意，调用该接口时可能驱动中该name的virtual interface 已经存在了，调用该接口更多的是为了在wpa_supplicant进程中创建并初始化一个wpa_supplicant类型的对象用来描述virtual interface，当然最终都会发送数据给驱动去创建virtual interface ，只是驱动发现如果已经有该name的virtual interface时没有再去创建直接返回成功**。

  

![image-20220506165530399](wifi.assets/image-20220506165530399.png)

来看下ensureConfigFileExists注释和一些conf文件所在绝对路径的宏定义：

![image-20220506170806424](wifi.assets/image-20220506170806424.png)

![image-20220506170840436](wifi.assets/image-20220506170840436.png)

因此在mtk平台struct wpa_interface类型的iface_params对象被填充成这样：

iface_params.name = ifname                                                                   //virtual interface 名，即网卡名,一般sta模式传入的是wlan0

iface_params.confname = "/data/vendor/wifi/wpa/wpa_supplicant.conf"  //该文件拷贝自/vendor/etc/wifi/wpa_supplicant.conf

iface_params.confanother = "/vendor/etc/wifi/wpa_supplicant_overlay.conf" //overlay,用以覆盖wpa_supplicant.conf中的配置



跟踪wpa_supplicant的main函数及其初始化可以知道，Supplicant是一个往hwServiceManager注册的hidl对象，new Supplicant对象时传入了传入的struct wpa_global *地址就是main函数中局部变量struct wpa_global *global指向的堆地址。在Supplicant对象构造时其内部变量wpa_global\_也指向了堆中的同样一个位置。

![image-20220506172215234](wifi.assets/image-20220506172215234.png)



接下来看wpa_supplicant_add_iface如何使用wpa_global\_和iface_params 

![image-20220507094045902](wifi.assets/image-20220507094045902.png)





##### 5.2.3.4  StaIface::addNetworkInternal







##### 5.2.3.5  StaNetwork::setEapMethod







##### 5.2.3.6  StaNetwork::setSsid





StaNetwork::setKeyMgmt

StaNetwork::setKeyMgmt_1_2

StaNetwork::setKeyMgmt_1_3





### 5.3  wpa_supplicant中重要数据结构及关系

```
struct wpa_global变量和 struct nl80211_global 整个程序中只有一个，struct wpa_global变量通过iface指向struct wpa_supplicant链表，
链表中的每个struct wpa_supplicant变量的global_drv_priv和struct wpa_global变量的drv_priv[i] 都指向了同一个struct nl80211_global
struct wpa_global.drv_priv[i]   =  struct wpa_supplicant.global_drv_priv   =  struct nl80211_global *
struct wpa_global.iface = struct wpa_supplicant *





对于每个struct wpa_supplicant变量，都有一个struct i802_bss变量和一个struct wpa_driver_nl80211_data变量
struct wpa_supplicant.global_drv_priv  =  struct nl80211_global*
struct wpa_supplicant.drv_priv         =  struct i802_bss*



struct i802_bss.drv        = struct wpa_driver_nl80211_data*
struct i802_bss.ctx        = struct wpa_supplicant *
struct i802_bss.nl_cb      = struct nl_cb *
struct i802_bss.nl_connect = struct nl_sock *

struct wpa_driver_nl80211_data.global        = struct nl80211_global *
struct wpa_driver_nl80211_data.first_bss     = struct i802_bss *
struct wpa_driver_nl80211_data.ctx           = struct wpa_supplicant *
struct wpa_driver_nl80211_data.eapol_tx_sock = socket(PF_PACKET, SOCK_DGRAM, 0)


struct nl80211_global.nl=struct nl_sock *
struct nl80211_global.nl_event=struct nl_sock *
struct nl80211_global.nl_cb = nl_cb_alloc(NL_CB_DEFAULT);
struct nl80211_global.ctx=struct wpa_global *
struct nl80211_global.ioctl_sock = socket(PF_INET, SOCK_DGRAM, 0)
struct nl80211_global.ioctl_sock = socket(PF_INET, SOCK_DGRAM, 0)
struct nl80211_global.netlink.sock= socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE)
struct nl80211_global.netlink.cfg.newlink_cb = wpa_driver_nl80211_event_rtm_newlink
struct nl80211_global.netlink.cfg.dellink_cb = wpa_driver_nl80211_event_rtm_dellink
struct nl80211_global.netlink.cfg.ctx=struct nl80211_global *




```









## 6.app-frameworks-hal主线调用关系

### 6.1 打开wifi

#### 6.1.1 WifiManager.setWifiEnabled

WifiManager.getWifiState()

WifiManager.setWifiEnabled(true/false)   



![WifiManager.setWifiEnabled](wifi.assets/WifiManager.setWifiEnabled.jpg)



WifiManager.setWifiEnabled()通过AIDL 调用到服务实现端WifiServiceImpl.setWifiEnabled():

![image-20220328185547654](wifi.assets/image-20220328185547654.png)

WifiServiceImpl.setWifiEnabled()经过对APK权限检查后调用到ActiveModeWarden.wifiToggled()：

![image-20220328185829987](wifi.assets/image-20220328185829987.png)

ActiveModeWarden.wifiToggled()调用WifiController.sendMessage()。
WifiController 继承StateMachine，
StateMachine类中包含SmHandler类，且StateMachine的sendMessage方法直接调用SmHandler类的sendMessage方法。
SmHandler继承Handler类，最终这里调用到了Handler.sendMessage向handlerThread线程的MessageQueue中放入一个Message。
之后一路返回，AIDL服务端负责实现的函数调用完毕。


#### 6.1.2 WifiController层次状态机
handlerThread线程专门用来对StateMachine进行状态切换。当MessageQueue不为空时从休眠中醒来，从MessageQueue中取出Message后调用Handler的handleMessage方法来处理。SmHandler重写了Handler的handleMessage方法：

StateMachine.SmHandler.handleMessage(Message msg)方法定义如下：

![image-20220328191248195](wifi.assets/image-20220328191248195.png)

StateMachine.SmHandler.processMsg(Message msg)从当前状态往上向其父状态出发，不断调用它们的processMessage方法。



![image-20220328191706005](wifi.assets/image-20220328191706005.png)



curStateInfo.state是WifiController内部当前所处状态，在WifiController层次状态机构造后，调用其重写的start方法启动层次状态机时

start方法会根据参数调用setInitialState将状态机的初始状态配置成DisabledState。

![image-20220328193201116](wifi.assets/image-20220328193201116.png)





类关系：

DisabledState和EnabledState 都继承BaseState，BaseState继承State

DefaultState继承State



层次状态机状态关系见WifiController状态机的构造器：

![image-20220328193116631](wifi.assets/image-20220328193116631.png)



由于curStateInfo.state是DefaultState，因此StateMachine.SmHandler.processMsg(Message msg)在循环中调用到了DefaultState.processMessage。DefaultState中没有定义processMessage方法，该方法继承自BaseState，BaseState.processMessage(Message msg)如下：

![image-20220328193931426](wifi.assets/image-20220328193931426.png)

BaseState.processMessage(Message msg)调用的BaseState.processMessageFiltered(Message msg)方法是一个抽象方法，在

DisabledState中对其进行了重写实现：

![image-20220328194153483](wifi.assets/image-20220328194153483.png)

DisabledState.processMessageFiltered()根据命令CMD_WIFI_TOGGLED做如下事情:

1、ActiveModeWarden.startClientModeManager()

​       十分重要，见下节分析。



2、StateMachine.SmHandler.transitionTo(EnableState)

​      切换WifiController层次状态机到EnableState状态



3、返回HANDLED

​      由于消息被处理，因此不用继续调用其父状态DefaultState状态的processMessage方法，之后一路返回，在StateMachine.SmHandler.handleMessage中调用完processMsg(msg)处理消息时进行了状态切换，后续调用StateMachine.SmHandler.performTransitions(State msgProcessedState, Message msg)时通调用invokeExitMethods(commonStateInfo)间接调用到DisabledState.exit()，通过调用invokeEnterMethods(stateStackEnteringIndex)调用到EnabledState.enter()。在调完后返回到StateMachine.SmHandler.handleMessage，然后一路返回，WifiController的HandlerThread处理线程处理完了当前Message,继续阻塞接收处理MessageQueue中的消息。

#### 6.1.3 ClientModeStateMachine层次状态机

***ActiveModeWarden.startClientModeManager()函数如下***：

![image-20220328211617168](wifi.assets/image-20220328211617168.png)

startClientModeManager主要做三件事：

##### ***1、mWifiInjector.makeClientModeManager():***

![image-20220328212045637](wifi.assets/image-20220328212045637.png)

ClientModeManager的构造器如下：

![image-20220328212155734](wifi.assets/image-20220328212155734.png)

ClientModeStateMachine继承StateMachine，也是一个状态机，它有IdleState、StartedState、ScanOnlyModeState、ConnectModeState四个状态，查看器构造器，四个状态的***层次树继承关系***和***初始状态***如下：

![image-20220328212446517](wifi.assets/image-20220328212446517.png)

addState后直接调用ClientModeStateMachine层次状态机的start方法开启动状态机，该方法继承自StateMachine，StateMachine.start()如下：

![image-20220329095047444](wifi.assets/image-20220329095047444.png)

该方法直接调用StateMachine.SmHandler.completeConstruction()

![image-20220329095404977](wifi.assets/image-20220329095404977.png)

obtainMessage是继承自Handler的方法，该方法创建并返回一个Message对象。sendMessageAtFrontOfQueue也是继承自Handler的方法，用来往HandlerThread线程的MessageQueue发送Message。



往HandlerThread线程收到Message后调用handleMessage方法：

![image-20220329101830596](wifi.assets/image-20220329101830596.png)

handleMessage方法前面已经提及过，但这里由于Message是SM_INIT_CMD走第二个分支，调用invokeEnterMethods(0)来调用到ConnectModeState.IdleState.enter()。此外由于没有进行状态切换，后面的performTransitions()也什么都没做。



##### ***2、manager.start()***

使用makeClientModeManager创建ClientModeManager对象后，接着调用ClientModeManager.start方法：

![image-20220328213349661](wifi.assets/image-20220328213349661.png)

此方法发送带有CMD_START命令的Message给ClientModeStateMachine层次状态机，ClientModeStateMachine层次状态机的HandlerThread线程从MessageQueue取出Message调用handleMessage进行处理，此时handleMessage走第一个分支调用processMsg进行处理。根据前面的分析，由于ClientModeStateMachine处于IdleState状态，因此processMsg会调用到ClientModeStateMachine.IdleState.processMessage(Message message)方法:

![image-20220329140223451](wifi.assets/image-20220329140223451.png)

ClientModeStateMachine.IdleState.processMessage(Message message)调用了WifiNative.setupInterfaceForClientInScanMode()方法，该方法十分关键，见后续章节分析。接着调用transitionTo(mScanOnlyModeState)切换到ScanOnlyModeState状态。最终返回HANDLED说明Message已经被处理。返回到handleMessage中调完processMsg后继续调用performTransitions(State msgProcessedState, Message msg)，由于进行了状态切换，因此在performTransitions中会调到invokeExitMethods(commonStateInfo)和invokeEnterMethods(stateStackEnteringIndex)。由于状态树层次关系，invokeExitMethods这里相当于什么也没做，**invokeEnterMethods调用了ScanOnlyModeState.enter()**。然后一路返回继续监听MessageQueue，Message(CMD_START)处理完毕。



##### 3、ClientModeStateMachine.ScanOnlyModeState.enter()

这个方法最终向WifiScanningServiceImpl.WifiSingleScanStateMachine发送了一个Message(CMD_ENABLE)，用来使能扫描控制状态机。

![image-20220408093858763](wifi.assets/image-20220408093858763.png)

##### 4、 AsyncChannel.sendMessage

AsyncChannel.sendMessage如下：

![image-20220408103921627](wifi.assets/image-20220408103921627.png)

调用重载的sendMessage方法：

![image-20220408104041600](wifi.assets/image-20220408104041600.png)

显然消息发往何处取决于mAsyncChannel对象内部Messenger类型的mDestMessenger对象被赋了什么值，mAsyncChannel是mWifiScanner对象的内部类，查看WifiScanner构造函数：

![image-20220408105937391](wifi.assets/image-20220408105937391.png)

AsyncChannel.connect如下：

![image-20220408105347864](wifi.assets/image-20220408105347864.png)

AsyncChannel.connect如下：

![image-20220408105418760](wifi.assets/image-20220408105418760.png)

真相大白，mDestMessenger对象通过mWifiScanner对象构造时传入的IWifiScanner接口对象的getMessenger()方法获得。这个传入的IWifiScanner接口的对象其实是一个BpWifiScanningServiceImpl代理对象，通过AIDL与BnWifiScanningServiceImpl服务实现对象进行通信，他们都实现了IWifiScanner接口。BnWifiScanningServiceImpl.getMessenger如下：

![image-20220408112847891](wifi.assets/image-20220408112847891.png)

**Messenger本身是一个AIDL interface，这里new 出了一个Messenger实现端，然后返回，在WifiScanner构造函数使用mService.getMessenger()其实就是使用代BpWifiScanningServiceImpl代理对象的getMessenger方法获得一个Messenger的代理对象，这个Messenger的代理对象的实现端在BnWifiScanningServiceImpl所在的进程中，且这个实现端构造时传入了一个ClientHandler类型的对象。ClientHandler是WifiScanningServiceImpl的内部类，ClientHandler继承自WifiHandler，WifiHandler继承Handler，ClientHandler重写了handleMessage(Message msg)方法。**

**根据Messenger的特性，当AsynChanel.sendMessage(CMD_ENABLE)发送Message(CMD_ENABLE)后，在ClientHandler的Looper所在线程中将调用ClientHandler的重写的handleMessage(Message msg)来处理消息：**  

frameworks/opt/net/wifi/service/java/com/android/server/wifi/scanner/WifiScanningServiceImpl.java

```
84     public class WifiScanningServiceImpl extends IWifiScanner.Stub {
......
246      private class ClientHandler extends WifiHandler {
......
252          @Override
253          public void handleMessage(Message msg) {
......
328              switch (msg.what) {
329                  case WifiScanner.CMD_ENABLE:
330                      Log.i(TAG, "Received a request to enable scanning, UID = " + msg.sendingUid);
331                      setupScannerImpls();
332                      mBackgroundScanStateMachine.sendMessage(Message.obtain(msg));
333                      mSingleScanStateMachine.sendMessage(Message.obtain(msg));
334                      mPnoScanStateMachine.sendMessage(Message.obtain(msg));
335                      break;
......
367              }
368          }
369      }
```

其中关键的是**setupScannerImpls()和mSingleScanStateMachine.sendMessage(Message.obtain(msg));**

##### 5、 WifiScanningServiceImpl.setupScannerImpls()

![image-20220408170731289](wifi.assets/image-20220408170731289.png)

这个方法使用mScannerImplFactory.create创建一个WifiScannerImpl接口的对象，然后把这个对象放入mScannerImpls中，看WifiScanningServiceImpl的构造：

![image-20220408171031438](wifi.assets/image-20220408171031438.png)

frameworks/opt/net/wifi/service/java/com/android/server/wifi/scanner/WifiScanningService.java

![image-20220408171112113](wifi.assets/image-20220408171112113.png)

显然mScannerImplFactory是WifiScannerImpl.DEFAULT_FACTORY，再看WifiScannerImpl.DEFAULT_FACTORY：

![image-20220408171336303](wifi.assets/image-20220408171336303.png)

暂且不论wifiNative.getBgScanCapabilities返回的是true还是false，HalWifiScannerImpl其实是对WificondScannerImpl对象的一个封装，即后续调用HalWifiScannerImpl对象的一些方法时其实都是通过HalWifiScannerImpl对象内部的一个WificondScannerImpl对象完成的，HalWifiScannerImpl和WificondScannerImpl都继承WifiScannerImpl抽象类。**因此，后续进行扫描时，从mScannerImpls取出的WifiScannerImpl类的子对象可以认为就是WificondScannerImpl对象。**





WifiSingleScanStateMachine收到Message(CMD_ENABLE)后从初始的DefaultState切换成了IdleState，注意，由于刚开始没有PendingScans,因此在IdleState.enter()中调用tryToStartNewScan时该方法直接return，与后续在扫描AP时不同。



##### 6、mSingleScanStateMachine.sendMessage(Message.obtain(msg));

WifiSingleScanStateMachine在构造初始化后处于DefaultState ，因此在这里处理Message(CMD_ENABLE):

![image-20220408172714499](wifi.assets/image-20220408172714499.png)

处理很简单，直接切换到IdleState，根据层次状态机原理以及IdleState与DefaultState的层次关系，当调完DefaultState.processMessage(Message msg)后由于状态发送了变化会调用到IdleState.enter()：

![image-20220408172954103](wifi.assets/image-20220408172954103.png)

IdleState.enter()直接调用WifiScanningServiceImpl.tryToStartNewScan()：

![image-20220408173140541](wifi.assets/image-20220408173140541.png)

由于mPendingScans.size() == 0，因此直接返回。







##### 7、switchClientModeManagerRole(manager)

在startClientModeManager()中调用到了ActiveModeWarden.switchClientModeManagerRole(@NonNull ClientModeManager modeManager)：



![image-20220329144111801](wifi.assets/image-20220329144111801.png)

根据前面分析，这里调用ClientModeManager.setRole(ActiveModeManager.ROLE_CLIENT_PRIMARY):

![image-20220329150006506](wifi.assets/image-20220329150006506.png)

最终发送Message(CMD_SWITCH_TO_CONNECT_MODE)给ClientModeStateMachine层次状态机，ClientModeStateMachine层次状态机的HandlerThread线程继续从MessageQueue取出Message调用handleMessage进行处理，此时handleMessage走第一个分支调用processMsg进行处理。由于前面在处理Message(CMD_START)时已经把状态机的状态切换成ScanOnlyModeState，因此processMsg会调用到ClientModeStateMachine.ScanOnlyModeState.processMessage(Message message)方法:

![image-20220329150513817](wifi.assets/image-20220329150513817.png)

显然ScanOnlyModeState.processMessage没法处理，返回NOT_HANDLED，然后processMsg会调用ScanOnlyModeState状态的父状态的processMessage方法进行处理，在ClientModeStateMachine层次状态机构造函数里面调用addState(mScanOnlyModeState, mStartedState)将ScanOnlyModeState的父状态设置成了StartedState状态，因此当ScanOnlyModeState.processMessage没法处理，返回NOT_HANDLED时processMsg会调用到StartedState.processMessage(Message message):

![image-20220329151412018](wifi.assets/image-20220329151412018.png)

根据switch判断，StartedState.processMessage(Message message)可以处理Message(CMD_SWITCH_TO_CONNECT_MODE)，在其中调用了WifiNative.switchClientInterfaceToConnectivityMode(@NonNull String ifaceName)进行切换，如果成功调用则最后调用transitionTo(mConnectModeState)把ClientModeStateMachine状态机切换到ConnectModeState状态。WifiNative.switchClientInterfaceToConnectivityMode(@NonNull String ifaceName)十分重要，见后续章节分析。



##### 8、关闭wifi

在ClientModeManager的构造器除了把looper作为参数new了一个ClientModeStateMachine外还new了一个DeferStopHandler对象，DeferStopHandler派生自WifiHandler， WifiHandler继承自Handler，ClientModeStateMachine继承自StateMachine，StateMachine的内部类SmHandler继承自Handler，显然looper的HandlerThread处理线程将同时处理DeferStopHandler和SmHandler这两个Handler发过来的消息。

DeferStopHandler最终调用continueToStopWifi()来关闭wifi。







#### 6.1.4 setupInterfaceForClientInScanMode()

根据前面章节分析，ClientModeStateMachine层次状态机处理Message(CMD_START)的过程中会调用WifiNative.setupInterfaceForClientInScanMode（）方法，该方法做如下事情：装载驱动，启动wpa_supplicant ，添加wpa_supplicant interface，获取驱动/模组信息，初始化成scan模式，wlan0 up

![image-20220408181731764](wifi.assets/image-20220408181731764.png)



##### 1、startHal()

startHal主要负责驱动装载，初始化wifi_hal的vendor func table

![image-20220505142521448](wifi.assets/image-20220505142521448.png)



![image-20220505142553409](wifi.assets/image-20220505142553409.png)



![image-20220505142657258](wifi.assets/image-20220505142657258.png)



![image-20220505143057750](wifi.assets/image-20220505143057750.png)

initIWifiIfNecessary中首先调用getWifiServiceMockable通过hwServiceManager获得Wifi服务的代理对象，然后往Wifi服务中注册hidl回调对象，最后调用Wifi::stopWifi()，回调对象和stopWifi方法暂时不去分析：

![image-20220505143125792](wifi.assets/image-20220505143125792.png)



![image-20220505143154533](wifi.assets/image-20220505143154533.png)

调用完initIWifiIfNecessary后回到startWifi方法，该方法中通过Wifi代理对象调用到Wifi服务端的start方法：

![image-20220505143249695](wifi.assets/image-20220505143249695.png)



注意startInternal方法中除了调用initializeModeControllerAndLegacyHal方法外还new了一个WifiChip对象，后续createStaInterface中获取驱动相关信息时会用到。

![image-20220505143609611](wifi.assets/image-20220505143609611.png)



initializeModeControllerAndLegacyHal方法在hardware/interfaces/wifi/1.4/default/wifi.cpp中实现：

![image-20220505143652427](wifi.assets/image-20220505143652427.png)

mode_controller\_和legacy_hal\_在Wifi对象构造时传入并赋值：

![image-20220505143827130](wifi.assets/image-20220505143827130.png)

来看下Wifi对象构造时传入了什么：

![image-20220505144018667](wifi.assets/image-20220505144018667.png)



因此initializeModeControllerAndLegacyHal方法中调用mode_controller_->initialize()最终调用到了：hardware/interfaces/wifi/1.4/default/wifi_mode_controller.cpp

![image-20220505144539301](wifi.assets/image-20220505144539301.png)



![image-20220505144649367](wifi.assets/image-20220505144649367.png)



wifi_load_driver是一个接口函数，不同厂家可能会修改加入自己的实现方案，例如rk平台和amlogic平台都进行了一些修改来满足不同wifi模组适配、驱动装载时设置参数等各种需求。以下是AOSP的实现，mtk并未进行修改。

![image-20220505144947267](wifi.assets/image-20220505144947267.png)



![image-20220505144123355](wifi.assets/image-20220505144123355.png)



![image-20220505144233506](wifi.assets/image-20220505144233506.png)



![image-20220505144316910](wifi.assets/image-20220505144316910.png)



##### 2、startSupplicant()

startSupplicant最终通过hidl接口向hwServiceManager获取supplicant服务的代理端，注意如果wpa_supplicant是lazy_hal，开机默认并未启动，当这里获取时hwServiceManager会读取xml文件发现supplicant确实是个hal服务，然后向init进程发送ctl.start系统属性设置请求来启动wpa_supplicant，init进程在开机时已经读取了wpa_supplicant的rc启动文件，在收到hwServiceManager设置ctl.start系统属性设置请求后按照rc文件中的描述去启动wpa_supplicant进程，wpa_supplicant进程启动后最终会向hwServiceManager进程注册supplicant服务，至此hwServiceManager再将supplicant服务的代理端返回给获取supplicant服务代理端的进程。

![image-20220505102057503](wifi.assets/image-20220505102057503.png)



![image-20220505102806859](wifi.assets/image-20220505102806859.png)



![image-20220505102833244](wifi.assets/image-20220505102833244.png)



![image-20220505102903542](wifi.assets/image-20220505102903542.png)



![image-20220505102949446](wifi.assets/image-20220505102949446.png)



![image-20220505103031444](wifi.assets/image-20220505103031444.png)



##### 3、mIfaceMgr.allocateIface

传入的参数是Iface.IFACE_TYPE_STA_FOR_SCAN，allocateIface方法使用传入的参数new一个WifiNative.Iface类型的对象

![image-20220505161906141](wifi.assets/image-20220505161906141.png)

![image-20220505161941832](wifi.assets/image-20220505161941832.png)



##### 4、createStaIface(iface)

使用wifi模组厂商提供的wifi_hal来跟驱动交互，读取驱动支持的virtual interface组合，判断是否能够创建该virtual interface, 结合需求找到需要创建的best virtual interface combo，然后可能在驱动中创建该virtual interface（注，目前只发现高通wifi_hal支持该功能，其他平台都没有在这里创建，因为驱动装载时已经默认创建了)。**最后返回该virtual interface 的名字，即网卡名。**



![image-20220505114108764](wifi.assets/image-20220505114108764.png)



![image-20220505114136031](wifi.assets/image-20220505114136031.png)



![image-20220505114206247](wifi.assets/image-20220505114206247.png)

注意，这里wifi chip info最终还是通过hidl接口调用到startHal时android.hardware.wifi@1.x-service中new的那个WifiChip对象。前面已经提及在Wifi::startInternal中new了一个WifiChip对象。

![image-20220505114232463](wifi.assets/image-20220505114232463.png)



![image-20220505114317051](wifi.assets/image-20220505114317051.png)



![image-20220505114638922](wifi.assets/image-20220505114638922.png)



通过HIDL调用到android.hardware.wifi@1.x-service进程中的实现端，注意JAVA端传入的lambda表达式，它有WifiStatus status, IWifiStaIface iface这两个参数。



![image-20220505114749492](wifi.assets/image-20220505114749492.png)

在validateAndCall的line 80调用WifiChip::createStaIfaceInternal，在validateAndCall的line 83使用调用WifiChip::createStaIfaceInternal得到的结果来调用hidl_cb，调用hidl_cb就相当于调用JAVA端的那个lambda表达式(通过hidl实现)，这样调用WifiChip::createStaIfaceInternal得到的结果(return的std::pair中的status和iface)就通过JAVA端lambda表达式的参数传入了hild client端，在lambda表达式中将 传入的status赋给了statusResp.value，将iface赋给了ifaceResp.value。

![image-20220506153308607](wifi.assets/image-20220506153308607.png)



![image-20220505191007108](wifi.assets/image-20220505191007108.png)



WifiNative.createStaIface的hidl服务实现端是WifiChip::createStaIfaceInternal，服务实现端做如下三件事：

* WifiChip::allocateStaIfaceName，用来获得一个virtual interface name，见下文分析。

* legacy_hal_.lock()->createVirtualInterface，可能会调用wifi hal创建该virtual interface, 一定会把所有的virtual interface相关数据与virtual interface name 对应放到一个map中。见下文分析。

* 创建WifiStaIface类型的iface对象，构造参数传入了先前获得的virtual interface name等信息。

* 把iface push_back到sta_iface\_这个vector中。

* 调用先前使用hidl注册到WifiChip中的callback对象。

* getFirstActiveWlanIfaceName()从该vector中返回第0个元素的Name，setActiveWlanIfaceNameProperty将该name写入wifi.active.interface系统属性。

* 将status和WifiStaIface类型的iface对象放入std::pair中返回。

  

WifiChip::createStaIfaceInternal中调用到了WifiChip::allocateStaIfaceName

![image-20220505191805641](wifi.assets/image-20220505191805641.png)



![image-20220505191227976](wifi.assets/image-20220505191227976.png)



![image-20220505191311420](wifi.assets/image-20220505191311420.png)

 在mtk平台，wifi.interface系统属性在system.prop中定义：

![image-20220506160335701](wifi.assets/image-20220506160335701.png)



WifiChip::createStaIfaceInternal中调用到了legacy_hal_.lock()->createVirtualInterface时，实际上调用到了WifiLegacyHal::createVirtualInterface

![image-20220505114901586](wifi.assets/image-20220505114901586.png)



进行扫描前需要有virtual interface。

目前只发现高通wifi模组的wifi_hal提供了WifiLegacyHal::wifi_virtual_interface_create函数，其他wifi模组的wifi_hal并未提供该函数。

注意wifi驱动装载时可能已经创建了默认的sta virtual interface，所以不需要实现建该virtual interface的方法。



mtk wifi模组的wifi_hal 未实现wifi_virtual_interface_create，因此调用默认函数返回WIFI_ERROR_NOT_SUPPORTED,看WifiLegacyHal::handleVirtualInterfaceCreateOrDeleteStatus如何处理返回结果：

![image-20220505192620572](wifi.assets/image-20220505192620572.png)

根据man手册，if_nametoindex可以根据网卡名返回网卡index顺序，如果没找到该网卡名则返回零，这样用来判断先前WifiChip::allocateStaIfaceName得到的网卡名(virtual interface)是否有效。如果有效则调用WifiLegacyHal::retrieveIfaceHandles

![image-20220506144758434](wifi.assets/image-20220506144758434.png)

WifiLegacyHal::retrieveIfaceHandles调用mtk提供的wifi_hal中的wifi_get_ifaces从global_handle\_中获得iface_handles数组，然后遍历该数组创建iface_name与iface_handles[i]的map映射iface_name_to_handle\_，即一个iface_name对应一个iface_handles[i]。然后返回WIFI_SUCCESS，最后一路返回到WifiChip::createStaIfaceInternal方法中。



**wifi模组厂家的wifi_hal中即使没提供WifiLegacyHal::wifi_virtual_interface_create函数，在WifiChip::createStaIfaceInternal中调用legacy_hal_.lock()->createVirtualInterface时也能得到WIFI_SUCCESS。**





##### 5、mWifiCondManager.setupInterfaceForClientMode

setupInterfaceForClientMode跟wificond进程和后续扫描的过程密切相关，扫描过程中用到的一个关键对象即IWifiScannerImpl代理对象就是在这里放入map中的，后续扫描过程从map中取出。

![image-20220412212230035](wifi.assets/image-20220412212230035.png)

调用链参考：

![image-20220505160211707](wifi.assets/image-20220505160211707.png)





##### 6、mSupplicantStaIfaceHal.setupIface(iface.name)

传入的virtual interface name 来自前面createStaIface的返回值，该方法主要是让framework和wpa_supplicant创建相关数据结构来描述驱动中已经存在的这个virtual interface。

 ![image-20220505163119517](wifi.assets/image-20220505163119517.png)



![image-20220505163214215](wifi.assets/image-20220505163214215.png)



![image-20220505163429155](wifi.assets/image-20220505163429155.png)



![image-20220505163538129](wifi.assets/image-20220505163538129.png)

后续参考第5章wpa_supplicant的分析。



#### 6.1.5 switchClientInterfaceToConnectivityMode()

根据前面章节分析，ClientModeStateMachine层次状态机处理Message(CMD_SWITCH_TO_CONNECT_MODE)的过程中会调用WifiNative.switchClientInterfaceToConnectivityMode(@NonNull String ifaceName)方法

***mtk把aosp中启动wpa_supplicant和创建wpa_supplicant vitrual interface的步骤被移动到了setupInterfaceForClientInScanMode中，即上一节中的 startSupplicant 和setupIface本来是在这里的switchClientInterfaceToConnectivityMode中调用的***



### 6.2 扫描AP



#### 6.2.1 WifiManager.startScan



WifiManager.startScan()通过AIDL调用到framework层服务实现端：



![image-20220414144606604](wifi.assets/image-20220414144606604.png)



AIDL调用到framework层服务实现端WifiServiceImpl.startScan()如下：



![image-20220329203108181](wifi.assets/image-20220329203108181.png)



WifiServiceImpl.startScan最主要调用WifiThreadRunner.call(），并传入了一个lambda表达式, WifiThreadRunner.call方法如下：



![image-20220329203039556](wifi.assets/image-20220329203039556.png)

WifiThreadRunner.call()主要调用WifiThreadRunner.runWithScissors(@NonNull Handler handler, @NonNull Runnable r,long timeout)方法，调用时第二个参数又传入了一个lambda表达式，根据runWithScissors形参列表可知第二个参数是Runnable 对象，因此第二次传入的lambda表达式的结果是一个Runnable 对象，由此反推出call方法中supplier.get()的返回值必须是Runnable 对象，因此call的形参列表中的第一个参数需要传入的类型是Supplier\<Runnable\> ,由此继续反推出调用call是传入的第一个lambda表达式的结果是Runnable，因此相当于从上往下传入了一个Runnable对象（记作A匿名对象），该对象继承并重写了run方法，在run方法内执行ScanRequestProxy.startScan(callingUid, packageName)。




WifiThreadRunner.runWithScissors方法如下：

![image-20220329203001380](wifi.assets/image-20220329203001380.png)



Looper是线程私有对象，该对象中有MessageQueue。

WifiThreadRunner.runWithScissors方法判断一下，BnWifiServiceImpl服务实现端所在线程的Looper，是否为WifiThreadRunner线程的handler中对应的handlerThread的Looper。显然这里不是同一个Looper,因此使用A匿名对象作为参数new一个BlockingRunnable对象，BlockingRunnable对象重写了run方法，并在run方法中调用A匿名对象的run()方法，然后把mDone设置成true。new BlockingRunnable对象后再把WifiThreadRunner线程的handler作为参数调用BlockingRunnable.postAndWait(Handler handler, long timeout)方法：

![image-20220329210444244](wifi.assets/image-20220329210444244.png)



BlockingRunnable.postAndWait(Handler handler, long timeout)方法首先使用handler.post(this)向handler对应的handlerThread线程发送Message(callback=this)，其中this是指WifiThreadRunner这个Runnable对象，当handlerThread线程接收到MessageQueue后调用WifiThreadRunner这个Runnable对象重写的run()方法来处理消息：   0

![image-20220329211846026](wifi.assets/image-20220329211846026.png)

而在WifiThreadRunner这个Runnable对象重写的run()方法又再次调用了WifiThreadRunner.runWithScissors方法，此次调用时由于Looper相等，直接调用A匿名对象的run()方法，待run()方法执行完毕后才返回true。



与此同时BnWifiServiceImpl线程调用完handler.post(this)发送完Message(callback=this)后接下来睡眠定时唤醒看mDone这个变量的值。当检测到为true时返回。

#### 6.2.2 ScanRequestProxy.startScan(int callingUid, String packageName)

经过前面的分析，A匿名对象的run()方法就是调用ScanRequestProxy.startScan(callingUid, packageName)，其实现如下：

![image-20220408103329192](wifi.assets/image-20220408103329192.png)

主要调用了WifiScanner.startScan:

![image-20220408103505251](wifi.assets/image-20220408103505251.png)

#### 6.2.3 WifiScanningServiceImpl.WifiSingleScanStateMachine扫描阶段处理

根据在打开wifi过程中对AsyncChanel的分析，将由某个线程从looper队列中把Message(CMD_START_SINGLE_SCAN)取出，然后调用到WifiScanningServiceImpl.ClientHandler.handleMessage(Message msg)来处理消息，ClientHandler的重写的handleMessage在处理Message(CMD_START_SINGLE_SCAN)时直接将其发送给了WifiScanningServiceImpl内部的WifiSingleScanStateMachine，WifiSingleScanStateMachine根据前面打开wifi流程的分析，现在处于IdleState，IdleState.processMessage无法处理，因此由其父状态DriverStartedState的processMessage进行处理，DriverStartedState.processMessage来处理，DriverStartedState.processMessage处理Message(CMD_START_SINGLE_SCAN)时先判断当前是否处于ScanningState状态，如果是则直接mPendingScans.addRequest，否则mPendingScans.addRequest后还要调用WifiScanningServiceImpl.tryToStartNewScan()。

frameworks/opt/net/wifi/service/java/com/android/server/wifi/scanner/WifiScanningServiceImpl.java

```
84     public class WifiScanningServiceImpl extends IWifiScanner.Stub {
......
246      private class ClientHandler extends WifiHandler {
......
252          @Override
253          public void handleMessage(Message msg) {
......
328              switch (msg.what) {
......
351                  case WifiScanner.CMD_START_SINGLE_SCAN:
352                  case WifiScanner.CMD_STOP_SINGLE_SCAN:
353                      mSingleScanStateMachine.sendMessage(Message.obtain(msg));
......
367              }
368          }
369      }
......
396      private WifiSingleScanStateMachine mSingleScanStateMachine;
......
626      class WifiSingleScanStateMachine extends StateMachine {
......
879          class DriverStartedState extends State {
......
892              @Override
893              public boolean processMessage(Message msg) {
......
896                  switch (msg.what) {
897                      case WifiScanner.CMD_ENABLE:
898                          // Ignore if we're already in driver loaded state.
899                          return HANDLED;
900                      case WifiScanner.CMD_START_SINGLE_SCAN:
......
939                              if (getCurrentState() == mScanningState) {
940                                  if (activeScanSatisfies(scanSettings)) {
941                                      mActiveScans.addRequest(ci, handler, workSource, scanSettings);
942                                  } else {
943                                      mPendingScans.addRequest(ci, handler, workSource, scanSettings);
944                                  }
945                              } else {
946                                  mPendingScans.addRequest(ci, handler, workSource, scanSettings);
947                                  tryToStartNewScan();
948                              }
......
961                  }
962              }
963          }
964  
965          class IdleState extends State {
......
971              @Override
972              public boolean processMessage(Message msg) {
973                  return NOT_HANDLED;
974              }
975          }
......
2678  }
```



WifiScanningServiceImpl.tryToStartNewScan()实现如下：

与前面打开wifi是通过IdleState.enter()调用tryToStartNewScan时不同，此时由于mPendingScans.addRequest，tryToStartNewScan不直接返回，而是去调用

WifiScanningServiceImpl.WifiSingleScanStateMachine.ScannerImplsTracker.startSingleScan(WifiNative.ScanSettings scanSettings)。

![image-20220408120554388](wifi.assets/image-20220408120554388.png)

WifiScanningServiceImpl.WifiSingleScanStateMachine.ScannerImplsTracker.startSingleScan(WifiNative.ScanSettings scanSettings)主要是从mScannerImpls中取出WifiScannerImpl抽象类的子对象，根据前面打开wifi时的分析，可以认为该对象就是WificondScannerImpl对象，然后调用WificondScannerImpl对象的startSingleScan方法。**注意，调用WificondScannerImpl.startSingleScan时第二个参数传入了一个WifiNative.ScanEventHandler对象,后续扫描结果通过这个对象返回**

WifiScanningServiceImpl.WifiSingleScanStateMachine.ScannerImplsTracker.startSingleScan(WifiNative.ScanSettings scanSettings)实现如下：

![image-20220413103704868](wifi.assets/image-20220413103704868.png)



WificondScannerImpl.startSingleScan() 首先使用传入的第二个参数也就是WifiNative.ScanEventHandler来new一个mLastScanSettings对象，其类型是LastScanSettings，然后再调用WifiNative.scan

frameworks/opt/net/wifi/service/java/com/android/server/wifi/scanner/WificondScannerImpl.java

![image-20220413101127700](wifi.assets/image-20220413101127700.png)







WifiNative.scan调用WifiCondManager.startScan

![image-20220408180715801](wifi.assets/image-20220408180715801.png)



***注意，MTK增加了对GBK编码的隐藏wifi名的支持，如果发现String类型SSID是中文，则除了将其转换成UTF-8编码的byte[]外，还将其转换成GBK编码的byte[]，然后再将这两个不同的byte[]放入hiddenNetworkSsidArrays变量中。***



mWifiCondManager是WifiNl80211Manager 类型的对象，因此调用到了WifiNl80211Manager.startScan:

frameworks/base/wifi/java/android/net/wifi/nl80211/WifiNl80211Manager.java

![image-20220408181115628](wifi.assets/image-20220408181115628.png)

WifiNl80211Manager.getScannerImpl如下：

![image-20220408181226918](wifi.assets/image-20220408181226918.png)

前面打开wifi时，在setupInterfaceForClientInScanMode中调用了WifiCondManager.setupInterfaceForClientMode，在其中把一个IWifiScannerImpl代理对象放入了mWificondScanners中，这里调用getScannerImpl将其取出并调用IWifiScannerImpl代理对象的scan方法，IWifiScannerImpl代理对象的实现端在wificond进程中。最终通过AIDL调用wificond进程提供的方法来使用netlink套接字发送数据给驱动。wificond进程对于扫描这块的处理参考第3节。



#### 6.2.4 BnWificond服务注册

1、创建一个android::wificond::LooperBackedEventLoop的对象event_dispatcher。

2、给event_dispatcher添加需要监听的binder通信的fd。

3、创建一个android::wificond::NetlinkManager类的netlink_manager对象，在NetlinkManager的构造函数中把传入的event_dispatcher对象保存起来。

4、调用netlink_manager.Start()创建netlink套接字并给event_dispatcher对象添加需要监听的netlink套接字fd。

5、创建android::wificond::Server类(继承android::net::wifi::nl80211::BnWificond)的对象server。

6、调用RegisterServiceOrCrash(server)，在该方法中获取BpServiceManager对象，调用addService(android::String16(kServiceName), service)把BnWificond服务实现端注册到ServiceManager。其服务名字kServiceName是"wifinl80211"

7、调用event_dispatcher->Poll()开始循环监听fd，如果有事件则从阻塞状态醒来去处理事件。



​                                                      system/connectivity/wificond/main.cpp

![image-20220411102823772](wifi.assets/image-20220411102823772.png)

其中NetlinkManager::Start()非常关键：

1、创建sync_netlink_fd\_和async_netlink_fd\_这两个netlink套接字

2、调用DiscoverFamilyId，最终先调用NetlinkManager::SendMessageAndGetResponses方法，该方法通过sync_netlink_fd\_  发送信息，poll等待，返回response，然后再调用NetlinkManager::OnNewFamily来处理返回的response。

3、WatchSocket(&async_netlink_fd\_)   把async_netlink_fd\_ 放入epoll的监听队列，main函数最终调用event_dispatcher->Poll()就是监听这个队列中的fd并调用相关处理函数进行处理。在WatchSocket中指定了async_netlink_fd\_的处理函数是NetlinkManager::ReceivePacketAndRunHandler

4、通过async_netlink_fd\_ 订阅NL80211_MULTICAST_GROUP_REG，NL80211_MULTICAST_GROUP_SCAN、NL80211_MULTICAST_GROUP_MLME



![image-20220412172855413](wifi.assets/image-20220412172855413.png)



![image-20220412174725454](wifi.assets/image-20220412174725454.png)



![image-20220412174824704](wifi.assets/image-20220412174824704.png)





![image-20220412174651295](wifi.assets/image-20220412174651295.png)





#### 6.2.5 android::wificond::Server::createClientInterface



直接new一个ClientInterfaceImpl对象，然后调用GetBinder()方法， 将其返回给 created_interface。



![image-20220411104911845](wifi.assets/image-20220411104911845.png)





GetBinder方法直接返回binder_







![image-20220411113140452](wifi.assets/image-20220411113140452.png)





binder_在ClientInterfaceImpl对象构造时指向了一个new出来的ClientInterfaceBinder对象。



![image-20220411115713818](wifi.assets/image-20220411115713818.png)





因此在其他进程调用createClientInterface时会得到一个ClientInterfaceBinder代理对象。





#### 6.2.6 android::wificond::ClientInterfaceBinder::getWifiScannerImpl



getWifiScannerImpl方法直接调用impl_的GetScanner()方法：system/connectivity/wificond/client_interface_binder.cpp

![image-20220411115126225](wifi.assets/image-20220411115126225.png)

impl_就是上节new ClientInterfaceBinder对象时传入的ClientInterfaceImpl对象，在ClientInterfaceBinder构造函数中保存到了impl\_

![image-20220411115158014](wifi.assets/image-20220411115158014.png)

ClientInterfaceImpl对象的GetScanner方法定义如下：system/connectivity/wificond/client_interface_impl.h

![image-20220411115750546](wifi.assets/image-20220411115750546.png)

ClientInterfaceImpl对象的GetScanner方法直接返回 scanner_     ，注意前面ClientInterfaceImpl的构造方法中把scanner_     指向了new出来的一个ScannerImpl对象。因此最终getWifiScannerImpl通过out_wifi_scanner_impl参数返回的是一个ScannerImpl对象。



#### 6.2.7 android::wificond::ScannerImpl::scan

system/connectivity/wificond/scanning/scanner_impl.cpp

![image-20220411175447849](wifi.assets/image-20220411175447849.png)



system/connectivity/wificond/scanning/scan_utils.cpp

注意，在栈上创建trigger_scan对象时其构造函数传入了NL80211_CMD_TRIGGER_SCAN



![image-20220411175614382](wifi.assets/image-20220411175614382.png)







![image-20220412175140580](wifi.assets/image-20220412175140580.png)



![image-20220412175210705](wifi.assets/image-20220412175210705.png)





![image-20220412175253123](wifi.assets/image-20220412175253123.png)



![image-20220412175327462](wifi.assets/image-20220412175327462.png)







​                                  system/connectivity/wificond/net/netlink_manager.cpp

![image-20220412201415008](wifi.assets/image-20220412201415008.png)



![image-20220623142937134](wifi.assets/image-20220623142937134.png)





​                                                   system/connectivity/wificond/net/netlink_manager.cpp

![image-20220412211459749](wifi.assets/image-20220412211459749.png)



从on_scan_result_ready_handler_这个std::map中通过if_index  find出来的是一个std::pair对象(即handler)，根据std::pair，first指key对象，即if_index  ，second指map中key对应的value对象。 根据system/connectivity/wificond/net/netlink_manager.h

```
75  typedef std::function<void(
76      uint32_t interface_index,
77      bool scan_stopped)> OnSchedScanResultsReadyHandler;
......
310    std::map<uint32_t, OnScanResultsReadyHandler> on_scan_result_ready_handler_;
```

显然这个value对象其实就是一个OnSchedScanResultsReadyHandler类型的对象。handler->second(if_index, aborted, ssids, freqs)其实就是调用OnSchedScanResultsReadyHandler类型的对象的operate()方法（也就是std::function指向的函数或类方法）。



查看ScannerImpl的构造方法可以发现调用了ScanUtils::SubscribeScanResultNotification

![image-20220412215913916](wifi.assets/image-20220412215913916.png)

ScanUtils::SubscribeScanResultNotification调用了NetlinkManager::SubscribeScanResultNotification

![image-20220412220058102](wifi.assets/image-20220412220058102.png)



NetlinkManager:：SubscribeScanResultNotification就是把OnSchedScanResultsReadyHandler类型的对象放入on_scan_result_ready_handler_这个std::map的地方，参考system/connectivity/wificond/net/netlink_manager.cpp

```
791  void NetlinkManager::SubscribeScanResultNotification(
792      uint32_t interface_index,
793      OnScanResultsReadyHandler handler) {
794    on_scan_result_ready_handler_[interface_index] = handler;
795  }
```



因此OnSchedScanResultsReadyHandler这个std::function最终在ScannerImpl的构造方法中通过std::bind实例化，指向了ScannerImpl::OnScanResultsReady，故最终handler->second(if_index, aborted, ssids, freqs)就是调用ScannerImpl::OnScanResultsReady

![image-20220412220748795](wifi.assets/image-20220412220748795.png)

在ScannerImpl::OnScanResultsReady中直接调用scan_event_handler_->OnScanResultReady()，**打开wifi时在framework层调用了setupInterfaceForClientMode，setupInterfaceForClientMode中new 一个ScanEventHandler对象(该类实现了IScanEvent接口)，然后调用wificondScanner.subscribeScanEvents()，把ScanEventHandler对象AIDL代理端传给wificond进程。wificondScanner在wificond进程中的实现端是ScannerImpl，看一下它的subscribeScanEvents方法：**

![image-20220412213611398](wifi.assets/image-20220412213611398.png)

ScannerImpl::subscribeScanEvents将通过AIDL传入的ScanEventHandler代理对象赋给了scan_event_handler\_，因此最终

scan_event_handler\_->OnScanResultReady()实际上是通过ScanEventHandler代理对象调到了AIDL实现端的ScanEventHandler对象的OnScanResultReady

![image-20220412221520731](wifi.assets/image-20220412221520731.png)



使用java.util.concurrent.Executor.execute去调用用于ScanEventCallback接口的对象的onScanResultReady()方法，这个callback对象在ScanEventHandler构造时传入，ScanEventHandler在setupInterfaceForClientMode中被new出来，new的时候使用了setupInterfaceForClientMode的形参，即scanCallback，再看

WifiNative.setupInterfaceForClientInScanMode中调用setupInterfaceForClientMode传入的callback对象：

![image-20220412222608940](wifi.assets/image-20220412222608940.png)

最终调到了NormalScanEventCallback.onScanResultReady()

![image-20220412222741164](wifi.assets/image-20220412222741164.png)

NormalScanEventCallback.onScanResultReady中最终调到了WifiMonitor.broadcastScanResultEvent(String iface)

![image-20220412222940648](wifi.assets/image-20220412222940648.png)



Message(SCAN_RESULT_EVENT) 在WificondScannerImpl.handleMessage(Message msg)终被处理:

![image-20220412223116394](wifi.assets/image-20220412223116394.png)

处理时先取消超时限制，然后调用WificondScannerImpl.pollLatestScanData()

![image-20220413102951221](wifi.assets/image-20220413102951221.png)





pollLatestScanData中首先调用WifiNative.getScanResults获取信息:

frameworks/opt/net/wifi/service/java/com/android/server/wifi/WifiNative.java

![image-20220412224329937](wifi.assets/image-20220412224329937.png)



WifiNl80211Manager.::getScanResults

![image-20220412224513346](wifi.assets/image-20220412224513346.png)



![image-20220412224625769](wifi.assets/image-20220412224625769.png)





![image-20220412224829133](wifi.assets/image-20220412224829133.png)



再回看WificondScannerImpl.pollLatestScanData()，在调用完WifiNative.getScanResults之后拿到结果，对结果进行处理后放到mLatestSingleScanResult这个 WifiScanner.ScanData类型的对象中，最后调用:

```
396                  mLastScanSettings.singleScanEventHandler
397                          .onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);
```

mLastScanSettings.singleScanEventHandler保存的就是先前在WifiScanningServiceImpl.WifiSingleScanStateMachine.ScannerImplsTracker.startSingleScan中调用impl.startSingleScan时传入的第二个参数:

```
747                      boolean success = impl.startSingleScan(
748                              scanSettings, new ScanEventHandler(ifaceName));
```

因此pollLatestScanData的最后相当于调用了：WifiScanningServiceImpl.WifiSingleScanStateMachine.ScannerImplsTracker.ScanEventHandler.onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE)  

![image-20220413104141252](wifi.assets/image-20220413104141252.png)

根据事件类型调用到了WifiScanningServiceImpl.WifiSingleScanStateMachine.ScannerImplsTracker.WifiScanningServiceImpl.WifiSingleScanStateMachine.ScannerImplsTracker.reportScanStatusForImpl

![image-20220413104354197](wifi.assets/image-20220413104354197.png)



#### 6.2.8 WifiScanningServiceImpl.WifiSingleScanStateMachine扫描结果处理

在经过前面的分析，扫描阶段WifiSingleScanStateMachine处于ScanningState状态，此时看这个状态如何处理Message(CMD_SCAN_RESULTS_AVAILABLE)

frameworks/opt/net/wifi/service/java/com/android/server/wifi/scanner/WifiScanningServiceImpl.java

![image-20220413105023693](wifi.assets/image-20220413105023693.png)

在处理Message(CMD_SCAN_RESULTS_AVAILABLE)时首先调用ScannerImplsTracker.getLatestSingleScanResults()

![image-20220413112058050](wifi.assets/image-20220413112058050.png)



![image-20220413112223017](wifi.assets/image-20220413112223017.png)



直接返回mLatestSingleScanResult，注意这个mLatestSingleScanResult，先前WificondScannerImpl.pollLatestScanData()方法中，在调用完WifiNative.getScanResults之后拿到结果，对结果进行处理后放到mLatestSingleScanResult这个 WifiScanner.ScanData类型的对象中。



再回看WifiScanningServiceImpl.WifiSingleScanStateMachine.ScanningState.processMessage(Message msg),调用getLatestSingleScanResults获取到结果后再调用WifiScanningServiceImpl.reportScanResults(@NonNull ScanData results)来处理获取到的扫描结果，最后调用transitionTo(mIdleState)切换到IdleState根据前面打开wifi时切换到IdleState的分析，在IdleState.enter()中调用tryToStartNewScan(); 在tryToStartNewScan中首先判断一下是否还有如果pending scan requests，如果没有则直接返回。



再来看WifiScanningServiceImpl.reportScanResults

![image-20220413113855724](wifi.assets/image-20220413113855724.png)

entry的类型是RequestInfo\<ScanSettings\>  使用它的reportEvent方法来发送Message(CMD_SCAN_RESULT)  ：

![image-20220413143433450](wifi.assets/image-20220413143433450.png)



RequestInfo\<ScanSettings\>的reportEvent方法直接调用RequestInfo\<ScanSettings\>对象构造时传入的ClientInfo对象的reportEvent方法。

在tryToStartNewScan之前调了mPendingScans.addRequest(ci, handler, workSource, scanSettings); addRequest如下：

![image-20220413145026816](wifi.assets/image-20220413145026816.png)



![image-20220413175419848](wifi.assets/image-20220413175419848.png)

ClientInfo对象从mClients这个map中取出，来看下它是什么时候调用放进去的：

![image-20220413175518685](wifi.assets/image-20220413175518685.png)

register()方法将this当前对象放入mClients这个map，其key是this当前对象中的mMessenger对象。再看下register的调用地方：

frameworks/opt/net/wifi/service/java/com/android/server/wifi/scanner/WifiScanningServiceImpl.java

![image-20220413180020981](wifi.assets/image-20220413180020981.png)



在WifiScanner构造时发送的Message(CMD_CHANNEL_FULL_CONNECTION)就是在这里处理。

上文所述ClientInfo接口的对象其实是在处理Message(CMD_CHANNEL_FULL_CONNECTION)时new出来的一个ExternalClientInfo对象，new的时候传入了ac(AsyncChanel类型)。ExternalClientInfo构造时把传入的AsyncChanel类型的ac保存到mChannel中，后面reportEvent方法使用mChannel来发送Message。ExternalClientInfo类如下：

![image-20220413180317524](wifi.assets/image-20220413180317524.png)

综上，WifiScanningServiceImpl.reportScanResults最终通过ExternalClientInfo.reportEvent调用AsyncChannel.sendMessage来发送Message(CMD_SCAN_RESULT) 。



ac在传入ExternalClientInfo构造函数做参数之前调用了connected：

![image-20220413181845430](wifi.assets/image-20220413181845430.png)





之前scan时使用mAsyncChannel.sendMessage方法给WifiSingleScanStateMachine发Message,

![image-20220414092423146](wifi.assets/image-20220414092423146.png)







在AsyncChannel.sendMessage中把replyTo = mSrcMessenger

![image-20220413183159918](wifi.assets/image-20220413183159918.png)



最终有如下关系

ac.mSrcHandler      = mAsyncChannel.mDstMessenger = ClientHandler对象     

ac.mDstMessenger = mAsyncChannel.mSrcHandler      = ServiceHandler对象

Message(CMD_START_SINGLE_SCAN)发往ClientHandler ，msg.replayTo 就是 ServiceHandler

Message(CMD_SCAN_RESULT)发往ServiceHandler，msg.replayTo 就是ClientHandler 



来看ServiceHandler.handleMessage如何处理Message(CMD_SCAN_RESULT)

![image-20220413120657891](wifi.assets/image-20220413120657891.png)



ServiceHandler.handleMessage首先调用getListenerWithExecutor从mListener这个map中获取listener。获得listener后使用executor.execute来调用它的onResult方法。

![image-20220413185021014](wifi.assets/image-20220413185021014.png)



![image-20220413185445502](wifi.assets/image-20220413185445502.png)





来看下listener如何放入mListenerMap中，在ScanRequestProxy.startScan时调用了retrieveWifiScannerIfNecessary()

![image-20220413185849590](wifi.assets/image-20220413185849590.png)



ScanRequestProxy.retrieveWifiScannerIfNecessary(）new了一个GlobalScanListener对象并将其作为第二个参数调用WifiScanner.registerScanListener :

![image-20220413190225214](wifi.assets/image-20220413190225214.png)



WifiScanner.registerScanListener(@NonNull @CallbackExecutor Executor executor, @NonNull ScanListener listener)调用WifiScanner.addListener，addListener传入的参数GlobalScanListener对象。

![image-20220413190556242](wifi.assets/image-20220413190556242.png)

WifiScanner.addListener调用WifiScanner.putListener，最终WifiScanner.putListener把GlobalScanListener对象放入了mListenerMap中。

![image-20220413190911469](wifi.assets/image-20220413190911469.png)



故最终ServiceHandler.handleMessage首先调用getListenerWithExecutor从mListener这个map中获取listener就是这个GlobalScanListener对象,接着使用executor.execute来调用GlobalScanListener.onResult方法，扫描结果作为onResult的参数:

![image-20220413191402302](wifi.assets/image-20220413191402302.png)

GlobalScanListener.onResult直接把扫描结果放到mLastScanResults中。最后发广播通知扫描结果已经就位。



### 6.3 获取扫描结果

#### 6.3.1 WifiManager.getScanResults()

APP收到扫描结果就位的广播后调用WifiManager.getScanResults()来获取扫描结果，

![image-20220414144946142](wifi.assets/image-20220414144946142.png)



WifiManager.getScanResults()的AIDL实现端是WifiManagerImpl.getScanResults() :

![image-20220413191517577](wifi.assets/image-20220413191517577.png)

ScanRequestProxy.getScanResults直接从mLastScanResults中读取先前放入的扫描结果：

![image-20220413191559976](wifi.assets/image-20220413191559976.png)



### 6.4添加网络

#### 6.4.1 WifiManager.addNetwork(WifiConfiguration  config)

WifiManager.addNetwork(WifiConfiguration  config);//返回networkid

![image-20220414145140959](wifi.assets/image-20220414145140959.png)

WifiManager.addNetwork直接调用WifiManager.addOrUpdateNetwork(WifiConfiguration config)

![image-20220414145228409](wifi.assets/image-20220414145228409.png)

WifiManager.addOrUpdateNetwork的AIDL实现端如下：







###  6.5 开始连接

#### 6.5.1 WifiManager.connect

WifiManager.connect(@NonNull WifiConfiguration config, @Nullable ActionListener listener)

WifiManager.connect(int networkId, @Nullable ActionListener listener)

两个方法都调用WifiManager.connectInternal(@Nullable WifiConfiguration config, int networkId,@Nullable ActionListener listener)

使用networkId的版本调用WifiManager.connectInternal时WifiConfiguration类型的config对象传入的是null

![image-20220414152257882](wifi.assets/image-20220414152257882.png)





connectInternal调用WifiServiceImpl.connect(WifiConfiguration config, int netId, IBinder binder,@Nullable IActionListener callback, int callbackIdentifier)

 



![image-20220414193045933](wifi.assets/image-20220414193045933.png)



在connectInternal中使用传入的listener回调对象作为构造参数new一个ActionListenerProxy类型的listenerProxy对象，然后把listenerProxy作为第四个参数调用WifiServiceImpl代理端的connect方法。根据AIDL原理调用到WifiServiceImpl.connect实现端：



![image-20220414193130376](wifi.assets/image-20220414193130376.png)



WifiServiceImpl.connect实现端调用ClientModeImpl.connect，把回调对象作为第四个参数传入





#### 6.5.2 WifiManager.enableNetwork

WifiManager.enableNetwork(networkid, true)也能去连接wifi，其实现如下

![image-20220414191335773](wifi.assets/image-20220414191335773.png)

WifiManager.enableNetwork通过AIDL调用到WifiServiceImpl.enableNetwork

![image-20220414191612907](wifi.assets/image-20220414191612907.png)

WifiServiceImpl.enableNetwork调用WifiServiceImpl.triggerConnectAndReturnStatus

![image-20220414191704876](wifi.assets/image-20220414191704876.png)

WifiServiceImpl.triggerConnectAndReturnStatus最终也通过ClientModeImpl.connect去连接wifi。只是与上节相比没法传入回调对象，因为这里传入ClientModeImpl.connect的第四个参数即回调对象已经在triggerConnectAndReturnStatus方法中写死了，而上节的是使用的是listenerProxy对象，该对象由WifiManager.connect传入的回调对象作为参数构造而成，因此WifiManager.connect可以自己定制回调对象。而WifiManager.enableNetwork不行。



#### 6.5.3 ClientModeImpl.connect

ClientModeImpl.connect方法根据config/netId来获得/构造一个NetworkUpdateResult类型的result对象，然后使用result对象构造一个Message(CMD_CONNECT_NETWORK，result)



![image-20220414194525778](wifi.assets/image-20220414194525778.png)





#### 6.5.4 ClientModeImpl层次状态机



根据打开wifi流程的分析，ClientModeImpl此时处于DisconnectedState，DisconnectedState父状态是ConnectModeState，ClientModeImpl.DisconnectedState.processMessage(Message message)没法处理Message(CMD_CONNECT_NETWORK，result)，接着由其父状态ConnectModeState的processMessage方法来处理，处理时从msg中取出result，从result取出netId，然后调用connectToUserSelectNetwork：



```
163  public class ClientModeImpl extends StateMachine {
......
3784      class ConnectModeState extends State {
......
3836          public boolean processMessage(Message message) {
......
3852              switch (message.what) {
......
4141                  case CMD_CONNECT_NETWORK:
4142                      callbackIdentifier = message.arg2;
4143                      result = (NetworkUpdateResult) message.obj;
4144                      netId = result.getNetworkId();
4145                      connectToUserSelectNetwork(
4146                              netId, message.sendingUid, result.hasCredentialChanged());
4147                      mWifiMetrics.logStaEvent(
4148                              StaEvent.TYPE_CONNECT_NETWORK,
4149                              mWifiConfigManager.getConfiguredNetwork(netId));
4150                      sendActionListenerSuccess(callbackIdentifier);
4151                      break;
......
4406              }
......
4413          }
4414      }
......
6615  }
```



ClientModeImpl.connectToUserSelectNetwork(int netId, int uid, boolean forceReconnect)，如下：



![image-20220414202631665](wifi.assets/image-20220414202631665.png)



ClientModeImpl.startConnectToNetwork(int networkId, int uid, String bssid)发送

Message(CMD_START_CONNECT, networkId, uid, SUPPLICANT_BSSID_ANY)：



![image-20220414203451354](wifi.assets/image-20220414203451354.png)

此时ClientModeImpl还是处于DisconnectedState状态，但ClientModeImpl.DisconnectedState.processMessage(Message message)没法处理Message，因此由其父状态ConnectModeState的processMessage方法来处理：

![image-20220414210106143](wifi.assets/image-20220414210106143.png)



ClientModeImpl.connectToNetwork(WifiConfiguration config)如下：

![image-20220414210406353](wifi.assets/image-20220414210406353.png)



WifiNative.connectToNetwork(@NonNull String ifaceName, WifiConfiguration configuration)

![image-20220414210553743](wifi.assets/image-20220414210553743.png)







#### 6.5.5 SupplicantStaIfaceHal.connectToNetwork



SupplicantStaIfaceHal对象的connectToNetwork方法定义在：frameworks/opt/net/wifi/service/java/com/android/server/wifi/SupplicantStaIfaceHal.java

![image-20220304093506450](wifi.assets/image-20220304093506450.png)



line 977  调用了 SupplicantStaIfaceHal的addNetworkAndSaveConfig方法来添加并配置一个网络，addNetworkAndSaveConfig方法在定义如下：frameworks/opt/net/wifi/service/java/com/android/server/wifi/SupplicantStaIfaceHal.java



![image-20220303200249654](wifi.assets/image-20220303200249654.png)

line 915调用SupplicantStaIfaceHal的addNetwork来在StaIface中添加一个network，line 922调用SupplicantStaNetworkHal的saveWifiConfiguration方法配置新增的网络接口。



![image-20220304100023104](wifi.assets/image-20220304100023104.png)



SupplicantStaNetworkHal的saveWifiConfiguration方法根据WifiConfiguration调用一系列的hidl接口向wpa_supplicant发送wifi网络配置数据，在467行调用了SupplicantStaNetworkHal的saveWifiEnterpriseConfig方法来把输**企业级**认证所需数据从WifiConfiguration传到wpa_supplicant，其定义如下：



![image-20220304100538924](wifi.assets/image-20220304100538924.png)





**返回之后在SupplicantStaNetworkHal的saveWifiConfiguration方法中在line 475调用SupplicantStaNetworkHal的registerCallback方法注册回调对象，registerCallback方法调用StaNetwork的代理对象，真正实现在wpa_supplicant_8/wpa_supplicant/hidl/1.3/sta_network.cpp中的line 147行。向wpa_supplicant注册回调对象后当wpa_supplicant触发一些事件就会调用这些代理回调对象里的一些方法，回调对象具体实现在SupplicantStaNetworkHal的一个内部类**



![image-20220304101842919](wifi.assets/image-20220304101842919.png)



**最后返回到SupplicantStaIfaceHal的connectToNetwork在line 1003 调用SupplicantStaNetworkHal的select方法来选择一个网络并开始认证流程。**



#### 6.5.6 StaNetwork::setKeyMgmt

前面SupplicantStaNetworkHal中调用saveWifiConfiguration方法时设置需要连接网络的类型:

![image-20220511201641525](wifi.assets/image-20220511201641525.png)



wifiConfigurationToSupplicantKeyMgmtMask将WifiConfiguration中的key_mgmt和hidl中的key_mgmt进行映射转换：

![image-20220511201829789](wifi.assets/image-20220511201829789.png)



看下hal文件中定义的key_mgmt是多少，一下只展示1.0版本的，后面1.x版本下的ISupplicantStaNetwork.hal可能会对其进行拓展：

![image-20220511202150384](wifi.assets/image-20220511202150384.png)





![image-20220511201254685](wifi.assets/image-20220511201254685.png)



通过hidl调用到wpa对应的hidl方法后传入的是key_mgmt在hal文件中定义的值:

![image-20220511194409391](wifi.assets/image-20220511194409391.png)

![image-20220511194447948](wifi.assets/image-20220511194447948.png)

![image-20220511194533691](wifi.assets/image-20220511194533691.png)

来看下wpa_supplciant中对key_mgmt的定义：

![image-20220511203302803](wifi.assets/image-20220511203302803.png)

因此：

**当在WifiConfiguration中选择KeyMgmt时WPA_PSK，wpa_supplicant内部设置的key_mgmt为WPA_KEY_MGMT_FT_PSK**

**当在WifiConfiguration中选择KeyMgmt时WPA_EAP，wpa_supplicant内部设置的key_mgmt为WPA_KEY_MGMT_FT_IEEE8021X**







#### 6.5.7 StaNetwork::select

SupplicantStaIfaceHal通过调用StaNetwork代理端的select方法调到了wpa_supplicant中的C++具体实现的select方法：

![image-20220305155744181](wifi.assets/image-20220305155744181.png)





![image-20220305155940747](wifi.assets/image-20220305155940747.png)

retrieveIfacePtr()根据interface名(一般是wlan0)获取到interface对应的wpa_supplicant结构体指针。

retrieveNetworkPtr()返回当前networkid对应的wpa_ssid结构体指针

接着调用external/wpa_supplicant_8/wpa_supplicant/wpa_supplicant.c中定义的wpa_supplicant_select_network函数:

![image-20220418152459972](wifi.assets/image-20220418152459972.png)

在wpa_supplicant_select_network函数中做如下事情：

1、首先判断当前wpa_supplicant和wpa_ssid的状态，如果当前的wpa_supplicant处于WPA_ATHENTICATING状态且wpa_supplicant当前的ssid不是传入的wpa_ssid则调用wpa_supplicant_deathenticate停止对wpa_supplicant中的其他wpa_ssid进行认证。

2、如果传入了wpa_ssid则把该wpa_ssid使能且把wpa_supplicant中的其他wpa_ssid失能，否则把wpa_supplicant中的所有wpa_ssid使能。

3、如果当前的wpa_supplicant处于WPA_ATHENTICATING状态且wpa_supplicant当前的ssid等于传入的wpa_ssid则什么也不做直接打个log然后返回。

4、如果传入了wpa_ssid则初始化eapol状态机。

5、调用wpa_supplicant_fast_associate

6、如果上一步返回不为1则调用wpa_scan_reset_sched_scan取消并停止扫描，然后调用wpa_supplicant_req_scan开始扫描。



##### wpa_supplicant_deauthenticate





##### wpa_supplicant_fast_associate

CONFIG_NO_SCAN_PROCESSING宏未定义

wpa_supplicant_fast_associate首先调用os_reltime_expired判断一下距离上层wpa_supplicant扫描是否过去了SCAN_RES_VALID_FOR_CONNECT秒，如果超时则直接返回-1，否则后续将直接调用wpas_select_network_from_last_scan。

![image-20220418163709000](wifi.assets/image-20220418163709000.png)

![image-20220418163945235](wifi.assets/image-20220418163945235.png)




##### wpa_supplicant_req_scan
wpa_supplicant_select_network函数经过一番判断最终如果有需要则调用wpa_supplicant_req_scan函数，第二和第三个参数传入的都是0。wpa_supplicant_req_scan函数如下：

![image-20220305161523593](wifi.assets/image-20220305161523593.png)



wpa_supplicant_req_scan在epoll队列中注册了一个超时函数，其间隔为0，即待调用完wpa_supplicant_req_scan函数后回到epoll监听时wpa_supplicant_scan函数马上就能得到执行。



external/wpa_supplicant_8/wpa_supplicant/scan.c

```
855  static void wpa_supplicant_scan(void *eloop_ctx, void *timeout_ctx)
856  {
......
954  	if (connect_without_scan) {
955  		wpa_s->connect_without_scan = NULL;
956  		if (ssid) {
957  			wpa_printf(MSG_DEBUG, "Start a pre-selected network "
958  				   "without scan step");
959  			wpa_supplicant_associate(wpa_s, NULL, ssid);
960  			return;
961  		}
962  	}
......
1307  	ret = wpa_supplicant_trigger_scan(wpa_s, scan_params);
......
1338  }
```



***通过wpa_supplicant_select_network调到wpa_supplicant_scan时传入的wpa_s->connect_without_scan为NULL，不在line 959调用wpa_supplicant_associate函数。而是往下执行调用到了wpa_supplicant_trigger_scan先触发一次扫描，之后就不用触发扫描直接调用wpa_supplicant_associate进行认证关联操作。***





![image-20220305162351286](wifi.assets/image-20220305162351286.png)



wpas_trigger_scan_cb是一个回调函数

external/wpa_supplicant_8/wpa_supplicant/scan.c

```
182  static void wpas_trigger_scan_cb(struct wpa_radio_work *work, int deinit)
183  {
......
219  	ret = wpa_drv_scan(wpa_s, params);
......
270  }
```



![image-20220305163143369](wifi.assets/image-20220305163143369.png)

addInterface的驱动初始化时wpa_s->driver指向了如下结构体：

scan2是函数指针，指向driver_nl80211_scan2。



![image-20220305163447138](wifi.assets/image-20220305163447138.png)



因此wpa_drv_scan最终调用到了driver_nl80211_scan2函数：



![image-20220305163706156](wifi.assets/image-20220305163706156.png)



external/wpa_supplicant_8/src/drivers/driver_nl80211_scan.c

```
323  int wpa_driver_nl80211_scan(struct i802_bss *bss,
324  			    struct wpa_driver_scan_params *params)
325  {
......
336  	msg = nl80211_scan_common(bss, NL80211_CMD_TRIGGER_SCAN, params);
......
371  	ret = send_and_recv_msgs(drv, msg, NULL, NULL);
······
413  	eloop_cancel_timeout(wpa_driver_nl80211_scan_timeout, drv, drv->ctx);
414  	eloop_register_timeout(timeout, 0, wpa_driver_nl80211_scan_timeout,
415  			       drv, drv->ctx);
......
421  }
```



![image-20220305164121652](wifi.assets/image-20220305164121652.png)





驱动回复数据后回调函数调用到do_process_drv_event函数来处理，注意，首先回复的是NL80211_CMD_TRIGGER_SCAN，之后回复的是NL80211_CMD_NEW_SCAN_RESULTS：

external/wpa_supplicant_8/src/drivers/driver_nl80211_event.c

```
2577  static void do_process_drv_event(struct i802_bss *bss, int cmd,
2578  				 struct nlattr **tb)
2579  {
......
2607  	switch (cmd) {
2608  	case NL80211_CMD_TRIGGER_SCAN:
2609  		wpa_dbg(drv->ctx, MSG_DEBUG, "nl80211: Scan trigger");
2610  		drv->scan_state = SCAN_STARTED;
2611  		if (drv->scan_for_auth) {
2612  			/*
2613  			 * Cannot indicate EVENT_SCAN_STARTED here since we skip
2614  			 * EVENT_SCAN_RESULTS in scan_for_auth case and the
2615  			 * upper layer implementation could get confused about
2616  			 * scanning state.
2617  			 */
2618  			wpa_printf(MSG_DEBUG, "nl80211: Do not indicate scan-start event due to internal scan_for_auth");
2619  			break;
2620  		}
2621  		wpa_supplicant_event(drv->ctx, EVENT_SCAN_STARTED, NULL);
2622  		break;
......
2632  	case NL80211_CMD_NEW_SCAN_RESULTS:
2633  		wpa_dbg(drv->ctx, MSG_DEBUG,
2634  			"nl80211: New scan results available");
2635  		if (drv->last_scan_cmd != NL80211_CMD_VENDOR)
2636  			drv->scan_state = SCAN_COMPLETED;
2637  		drv->scan_complete_events = 1;
2638  		if (drv->last_scan_cmd == NL80211_CMD_TRIGGER_SCAN) {
2639  			eloop_cancel_timeout(wpa_driver_nl80211_scan_timeout,
2640  					     drv, drv->ctx);
2641  			drv->last_scan_cmd = 0;
2642  		} else {
2643  			external_scan_event = 1;
2644  		}
2645  		send_scan_event(drv, 0, tb, external_scan_event);
2646  		break;
......
}
```



external/wpa_supplicant_8/src/drivers/driver_nl80211_event.c

```
1177  static void send_scan_event(struct wpa_driver_nl80211_data *drv, int aborted,
1178  			    struct nlattr *tb[], int external_scan)
1179  {
......
1249  	wpa_supplicant_event(drv->ctx, EVENT_SCAN_RESULTS, &event);
1250  }
```



不管是NL80211_CMD_TRIGGER_SCAN还是NL80211_CMD_NEW_SCAN_RESULTS最终都回调用到wpa_supplicant_event来处理，在wpa_supplicant_event中对应的事件分别是EVENT_SCAN_STARTED和EVENT_SCAN_RESULTS：

external/wpa_supplicant_8/wpa_supplicant/events.c

```
4595  void wpa_supplicant_event(void *ctx, enum wpa_event_type event,
4596  			  union wpa_event_data *data)
```

![image-20220419112606888](wifi.assets/image-20220419112606888.png)

wpa_supplicant_event在处理EVENT_SCAN_RESULTS时调用wpa_supplicant_event_scan_results







![image-20220305165224329](wifi.assets/image-20220305165224329.png)



external/wpa_supplicant_8/wpa_supplicant/events.c

```
1991  static int _wpa_supplicant_event_scan_results(struct wpa_supplicant *wpa_s,
1992  					      union wpa_event_data *data,
1993  					      int own_request, int update_only)
1994  {
......
2009  	scan_res = wpa_supplicant_get_scan_results(wpa_s,
2010  						   data ? &data->scan_info :
2011  						   NULL, 1);
......
2145  	return wpas_select_network_from_last_scan(wpa_s, 1, own_request);
......
2155  }
```



**_wpa_supplicant_event_scan_results调到了wpas_select_network_from_last_scan函数，回到起点，wpa_supplicant_fast_associate最终也调到了wpas_select_network_from_last_scan函数，看来该函数专门负责去连接AP**



##### wpas_select_network_from_last_scan

external/wpa_supplicant_8/wpa_supplicant/events.c

```
2158  static int wpas_select_network_from_last_scan(struct wpa_supplicant *wpa_s,
2159  					      int new_scan, int own_request)
2160  {
......
2179  	selected = wpa_supplicant_pick_network(wpa_s, &ssid);
......
2189  	if (selected) {
......
1925  		int skip;
1926  		skip = !wpa_supplicant_need_to_roam(wpa_s, selected, ssid);
1927  		if (skip) {
1928  			if (new_scan)
1929  				wpa_supplicant_rsn_preauth_scan_results(wpa_s);
1930  			return 0;
1931  		}
1932  
1933  		if (ssid != wpa_s->current_ssid &&
1934  		    wpa_s->wpa_state >= WPA_AUTHENTICATING) {
1935  			wpa_s->own_disconnect_req = 1;
1936  			wpa_supplicant_deauthenticate(
1937  				wpa_s, WLAN_REASON_DEAUTH_LEAVING);
1938  		}
1939  
1940  		if (wpa_supplicant_connect(wpa_s, selected, ssid) < 0) {
1941  			wpa_dbg(wpa_s, MSG_DEBUG, "Connect failed");
1942  			return -1;
1943  		}
1944  		if (new_scan)
1945  			wpa_supplicant_rsn_preauth_scan_results(wpa_s);
1946  		/*
1947  		 * Do not allow other virtual radios to trigger operations based
1948  		 * on these scan results since we do not want them to start
1949  		 * other associations at the same time.
1950  		 */
1951  		return 1;
1952  	} else {
......
2301  	}
2302  	return 0;
2303  }
```



external/wpa_supplicant_8/wpa_supplicant/events.c

```
1661  int wpa_supplicant_connect(struct wpa_supplicant *wpa_s,
1662  			   struct wpa_bss *selected,
1663  			   struct wpa_ssid *ssid)
1664  {
......
1714  		wpa_supplicant_associate(wpa_s, selected, ssid);
......
1721  }
```



##### wpa_supplicant_associate

wpa_supplicant_associate中对于是否支持SME做了不同的处理。



SME的详细的解释可以看这篇：https://blog.csdn.net/qq_33307581/article/details/110151985

SME 流程可参考https://blog.csdn.net/krokodil98/article/details/118612374

external流程可参考  https://blog.csdn.net/krokodil98/article/details/118761897



wpa_supplicant_associate，根据virtual interface创建时从驱动获取的flag是否有WPA_DRIVER_FLAGS_SME标志来决定，即是否支持SME，如果支持则调用sme_auth，如果不支持即external流程注册了一个wpas_start_assoc_cb回调函数，后续交给该回调函数处理。



​                                   external/wpa_supplicant_8/wpa_supplicant/wpa_supplicant.c

![image-20220512152833213](wifi.assets/image-20220512152833213.png)



#####  SME 流程

wpa_supplicant_associate中对于SME 流程调用sme_authenticate进行处理：

###### 802.11 sme_auth

![image-20220428171839656](wifi.assets/image-20220428171839656.png)



![image-20220428172028922](wifi.assets/image-20220428172028922.png)







```c
280  static void sme_send_authentication(struct wpa_supplicant *wpa_s,
281  				    struct wpa_bss *bss, struct wpa_ssid *ssid,
282  				    int start)
283  {
......
741  	wpa_supplicant_initiate_eapol(wpa_s);
......
824  	wpa_supplicant_cancel_sched_scan(wpa_s);
825  	wpa_supplicant_cancel_scan(wpa_s);
826  
827  	wpa_msg(wpa_s, MSG_INFO, "SME: Trying to authenticate with " MACSTR
828  		" (SSID='%s' freq=%d MHz)", MAC2STR(params.bssid),
829  		wpa_ssid_txt(params.ssid, params.ssid_len), params.freq);
830  
831  	eapol_sm_notify_portValid(wpa_s->eapol, false);
832  	wpa_clear_keys(wpa_s, bss->bssid);
833  	wpa_supplicant_set_state(wpa_s, WPA_AUTHENTICATING);
834  	if (old_ssid != wpa_s->current_ssid)
835  		wpas_notify_network_changed(wpa_s);
......
867  	if (skip_auth) {
868  		wpa_msg(wpa_s, MSG_DEBUG,
869  			"SME: Skip authentication step on reassoc-to-same-BSS");
870  		wpabuf_free(resp);
871  		sme_associate(wpa_s, ssid->mode, bss->bssid, WLAN_AUTH_OPEN);
872  		return;
873  	}
874  
875  
876  	wpa_s->sme.auth_alg = params.auth_alg;
877  	if (wpa_drv_authenticate(wpa_s, &params) < 0) {
878  		wpa_msg(wpa_s, MSG_INFO, "SME: Authentication request to the "
879  			"driver failed");
880  		wpas_connection_failed(wpa_s, bss->bssid);
881  		wpa_supplicant_mark_disassoc(wpa_s);
882  		wpabuf_free(resp);
883  		wpas_connect_work_done(wpa_s);
884  		return;
885  	}
886  
887  	eloop_register_timeout(SME_AUTH_TIMEOUT, 0, sme_auth_timer, wpa_s,
888  			       NULL);
889  
890  	/*
891  	 * Association will be started based on the authentication event from
892  	 * the driver.
893  	 */
894  
895  	wpabuf_free(resp);
896  }
```



![image-20220428181322907](wifi.assets/image-20220428181322907.png)



![image-20220428181506132](wifi.assets/image-20220428181506132.png)



![image-20220428181539144](wifi.assets/image-20220428181539144.png)



external/wpa_supplicant_8/src/drivers/driver_nl80211.c

```c
3600  static int wpa_driver_nl80211_authenticate(
3601  	struct i802_bss *bss, struct wpa_driver_auth_params *params)
3602  {
3603  	struct wpa_driver_nl80211_data *drv = bss->drv;
3604  	int ret = -1, i;
3605  	struct nl_msg *msg;
......//设置各种参数
3631  retry:
3632  	wpa_printf(MSG_DEBUG, "nl80211: Authenticate (ifindex=%d)",
3633  		   drv->ifindex);
3634  
3635  	msg = nl80211_drv_msg(drv, 0, NL80211_CMD_AUTHENTICATE);//把NL80211_CMD_AUTHENTICATE放入msg,后续驱动回复该事件
......//设置各种参数
3700  	ret = send_and_recv_msgs(drv, msg, NULL, NULL);//发送msg给驱动
3701  	msg = NULL;
3702  	if (ret) {
3703  		wpa_dbg(drv->ctx, MSG_DEBUG,
3704  			"nl80211: MLME command failed (auth): count=%d ret=%d (%s)",
3705  			count, ret, strerror(-ret));
3706  		count++;
......      //失败后进行一些判断，如果满足条件则goto retry
3765  	} else {
3766  		wpa_printf(MSG_DEBUG,
3767  			   "nl80211: Authentication request send successfully");
3768  	}
3769  
3770  fail:
3771  	nlmsg_free(msg);
3772  	return ret;
3773  }
```

调用nl80211_drv_msg来构造需要发送的msg

![image-20220429175208791](wifi.assets/image-20220429175208791.png)



![image-20220429175241984](wifi.assets/image-20220429175241984.png)

 cmd对于NL80211_CMD_AUTHENTICATE，往msg中填充cmd

![image-20220429175307504](wifi.assets/image-20220429175307504.png)

构造完msg后返回到wpa_driver_nl80211_authenticate中后续再对msg进行一些填充，然后调用send_and_recv_msgs来发送msg，第三个参数和第四个参数为空。

![image-20220511145931998](wifi.assets/image-20220511145931998.png)

​                                 external/wpa_supplicant_8/src/drivers/driver_nl80211.c

![image-20220511150430973](wifi.assets/image-20220511150430973.png)



**注意send_and_recv_msgs中调用send_and_recv时传入的前两个参数:**

* 第一个参数struct wpa_driver_nl80211_data.global= struct wpa_supplicant.global_drv_priv=struct wpa_global.drv_priv[i] = struct nl80211_global \*，传入了struct nl80211_global \*  
* 第二个参数struct nl80211_global.nl=struct nl_sock \*  

在**send_and_recv**中使用使用传入第一个参数struct nl80211_global的**nl_cb**克隆一个回调结构体，然后使用传入的第二个参数struct nl80211_global.**nl**对应的fd来发送msg，然后调用 nl_recvmsgs从nl的fd中接收netlink数据，并调用回调结构体中的相关函数进行处理。来看下nl_cb中的回调函数到底是什么，在创建初始化interface时wpa_driver_nl80211_init_nl_global函数中有如下这段：

![image-20220510160109292](wifi.assets/image-20220510160109292.png)

nl_cb_set函数将global->nl_cb的回调函数设置成了process_global_event，看下它的实现:

![image-20220510160426764](wifi.assets/image-20220510160426764.png)



external/wpa_supplicant_8/src/drivers/driver_nl80211_event.c      do_process_drv_event函数的switch case部分：

![image-20220512100031703](wifi.assets/image-20220512100031703.png)





do_process_drv_event中的switch case 是NL80211_CMD_AUTHENTICATE，因此调到了mlme_event函数：

external/wpa_supplicant_8/src/drivers/driver_nl80211_event.c

```c
921  static void mlme_event(struct i802_bss *bss,
922  		       enum nl80211_commands cmd, struct nlattr *frame,
923  		       struct nlattr *addr, struct nlattr *timed_out,
924  		       struct nlattr *freq, struct nlattr *ack,
925  		       struct nlattr *cookie, struct nlattr *sig,
926  		       struct nlattr *wmm, struct nlattr *req_ie)
927  {
......
970  	switch (cmd) {
971  	case NL80211_CMD_AUTHENTICATE:
972  		mlme_event_auth(drv, nla_data(frame), nla_len(frame));
973  		break;
974  	case NL80211_CMD_ASSOCIATE:
975  		mlme_event_assoc(drv, nla_data(frame), nla_len(frame), wmm,
976  				 req_ie);
977  		break;
......
1005  	default:
1006  		break;
1007  	}
1008  }
```



![image-20220511151424149](wifi.assets/image-20220511151424149.png)



![image-20220511151633554](wifi.assets/image-20220511151633554.png)

external/wpa_supplicant_8/wpa_supplicant/sme.c

```c
1454  void sme_event_auth(struct wpa_supplicant *wpa_s, union wpa_event_data *data)
1455  {
......//经过一系列判断，如果不使用SAE且从驱动返回802.11认证成功则接下来进行关联
1628  	sme_associate(wpa_s, ssid->mode, data->auth.peer,
1629  		      data->auth.auth_type);
1630  }
```

总结，从驱动接收数据后，调用nl_cb中的process_global_event 进行处理，对于sme_auth流程:

process_global_event(NL80211_CMD_AUTHENTICATE)->

do_process_drv_event(NL80211_CMD_AUTHENTICATE)->

mlme_event(NL80211_CMD_AUTHENTICATE)->

mlme_event_auth->

wpa_supplicant_event(drv->ctx, EVENT_AUTH, &event)->

sme_event_auth->

sme_associate

######    802.11 sme_associate

sme_event_auth中处理802.11认证事情时如果认证成功则调用sme_associate进行后续的关联操作：

external/wpa_supplicant_8/wpa_supplicant/sme.c

```c
1649  void sme_associate(struct wpa_supplicant *wpa_s, enum wpas_mode mode,
1650  		   const u8 *bssid, u16 auth_type)
1651  {
1652  	struct wpa_driver_associate_params params;
1653  	struct ieee802_11_elems elems;
......  
1973  	wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATING);
1974  	//填充关联所需的一些参数，从params.wpa_ie解析需要关联的AP的ies信息元素到elems，这些信息将被填入assoc requst帧中
1975  	if (params.wpa_ie == NULL ||
1976  	    ieee802_11_parse_elems(params.wpa_ie, params.wpa_ie_len, &elems, 0)
1977  	    < 0) {
1978  		wpa_dbg(wpa_s, MSG_DEBUG, "SME: Could not parse own IEs?!");
1979  		os_memset(&elems, 0, sizeof(elems));
1980  	}
1981  	if (elems.rsn_ie) {//根据elems内容填充params的其他字段
1982  		params.wpa_proto = WPA_PROTO_RSN;
1983  		wpa_sm_set_assoc_wpa_ie(wpa_s->wpa, elems.rsn_ie - 2,
1984  					elems.rsn_ie_len + 2);
1985  	} else if (elems.wpa_ie) {//根据elems内容填充params的其他字段
1986  		params.wpa_proto = WPA_PROTO_WPA;
1987  		wpa_sm_set_assoc_wpa_ie(wpa_s->wpa, elems.wpa_ie - 2,
1988  					elems.wpa_ie_len + 2);
1989  	} else if (elems.osen) {//根据elems内容填充params的其他字段
1990  		params.wpa_proto = WPA_PROTO_OSEN;
1991  		wpa_sm_set_assoc_wpa_ie(wpa_s->wpa, elems.osen - 2,
1992  					elems.osen_len + 2);
1993  	} else
1994  		wpa_sm_set_assoc_wpa_ie(wpa_s->wpa, NULL, 0);
1995  	if (elems.rsnxe)
1996  		wpa_sm_set_assoc_rsnxe(wpa_s->wpa, elems.rsnxe - 2,
1997  				       elems.rsnxe_len + 2);
1998  	else
1999  		wpa_sm_set_assoc_rsnxe(wpa_s->wpa, NULL, 0);
2000  	if (wpa_s->current_ssid && wpa_s->current_ssid->p2p_group)
2001  		params.p2p = 1;
2002  
2003  	if (wpa_s->p2pdev->set_sta_uapsd)
2004  		params.uapsd = wpa_s->p2pdev->sta_uapsd;
2005  	else
2006  		params.uapsd = -1;
2007  
2008  	if (wpa_drv_associate(wpa_s, &params) < 0) {                               //开始关联
2009  		wpa_msg(wpa_s, MSG_INFO, "SME: Association request to the "
2010  			"driver failed");
2011  		wpas_connection_failed(wpa_s, wpa_s->pending_bssid);
2012  		wpa_supplicant_set_state(wpa_s, WPA_DISCONNECTED);
2013  		os_memset(wpa_s->pending_bssid, 0, ETH_ALEN);
2014  		return;
2015  	}
2016  
2017  	eloop_register_timeout(SME_ASSOC_TIMEOUT, 0, sme_assoc_timer, wpa_s,
2018  			       NULL);
2019  
2020  #ifdef CONFIG_TESTING_OPTIONS
2021  	wpabuf_free(wpa_s->last_assoc_req_wpa_ie);
2022  	wpa_s->last_assoc_req_wpa_ie = NULL;
2023  	if (params.wpa_ie)
2024  		wpa_s->last_assoc_req_wpa_ie =
2025  			wpabuf_alloc_copy(params.wpa_ie, params.wpa_ie_len);
2026  #endif /* CONFIG_TESTING_OPTIONS */
2027  }
```



![image-20220511152706103](wifi.assets/image-20220511152706103.png)

![image-20220511152734483](wifi.assets/image-20220511152734483.png)





external/wpa_supplicant_8/src/drivers/driver_nl80211.c

```c
6117  static int wpa_driver_nl80211_associate(
6118  	void *priv, struct wpa_driver_associate_params *params)
6119  {
......
......
6133  	if (!(drv->capa.flags & WPA_DRIVER_FLAGS_SME)) {//因为支持SME，取反后不满足if条件
6134  		enum nl80211_iftype nlmode = params->p2p ?
6135  			NL80211_IFTYPE_P2P_CLIENT : NL80211_IFTYPE_STATION;
6136  
6137  		if (wpa_driver_nl80211_set_mode(priv, nlmode) < 0)
6138  			return -1;
6139  		if (params->key_mgmt_suite == WPA_KEY_MGMT_SAE ||
6140  		    params->key_mgmt_suite == WPA_KEY_MGMT_FT_SAE)
6141  			bss->use_nl_connect = 1;
6142  		else
6143  			bss->use_nl_connect = 0;
6144  
6145  		return wpa_driver_nl80211_connect(drv, params,
6146  						  get_connect_handle(bss));//后面的external auth走这个流程
6147  	}
......
6153  	msg = nl80211_drv_msg(drv, 0, NL80211_CMD_ASSOCIATE);//后期驱动回复数据时对应上报NL80211_CMD_ASSOCIATE这个事件
......
6181  	ret = send_and_recv_msgs_owner(drv, msg,
6182  				       get_connect_handle(drv->first_bss), 1,
6183  				       NULL, NULL);
......
6198  }
```

wpa_driver_nl80211_associate中调用send_and_recv_msgs_owner之前先调用get_connect_handle(drv->first_bss)获取 struct nl_sock *

![image-20220511153401793](wifi.assets/image-20220511153401793.png)

再看下send_and_recv_msgs_owner：

![image-20220511154942819](wifi.assets/image-20220511154942819.png)

综上，在使用SME的情况下，不管是否使用SAE：

* 如果驱动不支持Control port，get_connect_handle返回NULL，handle为NULL，msg中也不需要填NL80211_ATTR_CONTOL_PORT的相关属性，send_and_recv_msgs_owner跟send_and_recv_msgs等价。此时使用nl中的netlink socket发送数据到驱动。

* 如果驱动支持Control port，get_connect_handle返回bss->nl_connect，handle为bss->nl_connect，send_and_recv_msgs_owner需要先填入NL80211_ATTR_CONTOL_PORT，且后续再调用send_and_recv时传入的第二个参数不再是nl，而是nl_connect。此时使用nl_connect中的netlink socket发送数据到驱动。





相关参数描述如下：

* struct i802_bss*  = struct wpa_supplicant.drv_priv   =   struct wpa_global.drv_priv[i]

* struct wpa_driver_nl80211_data *  =   struct i802_bss.drv

* struct nl80211_global *   = struct wpa_driver_nl80211_data.global   =   struct wpa_global.drv_priv[i]   =  struct wpa_supplicant.global_drv_priv



send_and_recv接收数据后调用传入的第一个参数的nl_cb成员里面的函数指针，即 struct wpa_driver_nl80211_data.global.nl_cb

这个回调对象里面的函数指针，根据前面分析，nl_cb中的回调函数是process_global_event，process_global_event调用到了do_process_drv_event，此时do_process_drv_event中switch case 是NL80211_CMD_ASSOCIATE，与sme_auth时不同(switch case 是NL80211_CMD_AUTHENTICATE），do_process_drv_event中调用mlme_event来处理：

external/wpa_supplicant_8/src/drivers/driver_nl80211_event.c      do_process_drv_event函数的switch case部分：

![image-20220512100031703](wifi.assets/image-20220512100031703.png)

在mlme_event函数中，与sme_auth时不同(switch case 是NL80211_CMD_AUTHENTICATE，调用mlme_event_auth来处理），sme_associate时(switch case 是NL80211_CMD_ASSOCIATE)调用mlme_event_assoc来处理：

````c
921  static void mlme_event(struct i802_bss *bss,
922  		       enum nl80211_commands cmd, struct nlattr *frame,
923  		       struct nlattr *addr, struct nlattr *timed_out,
924  		       struct nlattr *freq, struct nlattr *ack,
925  		       struct nlattr *cookie, struct nlattr *sig,
926  		       struct nlattr *wmm, struct nlattr *req_ie)
927  {
......
970  	switch (cmd) {
971  	case NL80211_CMD_AUTHENTICATE:
972  		mlme_event_auth(drv, nla_data(frame), nla_len(frame));
973  		break;
974  	case NL80211_CMD_ASSOCIATE:
975  		mlme_event_assoc(drv, nla_data(frame), nla_len(frame), wmm,
976  				 req_ie);
977  		break;
......
1005  	default:
1006  		break;
1007  	}
1008  }
````

与sme_auth时类似，mlme_event_auth和mlme_event_assoc准备都调用wpa_supplicant_event

external/wpa_supplicant_8/src/drivers/driver_nl80211_event.c

```c
213  static void mlme_event_assoc(struct wpa_driver_nl80211_data *drv,
214  			     const u8 *frame, size_t len, struct nlattr *wmm,
215  			     struct nlattr *req_ie)
216  {
217  	const struct ieee80211_mgmt *mgmt;
218  	union wpa_event_data event;
......
234  	mgmt = (const struct ieee80211_mgmt *) frame;
......
261  	os_memset(&event, 0, sizeof(event));
......//根据驱动回复的数据填充event
290  	wpa_supplicant_event(drv->ctx, EVENT_ASSOC, &event);
291  }
```



external/wpa_supplicant_8/wpa_supplicant/events.c   函数 wpa_supplicant_event中的switch case:



![image-20220512142543086](wifi.assets/image-20220512142543086.png)

总结，从驱动接收数据后，调用nl_cb中的process_global_event 进行处理，对于sme_associate流程:

process_global_event(NL80211_CMD_ASSOCIATE)->

do_process_drv_event(NL80211_CMD_ASSOCIATE)->

mlme_event(NL80211_CMD_ASSOCIATE)->

mlme_event_assoc->

wpa_supplicant_event(drv->ctx, EVENT_ASSOC, &event)->

wpa_supplicant_event_assoc



#####  external 流程

wpa_supplicant_associate中对于不支持SME即external流程使用radio_add_work注册了wpas_start_assoc_cb回调函数（超时时间是0,因此后续能马上得到调用），来看下wpas_start_assoc_cb如何处理：

external/wpa_supplicant_8/wpa_supplicant/wpa_supplicant.c

```
3388  static void wpas_start_assoc_cb(struct wpa_radio_work *work, int deinit)
3389  {
......
3777  	ret = wpa_drv_associate(wpa_s, &params);
......
3869  }
```



![image-20220305170910327](wifi.assets/image-20220305170910327.png)



![image-20220305170935533](wifi.assets/image-20220305170935533.png)



![image-20220511152856200](wifi.assets/image-20220511152856200.png)

wpa_driver_nl80211_associate在因为不支持WPA_DRIVE_FLAGS_SME,取反后满足if条件，所以wpa_driver_nl80211_associate调用到了wpa_driver_nl80211_connect，在不使用SAE 的情况下bss->use_nl_connect = 0 ，看get_connect_handle(bss)返回的是什么：

![image-20220511153401793](wifi.assets/image-20220511153401793.png)

get_connect_handle的返回值就是调用wpa_driver_nl80211_connect时的第三个参数：

![image-20220511153826319](wifi.assets/image-20220511153826319.png)

wpa_driver_nl80211_connect先调用nl80211_drv_ms构造一个msg，不同的是现在的cmd是NL80211_CMD_CONNECT，经过一系列对msg的填充后调用send_and_recv_msgs_owner：

![image-20220511154942819](wifi.assets/image-20220511154942819.png)

综上，在不支持SME的情况(external 流程)下

* 如果要使用SAE，则bss->use_nl_connect ==1,get_connect_handle返回bss->nl_connect，后续使用nl_connect里面socket套接字发送接收数据。

* 在不使用SAE的情况下，如果驱动不支持Control port，get_connect_handle返回NULL，handle为NULL，msg中也不需要填NL80211_ATTR_CONTOL_PORT的相关属性，此时send_and_recv_msgs_owner跟sme_auth时调用的send_and_recv_msgs等价。此时使用nl中的netlink socket与驱动发送接收数据。如果驱动支持Control port，get_connect_handle返回bss->nl_connect，handle为bss->nl_connect，send_and_recv_msgs_owner需要先填入NL80211_ATTR_CONTOL_PORT，且后续再调用send_and_recv时传入的第二个参数不再是nl，而是nl_connect。此时使用nl_connect中的netlink socket发送数据到驱动。



相关参数描述如下：

* struct i802_bss*  = struct wpa_supplicant.drv_priv   =   struct wpa_global.drv_priv[i]

* struct wpa_driver_nl80211_data *  =   struct i802_bss.drv

* struct nl80211_global *   = struct wpa_driver_nl80211_data.global   =   struct wpa_global.drv_priv[i]   =  struct wpa_supplicant.global_drv_priv

  

send_and_recv接收数据后调用传入的第一个参数的nl_cb成员里面的函数指针，即 struct wpa_driver_nl80211_data.global.nl_cb

这个回调对象里面的函数指针，根据前面分析，nl_cb中的回调函数是process_global_event，process_global_event调用到了do_process_drv_event，此时do_process_drv_event中switch case 是NL80211_CMD_CONNECT，与sme_auth时不同(switch case 是NL80211_CMD_AUTHENTICATE），do_process_drv_event中调用mlme_event_connect来处理：

![image-20220512095911160](wifi.assets/image-20220512095911160.png)





在mlme_event开始于sme_auth时的处理变得不同，因为是NL80211_CMD_ASSOCIATE，所以调用到了mlme_event_assoc：



mlme_event_assoc先判断驱动是否回复关联成功，如果成功则跟前面sme_auth时一样都调用wpa_supplicant_event来处理，只是传进来的参数不同：

```c
294  static void mlme_event_connect(struct wpa_driver_nl80211_data *drv,
295  			       enum nl80211_commands cmd, struct nlattr *status,
296  			       struct nlattr *addr, struct nlattr *req_ie,
297  			       struct nlattr *resp_ie,
298  			       struct nlattr *timed_out,
299  			       struct nlattr *timeout_reason,
300  			       struct nlattr *authorized,
301  			       struct nlattr *key_replay_ctr,
302  			       struct nlattr *ptk_kck,
303  			       struct nlattr *ptk_kek,
304  			       struct nlattr *subnet_status,
305  			       struct nlattr *fils_erp_next_seq_num,
306  			       struct nlattr *fils_pmk,
307  			       struct nlattr *fils_pmkid)
308  {
309  	union wpa_event_data event;
......
336  	os_memset(&event, 0, sizeof(event));
......//判断驱动是否回复关联成功，如果成功则把驱动回复的相关ATTR保存到event，然后调用wpa_supplicant_event
473  	wpa_supplicant_event(drv->ctx, EVENT_ASSOC, &event);
474  }
```



wpa_supplicant_event用wpa_supplicant_event_assoc来进行后续处理。

![image-20220512142543086](wifi.assets/image-20220512142543086.png)



综上，从驱动接收数据后，调用nl_cb中的process_global_event 进行处理，对于external这种auth和associate一起做的流程：

process_global_event(NL80211_CMD_CONNECT)->

do_process_drv_event(NL80211_CMD_CONNECT)->

mlme_event_connect(NL80211_CMD_CONNECT)->

wpa_supplicant_event(drv->ctx, EVENT_ASSOC, &event)->

wpa_supplicant_event_assoc



##### wpa_supplicant_event_assoc

wpa_supplicant_associate中对于是否支持SME做了两种不同的处理流程，SME流程先发送NL80211_CMD_AUTHENTICATE命令，然后接收并处理NL80211_CMD_AUTHENTICATE命令，处理的过程中如果802.11认证成功再发送NL80211_CMD_ASSOCIATE命令，然后再接收并处理NL80211_CMD_ASSOCIATE命令，在处理时如果802.11关联成功最终调到了wpa_supplicant_event_assoc函数

不支持SME即external流程先发送NL80211_CMD_CONNECT命令，然后接收并处理NL80211_CMD_CONNECT命令，在处理时如果802.11认证和802.11关联都成功了最终就调用wpa_supplicant_event_assoc函数

不管是否支持SME，最终在关联成功后都是调用wpa_supplicant_event_assoc函数,，来看下它的实现：

external/wpa_supplicant_8/wpa_supplicant/events.c

```c
2987  static void wpa_supplicant_event_assoc(struct wpa_supplicant *wpa_s,
2988  				       union wpa_event_data *data)
2989  {
2990  	u8 bssid[ETH_ALEN];
2991  	int ft_completed, already_authorized;
......
3010  	eloop_cancel_timeout(wpas_network_reenabled, wpa_s, NULL);
3011  	wpa_s->own_reconnect_req = 0;
3012    //注意前面设置的key_mgmt，WPA_KEY_MGMT_FT，因此ft_completed == sm->ft_completed,sm的类型是struct wpa_sm *
3013  	ft_completed = wpa_ft_is_completed(wpa_s->wpa);//wpa_s->wpa的类型是struct wpa_sm *
3014  	if (data && wpa_supplicant_event_associnfo(wpa_s, data) < 0)//处理驱动上报的assoc相关信息，见后续
3015  		return;
3016  	/*
3017  	 * FILS authentication can share the same mechanism to mark the
3018  	 * connection fully authenticated, so set ft_completed also based on
3019  	 * FILS result.
3020  	 */
3021  	if (!ft_completed)
3022  		ft_completed = wpa_fils_is_completed(wpa_s->wpa);
......
3105  	/*
3106  	 * Set portEnabled first to false in order to get EAP state machine out
3107  	 * of the SUCCESS state and eapSuccess cleared. Without this, EAPOL PAE
3108  	 * state machine may transit to AUTHENTICATING state based on obsolete
3109  	 * eapSuccess and then trigger BE_AUTH to SUCCESS and PAE to
3110  	 * AUTHENTICATED without ever giving chance to EAP state machine to
3111  	 * reset the state.
3112  	 */
3113  	if (!ft_completed && !already_authorized) {//now,  already_authorized == 0
3114  		eapol_sm_notify_portEnabled(wpa_s->eapol, false);//Disable 802.1x port
3115  		eapol_sm_notify_portValid(wpa_s->eapol, false);//ivalidate 802.1x port
3116  	}
3117  	if (wpa_key_mgmt_wpa_psk(wpa_s->key_mgmt) ||
3118  	    wpa_s->key_mgmt == WPA_KEY_MGMT_DPP ||
3119  	    wpa_s->key_mgmt == WPA_KEY_MGMT_OWE || ft_completed ||
3120  	    already_authorized || wpa_s->drv_authorized_port)//already_authorized == 0
3121  		eapol_sm_notify_eap_success(wpa_s->eapol, false);//start eapol state machine
3122  	/* 802.1X::portControl = Auto */
3123  	eapol_sm_notify_portEnabled(wpa_s->eapol, true);//enable 802.1x port after init sucessfully
3124  	wpa_s->eapol_received = 0;
......
3146  	wpa_supplicant_cancel_scan(wpa_s);
......
3243  	wpas_wps_notify_assoc(wpa_s, bssid);
3244  
3245  	if (data) {
3246  		wmm_ac_notify_assoc(wpa_s, data->assoc_info.resp_ies,
3247  				    data->assoc_info.resp_ies_len,
3248  				    &data->assoc_info.wmm_params);
3249  
3250  		if (wpa_s->reassoc_same_bss)
3251  			wmm_ac_restore_tspecs(wpa_s);
3252  	}
......
3273  }
```



wpa_supplicant_event_assoc中首先根据要连接网络的key_mgmt类型来设置ft_completed，来看下，如果key_mgmt是ft，则ft_completed就是wpa_s->wpa->ft_completed

![image-20220511210930176](wifi.assets/image-20220511210930176.png)

![image-20220511211129988](wifi.assets/image-20220511211129988.png)

回到wpa_supplicant_event_assoc，判断下此时如果如果ft_completed为0，则ft_completed就是key_mgmt为fils时的ft_completed,此时：

![image-20220511211559742](wifi.assets/image-20220511211559742.png)

如果支持fils类型的key_mgmt，则ft_completed仍然是wpa_s->wpa->ft_completed，否则是0。

fils类型的key_mgmt如下：

external/wpa_supplicant_8/src/common/defs.h

```
45  #define WPA_KEY_MGMT_FILS_SHA256 BIT(18)
46  #define WPA_KEY_MGMT_FILS_SHA384 BIT(19)
47  #define WPA_KEY_MGMT_FT_FILS_SHA256 BIT(20)
48  #define WPA_KEY_MGMT_FT_FILS_SHA384 BIT(21)
```

结合前面设置key_mgmt的分析，如果在WifiConfiguration中指定使用WPA_PSK，则ft_completed总是取决于wpa_s->wpa->ft_completed，如果指定使用WPA_EAP，则ft_completed为始终0 。



##### wpa_supplicant_event_associnfo，wifi版本，信息元素ID



wpa_supplicant_event_assoc中调用了wpa_supplicant_event_associnfo来处理assoc后驱动上报的信息，首先调用ieee802_11_parse_elems将帧中信息元素原始比特流解析成struct ieee802_11_elems结构体，req_elems对应association request管理帧中的信息元素，resp_elems对应association response管理帧中的信息元素。，然后结合req_elems和resp_elems把信息放入wpa_s结构体的相关字段里面，例如当前关联上当前ap后使用的wifi版本

wpa_s-> connection_ht    wifi4  802.11n

wpa_s-> connection_vht  wifi5  802.11ac

wpa_s-> connection_he   wifi6  802.11ax

external/wpa_supplicant_8/wpa_supplicant/events.c

```c
2602  static int wpa_supplicant_event_associnfo(struct wpa_supplicant *wpa_s,
2603  					  union wpa_event_data *data)
2604  {
......
2646  	wpa_s->connection_set = 0;
2647  	if (data->assoc_info.req_ies && data->assoc_info.resp_ies) {
2648  		struct ieee802_11_elems req_elems, resp_elems;
2649  
2650  		if (ieee802_11_parse_elems(data->assoc_info.req_ies,
2651  					   data->assoc_info.req_ies_len,
2652  					   &req_elems, 0) != ParseFailed &&
2653  		    ieee802_11_parse_elems(data->assoc_info.resp_ies,
2654  					   data->assoc_info.resp_ies_len,
2655  					   &resp_elems, 0) != ParseFailed) {
2656  			wpa_s->connection_set = 1;
2657  			wpa_s->connection_ht = req_elems.ht_capabilities &&
2658  				resp_elems.ht_capabilities;
2659  			/* Do not include subset of VHT on 2.4 GHz vendor
2660  			 * extension in consideration for reporting VHT
2661  			 * association. */
2662  			wpa_s->connection_vht = req_elems.vht_capabilities &&
2663  				resp_elems.vht_capabilities &&
2664  				(!data->assoc_info.freq ||
2665  				 wpas_freq_to_band(data->assoc_info.freq) !=
2666  				 BAND_2_4_GHZ);
2667  			wpa_s->connection_he = req_elems.he_capabilities &&
2668  				resp_elems.he_capabilities;
2669  
2670  			int max_nss_rx_req = get_max_nss_capability(&req_elems, 1);
2671  			int max_nss_rx_resp = get_max_nss_capability(&resp_elems, 1);
2672  			wpa_s->connection_max_nss_rx = (max_nss_rx_resp > max_nss_rx_req) ?
2673  				max_nss_rx_req : max_nss_rx_resp;
2674  			int max_nss_tx_req = get_max_nss_capability(&req_elems, 0);
2675  			int max_nss_tx_resp = get_max_nss_capability(&resp_elems, 0);
2676  			wpa_s->connection_max_nss_tx = (max_nss_tx_resp > max_nss_tx_req) ?
2677  				max_nss_tx_req : max_nss_tx_resp;
2678  
2679  			struct supported_chan_width sta_supported_chan_width =
2680  				get_supported_channel_width(&req_elems);
2681  			enum chan_width ap_operation_chan_width =
2682  				get_operation_channel_width(&resp_elems);
2683  			if (wpa_s->connection_vht || wpa_s->connection_he) {
2684  				wpa_s->connection_channel_bandwidth =
2685  					get_sta_operation_chan_width(ap_operation_chan_width,
2686  					sta_supported_chan_width);
2687  			} else if (wpa_s->connection_ht) {
2688  				wpa_s->connection_channel_bandwidth = (ap_operation_chan_width
2689  					== CHAN_WIDTH_40) ? CHAN_WIDTH_40 : CHAN_WIDTH_20;
2690  			} else {
2691  				wpa_s->connection_channel_bandwidth = CHAN_WIDTH_20;
2692  			}
2693  		}
2694  	}
```

当使用wpa_cli通过UNIX套接字执行  wpa_cli  -g@android:wpa_wlan0        IFNAME=wlan0    时，wpa_supplicant将返回这些信息：

external/wpa_supplicant_8/wpa_supplicant/ctrl_iface.c

```c
2144  static int wpa_supplicant_ctrl_iface_status(struct wpa_supplicant *wpa_s,
2145  					    const char *params,
2146  					    char *buf, size_t buflen)
2147  {
......
2248  		if (wpa_s->connection_set &&
2249  		    (wpa_s->connection_ht || wpa_s->connection_vht ||
2250  		     wpa_s->connection_he)) {
2251  			ret = os_snprintf(pos, end - pos,
2252  					  "wifi_generation=%u\n",
2253  					  wpa_s->connection_he ? 6 :
2254  					  (wpa_s->connection_vht ? 5 : 4));
2255  			if (os_snprintf_error(end - pos, ret))
2256  				return pos - buf;
2257  			pos += ret;
2258  		}
......
```

也可以通过hidl接口去拿已连接AP的这些信息：

![image-20220620164244122](wifi.assets/image-20220620164244122.png)



注意，安卓9中也有wpa_supplicant_event_associnfo，但wpa_supplicant_event_associnfo没有代码去处理wifi版本相关信息，安卓11的wpa_suplicant才有这个功能。此外还需要驱动支持，即调用wpa_supplicant_event_associnfo时传入了union wpa_event_data *data，这是驱动通过netlink返回给wpa_supplciant的，需要在req_ies和resp_ies中有对应信息元素字段才行：

![image-20220620165116836](wifi.assets/image-20220620165116836.png)

信息元素字段定义在

![image-20220620165242479](wifi.assets/image-20220620165242479.png)

例如WLAN_EID_CAP这个ID对应的信息元素就是用来描述wifi4 802.11n相关物理层capability的。在《802.11无线网络权威指南》P103-P104对各种ID的信息元素有介绍，但由于其出版时间早于802.11n，ID为45的信息元素在该书上为保留字段，先如今已经用来描述802.11n物理层capability。





##### eapol_sm_notify_eap_success(wpa_s->eapol, false)

![image-20220513140907040](wifi.assets/image-20220513140907040.png)



##### eapol_sm_step

![image-20220527172425960](wifi.assets/image-20220527172425960.png)





![image-20220513141153254](wifi.assets/image-20220513141153254.png)

状态机相关的宏参考external/wpa_supplicant_8/src/utils/state_machine.h

在eapol_sm_step中不断循环调用SM_STEP_RUN来运行各种状态机，直到struct eapol_sm *sm->changed为false时退出eapol_sm_step循环，此外当循环100次也会离开循环(CPU占用时间太久了，需要临时退出去处理一下epoll队列中其他fd的可读写事件)，后续判断是因为timeout则使用eloop_register_timeout注册一个超时时间为0的回调函数eapol_sm_step_timeout，其中继续调用eapol_sm_step进行状态切换：

![image-20220513142151055](wifi.assets/image-20220513142151055.png)





eapol_sm_step的最后非常关键，如果eapol_sm->cb_status不为EAPOL_CB_IN_PROGRESS时本次wifi连接时eapol处理完毕，接着去调用:

```
sm->ctx->cb(sm, result, sm->ctx->cb_ctx);
```

当SUPP_PAE状态机切换到HELD或者AUTHENTICATED状态时都会调用给cb_status赋值让它不为EAPOL_CB_IN_PROGRESS

![image-20220527173433350](wifi.assets/image-20220527173433350.png)

来看以下SUPP_PAE状态机什么时候切换到该状态：

![image-20220527174130587](wifi.assets/image-20220527174130587.png)



来看下SUPP_PAE状态机在AUTHENTICATING状态时什么时候sm->suppFail = true;

![image-20220527174918259](wifi.assets/image-20220527174918259.png)



来看下 SUPP_BE 状态机什么时候处于FAIL状态：

![image-20220527175126418](wifi.assets/image-20220527175126418.png)

来看下 eapFail在哪里设置的：

![image-20220527175241767](wifi.assets/image-20220527175241767.png)

对应代码:

![image-20220527175412366](wifi.assets/image-20220527175412366.png)



因此，一旦EAP状态机进入FAILURE状态，eapFail=true，导致 SUPP_BE 状态机切换到FAIL状态。 SUPP_BE 状态机进入FAIL状态时执行suppFail = true操作，导致SUPP_PAE状态机处于HELD状态，SUPP_PAE状态机进入HELD状态时给cb_status赋值让它不为EAPOL_CB_IN_PROGRESS，最终导致在eapol_sm_step函数完成状态机切换循环后执行sm->ctx->cb(sm, result, sm->ctx->cb_ctx);函数指针，该函数指针在addInterface调用wpa_supplicant_init_eapol函数时被赋值，指向了wpa_supplicant_eapol_cb函数：

![image-20220530093516843](wifi.assets/image-20220530093516843.png)



![image-20220530093648217](wifi.assets/image-20220530093648217.png)

wpa_supplicant_eapol_cb函数如下：

![image-20220530094544557](wifi.assets/image-20220530094544557.png)

对于失败的情况wpa_supplicant_eapol_cb中调用wpa_supplicant_req_auth_timeout ，在wpa_supplicant_req_auth_timeout 中往epoll中注册超时函数。

![image-20220530094628052](wifi.assets/image-20220530094628052.png)

当fd处理完成返回epoll循环后调用到wpa_supplicant_timeout函数：



##### wpa_supplicant_rx_eapol

在addInterface时创建了PF_PACKET类型的套接字，然后给该套接字注册接收处理函数wpa_supplicant_rx_eapol。在完成关联后收到的EAPOL数据包由该函数来处理：

![image-20220530110431364](wifi.assets/image-20220530110431364.png)



对于PSK,EAP这些key_mgmt，wpa_supplicant_rx_eapol中调用eapol_sm_rx_eapol来处理：

![image-20220530105655986](wifi.assets/image-20220530105655986.png)

wpa_supplicant_rx_eapol根据EAPOL数据包类型(key 或 eap)最终又调用eapol_sm_step来处理。



##### 6.3. wpa-enterprise EAPOL-EAP-PEAP_90-GTC-TLV认证流程



![image-20220530143428727](wifi.assets/image-20220530143428727.png)



![image-20220530143623564](wifi.assets/image-20220530143623564.png)





![image-20220530143522035](wifi.assets/image-20220530143522035.png)



![image-20220530143911172](wifi.assets/image-20220530143911172.png)



peap内存认证处理：

```
928  static int eap_peap_phase2_request(struct eap_sm *sm,
929  				   struct eap_peap_data *data,
930  				   struct eap_method_ret *ret,
931  				   struct wpabuf *req,
932  				   struct wpabuf **resp)
933  {
...
949  	switch (*pos) {
950  	case EAP_TYPE_IDENTITY://传输identity
951  		*resp = eap_sm_buildIdentity(sm, hdr->identifier, 1);
952  		break;
953  	case EAP_TYPE_GTC://传输password
...
1020          break;
1021  	case EAP_TYPE_TLV://传输认证结果
1022  		os_memset(&iret, 0, sizeof(iret));
1023  		if (eap_tlv_process_ex(sm, data, &iret, req, resp,
1024  				    data->phase2_eap_started &&
1025  				    !data->phase2_eap_success)) {
1026  			ret->methodState = METHOD_DONE;
1027  			ret->decision = DECISION_FAIL;
1028  			return -1;
1029  		}
1030  		if (iret.methodState == METHOD_DONE ||
1031  		    iret.methodState == METHOD_MAY_CONT) {
1032  			ret->methodState = iret.methodState;
1033  			ret->decision = iret.decision;
1034  			data->phase2_success = 1;
1035  		}
1036  		break;
...
1037  	case EAP_TYPE_EXPANDED:
....
1155  	return 0;
1156  }
```





```
######TX SUPP_PAE -> CONNECTING   ,  packet type ==0x01, EAPOL-Start


  
######RX,packet type ==0x00 eapol-packet,   code=Request/Identity + null
######TX,packet type ==0x00 eapol-packet,   code=Response/Identity + qrKey


######RX,packet type ==0x00 eapol-packet,   code=Request/type=0x5a(PEAP_90), type data =0x20( SSL: Received packet - Flags 0x20)
######TX,packet type ==0x00 eapol-packet,   code=Response/type=0x5a(PEAP_90), type data= 明文  “客户”->“服务器”： handshake/client hello


######RX,packet type ==0x00 eapol-packet,   code=Request/type=0x5a(PEAP_90), type data = 明文 “服务器”->“客户”：你好，我是服务器，这里是数字证书的内容(包含公钥)，请查收。 
######TX,packet type ==0x00 eapol-packet,   code=Response/type=0x5a(PEAP_90), type data=0x00 “客户”->“服务器”：OK，接下来向我证明你就是服务器


######RX,packet type ==0x00 eapol-packet,   code=Request/type=0x5a(PEAP_90), type data = 密文   “服务器”->“客户”：{一个随机字符串,即handshake/server hello}[私钥|RSA]
######TX,packet type ==0x00 eapol-packet,   code=Response/type=0x5a(PEAP_90), type data= 密文   “客户”->“服务器”：{我们后面的通信过程，用对称加密来进行，这里是对称加密算法类型和及密钥，请查收}[公钥|RSA]


######RX,packet type ==0x00 eapol-packet,   code=Request/type=0x5a(PEAP_90), type data = 密文   “服务器”->“客户”：{OK，收到对称加密算法的类型和对应的密钥}[密钥|对称加密算法]
######TX,packet type ==0x00 eapol-packet,   code=Response/type=0x5a(PEAP_90), type data=0x00   “客户”->“服务器”：OK，接下来开始第二阶段认证


######RX,packet type ==0x00 eapol-packet,   code=Request/type=0x5a(PEAP_90), type data = 密文   “服务器”->“客户”：{code=Request/type=1(Identity)  + null }[密钥|对称加密算法]
######TX,packet type ==0x00 eapol-packet,   code=Response/type=0x5a(PEAP_90), type data= 密文   “客户”->“服务器”：{code=Response/type=1(Identity)  + identity}[密钥|对称加密算法]


######RX,packet type ==0x00 eapol-packet,   code=Request/type=0x5a(PEAP_90), type data = 密文   “服务器”->“客户”：{code=Request/type=6(身份验证方法为GTC，Generic Token Card，通用令牌卡) + ? }[密钥|对称加密算法]
######TX,packet type ==0x00 eapol-packet,   code=Response/type=0x5a(PEAP_90), type data= 密文   “客户”->“服务器”：{code=Response/type=6(身份验证方法为GTC，Generic Token Card，通用令牌卡)  + identity + password}[密钥|对称加密算法]


######RX,packet type ==0x00 eapol-packet,   code=Request/type=0x5a(PEAP_90), type data = 密文   “服务器”->“客户”：{code=Request/type=33(EAP_TYPE_TLV) + T+L+pMessage }[密钥|对称加密算法]
######TX,packet type ==0x00 eapol-packet,   code=Response/type=0x5a(PEAP_90), type data=密文   “客户”->“服务器”：{code=Response/type=33(EAP_TYPE_TLV)  + 0x000000001(int,EAP_TLV_RESULT_SUCCESS)}[密钥|对称加密算法]


######RX,packet type ==0x00 eapol-packet,   code=Success


######RX,  packet type=0x03  EAPOL-Key, descriptor_type=2(表示EAPOL RSN Key), descriptor_body=ANonece, Individual
######TX,  packet type=0x03  EAPOL-Key, descriptor_type=2(表示EAPOL RSN Key), descriptor_body=SNonece, Individual， MIC


######RX,  packet type=0x03  EAPOL-Key, descriptor_type=2(表示EAPOL RSN Key),descriptor_body=Install GTK,Individual,MIC,Encrypted(GTK,IGTK)
######TX,  packet type=0x03  EAPOL-Key, descriptor_type=2(表示EAPOL RSN Key),descriptor_body=Individual，MIC
```



http://www.youdzone.com/signature.html



https://blog.csdn.net/ly131420/article/details/38400583





![image-20220215160935072](wifi.assets/image-20220215160935072.png)





#### 6.3. wpa 4-way-handsheck && group handsheck

















































## 10 wifi驱动(rk平台)



![捕获](wifi.assets/捕获.PNG)





device_add

```
int device_add(struct device *dev) {

1、dev->parent   //parent必须不为空，用于在/sys/devices/${dev->parent}下面创建子设备

2、dev->class   //class可以为空，如果不为空则调用device_add_class_symlinks(dev)在/sys/class/${dev->class}下创建符号连接并指向在/sys/devices/${dev->parent}下创建的子设备。

3、dev->bus    //bus可以为空，如果不为空则调用bus_add_device(dev)在/sys/bus/${dev->bus}/devices/下创建符号连接并指向在/sys/devices/${dev->parent}下创建的子设备，并调用.match函数指针去匹配dev->bus上的driver如果匹配则调用driver的probe。

}
```



int platform_device_register(struct platform_device *pdev)                                                                                                            //1 (2) 3

struct device *device_create(struct class *class, struct device *parent,dev_t devt, void *drvdata, const char *fmt, ...)       //1 2

int mmc_add_host(struct mmc_host *host)                                                                                                                                       //1 2

int mmc_add_card(struct mmc_card *card)                                                                                                                                      //1 (2) 3





### 10.1platform_device

根据/rk3569_r/kernel/drivers/mmc/host/Kconfig和/rk3569_r/kernel/drivers/mmc/host/Makefile

分别定义了如下config，并且对应在/rk3569_r/kernel/drivers/mmc/host/下要编译进内核的驱动模块文件有：

CONFIG_MMC_SDHCI=y
obj-y + = sdhci.o                                  
CONFIG_MMC_SDHCI_PLTFM=y
obj-y + = sdhci-pltfm.o
CONFIG_MMC_SDHCI_OF_ARASAN=y
obj-y + = sdhci-of-arasan.o

CONFIG_MMC_SDHCI_OF_DWCMSHC=y
obj-y + = sdhci-of-dwcmshc.o

CONFIG_MMC_DW=y
obj-y + = dw_mmc.o                                  

CONFIG_MMC_DW_PLTFM=y
obj-y + = dw_mmc-pltfm.o

CONFIG_MMC_DW_ROCKCHIP=y
obj-y + = dw_mmc-rockchip.o
obj-y + = rk_sdmmc_ops.o

CONFIG_MMC_CQHCI=y
obj-y + = cqhci.o



kernel/arch/arm64/boot/dts/rockchip/rk3568.dtsi

```
2122 	sdmmc2: dwmmc@fe000000 {
2123 		compatible = "rockchip,rk3568-dw-mshc",
2124 			     "rockchip,rk3288-dw-mshc";
2125 		reg = <0x0 0xfe000000.msdc 0x0 0x4000>;
2126 		interrupts = <GIC_SPI 100 IRQ_TYPE_LEVEL_HIGH>;
2127 		max-frequency = <150000000>;
2128 		clocks = <&cru HCLK_SDMMC2>, <&cru CLK_SDMMC2>,
2129 			 <&cru SCLK_SDMMC2_DRV>, <&cru SCLK_SDMMC2_SAMPLE>;
2130 		clock-names = "biu", "ciu", "ciu-drive", "ciu-sample";
2131 		fifo-depth = <0x100>;
2132 		resets = <&cru SRST_SDMMC2>;
2133 		reset-names = "reset";
2134 		status = "disabled";
2135 	};
......
2420 	sdmmc0: dwmmc@fe2b0000 {
2421 		compatible = "rockchip,rk3568-dw-mshc",
2422 			     "rockchip,rk3288-dw-mshc";
2423 		reg = <0x0 0xfe2b0000.msdc 0x0 0x4000>;
2424 		interrupts = <GIC_SPI 98 IRQ_TYPE_LEVEL_HIGH>;
2425 		max-frequency = <150000000>;
2426 		clocks = <&cru HCLK_SDMMC0>, <&cru CLK_SDMMC0>,
2427 			 <&cru SCLK_SDMMC0_DRV>, <&cru SCLK_SDMMC0_SAMPLE>;
2428 		clock-names = "biu", "ciu", "ciu-drive", "ciu-sample";
2429 		fifo-depth = <0x100>;
2430 		resets = <&cru SRST_SDMMC0>;
2431 		reset-names = "reset";
2432 		status = "disabled";
2433 	};
2434 
2435 	sdmmc1: dwmmc@fe2c0000 {
2436 		compatible = "rockchip,rk3568-dw-mshc",
2437 			     "rockchip,rk3288-dw-mshc";
2438 		reg = <0x0 0xfe2c0000.msdc 0x0 0x4000>;
2439 		interrupts = <GIC_SPI 99 IRQ_TYPE_LEVEL_HIGH>;
2440 		max-frequency = <150000000>;
2441 		clocks = <&cru HCLK_SDMMC1>, <&cru CLK_SDMMC1>,
2442 			 <&cru SCLK_SDMMC1_DRV>, <&cru SCLK_SDMMC1_SAMPLE>;
2443 		clock-names = "biu", "ciu", "ciu-drive", "ciu-sample";
2444 		fifo-depth = <0x100>;
2445 		resets = <&cru SRST_SDMMC1>;
2446 		reset-names = "reset";
2447 		status = "disabled";
2448 	};
```

设备树被解析后生成三个平台设备，对应

/sys/devices/platform/fe000000.msdc

/sys/devices/platform/fe2b0000.msdc

/sys/devices/platform/fe2c0000.msdc

三个平台设备compatible有一项为"rockchip,rk3288-dw-mshc"，与如下驱动匹配(调三次probe，每次调用时传入的struct platform_device *pdev指向不同的平台设备)：

kernel/drivers/mmc/host/dw_mmc-rockchip.c

```
392  static const struct of_device_id dw_mci_rockchip_match[] = {
393  	{ .compatible = "rockchip,rk2928-dw-mshc",
394  		.data = &rk2928_drv_data },
395  	{ .compatible = "rockchip,rk3288-dw-mshc",
396  		.data = &rk3288_drv_data },
397  	{},
398  };
......
464  static struct platform_driver dw_mci_rockchip_pltfm_driver = {
465  	.probe		= dw_mci_rockchip_probe,
466  	.remove		= dw_mci_rockchip_remove,
467  	.driver		= {
468  		.name		= "dwmmc_rockchip",
469  		.of_match_table	= dw_mci_rockchip_match,
470  		.pm		= &dw_mci_rockchip_dev_pm_ops,
471  	},
472  };
473  
474  module_platform_driver(dw_mci_rockchip_pltfm_driver);
......
```



在dw_mmc-rockchip驱动模块装载时调用到dw_mci_rockchip_probe

kernel/drivers/mmc/host/dw_mmc-rockchip.c



![image-20220225171755400](wifi.assets/image-20220225171755400.png)



在dw_mci_rockchip_probe函数中调用到dw_mci_pltfm_register。dw_mci_pltfm_register是dw_mmc-pltfm驱动模块导出的函数符号，dw_mci_rockchip_probe和dw_mci_pltfm_register负责解析设备树中的部分信息并放入struct dw_mci结构体host，例如***软件中断号***。



![image-20220225172038379](wifi.assets/image-20220225172038379.png)



dw_mci_pltfm_register最后调用到了dw_mci_probe，dw_mci_probe是dw_mmc驱动模块导出的符号：

/rk3569_r/kernel/drivers/mmc/host/dw_mmc.c

```
3305  int dw_mci_probe(struct dw_mci *host)
3306  {
......
3433  	host->dma_ops = host->pdata->dma_ops;
3434  	dw_mci_init_dma(host);
......
3482  	tasklet_init(&host->tasklet, dw_mci_tasklet_func, (unsigned long)host);//tasklet_init软件中断
3483  	ret = devm_request_irq(host->dev, host->irq, dw_mci_interrupt,
3484  			       host->irq_flags, "dw-mci", host);
......
3502  	/* We need at least one slot to succeed */
3503  	ret = dw_mci_init_slot(host);
......
3528  }
3529  EXPORT_SYMBOL(dw_mci_probe);
```



dw_mci_init_dma负责DMA相关的初始化，给host->dma_ops赋值。



line 3482 初始化tasklet任务，其处理函数是dw_mci_tasklet_func



line 3483调用devm_request_irq注册dw_mci_interrupt为中断上半部处理函数，对于使用的软件中断号，在前面dw_mci_pltfm_register函数从设备树生成的paltform_device解析后放入了host->irq。

在dw_mci_interrupt中，如果有必要则唤醒 dw_mci_tasklet_func



dw_mci_probe调用到了dw_mci_init_slot。



### 10.2 mmc_host

平台驱动把host子设备注册到平台设备之下，一个sdio/sd/mmc控制器对应一个host子设备。



/rk3569_r/kernel/drivers/mmc/host/dw_mmc.c

```
2902  static int dw_mci_init_slot(struct dw_mci *host)
2903  {
2904  	struct mmc_host *mmc;
2905  	struct dw_mci_slot *slot;
2906  	int ret;
2907  
2908  	mmc = mmc_alloc_host(sizeof(struct dw_mci_slot), host->dev);
2909  	if (!mmc)
2910  		return -ENOMEM;
2911  
2912  	slot = mmc_priv(mmc);
2913  	slot->id = 0;
2914  	slot->sdio_id = host->sdio_id0 + slot->id;
2915  	slot->mmc = mmc;
2916  	slot->host = host;
2917  	host->slot = slot;
2918  
2919  	mmc->ops = &dw_mci_ops;
......
2968  	ret = mmc_add_host(mmc);
......
2981  }
```



![image-20220225180818170](wifi.assets/image-20220225180818170.png)



dw_mci_init_slot调用mmc_alloc_host分配一个mmc_host结构体mmc，并把mmc->private赋给struct dw_mci_slot *slot后进行初始化，mmc->ops指向了一个mmc_host_ops结构体。mmc_host_ops结构体中存放着大量mmc子系统core与host直接的操作接口函数，这些函数具体实现是操作SD/SDIO/MMC控制器的寄存器。这里的控制器使用的是

```
Rockchip specific extensions for Synopsys DW Memory Card Interface
```

即该控制器由Synopsys设计，Rockchip对其进行了一些定制化。

![image-20220225173723553](wifi.assets/image-20220225173723553.png)



mmc_alloc_host在kernel/drivers/mmc/core/host.c定义：

![image-20220308105945183](wifi.assets/image-20220308105945183.png)



mmc_alloc_host函数关键部分在

```
422  	host->class_dev.parent = dev;
423  	host->class_dev.class = &mmc_host_class;
......
if (mmc_gpio_alloc(host))
......
434  	INIT_DELAYED_WORK(&host->detect, mmc_rescan);
435  	INIT_DELAYED_WORK(&host->sdio_irq_work, sdio_irq_work);
```

dev和class_dev是struct device结构体

**dev是前面dw_mmc-rockchip驱动匹配的那个平台设备的dev(即pdev->dev)，该设备由与驱动compatible匹配的设备树节点解析生成，在/sys/devices下对应节点/sys/devices/platform/fe2c0000.msdc，在/sys/class下没有对应节点，在/sys/bus下对应节点 /sys/bus/platform/devices/fe2c0000.msdc。**

mmc_host_class定义在kernel/drivers/mmc/core/host.c：

![image-20220308113401558](wifi.assets/image-20220308113401558.png)



再来看mmc_gpio_alloc：





**返回到dw_mci_init_slot函数中，后续调用在mmc_add_host函数，在mmc_add_host函数中调用device_add函数时就会在/sys/devices/platform/fe2c0000.msdc/下创建${class_dev.name}节点，对应dev的子设备。此外还会在/sys/class/下创建${mmc_host_class.name}节点。且没有在/sys/bus下创建节点，因为class_dev.bus没有指定。**



![image-20220225175004964](wifi.assets/image-20220225175004964.png)



mmc_add_host函数调完device_add后再调mmc_start_host函数：

![image-20220308195711177](wifi.assets/image-20220308195711177.png)

mmc_start_host函数调用mmc_gpiod_request_cd_irq ，mmc_gpiod_request_cd_irq 调用 devm_request_threaded_irq，devm_request_threaded_irq调用 request_threaded_irq  根据来注册独立的线程化中断来处理，注册的所有信息来自：

```
struct mmc_gpio *ctx = host->slot.handler_priv;
```

![image-20220627211920371](wifi.assets/image-20220627211920371.png)









mmc_start_host函数调用_mmc_detect_change函数：

![image-20220308210325003](wifi.assets/image-20220308210325003.png)

_mmc_detect_change函数调用mmc_schedule_delayed_work函数：

![image-20220308210423981](wifi.assets/image-20220308210423981.png)

mmc_schedule_delayed_work函数调用queue_delayed_work函数将host->detect加入system_freezable_wq队列。host->detect先前在mmc_alloc_host函数中进行了初始化，指定处理函数为mmc_rescan：

kernel/drivers/mmc/core/host.c

```
434  	INIT_DELAYED_WORK(&host->detect, mmc_rescan);
```

调用queue_delayed_work后，一方面一路返回到平台驱动程序的dw_mci_rockchip_probe执行完毕，另一方面负责处理system_freezable_wq队列中任务的内核kwoker线程就会取出host->detect，并将其作为参数调用里面的mmc_rescan函数。mmc_rescan如下：

/rk3569_r/kernel/drivers/mmc/core/core.c

![image-20220308212407584](wifi.assets/image-20220308212407584.png)

mmc_rescan涉及如下函数：

mmc_claim_host(host);//尝试获得host子设备的使用全权
mmc_release_host(host);//释放host子设备
host->ops->xxx(yyy);//调用host子设备中的函数指针来配置硬件控制器的寄存器以实现某些功能。函数指针指向厂家实现的函数。

在for循环中以四个不同的频率作为参数调用mmc_rescan_try_freq函数，依次尝试几个给定频率扫卡，直至检测到mmc card的存在。

![image-20220308213125901](wifi.assets/image-20220308213125901.png)

经实测，在RK平台上由于host->f_min的限制mmc_rescan_try_freq执行了两次，传入的第二个参数(频率)分别为400000,300000



mmc_rescan_try_freq函数十分关键，首先调用mmc_power_up扫卡初始化，随后按顺序判断卡是sdio/sd/mmc那种，然后再调用对应的mmc_attach_xxx函数进行处理：





mmc_power_up     使能对应SD/sdio卡的电源/reset管脚，初始化sd控制器host->ios（位宽为1bit，chipselect=MMC_CS_DONTCARE...etc)



sdio_reset                发送CMD52



mmc_go_idle           发送CMD0



mmc_send_if_cond   发送CMD8





/rk3569_r/kernel/drivers/mmc/core/core.c

![image-20220315101347261](wifi.assets/image-20220315101347261.png)









### 10.3 mmc_card&&sdio_func







mmc_attach_sdio依次调用：

```
mmc_attach_bus
mmc_sdio_init_card
sdio_init_func
mmc_add_card
sdio_add_func
```



/rk3569_r/kernel/drivers/mmc/core/sdio.c

![image-20220309105025498](wifi.assets/image-20220309105025498.png)



**mmc_attach_bus(host, &mmc_sdio_ops);**

负责注册回调函数集合到host->bus_ops

![image-20220309105257319](wifi.assets/image-20220309105257319.png)

![image-20220309105220674](wifi.assets/image-20220309105220674.png)



**mmc_sdio_init_card(host, rocr, NULL, 0);**

创建一个card设备，指定父设备为host，指定card设备的bus是mmc_bus_type总线。

/rk3569_r/kernel/drivers/mmc/core/sdio.c

```
566  static int mmc_sdio_init_card(struct mmc_host *host, u32 ocr,
567  			      struct mmc_card *oldcard, int powered_resume)
568  {
......
622  	card = mmc_alloc_card(host, NULL);
......
838  }
```



![image-20220309110104968](wifi.assets/image-20220309110104968.png)


注意在mmc_sdio_init_card中还会发送一些CMD命令初始化SDIO卡，关于CMD命令和SD/SDIO协议可以参考如下：



http://cache.baiducontent.com/c?m=P-3DiAQagve1j4XgjT3V-xdesQ3AModAY8emxErvqgFZKKE3cKT7vkGXiF3Qim2kehd6KD7Lycj4rekIXNYnHLYysA3u2t1vsx-zd2rar93uWYHlpEoJALt1yjkNRty78TvFSadPGh07qvDTsCpRma&p=9e6cc7048cdb12a05abd9b7959&newp=8f63c54ad1d512a05aaf82375153d8274e3987683ac3864e1290c408d23f061d4863e1be2724100fd4c27c6c02ac495bebf7347523454df6cc8a871d81ed84&s=c0c08920eee6f233&user=baidu&fm=sc&query=SD%B2%D9%D7%F7%C1%F7%B3%CC&qid=9e63101000003ab6&p1=3

http://cache.baiducontent.com/c?m=BdymGqIhjKkgm-lg-khBjRWRhbJconUdigQ-ao-2uZuwU8B0guS1O4BqSxmP6GoK9QErj8aB_kOgQ_s-of39tydwCAs0VMjTPV6FHKB1IHZqe5yux9HVLbwabc9iyrOI3HrmfJF-E9G7Pn9XvKNObK&p=882a9e4e9cdd52fc57ef8a3e4e5d&newp=882a9e4e8d891abc09fbc7710f5592695d0fc20e3fd4d201298ffe0cc4241a1a1a3aecbe25271202d2c5796406af4f57edf63d723d0034f1f689df08d2ecce7e74c27c27210a&s=37693cfc748049e4&user=baidu&fm=sc&query=SDIO%B3%F5%CA%BC%BB%AF&qid=9efb928a00043f5e&p1=8

http://cache.baiducontent.com/c?m=mcfEJh7RFRjfXx-1MaisCOPwfl_C7lE3OGE7d5zxhv0KJatpnWetxT43MYZMAvJTuhGvnrTngY3QSdIlyqIcsIp8atm8sbN1uJ468wyQoVeBsvzMdheUe8NtdIuUF0hg4ZSiC-tGyP5zMBTPO4-5HfmqA-ArqtfZbYvLoTIQYJdqs4IQwAdWRpBIc986gWWA&p=8b2a971d93dd15fe04bd9b7f1b&newp=87769a47808a1ffe01bd9b7f4253d8224216ed643ad4c44324b9d71fd325001c1b69e6b922241300d7c07b6c0bac4d58e1f33278341766dada9fca458ae7c47e66d6766d&s=ad61ab143223efbc&user=baidu&fm=sc&query=mmc%5Fpower%5Fup&qid=cae403d500016348&p1=16

https://blog.csdn.net/u010443710/article/details/107014873

http://www.tjtech.me/sdio-card-init.html

https://blog.csdn.net/wzm_c1386666/article/details/120618310

https://blog.csdn.net/u013686019/article/details/69214891

https://so.csdn.net/so/search?q=sdio&t=blog&u=u013686019&urw=



**sdio_init_func(host->card, i + 1);**

sdio_init_func函数很关键，它负责分配一个sdio_func结构体用于描述sdio设备，并对其进行初始化，主要函数如下：

```
sdio_alloc_func
sdio_read_fbr
sdio_read_func_cis
```







![image-20220309170718845](wifi.assets/image-20220309170718845.png)



sdio_alloc_func负责分配一个sdio_func结构体并对func->dev.parent和func->dev.bus进行初始化

![image-20220309110801941](wifi.assets/image-20220309110801941.png)



sdio_read_fbr负责读卡并初始化func->class



![image-20220309170803432](wifi.assets/image-20220309170803432.png)

sdio_read_func_cis负责读卡并初始化func->vendor和func->device

![image-20220309171218317](wifi.assets/image-20220309171218317.png)



**mmc_add_card(host->card);**

让前面指定的父设备、总线生效，即在/sys/devices/platform/fe2c0000.msdc/mmc_host/下创建对应card节点

/rk3569_r/kernel/drivers/mmc/core/bus.c

```
287  int mmc_add_card(struct mmc_card *card)
288  {
......
358  	ret = device_add(&card->dev);
......
365  }
```



**sdio_add_func(host->card->sdio_func[i]);**

/rk3569_r/kernel/drivers/mmc/core/sdio_bus.c

![image-20220309110925530](wifi.assets/image-20220309110925530.png)



### 10.3 关系拓扑图



![image-20220309162242104](wifi.assets/image-20220309162242104.png)



### 10.4 sdio bus



![image-20220309162820107](wifi.assets/image-20220309162820107.png)



**sdio bus 注册:**

![image-20220309165143691](wifi.assets/image-20220309165143691.png)



![image-20220309165226081](wifi.assets/image-20220309165226081.png)



**sdio bus 匹配sdio驱动和sdio设备:**



![image-20220309162842427](wifi.assets/image-20220309162842427.png)



![image-20220309163010642](wifi.assets/image-20220309163010642.png)

![image-20220309162956537](wifi.assets/image-20220309162956537.png)





![image-20220309163356395](wifi.assets/image-20220309163356395.png)



![image-20220309163416411](wifi.assets/image-20220309163416411.png)



![image-20220309163509177](wifi.assets/image-20220309163509177.png)



sdio驱动和sdio设备在sdio总线上匹配的规则是：

1、sdio驱动可以包含多个sdio_device_id结构体，只要任意一个sdio_device_id结构体能够与sdio设备的sdio_func结构体匹配则驱动与设备匹配。

2、sdio_device_id结构体与sdio_func结构体中的class、vendor、device都匹配才算匹配。

3、如果sdio_device_id结构体的class、vendor、device为SDIO_ANY_ID则无论sdio_func结构体对应的那项属性是什么，该项属性必然匹配，否则就需要sdio_device_id结构体与sdio_func结构体的class、vendor、device对应相等则匹配。



kernel/include/linux/platform_device.h

```
180  struct platform_driver {
181  	int (*probe)(struct platform_device *);
182  	int (*remove)(struct platform_device *);
183  	void (*shutdown)(struct platform_device *);
184  	int (*suspend)(struct platform_device *, pm_message_t state);
185  	int (*resume)(struct platform_device *);
186  	struct device_driver driver;
187  	const struct platform_device_id *id_table;
188  	bool prevent_deferred_probe;
189  };
```

kernel/include/linux/device.h

```
298  struct device_driver {
299  	const char		*name;
300  	struct bus_type		*bus;
301  
302  	struct module		*owner;
303  	const char		*mod_name;	/* used for built-in modules */
304  
305  	bool suppress_bind_attrs;	/* disables bind/unbind via sysfs */
306  	enum probe_type probe_type;
307  
308  	const struct of_device_id	*of_match_table;
309  	const struct acpi_device_id	*acpi_match_table;
310  
311  	int (*probe) (struct device *dev);
312  	void (*sync_state)(struct device *dev);
313  	int (*remove) (struct device *dev);
314  	void (*shutdown) (struct device *dev);
315  	int (*suspend) (struct device *dev, pm_message_t state);
316  	int (*resume) (struct device *dev);
317  	const struct attribute_group **groups;
318  
319  	const struct dev_pm_ops *pm;
320  	void (*coredump) (struct device *dev);
321  
322  	struct driver_private *p;
323  
324  	ANDROID_KABI_RESERVE(1);
325  	ANDROID_KABI_RESERVE(2);
326  	ANDROID_KABI_RESERVE(3);
327  	ANDROID_KABI_RESERVE(4);
328  };
```

kernel/include/linux/mod_devicetable.h

```
235  struct of_device_id {
236  	char	name[32];
237  	char	type[32];
238  	char	compatible[128];
239  	const void *data;
240  };
```



### 10.5 mt7661驱动

已知  gen4-mt7663


interrupt        发送完成与收到数据时                wakeup           waitq_hif           hif_thread
cfg80211       发送数据时                                wakeup            waitq                main_thread
hif_thread中处理发送完成中断，如果是收到数据中断则wakeup   waitq  启动   main_thread
main_thread  调用  kalSetTxEvent2Rx             wakeup           waitq_rx             rx_thread

GLUE_FLAG_HIF_TX
GLUE_FLAG_TXREQ


GLUE_FLAG_HIF_TX_CMD







重要数据结构：

struct wireless_dev    

struct wiphy

prGlueInfo = (struct GLUE_INFO *) wiphy_priv(prWdev->wiphy);

prGlueInfo->prDevHandler  的类型是 struct net_dev * 即一个网络设备

prNetDevPrivate = (struct NETDEV_PRIVATE_GLUE_INFO *) netdev_priv(prGlueInfo->prDevHandler);

stuct net_device.ieee80211_ptr  =  struct wireless_dev *

stuctwireless_dev.netdev = stuct net_device *





 

#### 10.5.1 驱动初始化

vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/os/linux/gl_init.c

```
4535  module_init(initWlan);
4536  module_exit(exitWlan);
```



```c
4217  static int initWlan(void)
4218  {
......
4250  	wlanCreateWirelessDevice();     //cfg_80211
......
4293  	ret = ((glRegisterBus(wlanProbe,
4294  			      wlanRemove) == WLAN_STATUS_SUCCESS) ? 0 : -EIO);
......
4328  }
```





![image-20220627093502645](wifi.assets/image-20220627093502645.png)



![image-20220627093647172](wifi.assets/image-20220627093647172.png)



```c
3406  static int32_t wlanProbe(void *pvData, void *pvDriverData)
3407  {
......
3471  		prWdev = wlanNetCreate(pvData, pvDriverData);//创建struct net_dev
......
3493  		i4Status = glBusSetIrq(prWdev->netdev, NULL, prGlueInfo);//register sdio interrupt handle func
......
3513  		wlanGetConfig(prAdapter);//   load configuration file:   /vendor/firmware/wifi_mt7661.cfg
......
3619  		INIT_WORK(&prGlueInfo->rTxMsduFreeWork, kalFreeTxMsduWorker);
3620  
3621  		INIT_DELAYED_WORK(&prGlueInfo->rRxPktDeAggWork,
3622  				halDeAggRxPktWorker);
3623  
3624  
3625  		prGlueInfo->main_thread = kthread_run(main_thread,
3626  				prGlueInfo->prDevHandler, "main_thread");
3627  #if CFG_SUPPORT_MULTITHREAD
3628  		prGlueInfo->hif_thread = kthread_run(hif_thread,
3629  				prGlueInfo->prDevHandler, "hif_thread");
3630  		prGlueInfo->rx_thread = kthread_run(rx_thread,
3631  				prGlueInfo->prDevHandler, "rx_thread");
3632  		HAL_AGG_THREAD(prGlueInfo->prAdapter);
3633  #endif
......
3727  		i4DevIdx = wlanNetRegister(prWdev);
......
3958  }
```



#### 10.5.2 nl80211_cfg

初始化时initWlan中调用了  wlanCreateWirelessDevice()

```c
2077  static void wlanCreateWirelessDevice(void)
2078  {
2079  	struct wiphy *prWiphy = NULL;
2080  	struct wireless_dev *prWdev = NULL;
2081  	unsigned int u4SupportSchedScanFlag = 0;
2082  
2083  	/* 4 <1.1> Create wireless_dev */
2084  	prWdev = kzalloc(sizeof(struct wireless_dev), GFP_KERNEL);
2085  	if (!prWdev) {
2086  		DBGLOG(INIT, ERROR,
2087  		       "Allocating memory to wireless_dev context failed\n");
2088  		return;
2089  	}
2090  	/* 4 <1.2> Create wiphy */
2091  #if CFG_ENABLE_UNIFY_WIPHY
2092  	prWiphy = wiphy_new(&mtk_cfg_ops, sizeof(struct GLUE_INFO));
2093  #else
2094  	prWiphy = wiphy_new(&mtk_wlan_ops,
2095  			    sizeof(struct GLUE_INFO));
2096  #endif
2097  
2098  	if (!prWiphy) {
2099  		DBGLOG(INIT, ERROR,
2100  		       "Allocating memory to wiphy device failed\n");
2101  		goto free_wdev;
2102  	}
2103  
2104  	/* 4 <1.3> configure wireless_dev & wiphy */
2105  	prWdev->iftype = NL80211_IFTYPE_STATION;
2106  	prWiphy->iface_combinations = p_mtk_iface_combinations_sta;
2107  	prWiphy->n_iface_combinations =
2108  		mtk_iface_combinations_sta_num;
2109  	prWiphy->max_scan_ssids = SCN_SSID_MAX_NUM +
2110  				  1; /* include one wildcard ssid */
2111  	prWiphy->max_scan_ie_len = 512;
2112  #if CFG_SUPPORT_SCHED_SCAN
2113  	prWiphy->max_sched_scan_ssids     =
2114  		CFG_SCAN_HIDDEN_SSID_MAX_NUM;
2115  	prWiphy->max_match_sets           =
2116  		CFG_SCAN_SSID_MATCH_MAX_NUM;
2117  	prWiphy->max_sched_scan_ie_len    = CFG_CFG80211_IE_BUF_LEN;
2118  	u4SupportSchedScanFlag            =
2119  		WIPHY_FLAG_SUPPORTS_SCHED_SCAN;
2120  #endif /* CFG_SUPPORT_SCHED_SCAN */
2121  	prWiphy->interface_modes = BIT(NL80211_IFTYPE_STATION) |
2122  				   BIT(NL80211_IFTYPE_ADHOC);
2123  	prWiphy->bands[KAL_BAND_2GHZ] = &mtk_band_2ghz;
2124  	/* always assign 5Ghz bands here, if the chip is not support 5Ghz,
2125  	 *  bands[KAL_BAND_5GHZ] will be assign to NULL
2126  	 */
2127  	prWiphy->bands[KAL_BAND_5GHZ] = &mtk_band_5ghz;
2128  	prWiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;
2129  	prWiphy->cipher_suites = (const u32 *)mtk_cipher_suites;
2130  	prWiphy->n_cipher_suites = ARRAY_SIZE(mtk_cipher_suites);
2131  	prWiphy->flags = WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL
2132  			| u4SupportSchedScanFlag;
2133  
2134  #if CFG_SUPPORT_802_11R && CFG_SUPPORT_CFG80211_AUTH
2135  	prWiphy->features |= NL80211_FEATURE_DS_PARAM_SET_IE_IN_PROBES;
2136  	prWiphy->features |= NL80211_FEATURE_QUIET;
2137  	prWiphy->features |= NL80211_FEATURE_TX_POWER_INSERTION;
2138  #endif /* CFG_SUPPORT_ROAMING */
2139  
2140  #if (CFG_SUPPORT_ROAMING == 1)
2141  	prWiphy->flags |= WIPHY_FLAG_SUPPORTS_FW_ROAM;
2142  #endif /* CFG_SUPPORT_ROAMING */
2143  
2144  #if KERNEL_VERSION(3, 14, 0) > CFG80211_VERSION_CODE
2145  	prWiphy->flags |= WIPHY_FLAG_CUSTOM_REGULATORY;
2146  #else
2147  	prWiphy->regulatory_flags |= REGULATORY_CUSTOM_REG;
2148  #if (CFG_SUPPORT_DFS_MASTER == 1)
2149  	prWiphy->flags |= WIPHY_FLAG_HAS_CHANNEL_SWITCH;
2150  #if KERNEL_VERSION(3, 16, 0) <= CFG80211_VERSION_CODE
2151  	prWiphy->max_num_csa_counters = 2;
2152  #endif
2153  #endif /* CFG_SUPPORT_DFS_MASTER */
2154  #endif
2155  #if (CFG_SUPPORT_SAE == 1)
2156  	prWiphy->features |= NL80211_FEATURE_SAE;
2157  #endif /* CFG_SUPPORT_DFS_MASTER */
2158  
2159  	cfg80211_regd_set_wiphy(prWiphy);
2160  
2161  #if (CFG_SUPPORT_TDLS == 1)
2162  	TDLSEX_WIPHY_FLAGS_INIT(prWiphy->flags);
2163  #endif /* CFG_SUPPORT_TDLS */
2164  	prWiphy->max_remain_on_channel_duration = 5000;
2165  	prWiphy->mgmt_stypes = mtk_cfg80211_ais_default_mgmt_stypes;
2166  
2167  #if (CFG_SUPPORT_SCAN_RANDOM_MAC && \
2168  	(KERNEL_VERSION(3, 19, 0) <= CFG80211_VERSION_CODE))
2169  	prWiphy->features |= NL80211_FEATURE_SCAN_RANDOM_MAC_ADDR;
2170  	prWiphy->features |= NL80211_FEATURE_SCHED_SCAN_RANDOM_MAC_ADDR;
2171  #endif
2172  
2173  #if KERNEL_VERSION(3, 18, 0) <= CFG80211_VERSION_CODE
2174  	prWiphy->vendor_commands = mtk_wlan_vendor_ops;
2175  	prWiphy->n_vendor_commands = sizeof(mtk_wlan_vendor_ops) /
2176  				     sizeof(struct wiphy_vendor_command);
2177  	prWiphy->vendor_events = mtk_wlan_vendor_events;
2178  	prWiphy->n_vendor_events = ARRAY_SIZE(
2179  					   mtk_wlan_vendor_events);
2180  #endif
2181  	/* 4 <1.4> wowlan support */
2182  #ifdef CONFIG_PM
2183  #if KERNEL_VERSION(3, 11, 0) <= CFG80211_VERSION_CODE
2184  	prWiphy->wowlan = &mtk_wlan_wowlan_support;
2185  #else
2186  	kalMemCopy(&prWiphy->wowlan, &mtk_wlan_wowlan_support,
2187  		   sizeof(struct wiphy_wowlan_support));
2188  #endif
2189  #endif
2190  
2191  #ifdef CONFIG_CFG80211_WEXT
2192  	/* 4 <1.5> Use wireless extension to replace IOCTL */
2193  
2194  #if CFG_ENABLE_UNIFY_WIPHY
2195  	prWiphy->wext = NULL;
2196  #else
2197  	prWiphy->wext = &wext_handler_def;
2198  #endif
2199  #endif
2200  	/* initialize semaphore for halt control */
2201  	sema_init(&g_halt_sem, 1);
2202  
2203  #if CFG_ENABLE_UNIFY_WIPHY
2204  	prWiphy->iface_combinations = p_mtk_iface_combinations_p2p;
2205  	prWiphy->n_iface_combinations =
2206  		mtk_iface_combinations_p2p_num;
2207  
2208  	prWiphy->interface_modes |= BIT(NL80211_IFTYPE_AP) |
2209  				    BIT(NL80211_IFTYPE_P2P_CLIENT) |
2210  				    BIT(NL80211_IFTYPE_P2P_GO) |
2211  				    BIT(NL80211_IFTYPE_STATION);
2212  	prWiphy->software_iftypes |= BIT(NL80211_IFTYPE_P2P_DEVICE);
2213  	prWiphy->flags |= WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL |
2214  			  WIPHY_FLAG_HAVE_AP_SME;
2215  	prWiphy->ap_sme_capa = 1;
2216  #endif
2217  	if (wiphy_register(prWiphy) < 0) {                                    //注册struct wiphy
2218  		DBGLOG(INIT, ERROR, "wiphy_register error\n");
2219  		goto free_wiphy;
2220  	}
2221  	prWdev->wiphy = prWiphy;                     //struct wireless_dev.wiphy  = struct wiphy *
2222  	gprWdev = prWdev;                            //使用struct wireless_dev *类型的全局变量指向被初始化的prWdev
2223  	DBGLOG(INIT, INFO, "Create wireless device success\n");
2224  	return;
2225  
2226  free_wiphy:
2227  	wiphy_free(prWiphy);
2228  free_wdev:
2229  	kfree(prWdev);
2230  }
```





cfg80211
net/wireless/nl80211.c 定义了很多函数，例如 nl80211_trigger_scan，当netlink发来NL80211_CMD_TRIGGER_SCAN命令后nl80211_trigger_scan负责调用struct nl80211_ops里面的scan函数指针来进行处理。

net/wireless/scan.c    定义了很多函数，例如 cfg80211_put_bss      ___cfg80211_scan_done/nl80211_send_scan_result/ 



mac80211
common/net/mac80211/mlme.c
net/mac80211/scan.c

ieee80211_rx
  |------ieee80211_sta_rx_queued_mgmt 
  |------__ieee80211_data_to8023 ----- netif_receive_skb 









#### 10.5.3 NAPI 网卡

初始化时wlanProbe中调用了wlanNetCreate函数和wlanNetRegister函数

vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/os/linux/gl_init.c

```c
2369  static struct wireless_dev *wlanNetCreate(void *pvData,
2370  		void *pvDriverData)
2371  {
2372  	struct wireless_dev *prWdev = gprWdev;//wlanCreateWirelessDevice中对gprWdev进行赋值
2373  	struct GLUE_INFO *prGlueInfo = NULL;//mtk自己定义的数据结构，用于存储全局信息，struct wiphy.priv也指向这个地址
......
2376  	struct device *prDev;
......
2381  	struct wiphy *prWiphy = NULL;
......
2396  	prWiphy = prWdev->wiphy;
......
2412  	glGetDev(pvData, &prDev);//pvData是struct sdio_func *，prDev其实就是struct sdio_func中的struct device *
......
2417  	/* Some kernel API (ex: cfg80211_get_drvinfo) will use wiphy_dev().
2418  	 * Without set_wiphy_dev(prWdev->wiphy, prDev), those API will crash.
2419  	 */
2420  	set_wiphy_dev(prWdev->wiphy, prDev);
2421  
2422  	/* 4 <2> Create Glue structure */
2423  	prGlueInfo = (struct GLUE_INFO *) wiphy_priv(prWdev->wiphy);
2424  	kalMemZero(prGlueInfo, sizeof(struct GLUE_INFO));
......
2464  	prGlueInfo->prDevHandler =               //alloc_netdev_mq 创建分配初始化并返回一个struct net_dev *
2465  		alloc_netdev_mq(sizeof(struct NETDEV_PRIVATE_GLUE_INFO),
2466  				prInfName,
2467  				ether_setup, CFG_MAX_TXQ_NUM);
......
2478  	/* Device can help us to save at most 3000 packets, after we stopped
2479  	** queue
2480  	*/
2481  	prGlueInfo->prDevHandler->tx_queue_len = 3000;       //struct net_dev.tx_queue_len = 3000
2482  
2483  	/* 4 <3.1.1> Initialize net device varaiables */
2484    //struct net_dev后面的那段内存存储的是struct NETDEV_PRIVATE_GLUE_INFO，在alloc_netdev_mq时分配的
2485  	prNetDevPrivate = (struct NETDEV_PRIVATE_GLUE_INFO *)
2486  			  netdev_priv(prGlueInfo->prDevHandler);
2487  	prNetDevPrivate->prGlueInfo = prGlueInfo;//struct NETDEV_PRIVATE_GLUE_INF = struct GLUE_INFO *
......
2492  	prGlueInfo->prDevHandler->needed_headroom += NIC_TX_HEAD_ROOM;
......
2498  	prGlueInfo->prDevHandler->netdev_ops = &wlan_netdev_ops;//关键，网络协议栈最终调用里面的函数指针完成相关操作
2499  #ifdef CONFIG_WIRELESS_EXT
2500  	prGlueInfo->prDevHandler->wireless_handlers =     //WIRELESS_EXT相关,CFG80211_WEXT不用考虑
2501  		&wext_handler_def;
2502  #endif
2503  	netif_carrier_off(prGlueInfo->prDevHandler);//链路载波关闭，表明当前struct net_dev物理连接处于断开状态
2504  	netif_tx_stop_all_queues(prGlueInfo->prDevHandler);//于停止网络设备的所有发送队列。驱动层在关闭（shutdown）网络设备，或者设备链路断开及设备进入挂起节电模式等情况时，使用此函数停止所有发送队列
2505  	kalResetStats(prGlueInfo->prDevHandler);//网卡统计信息清零，即把struct net_device的stats成员清零
2506  
2507  #if CFG_SUPPORT_SNIFFER
2508  	INIT_WORK(&(prGlueInfo->monWork), wlanMonWorkHandler);//sniffer抓包相关，monitor模式使用，monwlan0
2509  #endif
2510  
2511  	/* 4 <3.1.2> co-relate with wiphy bi-directionally */
        //stuct net_device.ieee80211_ptr  =  struct wireless_dev *
2512  	prGlueInfo->prDevHandler->ieee80211_ptr = prWdev;
2513  
2514  	prWdev->netdev = prGlueInfo->prDevHandler;//stuctwireless_dev.netdev = stuct net_device *
2515  
2516  	/* 4 <3.1.3> co-relate net device & prDev */
2517  	SET_NETDEV_DEV(prGlueInfo->prDevHandler,  //struct net_device.dev->parent = struct wiphy.dev->parent
2518  		       wiphy_dev(prWdev->wiphy));     //这样struct net_device和struct wiphy在sysfs中处于同一目录下
2519  
2520  	/* 4 <3.1.4> set device to glue */
2521  	prGlueInfo->prDev = prDev;  //prGlueInfo->prDev  =  stuct net_device.dev
......
2540  	init_completion(&prGlueInfo->rScanComp);
2541  	init_completion(&prGlueInfo->rHaltComp);
2542  	init_completion(&prGlueInfo->rPendComp);
......
2575  	/* 4 <8> Init Queues */
2576  	init_waitqueue_head(&prGlueInfo->waitq);
2577  	QUEUE_INITIALIZE(&prGlueInfo->rCmdQueue);
2578  	prGlueInfo->i4TxPendingCmdNum = 0;
2579  	QUEUE_INITIALIZE(&prGlueInfo->rTxQueue);
2580  	glSetHifInfo(prGlueInfo, (unsigned long) pvData);
......
2585  	/* main thread is created in this function */
2586  #if CFG_SUPPORT_MULTITHREAD
2587  	init_waitqueue_head(&prGlueInfo->waitq_rx);
2588  	init_waitqueue_head(&prGlueInfo->waitq_hif);
......
2593  #endif
2594  
2595  	return prWdev;
2596  
2597  netcreate_err://错误处理
......
2608  	return NULL;
2609  }
```



vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/os/linux/gl_init.c

```c
1935  static int32_t wlanNetRegister(struct wireless_dev *prWdev)
1936  {
......
1949  		prGlueInfo = (struct GLUE_INFO *) wiphy_priv(prWdev->wiphy);
......
1951  		i4DevIdx = wlanGetDevIdx(prWdev->netdev);
......
1961  		if (register_netdev(prWdev->netdev) < 0) {   //向内核注册struct net_dev
......
1973  		netif_napi_add(prWdev->netdev, &prGlueInfo->rNapi,
1974  			kalRxNapiPoll, NAPI_POLL_WEIGHT);  //向sorftirq注册
1975  		skb_queue_head_init(&prGlueInfo->rRxNapiSkbQ);
......
2012  	return i4DevIdx;	/* success */
2013  }	
```



```
void netif_napi_add(
struct net_device *dev,  
struct napi_struct *napi, 
int (*poll)(struct napi_struct *, int), 
int weight)
```

netif_napi_add会初始化  struct napi_struct  *napi所指向的那块内存，将传入的参数和其他创建的变量来初始化napi

napi->dev = dev;//dev是传入的第一个参数

napi->poll = poll;//poll是传入的第二个参数

napi->weight = weight;//weight是传入的第三个参数

***最后netif_napi_add还会间接地将传入的poll函数指针注册到sorftirq中。在中断上半部使用napi_schedule之后，ksorftirqd线程(或中断下半部)负责回调该函数指针。***

```c
static inline void napi_schedule(struct napi_struct *n) //传入struct napi_struct *napi
```





在wlanNetCreate line 2498,指定了netdev_ops   	prGlueInfo->prDevHandler->netdev_ops = &wlan_netdev_ops;

![image-20220627203000380](wifi.assets/image-20220627203000380.png)



##### NAPI发送函数





##### sdio线程化中断函数注册

初始化时wlanProbe中调用了glBusSetIrq函数

![image-20220627101801954](wifi.assets/image-20220627101801954.png)

使用sdio_claim_irq注册mtk_sdio_interrupt函数后，当触发SDIO中断时mmc子系统的中断处理线程就会间接调用到该函数：

![image-20220627102159685](wifi.assets/image-20220627102159685.png)



![image-20220627102133049](wifi.assets/image-20220627102133049.png)

设置GLUE_FLAG_INT_BIT， 且wake_up_interruptible    waitq_hif这个等待队列





```c
3552  int hif_thread(void *data)
3553  {
......
3572  	while (TRUE) {
......
3593  		do {
3594  			ret = wait_event_interruptible(prGlueInfo->waitq_hif,//在waitq_hif这个wait_queue_head_t上等待
3595  				((prGlueInfo->ulFlag & GLUE_FLAG_HIF_PROCESS)
3596  				!= 0));
3597  		} while (ret != 0);
......
3613  		if (test_and_clear_bit(GLUE_FLAG_INT_BIT,                    //处理GLUE_FLAG_INT_BIT
3614  				       &prGlueInfo->ulFlag)) {
3615  			/* the Wi-Fi interrupt is already disabled in mmc
3616  			 * thread, so we set the flag only to enable the
3617  			 * interrupt later
3618  			 */
3619  			prAdapter->fgIsIntEnable = FALSE;
3620  			if (prGlueInfo->ulFlag & GLUE_FLAG_HALT
3621  #if CFG_CHIP_RESET_SUPPORT
3622  			    || kalIsResetting()
3623  #endif
3624  			   ) {
3625  				/* Should stop now... skip pending interrupt */
3626  				DBGLOG(INIT, INFO,
3627  				       "ignore pending interrupt\n");
3628  			} else {
3629  				/* DBGLOG(INIT, INFO, ("HIF Interrupt!\n")); */
3630  				prGlueInfo->TaskIsrCnt++;
3631  				wlanIST(prAdapter);                                   //真正处理中断事件的地方
3632  			}
3633  		}
......
3676  	}
......
3696  }
```



![image-20220627111117770](wifi.assets/image-20220627111117770.png)



![image-20220627111716724](wifi.assets/image-20220627111716724.png)





![image-20220627111639564](wifi.assets/image-20220627111639564.png)



nicProcessIST_impl依次调用apfnEventFuncTable函数指针数组中的函数指针进行处理，函数指针指向各种处理函数

![image-20220627111811535](wifi.assets/image-20220627111811535.png)





##### 对发送完成中断事件的处理

![image-20220627113421769](wifi.assets/image-20220627113421769.png)









##### 对接收中断事件的处理



![image-20220627140731757](wifi.assets/image-20220627140731757.png)



注意nicProcessRxInterrupt中调用halProcessRxInterrupt来从SDIO读取数据，然后处理数据，其实halProcessRxInterrupt还间接地调用其子函数设置了GLUE_FLAG_RX_BIT，并wakeup了一次可能阻塞在waitq上的main_thread。halProcessRxInterrupt函数返回到nicProcessRxInterrupt后再次设置了GLUE_FLAG_RX_BIT，并wakeup了一次可能阻塞在waitq上的main_thread。个人感觉有些多余，不过无所谓，多的那次main_thread可以什么都不处理(上一次循环已经处理了GLUE_FLAG_RX_BIT flag)。



![image-20220627140808063](wifi.assets/image-20220627140808063.png)



vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/os/linux/hif/sdio/hal_api.c

```c
1402  void halRxSDIOAggReceiveRFBs(IN struct ADAPTER *prAdapter)
1403  {
......
1413  	struct SDIO_RX_COALESCING_BUF *prRxBuf;
......
1429  	for (rxNum = 0; rxNum < 2; rxNum++) {
......
1520  		HAL_READ_RX_PORT(prAdapter, rxNum, u4RxAggLength,                     //从SDIO接口读取数据到prRxBuf
1521  			prRxBuf->pvRxCoalescingBuf, HIF_RX_COALESCING_BUFFER_SIZE);
......
1550  		halDeAggRxPkt(prAdapter, prRxBuf);                                   //处理接收到的数据
1551  
1552  		/* Update statistic counter */
1553  		prHifInfo->rStatCounter.u4PktReadCnt[rxNum] += u4RxAggCount;
1554  		prHifInfo->rStatCounter.u4PortReadCnt[rxNum]++;
1555  	}
1556  
1557  }
```





![image-20220628101317829](wifi.assets/image-20220628101317829.png)



![image-20220628101349039](wifi.assets/image-20220628101349039.png)



![image-20220628101646824](wifi.assets/image-20220628101646824.png)



读取从SDIO数据完毕一路返回到halRxSDIOAggReceiveRFBs后调用halDeAggRxPkt来处理数据：

![image-20220628103742060](wifi.assets/image-20220628103742060.png)



vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/os/linux/hif/sdio/hal_api.c

```
2304  void halDeAggRxPktProc(struct ADAPTER *prAdapter,
2305  			struct SDIO_RX_COALESCING_BUF *prRxBuf)
2306  {
......
2311  	struct QUE *prTempRxRfbList = &rTempRxRfbList;
2312  	struct RX_CTRL *prRxCtrl;
2313  	struct SW_RFB *prSwRfb = (struct SW_RFB *) NULL;
......
2330  	QUEUE_INITIALIZE(prTempFreeRfbList);
2331  	QUEUE_INITIALIZE(prTempRxRfbList);
......
2372  	for (i = 0; i < prRxBuf->u4PktCount; i++) {
......
2391  		prSwRfb->ucPacketType =                  //记录frame数据包的PacketType,后续nicRxProcessRFBs函数中使用
2392  		  (uint8_t)HAL_RX_STATUS_GET_PKT_TYPE(prSwRfb->prRxStatus);
......
2412  		QUEUE_INSERT_TAIL(prTempRxRfbList, &prSwRfb->rQueEntry);//把数据包放入prTempRxRfbList末尾
......
2415  	}
......
2446  		QUEUE_CONCATENATE_QUEUES(&prRxCtrl->rReceivedRfbList,
2447  					prTempRxRfbList);   //使用prTempRxRfbList来初始化prGlueInfo->rRxCtrl->rReceivedRfbList
......
2451  		/* Wake up Rx handling thread */
2452  		set_bit(GLUE_FLAG_RX_BIT, &(prAdapter->prGlueInfo->ulFlag));
2453  		wake_up_interruptible(&(prAdapter->prGlueInfo->waitq));
......
2468  }
```





main_thread经过waitq等待队列以后调用nicRxProcessRFBs来处理GLUE_FLAG_RX_BIT：

vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/os/linux/gl_kal.c

```c
3841  int main_thread(void *data)
3842  {
......
3870  	while (TRUE) {
......
3895  		do {
3896  			ret = wait_event_interruptible(prGlueInfo->waitq,
3897  				((prGlueInfo->ulFlag & GLUE_FLAG_MAIN_PROCESS)
3898  				!= 0));
3899  		} while (ret != 0);
......
4045  #if CFG_SUPPORT_MULTITHREAD
4046  		/* Process RX */
4047  		if (test_and_clear_bit(GLUE_FLAG_RX_BIT,
4048  				       &prGlueInfo->ulFlag))
4049  			nicRxProcessRFBs(prGlueInfo->prAdapter);
......
4113  	return 0;
4114  
4115  }
```







vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/nic/nic_rx.c

```c
3935  /*----------------------------------------------------------------------------*/
3936  /*!
3937   * @brief nicProcessRFBs is used to process RFBs in the rReceivedRFBList queue.
3938   *
3939   * @param prAdapter Pointer to the Adapter structure.
3940   *
3941   * @return (none)
3942   */
3943  /*----------------------------------------------------------------------------*/
3944  void nicRxProcessRFBs(IN struct ADAPTER *prAdapter)
3945  {
3946  	struct RX_CTRL *prRxCtrl;
3947  	struct SW_RFB *prSwRfb = (struct SW_RFB *) NULL;
3948  	struct QUE rTempRfbList;
3949  	struct QUE *prTempRfbList = &rTempRfbList;
3950  	uint32_t u4RxLoopCount, u4Tick;
3951  
3952  	KAL_SPIN_LOCK_DECLARATION();
3953  
3954  	DEBUGFUNC("nicRxProcessRFBs");
3955  
3956  	ASSERT(prAdapter);
3957  
3958  	prRxCtrl = &prAdapter->rRxCtrl;
3959  	ASSERT(prRxCtrl);
3960  
3961  	prRxCtrl->ucNumIndPacket = 0;
3962  	prRxCtrl->ucNumRetainedPacket = 0;
3963  	u4RxLoopCount = prAdapter->rWifiVar.u4TxRxLoopCount;
3964  	u4Tick = kalGetTimeTick();
3965  
3966  	QUEUE_INITIALIZE(prTempRfbList);
3967  
3968  	while (u4RxLoopCount--) {
3969  		while (QUEUE_IS_NOT_EMPTY(&prRxCtrl->rReceivedRfbList)) {//prGlueInfo->rRxCtrl->rReceivedRfbList
3970  
3971  			/* check process RFB timeout */
3972  			if ((kalGetTimeTick() - u4Tick) > RX_PROCESS_TIMEOUT) {
3973  				DBGLOG(RX, WARN, "Rx process RFBs timeout\n");
3974  				break;
3975  			}
3976  
3977  			KAL_ACQUIRE_SPIN_LOCK(prAdapter, SPIN_LOCK_RX_QUE);
3978  			QUEUE_MOVE_ALL(prTempRfbList,prTempRfbList        //把prGlueInfo->rRxCtrl->rReceivedRfbList中
3979  				&prRxCtrl->rReceivedRfbList);                 //的元素移动到prTempRfbList队列
3980  			KAL_RELEASE_SPIN_LOCK(prAdapter, SPIN_LOCK_RX_QUE);
3981  
3982  			while (QUEUE_IS_NOT_EMPTY(prTempRfbList)) {//遍历prTempRfbList队列中的元素
3983  				QUEUE_REMOVE_HEAD(prTempRfbList,       //每次遍历时取出并删除第一个元素
3984  					prSwRfb, struct SW_RFB *);
3985  
3986  				if (!prSwRfb)
3987  					break;
3988  #if CFG_SUPPORT_WAKEUP_REASON_DEBUG
3989  				if (kalIsWakeupByWlan(prAdapter))
3990  					nicRxCheckWakeupReason(prAdapter,
3991  							       prSwRfb);
3992  #endif
3993  
3994  				switch (prSwRfb->ucPacketType) {
3995  				case RX_PKT_TYPE_RX_DATA:
3996  					if (HAL_MON_EN(prAdapter)) {
3997  						nicRxProcessMonitorPacket(
3998  							prAdapter, prSwRfb);
3999  						break;
4000  					}
4001  
4002  					nicRxProcessDataPacket(             //最终使用NAPI丢入了网络协议栈，貌似是处理IP数据包
4003  						prAdapter, prSwRfb);
4004  					break;
4005  
4006  				case RX_PKT_TYPE_SW_DEFINED:
4007  					/* HIF_RX_PKT_TYPE_EVENT */
4008  					if ((prSwRfb->prRxStatus->u2PktTYpe &
4009  					     RXM_RXD_PKT_TYPE_SW_BITMAP) ==
4010  					    RXM_RXD_PKT_TYPE_SW_EVENT) {
4011  						nicRxProcessEventPacket(      //处理相关事件，例如扫描完成事件
4012  							prAdapter,
4013  							prSwRfb);
4014  					}
4015  					/* case HIF_RX_PKT_TYPE_MANAGEMENT: */
4016  					else if ((prSwRfb->prRxStatus->u2PktTYpe
4017  						& RXM_RXD_PKT_TYPE_SW_BITMAP) ==
4018  						RXM_RXD_PKT_TYPE_SW_FRAME) {
4019  						/* OFLD pkts should go data flow
4020  						 * 1: EAPOL
4021  						 * 2: ARP / NS
4022  						 * 3: TDLS
4023  						 */
4024  						if (HAL_RX_STATUS_GET_OFLD(               
4025  							prSwRfb->prRxStatus))
4026  							nicRxProcessDataPacket(//最终使用NAPI丢入网络协议栈，貌似是处理处理EAPOL/ARP/TDLS帧
4027  							prAdapter, prSwRfb);
4028  						else
4029  							nicRxProcessMgmtPacket(//处理收到的80211 MAC管理帧，例如probe response
4030  							prAdapter, prSwRfb);
4031  					} else {
4032  						DBGLOG(RX, ERROR,
4033  							"u2PktTYpe(0x%04X) is OUT OF DEF.!!!\n",
4034  						  prSwRfb->prRxStatus->
4035  						  u2PktTYpe);
4036  						DBGLOG_MEM8(RX, ERROR,
4037  							prSwRfb->pucRecvBuff,
4038  							prSwRfb->prRxStatus->
4039  							u2RxByteCount);
4040  
4041  						/*ASSERT(0);*/
4042  						nicRxReturnRFB(prAdapter,
4043  							prSwRfb);
4044  						RX_INC_CNT(prRxCtrl,
4045  							RX_TYPE_ERR_DROP_COUNT);
4046  						RX_INC_CNT(prRxCtrl,
4047  							RX_DROP_TOTAL_COUNT);
4048  
4049  					}
4050  					break;
4051  
4052  				case RX_PKT_TYPE_MSDU_REPORT://  MAC Service Data Unit   
4053  					nicRxProcessMsduReport(prAdapter,
4054  						prSwRfb);
4055  					break;
4056  
4057  				/* case HIF_RX_PKT_TYPE_TX_LOOPBACK: */
4058  				/* case HIF_RX_PKT_TYPE_MANAGEMENT: */
4059  				case RX_PKT_TYPE_TX_STATUS:
4060  				case RX_PKT_TYPE_RX_VECTOR:
4061  				case RX_PKT_TYPE_TM_REPORT:
4062  				default:
4063  					nicRxReturnRFB(prAdapter, prSwRfb);
4064  					RX_INC_CNT(prRxCtrl,
4065  						RX_TYPE_ERR_DROP_COUNT);
4066  					RX_INC_CNT(prRxCtrl,
4067  						RX_DROP_TOTAL_COUNT);
4068  					DBGLOG(RX, ERROR, "ucPacketType = %d\n",
4069  					       prSwRfb->ucPacketType);
4070  					break;
4071  				}
......
```





vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/nic/nic_rx.c

```c
1966  void nicRxProcessDataPacket(IN struct ADAPTER *prAdapter,
1967  			    IN OUT struct SW_RFB *prSwRfb)
1968  {
1969  	struct RX_CTRL *prRxCtrl;
1970  	struct SW_RFB *prRetSwRfb, *prNextSwRfb;
......
2180  		prRetSwRfb = qmHandleRxPackets(prAdapter, prSwRfb);//在里面把eDst = RX_PKT_DESTINATION_HOST;
......
2237  				switch (prRetSwRfb->eDst) {
2238  				case RX_PKT_DESTINATION_HOST:
......
2254  					nicRxProcessPktWithoutReorder(
2255  						prAdapter, prRetSwRfb);
2256  					break;
......
4052  				case RX_PKT_TYPE_MSDU_REPORT:
4053  					nicRxProcessMsduReport(prAdapter,
4054  						prSwRfb);
4055  					break;
```



vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/nic/nic_rx.c

```
1180  void nicRxProcessPktWithoutReorder(IN struct ADAPTER
1181  				   *prAdapter, IN struct SW_RFB *prSwRfb)
1182  {
......
1277  		kalSetTxEvent2Rx(prAdapter->prGlueInfo);
1306  }
```



![image-20220628113101815](wifi.assets/image-20220628113101815.png)



vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/os/linux/gl_kal.c

```c
3698  int rx_thread(void *data)
3699  {
......
3739  	while (TRUE) {
......
3755  		/*
3756  		 * sleep on waitqueue if no events occurred.
3757  		 */
3758  		do {
3759  			ret = wait_event_interruptible(prGlueInfo->waitq_rx,       // wait_event  waitq_rx
3760  			    ((prGlueInfo->ulFlag & GLUE_FLAG_RX_PROCESS) != 0));
3761  		} while (ret != 0);
......
3768  		if (test_and_clear_bit(GLUE_FLAG_RX_TO_OS_BIT,            //处理GLUE_FLAG_RX_TO_OS_BIT标志
3769  				       &prGlueInfo->ulFlag)) {
3770  			u4LoopCount =
3771  			    prGlueInfo->prAdapter->rWifiVar.u4Rx2OsLoopCount;
3772  
3773  			while (u4LoopCount--) {
3774  				while (QUEUE_IS_NOT_EMPTY(
3775  				       &prGlueInfo->prAdapter->rRxQueue)) {
3776  					QUEUE_INITIALIZE(prTempRxQue);
3777  
3778  					GLUE_ACQUIRE_SPIN_LOCK(prGlueInfo,
3779  					    SPIN_LOCK_RX_TO_OS_QUE);
3780  					QUEUE_MOVE_ALL(prTempRxQue,
3781  					    &prGlueInfo->prAdapter->rRxQueue);
3782  					GLUE_RELEASE_SPIN_LOCK(prGlueInfo,
3783  					    SPIN_LOCK_RX_TO_OS_QUE);
3784  
3785  					while (QUEUE_IS_NOT_EMPTY(
3786  						prTempRxQue)) {
3787  						QUEUE_REMOVE_HEAD(prTempRxQue,
3788  						    prQueueEntry,
3789  						    struct QUE_ENTRY *);
3790  						kalRxIndicateOnePkt(prGlueInfo,                      //真正处理的地方
3791  						    (void *)
3792  						    GLUE_GET_PKT_DESCRIPTOR(
3793  						    prQueueEntry));
3794  					}
3795  
3796  				    KAL_WAKE_LOCK_TIMEOUT(prGlueInfo->prAdapter,
3797  					&prGlueInfo->rTimeoutWakeLock,
3798  					MSEC_TO_JIFFIES(prGlueInfo->prAdapter
3799  					->rWifiVar.u4WakeLockRxTimeout));
3800  				}
3801  			}
3802  		}
3803  	}
```

vendor/mediatek/kernel_modules/connectivity/wlan/core/gen4-mt7663/os/linux/gl_kal.c

```
976  uint32_t kalRxIndicateOnePkt(IN struct GLUE_INFO
977  			     *prGlueInfo, IN void *pvPkt)
978  {
......
1130  		napi_schedule(&prGlueInfo->rNapi);
1131  		return WLAN_STATUS_SUCCESS;
......
1141  }
```



在wlanNetRegister中调用了

```
1973  		netif_napi_add(prWdev->netdev, &prGlueInfo->rNapi,
1974  			kalRxNapiPoll, NAPI_POLL_WEIGHT);
```

因此在kalRxIndicateOnePkt调用napi_schedule(&prGlueInfo->rNapi)后，在ksorftirqd线程中会间接调用到kalRxNapiPoll，在kalRxNapiPoll中负责调用NAPI的接口将skb发送到上层网络协议栈。



![image-20220628113633350](wifi.assets/image-20220628113633350.png)







## 11 模组适配

### 1、android-hardware-wifi@1.0-service

frameworks/opt/net/wifi/libwifi_hal/Android.mk

```
BOARD_WLAN_DEVICE           :=  MediaTek

ifeq ($(BOARD_WLAN_DEVICE), MediaTek)
103   # support MTK WIFI HAL
104   LIB_WIFI_HAL := libwifi-hal-mt66xx
......
114 include $(CLEAR_VARS)
115 LOCAL_MODULE := libwifi-hal
116 LOCAL_PROPRIETARY_MODULE := true
117 LOCAL_CFLAGS := $(wifi_hal_cflags)
118 LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
119 LOCAL_EXPORT_C_INCLUDE_DIRS := \
120     $(LOCAL_PATH)/include
121 LOCAL_EXPORT_HEADER_LIBRARY_HEADERS := libhardware_legacy_headers
122 LOCAL_HEADER_LIBRARIES := libhardware_legacy_headers
123 LOCAL_SHARED_LIBRARIES := \
124     libbase \
125     libcutils \
126     liblog \
127     libnl \
128     libutils \
129     librkwifi-ctrl \
130     $(VENDOR_LOCAL_SHARED_LIBRARIES)
131 LOCAL_SRC_FILES := \
132     driver_tool.cpp \
133     hal_tool.cpp
134 LOCAL_WHOLE_STATIC_LIBRARIES := $(LIB_WIFI_HAL) libwifi-hal-common
135 include $(BUILD_SHARED_LIBRARY)
```

 libwifi-hal.so由 libwifi-hal-mt66xx.a(模组厂商的)和libwifi-hal-common.a构成

android-hardware-wifi@1.x-service 使用 libwifi-hal.so







wifi_hal/Android.mk

```
LOCAL_PATH := $(call my-dir)

# Make the HAL library
# ============================================================
include $(CLEAR_VARS)

LOCAL_REQUIRED_MODULES :=

LOCAL_CFLAGS += -Wno-unused-parameter -Wno-int-to-pointer-cast
LOCAL_CFLAGS += -Wno-maybe-uninitialized -Wno-parentheses
LOCAL_CPPFLAGS += -Wno-conversion-null

ifneq ($(TARGET_BUILD_VARIANT), user)
LOCAL_CFLAGS += -DCONFIG_DEBUG_VER
endif

LOCAL_C_INCLUDES += \
	external/libnl/include \
	$(call include-path-for, libhardware_legacy)/hardware_legacy \
	external/wpa_supplicant_8/src/drivers

LOCAL_SRC_FILES := \
	wifi_hal.cpp \
	common.cpp \
	cpp_bindings.cpp \
	wifi_logger.cpp \
	wifi_offload.cpp

LOCAL_SHARED_LIBRARIES += libutils liblog
LOCAL_MODULE := libwifi-hal-mt66xx
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk

include $(BUILD_STATIC_LIBRARY)
```





### 2、wpa_supplicant 



```
WPA_SUPPLICANT_VERSION      := VER_0_8_X

BOARD_HOSTAPD_DRIVER        := NL80211

BOARD_WPA_SUPPLICANT_DRIVER := NL80211

BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_mt66xx

BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_mt66xx
```

lib_driver_cmd_mt66xx是静态库

wpa_supplicant 在构建时需要BOARD_WPA_SUPPLICANT_PRIVATE_LIB指定的静态库模块

 



/vendor/etc/wifi/目录下

wpa-supplicant.conf

wpa-supplicant-overlay.conf

p2p-supplicant-overlay.conf





### 3、固件及驱动配置文件

/vendor/firmware/目录下

firmware.bin（eg: mt7663_patch_e2_hdr.bin  WIFI_RAM_CODE_MT7663.bin）

driver_config_file(eg:   TxPwrLimit_MT76x3.dat  wifi_mt7661.cfg  )



TxPwrLimit_MT76x3.dat

```
# The unit of each TxPower value is 0.5dBm.
# 'x' means use efuse default value instead of from this table
# Channels in the table are treated as center channel.

<Ver:01>

[00]                    //这个是国家码，默认00
<cck,  c1, c2, c5, c11> //横行c1, c2, c5, c11分别对应1、2、5.5、11这四挡速率,竖行对应不同的信道
ch001, 43, 43, 43,  43  //ch001是信道0，43是该信道和速率下对应的Txpwr,单位是0.5dbm
......
</cck>                  //cck是802.11b

<ofdm, o6, o9, o12, o18, o24, o36, o48, o54> //横行分别对应6、9、12、18、24、36、48、54这几档速率,竖行对应不同的信道
ch001, 41, 41,  41,  41,  39,  39,  38,  37
......
</ofdm>                                      //ofdm是802.11a/g，2.4G部分是11g，5G部分是11a

<ht20, m0, m1, m2, m3, m4, m5, m6, m7>    //横行为不同的编码方式即mcs0、mcs1等等这些，竖行对应不同的信道
ch001, 39, 39, 39, 38, 38, 37, 37, 36
......
</ht20>                                    //ht20是802.11n HT20 

<ht40, m0, m1, m2, m3, m4, m5, m6, m7, m32>  //横行为不同的编码方式即mcs0、mcs1等等这些，竖行对应不同的信道
ch001, 39, 39, 39, 38, 38, 36, 36, 35,  39
......
</ht40>                                   //ht40是802.11n HT40 

<vht20, m0, m1, m2, m3, m4, m5, m6, m7, m8, m9>  //横行为不同的编码方式即mcs0、mcs1等等这些，竖行对应不同的信道
ch001,  39, 39, 39, 38, 38, 37, 37, 36, 33, 32
......
</vht20>                                   //vht20是802.11ac  

<vht40, m0, m1, m2, m3, m4, m5, m6, m7, m8, m9>  //横行为不同的编码方式即mcs0、mcs1等等这些，竖行对应不同的信道
ch001,  39, 39, 39, 38, 38, 36, 36, 35, 32, 31
......
</vht40>                                      //vht40是802.11ac 

<vht80, m0, m1, m2, m3, m4, m5, m6, m7, m8, m9>  //横行为不同的编码方式即mcs0、mcs1等等这些，竖行对应不同的信道
ch036,  37, 37, 37, 35, 35, 34, 34, 32, 30, 30
......
</vht80>                                    //vht80是802.11ac 
```



文件默认只有设置[00]国家码，如果在平台上有放置这个.dat文件，驱动会去读这个文件中的值。正常Efuse settings >= TxPwrLimit table， 即efuse中设定的值对TxPwrLimit表总是有较大的值或相等的值。如果想微调某个模式，某个信道的TX POWER值，可以修改TxPwrLimit_MT76x3.dat。

修改注意事项：

1） 比如要定义CN或US的，可以整个把目前.dat文件中内容copy再粘贴，然后将[00]改为[CN]或[US]，即增加一个[CN]或[US]的定义

2） 然后修改你对应国家码里某个模式某个信道下的设定值。Unit: 0.5 dBm 。





### 4、wifi driver

dts

sdio platform

sdio device

sdio drive



### 5、framework

WifiNative.java













## 12. wifi相关名词及概念



  

```
802.11b : WiFi 1
802.11a : WiFi 2
802.11g : WiFi 3
802.11n : WiFi 4
802.11ac : WiFi 5
802.11ax : WiFi 6
802.11be : WiFi 7
```

大话无线通信.pdf




书籍推荐

https://zhuanlan.zhihu.com/p/20716157



**3.Linux Kernel Networking**

该书主要是描述了Linux内核中网络的实现部分，其第12章具体就是对应无线模块。由于描述Linux内核中无线模块资料比较少，所以这本书也是比较推荐的，只不过对于细节部分，该书描述不是特别细致，不过总体还是不错的。该书以及其源码的资源如下，【[Linux Kernel Networking ](https://link.zhihu.com/?target=http%3A//download.csdn.net/detail/fzxy002763/9578509)】。

**4.Linux Wi-Fi open source drivers-mac 802.11**

该材料比较完整的叙述了从kernel到802.11驱动底层的一个函数调用过程，用来学习802.11具体驱动过程是一份比较好的材料。该资料的资源如下，【[Linux Wi-Fi open source drivers-mac 802.11](https://link.zhihu.com/?target=http%3A//download.csdn.net/detail/fzxy002763/9712191)】。











CSMA/CD与CSMA/CA

https://zhuanlan.zhihu.com/p/20731045

openwifi

https://www.zhihu.com/zvideo/1499133891747737600





mac地址：

https://baike.baidu.com/item/MAC%E5%9C%B0%E5%9D%80/1254181?fr=aladdin

网络中每台设备都有一个唯一的网络标识，这个地址叫MAC地址或网卡地址，由网络设备制造商生产时写在硬件内部。MAC地址则是48位的（6个字节），通常表示为12个16进制数，每2个16进制数之间用冒号隔开，如08：00：20：0A：8C：6D就是一个MAC地址。具体如下图所示，其前3字节表示OUI（Organizationally Unique Identifier），是[IEEE](https://baike.baidu.com/item/IEEE/150905)的注册管理机构给不同厂家分配的代码，区分不同的厂家。后3字节由厂家自行分配 [2] 。

MAC地址最高字节（MSB）的低第二位（LSb）表示这个MAC地址是全局的还是本地的，即U/L（Universal/Local）位，如果为0，表示是全局地址。所有的OUI这一位都是0。

MAC地址最高字节（MSB）的低第一位(LSb），表示这个MAC地址是单播还是多播。0表示单播。





https://zhuanlan.zhihu.com/p/20731045

https://www.zhihu.com/zvideo/1499133891747737600



* Hotspot 2.0  r2  OSEN  HS20    

Hotspot2.0是WFA（Wi-Fi Alliance）的一项技术规范，该规范允许无线终端在不需要额外身份标识的情况下，利用802.11u完成WLAN网络中的自动身份识别和无缝切换，使得无线终端在Wi-Fi漫游时，达到类似蜂窝网络用户体验的效果。简单来说用于wifi与2G/3G/4G网络直接进行漫游。它对应eap外层也即第一阶段认证的一种方式：

![image-20220428204036288](wifi.assets/image-20220428204036288.png)



* WAIP

 国内搞的一套wifi认证方式





* Wi-Fi Simple Configuration（以后简写为WSC）：该项技术用于简化SOHO环境中无线网络的配置和使用。举一个简单的例子，配置无线网络环境时，网管需要首先为AP设置SSID、安全属性（如身份认证方法、加密方法等）。然后他还得把SSID、密码告诉给该无线网络的使用者。可是这些安全设置信息对普通大众而言还是有些复杂。而有了WSC之后，用户只需输入PIN码（Personal Identification Number，一串数字），或者摁一下专门的按钮（WSC中，该按钮被称为Push Button）甚至用户只要拿着支持NFC的手机到目标AP（它必须也支持NFC）旁刷一下，这些安全设置就能被自动配置好。有了这些信息，手机就能连接上目标无线网络了。显然，相比让用户记住SSID、密码等信息，WSC要简单多了。wpas_wps_in_use判断是否需要在Probe Request中添加WSC IE。WPAS的判断标准比较简单，就是查询所有的wpa_ssid对象，判断它们的key_mgmt是否设置了**WPA_KEY_MGMT_WPS**。如果有，表明搜索的时候需要支持WSC IE。我们在介绍“WPS_PIN命令处理”时曾说过，WPAS将添加一个wpa_ssid对象，并设置key_mgmt为WPA_KEY_MGMT_WPS。https://www.kancloud.cn/alex_wsc/android-wifi-nfc-gps/414111



* Wi-Fi Peer-to-Peer（以后简写为P2P）：P2P的商品名（brand name）为Wi-Fi Direct。它支持多个Wi-Fi设备在没有AP的情况下相互连接。笔者个人认为P2P是Wi-Fi中最具应用前景的一项技术。例如，在家庭中，用户可直接把手机上的内容通过P2P技术传输到电视机上和家人分享。



* 帧格式


* 管理帧

![img](wifi.assets/aaaaa.png)



**管理帧的frame body由fixed field(固定字段)和Information Element(IE,信息元素)组成。**首先是固定长度字段，然后才是可变长度的信息元素。各种固定字段和各种信息元素的排列方式有一定顺序不能乱序，但管理帧可以选择携带或不携带某些类型的固定字段和信息元素。不同的管理帧只是选择携带的某些类型的固定字段和信息元素不同罢了。详细参考《802.11无线网络权威指南》



* 数据帧

  ![ccccc](wifi.assets/ccccc.jpg)

Address4字段(SA)通常情况下没有，即长度为0，只有在WDS(无线网络桥接)时才使用。

数据帧的frame body携带下图中802.11 MAC payload中除FCS之外的其他内容：

![image-20220428212105079](wifi.assets/image-20220428212105079.png)

根据Type字段的指示，IP Packet字段中可以存放对应的报文，例如IP报文，EAPOL报文等。
