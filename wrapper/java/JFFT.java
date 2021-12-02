/*  Date: 10/08/21
    Author: Artem Khlyupin
*/
package com.e1da.fft;

public class JFFT {
    static {
        System.loadLibrary("JFFT");
    }
    
    private native boolean init(int type, int n);

    public JFFT(int type, int n) throws Exception {
        if (!init(type, n)) {
            throw new Exception("FFT init error!");
        }
    }

    public native void close() throws Exception;
    public native void process(float[] in, float[] out) throws Exception;

    public native int getVersion();
    public String getVersionString() {
        int v = getVersion();
        return Integer.toString((v & 0xFF0000) >> 16) + "." + 
                Integer.toString((v & 0xFF00) >> 8) + "." + 
                Integer.toString(v & 0xFF);
    }
}