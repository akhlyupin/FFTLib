/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_e1da_fft_JFFT */

#ifndef _Included_com_e1da_fft_JFFT
#define _Included_com_e1da_fft_JFFT
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_e1da_fft_JFFT
 * Method:    init
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_com_e1da_fft_JFFT_init
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_e1da_fft_JFFT
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_e1da_fft_JFFT_close
  (JNIEnv *, jobject);

/*
 * Class:     com_e1da_fft_JFFT
 * Method:    process
 * Signature: ([F[F)V
 */
JNIEXPORT void JNICALL Java_com_e1da_fft_JFFT_process
  (JNIEnv *, jobject, jfloatArray, jfloatArray);

/*
 * Class:     com_e1da_fft_JFFT
 * Method:    getVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_e1da_fft_JFFT_getVersion
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
