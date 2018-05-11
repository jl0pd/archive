#include "command.h"
#include <math.h>

int main()
{
    FILE *f = fopen("input4gb.txt", "w+b");


    unsigned long long count = pow(2, 30);

    for (unsigned long long i = 0; i < count - 1; i++){
        fputc('a', f);
    }

    fclose(f);

}
