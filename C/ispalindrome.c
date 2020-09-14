#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main(int argc, char* argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s \"string\"\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char* p = argv[1];
	char* q = p;
	while (*(q) != '\0') {
		q = q + 1;
	}
	while (p < q) {
		*(p) = tolower(*(p));
		*(q) = tolower(*(q));
		if(isalnum(*(p)) && isalnum(*(q))) {
			if(*(p) != *(q)){
				exit(EXIT_FAILURE);
			}
			p = p + 1;
			q = q - 1;
		}
		else if (isalnum(*(p))) {
			q = q - 1;
		}
		else if (isalnum(*(q))) {
			p = p + 1;
		} else {
			p = p + 1;
			q = q - 1;
		}
	}
	return(0);
}
