//
// Created by root on 18-8-19.
//

#include "include/BpDaemon.h"

//#define LOG_TAG "BpDaemon"
#define LOG_TAG "alexander"

IMPLEMENT_META_INTERFACE(Daemon, MYDAEMON);

/****************************** BpDaemon ******************************/

BpDaemon::BpDaemon(const sp<IBinder> &impl)
        : BpInterface<IDaemon>(impl) {
    pid_t pid = getpid();
    LOGI("BpDaemon::BpDaemon()  created   %p PID: %d\n", this, pid);
}

BpDaemon::~BpDaemon() {
    pid_t pid = getpid();
    LOGI("BpDaemon::~BpDaemon() destroyed %p PID: %d\n", this, pid);
}

int BpDaemon::open(bool enableCapture) {
    pid_t pid = getpid();
    LOGI("BpDaemon::open() %p PID: %d enableCapture: %d\n", this, pid, enableCapture);

    Parcel data, reply;
    data.writeInterfaceToken(IDaemon::getInterfaceDescriptor());
    data.writeInt32(enableCapture);

    //先调用子类,由子类去调用父类
    //BnDaemon::onTransact
    //BnDaemon::onTransact
    LOGI("BpDaemon::open() remote()->transact\n");
    status_t status = remote()->transact(IDaemon::OPEN, data, &reply);
    if (status != OK) {
        return RESULT_ERR_BINDER_ERROR;
    }

    int ret = reply.readInt32();
    LOGI("BpDaemon::open() ret: %d\n", ret);
    return ret;
}

int BpDaemon::registerCallback(const android::sp<ICallback> &callback) {
    pid_t pid = getpid();
    LOGI("BpDaemon::registerCallback() %p PID: %d callback: 0x%0x\n", this, pid, &callback);
    // 直接调用MyCallback::onError(因为在同一个进程中)
    callback->onError(-1);

    Parcel data, reply;
    data.writeInterfaceToken(IDaemon::getInterfaceDescriptor());
    data.writeStrongBinder(IInterface::asBinder(callback));

    //先调用子类,由子类去调用父类
    //BnDaemon::onTransact
    //BnDaemon::onTransact
    LOGI("BpDaemon::registerCallback() remote()->transact");
    status_t status = remote()->transact(IDaemon::REGISTER_CALLBACK, data, &reply);
    if (status != OK) {
        LOGI("BpDaemon::registerCallback() transact failed. status: %d\n", status);
        return RESULT_ERR_BINDER_ERROR;
    }

    int ret = reply.readInt32();
    LOGI("BpDaemon::registerCallback() ret: %d\n", ret);
    return ret;
}