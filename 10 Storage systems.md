## 10 File systems
**Non-volatile storage:** Unlike DRAM, memory that is durable and retains its state across crashes and power outages. 
- However non-volatile storage do not allow random access to individual words of storage; instead access must be done in more coarse-grained units - 512, 2048, or more bytes at a time.
### 10.1 The File System Abstraction
**File systems:** An OS abstraction that provides persistent, named data.
- **Persistent data:** data stored until it is explicitly deleted, even if the computer storing it crashes or loses power
- **Named data:** data than can be accessed by a human-readable identifier, such as a file name.

Two key parts to the file system abstraction:
- **Files:** named collection of data in a file system
- **Directories:** list of human-readable names plus mapping from each name to a specific file or sub-directory

File's information consist of two parts, **metadata** and **data**
- A file's data is just an array of untyped bytes.
**Path:** string that identifies a file or directory. Relative and absolute

**Hard link:** the mapping between a file name and the underlying file, typically when there are multiple path names for the same underlying file. 

**Block:** fixed-size chunk of data storage, ranging from $256 \text{KB}$ to $4\text{MB}$ 
- Each block is assigned a unique identifier that the storage system uses to find and retrieve it
- Doesn't store hight-level metadata

**Volume:** A collection of physical storage blocks that form a logical storage device (e.g., a logical disk).
- Abstraction that corresponds to a logical disk
- A single physical disk can be partitioned and store multiple volumes or several physical disks can be combined so that a single volume spans multiple physical disks.
 - **Mount:** A mapping of a path in the existing file system to the root directory of another file system volume.
	 - Used to join the file system's directory hierarchies
### 10.3 Software Layers
![[Screenshot 2025-10-25 at 21.46.46.png | 300]]
Software stack

**File system API and performance:** Top levels -  user-level libraries, kernel-level file system, and kernel-level block cache - provide a API for accessing named files and minimizes slow storage accessing via cache, write buffering, and prefetching.

**Device access:** Lower levels - provides ways for the OS too access a wide range of I/O devices.

Device drivers
Block device
DRAM
Memory mapped I/O
**Direct memory access (DMA):** Hardware I/O devices transfer data directly into/out of main memory at a location specific by the OS with continuous involvement of the CPU.
## 11 Storage devices 
### 11.1 Magnetic disk
**Magnetic disk:** non volatile storage technology used in laptops, desktops, and servers. Disk drives work by magnetically storing data on a thin metallic film that rotates. 
### 11.2 Flash storage
Used in handheld devices such as camera, phones, and thumb drives
Type of *solid state storage:* no moving parts and stores data using electrical circuits
