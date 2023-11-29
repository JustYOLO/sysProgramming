#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    pid_t fork_return;
    char *tokens[3] = {"ls", "-al", NULL};

    if((fork_return = fork()) < 0) {
        printf("fork error\n");
    } else if(fork_return == 0) {
        int fd1 = open("testText.txt", O_WRONLY | O_CREAT, 0644);
        dup2(fd1, STDOUT_FILENO);
        close(fd1);
        if(fd1 < 0)
            printf("open err\n");
        int status_code = execvp(tokens[0], tokens);
        if(status_code < 0)
        {
            printf("error occured with code: %d\n", status_code);
            printf("errno: %d\n", errno);
            perror("perror ");
            exit(-1);
        }
    } else {
        int status;
        waitpid(fork_return, &status, 0);
    }

    return 0;
}