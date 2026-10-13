# Sorting Algorithms — First-Principles Conceptual Theory

> **Module:** `unified-notes/DSA/Sorting/theory.md`  
> **Philosophy:** Learn from the ground up. Plain-English intuition first, concrete examples second, formal terms third, clean C++ code fourth, intuitive complexity fifth, and quick checks sixth.  
> **Target Audience:** Anyone seeking complete conceptual clarity, from foundational learners to candidates preparing for Google SDE interviews.

---

## 3-Level Learning Framework

To keep your study efficient and prevent cognitive overload, this guide is divided into three clear learning tiers:

- **🟢 Level 1: Must Understand Now**  
  *Core foundation:* What each algorithm does, how the code works, when it is useful, its time and space complexity, stability, in-place behavior, and how to dry-run it on a small array.
- **🟡 Level 2: Understand After the Basics**  
  *Conceptual reasoning:* Why Merge Sort is $O(n \log n)$, why building a heap is $O(n)$, why comparison sorts cannot beat $\Omega(n \log n)$, how QuickSort partitions work, and handling duplicate keys.
- **🔴 Level 3: Advanced Reference (Optional Deep Dive)**  
  *Specialized depth:* Detailed mathematical decision-tree proofs, recurrence expansions, external-sort I/O calculations, and hardware micro-architectural trade-offs. (Revisit these when preparing for senior interview follow-ups).

---

## Table of Contents

