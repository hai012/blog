## linux中断处理

### 1.源码跟踪

![image-20220523205206134](linux中断处理.assets/image-20220523205206134.png)



#### 1.1中断上半部

ARM64 SMP系统的CPU进入中断后硬件会自动关闭当前CPU的所有中断，之后再跳转到异常向量表

![image-20220525173917989](linux中断处理.assets/image-20220525173917989.png)





如果CPU强制跳转到vectors向量表之前是在用户态则是el0,内核态未el1。

中断lable是irq，硬件故障异常或系统调用自陷的lable是sync



kernel_ventry宏最终跳转到相关elx_lable去执行。

![image-20220525173613714](linux中断处理.assets/image-20220525173613714.png)



el0_irq保存线程后执行irq_handler这个宏里面定义的代码块，执行完毕后进入ret_to_user后续进行线程调度

![image-20220525174100188](linux中断处理.assets/image-20220525174100188.png)

el1_irq保存线程后也执行irq_handler这个宏里面定义的代码块，执行完毕后进入el1_preempt后续进行线程调度

![image-20220525174210982](linux中断处理.assets/image-20220525174210982.png)

任务调度参考https://zhuanlan.zhihu.com/p/483899721

再来看irq_hander如何处理，irq_hander中最终调用handle_arch_irq函数指针。

![image-20220525174258873](linux中断处理.assets/image-20220525174258873.png)





如果使用gic,内核启动时在gic驱动程序的probe函数中通过调用set_handle_irq(gic_handle_irq)来将handle_arch_irq函数指针指向gic_handle_irq函数

​                                                      rk3569_r/kernel/kernel/irq/handle.c

![image-20220225141235222](linux中断处理.assets/image-20220225141235222.png)



gic驱动程序还定义了一系列gic处理函数并将其放到gic中断控制器的irq_domain结构体中。



gic_handle_irq函数读取gic寄存器得到gic的硬件中断号hwirq(也就是irqnr)，对于每个hwirq都调用一次handle_domain_irq函数。



![image-20220224213212122](linux中断处理.assets/image-20220224213212122.png)



gic寄存器相关操作详细参考https://blog.csdn.net/yhb1047818384/article/details/99706106

https://blog.csdn.net/yhb1047818384/article/details/87561438



![image-20220224213517939](linux中断处理.assets/image-20220224213517939.png)



handle_domain_irq函数调用到了\_\_handle_domain_irq，在\_\_handle_domain_irq中首先调用irq_enter()，再调用irq_find_mapping和generic_handle_irq函数处理中断上半部硬件中断，最后调用irq_exit()处理中断下半部的软件中断。

![image-20220224213606681](linux中断处理.assets/image-20220224213606681.png)





**irq_enter可以禁止当前被中断的线程被其他线程抢占(该线程被中断时可能处于用户态也可能处于内核态)。**

https://blog.csdn.net/yiyeguzhou100/article/details/49887975

preempt_count是进程调度时用到的.也就是系统会根据preempt_count的值来判断是否可以调度以及抢占。只有当preempt_count为0时才可以调度或抢占。 这样的话在do_IRQ执行期间（并且在irq_exit执行preempt_count-1之前），是不允许内核抢占以及调度到其他进程的，这样不论是在irq_enter或者irq_exit函数执行过程中，current宏得到的都是同一个进程，获取的都是同一个进程的preempt_count，即中断前运行的进程。当调用preempt_disable或add_preempt_count函数时都不可以进行调度，因为都会改变preempt_count的值为非0。所以irq_enter就是告诉系统，现在正在处理中断的上半部分工作,不可以进行调度。你可能会奇怪，既然此时的irq中断都是都是被禁止的，为何还要禁止抢占？这是因为要考虑中断嵌套的问题，一旦驱动程序主动通过local_irq_enable打开了IRQ（其实在handle_IRQ_event中就打开了IRQ），而此时该中断还没处理完成，新的irq请求到达，这时代码会再次进入irq_enter，在本次嵌套中断返回时，内核不希望进行抢占调度，而是要等到最外层的中断处理完成后才做出调度动作，所以才有了禁止抢占这一处理。

