//
// Created by root on 18-8-19.
//

#ifndef DAEMON_WEIDI_BNDAEMON_H
#define DAEMON_WEIDI_BNDAEMON_H

#include "../../include/LogDebug.h"
#include "../../Binder/IDaemon.h"

class BnDaemon : public BnInterface<IDaemon> {
public:
    BnDaemon();

    virtual ~BnDaemon();

    virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags = 0);
};

#endif //DAEMON_WEIDI_BNDAEMON_H
