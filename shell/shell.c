#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h> // bool type
#include <string.h> // strtok()
#include <time.h> 
#include <errno.h>
#define MAX_BUF 128
#define PATH_MAX 1024 
#define DELIM_CHARS " "
#define EX_PATH "/bin/" // path for external commands

int tokenize(char* buf, char* delims, char* tokens[], int maxTokens);
bool run(char* line);
int bulitinHandler(char* tokens[]); // handles bulit-in shell command
void printTime(); // print time and studnet id
int executeExternal(char* tokens[], int tokenCount); // executes program

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
    printTime();
    return 0;
}

int tokenize(char* buf, char* delims, char* tokens[], int maxTokens)
{
    char *newline = strchr(buf, '\n');
    if(newline)
        *newline = (char *) 0;
    int tokenCount = 0;
    char* token = strtok(buf, DELIM_CHARS);
    while(token != NULL && tokenCount < maxTokens)
    {
        tokens[tokenCount] = token;
        token = strtok(NULL, DELIM_CHARS);
        tokenCount++;
    }
    
    tokens[tokenCount] = NULL;
    return tokenCount;
}

bool run(char* line)
{
    char delims = " "; // default delimiter is ' ' (single space) 
    char* tokens[MAX_BUF];
    int tokenCount = tokenize(line, &delims, tokens, sizeof(tokens) / sizeof(char*));
    if(bulitinHandler(tokens))
    {
        printf("Not a bulit-in command %s\n", tokens[0]);
        executeExternal(tokens, tokenCount);
    }

    
    printf("%d tokens input\n", tokenCount);
    int i;
    for(i = 0; i < tokenCount; i++)
        printf("token %d: %s\n", i + 1, tokens[i]);

    return true;
}

int bulitinHandler(char* tokens[])
{ // returns 0 -> handled bulit-in command
    // returns 1 -> not a bulit-in command
    char* bulitInList[2] = {"cd", "exit"};
    int i;
    int cmdType = 0;
    for(i = 0; i < 3; i++)
    {
        if(strcmp(tokens[0], bulitInList[i]) == 0)
        {
            cmdType = i + 1;
            break;
        }
    }
    switch (cmdType)
    {
    case 1: // bulit-in command cd
        chdir(tokens[1]);
        return 0;

    case 2: // bulit-in command exit
        printTime();
        exit(0);
    
    default:
        break;
    }
    return 1;
}

void printTime()
{
    time_t timer;
    struct tm* t;
    timer = time(NULL);
    t = localtime(&timer);
    printf("Student id: 32203349\n");
    printf("Current time: %d.%d.%d. %d:%d:%d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}

int executeExternal(char* tokens[], int tokenCount)
{
    pid_t fork_return;
    // if(strcmp(tokens[0], "ls") == 0)
    // {
    //     if( (fork_return = fork()) < 0)
    //     {
    //         printf("fork error\n");
    //         exit(-1);
    //     } else if(fork_return == 0) // run ls
    //     {
    //         execl("/bin/ls", tokens, (char *) 0);
    //     } else {
    //         wait(NULL);
    //     }
    // }

    int curPath = 0;
    if(tokens[0][0] == '.' && tokens[0][1] == '/')
    { // if user wants to execute program in current path
        printf("it starts with the dot\n");
        curPath = 1;
    }

    bool isBackground = false;
    if(strcmp(tokens[tokenCount - 1], "&") == 0)
        isBackground = true;

    if((fork_return = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    } else if(fork_return == 0) // runs external command
    {
        int status_code;
        if(curPath != 1)
        { // executes program in path
            status_code = execvp(tokens[0], tokens);
        } else {
            status_code = execl(tokens[0], tokens, (char *) 0);
        }
        if(status_code < 0)
        {
            printf("error occured with code: %d\n", status_code);
            printf("errno: %d\n", errno);
            exit(-1);
        }
        
    } else {
        if(isBackground)
            printf("Program started in background using pid: %d\n", fork_return);
        else {
            int status;
            waitpid(fork_return, &status, 0);
        }
    }

    return 0;
}