![image-20220225102049009](linux中断处理.assets/image-20220225102049009.png)

irq_enter调用到\_\_irq_enter(),后者又调用到了preemt_count_add(HARDIRQ_OFFSET)将preemt_count++。



![image-20220225102616769](linux中断处理.assets/image-20220225102616769.png)





调完irq_enter()返回到\_\_handle_domain_irq函数后继续调用irq_find_mapping函数从gic中断控制器的irq_domain中找到hwirq对应的虚拟中断号irq，在generic_handle_irq函数中根据irq在irq_desc链表找到对应的中irq_desc结构体节点，再调用generic_handle_irq_desc函数。



![image-20220224214012646](linux中断处理.assets/image-20220224214012646.png)



generic_handle_irq_desc函数直接调用irq_desc结构体节点中的handle_irq函数指针。

![image-20220224214152981](linux中断处理.assets/image-20220224214152981.png)





handle_irq函数指针分为两种情况：



**情况1：指向gic子中断控制器的分发函数，例如GPIO控制器的分发函数**

GPIO控制器的分发函数负责读取GPIO控制器的寄存器确定GPIO中断的hwirq,再调用handle_domain_irq函数。接着开始套娃，handle_domain_irq函数根据gpio的hirq在gpio控制器的irq_domain中找到对应的虚拟irq，再根据虚拟irq在irq_desc链表中找到对应的irq_desc结构体节点，然后再调用irq_desc结构体节点中的handle_irq函数指针。这些函数由GPIO控制器驱动提供。



**情况2：指向通用分发函数**

系统内建了一些handle_irq通用分发函数，分为：

- handle_simple_irq()　　简单处理情况处理函数
- handle_level_irq()  　 电平触发方式情况处理函数
- handle_edge_irq()     边沿触发方式情况处理函数
- handle_fasteoi_irq()   用于需要EOI回应的中断控制器
- handle_percpu_irq()   此中断只需要单一CPU响应的处理函数
- handle_nested_irq()   用于处理使用线程的嵌套中断

在初始化阶段，中断描述符的handle_irq指针大多指向了handle_edge_irq函数或handle_level_irq函数。二者大致相似，都会调用irq_may_run判断一下别的CPU是不是正在处理当前的虚拟中断，如果是则设置irq_desc结构体节点中的中断标志为IRQS_PENDNG挂起并直接返回。最后真正处理时调用handle_irq_event函数。



![image-20220224220051996](linux中断处理.assets/image-20220224220051996.png)





![image-20220224220242774](linux中断处理.assets/image-20220224220242774.png)



handle_edge_irq函数和handle_level_irq函数**不同点：**

**二者最大的不同在于对中断线屏蔽的处理以及对handle_irq_event函数的调用次数。handle_edge_irq调用handle_irq_event函数时irq_desc结构体节点对应的该中断线处于未屏蔽状态，在一个do while循环中不断调handle_irq_event函数，只要中断线被屏蔽未屏蔽且irq_desc结构体节点处于IRQS_PENDING状态(处于该状态意味着在处理中断时该中断又发生了一次)就一直调。而handle_level_irq在调用handle_irq_event之前就已经调用了mask_ack_irq()函数屏蔽了中断线，只调一次handle_irq_event，且调完了再调cond_unmask_irq来开启中断线。**



irq_may_run根据当前irq_desc结构体节点中的istate是否有IRQD_IRQ_INPROGRESS标志来判断是否有其他CPU正在响应当前irq_desc结构体节点对应的虚拟中断号。

![image-20220224222711505](linux中断处理.assets/image-20220224222711505.png)













