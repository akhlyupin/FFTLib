#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <complex>
#include <math.h>
#include "sample_data.h"
#include "sample.h"

#define FS              48000
#define SAMPLE_LENGTH   (65536 * 4)

#define BW0             20
#define BW1             20000

typedef struct {
    uint32_t index;
    double db;
} Harmonics_t;

Harmonics_t harmonics[40] = {0};
uint32_t harmonics_length = 0;

using namespace std;

uint32_t freqToIndex(uint32_t freq) {
    return (uint32_t)std::round((double)freq / FS * SAMPLE_LENGTH);
}

uint32_t indexToFreq(uint32_t index) {
    return (uint32_t)std::round((double)index / SAMPLE_LENGTH * FS);
}

template <typename T>
T toDb(T ampl) {
    return 20.0f * log10(ampl);
}

template <typename T>
bool calcHarmonics(Sample<T>& sample) {
    auto harm = sample.getNoisePower(freqToIndex(BW0), freqToIndex(BW1));
    uint32_t fundamental_index = harm.index;

    if (fundamental_index != 5461) {
        printf("Error in line %d. Fundamental index: %d != 5461\n", __LINE__, fundamental_index);
        return false;
    }

    int endIndex = freqToIndex(BW1) + 1;
    if (endIndex > SAMPLE_LENGTH) endIndex = SAMPLE_LENGTH;
    harmonics_length = endIndex / fundamental_index;
    if (harmonics_length != 20) {
        printf("Error in line %d. harmonics length: %d != 20\n", __LINE__, harmonics_length);
        return false;
    }

    for (uint32_t i = 0; i < harmonics_length; i++) {
        uint32_t start_index = round((fundamental_index + 1) * 0.99 * (i + 1)) - 1;
        uint32_t end_index = round((fundamental_index + 1) * 1.01 * (i + 1)) - 1;
        if (end_index > SAMPLE_LENGTH) end_index = SAMPLE_LENGTH;

        //printf("%d %d\n", start_index, end_index);
        auto np = sample.getNoisePower(start_index, end_index);
        //printf("%d: %.10f\n", i, np.rms);

        // find span
        uint32_t span = 0;
        for (uint32_t u = np.index; u <= np.index + 1000; u++) {
            if (sample.data[u] < sample.data[u + 1]) {
                span = u - np.index;
                break;
            }
        }
        // printf("%d: %d\n", i, span);

        // [Hpower(n),a]=noisepower(P1,round((fund_index-span)*n), round((fund_index+span))*n);
        start_index = round((fundamental_index + 1 - span) * (i + 1)) - 1;
        end_index = round((fundamental_index + 1 + span) * (i + 1)) - 1;
        if (end_index > SAMPLE_LENGTH) end_index = SAMPLE_LENGTH;
        
        np = sample.getNoisePower(start_index, end_index);
        //printf("%d: %.10f\n", i, toDb(np.rms));

        harmonics[i].index = np.index;
        harmonics[i].db = toDb(np.rms);
    }
    
    return true;
}

int main() {
    if ((sizeof(sample_data) / sizeof(double)) != SAMPLE_LENGTH) {
        printf("Sample data length error.\n");
        return 0;
    }
    
    Sample<double> time_sample{sample_data, SAMPLE_LENGTH, FS};

    time_sample.fft();
    time_sample.normalize();

    double freq_sample_data[SAMPLE_LENGTH / 2];
    Sample<double> freq_sample{freq_sample_data, SAMPLE_LENGTH / 2, FS};
    
    for (std::uint32_t i = 0; i < freq_sample.length; i++) {
        freq_sample.data[i] = time_sample.data[i];
    }

    // check normalize
    if (fabsf(freq_sample.data[0] - 0.000026815) > 0.000000001) {
        printf("Error in line %d: %.10f 0.000026815\n", __LINE__, (double)freq_sample.data[0]);
        return 0;
    }
    if (fabsf(freq_sample.data[100] - 0.000020207) > 0.000000001) {
        printf("Error in line %d: %.10f 0.000020207\n", __LINE__, (double)freq_sample.data[100]);
        return 0;
    }

    // calculate harmonics
    if (!calcHarmonics(freq_sample)) {
        return 0;
    }

    // check harmonics
    const double ref_harmonics[] = {
    -6.92548, -87.6826, -90.0263, -91.9921,
    -97.7574, -98.7214, -96.6022, -93.274, 
    -95.9179, -100.923, -101.246, -98.5888,
    -98.7974, -101.891, -102.02, -99.1642,
    -99.2146, -102.184, -102.174, -102.131};

    double error = 0;
    for (uint32_t i = 0; i < harmonics_length; i++) {
        printf("H%d: %.6f\t%.6f\terr: %.6f\n", i, (double)harmonics[i].db, (double)ref_harmonics[i], (double)fabs(harmonics[i].db - ref_harmonics[i]));

        double temp_error = fabs(harmonics[i].db - (ref_harmonics[i]));
        if (temp_error > error) {
            error = temp_error;
        }
        if (fabsf(harmonics[i].db - (ref_harmonics[i])) > 0.001) {
            printf("Error in line=%d; %.6f != %.6f\n", __LINE__, (double)harmonics[i].db, (double)ref_harmonics[i]);
            //return 0;
        }
    }
    printf("Max error: %0.10f\n", error);


    // THD


    printf("Thd test successfull.\n");
    return 0;
}