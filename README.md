# CALAB


# Lab2

1. How do you **pass command line arguments** to a program when using gdb?
2. `ESC`
3. How do you **set a breakpoint** which only occurs when a **set of conditions is true** (e.g. when certain variables are a certain value)?
   `break . . . if expr`
   delete breakpoint: `clear`
   Watchpoints are similar to breakpoints. However, watchpoints are not set for functions or lines of code. Watchpoints are set on variables. When those variables are read or written, the watchpoint is triggered and program execution stops.
   set the watchpoint: `watch var exp`
4. How do you **execute the next line of C code** in the program after stopping at a breakpoint?
   `n`
5. If the next line of code is a function call, you'll execute the whole function call at once if you use your answer to #3. (If not, consider a different command for #3!) How do you tell GDB that you **want to debug the code inside the function** instead? (If you changed your answer to #3, then that answer is most likely now applicable here.)
   `s`
6. How do you **resume the program after stopping** at a breakpoint?
   `c`
7. How can you **see the value of a variable** (or even an expression like 1+2) in gdb?
   `print exp`
8. How do you configure gdb so it  **prints the value of a variable after every step** ?
   `display exp`
9. How do you **print a list of all variables and their values** in the current function?
   `info local`
10. How do you **exit** out of gdb?
    `q`

return a pointer to the stack memory (point to local varible `v`) which is invalid after returning

return an instance in the stack memory

# The difference between pointer to int and pointer to an array of ints

```c
int *x;
int y[10];
int (*z) [10];

```

z is an pointer of type `int[10].` An array in c is the address of the first element.

```c
int (*x)[6];                 //pointer to an array of integers
int y[6] = {1,2,3,4,5,6};    //array of integers
int *z;                      //pointer to integer
int i;

z = y;
for(i = 0;i<6;i++)
    printf("%d ",z[i]);

x = y; // should be x = &y but leave it for now!

for(i = 0;i<6;i++)
    printf("%d ",x[i]); // note: x[i] not (*x)[i]
```

```c
1 2 3 4 5 6 109247792 109247816 109247840 109247864 109247888 109247912
```
