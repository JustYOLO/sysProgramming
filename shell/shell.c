#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h> // bool type
#include <string.h> // strtok()
#define MAX_BUF 128
#define PATH_MAX 1024 // 경로 최대값
/*
*/

bool cmd_help(int argc, char* argv[]);

int tokenize(char* buf, char* delims, char* tokens[], int maxTokens);

bool run(char* line);

int main()
{
    char line[1024];
    char path[PATH_MAX];
    while(1)
    {
        if(getcwd(path, PATH_MAX) == NULL)
        {
            printf("function getcwd() error\n");
            exit(-1);
        }
        printf("%s $ ", path);
        fgets(line, sizeof(line) - 1, stdin);
        if(run(line) == false)
            break;
    }

    return 0;
}

int tokenize(char* buf, char* delims, char* tokens[], int maxTokens)
{
    int tokenCount = 0;
    char* token = strtok(buf, delims);
    while(token != NULL && tokenCount < maxTokens)
    {
        tokens[tokenCount] = token;
        token = strtok(NULL, delims);
        tokenCount++;
    }
    return tokenCount;
}

bool run(char* line)
{
    char delims = ' '; // default delimiter is ' ' (single space) 
    char* tokens[MAX_BUF];
    int tokenCount = tokenize(line, &delims, tokens, sizeof(tokens) / sizeof(char*));
    

    /* token checking code
    printf("%d tokens input\n", tokenCount);
    int i;
    for(i = 0; i < tokenCount; i++)
        printf("token %d: %s\n", i + 1, tokens[i]);    
    */

    return false;
}
