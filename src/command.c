#include "command.h"

void encode_file(char* in, char* out)
{
    FILE *input = open_to_encode(in);
    FILE *output = make_encoded_file(out);

    srand(time(NULL));
    struct Frequency_tree *freq_tree = (struct Frequency_tree*)malloc(sizeof(struct Frequency_tree) * CHAR_COUNT);

    freq_tree_init(freq_tree);

    while (!feof(input)){
       freq_tree[getc(input)].count++;
    }

    sort_freq_tree(freq_tree);
    assign_code(freq_tree);


    put_encoded(freq, input);
}

void put_encoded(struct Frequency_tree* freq, FILE* input)
{
    rewind(input);

    struct Code code;
    while(!feof(input)){
        code = make_8byte_code(freq);
        for(uchar_t i = 0; i < 8; i++){
            fputc(code[i], output);
        }
    }
}

struct Code make_8byte_code(struct Frequency_tree* freq, FILE* input)
{
    struct Code code;
    for (uchar_t i = 0; i < 8; i++)
        code[i] = 0;

    uchar_t leng = 0;
    uchar_t index;

    char tmp;
    while(leng % 8 != 0){
        tmp = getc(input);
        index = search_char(freq, tmp);
        leng += freq[index].code_len;
        code[leng / 8] |= freq[index].code
    
    }

    return code;
}

uchar_t search_char(struct Frequency_tree* freq, char chr)
{
    for(uchar_t i = CHAR_COUNT - 1; i >= 0; i--){
        if(freq[i].sym == chr)
            return i;
    }
    return 0;
}

void freq_tree_init(struct Frequency_tree* freq)
{
    for (uchar_t i = 0; i < CHAR_COUNT; i++){
        freq[i].sym = i;
        freq[i].code_len = 0;
        freq[i].count = 0;
        for(uchar_t j = 0; j < 8; j++)
            freq[i].code[j] = 0;
    }
}

void assign_code(struct Frequency_tree* freq)
{
    uchar_t i = 0;
    while(freq[i+1].count > 0){
        freq[i].code[7] = 1;
        freq[i].code_len = i + 1;
        i++;
    }
    freq[i].code[7] = 1;
    freq[i].code_len = i;
}

void sort_freq_tree(struct Frequency_tree* freq)
{
    sort(freq, 128);
}

void sort(struct Frequency_tree* arr, int count)
{
    uchar_t i = 0;
    while(i++ < CHAR_COUNT){
        if(arr[i].count > arr[i+1].count){
            swap(&arr[i], &arr[i+1]);
            i = 0;
        }
    }
}

void swap(struct Frequency_tree* p1, struct Frequency_tree* p2)
{
    struct Frequency_tree tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}
