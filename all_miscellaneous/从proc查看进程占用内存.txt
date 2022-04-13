
cat /proc/[pid]/status|grep Vm

VmPeak:	  608060 kB
VmSize:	  608060 kB
VmLck:	       0 kB
VmPin:	       0 kB
VmHWM:	    2588 kB
VmRSS:	    2588 kB
VmData:	   67768 kB
VmStk:	     248 kB
VmExe:	      12 kB
VmLib:	    2388 kB
VmPTE:	     160 kB
VmSwap:	       0 kB


VmHWM：进程物理内存占用峰值
VmRSS: 当前进程物理内存占用


ps -o pmem,s,pid,ppid,sid,tty,cmd -A|grep

pmem:占用内存百分比