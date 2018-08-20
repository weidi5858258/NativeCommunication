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

    //addService inside in BinderService
    //先执行BnDaemon的构造方法
    //再执行MyDaemon的构造方法
    MyDaemon::instantiate();

    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();

    LOGE("-------------------- CMRD main end --------------------\n");

    return 0;
}
