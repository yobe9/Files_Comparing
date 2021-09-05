// Yoav Berger

#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

int main(int argc,char* argv[]) {
    //arguments
    int fd1;
    char * failed;

    //open the configuration file
    if((fd1 = open(argv[1], O_RDONLY)) < 0){
        failed = "Error‬‬ ‫‪in: read";
        if(write(2,failed,strlen(failed))== -1){
            return -1;
        }
        return -1;
    }

    //handle if given relative path and save as full path
    char parentPath[150];
    memset(parentPath,'\0', 150);
    char * tokTemp;
    char argCpy[150];
    char oldtemp[150];
    strcpy(argCpy, argv[1]);
    tokTemp = strtok(argCpy, "/");
    //parentPath[0] = '/';
    while (tokTemp != NULL)
    {
        strcpy(oldtemp, tokTemp);
        strcat(parentPath, "/");
        
        tokTemp = strtok(NULL, "/");
        if (tokTemp != NULL)
        {
            strcat(parentPath, oldtemp);
        }
        
    }
    //("full - %s\n", parentPath);

    //do stat to get the file size so we could read him
    struct stat statConf;
    if (stat(argv[1], &statConf) ==-1){
        failed="Error‬‬ ‫‪in: Stat";
        if(write(2,failed,strlen(failed))== -1){ }
        //  in case failed
        if (close(fd1) == -1){
            failed = "Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){
                return -1;
            }
        }
        return -1;
    }

    //create buffer for stat file
    char * bufferConf;
    bufferConf = (char*)malloc(statConf.st_size);

    //read file into buffer
    if(read(fd1,bufferConf,statConf.st_size)< 0){
        // in case failed
        failed = "Error‬‬ ‫‪in: Read";
        if(write(2,failed,strlen(failed))== -1){ }
        if (close(fd1) ==-1){
            failed = "Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        free(bufferConf);
        return -1;
    }
    
    //strtok to the buffer to split to 3 strings - at end corect paths will be the CPY chars
    char * folderDir;
    char * input;
    char * output;
    folderDir = strtok(bufferConf, "\n");
    input = strtok(NULL, "\n");
    output = strtok(NULL, "\n");
    char folderDirCpy[150];
    strcpy(folderDirCpy, folderDir);
    char inputCpy[150];
    strcpy(inputCpy, input);
    char outputCpy[150];
    strcpy(outputCpy, output);
    //check if given relative or full path for first file
    if (folderDirCpy[0] != '/')
    {
        char parentCpy[150];
        strcpy(parentCpy,parentPath);
        strcat(parentCpy, folderDirCpy);
        strcpy(folderDirCpy, parentCpy);
    }
    //check if given relative or full path for first file
    if (inputCpy[0] != '/')
    {
        char parentCpy[150];
        strcpy(parentCpy,parentPath);
        strcat(parentCpy, inputCpy);
        strcpy(inputCpy, parentCpy);
    }
    //check if given relative or full path for first file
    if (outputCpy[0] != '/')
    {
        char parentCpy[150];
        strcpy(parentCpy,parentPath);
        strcat(parentCpy, outputCpy);
        strcpy(outputCpy, parentCpy);
    }
    // ("old relative - %s, full - %s\n", folderDirCpy, parentPath);
    // ("old relative - %s, full - %s\n", inputCpy, parentPath);
    // ("old relative - %s, full - %s\n", outputCpy, parentPath);

    //check if given directory is a real directory and exists
    //first do stat for the directory path
    struct stat folderStat;
    if (stat(folderDirCpy, &folderStat) ==- 1){
        // Write that stat faild and close fd and free malloc before exit.
        failed = "Error‬‬ ‫‪in: Stat";
        if(write(2,failed,strlen(failed))== -1){ }
        if (close(fd1) ==-1){
            failed = "Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        free(bufferConf);
        return -1;
    }
    //check directory valid
    if(access(folderDirCpy,F_OK) == -1 || !S_ISDIR(folderStat.st_mode)){
        failed = "Not a valid directory\n";
        if(write(2,failed,strlen(failed))== -1){
        }
        if (close(fd1) == -1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        free(bufferConf);
        return -1;
    }
    // open the given directory
    DIR *parentDir;
    if((parentDir = opendir(folderDirCpy)) == NULL){
        failed = "Error‬‬ ‫‪in: opendir";
        if(write(2,failed,strlen(failed)) == -1){ }
        if (close(fd1) == -1){
            failed = "Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        free(bufferConf);
        return -1;
    }

    //creating output file for the compile files that will be created to compare to curent output
    char workingPath[150];
    if(getcwd(workingPath, sizeof(workingPath)) == NULL){
        //in case failed
        failed = "Error‬‬ ‫‪in: getcwd";
        if(write(2,failed,strlen(failed))== -1){ }
        if (close(fd1) ==-1){
            failed = "Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (closedir(parentDir) != 0)
        {
            failed = "Error‬‬ ‫‪in: closedir";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        
        free(bufferConf);
        return -1;
    }
    // concat /res.txt to current dir so we would have the path to the new file
    char resPath[150];
    strcpy(resPath,workingPath);
    strcat(resPath, "/res.txt");
    
    //creating the csv file
    int csvFd;
    csvFd = open("results.csv",(O_WRONLY|O_CREAT|O_TRUNC),(S_IRUSR|S_IWUSR));
    if(csvFd < 0) {
        failed = "Error‬‬ ‫‪in: open";
        if(write(2,failed,strlen(failed))== -1){ }
        if (close(fd1) == -1){
            failed = "Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (closedir(parentDir) != 0)
        {
            failed = "Error‬‬ ‫‪in: closedir";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        free(bufferConf);
        return -1;
    }
    //creating res.txt at the path above
    int resFd;
    resFd = open("res.txt", (O_WRONLY|O_CREAT |O_TRUNC),(S_IRUSR | S_IWUSR));
    if(resFd<0) {
        failed = "Error‬‬ ‫‪in: open";
        if(write(2,failed,strlen(failed))== -1){
        }
        if (close(fd1) == -1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(csvFd) == -1){
            failed="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (closedir(parentDir) != 0)
        {
            failed = "Error‬‬ ‫‪in: closedir";
            if(write(2,failed,strlen(failed))== -1){ }
        }

        free(bufferConf);
        return -1;
    }

    //opening the output file from the user
    int outputFd;
    outputFd = open(outputCpy, O_RDONLY);
    if(outputFd<0) {
        failed ="Output File not exist\n";
        if(write(2,failed,strlen(failed))== -1){ }
        if (close(fd1) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(csvFd) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(resFd) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if(remove(resPath) < 0) {
            failed ="Error‬‬ ‫‪in: remove";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (closedir(parentDir) != 0)
        {
            failed = "Error‬‬ ‫‪in: closedir";
            if(write(2,failed,strlen(failed))== -1){ }
        }

        free(bufferConf);
        return -1;
    }

    //opening the input file from the user
    int inputFd;
    inputFd = open(inputCpy, O_RDONLY);
    if(inputFd < 0) {
        failed ="Input File not exist\n";
        if(write(2,failed,strlen(failed))== -1){ }
        if (close(fd1) ==-1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(csvFd) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(resFd) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if(remove(resPath) < 0) {
            failed ="Error‬‬ ‫‪in: remove";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(outputFd) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (closedir(parentDir) != 0)
        {
            failed = "Error‬‬ ‫‪in: closedir";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        
        free(bufferConf);
        return -1;
    }

    //creating error.txt to write there the errors that will be created
    char errorFilePath[150];
    strcpy(errorFilePath, workingPath);
    strcat(errorFilePath, "/");
    strcat(errorFilePath, "error.txt");
    int errorFd;
    errorFd = open(errorFilePath, (O_WRONLY|O_CREAT |O_TRUNC),(S_IRUSR | S_IWUSR));
    if(errorFd < 0) {
        failed ="Error‬‬ ‫‪in: open";
        if(write(2,failed,strlen(failed))== -1){ }
        if (close(fd1) ==-1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(csvFd) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(resFd) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(outputFd) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (close(inputFd) == -1){
            failed ="Error‬‬ ‫‪in: close";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if(remove(resPath) < 0) {
            failed ="Error‬‬ ‫‪in: remove";
            if(write(2,failed,strlen(failed))== -1){ }
        }
        if (closedir(parentDir) != 0)
        {
            failed = "Error‬‬ ‫‪in: closedir";
            if(write(2,failed,strlen(failed))== -1){ }
        }

        free(bufferConf);
        return -1;        

    }

    //opening the dir and starting iteration on the subdirs
    struct dirent *dirnetMain;
    char * subdirName;
    char * subdirFileName;
    char * lastTwoLetters;
    char * excellentMessage = "100,EXCELLENT\n";
    char * diffrentMessage = "50,WRONG\n";
    char * similarMessage = "75,SIMILAR\n";
    int isCProgExist = 0;
    int fileNameLen;
    int waitCompileChild, statusCompileChild, waitTimeChild, statusTimeChild,waitCompareChild,statusCompareChild;
    int compileExitStatus;
    char subdirPath[150];
    char runFilePath[150];
    char subdirFilePath[150];
    struct stat subdirStat;
    DIR * subdirPtr;
    struct dirent * SubdirentPtr;
    pid_t compileChildPid;
    pid_t timeChildPid;
    pid_t compareChildPid;
    time_t startTime, stopTime;
    while((dirnetMain = readdir(parentDir)) != NULL){
        //getting the name of the subdir and skip it in case it .||..
        subdirName = dirnetMain->d_name;
        if(strcmp(subdirName, ".") == 0 || strcmp(subdirName,"..") == 0){
            continue;
        }
        //reset cflag in the new subdir
        isCProgExist = 0;
        //initialize the path of the new student dir
        memset(subdirPath,0,150);
        strcpy(subdirPath,folderDirCpy);
        strcat(subdirPath, "/");
        strcat(subdirPath, subdirName);
        //intialize the path of the new run file
        memset(runFilePath,0,150);
        strcpy(runFilePath,subdirPath);
        strcat(runFilePath, "/");
        strcat(runFilePath, "a.out");

        //checking if the subdir is really a directory
        if (stat(subdirPath, &subdirStat) ==-1){
            failed = "Error‬‬ ‫‪in: stat";
            if(write(2,failed,strlen(failed))== -1){ }
            continue;
        }
        if(S_ISDIR(subdirStat.st_mode)){
            //change directory to the subdir
            if (chdir(subdirPath) != 0){
                failed = "Error‬‬ ‫‪in: chdir";
                if(write(2,failed,strlen(failed)) == -1){
                }
                continue;
            }
            //open subdir
            if((subdirPtr = opendir(subdirPath)) == NULL){
                failed = "Error‬‬ ‫‪in: opendir";
                if(write(2,failed,strlen(failed))== -1){ }
                continue;
            }

            //iterate over files in subdir
            while((SubdirentPtr = readdir(subdirPtr)) != NULL) {
                //extracting files name and their path
                subdirFileName = SubdirentPtr->d_name;
                memset(subdirFilePath,0,150);
                strcpy(subdirFilePath,subdirPath);
                strcat(subdirFilePath, "/");
                strcat(subdirFilePath, subdirFileName);

                //check if file is . or ..
                if (strcmp(subdirFileName, ".") == 0 || strcmp(subdirFileName, "..") == 0) {
                    continue;
                }

                //check if the file is end up with .c
                fileNameLen = strlen(subdirFileName);
                lastTwoLetters = subdirFileName +( fileNameLen-2);
                // Check if this is a c file.
                if(strcmp(lastTwoLetters, ".c") == 0){
                    //set flag on
                    isCProgExist = 1;

                    //setting the output of the program to error.txt
                    if(dup2(errorFd,2) < 0){
                        failed = "Error‬‬ ‫‪in: dup2";
                        if(write(2,failed,strlen(failed))== -1){ }
                        continue;
                    }

                    //compiling the file in diffrent process
                    compileChildPid = fork();
                    // check if failed
                    if(compileChildPid < 0){
                        failed = "Error‬‬ ‫‪in: fork";
                        if(write(2,failed,strlen(failed))== -1){ }
                        continue;
                    }
                    else if(compileChildPid == 0){
                        //child runs compile
                        char* arg[] = {"gcc",subdirFileName, NULL};
                        if(execvp(arg[0],arg) < 0){
                            failed="Error‬‬ ‫‪in: execvp";
                            if(write(2,failed,strlen(failed)) == -1){ }
                            continue;
                        }
                    }
                    else{
                        //father wait until child finish and check return status
                        waitCompileChild = waitpid(compileChildPid, &statusCompileChild, 0);
                        if(waitCompileChild < 0){
                            failed = "Error‬‬ ‫‪in: wait";
                            if(write(2,failed,strlen(failed))== -1){ }
                            continue;
                        }
                        
                        //check exit status to check if compilation succeed
                        compileExitStatus = WEXITSTATUS(statusCompileChild);
                        //in case of compile error
                        if(compileExitStatus == 1){
                            //write the right result in the CSV
                            if(write(csvFd, subdirName,strlen(subdirName))==-1){
                                failed = "Error‬‬ ‫‪in: write";
                                if(write(2,failed,strlen(failed))== -1){ }
                                continue;
                            }
                            if(write(csvFd,",",strlen(",")) == -1){
                                failed = "Error‬‬ ‫‪in: write";
                                if(write(2,failed,strlen(failed))== -1){ }
                                continue;
                            }
                            if(write(csvFd, "10,COMPILATION_ERROR\n",strlen("10,COMPILATION_ERROR\n")) == -1){
                                failed = "Error‬‬ ‫‪in: write";
                                if(write(2,failed,strlen(failed))== -1){ }
                                continue;
                            }
                            continue;
                        }

                        //initialize time to check timeout of the program and create fork to run it
                        time (&startTime);
                        timeChildPid = fork();
                        //in case failed
                        if(timeChildPid < 0){
                            failed = "Error‬‬ ‫‪in: fork";
                            if(write(2,failed,strlen(failed))== -1){ }
                            continue;
                        }
                        //in case we in child
                        else if(timeChildPid == 0) {
                            //setting the input for the function
                            if(dup2(inputFd,0) < 0){
                                failed = "Error‬‬ ‫‪in: dup2";
                                if(write(2,failed,strlen(failed))== -1){ }
                                continue;
                            }
                            //setting the output for the function
                            if(dup2(resFd, 1) < 0 ){
                                failed = "Error‬‬ ‫‪in: dup2";
                                if(write(2,failed,strlen(failed))== -1){ }
                                continue;
                            }
                            // run the compiled file
                            char* arg2[] = {"./a.out", NULL};
                            if(execvp(arg2[0],arg2)<0){
                                failed="Error‬‬ ‫‪in: execvp";
                                if(write(2,failed,strlen(failed))== -1){}
                                continue;
                            }
                        }
                        //in case we in father process
                        else{
                            waitTimeChild = waitpid(timeChildPid, &statusTimeChild,0);
                            if(waitTimeChild < 0){
                                failed="Error‬‬ ‫‪in: wait";
                                if(write(2,failed,strlen(failed))== -1){ }
                                continue;
                            }

                            //saving the finish time of the program and check if bigger than 5
                            time (&stopTime);
                            if(difftime (stopTime,startTime) > 5){
                                //writing the according information
                                if(write(csvFd, subdirName,strlen(subdirName)) ==-1){
                                    failed = "Error‬‬ ‫‪in: write";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                if(write(csvFd,",",strlen(",")) == -1){
                                    failed = "Error‬‬ ‫‪in: write";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                if(write(csvFd, "20,TIMEOUT\n",strlen("20,TIMEOUT\n"))==-1){
                                    failed = "Error‬‬ ‫‪in: write";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                //remove a.out file for the run of the timeout
                                if(remove(runFilePath) < 0) {
                                    failed = "Error‬‬ ‫‪in: remove";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                continue;
                            }
                            //remove a.out file for the run out of the timeout
                            if(remove(runFilePath) < 0) {
                                failed = "Error‬‬ ‫‪in: remove";
                                if(write(2,failed,strlen(failed))== -1){ }
                                continue;
                            }
                            
                            //runing the program to chech the result of the comparasion
                            compareChildPid = fork();
                            if(compareChildPid < 0){
                                failed = "Error‬‬ ‫‪in: fork";
                                if(write(2,failed,strlen(failed))== -1){ }
                                continue;
                            }
                            //in case we in child
                            else if(compareChildPid == 0){
                                //go to the comp file directory
                                chdir(workingPath);
                                //run comp with the out.txt and outputfile path as arguments
                                //printf("result path = %s, output path = %s\n", resPath, outputCpy);
                                char* args3[] = {"./comp.out", resPath, outputCpy, NULL};
                                if(execvp(args3[0],args3) < 0){
                                    failed = "Error‬‬ ‫‪in: execvp";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }

                            } else{
                                //in case we in father, wait for child
                                waitCompareChild = waitpid(compareChildPid, &statusCompareChild,0);
                                if(waitCompareChild < 0){
                                    failed = "Error‬‬ ‫‪in: wait";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                
                                //check which result came back from comp and write in CSV
                                char * writeStatus;
                                //printf("exit stat - %d\n", WEXITSTATUS(statusCompareChild));
                                switch (WEXITSTATUS(statusCompareChild))
                                {
                                case 1:
                                    writeStatus = excellentMessage;
                                    break;
                                case 2:
                                    writeStatus = diffrentMessage;
                                    break;
                                case 3:
                                    writeStatus = similarMessage;
                                    break;

                                default:
                                    break;
                                }

                                //write in CSV
                                if(write(csvFd, subdirName,strlen(subdirName)) == -1){
                                    failed = "Error‬‬ ‫‪in: write";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                if(write(csvFd,",",strlen(",")) == -1){
                                    failed = "Error‬‬ ‫‪in: write";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                if(write(csvFd, writeStatus,strlen(writeStatus)) == -1){
                                   failed = "Error‬‬ ‫‪in: write";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                if(lseek(inputFd,0,SEEK_SET)<0){
                                    failed = "Error‬‬ ‫‪in: lseek";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                //closing the old res.txt file, removing it and creating new one
                                if(close(resFd) < 0) {
                                    failed = "Error‬‬ ‫‪in: close";
                                    if(write(2,failed,strlen(failed))== -1){ }
                                    continue;
                                }
                                chdir(resPath);
                                if(remove(resPath) < 0) {
                                    failed = "Error‬‬ ‫‪in: remove";
                                    if(write(2,failed,strlen(failed)) == -1){ }
                                    continue;
                                }
                                resFd = open(resPath, (O_WRONLY|O_CREAT |O_TRUNC),(S_IRUSR | S_IWUSR));
                                if(resFd < 0){
                                    failed = "Error‬‬ ‫‪in: open";
                                    if(write(2,failed,strlen(failed)) == -1){ }
                                    continue;

                                }
                            }
                        }
                    }
                }
            }

            //check if the folder doesn't have c program in it and wtite it in csv
            if(isCProgExist == 0){
                if(write(csvFd, subdirName,strlen(subdirName)) == -1){
                    failed = "Error‬‬ ‫‪in: write";
                    if(write(2,failed,strlen(failed))== -1){ }
                    continue;
                }
                if(write(csvFd,",",strlen(",")) == -1){
                    failed = "Error‬‬ ‫‪in: write";
                    if(write(2,failed,strlen(failed))== -1){ }
                    continue;
                }
                if(write(csvFd, "0,NO_C_FILE\n",strlen("0,NO_C_FILE\n"))==-1){
                    failed = "Error‬‬ ‫‪in: write";
                    if(write(2,failed,strlen(failed))== -1){ }
                    continue;
                }
                if(lseek(inputFd,0,SEEK_SET)<0){
                    failed = "Error‬‬ ‫‪in: lseek";
                    if(write(2,failed,strlen(failed))== -1){ }
                    continue;
                }
                //closing res.txt removing it, and creating new one
                if(close(resFd) < 0) {
                    failed = "Error‬‬ ‫‪in: close";
                    if(write(2,failed,strlen(failed))== -1){ }
                    continue;
                }
                chdir(resPath);
                if(remove(resPath) < 0) {
                    failed = "Error‬‬ ‫‪in: remove";
                    if(write(2,failed,strlen(failed)) == -1){ }
                    continue;
                }
                resFd = open(resPath, (O_WRONLY|O_CREAT |O_TRUNC),(S_IRUSR | S_IWUSR));
                if(resFd < 0){
                    failed = "Error‬‬ ‫‪in: open";
                    if(write(2,failed,strlen(failed)) == -1){ }
                    continue;
                }
                continue;

            }
        }
    }
    
    //closing and removing the files that we work with during the program
    if (close(fd1) ==-1){
        failed ="Error‬‬ ‫‪in: close";
        if(write(2,failed,strlen(failed))== -1){ }
        return -1;
    }
    if (close(csvFd) == -1){
        failed ="Error‬‬ ‫‪in: close";
        if(write(2,failed,strlen(failed))== -1){ }
        return -1;
    }
    if (close(resFd) == -1){
        failed ="Error‬‬ ‫‪in: close";
        if(write(2,failed,strlen(failed))== -1){ }
        return -1;
    }
    if (close(outputFd) == -1){
        failed ="Error‬‬ ‫‪in: close";
        if(write(2,failed,strlen(failed))== -1){ }
        return -1;
    }
    if (close(inputFd) == -1){
        failed ="Error‬‬ ‫‪in: close";
        if(write(2,failed,strlen(failed))== -1){ }
        return -1;
    }
    if (close(errorFd) == -1){
        failed ="Error‬‬ ‫‪in: close";
        if(write(2,failed,strlen(failed))== -1){ }
        return -1;
    }
    if(remove(resPath) < 0) {
        failed ="Error‬‬ ‫‪in: remove";
        if(write(2,failed,strlen(failed))== -1){ }
        return -1;
    }
    if (closedir(parentDir) != 0)
    {
        failed = "Error‬‬ ‫‪in: closedir";
        if(write(2,failed,strlen(failed))== -1){ }
        return -1;
    }
    if (closedir(subdirPtr) != 0)
    {
        failed = "Error‬‬ ‫‪in: closedir";
        if(write(2,failed,strlen(failed))== -1){ }
        return -1;
    }

    free(bufferConf);

}
