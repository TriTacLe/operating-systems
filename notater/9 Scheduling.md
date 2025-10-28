**Definitions**
- Task/job: user requests, e.g., mouse click, web request, shell command etc
- Latency/response time: how long does a task take to complete
- Throughput: How many task can be done per unit of time
- Overhead: How much extra work is done by the scheduler
- Fairness: How equal is the performance received by different users
- Predictability: How consistent is the performance over time
- Workload: set of tasks for system to perform
- Preemptive scheduler: If we can take resources away from a running task
- Work-conserving: resource is used whenever there is a task to run
- Scheduling algorithm: Takes a workload as input, decides which task to do first, performance metrics as output, only preemptive, work-conserving schedulers to be considered
## 9.1 Uniprocessor Scheduling
**FIFO**
- Schedule task in the order they arrive: Continue to run them until they complete or yield the processor
- Minimizes overhead switching between tasks only when they completes. 
- When to not use FIFO
	- Different size of jobs: long job currently running when short jobs is in the queue waiting to run. Poor response time for the short jobs
- When to use FIFO
	- Have fixed number of tasks
- FIFO have minimal context switch. Only switch when need to change 

**SJF**
- Do the task that has the shortest remaining amount of work to do
	- Shortest is the remaining time left of the task, not its original length.
- SJF is optimal in terms of average response time
- Need to know the future
- Downsides:
	- By completing the shortest jobs first, the long jobs take longer time. Resource starvation for long jobs
	- Suffer from starvation and frequent context switches. If enough short task arrive, longer task may never complete. 

**Round Robin:** A scheduling policy that takes turns running each ready task for a limited period before switching to the next task.
- Addresses starvation
- Tasks take turns running on a processor for a limited period of time. 
- Preemptive scheduling algorithm
- The scheduler assigns the processor to the first task in the ready list, setting a timer interrupt for some delay, called the *time quantum*. 
- At the end of the quantum, if the task has not completed, the task is preempted and the processor is given to the next task in the ready list. 
- Pick time quantum
	- What if time quantum is to long: Job will complete and we can run the next run
	- What if time quantum is to short: Increased overhead
- If tasks are equal in size, Round Robin will have poor average response time. 
- Tasks that intermix processor and I/O benefit from SJF and can do poorly under Round Robin

**Max-min fairness:** a scheduling objective to maximize the minimum resource allocation given to a process (user, application, thread) until all resources are assigned.
- Improve response time for I/O-bound tasks
- Round Robin and Max-Min fairness both avoid starvation

**Multi-level Feedback Queue (MFQ):** A scheduling algorithm with multiple priority levels managed using round robin queues, where a task is moved between priority levels based on how much processing time it has used
- Achieve these goals:
	- Responsiveness: run short task quickly like SJF
	- Low overhead: minimize number of preemption like FIFO, and minimize the spent making scheduling decisions
	- Starvation free: All task should make progress like Round Robin
	- Background tasks: Defer system maintenance tasks so that they do not interfere with user work
	- Fairness: Assign processes approximately their max-min fair share of the processor
Implementation
- Uses multiple multiple Round Robin queues, each with different priority level and time quantum.
- High priority queues have short time slices, low priority queues have long time slices. Tasks at the same level are scheduled using Round Robin. 
- Scheduler picks first thread in highest priority queue
- New task enter in the highest priority queue with short time quantum. If time slice expires, task drops one priority level

So far the algorithm does not achieve starvation freedom or max-min fairness. Ex: if there are too many I/O-bound tasks, the compute-bound tasks may receive no time on the processor.
- Tasks whose processes have already reached their fair share are only scheduled if all other processes at that level have also received their fair share.
- Processes that receives less than its fair share will have its tasks increased in priority. Task that receive more than its fair share can be reduced in priority
## 9.2 Multiprocessor Scheduling

Potential performance problem for using MFQ on multiprocessor
- **Contention for scheduler spinlock:** The lock may become a bottleneck as the number of processor increases. This depends on how much computation each thread does before blocking I/O.
- **Cache coherence overhead:** Each processor will need to fetch the current state of the MFQ from the cache of the previous processor. On a uniprocessor scheduler, the scheduling data structure is likely to already be loaded into the cache. On a multiprocessor, the data structure are accessed and modified by multiple processors in turn, so the most reason version is likely to be cached by the processor that made the newest update. Fetching from remote cache takes longer than fetching from local cache. Because of this delay happens while holding the MFQ lock, the lock is held for longer period leading to more bottleneck
- **Limited cache reuse:** If threads runs on the first available processor, they are likely to be assigned to a different processor each time they are scheduled. This means that any data needed by the thread is unlikely to be cached on that processor. Furthermore, the most recent version of the thread's data is likely to be in a remote cache, requiring even more slowdown as the remote data is fetched into local cache

**Affinity Scheduling:** Scheduling policy where tasks are scheduled onto the same processor they had previously been assigned, to improve cache reuse. 

Per-Processor Multi-level Feedback with Affinity Scheduling

Scheduling parallell programs 

Bulk Synchronous Parallelism

Gang Scheduling

Parallell Program Speedup

Space Sharing

