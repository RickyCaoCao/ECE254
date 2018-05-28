# ECE 254 Notes

## C Review

### Data Types

``` C
int x;
double y;
struct point {
    float x;
    float y;
    float z;
}
// There is a library for booleans. Use 0/1 instead.
// NULL is identical to 0


// Instantiation
point p1;
p1.x = 1.0;
p1.y = 2.0;
p1.z = 3.0;

// Casting
x = (int) 4.0;

// Convert char array to int
int some_int = atoi("324");

// Function with struct
struct point add (struct point p1, struct point p2){...}

// Alternatively, if we define the struct as:
typedef struct {
    int value;
    Node* next;
} Node;

// then the function can be
Node add(Node n1, Node n2){...}
```

### Pointers

```C
int* x = malloc(sizeof(int)); // allocates bytes in heap that a 'int' uses
// points to value in stack

// need to clear pointer value before end of function
free(x); // removes value in stack
```

- `*p1.x = 9.9` is the same as `p1->x = 9.9`

### Arrays

- functions that use arrays must need length passed in (no len function in C)

``` C
int x[] = malloc(10*sizeof(int));
int* y = malloc(10*sizeof(int));
```



### Functions

**Main**

` int main(int argc, char** argv) {...} `

- `argc` = # of arguments
  - program itself counts as 1
- `argv[0]` = name of program
- `argv[...]` = value of other arguments (as char array)

**Printf**

``` C
printf("Hello World!");

int x = 12;
printf("The value of x is %d. \n", x);
```

- See http://www.cplusplus.com/reference/cstdio/printf/ for specifiers (e.g. `%d` means signed int)

**For Loops**

`for(int i = 0; i < 10; i++){...}`

### Strings (aka char arrays)

- strings don't exist
- remember null terminator when passing into function: `"Hello World\0"`

### Headers

- including files: `#include <stdio.h>`
- defining consts: `#define ZERO 0`
  - this constant is substituted by compiler

### Conventions

``` C
int bar ( bag* i, int val) {...}
```

- returns 0 to indicate success, other numbers for failure codes
- first element (`bag* i`) is changed

## Course Notes

### Lecture 1 - Intro to OS

<u>Operating System</u> - Programs that interfaces the machine with application programs (e.g. memory allocation)

![modern_computer_diagram](.\Graphics\lect1-modern_computer.PNG)

- in other words, OS sits on top of hardware but underneath executing programs

<u>Kernel</u> - core of OS

- always present in main memory and CPU

<u>Systems Programming</u> - programs just above OS

- not part of OS
- for example:
  - file manipulation
  - status information
  - programming-language support
  - communication between programs

### Lecture 2 - Computer Architecture

**Computer Organization**

- OS needs to know details of hardware (e.g. resources available)
- To run a program, we need
  1. Main memory
  2. System Bus
  3. Process
  4. I/O (optional but program generally not useful otherwise)

![Computer Organization](.\Graphics\lect2-computer_organization.PNG)



**Memory**

Wants:

- Fast enough so processor doesn't have to wait
- Large enough to hold all the data
- Inexpensive

Since we can't have everything, there are different levels:

| Memory Levels      | Access Time | Total Capacity |
| :----------------- | :---------- | -------------- |
| Register           | 1 ns        | < 1KB          |
| Cache              | 2 ns        | 6 MB           |
| Main Memory        | 10 ns       | 16 GB          |
| Magnetic Hard Disk | 10 ms       | 500 GB         |

- faster the memory, the more expensive

**System Bus**

- communication for components
- modern computers have multiple buses

**CPU**

- fetch -> decode -> execute
  - different steps may be completed in parallel (pipelining)
- <u>Word</u>  = largest unit
  - 32-bit computer means a word is 32 bits
- CPU instructions and functions is specific to processor
  - some instructions only run on supervisor/admin mode (rather than user mode)
- Uses registers
  - OS is responsible for managing registers usage

**Critical Registers**

- PC (Program Counter) - stores location of next instruction
- Status Register - stores flags
- Instruction Register - holds current instruction
- Stack Pointer - points to top of stack (for push/pop operations)
- General Purpose Registers - stores data / addresses

**4 CPU Instruction Categories**

1. Processor \<-> Memory
2. Processor \<-> I/O
3. Data Processing (ALU)
4. Control (if, else statements)

**Interrupts**

- OS responsible for storing state, handle interrupt, and restoring state

