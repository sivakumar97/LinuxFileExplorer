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

void print_dirent(directory_entry const& d,bool color=false){
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

void updateDirEnt(path p, int& first, int& last, int& curr){
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

void scrollDir(vector<directory_entry> &dir, int first,int last,int curr){
    path p;
    struct termios initialrsettings, newrsettings;
    tcgetattr(fileno(stdin), &initialrsettings);
    /*switch to canonical mode and echo mode*/
    newrsettings = initialrsettings;
    newrsettings.c_lflag &= ~ICANON;
	newrsettings.c_lflag &= ~ECHO;
	tcsetattr(fileno(stdin), TCSAFLUSH, &newrsettings);

    while(true){
        cout << "\033c";
        ostringstream out;
        out << "File Name\t\tFile Size\tPermission\tUserID\t\tGroupID\t\tLast Modified\n" ;
	    out << string(112,'-');
	    cout << out.str() << "\n";
        for (int i=first;i<last;i++){
            if(i==curr)print_dirent(dir[i],true);
            else print_dirent(dir[i]);
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
                    if(last<dir.size()){
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
                    updateDirEnt(p,first,last,curr);
                }
            }
            if(c=='D'){
                /*left arrow*/
                if(!backstk.empty()){
                    frwdstk.push(q);
                    p = backstk.top();
                    backstk.pop();
                    updateDirEnt(p,first,last,curr);
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
                updateDirEnt(p,first,last,curr);
            }
            else if(dir[curr].is_regular_file()){
                /*call the nano editor to open the file*/
                string fname = dir[curr].path().filename().string();
                bool check_media = ((dir[curr].path().extension() == ".mp4" )||
                                        (dir[curr].path().extension() == ".mp3" ) ||
                                            (dir[curr].path().extension() == ".pdf" )||
                                                (dir[curr].path().extension() == ".jpg" ));
                if(check_media == true) open_media_pdf(fname);
                else open_txt(fname);
            }
        }
        else if(key==127){
            /*backspace*/
            /*https://stackoverflow.com/questions/4363309/how-to-check-for-the-backspace-character-in-c*/
            backstk.push(q);
            p = q.parent_path();
            while(!frwdstk.empty()) frwdstk.pop();
            updateDirEnt(p,first,last,curr);
        }
        else if(key==104){
            /*h - home*/
            backstk.push(q);
            p = absolute("/home");
            while(!frwdstk.empty()) frwdstk.pop();
            updateDirEnt(p,first,last,curr);
        }
        else break;
    }
    tcsetattr(fileno(stdin), TCSANOW, &initialrsettings);
}

void printDir(path p){
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
    scrollDir(dir,first,last,curr);
}
