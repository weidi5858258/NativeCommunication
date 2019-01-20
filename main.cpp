#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include "../include/LogDebug.h"
#include "Daemon/MyDaemon.h"

#define LOG_TAG "main"

using namespace android;

int main(int argc, char *argv[]) {

    LOGE("-------------------- CMRD main start --------------------\n");

    pid_t pid = getpid();
    LOGE("PID=%d.\n", pid);
    uid_t uid = getuid();
    LOGE("UID= %d\n", uid);

    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();

    /***
     在android启动的时候,由系统启动时的init进程启动其他服务,
     这些服务需要在init.rc中进行配置过.
     MyDaemon作为一个服务从android开机启动后一直常驻后台.
     通过binder通信方式跟其他进程进行通信.
     如java代码通过jni方式执行native层代码,然后native层代码
     通过binder方式跟MyDaemon进行通信.
     */
    // addService inside in BinderService
    // 先执行BnDaemon的构造方法
    // 再执行MyDaemon的构造方法
    MyDaemon::instantiate();

    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();

    LOGE("-------------------- CMRD main end --------------------\n");

    return 0;
}
