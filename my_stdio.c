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
        int number = count/size;
        void* x = (void*)f->cursor;
        //p = f->cursor;
        p = x;
        p = (void*)f->cursor;
        f->cursor = (f->cursor + count*size);
        return number;
    }
    if ((size*nbelem)<=(f->end_cursor-f->cursor)) // norml case
    {
        p = f->cursor;
        f->cursor += size*nbelem;
        return nbelem;
    }
    if  ((size*nbelem)<=(f->buffer+ BUFF_SIZE-f->cursor)) //when we have end of file
    {
        p = f->cursor;
        int number = f->cursor-f->end_cursor;
        f->cursor = f->end_cursor;
        number = number/size;
        return number;
    }
    //if (size <BUFF_SIZE) //
    {
        strncpy(f->buffer,f->cursor, f->end_cursor-f->cursor);
        f->end_cursor = f->buffer+size;
        int count = f->buffer+BUFF_SIZE-f->end_cursor;
        f->cursor = f->buffer;
        if(read(f->handler,f->end_cursor,count) < 0 )
            return -1;
        if ((size*nbelem)<=(f->end_cursor-f->cursor)) // norml case
        {
            p = f->cursor;
            f->cursor += size*nbelem;
            return nbelem;
        }
        if  ((size*nbelem)<=(f->buffer+ BUFF_SIZE-f->cursor)) //when we have end of file
        {
            p = f->cursor;
            int number = f->cursor-f->end_cursor;
            f->cursor = f->end_cursor;
            number = number/size;
            return number;
        }
        else
        {
            p = f->cursor;
            return BUFF_SIZE/size;
            f->cursor = f->buffer + BUFF_SIZE;
        }
    }
}

/* int my_fread(void *p, size_t size, size_t nbelem, MY_FILE *f) {
	if (f->mode == WRITE)
		return -1;

	size *= nbelem;
	size_t pos = 0;
	while (f->len < size) {
		pos += f->len;
		size -= f->len;
		memcpy(p + pos, f->buffer + f->pos, f->len);

		// fill buffer
		int r = read(f->handler, f->buffer, BUFF_SIZE);
		if (r < 0) {
			return -1;
		} else if (r == 0) {
			f->empty = 1;
			return 0;
		}
		f->pos = 0;
		f->len = r;
	}

	memcpy(p + pos, f->buffer + f->pos, size);
	f->pos += size;
	f->len -= size;
	pos += size;

	size /= nbelem;
	return pos / size;
}*/

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
