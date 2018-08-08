# ECE 254 Notes Part 2

### Lecture 13 - Synchronization Patterns

- more synchronization patterns to coordinate threads and processes than serialization and mutex

**Signalling** - letting another thread know of an event

- can ensure that an instruction from thread A will run before an instruction from thread B
- in this case, a thread should be able to `signal` without having to initially `wait` for the semaphore



**Rendezvous**

- Ensure two threads are at the same point before continuing
  - e.g. both threads run statement 1 before statement 2

<u>Solution Attempts</u>

- Attempt 1 - both threads have `wait` of other thread, then `signal`
  - neither thread will proceed
  - <u>deadlock</u>: when all threads are permanently stuck
- Attempt 2 - both threads `signal` to other thread, then `wait`
  - **BEST SOLUTION**
- Attempt 3 - Thread A has `signal` then `wait`; Thread B has `wait` then `signal`
  - inefficient as extra switch between threads is possible

<u>Example Exam Questions</u>

- Declare an order of statements that is not possible 
- Show that the synchronization implementation is incorrect by showing a counter-example: 1 order of statements that will result in unexpected behaviour
- Show that the synch. impl. is correct by showing that all possible order of statements result in expected behaviour

**Mutex**

- see Lecture 12

- <u>Symmetric Solution</u>: different threads have same sections of code

  - Symmetric threads behave much more reliably and thus, scale well

    ``` pseudo
    Thread K
    1. wait ( mutex )
    2. // critical section
    3. signal ( mutex )
    ```

- <u>Asymmetric Solution</u>: threads have different sections of code



**Multiplex** - also known as general semaphore

- Maximum number of queries to database
  - Each thread should be a symmetric solution



**Barrier** - Generalization of rendezvous

- All threads have to reach a certain point before proceeding

<u>Solution Attempts</u>

- Attempt 1 - use a variable (`count`)to store number of completed threads
  - `mutex` sempahore to protect shared data`count` 
  - `barrier` semaphore to make threads `wait`
  - this solution has only 1 signal so only 1 thread can continue past `barrier`
- Attempt 2 - same as attempt 1 but signal *n-1* times
  - Inefficient solution
  - last thread is most likely lowest priority thread so every time it runs `signal` , the threads will switch to a higher priority thread
    - hence, this could result in *2n* process switches
- Attempt 3 - thread that has been unblocked will `signal`
  - good because one thread unblocks the next
  - <u>Turnstile Pattern</u>: allowing one thread to proceed through at a time
  - possible error as we read `count` while another thread could be writing to it
    - However, **putting a semaphore within a critical section** is common source of deadlocking, especially if both `wait(barrier)` and `signal(barrier)` are inside
    - **Avoid nesting semaphores** and nested `waits`

**Reusable Barrier**

- <u>Reusable</u>: initial state == end state
- Barrier is not reusable as there is 1 more `signal` than `wait`
- Add if statement to clean up turnstile by removing last signal
- 2 turnstile signals and 1 shared mutex signal (since 2 turnstile signals we can never reach both a)
  - we use 2 turnstile signals to prevent "lapping" - where a fast thread starts its 2nd iteration before all the other threads finish their 1st iteration
- Solution is called <u>two-phase barrier</u>



### Lecture 14 - Classic Synchronization Problems

- Classic problems can be used to test new synchronization or coordination scheme

**Producer - Consumer Problem**

<u>Background</u>

- Producer generates data and puts it into buffer
  - cannot write data if buffer is full
- Consumer reads data from the buffer
  - cannot read data if buffer is empty
- Shared data: `count` == amount of data in buffer
- Buffer size: `BUFFER_SIZE`

<u>Solution Attempts</u>

