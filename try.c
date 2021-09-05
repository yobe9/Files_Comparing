
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char* argv[]) {

int firstfd, secondfd;
//firstfd = open(argv[1], O_RDONLY);
if((firstfd = open(argv[1], O_RDONLY)) < 0){
    printf("error\n");
}
if((secondfd = open(argv[2], O_RDONLY)) < 0){
    printf("error\n");
}
char bufferFirst[1];
char bufferSecond[1];

//read(secondfd,bufferSecond,sizeof(bufferSecond));
//read(firstfd,bufferFirst,sizeof(bufferFirst));

//printf("%s\n", bufferFirst);
//printf("%s\n", bufferSecond);

char c, d;
for (int i = 0; i < 30; i++)
{
    if(read(secondfd,&c,1) == 0) {
        //printf("finish\n");
    }
    if (c == '\n')
    {
        //printf("finish\n"); 
    }
    
    read(firstfd,&d,1);

    if (c == d)
    {
        printf("same\n");
    }
    else
    {
        //printf("diff\n");
    }

    if (c == '\n')
    {
        //printf("enlding\n");
    }
    //while (c == ' ')
    {
        //printf("space -%cspace\n", c);
        //read(secondfd,&c,1);
    }
    
    
    
    printf("first - %c\n", d);
    printf("second - %c\n", c);
}



}