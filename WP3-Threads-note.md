## Multithreading Implementations
Ref: Kavi, K. 2013 *Multithreading Implementations* available at https://www.researchgate.net/publication/2734966_Multithreading_Implementations, accessed on 26 August 2021

- a thread is a single sequential flow of control
- multithreaded program allows multiple threads of control within a process
- unsuitable of process model as a unit of execution (thread of control) for concurrent app
- kernel thread support 

### Introduction
- traditional process is essentially an address space with a single point of control and associated state info and resources.
- traditionally process considered atomic unit of control flow in a system and was designed to be independent, self contained unit of computation

- Process management cost:
    - associated a large amount of state info such as file descriptors, virtual address memory maps, accounting info...
    - in UNIX, process creation involve copying the entire address space of the parent
    -> make process expensive
    - cooperative tasks in concurrent app did not need to be self contained

- Process interaction cost:
    - cooperating tasks do not require address space protection from each other
    - cross address space protection prevents processes including those belonging to the same app from easily sharing memory
    - while some OS allow processes to share memory, such mechanisms cumbersome and require form of kernel mediation to set up the shared memory segments and synchronize access to these segments
    - Synch bw processes require system level synch primitives -> create and use these synch involve a system call -> expensive because each argument passed through a system call has to be copied into kernel space and then checked by the kernel to ensure valid before system call can execute

- Resource costs:
    - Processes use kernel recources which limited
    - in most OS, kernel is permanently resident in physical memory and is never swapped out to disk -> limited memory
    - every process requires some kernel memory
    - many data structures used by kernel to control process as process table are fixed length arrays which cannot be dynamically extended.
    - process associate with them a large number of resources such as a virtual address space, root directory, working directory, user identifier, accounting info, signal masks

- Threads: the solution
    - term thread refer to a single path of execution or control flow.
    - in multithreaded process, multiple threads share the same address space
    - memory sharing & synch is simplified bc these threads exist within the same address space
    - each thread has same communication facilities as separate process
    - each thread can independently communicate with other processes through normal IPC mechanisms: semaphores, sockets and pipes
    - minimal component associated with a thread are thread control block and a user level stack
    - thread control block store context of the thread when not executing and other thread management info
    - threads in the same process can share data without requiring kernel intervention
    - threads share file desciptors, kernel process data structures, signal masks, virtual address translation maps, root directory and working directory -> make better use of system resoures and eliminate unnecessary duplication
    - threads are structuring mechanism to cleanly program independent tasks within a single app. OS supported threads allow overlap of computation and I/O on both uniprocessors and multiprocessors producing significant performance benefits
    - improved responsiveness by time slicing of its threads even if the app is mostly compute-bound
    - in addition, by using priority based scheduling, threaded programs can respond to intermittent events
    - traditional process can only run on a single processor, threaded programs used kernel threads can auto make use of multiple processors if available

