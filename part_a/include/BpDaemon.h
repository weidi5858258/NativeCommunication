//
// Created by root on 18-8-19.
//

#ifndef DAEMON_WEIDI_BPDAEMON_H
#define DAEMON_WEIDI_BPDAEMON_H

#include <sys/types.h>
#include <sys/stat.h>

#include "../../include/LogDebug.h"
#include "../../Binder/IDaemon.h"
#include "../../include/MyConst.h"

class BpDaemon : public BpInterface<IDaemon> {
public:
    //需要这样定义构造方法(指的是括号中的参数)
    BpDaemon(const sp<IBinder> &impl);

    virtual ~BpDaemon();

    virtual int open(bool enableCapture);

    virtual int registerCallback(const sp<ICallback> &callback);
};


#endif //DAEMON_WEIDI_BPDAEMON_H
