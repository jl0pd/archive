#include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

#include "string.h"
#include "command.h"

int main(int argc, char *argv[])
{
    // int index = 0;
    // int i = 0;
    // char *input;
    // char *output;


    encode_file("input.txt", "output.arch");

/*
    while (++index < argc){
        if (argv[index][0] == '-'){
            if(argv[index][1] == '-'){
                if      (scmp(argv[index], "--decode") == 0){
                    printf("l %s\n", argv[index]);
                
                }else if(scmp(argv[index], "--encode") == 0){
                    printf("l %s\n", argv[index]);
                
                }else if(scmp(argv[index], "--output") == 0){
                    output = argv[++index];
                }else{
                    printf("incorrect long flag\n");
                }
                
            }else{
                if      (argv[index][1] == 'd'){
                    printf("s %s\n", argv[index]);
                
                }else if(argv[index][1] == 'e'){
                    printf("s %s\n", argv[index]);
                
                }else if(argv[index][1] == 'o'){
                    output = argv[--index];
                }else{
                    printf("incorrect short flag\n");
                }
            }
        }else{
            input = argv[index];
        }
    }
*/

}