- Attempt 1 - one mutex semaphore to read/write to `count`
  - inefficient as busy-waiting (e.g. consumer can't do anything if producer is writing)
- Attempt 2 - two semaphores to indicate # of chars and # of spaces in buffer
  - efficient as producer can write data until buffer is full and consumer can read until buffer is empty
  - However, 2 assumptions
    1. Adding item to buffer add to next consecutive space (and removing item opens up the next space)
    2. Only 1 producer and 1 consumer
- General Producer-Consumer Solution
  - Addresses the above assumptions
  - Have a shared `mutex` within Producer and Consumer code

SEE SLIDES



**Readers - Writers Problem**

<u>Background</u>

1. Any number of readers are allowed in critical section
2. Only 1 writer is allowed in critical section
   - no readers are allowed either

<u>Solution Attempts</u>

- Attempt 1 - one variable to track number of readers, 1 `mutex` binary semaphore to protect that variable, and 1 `roomEmpty` semaphore to protect from writers
  - <u>Light Switch Pattern</u> - only first person and last person needs to flick light switch
    - only first reader needs to wait `roomEmpty` (i.e. check if writer is writing)
    - last reader needs to signal `roomEmpty`
  - Problem -- <u>Starvation</u> - a thread that never gets a chance to run
- Attempt 2 - stop new readers when writer arrived using common semaphore `turnstile`
  - the writer has the power to block `turnstile` and force both writers and readers to queue
  - on the reader's side, the reader waits and immediately signals `turnstile` so it doesn't really use `turnstile`
  - this solution does not give priorities
    - Example priority: after writer signals `turnstile`, another writer gets to write before any of the readers

**Dining Philosophers Problem**

<u>Background</u>

- Imagine 5 philosophers sitting around a table, wanting to eat rice in the middle
- There is 1 chopstick to their right and 1 chopstick to their left. They need both to eat
- Let each philosopher grab the left chopstick, then the right chopstick, and then eat
- **Deadlock Problem:** all 5 philosophers grab the left chopstick at the same time

<u>Idea 1</u>

- Have only 4 seats (pigeonhole principle means at least 1 philosopher will eat)

<u>Idea 2</u>

- Problem arises because all the philosophers try to grab left chopstick
- If some grab left and some grab right, then no deadlocking



### Lecture 15 - Deadlock Problem

**Deadlock** - *Permanent* blocking of a set of processes that either compete for system resources or communicate with each other

- Doesn't just have to be symmetric processes that try to do the same thing at the same time

**Fatal Region** - when deadlock inevitable

- analyze a <u>joint progress diagram</u> , analyze code if wanting to determine deadlock



**Reusable and Consumable Resources**

- <u>Reusable Resource</u> : used by 1 process at a time and is not depleted by that use
  - E.g. processors, memory, files, sempahores
- <u>Consumable Resource</u>: created and destroyed upon consumption
  - e.g. keypress, interrupts, signals, messages
  - less likely to cause deadlock



**Conditions for Deadlock**

All 4 conditions must satisfy in order to get deadlock

1. <u>Mutual Exclusion</u>: A resource can only belong to at most 1 thread/process
2. <u>Hold-and-wait</u>: A process that is currently holding some resources is blocked when it requests more resources
   - e.g.  `malloc` nested within `wait(mutex)` 
3. <u>No Preemption</u>: resource cannot be taken from the process and the process is not releasing the resource
4. <u>Circular-Wait</u>: cycle in resource allocation graph

- If we have first 3 conditions, deadlock is possible. However, we need 4th condition in order to get deadlocking



<u>Resource Allocation Graph</u>

- the graph below has no cycle so no deadlocking
- arrows indicates a request for resource (pointing to resource) and holding a resource (pointing to process)
- dots mean how many of that resource is available

![lect14-resource_graph](.\Graphics\lect15-resource_graph.PNG)

- If a cycle exists, process **may** be deadlocked
  - below graph is not deadlocked because if P1 requests R1 and P3 is using 1/2 of the R1, it will be unblocked once P2 releases resources from R1

![lect14-resource_graph](.\Graphics\lect15-graph_with_cycle.PNG)



**Dealing with Deadlock**

1. <u>Ignore it</u>
   - Some systems such as Windows are implemented like this
   - Bad
2. <u>Deadlock Prevention</u>
   - **Mutual Exclusion**
     - cannot disallow mutexs as it is needed to prevent errors
   - **Hold and Wait**
     - Must ensure that a process holding a resource cannot request another resource in middle of execution
     - Solution 1 - Request all resources at beginning of program
       - Problem: Processes don't know how much resources it needs at beginning of process (e.g. text editor)
       - Problem: Performance issues as processes cannot start until they get all the resources it needs
     - Solution 2 - Release currently-held resources before requesting new resources
       - Problem: we can't release memory without losing state of process
     - Solution 3 - Two-phase locking
       - Process attempts to lock group of resources at once. If it fails to do so, then it releases the locks and tries again
         - Ensures that a process does not wait while holding a recent resource lock
       - Problem: Not applicable to our model of semaphores as it wouldn't know the value of semaphore without `wait` call
         - However, calling `wait` could block the process, and it can no longer release locks
         - Only specific systems that support non-blocking requests for resources and semaphores can handle this problem
   - **No Preemption**
     - Allow forcible removal of resources from a process
     - Problem: resources must not be memory as process state will be lost
3. Deadlock Avoidance
4. Deadlock Detection



### Lecture 16 - Deadlock Avoidance

**Deadlock Avoidance**

- As we could not completely eliminate any of the 3 deadlock conditions, we can only mitigate by <u>preventing a cycle in the resource allocation graph</u>
  - e.g. using pigeonhole



**Ordering of Resources**

- resources are given an order, which are followed by requests

- There would be no deadlock if both threads A and B have:

  ``` pseudo
  1. wait (m1)
  2. wait (m2)
  ```

Generalization:

- Give a priority integer to all resources in the system
- If process already has resource `Ri`, then process can only request another resource `Rj` if Rj has lower priority (`f(Ri) < f(Rj)`)
  - This is <u>strictly greater_than</u>  so if the process wanted multiple `Ri`, then it needs to be requested at once
  - If the process wants the resource with higher priority, it must release any resources with equal or lower priority
- For the dining philosopher problem, all but one philosopher will get left and then right chopstick. Last philosopher will get right and then left chopstick
- Use proof by contradiction to prove that a circular wait is no longer possible
  - Assuming circular wait, we will get `f(R0) < f(R1) < ... f(Rn) < f(R0)` , which is a contradiction 

Implemented by OS or application developer



**Stay Alert, Stay Safe**

- processes say in advance the maximum amount and type of resources it needs to OS to prevent deadlocking

- **This deadlocking occurs when there are not enough resources to complete any running process**

  - from the joint process diagram, OS can determine that 2/6 paths will cause deadlocking so it avoids those paths

  ![lect16-joint_progress_diag](.\Graphics\lect16-joint_progress_diag.PNG)

**Safe State**: When there is a scheduling order in which every process can run and finish

- Even if all of them request their max resources immediately
  - Imagine that the process holds all resources immediately



**Safe Sequence**: Sequence of processes `P1, P2, ..., Pn` such that

- Max Resources = Current Available Resources + All Resources held by `Pj` where `j < i`

  is greater than resources requested by `Pi`

- `Pi` can wait for resources as well

- When `Pi` terminates, `Pi+1` can continue

**Unsafe:** Any state that is not safe (aka. a deadlock is possible)

**Safe State Example:** - <u>see slides for examples</u>

- For a state to be safe, there must be a safe sequence (order of processes that prevents deadlocking)
- ... if there's not enough resources (e.g. 'free' was initially 1), then the state is unsafe

**Adjustment to Resource Allocation Graph**

- Previously, we had:
  - Solid line from process to resource == request for resource
  - Solid line from resource to process == resource allocated to process
- Now we add:
  - Dashed line from process to resource == **claim edge** == process may request resource
- Resource request only granted if converting claim edge to request edge does not result in cycle

**Banker's Algorithm** -- see diagram online

- Process A == available pool

Steps:

1. Look for row in matrix where resource needs are <= available resources in A
   - If no row exists, then state is unsafe
2. Give process all resources. Then, terminate process and return all resources back to A
3. Repeat 1, 2 until all process are terminated or no row remains that satisfy step 1



**Summary of Avoidance**

Pros:

- Prevent deadlocks and processes don't have to suddenly give up resources and can hold certain resources while waiting 

Cons:

- processes rarely know how much resources it will use
- resource may suddenly vanish
- potentially infinite number of processes
- Avoids situations that might not have resulted in deadlock as it assumes worst case



**More Deadlock Avoidance Recommendations**

1. Minimize number and length of critical sections
2. All tasks release semaphores and resources ASAP
3. Do not suspend task in critical region. Avoid `yield` or other blocking call
4. Error-free critical region
5. No resource allocation in interrupt handlers
6. Always perform null checks on pointers in critical regions (seg fault or exception may mean the lock is never released)

### Lecture 17 - Deadlock Detection + Recovery

**Deadlock Detection**

- Replace resource allocation graph with **wait-for graph**
  - Only processes in graph
  - If `P1 -> R1`  and `R1 -> P2`, remove resource box so it becomes `P1 -> P2`, meaning process 1 waits for process 2



**General Deadlock Detection Algorithm**

<u>Background</u>

- `Big_Theta(m*n^2)` algorithm
  - `m` == number of resources
  - `n` == number of processes
- E = existing resource vector = total resources
- A = available resource vector = available resources
- For example, if resource `i` with 2 instances and 1 currently assigned will have `Ei = 2, Ai = 1`
- C = Current allocated resource matrix
  - Row `i` is the resources used by process `i`
  - `Cij` is the # of `Rj` used by `Pi`
- R = Request matrix
  - Row `i` is the resources requested by `Pi`
  - `Rij` is the # of `Rj` requested by `Pi`
- Note, `Sum of Cij + Aj = Ej`

![lect17-General_Deadlock_Detection](.\Graphics\lect17-General_Deadlock_Detection.PNG)

<u>Algorithm Setup</u>

- All processes are unmarked
- Assume worst-case conditions

<u>Algorithm</u>

1. Find unmarked process whose requests can all be satisfied with available resources. 
   - Find `Pi` such that `Ri` <= A.
2. If found, add allocated resources to A and mark process.
   - `A = A + Ci`
3. Repeat step 1 if another unmarked process satisfies condition. Otherwise, terminate algorithm

Any unmarked process is deadlocked

<u>Sanity Check</u>

- Compare A to E



**When to Run Deadlock Algorithm**

- Run if a resource request is not granted
- Run it periodically
- Run when CPU usage is low
  - Can indicate deadlock as processes are all blocked



**Deadlock Recovery Stategies **

- Robbery - Preemption
  - Select victim and take resources (must be appropriate type like processor or network access, not printer)
- Mass Murder
  - Terminate all processes involved in deadlock
  - Guarantees deadlock is resolved
  - Since all processes restarting, it could cause another deadlock (why couldn't the processes reach that state again?)
- Murder
  - Choose victim and kill process
  - Must repeatedly run deadlock detection algorithm to verify that deadlock is resolved
- Rollback
  - Return process states to an earlier safe state
  - <u>Checkpoint:</u> saved safe state created in advance 
    - Includes memory image, call stack, resource state
- System Reboot
  - 100% resolves system's deadlock



**Victim Selection**

Factors to consider

1. Priority of process
2. How long process has been executing
3. How long until process is finished execution
4. What resources do the process have
5. Future resource requests (so we can deallocate selected resources in anticipation)
6. Process is interacting with user?
7. How often has this process been selected as a victim
   - prevents starvation

- Factors favour killing younger processes
  - Faster to terminate younger processes
  - Prevents "Starvation"
  - No "cyclic" killing of resources (killing process is too quick before next oldest thread has finished running)

**Tradeoff**

- Since we err on the conservative side, we assume worst-case: that processes hold resources until its end. Then, we are not optimizing usage and might accidentally kill an innocent process
  - Takes longer
- However, the correctness of the program (and prevention of deadlocks) is preserved