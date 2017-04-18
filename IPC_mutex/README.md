# 使用方法
```
cd mutex/
make
./_main
make good
./_main
```

对比可以发现，不使用进程间mutex同步的话，两个进程的打印会交叉

# 描述
通过将两个进程间的mutex变量映射到同一片内存，可以保证获取和释放的同一个mutex，同时还需要设置mutex变量的pshared属性为PTHREAD_PROCESS_SHARED。

纯属娱乐，也算是一种奇技淫巧，不推荐在实际项目中使用。
