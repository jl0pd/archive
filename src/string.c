#include "string.h"

int slen(char* str)
{
    int len = 0;
    while(*str++ != '\0') len++;
    return len;
}

char** stok(char* str, char delim)
{
    int leng = slen(str);
    int i, j, k;
    int arr_count = 0;
    j = k = 0;
    
    for (i = 0; i < leng; i++){
        if (str[i] == delim){
            str[i] = '\0';
            arr_count++;
        }
    }
    arr_count++;

    char **res = (char**)malloc(sizeof(char*) * (arr_count + 1));
    if (res == NULL) return 0;
    res[arr_count] = NULL;

    for (i = 0; i < arr_count; i++){

        res[i] = (char*)malloc(sizeof(char) * (leng + 1));
        
        while(str[j] != '\0'){
            res[i][k] = str[j];
            k++;
            j++;
        }
        j++;

        res[i][k] = '\0';
        res[i] = realloc(res[i], k);
        k = 0;
    }

    return res;
}

void stok_free(char** arr)
{
    int i = 0;
    while(arr[i] != NULL){
        free(arr[i++]);
    }
    free(arr);
}

void stok_print(char** arr)
{
    int i = 0;
    int j = 0;
    while (arr[i] != NULL){
        while (arr[i][j] != '\0'){
            putchar(arr[i][j]);
            j++;
        }
        putchar('\n');
        i++;
        j=0;
    }
}

int sspn(char* str, char sym)
{
    for (int i = 0; i < slen(str); i++){
        if (str[i] == sym){
            return 1;
        }
    }
    return 0;
}

int scmp(char* str1, char* str2)
{
    int i = 0;
    while (str1[i] == str2[i]){
        i++;
    }
    return str2[i] - str1[i];
}

char* scpy(char* str)
{
    char* copy = (char*)malloc(sizeof(char) * slen(str));
    if (copy == NULL) return 0;

    for (int i = 0; i < slen(str); i++){
        copy[i] = str[i];
    }

    return copy;
}