**handle_irq_event**函数设置中断描述符的状态为IRQD_IRQ_INPROGRESS，在SMP系统中，同一个中断信号有可能发往多个CPU，但是中断处理只应该处理一次，所以设置状态为IRQD_IRQ_INPROGRESS，其他CPU执行此中断时都会先检查此状态

![image-20220224222230641](linux中断处理.assets/image-20220224222230641.png)



接着执行handle_irq_event_percpu函数，handle_irq_event_percpu函数调用\_\_handle_irq_event_percpu函数，\_\_handle_irq_event_percpu函数找到当前irq_desc结构体节点中的action链表，并对action链表中的action链表中的每个结构体节点调用action结构体节点中的handler函数指针。如果handler函数指针返回IRQ_WAKE_THREAD，则启动中断线程，停止继续调用下一个action结构体节点的handler函数指针并返回，如果handler函数指针返回IRQ_HANDLED则直接停止继续调用下一个action结构体节点的handler函数指针并返回。其他情况比如IRQ_NONE则继续调用下一个action结构体的handler函数指针，直到对所有的action结构体都调用了handler函数指针才返回。



![image-20220224222317096](linux中断处理.assets/image-20220224222317096.png)



#### 1.2中断下半部



中断上半部处理完了最终返回到\_\_handle_domain_irq函数中继续调用irq_exit函数来处理中断下半部。



![image-20220225100239840](linux中断处理.assets/image-20220225100239840.png)





\__ARCH_IRQ_EXIT_IRQS_DISABLED宏在arm和arm64中都被定义成了1，不需要再次关当前CPU中断。接着调用preempt_count_sub(HARDIRQ_OFFSET)让preemt_count--，然后调用in_interrupt根据preemt_count判断一下是否有软件中断被硬件中断打断，以及是否有软件中断要处理。如果没有软件中断被打断且有软件中断要处理，则调用invoke_softirq来处理软件中断。



invoke_softirq参考https://zhuanlan.zhihu.com/p/145908094

MTK、rk、amlogic、qcom平台的内核都定义了CONFIG_IRQ_FORCED_THREADING=y，导致force_irqthreads==true，实际上invoke_softirq里面主要是判断ksoftirqd内核线程是否已经在运行，如果已经运行则直接返回，否则wakeup ksoftirqd内核线程再返回。这种情况下软件中断在线程栈中执行。

![image-20220525170608919](linux中断处理.assets/image-20220525170608919.png)



如果未定义CONFIG_IRQ_FORCED_THREADING，例如早期版本的内核则在invoke_softirq中调用\_\_do_softirq，这种情况下软件中断在中断栈里执行。\_\_do_softirq调用\_\_local_bh_disable_ip(\_RET_IP_, SOFTIRQ_OFFSET)让preempt_count++，然后调用local_irq_enable();开启当前CPU的中断，接着去while循环处理软件中断，处理完了再调用local_irq_disable();关当前CPU的中断，接着再调用\_\_local_bh_enable(SOFTIRQ_OFFSET);让preempt_count--。

![image-20220225101215451](linux中断处理.assets/image-20220225101215451.png)









### 2.中断处理模型



**如果softirq被线程化，则并未执行下图的第5~9步，到第四步后如果是N则只是唤醒ksoftirqd内核线程去处理softirq，处理中断的当前CPU全程处于关所有中断状态。**softirq未被线程化的中断处理模型如下：

![image-20220225103409770](linux中断处理.assets/image-20220225103409770.png)



a. 硬件中断A处理过程中，没有其他中断发生：

一开始，preempt_count = 0；

上述流程图①～⑨依次执行，上半部、下半部的代码各执行一次。

 

b. 硬件中断A处理过程中，又再次发生了中断A：

一开始，preempt_count = 0；

**执行到第⑥时，一开中断后，中断A又再次使得CPU跳到中断向量表。**

***\*注意\****：这时preempt_count等于1，并且中断下半部的代码并未执行。

CPU又从①开始再次执行中断A的上半部代码：

在第①步preempt_count等于2；

