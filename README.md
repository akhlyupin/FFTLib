# FFTLib

FFTLib is fast fourier transform library on C lang with Java wrapper. 

Uses SIMD processor instructions:
- SSE
- ~~AVX~~
- ~~AVX512~~


___

## Build:

### Windows

Need CMake, MinGW.

```
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=mingw32-make
cmake --build .
```

### GNU/Linux & macOS

Need CMake.

```
mkdir build
cd build
cmake .. 
cmake --build .
```
## Run tests:

### C

```
cd build
./FFTLibTest
```

### Java

```
cd build
java -Djava.library.path="." -jar JFFTTest.jar
```