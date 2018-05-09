#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int index = 0;

    while (++index < argc){
        if (argv[index][0] == '-'){
            if(argv[index][1] == '-'){
                if      (strcmp(argv[index], "--decode") == 0){
                    printf("l %s\n", argv[index]);
                
                }else if(strcmp(argv[index], "--encode") == 0){
                    printf("l %s\n", argv[index]);
                
                }else if(strcmp(argv[index], "--output") == 0){
                    printf("l %s\n", argv[index++]);
                    printf("n %s\n", argv[index]);
                }else{
                    printf("incorrect long flag\n");
                }
                
            }else{
                if      (argv[index][1] == 'd'){
                    printf("s %s\n", argv[index]);
                
                }else if(argv[index][1] == 'e'){
                    printf("s %s\n", argv[index]);
                
                }else if(argv[index][1] == 'o'){
                    printf("s %s\n", argv[index++]);
                    printf("n %s\n", argv[index]);
                }else{
                    printf("incorrect short flag\n");
                }
            }
        }else{
            printf("error\n");
        }
    }
}