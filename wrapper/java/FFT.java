/*
 *  FFT.java
 *
 *  Created by Artem Khlyupin on 10/11/2021
 *  Copyright © 2021 Artem Khlyupin. All right reserved.
 */
package com.e1da.fft;

public interface FFT {
    void close() throws Exception;
    void process(float[] in, float[] out) throws Exception;
    int performance(int n);
}
