gcc -O3 -s -msse -I"src/" \
    tests/tests.c \
    src/FFTBasic.c \
    src/FFTLut.c \
    src/FFTSSE.c \
    src/FFT.c