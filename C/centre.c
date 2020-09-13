#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
static char *prog;
static int w = 80, cflag = 0;
static char c = ' ';
int main(int argc, char **argv)
{
    int ch, status = 0;
    FILE *fp;
    prog = argv[0];
    extern void process(FILE *fp);
    while ((ch = getopt(argc, argv, "w:c:")) != EOF) {
	switch(ch) {
	case 'w':
	    w = atoi(optarg);
	    break;
	case 'c':
	    c = *(optarg);
	    cflag = 1;
	    break;
	case '?':
	default:
	    status = 1;
	    break;
	}
    }
    if (status) {
	fprintf(stderr, "usage: %s [-w width] [-c character] [file ...]\n", prog);
	return(status);
    }
    if (optind == argc)
	process(stdin);
    else
	for (; optind < argc; optind++) {
		if (strcmp(argv[optind], "-") == 0)
	    		process(stdin);
        	else if ((fp = fopen(argv[optind], "r")) == NULL) {
            		perror(argv[optind]);
            		return(1);
        	} else {
            		process(fp);
           		fclose(fp);
		}
	}
    return(status);
}
void process(FILE *fp)
{
    int CUR_MAX = 500;
    char *line = (char*) malloc(sizeof(char) * CUR_MAX);
    int ch, difference, count = 0, front = 0, back = 0;
    while ((ch = getc(fp)) != EOF) {
	while (ch != '\n' && ch != EOF) {
            line[count] = ch;
            count++;
            if(count == CUR_MAX) {
                CUR_MAX *= 2;
                line = realloc(line, CUR_MAX);
		if (line == NULL) {
    			fprintf(stderr, "out of memory\n");
    			exit(1);
		}
            }
            ch = getc(fp);
        }
        line[count] = '\0';
        difference = w - count;
        if (difference <= 0) {
            printf("%s\n", line);
        } else {
            front = difference / 2;
            if (cflag)
                back = difference - front;
            for (int i = 0; i < front; i++)
                printf("%c", c);
            for (int i = 0; i < count; i++)
                printf("%c", line[i]);
            for (int i = 0; i < back; i++)
                printf("%c", c);
            printf("\n");
	}
        count = 0;
    }
}
