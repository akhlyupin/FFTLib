/*
 *  SinData.java
 *
 *  Created by Artem Khlyupin on 10/12/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.e1da.fft;

public class SinData {
    public static float[] getComplex(int n) {
        float[] d = new float[n * 2];

        for (int i = 0; i < n; i++) {
            d[i * 2] = (float)Math.sin(i);
            d[i * 2 + 1] = 0;
        }

        return d;
    }

    public static float[] getFloat(int n) {
        float[] d = new float[n];

        for (int i = 0; i < n; i++) {
            d[i] = (float)Math.sin(i);
        }

        return d;
    }
}
