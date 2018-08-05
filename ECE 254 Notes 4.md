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

1. Turnaround Time
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
   - Turnaround time
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

The OS maintains process data for scheduling criteria:

- The time spent waiting to run
- The time spent executing process
- The <u>expected</u> total time of execution 
  - Low expected total time means process might be halted midway
  - High expected total time might mean process never runs



**Algorithms** - All algorithms in this lecture is for batch processing systems (not interactive desktop systems)

1. <u>Highest Priority</u>
   - Implementation:
     - Stored as 1 big linked list and have pointers for each priority
       - Hence, making multiple priority queues
     - Alternatively, priority heap
     - If a process becomes blocked or changes priority, move it from current location
   - Pros
     - Good for life-and-safety critical systems
   - Cons
     - No Fairness (aka starvation)
     - Fails "Response Time" criteria
2. <u>First-Come, First-Served (FIFO)</u>
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
   - Shortest processes (measured by expected execution time) gets highest priority
   - Downside: No one knows exactly how long a process will take
     - Better to know length of CPU bursts (see below)
5. <u>Shortest Job First</u>
   - aka "Shortest CPU burst"
   - FIFO can tie-break
   - Equation: (next CPU burst is weighted average of all previous burst times)
     - `S1` is a guess
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



**Multiprocess Scheduling**

Three types of multiprocessor systems:

1. <u>Distributed</u> - Collection of autonomous systems that interact
2. <u>Functionally Specialized</u> - System has specialized processors
3. <u>Tightly Coupled</u> - Set of processors with common main memory and under control of OS



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
  - Only if threads are closely intertwined (fine-grained multithreading vs coarse-grained multithreading)

![lect27-hyperthreading](.\Graphics\lect27-hyperthreading.PNG)



**Summary: Two levels of scheduling**

1. Assigning process/thread to processor (done by OS)
2. Deciding when to swap between 2 threads (for memory stalls, done by hardware)



### Lecture 28 - 