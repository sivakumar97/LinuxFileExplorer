#include "header.h"

void open_media_pdf(string fname){
    pid_t pid;
    char *n = (char*)"xdg-open";
    char *temp = new char[fname.size()+1]; //extra 1 space for '\0'
    strcpy(temp,fname.c_str());
    char *argv[]  = {n,temp,NULL};
    int status = posix_spawn(&pid,"/bin/xdg-open",NULL,NULL,argv,environ);
    if(status == 0) waitpid(pid, &status, 0);
    else cout << strerror(status) << "\n";
}


void open_txt(string fname){
    pid_t pid;
    char *n = (char*)"nano";
    char *temp = new char[fname.size()+1]; //extra 1 space for '\0'
    strcpy(temp,fname.c_str());
    char *argv[]  = {n,temp,NULL};
    int status = posix_spawn(&pid,"/bin/nano",NULL,NULL,argv,environ);

    if(status == 0) waitpid(pid, &status, 0);
    else cout << strerror(status) << "\n";
}

