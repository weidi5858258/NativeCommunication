//
// Created by root on 18-8-19.
//

#include "include/BnCallback.h"

#define LOG_TAG "BnCallback"

BnCallback::BnCallback() {
    LOGI("BnCallback() created. %p\n", this);
}

BnCallback::~BnCallback() {
    LOGI("BnCallbackestroyed. %p\n", this);
}

status_t BnCallback::onTransact(
        uint32_t code,
        const Parcel &data,
        Parcel *reply,
        uint32_t flags) {
    LOGI("BnCallback::onTransact(). %p\n", this);

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

            LOGI("BnCallback::onTransact() captureType = %d resultLen = %d", captureType, resultLen);
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

            LOGI("BnCallback::onTransact() captureType = %d resultLen = %d", captureType, resultLen);
            //调用MyCallback方法
            int ret = onRecognize(captureType, result);
            reply->writeInt32(ret);
            break;
        }

        case ICallback::ON_ERROR: {
            LOGI("BnCallback::onTransact() ON_ERROR");
            CHECK_INTERFACE(ICallback, data, reply);
            int errorCode = data.readInt32();
            LOGI("BnCallback::onTransact() errorCode = %d", errorCode);
            //调用MyCallback方法
            int ret = onError(errorCode);
            reply->writeInt32(ret);
            break;
        }

        default:
            LOGI("BnCallback::onTransact() default");
            return BBinder::onTransact(code, data, reply, flags);
    }

    return NO_ERROR;
}