gcc -O3 -s -msse \
    src/main.c \
    src/Complex.c \
    src/FFTBasic.c \
    src/TestData.c \
    src/FFTLut.c \
    src/FFT2SSE.c