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

*Note: While opening a file:*
        
        i. text files open in nano
        
        ii. mp3,mp4,jpg,pdfs open but still buggy
        
        iii. opening any other type of files won't work

2. COMMAND MODE(*Press **:** to enter the command mode*)

**To enter shell commands**

a. Copy File/Direrectory- ‘copy <source_file(s)> <destination_directory>’

        Eg - copy foo.txt bar.txt baz.mp4 ./foobar 

b. Move - ‘move <source_file> <destination_directory>’ 

        Eg - move foo.txt ./foobar Rename - ‘rename <old_filename> <new_filename>’ rename foo.txt bar.txt

c. Create File - ‘create_file <file_name> <destination_path>’ Create Directory - ‘create_dir <dir_name> <destination_path>’

        Eg -create_file foo.txt ./foobar create_file foo.txt . create_dir foo ./foobar

d. Delete File - ‘delete_file <file_path>’ Delete Directory(recursive) - ‘delete_dir <dir_path>’

e. Goto - goto <directory_path>

f. Search - ‘search <file_name>’ or ‘search <directory_name>’
        Searchs for a given file or folder under the current directory recursively.

g. clear to clear the command mode screen.

h. Press ESC key to go back to the Normal Mode

i. exit - the application will close.
