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
