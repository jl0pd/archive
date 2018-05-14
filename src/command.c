#include "command.h"

void convey_next_byte(Conveyor *conv)
{
    for (uchar_t i = 0; i < conv->convey_max_len - 1; i++){
        conv->u[i] = conv->u[i + 1];
        conv->s_bit[i] = conv->s_bit[i + 1];
    }

    conv->u[conv->convey_max_len - 1] = 0;
    conv->s_bit[conv->convey_max_len - 1] = 0;

}

void print_convey(Conveyor *conv)
{
    for (uchar_t i = 0; i < conv->convey_max_len; i++){
        printf("%02X ", conv->u[i]);
    }
    putchar('\n');

    for (uchar_t i = 0; i < conv->convey_max_len; i++){
        printf("%02X ", conv->s_bit[i]);
    }
    putchar('\n');

    printf("ccl: %d\n", conv->convey_cur_len);

}
