# FFTLib

FTLib is fast fourier transform library on C lang with Java wrapper. Uses SSE instructions. In future i plan use AVX, AVX512, FMA instructions.

I develope this library for E1DA CosmosADC analyzer tool. Not found simple and fast fft library.
The project goal is to convert a buffer with a length of 8 million less than 200-300ms on an average 5-7 year old computer
