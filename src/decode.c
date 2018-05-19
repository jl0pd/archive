#include "decode.h"

void decode_file(char *in_str, char *out_str)
{
    FILE *in = fopen(in_str, "rb");
    if (in == NULL){
        printf("no input file %s\n", in_str);
        return;
    } else {
        printf("opened file %s\n", in_str);
    }

    FILE *out = fopen(out_str, "w+");
    if (out == NULL){
        printf("something wrong with %s\n", out_str);
        return;
    } else {
        printf("created file %s\n", out_str);
    }

    FileHeader *header = read_file_header(in);

    printf("file size %u Bytes\n", header->file_size);
    printf("char count %d\n", header->chr_count);

    convert_symbol_data(header);
    printf("data converted\n");

    finaly_decode_file(in, header, out);
}

void print_file_header_decode(FileHeader *header)
{
    for (uchar_t i = 0; i < CHAR_COUNT; i++){
        if(header->symbols[i].bit_code != 0){
            printf("header->symbols[%d].sym = %d '%c'\n",
                i, header->symbols[i].sym, header->symbols[i].sym);
            printf("header->symbols[%d].bit_code = %02X\n",
                i, header->symbols[i].bit_code);
            printf("header->symbols[%d].sign = %02X\n",
                i, header->symbols[i].sign);
            printf("header->symbols[%d].code_leng = %02X\n",
                i, header->symbols[i].code_leng);
            printf("header->symbols[%d].bit_offset = %02X\n",
                i, header->symbols[i].bit_offset);
    
            putchar('\n');
        }
    }
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
        
            header->symbols[i].code_leng = header->symbols[i].bit_code % 16;
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
    printf("finaly_decode_file\n");

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


    uchar_t flag = 1;

    while(flag){

        if (flag && input_convey->convey_cur_len < input_convey->convey_max_len * 7){

            read_from_file(input_convey, in, header, &flag);

        }

        printf("\x1b[32;1minput_convey\x1b[0m\n");
        print_convey(input_convey);

        while(output_convey->convey_cur_len < output_convey->convey_max_len * 8
            && input_convey->convey_cur_len > 0){

            interprate(input_convey, header, output_convey);

        }

        while(output_convey->convey_cur_len / 8 >= 1){

            fputc((char)output_convey->u[0], out);

            output_convey->convey_cur_len -= 8;

            convey_next_byte(output_convey);

        }
    }
}

void interprate(Conveyor *in, FileHeader *header, Conveyor *out)
{
    char tmp;
    uchar_t code_len;

     /*
    while (in->s_bit > 0){
        code_len = 1;

        printf("in->u[0]: %02X\n", in->u[0]);

        in->s_bit[0]--;

        while ((in->u[code_len / 8] & 0x01) != 1
            && in->s_bit[0] > 0
            && code_len < header->max_code_len){

            code_len++;
            in->u[0] >>= 1;
            in->s_bit[0]--;

            // if (in->s_bit == 0){
            //     0++;
            //     in->convey_cur_len -= 8;
            // }
        }

        tmp = search_char_by_code_len_and_sign(code_len, (in->u[0]) & 0x01, header);

        in->u[0] >>= 1;

        printf("%d '%c'\n", tmp, tmp);

        printf("0: %d\n", 0);
        printf("in->u[0]: %02X\n", in->u[0]);
        printf("code len: %d\n", code_len);
        printf("in->s_bit[0]: %d\n\n\n", in->s_bit[0]);

        out->u[out->convey_cur_len / 8] = tmp;

        out->convey_cur_len += 8;

        if(out->convey_cur_len == 64){
            return;
        }
    }
     */

    if(in->s_bit[0] <= 0){
        convey_next_byte(in);
    }

    while(in->s_bit[0] > 0
    && in->convey_cur_len < in->convey_max_len * 8
    && out->convey_cur_len < out->convey_max_len * 8){

        code_len = 1;
        in->s_bit[0]--;
        in->convey_cur_len--;

        while((in->u[0] & 0x01) != 1
        && code_len < header->max_code_len
        && in->s_bit[0] > 0){

            if (in->s_bit[0] <= 0
                && code_len < header->max_code_len
                && (in->u[0] & 0x01) != 1
                && in->convey_cur_len > 0){

                convey_next_byte(in);
                in->s_bit[0]--;
                in->convey_cur_len--;
                in->u[0] >>= 1;
            
            } else {

                code_len++;
                in->convey_cur_len--;
                in->u[0] >>= 1;
                in->s_bit[0]--;

            }

        }


        tmp = search_char_by_code_len_and_sign(code_len, (in->u[0]) & 0x01, header);


        if (in->s_bit[0] == 0
        && code_len < header->max_code_len
        && in->convey_cur_len > 0){
            convey_next_byte(in);
        } else {
            in->u[0] >>= 1;
        }

        out->u[out->convey_cur_len / 8] = tmp;

        out->convey_cur_len += 8;

        printf("\x1b[32;1minput_convey\x1b[0m\n");
        print_convey(in);


        printf("\x1b[31;1moutput_convey\x1b[0m\n");
        print_convey(out);

        putchar('\n');

        if(out->convey_cur_len == out->convey_max_len * 8){
            return;
        }
    }

    return;
}

void read_from_file(Conveyor *input_convey, FILE *in, FileHeader *header, uchar_t *flag)
{
    printf("start reading: %lu\n", ftell(in));

    while(input_convey->convey_cur_len < input_convey->convey_max_len * 7){
        if (ftell(in) == header->file_size - 2){

            input_convey->u[input_convey->convey_cur_len / 8] = getc(in);
            input_convey->s_bit[input_convey->convey_cur_len / 8] = (uchar_t)getc(in) % 9;
            input_convey->convey_cur_len += input_convey->s_bit[input_convey->convey_cur_len / 8];

            *flag = 0;
            break;

        } else {

            input_convey->u[input_convey->convey_cur_len / 8] = getc(in);
            input_convey->s_bit[input_convey->convey_cur_len / 8] = 8;
            input_convey->convey_cur_len += 8;

        }
    }

    printf("readed: %lu\n", ftell(in));
}

char search_char_by_code_len_and_sign(uchar_t code_len, uchar_t sign, FileHeader *header)
{
    for (uchar_t i = 0; i < CHAR_COUNT; i++){
        if(header->symbols[i].code_leng == code_len){
            if(header->symbols[i].sign == sign){
                return header->symbols[i].sym;
            }
        }
    }

    return -1;
}