- Sequential vs nested interrupts

  ![Computer Organization](.\Graphics\lect2-interrupt_types.PNG)

- Sources:

  1. Program
  2. Timer
  3. I/O
  4. Hardware failures

**Multiprogramming**

- saving current program and restoring another program (see future topic: scheduling)

**I/O Communication**

1. Programmed I/O - polling
2. Interrupt Driven I/O - Interrupts
3. Direct Memory Access (DMA) - CPU does setup for the following
   - Operation to perform (read/write)
   - Source and Destination
   - Amount of data to transfer



### Lecture 3 - Operating System Structure

**Traditional UNIX Structure**

- Layered structure
  - Layer 0 is hardware
  - Layer N is user interface
- Each layer contains modules that are static, loadable and swappable
- <u>Most modern OSes follow this structure</u>

**Layered Operating System**

- Programs execute in OS environment using services that OS provides

- <u>Services</u>

  ![lect3-os_structure](.\Graphics\lect3-os_structure.PNG)

  1. <u>User Interface</u> - GUI (Windows desktop), batch interface (commands in files that periodically run), command line interface (CLI)
  2. <u>System Calls</u> - functions that can be called from user level that initiates OS functions
  3. <u>Program Execution</u> - Loads program into memory
  4. <u>I/O Operations</u> - OS manages I/O devices and functions that read or write to the devices
  5. <u>File Systems</u> - OS manage files
  6. <u>Communication</u> - manages communication to the internet or between multiple processes
  7. <u>Resource Allocation</u> - allocates memory for multiple programs or users
  8. <u>Accounting</u> - tracks statistical data to ensure stability
     - program can terminate programs using too much CPU or memory
  9. <u>Error Detection</u> - handles all errors (from user programs to I/O devices)
     - can either terminate program or OS crashes (blue screen of death)
  10. <u>Protection and Security</u> - manages authentication and enforces access rights on users and programs

**Traps** - software generated interrupts

- generated by errors or user program request
- OS typically sends the error back to the program to handle
  - if program doesn't handle error, it will crash

**Modes and Privileges**

- Supervisor (kernel) mode 
  - can run all instructions and operations
- User Mode
  - user programs always start in user mode
  - can't run all instructions and operations so needs to enter "kernel" mode
  - uses traps to run OS instructions
    - user programs will send traps into a queue (to offset "greedy" programs)
- Performance impact but is good tradeoff for security
  - prevents two programs from modifying 1 file at same time

![lect3-modes](.\Graphics\lect3-modes.PNG)

**Example: Reading from Disks**

` int bytesRead = readfile( file, buffer, numBytes);`

1. User program pushes parameters onto stack
2. User program invoke system call
3. System call puts identifier in predefined location
4. System call issues trap to activate OS
5. OS responds to `trap` and gets identifier
6. OS runs matching system call handler
7. Routine executes.
8. When handler finishes, control exits kernel and goes back to system call (user mode)
9. System call returns control to user program

### Lecture 4 - Processes

**The Process** - to allow for multiple programs

1. Instructions + data
2. Current state
3. Resources needed for execution (memory, I/O device)

**OS Requirements** - mostly revolve around processes

- scheduling
- resource allocation
- inter-process communication

**Process Control Block (PCB)**

- created/updated by OS for each running process
- contains everything OS needs to know about the process
- Contents:
  1. Identifier (e.g. PID)
  2. State
  3. Priority
  4. PC
  5. Register Data - to save data quickly (e.g. in case of interrupt)
  6. Memory Pointers - OS can clean up after process terminates
  7. I/O status info
  8. Accounting info

**Process Switching**

- maximize time between saving 1 state and reloading another

![lect4-process_switching](.\Graphics\lect4-process_switching.PNG)

**Process Cycle**

1. Create PCB
2. Init data in block
3. Set vars, initial program state, instruction pointer to 1st instruction in `main`
4. Add PCB to list of PCBs in OS

**Program Termination**

- collects accounting info
- remove PCB from list of active processes

**Process Creators**

1. System bootstrap
   - will create both foreground (login screen) and background tasks (Daemon in Unix)
2. Users
3. Spawned by another process (e.g. clicking on an external link opens up web browser)
   - can help break program into different logic parts for parallelism and fault tolerance
   - Relationship: <u>parent</u> spawns <u>child</u>

**Process Exits**

