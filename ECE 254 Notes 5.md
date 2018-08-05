# ECE 254 Notes Part 5

### Lecture 30 - Input/Output Devices and Drivers

- I/O Devices have different data rates



**Application I/O Interface**

- Devices comes in all forms. Here are some of the dimensions:

  1. <u>Data Transfer Mode</u>
     - Character vs block (of data)
  2. <u>Access Method</u>
     - Access next location (e.g. modem) or access anywhere (e.g. CD)
  3. <u>Transfer Schedule</u>
     - Synchronous (e.g. tape) vs asynchronous (e.g. web request)
  4. <u>Dedication</u>
     - Can be used by multiple threads (e.g. speakers) or only 1 thread (e.g. printer)
  5. <u>Device Speed</u>
  6. <u>Transfer Direction</u>
     - Read only or read/write or write only

- Uniform programmed interface for all hardware

- <u>Device Drivers</u> - Translates commands from OS to hardware instructions

  - Problem: Hardware developers made poor device drivers
    - Especially on Windows, as device drivers ran at kernel level. As a result, an error will cause Blue Screen of Death
    - LINUX and Mac OS has user-mode and intermediate level (between user and kernel) drivers
  - Solution:
    - Windows included a lot more device drivers
    - Windows introduced static driver verifier, which will check drivers at compile/build time to verify that drivers will behave

- Kernel has I/O subsystem to mediate between device controller and kernel

  - ![lect30-kernel_io_system](C:/Users/Richard%20Cao/Documents/2B%20Term/ECE254/Notes/Graphics/lect30-kernel_io_system.PNG)

- Escape system (aka. escape sequence)

  - Application developers can pass in commands that OS developers didn't think of to the hardware (using an escape character or something)

  

**Block vs Character I/O**

<u>Block Devices</u>

- All block devices support `read`, `write`, and `seek` (for random-access devices) so OS can call the hardware through the interface
- OS can make it even easier for application developers through a <u>memory-mapped file</u>, making read/write commands as simple as read/write from "memory"

<u>Character-Oriented Device</u>

- System calls are `get` and `put`
- Libraries and other structures can implement `read/write` of whole lines
- Generally devices that produce small amounts of data and unpredictably. Or linear streams of bytes (e.g. printer, speakers)



**Buffering**

- Area of memory that stores data being transferred
- Good to deal with speed mismatch
- <u>Double Buffering</u>
  - While buffer 1 is being read, data is written into buffer 2



**Network Devices**

- Uses <u>sockets</u> rather than `read/write` or `seek`
  - Server opens sockets
  - Client "plugs into" socket
- Servers with multiple sockets uses `select` to manage
  - Which sockets have packet waiting to be received by client
  - And which sockets have packet to send

**Spooling and Reservations**

- <u>Spool:</u> A buffer for a device that can only serve one job at a time
- Best example is printer
- OS sends all communications through the spooling system

**I/O Protection**

- all requests go through OS to determine if mode allows for such request
- Some devices will allow direct access to speed performance
  - e.g. graphic cards

**Kernel I/O Data Structures**

- Kernel must keep track of the state of I/O devices and which processes are using them
- FYI only
  - ![lect30-UNIX_io_info](.\Graphics\lect30-UNIX_io_info.PNG)

**I/O Scheduling**

- I/O requests are scheduled for efficiency (no duplicate requests)
  - I/O requests are grouped
- Priority also taken into account



**Transforming I/O Requests to Hardware Operations**

<u>I/O Request Life Cycle</u>

![lect30-io_request-lifecycle](.\Graphics\lect30-io_request-lifecycle.PNG)

<u>Steps (in written form)</u>

1. Process issues a command (e.g. `read`)
2. System checks command for correctness
   - If data already exists (in cache or buffer), return it straight away
3. Otherwise, block process and I/O request sent to device driver
4. Device driver allocates buffer to receive data. The request is sent to device
5. Device controller operates hardware
6. Driver may poll, await interrupt, or wait for DMA signal
7. Interrupt handler receives and stores data. Signals device driver when complete
8. Device driver identifies the operation and state, then tells kernel I/O subsystem
9. Kernel transfers data to memory space of requesting process
   - Also unblocks process
