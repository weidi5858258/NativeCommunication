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
    LOGI("BpDaemon() created. %p\n", this);
}

BpDaemon::~BpDaemon() {
    LOGI("BpDaemonn", this);
}

int BpDaemon::open(bool enableCapture) {
    LOGI("BpDaemon::open() enableCapture = %d\n", enableCapture);
    pid_t pid = getpid();
    LOGE("PID=%d.\n", pid);
    Parcel data, reply;
    data.writeInterfaceToken(IDaemon::getInterfaceDescriptor());
    data.writeInt32(enableCapture);
    LOGI("BpDaemon::open() remote()->transact");

    //先调用子类,由子类去调用父类
    //MyDaemon::onTransact
    //BnDaemon::onTransact
    status_t status = remote()->transact(IDaemon::OPEN, data, &reply);
    if (status != OK) {
        return RESULT_ERR_BINDER_ERROR;
    }

    int ret = reply.readInt32();
    LOGI("BpDaemon::open() ret = %d\n", ret);
    return ret;
}

int BpDaemon::registerCallback(const android::sp<ICallback> &callback) {
    LOGI("BpDaemon::registerCallback() callback: 0x%0x", &callback);
    //直接调用MyCallback::onError(因为在同一个进程中)
    callback->onError(-1);
    pid_t pid = getpid();
    LOGE("PID=%d.\n", pid);
    Parcel data, reply;
    data.writeInterfaceToken(IDaemon::getInterfaceDescriptor());
    data.writeStrongBinder(IInterface::asBinder(callback));

    //先调用子类,由子类去调用父类
    //MyDaemon::onTransact
    //BnDaemon::onTransact
    status_t status = remote()->transact(IDaemon::REGISTER_CALLBACK, data, &reply);
    if (status != OK) {
        LOGI("BpDaemon::registerCallback() transact failed. status=%d\n", status);
        return RESULT_ERR_BINDER_ERROR;
    }

    int ret = reply.readInt32();
    LOGI("BpDaemon::registerCallback() ret = %d\n", ret);
    return ret;
}