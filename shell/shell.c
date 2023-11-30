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
/* handles bulit-in shell command (cd, exit)
 * returns -1 if exit command executed,
 * returns 0 if bulit-in command executed,
 * returns 1 if external command inputs.
*/
void printTime();
// print time and studnet id
int executeExternal(char* tokens[], int tokenCount);
/* executes external program (ls, ps, kill...), uses execvp
 * returns 0 if command successfully exectued,
 * returns -1 if failed. 
*/

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
    int tokenCount = 0;

    char *newline = strchr(buf, '\n'); // finds newline char
    if(newline) // if buf have newline char
        *newline = (char *) 0; // replace it to \0
    
    char* token = strtok(buf, DELIM_CHARS); // tokenize by delimiters
    while(token != NULL && tokenCount < maxTokens)
    { // tokenize buf
        tokens[tokenCount] = token; // save to each token
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

    int returnCode = bulitinHandler(tokens); // handle bulit-in command
    if(returnCode > 0)
    { // if command is not bulit-in handler
        int executeCode = executeExternal(tokens, tokenCount); // execute external command
        if(executeCode < 0)
            printf("Execute external command failed\n");
    } else if(returnCode < 0) // if returns -1
        return false; // return false

    return true; // handled input line (command)
}

int bulitinHandler(char* tokens[])
{ // returns 0 -> handled bulit-in command
  // returns 1 -> not a bulit-in command
  // returns -1 -> exit command
    char* bulitInList[2] = {"cd", "exit"}; // list of bulit-in command
    int i;
    int cmdType = 0;
    for(i = 0; i < 3; i++)
    {
        if(strcmp(tokens[0], bulitInList[i]) == 0)
        { // search if given command matches to bulit-in command
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
        return -1;
    
    default:
        break;
    }
    return 1;
}

void printTime()
{ // prints time and student id
    time_t timer;
    struct tm* t;
    timer = time(NULL);
    t = localtime(&timer);
    printf("Student id: 32203349\n");
    printf("Current time: %d.%d.%d. %d:%d:%d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    // formating the time
}

int executeExternal(char* tokens[], int tokenCount)
{ // executes external command(program) using execvp
  // returns 0 if successfully executed external command,
  // returns -1 if failed.
    pid_t fork_return;

    bool isBackground = false;
    bool isRedirection = false;
    // flag for background(&) processing or redirection(>)
    if(strcmp(tokens[tokenCount - 1], "&") == 0)
    { // if last token is "&"
        isBackground = true; // flag -> true
        tokens[tokenCount - 1] = NULL; // change the token to NULL
        tokenCount--;
    }

    int i;
    char *fname = NULL;
    for(i = 0; i < tokenCount; i++)
    { // check all the tokens to find redirection point
        if(strcmp(tokens[i], ">") == 0)
        { // if one of the token is ">"
            isRedirection = true; // flag -> true
            fname = tokens[i + 1]; // get the redirection destination(file name)
            tokens[i] = NULL; 
            tokens[i + 1] = NULL; // change into NULL
            break;
        }
    }

    if((fork_return = fork()) < 0)
    { // if fork fails
        printf("fork error\n");
        perror("perror "); // display error msg
        return -1;
    } else if(fork_return == 0)
    { // child process runs external command
        int status_code;
        if(isRedirection) // if redirection flag is true
        {
            int fd = open(fname, O_WRONLY | O_CREAT, 0644);
            // make or open the target file
            dup2(fd, STDOUT_FILENO); // change the file descriptor
            close(fd);
            if(fd < 0)
            { // if open failed,
                dup2(1, STDOUT_FILENO); // undo changing file descriptor
                perror("file open error, perror ");
            }
        }

        status_code = execvp(tokens[0], tokens); // executes command with argument

        if(status_code < 0) // if execution failed
        {
            dup2(1, STDOUT_FILENO); // prints error msg in STDOUT_FILENO
            if(errno == 2)
            { // if errno is 2 (no such file or directory)
                printf("Cannot find command \"%s\"\n", tokens[0]);
            }
            printf("error occured with code: %d\n", status_code);
            printf("errno: %d\n", errno);
            perror("perror ");
            exit(-1); // exits child process
        }
        
    } else { // parent process
        int status;
        if(isBackground) { // if background flag is true
            printf("Program started in background using pid: %d\n", fork_return);
        }
        else { // if background flag is false
            waitpid(fork_return, &status, 0); // wait until the child process ends
        }
    }

    return 0; // sucessfully executed external command.
}