在第③步preempt_count等于1；

在第④步发现preempt_count等于1，所以直接结束当前第2次中断的处理；

***\*注意\****：重点来了，第2次中断发生后，打断了第一次中断的第⑦步处理。当第2次中断处理完毕，CPU会继续去执行第⑦步。

可以看到，发生2次硬件中断A时，它的上半部代码执行了2次，但是下半部代码只执行了一次。

所以，同一个中断的上半部、下半部，在执行时是多对一的关系。

 

c. 硬件中断A处理过程中，又再次发生了中断B：

一开始，preempt_count = 0；

执行到第⑥时，一开中断后，中断B又再次使得CPU跳到中断向量表。

***\*注意\****：这时preempt_count等于1，并且中断A下半部的代码并未执行。

CPU又从①开始再次执行中断B的上半部代码：

在第①步preempt_count等于2；

在第③步preempt_count等于1；

在第④步发现preempt_count等于1，所以直接结束当前第2次中断的处理；

***\*注意\****：重点来了，第2次中断发生后，打断了第一次中断A的第⑦步处理。当第2次中断B处理完毕，CPU会继续去执行第⑦步。

在第⑦步里，它会去执行中断A的下半部，也会去执行中断B的下半部。

所以，多个中断的下半部，是汇集在一起处理的。



### 3. 重要数据结构




doc_and_source_for_drivers\IMX6ULL\doc_pic\08_Interrupt\04_Linux中断系统中的重要数据结构.tif





### 4.注册中断上半部中断处理函数



irq_desc结构体中的name是虚拟中断名称。

当调用request_irq、request_threaded_irq注册中断处理函数时，内核就会构造一个irqaction结构体用来描述挂载在某一虚拟中断下的具体设备。在里面保存name(为挂在某个虚拟中断上的设备的名字)、dev_id(属于私有设备，即使同一个irq_desc下不同irqaction中的dev_id需要指向不同的地址)等，最重要的是handler、thread_fn、thread。

handler是中断处理的上半部函数，用来处理紧急的事情。

thread_fn对应一个内核线程thread，当handler执行完毕，Linux内核会唤醒对应的内核线程。在内核线程里，会调用thread_fn函数。

可以提供handler而不提供thread_fn，就退化为一般的request_irq函数。

可以不提供handler只提供thread_fn，完全由内核线程来处理中断。

也可以既提供handler也提供thread_fn，这就是中断上半部、下半部。

 

**在reqeust_irq时可以传入dev_id，为何需要dev_id？作用有2：**

**① 中断处理函数执行时，可以使用dev_id**

**② 卸载中断时要传入dev_id，这样才能在action链表中根据dev_id找到对应项**

**所以在共享中断中必须提供dev_id，非共享中断可以不提供。**

request_irq

request_threaded_irq

具体用法参考《linux设备驱动开发  马厄迪 pdf》148页。

### 4.软件中断之softirq与tasklet

https://zhuanlan.zhihu.com/p/145908094





https://www.ktanx.com/blog/p/2456

安卓平台定义了CONFIG_IRQ_FORCED_THREADING 宏，如今所有的softirq都在ksoftirqd内核线程的栈中运行。





**softirq**

softirq在2.3版本内核被引入，相关代码在中定义，内核中直接使用softirq的场景较少。

可用的softirq有以下几种：

```text
enum
{
HI_SOFTIRQ=0,
TIMER_SOFTIRQ,
NET_TX_SOFTIRQ,
NET_RX_SOFTIRQ,
BLOCK_SOFTIRQ,
BLOCK_IOPOLL_SOFTIRQ,
TASKLET_SOFTIRQ,
SCHED_SOFTIRQ,
HRTIMER_SOFTIRQ,
RCU_SOFTIRQ, /* Preferable RCU should always be the last softirq */
NR_SOFTIRQS
};
```

提交一个softirq需要调用raise_softirq函数，raise_softirq调用raise_softirq_irqoff，该函数将相应软中断标识为pending、完成软中断提交。

