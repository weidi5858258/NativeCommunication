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
#include "../part_b/include/BnDaemon.h"

/****************************** MyDaemon ******************************/

class MyDaemon :
        public BinderService<MyDaemon>,
        public BnDaemon {

    friend class BinderService<MyDaemon>;

public:
    static const char *getServiceName() {
        return SERVER_NAME;
    }

public:
    MyDaemon();

    virtual ~MyDaemon();

    virtual int open(bool enableCapture);

    virtual int registerCallback(const sp<ICallback> &callback);

    virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags = 0);

private:
    sp<ICallback> mCallback;
};

#endif // __MY_DAEMON_H__
