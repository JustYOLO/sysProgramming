#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t fork_return;
    printf("hello, my pid : %d\n", getpid());

    if( (fork_return = fork()) < 0)
    {
        printf("fork error\n");
        exit(-1);
    } else if(fork_return == 0) {
        printf("child pid: %d, parent pid: %d\n", getpid(), getppid());
    } else {
        wait(NULL);
    }
}