10. Process resumes execution when selected by scheduler



### Lecture 31 - Disk Scheduling

- For magnetic hard disks
- SSD disk reads/writes have uniform access times so scheduling is FCFS



**Magnetic Disks**

- Circular <u>tracks</u>, each divided into <u>sectors</u>
- Two values that measure disk performance
  1. <u>Transfer Rate:</u> Speed at which data is moved from disk to computer
  2. <u>Random-Access Time</u>: how long it takes to get to particular piece of data
     - <u>Seek-Time</u>: moving the arm to right position
     - <u>Rotational Latency</u>: time for disk to rotate to right position
- Total Average Access Time: - **REVIEW SLIDES for equation**
  - ![lect31-disk_access_time](.\Graphics\lect31-disk_access_time.PNG)
    - `Ts` is average seek time
    - `r` is rotation speed
    - `b` is number of bytes to transfer
    - `N` is # of bytes on a track
  - <u>Sequentially Organized</u>
    - file is stored compactly on disk (e.g. on adjacent tracks)
    - Considerably faster than data that is randomly distributed as seek time + rotation time is added per each sector of data



**Disk Scheduling Algorithms**

- <u>Bandwidth:</u> Total # of bytes transferred / total time between request to completion



<u>Disk Request Info</u>

1. If operation is read/write
2. Disk Address of transfer
3. Memory Address of Transfer
4. Amount of data to transfer



