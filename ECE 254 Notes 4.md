# ECE 254 Notes Part 4

### Lecture 25 - Uniprocessor Scheduling 

<u>Scheduling:</u> Given multiple processes, determine which process to execute and when



**Four Types of Scheduling**

1. **Long Term Scheduling**
   - Which programs should be "up-to-the-bat"
     - Controls "New" -> "Ready-To-Run" state
   - For computers, users typically control what programs to un
   - Mobile OSes are more aggressive and will suspend/kill processes to make room for new ones
2. **Medium Term Scheduling**
   - Swapping processes to and from disk
3. **Short Term Scheduling** (aka <u>dispatcher</u>)
   - What process to run right now
   - If we have "naive" cooperative multitasking, the dispatcher will run when:
     - running process yields to the CPU
     - running process terminates
   - With <u>pre-emptive multitasking</u>, the dispatcher will run if
     - one of the previous 2 scenarios
     - running process become blocked (I/O operation, mutex/semaphore, page fault)
     - after handling an interrupt
     - potentially after `fork()` or signaling a semaphore
     - using **time slicing** -- run dispatcher after a certain amount of time
4. **I/O Scheduling** (discussed later)



**Process Behaviour - Rate Limiting Step**

Processes alternate between computing and I/O requests

- <u>CPU Burst</u>: time where the CPU does a lot of work
- <u>I/O Burst:</u> time where the process waits for I/O requests
- <u>CPU-Bound</u>: processes that spend most of their time computing
  - Rate limiting step is speed of CPU
- <u>I/O-Bound:</u> processes that spend most of their time waiting for I/O
  - ie. I/O Bound programs tend to have smaller CPU bursts



Since disk is slow, we should:

1. Keep balance between CPU and I/O Bound tasks
2. Ensure that disk is always running by scheduling a process with disk request



**Scheduling Criteria**

1. Turnaround Time (total time)
   - Time to start a process to when it finishes
   - Execution time + Resource waiting
2. Response Time
   - Time for process to start producing any output
3. Deadlines
   - A period of time which a process must/should be completed
     - e.g. video stream
   - Bigger concern for real-time systems
4. Predictability
   - Runtime of the same process should be consistent
5. Throughput
   - Maximize # of processes completed in a given amount of time
6. Processor Utilization
   - Maximize CPU usage
7. Fairness
   - Processes should not be starved
8. Priorities
   - Respect priorities of processes (e.g. higher priority processes should run the most often)
9. Balancing Resources
   - CPU, disk, I/O resource usage should be optimized
     - Also, if a resource is busy, then we shouldn't run a process that require the resource



**Most Important Criteria for Different Systems**

1. All Systems
   - Fairness
   - Priorities
   - Balancing Resources
2. Batch Systems
   - Throughput
   - Turnaround time (total time)
   - CPU utilization
3. Interactive (User) Systems
   - Response time
   - Predictability



**Use of Priorities**

- In UNIX, lower number == higher priority
- In Windows, higher number == higher priority
- Priorities is very good for making decisions on resources
- It is not good as a sole measure of what process run as it could cause starvation



### Lecture 26 - Scheduling Algorithms

Accounting Data of Processes (used in scheduling):

1. The time spent waiting to run
2. The time spent executing process
3. The <u>expected</u> total time of execution 
  - Lower than actual execution time means process might be halted midway
  - Higher than expected execution time mean process never runs



