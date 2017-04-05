#include <jni.h>

JNIEXPORT jthrowableJNICALL
Java_cc_stephenw_enginxandroid_EnginxAndroid_loadConfig(JNIEnv
*env,
jclass type,
        jstring config_)
{
const char *config = (*env)->GetStringUTFChars(env, config_, 0);

// TODO

(*env)->ReleaseStringUTFChars(env, config_, config);
}