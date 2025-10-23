### 8.1 Multiprocessor Lock Performance
Lock contention: only one thread at a time can hold a given lock.
Shared data protected by a lock may ping back and forth between cores
False sharing: communication between cores for data that is not shared. Extra inter-processor communication required because a single cache entry contains portions of two different data structures with different sharing patterns. 
### 8.2 Lock Design Patterns
#### 8.2.1 Fine-Grained Locking
Locking each element
#### 8.2.2 Per-Processor Data Structures
PArtision the work so never e
#### 8.2.3 Ownership Design Pattern
Only one thread at a time access shared data
#### 8.2.4 Staged Architecture
### 8.3 Lock Contention
Both method rely on atomic read-modify-write instructions
Solve: Lock contention in multicore systems
#### 8.3.1 MCS Locks
**MCS Locks:** Spinlock implementation where each waiting thread spins on a separate memory location. To release the lock, the bit is set for one thread, telling it that it needs to acquire the lock. Uses compare-and-swap
- Use case: when there are a significant number of waiting threads
**Compare-and-swap:** an atomic read-modify-write instruction that first tests the value of a memory location, and if the value has not been changed, sets it to a new value
- Use to build queue of waiting threads. A waiting threads adds itself to the *tail* of the queue, then spins on a flag in its queue entry. When a thread releases the lock, it sets the flag in the next queue entry, signaling to the thread that its turn next.
- Because each thread spins on its own queue entry, the lock can be passed efficiently.
- Because the overhead of setting up the queue means that an MCS lock is less efficient than a normal spinlock unless there are a large number of waiting threads.

**MCS Lock implementation**
Maintain a list of threads waiting for the lock
- New thread uses compare-and-swap to add to the tail
- Lock is passed by setting next -> needToWait = FALSE;
#### 8.3.2 Read-Copy-Update (RCU)
How can concurrent reads access a data structure - one that can also be written - without having to update the state of a synchronization variable on each read?

**RCU Locks:** implementation of reader/writer lock, readers (and writers) surround each critical section with calls to acquire and release the RCU lock in read-mode (or write-mode). Optimized for the case when there are many more readers than writers. 
- Reduces overhead for readers at the cost increased overhead for the writers.
- Readers proceed without first acquiring lock
- One write to happen concurrently
**Three important changes to the standard interface:**
- *Restricted update:* writer thread must publish its changes to the shared data structure with a single, atomic memory write. 
	- *Publish:* For a read-copy lock, a single, atomic memory write that shared object protected by the lock. This write allows the new reader threads to observe the new version og the object.
- *Multiple concurrent version:* Readers may see old or new version because RCU allows any number of concurrent read on critical sections
- *Integration with thread scheduler:* Guarantee that all readers complete within a *grace period* and then garbage collect old version. RCU uses information provided from the thread scheduler to determine when a grace period ends.

**Grace period:** time from when an update (write) is published until the last reader is done with the previous version.

Readers disable interrupts on entry 
### 8.4 Multi-Object Atomicity
#### 8.4.1 Careful Class Design
#### 8.4.2 Acquire-All/Release-All
#### 8.4.3 Two-Phase Locking
### 8.5 Deadlock
**Deadlock:** A cycle waiting among a set of threads, where each thread waits for some other thread in the cycle to take some action. Deadlock is a starvation
Resources: any passive thing needed by a thread to do its job (Shared memory, CPU, lock, disk space, memory). Two types of resources:
- Preemptable: can be taken away by OS
- Non-preemptable: must leave with thread. Voluntarily giving up the resource for others to get it
Starvation: thread waits indefinitely for a resource
#### 8.5.1 Deadlock vs. Starvation
**Starvation**: a thread fails to make progress for an indefinite period of time. 

**Deadlock** is a form of starvation but with the stronger condition: a group of threads forms a cycle where none of the threads make progress because each thread is waiting for some other thread in the cycle to take action.

Thus, deadlock implies starvation, but starvation does not imply deadlock.
#### 8.5.2 Necessary Conditions for Deadlock
Four necessary conditions:
1. Limited access to resources
	- If infinite resources, no deadlock
2. No preemption
	- If resources can be given up voluntarily, there wont be any deadlocks
3. Multiple independent requests
	- Wait while holding
4. Circular chain of requests
#### 8.5.3 Preventing Deadlock
Three mains ways to prevent deadlock
1. Exploit or limit program behavoir
	- Limit program from doing anything that might lead to deadlock
	- Provide enough resources
	- Eliminate wait while holding
	- Eliminate circular waiting
		- Lock ordering: always acquire locks in a fixed order. Scheduler to predict and acquire lock order
		- Example: mv file from one dir to another
2. Predict the future
	- If we what know what the program will do, we can tell if granting a resource might lead to deadlock
3. Detect and recover
	- If we figure out that we have a deadlock, then we can rollback a thread, preempt one of its resources, we can fix that deadlock
#### 8.5.4 The Banker’s Algorithm for Avoiding Deadlock
Way of predicting the future. 
Each thread has to state maximum resources needed in advance
Step by step allocate resources dynamically when resource is needed. Wait if granting request would lead to deadlock
Request can be granted if some sequential ordering of threads is deadlock free

Grant request if and only if (iff) result is a safe state
Sum of maximum resource needs of current threads can be greater than the total resources
Proceed iff
- Total available resources - numbers of allocated resources $\geq$ max remaining number of resources that might needed by this thread in order to finish
- 

#### 8.5.5 Detecting and Recovering From Deadlocks
Safe state
- For all possible sequence of future resources requests, it is possible to eventually grant all requests
- In the context of deadlock, a state of an execution such that regardless of the sequence of future resource requests, there is at least one safe sequence of decisions as to when to satisfy requests such that all pending and future requests are met
Unsafe state
- Some sequence of resource requests can result in deadlock
Doomed state
- All possible computations lead to deadlock

**Detect and repair**
Algoritm
- Scan wait for graph
- Detect cycles
- Fix cycles
Proceed without the resource
- Requires robust exception handling code
Roll back and retry
- Transaction: all operations are provisional until have all required resources to complete operation
### 8.6 Non-Blocking Synchronization
