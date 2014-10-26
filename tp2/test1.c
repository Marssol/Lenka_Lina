#include <stdlib.h>
#include "my_stdio.h"
#include <stdio.h>


//bufsize 10
/*
int main (int argc, char * argv[]) {
    MY_FILE * f1;
    MY_FILE * f2;
    char buff[50];
    int result;
    if (argc != 3)
        exit (1);
    f1 = my_fopen(argv[1], "r");
    if (f1 == NULL) exit (2);
    f2 = my_fopen(argv[2], "w");
    if (f2 == NULL) exit (3);

    result = my_fread(&buff, 1, 5, f1);
    if (result!=5) exit(2);
    result = my_fwrite(&buff, 1, 5, f2);
    if (result!=5) exit(2);
    result = my_fread(&buff, 1, 5, f1);
    result = my_fwrite(&buff, 1, 5, f2);
    result = my_fread(&buff, 1, 11, f1);
    result = my_fwrite(&buff, 1, 11, f2);
    result = my_fread(&buff, 1, 20, f1);
    result = my_fwrite(&buff, 1, 20, f2);
    result = my_fread(&buff, 1, 4, f1);
    result = my_fwrite(&buff, 1, result, f2);

    my_fclose(f1);
    my_fclose(f2);
    return 0;
}*/

int main (int argc, char * argv[]) {
	MY_FILE * f1;
	MY_FILE * f2;
	char c;
	int result;
    if (argc != 3)
		exit (1);
	f1 = my_fopen(argv[1], "r");
	if (f1 == NULL)
		exit (2);
	f2 = my_fopen(argv[2], "w");
	perror("");
	if (f2 == NULL)
		exit (3);
	result = my_fread(&c, 1, 1, f1);
	while (result == 1)
	{
		result = my_fwrite(&c, 1, 1, f2);
		if (result == -1)
			exit(4);
		result = my_fread(&c, 1, 1, f1);
	}
	if (result == -1)
		exit(5);
	my_fclose(f1);
	my_fclose(f2);
	return 0;
}
