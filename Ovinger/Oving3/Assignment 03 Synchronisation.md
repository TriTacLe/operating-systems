## 1 Synchronisation
1. **The principle of process isolation in an operating system means that processes must not have access to the address spaces of other processes or the kernel. However, processes also need to communicate.**
**a. Give an example of such communication.**
Processes often need to communicate with each other thus needing to share address space. Shared memory allows this. A example of this is the "Too much milk" case described in the book. It describes two person as processes and the fridge as the shared memory segment. Another known example of shared memory is the "Produces & Consumer" problem, where producer writes to and consumer from a share memory segment. 
https://www.geeksforgeeks.org/operating-systems/ipc-shared-memory/ 
**b. How does this communication work?**
Works by creating a memory segment that can be accessed by multiple processes. A process (parent) creates the shared memory segment using syscall `shmget()`and assign it the identifier `shmid`. The processes that need access to the shared memory attaches themselves to this segment using syscall `shmat()`. Now the processes can read/write to the shared memory. When processes no longer need access to the shared memory, they can detach from the segment using syscall`schmtl()`. Once all processes are detached the segment can be removed from the system using `schmctl`.
**c. What problems can result from inter-process communication?**
There are three problems when using cooperative threads if not handled correctly: 
1 Race condition: Program execution depends on the interleaving of the thread's access to shared data. The threads run a race between each other and the result depends on who wins the race

2 Program execution prone to be nondeterministic. Different executions of the same program gives different results. Affected by factors such as scheduler makes different scheduling decision. 

3 Compiler reorder instructions to improve performance, but this reordering is not visible to single-threaded programs, but can become visible when multiple processes interact through shared data.
2. **What is a critical region? Can a process be interrupted while in a critical region? Explain.**
Critical region is a segment of code where a constraint is applied so that only one thread can execute at once (atomically accesses shared memory). The purpose is to avoid problems such as race conditions and data inconsistency. Because it accesses shared memory atomically, other threads or processes can not access that shared memory, thus implies that the process that executes the critical region is isolated. 

A thread running a critical region can be interrupted by the OS because critical region only implies that no thread can execute the same critical region at once. Interrupts can be disabled if wanted.
3. **Explain the difference between busy waiting (polling) versus blocking (wait/signal) in the context of a process trying to get access to a critical section.** 
	
4. **What is a race condition? Give a real-world example.** 
When the behaviour of a program depends on interleaving of operations of different threads. So the threads run a race between their operations, and the result of the program execution is depended on who wins the race. 

Example:
Two people both have access to the same bank account and both try to pay for something costing 1kr. The system check their balance and validates if there is enough money for both payments. If there is not proper synchronization, the system allows both transaction to go through even though the bank account amount is only 1kr.

```C
#include <stdlib.h>
 
int balance = 1 

int threadCount
pthread_t thread[threadCount];

void pay(int cost){
	balance -= cost;
}

int main(){
	
	int cost = 1
	
	for(int i = 0; i<threadCount;i++){
		thread_create(&thread, NULL, pay, cost);
	}
}
```

5. **What is a spin-lock, and why and where is it used?**
A lock where a thread is waiting for a BUSY lock "spins" (busy/wait) in a tight loop until some other thread makes it FREE. Spinlock is used to protect shared resourses from being accessed by multiple threads or processes at the same time. 

There is other synchronization methods like semaphores and mutexes, but spinlock uses a busy-wait method where a thread continuously asks for the lock until is it available. The purpose of this is too save time by avoiding overhead of blocking/pausing the thread.

The downside of spinlock is if multiple threads are spinning and threads keeps checking if the lock is available, which would waste time and CPU-cycles. To answer `where`a spinlock is used I would say that it should be used when the critical region is anticipated to be held for a short period of time. Other methods mentioned above may be more effective if the waiting period is longer or there is many threads asking for the lock. 
6. **List the issues involved with thread synchronisation in multi-core architectures. Two lock algorithms are MCS and RCU (read-copy-update). Describe the problems they attempt to address. What hardware mechanism lies at the heart of each?**
