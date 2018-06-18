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
  3. Processor
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

- generated by errors or user program request for OS function
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
    - cannot be handled by process
  - `SIGTERM` (val = 15): termination signal (termination can be gracefully handled by PID)
- `memset(&var, 0, SIZE)` = sets 'var' pointer to 0 x SIZE
  - need initialization or there will be bad values

**'Screen' Command**

- tabs for terminal
- `screen vi file.txt`

**Spawning Child Processes**

- child can request resources from OS or parent
- parent spawns child with `fork()`
  - child is a copy of parent, including state (eg. what line of code the parent is on)
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

- `pthread_join(thread_id, &&return_vars)` - wait for specific thread to exit

  - will wait until `thread_id` is done and then pass pointer to `&return_vars`
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
- `kill ( pid_t pid, int signal )` is UNIX command to send signals (even if it isn't kill)
- `pthread_kill(...)` for POSIX threads
- kernel will use default handler unless process has informed OS that it can handle the signal

<u>2 Types:</u>

- Synchronous - happens within process (e.g. seg fault)
- Async - happens outside process

<u>Where the Signal is Sent</u>

1. To the thread the signal applies
2. To all threads in process
3. To certain threads in process
4. To assigned thread that handles all signals to the process

**Cancellation**

- `pthread_testcancel()` will check if the thread has been given cancellation signal
  - use at start/end of every loop

###Lecture 10 - Symmetric Multiprocessing

- modern computers have multiple cores

**Multiprocessing Types**

1. Asymmetric Multiprocessing
   - Boss processor - in charge of contrlling system
   - Worker processor - performs tasks given by boss
2. Symmetric Multiprocessing
   - all equal workers

**Symmetric Multiprocessing**

1. 2+ general purpose processors
2. Processors share main memory, I/O devices, and bus
3. Processors are able to perform the same functions
4. System is controlled by OS

![lect10-cpus](.\Graphics\lect10-cpus.PNG)



Note: Core = 1 logical processing unit and CPU refers to a physical chip with 1+ cores

- OS cares about the # of cores

**Time Slicing** - Fixed times for processes and threads

- ensures that all processes/threads get processing time

**Parallelism**

- more cores means faster time for parallelizable parts of the task
- some parts of overhead is not parallelizable 

<u>Amadhl's Law - Calculate Speedup percentage</u>

- Let S be percentage of the task that must be performed serially (in order)

- Let N be number of cores

- Thus, 1-S is percentage of task that can be performed in parallel

- Equation:

  ![lect10-speedup_equation](.\Graphics\lect10-speedup_equation.PNG)

- As N -> infinity, clearly the speedup is 1/S

- Diminishing return as we add more processors

**See online coding example of Merge Sort Example**

- if length == 1 return
  - in the online version, it uses insertion sort for length < 30
- use `pthread_create` within merge sort function to divide and conquer
- wait for both threads to finish using `pthread_join`
- finally, call `merge` at end
- runtime reduced from O ( n lg n ) to O ( n )
- good for all divide and conquer algos



### Lecture 11 - Synchronization and Atomicity

**Synchronization in Computers**

- we want some relationship between events (before, during, after)
- <u>Serialization</u> - ordering in events
  - for example, having 1 thread use `waits`
- <u>Mutual Exclusion</u> - events must not happen at the same time
  - for example, reads/writes to shared memory

**Serialization through Messages**

- <u>Sequential</u> - when we are certain that event A occurs before event B
- <u>Concurrent</u> - when order of event A and event B is undeterminable
- <u>Nondeterminism</u> - when the order of events can change on two runs of the program
-  <u>Heisenbug</u> - when incorrect bug happens very infrequently
  - very difficult to determine exact reason
  - concurrency is a common source of heisenbugs
- We can serialize using messages (ie. do X if I get message Y)



**Shared Data and Atomic Operations**

- No co-ordination mechanism does not mean all outcomes are possible

- **Interrupts can cause concurrency problems** (even if it is just 1 thread)

  - Simple C commands can have multiple instructions. For example,

    ``` C
    count++;
    // 1. Read current value of count
    // 2. Add 1 to value
    // 3. Write new value back to memory
    ```

- <u>Atomic Operation</u> - operations that cannot be interrupted



**Mutual Exclusion (mutex) using flags**

- when both orders of event A and event B is possible, we cannot prevent concurrency problems with serialization
- <u>Mutual Exclusion</u> - two threads should not run at the same time
- <u>Critical Section</u> - the code section that should only be accessed by 1 thread at a time
  - any code that modifies shared data should be defined as a "critical section"
  - increases magnitude of S (% of tasks that can only be serially run) in Amdahl's law)



**Mutex with Flags Approaches**

- Problem with 1st Approach - waiting on a integer flag that is set by the other thread

  - thread A cannot run more often than thread B
  - early termination of thread B means it does not set the flag -> thread A waits indefinitely

- Problem with 2nd Approach - waiting on a boolean `busy` flag

  - reading and writing to `busy` flag are 2 separate operations so both threads can be running in critical section

- Problem with 3rd Approach - each thread has it's own boolean variable and the other thread reads all boolean variables but their own

  - requires two writes (one for each thread to indicate it is busy). hence, both threads can be 'busy' and neither thread proceeds (deadlock)

- Problem with 4th Approach - disabling interrupts

  - no response to user input
  - early termination of process means interrupts are not re-enabled
    - this is a problem because interrupts are not process specific (everyone uses interrupts)

- Best Approach - use special machine instruction `test_and_set (int* i)`

  - `test_and_set` is an atomic read and write

    ``` C
    //Description of functionality in C (not actual implementation)
    boolean test_and_set (int* i) {
        if (*i == 0) {
            *i = 1;
            return true;
        } else {
            return false;s
        }
    }
    ```

  - Example code:

    ``` C
    while ( !test_and_set (busy) ) {
        // wait
    }
    // critical section
    busy = 0; // might crash just before this line but who cares as process died
    ```

- Avoid <u>busy-waiting</u>  - constantly checking the condition



### Lecture 12 - Semaphores

**Updated Criteria for Mutual Exclusion**

1. Mutual exclusion must apply (threads cannot be running the critical section at the same time)
2. A thread that halts outside critical section cannot interfere with other threads
   - `test_and_set` fails this criterion
3. A thread requiring access to critical section cannot be delayed indefinitely
4. When no other thread is in the critical section, a thread that requests it should enter immediately - **no unnecessary waiting**
5. No assumption is made about the thread activity and # of processors **(general solution)**
6. A thread should only be in critical section for a finite time



**Semaphore Motivation**

- Want to avoid busy-waiting
  - however, using a sleep instruction is not ideal (e.g. thread slept longer than necessary)

- Idea: `A` leaves message in mailbox of `B`. `B` will send message to `A` when B exits critical section 
  - rather than thread-specific mailboxes, use a number that indicates state



**Binary Semaphore**

- Value: 0 (busy) or 1 (ready)
- Functions: `wait`, `signal (aka post)`
- `wait` is used to enter critical section
  - wait for our turn 
  - If semaphore == 1 (ready), change value to 0 and enter critical section
    - Called <u>decrementing the semaphore</u>
  - If semaphore == 0 (busy), thread waits
- `signal` is used to indicate that a thread has finished critical section
  - If semaphore == 1 (ready), do nothing
  - If semaphore == 0 (busy), set semaphore to 1 to unblock waiting threads
    - Called <u>incrementing the semaphore</u>
- A thread does not know in advance if it will wait
- A thread running critical section does not know if another thread is waiting

**Bad Behaviour**:

- Thread A can `signal` semaphore even if thread B is running critical section
  - However, no guaranteed thread A will be unblocked. Thread C might run next
  - Likely to occur by programming error
- Solution: Use a special data structure - **mutex**
  - A binary semaphore that can only `signal` by the thread that has called `wait`



**Linked List Example** - see slides

- `push_front` - could result in erroneous `size` or `head*` or `tail*`
- <u>Inconsistent State</u>: unexpected behaviour



**Binary Semaphore Functions**

- `semaphore_init ( semaphore_t *sem, int i)`
  - initialize semaphore to i
- `semaphore_wait (semaphore_t *sem ) `
  - wait on semaphore
- `semaphore_signal ( semaphore_t *sem )`
  - If semaphore == 1, do nothing
  - If semaphore == 0, set to 1 to unblock other tasks



**General Implementation**

- `wait` before changing shared data
- `signal` after changing shared data and before any return statement (especially error checks)
- **C Convention**  -- manual scoping 
  - adding {}
  - does nothing programmatically but easy to see what's happening
- **do not put malloc in critical section** as `malloc` is a blocking function
  - could cause system to get stuck



**Counting/General Semaphores**

- Sets initial value to the maximum number of tasks that can run at once
  - A thread that passed `wait` command (i.e. thread is now running critical section) will decrement semaphore by 1
  - A thread that `signal` will increment semaphore by 1
  - If the semaphore is -1, then the thread is blocked
- `semaphore_init ( semaphore_t *sem, int initial, int max)`
- General semaphores not necessarily for critical sections but to prevent overloading of a server and etc.
  - Mutex semaphore can be a general semaphore by initializing max to 1
