#include <stdio.h>
#include "string.h"
#include "command.h"
#include "encode.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    int index = 0;

    char *action[3] = {0, 0, 0};
/*
    # - mean
    0 - mode (1 - d; 2 - e)
    1 - input (argv_number)
    2 - output (0 - a.archive; argv_number)
*/

    char modes[3][2] = {
        "\0\0",
        "d\0",
        "e\0"
    };

    while (++index < argc){
        if (argv[index][0] == '-'){
            if(argv[index][1] == '-'){
                if (scmp(argv[index], "--decode") == 0){
                    action[0] = modes[1];
                
                }else if(scmp(argv[index], "--encode") == 0){
                    action[0] = modes[2];
                
                }else if(scmp(argv[index], "--output") == 0){
                    action[2] = argv[++index];
                
                }else{
                    printf("incorrect long flag\n");
                }
                
            }else{
                switch (argv[index][1]){
                case 'd':
                    action[0] = modes[1];
                    break;

                case 'e':
                    action[0] = modes[2];
                    break;

                case 'o':
                    action[2] = argv[++index];
                    break;

                default:
                    printf("incorrect short flag\n");
                    break;
                }
            }
        }else{
            action[1] = argv[index];
        }
    }

    if(action[0] == 0){
        printf("-e or --encode to archive\n");
        printf("-d or --decode to extract\n");
        printf("-o or --output to name output file\n");

        return 0;
    }

    if (action[2] == 0){
        switch (*action[0]){
        case 'e':
            action[2] = "a.archive";
            break;
        
        case 'd':
            action[2] = "decoded.txt";
            break;
        }
    }

    switch(*action[0]){
    case 'd':
        decode_file(action[1], action[2]);
        break;
    case 'e':
        encode_file(action[1], action[2]);
        break;
    case '\0':
        printf("no mode\n");
        return 0;
    }

}