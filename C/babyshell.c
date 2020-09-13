#include <stdio.h>
#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define MAX_PATH 1000

int laststatus = 0;
extern char **environ;

int main()
{
    char buf[1000], **p;
    extern void execute(char **argv);

    while (printf("$ "), fgets(buf, sizeof buf, stdin))
        if ((p = parse(buf)))
            execute(p);

    return(laststatus);
}
void execute(char **argv)
{
    struct stat statbuf;
    char path_bin[MAX_PATH];
    char path_usr[MAX_PATH];
    char path_cd[MAX_PATH];
    char execute[MAX_PATH];
    int prog_name_len = strlen(argv[0]);
    int found = 0;
    if (strchr(argv[0], '/') == NULL) {
        //first case
        if (prog_name_len + strlen("/bin/") >= MAX_PATH) {
                fprintf(stderr, "string size exceeded for path %s%s\n", "/bin/", argv[0]);
                exit(1);
        }
        strcpy(path_bin, "/bin/");
        strcat(path_bin, argv[0]);
        if ( stat(path_bin, &statbuf) == 0 ) {
            strcpy(execute, path_bin);
            found = 1;
        }
        else { //second case
            if (prog_name_len + strlen("/usr/bin/") >= MAX_PATH) {
                fprintf(stderr, "string size exceeded for path %s%s\n", "/usr/bin/", argv[0]);
                exit(1);
            }
            strcpy(path_usr, "/usr/bin/");
            strcat(path_usr, argv[0]);
            if ( stat(path_usr, &statbuf) == 0 ) {
                strcpy(execute, path_usr);
                found = 1;
            }
            else { //third case
                //no need to check path length because prev checks are sufficient
                strcpy(path_cd, "./");
                strcat(path_cd, argv[0]);
                if ( stat(path_cd, &statbuf) == 0 ) {
                    strcpy(execute, path_cd);
                    found = 1;
                }
            }
        }
        if (!found) {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            laststatus = 1;
            return;
        }
    }
    else {
        if (prog_name_len >= MAX_PATH) {
            fprintf(stderr, "string size exceeded for path %s\n", argv[0]);
            exit(1);
        }
        strcpy(execute, argv[0]);
    }

    int x = fork();
    if (x == -1) {
        perror("fork");
        exit(1);
    }
    else if (x == 0) {
        //child
        execve(execute, argv, environ);
        perror(execute);
        exit(1);
    }
    else {
        //parent
        int status;
        if (wait(&status) == -1) {
            perror("wait");
            exit(1);
        }
        if (status != 0)
            printf("exit status %d\n", WEXITSTATUS(status));
        laststatus = WEXITSTATUS(status);
    }
}
