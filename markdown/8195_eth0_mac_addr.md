

目录：device/mediateksample/xxxxx/ProjectConfig.mk

ProjectConfig.mk结构：
AUTO_ADD_GLOBAL_DEFINE_BY_NAME:  将它后面的名字，定义为宏
AUTO_ADD_GLOBAL_DEFINE_BY_NAME_VALUE:将它后面的名字，定义为宏，同时宏的值是配置文件中的值
AUTO_ADD_GLOBAL_DEFINE_BY_VALUE:将他后面的名字对应的每个值，都定义为宏







devm_kmalloc

devm_kzmalloc

https://www.byteisland.com/devm_kmalloc-%E5%AE%9E%E7%8E%B0%E4%B8%8E%E4%BD%BF%E7%94%A8/







of_device_get_match_data





const   char  * a



char *   const    b



```
const   char  * * a;

a是一个指针，指向的类型是const   char  *


const   char  ****......*** b;
b是一个指针，指向的类型是const   char  ****......**
```





```
struct stmmac_resources结构体
http://10.234.22.197:6015/source/xref/mtk8195_android_R/kernel-4.19/drivers/net/ethernet/stmicro/stmmac/stmmac.h#36


//在mediatek_dwmac_probe函数中设置mac地址
struct stmmac_resources stmmac_res;
plat_dat = stmmac_probe_config_dt(pdev, &stmmac_res.mac);
~/project\mtk8195\kernel-4.19\drivers\net\ethernet\stmicro\stmmac\dwmac-mediatek.c       1047
stmmac_probe_config_dt(pdev, &stmmac_res.mac);

stmmac_probe_config_dt
~/project/mtk8195/kernel-4.19/drivers/net/ethernet/stmicro/stmmac/stmmac_platform.c  386
*mac = of_get_mac_address(np);


of_get_mac_address
http://10.234.22.197:6015/source/xref/mtk8195_android_R/kernel-4.19/drivers/of/of_net.c#73
addr = of_get_mac_addr(np, "mac-address");

```



 