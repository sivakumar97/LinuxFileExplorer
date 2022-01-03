#ifndef HEADER_H
#define HEADER_H

#include <bits/stdc++.h>
#include <filesystem>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>

#define CLEAR "\033c"

using namespace std;
using namespace std::filesystem;

/*For directory operations*/
extern path t;
extern vector<directory_entry> dir;
extern stack<path> backstk;
extern stack<path> frwdstk;
extern path q;
void print_directory(path );
void scroll_directory(vector<directory_entry> &, int ,int ,int );
string perms_linux(perms );
void print_directory_entry(directory_entry const& ,bool );
void update_directory_entry(path , int &, int &, int & );
bool enter_command_mode(path );

/*For opening a file in nano*/
extern char **environ;
extern char *temp;
extern pid_t pid;
extern int status;
extern char* arg[];
void open_media_pdf(string );
void open_txt(string );


/*For command mode*/
void printheader(path );
void split_command(vector <string> &, string , int);
void execute_command(string );



#endif // HEADER_H
