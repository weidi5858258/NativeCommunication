#ifndef __IDAEMON__
#define __IDAEMON__

#include <stdint.h>

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

#include "ICallback.h"

using namespace android;

/****************************** IDaemon ******************************/

/***
 IDaemon是作为Bn端和Bp端共有的接口(只定义接口,不用实现,不在这里实现),不能少.
 而且这个接口中定义的方法在Bn端和Bp端都要有相应的对应.
 因为执行的流程是这样的:
 1.作为服务端的Bn要先启动起来.
 2.当需要连接到Bn时,必定要先得到相应的服务.
   相应代码在MyJNI.cpp文件中的getDaemon()方法.
   其中interface_cast<IDaemon>(binder);
   这句代码会触发Bp的构造方法的执行.
 3.这样得到的sp<IDaemon>指针调用IDaemon中定义的方法(如open()),
   实际会执行Bp端对应的方法(如open()),
   然后调用到Bn端的onTransact()方法,
   最后调用到Bn端相应的方法(如open()).
 */
class IDaemon : public IInterface {
public:
    /***
     跟BpDaemon.cpp中的
     IMPLEMENT_META_INTERFACE(Daemon, DAEMON);
     对应,能够把两者关联起来的元素: Daemon
     */
    DECLARE_META_INTERFACE(Daemon);

public:
    enum {
        OPEN = IBinder::FIRST_CALL_TRANSACTION,
        REGISTER_CALLBACK,
    };

    virtual int open(bool enableCapture) = 0;

    virtual int registerCallback(const sp<ICallback> &callback) = 0;

    // 不能定义
    // virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags = 0) = 0;

};

#endif // __IDAEMON__
