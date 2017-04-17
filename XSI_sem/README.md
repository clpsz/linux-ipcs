# 使用方法
```
cd XSI_semaphore/
make
./_main
make good
./_main

```

对比可以发现，不使用信号量同步的话，两个进程的打印会交叉

# 描述
semaphore是三种XSI IPC方式之一，另外两种分别是message queue和shared memory，这里要将XSI的信号量与POSIX的信号量区分开来。

XSI的信号量可以一次初始化和操作多个，但是POSIX的一次只能操作一个。

