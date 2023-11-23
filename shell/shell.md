<h1>Shell</h1>

<h2>TODO:</h2>

~~get current dir in c~~ <br>
~~-> use getcwd() in unistd.h~~

~~how to tokenize instruction~~ <br>
~~-> use strtok() in string.h~~


internal commands implemented

external commands implemented

background processing implemented

TODO:
    redirection


difference in:
```c
    #include <sys/types.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    // may differ by include order
    // sys/types.h -> stdlib.h => worked
    // stdlib.h -> sys/types.h => not worked in linux

    wait(); // in linux POSIX?
    wait(NULL); // in Darwin POSIX?
    // wtf
```