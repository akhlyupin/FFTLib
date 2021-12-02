echo "Dont forget update JFFT library in JDK folder"
cd build/wrapper/java

java -Djava.library.path="." com.e1da.fft.Test com.e1da.fft.JFFT