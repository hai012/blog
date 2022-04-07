vendor/mediatek/proprietary/bootable/bootloader/lk/Android.mk中包含了vendor/mediatek/proprietary/bootable/bootloader/lk/build_lk.mk

build_lk.mk中定义了lk.img这个目标以及生成该目标的命令

目标:

 out/target/product/xt501v2_64_bsp/obj/LK_FES_OBJ/build-xt501v2_64_bsp/lk.img

生成该目标的命令:

```
 ./prebuilts/build-tools/linux-x86/bin/make -j24  -C vendor/mediatek/proprietary/bootable/bootloader/lk TOOLCHAIN_PREFIX=/home/gonghj/project/mtk8195/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.9.1/bin/arm-linux-androideabi-BOOTLOADER_OUT=/home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/LK_OBJ ROOTDIR=/home/gonghj/project/mtk8195 LCM_WIDTH=1200 LCM_HEIGHT=1920 COMPILER=/home/gonghj/project/mtk8195/prebuilts/clang/host/linux-x86/clang-r383902b/bin/clang:PJT xt501v2_64_bsp
```

vendor/mediatek/proprietary/bootable/bootloader/lk/makefile中包含了vendor/mediatek/proprietary/bootable/bootloader/lk/make/build.mk

build.mk中定义了lk的连接命令：

```
/home/gonghj/project/mtk8195/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.9.1/bin/arm-linux-androideabi-ld.bfd -gc-sections -T /home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/LK_OBJ/build-xt501v2_64_bsp/system-onesegment.ld
/home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/LK_OBJ/build-xt501v2_64_bsp/arch/arm/crt0.o /home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/LK_OBJ/build-xt501v2_64_bsp/target/xt501v2_64_bsp/init.o
......
 /home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/LK_OBJ/build-xt501v2_64_bsp/platform/common/storage/mmc/msdc_irq.o 
 /home/gonghj/project/mtk8195/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.9.1/bin/../lib/gcc/arm-linux-androideabi/4.9.x/thumb/libgcc.a
 -Lplatform/mt8195/lib --start-group -lsec -lcrypto -lsecplat -ldevinfo --end-group
 -o /home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/LK_OBJ/build-xt501v2_64_bsp/lk
```

连接脚本/home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/LK_OBJ/build-xt501v2_64_bsp/system-onesegment.ld：

    
    1 OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
    2 OUTPUT_ARCH(arm)
    3
    4 ENTRY(_start)
    5 SECTIONS
    6 {
    7         . = 0x56000000 ;
    8
    9         /* text/read-only data */
    10         .text.boot : { *(.text.boot) }
    11         .text : { *(.text .text.* .glue_7* .gnu.linkonce.t.*) } =0x9090
    12
    13         .interp : { *(.interp) }
    14         .hash : { *(.hash) }
    15         .dynsym : { *(.dynsym) }
    16         .dynstr : { *(.dynstr) }
    17         .rel.text : { *(.rel.text) *(.rel.gnu.linkonce.t*) }
    18         .rela.text : { *(.rela.text) *(.rela.gnu.linkonce.t*) }
    19         .rel.data : { *(.rel.data) *(.rel.gnu.linkonce.d*) }
    20         .rela.data : { *(.rela.data) *(.rela.gnu.linkonce.d*) }
    21         .rel.rodata : { *(.rel.rodata) *(.rel.gnu.linkonce.r*) }
    22         .rela.rodata : { *(.rela.rodata) *(.rela.gnu.linkonce.r*) }
    23         .rel.got : { *(.rel.got) }
    24         .rela.got : { *(.rela.got) }
    25         .rel.ctors : { *(.rel.ctors) }
    26         .rela.ctors : { *(.rela.ctors) }
    27         .rel.dtors : { *(.rel.dtors) }
    28         .rela.dtors : { *(.rela.dtors) }
    29         .rel.init : { *(.rel.init) }
    30         .rela.init : { *(.rela.init) }
    31         .rel.fini : { *(.rel.fini) }
    32         .rela.fini : { *(.rela.fini) }
    33         .rel.bss : { *(.rel.bss) }
    34         .rela.bss : { *(.rela.bss) }
    35         .rel.plt : { *(.rel.plt) }
    36         .rela.plt : { *(.rela.plt) }
    37         .init : { *(.init) } =0x9090
    38         .plt : { *(.plt) }
    39
    40         .rodata : {
    41                 *(.rodata .rodata.* .gnu.linkonce.r.*)
    42                 . = ALIGN(4);
    43                 __commands_start = .;
    44                 KEEP (*(.commands))
    45                 __commands_end = .;
    46                 . = ALIGN(4);
    47                 __apps_start = .;
    48                 KEEP (*(.apps))
    49                 __apps_end = .;
    50                 . = ALIGN(4);
    51                 __rodata_end = . ;
    52         }
    53
    54         /* writable data  */
    55         __data_start_rom = .;   /* in one segment binaries, the rom data address is on top of the ram data address */
    56         __data_start = .;
    57         .data : SUBALIGN(4) { *(.data .data.* .gnu.linkonce.d.*) }
    58
    59         __ctor_list = .;
    60         .ctors : { *(.ctors) }
    61         __ctor_end = .;
    62         __dtor_list = .;
    63         .dtors : { *(.dtors) }
    64         __dtor_end = .;
    65         .got : { *(.got.plt) *(.got) }
    66         .dynamic : { *(.dynamic) }
    67
    68         __data_end = .;
    69
    70         /* unintialized data (in same segment as writable data) */
    71         . = ALIGN(4);
    72         __bss_start = .;
    73         .bss : { *(.bss .bss.*) }
    74
    75         . = ALIGN(4);
    76         _end = .;
    77
    78         ASSERT(. <= 0x56000000  + 0x00400000 , "Runtime image has exceeded its limit.")
    79         . = 0x56000000  + 0x00400000 ;
    80         _end_of_ram = .;
    81
    82         /* Strip unnecessary stuff */
    83         /DISCARD/ : { *(.comment .note .eh_frame) }
    84 }

