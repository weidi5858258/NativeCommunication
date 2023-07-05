#include "MyJNI.h"

//#define LOG_TAG "MyJNI"
#define LOG_TAG "alexander"

MyJNI::MyJNI() {
    LOGI("MyJNI::MyJNI()  created   %p\n", this);
}

MyJNI::~MyJNI() {
    LOGI("MyJNI::~MyJNI() destroyed %p\n", this);
}

static sp<ICallback> sCallback = NULL;

sp<IDaemon> getDaemon() {
    LOGI("getDaemon()");
    // 固定写法，得到android的IServiceManager对象
    sp<IServiceManager> serviceManager = defaultServiceManager();
    // 通过 SERVER_NAME 名称找到服务端
    /***
     binder不为NULL时,表示可以得到系统服务.
     如果为NULL,那么可能是客户端没有权限去得到服务端的对象.
     所以这里的binder是否为NULL很关键.
     */
    sp<IBinder> binder = serviceManager->getService(String16(SERVER_NAME));
    if (binder == NULL) {
        // 返回NULL
        return sp<IDaemon>(NULL);
    }
    LOGI("getDaemon() binder: %p\n", &binder);
    /***
     调用下面的代码后系统会去创建BpDaemon对象
     因为要连接到服务端,必须由BpDaemon对象完成的
     */
    return interface_cast<IDaemon>(binder);
}

