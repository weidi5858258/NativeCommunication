//
// Created by root on 18-8-19.
//

#include "include/BnDaemon.h"
#define LOG_TAG "alexander"

BnDaemon::BnDaemon() {
    LOGI("BnDaemon::BnDaemon()  created   %p\n", this);
}

BnDaemon::~BnDaemon() {
    LOGI("BnDaemon::~BnDaemon() destroyed %p\n", this);
}

status_t BnDaemon::onTransact(uint32_t code,
                              const Parcel &data,
                              Parcel *reply,
                              uint32_t flags) {
    LOGI("BnDaemon::onTransact() %p\n", this);
    pid_t pid = getpid();
    LOGI("BnDaemon::onTransact() PID: %d\n", pid);

    switch (code) {
        case IDaemon::OPEN: {
            LOGI("BnDaemon::onTransact() OPEN");
            CHECK_INTERFACE(IDaemon, data, reply);
            bool enableCapture = (bool) data.readInt32();
            LOGI("BnDaemon::onTransact() OPEN enableCapture: %d\n", enableCapture);
            //MyDaemon::open
            int ret = open(enableCapture);
            reply->writeInt32(ret);
            break;
        }

        case IDaemon::REGISTER_CALLBACK: {
            LOGI("BnDaemon::onTransact() REGISTER_CALLBACK");
            CHECK_INTERFACE(IDaemon, data, reply);
            //BpCallback() created. 0x40890440
            sp<ICallback> callback = interface_cast<ICallback>(data.readStrongBinder());
            LOGI("BnDaemon::onTransact() callback: 0x%0x", &callback);
            //不能直接调用MyCallback::onError(因为不在同一个进程中)
            //1.BpCallback::onError() 0x40890440 errorCode = -2
            //2.BnCallback::onTransact() ON_ERROR
            //3.MyCallback::onError() 0x4070a600 errorCode = -2
            callback->onError(-2);
            //MyDaemon::registerCallback
            int ret = registerCallback(callback);
            reply->writeInt32(ret);
            break;
        }

        default: {
            // C++中default后面必须要有代码,java中就不需要.
            // C++中case语句中必须要用{}括起来,java中就不需要.
            LOGI("BnDaemon::onTransact() default");
            return BBinder::onTransact(code, data, reply, flags);
        }
    }

    return NO_ERROR;
}