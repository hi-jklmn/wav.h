#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef WAV_H
#define WAV_H

#define WAV_ERROR_CHECK(assert, msg)\
    if(assert){\
        fprintf(stderr,"[ERROR] %s:%s:%d "msg"\n",__FILE__,__func__,__LINE__);\
        goto wav_error;\
    }

// Resources:
// https://www.videoproc.com/resource/wav-file.htm 
// http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html

typedef enum {
    WAVE_FORMAT_PCM        = 0x0001,
    WAVE_FORMAT_IEEE_FLOAT = 0x0003,
    WAVE_FORMAT_ALAW       = 0x0006,
    WAVE_FORMAT_MULAW      = 0x0007,
    WAVE_FORMAT_EXTENSIBLE = 0xFFFE
} wav_format_type;

typedef struct {
    // Always "RIFF"
    char  chunk_id[4];
    // The final file size minus 8 bytes
    uint32_t chunk_size;
    // Always "WAVE"
    char  format[4];
    // Always "fmt "
    char  format_chunk_id[4];
    // Always 16
    uint32_t format_chunk_size;
    // integer value 1 specifies PCM
    uint16_t format_type;
    uint16_t num_channels;
    uint32_t sample_rate;
    // (sample_rate * bits_per_sample * num_channels) / 8
    uint32_t byte_rate;
    // (bytes_per_sample * channels)
    uint16_t block_align;
    uint16_t bits_per_sample;
    // Always "data"
    char  data[4];
    // Number of bytes
    uint32_t data_size;
} wav_header;

typedef struct {
    wav_format_type wav_format;
    uint32_t sample_rate;
    uint16_t num_channels;
    uint32_t num_samples;
    void *data;
} wav_fmt_desc;

void *wav_load_data(FILE *file, wav_fmt_desc *fmt_desc) {
    void *data = NULL;

    WAV_ERROR_CHECK(file == NULL, "file is NULL");

    fseek(file, 0, SEEK_END);
    int64_t f_size = ftell(file);
    rewind(file);

    WAV_ERROR_CHECK(f_size < 44, "invalid file size");

    wav_header header;

    WAV_ERROR_CHECK(fread(&header, 1, 44, file) != 44,
            "could not read WAVE header");

    WAV_ERROR_CHECK(memcmp(header.chunk_id, "RIFF", 4) != 0,
            "RIFF 4CC Chunk ID missing");

    WAV_ERROR_CHECK(f_size - sizeof(wav_header) != header.data_size,
            "header data size does not match file size");

    data = malloc(header.data_size);

    WAV_ERROR_CHECK(fread(data, 1, header.data_size, file) != header.data_size,
            "failure reading data");

    fmt_desc->wav_format   = header.format_type;
    fmt_desc->sample_rate  = header.sample_rate;
    fmt_desc->num_channels = header.num_channels;
    fmt_desc->num_samples  = header.data_size / header.bits_per_sample / 8;

    rewind(file);

    return data;

wav_error:
    if (data != NULL) { free(data); }

    *fmt_desc = (wav_fmt_desc) { 0 };

    rewind(file);

    return NULL;
}

#endif

