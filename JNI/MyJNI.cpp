#include "MyJNI.h"

#define LOG_TAG "MyJNI"

MyJNI::MyJNI() {
}

static sp<ICallback> sCallback = NULL;

sp<IDaemon> getDaemon() {
    LOGI("getDaemon()");
    // 固定写法，得到android的IServiceManager对象
    sp<IServiceManager> serviceManager = defaultServiceManager();
    //
    sp<IBinder> binder = serviceManager->getService(String16(SERVER_NAME));
    if (binder == NULL) {
        // 返回NULL
        return sp<IDaemon>(NULL);
    }

    return interface_cast<IDaemon>(binder);
}



class MyCallback :
        public BinderService<MyCallback>,
        public BnCallback {

    friend class BinderService<MyCallback>;

public:
    static const char *getServiceName() {
        return SERVER_NAME_;
    }

public:
    MyCallback() {
        LOGI("MyCallback() created. %p\n", this);
    }

    virtual ~MyCallback() {
        LOGI("MyCallback() destroyed. %p\n", this);
    }

    virtual int onRecognize(size_t len,
                            int captureType,
                            int width,
                            int height,
                            const char *fileName,
                            const char *result) {
        LOGI("MyCallback::onRecognize(6) %p length=%d fileName=%p captureType=%#08x width=%d height=%d\n",
             this, len, fileName, captureType, width, height);
        return RESULT_OK;
    }

    virtual int onRecognize(int captureType, const char *result) {
        LOGI("MyCallback::onRecognize(2) %p captureType = %d result = %s\n",
             this, captureType, result);
        return RESULT_OK;
    }

    virtual int onError(int errorCode) {
        LOGI("MyCallback::onError() %p errorCode = %d\n", this, errorCode);
        return RESULT_OK;
    }

private:
    bool getEnv(JNIEnv **env) {
        bool isAttached = false;
        return isAttached;
    }
};

///////////////////////////jni///////////////////////////

jint Java_com_weidi_JniWrapper_open(
        JNIEnv *env, jobject thiz, jboolean enableCapture) {
    LOGI("Java_com_weidi_JniWrapper_open()");

    sp<IDaemon> daemon = getDaemon();
    if (daemon == NULL) {
        return RESULT_ERR_BINDER_ERROR;
    }

    if (sCallback == NULL) {
        sCallback = new MyCallback();
    }

    /***
    得到了IDaemon对象,
    那么就可以调用IDaemon这个对象下面定义的虚函数,
    由于BpDaemon类fu写了虚函数,
    那么得到的对象实际上就是BpDaemon类对象.
    BpDaemon是作为客户端的.
    然后使用android特有的进程间通信的方式跟BnDaemon类进行通信.
    */
    daemon->registerCallback(sCallback);
    daemon->open(enableCapture);
    return 0;
}

/***
 先开启进程1
 然后在进程2中向进程1发起请求
 this here(进程2) ---> BpDaemon(进程2,传递callback) --->
                      BnDaemon(进程1,通过callback) --->
                      BpCallback(进程1)           ---> BnCallback(进程2)
 */
int main(int argc, char *argv[]) {

    LOGE("-------------------- CMRD main start --------------------\n");

    pid_t pid = getpid();
    LOGE("PID=%d.\n", pid);
    uid_t uid = getuid();
    LOGE("UID= %d\n", uid);

//    sp<ProcessState> proc(ProcessState::self());
//    sp<IServiceManager> sm = defaultServiceManager();
//
//    //addService inside in BinderService
//    MyCallback::instantiate();

    sp<IDaemon> daemon = getDaemon();
    if (daemon == NULL) {
        return RESULT_ERR_BINDER_ERROR;
    }

    if (sCallback == NULL) {
        //BnCallback() created. 0x4078a600
        //MyCallback() created. 0x4078a600
        sCallback = new MyCallback();
    }

    daemon->registerCallback(sCallback);
    daemon->open(1);

//    ProcessState::self()->startThreadPool();
//    IPCThreadState::self()->joinThreadPool();

    LOGE("-------------------- CMRD main end --------------------\n");
    return 0;
}

/***

*/