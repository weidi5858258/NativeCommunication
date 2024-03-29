#ifndef __MY_DAEMON_H__
#define __MY_DAEMON_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h>

#include <binder/BinderService.h>

#include "../include/MyConst.h"
#include "../include/LogDebug.h"
#include "../Binder/ICallback.h"
#include "../Binder/IDaemon.h"

using namespace android;

/****************************** MyDaemon ******************************/
/****************************** 服务端 ******************************/

/***
一般作为系统服务,开机后就启动该服务.
必须继承下面两个类才能达到"作为服务端和作为系统服务"的目的.
  */

class BnDaemon :
        public BinderService<BnDaemon>,
        // 直接继承BnInterface,不再使用BnDaemon
        public BnInterface<IDaemon> {
    // 固定写法
    friend class BinderService<BnDaemon>;

public:
    // 固定写法
    static const char *getServiceName() {
        // BnDaemon这个服务的名称
        // 其他进程要通过这个名称才能找到BnDaemon服务
        return SERVER_NAME;
    }

public:
    BnDaemon();

    virtual ~BnDaemon();

    virtual int open(bool enableCapture);

    virtual int registerCallback(const sp<ICallback> &callback);

    // 必须的(Bn端才会有这个方法,Bp端是没有这个方法的)
    virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags = 0);

private:
    sp<ICallback> mCallback;

};

#endif // __MY_DAEMON_H__
