#ifndef COMMAND_CURS_H
#define COMMAND_CURS_H

#include "files.h"

#define CHAR_COUNT 128
#define uchar_t unsigned char
#define uint32_t unsigned int
#define uint64_t unsigned long long

typedef struct {
    uchar_t chr_count;
    uint32_t file_size;
    struct FileSymCode *symbols;
} FileHeader;

typedef struct FileSymCode{
    char sym;
    uchar_t code_leng; //bit
    uchar_t bit_offset; //bit
    uchar_t sign;
} FileSymCode;

#endif