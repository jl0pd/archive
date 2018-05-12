#include "decode.h"

void decode_file(char *in_str, char *out_str)
{
    FILE *in = open_to_decode(in_str);
    if (in == NULL){
        printf("no input file %s\n", in_str);
        return;
    } else {
        printf("opened file %s\n", in_str);
    }

    FILE *out = make_decoded_file(out_str);
    if (out == NULL){
        printf("something wrong with %s\n", out_str);
        return;
    } else {
        printf("created file %s\n", out_str);
    }

    FileHeader *header = read_file_header(in);

    printf("file size %u Bytes\n", header->file_size);
    printf("char count %d\n", header->chr_count);
    // for (uchar_t i = 0; i < CHAR_COUNT; i++){
    //     if(header->symbols[i].bit_code != 0){
    //         printf("header->symbols[%d].sym = %d '%c'\n",
    //             i, header->symbols[i].sym, header->symbols[i].sym);
    //         printf("header->symbols[%d].bit_code = %02X\n",
    //             i, header->symbols[i].bit_code);
    //         putchar('\n');
    //     }
    // }

    convert_symbol_data(header);

    // finaly_decode_file(in, header, out);
}

FileHeader* read_file_header(FILE *in)
{
    FileHeader *header = (FileHeader*)malloc(sizeof(FileHeader));

    header->file_size = getc(in) | (getc(in) << 8)
        | (getc(in) << 16) | (getc(in) << 24);

    header->chr_count = getc(in);

    FileSymCode *symbols =
        (FileSymCode*)malloc(sizeof(FileSymCode) * CHAR_COUNT);

    header->symbols = symbols;

    char tmp;
    for (uchar_t i = 0; i < header->chr_count; i++){
        tmp = getc(in);
        header->symbols[(uchar_t)tmp].sym = tmp;
        header->symbols[(uchar_t)tmp].bit_code = getc(in);
    }

    return header;
}

void convert_symbol_data(FileHeader *header)
{
    for (uchar_t i = 0; i < CHAR_COUNT; i++){
        if(header->symbols[i].bit_code != 0){
            header->symbols[i].bit_code = header->symbols[i].bit_code % 16;
            header->symbols[i].bit_offset = (header->symbols[i].bit_code >> 4) % 8;
            header->symbols[i].sign = header->symbols[i].bit_code >> 7;
        } else {
            continue;
        }
    }
}

void finaly_decode_file(FILE *in, FileHeader *header, FILE *out)
{

    Conveyor *bit_stream = (Conveyor*)malloc(sizeof(Conveyor) * 1);
    bit_stream->convey_cur_len = 0;
    bit_stream->convey_max_len = 32;
    for (uchar_t i = 0; i < bit_stream->convey_max_len; i++){
        bit_stream->u[i] = 0;
    }

    uchar_t bsindex = 0;
    while(ftell(in) < header->file_size){
        while(bit_stream->convey_cur_len < bit_stream->convey_max_len * 8){
            bit_stream->u[bsindex++] = getc(in);
            bit_stream->convey_cur_len += 8;
        }


    }
}