#include "header.h"
directory_entry pos;

void printheader(path p){
    ostringstream head;
    string full_path=string(absolute(p));
    struct stat fileStat;
    stat(p.filename().c_str(), &fileStat);
    struct passwd *pw = getpwuid(fileStat.st_uid);

    head << "\033[1;34m"<<pw->pw_name << "@" << "\033[1;32m" << full_path<<":"<<"\033[0m";

    cout << head.str() << flush;
}

int search_file(path p, string key){
    directory_iterator d_itr(p);
    for(auto e: d_itr){
        if(e.path().filename().string() == key){
            pos = e;
            return 1;
        }
        if(e.is_directory()){
            if(((e.status().permissions()&perms::owner_read) != perms::none)&&search_file(e.path(),key)){
                return 2;
            }
        }
    }
    return 0;
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

bool execute_command(string cmd){
    int cmd_len = cmd.length();
    vector <string> command;
    split_command(command, cmd, cmd_len);

    int end = command.size() -1;
    string loc = command[end];
    int loc_length = loc.length();
    char loc_0 = loc[0];
    bool relative = 0;
    /*TO HANDLE RELATIVE PATHS*/
    if(loc_length  > 1){
        char loc_1 = loc[1];
        if(loc_0 == '.' && loc_1 == '/'){
            loc = string(current_path()) + "/"+ loc.substr(2);
            relative = 1;
        }
        else if(loc_0 == '~' && loc_1 == '/'){
            loc = "/home/" + loc.substr(2);
            relative = 1;
        }
    }
    else if(loc_length == 1){
        if(loc_0 == '.') loc = string(current_path());
        else if(loc_0 == '~') loc = "/home";
        relative = 1;
    }

    string type = command[0];
    char a = type[0];
    switch(a)
    {
    case 'c':
        if(type == "copy"){
            const auto copyOptions = copy_options::overwrite_existing|copy_options::recursive;
            for(int i = 1; i < end; ++i){
                bool flag;
                if(!directory_entry(command[i]).exists()) flag = 0;
                else{
                    if(directory_entry(command[i]).is_regular_file()){
                        cout << "ok1";
                        copy(path(string(current_path())+"/"+command[i]), path(loc),copyOptions);
                    }
                    else if(directory_entry(command[i]).is_directory()){
                        string com = command[i];
                        int token_len = com.length();
                        char token_0 = com[0];
                        if(token_len  > 1){
                            char token_1 = com[1];
                            if(token_0 == '.' && token_1 == '/') com = string(current_path()) + "/"+ com.substr(2);
                            else if(token_0 == '~' && token_1 == '/') com = "/home/" + com.substr(2);
                        }
                        else if(token_len == 1){
                            if(token_0 == '.') com = string(current_path());
                            else if(token_0 == '~') com = "/home";
                        }
                        if(!relative){
                            create_directory(path(loc)/path(command[i]).stem());
                            copy(path(com), path(loc+"/"+string(path(command[i]).stem())),copyOptions);
                        }
                        else{
                            create_directory(path(loc)/command[i]);
                            copy(path(com), path(loc+"/"+command[i]),copyOptions);
                        }
                    }
                    flag = 1;
                }
                if(flag) cout << "\nFile(s) copied successfully to " << loc;
                else cout <<"\nFile/Directory " << command[i] << " doesn't exist in " << loc;
            }
        }
        else if(type == "clear"){
             cout <<  CLEAR;
             return true;
        }
        else if(type == "create_dir"){
                create_directory(current_path()/command[1]);
        }
        else if(type == "create_file"){
            fstream f;
            f.open(command[2] + "/" + command[1], ios::app);
            f.close();
        }
    case 'r':
        if(type == "rename"){
            if(!directory_entry(command[1]).exists())
                cout <<"\nFile/Directory " << command[1] << " doesn't exist in " << current_path() << flush;
            else rename(path(command[1]), path(command[2]));
        }
    case 'd':
        if(type == "del_file"){
            if(!directory_entry(command[1]).exists())
                cout <<"\nFile " << command[1] << " doesn't exist in " << current_path() << flush;
            else{
                remove(path(string(current_path())+"/"+command[1]));
            }
        }
        else if(type == "del_dir"){
            if(!directory_entry(command[1]).exists())
                cout <<"\nDirectory " << command[1] << " doesn't exist in " << current_path() << flush;
            else{
                remove_all(path(string(current_path())+"/"+command[1]));
            }
        }
    case 'g':
        if(type == "goto"){
            if(!directory_entry(command[1]).exists())
                cout <<"\nDirectory " << command[1] << " doesn't exist in " << current_path() ;
            else {
                current_path(absolute(path(command[1])));
            }
        }
    case 's':
        if(type == "search"){
            string position;
            int k = search_file(current_path(),command[1]);
            if(k == 1 || k == 2){
                position = string(pos.path());
                cout << "\nFile found at: " << position ;
            }
            //else if(k == 2) position = string(pos.path());
            else cout << "\nFile not found in " << current_path() ;
        }
    case 'm':
        if(type == "move"){
            if(!directory_entry(command[1]).exists())
                cout <<"\nFile/Directory " << command[1] << " doesn't exist in " << current_path() ;
            else{
                rename(path(command[1]),path(command[2]+"/"+command[1]));
            }
        }
    case 'e':
        if(type == "exit") return false;
    default:
        cout << "\n";
        return true;
    }
    return true;
}