故lk从vendor/mediatek/proprietary/bootable/bootloader/lk/arch/arm/crt0.o文件的.text.boot段开始启动:

```

27 .section ".text.boot"
28 .globl _start
29 _start:
30 	b	reset
31 	b	arm_undefined
32 	b	arm_syscall
33 	b	arm_prefetch_abort
34 	b	arm_data_abort
35 	b	arm_reserved
36 	b	arm_irq
37 	b	arm_fiq
38 
39 /*pre-loader to uboot argument Location*/
40 .global BOOT_ARGUMENT_LOCATION
41 BOOT_ARGUMENT_LOCATION:
42         .word 0x00000000
43 
44 reset:
45   ldr     r6, =BOOT_ARGUMENT_LOCATION
46   str     r4, [r6]
......
187 	bl		kmain
```

crt0.s中的`_start`函数为入口函数，crt0.s主要初始化CPU，最终长跳转到vendor/mediatek/proprietary/bootable/bootloader/lk/kernel/main.c中kmain函数。kmain函数初始化完成后创建bootstrap2线程并使用thread_resume启动线程：

```
83  void kmain(void)
84  {
85  #if !defined(MACH_FPGA) && !defined(SB_LK_BRINGUP)
86  	boot_time = get_timer(0);
87  #endif
88  
89  	// get us into some sort of thread context
90  	thread_init_early();
91  
92  	// early arch stuff
93  	arch_early_init();
94  
95  	// do any super early platform initialization
96  	platform_early_init();
97  
98  #if defined(MACH_FPGA) || defined(SB_LK_BRINGUP)
99  	boot_time = get_timer(0);
100  #endif
101  
102  	// do any super early target initialization
103  	target_early_init();
104  
105  	dprintf(INFO, "welcome to lk\n\n");
106  	dprintf(INFO, "LK_VER_TAG = %s\n", LK_VER_TAG);
107  
108  	// deal with any static constructors
109  	dprintf(SPEW, "calling constructors\n");
110  	call_constructors();
111  
112  	// bring up the kernel heap
113  	dprintf(SPEW, "initializing heap\n");
114  	heap_init();
115  
116  	// initialize the threading system
117  	dprintf(SPEW, "initializing threads\n");
118  	thread_init();
119  
120  	// initialize the dpc system
121  	dprintf(SPEW, "initializing dpc\n");
122  	dpc_init();
123  
124  	// initialize kernel timers
125  	dprintf(SPEW, "initializing timers\n");
126  	timer_init();
127  
128  #ifdef  MTK_LK_IRRX_SUPPORT
129     mtk_ir_init(0);
130  #endif
131  
132  
133  #if (!ENABLE_NANDWRITE)
134  	// create a thread to complete system initialization
135  	dprintf(SPEW, "creating bootstrap completion thread\n");
136  
137  	thread_t *thread_bs2 = thread_create("bootstrap2", &bootstrap2, NULL,
138  		DEFAULT_PRIORITY, DEFAULT_STACK_SIZE);
139  	if (thread_bs2)
140  		thread_resume(thread_bs2);
141  	else {
142  		dprintf(CRITICAL, "Error: Cannot create bootstrap2 thread!\n");
143  		assert(0);
144  	}
145  
146  	thread_t *thread_io = thread_create("iothread", &iothread, NULL,
147  		IO_THREAD_PRIORITY, DEFAULT_STACK_SIZE);
148  	if (thread_io)
149  		thread_resume(thread_io);
150  	else {
151  		dprintf(CRITICAL, "Error: Cannot create I/O thread!\n");
152  		assert(0);
153  	}
154  
155  	// enable interrupts
156  	exit_critical_section();
157  
158  	// become the idle thread
159  	thread_become_idle();
160  #else
161          bootstrap_nandwrite();
162  #endif
163  }
164  
165 
166  static int bootstrap2(void *arg)
167  {
168  	dprintf(SPEW, "top of bootstrap2()\n");
169  
170  	print_stack_of_current_thread();
171  
172  	arch_init();
173  
174  	// XXX put this somewhere else
175  #if WITH_LIB_BIO
176  	bio_init();
177  #endif
178  #if WITH_LIB_FS
179  	fs_init();
180  #endif
181  	// Allocate LK memory from mb, free before jump to kernel
182  	mboot_allocate_lk_scratch_from_mblock();
183  
184  	// initialize the rest of the platform
185  	dprintf(SPEW, "initializing platform\n");
186  	platform_init();
187  
188  	// initialize the target
189  	dprintf(SPEW, "initializing target\n");
190  	target_init();
191  
192  	dprintf(SPEW, "calling apps_init()\n");
193  	apps_init();
194  
195  	return 0;
196  }
```

