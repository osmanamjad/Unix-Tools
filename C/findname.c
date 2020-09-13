#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
static char *name, *prog;
static int status = 1;
int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "usage: %s name dir ...\n", argv[0]);
		exit(2);
	}
	prog = argv[0];
	name = argv[1];
        extern void find_name(char* dir);
	for (int i = 2; i < argc; i++) {
		find_name(argv[i]);
	}
        return(status);
}
void find_name(char *dir)
{
	char temp_dir[2000];
	strcpy(temp_dir, dir);
	DIR *dp;
	struct dirent *p;
	struct stat statbuf;
	if ((dp = opendir(dir)) == NULL) {
		perror(dir);
		exit(2);
	}
	while ((p = readdir(dp))) {
		if (strlen(temp_dir) + strlen("/") + strlen(p->d_name) > 1999) {
			fprintf(stderr, "%s: string size exceeded for path %s\n", prog, temp_dir);
			exit(2);
		}

		strcat(strcat(temp_dir, "/"), p->d_name);
		if ( lstat(temp_dir, &statbuf) ) {
            		perror(p->d_name);
           		exit(2);
        	}
		if (strcmp(p->d_name, name) == 0) {
			printf( "%s\n", temp_dir);
			status = 0;
		}
		if (S_ISDIR(statbuf.st_mode) && strcmp(p->d_name, "..") != 0 && strcmp(p->d_name, ".") != 0) {
			find_name(temp_dir);
		}
		strcpy(temp_dir, dir);
	}
	closedir(dp);
}
