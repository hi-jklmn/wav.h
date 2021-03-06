#include <assert.h>

#include "wav.h"

#define TEST(expr) printf("[%s] " #expr "\n", expr ? "PASSED" : "FAILED");

#define TEST_FILE "/usr/share/sounds/alsa/Noise.wav"

int main() {
    FILE *file;

    printf("-- TESTING: Loading Noise.wav ----------------------------\n");

    TEST(sizeof(wav_header) == 44);

    file = fopen(TEST_FILE, "rb");

    wav_fmt_desc fmt_desc;
    void *data = wav_load_data(file, &fmt_desc);

    TEST(fmt_desc.sample_rate == 48000);

    fclose(file);

    printf("-- TESTING: Writing out.wav ----------------------------\n");

    file = fopen("./out.wav", "wb");

    wav_write_data(file, fmt_desc, data);

    fclose(file);

    TEST(system("diff " TEST_FILE " out.wav") == 0);

    return 0;
}
