#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define MAX_BUF 64

char fname[] = "newfile_lseek.txt";
char dummy_data[] = "abcdefg\n";

int main()
{
    int fd, write_size, read_size, new_offset;
    char buf[MAX_BUF];

    fd = open(fname, O_RDWR | O_CREAT | O_EXCL, 0664);
    write_size = write(fd, dummy_data, sizeof(dummy_data));
    printf("write_size = %d\n", write_size);
    close(fd);

    fd = open(fname, O_RDONLY);
    new_offset = lseek(fd, -1, SEEK_END);
    read_size = read(fd, buf, MAX_BUF);
    printf("read_size = %d\n", read_size);
    write_size = write(STDOUT_FILENO, buf, read_size);
    close(fd);

    return 0;
}