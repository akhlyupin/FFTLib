/*  Date: 10/08/21
    Author: Artem Khlyupin
*/
package com.e1da.fft;

import java.util.Calendar;

public class JFFT implements FFT {
    static {
        System.loadLibrary("JFFT");
    }
    
    private native boolean init(int type, int n);

    public JFFT(int type, int n) throws Exception {
        if (!init(type, n)) {
            throw new Exception("FFT init error!");
        }
    }

    @Override
    public native void close() throws Exception;
    @Override
    public native void process(float[] in, float[] out) throws Exception;

    public native int getVersion();

    @Override
    public String toString() {
        int v = getVersion();
        return Integer.toString((v & 0xFF0000) >> 16) + "." + 
                Integer.toString((v & 0xFF00) >> 8) + "." + 
                Integer.toString(v & 0xFF);
    }

    @Override
    public int performance(int n) {
        float[] in = SinData.getComplex(n);
        float[] out = new float[in.length];

        try {
            long time = Calendar.getInstance().getTimeInMillis();
            process(in, out);
            return (int)(Calendar.getInstance().getTimeInMillis() - time);

        } catch (Exception e) {
            System.out.println(e);
        }
        return -1;
    }
}