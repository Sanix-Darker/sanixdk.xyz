<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/the-weird-concept-of-branchless-programming/head.png" />

## THE WEIRD CONCEPT OF 'BRANCHLESS PROGRAMMING'

`2025-07-08 01:37` • 19+ min read • #c #branchless

---

## DISCLAIMERS

- If you came for readable code, sorry, branchless programming is basically code golf with caffeine.
- Code examples are going to be ran in C code.
- Drastic measures (e.g. `-O3 -ffast-math -march=native -fomit-this-entire-function`) were taken.



## 0×00 FUNKY INTRODUCTION

Modern CPUs are predictive creatures. They guess what you're about to do, like a nosy algorithm trying to sell you sneakers after you Googled "foot pain." Branch predictors make CPUs fast by speculating on branches... until they guess wrong and everything grinds to a halt for 15,20 cycles.

Branchless programming is how we get around this: we rewrite our code to **not branch at all**. Instead of jumping to conclusions, we manipulate bits like 1980s assembly gremlins.



### WHAT IS A BRANCH

What is a branch first of all?
In a program, you may observe parts like this:

```bash
if (condition), then
    ...
elif (condition), then
    ...
else
    ...
fi
```

This block of instructions is a collection of three branches. Each `if`, `elif`, and `else` represents a possible execution path the CPU can take depending on the evaluation of the conditions. At runtime, only one of these paths is taken, and the others are skipped. This choice ,  this deviation in the control flow ,  is what we call a branch.

In terms of machine code, a branch is often implemented as a jump (`jmp`, `je`, `jne`, etc.). These are instructions that tell the CPU: “if this condition holds, skip to label X; otherwise, keep going.” That jump disrupts the nice linear stream of instruction execution, forcing the CPU to guess where you’re going next.

Here’s a simple ASCII representation of how this decision tree looks:

```
          [Condition A]
              |
        +-----+------+
       Yes          No
       |             |
[Block A]     [Condition B]
                  |
            +-----+------+
           Yes          No
           |             |
      [Block B]      [Block C]
```

From a CPU perspective, each conditional check and potential jump is a “fork in the road.” If your code has a predictable pattern (e.g., always taking the same branch), the CPU can guess well and maintain performance. But if it's unpredictable, say, random data or user input, then the CPU may guess wrong, flush its pipeline, and pay a heavy penalty.

This is why branches can be so dangerous in tight loops or performance-critical code: even one mispredicted branch can cost dozens of cycles, ruining your cache-fueled dreams.


### WHY IS BRANCHLESS PROGRAMMING IMPORTANT?

Branches, when predictable, are cheap. But when unpredictable, they're evil. Imagine a tight loop that checks a condition based on data from user input, or real-world sensors, or shuffled arrays. The branch predictor stumbles, and every misstep means flushing the pipeline ,  a costly affair on modern superscalar out-of-order CPUs.

Branchless code avoids that entirely. By rewriting conditional logic into arithmetic and bit operations, or using CPU instructions like `cmov`, we let the CPU chew through code without pausing to guess. It’s smoother, faster, and often *more deterministic*, which is crucial in performance-critical or side-channel-resistant scenarios (looking at you, cryptography).

We're going to take you on a wild ride through three increasingly complex examples:

1. `abs(x)` ,  a gentle warm-up with unary fun
2. `clamp(x, min, max)` ,  a common pattern with two conditions
3. `partition()` ,  a full algorithm with data-dependent control flow

We'll compare these in **C** (our performance-hungry workhorse), we'll show you how these concepts look in both worlds, how they perform.



## 0×01 ABS: THE LOW-HANGING BRANCH

Absolute value is your first ticket to understanding how to cut down branches without cutting performance.

We want to calculate the absolute value of a signed integer without using a conditional branch. This is foundational — a single-bit operation can turn a branch into math.