- [Module 0: The Friendly Foundation & Math Guide](#module-0-the-friendly-foundation--math-guide)
  - [0.1 What is Sorting and Why Does It Matter?](#01-what-is-sorting-and-why-does-it-matter) `[Level 1]`
  - [0.2 The 5 Core Questions to Ask About Any Sorting Algorithm](#02-the-5-core-questions-to-ask-about-any-sorting-algorithm) `[Level 1]`
  - [0.3 The Friendly Math Guide: How We Measure Growth](#03-the-friendly-math-guide-how-we-measure-growth) `[Level 1 & Level 2]`
- [Part A: Course-Taught Algorithms (Core Priority)](#part-a-course-taught-algorithms-core-priority)
  - [1. Bubble Sort](#1-bubble-sort) `[Level 1]`
  - [2. Selection Sort](#2-selection-sort) `[Level 1]`
  - [3. Insertion Sort](#3-insertion-sort) `[Level 1]`
  - [4. Counting Sort](#4-counting-sort) `[Level 1 & Level 2]`
- [Part B: Essential Interview Algorithms](#part-b-essential-interview-algorithms)
  - [5. Merge Sort](#5-merge-sort) `[Level 1 & Level 2]`
  - [6. Quick Sort](#6-quick-sort) `[Level 1 & Level 2]`
  - [7. Heap Sort](#7-heap-sort) `[Level 1 & Level 2]`
- [Part C: Important Additional Algorithms](#part-c-important-additional-algorithms)
  - [8. Radix Sort](#8-radix-sort) `[Level 2]`
  - [9. Bucket Sort](#9-bucket-sort) `[Level 2]`
  - [10. Shell Sort](#10-shell-sort) `[Level 2]`
- [Part D: Practical C++ STL Sorting](#part-d-practical-c-stl-sorting)
  - [11. C++ Standard Library Sorting Functions](#11-c-standard-library-sorting-functions) `[Level 1]`
  - [12. Custom Comparators & The Fatal `<=` Bug](#12-custom-comparators--the-fatal--bug) `[Level 1]`
- [Part E: Essential Theory & System Concepts](#part-e-essential-theory--system-concepts)
  - [13. Why Comparison Sorts Cannot Beat $O(n \log n)$ (The 20 Questions Game)](#13-why-comparison-sorts-cannot-beat-on-log-n-the-20-questions-game) `[Level 2 & Level 3]`
  - [14. Real-World Systems: External Sort, Linked Lists, and $k$-Sorted Arrays](#14-real-world-systems-external-sort-linked-lists-and-k-sorted-arrays) `[Level 2 & Level 3]`

---

# Module 0: The Friendly Foundation & Math Guide

## 0.1 What is Sorting and Why Does It Matter? `[Level 1: Must Understand Now]`

Imagine looking up a word in a dictionary where the pages were scattered at random. You would have to look through every single page from start to finish. But because the dictionary is **sorted alphabetically**, you can flip straight to the middle, skip thousands of irrelevant words, and find what you need in seconds.

In computer science, **sorting** means taking a collection of items (like numbers, names, or dates) and rearranging them into a specific order:
- **Ascending order:** Smallest to largest ($1, 2, 3, 4, 5$).
- **Descending order:** Largest to smallest ($5, 4, 3, 2, 1$).

Sorting is one of the most critical foundational operations in computing because it turns hard problems into easy ones:
- **Searching:** On an unsorted list of $n$ items, searching takes $n$ steps. On a sorted list, **Binary Search** finds any item in only $\log_2 n$ steps (for 1,000,000 items, that is 20 steps instead of 1,000,000!).
- **Finding Duplicates:** While a hash set can find duplicates in expected $O(n)$ time using extra memory, sorting offers an in-place alternative: once sorted, identical items sit right next to each other, allowing duplicates to be detected in a single linear scan without auxiliary hash table memory.
- **Finding Extremes:** The minimum is at the very beginning; the maximum is at the very end.

---

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
- **Plain English:** If two items have the **exact same value**, does the algorithm keep them in the same relative order they started in?
- **Real-Life Example:** Imagine a list of students already sorted alphabetically by name:
  `[(Alice, Grade B), (Bob, Grade A), (Charlie, Grade B)]`.
  If you re-sort by Grade:
  - A **Stable** sort guarantees `Alice` stays before `Charlie` because both got `Grade B` and Alice came first originally:  
    `[(Bob, Grade A), (Alice, Grade B), (Charlie, Grade B)]`.
  - An **Unstable** sort might flip them, placing `Charlie` before `Alice`.
- **Why this matters:** In databases, spreadsheets, and online stores, users constantly sort by one column, then by another (e.g., sort by Date, then sort by Price). If the sort is unstable, the earlier sort order is scrambled.

### 3. Does it need extra memory? (In-Place vs. Out-of-Place)
- **In-Place:** The algorithm sorts the elements directly inside the existing array by swapping them around. It uses almost zero extra memory ($O(1)$ extra space).
- **Out-of-Place:** The algorithm creates a brand-new copy or temporary helper array to do its work.

### 4. Does it adapt to already sorted data? (Adaptive)
- **Adaptive:** If you hand the algorithm an array that is already sorted (or nearly sorted), it detects this and finishes much faster (e.g., taking $n$ steps instead of $n^2$).
- **Non-Adaptive:** The algorithm does the exact same fixed number of steps regardless of whether the array was already sorted or completely scrambled.

### 5. How does it decide the order? (Comparison vs. Non-Comparison)
- **Comparison-Based:** Compares elements two at a time ("is $A$ smaller than $B$?"). Works for any data type you can compare.
- **Non-Comparison-Based:** Does not compare elements directly. Instead, it looks at the numerical digits or character codes of the values (like counting votes into buckets).

---

## 0.3 The Friendly Math Guide: How We Measure Growth `[Level 1 & Level 2]`

You do not need calculus or complex formulas to understand Big-O notation. Here is all the math you need, explained with simple numbers.

### What is $n$?
$n$ is simply the **number of items** in your list. If an array has 5 numbers, $n = 5$. If it has 1,000 numbers, $n = 1000$.

### What is an "Operation"?
One basic action a computer takes: comparing two numbers, swapping two numbers, or copying a number to another spot.

### The 4 Growth Speeds You Need to Know

| Notation | Name | Meaning in Plain English | Steps for $n = 10$ | Steps for $n = 1,000$ | Real-World Analogy |
|---|---|---|---|---|---|
| **$O(1)$** | Constant | Takes the exact same time no matter how big $n$ is. | 1 step | 1 step | Looking at the first card on top of a deck. |
| **$O(n)$** | Linear | Steps grow directly in proportion to $n$. Double the items $\to$ double the time. | 10 steps | 1,000 steps | Reading a book page by page from start to finish. |
| **$O(n^2)$** | Quadratic | Steps grow with $n \times n$. Double the items $\to$ **four times** the time! | 100 steps | 1,000,000 steps | Comparing every single person in a room with every other person. |
| **$O(\log_2 n)$** | Logarithmic | Steps grow by only 1 each time you **double** $n$! | ~3 steps | ~10 steps | Playing the guessing game: "Is your number higher or lower than 50?" |
| **$O(n \log n)$** | Linearithmic | The gold standard for fast comparison sorting. | ~33 steps | ~10,000 steps | Cutting a deck of cards in half repeatedly, then scanning each half. |

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
  - $n^2 = 1,000,000 \times 1,000,000 = 1,000,000,000,000$ (One Trillion steps! Takes hours).
  - $n \log_2 n = 1,000,000 \times 20 = 20,000,000$ (Twenty Million steps! Takes a fraction of a second).

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
Since **zero swaps occurred** during this entire pass, the array is already sorted! We stop early and do not waste time on further passes.

---

### Step 3: Formal Terminology
- **Pass ($i$):** One complete sweep through the unsorted portion of the array.
- **Adjacent Swap:** Exchanging two immediate neighbors `arr[j]` and `arr[j+1]`.
- **Adaptive Optimization:** Using a boolean flag (`swapped`) to detect if an array is already sorted so we can break out of the loop early.

---

### Step 4: Complete C++ Implementation
```cpp
#include <vector>
#include <utility> // for std::swap

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return; // An array of 0 or 1 item is already sorted

    // Outer loop: controls how many passes we make
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false; // Flag to track if any swap happens in this pass

        // Inner loop: compares neighboring pairs
        // Notice the limit: n - 1 - i. We don't need to check the last i items
        // because they are already sorted and locked in!
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) { // Use strict '>' to keep it stable
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped, the array is already completely sorted!
        if (!swapped) {
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

---

### Step 4: Complete C++ Implementation
```cpp
#include <vector>
#include <utility>

void selectionSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    // Loop through each position in the array
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i; // Assume the current spot has the minimum

        // Scan the rest of the array to find the true minimum
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j; // Found a smaller number, record its index
            }
        }

        // Swap the true minimum into its correct position
        if (minIdx != i) {
            std::swap(arr[i], arr[minIdx]);
        }
    }
}
```

---

### Step 5: Complexity & The "Flash Memory" Property
- **Time Complexity:** Strictly $\Theta(n^2)$ in **all cases** (Best, Average, and Worst). It always performs $\frac{n(n-1)}{2}$ comparisons because it cannot verify the minimum without inspecting every remaining item.
- **Auxiliary Space:** $O(1)$.
- **Stability:** **Unstable!**  
  *Proof by Counterexample:* Sort `[4a, 4b, 1]`.
  - Pass 0 finds minimum `1` at index 2.
  - It swaps `arr[0]` (`4a`) with `arr[2]` (`1`).
  - Result: `[1, 4b, 4a]`. `4b` now precedes `4a`! Their original order was reversed.
- **When is Selection Sort Actually Useful?**  
  When writing to memory is physically costly or slow. On **Flash memory** and **EEPROM** chips, writing wears down the silicon cells. Bubble Sort and Insertion Sort can make thousands of writes ($O(n^2)$), while Selection Sort makes at most $n - 1$ writes ($O(n)$ writes).

---

### Step 6: Verify Understanding
> **Question:** If an array is already sorted, does Selection Sort run faster?  
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

### Step 3: Shifting vs. Swapping (Why Shifting is 3x Faster)
Some beginners write Insertion Sort by repeatedly swapping: `swap(arr[j], arr[j-1])`.  
- Swapping two numbers takes **3 memory operations** (save to temp, copy $A$ to $B$, copy temp to $A$).
- **Shifting** saves the new card in a variable (`key`), slides older numbers one spot to the right with a single write (`arr[j+1] = arr[j]`), and writes `key` once at the end.  
**Shifting does about 3 times fewer writes than swapping!**

---

### Step 4: Complete C++ Implementation
```cpp
#include <vector>

void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    // Start at index 1 because a single element (index 0) is already sorted
    for (int i = 1; i < n; ++i) {
        int key = arr[i]; // The new card we want to insert
        int j = i - 1;    // Walk backwards through the sorted hand

        // Slide elements that are greater than key to the right
        while (j >= 0 && arr[j] > key) {
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
- **Why C++ `std::sort` Uses Insertion Sort:**  
  Modern C++ standard libraries use **Introsort**. When recursion breaks large arrays down into tiny pieces of 16 or fewer items, the library switches directly to **Insertion Sort**! For small arrays, Insertion Sort has virtually zero overhead, runs directly in CPU cache, and is faster than QuickSort.

---

### Step 6: Verify Understanding
> **Question:** An array has 1,000 numbers, but only 2 of them are out of order. Which algorithm will sort it fastest: Bubble, Selection, or Insertion Sort?  
> <details><summary><b>Click for Answer</b></summary>
> <b>Insertion Sort!</b> Its running time is directly proportional to the number of out-of-order pairs (inversions). On nearly-sorted data, Insertion Sort runs in almost pure linear $O(n)$ time.
> </details>

---

<a id="4-counting-sort"></a>
# 4. Counting Sort `[Level 1 & Level 2]`

### Step 1: Plain-English Intuition `[Level 1]`
Every sorting algorithm we looked at so far works by comparing two numbers: *"Is $A < B$?"* Because of this, they are called **Comparison Sorts**.

Counting Sort is completely different: **It never compares two numbers!**  
Imagine you are tallying votes in a school election where candidates are numbered 1 to 5:
1. You prepare 5 boxes labeled 1, 2, 3, 4, 5.
2. For every vote slip, you drop a tally mark into that candidate's box.
3. At the end, you look at the boxes: Candidate 1 got two votes, Candidate 2 got zero, Candidate 3 got one...
4. You write down the results in order: `1, 1, 3...`

Because you just counted frequencies, you sorted the list in **linear time $O(n)$**!

---

### Step 2: The Two Ways to Do Counting Sort `[Level 1]`

#### Way 1: The Simple Way (Frequency Overwrite)
Count frequencies, then loop through the counts and overwrite the array with numbers.
- **The Huge Flaw:** This only works for plain numbers! If you are sorting customer records by age (`{Age: 25, Name: "Alice"}`), overwriting only the age loses or scrambles the customer's name!

#### Way 2: The Stable Way (Prefix Sums & Output Array)
To sort real-world objects and preserve stability, we use **Prefix Sums (Cumulative Counts)**:
1. Count the frequencies of each number.
2. Add each count to the running total. Now, the count tells you **the exact finishing position** of that number in the sorted array!
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
   *What this means:* Numbers in bucket 0 ($-1$) end at index 2. Numbers in bucket 3 ($2$) end at index 3. Numbers in bucket 5 ($4$) end at index 5.

4. **Backward Placement (Guarantees Stability):**
   - Last item is `-1` (bucket 0). Running count is 2 $\to$ Place at index $2 - 1 = 1$. Decrement count to 1.
   - Next item is `4` (bucket 5). Running count is 5 $\to$ Place at index $5 - 1 = 4$. Decrement count to 4.
   - Next item is `2` (bucket 3). Running count is 3 $\to$ Place at index $3 - 1 = 2$. Decrement count to 2.
   - Next item is `-1` (bucket 0). Running count is 1 $\to$ Place at index $1 - 1 = 0$. Decrement count to 0.
   - First item is `4` (bucket 5). Running count is 4 $\to$ Place at index $4 - 1 = 3$. Decrement count to 3.

Result: `[-1, -1, 2, 4, 4]`. Perfectly sorted and 100% stable!

---

### Step 4: Complete C++ Implementation (Guarded Against Integer Overflow) `[Level 2]`
```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <climits>

void countingSort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;

    // Step 1: Find the minimum and maximum values
    int minVal = *std::min_element(arr.begin(), arr.end());
    int maxVal = *std::max_element(arr.begin(), arr.end());

    // CRITICAL OVERFLOW GUARD:
    // If minVal == INT_MIN and maxVal == INT_MAX, doing (maxVal - minVal + 1)
    // in standard 32-bit signed 'int' causes immediate arithmetic overflow!
    // We cast to 'long long' first to compute the true mathematical range safely.
    long long range = static_cast<long long>(maxVal) - minVal + 1;

    // Safety guard: Reject impractical allocations (e.g. > 10 million buckets)
    if (range > 10000000) {
        throw std::invalid_argument("Value range is too large for Counting Sort memory allocation.");
    }

    // Step 2: Build the frequency count array
    std::vector<int> count(range, 0);
    for (int num : arr) {
        count[num - minVal]++; // Offset by minVal to handle negative numbers safely!
    }

    // Step 3: Transform into prefix sums (cumulative counts)
    for (size_t i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }

    // Step 4: Build output array by traversing the original array backwards
    std::vector<int> output(arr.size());
    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
        int bucket = arr[i] - minVal;
        output[count[bucket] - 1] = arr[i];
        count[bucket]--; // Decrement so the next identical item goes to the left
    }

    // Copy sorted result back into arr
    arr = std::move(output);
}
```

---

### Step 5: Complexity & When Counting Sort FAILS `[Level 1]`
- **Time Complexity:** $O(n + k)$, where $n$ is the number of elements and $k$ is the range of values ($\text{max} - \text{min} + 1$).
- **Auxiliary Space:** $O(n + k)$ (needs a count array of size $k$ and an output array of size $n$).
- **The Catastrophic Trap (When NOT to use Counting Sort):**  
  Suppose you have an array with just **5 numbers**: `[1, 5, 2, 8, 1000000000]`.  
  Here, $n = 5$, but $k = 1,000,000,000$ (One Billion)! Counting Sort would try to allocate an array of 1 Billion integers (requiring 4 Gigabytes of RAM) just to sort 5 numbers. Meanwhile, QuickSort or MergeSort would sort those 5 numbers in less than 15 comparisons.  
  > **Rule of Thumb:** Use Counting Sort only when the range of values $k$ is not significantly larger than the number of elements $n$.

---

### Step 6: Verify Understanding
> **Question:** Why do we loop **backwards** (from $n-1$ down to $0$) when filling the output array in Step 4?  
> <details><summary><b>Click for Answer</b></summary>
> To preserve <b>stability</b>! Since <code>count[bucket]</code> points to the rightmost available slot for that value, reading from right to left ensures that the duplicate item that appeared later in the input gets placed in the rightmost slot, preserving the original relative order.
> </details>

---

# Part B: Essential Interview Algorithms

---

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
                       [ 5 , 2 ]          [ 4 , 1 , 3 ]
                        /     \            /         \
                      [ 5 ]   [ 2 ]      [ 4 ]     [ 1 , 3 ]
                                                    /     \
                                                  [ 1 ]   [ 3 ]
═════════════════════════════════════════════════════════════════════
MERGE BACK UP:
- Merge [1] and [3]       ───►  [ 1 , 3 ]
- Merge [4] and [1, 3]    ───►  [ 1 , 3 , 4 ]
- Merge [5] and [2]       ───►  [ 2 , 5 ]
- Merge [2, 5] and [1,3,4]───►  [ 1 , 2 , 3 , 4 , 5 ] (Done!)
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
#include <vector>

void merge(std::vector<int>& arr, int low, int mid, int high, std::vector<int>& temp) {
    int i = low;      // Pointer for left half
    int j = mid + 1;  // Pointer for right half
    int k = low;      // Pointer for temporary output buffer

    while (i <= mid && j <= high) {
        // Use '<=' to preserve STABILITY: if numbers are equal, take left half first!
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    // Copy any leftover elements from the left half
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // Copy any leftover elements from the right half
    while (j <= high) {
        temp[k++] = arr[j++];
    }

    // Copy merged results back into the original array
    for (int p = low; p <= high; ++p) {
        arr[p] = temp[p];
    }
}

void mergeSortInternal(std::vector<int>& arr, int low, int high, std::vector<int>& temp) {
    if (low >= high) return; // Base case: 0 or 1 element is already sorted

    // Safe midpoint calculation (prevents integer overflow)
    int mid = low + (high - low) / 2;

    mergeSortInternal(arr, low, mid, temp);      // Sort left half
    mergeSortInternal(arr, mid + 1, high, temp);  // Sort right half
    merge(arr, low, mid, high, temp);            // Merge both sorted halves
}

void mergeSort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;
    // Single upfront allocation to avoid repeating dynamic allocations inside recursion
    std::vector<int> temp(arr.size());
    mergeSortInternal(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}
```

---

### Step 5: Intuitive Complexity Proof (Why is it $O(n \log n)$?) `[Level 2]`
1. **Work per level:** On every level of the tree, all $n$ numbers participate in a merge step. So each level takes $n$ operations.
2. **Number of levels:** Because the array is cut in half at every step, the number of levels is simply the "halving counter": $\log_2 n$.
3. **Total Work:**  
   $$\text{Total Work} = (\text{Work per level}) \times (\text{Number of levels}) = n \times \log_2 n = \mathbf{O(n \log n)}$$
This holds true across **Best Case, Average Case, and Worst Case**!

- **Auxiliary Space:** $O(n)$ temporary buffer memory $+ O(\log n)$ recursion call stack.

---

### Step 6: Verify Understanding
> **Question:** Why do we write `mid = low + (high - low) / 2` instead of `mid = (low + high) / 2`?  
> <details><summary><b>Click for Answer</b></summary>
> To prevent <b>32-bit signed Integer Overflow</b>! If <code>low</code> and <code>high</code> are both large positive integers, <code>low + high</code> exceeds <code>INT_MAX</code> and turns negative, causing an out-of-bounds memory crash.
> </details>

---

# 6. Quick Sort `[Level 1 & Level 2]`

### Step 1: Plain-English Intuition `[Level 1]`
Merge Sort divides blindly down the middle and does all the hard work during the merge.  
**Quick Sort is the opposite: it does all the hard work up front!**

1. **Pick a Leader (The Pivot):** Choose one element from the array to be the "pivot" (for example, the last element).
2. **Partition:** Rearrange the array so that:
   - Every number **smaller** than the pivot is moved to the pivot's left.
   - Every number **greater** than the pivot is moved to the pivot's right.
3. **The Pivot is Done!** The pivot is now sitting in its exact, permanent sorted position!
4. **Repeat:** Recursively repeat the same process for the numbers on the left and the numbers on the right.

---

### Step 2: The Lomuto Partitioning Scheme (Step-by-Step) `[Level 1]`
Let's partition `[5, 2, 4, 1, 3]` using the last element (`3`) as the pivot:
- Maintain a pointer $i$ that marks the boundary of elements $\le \text{pivot}$. Start with $i = -1$.
- Scan $j$ from index $0$ to $3$:
  1. $j = 0$ (`5`): $5 > 3$, do nothing.
  2. $j = 1$ (`2`): $2 \le 3 \implies$ increment $i$ to $0$, swap `arr[0]` (`5`) and `arr[1]` (`2`):  
     Array becomes: `[2, 5, 4, 1, 3]`.
  3. $j = 2$ (`4`): $4 > 3$, do nothing.
  4. $j = 3$ (`1`): $1 \le 3 \implies$ increment $i$ to $1$, swap `arr[1]` (`5`) and `arr[3]` (`1`):  
     Array becomes: `[2, 1, 4, 5, 3]`.
- **Final step:** Swap the pivot (`3`) with `arr[i + 1]` (`4`):  
  Array becomes: `[2, 1, 3, 5, 4]`. Returns pivot index 2.

---

### Step 3: Complete C++ Implementation `[Level 1]`
```cpp
#include <vector>
#include <utility>

// Lomuto Partition Scheme
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Choose last element as pivot
    int i = low - 1;       // Boundary for elements <= pivot

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    // Place pivot in its correct sorted position
    std::swap(arr[i + 1], arr[high]);
    return i + 1; // Return the pivot's final index
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low >= high) return; // Base case: 0 or 1 element

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
void quickSortLogarithmicStack(std::vector<int>& arr, int low, int high) {
    while (low < high) {
        int pIdx = partition(arr, low, high);

        // Always recurse on the smaller partition; loop on the larger!
        if (pIdx - low < high - pIdx) {
            quickSortLogarithmicStack(arr, low, pIdx - 1); // Recurse on smaller left
            low = pIdx + 1;                                // Loop on larger right
        } else {
            quickSortLogarithmicStack(arr, pIdx + 1, high); // Recurse on smaller right
            high = pIdx - 1;                               // Loop on larger left
        }
    }
}
```
*Why this works:* Because the recursive call always processes a partition of size at most half ($\le n/2$), the call stack depth cannot exceed $\lfloor\log_2 n\rfloor$ frames!

---

### Step 5: Is QuickSort Universally Faster Than MergeSort? `[Level 2]`
No algorithm is universally superior under all constraints:
- **Where QuickSort Wins:** On contiguous arrays in RAM, QuickSort's sequential memory access provides superior CPU cache line reuse, and its in-place execution avoids dynamic buffer allocations.
- **Where MergeSort Wins:**  
  1. **Stability:** MergeSort is stable; standard QuickSort is not.
  2. **Linked Lists:** Merging linked lists requires zero buffer allocations ($O(1)$ extra space by pointer rewiring), while QuickSort cannot perform $O(1)$ random-access pivot indexing.
  3. **Expensive Copies / Swaps:** If moving or copying elements is much more costly than comparing them, MergeSort's predictable movement pattern can outperform QuickSort.
  4. **Guaranteed $O(n \log n)$:** MergeSort never suffers $O(n^2)$ worst-case degradation.

---

### Step 6: 3-Way Partitioning (Dutch National Flag) for Duplicate Keys `[Level 2]`
When an array contains many identical values (e.g. `[7, 7, 7, 7, 7]`), Lomuto partitioning degrades to $O(n^2)$. Dijkstra's 3-way partition groups the array into three segments:
`[ < pivot | == pivot | > pivot ]`
```cpp
void quickSort3Way(std::vector<int>& arr, int low, int high) {
    if (low >= high) return;

    int pivot = arr[low + (high - low) / 2];
    int lt = low;  // arr[low ... lt-1] < pivot
    int gt = high; // arr[gt+1 ... high] > pivot
    int i = low;   // arr[lt ... i-1] == pivot

    while (i <= gt) {
        if (arr[i] < pivot) {
            std::swap(arr[lt++], arr[i++]);
        } else if (arr[i] > pivot) {
            std::swap(arr[i], arr[gt--]); // Do NOT increment i! Re-examine incoming element
        } else {
            i++;
        }
    }

    quickSort3Way(arr, low, lt - 1);
    quickSort3Way(arr, gt + 1, high);
}
```

---

# 7. Heap Sort `[Level 1 & Level 2]`

### Step 1: Plain-English Intuition `[Level 1]`
Imagine a tennis tournament bracket. Players compete in matches, and the winners move up. At the very top of the bracket sits the tournament champion (the maximum).  
In Heap Sort:
1. We organize our array into a tournament bracket called a **Max-Heap**, where every parent number is greater than its children. The overall maximum number sits at index 0.
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

---

### Step 3: Complete C++ Implementation `[Level 1]`
```cpp
#include <vector>
#include <utility>

// Sifts down an element at index i until it satisfies the Max-Heap property
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // Left child index
    int right = 2 * i + 2; // Right child index

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest); // Recursively sift down
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    // Phase 1: Build the Max-Heap in O(n) time
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }

    // Phase 2: Extract elements from the heap one by one in O(n log n)
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]); // Move current maximum to the end
        heapify(arr, i, 0);        // Restore heap property on reduced heap
    }
}
```

---

### Step 4: Complexity & The "Build-Heap is $O(n)$" Insight `[Level 2]`
- **Time Complexity:** Strictly $\Theta(n \log n)$ across Best, Average, and Worst cases.
- **Auxiliary Space:** $O(1)$ (runs strictly in-place).
- **Stability:** **Unstable** (swapping root to the end jumps over equal elements).
- **Why Building a Heap is $O(n)$:**  
  Half of all nodes in a tree ($\approx n/2$) are leaves at the very bottom! They have height 0 and do **zero swaps**. Only the single root node can travel the full tree height. The work across all nodes mathematically converges to linear time $O(n)$.

---

# Part C: Important Additional Algorithms

---

# 8. Radix Sort `[Level 2: Understand After the Basics]`

### Step 1: Plain-English Intuition
How did mechanical postal machines sort envelopes by 5-digit ZIP codes?  
They sorted by the **ones digit** first (0 to 9), putting letters into 10 mail slots. Then they gathered them up, sorted by the **tens digit**, then the **hundreds digit**, and so on.  
By the time they finished the highest digit, the entire pile of mail was in perfect numerical order!

---

### Step 2: Input Assumptions & Limitations
> [!IMPORTANT]
> **Precondition: Non-Negative Integers!**  
> The standard LSD Radix Sort implementation assumes non-negative integers (`>= 0`) because digit extraction using `(arr[i] / exp) % 10` requires non-negative values.  
> **How to Handle Signed Negative Integers:** Either partition the array into negative and non-negative numbers, sort the absolute values of the negatives and reverse them, or add an offset `abs(minVal)` to shift all values into non-negative territory.

---

### Step 3: Complete C++ Implementation (LSD Base-10)
```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>

void radixCountingSort(std::vector<int>& arr, long long exp) {
    int n = arr.size();
    std::vector<int> output(n);
    int count[10] = {0};

    // Frequency of current digit
    for (int i = 0; i < n; ++i) {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    // Prefix sums
    for (int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }

    // Backward loop preserves stability
    for (int i = n - 1; i >= 0; --i) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    arr = std::move(output);
}

void radixSort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;

    // Verify precondition: non-negative integers only
    for (int x : arr) {
        if (x < 0) {
            throw std::invalid_argument("Radix Sort requires non-negative integers.");
        }
    }

    int maxVal = *std::max_element(arr.begin(), arr.end());
    // Safe 'long long' exponent prevents signed 32-bit int overflow when maxVal is near INT_MAX
    for (long long exp = 1; maxVal / exp > 0; exp *= 10) {
        radixCountingSort(arr, exp);
    }
}
```

- **Time Complexity:** $O(d \cdot (n + b))$, where $d$ is the number of digits in the maximum value and $b$ is the numerical base ($b = 10$).
- **Auxiliary Space:** $O(n + b)$.
- **Stability:** **Stable**.

---

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
> If your data contains arbitrary numbers outside this interval, you must first **normalize** the values:
> $$\text{normalized} = \frac{x - \text{min}}{\text{max} - \text{min} + \epsilon}$$

---

### Step 3: Complete C++ Implementation
```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>

void bucketSort(std::vector<double>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    // Validate precondition: all elements must be in [0.0, 1.0)
    for (double val : arr) {
        if (val < 0.0 || val >= 1.0) {
            throw std::invalid_argument("Bucket Sort precondition violated: values must be in [0.0, 1.0).");
        }
    }

    std::vector<std::vector<double>> buckets(n);

    // 1. Scatter into buckets
    for (double val : arr) {
        int idx = std::min(static_cast<int>(n * val), n - 1);
        buckets[idx].push_back(val);
    }

    // 2. Sort individual buckets
    // NOTE ON STABILITY: Using std::sort makes Bucket Sort UNSTABLE.
    // To make Bucket Sort STABLE, use std::stable_sort or Insertion Sort here!
    for (int i = 0; i < n; ++i) {
        std::sort(buckets[i].begin(), buckets[i].end());
    }

    // 3. Gather back into arr
    int k = 0;
    for (int i = 0; i < n; ++i) {
        for (double val : buckets[i]) {
            arr[k++] = val;
        }
    }
}
```

- **Time Complexity:** Average Case $O(n)$ if values are **uniformly distributed**. Worst Case $O(n^2)$ if all elements clump into a single bucket.
- **Auxiliary Space:** $O(n + k)$.
- **Stability:** **Not guaranteed stable** when implemented with `std::sort` within buckets. It is only stable if a stable sorting algorithm (such as `std::stable_sort` or Insertion Sort) is deliberately used to sort individual buckets.

---

# 10. Shell Sort `[Level 2: Understand After the Basics]`

### Step 1: Plain-English Intuition
Standard Insertion Sort is slow on reverse-sorted lists because numbers only move one position at a time.  
**Shell Sort allows large leaps.** It compares elements separated by a gap $h$ (say, $h = 4$). It sorts those, then shrinks $h$ to $2$, and finally does an Insertion Sort pass with $h = 1$. The earlier passes eliminate long-range inversions, so elements generally have to shift fewer positions during the final $h = 1$ pass. However, the runtime of this final pass is not unconditionally linear: its exact cost depends on how well earlier passes ordered the array, which is dictated by the chosen gap sequence and the initial distribution of the input.

---

### Step 2: Why Complexity Depends Strictly on the Gap Sequence
> [!WARNING]
> **Shell Sort has NO single universal time complexity!**  
> Its performance is completely determined by the sequence of gaps chosen:
> - **Shell's Original Gaps ($N/2, N/4, \dots, 1$):** Worst case $\Theta(n^2)$.
> - **Hibbard's Gaps ($1, 3, 7, 15, \dots, 2^k - 1$):** Worst case $\Theta(n^{3/2}) \approx O(n^{1.5})$.
> - **Knuth's Gaps ($1, 4, 13, 40, \dots, \frac{3^k - 1}{2}$):** Worst case $\Theta(n^{3/2}) \approx O(n^{1.5})$.
> - **Sedgewick's Gaps:** Worst case $\Theta(n^{4/3}) \approx O(n^{1.33})$.
> There is no known closed-form mathematical formula for the universal average-case complexity across arbitrary gap sequences.

---

### Step 3: Complete C++ Implementation (Knuth's Gap Sequence)
```cpp
#include <vector>

void shellSort(std::vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    // Calculate initial gap using Knuth's sequence: 1, 4, 13, 40...
    int h = 1;
    while (h < n / 3) {
        h = 3 * h + 1;
    }

    // Shrink gap until h reaches 1
    while (h >= 1) {
        for (int i = h; i < n; ++i) {
            int temp = arr[i];
            int j = i;
            // Shift elements separated by gap h
            while (j >= h && arr[j - h] > temp) {
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

# Part D: Practical C++ STL Sorting

---

# 11. C++ Standard Library Sorting Functions `[Level 1: Must Understand Now]`

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

// 1. std::sort (Introsort: QuickSort + HeapSort + InsertionSort)
std::vector<int> a = {5, 2, 8, 1, 9};
std::sort(a.begin(), a.end()); // Result: [1, 2, 5, 8, 9] (Unstable)

// 2. std::stable_sort (Adaptive MergeSort)
std::vector<int> b = {5, 2, 8, 1, 9};
std::stable_sort(b.begin(), b.end()); // Result: [1, 2, 5, 8, 9] (Guaranteed Stable)

// 3. std::partial_sort (Find and sort only the Top-3 smallest out of 10,000)
std::vector<int> c = {10, 2, 8, 6, 7, 3, 1, 5};
std::partial_sort(c.begin(), c.begin() + 3, c.end());
// Indices [0, 1, 2] are now fully sorted: [1, 2, 3, ... (rest unsorted)]

// 4. std::nth_element (QuickSelect: Find median or kth smallest in O(n) average)
std::vector<int> d = {10, 2, 8, 6, 7, 3, 1, 5};
std::nth_element(d.begin(), d.begin() + 3, d.end());
// d[3] is now the exact 4th smallest number;
// All elements before d[3] are <= d[3], all elements after are >= d[3]!
```

---

# 12. Custom Comparators & The Fatal `<=` Bug `[Level 1: Must Understand Now]`

### Multi-Key Sorting with Lambdas
```cpp
struct Candidate {
    std::string name;
    int score;
    int id;
};

// Sort by score descending; if scores are equal, sort by id ascending
std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
    if (a.score != b.score) {
        return a.score > b.score; // High score first
    }
    return a.id < b.id; // Low ID breaks ties
});
```

---

### The `<=` Bug Explained
> [!CAUTION]
> **NEVER write `<=` or `>=` inside a C++ comparator!**  
> In C++, comparators must satisfy **Strict Weak Ordering**: an element cannot be smaller than itself (`comp(x, x)` must evaluate to `false`).  
> If you write `a <= b`, then for equal values `comp(pivot, pivot)` returns `true`, violating the irreflexivity requirement.  
> Violating strict weak ordering results in **Undefined Behavior (UB)** under the ISO C++ standard.  
> In many standard library implementations (such as GNU libstdc++), the inner partitioning loop runs without boundary checks for CPU performance reasons:
> ```cpp
> while (comp(*first, pivot)) ++first;
> ```
> The algorithm relies on `comp(pivot, pivot) == false` as an in-buffer sentinel to stop pointer advancement. When `comp` returns `true`, the pointer can advance past allocated memory bounds. While this frequently causes a segmentation fault on such runtimes, **a crash is possible, not guaranteed**; other manifestations include silent memory corruption, invalid sorting order, or infinite loops.

---

# Part E: Essential Theory & System Concepts

---

# 13. Why Comparison Sorts Cannot Beat $O(n \log n)$ (The 20 Questions Game) `[Level 2 & Level 3]`

### The Intuitive Explanation `[Level 2]`
In the game **20 Questions**, each Yes/No question cuts the remaining possibilities in half:
- 1 question narrows down 2 possibilities ($2^1$).
- 20 questions narrows down $2^{20} = 1,048,576$ possibilities!

When sorting $n$ numbers, how many different scrambled orders are possible? **$n!$** (n factorial).  
Every time a comparison sort asks: *"Is $A < B$?"*, it gets a Yes/No answer, cutting the possible scrambled orders in half.  
To identify the single correct sorted order among all $n!$ possibilities, it must ask at least $\log_2(n!)$ questions.  
Mathematically, $\log_2(n!) \approx n \log_2 n$.  
Therefore, **no comparison sort can beat $\Omega(n \log n)$ in the worst case.**

<details>
<summary><b>[Level 3: Optional Deep Dive] Formal Mathematical Proof via Stirling's Formula</b></summary>

1. A binary decision tree with $L$ leaves and height $h$ satisfies $2^h \ge L$.
2. To sort $n$ distinct items, there are $n!$ leaves $\implies 2^h \ge n! \implies h \ge \log_2(n!)$.
3. By Stirling's approximation: $\ln(n!) = n \ln n - n + O(\log n)$.
4. Converting to base-2 logarithms:
   $$\log_2(n!) = \frac{\ln(n!)}{\ln 2} = n \log_2 n - n \log_2 e + O(\log n) = \Omega(n \log n)$$
</details>

---

# 14. Real-World Systems: External Sort, Linked Lists, and $k$-Sorted Arrays `[Level 2 & Level 3]`

### 14.1 External Sorting (Sorting 10 Terabytes on 32 GB RAM) `[Level 2]`
1. **Pass 0 (Run Generation with Memory Headroom):** A 32 GB system cannot dedicate all 32 GB to input chunks because the operating system kernel, filesystem page cache, and I/O buffers require memory headroom. In practice, allocate a safe chunk size (e.g., 16 GB to 24 GB). Read each chunk into RAM, sort it with an efficient in-memory algorithm (like Introsort), and write the sorted "run" to disk. For 10 TB with 20 GB chunks, this produces $\approx 500$ sorted run files on disk.
2. **Pass 1 ($k$-Way Merge):** Open buffered read streams for all run files simultaneously. Load the first record of each file into a **Min-Heap** of size $k$ (e.g. 500). Repeatedly extract the minimum record to an output buffer, flush sequentially to disk when the buffer fills, and read the next record from the corresponding file stream into the heap.

### 14.2 Why Merge Sort Wins on Linked Lists `[Level 2]`
Linked lists do not allow $O(1)$ random indexing, making QuickSort and HeapSort slow. But Merge Sort splits a list in $O(n)$ with slow/fast pointers, and merging rewires pointers with **$O(1)$ extra memory**, eliminating the array buffer penalty!

### 14.3 Sorting $k$-Sorted (Nearly Sorted) Arrays `[Level 2]`
Maintain a **Min-Heap of size $k + 1$**. Push the first $k + 1$ elements. Pop the root to index 0, push the next element, and repeat.  
Time complexity is $O(n \log k)$. If $k = 5$, this runs in **pure linear time $O(n)$**!
