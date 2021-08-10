# The standard template Library (STL) for AVR with C++ streams

- Version SGI STL, release 2000. Other version is GNU STL, uSTL

### Installation and Configuration

- STL consist only of header files with all source code inline

- Configuration : is optional. only need to change the defaults if you want to influence the STL memory management strategy

- 1st section show how u can influence how many places a vector allocates-ahead so it has storage in the bank ready for furture allocation requests. Default allocates 20 places ahead. 
- 2nd and 3rd show how u can control the allocate-ahead strategy for deque and string. Like vectors the default is to be conservative with memory use

### Stream Support

- SGI STL is quite pure in that it does not attempt to supply a streams implementation itself, instead relying on its presence in the environment. avr-libc does not provide streams, so i have provided it via a port of the streams part of the uClibc++ project. Specifically, u get:
    - ostream, istream, iostream base classes
    - the istringstream and ostringstream string streams
    - stream iterators

- Some bonus if you use Arduino Platform
    - ohserialstream, ihserialstream, iohserialstream for reading and writing from and to the hardware serial ports on the Arduino. These streams wrap an instance of the HardwareSerial class
    - olcdstream for writing to an LCD character display (wrap an instance of LiquidCrystal)

### Memory Considerations

- Flash memory: your flash usage is depend on the templates you use because templates take up no space until declared
- if all u need are the most popular templates such as string and vector then 16k microcontroller may be enough.
- SRAM depend on your usage, he made modifications to ensure the growth policy of the vector and string classes is suitable for 2k controllers. The complex containers such as map and set use a memory-hungry backing tree to implement their structure
- all containers are neutral in their use of dynamic memory. if you declare a container, use it as much as u want and then let it go out of scope then your controller's dynamic memory is returned to exactly the state in which it started

### Templates in C++

- a template is a simple and yet very powerful tool in C++. Simple idea is to pass data type as a parameter so we don't need to write the same code for different data types
- C++ adds 2 new keywords to support templates *template* and *typename*
- Templates are expanded at compiler time, like macros. The difference is compiler does type checking before template expansion. Idea is simple, source code contains only function/class, but compiled code may contain multiple copies of same function/class

### Using C++n and WSL in VS code

- configure VS Code to use GCC C++ compiler (g++) and GDB debugger on ubuntu in Window Subsystem for Linux (WSL). GCC stand for GNU Compiler Collection, GDB is GNU debugger

- VS Code has support for working directly in WSL with the remote - WSL extension. We recommend this mode of WSL development,  where all your source code files, in addition to the compiler are hosted on the Linux distro. 

```
- sudo apt-get update to update Ubuntu package lists
- sudo apt-get install build-essential gdb

```

- STL is a set of C++ template class to provide common programming data structures and functinons such as lists, stacks, arrays, etc. It is a library of container classes, algorithms, and iterators. It is a generalized library and so, its component are parameterized. A working knowledge of template classes is a prerequisite for working with STL

- STL has 4 components:
    - Algorithms
    - Containers
    - Functions
    - Iterators

- Algorithms: the header algorithm defines a collection of functions especially designed to be used on ranges of elements. They act on containers and provide means for various operations for the contents of the containers.
    - Sorting
    - Searching
    - Important STL Algorithms
    - Useful Array algorithms
    - Partition Operations
    - valarray class

- Containers:  Container and container classes store objects and data. There are in total 7 standard "first-class" container class and three container adaptor classes and only 7 header files that provide access to these containers or container adaptors
    - Sequence Containers: implement data structures which can be accessed in a sequential manner
        - vector
        - list
        - deque
        - arrays
        - forward_list
    - Container Adaptors: provide a different interface for sequential containers
        - queue
        - priority_queue
        - stack
    - Associative Containers: implement sorted data structures that can be quickly searched (O(log n) complexity);
        - set
        - multiset
        - map 
        - multimap
    - Unordered Associative Containers: implement unordered data structures that can be quickly searched
        - unordered_set
        - unordered_multiset
        - unordered_map: is an associated container that stores elements formed by the combination of key value and a mapped value. The key value is used to uniquely identify the element and the mapped value is the content associated with the key. Both key and value can be of any type predefined or user-defined.
            - Internally unordered_map is implemented using Hash Table, the key provided to map are hashed into indices of a hash table that is why the performance of data structure depends on hash function a lot but on an average, the cost of search, intert and delete from the hash table is O(1)
        - unordered_multimap

- Functions: STL includes classes that overload the function call operator. Instances of such classes are called function objects or functors. Functors allow the working of the associated function to be customized with the help of parameters to be passed

- Iterators: iterators are used for working upon a sequence of values. They are the major feature that allow generality in STL

- Utility Library: defined in header <utility> pair

### Hashing

- Suppose we want to design a system for storing employee records keyed using phone numbers and we want following queries to be performed efficiently

