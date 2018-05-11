#include <malloc.h>
#include <time.h>

#include "command.h"

typedef struct {
    int count;
    FileSymCode symbol;
} Frequency_tree;

typedef struct {
    short convey_cur_len; //bit
    uchar_t convey_max_len; //byte
    uchar_t u[32];
} Conveyor;

void encode_file(char* in, char* out);
void print_freq_tree(Frequency_tree *freq_tree);
void put_encoded(Frequency_tree *freq, FILE *input, FILE *output);
FileHeader* put_file_header(Frequency_tree *freq, FILE *output);
void convey(Conveyor *bit_stream, Frequency_tree *freq,FILE *input);
uchar_t search_char(Frequency_tree* freq, char chr);
void freq_tree_init(Frequency_tree* freq);
void assign_code(Frequency_tree* freq);
void sort_freq_tree(Frequency_tree* freq);
void sort(Frequency_tree* arr, uchar_t count);
void swap(Frequency_tree* p1, Frequency_tree* p2);