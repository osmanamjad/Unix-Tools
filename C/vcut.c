#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
static int length = 0, keep[100];
static char *prog, *spec;
int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s spec [file ...]\n", argv[0]);
		return(1);
	}
	prog = argv[0];
	spec = argv[1];
	length = strlen(argv[1]);
	if ( !(isdigit(spec[0])) ) {
		fprintf(stderr, "%s: unexpected character '%c' in spec \"%s\"\n", prog, spec[0], spec);
		return(1);
	}
	for (int i = 0; i < length - 1; i++)
		if ( spec[i] == ',' && !(isdigit(spec[i + 1])) ) {
			fprintf(stderr, "%s: unexpected character '%c' in spec \"%s\"\n", prog, spec[i + 1], spec);
			return(1);
		}
	if (spec[length - 1] == ',')
		length--;
	int first = -1, second = -1, num_digits = 0;
	FILE *fp;
	extern void check_third_digit(int i);
	extern void process(FILE *fp);
	extern int check_dash(int i);
	for (int i = 0; i < length; i++) {
		if (spec[i] == ',')
			i++;
		else if ( !(isdigit(spec[i])) ) {
			fprintf(stderr, "%s: in spec \"%s\" expected comma, saw '%c'\n", prog, spec, spec[i]);
			return(1);
		}
		first = spec[i] - '0';
		if (i + 1 < length) {
			if (isdigit(spec[i + 1])) {
				i++;
				first = first * 10 + (spec[i] - '0');
				check_third_digit(i);
				if ((second = check_dash(i)) >= 0) {
					num_digits = second < 10 ? 1 : 2;
					i += num_digits + 1;
                        	}
			}
			else if ((second = check_dash(i)) >= 0) {
				num_digits = second < 10 ? 1 : 2;
				i += num_digits + 1;
                        }
		}
		if (first >= 0 && second >= 0)
			for(int i = first; i <= second; i++)
				keep[i] = 1;
		else
			keep[first] = 1;
		second = -1;
	}
	if (argc == 2)
		process(stdin);
	else
		for (int i = 2; i < argc; i++)
			if ((fp = fopen(argv[i], "r")) == NULL) {
                        	perror(argv[i]);
                        	return(1);
                	} else {
                        	process(fp);
                        	fclose(fp);
                	}
	return(0);
}
void process(FILE *fp)
{
	int ch, count = 1;
	while ((ch = getc(fp)) != EOF && count < 100) {
		if (keep[count] == 1)
			putc(ch, stdout);
		if (ch == '\n')
			count++;
	}
}
void check_third_digit(int i)
{
	if (i + 1 < length && isdigit(spec[i + 1])) {
		fprintf(stderr, "%s: specifiers can be at most two digits\n", prog);
                exit(1);
        }
}
int check_dash(int i)
{
	int second = -1;
	extern void check_third_digit(int i);
	if (i + 1 < length && spec[i + 1] == '-') {
		i += 2;
		//i >= length considers the case where spec ends with a -
		if ( !(isdigit(spec[i])) || i >= length) {
			fprintf(stderr, "%s: expected digit after '-' in spec \"%s\"\n", prog, spec);
			exit(1);
		}
		second = spec[i] - '0';
                if (i + 1 < length && isdigit(spec[i + 1])) {
			i++;
                       	second = second * 10 + (spec[i] - '0');
                       	check_third_digit(i);
               	}
	}
	return(second);
}