**标识为pending的软中断可在以下时机得到处理**：

1. 对于非线程化的softirq, 硬中断处理完成、在irq_exit中调用到invoke_softirq函数调用\_\_do_softirq。在__do_softirq函数中调用softirq数组里面已经pending项的处理函数。

2. ksoftirqd内核线程被唤醒后处理

3. 显式地在线程上下文调用do_softirq进行软中断处理，例如在内核网络代码中，netif_rx_ni函数会主动调用do_softirq进行软中断理





**tasklet**

tasklet基于softirq实现，其本质也是softirq，对应softirq枚举类型中的HI_SOFTIRQ和TASKLET_SOFTIRQ，HI_SOFTIRQ优先级较高。

相比softirq，即使相同类型的softirq也可同时在不同的cpu上处理，而相同类型的tasklet不可同时在不同cpu上处理，不同类型的tasklet可以。

可通过tasklet_schedule或tasklet_hi_schedule提交tasklet，这两个函数最终调用raise_softirq_irqoff提交软中断。

因tasklet提交的同样是软中断，所以还是由do_softirq函数完成tasklet的处理。

tasklet_init

tasklet_schedule

具体用法参考《linux设备驱动开发  马厄迪 pdf》156页。









### 5.工作队列及kworker内核线程

创建结构体，填充数据，并将该结构体交给kworker内核线程来处理。

DECLARE_WORK

INIT_WORK

schedule_work

具体用法参考《linux设备驱动开发  马厄迪 pdf》157页。





内核几个缺省的kworker工作队列：${KERNEL_ROOT_DIR}/include/linux/workqueue.h

```
391  extern struct workqueue_struct *system_wq;
392  extern struct workqueue_struct *system_highpri_wq;
393  extern struct workqueue_struct *system_long_wq;
394  extern struct workqueue_struct *system_unbound_wq;
395  extern struct workqueue_struct *system_freezable_wq;
396  extern struct workqueue_struct *system_power_efficient_wq;
397  extern struct workqueue_struct *system_freezable_power_efficient_wq;
```

这些工作队列不同的是背后对应的kworker处理线程属性不同，freezable队列中的每一项任务可以阻塞，阻塞后处理该任务的kwoker线程并没有阻塞而是去处理同队列的下一项任务，不影响同队列的下一项任务执行。不带freezable队列中的前一项任务阻塞后kwoker线程就阻塞了，下一项任务在前一项任务完成之前得不到执行。



#### 5.1一般工作队列

处理函数原型：

void handler_func(struct work_struct *work) {

//do something

}

#### 5.1.1 定义与初始化

//定义与初始化分开
struct work_struct  work;
INIT_WORK(&work, handler_func);

//定义与初始化合二为一
//DECLARE_WORK(work, handler_func);

#### 5.1.2 加入内核线程进行处理(调用handler_func)

//调用schedule_work把work加入内核kwoker线程工作队列,直接使用内核中每个CPU对应的一个 缺省工作者线程envents/n（n代表CPU的序号，从0开始）

schedule_work(work);



此外还可以自己创建一个专用的kwoker线程,只需要调用：
struct workqueue_struct *create_workqueue(const char *name);

调度时使用如下函数将work加入专用kworker线程的工作队列：
int queue_work(struct workqueue_struct *wq, struct work_struct *work);



schedule_work其实就是queue_work函数的一个封装：${KERNEL_ROOT_DIR}/include/linux/workqueue.h

```
574  static inline bool schedule_work(struct work_struct *work)
575  {
576  	return queue_work(system_wq, work);
577  }
```



#### 5.2 延迟工作队列

处理函数原型：

void handler_func(struct delayed_work *work) {

//do something

}

##### 5.2.1 定义与初始化

定义与初始化分开
struct delayed_work work;//声明一个队列,struct delayed_work中包含有一个一般工作队列使用的struct work_struct
INIT_DELAYED_WORK(&work, handler_func);//初始化这个队列

