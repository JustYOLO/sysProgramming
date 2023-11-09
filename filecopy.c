#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define MAX_BUF 64

int main(int argc, char *argv[])
{
    int source, dest, read_size, write_size;
    char buf[MAX_BUF];
    if(argc != 3)
    {
        printf("USAGE : %s source_File destination_File\n", argv[0]);
        exit(-1);
    }
    source = open(argv[1], O_RDONLY);
    if(source < 0)
    {
        printf("Can't open %s file with errno %d\n", argv[1], errno);
        exit(-1);
    }
    dest = open(argv[2], O_RDWR | O_CREAT | O_EXCL, 0774);
    // When the source file is non-executable due to permission,
    // read the source file and make a new file with permission.
    while(1)
    {
        read_size = read(source, buf, MAX_BUF);
        if(read_size == 0)
            break;
        write_size = write(dest, buf, read_size);
    }
    close(source);
    close(dest);

    return 0;
}