```c
int abs_branch(int x) {
    return x < 0 ? -x : x;
}

// Compiles to a `cmp` and a `jge` or `jl`, depending on compiler and optimization level.

int abs_branchless(int x) {
    int mask = x >> 31;
    return (x + mask) ^ mask;
}
```


### BRANCHLESS EXPLANATION

- Arithmetic right-shift of a signed 32-bit integer by 31 yields all 0s for non-negatives, all 1s (i.e., -1) for negatives.
- Adding `mask` to `x` either increments or doesn't.
- XORing with `mask` flips bits only if `mask` is -1.

### BONUS VARIANT

```c
int abs_alt(int x) {
    int mask = x >> 31;
    return (x ^ mask) - mask;
}
```

Produces identical results; different taste of the same bit soup.

### ASSEMBLY BREAKDOWN

```asm
mov eax, edi       ; move x into eax
sar eax, 31        ; sign-extend right shift to produce mask
mov ecx, eax       ; duplicate mask
add edi, ecx       ; edi = x + mask
xor eax, edi       ; eax = result = (x + mask) ^ mask
```

Fast. No jumps. Pure ALU (arithmetic logic unit).


## 0×02 CLAMP: SQUASHING RANGES WITHOUT IFS

The clamp is more complex. You want to bound a value between a `min` and `max`.
We want to ensure a value stays within `[min, max]` without branches. This is key in physics simulations, rendering, and safety constraints.

```c
int clamp(int x, int min, int max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

int clamp_branchless(int x, int min, int max) {
    int r1 = x - ((x - min) & ((x - min) >> 31));
    return r1 - ((r1 - max) & ((r1 - max) >> 31));
}
```

### BRANCHLESS EXPLANATION

- `(x - min) >> 31` creates a mask that's all 1s if `x < min`.
- Mask is used to select `min` when necessary.
- Second line applies the same trick to limit the upper bound.

### DISASSEMBLED STRATEGY

```asm
sub eax, min
sar eax, 31       ; create mask_low
and eax, (x - min)
sub x, eax        ; x = max(x, min)

sub x, max
sar ..., 31       ; create mask_high
and ..., (x - max)
sub x, ...        ; x = min(x, max)
```

Every operation is ALU-based. No branching, just pure logic.
This removes branches by computing masks and blending values accordingly. Elegant? No. Effective? Absolutely.


## 0×03 PARTITION: QUICKLY AND DIRTY

This is where branchless logic makes the biggest splash, in algorithms that iterate over data and make conditional swaps.
Partition an array around a pivot such that all elements < pivot come before elements >= pivot, without any conditional branching in the inner loop.

```c
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

int partition_branchless(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low;
    for (int j = low; j < high; j++) {
        swap(&arr[i], &arr[j]);
        i += arr[i] < pivot;
    }
    swap(&arr[i], &arr[high]);
    return i;
}
```

### OPTIONAL BITMASK VARIANT

```c
i += ((arr[i] - pivot) >> 31) & 1;
```

Relies on arithmetic right shift and masking to conditionally increment.

### ASSEMBLY-LEVEL INSIGHT

```asm
mov eax, [arr+i*4]
cmp eax, pivot
setl bl
add i, ebx
```

Or:

```asm
cmp eax, pivot
adc i, 0
```

Clever use of `adc` (add with carry) after compare to branchlessly increment.


## BENCHMARKS BABY

```
| Operation      | Branchy | Branchless | Speedup |
| -------------- | ------- | ---------- | ------- |
| `abs(x)`       | \~5ms   | \~5ms      | 1.00x   |
| `clamp(x,m,M)` | \~6ms   | \~6ms      | 1.00x   |
| `partition()`  | \~6ms   | \~5ms      | 1.20x   |
```

### SUMMARY

* `abs()` and `clamp()` show negligible gains; branch prediction handles them well.
* `partition()` shows improvement due to high branch unpredictability.


## APPENDIX: FULL SOURCE CODE FOR C BENCHMARKS

See appendix below for full C benchmark code, compilation flags, and timing logic.

