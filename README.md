# Conway-s-Game-of-Life
Multi-threaded implementation in C of Conway's Game of Life using POSIX threads.

1.  Make sure Cells.h and Barrier.h header files are in the same directory as the 
    GameofLife.c file.
    
2.  Use the following command to compile and run the code on your machine:
        gcc GameofLife.c -lpthread -lrt

3.  No sleep(n) has been implemented so the multi-threaded code runs really fast.
    Comment out line 53 so that the code stops for a second after every 500 generations.

4.  The code runs in an infinite loop right now. If it is supposed to stop after a certain
    number of iterations, that can be implemented by forcefully exiting all threads when 
    generations variable reaches a certain threshold. It is to be noted that generations is not a 
    shared variable and it is only manipulated by the zeroth thread, so the condition can be 
    easily implemented.
