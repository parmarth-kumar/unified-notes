# Sorting Algorithms — First-Principles Conceptual Theory

> **Module:** `unified-notes/DSA/Sorting/theory.md`
> **Philosophy:** Learn from the ground up. Plain-English intuition first, concrete examples second, formal terms third, clean C++ code fourth, intuitive complexity fifth, and quick checks sixth.
> **How to study:** Read one algorithm at a time. Trace it on paper, write its code, and explain why it works in your own words. You do not need advanced English.

---

## 3-Level Learning Framework

To keep your study efficient and prevent cognitive overload, this guide is divided into three clear learning tiers:

- **🟢 Level 1: Must Understand Now***Core foundation:* What each algorithm does, how the code works, when it is useful, its time and space complexity, stability, in-place behavior, and how to dry-run it on a small array.
- **🟡 Level 2: Understand After the Basics***Conceptual reasoning:* Why Merge Sort is $O(n \log n)$, why building a heap is $O(n)$, the worst-case limit of comparison sorting, and how Quick Sort handles equal values.
- **🔴 Level 3: Advanced Reference (Optional Deep Dive)**
  *Specialized depth:* Detailed mathematical decision-tree proofs, recurrence expansions, external-sort I/O calculations, and hardware micro-architectural trade-offs. (Revisit these when preparing for senior interview follow-ups).

---

## Table of Contents

