#include "header.h"

void printheader(path p){
    ostringstream head;
    string full_path=string(absolute(p));
    struct stat fileStat;
    stat(p.filename().c_str(), &fileStat);
    struct passwd *pw = getpwuid(fileStat.st_uid);

    head << "\033[1;34m"<<pw->pw_name << "@" << "\033[1m" << "\033[1;32m"<<full_path<<":"<<"\033[1m";

    cout << head.str() << flush;
}
void split_command(vector <string> &v, string s, int cmd_len){
    string temp= "";
    int counter = 0;
    for(int i =0; i < cmd_len; i++){
        if(s[i] == '\'') counter++;
        else if(counter%2 == 0 && s[i] == ' '){
            if(!temp.empty()) v.push_back(temp);
            temp = "";
        }
        else temp += s[i];
    }
    if(!temp.empty()) v.push_back(temp);
}

void execute_command(string cmd){
    int cmd_len = cmd.length();
    vector <string> command;
    split_command(command, cmd, cmd_len);
    int end = command.size() -1;
    string loc = command[end];
    int loc_length = loc.length();
    char loc_0 = loc[0], loc_1 = loc[1];
    if(loc_length  > 1){
        if(loc_0 == '.' && loc_1 == '/') loc = string(current_path()) + "/"+ loc.substr(2);
        else if(loc_0 == '~' && loc_1 == '/') loc = "/home/" + loc.substr(2);
    }
    else if(loc_length == 1){
        if(loc_0 == '.') loc = string(current_path());
        else if(loc_1 == '~') loc = "/home";
    }
    string type = command[0];
    char a = type[0];
    switch(a)
    {
    case 'c':
        if(type == "copy"){
        const auto copyOptions = copy_options::overwrite_existing|copy_options::recursive;
        for(int i = 1; i < end; ++i){
            if(directory_entry(command[i]).is_regular_file()){
                copy(path(string(current_path())+"/"+command[i]), path(loc),copyOptions);
            }
            if(directory_entry(command[i]).is_directory()){
                copy(path(command[i]), path(loc),copyOptions);
            }
        }
        cout << "\nFile(s) copied successfully to " << loc << "\n" << flush;
        }
        else if(type == "clear"){
             cout <<  "\033c";
             return;
        }
        else if(type == "create_dir"){
        }
        else if(type == "create_file"){
        }
    case 'r':
        if(type == "rename"){

        }
    }


}

