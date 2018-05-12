#ifndef COMMAND_CURS_H
#define COMMAND_CURS_H

#include <time.h>
#include <malloc.h>

#include "files.h"

#define CHAR_COUNT 128
#define uchar_t unsigned char
#define uint32_t unsigned int
#define uint64_t unsigned long long

typedef struct {
    uchar_t chr_count;
    uchar_t max_code_len;
    uint32_t file_size;
    struct FileSymCode *symbols;
} FileHeader;

typedef struct FileSymCode{
    char sym;
    uchar_t code_leng; //bit
    uchar_t bit_offset;
    uchar_t sign;
    uchar_t bit_code;
} FileSymCode;

typedef struct {
    short convey_cur_len; //bit
    uchar_t convey_max_len; //byte
    uchar_t u[32];
} Conveyor;

#endif
