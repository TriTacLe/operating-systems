File systems implement a dictionary that maps key (file name and offset) to values (block number)
### 11.1 Implementation
Four key ideas: directories, index structure, free space maps, and locality heuristics.
File systems map file names and file offset to storage block in two steps:
1. Directories to map names til file number
2. Uses the offset and a index structure such as a persistently stored tree to find the storage block 

**Free space maps:** A file system data structure used to track which storage blocks are free and which are in use as files grow and shrink.

**Locality heuristics:** A file system block allocation policy that places files in nearby disk sectors if they are likely to be read or written at the same time.

### 11.2 Directories: Naming data
A directory is a file that contains a collection of *file name* $\to$ *file number* mappings

**Hard links:** mapping between a file name and the underlying file number, often when there are multiple path names for the same underlying file 

To handle garbage collection, file systems uses reference counts by storing with each file the number of hard links to it. When a file is created, it has reference count of one, and each additional hard link made to the file, increments its reference count. When the reference count is decremented to zero, the underlying file is removed.

**Soft links:** directory entries that map one file or directory name to another name, rather than mapping a name to a number.

When a file attribute like size changes, all of a file's directory entries would have been located and updated. 

FAT stores file metadata in directory entries, but does not support hard links.

### 11.3 Files: finding data
Once the file system har translated the file name to the file number using a directory, the file system must be able to find the blocks that belong to that file.

**Spatial locality:** programs tend to reference instructions and data near those that have been recently accessed.

**Inodes:** In FFS and related file system, an inode stores s file's metadata, including an array of pointers that can be used to find all of the file's blocks. The term inode is sometimes used more generally to refer to any file system's per-file metadata data structure.

**File system approach**
*FAT. The Microsoft File Allocation Table.*
- Linked list as an index structure.
- Used in flash memory sticks and digital cameras.
*FFS. The Unix Fast File System.*
- Uses three-based multilevel index for its index structure to improve random access efficiency, and it uses a collection of *locality heuristics* to get good spatial locality for wide range of workloads.
*NTFS. The Microsoft New Technology File System.*
- Replacement for FAT
- Three-based index structure, but the three is more flexible than FFS's fixed three
- Optimizes its index structure for sequential file layout by indexing variable-sized extents rather than individual blocks.
- Primary file system for Microsoft OS's.
*COW/ZFS. Copy-on-write.*
- Update existing data and metadata blocks by writing new version to free disk blocks. This optimizes write perfomance: because any data and metadata can be written to any free space on disk, the file system can group otherwise random writes into large, sequential group writes. 

#### 11.1.1 FAT linked list
- FAT-32 (most recent version) that support volumes up to $2^{28}$ blocks and files up to $2^{32} - 1$ bytes (~4GB)
- Entries in FAT are 32 bits, where 4 is reserved, leaving 28 bits on disk)
#### 11.3.2 FFS: Fixed Tree

#### 11.1.3 NTFS: Flexible Tree With Extents
**Extents:** variable-size regions of files that are each stored in contiguous region on the storage device. 

Master file table (MFT)
**Attribute records:** a variable-size data structure containing either file data or file metadata
**Resident attribute** stores its content directly in the MFT record
**Non-resident attribute** stores extent pointers in its MFT record and stores its content in those extents

