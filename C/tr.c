#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int pid;
    int pipefd[2];
    if (pipe(pipefd)) {
        perror("pipe");
        return(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return(1);
    }
    else if (pid == 0) {
        //child
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        close(pipefd[1]);
        char line[7];
        while (fgets(line, sizeof(line), stdin) != 0)
        {
            size_t len = strlen(line);
            if (write(1, line, len) != len)
            {
                perror("write");
                return(1);
            }
        }
    }
    else {
        //parent
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        execl("/usr/bin/tr", "tr", "e", "f", (char *)NULL);
        perror("/usr/bin/tr");
        return(1);
    }
    return(0);
}
