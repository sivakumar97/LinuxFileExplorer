#ifndef HEADER_H
#define HEADER_H

#include <bits/stdc++.h>
#include <filesystem>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <termios.h>
#include <sys/ioctl.h>
using namespace std;
using namespace std::filesystem;
/*
For directory operations
*/

void printDir(path p);
void scrollDir(vector<directory_entry> &dir, int first,int last,int curr);
string perms_linux(perms p);
void print_dirent(directory_entry const& d,bool color);

/*
For opening a file in nano
*/


#endif // HEADER_H
