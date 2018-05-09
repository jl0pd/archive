#include "command.h"

void encode_file(char* in, char* out)
{
    FILE *input = open_to_encode(in);
    FILE *output = make_encoded_file(out);

    srand(time(NULL));
    Frequency_tree *freq_tree = 
        (Frequency_tree*)malloc(sizeof(Frequency_tree) * CHAR_COUNT);

    freq_tree_init(freq_tree);

    while (!feof(input)){
       freq_tree[getc(input)].count++;
    }

    sort_freq_tree(freq_tree);
    assign_code(freq_tree);

    put_encoded(freq_tree, input, output);
}

void put_encoded(Frequency_tree *freq, FILE *input, FILE *output)
{
    rewind(input);

    Conveyor *bit_stream = (Conveyor*)malloc(sizeof(Conveyor*));

    bit_stream->convey_len = 32;
    bit_stream->convey_cur_len = 0;
    for(uchar_t i = 0; i < bit_stream->convey_len; i++){
        bit_stream->u[i] = 0;
    }
    
    while(!feof(input)){
        convey(bit_stream, freq, input);

        for(uchar_t i = 0; i < bit_stream->convey_len / 2; i++){
            fputc(bit_stream->u[0], output);

            for(uchar_t j = 0; j < bit_stream->convey_len; j++){
                bit_stream->u[j] = bit_stream->u[j+1];
            }

            bit_stream->u[bit_stream->convey_len - 1] = 0;
            bit_stream->convey_cur_len -= 8;
        }
    }
    free(bit_stream);
}

void convey(Conveyor *bit_stream, Frequency_tree *freq,FILE *input)
{
    uchar_t index;

    uchar_t arpos, subarpos;

    char tmp;

    while(bit_stream->convey_cur_len < bit_stream->convey_len * 4){
        index = search_char(freq, getc(input));

        bit_stream->u[bit_stream->convey_cur_len / 8] 
            |= (freq[index].code << subarpos = bit_stream->convey_cur_len % 8);

        bit_stream->convey_cur_len += freq[index].code_len;
    }
}

uchar_t search_char(Frequency_tree* freq, char chr)
{
    for(uchar_t i = CHAR_COUNT - 1; i >= 0; i--){
        if(freq[i].sym == chr)
            return i;
    }
    return 0;
}

void freq_tree_init(Frequency_tree* freq)
{
    for (uchar_t i = 0; i < CHAR_COUNT; i++){
        freq[i].sym = i;
        freq[i].code_len = 0;
        freq[i].count = 0;
        for(uchar_t j = 0; j < 8; j++)
            freq[i].code[j] = 0;
    }
}

void assign_code(Frequency_tree* freq)
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

void sort_freq_tree(Frequency_tree* freq)
{
    sort(freq, 128);
}

void sort(Frequency_tree* arr, uchar_t count)
{
    uchar_t i = 0;
    while(i++ < CHAR_COUNT){
        if(arr[i].count > arr[i+1].count){
            swap(&arr[i], &arr[i+1]);
            i = 0;
        }
    }
}

void swap(Frequency_tree* p1, Frequency_tree* p2)
{
    Frequency_tree tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}
