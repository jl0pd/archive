#include <stdlib.h>
#include "files.h"

#define CHAR_COUNT 128
#define uchar_t unsigned char
#define uint32_t unsigned int

typedef struct Frequency_tree{
    char sym;
    uchar_t code_len;
    int count;
    struct Code code;
} freq_tree;

struct Code{
    uchar_t code[8];
};

void sort(struct Frequency_tree* arr, int count);
void swap(struct Frequency_tree* p1, struct Frequency_tree* p2);
void sort_freq_tree(struct Frequency_tree* freq);

void assign_code(struct Frequency_tree* freq);
void freq_tree_init(struct Frequency_tree* freq);
uchar_t make_byte_code(struct Frequency_tree* freq, FILE* input);
uchar_t search_char(struct Frequency_tree* freq, char chr);

void encode_file(char* in, char* out);
uchar_t make_8byte_code(struct Frequency_tree* freq, FILE* input)
uchar_t search_char(struct Frequency_tree* freq, char chr)
