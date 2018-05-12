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
    printf("data converted\n");

    // finaly_decode_file(in, header, out);
}

FileHeader* read_file_header(FILE *in)
{
    FileHeader *header = (FileHeader*)malloc(sizeof(FileHeader));

    header->file_size = (uint32_t)(getc(in) | (getc(in) << 8)
        | (getc(in) << 16) | (getc(in) << 24));

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
    header->max_code_len = 0;
    for (uchar_t i = 0; i < CHAR_COUNT; i++){
        if(header->symbols[i].bit_code != 0){
            header->symbols[i].bit_code = header->symbols[i].bit_code % 16;
            header->symbols[i].bit_offset = (header->symbols[i].bit_code >> 4) % 8;
            header->symbols[i].sign = header->symbols[i].bit_code >> 7;
            if (header->symbols[i].bit_offset > header->max_code_len){
                header->max_code_len = header->symbols[i].bit_offset;
            }
        } else {
            continue;
        }
    }
}

void finaly_decode_file(FILE *in, FileHeader *header, FILE *out)
{

    Conveyor *input_convey = (Conveyor*)malloc(sizeof(Conveyor) * 1);
    input_convey->convey_cur_len = 0;
    input_convey->convey_max_len = 32;
    for (uchar_t i = 0; i < input_convey->convey_max_len; i++){
        input_convey->u[i] = 0;
    }

    Conveyor *output_convey = (Conveyor*)malloc(sizeof(Conveyor) * 1);
    output_convey->convey_cur_len = 0;
    output_convey->convey_max_len = 32;
    for (uchar_t i = 0; i < output_convey->convey_max_len; i++){
        output_convey->u[i] = 0;
    }


    uchar_t bsindex = 0;
    while(1){
        while(input_convey->convey_cur_len < input_convey->convey_max_len * 8){
            if (ftell(in) == header->file_size){
                return;
            } else {
                input_convey->u[bsindex++] = getc(in);
                input_convey->convey_cur_len += 8;
            }
        }


        while(output_convey->convey_cur_len < input_convey->convey_max_len * 8
        && input_convey->convey_cur_len > 0){
            interprate(input_convey, header, output_convey);
        }


        for (uchar_t i = 0; i < output_convey->convey_cur_len / 8; i++){
            fputc(output_convey->u[0], out);

            for (uchar_t j = 0; j < output_convey->convey_max_len - 1; j++){
                output_convey->u[j] = output_convey->u[j + 1];
            }
        }

    }
}

void interprate(Conveyor *in, FileHeader *header, Conveyor *out)
{
    // char i = 0;
    // while(out->u[(out->convey_cur_len / 8) - 1] % (uchar_t)pow(2, i) == 0){
    //     i++;
    // }

    // i++;

    uchar_t code_len = 0;
    uchar_t offset = 0;

    while (in->u[code_len / 8] % 1 != 1 || offset < header->max_code_len){
        code_len++;
        in->u[0] >>= offset;
        offset++;
    }

    out->u[0] = search_char_by_code_len_and_sign(code_len, in->u[0] % 1, header);

    out->convey_cur_len += 8;
    in->convey_cur_len -= 8;
}

uchar_t is_pow_of_two(uchar_t num)
{
    uchar_t powers[9] = {0, 1, 2, 4, 8, 16, 32, 64, 128};

    for (uchar_t i = 0; i < 9; i++){
        if (num == powers[i]){
            return 1;
        }
    }

    return 0;
}

char search_char_by_code_len_and_sign(uchar_t code_len, uchar_t sign, FileHeader *header)
{
    for (uchar_t i = 0; i < CHAR_COUNT; i++){
        if(header->symbols[i].code_leng == code_len){
            if(header->symbols[i].sign == sign){
                return i;
            }
        }
    }

    return -1;
}