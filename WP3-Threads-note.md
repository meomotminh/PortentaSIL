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

        - **Many to one model**

