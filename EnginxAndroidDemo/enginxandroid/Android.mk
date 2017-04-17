LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := enginx-android
LOCAL_SRC_FILES :=  src/main/cpp/enginx-android.cpp  src/main/cpp/core/enginxInstance.cpp src/main/cpp/core/enginxDefinitions.cpp src/main/cpp/core/enginxLocationProcessor.cpp src/main/cpp/core/enginxWorker.cpp src/main/cpp/core/enginxServerProcessor.cpp src/main/cpp/core/vendor/http_parser/http_parser.c
LOCAL_CPP_FEATURES := exceptions
include $(BUILD_SHARED_LIBRARY)