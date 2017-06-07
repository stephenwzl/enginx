//
// Created by stephenw on 2017/4/5.
//

#include <jni.h>
extern "C" {
#include "enginx.h"
#include <stdlib.h>
}

extern "C"
JNIEXPORT jstring JNICALL
Java_cc_stephenw_enginxandroid_EnginxAndroid_rewriteURL(JNIEnv *env, jclass type,
                                                        jstring absoluteURLString_) {
    const char *nativeString = (*env).GetStringUTFChars(absoluteURLString_, JNI_FALSE);
    char* rewrited = enginx_rewrite_url((char*)nativeString);
    jstring ret = env->NewStringUTF(rewrited);
    free(rewrited);
    return ret;
}


extern "C"
JNIEXPORT jstring JNICALL
Java_cc_stephenw_enginxandroid_EnginxAndroid_loadConfig(JNIEnv
                                                        *env,
                                                        jclass type,
                                                        jstring config) {
    const char *nativeString = (*env).GetStringUTFChars(config, JNI_FALSE);
    enginx_load_config_string((char*)nativeString);
    enginx_compile_error *error = enginx_get_current_error();
    if (error != NULL) {
        jstring ret = env->NewStringUTF(error->error_msg);
        enginx_compile_error_release(&error);
        return ret;
    }
    return env->NewStringUTF("");
}