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
                
                iii. Backspace; One level up
                
                iv. h; Home Directory(Directory where the application is started)

*Note: While opening a file:
        i. text files open in nano
        ii. mp3,mp4,jpg,pdfs open but still buggy
        iii. opening any other type of files won't work 

2.COMMAND MODE(*Press **:** to enter the command mode)

**To enter shell commands**
