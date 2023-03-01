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

return by value may lead to following problems:

1. Forget `free(v.data)`
2. `vector` is a large object, we often pass by value to avoid coping

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




## Lab 3

[Computer Architecture I](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/index.html) [ShanghaiTech University](http://www.shanghaitech.edu.cn/)

## Objectives

* Run and debug RISC-V assembly code.
* Write RISC-V functions using the right calling procedure.
* Get an idea of how to translate C code to RISC-V

## Intro to Assembly with RISC-V Simulator

So far, we have been dealing with C program files (.c file extension), and have been using the gcc compiler to execute these higher-level language programs. Now, we are learning about the RISC-V assembly language, which is a lower-level language much closer to machine code. For context, gcc takes the C code we write, first compiles this down to assembly code (gcc uses a more complex assembly language than RISC-V), and then assembles this down to machine code/binary.

In this lab, we will deal with several RISC-V assembly program files, each of which have a .s file extension. To run these, we will need to use a RISC-V simulator. The simulator we will use was developed by [Keyhan Vakil](https://github.com/kvakil) and improved by [Stephan Kaminsky](https://github.com/ThaumicMekanism). The simulator is called **Venus** and can be found online [here](https://venus.cs61c.org/). We have deployed Venus on our Autolab server ([link](https://autolab.sist.shanghaitech.edu.cn/venus/)).

### Assembly/Venus Basics

* Enter your RISC-V code in the “Editor” tab
* Programs start at the first line regardless of the label. That means that the main function must be put first.
* Programs end with an ecall with argument value 10. This signals for the program to exit. The ecall instructions are analogous to [System Calls](https://en.wikipedia.org/wiki/System_call) and allow us to do things such as print to the console or request chunks of memory from the heap.
* Labels end with a colon (:).
* Comments start with a pound sign (#).
* You CANNOT put more than one instruction per line.
* When you are done editing, click the “Simulator” tab to prepare for execution.

**For the following exercises, please save your completed code in a file on your local machine. This is crucial for the checkoff portion to work.**

## Exercise 1: Familiarizing yourself with Venus

Getting started:

1. Paste the contents of [lab2_ex1.s](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab2_ex1.txt) into the editor.
2. Click the “Simulator” tab. This will prepare the code you wrote for execution.
3. In the simulator, click “Assemble & Simulate from Editor”
4. In the simulator, to execute the next instruction, click the “step” button.
5. To undo an instruction, click the “prev” button.
6. To run the program to completion, click the “run” button.
7. To reset the program from the start, click the “reset” button.
8. The contents of all 32 registers are on the right-hand side, and the console output is at the bottom
9. To view the contents of memory, click the “Memory” tab on the right. You can navigate to different portions of your memory using the dropdown menu at the bottom.

### Action Item

Record your answers to the following questions in a text file. Some of the questions will require you to run the RISC-V code using Venus’ simulator tab.

1. What do the .data, .word, .text directives mean (i.e. what do you use them for)?  **Hint** : think about the 4 sections of memory.
   `.data` tells the assembler to the data segment in memory containing global variables and static local viariavles.  `.word` allocate space in “data segment" memory to store data. `.text` tells the assembler to the 'Text data memory'.
2. Run the program to completion. What number did the program output? What does this number represent?
   `34`. The Fibonacci numbers `F(9)`
3. At what address is n stored in memory?  **Hint** : Look at the contents of the registers.
   `0x10000010`
4. Without using the “Edit” tab, have the program calculate the 13th fib number (0-indexed) by *manually* modifying the value of a register. You may find it helpful to first step through the code. If you prefer to look at decimal values, change the “Display Settings” option at the bottom.
   `t3 = 13`

#### Check-off

Show your TA that you are able to run through the above steps and provide answers to the questions.

## Exercise 2: Translating from C to RISC-V

Open the files [lab2_ex2_c.c](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab2_ex2_c.c) and [lab2_ex2_assembly.s](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab2_ex2_assembly.txt). The assembly code provided (.s file) is a translation of the given C program into RISC-V.

### Action Item

Find/explain the following components of the assembly file and put your answers in a text file.

* The register representing the variable k.  `t0`
* The registers acting as pointers to the source and dest arrays. `t1` and `t2`
* The assembly code for the loop found in the C code. `Loop:`
* How the pointers are manipulated in the assembly code.  `lw t5, 0(t4)`

After you’ve answered explained the above components, edit lab2_ex2_assembly.s so that it dest satisfies the following conditions.

* dest[i] = 2 * source[i] for even i
* dest[i] = 1 for odd i

 **Hint** : This can be done by adding one line of code and modifying another (in other words, you only need to make 2 changes). Look at the initial values of dest; how does this help you implement this modification?

Verify that your changes work for the given source and dest arrays by running your code in a new Venus tab and check that the output looks like:

<pre class="highlight"><tt>3 1 4 1 5 9
6 1 8 1 10 1
</tt></pre>

#### Check-off

Show lab2_ex2_assembly.s to your TA, and run it in Venus, which should give the correct result.

## Exercise 3: Factorial

In this exercise, you will be implementing a function factorial in RISC-V that has a single integer parameter n and returns n!. A stub of this function can be found in the file [factorial.s](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/factorial.txt). You will only need to add instructions under the factorial label, and the arguments that are passed into the function are defined at the top of the file. You may solve this problem using either recursion or iteration.

### Action Item

Implement factorial and make sure that the program correctly outputs 3!, 4!, 8!, and 9!.

#### Check-off

Show factorial.s to your TA, and run it in Venus, which should give the correct result.