- Classification
    - Implementation model: refers to the level of OS support offered for multithreading, visible to kernel and can be independently scheduled on a single or multi processors ?
        - User level threads: create and manage entirely at user level. lightweight program structuring tool.
            - inexpensive
            - invisible outside the process that created them
            - kernel is unaware of threads -> if thread block in the kernel, the entire process block 
        - Kernel level threads: share same address space if belong to the same process. kernel threads can be scheduled independently
        - Hyrid thread model: multiplex 1 or more user level threads on 1 or more kernel level threads in a process. Each process may have multiple kernel threads which may be scheduled independently. in hybrid, scheduling occurs at 2 distinct levels. At user level, user level threads lib schedule the user threads while kernel schedule the kernel level thread
    
    - Schedulig model: scheduling policy
        - **nonpreemptive scheduling**: a threads run until it either block for a resource or volutarily gives up the processor -> not practical at kernel level as ill behaved app could refuse to yield processor to other -> useful at user level with excellent performance (no lock). but not possible to implement preemptive -> real time app and GUIs cannot use this model, timeslicing not available

        - **preemptive priority non time sliced scheduling**: highest priority runnable threads run until volunteer yield the processor or block for a resource or preempted by higher thread. 
            - priority is fixed
            - well suited for user level thread
            - not appropriate for kernel level threads since unfair at best for low priority threads in the system

        - **preemptive time sliced (Round robin) scheduling**: threads are alloted a time slice and a thread run until it either block for a resource, yield processor or uses up its time slice.
            - all equal priority
            - FIFO or LIFO (may lead to starvation)
            - not useful for user level thread as can result in unnecessary context switching on threads
        
        - **preemptive priority time sliced scheduling**: commonly used for kernel level threads. it is fair and prevent low priority process from starvation. Priority of a process is increased or decreased depend on how much CPU time it has already obtained
            - threads with same priority are preemptive time sliced
            - higher priority threads can preempt lower priority threads and run beyond their time slice if no thread of equal or higher priority is runnable
            - possible for user level threads, scheduling algorithm is complex because priorities are constantly recomputed
            - mostly used for kernel level threads

    - Programming Interface: interface provided to the developer, POSIX and Win32

        - POSIX: portable thread programming interface without specifying underlying implementation
            - user level thread packages (Provenzano), kernel level threads (LinuxThreads), hybrid thread packages (Solaris threads)

        - UNIX International threads: known as Solaris thread interface, closely resembles POSIX interface

        - DCE threads: corresponds to Draft 4 of POSIX standard and is quite similar to the POSIX standard (Digital UNIX, HP/UX from HP and AIX from IBM)

        - Win32 threads: kernel level threads

        - OS/2 threads: developed by Microsoft, reimplemented by IBM, resembles Win32 threads to some extent

        - Java threads: different from POSIX and Win32

    - Multiplexing Model: classified based on their multiplexing model which describes all threads based on an abstract 2 level model. all thread packages assumed to consist of 2 level of thread-like entities and models differ in the manner in which the upper level threads are multiplexed on the lower level threads. 
        - upper level threads -> user level threads
        - lower level threads -> kernel level threads

        - **Many to one model**: many upper level threads or user threads are multiplexed on 1 lower level thread. all systems which have traditional single threaded processes and user level thread lib correspond to this model. SunOS, lighweight processeLWPs
        (JAVA)
        - **One to One model**: user level thread bound permanently to a kernel level thread (Win32)
        - **Many to Many model**: most flexible model of multithreading. it allow for multiple lower level thread within each process on which multiple user level threads can be multiplexed.
    
    - Implementation:
        - Solaris Threads:  hybrid category, offer both user level thread and kernel level thread, many to many model. offer 3 types of thread objects: kernel threads, lightweight processes (LWPs) and user level threads
            - Kernel Threads:
                - can be scheduled independently by the kernel
                - need not a corresponding process
                - perform many kernel level functions and kernel itself consist of a set of kernel threads
                - kernel threads which perform internal kernel functions do not have a corresponding process and share the same address space
                - a kernel thread consist of a thread data structure and a stack. thread data structure contains:
                    1. contents of the kernel threads register context 
                    2. priority and scheduling info
                    3. pointers for putting the thread in scheduler queues and queues for resources
                    4. pointer to the thread's stack
                    5. pointers to the associated LWP if the kernel thread has an associated LWP else this contain NULL
                    6. pointers to the queue of all threads in the system. also pointer to the queue of all threads in the associated process if the kernel thread has a process associated with it
                    7. Info about the associated LWP if there is an associated LWP
                    
                - Bare kernel threads are not exported outside the kernel. user interface for using kernel threads are LWPs. LWPs built on top of kernel threads

                - Lightweight processes (LWPs) actually kernel level thread wrappers which exported by the kernel -> can be scheduled independently by the kernel and transparently make use of multiple processors.

                - UNIX single threaded process has 2 important kernel per process data structures, the *proc* structure and user area (*u*). In fact the process table in UNIX kernel is actually an array of proc structures with 1 proc per process in the system. *proc* structure contains process info which is required even when the process is not running, never paged out to disk. *u* area on the other hand contains info which is required only when the process is running. *u* area can be paged out to disk

                - These UNIX per-process data structures do not map well to multithreaded processes where kernel data structures can be per process or per LWP. In Solaris, all process specific and LWP independent info is grouped into the *proc* data structure. Thus the Solaris *proc* structure correspond to the traditional *proc* structure and LWP independent part of *u* area. the per LWP info is stored in new per LWP data structure called *lwp* structure which contains:
                    - user lvl register context of the user level thread associated with the LWP if LWP is not running
                    - Signal handling info
                    - System call arguments, results and error code                    
                    - Accounting info and profiling data
                    - Virtual time alarms
                    - User time usage and CPU usage
                    - Pointer to the kernel thread associated with LWP
                    - Pointer to the associated *proc* structure
            
            - User level threads: run user level code. in order for user level thread to run, it must have an associated LWP. binding maybe temp or per -> managed by a user level thread lib which multiplexes them on top of LWPs. when a multithreaded process begin, the thread lib create a pool of LWPs. The size of the pool is a default which depends on the number of user level thread and processors.
                - lib call *thr_setconcurrency(int n)*, allow to set number of LWPs. thread lib not auto create number of LWPs requested by app, it only create LWP is there are user threads which can run on them. as long as number of user threads is <= n, number of LWPs = number of user level threads. if n.o of user threads > n, number of LWPs = n. Thread lib destroy LWPs if idle for 5 min. if n.o of user level threads falls below n, n.o of LWPs will also fall until = n.o of user level thread.
                - each user level thread also has state which must be kept separate from LWP state as threads and LWPs have separate identities. Each thread has a user level stack and thread data structure associated with it
                - Thread data structure contains:
                    - Thread's ID
                    - saved register context of user level thread (when thread not running)
                    - thread signal mask
                    - priority and scheduling info
                    - thread specific private data
                - Signal management: all LWPs in a process share a common set of signal handlers. Each LWP have its own signal mask so it can block unwanted signals. An LWP specify its own alternate stack for handling signals.Traps which are synch signals produced by action of an LWP are delivered to the LWP that produced them. Interrupt signal (asynch) can be delivered to any LWP has not masked the signal. Since LWPs invisible outside the process, an LWP in 1 process cannot send a signal to a specific LWP in another process.
                - Fork semantic: fork system create a child process. 2 possible behaviors: entire process copied or only process and calling thread copied
                - Multiprocessing: mutual exclusion mechanism as *adaptive lock*, spin on the lock
            
        - Windown NT: kernel offer services which any OS is minimally required to provide. any additional services can be provided by optional user mode servers
            - NT executive provides services like
                - Virtual memory management
                - Resource management
                - I/O and file system management
                - IPC
            - uniform object based approach to resource management
            - native NT process differ from process in UNIX OS:
                - Processes and Threads are treated as objects
                - no parent child relationship bw processes
                - process and threads have integrated synch capabilities
            - their creation is by an object manager. 
                - object header: attributes (obj name, security descriptor, accounting info, list of processes have handles to that obj)
                - obj creator control and use obj body (variable component of the obj). creator supplies services associated with the obj body
                    - process ID
                    - base priority
                    - default processor affinity
                    - quota limits
                    - execution time
                    - miscellaneous accounting info
                    - exception/debugging ports
                    - exit status
            - in window NT, process is statuc, not associated with a process
                - thread body:
                    - client ID
                    - Thread context
                    - Dynamic priority
                    - base priority +-2 base priority of process
                    - thread processor affinity
                    - thread execution time
                    - termination port
                    - thread exit status
            - Synch obj: process obj, thread obj, file obj, event obj, event pair obj, semaphore obj, timer obj, mutuant obj. calling wait routine.
                - thread obj terminate -> all waiting wake up
                - mutuant obj released -> one of waiting threads woken up
            - Thread scheduling: 2 types of native obj: executive obj and kernel obj
                - executive obj created by an executive component and can be exported to the user level
                - kernel obj created by kernel, restricted to kernel mode use
                - kernel process obj has a pointer to a linked list of all kernel thread obj associated with the process
                    - ready
                    - standby
                    - running
                    - waiting
                    - transition
                    - terminated
                - kernel scheduler is a preemptive priority timesliced scheduler
                - kernel code include critical sections as those which update global kernel data structures, which must be executed at any given by only 1 processor. kernel code must acquire a spin lock before entering critical sections.
                - spin locks are expensive, waster CPU cycles and threads spinning on locks cannot be preempted.

            - Win32 threads: in win32 env, a process consists of a virtual address space and associated state info and resources. the threads are kernel threads and correspond to 1 to 1 multiplexing model with no user visible distinction between user level context and kernel level context of the thread.
                - Processes and Threads are created by CreateProcess() and CreateThread() calls 
                - each process in a system has a priority class associated with it.
                - 4 priority class process:
                    - real-time
                    - high
                    - normal (*default*)
                    - idle (run only when system idle)
                - 5 priority levels in each priority class:
                    - (-2) lowest 1
                    - (-1) below normal
                    - (0) normal (*default*)
                    - (1) above normal
                    - (2) highest
                - Parent process can request a specific priority class for the process using *CreateProcess()*
                - prioritiy of process can be examined using *GetPriorityClass()*
                - changed using *SetPriorityClass()*
                - priority level of a thread decay with CPU usage and increases if it has been blocked
                - *SuspendThread()*, *ResumeThread()*, *WaitForSingleObject()*, *WaitForMultipleObjects())*
                - 3 synch : mutex, semaphore and event obj (signal the occurance of an event, manual-reset or auto-reset)
                - *SetEvent()* and *PulseEvent()* (free only waiting threads)
                - Thread Local Storage (TLS) *TlsSetValue(TlsIndexVar, TlsDynDataVarPtr)* and *TlsGetValue(TlsIndexVar)*
                - process exts if any thread call *ExitProcess()*
        - JAVA threads
            - has integrated Thread
            - java is pure oop language, unlike C++, there are no global functions or data
            - 1 way threads can be created is subclassing (*extending*) the Thread class and instantiating objects of the subclass
            - developer has to add behavior by overriding methods of Thread class. *start() -> call run()*, the run method corresponds to the start_routine passed to Pthread_create in thread interfaces like POSIX Pthreads
            - Applet class, java does not support multiple inheritancen replaced by interfaces
            - Applet can use Thread by implementing Runnable interface
            - disadvantages of using runnable interface is that none of the methods of the Thread class can be used by the thread which uses the interface's run method
            - *suspend(), resume()*, *stop()* kills the thread, *join()* not time-out, *join(int)* has time-out
            - In Thread group, methods can be called collectively 

            - Scheduling: preemptive priority scheduled, 10 priority levels
            - Synch: 
                - *monitor*, every class and object can potentially have a monitor associated with it. It is created when an obj has 1 or more of its methods declared as synch. obj *monitor* ensure that only 1 thread is executing in any of the obj synch methods
                - *wait(), notify() and notifyall()* to wait for some event, can only call within synch methods (has a *monitor*). *wait()* cause thread to relinquish control of the monitor and wait until someone call *notify()* or *notifyall()*
                - java allow designate var as volatile. Java virtual machine always reads from main memory and writes to main memory -> ensure coherent memory location

        - MACH: microkernel based OS, is based on some earliest microkernel and multithreaded OS such as RIG and Accent, *C-Threads*
            - replace traditional UNIX process with 2 entities, a static entity known as a task which consisted of an address space and associated resources and threads which were dynamic executing entities
            - use client server architecture, microkernel + user lvl servers which implement services and env needed to run programs
            - Task contains:
                - pointer to the virtual mem address translation maps
                - pointer to the list of threads in the task
                - info related to ports (abstraction of interprocess communication channels)
            - Thread structures:
                - pointer to task structure of the task to which it belongs
                - pointer to the list of threads in the task
                - pointer to the thread's kernel stack
                - state of thread and priority info
                - pointer for putting thread on various queues
                - per thread port related info
            - different implementation of C-THread interface
                - coroutine: each C-thread maps to a user level thread. All threads in an app are multiplexed on a single Mach kernel thread in a single task
                - Kernel thread implementation: each C-threads map to a single Mach kernel level thread
                - Task implementation: each C-thread map to a Mach task

# Combine Assembly with C Programming
    assembly programming is indispensable when accessing and manipulating CPU registers but also tedious. 
## Execution Image
    an execution image (file) generated by a complier-linker consists of 3 logical parts:
        - Text section: Code section containing executable code
        - Data section: initialized global and static variables, static constants
        - BSS section: un-initialized global and static variables (BSS is not in the image)
    During execution, the executable image is loaded into memory to create a run-time image
                --------------------------------------------
    (Low Address)   | Code | Data | BSS | Heap | Stack  |   (High Address)
                --------------------------------------------
    
    - A run-time image consists of 5 (logically) contiguous sections. Code & Data sections are loaded directly from the executable file. BSS section is created by the BSS section size in the executable file header.
    - in execution image, the Code, Data and BSS sections are fixed and do not change.
    - Heap area is for dynamic memory allocation within the execution image. The stack is for functions calls during execution. Stack is logically at the high address end of the execution image and grows downward