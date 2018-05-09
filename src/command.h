#include <stdlib.h>
#include "files.h"

#define CHAR_COUNT 128
#define uchar_t unsigned char
#define uint32_t unsigned int
#define uint64_t unsigned long long

typedef struct {
    char sym;
    uchar_t code_len;
    int count;
    BinCode code;
} Frequency_tree;

typedef struct {
    uchar_t code_len;
    uchar_t code[16];
} BinCode;

typedef struct {
    BinCode *u1;
    BinCode *u2;
} WCode;

typedef struct {
    uchar_t convey_len;
    uchar_t u[32];
} Conveyor;

void sort(Frequency_tree *arr, int count);
void swap(Frequency_tree *p1, Frequency_tree *p2);
void sort_freq_tree(Frequency_tree *freq);

void assign_code(Frequency_tree *freq);
void freq_tree_init(Frequency_tree *freq);
BinCode *make_16byte_code(Frequency_tree *freq, FILE *input);

uchar_t search_char(Frequency_tree *freq, char chr);

void encode_file(char *in, char *out);
uchar_t make_8byte_code(Frequency_tree *freq, FILE *input);
uchar_t search_char(Frequency_tree *freq, char chr);