<details><summary>Click to expand the whole weird C code.</summary>

> Warning: long code ahead. Skip if you value your retina and you have a life.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ABS branchy and branchless
int abs_branch(int x) {
    return x < 0 ? -x : x;
}

int abs_branchless(int x) {
    int mask = x >> 31;
    return (x + mask) ^ mask;
}

// CLAMP branchy and branchless
int clamp(int x, int min, int max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

int clamp_branchless(int x, int min, int max) {
    int r1 = x - ((x - min) & ((x - min) >> 31));
    return r1 - ((r1 - max) & ((r1 - max) >> 31));
}

// PARTITION branchy and branchless
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

int partition_branchless(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low;
    for (int j = low; j < high; j++) {
        swap(&arr[i], &arr[j]);
        i += arr[i] < pivot;
    }
    swap(&arr[i], &arr[high]);
    return i;
}

// Benchmarking helpers
void benchmark_abs(int* data, int count) {
    clock_t start = clock();
    volatile long long sum = 0;
    for (int i = 0; i < count; ++i) sum += abs_branch(data[i]);
    printf("ABS (branch):     %.3f sec\n", (double)(clock() - start)/CLOCKS_PER_SEC);

    start = clock(); sum = 0;
    for (int i = 0; i < count; ++i) sum += abs_branchless(data[i]);
    printf("ABS (branchless): %.3f sec\n", (double)(clock() - start)/CLOCKS_PER_SEC);
}

void benchmark_clamp(int* data, int count) {
    clock_t start = clock();
    volatile long long sum = 0;
    for (int i = 0; i < count; ++i) sum += clamp(data[i], -50, 50);
    printf("CLAMP (branch):     %.3f sec\n", (double)(clock() - start)/CLOCKS_PER_SEC);

    start = clock(); sum = 0;
    for (int i = 0; i < count; ++i) sum += clamp_branchless(data[i], -50, 50);
    printf("CLAMP (branchless): %.3f sec\n", (double)(clock() - start)/CLOCKS_PER_SEC);
}

void benchmark_partition(int* data, int count) {
    int* copy = malloc(sizeof(int) * count);
    memcpy(copy, data, sizeof(int) * count);

    clock_t start = clock();
    partition(data, 0, count - 1);
    printf("PARTITION (branch):     %.3f sec\n", (double)(clock() - start)/CLOCKS_PER_SEC);

    memcpy(data, copy, sizeof(int) * count);
    start = clock();
    partition_branchless(data, 0, count - 1);
    printf("PARTITION (branchless): %.3f sec\n", (double)(clock() - start)/CLOCKS_PER_SEC);

    free(copy);
}

int main() {
    const int N = 10000000;
    int* data = malloc(sizeof(int) * N);

    // Populate with mixed signed integers
    for (int i = 0; i < N; ++i)
        data[i] = rand() - (RAND_MAX / 2);

    puts("== Benchmarking ABS ==");
    benchmark_abs(data, N);

    puts("\n== Benchmarking CLAMP ==");
    benchmark_clamp(data, N);

    puts("\n== Benchmarking PARTITION ==");
    benchmark_partition(data, 1000000); // smaller size due to O(n log n) behavior

    free(data);
    return 0;
}
```

(If the scroll wheel starts smoking, you’ve found the end.)

</details>



## FINAL THOUGHTS: SHOULD YOU BRANCHLESS?


### WHEN YOU SHOULD

* Hot loops with unpredictable conditions
* Cryptographic or timing-sensitive code
* SIMD/vectorization where branching kills performance

### WHEN YOU SHOULDN’T

* Code clarity matters more than nanoseconds
* Predictable branches dominate (e.g., error handling)

Branchless programming is a scalpel, not a sledgehammer. Used wisely, it can make your code faster, safer, and cooler. Misused, it turns your logic into incomprehensible bit spaghetti.


> *"Premature optimization is the root of all evil — except when it's branchless, then it's performance art."*



-----------