bootstrap2初始化完成后调用apps_init();

```
33  void apps_init(void)
34  {
35  	const struct app_descriptor *app;
36  
37  	/* call all the init routines */
38  	for (app = __apps_start; app != __apps_end; app++) {
39  		if (app->init)
40  			app->init(app);
41  	}
42  
43  	/* start any that want to start on boot */
44  	for (app = __apps_start; app != __apps_end; app++) {
45  		if (app->entry && (app->flags & APP_FLAG_DONT_START_ON_BOOT) == 0) {
46  			start_app(app);
47  		}
48  	}
49  }
```

 \_\_apps_start和__apps_end符号在连接脚本中定义，二者直接存放的是所有.o文件中用.apps修饰的段属性对象。

apps_init调用该段中的所有struct app_descriptor结构体中的init函数指针。vendor/mediatek/proprietary/bootable/bootloader/lk/include/app.h中定义了struct app_descriptor结构体、APP_START、APP_END宏。

```
30  struct app_descriptor;
31  typedef void (*app_init)(const struct app_descriptor *);
32  typedef void (*app_entry)(const struct app_descriptor *, void *args);
33  
34  /* app startup flags */
35  #define APP_FLAG_DONT_START_ON_BOOT 0x1
36  
37  /* each app needs to define one of these to define its startup conditions */
38  struct app_descriptor {
39  	const char *name;
40  	app_init  init;
41  	app_entry entry;
42  	unsigned int flags;
43  };
44  
45  #define APP_START(appname) struct app_descriptor _app_##appname __SECTION(".apps") = { .name = #appname,
46  #define APP_END };
```

在vendor/mediatek/proprietary/bootable/bootloader/lk/app/aboot/aboot.c的文件末尾使用APP_START、APP_END宏定义了一个struct app_descriptor结构体该结构体具有.apps的段属性，并将init函数指针指向aboot_init函数，aboot_init函数调用了boot_linux_from_mmc函数，boot_linux_from_mmc函数调用了boot_linux函数，boot_linux函数最终调用entry(0, machtype, tags)启动linux内核：

```
242  void boot_linux(void *kernel, unsigned *tags,
243  		const char *cmdline, unsigned machtype,
244  		void *ramdisk, unsigned ramdisk_size)
245  {
......
421  	enter_critical_section();
422  	/* do any platform specific cleanup before kernel entry */
423  	platform_uninit();
424  	arch_disable_cache(UCACHE);
425  	arch_disable_mmu();
426  	entry(0, machtype, tags);
427  }
......
436  int boot_linux_from_mmc(void)
437  {
......
580  	dprintf(INFO, "\nBooting Linux\n");
581  	boot_linux((void *)hdr->kernel_addr, (unsigned *) hdr->tags_addr,
582  		   (const char *)cmdline, board_machtype(),
583  		   (void *)hdr->ramdisk_addr, hdr->ramdisk_size);
584  
585  	return 0;
586  }

1322  void aboot_init(const struct app_descriptor *app)
1323  {
......
1374  
1375  	if (target_is_emmc_boot())
1376  	{
1377  		if(emmc_recovery_init())
1378  			dprintf(ALWAYS,"error in emmc_recovery_init\n");
1379  		if(target_use_signed_kernel())
1380  		{
1381  			if((device.is_unlocked) || (device.is_tampered))
1382  			{
1383  			#ifdef TZ_TAMPER_FUSE
1384  				set_tamper_fuse_cmd();
1385  			#endif
1386  			#if USE_PCOM_SECBOOT
1387  				set_tamper_flag(device.is_tampered);
1388  			#endif
1389  			}
1390  		}
1391  		boot_linux_from_mmc();
1392  	}
.......
1437  }
1438  
1439  APP_START(aboot)
1440  	.init = aboot_init,
1441  APP_END
```