//定义与初始化合二为一
DECLARE_DELAYED_WORK(work, handler_func);

##### 5.2.2 加入内核线程进行处理

//调用schedule_work把work加入内核kwoker线程工作队列，直接使用内核中每个CPU对应的一个 缺省工作者线程
schedule_delayed_work(work,  delay);



此外还可以自己创建一个专用的kwoker线程,只需要调用：
struct workqueue_struct *create_workqueue(const char *name);

调度时使用如下函数将work加入专用kworker线程的工作队列：
int queue_delayed_work(struct workqueue_struct *wq, struct work_struct *work, unsigned long delay);

delay是unsigned long 类型，代表着内核时钟节拍数，一般是微秒。

schedule_delayed_work其实就是queue_delayed_work函数的一个封装:  ${KERNEL_ROOT_DIR}/include/linux/workqueue.h

```
631  static inline bool schedule_delayed_work(struct delayed_work *dwork,
632  					 unsigned long delay)
633  {
634  	return queue_delayed_work(system_wq, dwork, delay);
635  }
```



### 6.使用独立的线程化中断

以前用work来线程化地处理中断，一个worker线程只能由一个CPU执行，多个中断的work都由同一个worker线程来处理，在单CPU系统中也只能忍着了。但是在SMP系统中，明明有那么多CPU空着，你偏偏让多个中断挤在这个CPU上？新技术threaded irq，为每一个中断都创建一个内核线程；多个中断的内核线程可以分配到多个CPU上执行，这提高了效率。

request_threaded_irq









### 7.中断相关函数



desc->irq_data.chip->irq_enable/irq_disable              是否使能中断线

disable时中断控制器根本就不响应该irq



desc->irq_data.chip->irq_mask/irq_unmask               是否屏蔽某个中断线

mask时，子中断控制器或gic接收该irq，只是不往上级传递，这时，该irq处于pending状态



desc->irq_data.chip->irq_ack                                         清除某个中断线的中断标志位。gic向CPU发出中断后，只有在某个CPU调用irq_ack清除gic寄存器相关bit位后，如果该中断线再次到达，gic才会继续发出中断请求。如果是子中断控制器例如GPIO控制器就清除GPIO控制器中的一些标志位，如果是gic中断控制器清除gic中断控制器中的一些标志位



desc->irq_data.chip->irq_mask_ack                              irq_mask+irq_ack 





local_irq_disable/local_irq_enable                                是否屏蔽当前CPU的所有IRQ中断，通过设置CPU CPSR的第I位来实现





enable_irq    最终调用desc->irq_data.chip->irq_enable或desc->irq_data.chip->irq_unmask来实现

disable_irq   最终调用desc->irq_data.chip->irq_disable或desc->irq_data.chip->irq_mask来实现

enable_irq和disable_irq调用次数是相关的，调了多少次enable_irq就要调用相应次数的disable_irq才调到其最终实现，反之亦然，调用次数使用desc->depth来记录。enable_irq/disable_irq可以让某个中断线对所有CPU有效/失效。







CPU被中断打断跳转到中断向量时自动CPU CPSR的第I位置为1，表明屏蔽当前CPU的所有IRQ中断





CPU    CPSR[I] ==1

preempt_count++                   irq_enter

​                                                   desc->handle_irq指向 handle_edge_irq 这个通用分发函数







对于所有中断线与单个CPU，当前处理中断的CPU缺省CPSR的第I位置为1处于屏蔽当前CPU的所有IRQ中断状态，在softirq未被线程化时会在处理softirq之前调用local_irq_enable清除这个屏蔽位，softirq执行完毕又会调用local_irq_disable置位该屏蔽位。



