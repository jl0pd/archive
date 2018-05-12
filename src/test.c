#include "decode.h"

int main()
{
    char in[] = "output.archive";
    char out[] = "decoded_arch.txt";

    decode_file(in, out);

    return 0;
}