/*  Date: 10/08/21
    Author: Artem Khlyupin
*/
package com.e1da.fft;

public class FFT {
    static {
        System.loadLibrary("JFFT");
    }
    
    private native boolean init(int type, int n);

    public FFT(int type, int n) throws Exception {
        if (!init(type, n)) {
            throw new Exception("FFT init error!");
        }
    }

    public native void close() throws Exception;
    public native void process(float[] in, float[] out) throws Exception;

}