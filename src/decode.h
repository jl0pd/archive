#ifndef DECODE_CURS_H
#define DECODE_CURS_H

#include <math.h>
#include "command.h"

void decode_file(char *in_str, char *out_str);
void print_file_header_decode(FileHeader *header);
FileHeader* read_file_header(FILE *in);
void convert_symbol_data(FileHeader *header);
void finaly_decode_file(FILE *in, FileHeader *header, FILE *out);
void interprate(Conveyor *in, FileHeader *header, Conveyor *out);
char search_char_by_code_len_and_sign(uchar_t code_len, uchar_t sign, FileHeader *header);
void read_from_file(Conveyor *input_convey, FILE *in, FileHeader *header, uchar_t *flag);

#endif
