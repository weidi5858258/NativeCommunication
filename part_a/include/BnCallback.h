//
// Created by root on 18-8-19.
//

#ifndef DAEMON_WEIDI_BNCALLBACK_H
#define DAEMON_WEIDI_BNCALLBACK_H

#include "../../include/LogDebug.h"
#include "../../Binder/ICallback.h"

/***
服务端
 */
class BnCallback : public BnInterface<ICallback> {
public:
    BnCallback();

    virtual ~BnCallback();

    virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags = 0);
};

#endif //DAEMON_WEIDI_BNCALLBACK_H
