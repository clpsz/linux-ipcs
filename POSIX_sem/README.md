# 使用方法
```
cd POSIX_semaphore/
make
./multi_threads_sem
./multi_processes_sem
```

# 描述
POSIX版的semaphore有两种类型，一种是多进程之间的，需要使用一个名字来初始化一个sem_t数组结构。

还有一个进程之内多个线程之间的版本，直接定义一个sem_t类型的变量即可，使用sem_init初始化，使用结束之后使用sem_destroy来销毁。

