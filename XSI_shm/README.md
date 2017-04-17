# 使用方法
```
cd XSI_shm/
make
./_main

```

# 描述
shared memory是三种XSI IPC方式之一，另外两种分别是message queue，semaphore。我这里实现的是一个toy版的，如果需要在实际项目中使用，需要考虑同步，在写进程在写共享内存时，读进程不可以进行读操作。

