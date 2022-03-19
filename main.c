#include "wav.h"
#include <assert.h>

#define TEST(expr) printf("[%s] "#expr"\n", expr ? "PASSED" : "FAILED" );

int main() {

    printf("-- TESTING: Loading Noise.wav ----------------------------\n");

    TEST(sizeof(wav_header) == 44);

    FILE *file = fopen("/usr/share/sounds/alsa/Noise.wav", "r");

    wav_fmt_desc fmt_desc;
    wav_load_data(file, &fmt_desc);

    TEST(fmt_desc.sample_rate == 48000);

    return 0;
}
