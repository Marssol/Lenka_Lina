#include "string.h"
#include "my_stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#define BUFF_SIZE 10
#define INT_DIGIT 7

MY_FILE* my_fopen(char *name, char *mode)
{
    MY_FILE* my_file = malloc(sizeof(MY_FILE));
    my_file->buffer = malloc(BUFF_SIZE);
    my_file->empty = 1;
    my_file->cursor = my_file->buffer;
    my_file->end_cursor = my_file->buffer;
    my_file->eof = 0;

    switch(*mode)
    {
    case 'r':
        my_file->handler = open(name,O_CREAT|O_RDONLY);
        if(my_file->handler<0)return NULL;
        //my_file->cursor = buffer;
        my_file->mode = READ;
        //my_file->pos = 0;
       // my_file->len = 0;
        return my_file;
    case 'w':
        my_file->handler = open(name,O_CREAT|O_WRONLY|O_TRUNC, 0644);
        if (my_file->handler < 0) return NULL;
        //my_file->cursor = buffer;
        my_file->mode = WRITE;
        //my_file->pos = 0;
        //my_file->len = 0;
        return my_file;
    default:
        return NULL;
    }
}

int my_fclose(MY_FILE *f)
{
    if (f->mode==WRITE)
    {
        //TO DO:write the buffer into file
        int x = f->end_cursor - f->buffer;
        int a = write(f->handler, f->buffer, f->end_cursor - f->buffer);
        //f->len = 0;
        //f->pos = 0:
    }
    
    int r = close(f->handler);
    free(f);
    
    return r;
}

int my_fread(void *p, size_t size, size_t nbelem, MY_FILE *f)
{
    if (f->mode == WRITE)
        return -1;
    if (f->empty) //for first time... buffer is empty
    {
        int count;
        count = read(f->handler,f->buffer,BUFF_SIZE);
        if (count <0) return -1;
        f->end_cursor = (f->cursor + count);
        f->empty = 0;
    }
    if ((size*nbelem)<=(f->end_cursor-f->cursor)) // normal case
    {
        strncpy(p,f->cursor,size*nbelem);
        f->cursor += size*nbelem;
        return nbelem;
    }
    if  ((size*nbelem)<=(f->buffer+ BUFF_SIZE-f->cursor)) //when we have end of file
    {
        //int number = f->cursor-f->end_cursor;
        int number = f->end_cursor-f->cursor;
        number = number/size;
        strncpy(p,f->cursor,number*size);
        f->cursor = f->end_cursor;
        f->eof = 1;
        return number;
    }
    //if (size <BUFF_SIZE) //
    {
        strncpy(p,f->cursor, f->end_cursor-f->cursor);
        char* u_pointer = p + (long int)(f->end_cursor-f->cursor);
        int number_left = size*nbelem - (f->end_cursor-f->cursor);
        int number_read = f->end_cursor-f->cursor;
        int count;
        //
        while(number_left>0)
        {
            f->cursor = f->buffer;
            count = read(f->handler,f->cursor,BUFF_SIZE);
            if(count < 0) return -1;
            f->end_cursor = f->buffer+count;
            if ((number_left)<=(f->end_cursor-f->cursor)) // norml case
            {
                strncpy(u_pointer,f->cursor, number_left);
                u_pointer = f->cursor;
                f->cursor += number_left;
                return nbelem;
            }
            if  ((number_left)<=(f->buffer+ BUFF_SIZE-f->cursor)) //when we have end of file
            {
                strncpy(u_pointer,f->cursor,f->end_cursor-f->cursor);
                int number = f->cursor-f->end_cursor;
                f->cursor = f->end_cursor;
                number_read += number;
                f->eof = 1;
                return number_read/size;
            }
            strncpy(u_pointer,f->cursor,BUFF_SIZE);
            u_pointer += BUFF_SIZE;
            number_left -= BUFF_SIZE;
        }
    }
    return -1;
}


