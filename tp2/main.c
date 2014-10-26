#include <stdio.h>
#include "my_stdio.h"

int main(void)
{
    printf("Hello World!\n");
    MY_FILE* f = my_fopen("toto.txt","r");
    char* buff  = malloc(30*sizeof(char));
    fflush(stdout);
    if(my_fread((void*)buff,1,2,f) == -1) printf("error\n");
    int i=0;
    for(i=0;i<30;i++)
    {
        printf("%d %c \n",i,buff[i]);
    }
    my_fclose(f);
    MY_FILE* f2 = my_fopen("toto2.txt","w");
    //my_fprintf(f2,"aho%cahoj%d",'j',7);
    my_fprintf(f2,"%s","ahoj");
    my_fclose(f2);

    return 0;
}

