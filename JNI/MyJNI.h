#ifndef MYJNI_H
#define MYJNI_H

#include <jni.h>
#include <stdio.h>
#include <binder/BinderService.h>
#include <android/log.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../include/LogDebug.h"
#include "ICallback.h"
#include "IDaemon.h"
#include "MyConst.h"
#include "../part_a/include/BnCallback.h"

using namespace android;

class MyJNI {
public:
    MyJNI();
    virtual ~MyJNI();
};

#endif // MYJNI_H