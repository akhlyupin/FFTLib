gcc -O3 -s -msse \
    src/main.c \
    src/Complex.c \
    src/FFTCpuRecursive.c \
    src/TestData.c \
    src/FFTCpuIterate.c \
    src/FFT2SSE.c