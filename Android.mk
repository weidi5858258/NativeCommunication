LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    main.cpp \
    Daemon/MyDaemon.cpp \
    part_a/BpDaemon.cpp \
    part_b/BnDaemon.cpp \
    part_a/BnCallback.cpp \
    part_b/BpCallback.cpp \

#LOCAL_LDLIBS    := -L$(LOCAL_PATH)/../libs/ -lABC -lSF

LOCAL_C_INCLUDES := \
    bionic \
    external/libcxx/include \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/Binder

LOCAL_CXX_STL:=libc++

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libcutils \
    libutils \
    libc++ \
    liblog

LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_EXECUTABLES)
LOCAL_MODULE:= partb

include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