对于单个中断线与所有CPU，如果该中断是水平触发，处理该中断线的CPUx在handle_level_irq中调用mask_ack_irq，屏蔽该中断线后清除该中断线的标志位，其他所有CPU无法再收到该中断，CPUx执行中断下半部前判断如果没有IRQF_ONESHOT标志则调用unmask_irq，否则不调用unmask_irq待后续中断线程完成后再调用，在mask_ack_irq与unmask_irq之间该中断线被屏蔽，即使再次发生也不会有任何CPU去处理该中断。如果该中断是边沿触发，不同的CPU在执行handle_edge_irq期间具有不同的操作：

* 对于处理该中断中断的CPUx:    CPUx执行handle_edge_irq时调用irq_ack清除中断标志位(此时未屏蔽中断线，清除ack后即使处理该中断线的CPUx未中断返回，如果再次发生该中断，gic中断控制器可以将后续的中断请求发送给其他CPUy去处理)。然后判断该IRQ是否有 IRQS_PENDING 标志 如果有则对该中断线unmask_irq。接着调用 handle_irq_event 去处理中断，在 handle_irq_event 中清除IRQS_PENDING标志后再调用handle_irq_event_percpu。handle_irq_event调完后do-while判断是否还有IRQS_PENDING标志，如果有则继续循环调用handle_irq_event。

* 如果CPUx还在处理，此时又有该中断发生，中断请求被发送到其他CPUy上，CPUy上执行handle_edge_irq时设置IRQS_PENDING标志，对该中断线mask_ack_irq(CPUy在这里mask后直到CPUx执行到unmask_irq期间该中断后续如果再发生gic也不会把请求传递到任何CPU了)，然后之间中断返回。

简单来说对于边缘触发，当处理上一个事件的CPU未处理完成退出中断而此时又发生中断在另一个CPU上同时处理时，如果是第一次发生则在循环中不断调用 handle_irq_event 去处理中断，循环次数小于边沿触发事件发生的次数(因为如果第二次发生后会mask该中断线，如果未及时处理会丢失事件)。第二次发生后则并未真正去处理而是mask该中断设置相关标志让正在处理第一次中断的CPU多循环一次。





对于线程化的softirq，CPU中断里面没有中断下半部。



边沿/水平触发决定中断什么时候发生以及发生时是否





使用独立中断线程的中断，request_threaded_irq注册的中断上半部具体处理函数fn()永远某一时刻只有一个CPU去执行，此时该CPU处于关中断状态(对其他类型的irq也是如此)。而对于该中断线，如果是水平触发则在执行fn()之前被屏蔽，执行完fn()之后可能会umask也可能会等到中断返回在独立线程中处理完thread_fn后再umask，





有 IRQD_IRQ_INPROGRESS，已经在处理，irq_may_run=true

无 IRQD_IRQ_INPROGRESS，未处理，irq_may_run=false





request_threaded_irq时如果上半部处理函数为空，thread_fn不为空则需要带有IRQF_ONESHOT标志。

IRQF_ONESHOT仅在水平触发方式里面用于执行完中断上半部后不umask该中断源，以免中断退出后一直触发中断。

内核在中断上下文执行中断上半部后唤醒独立内核线程去处理thread_fn，然后退出中断处理转去进行线程调度，并非在中断上下文唤醒一次独立内核线程就执行一次，如此一一对应交错运行，内核可以在中断上下文多次唤醒独立内核线程，独立线程对于每次的唤醒请求都执行一次thread_fn，并没有强制要求中断上下文唤醒与独立内核线程调用thread_fn交错运行。当然指定了IRQF_ONESHOT的水平触发方式确实一定交错运行。而边沿触发是否交错运行完全是一个随机事件。

thread_fn在独立内核线程中执行天生就某一时刻只能有一个CPU去运行。

独立内核线程执行完thread_fn后会判断当前irq是否处于mask状态，如果是则将其umask。(IRQF_ONESHOT+水平触发方式时会执行到umask，而边沿触发不会执行到umask)。

IRQF_ONESHOT可以参考

https://blog.csdn.net/melo_fang/article/details/78224326

