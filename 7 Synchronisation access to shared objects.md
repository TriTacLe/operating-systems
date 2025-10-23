	## 7.1 Synchronization Motivation
- Sharing state among threads is useful because it allows threads to communicate, to coordinate work, and to share information. However, when cooperating threads use **shared** state, writing correct multi-threaded programs becomes much more difficult. *Cooperative threads* are proned to mainly three different obstacles
	1. Program execution depends on the interleaving's of the thread's access to shared data: if two threads write a shared variable, the final value of the variable depends on which of the threads' writes finishes last 
	2. Program execution can be nondeterministic: Different runs of the same program may produce different results. Ex: Scheduler make different scheduling decisions, processor may run at a different frequency, or another concurrently running program may affect the cache hit rate.
	3. Compilers and architectures reorder instructions: Modern compilers and hardware will reorder instructions to improve performance. 

**Structured synchronization**
- Given all of these challenges, multi-threaded code can introduce subtle bugs. This chapter describes a structured synchronization approach to sharing state in multi-threaded programs. Rather than scattering access to shared state throughout the program and attempting ad hoc reasoning about what happens when the threads’ accesses are interleaved in various ways, we structure the program to facilitate reasoning about it and we use a set of standard synchronization primitives to control access to shared state

**Busy-waiting (polling):** is when a threads spins in a loop waiting for a concurrent event to occur, consuming CPU cycles while waiting. It is a problem
## 7.2 Challenges
Why are unstructured multi-threaded programs difficult to handle?

**Race condition:** when the behaviour of a program depends on the interleaving of operations of different threads. The threads run a race between their operations, and the result of the program execution depends on who wins the race 

**Atomic operators:** invisible operation that cannot be interleaved or split with or by other operations. 
- Non atomic operations may be interrupted by other threads.
	- Use *mutual exclusion* (only one thread does a particular thing at a time). 
	- *Critical section*: piece of code that only one thread can execute at once
	- Can also *lock* the critical section, thus preventing someone from doing something until we unlock it after we are finisheNentering critical section, before accessing shared data 

## 7.3 Shared objects and synchronization variables
- **Shared objects:** objects that can safely be accessed by multiple threads. Hides the details of synchronizing the actions of multiple threads. 
- All shared state in a program - including variables allocated on the heap (objects allocated with `malloc()` or `new()`) and static, global variables - should be encapsulated in $\geq$ 1 shared objects 
**Implementing shared objects** - three layers
1. **Shared objects layer** define the application logic and hides the internal implementation details. 
2. **Synchronization variables layer.** Shared objects include synchronization variables as member variable. These variables are stored in memory like any other objects and can be included in data structures. Build shared objects using two types of synchronization: variable *locks* and *condition variables*. 
	- A **synchronization variable** is a data structure used for coordinating concurrent access to shared state variables, which are normal member variables of an object in single-threaded programming.
3. **Atomic read-modify-write instructions** allow one thread to have exclusive access of a memory location. But the instruction's read, modification, and write of that location are guaranteed not be interleaved with any other thread's access to that memory.
## 7.4 Locks: mutual exclusion
**Lock:** synchronization variable that provides mutual exclusion - when a thread holds a lock, no other thread can hold the same lock. 
- Lock before entering critical section, before accessing shared data.
- Unlock when leaving, after done accessing shared data

**Locks**
- Lock can be in two states: BUSY or FREE
- Lock is initially FREE
- Lock::acquire: wait until lock is FREE, then automatically makes the lock BUSY
- Lock::release: makes the lock FREE, waking up anyone waiting for it
A lock should ensure these properties:
1. **Mutual exclusion:** At most one thread holds the lock (safety)
2. **Progress:** If no thread holds the lock and any thread attempts to acquire the lock, then eventually some threads succeeds in acquiring the lock (progress)
3. **Bounded waiting:** If thread T attempts to acquire the lock, then there exist a bound on the number of times other thread can successfully acquire the lock before T does. (progress)

**Rules for locks**
- Always acquire before accessing shared data structure.
	- Beginning of procedure
- Always release after finished with shared data
	- End of procedure
	- Only lock holder can release
	- DO NOT throw lock for someone else to release
- Never access shared data without lock

## 7.5 Condition variables
**Condition variable:** synchronization object that enables a thread to efficiently wait for a change to shared state that is protected by a lock. 
- *Purpose:* have a thread waiting for something else to change the state of the system so that it can make progress. 
- Waiting inside a critical section: called only when holding a lock

A conditon variable has three methods:
- `CV::wait(Lock *lock)`: makes the thread wait for the lock: Atomically release lock and suspends execution of the calling thread, placing the calling thread on the condition variable's waiting queue. Later, when calling the thread is reenabled, it reacquires the lock before returning from the `wait()`call.
- `CV::Signal()`: makes a thread ready to run: Takes one waiting thread off the condition variable's waiting queue and marks it as ready to run (put the thread on the scheduler's ready list)
- `CV::Broadcast()`: takes all waiting variable off the condition variable's waiting queue and marks them as ready to run:

Condition variables integration with locks
- A condition variable is memoryless; it has no internal state other than queue of waiting threads. They do not need state of their own because they are always used within shared objects that define their own state
- `wait()`automatically releases the lock. Atomicity ensure that there is no separation between:
	- checking the shared object's state
	- deciding to wait
	- adding the waiting thread to the conditional variable's queue
	- releasing the lock so that the other thread can access the shared object to change its state.
- When a waiting thread is reenabled via `signal()`or `broadcast()`, it may not run immediately because it is moved to the scheduler's ready queue with no special priority, and the scheduler may run it at some time later. Needed for other concurrent programs to finish.  
## 7.6 Semaphore
**Semaphore:** type of synchronization variable with only two atomic operations, `P()` and `V()`. `P`waits for the value of the semaphore to be positive, and then atomically decrements it. `V`atomically increments the value, and if any threads are waiting in `P`, triggers the completion of the `P`operation,