Threads have less overhead then processes
With process we create an entire address space, with threads we use (inherit) the address space of an existing process
### 4.1 Threads: Definitions
**Why use threads?**
- OS often need to be able to handle multiple things happening at the same time, such as handling GUI as well as handling background tasks or more specific process execution, interrupts, background tasks, system maintenance
- Threads are an abstraction to help with these
- Example, word processor:
	- Thread for reading from keyboard
	- Thread for formatting the document
	- Thread for saving changes to disk
	- Thread for spellcheck
	- Thread for GUI
#### 4.1.1 Definition
- **Thread:** a *single execution sequence* that represents a *separately schedulable task* (Code can be swapped on to and from CPU)
	- **Single execution sequence:** Thread executes a sequence of instructions (assigments, loops, procedures, etc.) just as in the familiar programming model
	- **Separately schedulable:** OS can run, suspend or resume a thread at any time
- Threads provide the illusion of an infinite number of processors. To map an arbitrary set of threads to a fixed set of processors, OS include a scheduler that can switch between threads that are currently running and threads that are ready to run. 
- Switching between threads is transparent to code being executed by the threads. Threads thus provides an execution mode in which each thread runs on a *dedicated virtual processor with unpredictable and variable speed*

**Different context in which thread can be used: Threads in the Kernel and at User-Level**
- Multi-threaded kernel
	- Multiple threads, sharing kernel data structures, and capable of using privileged instructions
- Multiprocess kernel
	- User processes
	- Multiple single-threaded processes
	- System calls access shared kernel data structures
- Multiple multi-threaded user processes
	- When we extend multiprocess kernel model to have threads
	- Each process with multiple threads, schedulable, sharing same data structures, and process are isolated from other user processes
### 4.2 Threads: Abstraction
 - **Threads** virtualize the processor, providing an illusion of running that programs runs on machines with an infinite number of virtual processors. The program can then create how many threads they want without worrying about the number of physical number, and each thread runs on its own virtual processor. OS job to multiplex all of the system's threads on the actual physical processor.
 - **Thread abstraction:** Each threads runs on its own virtual processer with unpredictable and variable speed
 - A single sequence of instructions has the illusion of running on multiple processors at the same time
 - Threads execute with variable speed and programs must be designed to work with any schedule ![[Screenshot 2025-09-14 at 12.05.29.png]]![[Screenshot 2025-09-14 at 12.08.31.png]]![[Screenshot 2025-09-14 at 12.13.40.png]]
 - Context switch, multiple CPU cores
 - Throughput and latency
#### 4.2.1 Thread operations
- ``thread_create(thread, func, args)``
	- Create a new thread to run a func(args)
- ``thread_yield()``
	- Relinquish processor voluntarily so you can schedule another thread
- `thread_join(thread)`
	- In parent, wait for forked thread to exit, then return
