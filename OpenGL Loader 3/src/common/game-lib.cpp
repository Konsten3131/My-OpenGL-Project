#include <jni.h>
#include "game.h"
#include "ResourceUtils.h"

extern "C"
JNIEXPORT void JNICALL Java_com_example_xxx_myopenglportandroid_MainActivity_on_1surface_1created
(JNIEnv * env, jclass cls) {
   on_surface_created();
}

extern "C"
JNIEXPORT void JNICALL Java_com_example_xxx_myopenglportandroid_MainActivity_on_1surface_1changed
(JNIEnv * env, jclass cls, jint width, jint height) {
    on_surface_changed(width, height);
}


extern "C"
JNIEXPORT void JNICALL Java_com_example_xxx_myopenglportandroid_MainActivity_on_1draw_1frame
(JNIEnv * env, jclass cls) {
    on_draw_frame();
}

extern "C"
JNIEXPORT void JNICALL Java_com_example_xxx_myopenglportandroid_MainActivity_setAppPath
        (JNIEnv * env, jclass cls, jstring path) {
    const char* appPath = env->GetStringUTFChars(path,0);
    ResourceUtils::setPathForSpecifiedResource(appPath);
}

extern "C"
JNIEXPORT void JNICALL Java_com_example_xxx_myopenglportandroid_MainActivity_rotateCameraAroundAxis
        (JNIEnv * env, jclass cls, jint distance) {

    rotateCameraAroundAxis(distance);
}

extern "C"
JNIEXPORT float JNICALL Java_com_example_xxx_myopenglportandroid_MainActivity_getAngleX
        (JNIEnv * env, jclass cls) {

   return getAxisAngleX();

}

extern "C"
JNIEXPORT float JNICALL Java_com_example_xxx_myopenglportandroid_MainActivity_getAngleY
        (JNIEnv * env, jclass cls) {

    return getAxisAngleY();

}

extern "C"
JNIEXPORT void JNICALL Java_com_example_xxx_myopenglportandroid_MainActivity_setAngleX
        (JNIEnv * env, jclass cls, float angle) {

     setAxisAngleX(angle);

}

extern "C"
JNIEXPORT void JNICALL Java_com_example_xxx_myopenglportandroid_MainActivity_setAngleY
        (JNIEnv * env, jclass cls, float angle) {

    setAxisAngleY(angle);

}

