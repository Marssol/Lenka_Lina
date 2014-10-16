#ifndef MY_STDIO_H
#define MY_STDIO_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

typedef struct my_file
{
    char* buffer;
    char* cursor;
    char* end_cursor;
    int handler;
    char mode;
    int empty;
    size_t pos;
    size_t len;

}MY_FILE;
//Opens an access to a file, where name is the path of the file and mode is either ”r” for read or ”w” for
//write. Returns a pointer to a MY FILE structure upon success and NULL otherwise.
MY_FILE* my_fopen(char *name, char *mode);

//Closes the access to a file associated to f. Returns 0 upon success and -1 otherwise.
int my_fclose(MY_FILE *f);

//Reads at most nbelem elements of size size from file access f, that has to have been opened with
//mode ”r”, and places them at the address pointed by p. Returns the number of elements actually read,
//0 if an end-of-file has been encountered before any element has been read and -1 if an error occured.
int my_fread(void *p, size_t size, size_t nbelem, MY_FILE *f);

//Writes at most nbelem elements of size size to file access f, that has to have been opened with mode
//”w”, taken at the address pointed by p. Returns the number of elements actually written and -1 if an
//error occured.
int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f);

//Returns 1 if an end-of-file has been encountered during a previous read and 0 otherwise.
int my_feof(MY_FILE *f);


#endif // MY_STDIO_H
