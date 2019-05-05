
#include "MyDaemon.h"


//#define LOG_TAG "MyDaemon"
#define LOG_TAG "alexander"

/****************************** Daemon ******************************/

sp<ICallback> getCallback() {
    LOGI("getCallback() defaultServiceManager()");
    //固定写法，得到android的IServiceManager对象
    sp<IServiceManager> serviceManager = defaultServiceManager();
    LOGI("getCallback() getService()");
    //
    sp<IBinder> binder = serviceManager->getService(String16(SERVER_NAME_));

    if (binder == NULL) {
        LOGE("getCallback() binder = NULL");
        // 返回NULL
        return sp<ICallback>(NULL);
    }

    LOGI("getCallback() interface_cast<ICallback>()");
    return interface_cast<ICallback>(binder);
}

MyDaemon::MyDaemon() {
    LOGI("MyDaemon::MyDaemon()  created   %p\n", this);
}

MyDaemon::~MyDaemon() {
    LOGI("MyDaemon::~MyDaemon() destroyed %p\n", this);
}

int MyDaemon::open(bool enableCapture) {
    LOGI("MyDaemon::open() %p enableCapture: %d\n", this, enableCapture);
//    getCallback()->onError(1);
    return 0;
}

int MyDaemon::registerCallback(const android::sp<ICallback> &callback) {
    LOGI("MyDaemon::registerCallback() %p\n", this);

    if (callback == NULL) {
        LOGI("callback is null\n");
        return -1;
    }

    mCallback = callback;
    return NO_ERROR;
}

status_t MyDaemon::onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags) {
    LOGI("MyDaemon::onTransact() %p\n", this);
    status_t ret = BnDaemon::onTransact(code, data, reply, flags);
    return ret;
}