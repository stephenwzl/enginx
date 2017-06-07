LOCAL_PATH := $(call my-dir)/../../
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/includes/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/http_parser/
LOCAL_MODULE    := enginx-android
LOCAL_SRC_FILES := http_parser/http_parser.c \
                   implements/creator.c \
                   implements/enginx.c \
                   Lexer/enginx_parse.c \
                   Lexer/lex.enginx.c \
                   memory/memory.c \
                   memory/storage.c \
                   EnginxAndroidDemo/enginxandroid/src/main/cpp/enginx-android.cpp
LOCAL_CPP_FEATURES := exceptions
include $(BUILD_SHARED_LIBRARY)