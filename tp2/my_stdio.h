#ifndef MY_STDIO_H
#define MY_STDIO_H

#include <stdio.h>
#include <string.h>

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
    int eof;

}MY_FILE;

MY_FILE* my_fopen(char *name, char *mode);
int my_fclose(MY_FILE *f);
int my_fread(void *p, size_t size, size_t nbelem, MY_FILE *f);
int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f);
int my_feof(MY_FILE *f);
int my_fprintf(MY_FILE *f, char *format, ...);
int my_fscanf(MY_FILE *f, char *format, ...);

#endif // MY_STDIO_H
