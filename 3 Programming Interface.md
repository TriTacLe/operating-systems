Interface between user application and kernel
System calls
libc: making it easier for the programmer to use syscalls
### Main points
- Creating and managing processes
	- Fork, exec, wait
- Performing I/O
	- Open, read, write, close
- Communicating between processes
	- Pipe, dup, select, connect
- Example: implementing a shell
### Shell
- A shell is a job control system
	- Part of user-space, not kernel
	- Allows programmer to create and manage a set of programs to do some task
	- Windows, MacOS, Linux all have shells
### Question
- If the shell runs at user-level, what system calls does it make to run each of the programs?
	- Ex: ``cc, ln``
### Windows CreateProcess sysall 
System call to create a new process to run a program 
- Create and initialize the process control block (PCB) in the kernel 
- Create and initialize a new address space 
- Load the executable program into the address space (text segment) 
- Copy arguments into memory in the address space 
- Initialize the hardware context to start execution at ``_start`` 
- Inform the scheduler that the new process is ready to run 
### How does Windows CreateProcess look like 
```c
if (!CreateProcess( 
	NULL, // No module name (use command line) 
	argv[1], // Command line 
	NULL, // Process handle not inheritable 
	NULL, // Thread handle not inheritable 
	FALSE, // Set handle inheritance to FALSE 
	0, // No creaton fags 
	NULL, // Use parent's environment block 
	NULL, // Use parent's startng directory 
	&si, // Pointer to STARTUPINFO structure 
	&pi ) // Pointer to PROCESS_INFORMATION structure
)
```
### UNIX Process Management
- UNIX fork - system call to create a copy of the current process, and start it running
	- No arguments
- UNIX exec - system call to change the program being run by the process
- UNIX wait - system call to wait for a process to wait
- UNIX signal - system call to send a notification to another process 
### Questions
- Can UNIX exec return error?
	- Can return error but wont return anything if it suceeds
- Can UNIX fork return error?
- Can UNIX wait() return
Tips use `man`
### Implementing UNIX fork
Steps to implement UNIX fork
- Create and initialize the process control block (PCB) in the kernel
- Create a new address space
- Initialize the address space with a copy of the entire contents of the address space of the parent.
- Inherit the execution context of the parent (e.g., any open files)
- Inform the scheduler that the new process is ready to run
### Implementing UNIX exec
Steps to implement UNIX exec
- Load the program into the current address space
- Copy arguments into memory in the address space
- Initialize the hardware context to start execution at ``start``
### UNIX I/O
Uniformity
- All operations on all files, devices use the same set of system calls: open, close, read, write.
Open before use
- Open returns a handle (file descriptor) for use in later calls on the file
Byte-oriented 
Kernel-buffered read/write 
Explicit close 
- To garbage collect the open file descriptor 
### UNIX File System Interface 
UNIX file does a lot of things 
- Open the file, return file descriptor 
- Options: 
### Interface Design Question 
- Why not separate syscalls for open/create/exists
### Implementing a Shell 

Kill 
Upcall