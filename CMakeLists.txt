cmake_minimum_required(VERSION 3.12)

# set the project name
project(FFTLib)

# add the static library
add_library(FFTLibStatic STATIC src/FFT.c src/FFTBasic.c src/FFTLut.c src/FFTSSE.c)
target_compile_options(FFTLibStatic PRIVATE -msse -O3 -fPIC)
target_link_options(FFTLibStatic PUBLIC -static -static-libgcc)
set_target_properties(FFTLibStatic PROPERTIES OUTPUT_NAME FFTLib)

# add the shared library
add_library(FFTLib SHARED src/FFT.c src/FFTBasic.c src/FFTLut.c src/FFTSSE.c)
target_compile_options(FFTLib PRIVATE -msse -O3 -fPIC)
target_link_options(FFTLib PUBLIC -static -static-libgcc)

# add java library
find_package(JNI REQUIRED)
message("JNI inv dir: " ${JNI_INCLUDE_DIRS})
message("JNI lib dir: " ${JNI_LIBRARIES})

add_library(JFFT SHARED wrapper/java/JFFT.c)
target_include_directories(JFFT PRIVATE src/ ${JNI_INCLUDE_DIRS})
target_link_options(JFFT PUBLIC -static -static-libgcc)
target_link_libraries(JFFT PUBLIC FFTLibStatic)

# add the tests
add_executable(FFTLibTest tests/tests.c)
target_compile_options(FFTLibTest PRIVATE -msse -O3)

target_include_directories(FFTLibTest PRIVATE src/)

target_link_libraries(FFTLibTest PUBLIC FFTLib m)

