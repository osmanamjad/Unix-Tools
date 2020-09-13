#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main()
{
	long inode;
	struct stat statbuf;
	DIR *dp;
	struct dirent *p;
	if (lstat(".", &statbuf)) {
		perror(".");
            	return(1);
        } else {
		inode = statbuf.st_ino;
		if (chdir("..") != 0) {
			perror("..");
			return(1);
		}
		while (inode != 2) {
        		if ((dp = opendir(".")) == NULL) {
                		perror(".");
                		return(1);
        		}
			while ( (p = readdir(dp)) ) {
				if (p->d_ino == inode) {
					printf("%s\n", p->d_name);
					break;
				}
			}
			closedir(dp);
			if (lstat(".", &statbuf)) {
				perror(".");
            			return(1);
			} else {
				inode = statbuf.st_ino;
				if (chdir("..") != 0) {
					perror("..");
					return(1);
				}
			}
		}
		printf("and then we're at the root directory.\n");
	}
	return(0);
}
