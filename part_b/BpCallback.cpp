//
// Created by root on 18-8-19.
//

#include "include/BpCallback.h"

//#define LOG_TAG "BpCallback"
#define LOG_TAG "alexander"

IMPLEMENT_META_INTERFACE(Callback, CALLBACK);

BpCallback::BpCallback(const sp<IBinder> &impl)
        : BpInterface<ICallback>(impl) {
    LOGI("BpCallback::BpCallback()  created   %p\n", this);
}

BpCallback::~BpCallback() {
    LOGI("BpCallback::~BpCallback() destroyed %p\n", this);
}

int BpCallback::onRecognize(size_t len,
                                int captureType,
                                int width,
                                int height,
                                const char *fileName,
                                const char *result) {
    LOGI("BpCallback::onRecognize(6) %p captureType: %d fileName: %s", this, captureType, fileName);
    Parcel data, reply;
    data.writeInterfaceToken(ICallback::getInterfaceDescriptor());
    data.writeInt32(len);
    data.writeInt32(captureType);
    data.writeInt32(width);
    data.writeInt32(height);

    if ((len > 0) && (fileName != NULL)) {
        data.writeCString(fileName);
    }

    if (result != NULL) {
        data.writeInt32(strlen(result));
        data.writeCString(result);
    } else {
        data.writeInt32(0);
    }

    status_t status = remote()->transact(ICallback::ON_RECOGNIZE, data, &reply);
    LOGI("BpCallback::onRecognize(6) status: %d\n", status);
    return status;
}

int BpCallback::onRecognize(int captureType, const char *result) {
    LOGI("BpCallback::onRecognize(2) %p captureType: %d result: %s", this, captureType, result);
    Parcel data, reply;
    data.writeInterfaceToken(ICallback::getInterfaceDescriptor());
    data.writeInt32(captureType);

    if (result != NULL) {
        data.writeInt32(strlen(result));
        data.writeCString(result);
    } else {
        data.writeInt32(0);
    }

    status_t status = remote()->transact(ICallback::ON_RECOGNIZE_NON_IMAGE, data, &reply);
    LOGI("BpCallback::onRecognize(2) status: %d\n", status);
    return status;
}

int BpCallback::onError(int errorCode) {
    LOGI("BpCallback::onError() %p errorCode: %d", this, errorCode);
    Parcel data, reply;
    data.writeInterfaceToken(ICallback::getInterfaceDescriptor());
    data.writeInt32(errorCode);
    status_t status = remote()->transact(ICallback::ON_ERROR, data, &reply);
    LOGI("BpCallback::onError() status: %d\n", status);
    return status;
}