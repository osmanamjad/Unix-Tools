//receives a byte using signals
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int signalcount = 0;
int c = 0;
int main()
{
    struct sigaction sa;
    extern void myhandler();

    sa.sa_handler = myhandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    printf("My pid is %ld\n", (long)getpid());
    while (signalcount < 8)
        sleep(1234);  /* will be interrupted by signal */
    printf("%c\n", c);
    return(0);
}

void myhandler(int code)
{
    signalcount++;
    if (code == SIGUSR1) {
        c <<= 1;
    } else if (code == SIGUSR2) {
        c <<= 1;
        c += 1;
    }
}
