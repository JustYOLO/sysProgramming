#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <utime.h> // to use utimbuf
#include <sys/stat.h> // to use stat()
#include <time.h> // to get current time
#define MAX_BUF 128

int main(int argc, char* argv[])
{
  int input_fd, output_fd, write_size, read_size;
  char buf[MAX_BUF];
  struct stat st; // to get original file's attribute
  struct utimbuf time_buf; // to get original file's time

  if(argc != 3) // gets 3 arguments
  {
    printf("USAGE: %s target_file_name output_file_name\n", argv[0]);
    exit(-1);
  }

  if(stat(argv[1], &st) < 0)
  {
    printf("Cannot get stat from file %s\n", argv[1]);
    exit(-1);
  }

  time_buf.actime = st.st_atime;
  time_buf.modtime = st.st_mtime; // get original file's time
  
  input_fd = open(argv[1], O_RDONLY | O_EXCL);
  if(input_fd < 0) // cannot open file
  {
    printf("Cannot open %s file with errno %d\n", argv[1], errno);
    exit(-1);
  }
  
  output_fd = open(argv[2], O_WRONLY | O_EXCL | O_CREAT, st.st_mode);
  // make a new file with originial file's permission
  if(output_fd < 0) // cannot make file
  {
    printf("Cannot make %s file with errno %d\n", argv[2], errno);
    exit(-1);
  }
  
  while(1) // copy file data
  {
    read_size = read(input_fd, buf, MAX_BUF);
    if(read_size == 0)
      break;
    write_size = write(output_fd, buf, read_size);
  }
  
  utime(argv[2], &time_buf); // change time to original file's time

  close(input_fd);
  close(output_fd);

  time_t timer;
  struct tm* t;
  timer = time(NULL);
  t = localtime(&timer); 
  printf("Student id: 32203349\n");
  printf("Current time: %d.%d.%d. %d:%d:%d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
  // print student id and current time

  return 0;
}
