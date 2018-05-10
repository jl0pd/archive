#include <malloc.h>

int slen(char* str);

char** stok(char* str, char delim);
void stok_free(char** arr);
void stok_print(char** arr);

int sspn(char* str, char sym);

int scmp(char* str1, char* str2);

char* scpy(char* str);
