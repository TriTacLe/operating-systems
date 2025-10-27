## 1 File systems
1. **Name two factors that are important in the design of a file system.**
1 Reliability: A user's data should be safely stored even though the machine is turned off, its operating system crashes or the file system itself crashes. 
2 Performance: For programs to use data, they must be able to read, write, update and delete (CRUD) the data, and for large amount of data, this process must be fast. 

Both of these factors is important to make the user satisfied.
2. **Name some examples of file metadata.**
A file's metadata is information about the file that is managed by the operating system, but not including the file content (file data). It is like the HTTP header data, but for files.

Example of a file's metadata: size, modification time, owner, security information 
## 2 Files and directories
1. **Consider a Fast File System (FFS) like Linux’s ext4.**
**(a) Explain the difference between a hard link and a soft link in this file system. What is the length of the content of a soft link file?**
The difference between the type of links in the FFS is whether they map multiple entities or singular entities and mapping to names vs mapping to numbers

Hard link maps between a file/directory name and the file number, allowing multiple path names for the same file. On the other hand soft link map one file/directory name to another name, rather than a number.

The length of the content of a soft link file is number of characters the path of the original file it points to, which is the file's size.
https://www.geeksforgeeks.org/linux-unix/soft-hard-links-unixlinux/ 

**(b) What is the minimum number of references (hard links) for any given folder?**
$1$ because when a file is created, it gets a reference count to one. When the reference count is zero, the file is removed.

**(c) Consider a folder /tmp/myfolder containing 5 subfolders. How many references (hard links) does it have? Try it yourself on a Linux system and include the output. Use ls -ld/tmp/myfolder to view the reference count (hint, it’s the second column in the output).**
Input:
```bash
ls -ld tmp/myfolder
```
Output:
```bash
drwxr-x--x 7 trile eall_s 4096 Oct 25 22:45 tmp/myfolder
```
Total hardlinks in the folder is *$7$*, because we have $5$ subdirectories + $2$ (tmp and myfolder).
**(d) Explain how spatial locality is acheived in a FFS.**
FFS uses a number of *locality heuristics* to achieve spatial locality. Locality heuristics are file system block allocation technique that places files nearby disk sectors if they are likely to be read/written at the same time. 

2. **NTFS - Flexible tree with extents**
**(a) Explain the differences and use of resident versus non-resident attributes in NTFS.**
Resident attributes store its content directly in the MFT record while a non-resident stores extent pointer in the MFT record and stores the content in those extent pointers. 

How it is used: Large attributes that are too big to fit in the MFT are stored as a non-resident, but attributes that fits are stored as a resident attribute. This approach is often used in huge files. For small file, the data attribute structure can be used to store the file content in the MFT record as a resident attribute.

**(b) Discuss the benefits of NTFS-style extents in relation to blocks used by FAT or FFS.**
The benefit of extends is the variable-size implementation which can hold variable number of blocks. Thus it support huge files, even multi-gigabyte file can be represented by the extent pointers.

While block is defined as a fixed-size chunk of data storage, from 256KB to 4MB, which can only hold data that is in that interval.

The NTFS-style extents is therefore more scalable and robust for increase in file size.
3. **Explain how copy-on-write (COW) helps guard against data corruption.**

## 3 Security
1. **Authentication**
**(a) Why is it important to hash passwords with a unique salt, even if the salt can be publicly known?**


**(b) Explain how a user can use a program to update the password database, while at the same time does not have read or write permissions to the password database file itself. What are the caveats of this?**


2. **Software vulnerabilities**
**(a) Describe the problem with the well-known gets() library call. Name another library call that is safe to use that accomplishes the same thing.**


**(b) Explain why a microkernel is statistically more secure than a monolithic kernel.**
