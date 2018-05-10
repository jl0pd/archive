#include "command.h"

int main()
{
    FILE *f = fopen("input.txt", "w+");

    fwrite("test string with aaaaaaaaaaaa", sizeof(char), 30, f);

    fclose(f);

    return 0;    
}