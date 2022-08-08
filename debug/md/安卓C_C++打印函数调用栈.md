```
LOCAL_SHARED_LIBRARIES += libutilscallstack





#include <utils/CallStack.h>
extern "C" void dumping_callstack(void)
{
	   ::android::CallStack cs("tag");//使用DEBUG级别打印,tag换成进程名即可
}
```

