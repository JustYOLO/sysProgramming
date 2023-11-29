#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char *tokens[100] = {"ls", "-al"};
    char nullchar = '\0';
    char charConv = (char *) 0;

    if(nullchar == charConv)
        printf("is same\n");
    
    // if(strcmp(NULL, "c") == 0)
    //     printf("testing\n");
    // results segfault
    return 0;
}