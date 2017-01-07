#include <jni.h>
#include <string>

extern "C"
jint
Java_com_example_xxx_myopenglportandroid_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++ yes";
    return 22;//env->NewStringUTF(hello.c_str());
}
