//
// Created by stephenw on 2017/4/5.
//

#include <jni.h>
#include <string>
#include "core/enginx.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_cc_stephenw_enginxandroid_EnginxAndroid_rewriteURL(JNIEnv *env, jclass type,
                                                        jstring absoluteURLString_) {
    const char *absoluteURLString = env->GetStringUTFChars(absoluteURLString_, 0);
    std::string rewritedURL;
    enginx::Enginx::transfer(absoluteURLString, rewritedURL);
    env->ReleaseStringUTFChars(absoluteURLString_, absoluteURLString);
    return env->NewStringUTF(rewritedURL.c_str());
}


extern "C"
JNIEXPORT jstring JNICALL
Java_cc_stephenw_enginxandroid_EnginxAndroid_loadConfig(JNIEnv
                                                        *env,
                                                        jclass type,
                                                        jstring config) {
    const char *nativeString = (*env).GetStringUTFChars(config, JNI_FALSE);
    enginx::EnginxError error;
    enginx::Enginx::load(nativeString, error);
    (*env).ReleaseStringUTFChars(config, nativeString);
    if (error.code) {
        return env->NewStringUTF(error.message.c_str());
    }
    return env->NewStringUTF("");
}