1. <u>Normal exit</u> (voluntary) - program has finished successfully
2. <u>Error exit</u> (voluntary) - program exited due to error
3. <u>Fatal Error</u> (involuntary) - OS sends error not handled by program
4. <u>Killed by Another Process (involuntary)</u>
   - typically initialized by user
   - user or process must have rights to execute victim
   - users can only kill processes that they have created (unless user is admin)
   - <u>Killing parent process will also kill child processes</u>

**Process Family Tree in UNIX**

- UNIX maintains parent-child relationship (but not Windows)
- <u>Process Group</u>: process and all its descendants
  - certain operations can send signals to whole group
- 1st process in Unix (aka parent of all processes): `init`

**Process Family Tree: Windows**

- Parent gets reference of child
- Parent can give child to another process (adoption)

**Process Return Codes**

- CLI or GUI return values are not presented to user
- In UNIX, parent gets the code the child returns
  - `0` means success

**Zombies**

- when child process has finished but parent has not collected return value

- zombies holds onto allocated resources until return value is collected
- sometimes, parents wait for child result -- this is not considered a zombie
- <u>reaped:</u> cleaning finished processes

**Orphans** - a child without a parent

- in UNIX, orphans are adopted by `init`
- `init` will `wait` on child processes but discard return values
- processes that run in the background are intentionally orphaned

### Lecture 5 - Process State

- process stored in PCB

**Two State Model**

![lect5-two_state](.\Graphics\lect5-two_state.PNG)

States:

1. Running
2. Not-Running

Transitions

1. Create
2. Dispatch: process that begins executing
3. Suspend: pauses execution
4. Exit: `exit` and removed from list of processes

**Three State Model**

![lect5-three_state](.\Graphics\lect5-three_state.PNG)

New States

- Ready: Not-running process that is ready to execute
- Blocked: waiting for resource (memory allocation, user input)

New Transitions

- Blocked
- Unblocked

**Five State Model**

![lect5-five_state](.\Graphics\lect5-five_state.PNG)

New States

- Terminated - finished process that has not been reaped (cleaned)
- New: New process with ID and PCB but not added to list of ready processes
  - process may also still be on disk rather than in memory

New Transitions

- Admit: New -> Ready
- Reap: terminated process's return value is collected by `wait`
- Exit (not shown on graph) from Ready/Blocked - occurs when process is killed

**Seven State Model**

Problem - Memory Issues

- We cannot start programs if there is not enough space in memory
- Not enough memory for all ready processes
- Programs do not know how much memory it needs

- **Solution:** put programs on disk

![lect5-seven_state](C:\Users\Richard Cao\Documents\2B Term\ECE 254\Notes\Graphics\lect5-seven_state.PNG)

New State

- Ready/Swapped: process ready to run, not in memory
- Blocked/Swapped: process not ready to run, not in memory

New Transitions

- Admit: new programs are not placed in memory if there is not enough space
  - programs don't even know how much memory it will use
  - hence, it goes from New -> Ready/Swapped
- Unblock (Blocked/Swapped -> Ready/Swapped)
- Unshown 'Exits' - program moves to terminated from any state if killed

**Storing Data Structures**

- different queues for different states (e.g. "waiting for disk" queue, "ready" queue)

### Lecture 6 - Process in UNIX

- All processes have the `init` process as eventual parent 
- Process ID = pid
  - `init` has pid of 1

![lect6-unix_process_tree](.\Graphics\lect6-unix_process_tree.PNG)



**Commands**

- `ps`: obtain list of processes
- `ls` or `top`: provides table of processes
  - command itself is a process
- `<some_command> &`: the & allows the command to run in background
  - will receive update when process is finished
  - essentially, & gets parent to disown child and have `init` adopt child
  - `sudo service xyz start &` : runs service xyz in background
    - `tail -f /var/log/xyz/console.log` to access console log of xyz service
- `<command> > <file.txt>`: the > will pipe results into output file
- `kill <signal> <PID>` 
  - `SIGHUP` - (value == 1): disconnects child from parent process
  - `SIGINT`- (value == 2): keyboard interrupt (Ctrl + C)
  - `SIGKILL` (val == 9): kill signal
  - `SIGTERM` (val = 15): termination signal (termination can be gracefully handled by PID)
- `memset(&var, 0, SIZE)` = sets 'var' pointer to 0 x SIZE
  - need initialization or there will be bad values

**'Screen' Command**

- tabs for terminal
- `screen vi file.txt`

**Spawning Child Processes**