- [Module 0: The Friendly Foundation &amp; Math Guide](#module-0-the-friendly-foundation--math-guide)
  - [0.1 What is Sorting and Why Does It Matter?](#01-what-is-sorting-and-why-does-it-matter) `[Level 1]`
  - [0.2 The 5 Core Questions to Ask About Any Sorting Algorithm](#02-the-5-core-questions-to-ask-about-any-sorting-algorithm) `[Level 1]`
  - [0.3 The Friendly Math Guide: How We Measure Growth](#03-the-friendly-math-guide-how-we-measure-growth) `[Level 1 & Level 2]`
  - [0.4 Small English glossary](#04-small-english-glossary)
  - [0.5 Code boundaries and a runnable driver](#05-read-code-boundaries-before-memorising-loops)
  - [0.6 Best, average, worst, and notation](#06-best-average-worst-and-notation)
  - [0.7 Checking whether an array is sorted](#07-checking-whether-an-array-is-sorted)
- [Part A: Course-Taught Algorithms (Core Priority)](#part-a-course-taught-algorithms-core-priority)
  - [1. Bubble Sort](#1-bubble-sort) `[Level 1]`
  - [2. Selection Sort](#2-selection-sort) `[Level 1]`
  - [3. Insertion Sort](#3-insertion-sort) `[Level 1]`
  - [4. Counting Sort](#4-counting-sort) `[Level 1 & Level 2]`
- [Part B: Essential Interview Algorithms](#part-b-essential-interview-algorithms)
  - [5. Merge Sort](#5-merge-sort) `[Level 1 & Level 2]`
  - [6. Quick Sort](#6-quick-sort) `[Level 1 & Level 2]`
    - [Hoare partition and recursion boundaries](#hoare-partition)
  - [7. Heap Sort](#7-heap-sort) `[Level 1 & Level 2]`
- [Part C: Important Additional Algorithms](#part-c-important-additional-algorithms)
  - [8. Radix Sort](#8-radix-sort) `[Level 2]`
  - [9. Bucket Sort](#9-bucket-sort) `[Level 2]`
  - [10. Shell Sort](#10-shell-sort) `[Level 2]`
- [Part D: Practical C++ STL Sorting](#part-d-practical-c-stl-sorting)
  - [11. C++ Standard Library Sorting Functions](#11-c-standard-library-sorting-functions) `[Level 1]`
  - [12. Custom Comparators &amp; The Fatal `<=` Bug](#12-custom-comparators--the-fatal--bug) `[Level 1]`
- [Part E: Essential Theory &amp; System Concepts](#part-e-essential-theory--system-concepts)
  - [13. Why Comparison Sorts Cannot Beat $O(n \log n)$ (The 20 Questions Game)](#13-why-comparison-sorts-cannot-beat-on-log-n-the-20-questions-game) `[Level 2 & Level 3]`
  - [14. Real-World Systems: External Sort, Linked Lists, and $k$-Sorted Arrays](#14-real-world-systems-external-sort-linked-lists-and-k-sorted-arrays) `[Level 2 & Level 3]`
- [Part F: Course Problems — Sort Colors, Merge Sorted Arrays, Next Permutation](#course-problems)
  - [15. Descending order](#descending-order)
  - [16. Sort Colors / DNF](#sort-colors)
  - [17. Merge into a padded array](#merge-sorted-arrays)
  - [18. Next Permutation](#next-permutation)
- [Teach-back checklist](#teach-back)
- [Sources and verification](#sources)

---

# Module 0: The Friendly Foundation & Math Guide

<a id="01-what-is-sorting-and-why-does-it-matter"></a>

## 0.1 What is Sorting and Why Does It Matter? `[Level 1: Must Understand Now]`

Imagine looking up a word in a dictionary where the pages were scattered at random. You would have to look through every single page from start to finish. But because the dictionary is **sorted alphabetically**, you can flip straight to the middle, skip thousands of irrelevant words, and find what you need in seconds.

In computer science, **sorting** means taking a collection of items (like numbers, names, or dates) and rearranging them into a specific order:

- **Ascending order:** Smallest to largest. Equal values are allowed: `[1, 2, 2, 5]`. The exact term is **non-decreasing**: `arr[i] <= arr[i + 1]`.
- **Descending order:** Largest to smallest. Equal values are allowed: `[5, 2, 2, 1]`. The exact term is **non-increasing**: `arr[i] >= arr[i + 1]`.

A correct sort must keep every original item, including every duplicate. `[1, 2, 2]` is not a correct sorted result for `[2, 1, 2, 2]`: one item is missing.

Sorting is one of the most critical foundational operations in computing because it turns hard problems into easy ones:

- **Searching:** A linear search may examine all $n$ items. Binary search on a sorted array takes $O(\log n)$ comparisons. Sorting first costs time, so this is useful especially when you will make many searches.
- **Finding Duplicates:** While a hash set can find duplicates in expected $O(n)$ time using extra memory, sorting offers an in-place alternative: once sorted, identical items sit right next to each other, allowing duplicates to be detected in a single linear scan without auxiliary hash table memory.
- **Finding Extremes:** In ascending order, the minimum is first and the maximum is last. If you only need the minimum or maximum once, scan in $O(n)$ time; sorting is unnecessary.

---

<a id="02-the-5-core-questions-to-ask-about-any-sorting-algorithm"></a>

## 0.2 The 5 Core Questions to Ask About Any Sorting Algorithm `[Level 1: Must Understand Now]`

Whenever you study a sorting algorithm, ask these five simple questions:

```
                                 5 CORE QUESTIONS
                                        │
     ┌──────────────────┬───────────────┼───────────────┬──────────────────┐
     ▼                  ▼               ▼               ▼                  ▼
1. Is it fast?     2. Is it        3. Does it need 4. Does it         5. How does it
   (Time              STABLE?         extra memory?   ADAPT?             decide order?
   Complexity)     (Keep equal     (IN-PLACE vs.   (Faster if         (Comparison vs.
                    order?)         Out-of-Place)   already sorted?)   Non-comparison)
```

### 1. How fast is it? (Time Complexity)

How does the number of operations grow as the list size $n$ grows from 10 to 1,000 to 1,000,000?

### 2. Is it Stable? (Stability)

- **Plain English:** If two records have the same **sorting key**, does the algorithm keep their original order? A key is the field we compare, such as a student's grade. Names may differ even when grades are equal.
- **Real-Life Example:** Imagine a list of students already sorted alphabetically by name:
  `[(Alice, Grade B), (Bob, Grade A), (Charlie, Grade B)]`.
  If you re-sort by Grade:
  - A **Stable** sort guarantees `Alice` stays before `Charlie` because both got `Grade B` and Alice came first originally:`[(Bob, Grade A), (Alice, Grade B), (Charlie, Grade B)]`.
  - An **Unstable** sort might flip them, placing `Charlie` before `Alice`.
- **Why this matters:** In databases, spreadsheets, and online stores, users constantly sort by one column, then by another (e.g., sort by Date, then sort by Price). If the sort is unstable, the earlier sort order is scrambled.

### 3. Does it need extra memory? (In-Place vs. Out-of-Place)

- **Strictly in-place:** Uses $O(1)$ extra memory, including the function call stack. Bubble, Selection, Insertion, and iterative Heap Sort fit this definition.
- **With a buffer:** Uses another array while sorting, as our Merge Sort does. Copying the result back into the original array does not make the memory cost $O(1)$.
- **Quick Sort convention:** Often called in-place because partitioning uses the existing array. Its recursive calls still use memory. State the stack cost separately.

### 4. Does it adapt to already sorted data? (Adaptive)

- **Adaptive:** If you hand the algorithm an array that is already sorted (or nearly sorted), it detects this and finishes much faster (e.g., taking $n$ steps instead of $n^2$).
- **Non-Adaptive:** Does not use existing order to improve its time bound. Its exact number of swaps may still vary. Selection Sort, for example, skips swaps on sorted input but still makes all its comparisons.

### 5. How does it decide the order? (Comparison vs. Non-Comparison)

- **Comparison-Based:** Compares elements two at a time ("is $A$ smaller than $B$?"). Works for any data type you can compare.
- **Non-Comparison-Based:** Uses key values as indices or examines their digits. It needs suitable keys and range assumptions. Counting Sort may compare values to find the minimum and maximum; it avoids using pairwise comparisons to determine the final order.

---

<a id="03-the-friendly-math-guide-how-we-measure-growth"></a>

## 0.3 The Friendly Math Guide: How We Measure Growth `[Level 1 & Level 2]`

You do not need calculus or complex formulas to understand Big-O notation. Here is all the math you need, explained with simple numbers.

### What is $n$?

$n$ is simply the **number of items** in your list. If an array has 5 numbers, $n = 5$. If it has 1,000 numbers, $n = 1000$.

### What is an "Operation"?

One basic action a computer takes: comparing two numbers, swapping two numbers, or copying a number to another spot.

### The 5 Growth Speeds You Need to Know

| Notation                  | Name         | Meaning in Plain English                                                                | Steps for $n = 10$ | Steps for $n = 1,000$ | Real-World Analogy                                                   |
| ------------------------- | ------------ | --------------------------------------------------------------------------------------- | -------------------- | ----------------------- | -------------------------------------------------------------------- |
| **$O(1)$**        | Constant     | Work is bounded independently of $n$.                                                 | ~1 unit              | ~1 unit                 | Looking at the first card on top of a deck.                          |
| **$O(n)$**        | Linear       | Steps grow directly in proportion to $n$. Double the items $\to$ double the time.   | 10 steps             | 1,000 steps             | Reading a book page by page from start to finish.                    |
| **$O(n^2)$**      | Quadratic    | Steps grow with $n \times n$. Double the items $\to$ **four times** the time! | 100 steps            | 1,000,000 steps         | Comparing every single person in a room with every other person.     |
| **$O(\log_2 n)$** | Logarithmic  | Steps grow by only 1 each time you **double** $n$!                              | ~3 steps             | ~10 steps               | Playing the guessing game: "Is your number higher or lower than 50?" |
| **$O(n \log n)$** | Linearithmic | The gold standard for fast comparison sorting.                                          | ~33 steps            | ~10,000 steps           | Cutting a deck of cards in half repeatedly, then scanning each half. |

### Visualizing $\log_2 n$ (The "Halving Counter") `[Level 2]`

Whenever you see $\log_2 n$ (read as "log base 2 of $n$"), just ask:
**"How many times can I cut $n$ in half until I reach 1?"**

Let's test it:

- If $n = 8$: $8 \to 4 \to 2 \to 1$ (Cut in half 3 times) $\implies \log_2 8 = 3$.
- If $n = 16$: $16 \to 8 \to 4 \to 2 \to 1$ (Cut in half 4 times) $\implies \log_2 16 = 4$.
- If $n = 1,000$: You can cut it in half only about 10 times! $\implies \log_2 1000 \approx 10$.
- If $n = 1,000,000$: You can cut it in half only about 20 times! $\implies \log_2 1000000 \approx 20$.

This is why an $O(n \log n)$ algorithm is vastly faster than an $O(n^2)$ algorithm:

- For $n = 1,000,000$:
  - $n^2 = 1,000,000,000,000$ growth units.
  - $n \log_2 n \approx 20,000,000$ growth units.

These numbers show growth, not exact instructions or seconds. Actual time depends on the machine, implementation, and cost of each comparison.

## 0.4 Small English glossary

| Word            | Simple meaning                                              | Example                                                         |
| --------------- | ----------------------------------------------------------- | --------------------------------------------------------------- |
| Index           | Position of an item, starting at 0                          | In `[8, 3, 5]`, index 1 holds `3`                           |
| Key / record    | Field used for sorting / whole item                         | Sort `{age: 20, name: "Asha"}` by age                         |
| Prefix / suffix | A part starting at the beginning / ending at the end        | `[1, 2]` is a prefix of `[1, 2, 7, 4]`                      |
| Pass            | One round of an algorithm's outer loop                      | One left-to-right sweep in Bubble Sort                          |
| Swap / shift    | Exchange two items / move an item one position              | Swap `5, 2` to `2, 5`; shift `5` right to make a gap      |
| Invariant       | A statement that stays true at a chosen point in every loop | Before each Insertion Sort pass, the processed prefix is sorted |
| Auxiliary space | Extra memory beyond the input                               | A helper array and recursive call frames count                  |
| Precondition    | What must be true before code runs                          | DNF requires values from `{0, 1, 2}`                          |
| Partition       | Divide items into groups using a rule                       | Values smaller than, equal to, and greater than a pivot         |
| Inversion       | A pair in the wrong order                                   | In `[3, 1, 2]`, `(3, 1)` and `(3, 2)` are inversions      |

## 0.5 Read code boundaries before memorising loops

- A size-$n$ array has indices `0` through `n - 1`. Index `n` is outside it.
- `[low ... high]` includes both ends. It has `high - low + 1` items.
- C++ iterator ranges `[first, last)` include `first` and exclude `last`. `end()` points just past the last item.
- For Bubble Sort, `j + 1` must be valid. After `i` passes, the last `i` items are settled. So `j + 1 <= n - 1 - i`, which gives `j < n - 1 - i`.
- `j >= 0 && arr[j] > key` checks `j` first. C++ stops evaluating `&&` when its left side is false. Reversing these checks could read `arr[-1]`.
- `vector<int>& arr` means the function can change the caller's vector. Passing a vector by value makes a copy instead.
- `size()` returns an unsigned size. For these teaching examples, arrays must fit in `int` indices, and `int` is assumed to be 32 bits. Use `static_cast<int>(arr.size()) - 1`, not `arr.size() - 1`, when making a signed last index for an empty vector.

The function blocks are reusable definitions. To run one, put its block above this driver and replace `bubbleSort` with the function you want:

```cpp
#include <iostream> // for input and output
#include <vector>   // for dynamic arrays
using namespace std;

int main(){
    vector<int> values = {5, 2, 4, 1, 3};
    bubbleSort(values);
    for(int x : values){
        cout << x << ' ';
    }
}
```

For the indexed Quick Sort functions, call `quickSort(values, 0, static_cast<int>(values.size()) - 1)`.

### Code style used throughout this package

We follow the style of your `C++ & DSA` examples: four-space indentation, consistent braces, and `using namespace std;`. This lets us write `vector`, `cout`, `sort`, and `swap` without repeating `std::`.

Each complete example includes only the headers its code needs. The short comment beside a header explains its purpose. `iostream` is needed for printing or input; it is not needed in a sorting function that only changes an array. `utility` declares `swap` and `move`, and `algorithm` declares helpers such as `sort`, `reverse`, `min`, and `max`.

Short revision fragments reuse the headers and namespace from the corresponding complete program.

## 0.6 Best, average, worst, and notation

Best and worst case describe the easiest and hardest inputs of the **same size**. Average case needs an input model, often a random order of distinct values. Expected time for a randomised algorithm averages over its random choices. It is not a promise for every run.

The usual sorting time bounds assume comparing or moving one item costs $O(1)$. Comparing long strings or fetching a key from a database can cost more; include that cost when analysing the real task.

- $O(f(n))$: an upper bound on growth.
- $\Omega(f(n))$: a lower bound on growth.
- $\Theta(f(n))$: matching upper and lower bounds.

For example, Selection Sort makes exactly `n(n - 1) / 2` key comparisons, so its time is $\Theta(n^2)$. Here a key comparison means comparing two values; loop-bound checks are counted separately if you need exact operation totals.

## 0.7 Checking whether an array is sorted

Compare neighbours. Stop at the first decreasing pair. Equal neighbours are allowed.

```cpp
#include <vector>  // for dynamic arrays
#include <cstddef> // for size_t, an array-size type
using namespace std;

bool isSortedAscending(const vector<int>& arr){
    for(size_t i = 1; i < arr.size(); ++i){
        if(arr[i - 1] > arr[i]){
            return false;
        }
    }
    return true;
}
```

`[]`, `[7]`, and `[1, 1, 2]` return `true`; `[1, 3, 2]` returns `false`. Worst time is $O(n)$; extra space is $O(1)$. An inversion near the beginning can be found in $O(1)$ time. C++ also provides `is_sorted(arr.begin(), arr.end())` in `<algorithm>`.

---

# Part A: Course-Taught Algorithms (Core Priority)

---

<a id="1-bubble-sort"></a>

# 1. Bubble Sort `[Level 1: Must Understand Now]`

### Step 1: Plain-English Intuition

Imagine air bubbles underwater. The largest bubbles are the most buoyant, so they rise to the surface first.
In Bubble Sort, we walk through the array from left to right. Whenever we see two neighboring numbers in the wrong order, we swap them. By the time we reach the end of the first pass, the **single largest number** has bubbled all the way to the very end! We then repeat the process for the remaining numbers.

---

### Step 2: Concrete Example

Let's sort `[5, 2, 4, 1, 3]`:

#### Pass 0 (Bubbling the largest number to the end):

1. Compare `5` and `2`: $5 > 2$, so swap $\to [2, 5, 4, 1, 3]$
2. Compare `5` and `4`: $5 > 4$, so swap $\to [2, 4, 5, 1, 3]$
3. Compare `5` and `1`: $5 > 1$, so swap $\to [2, 4, 1, 5, 3]$
4. Compare `5` and `3`: $5 > 3$, so swap $\to [2, 4, 1, 3, \mathbf{5}]$
   *(Notice: `5` is now locked into its permanent, final position!)*

#### Pass 1:

1. Compare `2` and `4`: $2 < 4$, no swap $\to [2, 4, 1, 3, 5]$
2. Compare `4` and `1`: $4 > 1$, so swap $\to [2, 1, 4, 3, 5]$
3. Compare `4` and `3`: $4 > 3$, so swap $\to [2, 1, 3, \mathbf{4}, \mathbf{5}]$
   *(`4` is now locked into place).*

#### Pass 2:

1. Compare `2` and `1`: $2 > 1$, so swap $\to [1, 2, 3, 4, 5]$
2. Compare `2` and `3`: $2 < 3$, no swap $\to [1, 2, \mathbf{3}, \mathbf{4}, \mathbf{5}]$

#### Pass 3 (The Adaptive Check):

We scan the remaining elements `[1, 2]`. $1 < 2$, no swap.
Since zero swaps occurred, the array is sorted. In this five-item example, this is already the last possible pass. Early exit saves later passes on inputs such as `[1, 2, 3, 4, 5]`, where the first pass has no swaps.

---

### Step 3: Formal Terminology

- **Pass ($i$):** One complete sweep through the unsorted portion of the array.
- **Adjacent Swap:** Exchanging two immediate neighbors `arr[j]` and `arr[j+1]`.
- **Adaptive Optimization:** Using a boolean flag (`swapped`) to detect if an array is already sorted so we can break out of the loop early.

**Why it works:** After each comparison, the largest value seen so far is at the right end of the pair. At the end of pass `i`, the largest remaining value is at `n - 1 - i`. Later passes leave that sorted suffix alone. If a full pass has no swaps, all neighbouring pairs in the active part are ordered, so it can stop.

Reset `swapped = false` **inside** the outer loop. A flag left true from an earlier pass cannot detect a later pass with no swaps. Without this early-exit check, even sorted input takes $\Theta(n^2)$ comparisons.

---

### Step 4: Complete C++ Implementation

```cpp
#include <vector>  // for dynamic arrays
#include <utility> // for swap, move, and pair
using namespace std;

void bubbleSort(vector<int>& arr){
    int n = arr.size();
    if(n <= 1){
        return; // An array of 0 or 1 item is already sorted
    }

    // Outer loop: controls how many passes we make
    for(int i = 0; i < n - 1; ++i){
        bool swapped = false; // Flag to track if any swap happens in this pass

        // Inner loop: compares neighboring pairs
        // Notice the limit: n - 1 - i. We don't need to check the last i items
        // because they are already sorted and locked in!
        for(int j = 0; j < n - 1 - i; ++j){
            if(arr[j] > arr[j + 1]){ // Use strict '>' to keep it stable
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped, the array is already completely sorted!
        if(!swapped){
            break;
        }
    }
}
```

---

### Step 5: Complexity Analysis

- **Best Case ($O(n)$):** When the array is already sorted. The inner loop makes $n-1$ comparisons, does zero swaps, sees `swapped == false`, and terminates immediately.
- **Worst Case ($O(n^2)$):** When the array is reverse sorted (`[5, 4, 3, 2, 1]`). Total comparisons $= (n-1) + (n-2) + \dots + 1 = \frac{n(n-1)}{2} \approx \frac{1}{2} n^2$.
- **Auxiliary Space:** $O(1)$ (strictly in-place).
- **Stability:** **Stable**. Because we only swap if `arr[j] > arr[j+1]`, two equal numbers are never swapped past each other.

---

### Step 6: Verify Understanding

> **Question:** What happens if you change `if (arr[j] > arr[j + 1])` to `if (arr[j] >= arr[j + 1])`?
>
> <details><summary><b>Click for Answer</b></summary>
> The sort will still order the numbers correctly, but it will <b>lose its stability</b>! Equal numbers will now swap past each other, scrambling their original relative order.
> </details>

---

<a id="2-selection-sort"></a>

# 2. Selection Sort `[Level 1: Must Understand Now]`

### Step 1: Plain-English Intuition

Imagine you have a messy bookshelf. You want to arrange the books by height.
You scan the entire shelf, find the **shortest book**, and swap it with whatever book is sitting at spot 0.
Then you look at the rest of the shelf, find the shortest remaining book, and swap it with whatever book is sitting at spot 1.
You keep doing this until you reach the end.

---

### Step 2: Concrete Example

Let's sort `[5, 2, 4, 1, 3]`:

```
Initial:                      [ 5 , 2 , 4 , 1 , 3 ]
Pass 0: Smallest in whole list is 1 (idx 3). Swap with idx 0 (5):
                              [ 1 | 2 , 4 , 5 , 3 ]
Pass 1: Smallest in remaining is 2 (idx 1). Already at idx 1:
                              [ 1 , 2 | 4 , 5 , 3 ]
Pass 2: Smallest in remaining is 3 (idx 4). Swap with idx 2 (4):
                              [ 1 , 2 , 3 | 5 , 4 ]
Pass 3: Smallest in remaining is 4 (idx 4). Swap with idx 3 (5):
                              [ 1 , 2 , 3 , 4 | 5 ]
Done!
```

---

### Step 3: Formal Terminology

- **Sorted Prefix:** The left portion of the array `arr[0 ... i-1]` that is already in final sorted order.
- **Unsorted Suffix:** The right portion `arr[i ... n-1]` that we scan to find the minimum.
- **Memory Write (Swap):** Selection Sort makes **at most 1 swap per pass**, totaling at most $n - 1$ swaps for the entire array!

**Why it works:** Before pass `i`, the prefix contains the `i` smallest items in their final positions. The scan finds the smallest item still outside this prefix. Placing it at `i` extends the prefix by one. Keep the swap **after** the inner loop; swapping every time you find a smaller candidate is a different procedure with more writes.

---

### Step 4: Complete C++ Implementation

```cpp
#include <vector>  // for dynamic arrays
#include <utility> // for swap, move, and pair
using namespace std;

void selectionSort(vector<int>& arr){
    int n = arr.size();
    if(n <= 1){
        return;
    }

    // Loop through each position in the array
    for(int i = 0; i < n - 1; ++i){
        int minIdx = i; // Assume the current spot has the minimum

        // Scan the rest of the array to find the true minimum
        for(int j = i + 1; j < n; ++j){
            if(arr[j] < arr[minIdx]){
                minIdx = j; // Found a smaller number, record its index
            }
        }

        // Swap the true minimum into its correct position
        if(minIdx != i){
            swap(arr[i], arr[minIdx]);
        }
    }
}
```

---

### Step 5: Complexity & Number of Writes

- **Time Complexity:** Strictly $\Theta(n^2)$ in **all cases** (Best, Average, and Worst). It always performs $\frac{n(n-1)}{2}$ comparisons because it cannot verify the minimum without inspecting every remaining item.
- **Auxiliary Space:** $O(1)$.
- **Stability:** **Unstable!***Proof by Counterexample:* Sort `[4a, 4b, 1]`.
  - Pass 0 finds minimum `1` at index 2.
  - It swaps `arr[0]` (`4a`) with `arr[2]` (`1`).
  - Result: `[1, 4b, 4a]`. `4b` now precedes `4a`! Their original order was reversed.
- **When is Selection Sort Useful?** For learning, or for small inputs where reducing item movement matters. It uses at most `n - 1` swaps, so $O(n)$ array writes. One swap writes two array positions; a swap is not one write. This is a useful trade-off against Bubble and Insertion Sort, not a claim that Selection Sort minimises writes among every possible sorting algorithm.

---

### Step 6: Verify Understanding

> **Question:** If an array is already sorted, does Selection Sort run faster?
>
> <details><summary><b>Click for Answer</b></summary>
> <b>No!</b> Selection Sort is non-adaptive. Even if the array is perfectly sorted, it still scans the unsorted suffix every single pass to verify that no smaller element exists, taking $\Theta(n^2)$ time.
> </details>

---

<a id="3-insertion-sort"></a>

# 3. Insertion Sort `[Level 1: Must Understand Now]`

### Step 1: Plain-English Intuition

Imagine playing cards. You hold your hand of cards in your left hand, sorted from left to right. When dealt a new card, you slide it backwards past any cards that are bigger than it, and drop it into its exact sorted spot.

---

### Step 2: Concrete Example

Let's sort `[5, 2, 4, 1, 3]`:

```
Start: Hand has [5]. Next card is 2.
- 5 is bigger than 2, so shift 5 right. Drop 2 into spot 0:
  [ 2 , 5 | 4 , 1 , 3 ]

Next card is 4:
- 5 is bigger than 4, shift 5 right.
- 2 is smaller than 4, stop! Drop 4 into spot 1:
  [ 2 , 4 , 5 | 1 , 3 ]

Next card is 1:
- 5 > 1 (shift 5), 4 > 1 (shift 4), 2 > 1 (shift 2). Drop 1 into spot 0:
  [ 1 , 2 , 4 , 5 | 3 ]

Next card is 3:
- 5 > 3 (shift 5), 4 > 3 (shift 4).
- 2 is smaller than 3, stop! Drop 3 into spot 2:
  [ 1 , 2 , 3 , 4 , 5 ]
Done!
```

---

### Step 3: Shifting vs. Swapping

Some beginners write Insertion Sort by repeatedly swapping: `swap(arr[j], arr[j-1])`.

- A simple swap uses three assignments through a temporary variable, including two writes to array positions.
- **Shifting** saves the new card in a variable (`key`), slides older numbers one spot to the right with a single write (`arr[j+1] = arr[j]`), and writes `key` once at the end.
  If the key moves left by `t` positions, adjacent swapping writes array positions about `2t` times. Shifting writes `t + 1` times: `t` shifts and one key insertion. Fewer writes can help, but this does not prove an exact speed multiplier.

**Why save `key`?** While inserting `2` into `[1, 5, 7, 2]`, shifts temporarily produce `[1, 5, 7, 7]`, then `[1, 5, 5, 7]`. The value `2` is safe in `key`; finally write it at index 1. Temporary repeated values during shifting do not mean data was lost.

**Why it works:** Before pass `i`, `arr[0 ... i-1]` is sorted. Shift only values greater than `key`. This opens the correct gap without changing their order. Inserting `key` makes `arr[0 ... i]` sorted. Unlike Selection Sort, this prefix is sorted but its items are not necessarily in their final positions: a later smaller item may shift them right.

---

### Step 4: Complete C++ Implementation

```cpp
#include <vector> // for dynamic arrays
using namespace std;

void insertionSort(vector<int>& arr){
    int n = arr.size();
    if(n <= 1){
        return;
    }

    // Start at index 1 because a single element (index 0) is already sorted
    for(int i = 1; i < n; ++i){
        int key = arr[i]; // The new card we want to insert
        int j = i - 1;    // Walk backwards through the sorted hand

        // Slide elements that are greater than key to the right
        while(j >= 0 && arr[j] > key){
            arr[j + 1] = arr[j]; // Shift element one spot right
            j--;
        }

        // Drop the key into its correct spot
        arr[j + 1] = key;
    }
}
```

---

### Step 5: Complexity & Why Standard Libraries Use It

- **Best Case ($O(n)$):** When the array is already sorted. For every card, `arr[j] > key` is false immediately on the first check. It does only $n - 1$ comparisons and 0 shifts!
- **Worst Case ($O(n^2)$):** When the array is reverse sorted. Every new card must slide past every card before it.
- **Auxiliary Space:** $O(1)$.
- **Stability:** **Stable**. Because the loop checks `arr[j] > key` (strict inequality), an identical card stops the slide immediately and stays behind its twin.
- **Nearly sorted input:** Let $I$ be the inversion count. Each shift removes one inversion, so the total work is $\Theta(n + I)$. `[1, 2, 4, 3, 5]` has just one inversion. A small number of misplaced items can still create many inversions; use $I$ to reason precisely.
- **Why sorting libraries use it:** Insertion Sort has simple loops and little setup work. Many hybrid library implementations use it for small parts. The exact size threshold and algorithm choices depend on the library.

---

### Step 6: Verify Understanding

> **Question:** An array has 1,000 numbers and only 2 inversions. Which of Bubble, Selection, and Insertion Sort has a direct $O(n + I)$ bound?
>
> <details><summary><b>Click for Answer</b></summary>
> <b>Insertion Sort!</b> Its running time is directly proportional to the number of out-of-order pairs (inversions). On nearly-sorted data, Insertion Sort runs in almost pure linear $O(n)$ time.
> </details>

---

<a id="4-counting-sort"></a>

# 4. Counting Sort `[Level 1 & Level 2]`

### Step 1: Plain-English Intuition `[Level 1]`

Every sorting algorithm we looked at so far works by comparing two numbers: *"Is $A < B$?"* Because of this, they are called **Comparison Sorts**.

Counting Sort uses each integer key as a count-array index. It does not compare every pair to decide their order.
Imagine you are tallying votes in a school election where candidates are numbered 1 to 5:

1. You prepare 5 boxes labeled 1, 2, 3, 4, 5.
2. For every vote slip, you drop a tally mark into that candidate's box.
3. At the end, you look at the boxes: Candidate 1 got two votes, Candidate 2 got zero, Candidate 3 got one...
4. You write down the results in order: `1, 1, 3...`

The work is $O(n + k)$: count $n$ items and scan $k$ possible values. It is linear in $n$ when the range $k$ is at most proportional to $n$.

---

### Step 2: The Two Ways to Do Counting Sort `[Level 1]`

#### Way 1: The Simple Way (Frequency Overwrite)

Count frequencies, then loop through the counts and overwrite the array with numbers.

- **Limitation:** This rebuilds plain numbers from their counts. It cannot reconstruct whole customer records from ages alone. If the items have names or IDs, move the whole records instead.

```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
#include <stdexcept> // for reporting invalid input
#include <cstddef>   // for size_t, an array-size type
using namespace std;

void countingSortValues(vector<int>& arr, bool descending = false){
    if(arr.empty()){
        return;
    }
    int minimum = *min_element(arr.begin(), arr.end());
    int maximum = *max_element(arr.begin(), arr.end());
    long long range = static_cast<long long>(maximum) - minimum + 1;
    if(range > 10000000){
        throw invalid_argument("Range too large");
    }

    vector<size_t> count(static_cast<size_t>(range), 0);
    for(int value : arr){
        ++count[static_cast<size_t>(value - minimum)];
    }
    size_t write = 0;
    for(long long step = 0; step < range; ++step){
        long long bucket = descending ? range - 1 - step : step;
        for(size_t copies = count[static_cast<size_t>(bucket)]; copies > 0; --copies){
            arr[write++] = static_cast<int>(minimum + bucket);
        }
    }
}
```

Time is $O(n + k)$, extra space is $O(k)$, and the original vector is overwritten. This is **not strictly in-place** because the count array grows with the range. The range cap is a choice for this example, not a universal Counting Sort rule. Allocation can still fail if the machine lacks memory.

#### Way 2: The Stable Way (Prefix Sums & Output Array)

To sort real-world objects and preserve stability, we use **Prefix Sums (Cumulative Counts)**:

1. Count the frequencies of each number.
2. Add each count to the running total. This gives the number of items less than or equal to that key. Subtract 1 to obtain the last available zero-based output index.
3. Walk backwards through the original array and place each element directly into its target slot.

---

### Step 3: Concrete Trace of Stable Counting Sort `[Level 1]`

Let's sort `[4, -1, 2, 4, -1]` (notice the negative numbers!):

1. **Find Range:**

   - Smallest value (`minVal`) $= -1$.
   - Largest value (`maxVal`) $= 4$.
   - Range $k = 4 - (-1) + 1 = 6$ buckets (from index 0 to 5).
   - We shift numbers by subtracting `minVal`: value $-1$ maps to bucket $0$, value $4$ maps to bucket $5$.
2. **Tally Frequencies:**

   ```
   Value:    -1   0   1   2   3   4
   Bucket:    0   1   2   3   4   5
   Count:     2   0   0   1   0   2
   ```
3. **Compute Cumulative Counts (Prefix Sums):**

   ```
   Bucket:    0   1   2   3   4   5
   Running:   2   2   2   3   3   5
   ```

   These are counts, not indices. The `-1` group ends at index `2 - 1 = 1`, the `2` group at index `3 - 1 = 2`, and the `4` group at index `5 - 1 = 4`.
4. **Backward Placement (Guarantees Stability):**

   - Last item is `-1` (bucket 0). Running count is 2 $\to$ Place at index $2 - 1 = 1$. Decrement count to 1.
   - Next item is `4` (bucket 5). Running count is 5 $\to$ Place at index $5 - 1 = 4$. Decrement count to 4.
   - Next item is `2` (bucket 3). Running count is 3 $\to$ Place at index $3 - 1 = 2$. Decrement count to 2.
   - Next item is `-1` (bucket 0). Running count is 1 $\to$ Place at index $1 - 1 = 0$. Decrement count to 0.
   - First item is `4` (bucket 5). Running count is 4 $\to$ Place at index $4 - 1 = 3$. Decrement count to 3.

Result: `[-1, -1, 2, 4, 4]`. To see stability, label the input `[4a, -1a, 2, 4b, -1b]`. The result is `[-1a, -1b, 2, 4a, 4b]`: equal keys keep their original order. The integer code below uses the same placement rule, but plain equal integers cannot show identity.

---

### Step 4: Complete C++ Implementation (Guarded Against Integer Overflow) `[Level 2]`

```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
#include <utility>   // for swap, move, and pair
#include <stdexcept> // for reporting invalid input
#include <cstddef>   // for size_t, an array-size type
using namespace std;

void countingSort(vector<int>& arr){
    if(arr.size() <= 1){
        return;
    }

    // Step 1: Find the minimum and maximum values
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());

    // CRITICAL OVERFLOW GUARD:
    // If minVal == INT_MIN and maxVal == INT_MAX, doing (maxVal - minVal + 1)
    // in standard 32-bit signed 'int' causes immediate arithmetic overflow!
    // We cast to 'long long' first to compute the true mathematical range safely.
    long long range = static_cast<long long>(maxVal) - minVal + 1;

    // Safety guard: Reject impractical allocations (e.g. > 10 million buckets)
    if(range > 10000000){
        throw invalid_argument("Value range is too large for Counting Sort memory allocation.");
    }

    // Step 2: Build the frequency count array
    vector<int> count(range, 0);
    for(int num : arr){
        count[num - minVal]++; // Offset by minVal to handle negative numbers safely!
    }

    // Step 3: Transform into prefix sums (cumulative counts)
    for(size_t i = 1; i < count.size(); ++i){
        count[i] += count[i - 1];
    }

    // Step 4: Build output array by traversing the original array backwards
    vector<int> output(arr.size());
    for(int i = static_cast<int>(arr.size()) - 1; i >= 0; --i){
        int bucket = arr[i] - minVal;
        output[count[bucket] - 1] = arr[i];
        count[bucket]--; // Decrement so the next identical item goes to the left
    }

    // Copy sorted result back into arr
    arr = move(output);
}
```

---

### Step 5: Complexity & When Counting Sort FAILS `[Level 1]`

- **Time Complexity:** $O(n + k)$, where $n$ is the number of elements and $k$ is the range of values ($\text{max} - \text{min} + 1$).
- **Auxiliary Space:** $O(n + k)$ (needs a count array of size $k$ and an output array of size $n$).
- **The Catastrophic Trap (When NOT to use Counting Sort):**Suppose you have an array with just **5 numbers**: `[1, 5, 2, 8, 1000000000]`.Here, $n = 5$, but $k = 1,000,000,000$ (One Billion)! Counting Sort would try to allocate an array of 1 Billion integers (requiring 4 Gigabytes of RAM) just to sort 5 numbers. A comparison sort needs only a small number of comparisons for those five values, without allocating a billion counters.
  > **Rule of Thumb:** Use Counting Sort only when the range of values $k$ is not significantly larger than the number of elements $n$.
  >

---

### Step 6: Verify Understanding

> **Question:** Why do we loop **backwards** (from $n-1$ down to $0$) when filling the output array in Step 4?
>
> <details><summary><b>Click for Answer</b></summary>
> To preserve <b>stability</b>! Since <code>count[bucket]</code> points to the rightmost available slot for that value, reading from right to left ensures that the duplicate item that appeared later in the input gets placed in the rightmost slot, preserving the original relative order.
> </details>

---

# Part B: Essential Interview Algorithms

---

<a id="5-merge-sort"></a>

# 5. Merge Sort `[Level 1 & Level 2]`

### Step 1: Plain-English Intuition `[Level 1]`

Merge Sort uses the classic strategy: **Divide and Conquer**.

1. **Divide:** If you have a huge stack of 100 exam papers to sort alphabetically, it's exhausting. But if you split the stack into two stacks of 50, and give each half to a friend to sort, your job becomes much simpler.
2. **Conquer:** Keep splitting the stacks in half until each stack has only **1 single paper**. (A stack with 1 paper is already sorted by definition!).
3. **Combine (Merge):** Now take two sorted stacks and combine them into one. You look at the top paper of each stack, take the smaller one, and put it into your new pile. You repeat this until both stacks are merged.

---

### Step 2: Visual Tree of the Merge Process `[Level 1]`

Let's sort `[5, 2, 4, 1, 3]`:

```
                           [ 5 , 2 , 4 , 1 , 3 ]
                               /           \
                     [ 5 , 2 , 4 ]          [ 1 , 3 ]
                        /     \              /    \
                     [ 5 , 2 ] [ 4 ]      [ 1 ]  [ 3 ]
                      /    \
                   [ 5 ]  [ 2 ]
═════════════════════════════════════════════════════════════════════
MERGE BACK UP:
- Merge [1] and [3]       ───►  [ 1 , 3 ]
- Merge [5] and [2]       ───►  [ 2 , 5 ]
- Merge [2, 5] and [4]    ───►  [ 2 , 4 , 5 ]
- Merge [2,4,5] and [1,3] ───►  [ 1 , 2 , 3 , 4 , 5 ] (Done!)
```

---

### Step 3: The 2-Pointer Linear Merge Walkthrough `[Level 1]`

How do we merge `[2, 5]` and `[1, 3, 4]`?

- Place pointer $i$ at the start of `[2, 5]` and pointer $j$ at the start of `[1, 3, 4]`:
  1. Compare `2` and `1`: $1$ is smaller. Pick `1`, advance $j$. Merged: `[1]`.
  2. Compare `2` and `3`: $2$ is smaller. Pick `2`, advance $i$. Merged: `[1, 2]`.
  3. Compare `5` and `3`: $3$ is smaller. Pick `3`, advance $j$. Merged: `[1, 2, 3]`.
  4. Compare `5` and `4`: $4$ is smaller. Pick `4`, advance $j$. Merged: `[1, 2, 3, 4]`.
  5. The right list is empty. Copy remaining items from left list (`5`). Merged: `[1, 2, 3, 4, 5]`.
     Notice: We touched each number only once! Merging takes **pure linear time $O(n)$**.

---

### Step 4: Complete C++ Implementation `[Level 1]`

```cpp
#include <vector> // for dynamic arrays
using namespace std;

void merge(vector<int>& arr, int low, int mid, int high, vector<int>& temp){
    int i = low;     // Pointer for left half
    int j = mid + 1; // Pointer for right half
    int k = low;     // Pointer for temporary output buffer

    while(i <= mid && j <= high){
        // Use '<=' to preserve STABILITY: if numbers are equal, take left half first!
        if(arr[i] <= arr[j]){
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    // Copy any leftover elements from the left half
    while(i <= mid){
        temp[k++] = arr[i++];
    }

    // Copy any leftover elements from the right half
    while(j <= high){
        temp[k++] = arr[j++];
    }

    // Copy merged results back into the original array
    for(int p = low; p <= high; ++p){
        arr[p] = temp[p];
    }
}

void mergeSortInternal(vector<int>& arr, int low, int high, vector<int>& temp){
    if(low >= high){
        return; // Base case: 0 or 1 element is already sorted
    }

    // Safe midpoint calculation (prevents integer overflow)
    int mid = low + (high - low) / 2;

    mergeSortInternal(arr, low, mid, temp);      // Sort left half
    mergeSortInternal(arr, mid + 1, high, temp); // Sort right half
    merge(arr, low, mid, high, temp);            // Merge both sorted halves
}

void mergeSort(vector<int>& arr){
    if(arr.size() <= 1){
        return;
    }
    // Single upfront allocation to avoid repeating dynamic allocations inside recursion
    vector<int> temp(arr.size());
    mergeSortInternal(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}
```

---

### Step 5: Intuitive Complexity Proof (Why is it $O(n \log n)$?) `[Level 2]`

1. **Work per level:** On every level of the tree, all $n$ numbers participate in a merge step. So each level takes $n$ operations.
2. **Number of levels:** Because the array is cut in half at every step, the number of levels is simply the "halving counter": $\log_2 n$.
3. **Total Work:**
   $$
   \text{Total Work} = (\text{Work per level}) \times (\text{Number of levels}) = n \times \log_2 n = \mathbf{O(n \log n)}
   $$

This holds true across **Best Case, Average Case, and Worst Case**!

- **Auxiliary Space:** $O(n)$ temporary buffer memory $+ O(\log n)$ recursion call stack.

**Why it works:** A one-item range is sorted. Assume the recursive calls correctly sort both smaller halves. During merge, their front items are their smallest remaining items. Taking the smaller front item therefore gives the smallest remaining item overall. Repeat and copy leftovers. This proves each merged range is sorted and keeps every item.

**Useful optional optimisation:** After both recursive calls, check `if (arr[mid] <= arr[mid + 1]) return;` before `merge`. If the largest left item is no greater than the smallest right item, the halves already fit together. This variant has $O(n)$ best time on sorted input. The unoptimised code above still does every merge and takes $\Theta(n \log n)$.

---

### Step 6: Verify Understanding

> **Question:** Why do we write `mid = low + (high - low) / 2` instead of `mid = (low + high) / 2`?
>
> <details><summary><b>Click for Answer</b></summary>
> To avoid signed integer overflow when <code>low + high</code> is too large. Signed overflow is undefined behaviour; it is not guaranteed to wrap or turn negative. For valid non-negative indices, <code>low + (high - low) / 2</code> stays within the range.
> </details>

---

<a id="6-quick-sort"></a>

# 6. Quick Sort `[Level 1 & Level 2]`

### Step 1: Plain-English Intuition `[Level 1]`

Merge Sort divides blindly down the middle and does all the hard work during the merge.**Quick Sort is the opposite: it does all the hard work up front!**

1. **Pick a Leader (The Pivot):** Choose one element from the array to be the "pivot" (for example, the last element).
2. **Partition:** Rearrange the array so that:
   - Every number **less than or equal to** the pivot is moved to its left in the Lomuto code below.
   - Every number **greater** than the pivot is moved to the pivot's right.
3. **The Pivot is Done!** The pivot is now sitting in its exact, permanent sorted position!
4. **Repeat:** Recursively repeat the same process for the numbers on the left and the numbers on the right.

---

### Step 2: The Lomuto Partitioning Scheme (Step-by-Step) `[Level 1]`

Let's partition `[5, 2, 4, 1, 3]` using the last element (`3`) as the pivot:

- Maintain a pointer $i$ that marks the boundary of elements $\le \text{pivot}$. Start with $i = -1$.
- Scan $j$ from index $0$ to $3$:
  1. $j = 0$ (`5`): $5 > 3$, do nothing.
  2. $j = 1$ (`2`): $2 \le 3 \implies$ increment $i$ to $0$, swap `arr[0]` (`5`) and `arr[1]` (`2`):Array becomes: `[2, 5, 4, 1, 3]`.
  3. $j = 2$ (`4`): $4 > 3$, do nothing.
  4. $j = 3$ (`1`): $1 \le 3 \implies$ increment $i$ to $1$, swap `arr[1]` (`5`) and `arr[3]` (`1`):
     Array becomes: `[2, 1, 4, 5, 3]`.
- **Final step:** Swap the pivot (`3`) with `arr[i + 1]` (`4`):
  Array becomes: `[2, 1, 3, 5, 4]`. Returns pivot index 2.

---

### Step 3: Complete C++ Implementation `[Level 1]`

```cpp
#include <vector>  // for dynamic arrays
#include <utility> // for swap, move, and pair
using namespace std;

// Lomuto Partition Scheme
int partition(vector<int>& arr, int low, int high){
    int pivot = arr[high]; // Choose last element as pivot
    int i = low - 1;       // Boundary for elements <= pivot

    for(int j = low; j < high; ++j){
        if(arr[j] <= pivot){
            i++;
            swap(arr[i], arr[j]);
        }
    }
    // Place pivot in its correct sorted position
    swap(arr[i + 1], arr[high]);
    return i + 1; // Return the pivot's final index
}

void quickSort(vector<int>& arr, int low, int high){
    if(low >= high){
        return; // Base case: 0 or 1 element
    }

    int pIdx = partition(arr, low, high); // Partition and get pivot index

    quickSort(arr, low, pIdx - 1);  // Recursively sort left of pivot
    quickSort(arr, pIdx + 1, high); // Recursively sort right of pivot
}
```

---

### Step 4: Guaranteeing Logarithmic Stack Space in C++ `[Level 2]`

> [!IMPORTANT]
> **C++ Does Not Guarantee Automatic Tail-Call Optimization!**
> In standard recursion, an unbalanced QuickSort can create a call stack of depth $n$, triggering a stack overflow. To strictly guarantee at most $O(\log_2 n)$ stack frames, the programmer must explicitly recurse into the **smaller partition** first and use an explicit `while` loop to iterate over the larger partition:

```cpp
#include <vector> // for dynamic arrays
using namespace std;

void quickSortLogarithmicStack(vector<int>& arr, int low, int high){
    while(low < high){
        int pIdx = partition(arr, low, high);

        // Always recurse on the smaller partition; loop on the larger!
        if(pIdx - low < high - pIdx){
            quickSortLogarithmicStack(arr, low, pIdx - 1); // Recurse on smaller left
            low = pIdx + 1;                                // Loop on larger right
        } else {
            quickSortLogarithmicStack(arr, pIdx + 1, high); // Recurse on smaller right
            high = pIdx - 1;                                // Loop on larger left
        }
    }
}
```

*Why this works:* Every nested recursive call handles at most half as many items. There are only $O(\log n)$ such halvings, with a constant number of additional frames. This controls **memory**, not time: last-element Lomuto can still take $\Theta(n^2)$ time.

### Quick Sort time and pivot choices

A partition scans $n - 1$ items. Balanced splits give about $\log n$ levels with $O(n)$ work per level: $\Theta(n \log n)$. Repeated splits of sizes `n - 1` and `0` give `(n - 1) + (n - 2) + ... + 1`, or $\Theta(n^2)$.

- Last-element Lomuto has bad splits on sorted, reverse-sorted, and all-equal input.
- A random pivot gives expected $O(n \log n)$ time for distinct keys, but still permits a bad run. Randomising Lomuto does not fix an all-equal array.
- **Median-of-three** chooses the middle-valued item among the first, middle, and last items. It often improves typical splits; it does not guarantee balanced partitions.
- **Introsort** uses a depth limit and switches to Heap Sort if partitions stay too uneven. This can guarantee $O(n \log n)$ worst time.

Before each Lomuto scan step, `[low ... i]` contains keys `<= pivot`, `[i + 1 ... j - 1]` contains keys `> pivot`, and `[j ... high - 1]` is unchecked. The final swap puts the pivot between the first two groups. Those groups need further sorting: partitioning alone does not sort them.

<a id="hoare-partition"></a>

### Hoare partition: two pointers moving inward

The left pointer stops at a value `>= pivot`. The right pointer stops at a value `<= pivot`. If they have not crossed, swap those misplaced values. Both scans move at least one position on the next round, even when values are equal.

```cpp
#include <vector>  // for dynamic arrays
#include <utility> // for swap, move, and pair
using namespace std;

int hoarePartition(vector<int>& arr, int low, int high){
    int pivot = arr[low + (high - low) / 2]; // Save the value, not a reference
    int i = low - 1;
    int j = high + 1;
    while(true){
        do {
            ++i;
        } while(arr[i] < pivot);
        do {
            --j;
        } while(arr[j] > pivot);
        if(i >= j){
            return j;
        }
        swap(arr[i], arr[j]);
    }
}

void quickSortHoare(vector<int>& arr, int low, int high){
    if(low >= high){
        return;
    }
    int split = hoarePartition(arr, low, high);
    quickSortHoare(arr, low, split);
    quickSortHoare(arr, split + 1, high);
}
```

**Important difference:** Hoare returns a **split boundary**, not the pivot's final index. Recurse on `[low ... split]` and `[split + 1 ... high]`. Using Lomuto's `split - 1` rule can leave an item unsorted. The midpoint pivot and these boundaries ensure the recursive ranges shrink.

| Scheme            | Return value                | Recursive ranges                        | Equal keys                                              |
| ----------------- | --------------------------- | --------------------------------------- | ------------------------------------------------------- |
| Lomuto shown here | Final pivot index `p`     | `[low ... p-1]`, `[p+1 ... high]`   | All equal gives very uneven splits                      |
| Hoare shown here  | Split boundary `s`        | `[low ... s]`, `[s+1 ... high]`     | Strict scans and pointer movement divide equal keys too |
| Three-way         | Equal range `[lt ... gt]` | `[low ... lt-1]`, `[gt+1 ... high]` | Skips the whole equal range in later recursion          |

Hoare often uses fewer swaps than Lomuto, but an exact ratio depends on the input and implementations. Ordinary two-sided Hoare recursion is not a general worst-case $O(\log n)$ stack guarantee.

---

### Step 5: Is QuickSort Universally Faster Than MergeSort? `[Level 2]`

No algorithm is universally superior under all constraints:

- **Where Quick Sort Often Helps:** Partitioning accesses arrays directly and avoids a linear merge buffer. This can be fast, but Merge Sort also uses sequential scans. Compare actual implementations before making a hardware speed claim.
- **Where MergeSort Wins:**
  1. **Stability:** MergeSort is stable; standard QuickSort is not.
  2. **Linked Lists:** Merging can change links without an array buffer. A recursive Merge Sort still uses $O(\log n)$ stack space; a bottom-up iterative version can use $O(1)$ extra space. Quick Sort can be written for lists too, but array-style indexing is unsuitable.
  3. **Expensive Copies / Swaps:** If moving or copying elements is much more costly than comparing them, MergeSort's predictable movement pattern can outperform QuickSort.
  4. **Guaranteed $O(n \log n)$:** MergeSort never suffers $O(n^2)$ worst-case degradation.

---

### Step 6: 3-Way Partitioning (Dutch National Flag) for Duplicate Keys `[Level 2]`

When an array contains many identical values (e.g. `[7, 7, 7, 7, 7]`), Lomuto partitioning degrades to $O(n^2)$. Dijkstra's 3-way partition groups the array into three segments:
`[ < pivot | == pivot | > pivot ]`

```cpp
#include <vector>  // for dynamic arrays
#include <utility> // for swap, move, and pair
using namespace std;

void quickSort3Way(vector<int>& arr, int low, int high){
    if(low >= high){
        return;
    }

    int pivot = arr[low + (high - low) / 2];
    int lt = low;  // arr[low ... lt-1] < pivot
    int gt = high; // arr[gt+1 ... high] > pivot
    int i = low;   // arr[lt ... i-1] == pivot

    while(i <= gt){
        if(arr[i] < pivot){
            swap(arr[lt++], arr[i++]);
        } else if(arr[i] > pivot){
            swap(arr[i], arr[gt--]); // Do NOT increment i! Re-examine incoming element
        } else {
            i++;
        }
    }

    quickSort3Way(arr, low, lt - 1);
    quickSort3Way(arr, gt + 1, high);
}
```

---

Three-way Quick Sort finishes all-equal input in $\Theta(n)$ time. It does not prevent every bad split on distinct keys. The middle-index pivot above is deterministic, so worst time remains $O(n^2)$ and worst recursive stack space remains $O(n)$.

<a id="7-heap-sort"></a>

# 7. Heap Sort `[Level 1 & Level 2]`

### Step 1: Plain-English Intuition `[Level 1]`

Imagine a tennis tournament bracket. Players compete in matches, and the winners move up. At the very top of the bracket sits the tournament champion (the maximum).In Heap Sort:

1. We organise the array as a **max-heap**: every parent is greater than or equal to its children. The maximum is at index 0. Siblings need not be ordered, so a heap is not a fully sorted array.
2. We swap the maximum number to the very end of the array (its correct final position).
3. We fix the remaining tournament bracket using a fast procedure called `heapify` so the next biggest number rises to the top.
4. We repeat this until the entire array is sorted!

---

### Step 2: The Binary Heap Array Trick `[Level 1]`

You can represent a complete binary tree inside an ordinary array without using any pointers:

- For any element at index $i$:
  - Its **Left Child** is at index: $2i + 1$
  - Its **Right Child** is at index: $2i + 2$
  - Its **Parent** is at index: $\lfloor(i - 1) / 2\rfloor$

The root has no parent. A **complete binary tree** fills each level before the next; the last level fills from left to right. Leaves start at index `n / 2`, so the last parent is `n / 2 - 1`. Building bottom-up ensures a parent's child subtrees are already heaps before we fix that parent.

---

### Step 3: Complete C++ Implementation `[Level 1]`

```cpp
#include <vector>  // for dynamic arrays
#include <utility> // for swap, move, and pair
using namespace std;

// Sifts down an element at index i until it satisfies the Max-Heap property
void heapify(vector<int>& arr, int n, int i){
    while(i < n / 2){ // Only parents have children; avoids large-index overflow
        int left = 2 * i + 1;
        int right = left + 1;
        int largest = left;
        if(right < n && arr[right] > arr[left]){
            largest = right;
        }
        if(arr[i] >= arr[largest]){
            break;
        }
        swap(arr[i], arr[largest]);
        i = largest; // Continue downward without recursion
    }
}

void heapSort(vector<int>& arr){
    int n = arr.size();
    if(n <= 1){
        return;
    }

    // Phase 1: Build the Max-Heap in O(n) time
    for(int i = n / 2 - 1; i >= 0; --i){
        heapify(arr, n, i);
    }

    // Phase 2: Extract elements from the heap one by one in O(n log n)
    for(int i = n - 1; i > 0; --i){
        swap(arr[0], arr[i]); // Move current maximum to the end
        heapify(arr, i, 0);   // Restore heap property on reduced heap
    }
}
```

---

### Step 4: Complexity & The "Build-Heap is $O(n)$" Insight `[Level 2]`

- **Time Complexity:** $O(n \log n)$ for every input; $\Theta(n \log n)$ worst time. With this early-stopping sift-down, all-equal input takes $\Theta(n)$, so claiming a tight $\Theta(n \log n)$ best bound for all inputs would be incorrect.
- **Auxiliary Space:** $O(1)$ for this iterative sift-down. A recursive sift-down adds $O(\log n)$ stack space.
- **Stability:** **Unstable** (swapping root to the end jumps over equal elements).
- **Why Building a Heap is $O(n)$:**
  Half of all nodes in a tree ($\approx n/2$) are leaves at the very bottom! They have height 0 and do **zero swaps**. Only the single root node can travel the full tree height. The work across all nodes mathematically converges to linear time $O(n)$.

**Small dry run:** Build a heap from `[4, 1, 3, 2]`. Fix parent 1 to get `[4, 2, 3, 1]`; parent 0 already fits. Move `4` to the end: `[1, 2, 3 | 4]`. Sift the root down: `[3, 2, 1 | 4]`. Extract again to get `[2, 1 | 3, 4]`, then `[1 | 2, 3, 4]`. The bar separates the active heap from the final sorted suffix.

For build-heap, the work is bounded by $n(1/4 + 2/8 + 3/16 + \cdots)$ plus linear checks. The series is bounded by a constant, so the total is $O(n)$. Here the term `h / 2^(h+1)` describes the proportion of nodes at height `h` multiplied by their maximum downward travel.

---

# Part C: Important Additional Algorithms

---

<a id="8-radix-sort"></a>

# 8. Radix Sort `[Level 2: Understand After the Basics]`

### Step 1: Plain-English Intuition

How did mechanical postal machines sort envelopes by 5-digit ZIP codes?
They sorted by the **ones digit** first (0 to 9), putting letters into 10 mail slots. Then they gathered them up, sorted by the **tens digit**, then the **hundreds digit**, and so on.
By the time they finished the highest digit, the entire pile of mail was in perfect numerical order!

**Dry run:** `[21, 13, 12]` becomes `[21, 12, 13]` after sorting by ones digits `1, 2, 3`. A stable tens-digit pass gives `[12, 13, 21]`. The order of `12` and `13` from the ones pass stays intact because their tens digits are equal.

**Why each pass must be stable:** After a pass, numbers are sorted by all digits processed so far. The next pass groups them by the next digit while preserving the earlier order inside each group. An unstable pass could undo the earlier work.

**LSD** means least significant digit first, such as ones before tens. **MSD** means most significant digit first; it separates items by their first digit and sorts inside those groups, often recursively. This module implements LSD. The stability requirement above is for LSD; MSD is a different procedure.

---

### Step 2: Input Assumptions & Limitations

> [!IMPORTANT]
> **Precondition: Non-Negative Integers!**
> The standard LSD Radix Sort implementation assumes non-negative integers (`>= 0`) because digit extraction using `(arr[i] / exp) % 10` requires non-negative values.
> **Signed keys need a different mapping:** Do not call `abs(INT_MIN)` or add a large offset in `int`; these can overflow. Under the 32-bit `int` assumption, compute shifted keys as `static_cast<long long>(value) - minimum`, and move each whole original record using that wider key. Splitting negatives and reversing their result needs care if equal-key record stability matters.

---

### Step 3: Complete C++ Implementation (LSD Base-10)

```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
#include <utility>   // for swap, move, and pair
#include <stdexcept> // for reporting invalid input
using namespace std;

void radixCountingSort(vector<int>& arr, long long exp){
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0};

    // Frequency of current digit
    for(int i = 0; i < n; ++i){
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    // Prefix sums
    for(int i = 1; i < 10; ++i){
        count[i] += count[i - 1];
    }

    // Backward loop preserves stability
    for(int i = n - 1; i >= 0; --i){
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    arr = move(output);
}

void radixSort(vector<int>& arr){
    // Verify precondition: non-negative integers only
    for(int x : arr){
        if(x < 0){
            throw invalid_argument("Radix Sort requires non-negative integers.");
        }
    }

    if(arr.size() <= 1){
        return;
    }

    int maxVal = *max_element(arr.begin(), arr.end());
    // Safe 'long long' exponent prevents signed 32-bit int overflow when maxVal is near INT_MAX
    for(long long exp = 1; maxVal / exp > 0; exp *= 10){
        radixCountingSort(arr, exp);
    }
}
```

- **Time Complexity:** $O(d \cdot (n + b))$, where $d$ is the number of digits in the maximum value and $b$ is the numerical base ($b = 10$).
- **Auxiliary Space:** $O(n + b)$.
- **Stability:** **Stable**.

---

<a id="9-bucket-sort"></a>

# 9. Bucket Sort `[Level 2: Understand After the Basics]`

### Step 1: Plain-English Intuition

Imagine sorting student test percentages. You set up 10 buckets for ranges `[0.0 - 0.1)`, `[0.1 - 0.2)`... `[0.9 - 1.0)`.

1. **Scatter:** Drop each number into its bucket.
2. **Sort:** Sort each bucket individually using Insertion Sort.
3. **Gather:** Concatenate the buckets in order.

---

### Step 2: Input Assumptions & Range Normalization

> [!IMPORTANT]
> **Precondition: Input values must be in $[0.0, 1.0)$!**
> Values must also be finite: reject NaN and infinity. Here `[0.0, 1.0)` includes 0 but excludes 1. For other known ranges, define a separate mapping and handle the maximum endpoint explicitly. Avoid adding an unexplained tiny epsilon: rounding can still put a value in the wrong bucket. If all values are equal, return before dividing by `max - min`.

---

### Step 3: Complete C++ Implementation

```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
#include <stdexcept> // for reporting invalid input
#include <cmath>     // for checking decimal values
#include <cstddef>   // for size_t, an array-size type
using namespace std;

void bucketSort(vector<double>& arr){
    int n = arr.size();

    // Validate precondition: all elements must be in [0.0, 1.0)
    for(double val : arr){
        if(!isfinite(val) || val < 0.0 || val >= 1.0){
            throw invalid_argument(
                "Bucket Sort precondition violated: values must be in [0.0, 1.0).");
        }
    }

    if(n <= 1){
        return;
    }

    vector<vector<double>> buckets(n);

    // 1. Scatter into buckets
    for(double val : arr){
        int idx = min(static_cast<int>(n * val), n - 1);
        buckets[idx].push_back(val);
    }

    // 2. Sort individual buckets
    // Stable insertion sort within each bucket
    for(auto& bucket : buckets){
        for(size_t i = 1; i < bucket.size(); ++i){
            double key = bucket[i];
            size_t j = i;
            while(j > 0 && bucket[j - 1] > key){
                bucket[j] = bucket[j - 1];
                --j;
            }
            bucket[j] = key;
        }
    }

    // 3. Gather back into arr
    int k = 0;
    for(int i = 0; i < n; ++i){
        for(double val : buckets[i]){
            arr[k++] = val;
        }
    }
}
```

- **Time Complexity:** Expected $O(n)$ for independently uniform values with $n$ buckets. Worst time is $O(n^2)$ with the insertion-sort subroutine above: one large, badly ordered bucket can require quadratic work. Clumping alone does not force that worst case if the bucket is already ordered.
- **Auxiliary Space:** $O(n + B)$ for $B$ buckets; this code sets $B = n$.
- **Stability:** Stable when items are appended in input order, equal keys map to the same bucket, and each bucket uses stable sorting. This example uses stable insertion sort. Using `sort` instead gives $O(n \log n)$ worst time and does not guarantee stability.

**Dry run:** For `[0.42, 0.12, 0.35, 0.18]`, four buckets receive `[0.12, 0.18]`, `[0.42, 0.35]`, `[]`, `[]`. Sort each bucket, then join: `[0.12, 0.18, 0.35, 0.42]`. A key from an earlier bucket cannot be greater than one from a later bucket, so sorting inside buckets completes the job.

---

<a id="10-shell-sort"></a>

# 10. Shell Sort `[Level 2: Understand After the Basics]`

### Step 1: Plain-English Intuition

Standard Insertion Sort is slow on reverse-sorted lists because numbers only move one position at a time.
**Shell Sort allows large leaps.** It compares elements separated by a gap $h$ (say, $h = 4$). It sorts those, then shrinks $h$ to $2$, and finally does an Insertion Sort pass with $h = 1$. The earlier passes eliminate long-range inversions, so elements generally have to shift fewer positions during the final $h = 1$ pass. However, the runtime of this final pass is not unconditionally linear: its exact cost depends on how well earlier passes ordered the array, which is dictated by the chosen gap sequence and the initial distribution of the input.

---

### Step 2: Why Complexity Depends Strictly on the Gap Sequence

> [!WARNING]
> **Shell Sort has NO single universal time complexity!**Its performance is completely determined by the sequence of gaps chosen:
>
> - **Shell's Original Gaps ($N/2, N/4, \dots, 1$):** Worst case $\Theta(n^2)$.
> - **Knuth's Gaps ($1, 4, 13, 40, \dots$):** $O(n^{3/2})$ worst time for the implementation below; sorted input takes $\Theta(n \log n)$ because it still checks each gap.
> - **Other gap families:** Hibbard and Sedgewick have their own bounds. Specify the exact sequence before attaching a bound; the family name alone can be ambiguous.
>   There is no known closed-form mathematical formula for the universal average-case complexity across arbitrary gap sequences.

---

### Step 3: Complete C++ Implementation (Knuth's Gap Sequence)

```cpp
#include <vector> // for dynamic arrays
using namespace std;

void shellSort(vector<int>& arr){
    int n = arr.size();
    if(n <= 1){
        return;
    }

    // Calculate initial gap using Knuth's sequence: 1, 4, 13, 40...
    int h = 1;
    while(h < n / 3){
        h = 3 * h + 1;
    }

    // Shrink gap until h reaches 1
    while(h >= 1){
        for(int i = h; i < n; ++i){
            int temp = arr[i];
            int j = i;
            // Shift elements separated by gap h
            while(j >= h && arr[j - h] > temp){
                arr[j] = arr[j - h];
                j -= h;
            }
            arr[j] = temp;
        }
        h /= 3; // Shrink gap
    }
}
```

---

For a gap of 4, index groups are `0, 4, 8, ...`, `1, 5, 9, ...`, and so on. Each pass uses Insertion Sort inside these groups. The final gap must be 1 to make the whole array sorted. For `[8, 7, 6, 5, 4, 3, 2, 1]`, gap 4 gives `[4, 3, 2, 1, 8, 7, 6, 5]`; gap 1 finishes the sort. Long jumps can move one equal-key record past another, so Shell Sort is unstable.

# Part D: Practical C++ STL Sorting

---

<a id="11-c-standard-library-sorting-functions"></a>

# 11. C++ Standard Library Sorting Functions `[Level 1: Must Understand Now]`

The C++ standard specifies results and comparison bounds. It does not require a particular sorting algorithm or a fixed small-array threshold. Introsort is a common implementation choice, not the definition of `sort`.

| Function              | What it gives                                                                                       | Comparison bound (ordinary, non-parallel overload)                    |
| --------------------- | --------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- |
| `sort`         | Whole range sorted; stability not guaranteed                                                        | $O(n \log n)$ worst case                                            |
| `stable_sort`  | Whole range sorted; equal-key order preserved                                                       | $O(n \log n)$ with enough extra memory, otherwise $O(n \log^2 n)$ |
| `partial_sort` | Smallest `k` values sorted at the front                                                           | About $O(n \log k)$ for `k >= 2`; `k = 1` is linear             |
| `nth_element`  | Item at index `k` is the one full sorting would put there; partitions are not individually sorted | $O(n)$ on average                                                   |

These bounds do not establish exact memory use for every standard-library implementation. `sort`, `stable_sort`, `partial_sort`, and `nth_element` need **random-access iterators**, which allow jumping to any index. Use `list.sort()` for `list`; `sort(list.begin(), list.end())` is invalid. The guarantees above are specified in the [C++ sorting clauses](https://eel.is/c++draft/alg.sort) and [nth-element clause](https://eel.is/c++draft/alg.nth.element).

```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
using namespace std;

void sortingLibraryDemo(){
    // 1. sort (often a hybrid such as Introsort)
    vector<int> a = {5, 2, 8, 1, 9};
    sort(a.begin(), a.end()); // Result: [1, 2, 5, 8, 9] (Unstable)

    // 2. stable_sort (Adaptive MergeSort)
    vector<int> b = {5, 2, 8, 1, 9};
    stable_sort(b.begin(), b.end()); // Result: [1, 2, 5, 8, 9] (Guaranteed Stable)

    // 3. partial_sort (Find and sort only the Top-3 smallest out of 10,000)
    vector<int> c = {10, 2, 8, 6, 7, 3, 1, 5};
    partial_sort(c.begin(), c.begin() + 3, c.end());
    // Indices [0, 1, 2] are now fully sorted: [1, 2, 3, ... (rest unsorted)]

    // 4. nth_element (QuickSelect: Find median or kth smallest in O(n) average)
    vector<int> d = {10, 2, 8, 6, 7, 3, 1, 5};
    nth_element(d.begin(), d.begin() + 3, d.end());
    // d[3] is now the exact 4th smallest number;
    // All elements before d[3] are <= d[3], all elements after are >= d[3]!
}
```

For `partial_sort`, require `0 <= k <= v.size()`. For a kth-smallest answer, require `1 <= k <= v.size()`, then call `nth_element` at `k - 1`. For kth largest, use index `v.size() - k`. Do not read `v[k]` when `k == v.size()`.

### Raw arrays, vectors, and descending order

```cpp
#include <vector>     // for dynamic arrays
#include <algorithm>  // for sorting and array helpers
#include <functional> // for comparison and callable types
using namespace std;

void ascendingAndDescendingDemo(){
    int arr[] = {3, 1, 7, 5, 4};
    int n = sizeof(arr) / sizeof(arr[0]); // Works here: arr is an actual array
    sort(arr, arr + n);                   // [1, 3, 4, 5, 7]
    sort(arr, arr + n, greater<int>{});   // [7, 5, 4, 3, 1]
    vector<int> v = {3, 1, 2};
    sort(v.begin(), v.end());
}
```

`arr + n` is one past the array, so it is the excluded endpoint. In a parameter such as `void f(int arr[])`, `arr` is a pointer: `sizeof(arr)` gives pointer size, not the original array length. Pass the length separately.

---

<a id="12-custom-comparators--the-fatal--bug"></a>

# 12. Custom Comparators & The `<=` Bug `[Level 1: Must Understand Now]`

### Multi-Key Sorting with Lambdas

```cpp
#include <vector>    // for dynamic arrays
#include <string>    // for text
#include <algorithm> // for sorting and array helpers
using namespace std;

struct Candidate {
    string name;
    int score;
    int id;
};

// Sort by score descending; if scores are equal, sort by id ascending
void sortCandidates(vector<Candidate>& candidates){
    sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b){
        if(a.score != b.score){
            return a.score > b.score; // High score first
        }
        return a.id < b.id; // Low ID breaks ties
    });
}
```

---

### The `<=` Bug Explained

> [!CAUTION]
> **Do not use `return a <= b` as your ordering rule.**
> In C++, comparators must satisfy **Strict Weak Ordering**: an element cannot be smaller than itself (`comp(x, x)` must evaluate to `false`).
> If you write `a <= b`, then for equal values `comp(pivot, pivot)` returns `true`, violating the irreflexivity requirement.
> Violating strict weak ordering results in **Undefined Behavior (UB)** under the ISO C++ standard.
> A crash, wrong output, or other unpredictable behaviour is possible. A successful run on one example does not make the comparator valid.

Read `comp(a, b)` as **"Should a come before b?"** It does not ask whether a and b are already correctly placed. For ascending integers use `<`; for descending integers use `>`.

The [C++ comparison rules](https://eel.is/c++draft/alg.sorting) require strict weak ordering:

1. **Irreflexive:** `comp(a, a)` is false.
2. **Asymmetric:** If a comes before b, b does not come before a.
3. **Transitive:** If a comes before b and b before c, a comes before c.
4. **Consistent ties:** Define a tie as neither item coming before the other. If a ties with b and b ties with c, a must tie with c.

Testing just rule 1 is not enough. A cyclic rule such as `0 before 1`, `1 before 2`, `2 before 0` passes that check but breaks transitivity. Do not compare by subtracting large signed integers: `a - b` can overflow; compare directly. Ordinary floating-point `<` on a domain including NaNs does not give the needed tie consistency; reject NaNs or define a suitable order.

For repeated stable sorts by several fields, sort the least important field first and the most important field last. Example: stable-sort by name, then stable-sort by grade; grade becomes the primary key and names stay ordered within equal grades.

---

# Part E: Essential Theory & System Concepts

---

<a id="13-why-comparison-sorts-cannot-beat-on-log-n-the-20-questions-game"></a>

# 13. Comparison Sorting Has a Worst-Case Lower Bound `[Level 2 & Level 3]`

### The Intuitive Explanation `[Level 2]`

In the game **20 Questions**, a well-chosen Yes/No question divides the possible answers into two groups. With balanced questions:

- 1 question can distinguish 2 possibilities ($2^1$).
- 20 questions can distinguish at most $2^{20} = 1,048,576$ possibilities.

When sorting $n$ numbers, how many different scrambled orders are possible? **$n!$** (n factorial).
Every time a comparison sort asks "Is A < B?", it gets a Yes/No answer. It can separate the possibilities into two groups, which are not necessarily equally large.
To identify the single correct sorted order among all $n!$ possibilities, it must ask at least $\log_2(n!)$ questions.
Mathematically, $\log_2(n!) \approx n \log_2 n$.
Therefore, **no comparison sort can beat $\Omega(n \log n)$ in the worst case.**

This argument is for arbitrary inputs of $n$ distinct keys. It does not forbid a linear best case on already-sorted input or linear algorithms restricted to three possible values. It is a [limit of the comparison model](https://courses.csail.mit.edu/6.006/fall09/lecture_notes/lecture10.pdf), not a limit on all sorting methods.

<details>
<summary><b>[Level 3: Optional Deep Dive] Formal Mathematical Proof via Stirling's Formula</b></summary>

1. A binary decision tree with $L$ leaves and height $h$ satisfies $2^h \ge L$.
2. To sort $n$ distinct items, there are $n!$ leaves $\implies 2^h \ge n! \implies h \ge \log_2(n!)$.
3. By Stirling's approximation: $\ln(n!) = n \ln n - n + O(\log n)$.
4. Converting to base-2 logarithms:
   $$
   \log_2(n!) = \frac{\ln(n!)}{\ln 2} = n \log_2 n - n \log_2 e + O(\log n) = \Omega(n \log n)
   $$

</details>

---

<a id="14-real-world-systems-external-sort-linked-lists-and-k-sorted-arrays"></a>

# 14. Real-World Systems: External Sort, Linked Lists, and $k$-Sorted Arrays `[Level 2 & Level 3]`

### 14.1 External Sorting (Sorting 10 Terabytes on 32 GB RAM) `[Level 2]`

1. **Pass 0 (Run Generation with Memory Headroom):** A 32 GB system cannot dedicate all 32 GB to input chunks because the operating system kernel, filesystem page cache, and I/O buffers require memory headroom. In practice, allocate a safe chunk size (e.g., 16 GB to 24 GB). Read each chunk into RAM, sort it with an efficient in-memory algorithm (like Introsort), and write the sorted "run" to disk. For 10 TB with 20 GB chunks, this produces $\approx 500$ sorted run files on disk.
2. **Pass 1 ($k$-Way Merge):** Open buffered read streams for all run files simultaneously. Load the first record of each file into a **Min-Heap** of size $k$ (e.g. 500). Repeatedly extract the minimum record to an output buffer, flush sequentially to disk when the buffer fills, and read the next record from the corresponding file stream into the heap.

If the buffers or open-file limit cannot support all runs at once, merge smaller groups in several rounds. Each full round reads and writes the data once; disk I/O matters more than counting comparisons alone. For stable output, sort each run stably and resolve equal merge keys using original run/order information. The example uses decimal units: `10 TB / 20 GB = 500`.

### 14.2 Why Merge Sort Wins on Linked Lists `[Level 2]`

Linked lists do not allow $O(1)$ random indexing. Merge Sort fits their sequential access: split with slow/fast pointers and merge by changing links. Merging needs $O(1)$ extra memory. Recursive list Merge Sort uses $O(\log n)$ stack space; bottom-up iterative list Merge Sort can use $O(1)$ total auxiliary space. List Quick Sort is possible, so lack of random indexing is not a proof that it cannot work.

### 14.3 Sorting $k$-Sorted (Nearly Sorted) Arrays `[Level 2]`

Maintain a **Min-Heap of size $k + 1$**. Push the first $k + 1$ elements. Pop the root to index 0, push the next element, and repeat.
For $k >= 1$, time is $O(n \log(k + 1))$ and extra space is $O(\min(n, k + 1))$. Use $O(n \log(k + 2))$ as a bound covering `k = 0` too; `log 0` is undefined. A fixed `k = 5` gives linear time. The displacement promise is stronger than simply saying the array looks nearly sorted.

---

<a id="course-problems"></a>

# Part F: Course Problems — Sorting and Rearranging Arrays

These problems are in the source course's sorting module. Next Permutation changes an arrangement; it does not fully sort the array on every call.

<a id="descending-order"></a>

## 15. Change the required order carefully

For `[3, 6, 2, 1, 8, 7, 4, 5, 3, 1]`, descending output must be `[8, 7, 6, 5, 4, 3, 3, 2, 1, 1]`.

| Algorithm          | Ascending decision                     | Descending decision                    |
| ------------------ | -------------------------------------- | -------------------------------------- |
| Bubble             | Swap if left `>` right               | Swap if left `<` right               |
| Selection          | Select the minimum                     | Select the maximum                     |
| Insertion          | Shift prefix items `>` key           | Shift prefix items `<` key           |
| Frequency Counting | Visit buckets from smallest to largest | Visit buckets from largest to smallest |

Use strict comparisons in Bubble and Insertion Sort to keep equal-key order. Sorting ascending and then reversing produces descending values, but reverses equal-key groups too. Use a descending comparator directly when stability matters.

For lowercase characters, the same Insertion Sort idea works:

```cpp
#include <string> // for text
using namespace std;

void insertionSortDescendingChars(string& text){
    for(int i = 1; i < static_cast<int>(text.size()); ++i){
        char key = text[i];
        int j = i - 1;
        while(j >= 0 && text[j] < key){
            text[j + 1] = text[j];
            --j;
        }
        text[j + 1] = key;
    }
}
```

`"fbaecd"` becomes `"fedcba"`. Worst time is $O(n^2)$, best time is $O(n)$, and extra space is $O(1)$. This exercise uses lowercase ASCII letters; sorting character codes is not a general rule for dictionary order across languages.

<a id="sort-colors"></a>

## 16. Sort Colors: DNF explained before coding

**Task:** Sort an array containing only `0`, `1`, and `2`.

Three approaches:

1. Full comparison sorting: $O(n \log n)$.
2. Count the three values, then write the correct number of each: $O(n)$ time, $O(1)$ space because there are exactly three counters. This uses two scans.
3. Dutch National Flag (**DNF**): classify items with swaps in one loop, $O(n)$ time and $O(1)$ space.

Keep four regions: `[0s | 1s | unknown | 2s]`. `low` starts the ones region, `mid` starts the unknown region, and `high` ends it.

| Value at `mid` | Action                                         | Why                                                                                                               |
| ---------------- | ---------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| `0`            | Swap with `low`; advance `low` and `mid` | If `low < mid`, the incoming item is a known `1`. If `low == mid`, it is a self-swap. Both are safe to skip |
| `1`            | Advance `mid`                                | This extends the ones region                                                                                      |
| `2`            | Swap with `high`; decrease `high`          | The incoming item is unknown, so inspect `mid` again                                                            |

The unknown region has `high - mid + 1` items. Each iteration reduces it by exactly one. Therefore the loop runs exactly `n` iterations. This is what **single pass** means here; the same index can be inspected more than once.

Dry run for `[2, 0, 2, 1, 1, 0]`:

| Action       | Array after action     | `low` | `mid` | `high` |
| ------------ | ---------------------- | ------: | ------: | -------: |
| Start        | `[2, 0, 2, 1, 1, 0]` |       0 |       0 |        5 |
| Move 2 right | `[0, 0, 2, 1, 1, 2]` |       0 |       0 |        4 |
| Place 0      | Same                   |       1 |       1 |        4 |
| Place 0      | Same                   |       2 |       2 |        4 |
| Move 2 right | `[0, 0, 1, 1, 2, 2]` |       2 |       2 |        3 |
| Accept 1     | Same                   |       2 |       3 |        3 |
| Accept 1     | Same                   |       2 |       4 |        3 |

Stop when `mid > high`. DNF keeps each whole item when swapping, but is unstable: tagged input `[2a, 2b, 0]` can become `[0, 2b, 2a]`. See the [complete implementation](./interview-prep.md#problem-2-sort-colors--dutch-national-flag-leetcode-75).

<a id="merge-sorted-arrays"></a>

## 17. Merge two sorted arrays into a padded array

**Task:** `nums1` has `m + n` actual vector elements. Its first `m` elements are sorted data; its final `n` elements are spare positions. `nums2` has `n` sorted elements. Put the merged result in `nums1`.

The spare positions may contain any values; `m` identifies valid data. A zero in the first `m` positions is real data. Calling `reserve(m + n)` alone is insufficient: it adds capacity but does not create writable vector elements. Use a vector of size `m + n`, or `resize` it first. The two vectors must be distinct.

**Simple approach:** Merge into a new vector using two forward pointers, then copy back. Time $O(m + n)$, extra space $O(m + n)$. Sorting all valid values together also works but costs $O((m + n)\log(m + n))$ comparisons.

**Better approach:** Fill spare positions from the back. The largest remaining value must go in the last remaining output position. Writing from the front could overwrite unread items in `nums1`.

```cpp
#include <vector>    // for dynamic arrays
#include <stdexcept> // for reporting invalid input
#include <cstddef>   // for size_t, an array-size type
using namespace std;

void mergeSortedArrays(vector<int>& nums1, int m, const vector<int>& nums2, int n){
    if(m < 0 || n < 0 || &nums1 == &nums2 ||
       static_cast<long long>(m) + n != static_cast<long long>(nums1.size()) ||
       static_cast<size_t>(n) != nums2.size()){
        throw invalid_argument("Invalid merge sizes or aliased arrays");
    }
    // Precondition: nums1[0..m) and nums2 are sorted; sizes fit int indices.
    int i = m - 1;
    int j = n - 1;
    int write = static_cast<int>(nums1.size()) - 1;
    while(j >= 0){
        if(i >= 0 && nums1[i] > nums2[j]){
            nums1[write--] = nums1[i--];
        } else {
            nums1[write--] = nums2[j--];
        }
    }
}
```

Why stop when `nums2` is exhausted? Then `write == i`, so remaining `nums1` items are already where they belong. While `j >= 0`, `write = i + j + 1 > i`; the write stays after unread `nums1` items. This explains why we do not lose data.

Dry run: `nums1 = [1, 2, 3, _, _, _]`, `nums2 = [2, 5, 6]`, `m = n = 3`. `_` means a spare position, not a C++ value.

| `i`, `j`, `write` before action | Write                  | `nums1` after action |
| ------------------------------------- | ---------------------- | ---------------------- |
| `2, 2, 5`                           | `6` from nums2       | `[1, 2, 3, _, _, 6]` |
| `2, 1, 4`                           | `5` from nums2       | `[1, 2, 3, _, 5, 6]` |
| `2, 0, 3`                           | `3` from nums1       | `[1, 2, 3, 3, 5, 6]` |
| `1, 0, 2`                           | `2` from nums2 (tie) | `[1, 2, 2, 3, 5, 6]` |

Worst time $O(m + n)$; auxiliary space $O(1)$. Empty `nums2` does no merge work; empty valid `nums1` copies all of `nums2`. For records, taking from the **right** input on a tie while filling backwards preserves left-input-before-right-input tie order in the final result.

`merge` needs a destination that does not overlap either input. It is not a direct substitute for the backward overwrite above. For two adjacent sorted ranges already in one vector, `inplace_merge` is another tool, but its name does not promise constant extra memory. See the [C++ merge requirements](https://eel.is/c++draft/alg.merge).

<a id="next-permutation"></a>

## 18. Next Permutation: the next arrangement in dictionary order

A **permutation** is an arrangement of the same items. **Lexicographic order** means compare from the left and use the first position where two arrangements differ. `[1, 3, 2]` comes after `[1, 2, 3]` because `3 > 2` at their first difference.

For three distinct values, the order is:

`[1,2,3] -> [1,3,2] -> [2,1,3] -> [2,3,1] -> [3,1,2] -> [3,2,1]`

**Task:** Produce the smallest arrangement greater than the current one. If none exists, return to the smallest arrangement. Do not simply sort each input: for `[1,2,3]`, the next answer is `[1,3,2]`, not `[1,2,3]`.

**Brute force:** Generate all unique permutations, sort them, and find the next one. There can be `n!` arrangements, so this becomes impractical very quickly.

**Linear approach, with reasons:**

1. Scan from the right for the first `i` with `nums[i] < nums[i + 1]`. The part after `i` is **non-increasing**: it may include equal values. It is already the largest arrangement of those suffix items, so changing the suffix alone cannot increase the whole array.
2. Increase `nums[i]` by the smallest amount possible: swap it with the rightmost suffix item strictly greater than it. Because the suffix is non-increasing, this is the smallest valid replacement.
3. Make the remaining suffix as small as possible. It is still non-increasing after the swap, so reversing it gives non-decreasing order in linear time.

Changing the rightmost possible position keeps the longest prefix unchanged. Using the smallest larger replacement and the smallest suffix then gives the immediate next arrangement.

```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
#include <utility>   // for swap, move, and pair
using namespace std;

bool nextPermutation(vector<int>& nums){
    int n = static_cast<int>(nums.size());
    if(n < 2){
        return false;
    }
    int pivot = n - 2;
    while(pivot >= 0 && nums[pivot] >= nums[pivot + 1]){
        --pivot;
    }
    if(pivot < 0){
        reverse(nums.begin(), nums.end());
        return false; // Wrapped to the smallest arrangement
    }
    int successor = n - 1;
    while(nums[successor] <= nums[pivot]){
        --successor;
    }
    swap(nums[pivot], nums[successor]);
    reverse(nums.begin() + pivot + 1, nums.end());
    return true;
}
```

Dry run: `[1, 2, 5, 4, 3]`. The suffix `[5, 4, 3]` is non-increasing. Pivot is index 1, value `2`. Rightmost greater value is `3`. Swap: `[1, 3, 5, 4, 2]`. Reverse the suffix `[5, 4, 2]`: answer `[1, 3, 2, 4, 5]`.

Duplicates work too: `[1, 5, 5] -> [5, 1, 5]`. Strictly greater replacement matters. `[3, 2, 1]` wraps to `[1, 2, 3]` and returns `false`; empty, one-item, and all-equal arrays also return `false`. Worst time $O(n)$, auxiliary space $O(1)$.

C++ provides `next_permutation(nums.begin(), nums.end())` with the same success/wrap meaning. Starting from sorted order and repeatedly calling it visits each distinct arrangement once, even with duplicates. See the [C++ permutation contract](https://eel.is/c++draft/alg.permutation.generators).

<a id="teach-back"></a>

# What you should be able to explain without looking

- Draw the active and finished regions of Bubble, Selection, and Insertion Sort.
- Explain why Selection's prefix is final while Insertion's prefix may move later.
- Trace negative-key Counting Sort and explain why cumulative count 5 means index 4.
- Explain why merge uses `<=` for stability, while a library comparator uses `<`.
- State the return meaning and recursion boundaries of Lomuto and Hoare.
- Explain why smaller-side recursion bounds stack memory without fixing worst time.
- Build a small heap, extract its maximum, and explain why a heap is not fully sorted.
- Explain Radix Sort stability and Bucket Sort's distribution assumption.
- Merge into padding without overwriting unread data, and explain all three Next Permutation steps.
- Choose an algorithm from the task's needs: full sort, stable order, small integer range, kth item, or only minimum/maximum.

If you cannot explain a step yet, retrace a three-item example. Use [practice](./practice.md) before the [revision sheet](./revision.md).

<a id="sources"></a>

# Sources and verification

Local source coverage and corrections are listed in the [audit report](./audit-report.md). Code examples use C++17. [verify_notes.py](./verify_notes.py) extracts the current complete implementations from these Markdown notes and checks them against standard-library results and small brute-force answers.

- [C++ draft: sorting](https://eel.is/c++draft/alg.sort) — required results and comparison bounds.
- [C++ draft: comparison rules](https://eel.is/c++draft/alg.sorting) — strict weak ordering.
- [C++ draft: nth element](https://eel.is/c++draft/alg.nth.element), [merge](https://eel.is/c++draft/alg.merge), and [permutations](https://eel.is/c++draft/alg.permutation.generators) — function contracts.
- [MIT 6.006: lower bounds, Counting and Radix Sort](https://courses.csail.mit.edu/6.006/fall09/lecture_notes/lecture10.pdf) — comparison-model limits and restricted integer sorting.
- [Princeton: Shell Sort with Knuth gaps](https://algs4.cs.princeton.edu/code/javadoc/edu/princeton/cs/algs4/Shell.html) — sequence-specific complexity.

The explanations and traces here are written for this learning package. The standard-library implementation choices and hardware speed comparisons are examples, not universal guarantees.
