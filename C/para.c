#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

struct llnode
{
    char *command;
    int fd;
    struct llnode *next;
};

struct llnode *head = NULL;
int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s cmd <parameters\n", argv[0]);
        exit(1);
    }
    extern void parse_cmdline(char **argv);
    extern void execute_commands();
    extern void print_output();
    fflush(stdout);
    parse_cmdline(argv);
    execute_commands();
    print_output();
    return(0);
}

void parse_cmdline(char **argv)
{
    extern void insert_at_head(struct llnode **head_ref, char *command, int fd);
    char *replace = strchr(argv[1], '%');
    char *command;
    int replace_index = replace - argv[1];
    int counter = 0;
    char param[1000];
    //start with head of LL and store the parameter in each node.
    while (fgets(param, 1000, stdin) != NULL) {
        if (replace != NULL) {
            //we want to strip the newline character
            param[strlen(param) - 1] = '\0';

            command = (char *)malloc( strlen(param) + strlen(argv[1]) + 1 );
            if (command == NULL) {
                fprintf(stderr, "out of memory\n");
                exit(1);
            }
            strncpy(command, argv[1], replace_index);
            //we need to add the null term before using strcat
            command[replace_index] = '\0';

            strcat(command, param);
            strcat(command, argv[1] + replace_index + 1);
            insert_at_head(&head, command, -1);
        }
        else {
            counter++;
            command = (char *)malloc( strlen(argv[1]) + 1 );
            if (command == NULL) {
                fprintf(stderr, "out of memory\n");
                exit(1);
            }
            strcpy(command, argv[1]);
            insert_at_head(&head, command, -1);
        }
    }
}

void execute_commands()
{
    extern void docommand(struct llnode *p);
    struct llnode *p;
    for (p = head; p; p = p->next) {
        docommand(p);
    }
}

void print_output()
{
    struct llnode *p;
    for (p = head; p; p = p->next) {
        printf("---- %s ----\n", p->command);
        char c;
        int ret;
        while( (ret = read(p->fd, &c, 1)) != 0 ) {
            if (ret < 0) {
                perror("read");
                exit(1);
            }
            putchar(c);
        }
    }
}

void docommand(struct llnode *p)
{
    int pipefd[2];
    if (pipe(pipefd)) {
        perror("pipe");
        exit(1);
    }
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {
        dup2(pipefd[1], 1);
        dup2(pipefd[1], 2);
        int n = sysconf(_SC_OPEN_MAX);
        for(int i = 3; i < n; i++)
            close(i); //this will also close pipefd[0] and pipefd[1]

        close(0);
        if(open("/dev/null", O_RDONLY) < 0) {
            perror("/dev/null");
            exit(1);
        }
        execl("/bin/sh", "sh", "-c", p->command, (char *)NULL);
        perror("/bin/sh");
        exit(1);
    }
    else {
        close(pipefd[1]);
        p->fd = pipefd[0];
    }
}

void insert_at_head(struct llnode **head_ref, char *command, int fd)
{
    struct llnode *new_node = (struct llnode *) malloc(sizeof(struct llnode));
    if (new_node == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    new_node->command = (char *)malloc(strlen(command) + 1);
    if (new_node->command == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    strcpy(new_node->command, command);
    new_node->fd = fd;

    new_node->next = (*head_ref);
    (*head_ref) = new_node;
}
