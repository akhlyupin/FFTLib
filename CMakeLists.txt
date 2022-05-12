cmake_minimum_required(VERSION 3.14)

# set the project name
project(FFTLib)

# add the static library
add_library(FFTLibStatic STATIC src/FFT.c src/FFTBasic.c src/FFTLut.c src/FFTSSE.c)
target_compile_options(FFTLibStatic PRIVATE -msse -O3 -fPIC)
if (MINGW)
        message("MinGw usage")
        target_link_options(FFTLibStatic PUBLIC -static -static-libgcc)
endif()
set_target_properties(FFTLibStatic PROPERTIES OUTPUT_NAME FFTLib)

# add the shared library
add_library(FFTLib SHARED src/FFT.c src/FFTBasic.c src/FFTLut.c src/FFTSSE.c)
target_compile_options(FFTLib PRIVATE -msse -O3 -fPIC)
if (MINGW)
        message("MinGw usage")
        target_link_options(FFTLib PUBLIC -static -static-libgcc)
endif()

# add java library
find_package(JNI REQUIRED)
message("JNI inv dir: " ${JNI_INCLUDE_DIRS})
message("JNI lib dir: " ${JNI_LIBRARIES})

add_library(FFTJavaWrapper SHARED wrapper/java/JFFT.c)
target_include_directories(FFTJavaWrapper PRIVATE src/ ${JNI_INCLUDE_DIRS})
target_link_libraries(FFTJavaWrapper PUBLIC FFTLibStatic)
set_target_properties(FFTJavaWrapper PROPERTIES OUTPUT_NAME JFFT)

# add fft jar
find_package(Java REQUIRED)
include(UseJava)
add_jar(JFFT SOURCES wrapper/java/FFT.java wrapper/java/JFFT.java wrapper/java/SinData.java)

# add fft test jar
add_jar(JFFTTest SOURCES wrapper/test/Test.java 
        INCLUDE_JARS build/JFFT.jar 
        MANIFEST "${CMAKE_CURRENT_SOURCE_DIR}/wrapper/MANIFEST.MF")

# add the tests
add_executable(FFTLibTest tests/tests.c)
target_compile_options(FFTLibTest PRIVATE -msse -O3)

target_include_directories(FFTLibTest PRIVATE src/)

target_link_libraries(FFTLibTest PUBLIC FFTLib m)

