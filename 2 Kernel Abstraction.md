### Introduction and terms
- **Process: instance of a program, program + private address space + execution context (regs, stacks) + privileges.**
- **Kernel** enforces isolation (protection) and provides safe control transfers (syscalls, interrupts, exceptions). Hardware timer ensures the kernel can regain control
- **Thread:** sequence of instruction within a process
	- Can have many threads. Parents and child thread
- **Address space:** set of right a process has, which memory it can access. 
- Hardware timer: guarantees the kernel can preempt a looping user program
- **Executable stack:** region in program's memory that grows/shrinks on function call/return. Holds call frames: return address, saved registers, and local variables whose size is known at compile time. Each thread has its own user stack. User stack (for you code) and kernel stack (for kernel runs on behalf of thread during syscall/interrupts).
	- Uses when: small, short-lived data inside a function
- **Heap:** region where you get dynamically allocated memory (e.g `malloc/new`) when the size or lifetime isn't tide to a single function call. Holds big, variable-size data structures (lists, three, buffers)
	- Use when: size isn't known at compile time or data must outlive the function
- **Handler:** code that runs automatically when a spesific event happens.
### 2.1 Process abstraction
- **What:** OS's protection unit. Each process has its own address space and CPU context
- **Why:** isolate malicious code 
	- Compiler converts code into sequence of machine instructions and store it in a file, the program's executable image.
- The OS set aside a memory region, the execution stack, to hold the state of local variables during procedure calls
- The OS also set aside a memory region, called the heap, for any dynamically allocated data structures the program might need.
- The OS keep track of various processes on the computer using a data structure called the process control block (*PCB*). PCB stores all the information the OS needs about a particular process.
### 2.2 Dual-mode Operation
- Breaks down executions on the CPU into two modes:
	- **Kernel mode** can execute with complete privileges. Can read/write to any memory, disk sector, access I/O devices, read any packages. 
	- **Use-mode:** executes with limited privileges. These privileges are only granted by the OS kernel. 
- On the x86, the user/kernel mode are stored in the EFLAGS register that maintains when and what an execution has privileges to do. 
- EFLAGS register tells the cpu which mode we are in. 
- Can change between these two mode through a safe control transfer. 
- Only way to access hardware is through kernel
- **Privileged instructions:** change page tables, disable/enable interrupts, do I/O, set timer
- **Memory protection:** user cannot touch kernel memory or other processes
- **Timer interrupts:** periodic preemption $\to$ kernel stays in control
#### Model of a CPU
- PC (Program counter): points at the next instruction; normal execution advances PC; branches/exceptions change it. 
- CPU: fetch and execute stage
- Fetches instruction from cache hierarchy and executes it through a pipeline. Everytime we fetches an instruction we increment the program counter (typically by 4 bites)
Limits on memory accesses
#### Simple Memory Protection
- Base and Bound: hardware checks every user memory reference falls in (base, base + bound), out-of-range causes an exception
- Virtual addresses:: Hardware translates via a table the kernel sets up (page tables), solving base/bound limitations
- CPU receiving instructions referencing memory. CPU only sees virtual addresses.
- Physical addresses
#### Hardware Timer
- Hardware device that periodically interrupts the processor (CPU), returning control to a kernel handler
### 2.3 Types of Mode Transfer
- User $\to$ kernel, when/why:
	- *Interrupts* (from devices/timer, async)
	- *Exceptions* (faults/traps from code, divide-by-zero, page fault)
	- *System calls* (program asks kernel to act)
	- Needeed for protection and services
	- How?
		- Common entry sequence must guarantee: limited entry points (IVT), atomic change of mode/PC/SP/memory protection, and transparent restartability. Then the handler runs
- Kernel $\to$ User 
	- *Return* from interrupt/exception/syscall (resume same place)
	- *Start* a new process/thread (jump to first instruction)
	- Context *switch* (resume a different process)
	- *Upcall* (e.g, UNIX signal handler entry)
	- How?
		- Kernel restores saved registers and PC, flips mode back to user, and resumes or start a new process/thread, context-switches to another process, or performs an upcall.
### 2.4 Implementing Safe Mode Transfer (the "gate" details)
**Core properties:** limited entry points, atomic state change, restartable user execution
- **Interrupt Vector Table:** Array of kernel entry points for each event type
- **Interrupt Stack:** kernel uses a separate privileged stack on entry
- **Two Stacks per Process:** user stack for user code; kernel stack for in-kernel work and to park saved user context
- **Interrupts masking:** temporarily defer nested interrupts while switching/scheduling
- **Save/Restore registers:** hardware + handler cooperate to push/pop full context reliably
### 2.6 Implementing Secure Syscalls
- **Limited gates:** only vetted entry points may be used
- **Validation:** check syscall number, copy in/out user buffers, validate pointers/lengths, check permissions
- **Principle of least privileged:** never trust user memory after validation (TOCTTOU hazards)
**Checklist for syscall handler:** parse $\to$ validate $\to$ copy-in $\to$ operate $\to$ copy-out $\to$ return
### 2.7 Starting a New Process
- Start with allocating and init PCB
- Memory is allocated for the process before the program is copied from the disk.
- The PC is then set to be the first instruction of the process
	- Stack pointer 
- 

### 2.8 Implementing Upcalls
- **What is upcalls:** A way for the kernel to notify a user process immediately
- **When/why:** User-level threads time-slicing, async I/O completion, debugger stop/resume.
- **How?** Kernel saves user CPU state, arranges a user-mode stack frame, jumps to the process's signal handler, then restores state so the app resume where it paused. Signals can be masked while handler runs 
### 2.9 Device Interrupts
- **What:** Async hardware events (timer tick, dick done, mouse move) that preempt the running code and enter the kernel
- **When/why:** to preempt for fairness /timer, handle I/O completions, coordinate CPUs, avoid wasteful polling.
- **How** hardware finishes/annuls instruction, switches to kernel mode, switches to the kernel interrupts stack, pushes saved PC/SP/status, and vectores to the right handler via the interrupt vector table. Handler saves other regs, and returns; the kernel restores state so user code resumes as if nothing happened
- Devices operate async from the CPU, two kernel strategies
	- Polling vs Interrupts
- **Device access to memory**
	- Programmed I/O: CPU copies data
	- Direct memory access (DMA): devices performs copies directly
	- Queue of buffer descriptors: Batch multiple DMA segments
### How to take interrupts safely
1. Interrupts vector with vetted kernel entry points
	- Table set up by OS kernel
2. Atomic transfer of control: in one hardware step, switch mode, PC, SP, and protection
3. Kernel (interrupt) stack: never run the handler on user memory
4. Save/restore registers (hardware + handler)
5. Interrupts masking during the tiny critical window, then re-enable quickly to avoid drops. Split work into bottom half (fast, marked) and top half (deferred, with interrupts enabled)
**Upcalls vs Interrups**
- Signal handler = interrupt vevctor
- Signal stack = interrupt stack.
### Some linux and C programming 
strace