/*int xmy_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f)
{	
	if (f->mode == READ)
		return -1;
	
	taille *= nbelem;
    int act_taille = taille;
	size_t pos = 0;
    while (BUFF_SIZE - f->len - f->pos < act_taille) {
		// fill buffer entirely
        size_t s = BUFF_SIZE - f->len;
		memcpy(f->buffer + f->pos, p + pos, s);
		pos += s;
        act_taille -= s;
		
		// flush it
		int r = write(f->handler, f->buffer, BUFF_SIZE);
		if (r < f->len)
			return r;
		f->len = 0;
		f->pos = 0;
	}

	memcpy(f->buffer + f->pos, p + pos, taille);
    f->len += act_taille;
    f->pos += act_taille;
    pos += act_taille;

	taille /= nbelem;
	return pos / taille;
	
}*/
int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f)
{

    if (f->mode == READ)
        return -1;

    taille *= nbelem;
    int act_taille = taille;
    size_t pos = 0;
    while (f->buffer + BUFF_SIZE - f->end_cursor <= act_taille) {
        // fill buffer entirely
        size_t s = f->buffer + BUFF_SIZE - f->end_cursor;
        memcpy(f->end_cursor, p + pos, s);
        pos += s;
        act_taille -= s;

        // flush it
        int r = write(f->handler, f->buffer, BUFF_SIZE);
        //if (r < f->len)
        //    return r;
        f->end_cursor = f->buffer;
    }

    memcpy(f->end_cursor, p + pos, taille);
    f->end_cursor += act_taille;
    pos += act_taille;

    taille /= nbelem;
    return pos / taille;
}
int my_feof(MY_FILE *f)
{
    return f->eof;
}

int my_fprintf(MY_FILE *f, const char *format, ...)
{
    va_list args;
    va_start(args,format);
    int i = 0;
    char* xchar[10];
    int xint;
    char* xstring;
    int x;
    while( format[i]!='\0')
    {
        if(format[i]!='%') my_fwrite(format+i,1,1,f);
        else
        {
            i++;
            switch(format[i])
            {
                case 'd':
                    xint = va_arg(args, int);
                    x = sprintf(xchar,"%d",xint);
                    my_fwrite(xchar,1,x,f);
                    break;
                case 's':
                    xstring = va_arg(args,char*);
                    x = strlen(xstring);
                    my_fwrite(xstring,1,x,f);
                    break;
                case 'c':
                    xchar[0] = va_arg(args, int);
                    my_fwrite(xchar,1,1,f);
                    break;
                default:return;
            }
        }
        i++;
    }
    va_end(args);
}
int my_fscanf(MY_FILE *f, char *format, ...)
{
    va_list args;
    va_start(args,format);
    int i = 0;
    int elem_read = 0;
    char xchar[INT_DIGIT];
    char* xxchar;
    int xint;
    int* pint;
    char* xstring;
    int x;
    int counter;
    while( format[i]!='\0')
    {
        if(format[i]!='%')
        {
            my_fread(xchar,1,1,f);
            if (xchar != format[i]) return elem_read;
        }
        else
        {
            i++;
            switch(format[i])
            {
                case 'd':
                    counter = 0;
                    do
                    {
                        my_fread(xchar,1,1,f);
                        counter++;
                    }while(isdigit(xchar));
                    //unget
                    counter--;
                    if(counter>INT_DIGIT) break;
                    sscanf(xchar,"%d",&xint);
                    pint =va_arg(args,int*);
                    *pint = xint;
                    elem_read++;
                    break;
                case 's':
                    xstring = va_arg(args,char*);
                    int j =0;
                    do
                    {
                        my_fread(xstring+j,1,1,f);
                        j++;
                    }while(xstring[j-1]!=' ');
                    elem_read++;
                    xstring[j] = '\0';
                    break;
                case 'c':
                    my_fread(xchar,1,1,f);
                    if (xchar==EOF)break;
                    xxchar = va_arg(args,char*);
                    *xxchar = *xchar;
                    break;
                default:return;
            }
        }
        i++;
    }
    va_end(args);
}