<u>Algorithms</u> (check [slides](https://learn.uwaterloo.ca/d2l/le/content/391638/viewContent/2170175/View) for pictures)

- Cylinder is essentially a track but regards all disks rather than a track on just 1 disk

1. <u>Highest Priority First</u>
   - Priority provided by process
2. <u>First-Come First-Served (FCFS)</u>
   - A lot of wasted time moving the head (compared to moving the head to a sequential spot)
3. <u>Shortest Seek Time First</u>
   - Choose the request with the least seek time (solves above problem)
   - Pros:
     - Takes advantage of spatial locality
   - Cons:
     - Starvation
     - Not most optimal (doesn't pre-calculate overall distance travelled so huge leaps still possible
4. <u>SCAN Scheduling</u>
   - aka. elevator algorithm
   - Move from cylinder 1 to 199 and then reverse direction
   - Pros:
     - no starvation
   - Cons:
     - does not consider spatial locality
5. <u>C-SPAN Scheduling</u>
   - Instead of reversing direction, jump all the way back to cylinder 1
   - This is due to most requests likely being at other end of disk
6. <u>LOOK Scheduling</u>
   - optimization of SCAN scheduling by stopping only at last request (rather than ending all the way to cylinder 199)
   - C-LOOK is equivalent optimization of C-SCAN



**More Details**

- To prevent large amount of wait times, use 2 buffers
- Each buffer has capacity `C`, meaning requests will wait at most `C` accesses
  - Large C resembles SCAN
  - Small C resembles FCFS
- Since disk controller is responsible for placement of data, OS just provides grouping of requests and leave it to hard disk controller to schedule for best performance
- However, OS should handle some disk requests scheduling because of certain goals 
  - e.g. requests based on priority of processes



### Lecture 32 - File System Interface

<u>File:</u> a designated area of disk

- could contain data or is a program

File directories are like files too



**File Attributes**

1. Name - file name in human-readable form
2. Identifier - a number
3. Type - what kind of file
4. Location - physical location
5. Size - current size and max size
6. Protection - file creator/owner and who has read/write access
7. Time, Date, User ID



**File Operations**

1. Creating a file
   - Find place to put file
   - Allocate space
2. Writing a file
   - System finds file by name or ID
   - System writes data to file, replacing or appending to existing data
   - Pointer is saved for next write
3. Reading a file
   - System requires name or ID
   - Pointer is saved for next read
4. Repositioning within File
   - repositions the read/write pointer (aka. `seek` operation)
5. Deleting a file
   - Simple Delete (does not remove data):
     - Find file by name or ID
     - Make space as free
     - Remove from directory listing
6. Truncating a file
   - Erases file but keeps attributes



The six above basic operations allows for other operations

- e.g. Copy File == Create new file, read from old file, write to new file



Operations other than creating and deleting a file requires it to be open

- processes should close files when they are not using it



<u>Per-process</u> and <u>system-wide tables</u> are created to know which files are open and used by which processes



File locks may be exclusive or non-exclusive

- Non-exclusive might prevent deletion
- Exclusive might prevent opening
- Windows prevents deletion of files that are open
- UNIX allows deletion of files (will be marked free as soon as last process closes the file)



<u>File Types</u>

- `.txt` or `.doc`
- Any program can open any files. The types EW just a recommendation



**Access Methods**

- Open files are generally in main memory
- Inherent internal fragmentation

1. <u>Sequential Access:</u> information in file is processed in order
   - Some operations allow to go backwards by starting from beginning or reversing a few lines
2. <u>Direct Access:</u> any part of file can be accessed at any time
   - Due to disk systems using blocks, it takes time for OS to find the exact byte that process wants to access



**Directories**

<u>Operations</u>

1. Search - search by file name and/or search data within file
2. Add a File
3. Remove a File
4. List Directory
5. Rename File - might change file position if directory sorted by name
6. Navigate File System - open subdirectories or parent



<u>Basic UNIX Directory</u>

- Root directory at `/`
- Relative paths (depends on current directory) vs absolute paths



<u>Deletion</u>

- Some OSes prevent deletion of directories that have files
- Delete might mean "move to recycle bin"



<u>Links</u>: A pointer to a file

- Like "shortcuts" in Windows
- <u>Symlinks</u> - references by file name
- <u>Hardlinks</u> - similar to symlinks but files will not be deleted until all hardlinks are removed



**File Permissions**

1. Read
2. Write
3. Execute
4. Append (write at end of file)
5. Delete
6. List (view file attributes)



<u>UNIX-Style Permissions</u>

- each file has an owner, a group, and everyone
- 3 basic permissions - read, write, execute
- 10 bits
  - 1st bit is directory bit - "is this a directory file?"
  - every 3 bits is read, write, execute permissions for owner, group, everyone, in those respective orders
  - `-` means no permission, `d` means directory, `rwx` means read, write, execute
  - can also be represented in octal

<u>Access Control Lists</u>

- implemented in SELinux (Security Enhanced Linux), NTFS (Windows NT File System)
- infinite number of security descriptors
- More permissions than `rwx`
- Inheritance
  - any new file created in directory will inherit ACL
  - however, the file will retain ACL (unless explicitly set) when it is moved to another directory



### Lecture 33 - File System Implementation

**Layers of File System Design**

- Moving down the list == closer to hardware

1. The File System

   - Interface for users/application developers

2. I/O Control

   - Interface for device drivers and interrupt handler
   - Outputs are hardware-specific instructions

3. Basic File System

   - Identifies physical addresses (drive, track, sector)
   - Also interacts with cache and buffers on hard disk
     - Which has a trade off of losing data if power is cut

4. File Organization Module

   - translates logical block to physical address
   - keeps track of free space

5. Logical File System

   - manages metadata (file system structure, directory structure)
   - <u>File Control Block (FCB)</u> - contains data about a file
     - <u>Inode</u>: the UNIX name
     - Contains:
       - ownership
       - permissions
       - location of file contents
       - file dates (create, access, write)
       - file size

   

**Disk Organization**

<u>Basics</u>

- Each file system needs to keep track of
  - total number of blocks
  - number and location of free blocks
  - directory structure
  - file info
- 1 disk (generally first disk) will contain info on how to boot up OS
  - BIOS will transfer control to this disk on powerup
- <u>Partitions</u>: several different areas of disks
  - Partition table (aka superblock or master file table) tracks which part of disk belongs to which partition
  - In Windows, C: drive is 1 logical partition
  - In Linux, the disk is partitioned (e.g. temporary/swap directory, home directory, boot partition)



<u>Structures in Memory</u>

1. Mount Table 
   - information about each mounted volume (disk/partition)
2. Cache
   - directory info for recently accessed directories
3. Global Open File Table
   - copy of FCB for each open file
   - when user wants to modify a file, the `open` system call checks the global open file table
     - check lock access (exclusive vs non-exclusive)
     - store FCB if file is retrieved from disk
   - when a user wants to close a file, the process open file table will send a signal to remove FCB if no more processes are using file
4. Process Open File Table
   - references to global open file table but sorted by processes
   - additional info:
     - next section to read/write
     - access mode
   - when user wants to modify a file, the `open` system call returns a pointer
     - in UNIX, called <u>file descriptor</u>
     - in Windows, called <u>file handle</u>
   - when user wants to close a file, the entry is removed from process open file table
5. Buffers
   - location of buffers for read/write operations

![lect32-disk_operations](.\Graphics\lect32-disk_operations.PNG)



**Search with Spotlight** (from Mac OS)

- searches ignored file content
- updates metadata on creation and modification
- indexing maintained within Spotlight (vs. preparing that index when user decides to search)



**Virtual File System**

- abstraction layer than handles different file systems (FatFS, NTFS)
  - separates file system operations from actual implementation
  - mechanism to represent files consistently, even if system uses disks with different file systems
  - <u>vnode</u> - file representation structure (similar to FCB, inode)



<u>Linux's VFS Architecture</u>

1. Inode (individual file)
2. File (an open file)
3. Superblock (file system)
4. Dentry (director entry)

Each object has defined operations (read, write, open, flush, release)

- see [slides](https://learn.uwaterloo.ca/d2l/le/content/391638/viewContent/2170179/View) for headers



**Directory Implementation**

1. Linear List

   - Creation: If file name doesn't exist in search directory, then insert entry to list
   - Deletion: search list for matching file name
   - Pros:
     - Easiest Implementation
   - Cons:
     - Searches take a lot of time, especially if file content is searched

2. Hash Table

   - hashes made based on file name

3. B-Tree (with degree `d`)

   - **Best structure**

   - Structure:
     - Nodes and leaves contain keys
     - Nodes have at most `2d - 1` keys and `2d` children
     - Nodes (excluding root) have at least `d-1` keys and `d` children
   - Search Algorithm - I already know
   - Insertion Algorithm
     - If node has fewer than `2d-1` keys, insert accordingly
     - If node is full, split into 2 new nodes around median key
       - Median key is inserted into parent (which will split if parent was full and so on)
       - If root is split, height of tree increases by 1



### Lecture 34 - File Allocation Methods

3 ways to allocate disk space

1. Contiguous
2. Linked
3. Indexed



**Contiguous Allocation**

- File occupies continuous blocks
  - Given a file with size `n` blocks and starts at block `b`, then file takes up from block `b` to `b + (n - 1)`
- File system keeps track of `b` and `n`
  - ![lect34-contiguous_allocation](.\Graphics\lect34-contiguous_allocation.PNG)
- Pros:
  - Keeps seek time to nonexistent
  - Sequential and direct access is easy
    - If offset is `i`, check that `0 < i < n`, and then access `b + i`
- Cons:
  - Can we find contiguous block of size `n`
    - And choosing a block will cause external fragmentation
  - Requires regular compaction (moving memory around to create larger free spaces)
    - Very expensive and time-consuming
  - Requires clairvoyance: OS has no idea how much space user will use for file



**Linked Allocation**

- Each block has pointers to prev and next blocks
- Directory listing keeps track of head and tail
  - ![lect34-linked_allocation](.\Graphics\lect34-linked_allocation.PNG)
- File Creation:
  - A new entry is created with size 0 and `head == null`, `tail == null`
- Improvement: <u>Clusters</u> - group of blocks
  - Wastes less memory maintaining pointers
  - Better disk access time and direct access
- Pros:
  - Compaction and relocation are not major problems
- Cons:
  - Direct Access is no longer simple (following multiple pointers)



<u>File Allocation Table (FAT)</u>

- Very prominent since FAT32 supported by Windows, Mac OS X, Linux
- File data table at beginning of disk
  - One entry for each block
  - Directory entry for file points to first block
  - Each block has block number of next block or 0 if unused)
  - Last block has special value to indicate end-of-file
- Block Allocation - Find first 0-valued entry and replace previous `tail` value to new address
- ![lect34-FAT_allocation_system](.\Graphics\lect34-FAT_allocation_system.PNG)
- FAT should be cached (required quite often)



**Indexed Allocation**

- <u>Index Block:</u> Block pointers all stored in this block (generally starts at first block)
  - Go to index `i` to go to block `i`
  - Pointers start as null

![lect34-indexed_allocation](.\Graphics\lect34-indexed_allocation.PNG)



<u>Sizing of Index Block</u>

1. Linked Scheme - Index block is a disk block
   - several index blocks linked
   - last entry of index block is pointer to next index block (or null if it's the last index block)
2. Multilevel Index - index blocks have levels
   - For example, 1st level index block points to 2nd level index blocks. 2nd level index blocks either all point to 3rd level index blocks, or actual data
3. Combined Scheme
   - used by UNIX
   - First 12 pointers point to file data
   - 13th pointer points to indirect block (an index block that points to data)
   - 14th pointer points to double indirect block (an index block that points to index blocks that points to data)
   - 15th pointer points to triple indirect block (self-explanatory) 
   - ![lect34-UNIX_implementation](.\Graphics\lect34-UNIX_implementation.PNG)



**Other Disk Issues**

<u>Free Space</u>

- Similar to memory allocation (bitmap vs linked list to indicate free blocks)
- Improve efficiency by grouping 
  - First free block is a control block that stores addresses of `n-1` free blocks and `1` control block
  - Can also store number of contiguous blocks after each of the `n-1` addresses



<u>Preallocated UNIX Inodes</u>

- Improves file system performance
  - Can keep file data close to inode to reduce seek time



**Consistency Checking and Journalling**

<u>Checking + Correction</u>

- most OS runs this on startup
- check for and repair corrupted FCBs, inodes
- UNIX command: `fsck`
- Windows command: `chkdsk`



<u>Prevention</u>

- "atomic operations"
- <u>Transaction</u>: a list of all the things we plan to do and the transaction is only complete after all the things are done
  - Required metadata changes are written sequentially to a log file
  - Control may return to program but log entries are carried out
  - A pointer is updated to indicate which log entries have happened
  - Log entries are erased once ENTIRE transaction is completed
  - If system crashes, system will check log file to see if all transactions were completed
  - Aborted transactions == walking back on log entries (undoing operations)
- Pros:
  - system is always in consistent state
  - potential scheduling of writes
- Systems like Solaris ZFS puts new data and metadata into new blocks rather than overwrite data in old blocks
  - Only when a transaction is complete does old references get replaced 



**Example: NTFS (Windows File System)**

<u>Storage Levels</u>

1. Sectors - smallest physical storage unit (~512 bytes)
2. Cluster - one or more contiguous sectors (stored in power of 2)
   - fundamental unit of allocation
   - allows file system to be independent of physical sectors on disk
3. Volume - logical partition of disk (1+ clusters)
   - contains partition boot sector, master file table, system files block, and general files
   - <u>Master File Table (MFT)</u>: contains info about all files and folders
   - System Files: contains important system info
     1. MFT2: mirror of first few rows of MFT (as backup)
     2. Log File: see "transactions" above
     3. Cluster Bitmap: which clusters are in use
     4. Attribute Definition Table - attribute types supported by volume
        - ie. what attributes can the FCB have (e.g. file name, security descriptor, data)



<u>Journaling</u>

- Designated service responsible for maintaining a log file used for recovery
- Goal is to recover system into consistent state, not recover user data
- <u>Implementation</u>:
  1. Record changes in log into cache
  2. Modify the volume in the cache
  3. Flush log file to disk
  4. Flush volume changes



**Backups** - hard drives do fail so have backups



### Lecture 35 - Virtualization (Virtual Machines)



