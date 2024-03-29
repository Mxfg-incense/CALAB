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

## Lab 4

[Computer Architecture I](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/) [ShanghaiTech University](http://www.shanghaitech.edu.cn/)

## Goals

These exercises are intended to give you more practice with

* Download source code from [here](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/New%20Lab4/New%20Lab4.zip)
* calling conventions, including prologue and epilogue.
* function calls and manipulating pointers in RISC-V.
* implementing extended bitwise operations based on existing instructions.

## Exercises

### Exercise 1: calling convention checker

This exercise uses the file `ex1.s`.

A quick recap of RISC-V calling conventions: all functions that overwrite registers that are preserved by convention must have a prologue, which saves those register values to the stack at the start of the function, and an epilogue, which restores those values for the function's caller. You can find a more detailed explanation along with some concrete examples in [these notes](https://inst.eecs.berkeley.edu/~cs61c/resources/RISCV_Calling_Convention.pdf).

Bugs due to calling convention violations can often be difficult to find manually, so Venus provides a way to automatically report some of these errors at runtime.

Take a look at the contents of the `ex1.s` file, particularly at the `main`, `simple_fn`, `naive_pow`, `inc_arr`, and `helper_fn` functions. Enable the CC checker in Venus-Settings-Calling Convention, then run the program in the simulator. You should see an output similar to the following:

```
[CC Violation]: (PC=0x00000080) Usage of unset register t0! editor.S:58 mv a0, t0
[CC Violation]: (PC=0x0000008C) Setting of a saved register (s0) which has not been saved! editor.S:80 li s0, 1
[CC Violation]: (PC=0x00000094) Setting of a saved register (s0) which has not been saved! editor.S:83 mul s0, s0, a0
......
```

Find the source of each of the errors reported by the CC checker and fix it. You can find a list of CC error messages, as well as their meanings, in the [Venus reference](https://inst.eecs.berkeley.edu/~cs61c/su21/resources/venus-reference/#calling-convention-checker).

Once you've fixed all the violations reported by the CC checker, the code might still fail: this is likely because there's still some remaining calling convention errors that Venus doesn't report. Since function calls in assembly language are ultimately just jumps, Venus can't report these violations without more information, at risk of producing false positives.

The fixes for all of these errors (both the ones reported by the CC checker and the ones it can't find) should be added near the lines marked by the `FIXME` comments in the starter code.

**Note:** Venus's calling convention checker will not report all calling convention bugs; it is intended to be used primarily as a sanity check. Most importantly, it will only look for bugs in functions that are exported with the `.globl` directive - the meaning of `.globl` is explained in more detail in the [Venus reference](https://inst.eecs.berkeley.edu/~cs61c/su21/resources/venus-reference/#working-with-multiple-files).

### Action Item

Resolve all the calling convention errors in `ex1.s`, and be able to answer the following questions:

* What caused the errors in `simple_fn`, `naive_pow`, and `inc_arr` that were reported by the Venus CC checker?

  `simple_fn`: Use the uninitialized register.
  `naive_pow`: miss prologue and epilogue.
  `inc_arr`: Forget save `s0` and `s1`, and `t0` need to be caller saved.
* In RISC-V, we call functions by jumping to them and storing the return address in the `ra` register. Does calling convention apply to the jumps to the `naive_pow_loop` or `naive_pow_end` labels?

  No. Three function are executed sequentailly.
* Why do we need to store `ra` in the prologue for `inc_arr`, but not in any other function?

  Since `jal helper_fn` would overwrite the `ra`, and `helper_fn` would not call any other functions.
* Why wasn't the calling convention error in `helper_fn` reported by the CC checker? (Hint: it's mentioned above in the exercise instructions.)

  it will only look for bugs in functions that are exported with the .globl directive

### Testing

After fixing the errors with `FIXME` in `ex1.s`, run Venus locally with the command from the beginning of this exercise to make sure the behavior of the functions hasn't changed and that you've remedied all calling convention violations.

Once you have fixed everything, running the above Venus command should output the following:

```
Sanity checks passed! Make sure there are no CC violations.
Found 0 warnings!
```

#### Checkoff

* Show your TA your code and its test run.
* Provide answers to the questions.

### Exercise 2: RISC-V function calling with `map`

This exercise uses the file `ex2.s`.

In this exercise, you will complete an implementation of [`map`](https://en.wikipedia.org/wiki/Map_(higher-order_function)) on linked-lists in RISC-V. Our function will be simplified to mutate the list in-place, rather than creating and returning a new list with the modified values.

You will find it helpful to refer to the [RISC-V green card](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/lecture_notes/riscvcard.pdf) to complete this exercise. If you encounter any instructions or pseudo-instructions you are unfamiliar with, use this as a resource.

Our `map` procedure will take two parameters; the first parameter will be the address of the head node of a singly-linked list whose values are 32-bit integers. So, in C, the structure would be defined as:

```
struct node {
    int value;
    struct node *next;
};
```

Our second parameter will be the **address of a function** that takes one `int` as an argument and returns an `int`. We'll use the `jalr` RISC-V instruction to call this function on the list node values.

Our `map` function will recursively go down the list, applying the function to each value of the list and storing the value returned in that corresponding node. In C, the function would be something like this:

```
void map(struct node *head, int (*f)(int))
{
    if (!head) { return; }
    head->value = f(head->value);
    map(head->next,f);
}
```

If you haven't seen the `int (*f)(int)` kind of declaration before, don't worry too much about it. Basically it means that `f` is a pointer to a function that takes an `int` as an argument. We can call this function `f` just like any other.

There are exactly ten (10) markers (1 in `done`, 7 in `map`, and 2 in `main`) in the provided code where it says `TODO: YOUR CODE HERE`.

### Action Item

Complete the implementation of `map` by filling out each of these ten markers with the appropriate code. Furthermore, provide a call to `map` with `square` as the function argument. There are comments in the code that explain what should be accomplished at each marker. When you've filled in these instructions, running the code should provide you with the following output:

```
9 8 7 6 5 4 3 2 1 0 
81 64 49 36 25 16 9 4 1 0 
80 63 48 35 24 15 8 3 0 -1 
```

The first line is the original list, and the second line is the list with all elements squared after calling `map(head, &square)`, and the third is the list with all elements decremented after now calling `map(head, &decrement)`.

#### Checkoff

* Show your TA your test run.

### Exercise 3: implementing bitwise reverse operation in two ways

This exercise uses the file `ex3.s`.

Write two versions of a bitwise reverse function that, given a value in `a0`, returns the value in `a0` with the bits reversed, e.g. 01011101 to 10111010.

* In the first version `bitrev1`, a loop is required to generate the reversed value bit by bit.
* In the second version `bitrev2`, only use `li`, `and`, `or`, `slli`, `srli` instructions. Any branch and jump instructions are not allowed (except the last `ret`), and manually expanding the loop in `bitrev1` is also not allowed.
  The solution is a little tricky. You can think of this solution as a kind of divide and conquer. The input is `k` binary bits, and the output is the reversed bits.

  ```
  Algorithm bitwise-reverse( b[0:k] )
      if k==1:
          return the single bit
      b[0:k/2] = bitwise-reverse( b[0:k/2] )
      b[k/2:k] = bitwise-reverse( b[k/2:k] )
      swap(b[0:k/2], b[k/2:k])
      return b[0:k]
  ```

  However, you are not going to implement this recursive algorithm. By using `li` to load several specific constants and `and`, `or`, `slli`, `srli` to modify the bits, the whole procedure can be parallelized. Here is the example:

  ```
  0101-1101
  1101-0101        # step1, k=8

  11-01 01-01
  01-11 01-01      # step2, k=4, parallelized

  0-1 1-1 0-1 0-1
  1-0 1-1 1-0 1-0  # step3, k=2, parallelized
  ```

When you've finished the two functions, running the code should provide you with the following output:

```
0x12345678 0x1E6A2C48 0x1E6A2C48 0x1E6A2C48 
0x71C924BF 0xFD24938E 0xFD24938E 0xFD24938E 
0x19260817 0xE8106498 0xE8106498 0xE8106498 
```

#### Checkoff

* Show your TA both versions of the function and its test run.

## Lab 7

[Computer Architecture I](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/) [ShanghaiTech University](http://www.shanghaitech.edu.cn/)
[Lab 6](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab6/lab6.html) Lab 7## PipeLine

## Setup

* Download source code from [here](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab7/lab07.tar)

### Shift and Add Multiplier

Here we review a simplest implementation of Multiplier: Shift and Add Multiplier. The block diagram is shown below:

![](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab7/picture/block_diagram.png)

In this practice, we assume the multiplier and multiplicand are 4-bit unsigned integers, and the output is an 8-bit unsigned integer. Hence the the whole multiplier can be flattened to a combined logic.

### Practice

#### 1. non_pipelined 4-bit multiplier

We're going to finish a 4-bit multiplier.

#### Action Item

Complete the following steps to create the splitter circuit, and show this to your TA (remember to save). When you've completed the circuit, answer the question in the checkoff session.

1. Open up the Exercise 1 schematic (`File->Open->ex1.circ`) and go to the `move_and_add` circuit.
2. Connect all Gates, Plexers, and Arithmetic Blocks to implement a single shift and add step. Once you finished, the circuit should correctly decide whether to add or not to add the input multiplicand on the current result, and output the shifted multiplicand as well.
3. Now refer to the `non-pipelined` circuit. Play with the inputs to see if your implementation is correct, and adjust your design if necessary.
4. Let the propagation delay of an adder block be 45ns, the propagation delay of MUX be 20ns, and the propagation delay of a shifters block (since we have a const offset, it is very efficient) be 5ns. The register has a CLK-to-Q delay of 10ns, setup time of 10ns, and hold time of 5ns. Calculate the maximum clock rate at which this circuit can operate. Assume that both inputs come from clocked registers that receive their data from an outside source.

$$
T = t_{clk-to-q} + t_{combination}+t_{setup} = 10 + 4(20+45) + 10 = 280ns
$$

#### [Checkoff]()

* Show your TA the calculations you performed to find the maximum clock rate (non-pipelined).

### 2. Pipe that line

Recalling common operations in ALU, the shift addition multiplier we implemented is significantly inefficient compared to other operations, which reduces the latency performance of ALU. Try splitting the implementation of the shift addition multiplier into two stages to maximize the maximum clock cycle.

In order to check that your pipelining still produces correct outputs, we will consider the outputs from the circuit "correct" if and only if it corresponds to the sequence of outputs the non-pipelined version would emit, bar some leading zeroes. For example, if for some sequence of inputs, the non-pipelined version emits the sequence [3, 5, 1, 2, 4, …]. Then, the correct pipelined circuit might emit the sequence [0, 3, 5, 1, 2, 4, …] for the same sequence of inputs. You can check this by simulating the circuit (using the "Simulate" menu dropdown) and either ticking the clock manually or enabling continuous ticks.

Note that in order to pipeline the circuit, we need a register to hold the intermediate value of the computation between pipeline stages. This is a general theme with pipelines.

#### Action Item

* Complete the sub-circuit pipelined. You will need to add a register to divide the multiplication and addition stages up.
* Calculate the maximum clock rate for the pipelined version of the circuit that you have created.
  $$
  T = t_{clk-to-q} + t_{combination}+t_{setup} = 10 + 2(20+45) + 10 = 150ns
  $$

#### [Checkoff]()

* Show your TA the completed, pipelined circuit.
* Show your TA the calculations you performed to find the maximum clock rate (pipelined).

#### Futher thinking (Optional)

* Is there any better implementation for Adder and Multiplier?
* The pipeline multiplier we implemented requires more than one clock cycle to complete calculations. Do you know any instructions having similar situations in modern CPUs? How are modern CPUs compatible with this situation?

## Lab 8

Computer Architecture I ShanghaiTech UniversityLab 7 Lab 8 Lab 9## Exercise 1: Cache Visualization

Caches is typically one of the hardest topics for students in Computer Architecture to grasp at first. This exercise will use some cool cache visualization tools in Venus to get you more familiar with cache behavior and performance terminology with the help of the file [Cache.S](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab8/Lab%208%20-%20Computer%20Architecture%20I%20-%20ShanghaiTech%20University_files/cache.S.txt). At this point, read through cache.s to get a rough idea of what the program does.

To get started with each of the scenarios below:

1. Copy the code in cache.s to Venus.
2. In the code for cache.s, set the appropriate Program Parameters as indicated at the beginning of each scenario (by changing the immediates of the commented li instructions in main).
3. Click Simulator, in right partition, there is a tag called Cache.
4. Set the appropriate Cache Parameters as indicated at the beginning of each scenario.
5. Click Simulator-->Assemble & Simulate from Editor.
6. Click Simulator-->Step and see the cache states.

Get familiar with the parameters in cache windows:

1. Cache Levels: The number of layers your cache simulator will have. We will only use L1 cache in this lab, but you can play with it to learn more.
2. Block Size: Every block's size, should be a power of 2, take a quick review of the lecture content, the number of offset should be decided by block size.
3. Number of Blocks: How many blocks your cache have totally. Attention, the number is the total number, no matter how many ways you choose, therefore, if you want to satisfy the requirement, please take care of it (divide associativity).
4. Associativity: The number of ways, only if you select the N-way Set Associative can you change this value.
5. Cache size: The result of block size multiply number of blocks. You cannot change it.

The Data Cache Simulator will show the state of your data cache. Please remember that these are running with your code, so if you reset your code, it will also reset your cache and memory status.

If you run the code all at once, you will get the final state of the cache and hit rate. You will probably benefit the most from setting breakpoints at each memory access to see exactly where the hits and misses are coming from. Themethod to set a breakpoint in Venus is just click the corresponding line in the simulator, if the line become red, that means your program will stop when the execution meets that line.

Simulate the following scenarios and record the final cache hit rates. Try to reason out what the hit rate will be BEFORE running the code. After running each simulation, make sure you understand WHY you see what you see (the TAs will be asking)!

**Do not hesitate to ask questions if you feel confused! This is perfectly normal and the TA is there to help you out!**

Good questions to ask yourself as you do these exercises:

* How big is your cache block? How many consecutive accesses fit within a single block?
* How big is your cache? How many jumps do you need to make before you "wrap around?"
* What is your cache's associativity? Where can a particular block fit?
* Have you accessed this piece of data before? If so, is it still in the cache or not?

### Scenario 1:

**Cache Parameters:**

* **Cache Levels:** 1
* **Block Size (Bytes):** 8
* **Number of blocks:** 4
* **Associativity:** 1 (Venus won't let you change this, why?)
  *Directed Mapping*
* **Cache Size (Bytes):** 32 (Why?)
  *#block * block size * associativity = 32*
* **Placement Policy:** Direct Mapping
* **Block Replacement Policy:** LRU
* **Enable current selected level of the cache.**

**Program Parameters:**

* **Array Size:** 128
* **Step Size:** 8
* **Rep Count:** 4
* **Option:** 0

Checkoff

1. What combination of parameters is producing the hit rate you observe? (Hint: Your answer should be the process of your calculation.)
2. What is our hit rate if we increase Rep Count arbitrarily? Why?
   0.  index: 2 offset:3
3. How could we modify our program parameters to maximize our hit rate?
   **Step Size**: 1 **Option**: 1

### Scenario 2:

**Cache Parameters:**

* **Cache Levels:** 1
* **Block Size (Bytes):** 16
* **Number of blocks:** 16
* **Associativity:** 4
* **Cache Size (Bytes):** 256
* **Placement Policy:** N-Way Set Associative
* **Block Replacement Policy:** LRU
* **Enable current selected level of the cache.**

**Program Parameters:**

* **Array Size:** 256
* **Step Size:** 2
* **Rep Count:** 1
* **Option:** 1

Checkoff

1. What combination of parameters is producing the hit rate you observe? (Hint: Your answer should be the process of your calculation.)
   **offset: 4 Set Index:  2 Associativity: 4 ** 3/4
2. What happens to our hit rate as Rep Count goes to infinity? Why?
   1
3. Suppose we have a program that uses a very large array and during each Rep, we apply a different operator to the elements of our array (e.g. if Rep Count = 1024, we apply 1024 different operations to each of the array elements). How can we restructure our program to achieve a hit rate like that achieved in this scenario? (Assume that the number of operations we apply to each element is very large and that the result for each element can be computed independently of the other elements.) What is this technique called? (Hint)
   `Cache Blocking`
   ```C
   #                  // repeat repcount times
   #   // Step through the selected array segment with the given step size.
   #       for (index1 = 0; index1 < arraysize; index1 += Block) 
   		for (k = 0; k < repcount; k++)  {
   #	    for(index = index1, index < index1 + Block ; index ++)
   #           if(option==0)
   #               array[index] = 0;                   // Option 0: One cache access - write
   #           else
   #               array[index] = array[index] + 1;    // Option 1: Two cache accesses - read AND write
   #       }
   #   }
   ```

### Scenario 3:

**Cache Parameters:**

* **Cache Levels:** 1
* **Block Size (Bytes):** 16
* **Number of blocks:** 16
* **Associativity:** 4
* **Cache Size (Bytes):** 256
* **Placement Policy:** N-Way Set Associative
* **Block Replacement Policy:** Random
* **Enable current selected level of the cache.**

**Program Parameters:**

* **Array Size:** 256
* **Step Size:** 8
* **Rep Count:** 2
* **Option:** 0

Checkoff

1. Run the simulation a few times. Every time, set a different seed value (bottom of the cache window). Note that the hit rate is non-deterministic. What is the range of its hit rate? Why is this the case? ("The cache eviction is random" is not a sufficient answer)
   **<0.5**
2. Which Cache parameter can you modify in order to get a constant hit rate? Record the parameter and its value (and be prepared to show your TA a few runs of the simulation). How does this parameter allow us to get a constant hit rate? And explain why the constant hit rate value is that value.
   Rep Count: 1
3. Ensure that you thoroughly understand each answer. Your TA may ask for additional explanations.

## Exercise 2: Matrix multiplication and Execution order

#### Gaussian Blur On Image

In image processing, a Gaussian blur (also known as Gaussian smoothing) is the result of blurring an image by a Gaussian function. Mathematically, applying a Gaussian blur to an image is the same as convolving the image with a Gaussian function. In this lab, we adopt a 1-dimensional Gaussian distribution kernel, and the blurring process is done in two steps: Given image A as our input, we first convolve the kernel over the rows of image A to produce a horizontally blurred image B. We then convolve the kernel over the columns of image B to produce a horizontally and vertically blurred image C. The image C is our final blurred image

The process of convolving a image works like below. It consists a simple multiplication and add.

![](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab8/Lab%208%20-%20Computer%20Architecture%20I%20-%20ShanghaiTech%20University_files/gaussian-blur.png)

We provide an implement of Gaussian Blur [here](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab8/Lab%208%20-%20Computer%20Architecture%20I%20-%20ShanghaiTech%20University_files/exe2_template.tar) and your job is to optimize the program without changing the algorithm. To make things easy, you only need to focus on apply_gb_fast.c.

In apply_gb_fast.c, there is a function called apply_gb(). This function will receive two parameters, where Image a indicates the input image and FVec gv indicates the kernel. It will call gb_h and gb_v to do convolvtion horizontally and vertically. gb_h and gb_v will return a new image.

At first, you can use make base_test to run the origin version of gaussian blur, which will show you the time of gb_h and gh_v. Then, you will find there is a gap between the two time.

Then, to optimize the program, we can take another look on the execution order of Gaussian Blur. The vertial onvolvtion equals to apply horizontal convolvtion to a transposed matrix. Thus, we can first transpose the image , apply horizontal concolvtion to it and finally transpose it angin to get a correct result. In this way, we can optimize the memory access performance of the program.

In apply_gb_fast.c, there is a completed function transpose(), which will return a transposed image of the input image. You can use it to optimize your program following the method mentioned above.

You can run make all to test your modified program. The program test_accuracy will test the result of your program and output the average error between your result and the correct result.

#### (Optional)

To make the program even faster, we can apply **cache blocking** to the function transpose(), which can be learned from [here](https://www.intel.com/content/www/us/en/developer/articles/technical/cache-blocking-techniques.html)

**Checkoff: **Show your program to your TA and answer the following questions:

1. Why there is a gap between gb_v and gb_h ?
   `img.data+img.numChannels* (y*img.dimX+x)`
   in gb_h cache access is far
2. Why the changed execution order will achieve a better performance even if we do more things(transpose)?

## Exercise 3: Effort of Cache Miss

### Cache Friendly Data Structure

Some data structures are cache friendly while others will cause a lot of cache miss. For those programs whose workloads are mainly in data access instead of calculation, cache miss will influence to performance significantly.

### Demo Web Log Engine

Every time there is someone who visits our website, the website log engine will record some information such as ip and state. The website log engine will do some operations on the recorded logs, where the main work is accessing data. To simplify the situation, we provide a [demo](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab8/Lab%208%20-%20Computer%20Architecture%20I%20-%20ShanghaiTech%20University_files/exe3_template.tar) web log engine which will tranverse all logs and do map function to some information

Your task is to modify the given struct log_entry in log_fast.c to make the data structure more cache friendly.You can use the following command to test your program's performance compared with the origin one.(If you use a virtual machine, you may need to increase the memory if there comes a sigment fault)

```
$ make all
```

#### Hint

In the function tranverse(), we only use three members in the log_entry. However, the three members are seperated be the large arries, which make them placed into three differnet cache lines. Thus, each access of one element in the array logs will cause 3 cache misses.

**Checkoff: **Show your result to your TA and explain why you do this modification.

---

Linjie Ma <`malj` AT `shanghaitech.edu.cn`>Modeled after UC Berkeley's CS61C.Last modified: 2022-04-5


# CS110 Lab 9

Download the starter code [here](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab9/lab9.tar)## Introduction to SIMD

[SIMD](https://en.wikipedia.org/wiki/Single_instruction,_multiple_data) makes a program faster by executing the same instruction on multiple data at the same time. In this lab, we will use [Intel Intrinsics](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html) to implement simple programs.## Part 1: Vector addition

In this part, you will implement a vector addition program using SIMD. Please "translate" naive_add() to simd_add().
You may use the following intrinsics, search in the [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html):* _mm_loadu_si128

* _mm_storeu_si128
* _mm_add_epi32

Try to tell the difference of the following "load" intrisics:* _mm_load_si128

* _mm_loadu_si128
* _mm_load_pd
* _mm_load1_pd

这些 intrinsics 都是针对 SSE 指令集的，主要用于优化处理一些涉及大量浮点运算的应用，例如图形图像处理、声音处理等。在处理这些类型应用时，使用 SSE 指令可以提高计算速度和效率


---

**_mm_loadu_si128 函数是用于加载一个 128 位（16 字节）无序的数据存储到一个 __m128i 类型的变量中。**

**_mm_storeu_si128 函数是用于将一个 __m128i 类型的数据存储到一个 128 位（16 字节）未对齐的内存地址中。**

**_mm_add_epi32 函数是执行两个 __m128i 变量的 32 位整数相加，返回一个 128 位的结果。**

**mm_load_si128 用于加载一个 128 位的有序的数据存储到一个 __m128i 变量中。这个内存地址必须是 16 字节对齐的。**

**_mm_loadu_si128 用于加载一个 128 位的无序的数据存储到一个 __m128i 变量中。这个内存地址可以是任意对齐的。**

**_mm_load_pd 用于加载两个双精度浮点数（128 位的前后 64 位）到一个 __m128d 变量中。这个内存地址必须是 16 字节对齐的。**

**_mm_load1_pd 用于重复加载一个双精度浮点数到一个 __m128d 变量中的两个位置。这个内存地址必须是 8 字节对齐的。**

**这些函数的选择要根据加载数据存储的内存地址对齐方式来决定。有对齐限制的指令执行速度快于没有对齐限制的指令**

**_mm_setzero_ps 用于创建一个全 0 的 128 位浮点数变量 __m128。**

**_mm_set1_ps 用于创建一个 128 位浮点数变量 __m128，并将其所有 4 个单精度浮点数元素初始化为同一个值。**

**_mm_loadu_ps 用于加载一个 128 位的无序的数据存储到一个 __m128 变量中。这个内存地址可以是任意对齐的。**

**_mm_add_ps 用于执行两个 __m128 变量的单精度浮点数相加，并返回一个 128 位的结果。**

**_mm_mul_ps 用于执行两个 __m128 变量的单精度浮点数相乘，并返回一个 128 位的结果。**

**_mm_storeu_ps 用于将一个 __m128 类型的数据存储到一个 128 位（16 字节）未对齐的内存地址中。**

## Part 2: Matrix multiplication

In this part, you will implement a matrix multiplication program using SIMD. Please "translate" naive_matmul() to simd_matmul().
You may use the following intrinsics:* _mm_setzero_ps

* _mm_set1_ps
* _mm_loadu_ps
* _mm_add_ps
* _mm_mul_ps
* _mm_storeu_ps

Explain why this makes the program faster.## Part 3: Loop unrolling

Read Wikipedia and try to understand the concept of loop unrolling:* [Loop unrolling](https://en.wikipedia.org/wiki/Loop_unrolling)

Implement loop_unroll_matmul() and loop_unroll_simd_matmul(), explain the performance boost they brought.

## Part 4: Compiler optimization

Run `make test`, explain why `-O3` makes the program much faster.

---

Running part2_3.c with -O0
naive: 12.565695
simd: 4.857591
unroll: 8.165401
unroll+simd: 3.864877
Running part2_3.c with -O3
naive: 1.132854
simd: 1.227327
unroll: 0.389392

---




For checkup: Put this piece of code into [godbolt.org](https://godbolt.org/) , compile them with a risc-v compiler, and tell the difference between `-O0` and `-O3`.

```
int a = 0;

void modify(int j) {
    a += j;
}


int main() {
    for (int i = 0; i < 1000; i++) {
        a += 1;
    }

    for (int i = 0; i < 1000; i++) {
        a += i;
    }

    return a;
}
```


```
a:
        .zero   4
modify:
        addi    sp,sp,-32
        sd      s0,24(sp)
        addi    s0,sp,32
        mv      a5,a0
        sw      a5,-20(s0)
        lui     a5,%hi(a)
        lw      a5,%lo(a)(a5)
        lw      a4,-20(s0)
        addw    a5,a4,a5
        sext.w  a4,a5
        lui     a5,%hi(a)
        sw      a4,%lo(a)(a5)
        nop
        ld      s0,24(sp)
        addi    sp,sp,32
        jr      ra
main:
        addi    sp,sp,-32
        sd      s0,24(sp)
        addi    s0,sp,32
        sw      zero,-20(s0)
        j       .L3
.L4:
        lui     a5,%hi(a)
        lw      a5,%lo(a)(a5)
        addiw   a5,a5,1
        sext.w  a4,a5
        lui     a5,%hi(a)
        sw      a4,%lo(a)(a5)
        lw      a5,-20(s0)
        addiw   a5,a5,1
        sw      a5,-20(s0)
.L3:
        lw      a5,-20(s0)
        sext.w  a4,a5
        li      a5,999
        ble     a4,a5,.L4
        sw      zero,-24(s0)
        j       .L5
.L6:
        lui     a5,%hi(a)
        lw      a5,%lo(a)(a5)
        lw      a4,-24(s0)
        addw    a5,a4,a5
        sext.w  a4,a5
        lui     a5,%hi(a)
        sw      a4,%lo(a)(a5)
        lw      a5,-24(s0)
        addiw   a5,a5,1
        sw      a5,-24(s0)
.L5:
        lw      a5,-24(s0)
        sext.w  a4,a5
        li      a5,999
        ble     a4,a5,.L6
        lui     a5,%hi(a)
        lw      a5,%lo(a)(a5)
        mv      a0,a5
        ld      s0,24(sp)
        addi    sp,sp,32
        jr      ra
```

---

Suting Chen <`chenst` AT `shanghaitech.edu.cn`>

Last modified: 2023-04-12


Lab 10
Computer Architecture I ShanghaiTech University
Objectives:
Learn about basic OpenMP directives
Write code to learn two ways of how #pragma omp for could be implemented. Learn about false sharing.
Learn about basic multi-processing programming
Setup
Download source code from here

Multi-threading programming using OpenMP
OpenMP stands for Open specification for Multi-Processing. It is a framework that offers a C interface. It is not a built-in part of the language – most OpenMP features are directives to the compiler.

Benefits of multi-threaded programming using OpenMP include:

Very simple interface allows a programmer to separate a program into serial regions and parallel regions.
Convenient synchronization control (Data race bugs in POSIX threads are very hard to trace).
In this lab, we will practice on basic usage of OpenMP.

Exercise 1 - OpenMP Hello World
Consider the implementation of Hello World (hello.c):

int main ()
{
  #pragma omp parallel
  {
    int thread_ID = omp_get_thread_num ();
    printf (" hello world %d\n", thread_ID);
  }
  return 0;
}
This program will fork off the default number of threads and each thread will print out "hello world" in addition to which thread number it is. You can change the number of OpenMP threads by setting the environment variable OMP_NUM_THREADS or by using the omp_set_num_threads function in your program. The #pragma tells the compiler that the rest of the line is a directive, and in this case it is omp parallel. omp declares that it is for OpenMP and parallel says the following code block (what is contained in { }) can be executed in parallel. Give it a try:

$ make hello && ./hello
If you run ./hello a couple of times, you should see that the numbers are not always in numerical order and will most likely vary across runs. Think about the reason and explain to your TA.

It is also vital to note that the variable thread_ID is local to a specific thread and not shared across all threads. In general with OpenMP, variables declared inside the parallel block will be private to each thread, but variables declared outside will be global and accessible by all the threads.

Exercise 2 - Matrix Multiplication
Matrix multiplication is a common operation in scientific computing and machine learning. In this exercise, we will optimize a matrix multiplication implementation using OpenMP. The matrix multiplication is implemented in matmul.c.

Your task is to optimize matmul.c (speedup may plateau as the number of threads continues to increase). To aid you in this process, two useful OpenMP functions are:

int omp_get_num_threads()
int omp_get_thread_num()
Divide up the work for each thread through two different methods (write different code for each of these methods):

First task, slicing: have each thread handle adjacent rows: i.e. Thread 0 will compute the rows at indices i such that i % omp_get_num_threads() is 0, Thread 1 will compute the rows where i % omp_get_num_threads() is 1, etc.
Second task, chunking: if there are N threads, break the matrices into N contiguous chunks along the first dimension (the rows), and have each thread compute the product of the chunk of matrix A and the entire matrix B.
Hints:

Use the two functions we listed above somehow in the for loop to choose which rows each thread handles in the slicing method.
You may need a special case to prevent going out of bounds for matmul_optimized_chunks. Don't be afraid to write one.
Be careful about cache line alignment and false sharing. To avoid false sharing, each thread should have its own output buffer to store the computed rows.
For this exercise, we are asking you to manually split the work amongst threads since this is a common pattern used in software optimization. The designers of OpenMP actually made the #pragma omp for directive to automatically split up independent work. Here is the function rewritten using it. You may NOT use this directive in your solution to this exercise.

void matmul (double *a, double *b, double *c)
{
  #pragma omp parallel for 
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      for (int k = 0; k < MATRIX_SIZE; k++) {
        c[i * MATRIX_SIZE + j] += a[i * MATRIX_SIZE + k] * b[k * MATRIX_SIZE + j];
      }
    }
  }
}
Test the performance of your code with:

$ make matmul && ./matmul
Checkoff
Show the modified code for both the slicing and chunking methods in matmul.c.
Describe the performance differences between the methods you implemented and try to analyze the reason(Run more times to find a common pattern instead of just running once).
Explain why using OpenMP may not necessarily lead to optimal performance on a single compute node with multiple cores.
Bonus: Implement an additional optimization and discuss its impact on performance.
Exercise 3 - Dot Product
The next task is to compute the dot product of two vectors. At first glance, implementing this might seem not too different from v_add, but the challenge is how to sum up all of the products into the same variable (reduction). A sloppy handling of reduction may lead to data races: all the threads are trying to read and write to the same address simultaneously. One solution is to use a critical section. The code in a critical section can only be executed by a single thread at any given time. Thus, having a critical section naturally prevents multiple threads from reading and writing to the same data, a problem that would otherwise lead to data races. One way to avoid data races is to use the critical primitive provided by OpenMP. An implementation, dotp_naive in dotp.c, protects the sum with a critical section.

Try out the code (make dotp &&./dotp). Notice how the performance gets much worse as the number of threads goes up. By putting all of the work of reduction in a critical section, we have flattened the parallelism and made it so only one thread can do useful work at a time (not exactly the idea behind thread-level parallelism). This contention is problematic; each thread is constantly fighting for the critical section and only one is making any progress at any given time. As the number of threads goes up, so does the contention, and the performance pays the price. Can we reduce the number of times that each thread needs to use a critical section?

In this exercise, you have 2 tasks:

Fix the performance problem without using OpenMP's built-in Reduction keyword.
Fix the performance problem using OpenMP's built-in Reduction keyword. (Note that your code should no longer contain #pragma omp critical)
Checkoff
Show the TA your manual fix to dotp.c that gets speedup over the single threaded case.
Show the TA your Reduction keyword fix for dotp.c, and explain the difference in performance.
Zongze Li <lizz AT shanghaitech.edu.cn>

Last modified: 2023-04-26
