package com.e1da.fft;

public class Test {
    public static void main(String[] args) {
        float[] in = new float[]{1, 0, 2, 0, 3, 0, 4, 0};
        float[] out = new float[in.length];

        try {
            JFFT fft = new JFFT(1, 4);
            System.out.println("Version: " + fft.toString());

            fft.process(in, out);
            fft.close();

            for (int i = 0; i < out.length; i += 2) {
                System.out.println("["+ (i >> 1) + "]: " + 
                    out[i] + " + " + out[i + 1] + "*I");
            }

        } catch (Exception e) {
            System.out.println(e.toString());
        }
        
    }
}