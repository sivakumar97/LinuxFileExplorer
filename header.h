#ifndef HEADER_H
#define HEADER_H

#include <bits/stdc++.h>
#include <filesystem>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <spawn.h>
#include <sys/wait.h>
using namespace std;
using namespace std::filesystem;
/*For directory operations*/
extern vector<directory_entry> dir;
extern stack<path> backstk;
extern stack<path> frwdstk;
extern path q;
void printDir(path );
void scrollDir(vector<directory_entry> &, int ,int ,int );
string perms_linux(perms );
void print_dirent(directory_entry const& ,bool );
void updateDirEnt(path , int &, int &, int & );

/*For opening a file in nano*/
extern char **environ;
extern char *temp;
extern pid_t pid;
extern int status;
extern char* arg[];
void open_media_pdf(string );
void open_txt(string );
#endif // HEADER_H
