How the operating system or the combination of the operating system and the hardware, translating virtual addresses (address space of a process) into physical addresses (address space of physical memory) 
**Main Points**
- Address Translation Concept
	- How do we convert a virtual address to a physical address?
- Flexible Address Translation: how we can accomplish many things at once: process isolation, interprocess communication, shared code segments.
	- Base and bound
	- Segmentation
	- Paging
	- Multilevel translation
- Efficient Address Translation
	- Translation Lookaside Buffers
	- Virtually and physically addressed caches
### Virtual memory & address translation
**What is virtual memory?**
- Generally: the address space a process can “see”. Addressable memory location within a processes memory space.
- Address space size dependent on instruction set architecture (register size)
	- 64 bit CPU? 64 bit memory addresses 
	- 32 bit CPU? 32 bit memory addresses 
- How large can virtual address space/virtual memory be?
	- Exabytes
- How large is a typical physical memory?
	- 8, 16, 32 gigabytes 
	
Address Translation Concept
- Processor take virtual address (if valid) to convert them into physical address 
- Invalid address will raise CPU exception
	- Try to access address 0 or addresses that is not currently mapped into our process address space

**Address Translation Goals**
- Memory protection. process isolation
	- Process not access memory outside of process space
- Memory sharing
	- Shared libraries, interprocess communication
- Sparse addresses
	- Processes that have multiple regions of dynamic allocation (heaps/stacks). Dont allocate a heap all at once to a process because we never know how much memory a process may need. Want to allocate parts of memory as the program runs
- Efficiency address transition
	- Memory placement 
	- Runtime lookup: address translation faster than the instructions that the program want to execute 
	- Compact translation tables: efficient in space
- Portability
	- Want to provide a standardized interface for address translation, so we dont have to change the kernel when we want to support a different architecture. Can only have a different hardware independent mapping from datastructures to the architecture. 
**Virtually Addressed Base and Bounds**
- To provide process isolation is with base and bounds registers. Each process gets a base and bound register, when a process is swapped into the CPU we have to update so the CPU knows when we have a valid address 
- Processors view
	- Fetch virtual memory address and write to the address
	- Only sees virtual memory address
- Implementation
	- Base added to the virtual address that points to the lower limit in physical memory the process can address. 
	- If address is beyond the bound, raise exception 
- With virtually addressed base and bounds, what is saved/restored on a process context switch?
	- When switch process registers need to be updated because they are added to the virtual addresses that each process will be referencing. Save and restore these base and bound register
- Pros?
	- Simple: 
	- Fast (2 registers, adder, comparator)
	- Safe
	- Can relocate in physical memory without changing process
- Cons?
	- Can’t keep program from accidentally overwriting its own code
	- Can’t share code/data with other processes
	- Can’t grow stack/heap as needed

### Segmentation
- Segmented memory: dividing virtual and physical memory into small pages/separate memory segments, each with a specific purpose like code, data or stack.
	- *Segment* is a contiguous region of virtual memory
- Each process has a segment table (in hardware)
	- Entry in the table = segment
- Segment can be located anywhere in physical memory
	- Each segment has: start, length, access permission
- Processes can share segments
	- Same start, length, and same or different access permissions
- Processors view: 
	- Processor, virtual addresses and virtual memory
- Implementation
	- Provide an interface the physical memory
	- Virtual address sent over the memory bus through a segment table, which divides the virtual address up into two parts: segment number and offset.
		- Segment number directly addresses an entry in the segment table
		- In the segment table we have base, bound and access permission
	- Segment is looked up in the table, base fetched and added to the offset, and that gives us the start of the physical address. Provides the mapping between virtual and physical memory
	- Bound is used to raise an exception if we go beyond the bound
- **Question: With segmentation, what is saved/restored on a process context switch?**
	- Generally when doing a process switch: Save the CPU registers in the process descriptor and PCB, save pointer to the stack and base pointer, instruction pointer
	- If using segmented memory address translation, we need to store segment table for each process

**UNIX fork and Copy on Write**
- UNIX fork command: creates a complete copy of a process
- Segments allow a more efficient implementation by *copy on write*: 
	- Copy segment table into the child (pointing to the same physical memory)
	- Mark parent and child segments read-only
	- Start child process and return to parent. Gets return value from fork: child get 0 and parent gets the process idea of the child process
	- If child or parent writes to a segment, ex: stack or heap, trap into kernel with syscall and make a copy of the segment and resume
		- allows to read-only segments until we need to make a write

**Zero-on-Reference**
- **Definition:** Case where we do not know how much physical memory is needed for the stack or heap in advance. Stack and heap can go
	- How much is needed is what is currently in use.
- When the program uses memory beyond the end of stack 
	- Segmentation fault into OS kernel
	- Kernel allocates some memory
	- Zeros the memory
		- avoid accidentally leaking information
	- Modify segment table: update the position of new memory segment
	- Resume process

**Pros and cons segmentation**
- Pros?
	- Can share code/data segments between processes
	- Can protect code segment from being overwritten
	- Can transparently grow stack/heap as needed (allocate memory as needed by the process)
	- Can detect if need to copy-on-write
- Cons?
	- Complex memory management
		- Every time we put a segment into the physical memory we need to find free space of a that particular size
	- May need to rearrange memory from time to time to make room for new segment or growing segment
		- External fragmentation: wasted space between chunks

### Paging

