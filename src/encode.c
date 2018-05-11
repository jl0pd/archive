#include "encode.h"

void encode_file(char* in, char* out)
{
    FILE *input = open_to_encode(in);
    if (input == NULL){
        printf("no input file\n");
        return;
    } else {
        printf("opened file %s\n", in);
    }

    FILE *output = make_encoded_file(out);
    if (input == NULL){
        printf("something wrong with output file\n");
        return;
    } else {
        printf("created file %s\n", out);
    }

    Frequency_tree *freq_tree = 
        (Frequency_tree*)malloc(sizeof(Frequency_tree) * CHAR_COUNT);

    freq_tree_init(freq_tree);
    printf("frequency tree initialised\n");

    rewind(input);

    double time_start = clock();

    char index;
    while (1){
        index = getc(input);
        if (index == EOF){
            break;
        }
    
        freq_tree[(int)index].count++;
    }

    double time_end = clock();
    printf("symbol counted, time elapsed: %fs\n", (time_end - time_start) / CLOCKS_PER_SEC);

    sort_freq_tree(freq_tree);
    printf("frequency tree sorted\n");

    assign_code(freq_tree);
    printf("code assigned\n");

    time_start = clock();
    put_encoded(freq_tree, input, output);
    time_end = clock();
    printf("archieved, time elapsed: %fs\n", (time_end - time_start) / CLOCKS_PER_SEC);
    printf("done\n");

    fclose(input);
    fclose(output);
}

void print_freq_tree(Frequency_tree *freq_tree)
{
    for (int i = 0; i < CHAR_COUNT; i++){
        printf("freq[%d].count = %d\n", i, freq_tree[i].count);
        printf("freq[%d].symbol.bit_offset = %d\n", i, freq_tree[i].symbol.bit_offset);
        printf("freq[%d].symbol.code_leng = %d\n", i, freq_tree[i].symbol.code_leng);
        printf("freq[%d].symbol.sign = %d\n", i, freq_tree[i].symbol.sign);
        printf("freq[%d].symbol.sym = %d '%c'\n", i, freq_tree[i].symbol.sym, freq_tree[i].symbol.sym);
        putchar('\n');
    }
}

void put_encoded(Frequency_tree *freq, FILE *input, FILE *output)
{
    rewind(input);

    Conveyor *bit_stream = (Conveyor*)malloc(sizeof(Conveyor));
    if (bit_stream == NULL){
        printf("no mem for conveyor\n");
        return;
    }

    bit_stream->convey_max_len = 32;
    bit_stream->convey_cur_len = 0;

    uchar_t i;
    for(i = 0; i < bit_stream->convey_max_len - 1; i++){
        bit_stream->u[i] = 0;
    }

    FileHeader *header = put_file_header(freq, output);
    printf("header putted\n");

    printf("archivating...\n");
    
    while(!feof(input)){
        convey(bit_stream, freq, input);

        for(short i = 0; bit_stream->convey_cur_len > 0; i++){
            fputc(bit_stream->u[0], output);
            
            for(uchar_t j = 0; j < bit_stream->convey_max_len; j++){
                bit_stream->u[j] = bit_stream->u[j+1];
            }

            bit_stream->u[bit_stream->convey_max_len - 1] = 0;
            bit_stream->convey_cur_len -= 8;

            header->file_size++;
        }
    }

    printf("file size: %d\n", header->file_size);
    printf("symbol count: %d\n", header->chr_count);

    rewind(output);
    fwrite(&header->file_size, sizeof(uint32_t), 1, output);

    free(bit_stream);
    free(header);
}

void convey(Conveyor *bit_stream, Frequency_tree *freq, FILE *input)
{
    uchar_t index;

    while(bit_stream->convey_cur_len < bit_stream->convey_max_len * 4){
        index = getc(input);
        if ((index < 0) || (index > 127)){
            bit_stream->convey_cur_len += (8 * 16) - bit_stream->convey_cur_len;
            bit_stream->u[15] =
                (bit_stream->convey_cur_len % 8) |
                ((15 - (bit_stream->convey_cur_len / 8)) << 4);
            
            return;
        }
        index = search_char(freq, index);

        bit_stream->convey_cur_len += freq[index].symbol.code_leng;

        bit_stream->u[bit_stream->convey_cur_len / 8] |=
            1 << 
            ((bit_stream->convey_cur_len % 8
                + (freq[index].symbol.code_leng - 1) % 8) % 8);

    }
}

FileHeader* put_file_header(Frequency_tree *freq, FILE *output)
{
    rewind(output);

    FileHeader *header = (FileHeader*)malloc(sizeof(FileHeader*));

    header->file_size = 0;
    header->chr_count = 0;

    uchar_t index = CHAR_COUNT - 1;
    while(freq[index].count > 0){
        fputc(freq[index].symbol.sym, output);
        fputc(freq[index].symbol.sign << 7 
            | freq[index].symbol.bit_offset << 4
            | freq[index].symbol.code_leng,
            output);

        index--;
        header->file_size += 2;
        header->chr_count++;
    }

    return header;
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
    for (uchar_t i = 0; i < CHAR_COUNT + 1; i++){
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

    while(freq[CHAR_COUNT - i - 2].count > 0){
        freq[CHAR_COUNT - i - 1].symbol.sign = 1;
        freq[CHAR_COUNT - i - 1].symbol.code_leng = i + 1;
        freq[CHAR_COUNT - i - 1].symbol.bit_offset = i % 8;
        
        i++;
    }
    freq[CHAR_COUNT - i - 1].symbol.sign = 0;
    freq[CHAR_COUNT - i - 1].symbol.code_leng = i;
    freq[CHAR_COUNT - i - 1].symbol.bit_offset = i % 8;
}

void sort_freq_tree(Frequency_tree* freq)
{
    sort(freq, CHAR_COUNT);
}

void sort(Frequency_tree* arr, uchar_t count)
{
    uchar_t i = 0;
    while(i < CHAR_COUNT - 1){
        if(arr[i].count > arr[i+1].count){
            swap(&arr[i], &arr[i+1]);
            i = 0;
        }
        i++;
    }
}

void swap(Frequency_tree* p1, Frequency_tree* p2)
{
    Frequency_tree tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}
