#ifndef DECODE_CURS_H
#define DECODE_CURS_H

#include "command.h"

void decode_file(char *in_str, char *out_str);
FileHeader* read_file_header(FILE *in);
void convert_symbol_data(FileHeader *header);
void finaly_decode_file(FILE *in, FileHeader *header, FILE *out);

#endif