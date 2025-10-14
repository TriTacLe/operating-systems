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
**MCS Locks:** Spinlock implementation where each waiting thread spins on a separate memory location
- Use case: when there are a significant number of waiting threads
**Compare-and-swap:** an atomic read-modify-write instruction that first tests the value of a memory location, and if the value has not been changed, sets it to a new value

Maintain a list of threads waiting for the lock
- New thread uses compare-and-swap to add to the tail
- Lock is passed by setting next -> needToWait = FALSE;

MCS Lock implementation
#### 8.3.2 Read-Copy-Update (RCU)
**RCU Locks:** implementation of reader/writer lock, optimized for the case when there are many more readers than writers. 
- Reduces overhead for readers at the cost increased overhead for the writers.
- Readers proceed without first acquiring lock
- One write to happen concurrently
Restricted update
- Writer computes new version of data structure
- Publishes new version with a single atomic instruction
Multiple concurrent version
- Readers may see old or new version
Integration with thread scheduler
- Guarantee that all readers complete within a grace period and then garbage collect old version

Grace period

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
Starvation is not a sufficient condition for a deadlock
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
