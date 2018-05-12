#ifndef DECODE_CURS_H
#define DECODE_CURS_H

#include <math.h>
#include "command.h"

void decode_file(char *in_str, char *out_str);
FileHeader* read_file_header(FILE *in);
void convert_symbol_data(FileHeader *header);
void finaly_decode_file(FILE *in, FileHeader *header, FILE *out);
void interprate(Conveyor *in, FileHeader *header, Conveyor *out);
uchar_t is_pow_of_two(uchar_t num);
char search_char_by_code_len_and_sign(uchar_t code_len, uchar_t sign, FileHeader *header);


#endif