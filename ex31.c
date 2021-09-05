// Yoav Berger

#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char* argv[]) {
    //opening the files and return -1 if failed
    int fd1, fd2;
    char * failed;

    // Open first path on read mode and check if faild.
    if((fd1 = open(argv[1], O_RDONLY)) < 0){
        failed="‫‪Error‬‬ ‫‪in:‬‬ ‫‪open";
        //writing to the screen
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        return -1;
    }

    // Open second path on read mode and check if faild.
    if((fd2 = open(argv[2], O_RDONLY)) < 0){
        failed="‫‪Error‬‬ ‫‪in:‬‬ ‫‪open";
        //writing to the screen
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        //closing first file
        if (close(fd1) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
            return -1;
        }
        return -1;
    }

    //getting the size of the files
    struct stat stat1;
    struct stat stat2;

    //getting stat of first file
     if (stat(argv[1], &stat1) ==-1){
        failed="Error‬‬ ‫‪in: stat";
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        if (close(fd1) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        if (close(fd2) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        return -1;
    }

    //getting stat of second file
    if (stat(argv[2], &stat2) ==-1){
        failed="Error‬‬ ‫‪in: stat";
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        if (close(fd1) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        if (close(fd2) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        return -1;
    }

    //allocate 2 buffers to read the files
    char * buffer1 = (char*)malloc(stat1.st_size);
    char * buffer2 = (char*)malloc((stat2.st_size));

    //read first file to buffer
    if(read(fd1,buffer1,stat1.st_size)< 0){
        free(buffer1);
        free(buffer2);
        failed="Error‬‬ ‫‪in: read";
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        if (close(fd1) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        if (close(fd2) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        return -1;
    }

    //read second file to buffer
    if(read(fd2,buffer2,stat2.st_size)< 0){
        free(buffer1);
        free(buffer2);
        failed="Error‬‬ ‫‪in: read";
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        if (close(fd1) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        if (close(fd2) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        return -1;
    }

    //compare the two buffers to check for identical
    if(strcmp(buffer1,buffer2) == 0){
        free(buffer1);
        free(buffer2);
        if (close(fd1) ==-1){
            failed = "Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        if (close(fd2) ==-1){
            failed =" Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        return 1;
    }

    //checking similarity and diffrence
    //check who is bigger file
    int biggerFileSize;
    if (strlen(buffer1) > strlen(buffer2))
    {
        biggerFileSize = strlen(buffer1);
    }
    else
    {
        biggerFileSize = strlen(buffer2);
    }
    


    //*********************************************************************************************
    //checking similarity - starting by free old files and read from the start
    free(buffer1);
    free(buffer2);
    if (close(fd1) ==-1){
        failed = "Error‬‬ ‫‪in: close";
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
    }
    if (close(fd2) ==-1){
        failed =" Error‬‬ ‫‪in: close";
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
    }
    // Open first path on read mode and check if faild.
    if((fd1 = open(argv[1], O_RDONLY)) < 0){
        failed="‫‪Error‬‬ ‫‪in:‬‬ ‫‪open";
        //writing to the screen
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        return -1;
    }
    // Open second path on read mode and check if faild.
    if((fd2 = open(argv[2], O_RDONLY)) < 0){
        failed="‫‪Error‬‬ ‫‪in:‬‬ ‫‪open";
        //writing to the screen
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        //closing first file
        if (close(fd1) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
            return -1;
        }
        return -1;
    }
    
    char firstFileChar, secondFileChar;
    int readRet1, readRet2;
    int diffascii = 32;
    
    //reading again char by char
    readRet1 = read(fd1,&firstFileChar,1);
    if (readRet1 < 0)
    {
        failed="Error‬‬ ‫‪in: read";
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        if (close(fd1) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        if (close(fd2) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        return -1;
    }
    
    readRet2 = read(fd2,&secondFileChar,1);
    if (readRet2 < 0)
    {
        failed="Error‬‬ ‫‪in: read";
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        if (close(fd1) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        if (close(fd2) ==-1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        return -1;
    }
    
    //printf("first - %c, second - %c", firstFileChar, secondFileChar);
    int i;
    for (i = 0; i < biggerFileSize; i++)
    {
        //if first file char is ' ' or '\n'
        while (firstFileChar == ' ' || firstFileChar == '\n')
        {
            //if arrived end of first file 
            if (readRet1 == 0)
            {
                break;
            }
            readRet1 = read(fd1,&firstFileChar,1);
            if (readRet1 < 0)
            {
                failed="Error‬‬ ‫‪in: read";
                if(write(2,failed,strlen(failed))== -1){
                    return -1;
                }
                if (close(fd1) ==-1){
                    failed="Error‬‬ ‫‪in: close";
                    if(write(2,failed,strlen(failed))== -1){
                        return -1;
                    }
                }
                if (close(fd2) ==-1){
                    failed="Error‬‬ ‫‪in: close";
                    if(write(2,failed,strlen(failed))== -1){
                        return -1;
                    }
                }
                return -1;
            }
        }
        
        //if second file char is ' ' or '\n'
        while (secondFileChar == ' ' || secondFileChar == '\n')
        {
            //if arrived end of second file
            if (readRet2 == 0)
            {
                break;
            }
            readRet2 = read(fd2,&secondFileChar,1);
            if (readRet2 < 0)
            {
                failed="Error‬‬ ‫‪in: read";
                if(write(2,failed,strlen(failed))== -1){
                    return -1;
                }
                if (close(fd1) ==-1){
                    failed="Error‬‬ ‫‪in: close";
                    if(write(2,failed,strlen(failed))== -1){
                        return -1;
                    }
                }
                if (close(fd2) ==-1){
                    failed="Error‬‬ ‫‪in: close";
                    if(write(2,failed,strlen(failed))== -1){
                        return -1;
                    }
                }
                return -1;
            }
        }

        //if arrived end of both files so similar
        if (readRet1 == 0 && readRet2 == 0)
        {
            if (close(fd1) ==-1){
                failed="Error‬‬ ‫‪in: close";
                if(write(2,failed,strlen(failed))== -1){
                    return -1;
                }
            }
            if (close(fd2) ==-1){
                failed="Error‬‬ ‫‪in: close";
                if(write(2,failed,strlen(failed))== -1){
                    return -1;
                }
            }

            return 3;
        }

        //in case chars are diffrent
        if (firstFileChar != secondFileChar && abs(secondFileChar - firstFileChar) != diffascii)
        {
            if (close(fd1) ==-1){
                failed="Error‬‬ ‫‪in: close";
                if(write(2,failed,strlen(failed))== -1){
                    return -1;
                }
            }
            if (close(fd2) ==-1){
                failed="Error‬‬ ‫‪in: close";
                if(write(2,failed,strlen(failed))== -1){
                    return -1;
                }
            }
            //printf("first - %c, second - %c round %d\n", firstFileChar, secondFileChar, i);
            return 2;
        }

        //in case chars are equals
        if (firstFileChar == secondFileChar || abs(secondFileChar - firstFileChar) == diffascii)
        {
            //printf("first - %c, second - %c round %d\n", firstFileChar, secondFileChar, i);
            //reading next char in both files
            readRet1 = read(fd1,&firstFileChar,1);
            if (readRet1 < 0)
            {
                failed="Error‬‬ ‫‪in: read";
                if(write(2,failed,strlen(failed))== -1){
                    return -1;
                }
                if (close(fd1) ==-1){
                    failed="Error‬‬ ‫‪in: close";
                    if(write(2,failed,strlen(failed))== -1){
                        return -1;
                    }
                }
                if (close(fd2) ==-1){
                    failed="Error‬‬ ‫‪in: close";
                    if(write(2,failed,strlen(failed))== -1){
                        return -1;
                    }
                }
                return -1;
            }
            readRet2 = read(fd2,&secondFileChar,1);
            if (readRet2 < 0)
            {
                failed="Error‬‬ ‫‪in: read";
                if(write(2,failed,strlen(failed))== -1){
                    return -1;
                }
                if (close(fd1) ==-1){
                    failed="Error‬‬ ‫‪in: close";
                    if(write(2,failed,strlen(failed))== -1){
                        return -1;
                    }
                }
                if (close(fd2) ==-1){
                    failed="Error‬‬ ‫‪in: close";
                    if(write(2,failed,strlen(failed))== -1){
                        return -1;
                    }
                }
                return -1;
            }
        }
    }
    

}
