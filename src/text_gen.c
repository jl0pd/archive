#include "command.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    FILE *f = fopen("input.txt", "w+b");


    unsigned long long count = 1024 * 1024 * 100;

    for (unsigned long long i = 0; i < count - 1; i++){
//        fputc(rand() % 127, f);
        fputc('a', f);

    }
    fputc('b', f);

    fclose(f);

}
