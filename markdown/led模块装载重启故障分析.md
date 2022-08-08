https://i.zte.com.cn/#/space/5281ec5b761f4c19b7ef3868290cbbe1/wiki/page/0a348123119e49d4ba2abf621ac05f1a/view



adb pull /data/vendor/aee_exp  .

打开aee_exp下最后一次的db.fatal.07.KE\db.fatal.07.KE.dbg.DEC\SYS_KERNEL_LOG找到崩溃前的内核日志找到insmod相关







\aee_exp\db.fatal.07.KE\db.fatal.07.KE.dbg.DEC\__exp_main.txt中有崩溃的Backtrace信息

<1>[  976.945656] (5)[7247:insmod]Unable to handle kernel NULL pointer dereference at virtual address 0000000000000000



<1>[  976.945666] (5)[7247:insmod]Mem abort info:
<1>[  976.945670] (5)[7247:insmod]  ESR = 0x96000005
<1>[  976.945674] (5)[7247:insmod]  Exception class = DABT (current EL), IL = 32 bits
<1>[  976.945676] (5)[7247:insmod]  SET = 0, FnV = 0
<1>[  976.945679] (5)[7247:insmod]  EA = 0, S1PTW = 0
<1>[  976.945681] (5)[7247:insmod]Data abort info:
<1>[  976.945684] (5)[7247:insmod]  ISV = 0, ISS = 0x00000005
<1>[  976.945687] (5)[7247:insmod]  CM = 0, WnR = 0
<1>[  976.945691] (5)[7247:insmod]user pgtable: 4k pages, 39-bit VAs, pgdp = 00000000b6788e2b
<1>[  976.945694] (5)[7247:insmod][0000000000000000] pgd=0000000000000000, pud=0000000000000000
<0>[  976.945701]-(5)[7247:insmod]Internal error: Oops: 96000005 [#1] PREEMPT SMP
<5>[  976.945707]-(5)[7247:insmod]Kernel Offset: 0x17ef800000 from 0xffffff8008000000
<5>[  976.945710]-(5)[7247:insmod]PHYS_OFFSET: 0xfffffff400000000
<4>[  976.945712]-(5)[7247:insmod]Modules linked in: leddrv ffffff97a5afe000 0 16384 0 (O+) wlan_drv_gen4_mt7921 ffffff97a5b4d000 ffffff97a5d5a000 2146304 167936 (O) btmtk_usb_unify ffffff97a5b1d000 ffffff97a5b48000 172032 16384 (O) kdrv_dolby_vision ffffff97a5b15000 ffffff97a5b1a000 16384 8192 (O) kdrv_alg ffffff97a5b09000 ffffff97a5b10000 24576 16384 (O) kdrv_xc ffffff97a5aa8000 ffffff97a5afe000 348160 40960 (O) fpsgo ffffff97a59de000 ffffff97a5aa5000 811008 8192 (PO)
<4>[  976.946735]-(5)[7247:insmod]CPU: 5 PID: 7247 Comm: insmod Tainted: P S         O      4.19.127 #2
<4>[  976.946738]-(5)[7247:insmod]Hardware name: MT8195V/C(ENG) (DT)
<4>[  976.946742]-(5)[7247:insmod]pstate: 80400009 (Nzcv daif +PAN -UAO)
<4>[  976.946752]-(5)[7247:insmod]pc : __pi_strcmp+0x8c/0x154
<4>[  976.946757]-(5)[7247:insmod]lr : kset_find_obj+0x50/0x90
<4>[  976.946759]-(5)[7247:insmod]sp : ffffff80233e3a90
<4>[  976.946762]-(5)[7247:insmod]x29: ffffff80233e3a90 x28: ffffff80233e3dd0 
<4>[  976.946765]-(5)[7247:insmod]x27: ffffffcdb6517c20 x26: 0000000000000026 
<4>[  976.946769]-(5)[7247:insmod]x25: ffffff97f8d41018 x24: ffffff97f8d6db00 
<4>[  976.946772]-(5)[7247:insmod]x23: ffffffce25917100 x22: ffffffce25917108 
<4>[  976.946775]-(5)[7247:insmod]x21: ffffffce25e4fe00 x20: 0000000000000000 
<4>[  976.946778]-(5)[7247:insmod]x19: ffffffce25e4fe10 x18: 0000000000000000 
<4>[  976.946781]-(5)[7247:insmod]x17: 00000000030ae7c6 x16: ffffff97f7f03428 
<4>[  976.946784]-(5)[7247:insmod]x15: 0000000000000000 x14: 0000000000100000 
<4>[  976.946787]-(5)[7247:insmod]x13: 0000000000000004 x12: ffffff97f7f03570 
<4>[  976.946790]-(5)[7247:insmod]x11: 0000000000000008 x10: 0101010101010101 
<4>[  976.946793]-(5)[7247:insmod]x9 : fffffffffffffffc x8 : 0000000000000008 
<4>[  976.946796]-(5)[7247:insmod]x7 : 0000000000000004 x6 : ffffff80138cba34 
<4>[  976.946799]-(5)[7247:insmod]x5 : ffffff80233e3ad0 x4 : ffffffce3fe53520 
<4>[  976.946802]-(5)[7247:insmod]x3 : 0000000000299c4d x2 : 0000000000000072 
<4>[  976.946805]-(5)[7247:insmod]x1 : 0000000000000000 x0 : ffffff97f8b16c15 
<4>[  976.946809]-(5)[7247:insmod]CPU: 5 PID: 7247 Comm: insmod Tainted: P S         O      4.19.127 #2
<4>[  976.946812]-(5)[7247:insmod]Hardware name: MT8195V/C(ENG) (DT)
<4>[  976.946815]-(5)[7247:insmod]Call trace:
<4>[  976.946820]-(5)[7247:insmod] dump_backtrace+0x0/0x18c
<4>[  976.946825]-(5)[7247:insmod] show_stack+0x14/0x1c
<4>[  976.946828]-(5)[7247:insmod] dump_stack+0xb8/0xf0
<4>[  976.946833]-(5)[7247:insmod] mrdump_common_die+0xc4/0x10c
<4>[  976.946836]-(5)[7247:insmod] ipanic_die+0x30/0x40
<4>[  976.946839]-(5)[7247:insmod] notify_die+0x64/0xb4
<4>[  976.946843]-(5)[7247:insmod] die+0xac/0x37c
<4>[  976.946847]-(5)[7247:insmod] die_kernel_fault+0x60/0x70
<4>[  976.946851]-(5)[7247:insmod] die_kernel_fault+0x0/0x70
<4>[  976.946854]-(5)[7247:insmod] do_page_fault+0x54/0x4e0
<4>[  976.946858]-(5)[7247:insmod] do_translation_fault+0x2c/0x40
<4>[  976.946860]-(5)[7247:insmod] do_mem_abort+0x4c/0xf8
<4>[  976.946863]-(5)[7247:insmod] el1_da+0x1c/0xac
<4>[  976.946867]-(5)[7247:insmod] __pi_strcmp+0x8c/0x154
<4>[  976.946871]-(5)[7247:insmod] driver_register+0x58/0x108
<4>[  976.946875]-(5)[7247:insmod] __platform_driver_register+0x40/0x48
<4>[  976.946881]-(5)[7247:insmod] init_module+0x1c/0xfe0 [leddrv]
<4>[  976.946884]-(5)[7247:insmod] do_one_initcall+0x120/0x2a8
<4>[  976.946889]-(5)[7247:insmod] do_init_module+0x5c/0x238
<4>[  976.946893]-(5)[7247:insmod] load_module+0x3048/0x3824
<4>[  976.946897]-(5)[7247:insmod] __arm64_sys_finit_module+0xe4/0x114
<4>[  976.946901]-(5)[7247:insmod] el0_svc_common+0x98/0x148
<4>[  976.946904]-(5)[7247:insmod] el0_svc_handler+0x60/0x78
<4>[  976.946907]-(5)[7247:insmod] el0_svc+0x8/0xc
<5>[  976.947056]-(5)[7247:insmod]mrdump: cpu[5] tsk:0xffffffcdf02a3a00 ti:0xffffff80233e0000
<6>[  976.955760]-(5)[7247:insmod]aee_save_modules failed
<5>[  976.955763]-(5)[7247:insmod]mrdump: add MSDC:0x0 sz:0x0 failed
<5>[  976.955769]-(5)[7247:insmod]mrdump: add UFS:0x0 sz:0x0 failed
<5>[  976.956085]-(5)[7247:insmod]mrdump: add CCCI:0xffffff97f915ec40 sz:0x0 failed
<5>[  976.956088]-(5)[7247:insmod]mrdump: add MD:0xffffff97f915ec40 sz:0x0 failed

el0_svc是系统调用的自陷异常，进入内核态后依次执行了
el0_svc
__arm64_sys_finit_module
load_module
do_init_module
do_one_initcall
init_module
__platform_driver_register
driver_register
__pi_strcmp
执行到__pi_strcmp+0x8c处发生了异常硬件跳转到了el1_da进行执行

el1_da 是定义在kernel-4.19/arch/arm64/kernel/entry.S中的异常向量处理函数
显然在执行异常向量前的__pi_strcmp符号加0x8c偏移的地方崩溃了:

<4>[  976.946867]-(5)[7247:insmod] __pi_strcmp+0x8c/0x154

打开trace32 arm simulator软件，首先依次点击上方菜单栏
CPU   ->   System Settings ->   设置CPU为 Cortex A57， Mode为Up
接着将vmlinux文件拖入下方命令栏装载vmlinux，随后在命令栏中输入：
l __pi_strcmp+0x8c
由此弹出的对话框可以直接定位到__pi_strcmp+0x8c的那条指令：

```
                  125 | ////////////////////////////////////////////////////////////////////////////////////
MX:0xFFFFFF8008F18978 | 38401423       ldrb    w3,[x1],#0x1     ; w3,[x1],#1
```

点击这条指令所在行后，在左下角也能看到该条指令相对某个符号的偏移:
MX:0xFFFFFF8008F18978  \\vmlinux\Global\__pi_strcmp+0x8C
该条指令上面的一行意思是该条指令由源文件中第125行生成。

因为该条指令导致异常硬件跳转到了el1_da，一定是因为x1寄存器中的地址为空。
左键选中某条命令再右键点击View Info可以看到更多的信息包括源文件路径：
\home\gonghj\project\mtk8195\kernel-4.19\arch\arm64\lib\strcmp.S
找到源文件的第125行附近：
		110 .Lmisaligned8:
		111 	/*
		112 	* Get the align offset length to compare per byte first.
		113 	* After this process, one string's address will be aligned.
		114 	*/
		115 	and	tmp1, src1, #7
		116 	neg	tmp1, tmp1
		117 	add	tmp1, tmp1, #8
		118 	and	tmp2, src2, #7
		119 	neg	tmp2, tmp2
		120 	add	tmp2, tmp2, #8
		121 	subs	tmp3, tmp1, tmp2
		122 	csel	pos, tmp1, tmp2, hi /*Choose the maximum. */
		123 .Ltinycmp:
		124 	ldrb	data1w, [src1], #1
		125 	ldrb	data2w, [src2], #1
		126 	subs	pos, pos, #1
		127 	ccmp	data1w, #1, #0, ne  /* NZCV = 0b0000.  */
		128 	ccmp	data1w, data2w, #0, cs  /* NZCV = 0b0000.  */
		129 	b.eq	.Ltinycmp
		130 	cbnz	pos, 1f /*find the null or unequal...*/
		131 	cmp	data1w, #1
		132 	ccmp	data1w, data2w, #0, cs
		133 	b.eq	.Lstart_align /*the last bytes are equal....*/
		134 1:
		135 	sub	result, data1, data2
		136 	ret

显然是第二个参数src2为空导致的,现在要找到谁调用strcmp


针对__pi_strcmp的上一个地方：
<4>[  976.946871]-(5)[7247:insmod] driver_register+0x58/0x108
继续输入指令

l driver_register+0x58

弹出对话框：
                        ////////////////////////////////////////////////////////////////////////////////////
                        ////////////////////////////////////////////////////////////////////////////////////
                  218 | ////////////////////////////////////////////////////////////////////////////////////
MX:0xFFFFFF8008701DE0 | 34000080       cbz    w0,0xFFFFFF8008701DF0

View Info:
\home\gonghj\project\mtk8195\kernel-4.19\drivers\base\driver.c

		213  struct device_driver *driver_find(const char *name, struct bus_type *bus)
		214  {
		215  	struct kobject *k = kset_find_obj(bus->p->drivers_kset, name);
		216  	struct driver_private *priv;
		217  
		218  	if (k) {
		219  		/* Drop reference added by kset_find_obj() */
		220  		kobject_put(k);
		221  		priv = to_driver(k);
		222  		return priv->driver;
		223  	}
		224  	return NULL;
		225  }

显然在driver_find函数或其子函数中调用了strcmp

打开kset_find_obj函数定义的地方kernel-4.19/lib/kobject.c
			863  struct kobject *kset_find_obj(struct kset *kset, const char *name)
			864  {
			865  	struct kobject *k;
			866  	struct kobject *ret = NULL;
			867  
			868  	spin_lock(&kset->list_lock);
			869  
			870  	list_for_each_entry(k, &kset->list, entry) {
			871  		if (kobject_name(k) && !strcmp(kobject_name(k), name)) {
			872  			ret = kobject_get_unless_zero(k);
			873  			break;
			874  		}
			875  	}
			876  
			877  	spin_unlock(&kset->list_lock);
			878  	return ret;
			879  }
其中果然调用了strcmp,看来传的参数有问题，结合src2,可知第二个参数name为空，name来自driver_find函数的第一个
参数，现在找谁调用了driver_find函数并传了个空的name指针，显然要从如下信息下手：
<4>[  976.946875]-(5)[7247:insmod] __platform_driver_register+0x40/0x48


l __platform_driver_register+0x40


同样地，定位到\home\gonghj\project\mtk8195\kernel-4.19\drivers\base\platform.c中，有：

	657  int __platform_driver_register(struct platform_driver *drv,
	658  				struct module *owner)
	659  {
	660  	drv->driver.owner = owner;
	661  	drv->driver.bus = &platform_bus_type;
	662  	drv->driver.probe = platform_drv_probe;
	663  	drv->driver.remove = platform_drv_remove;
	664  	drv->driver.shutdown = platform_drv_shutdown;
	665  
	666  	return driver_register(&drv->driver);
	667  }
跟进找到
	146  int driver_register(struct device_driver *drv)
	147  {
	148  	int ret;
	149  	struct device_driver *other;
	150  
	151  	if (!drv->bus->p) {
	152  		pr_err("Driver '%s' was unable to register with bus_type '%s' because the bus was not initialized.\n",
	153  			   drv->name, drv->bus->name);
	154  		return -EINVAL;
	155  	}
	156  
	157  	if ((drv->bus->probe && drv->probe) ||
	158  	    (drv->bus->remove && drv->remove) ||
	159  	    (drv->bus->shutdown && drv->shutdown))
	160  		printk(KERN_WARNING "Driver '%s' needs updating - please use "
	161  			"bus_type methods\n", drv->name);
	162  
	163  	other = driver_find(drv->name, drv->bus);
	164  	if (other) {
	165  		printk(KERN_ERR "Error: Driver '%s' is already registered, "
	166  			"aborting...\n", drv->name);
	167  		return -EBUSY;
	168  	}
	169  
	170  	ret = bus_add_driver(drv);
	171  	if (ret)
	172  		return ret;
	173  	ret = driver_add_groups(drv, drv->groups);
	174  	if (ret) {
	175  		bus_remove_driver(drv);
	176  		return ret;
	177  	}
	178  	kobject_uevent(&drv->p->kobj, KOBJ_ADD);
	179  
	180  	return ret;
	181  }
	显然在调用other = driver_find(drv->name, drv->bus);时drv->name为空，而drv指针由
	__platform_driver_register调用driver_register时传入。
	
	
  #define platform_driver_register(drv) \
  	__platform_driver_register(drv, THIS_MODULE)
	
	
	<4>[  976.946881]-(5)[7247:insmod] init_module+0x1c/0xfe0 [leddrv]


​	
​	platform_driver_register为module_init中注册platform_drive结构体使用的函数，故最终定位故障为
​	驱动的platform_drive结构体的某个name字段为空导致。


​	
 #define platform_driver_register(drv) \
  	__platform_driver_register(drv, THIS_MODULE)
​	
​	
  struct platform_driver {
   int (*probe)(struct platform_device *);
   int (*remove)(struct platform_device *);
   void (*shutdown)(struct platform_device *);
   int (*suspend)(struct platform_device *, pm_message_t state);
   int (*resume)(struct platform_device *);
   struct device_driver driver;
  };
​	
​	
 struct device_driver {
   const char * name;
   struct bus_type * bus;

   struct completion unloaded;
   struct kobject kobj;
   struct klist klist_devices;
   struct klist_node knode_bus;

   struct module * owner;

   int (*probe) (struct device * dev);
   int (*remove) (struct device * dev);
   void (*shutdown) (struct device * dev);
   int (*suspend) (struct device * dev, pm_message_t state);
   int (*resume) (struct device * dev);
 };