class BnCallback :
        public BinderService<BnCallback>,
        //public BnCallback {
        public BnInterface<ICallback> {
    // 固定写法
    friend class BinderService<BnCallback>;

public:
    // 固定写法
    static const char *getServiceName() {
        return SERVER_NAME_;
    }

public:
    BnCallback() {
        pid_t pid = getpid();
        LOGI("BnCallback::BnCallback()  created   %p PID: %d\n", this, pid);
    }

    virtual ~BnCallback() {
        pid_t pid = getpid();
        LOGI("BnCallback::BnCallback() destroyed %p PID: %d\n", this, pid);
    }

    virtual status_t onTransact(
            uint32_t code,
            const Parcel &data,
            Parcel *reply,
            uint32_t flags) {
        pid_t pid = getpid();
        LOGI("BnCallback::onTransact() %p PID: %d\n", this, pid);

        switch (code) {
            case ICallback::ON_RECOGNIZE: {
                LOGI("BnCallback::onTransact() ON_RECOGNIZE");
                CHECK_INTERFACE(ICallback, data, reply);
                int len = data.readInt32();
                int captureType = data.readInt32();
                int width = data.readInt32();
                int height = data.readInt32();
                const char *fileName = NULL;

                if (len > 0) {
                    fileName = data.readCString();
                }

                const char *result = NULL;
                int resultLen = data.readInt32();

                if (resultLen > 0) {
                    result = data.readCString();
                }

                LOGI("BnCallback::onTransact() ON_RECOGNIZE captureType: %d resultLen: %d\n", captureType, resultLen);
                //调用MyCallback方法
                int ret = onRecognize(len, captureType, width, height, fileName, result);
                reply->writeInt32(ret);
                break;
            }

            case ICallback::ON_RECOGNIZE_NON_IMAGE: {
                LOGI("BnCallback::onTransact() ON_RECOGNIZE_NON_IMAGE");
                CHECK_INTERFACE(ICallback, data, reply);
                int captureType = data.readInt32();
                const char *result = NULL;
                int resultLen = data.readInt32();

                if (resultLen > 0) {
                    result = data.readCString();
                }

                LOGI("BnCallback::onTransact() ON_RECOGNIZE_NON_IMAGE captureType: %d resultLen: %d\n", captureType,
                     resultLen);
                //调用MyCallback方法
                int ret = onRecognize(captureType, result);
                reply->writeInt32(ret);
                break;
            }

            case ICallback::ON_ERROR: {
                LOGI("BnCallback::onTransact() ON_ERROR");
                CHECK_INTERFACE(ICallback, data, reply);
                int errorCode = data.readInt32();
                LOGI("BnCallback::onTransact() ON_ERROR errorCode: %d", errorCode);
                //调用MyCallback方法
                int ret = onError(errorCode);
                reply->writeInt32(ret);
                break;
            }

            default: {
                LOGI("BnCallback::onTransact() default");
                return BBinder::onTransact(code, data, reply, flags);
            }
        }

        return NO_ERROR;
    }

    virtual int onRecognize(size_t len,
                            int captureType,
                            int width,
                            int height,
                            const char *fileName,
                            const char *result) {
        LOGI("BnCallback::onRecognize(6) %p length: %d fileName: %p captureType: %#08x width: %d height: %d\n",
             this, len, fileName, captureType, width, height);
        return RESULT_OK;
    }

    virtual int onRecognize(int captureType, const char *result) {
        LOGI("BnCallback::onRecognize(2) %p captureType: %d result: %s\n",
             this, captureType, result);
        return RESULT_OK;
    }

    virtual int onError(int errorCode) {
        pid_t pid = getpid();
        LOGI("BnCallback::onError() %p PID: %d errorCode: %d\n", this, pid, errorCode);
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
        sCallback = new BnCallback();
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
                      BpCallback(进程1)           --->
                      BnCallback(进程2)
 */
int main(int argc, char *argv[]) {

    LOGI("-------------------- client main start --------------------\n");

    pid_t pid = getpid();
    LOGI("Client main() PID: %d\n", pid);
    uid_t uid = getuid();
    LOGI("Client main() UID: %d\n", uid);

//    sp<ProcessState> proc(ProcessState::self());
//    sp<IServiceManager> sm = defaultServiceManager();

//    BnCallback::instantiate();

    sp<IDaemon> daemon = getDaemon();
    if (daemon == NULL) {
        LOGI("Client main() daemon = NULL\n");
        return RESULT_ERR_BINDER_ERROR;
    }

    if (sCallback == NULL) {
        //BnCallback() created. 0x4078a600
        //BnCallback() created. 0x4078a600
        sCallback = new BnCallback();
    }

    /***
     在getDaemon()方法中执行最后一句代码时会创建BpDaemon对象(设为bpDaemon)
     然后通过bpDaemon对象调用registerCallback()方法,
     接着调用到bnDaemon(服务端)的onTransact()方法.
     */
    daemon->registerCallback(sCallback);
    daemon->open(1);

//    ProcessState::self()->startThreadPool();
//    IPCThreadState::self()->joinThreadPool();

    LOGI("-------------------- client main end --------------------\n");

    return 0;
}

/***
 某次执行的结果:
05-06 00:52:07.997 8030-8030/? D/alexander: -------------------- server main start --------------------
05-06 00:52:07.997 8030-8030/? D/alexander: Daemon main() PID: 8030
05-06 00:52:07.997 8030-8030/? D/alexander: Daemon main() UID: 0
05-06 00:52:07.999 8030-8030/? D/alexander: BnDaemon::BnDaemon()  created   0x40890400
05-06 00:52:10.169 8034-8034/? I/alexander: -------------------- client main start --------------------
05-06 00:52:10.169 8034-8034/? I/alexander: Client main() PID: 8034
05-06 00:52:10.169 8034-8034/? I/alexander: Client main() UID: 0
05-06 00:52:10.169 8034-8034/? I/alexander: getDaemon()
05-06 00:52:10.172 8034-8034/? I/alexander: getDaemon() binder: 0xbeabc970
05-06 00:52:10.172 8034-8034/? I/alexander: BpDaemon::BpDaemon()  created   0x40810400 PID: 8034
05-06 00:52:10.172 8034-8034/? I/alexander: BnCallback::BnCallback()  created   0x4080a5d0 PID: 8034
05-06 00:52:10.172 8034-8034/? I/alexander: BpDaemon::registerCallback() 0x40810400 PID: 8034 callback: 0x40067004
05-06 00:52:10.172 8034-8034/? I/alexander: BnCallback::onError() 0x4080a5d0 PID: 8034 errorCode: -1
05-06 00:52:10.172 8034-8034/? I/alexander: BpDaemon::registerCallback() remote()->transact
05-06 00:52:10.172 8030-8030/? D/alexander: BnDaemon::onTransact() 0x40890400 PID: 8030
05-06 00:52:10.172 8030-8030/? D/alexander: BnDaemon::onTransact() REGISTER_CALLBACK
05-06 00:52:10.172 8030-8030/? D/alexander: BpCallback::BpCallback()  created   0x40890420 PID: 8030
05-06 00:52:10.172 8030-8030/? D/alexander: BnDaemon::onTransact() REGISTER_CALLBACK callback: 0xbee78854
05-06 00:52:10.173 8030-8030/? D/alexander: BnDaemon::onTransact() REGISTER_CALLBACK callback->onError(-2)
05-06 00:52:10.173 8030-8030/? D/alexander: BpCallback::onError() 0x40890420 PID: 8030 errorCode: -2
05-06 00:52:10.173 8034-8034/? I/alexander: BnCallback::onTransact() 0x4080a5d0 PID: 8034
05-06 00:52:10.173 8034-8034/? I/alexander: BnCallback::onTransact() ON_ERROR
05-06 00:52:10.173 8034-8034/? I/alexander: BnCallback::onTransact() ON_ERROR errorCode: -2
05-06 00:52:10.173 8034-8034/? I/alexander: BnCallback::onError() 0x4080a5d0 PID: 8034 errorCode: -2
05-06 00:52:10.173 8030-8030/? D/alexander: BpCallback::onError() status: 0
05-06 00:52:10.173 8030-8030/? D/alexander: BnDaemon::registerCallback() 0x40890400 callback: 0xbee78854
05-06 00:52:10.173 8034-8034/? I/alexander: BpDaemon::registerCallback() ret: 0
05-06 00:52:10.173 8034-8034/? I/alexander: BpDaemon::open() 0x40810400 PID: 8034 enableCapture: 1
05-06 00:52:10.173 8034-8034/? I/alexander: BpDaemon::open() remote()->transact
05-06 00:52:10.173 8030-8033/? D/alexander: BnDaemon::onTransact() 0x40890400 PID: 8030
05-06 00:52:10.173 8030-8033/? D/alexander: BnDaemon::onTransact() OPEN
05-06 00:52:10.173 8030-8033/? D/alexander: BnDaemon::onTransact() OPEN enableCapture: 1
05-06 00:52:10.174 8030-8033/? D/alexander: BnDaemon::open() 0x40890400 enableCapture: 1
05-06 00:52:10.174 8034-8034/? I/alexander: BpDaemon::open() ret: 0
05-06 00:52:10.174 8034-8034/? I/alexander: -------------------- client main end --------------------
05-06 00:52:10.174 8034-8034/? I/alexander: BpDaemon::~BpDaemon() destroyed 0x40810400 PID: 8034


*/