- child can request resources from OS or parent
- parent spawns child with `fork()`
  - child is a copy of parent,  including state (eg. what line of code the parent is on)
  - `Fork` Return Codes
    - < 0 means failed
    - 0 means process is child
    - \> 0 means process is parent, value is PID of child
  - parent can wait for child to send a return code (eg. parent is )
    - **??? how does parent spawn multiple children that all do something ??? ** 
  - **see code example**
  - child can use system call `exit(1)` to terminate normally

**9 Unix States**

![lect6-unix_process_states](.\Graphics\lect6-unix_process_states.PNG)

1. User Running
2. Kernel Running
3. Ready to Run in memory
4. Asleep in memory
5. Ready to Run, Swapped
6. Sleeping, Swapped
7. **Preempted** - logically same as **ready to run, in memory**; happens when process is being returned from kernel mode to user mode but the kernel is going to run another process in the meantime
8. Created
9. Zombie

**Fork() Breakdown**

1. Allocates slot in process table/list
2. Assigns unique PID to child process
3. Copy process image of parent (except shared memory)
4. Increment counters for any files (to indicate more processes are using the files)
5. Child process enters "Ready to Run"
6. Values are returned (0 for child, PID for parent)

**Fork() Bomb**

- DDoS using while loop with `fork()` call



### Lecture 7 - Inter-process Communication (IPC)

- when 2+ processes would like to exchange data
- <u>Message</u>: data being transferred
- <u>Sender</u>: process sending the message
- <u>Receiver</u>: process receiving the message
- <u>Producer:</u> creates data
- <u>Consumer:</u> uses data
- Format of message (e.g. XML) is determined by sender/receiver, not OS

**Message Types**

- Synchronous Send: sender sends message and becomes blocked until acknowledgement
- Asynchronous Send: Send can post message and continue
- Synch. Receive: Receiver is blocked until a message is received
- Async. Receive: Receiver continues execution even if no message is available
  - also common to send ack. message

**Message Combinations**

1. Sync. send -> sync. receive
2. Sync. send -> async. receive (least common)
3. Async. send -> sync. receive
4. Async. send -> async. receive

**IPC Implementation Approaches**

1. Shared memory
   - notify OS of shared memory
     - OS can allow multiple processes to use the memory
     - OS knows not to free memory until both processes are terminated
2. File System
   - advantage over shared memory is that it can survive reboot
   - sender and receiver do not need to know the other exists
   - OS only manages file creation and manipulation
   - Prevention of overwrites:
     - Producer creates files with unique ID in directory
     - Consumer scans directory and consumes all files
3. Message passing
   - Sender passes message to OS to deliver to recipient
   - <u>Symmetric Addressing</u>: Sender and receiver needs to know each other to communicate
   - <u>Asymmetric Addressing:</u> Sender names recipient but receiver can get messages from anyone
     - `send(A, message)`
     - `receive(process_id, message)`
   - <u>Direct Communication</u>: Explicitly name sender and recipient
     - `send(A, message)`
     - `receive(B, message)`
   - <u>Indirect Communication</u>: Send message to mailboxes (no named sender/recipient)
     - `send(M, message)`
     - `receive(M, message)`
     - very scalable
     - anyone can send a message
     - if mailbox owned by a process, only that process can receive messages
     - if mailbox owned by OS, multiple processes can read mailbox messages
     - To prevent multiple reads, either:
       - OS only allows one process to read messages at a time
       - OS implements strategy of reading messages (e.g. message queues)

**Message Queue:**

- handles if sender is ready with 2nd message before receiver gets 1st message

Strategies:

1. Wait for previous message to be read
2. Overwrite last message
3. Discard current message
   - e.g. package drops

- Message queue are fixed size so if consumer is faster than producer, message queues only kick the problem down the road

**UNIX Pipes**:

- `pipe( int fileDescriptors[])`
  - `fd[0]` is read-end
  - `fd[1]` write-end
  - `close(fd[0])` if read-end is not being used and etc.
- read end, write end
- unidirectional
- stored in memory
- **require** parent-child process relationship (`fork` call)
- unix treats pipe as a file
- circular queue with each entry typically being 1 char
  - require termination character to indicate end of message

<u>Named Pipe</u> - UNIX pipe stored on disk

- `mkfifo()`
  - manipulate pipe with file calls instead (`open`, `read`, `write`, `close`)

- permanent storage
- does not need parent-child relationship
- bidirectional but only one direction at a time

UNIX Command Line: `cat fork.c | less `

