## Lab 10

[Computer Architecture I](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/index.html) [ShanghaiTech University](http://www.shanghaitech.edu.cn/)

## Objectives:

* Learn about basic OpenMP directives
* Write code to learn two ways of how `#pragma omp for` could be implemented. Learn about false sharing.
* Learn about basic multi-processing programming

## Setup

Download source code from [here](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2023/labs/lab_10.zip)

## Multi-threading programming using OpenMP

OpenMP stands for Open specification for Multi-Processing. It is a framework that offers a C interface. It is not a built-in part of the language – most OpenMP features are directives to the compiler.

Benefits of multi-threaded programming using OpenMP include:

* Very simple interface allows a programmer to separate a program into serial regions and parallel regions.
* Convenient synchronization control (Data race bugs in POSIX threads are very hard to trace).

In this lab, we will practice on basic usage of OpenMP.

### Exercise 1 - OpenMP Hello World

Consider the implementation of Hello World (hello.c):

```
int main ()
{
  #pragma omp parallel
  {
    int thread_ID = omp_get_thread_num ();
    printf (" hello world %d\n", thread_ID);
  }
  return 0;
}
```

This program will fork off the default number of threads and each thread will print out "hello world" in addition to which thread number it is. You can change the number of OpenMP threads by setting the environment variable OMP_NUM_THREADS or by using the [omp_set_num_threads](https://gcc.gnu.org/onlinedocs/libgomp/omp_005fset_005fnum_005fthreads.html) function in your program. The #pragma tells the compiler that the rest of the line is a directive, and in this case it is omp parallel. omp declares that it is for OpenMP and parallel says the following code block (what is contained in { }) can be executed in parallel. Give it a try:

```
$ make hello && ./hello
```

If you run ./hello a couple of times, you should see that the numbers are not always in numerical order and will most likely vary across runs. Think about the reason and explain to your TA.

It is also vital to note that the variable thread_ID is local to a specific thread and not shared across all threads. In general with OpenMP, variables declared inside the parallel block will be private to each thread, but variables declared outside will be global and accessible by all the threads.

```bash
 hello world 1
 hello world 15
 hello world 8
 hello world 4
 hello world 7
 hello world 13
 hello world 5
 hello world 10
 hello world 14
 hello world 12
 hello world 2
 hello world 3
 hello world 0
 hello world 6
 hello world 11
 hello world 9
```

**OpenMP的线程调度策略是动态调度（Dynamic Scheduling）。动态调度的主要特点是线程在运行时根据需要动态创建，而不是在并行区域开始时就创建所有线程。**

**在OpenMP中，线程池维护一定数量的线程，而在并行区域运行时，线程池中的线程被动态创建和分配。当一个线程完成了它的工作后，它会等待其他线程继续工作，而不是立即退出。新任务将被分配给等待线程池中的线程，以保持各线程工作负载的平衡。**

**线程调度的方式和顺序是OpenMP运行时系统自行决定的，由于调度器的实现方式可能会因时间分片、CPU核数、缓存大小和多核处理器内部的互联过程等因素而产生巨大的不同，因此需要合理地使用OpenMP的指令控制或者手动创建线程并控制线程执行次序。**

### Exercise 2 - Matrix Multiplication

Matrix multiplication is a common operation in scientific computing and machine learning. In this exercise, we will optimize a matrix multiplication implementation using OpenMP. The matrix multiplication is implemented in matmul.c.

Your task is to optimize matmul.c (speedup may plateau as the number of threads continues to increase). To aid you in this process, two useful OpenMP functions are:

* [int omp_get_num_threads()](https://gcc.gnu.org/onlinedocs/libgomp/omp_005fget_005fnum_005fthreads.html)
* [int omp_get_thread_num()](https://gcc.gnu.org/onlinedocs/libgomp/omp_005fget_005fthread_005fnum.html)

Divide up the work for each thread through two different methods (write different code for each of these methods):

1. First task,  **slicing** : have each thread handle adjacent rows: i.e. Thread 0 will compute the rows at indices i such that i % omp_get_num_threads() is 0, Thread 1 will compute the rows where i % omp_get_num_threads() is 1, etc.
2. Second task,  **chunking** : if there are N threads, break the matrices into N contiguous chunks along the first dimension (the rows), and have each thread compute the product of the chunk of matrix A and the entire matrix B.

Hints:

* Use the two functions we listed above somehow in the for loop to choose which rows each thread handles in the slicing method.
* You may need a special case to prevent going out of bounds for matmul_optimized_chunks. Don't be afraid to write one.
* Be careful about cache line alignment and false sharing. To avoid false sharing, each thread should have its own output buffer to store the computed rows.

For this exercise, we are asking you to manually split the work amongst threads since this is a common pattern used in software optimization. The designers of OpenMP actually made the #pragma omp for directive to automatically split up independent work. Here is the function rewritten using it.  **You may NOT use this directive in your solution to this exercise** .

```
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
```

Test the performance of your code with:

```
$ make matmul && ./matmul
```

#### Checkoff

* Show the modified code for both the slicing and chunking methods in matmul.c.
* Describe the performance differences between the methods you implemented and try to analyze the reason(Run more times to find a common pattern instead of just running once).
* **chunk method may result false sharing when chunking by the first dimension ,  different thread access the same cache line.**
* ```c
  # The first time
  Naive: 10.58415 seconds
  Optimized (slices): 1.54374 seconds
  Optimized (chunks): 1.97638 seconds

  # change the k loop and j loop
  Optimized (slices): 1.53934 seconds
  Optimized (chunks): 0.67424 seconds
  ```
* Explain why using OpenMP may not necessarily lead to optimal performance on a single compute node with multiple cores.
* Bonus: Implement an additional optimization and discuss its impact on performance.

**在单个计算节点上使用OpenMP不一定会导致最佳性能，主要原因有以下几点：**

1. **阿姆达尔定律：阿姆达尔定律指出，并行执行的加速比受程序中不能并行化的序列部分的限制。如果程序中的串行部分很大，则并行化带来的性能提升将受限。使用OpenMP最有效的前提是代码中有大量的并行性。**
2. **内存带宽：单个计算节点上的多个处理器共享内存带宽。如果每个处理器都具有高数据吞吐量，则多个处理器可能会竞争内存带宽，从而严重降低整个应用程序的性能。**
3. **缓存一致性：多个处理器同时访问同一内存位置会导致缓存一致性问题。OpenMP减少了对缓存的使用，因为所有线程共享内存。缓存一致性意味着必须协调线程，以便每次仅有一个线程可以修改给定的内存位置。**

### Exercise 3 - Dot Product

The next task is to compute the dot product of two vectors. At first glance, implementing this might seem not too different from v_add, but the challenge is how to sum up all of the products into the same variable (reduction). A sloppy handling of reduction may lead to  **data races** : all the threads are trying to read and write to the same address simultaneously. One solution is to use a  **critical section** . The code in a critical section can only be executed by a single thread at any given time. Thus, having a critical section naturally prevents multiple threads from reading and writing to the same data, a problem that would otherwise lead to data races. One way to avoid data races is to use the critical primitive provided by OpenMP. An implementation, dotp_naive in dotp.c, protects the sum with a critical section.

Try out the code (make dotp &&./dotp). Notice how the performance gets much worse as the number of threads goes up. By putting all of the work of reduction in a critical section, we have flattened the parallelism and made it so only one thread can do useful work at a time (not exactly the idea behind thread-level parallelism). This contention is problematic; each thread is constantly fighting for the critical section and only one is making any progress at any given time. As the number of threads goes up, so does the contention, and the performance pays the price. Can we reduce the number of times that each thread needs to use a critical section?

In this exercise, you have 2 tasks:

1. Fix the performance problem without using OpenMP's built-in Reduction keyword.
2. Fix the performance problem using OpenMP's built-in Reduction keyword. (Note that your code should no longer contain #pragma omp critical)

#### Checkoff

* Show the TA your manual fix to dotp.c that gets speedup over the single threaded case.
* Show the TA your Reduction keyword fix for dotp.c, and explain the difference in performance.

---

Zongze Li <`lizz` AT `shanghaitech.edu.cn`>

Last modified: 2023-04-26
