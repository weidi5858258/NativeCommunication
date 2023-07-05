底层进程间通信看这个
part_a 甲方
part_b 乙方
part_a: BpDaemon BnCallback 在a进程
part_b: BnDaemon BpCallback 在b进程
整体调用过程:
BpDaemon ---> BnDaemon ---> BpCallback ---> BnCallback
有两种方式可以实现双向通信
一.
由a进程向b进程传递一个Callback过去,然后b进程通过Callback反馈给a进程.
二.
不传递Callback,两个进程都通过android正常通信方式传递消息.

先记录上面两种方式的共同部分.
1.
定义IDaemon.h和ICallback.h
class IDaemon   : public IInterface {...}
class ICallback : public IInterface {...}
//固定写法,去掉前面的"I",不能乱命名
DECLARE_META_INTERFACE(Daemon);
DECLARE_META_INTERFACE(Callback);
需要跟BpDaemon.cpp和BpCallback.cpp中的
IMPLEMENT_META_INTERFACE(Daemon, MYDAEMON);
IMPLEMENT_META_INTERFACE(Callback, CALLBACK);
相对应(写在cpp文件中,不能写在h文件中).
定义好enum和需要用到的各种方法(纯虚函数).
在基类中不能定义
virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags = 0);
或
virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags = 0) = 0;
2.
实现子类




































