- remember elixir!
- passes results forward

### Lecture 8 - Threads

- processes can have 1 or more threads
- short term for 'Thread of Execution'

![lect8-threads](.\Graphics\lect8-threads.PNG)

**Possessions**

1. Thread execution state
2. Saved thread context (for when it's not running)
3. Execution stack
4. Local Variables
5. Access to process memory and resources (shared between all threads)

![lect8-thread_items](.\Graphics\lect8-thread_items.PNG)

Example: UI Thread

- allows user to use GUI while actual work done on another thread

**Thread vs Process**: performance is better and easier to use

Pros:

- creation time is 10x faster
- termination and cleanup is faster
- switch time is 5x faster
- shared memory space within process

Cons:

- no security between threads
  - programmer error can cause one thread to disrupt other threads
- errors can crash entire process (segment error, divide by 0)

**Common Thread Usage**

1. Foreground vs background work
2. Async. processing (e.g. Word periodically saving work)
3. Speed of Execution (concurrent threads to speed up work)
4. Modularity

**Thread States**

- No 'Swapped' states because entire process would be swapped
- termination of process means all threads goes to "terminated" state

5-State Model:![lect8-thread_states](.\Graphics\lect8-thread_states.PNG)

**Thread Cancellation**

- terminated thread that has not finished work
- <u>Target:</u> thread that we are going to cancel

Methods of Cancellation:

1. Async. Cancellation - One thread terminates the target
2. Deferred Cancellation - Target informed of cancellation so it should enter "graceful" termination stage
   - good programming practice to check for cancellation
   - can technically be ignored
   - benefit is that it allows OS to free up all resources

**Thread Types**

1. User-level threads
2. Kernel-level threads

<u>3 Approaches</u>

1. All threads are user-level: <u>many-to-one</u>
   - can use a library to handle creation, management, and cleanup of threads
   - Pros:
     - Thread switches do not require kernel mode
     - Program decides on scheduling
     - Portability over different OS
   - Cons:
     - Since there is only 1 kernel-level thread, it needs <u>jacketing</u> to prevent one thread from blocking the whole process
   - **Jacketing:**
     - Thread library implements its own version of the system call for testing
     - Will only make system call if it verifies that the thread will not be blocked
2. All threads are kernel-level: <u>one-to-one</u>
   - Windows does this
     - however, if we don't implement user-level thread library, it will require kernel mode to switch threads
3. Combination: <u>many-to-many</u>

### Lecture 9 - POSIX Threads

- `pthread` is a POSIX Thread, used in UNIX systems

**Functions**

- `pthread_create(<params>)`: Create a new thread

  - `pthread_t *thread` - a pointer to a p_thread
  - `const pthread_attr_t *attr` - contain various characteristics of thread (e.g. priority)
    - can be NULL
  - `void* foo (void *)` - void* function that takes void* arguments
    - function that the thread will run
  - `void* args` - arguments past into function
    - can pass in array or struct for extensive parameters
    - parameters will require casting once we enter the function

- `pthread_join(thread_id, &return_vars)` - wait for specific thread to exit

  - will wait until `thread_id` is done and then pass pointer to `return_vars`
    - if no return value, then it is just NULL
    - cast `return_vars`: `return_type *return_value = (return_type *) return_vars;`

  - by default, new threads are joinable

- `pthread_exit` - terminate calling thread

  - allows for a return value
  - is a substitute for `return(<code>)`

- `pthread_cancel` - signal cancellation to thread (async or deferred) 

- `pthread_attr_init` - create and initialize a thread's attributes

- `pthread_yield` - release the CPU and let another thread run

**Thread Creation**

- no guarantee which thread will run after thread creation

**Thread Attributes**

Examples:

- Priority
- Detached or joinable state (see `pthread_join`)
- Scheduling data
- Stack size
- Stack address

**Thread Termination**

- use `pthread_exit` or `pthread_cancel`
- if main finishes, then all threads will suddenly terminate
  - however, if main calls `pthread_exit`, then it will wait for all threads to finish

**2 Implementations of Fork() for Threads**

1. Copy all threads of process
2. Copy only executing thread

**Signals**

- indicate events

2 Types:

- Synchronous - happens within program
- Async - happens outside process

- synch (within program) and async (outside program) signals for events
  - programs can handle their own signals if it tells OS
  - some signals cannot be handled by program
- `kill ( pid_t pid, int signal )` is command to send signals (even if it isn't kill)
  - UNIX
- `pthread_kill(...)` for POSIX threads
- use `pthread_testcancel()` for iterative tasks within thread



###Lecture 10

- asymmetric vs symmetric multiprocessor systems
  - Asymmetric: boss processor, workers
  - Symmetric: all equal workers
    - **See formal definition**
- Core - 1 logical processing unit
- Time Slicing - fixed times for threads
- Amadhl's Law (Equation) - formula for speedup percentage depending on serial and parallelizable parts of the application
- runtime reduced from O ( n lg n ) to O ( n ) ???



### Lecture 11

- Concurrency = nondeterminate state of order of processes
- mutex = order doesn't matter but only one thread can run at one time (e.g. file write access) 
  - critical section: code that should only be accessed by one thread max at a time

**Mutual Exclusion with Flags**

- 1st Approach (int turn flag) Problem - doesn't allow one thread to run more often
  - termination of thread during 'critical section' means other thread wait indefinitely
- 2nd Approach (1 bool flag) Problem - both threads can be running in critical section
- 3rd Approach (2 bool flag) Problem - both threads could be waiting indefinitely
- 4th Approach (disabling interrupts) - no response to user input
  - termination of process means interrupts disabled still
- Answer - `test_and_set (int* i)`
  - test *i and sets *i in 1 instruction cycle
- Avoid busy-waiting (avoid constantly checking the condition after each loop of `while`)



### Lecture 12

- **IMPORTANT** - look through additional desireable properties
- Use scheduling



**Busy-Waiting Alternatives**

- wait time period
  - inadequate for computers as the other thread could've immediately finished
- leave message in mailbox



**Binary Semaphore**

- 0, 1 value
- Functions: `wait`, `signal/post`
- `wait` is used to enter critical section
  - wait for our turn
  - Semaphore val == 1 means change value to 0 and enter critical section
  - Semaphore val == 0 means waits
- `signal` is used to indicate finished critical section
  - semaphore val == 1 means do nothing (nothing waiting)
  - semaphore val == 0  means there is a task blocked and waiting
- does not protect against bad behaviour
- another thread can signal semaphore when they aren't running critical section
  - might not enter its own critical section as there may be other tasks waiting
  - most likely to occur by programming error
  - use `mutex` data structure, that only allows thread that called `wait` to run `signal`





Inconsistent State: unexpected behaviour



- `wait` before changing shared data
- `signal` after changing shared data and before any return statement (especially error checks)
- **do not use malloc in critical section**
- **C Convention**  -- manual scoping 
  - adding {}
  - does nothing programmatically but easy to see what's happening

**General Semaphore**

- sets init value to INT_MAX value
  - init value means maximum number of tasks that can run at once
- a thread that waits will decrement int by 1
- a thread that signals will increment int by 1
- general semaphore not necessarily for mutex but to prevent overloading of a server and etc.
- mutex semaphore initialized to 1
- if semaphore < 0, then abs(semaphore) == number of tasks waiting



### Lecture 13

- use semaphore to coordinate threads and processes
- Rendezvous: two threads should be at same point before further proceeding
  - e.g. both threads run statement 1 before statement 2
- Deadlock: When all threads are permanently stuck
- EXAM: point an order of statements that synchronization doesn't work to show wrong
  - consider multiple order of statements to prove that synch works

Symmetric threads behave much more reliably and thus, scale well



Mutex Pseudocode

``` pseudo
wait(mutex)
//critical section
signal(mutex)
```

**Note:** 1 signal will only unblock 1 wait

- for 3+ threads, u may need to send signals multiple times



Barrier: generalization of rendezvous pattern where we keep a variable to determine the number of threads that have reached the appropriate point



Barrier Solution 2:

- bad because of a lot of process switches
- last thread is most likely lowest priority thread but it will be the next to run

Barrier Solution 3:

- good because one thread unblocks the next (can be any next thread)
- possible error as reads shared data (`count`) that's not in wait/signal mutex

Turnstile Pattern:

- one thread at a time to proceed through

Reusable Barrier

- Barrier is not reusable as there is 1 more signal than wait

- Clean up turnstile by removing last signal
- **for exam**: to show something is reusable, show initial state == end state

Reusable Barrier Solution 2

- One problem -- one thread may be super fast and finish line 15 before any other thread started line 1
- Solution 3: Multiple turnstile signals
- 

Avoid nested waits

### Lecture 14 - Classic Synchronization Problemss

