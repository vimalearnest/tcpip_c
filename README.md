TCP/IP servers in C

My attempt to implement code at https://github.com/rspivak/csdesign in C.

1. TCP iterative server (iterative)

2. TCP Concurrent Server, One Child per Client (con_fork)
 
3. TCP Concurrent Server, I/O Multiplexing (con_select)
 
4. TCP Concurrent Server, I/O Multiplexing (con_poll)

5. TCP Preforked Server, Children Call (con_accept)

6. TCP Concurrent Server, One Thread per Client (con_thread)

7. TCP Prethreaded Server, Pool of Threads (con_thread_pool)

8. TCP Concurrent Server, I/O Multiplexing (con_epoll)

9. TCP client (client)

TODO

 1. Refactor common code

 2. Modify makfile to build to a common folder also

     (a) make debug builds
     (b) Valgrind
     (c) Electric Fence
 
 3. Install and setup daemon tools

 4. Modify the data transaxtion as follows

     (a) Clients request images 
     (b) Client identifies which image it wants
     (c) Server reads the imag from a file on disk

 5. Implement code to measure performance

 6. RST Example

 7. Thundering herd demonstration

 8. TCP_CORK, Example  

 9. Documentation for every example

10. Setup to start daily nigthly builds and tests and capture metrics.

11. Publish performance metrics in a web page (with history to see improvements over time)