**Paged Translation**
- Manage memory in *fixed size units*, or *pages*. In segmentation segments have different sizes which led to fragmentation. Paging solved the problem of fragmentation.
- Finding a free page is easy
	- Bitmap allocation: 0011111100000001100, scanning at the beginning and check if a page is unused (1 represent used, 0 unused)
	- Each bit represents one physical page frame
	- Pages can be anywhere in memory, does not need to have memory contiguous as in segmentation
	- Page size = frame size
- Each process has its own *page table*, (replacing segment table with page table)
	- Table stored in physical memory, part of PCB or pointed to from the PCB 
	- Need hardware registers to pointer to page table start and page table length

**Paged Translation (Abstract, processors view)** 
- Allocating 
**Paged Translation (Implementation)**
- Processor gets a virtual address and we divides virtual address into two parts which gives us the absolute location in the physical memory: 
	- Page number which identifies the location in the page table 
	- Offset added to the frame number which is indexed in the page table
- Page references a frame number

**Paging Questions**
- With paging, what is saved/restored on a process context switch?
	- Pointer to page table, size of page table
	- Page table itself is in main memory (physical memory)
- What if page size is very small?
	- Frame size has to be the same size. 
	- Much larger page table, divide same amount of memory in to smaller chunks. It will reduce fragmentation, but a lot of overhead managing 
- What if page size is very large?
	- Internal fragmentation: if we don’t need all of the space inside a fixed size chunk

**Paging and Copy on Write**
- If we want to share memory between processes
	- Can set entries in both page tables to point to same page frames
	- Need core map of page frames to track which processes are pointing to which page frames (e.g., reference count)
- UNIX fork with copy on write
	- Copy page table of parent into child process
	- Mark all pages (in new and old page tables) as read-only
	- Trap into kernel on write (in child or parent)
	- Copy page
	- Mark both as writeable
	- Resume execution

**Fill On Demand**
- Can start running a program before its code is stored in physical memory by:
	- Set all page table entries to invalid
	- When a page is referenced for first time, kernel trap
	- Kernel brings page in from disk
	- Resume execution
	- Remaining pages can be transferred in the background while program is running 

**Weaknesses with page translation**
- Might want many separate dynamic segments:
	- Per-processor heaps
	- Per-thread stacks
	- Memory-mapped files
	- Dynamically linked libraries
- What if virtual address space is large?
	- 32-bits, 4KB pages => 500K page table entries
	- 64-bits => 4 quadrillion page table entries
	- A lot of overhead for memory and can cause internal fragmentation which is wasted memory

**Multi-level Translation**
- Tree of translation tables
	- Paged segmentation
	- Multi-level page tables
	- Multi-level paged segmentation
- Fixed-size page as lowest level unit of allocation
	- Efficient memory allocation (compared to segments)
	- Efficient for sparse addresses (compared to paging)
	- Efficient disk transfers (fixed size units)
	- Easier to build translation lookaside buffers
	- Efficient reverse lookup (from physical -> virtual)
	- Variable granularity for protection/sharing

### Paged Segmentation
- Process memory is segmented
- Segment table entry:
	- Pointer to page table
	- Page table length (number of pages in that segment)
	- Access permissions
- Page table entry:
	- Page frame
	- Access permissions
- Share/protection at either page or segment-level
Paged Segmentation (Implementation)

**Question**
- With paged segmentation, what must be saved/restored across a process context switch?
	- Since we are hierarchical here we can store the segment table per process

### Portability
- Many operating systems keep their own memory translation data structures
	- List of memory objects (segments)
	- Virtual page -> physical page frame
	- Physical page frame -> set of virtual pages
- One approach: Inverted page table
	- Hash from virtual page -> physical page
	- Space proportional to the number of physical pages

### Efficient Address Translation
- **Translation lookaside buffer (TLB):** provide more efficient address translation by caching the addresses to recently addressed pages, thereby avoiding walking page tables.
	- Cache that holds recent translations between virtual pages physical page 
	- Hold a table of recently used translations
	- If cache hit (if we find the virtual address that we looking to translate in the TLB), use translation, take it out of the cache memory directly  
	- If cache miss, walk multi-level page table, revert back to regular virtual to physical page translation
- Cost of translation $=$ Cost of TLB lookup $+$ Prob(TLB miss) $\cdot$ cost of page table lookup (how much time it takes)

TLB and Page Table Translation
TLB Lookup

**Hardware Design Principle:** The bigger the memory, the slower the memory
- **Memory Hierarchy**
	- Hit cost for cache with less size is cheaper 

**When Do TLBs Work/Not Work**
- TLB populated per process, contains virtual addresses of a certain process that is currently running on the CPU 
- What happens on a context switch?
	- Reuse TLB?
	- Discard TLB?
- Solution: Tagged TLB
	- Each TLB entry has process ID
	- TLB hit only if process ID matches current process

**Address Translation Uses**
- Process isolation
	- Keep a process from touching anyone else’s memory, or the kernel’s
- Efficient interprocess communication
	- Shared regions of memory between processes
- Shared code segments
	- E.g., common libraries used by many different programs
- Program initialization
	- Start running a program before it is entirely in memory
- Dynamic memory allocation
	- Allocate and initialize stack/heap pages on demand
- Program debugging
	- Data breakpoints when address is accessed
- Zero-copy I/O 
	- Directly from I/O device into/out of user memory
- Memory mapped files
	- Access file data using load/store instructions
- Demand-paged virtual memory
	- Illusion of near-infinite memory, backed by disk or memory on other machines