#include <stdio.h>

#ifndef ARCH_FILES_CURS_H
#define ARCH_FILES_CURS_H
//decode - расшифровать
//encode - зашифровать

FILE* open_to_decode(char* path);

FILE* open_to_encode(char* path);

FILE* make_coded_file(char* path);

FILE* make_encoded_file(char* path);

#endif