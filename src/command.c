#include "command.h"

void encode_file(char* in, char* out)
{
    FILE *input = open_to_encode(in);
    FILE *output = make_encoded_file(out);

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

    uint64_t file_size_byte = 0;

    Conveyor *bit_stream = (Conveyor*)malloc(sizeof(Conveyor*));

    bit_stream->convey_max_len = 32;
    bit_stream->convey_cur_len = 0;
    for(uchar_t i = 0; i < bit_stream->convey_max_len; i++){
        bit_stream->u[i] = 0;
    }

    FileHeader *header = put_file_header(freq, output);
    
    while(!feof(input)){
        convey(bit_stream, freq, input);

        for(uchar_t i = 0; i < bit_stream->convey_max_len / 2; i++){
            fputc(bit_stream->u[0], output);
            file_size_byte++;

            for(uchar_t j = 0; j < bit_stream->convey_max_len; j++){
                bit_stream->u[j] = bit_stream->u[j+1];
            }

            bit_stream->u[bit_stream->convey_max_len - 1] = 0;
            bit_stream->convey_cur_len -= 8;

            header->file_size++;
        }
    }
    free(bit_stream);
}

FileHeader* put_file_header(Frequency_tree *freq, FILE *output)
{
    rewind(output);

    FileHeader *header = (FileHeader*)malloc(sizeof(FileHeader));

    header->file_size = 0;
    header->chr_count = 0;

    uchar_t index = CHAR_COUNT - 1;

    for(uchar_t ind = 0; ind < CHAR_COUNT; ind++){
        if(freq[ind].count > 0){
            header->chr_count++;
        } else {
            break;
        }
    }
    fputc(header->chr_count, output);

    while(freq[index].count > 0){
        fputc(freq[index].symbol.sym, output);
        fputc(freq[index].symbol.sign << 7 
            | freq[index].symbol.bit_offset << 4
            | freq[index].symbol.code_leng,
            output);

        index--;
    }

    return header;
}

void convey(Conveyor *bit_stream, Frequency_tree *freq, FILE *input)
{
    uchar_t index;

    uchar_t arpos, subarpos;


    while(bit_stream->convey_cur_len < bit_stream->convey_max_len * 4){
        index = search_char(freq, getc(input));

        arpos = bit_stream->convey_cur_len / 8;
        subarpos = bit_stream->convey_cur_len % 8;

        bit_stream->u[arpos] |= freq[index].symbol.sign << subarpos;

        bit_stream->convey_cur_len += freq[index].symbol.code_leng;
    }
}

uchar_t search_char(Frequency_tree* freq, char chr)
{
    for(uchar_t i = CHAR_COUNT - 1; i >= 0; i--){
        if(freq[i].symbol.sym == chr)
            return i;
    }
    return 0;
}

void freq_tree_init(Frequency_tree* freq)
{
    for (uchar_t i = 0; i < CHAR_COUNT; i++){
        freq[i].count = 0;
        freq[i].symbol.sym = i;
        freq[i].symbol.code_leng = 0;
        freq[i].symbol.bit_offset = 0;
        freq[i].symbol.sign = 0;

    }
}

void assign_code(Frequency_tree* freq)
{
    uchar_t i = 0;

    while(freq[CHAR_COUNT - i].count > 0){
        freq[CHAR_COUNT - i].symbol.sign = 1;
        freq[CHAR_COUNT - i].symbol.code_leng = i + 1;
        freq[CHAR_COUNT - i].symbol.bit_offset = 0;
        
        i++;
    }
    freq[CHAR_COUNT - i].symbol.sign = 0;
    freq[CHAR_COUNT - i].symbol.code_leng = i;
    freq[CHAR_COUNT - i].symbol.bit_offset = 0;
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
