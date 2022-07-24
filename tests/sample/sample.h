#pragma once

template <typename T>
struct Sample {
    T* data;
    std::uint32_t length;
    std::uint32_t fs;

    struct NoisePower {
        uint32_t index;
        T rms;
    };

    Sample(T* _data, std::uint32_t _length, std::uint32_t _fs) : 
                    data{_data}, 
                    length{_length}, 
                    fs{_fs} {}


    uint32_t freqToIndex(uint32_t freq) {
        return (uint32_t)std::round((double)freq / fs * length);
    }

    uint32_t indexToFreq(uint32_t index) {
        return (uint32_t)std::round((double)index / length * fs);
    }

    NoisePower getNoisePower(uint32_t index0, uint32_t index1) {
        NoisePower noisePower = {0, 0};

        T max = 0;
        for (uint32_t i = index0; i <= index1; i++) {
            noisePower.rms += data[i] * data[i];
            if (max < data[i]) {
                max = data[i];
                noisePower.index = i;
            }
        }
        noisePower.rms = sqrt(noisePower.rms);
        return noisePower;
    }

    void fft() {
        std::complex<T> * in = new std::complex<T>[length]; 
        std::complex<T> * out = new std::complex<T>[length]; 

        // convert data to complex
        for (std::uint32_t i = 0; i < length; i++) {
            in[i] = data[i];
        }
        fft_iter(in, out, 1, length);

        // get abs
        for (size_t i = 0; i < length; i++) {
            data[i] = std::abs(out[i]);
        }

        delete[] in;
        delete[] out;
    }
    
    void normalize() {
        for (uint32_t i = 0; i < length; i++) {
            data[i] /= length;
        }
    }
private:

    void fft_iter(std::complex<T> * in, std::complex<T> * out, int stride, int n) {
        if (n == 2) { 
            out[0] = in[0] + in[stride];
            out[1] = in[0] - in[stride];
        } else {
            fft_iter(in, out, stride << 1, n >> 1);
            fft_iter(in + stride, out + n / 2, stride << 1, n >> 1);

            std::complex<T> even = out[0];
            std::complex<T> odd = out[n / 2];
            out[0] = even + odd;
            out[n / 2] = even - odd;

            for (int k = 1; k < n / 2; k++) {
                even = out[k];
                odd = out[k + n / 2];
                
                using namespace std;
                std::complex<T> w = std::exp((T)(-2.0 * M_PI) * (std::complex<T>)1i * (T)k / (T)n);
                out[k] = even + w * odd;
                out[k + n / 2] = even - w * odd;
            }
        }
    }
};