**Algorithms** - All algorithms in this lecture (#26) is for batch processing systems (not interactive desktop systems)

1. <u>Highest Priority</u> (self-explanatory)
   - Implementation:
     - Stored as 1 big linked list and have pointers for each priority level
       - Hence, making multiple priority queues
     - Alternatively, priority heap
     - If a process becomes blocked or changes priority, move it from current location
   - Pros
     - Good for life-and-safety critical systems
   - Cons
     - No Fairness (aka starvation)
     - Fails "Response Time" criteria
2. <u>First-Come, First-Served (FCFS)</u>
   - Essentially FIFO
   - Implementation
     - Simple queue
       - If current process is blocked, it gets moved to back
   - Cons
     - Bad expected response time as it depends on order of processes
     - Favours CPU-Bound processes, effectively wasting resources
     - Not preemptive
3. <u>Round Robin</u>
   - FCFS/FIFO with preemption using time slicing
     - `l`, time between timer interrupt that runs short-term scheduler, can't be too long or else short processes will wait forever
     - `l` cannot be too short or system will spend more time handling clock interrupt
     - <u>Solution</u>:  If the avg process time is `r`, then `l > r`, which allows processes to just finish before running scheduler
   - Downside -- Fairness
     - Since I/O - Bound processes will go to back of queue every time it requires disk swap, this algo favours CPU-Bound
     - <u>Solution: Virtual Round Robin</u> 
       - Running processes that become blocked goes into I/O queue
       - When processes get unblocked, they join auxiliary queue, which has higher priority than normal queue
       - After running up to time limit `l`, it goes back to normal queue
       - ![lect26-virtual_round_robin](.\Graphics\lect26-virtual_round_robin.PNG)
4. <u>Shortest Process Next</u>
   - Shortest expected execution time gets highest priority
   - Downside: No one knows exactly how long a process will take
     - Better to know length of CPU bursts (see below)
5. <u>Shortest Job First</u>
   - aka "Shortest CPU burst"
   - FIFO can tie-break
   - Equation: (next CPU burst is weighted average of all previous burst times)
     - `S1` = 0 (first guess is 0, allowing new processes to at least start)
     - Equation: ![lect26-shortest_job_first_eq1](.\Graphics\lect26-shortest_job_first_eq1.PNG)
     - Modified equation using previous estimate: ![lect26-shortest_job_first_eq2](.\Graphics\lect26-shortest_job_first_eq2.PNG)
     - Modified eq. weighing most recent bursts more heavily:![lect26-shortest_job_first_besteq](.\Graphics\lect26-shortest_job_first_besteq.PNG)
       - Greater alpha means more weighting, and usually better estimates
   - Pros
     - Best minimum average waiting time
   - Cons
     - Longer processes could still starve
6. <u>Shortest Remaining Time</u>
   - If newly unblocked or scheduled process has shorter expected running time, the  running process will be replaced
     - If new processes are given the 'prediction' `S1 = 0`, then new or unblocked processes will always  replace and run first
   - Time slicing is no longer needed
     - Can simply wait for CPU burst to complete as they are expected to be small (so shorter processes do not wait too long)



### Lecture 27 - Scheduling: Idling, Priorities, Multiprocessor

**More Algorithms**

7. <u>Highest Response Ratio Next</u>
   -  <u>Normalized Turnaround Time:</u> Turnaround time / Service time
     - aka. (waiting time + execution time) / execution time
       - execution time is estimation
     - Longer processes may have longer waiting time
   - Goal: Minimize average normalized turnaround time
     - New processes start with ratio of 1 (wait time is 0)
     - Process with highest ratio is selected next
   - Pros
     - no process will starve as the longer a process waits, the higher its ratio
   - Cons
     - Has to estimate service time
8. <u>Multilevel Queue (Feedback)</u>
   - Ready queue broken into multiple queues, each process being in only 1 queue
     - Division of queue based on some attribute such as priority, memory needs, foreground/background
   - Example from IBM:
     - Queues divided based on time slices
     - If a process took longer than 1 time slice, they get moved from highest priority queue to 2nd queue, which gave them 2 time slices
     - Processes in 3rd queue were given 4 time slices, and so on
   - Process priority is assigned based on amount of CPU time used already
     - More CPU time means lower priority
9. <u>Guaranteed Scheduling</u>
   - For `m` processes, each process is given `1/m` of CPU time
10. <u>Lottery</u> - Not exactly a scheduling algorithm
    - A ticket is drawn and the process with that number will run next
    - Higher priority processes gets more tickets
    - Processes that depend on each other can pool tickets
    - Pros:
      - A lot less overhead than scheduling
      - Simple implementation



**Idle Task**

- When there are no process to run next
- Could repeatedly invoke scheduler or switch CPU to low power state



**Bumping the Priority**

<u>Priority Inversion</u>: When a high priority process is waiting for low priority process

- Solution: <u>Priority Inheritance</u> - low priority process inherits higher priority of waiting process



**Multiprocessor Scheduling**

Three types of multiprocessor systems:

1. <u>Distributed</u> - Collection of autonomous systems that interact
2. <u>Functionally Specialized</u> - System has specialized processors
3. <u>Tightly Coupled</u> - Set of processors with common main memory and under control of OS
   - We will work with this



Higher interactions between processes == "finer-grained" == more careful

- For symmetric multiprocessing, each processor is responsible for scheduling itself so mutex and other synchronization techniques are needed to prevent multiple processes from grabbing same process to run



**Processor Affinity** - Sticking with 1 processor as each processor has own cache

- <u>Soft Affinity</u>: CPU tries to stick with 1 processor
- <u>Hard Affinity:</u> CPU forces process to only run on specific processor
- Can mix and match soft and hard affinity
- Locking a process to a processor can also prevent <u>non-uniform memory access</u>, where some parts of memory take longer to access for some processes
  - Also encourages process instructions to all be in 1 place in main memory



**Load Balancing** - Keeping the workload balanced between processes

- Only needed if each processor has it's own process queue
  - If shared queue, each processor just grabs next task if they are idle
- <u>Push Migration</u> - Processor periodically checks how busy other processors are and balances tasks
- <u>Pull Migraton</u> - Idle processor takes a process from a queue of busy processor
- Since cache misses means added time to a process, we add a "penalty" to moving a process to determine if its worthwhile



**Multicore Processors** - Single chip made of multiple processors (called cores)

- <u>Memory stall</u> - time spent waiting for memory
- Solution: <u>Hyperthreading</u> - assign two threads to a core and switch threads if one had memory stall
  - Only if threads are closely intertwined (fine-grained multithreading vs coarse-grained multithreading) as switching coarse-grained threads is very expensive

![lect27-hyperthreading](.\Graphics\lect27-hyperthreading.PNG)



**Summary: Two levels of scheduling**

1. OS assigns process/thread to a processor
2. Hardware decides how/when to swap between 2 threads (e.g. memory stalls)



### Lecture 28 - Real Time Scheduling (Windows, UNIX) 

 **Real Time Scheduling**

- deadlines, predictability more important than fast

<u>Hard Real-time</u>: A task where deadline must be met

- Otherwise, system will fatal error
- Two Scenarios:
  - Scheduled too late
    - As a result, scheduler will reject the task (why waste time that will not finish on time?)
  - Other higher priority tasks used up too much time
    - As soon as not enough time is left before deadline, the task will be rejected

<u>Soft Real-Time</u>: A task where missing the deadline degrades its usefulness



**Timeline Scheduling**

<u>Periodic</u>: A process that occurs at regular intervals (e.g. check temperature sensor)



Say a periodic task has:

- `c` = total completion time per run
- `tau` = period (time before the task will run again)

If `U > 1` in the equation below, then the total completion time of all tasks is greater than the time allotted for all the task, meaning the system is overloaded

![lect28-periodic_task_eq](.\Graphics\lect28-periodic_task_eq.PNG)



<u>Aperiodic Tasks</u>: Tasks that don't occur at regular intervals

- generally soft real-time tasks as system cannot guarantee enough time leftover to run these tasks
- <u>Sporadic:</u> Tasks that are aperiodic but have deadlines
  - we require `tau`, a minimum time between occurrences

**Two Optimal Algorithms**

- "Optimal" because it ensures all tasks will meet deadlines if system not overloaded

1. <u>Earliest Deadline First</u>
   - Self-explanatory
   - Random selection (or another criteria) will tie-break tasks with same deadline
   - If a new task has the earliest deadline, then current task will be suspended and new task will run
2. <u>Least Slack First</u>
   - <u>Slack:</u> how long a task can wait before it must be scheduled
     - A 10ms task with a deadline 50ms from now has `50 - 10 = 40 ms slack time`



**Traditional UNIX OS Scheduling**

- Multilevel Feedback System (multiple ready queues) with Round Robin on each queue
- Time slicing of 1s
- Priority based on process type and execution history
- Originally a time-sharing system with multiple users and many processes



<u>Priority Formula</u>

![lect28-UNIX_priority_formula](.\Graphics\lect28-UNIX_priority_formula.PNG)

- Priority of process `j` at time `i`
- B is base priority of process
- N is "nice" value of process
  - Users can increase the value to reduce priority of the process
- CPU is processor utilization
  - Clearly, the longer a process has ran, the lower the CPU (means higher priority)
  - ![lect28-UNIX_CPU_util_formula](.\Graphics\lect28-UNIX_CPU_util_formula.PNG)
- CPU and N are limited values. They have to stay within their priority class



<u>Priority Classes</u> (from highest to lowest)

1. Swapper (moves processes to/from disk)
2. Block I/O device control (e.g. disk)
   - data is passed in blocks
3. File Manipulation
4. Character I/O device control (e.g. keyboard)
   - data is passed a character at a time
5. User processes



<u>Pros:</u>

- Ensures that the I/Os (e.g. disk) is always running
  - Good for I/O bound processes
  - Bad for CPU-bound processes that will have to halt 



**Windows Scheduling Algorithm** (called <u>dispatcher</u>)

- Highest priority thread runs
  - 32 Priority levels
    - Priority 1-15 are regular classes
    - Priority 16-31 are real-time classes
    - Priority 0 is for a specific "Memory Management Task"
- Originally designed as a desktop operating system with few processes



<u>Six Priority Classes</u> (can be set in Task Manager)

1. Realtime
2. High
3. Above Normal
4. Normal
5. Below Normal
6. Low

- Processes are typically in "Normal"



<u>Relative Priority</u>

- ![lect28-windows_relative_priorities](.\Graphics\lect28-windows_relative_priorities.PNG)
- For processes that aren't real-time:
  - The relative priority is lowered:
    - if a process takes longer than its time slice
  - The relative priority is boosted:
    - if a process was recently unblocked
      - A process that was waiting for keyboard will get a higher boost than a process waiting for disk operation



<u>Selected Foreground Process</u>

- A foreground process that is "on top" in the UI (i.e. currently selected UI window) will have a higher priority AND longer time slices



### Lecture 29 - Scheduling in Linux

2 Schedulers:

1. Real-time
2. Non Real-Time (ie. normal)



**Real-Time Scheduler**

<u>3 Scheduling Classes</u>

1. `SCHED_FIFO` - FIFO Real-Time Threads
2. `SCHED_RR` - Round Robin Real Time Threads
3. `SCHED_OTHER` - Non-Real-Time Threads
   - Non-real-time threads will be scheduled using the normal routine

<u>Priorities Numbers</u>

- Real-Time: 0 - 99
- Non-Real-Time: 100-139



<u>Rules</u>

1. System will interrupt current thread if
   - A FIFO thread with higher priority becomes READY
   - Current FIFO thread is blocked
   - Current FIFO thread yields (using `sched_yield`)
2. If a FIFO thread is interrupted, it gets placed into queue for its priority
3. To tie-break, OS picks thread that has waited the longest
4. For `SCHED_RR`, time-slicing is implemented
   - The current thread does not lose priority however (so it might continue running)
5. `SCHED_OTHER` threads will only run if `SCHED_FIFO` and `SCHED_RR` are empty



**Non-Real-Time O(1) Scheduler**

<u>Problems of Tradition UNIX Scheduler</u>

- O(n) algorithm

- Can't handle symmetric multiprocessor (SMP) systems due to:
  - Single run queue
    - Results in lack of processor affinity
  - Single run queue lock
    - Modifying queue means all processors have to wait
  - Inability to preempt running process
    - Remember, time-slice != preemption

  

<u>Structure</u>

``` C
struct prio_array {
    int nr_active; /* number of tasks in the array */
    unsigned long bitmap[BITMAP_SIZE]; /* priority bitmap */
    struct list_head queue[MAX_PRIO]; /* priority queue */
}
```

- One queue for each priority level

  - `MAX_PRIO` (which is the number of queues) is 140

- Priority bitmap tells which queue is empty

  - `BITMAP_SIZE` is 5 for the 140-leveled, 32-bit word system

  

2 Structures: 1 for Active Queue Structure, 1 for Expired Queue Structure

- Expired queue is for processes that took longer than a time-slice
- The Expired Queue will only start only if the entire Active Queue Structure is empty



<u>Downside</u>

- Due to Active vs Expired queue, there is possible starvation
- Especially bad for user-interactive processes



**Non-Real-Time Scheduler: Completely Fair Scheduler**

- O(ln n) operations
- Red-black tree ordered by execution time
- <u>Target Latency:</u> The interval of time every ready task should run at least once
  - Used instead of constant time-slice
  - OS determines actual runtime based on target latency
  - Target latency increases if there are more tasks in tree
    - Otherwise, the task will have to wait a long time before the CPU gets back to it
- <u>Virtual Runtime</u> - `vruntime`
  - Most recent execution burst time is weighed higher
    - Since higher priority processes are expected to run more often, the decay is higher (e.g. most recent burst time is weighed significantly more)
  - Depends on nice value (ie. priority)
    - Nice Value of 0 means `vruntime` = actual runtime
    - Nice Value of 1-19: `vruntime` > actual runtime
    - Nice Value of (-20) - (-1): `vruntime` < actual runtime

-  Priority Assignment
  - Higher Priority: processes that were waiting (e.g. for user input)
  - Lower Priority: processes that recently used CPU
- Group Scheduling
  - Processes can be grouped together
  - Good for processes with many threads for multiple related processes



**A Decade of Wasted Cores**

<u>Background</u>

- Simple load balancing (move threads from busiest core to leasy busy core) does not consider cache locality or non-uniform memory access

- Thus, there exists a more complex system

  - In the picture below, there are 4 <u>scheduling domains</u>

    1. Two cores that have SMT-sharing
    2. Cores within a group
       - Cores are grouped together if they have common hardware (e.g. L1 cache)
    3. Groups that are 1-hop away
    4. All groups

  - There are 32 cores and 4 groups

  - ![lect28-scheduling_domain](.\Graphics\lect28-scheduling_domain.PNG)

  - In each scheduling domain, there is a designated core to load balance (either the lowest numbered idle core or if all cores are busy, the lowest-numbered core)

    - Designated core runs normal processes as well

    - Only busy cores can wake up idle cores and tell it to start load-balancing
    - Within the scheduling domain, the designated core will compare the average load to the average load of the busiest scheduling domains (picked by heuristics)
      - Balance is achieved when average load of local domain > average load of busiest domain



<u>4 Bugs That Resulted in Idle Cores</u>

1. **Group Imbalance Bug**
   - Cores only steal a process from another group based on average load of group
   - As a result, idle cores will fail to steal from very busy cores of another group
   - Solution: Compare minimum load of group (rather than average load)
2. **Scheduling Group Construction**
   - Groups are constructed from perspective of core 0 but aren't compared as such
   - Core 31 fails to steal from neighbouring core since it seems like its 2 hops away
3. **Overload on Wakeup**
   - unblocked threads will rejoin same core (even if other cores aren't busy)
4. **Missing Scheduling Domains**
   - Coding error
   - If a core was removed and re-added, all threads of an application designated to that core will only run on that 1 core instead of multiple cores



