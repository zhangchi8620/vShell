vFileSystem 1.0
===========================

I. Overview
vFileSystem is a UNIX-like file system with a hierarchical directory structure. Users can browse the directory structure, create and delete new files as well as directory. 

II. Usage

1. Package Structure
The vFileSystem package contains the basic tools for file system operations and source codes. 

vhdd:
        A fixed size large file representing virtual hard disk drive. Initialized content is '~' in vhdd. So '~' is not allowed to use as the content in vFileSystem.

Source code for the vFilesystem implementations:
        Blocks                                : Blocks functions implementation
        Filesystem                        : File functions implementation
        Types                                : Directory, Inode and Blocks definition
        Interpreter                        : Shell-like interpreter
        Makefile                        : Makefiles for vFileSystem
        test.c                                : Main function

Testing files:
        Input                                : Reading commands from
        Out                                        : Write output into
        input.txt                        : Sample input file

2. Build vFileSystem
To compile the programs of vFileSystem, simply use the make file to compile all the programs.

3. Run vShell
After building, execute ./vShell to run vFileSystem. "$" indicates you have entered vFileSystem. 

4. Shell Commands 
mkfs: Make a new file system, i.e., format the disk so that it is ready for other file system operations.

open <filename> <flag>: Open a file with the given <flag>, return a file descriptor <fd> associated with this file. <flag>: 1: r; 2: w; 3: rw.

read <fd> <size>: Read <size> bytes from the file associated with <fd>, from current file offset. The current file offset will move forward <size> bytes after read.

write <fd> <string>: Write <string> into file associated with <fd>, from current file offset. The current file offset will move forward the size of the string after write. Here <string> must be formatted as a string. If the end of the file is reached, the size of the file will be increased.

seek <fd> <offset>: Move the current file offset associated with <fd> to a new file offset at <offset>. The <offset> means the number of bytes from the beginning of the file.

close <fd>: Close the file associated with <fd>.

mkdir <dirname>: Create a sub-directory <dirname> under the current directory.

rmdir <dirname>: Remove the sub-directory <dirname>. This directory has to be empty when it is removed.

cd <dirname>: Change the current directory to <dirname>.

link <src> <dest>: Create a link named <dest> to an existing file or directory named <src>. Note that <dest> and <src> may not be in the same directory. Both 
<src> and <dest> are files, not directories.

unlink <name>: Remove a link to the name. (When link count drop to 0, delete the file).

stat <name>: Show the status of the file or directory with name <name>. It displays its inode information; whether it is a file or directory; its link count; size of the file/directory; number of blocks allocated; date modified; items it contains.

ls: Show the content of the current directory. No parameters need to be supported.

cat <filename>: Show the content of the file.

cp <src> <dest>: Copy the file from a source location to the destination location. Note that <src> and <dest> may not be in the same directory. If the <dest> already exists, the program should report an error.

tree: List the contents of the current directory in a tree-format. For each file listed, its date file size should be included.

import <srcname> <destname>: Import a file from the host machine file system to the current directory.

export <srcname> <destname>: Export a file from the current directory to the host machine file system.

q: Quit the vFileSystem.

5. Cache
This function enables caching data blocks of files in memory. While a file access is invoked, such as read or write, the system call will go to cache to look for data blocks first and handles cache miss with LRU replacement policy. Consistence is also maintained.

III.  Contact
Jilong Liao: jliao2@utk.edu
Chi Zhang:          czhang24@utk.edu