- `thread_exit
	- Quit thread and clean up, wake up anyones who waits for `thread_join()`if any
#### 4.2.2 Fork/Join Concurrency
- Threads can create children, and wait for their completion
- Data only shared before fork/after join - ensures independence
- Examples:
	- Web server: fork a new thread for every new connection
		- As long as the threads are completely independent
	- Merge sort
	- Parallel memory copy
### 4.3 Thread datastructure
**Thread control block (TCB)**
- **Definition:** Data structure that holds the threads's state. For every thread that OS creates, i will create a TCB. The TCB holds to types of information
	1. The state of the computation being performed by the thread
	2. Metadata about the thread that is used to manage the thread
**Per-thread computation state**
- Each thread represents its own sequentially executed computation, so to create multiple threads, we must allocate per-thread state to represent the current state of each thread's computation. This is done by adding a *stack* and a *copy of processor registers* 
- **Stack:** A thread's stack is the same as the stack for a single-threaded computation - it stores information needed by the nested procedures the threads is currently running. A pointer for the thread's stack is stored in the TCB. 
- **Copy of the processor registers:** The TCB also contains fields in which the OS can store a copy of all the processors registers because a thread can be suspended or later resumed.
**Per thread metadata**
- The TCB also includes per-thread metadata (information about each thread), which is used by the OS to manage the thread. 
**Shared state**
- The program code is shared by all of the threads in a process. 
- Statically-allocated global variables and dynamically-allocated heap can store information accessible to all threads
- Unique to each thread: stack
### 4.4 Thread lifecycle
![[Screenshot 2025-09-17 at 10.44.07.png]]
**Init:** Thread creation puts a thread into its initial *init* state and allocates and initializes per-thread data structures. Then the thread creation code puts the thread into the *ready* state by adding the thread to the *ready* list.
**Ready:** A thread in the *ready* state is available to be run but is currently not running. Its TCB is on the *ready* list, and the values of its registered are stored in its TCB. The scheduler can cause a thread to transition from *ready* to running by copying its register values from TPC to processes registers.
**Running:** A thread in the *running* state is running on a processor. Its register values are stored on a processor rather than in the TCB. A *running* thread can transition to the *ready* state in two ways:
	1. The scheduler can preempt a running thread and move it to the *ready* state at any time by saving the thread's registers to the TCB and switching processor to running the next thread on the *ready* list
	2. A running thread can voluntarily relinquish the processor and go from *running* to *ready* by calling `yield()` 
**Waiting:** A thread in the *waiting* state is waiting for some event to run. Whereas the scheduler can move a thread in the *ready* state to *running* state, a thread in the *waiting* state is not eligible to run until some action by another thread moves it from the *waiting* state to the *ready* state.
**Finished:** A thread in the *finished* state will never run again. The OS can free some or all of its state for other uses.
### 4.5 Implementating threads
**Creating a thread**
- Must allocate its per-thread state. The thread constructor allocates a TCB and a stack. The stack is a memory area like any other data structure allocated in memory. We have to initialize the per-thread states by setting the TCB's registers to some values. 
- `Thread_create(func, args)`
	- Allocate thread control block (TCB)
	- Allocate stack 
	- Build stack frame for base of stack (stub)
	- Put func and its args on stack
	- Put thread on ready list
	- Will run sometime later (maybe right away)
- `stub(func, args):` 
	- Call`(*func)(args)`
	- If return, call`thread_exit()`
**Deleting a thread**
- We need to (1) remove it from the *ready* list and (2) free the per-thread state we allocated for it. 
- A thread transitions to the *finished* state by moving its TCB from the *ready* list to a list of finished threads the scheduler should never run. At any time the thread library code runs (when some other threads call `yield()`or when an interrupt occurs), the thread library can call a method to free the state of any threads on the finished list.
### Roadmap
- Kernel threads
	- Thread abstraction only available to kernel
	- To the kernel, a kernel thread and a single threaded user process look quite similar
- Multithreaded processes using kernel threads (Used by Linux, MacOS)
	- Kernel thread operations available via syscall
- User-level threads
	- Thread operations without system calls
### Thread Stack
- Can run out of stack space
- What if a thread puts too many procedures on its stack?
	- What happens in Java?
	- What happens in the Linux kernel?
	- What should happen?
### 4.6 Thread Context Switch
**Definition:** suspends execution of the currently running thread and resumes execution of some other thread. 
- Done by copying the currently running thread's registers from the processor to the thread's TCB, and then copying the other thread's registers from that thread's TCP to the processor. 

**How does a kernel thread context switch work?**
1. Copy the *running* thread's register from the processor to the thread's TCB.
2. Copy the *ready* thread's registers from the thread's TCB to the processor.
- The state is firstly saved, then either if it is a hardware triggered switch, the kernel’s handler is ran/ and if it is a library called switch (`thread.yield()`) the code that handles the library call is ran. Finished by restoring the next ready threads registers.

**What causes the kernel to context switch**
We have a way to both have cooperative threads and preempted threads and these can be broken down into two categories. 
- Cooperative: Voluntary giving up (yielding) the CPU so that another thread can be scheduled, by using:
	- `Thread_yield`
	- `Thread_join` (if child is not done yet)
- Involuntary. 
	- We need interrupt or exception to wake up the kernel and tell us to switch threads
	- Some other thread is higher priority
#### 4.6.1 Voluntary thread context switch
When voluntarily context switch you need to
1. Save the state of the current thread by saving registers on old stack
2. Switch to new stack and new thread
3. Restore registers from new stack
4. Return
- Exactly the same with kernel threads or user threads
**A Subtlety**
- When`thread_create` we put that thread on a *ready* list
- When it first runs, some thread calls switchframe. This:
	- Saves old thread state to stack
	- Restores new thread state from stack
- Then set up new thread’s stack as if it had saved its state in switchframe
	- “returns” to stub at base of stack to run func. Run the task that out thread is supposed to run
#### 4.6.2 Involuntary Thread/Process Switch
If we have preemptive threading schedule, we need to involuntary preempt the thread so we have to take the the CPU from it and give the CPU to another thread. This is done with:
- Timer or I/O interrupt
	- Tells OS some other thread should run
- Alternative
	- Have the end of interrupt handler calls `switch()`
	- When resumed, return from handler resumes kernel thread or user process
	- Thus, processor context is saved/restored twice (once by interrupt handler, once by thread switch)
### 4.7 Multi-threaded processes 
**Multiple-threaded kernel:** Multiple threads, sharing kernel data structures , capable of using privileged instructions
**Multiprocess kernel:** Multiples singel threaded user processes, uses syscalls to access kernel shared data structures.
**Multiple multi-threaded user processes:** Each with multiple threads, sharing same data structures, isolated from other user processes via memory protection.

Two different implementations
- **Preemptive:** interrupts, save one threads and restore state of another (involuntarily, interrupt received by the CPU)
- **Erative:** yield, save one thread's state and restore another (voluntarily)

**Implementing multi-threaded process using kernel threads**
- Procedure call

**Implementing user-level threads with kernel support**
#### Take 1
This is the Linux and MacOS model
- User thread = kernel thread 
	- System calls for thread fork, join, exit (and lock, unlock,…)
	- Kernel does context switch, not the user-library
	- Simple, but a lot of transitions between user and kernel mode
- Thread can access address space
#### Take 2
- Green threads (early Java)
	- User-level library, within a single-threaded process
	- Library does thread context switch, which means we don't have to do any context switch between user-mode and kernel-mode
	- Preemption via upcall/UNIX signal on timer interrupt, so its the kernel who is telling the Java program to preempt the current thread and change
	- Use multiple processes for parallelism
		- Shared memory region is mapped into each process
#### Take 3
- Scheduler activations (Windows)
	- Kernel allocates processors to user-level library
	- This thread library implements context switch, so it handles scheduling 
	- The threading library decides what thread to run next
- Upcall (event-driven) whenever kernel needs a user-level scheduling decision
	- Process assigned a new processor
	- Processor removed from process
	- Syscall blocks in kernel

#### 4.7.1 Multi-threaded programs – advantages / disadvantages
