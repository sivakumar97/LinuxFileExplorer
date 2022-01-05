# LinuxFileExplorer

A functional terminal based File Explorer with restricted feature set

1. NORMAL MODE(DEFAULT MODE)

**To explore the current directory and navigate the filesystem**

Features:
        a. Display a list of directories and files(along with it's attributes viz. name,size, ownership, permissions, last modified) in the current directory
               i. "." and ".." represent the current and parent directory respectively
        b. Scroll within the directory using arrow keys and press enter to open a file/directory
        c. Traversal(Press the given keys)
                
                i. 🠈  To go back to the previously visited directory
                
                ii. 🠊 To enter into the next directory
                
                iii. Backspace, One level up
                
                iv. h, Home Directory(Directory where the application is started)

*Note: While opening a file:
        i. text files open in nano
        ii. mp3,mp4,jpg,pdfs open but still buggy
        iii. opening any other type of files won't work*

2.COMMAND MODE(*Press **:** to enter the command mode*)

**To enter shell commands**

Copy File/Direrectory- ‘copy <source_file(s)> <destination_directory>’
Eg - copy foo.txt bar.txt baz.mp4 ./foobar Move - ‘move <source_file> <destination_directory>’ move foo.txt ./foobar Rename - ‘rename <old_filename> <new_filename>’ rename foo.txt bar.txt

Create File - ‘create_file <file_name> <destination_path>’ Create Directory - ‘create_dir <dir_name> <destination_path>’
Eg -create_file foo.txt ./foobar create_file foo.txt . create_dir foo ./foobar

Delete File - ‘delete_file <file_path>’ Delete Directory(recursive) - ‘delete_dir <dir_path>’
Goto -
goto <directory_path>

Search - ‘search <file_name>’ or ‘search <directory_name>’
Searchs for a given file or folder under the current directory recursively.
Prints True or False depending on whether the file or folder exists.
Enter switch command, the application will go back to Normal Mode
On pressing q key, the application will close.
Enter clear to clear the command mode screen.
