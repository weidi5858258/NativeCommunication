#ifndef __ICALLBACK_H__
#define __ICALLBACK_H__

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

using namespace android;

/****************************** ICallback ******************************/

class ICallback : public IInterface {
public:
    DECLARE_META_INTERFACE(Callback);

public:
    enum {
        ON_RECOGNIZE = IBinder::FIRST_CALL_TRANSACTION,
        ON_RECOGNIZE_NON_IMAGE,
        ON_ERROR
    };

    virtual int onRecognize(size_t len,
                            int captureType,
                            int width,
                            int height,
                            const char *fileName,
                            const char *result) = 0;

    virtual int onRecognize(int captureType, const char *result) = 0;

    virtual int onError(int errorCode) = 0;

};

#endif //__ICALLBACK_H__
