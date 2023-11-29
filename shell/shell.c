#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h> // bool type
#include <string.h> // strtok(), strcmp() ...
#include <time.h> 
#define MAX_BUF 128
#define PATH_MAX 1024 
#define DELIM_CHARS " "

int tokenize(char* buf, char* tokens[], int maxTokens);
// tokenizes the given string (buf)
bool run(char* line);
// run command by given line
int bulitinHandler(char* tokens[]);
// handles bulit-in shell command (cd, exit)
// returns 0 if bulit-in command executed,
// returns 1 if external command inputs.
void printTime();
// print time and studnet id
int executeExternal(char* tokens[], int tokenCount);
// executes external program (ls, ps, kill), uses execvp

int main()
{
    char line[1024]; // saves command input
    char path[PATH_MAX]; // saves current path
    while(1)
    {
        if(getcwd(path, PATH_MAX) == NULL) // gets current path
        { // when get current path fails:
            printf("function getcwd() error\n");
            exit(-1); 
        }
        printf("%s $ ", path); // prints path to shell
        fgets(line, sizeof(line) - 1, stdin); // gets command from user
        if(run(line) == false)
            break; // if run() returns false, means exit the shell
    }
    printTime(); // before exit, prints time and student id.
    return 0;
}

int tokenize(char* buf, char* tokens[], int maxTokens)
{ // tokenizes command input
    char *newline = strchr(buf, '\n'); // finds newline char
    if(newline) // if buf have newline char
        *newline = (char *) 0; // replace it to \0

    int tokenCount = 0;
    char* token = strtok(buf, DELIM_CHARS); // tokenize by delimiters
    while(token != NULL && tokenCount < maxTokens)
    {
        tokens[tokenCount] = token;
        token = strtok(NULL, DELIM_CHARS);
        tokenCount++;
    }
    
    tokens[tokenCount] = NULL; // put NULL after token ends
    return tokenCount;
}

bool run(char* line)
{ // runs command
    char* tokens[MAX_BUF];
    int tokenCount = tokenize(line, tokens, sizeof(tokens) / sizeof(char*)); // tokenize the given line
    if(bulitinHandler(tokens))
    { // if command is not bulit-in handler
        executeExternal(tokens, tokenCount); // execute external command
    }

    return true;
}

int bulitinHandler(char* tokens[])
{ // returns 0 -> handled bulit-in command
    // returns 1 -> not a bulit-in command
    //
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

    bool isBackground = false;
    bool isRedirection = false;
    if(strcmp(tokens[tokenCount - 1], "&") == 0)
    {
        isBackground = true;
        tokens[tokenCount - 1] = NULL;
        tokenCount--;
    }
    int i;
    char *fname = NULL;
    for(i = 0; i < tokenCount; i++)
    {
        if(strcmp(tokens[i], ">") == 0)
        {
            isRedirection = true;
            tokens[i] = NULL; // change into NULL
            fname = tokens[i + 1];
            break;
        }
    }
    // use both of background processing and redirection ?

    if((fork_return = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    } else if(fork_return == 0) // child process runs external command
    {
        int status_code;
        if(isRedirection)
        {
            int fd = open(fname, O_WRONLY | O_CREAT, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            if(fd < 0)
            {
                dup2(1, STDOUT_FILENO);
                perror("file open error, perror ");
            }
        }

        status_code = execvp(tokens[0], tokens);
        if(status_code < 0)
        {
            dup2(1, STDOUT_FILENO);
            printf("error occured with code: %d\n", status_code);
            printf("errno: %d\n", errno);
            perror("perror : ");
            exit(-1);
        }
        
    } else {
        int status;
        if(isBackground) {
            printf("Program started in background using pid: %d\n", fork_return);
        }
        else {
            waitpid(fork_return, &status, 0);
        }
    }

    return 0;
}