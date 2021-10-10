/*  Date: 10/09/21
    Author: Artem Khlyupin
*/
#include "JFFT.h"
#include "FFT.h"
#include <complex.h>

static jint throwError(JNIEnv * env, const char * msg) {
    return (*env)->ThrowNew(
        env, (*env)->FindClass(env, "java/lang/RuntimeException"), msg);

}

JNIEXPORT jboolean JNICALL Java_com_e1da_fft_FFT_init(
    JNIEnv * env, jobject o, jint type, jint n) {

    if (type < 0) type = 0;
    if (type > 3) type = 0;

    if (!FFT_Init(type, n)) {
        throwError(env, "FFT init error.\n");
        return false;
    }
    return true;
}

/*
 * Class:     FFT
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_e1da_fft_FFT_close(
    JNIEnv * env, jobject o) {
    
    if (!FFT_Close()) {
        throwError(env, "FFT close error.\n");
    }
}

/*
 * Class:     FFT
 * Method:    process
 * Signature: ([F[F)V
 */
JNIEXPORT void JNICALL Java_com_e1da_fft_FFT_process(
    JNIEnv * env, jobject o, jfloatArray in, jfloatArray out) {

    jsize inLength = (*env)->GetArrayLength(env, in);
    jsize outLength = (*env)->GetArrayLength(env, out);

    if (inLength != outLength) {
        throwError(env, "Error. In and Out fft arrays are not equals.\n");
        return;
    }

    jfloat * inArr = (*env)->GetFloatArrayElements(env, in, 0);
    jfloat * outArr = (*env)->GetFloatArrayElements(env, out, 0);

    if (!FFT_Process((complex float *)inArr, (complex float *)outArr, inLength >> 1)) {
        throwError(env, "FFT process error.\n");
    }

    (*env)->ReleaseFloatArrayElements(env, in, inArr, 0);
    (*env)->ReleaseFloatArrayElements(env, out, outArr, 0);

}
