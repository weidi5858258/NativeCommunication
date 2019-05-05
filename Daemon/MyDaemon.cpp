
#include "MyDaemon.h"


//#define LOG_TAG "MyDaemon"
#define LOG_TAG "alexander"

/****************************** Daemon ******************************/

sp<ICallback> getCallback() {
    LOGD("getCallback() defaultServiceManager()");
    // 固定写法，得到android的IServiceManager对象
    sp<IServiceManager> serviceManager = defaultServiceManager();
    LOGD("getCallback() getService()");
    //
    sp<IBinder> binder = serviceManager->getService(String16(SERVER_NAME_));

    if (binder == NULL) {
        LOGE("getCallback() binder = NULL");
        // 返回NULL
        return sp<ICallback>(NULL);
    }

    LOGD("getCallback() interface_cast<ICallback>()");
    return interface_cast<ICallback>(binder);
}

MyDaemon::MyDaemon() {
    LOGD("MyDaemon::MyDaemon()  created   %p\n", this);
}

MyDaemon::~MyDaemon() {
    LOGD("MyDaemon::~MyDaemon() destroyed %p\n", this);
}

int MyDaemon::open(bool enableCapture) {
    LOGD("MyDaemon::open() %p enableCapture: %d\n", this, enableCapture);
//    getCallback()->onError(1);
    return 0;
}

int MyDaemon::registerCallback(const android::sp<ICallback> &callback) {
    LOGD("MyDaemon::registerCallback() %p\n", this);

    if (callback == NULL) {
        LOGD("MyDaemon::registerCallback() callback is null\n");
        return -1;
    }

    mCallback = callback;
    return NO_ERROR;
}

status_t MyDaemon::onTransact(uint32_t code,
                              const Parcel &data,
                              Parcel *reply,
                              uint32_t flags) {
    pid_t pid = getpid();
    LOGD("MyDaemon::onTransact() %p PID: %d\n", this, pid);

    switch (code) {
        case IDaemon::OPEN: {
            LOGD("MyDaemon::onTransact() OPEN");
            CHECK_INTERFACE(IDaemon, data, reply);
            bool enableCapture = (bool) data.readInt32();
            LOGD("MyDaemon::onTransact() OPEN enableCapture: %d\n", enableCapture);
            //MyDaemon::open
            int ret = open(enableCapture);
            reply->writeInt32(ret);
            break;
        }

        case IDaemon::REGISTER_CALLBACK: {
            LOGD("MyDaemon::onTransact() REGISTER_CALLBACK");
            CHECK_INTERFACE(IDaemon, data, reply);
            //BpCallback() created. 0x40890440
            sp<ICallback> callback = interface_cast<ICallback>(data.readStrongBinder());
            LOGD("MyDaemon::onTransact() REGISTER_CALLBACK callback: 0x%0x", &callback);
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
            LOGD("MyDaemon::onTransact() default");
            return BBinder::onTransact(code, data, reply, flags);
        }
    }

    return NO_ERROR;
}
