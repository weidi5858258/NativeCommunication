
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

status_t MyDaemon::onTransact(uint32_t code,
                              const Parcel &data,
                              Parcel *reply,
                              uint32_t flags) {
    LOGI("MyDaemon::onTransact() %p\n", this);
    pid_t pid = getpid();
    LOGI("MyDaemon::onTransact() PID: %d\n", pid);

    switch (code) {
        case IDaemon::OPEN: {
            LOGI("MyDaemon::onTransact() OPEN");
            CHECK_INTERFACE(IDaemon, data, reply);
            bool enableCapture = (bool) data.readInt32();
            LOGI("MyDaemon::onTransact() OPEN enableCapture: %d\n", enableCapture);
            //MyDaemon::open
            int ret = open(enableCapture);
            reply->writeInt32(ret);
            break;
        }

        case IDaemon::REGISTER_CALLBACK: {
            LOGI("MyDaemon::onTransact() REGISTER_CALLBACK");
            CHECK_INTERFACE(IDaemon, data, reply);
            //BpCallback() created. 0x40890440
            sp<ICallback> callback = interface_cast<ICallback>(data.readStrongBinder());
            LOGI("MyDaemon::onTransact() callback: 0x%0x", &callback);
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
            LOGI("MyDaemon::onTransact() default");
            return BBinder::onTransact(code, data, reply, flags);
        }
    }

    return NO_ERROR;
}

/*
// 继承BnDaemon时使用
status_t MyDaemon::onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags) {
    LOGI("MyDaemon::onTransact() %p\n", this);
    // 直接调用父类方法
    status_t ret = MyDaemon::onTransact(code, data, reply, flags);
    return ret;
}*/
