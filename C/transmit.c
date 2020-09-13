#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
int main(int argc, char **argv)
{
    int c, i, pid;
    if (argc != 3 || strlen(argv[2]) != 1 || (pid = atoi(argv[1])) == 0) {
        fprintf(stderr, "usage: %s pid character\n", argv[0]);
        return(1);
    }
    c = argv[2][0];
    for (i = 0; i < 8; i++) {
        if (c & 128) {
            if( (kill(pid, SIGUSR2)) == -1) {
                perror("kill");
                return(1);
            }
        } else {
            if( (kill(pid, SIGUSR1)) == -1) {
                perror("kill");
                return(1);
            }
        }
        usleep(100000);
        c <<= 1;
    }

    return(0);
}
