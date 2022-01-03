#include "header.h"
path q;
vector<directory_entry> dir;
stack<path> backstk;
stack<path> frwdstk;

string perms_linux(perms p){
	ostringstream ss;
	ss << ((p & perms::owner_read) != perms::none ? "r" : "-")
		<< ((p & perms::owner_write) != perms::none ? "w" : "-")
		<< ((p & perms::owner_exec) != perms::none ? "x" : "-")
		<< ((p & perms::group_read) != perms::none ? "r" : "-")
		<< ((p & perms::group_write) != perms::none ? "w" : "-")
		<< ((p & perms::group_exec) != perms::none ? "x" : "-")
		<< ((p & perms::others_read) != perms::none ? "r" : "-")
		<< ((p & perms::others_write) != perms::none ? "w" : "-")
		<< ((p & perms::others_exec) != perms::none ? "x" : "-");
	return ss.str();
}

string file_size(double size) {
	ostringstream ss;
	string PreFix = "B";
	if (size > 1000) {
		size /= 1000;
		PreFix = "KB";
	}
	if (size > 1000) {
		size /= 1000;
		PreFix = "MB";
	}
	if (size > 1000) {
		size /= 1000;
		PreFix = "GB";
	}
	if (size > 1000) {
		size /= 1000;
		PreFix = "TB";
	}
	ss.precision(2);
	ss << fixed << size << " " << PreFix;
	return ss.str();
}

void print_directory_entry(directory_entry const& d,bool color=false){
        ostringstream out;
        string filename=d.path().filename().c_str();
        filename=(filename.size()>24)?(filename.substr(0,21)+".. "):filename;
		out << filename<< string(24 - out.str().length(), ' ');

		if(!d.is_directory()) out << file_size(d.file_size());
		out << string(40 - out.str().length(), ' ');

		out << ((d.is_directory()) ? 'd' : '-');

		out << perms_linux(d.status().permissions())<< string(56 - out.str().length(), ' ');

		struct stat fileStat;
        stat(d.path().filename().c_str(), &fileStat);
        //Unsigned Int type User ID to user name and Group ID to group name
        struct passwd *pw = getpwuid(fileStat.st_uid);
		out << pw->pw_name << string(72 - out.str().length(), ' ');
		struct group *gp = getgrgid(fileStat.st_gid);
		out << gp->gr_name << string(88 - out.str().length(), ' ');

		out << ctime(&fileStat.st_mtime);

		string entry=out.str();
		if(color) entry="\033[7m"+entry+"\033[0m";
		cout << entry;
}

void update_directory_entry(path p, int& first, int& last, int& curr){
    q = p;
    current_path(q);
    dir.clear();
    dir = {directory_entry("."),directory_entry("..")};
    directory_iterator d_itr(q);
    for (auto e : d_itr){
        dir.push_back(e);
    }
    first=0,last=dir.size();
    if(last>25)last=25;
    curr=1;
}

bool enter_command_mode(path p){
    cout << CLEAR;
    printheader(p);
    string cmd = "";
    while(true){
        char c = cin.get();
        //handle arrow keys;
        if(c == 127 || c == 8){
            if(cmd.empty())continue;
            cout <<"\b \b";
            cmd.pop_back();
        }
        else if(c == 27) return 0;
        else if(c == 10){
            //process command
            bool after_cmd = execute_command(cmd);
            if(!after_cmd) return 1;
            cmd = "";
            p = current_path();
            printheader(p);
        }
        else{
            cmd += c; //append to cmd char-by-char
            cout << c;
        }
    }
    return 0;
}

void scroll_directory(vector<directory_entry> &dir, int first,int last,int curr){
    path p;
    struct termios initialrsettings, newrsettings;
    tcgetattr(fileno(stdin), &initialrsettings);
    /*switch to canonical mode with no-echo*/
    newrsettings = initialrsettings;
    newrsettings.c_lflag &= ~ICANON;
	newrsettings.c_lflag &= ~ECHO;
	tcsetattr(fileno(stdin), TCSAFLUSH, &newrsettings);
    while(true){
        cout << CLEAR;
        ostringstream out;
        out << "File Name\t\tFile Size\tPermission\tUserID\t\tGroupID\t\tLast Modified\n" ;
	    out << string(112,'-');
	    cout << out.str() << "\n";
        for (int i=first;i<last;i++){
            if(i==curr)print_directory_entry(dir[i],true);
            else print_directory_entry(dir[i]);
        }

        char key=cin.get();
        if (key==27){
            cin.get();
            char c=cin.get();
            if (c=='A') {
                curr=curr-1;
                if(curr<first){
                    if(curr>=0){
                        first--;
                        last--;
                    }else{
                        last=dir.size();
                        curr=last-1;
                        first=last-25;
                        if(first<0)first=0;
                    }
                }
            }
            else if (c=='B') {
                curr=curr+1;
                if(curr>=last){
                    if((uint32_t)last < dir.size()){
                            last++;
                            first++;
                    }
                    else {
                        curr=0;
                        first=0;
                        last=dir.size();
                        if(last>25)last=25;
                    }
                }
            }
            if(c=='C'){
                /*right arrow*/
                if(!frwdstk.empty()){
                    backstk.push(q);
                    path p = frwdstk.top();
                    frwdstk.pop();
                    update_directory_entry(p,first,last,curr);
                }
            }
            if(c=='D'){
                /*left arrow*/
                if(!backstk.empty()){
                    frwdstk.push(q);
                    p = backstk.top();
                    backstk.pop();
                    update_directory_entry(p,first,last,curr);
                }
            }
        }
        else if(key==10){
            /* enter into a directory */
            if(dir[curr].is_directory()){
                if(curr==0)continue;
                p = dir[curr].path();
                backstk.push(current_path());
                while(!frwdstk.empty()) frwdstk.pop();
                update_directory_entry(p,first,last,curr);
            }
            else if(dir[curr].is_regular_file()){
                /*call the nano editor to open the file*/
                string fname = dir[curr].path().filename().string();
                string ext = dir[curr].path().extension();
                bool check_media = ((ext == ".mp4" )||
                                        (ext == ".mp3" ) ||
                                            (ext == ".pdf" )||
                                                (ext == ".jpg" ));
                if(check_media) open_media_pdf(fname);
                else open_txt(fname);
            }
        }
        else if(key==127){
            /*backspace*/
            /*https://stackoverflow.com/questions/4363309/how-to-check-for-the-backspace-character-in-c*/
            backstk.push(q);
            p = q.parent_path();
            while(!frwdstk.empty()) frwdstk.pop();
            update_directory_entry(p,first,last,curr);
        }
        else if(key==104){
            /*h - home*/
            backstk.push(q);
            p = absolute("/home");
            while(!frwdstk.empty()) frwdstk.pop();
            update_directory_entry(p,first,last,curr);
        }
        /*enter command mode on ':' press*/
        else if(key==58){
            /*within command mode */
            bool check = enter_command_mode(current_path());
            p = current_path();
            if(check) break;
            update_directory_entry(p,first,last,curr);
        }
        else if(key == 'q') break;
        else break;
    }
    tcsetattr(fileno(stdin), TCSANOW, &initialrsettings);
}

void print_directory(path p){
	p = absolute(p);
	directory_iterator d_itr(p);
	dir.push_back(directory_entry("."));
	dir.push_back(directory_entry(".."));
	for (auto e : d_itr){
		dir.push_back(e);
	}
    int first=0,last=dir.size();
    if(last>25)last=25;
    int curr=1;
    scroll_directory(dir,first,last,curr);
}
