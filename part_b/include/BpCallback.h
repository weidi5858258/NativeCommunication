//
// Created by root on 18-8-19.
//

#ifndef DAEMON_WEIDI_BPCALLBACK_H
#define DAEMON_WEIDI_BPCALLBACK_H

#include "../../include/LogDebug.h"
#include "../../Binder/ICallback.h"

class BpCallback : public BpInterface<ICallback> {
public:
    //需要这样定义构造方法
    BpCallback(const sp<IBinder> &impl);

    virtual ~BpCallback();

    /***

     */
    virtual int onRecognize(size_t len,
                            int captureType,
                            int width,
                            int height,
                            const char *fileName,
                            const char *result);

    virtual int onRecognize(int captureType, const char *result);

    virtual int onError(int errorCode);
};


#endif //DAEMON_WEIDI_BPCALLBACK_H
