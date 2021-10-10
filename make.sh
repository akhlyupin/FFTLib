gcc -O3 -msse -I"src/" -c -o build/FFTBasic.o src/FFTBasic.c
gcc -O3 -msse -I"src/" -c -o build/FFTLut.o src/FFTLut.c
gcc -O3 -msse -I"src/" -c -o build/FFTSSE.o src/FFTSSE.c
gcc -O3 -msse -I"src/" -c -o build/FFT.o src/FFT.c
gcc -O3 -msse -I"src/" -c -o build/tests/test_equal.o tests/test_equal.c
gcc -O3 -msse -I"src/" -c -o build/tests/tests.o tests/tests.c

ar rcs build/libFFT.lib \
    build/FFT.o \
    build/FFTSSE.o \
    build/FFTLut.o \
    build/FFTBasic.o

gcc -shared -o build/FFT.dll \
    build/FFT.o \
    build/FFTSSE.o \
    build/FFTLut.o \
    build/FFTBasic.o

gcc -O3 -s -msse -o build/tests/test_perfomance \
    build/tests/tests.o \
    build/libFFT.lib

gcc -O3 -s -msse -o build/tests/test_equal \
    build/tests/test_equal.o \
    build/libFFT.lib

gcc -shared -o build/wrapper/java/JFFT.dll \
    -I"C:\Program Files\Java\jdk-15.0.2\include" \
    -I"C:\Program Files\Java\jdk-15.0.2\include\win32" \
    -I"src/" \
    build/FFT.o \
    build/FFTSSE.o \
    build/FFTLut.o \
    build/FFTBasic.o \
    wrapper/java/JFFT.c

rm build/*.o build/tests/*.o

cd wrapper/java
./make.sh