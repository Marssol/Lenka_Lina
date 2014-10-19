#include "string.h"
#include "my_stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFF_SIZE 5

MY_FILE* my_fopen(char *name, char *mode)
{
    MY_FILE* my_file = malloc(sizeof(MY_FILE));
    my_file->buffer = malloc(BUFF_SIZE);
    my_file->empty = 1;
    my_file->cursor = my_file->buffer;
    my_file->end_cursor = my_file->buffer;

    switch(*mode)
    {
    case 'r':
        my_file->handler = open(name,O_CREAT|O_RDONLY);
        if(my_file->handler<0)return NULL;
        //my_file->cursor = buffer;
        my_file->mode = READ;
        my_file->pos = 0;
        my_file->len = 0;
        return my_file;
    case 'w':
        my_file->handler = open(name,O_CREAT|O_WRONLY|O_TRUNC, 0644);
        if (my_file->handler < 0) return NULL;
        //my_file->cursor = buffer;
        my_file->mode = WRITE;
        my_file->pos = 0;
        my_file->len = 0;
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
        int a = write(f->handler, f->buffer, f->len);
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
        int number = f->cursor-f->end_cursor;
        number = number/size;
        strncpy(p,f->cursor,number*size);
        f->cursor = f->end_cursor;
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
                return number_read/size;
            }
            strncpy(u_pointer,f->cursor,BUFF_SIZE);
            u_pointer += BUFF_SIZE;
            number_left -= BUFF_SIZE;
        }
    }
    return -1;
}


int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f)
{	
	if (f->mode == READ)
		return -1;
	
	taille *= nbelem;
	size_t pos = 0;
	while (BUFF_SIZE - f->len - f->pos < size) {
		// fill buffer entirely
		size_t s = BUFF_SIZE - f->len - f->pos;
		memcpy(f->buffer + f->pos, p + pos, s);
		pos += s;
		taille -= s;
		
		// flush it
		int r = write(f->handler, f->buffer, BUFF_SIZE);
		if (r < f->len)
			return r;
		f->len = 0;
		f->pos = 0;
	}

	memcpy(f->buffer + f->pos, p + pos, taille);
	f->len += taille;
	f->pos += taille;
	pos += taille;

	taille /= nbelem;
	return pos / taille;
	
}