1. insert a phone number and corresponding info
2. search a phone number and fetch the info
3. delete a phone number and related info

we can think of using the following data structures to maintain info about different phone number

1. Array of phone numbers and records
2. Linked List of phone numbers and records
3. Balanced binary search tree with phone numbers as keys
4. Direct Access Table

for *array and linked lists* we need to search in a linear fashion, which can be costly in practice. If we use arrays and keep the data sorted, then a phone number can be searched in O(logn) time using binary search, but insert and delete operations become costly as we have to maintain sorted order

With *balanced binary search tree* we get moderate search, insert and delete times. All of these operations can be guaranteed to be in O(logn) time.

Another solution is *direct access table* where we make a big array and use phone numbers as index in the array. An entry in array is NIL if phone number is not present, else the array entry stores. Time complexity wise this solution is the best among all, can do all operations in O(1) time. For example to insert a phone number, create a record with details of given phone number, use phone number as index and store the pointer to the created record in table

- *Hashing* is the solution that can be used in almost all such situations and performs extremely well compared to above data structures like Array, Linked List, Balanced BST in practice. With hasing we get O(1) search time on average and O(n) in worst case

- *Hash Function*: function convert a given big phone number to a small practical integer value. The mapped integer value is used as an index in hash table. A good hash function should have following properties:
    - Efficiently computable
    - Should uniformly distribute the keys

- *Hash Table*: an array that stores pointers to records corresponding to a given phone number. An entry in hash table is NIL if no existing phone number has hash function value equal to the index for the entry
    - Collision Handling: newly inserted key maps to an alreadly occupied slot is hash table is called collision and must be handled using some collision handling technique.
        - Chaining: make each cell of hash table point to a linked list of records that have same hash function value-> simple but require additional memory
        - Open addressing: all elements are stored in the hash table itself. Each table entry contains either a record or NIL. when searching for an element, we examine the table slots one by one until the desired element is found or it is clear that the element is not in the table.

### Maps

associative containers that store elements in a mapped fashion. Each element has a key value and a mapped value. No 2 mapped values can have same key values

- Basic functions associated with Map:
    - begin(): return an iterator to the 1st element in the map
    - end(): return an iterator to the theoretical element that follows last element in the map
    - size(): return number of elements in the map
    - max_size(): return maximum number of elements that map can hold
    - empty(): return whether the map is empty
    - pair_insert(keyvalue, mapvalue): add a new element to the map
    - erase(iterator_position): remove the element at the position pointed by the iterator;
    - erase(const g): remove the key value "g" from the map
    - clear(): remove all the elements from the map

## Debugging AVR dynamic memory allocation

the avr-libc port of the standard C library for gcc support dynamic memory allocation throuhg malloc() and free(). These function (particularly when used to implement the new and delete C++ operators) allow you to use more memory throughout the life of your program as it's needed than you could possibly statically allocate upfront

The caveat to this is that an AVR MCU does not have much RAM available for you to use. Memory leaks due to poor coding have always been bad. On a system with only a few kB of RAM memory leaks can be catastrophic. Combine that with no visual debugger and you can see how the advice that is sometimes given is to avoid dynamic memory allocation entirely.

The advice to avoid is too extreme. if u don't understand how it works or u don't trust your ability to write leak-free code then do stay away because you will just run into very hard to debug problems as your codebase grows. However, for those of us that have been doing this for longer than we care to remember and know exactly what's going on, here are a few debug functions we can use in lieu of a real debugger

### How dynamic memory allocation is implemented

It's all in a couple of hundred lines of well commented code in avr-libc, the file location is libc/stdlib/malloc.c.

- The amount of memory available for dynamic allocation the difference between __malloc_heap_end and __malloc_heap_start. u can customize these values, e.g put the heap in external RAM if u using an AVR has an external SRAM chip mapped into the address space. As memory is free up, it is accounted for in a data structure called the free list.

When u call malloc this is how it tries to satisfy your request:

1. very small size requests are rounded up to an internal minumum size
2. An exact match is sought from the free list, if one is found then that's used
3. if the free list contain block larger than the request then the smallest of those blocks will be used and the remainder kept in the free list as a new, smaller entry
4. A fresh block is taken from the heap as long as there is space between the current heap end and the stack pointer
5. memory is full, fail and return NULL

It's worth discussing what happen during call free():

1. if there is no free list then one might be created with this deallocation on it, unless this is the last allocated entry in which case there is no need for a free list and the end-of-heap pointer is just reduced instead
2. The correct location in the free list is found and compaction occurs, that is adjacent free memory blocks are combined into 1
3. If the compaction results in a new topmost chunk then the end-of-heap marker is reduced accordingly

#### Debugging Functions

 reference : 
    - https://andybrown.me.uk/2011/01/01/debugging-avr-dynamic-memory-allocation/
    - https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
