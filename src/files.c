#include "files.h"

FILE* open_to_decode(char* path)
{
    FILE *f = fopen(path, "rb");
    return f;
}

FILE* open_to_encode(char* path)
{
    FILE *f = fopen(path, "r");
    return f;
}

FILE* make_decoded_file(char* path)
{
    FILE *f = fopen(path, "w+b");
    return f;
}

FILE* make_encoded_file(char* path)
{
    FILE *f = fopen(path, "w+");
    return f;
}