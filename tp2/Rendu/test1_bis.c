#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main (int argc, char * argv[]) {
	FILE * f1;
	FILE * f2;
	char c;
	int result;
    if (argc != 3)
		exit (1);
	f1 = open(argv[1], O_RDONLY);
	if (f1 == NULL)
		exit (2);
	f2 = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0644);
	perror("");
	if (f2 == NULL)
		exit (3);
	result = read(f1, &c, 1);
	while (result == 1)
	{
		result = write(f2, &c, 1);
		if (result == -1)
			exit(4);
		result = read(f1, &c, 1);
	}
	if (result == -1)
		exit(5);
	close(f1);
	close(f2);
	return 0;
}
