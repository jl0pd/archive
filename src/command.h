#include <malloc.h>
#include "files.h"

#define CHAR_COUNT 128
#define uchar_t unsigned char
#define uint32_t unsigned int
#define uint64_t unsigned long long

typedef struct {
    uint32_t file_size;
    uchar_t chr_count;
    struct FileSymCode *symbols;
} FileHeader;

typedef struct FileSymCode{
    char sym;
    uchar_t code_leng;
    uchar_t bit_offset;
    uchar_t sign;
} FileSymCode;

typedef struct {
    int count;
    FileSymCode symbol;
} Frequency_tree;

typedef struct {
    short convey_cur_len;
    uchar_t convey_max_len; //byte
    uchar_t u[32];
} Conveyor;

void encode_file(char* in, char* out);
void put_encoded(Frequency_tree *freq, FILE *input, FILE *output);
FileHeader* put_file_header(Frequency_tree *freq, FILE *output);
void convey(Conveyor *bit_stream, Frequency_tree *freq,FILE *input);
uchar_t search_char(Frequency_tree* freq, char chr);
void freq_tree_init(Frequency_tree* freq);
void assign_code(Frequency_tree* freq);
void sort_freq_tree(Frequency_tree* freq);
void sort(Frequency_tree* arr, uchar_t count);
void swap(Frequency_tree* p1, Frequency_tree* p2);