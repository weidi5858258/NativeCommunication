#ifndef __IDAEMON__
#define __IDAEMON__

#include <stdint.h>

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

#include "ICallback.h"

using namespace android;

/****************************** IDaemon ******************************/

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

    //不能定义
//    virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags = 0) = 0;

};

#endif // __IDAEMON__
