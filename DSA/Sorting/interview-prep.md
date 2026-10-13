# Sorting Algorithms — Google SDE Interview Preparation Guide

> **Module:** `unified-notes/DSA/Sorting/interview-prep.md`  
> **Target Audience:** Google SDE I / II / Senior candidates, competitive programmers, and technical interviewees.  
> **Focus:** Architectural trade-offs, rigorous complexity proofs, core coding problems, edge-case traps, and verbal interview articulation.

---

## 1. Fundamental Conceptual Questions

### Q1: What is a Stable Sort, and why is it critical in real-world systems?
**Model Spoken Answer:**  
> "A sorting algorithm is stable if elements with identical keys maintain their original relative order from the input in the sorted output. Stability is indispensable in multi-attribute sorting. For instance, in an e-commerce platform where items are already sorted chronologically by purchase date, if a user re-sorts by price using an unstable algorithm, items with identical prices will have their chronological order scrambled. A stable sort preserves the earlier date ordering among items with identical prices."

---

### Q2: What is the theoretical lower bound for comparison-based sorting, and how do linear sorts bypass it?
**Model Spoken Answer:**  
> "Any comparison-based sort has an absolute worst-case lower bound of $\Omega(n \log n)$ comparisons.  
> **The Intuition:** Think of the game '20 Questions'. Every pairwise comparison provides at most 1 bit of information (Yes or No), cutting remaining possibilities in half. Since $n$ distinct numbers can be arranged in $n!$ possible scrambled permutations, any algorithm must ask at least $\log_2(n!) \approx n \log_2 n$ comparisons to pinpoint the correct sorted order.  
> Non-comparison algorithms like Counting Sort and Radix Sort bypass this barrier because they do not rely on pairwise comparisons. Instead, they treat keys as numerical indices or sequences of digits, directly computing target positions in $O(n + k)$ or $O(d(n + b))$ time."

---

### Q3: Why is Selection Sort considered non-adaptive and unstable?
**Model Spoken Answer:**  
> "Selection Sort is non-adaptive because it always scans the entire unsorted suffix to find the minimum element, performing exactly $\frac{n(n-1)}{2}$ comparisons regardless of whether the input is already sorted, reversed, or random.  
> The conventional swap-based implementation is unstable because long-range swaps move elements over identical keys. For example, sorting `[4a, 4b, 1]` swaps `4a` with `1`, producing `[1, 4b, 4a]`, which reverses the original relative order of `4a` and `4b`."

---

## 2. Intermediate Conceptual Questions

### Q4: Is QuickSort universally faster than MergeSort and HeapSort on modern CPUs?
**Model Spoken Answer:**  
> "QuickSort is often faster in practice on contiguous in-memory arrays, but it is **not universally faster**:
> - **Where QuickSort excels on arrays:** (1) Contiguous cache line traversal yields high CPU cache hit rates; (2) In-place execution avoids the memory bandwidth overhead of dynamic auxiliary allocations; (3) Partitioning loops have minimal constant-factor instructions per comparison.
> - **Where MergeSort excels:** (1) When **stability** is strictly required; (2) On **linked lists**, where MergeSort requires $O(1)$ extra space by pointer rewiring while QuickSort suffers from lack of $O(1)$ random indexing; (3) When **element copying or swapping is very costly** relative to comparisons; (4) When a **hard $O(n \log n)$ worst-case guarantee** is needed without randomized overhead. Actual performance always depends on compiler optimization, data types, and hardware."

---

### Q5: How can QuickSort's worst-case $O(n^2)$ time and deep stack usage be avoided in production C++?
**Model Spoken Answer:**  
> "Three industry-standard techniques prevent degeneration:
> 1. **Randomized Pivot or Median-of-Three:** Selecting a randomized element or the median of `first`, `middle`, and `last` prevents predictable worst-case splits on sorted or reverse-sorted inputs.
> 2. **Explicit Bounded Call Stack:** Because C++ compilers do not guarantee tail-call optimization, we explicitly recurse into the *smaller* partition first and loop over the *larger* partition using a `while` loop. This strictly caps call-stack depth at $\lfloor\log_2 n\rfloor$ frames.
> 3. **Introsort Bailout:** Tracking recursion depth and falling back to HeapSort if depth exceeds $2\lfloor\log_2 n\rfloor$ provides a hard theoretical guarantee of $O(n \log n)$ worst-case time."

---

### Q6: Why is `std::sort` in C++ not stable, and what does `std::stable_sort` do instead?
**Model Spoken Answer:**  
> "The C++ standard mandates time complexity ($O(n \log n)$) rather than specific algorithms, but in major standard libraries (such as GCC's libstdc++ and LLVM's libc++), `std::sort` is commonly implemented using **Introsort** (QuickSort + HeapSort + InsertionSort). Because it performs non-adjacent swaps, it is unstable.  
> When stability is required, C++ provides `std::stable_sort`, commonly implemented using an **adaptive MergeSort**. If sufficient temporary memory is available, it allocates an $O(n)$ buffer and runs in $O(n \log n)$. If system memory is constrained, it falls back to an in-place merge algorithm running in $O(n \log^2 n)$ time."

---

## 3. Advanced Architectural Questions

### Q7: Explain the Strict Weak Ordering axioms for C++ comparators and the consequence of using `<=`.
**Model Spoken Answer:**  
> "A comparator in C++ must define a Strict Weak Ordering, which requires four mathematical properties: Irreflexivity (`comp(x, x) == false`), Asymmetry, Transitivity, and Transitivity of Incomparability.  
> Writing `<=` (e.g., `return a <= b;`) violates **Irreflexivity** because when `a == b`, `a <= a` evaluates to `true`. Under the ISO C++ standard, violating strict weak ordering triggers **Undefined Behavior (UB)**. In practice, standard library partitioning loops (like Hoare's partition in `std::sort`) advance pointers while `comp(*ptr, pivot)` is true without boundary checks, relying on `comp(pivot, pivot) == false` as an in-buffer sentinel. When it returns `true`, the pointer can run past array bounds. While a segmentation fault is a common symptom on many platforms, a crash is **possible, not guaranteed**; other manifestations include silent memory corruption, incorrect sorting order, or infinite loops."

---

### Q8: How would you sort 10 Terabytes of data on a server with only 32 Gigabytes of RAM?
**Model Spoken Answer:**  
> "This requires **External Merge Sort**:
> 1. **Run Generation with Memory Headroom:** A 32 GB machine cannot devote all 32 GB directly to input data; the operating system kernel, page cache, runtime overhead, and I/O buffers require memory headroom. In practice, allocate a safe chunk size (e.g., 16 GB to 24 GB). Stream each chunk into RAM, sort it using an efficient in-memory algorithm (like Introsort), and write the sorted 'run' back to disk. (For 10 TB with 20 GB chunks, this produces $\approx 500$ sorted run files on disk).
> 2. **$k$-Way Merge:** Open a buffered read stream for all run files simultaneously. Populate a Min-Heap (priority queue) of size $k$ (e.g., 500) where each entry is `(record, run_id)`.
> 3. **Streaming Output:** Repeatedly extract the minimum record from the heap, append it to an output disk buffer, and read the next record from the corresponding run stream into the heap. When the output buffer fills, flush it sequentially to disk."

---

## 4. Practical C++ STL Interview Tools

In Google interviews, knowing when to use standard selection and partial sorting primitives can turn an $O(n \log n)$ problem into an $O(n)$ or $O(n \log k)$ solution:

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

// Tool 1: std::partial_sort — Finding and sorting only Top-k elements
// Complexity: O(n log k) time, O(1) space
void demoPartialSort() {
    std::vector<int> v = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    // Sort only the 3 smallest elements into v[0...2]
    std::partial_sort(v.begin(), v.begin() + 3, v.end());
    // v[0], v[1], v[2] are now [1, 2, 3]; remaining elements are in unspecified order.
}

// Tool 2: std::nth_element — QuickSelect in O(n) average time
// Complexity: O(n) average time, O(1) space
// INDEXING CONVENTION:
// v.begin() + k places the element that belongs at ZERO-BASED index k.
// In 1-based human terms, this is the (k + 1)th smallest element.
// (e.g. k = 0 finds the 1st smallest/minimum; k = 4 finds the 5th smallest / median).
void demoNthElement() {
    std::vector<int> v = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    // Place the element that belongs at index 4 (the 5th smallest element)
    std::nth_element(v.begin(), v.begin() + 4, v.end());
    // v[4] is now 5. All elements before index 4 are <= 5; all elements after are >= 5.
}
```

---

## 5. Deep Comparison Bank

| Comparison | Key Decision Criteria | Winner / Recommendation |
|---|---|---|
| **Merge Sort vs. Quick Sort (Arrays)** | Memory vs. Speed | **Quick Sort** typically wins on cache speed and $O(1)$ heap memory; Merge Sort wins if stability is strictly required. |
| **Merge Sort vs. Quick Sort (Linked Lists)** | Random Access vs. Pointers | **Merge Sort** wins decisively: $O(1)$ auxiliary memory (pointer rewiring) and no random access required. |
| **Heap Sort vs. Quick Sort** | Worst-Case Guarantee vs. Cache Hits | **Quick Sort** is often faster in practice due to contiguous cache lines; Heap Sort is useful when $O(n \log n)$ must be guaranteed with strict $O(1)$ memory. |
| **Counting Sort vs. Quick Sort** | Value Range ($k$) vs. Input Size ($n$) | **Counting Sort** wins when $k \le O(n)$ (integers); Quick Sort wins when $k \gg n \log n$ or sorting non-integer types. |
| **Insertion Sort vs. Selection Sort** | Adaptivity & Writes | **Insertion Sort** wins for nearly sorted data ($O(n)$) and general use; **Selection Sort** wins only when memory writes must be strictly minimized ($N-1$ writes). |
| **Shell Sort vs. Insertion Sort** | Gap Leaps vs. Single Shifts | **Shell Sort** beats Insertion Sort on larger, reverse-sorted arrays by allowing long-range leaps; complexity depends strictly on the gap sequence (e.g. $O(n^{1.5})$ with Knuth gaps). |

---

## 6. Core Coding Interview Problems (Google SDE Level)

### Problem 1: Merge Intervals (LeetCode 56)

#### Problem Statement
Given an array of `intervals` where `intervals[i] = [start_i, end_i]`, merge all overlapping intervals, and return an array of the non-overlapping intervals that cover all the intervals in the input.

#### Clarification Questions for Interviewer
- Are the intervals guaranteed to have `start <= end`? (Assume yes).
- Can the input array be empty? (Handle `size <= 1` gracefully).
- Can the intervals be returned in any order? (Standard is sorted by start time).

#### Optimal Approach
1. Sort intervals by start time: $O(n \log n)$.
2. Initialize `merged` list with the first interval.
3. Iterate through remaining intervals:
   - If current interval starts after the last merged interval ends (`curr.start > merged.back().end`), no overlap: append `curr`.
   - Otherwise, overlap: update `merged.back().end = max(merged.back().end, curr.end)`.

#### Verified C++ Implementation
```cpp
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>>& intervals) {
    if (intervals.size() <= 1) return intervals;

    // Step 1: Sort by start time
    std::sort(intervals.begin(), intervals.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        return a[0] < b[0];
    });

    std::vector<std::vector<int>> merged;
    merged.push_back(intervals[0]);

    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] <= merged.back()[1]) {
            // Overlapping intervals: expand current interval
            merged.back()[1] = std::max(merged.back()[1], intervals[i][1]);
        } else {
            // Disjoint interval: push as new entry
            merged.push_back(intervals[i]);
        }
    }

    return merged;
}
```

- **Time Complexity:** $O(n \log n)$ due to sorting. The linear scan takes $O(n)$.
- **Space Complexity:** $O(1)$ auxiliary space beyond the output array.

---

### Problem 2: Sort Colors / Dutch National Flag (LeetCode 75)

#### Problem Statement
Given an array `nums` with $n$ objects colored red, white, or blue (represented as $0, 1, 2$), sort them in-place so that objects of the same color are adjacent, with the colors in the order $0, 1, 2$. Must be solved in a single pass with $O(1)$ auxiliary space.

#### 3-Pointer Invariant Mechanics
- `nums[0 ... low - 1]` contains `0`s.
- `nums[low ... mid - 1]` contains `1`s.
- `nums[mid ... high]` contains uninspected elements.
- `nums[high + 1 ... n - 1]` contains `2`s.

#### Verified C++ Implementation
```cpp
#include <vector>
#include <utility>

void sortColors(std::vector<int>& nums) {
    int low = 0;
    int mid = 0;
    int high = static_cast<int>(nums.size()) - 1;

    while (mid <= high) {
        if (nums[mid] == 0) {
            std::swap(nums[low], nums[mid]);
            low++;
            mid++;
        } else if (nums[mid] == 1) {
            mid++;
        } else { // nums[mid] == 2
            std::swap(nums[mid], nums[high]);
            high--;
            // Do NOT increment mid! Element swapped from high is unexamined.
        }
    }
}
```

- **Time Complexity:** $O(n)$ strictly single-pass.
- **Space Complexity:** $O(1)$ auxiliary space.

---

### Problem 3: Count Inversions in an Array

#### Problem Statement
An inversion is a pair of indices $(i, j)$ such that $i < j$ and $arr[i] > arr[j]$. Count the total number of inversions in $O(n \log n)$ time.

#### Algorithmic Insight
Modify Merge Sort: During the merge step, when an element from the right subarray `arr[j]` is smaller than `arr[i]`, then `arr[j]` is smaller than **all** remaining elements in the left subarray from `i` to `mid`.  
Therefore, add `(mid - i + 1)` directly to the inversion count!

#### Verified C++ Implementation
```cpp
#include <vector>

long long mergeAndCount(std::vector<int>& arr, int low, int mid, int high, std::vector<int>& temp) {
    int i = low;
    int j = mid + 1;
    int k = low;
    long long count = 0;

    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            count += (mid - i + 1); // Key inversion accumulator
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= high) temp[k++] = arr[j++];

    for (int p = low; p <= high; ++p) arr[p] = temp[p];
    return count;
}

long long countInversionsRec(std::vector<int>& arr, int low, int high, std::vector<int>& temp) {
    long long count = 0;
    if (low < high) {
        int mid = low + (high - low) / 2;
        count += countInversionsRec(arr, low, mid, temp);
        count += countInversionsRec(arr, mid + 1, high, temp);
        count += mergeAndCount(arr, low, mid, high, temp);
    }
    return count;
}

long long countInversions(std::vector<int>& arr) {
    if (arr.size() <= 1) return 0;
    std::vector<int> temp(arr.size());
    return countInversionsRec(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}
```

- **Time Complexity:** $O(n \log n)$ guaranteed.
- **Space Complexity:** $O(n)$ auxiliary space.

---

### Problem 4: Kth Largest Element in an Array (Iterative QuickSelect — LeetCode 215)

#### Problem Statement
Given an integer array `nums` and an integer `k`, return the $k^{\text{th}}$ largest element in the array. Solve in average $O(n)$ time with **strict $O(1)$ auxiliary space**.

#### Iterative Approach & Bounded Memory
The $k^{\text{th}}$ largest element is the $(n - k)^{\text{th}}$ element in 0-indexed sorted order.  
While recursive QuickSelect can consume up to $O(n)$ call-stack space in the worst case, **iterative QuickSelect** eliminates all recursive call-stack overhead by updating `low` and `high` in a `while` loop, strictly guaranteeing **$O(1)$ auxiliary space**.

#### Verified C++ Implementation
```cpp
#include <vector>
#include <random>
#include <utility>

// Randomized Lomuto Partition Scheme
int partitionQuickSelect(std::vector<int>& nums, int low, int high) {
    // Modern C++ Mersenne Twister engine for uniform, unbiased random pivot selection
    static std::mt19937 rng(1337);
    std::uniform_int_distribution<int> dist(low, high);
    int randomIdx = dist(rng);
    std::swap(nums[randomIdx], nums[high]);

    int pivot = nums[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (nums[j] <= pivot) {
            i++;
            std::swap(nums[i], nums[j]);
        }
    }
    std::swap(nums[i + 1], nums[high]);
    return i + 1;
}

// Iterative QuickSelect: Guarantees strict O(1) auxiliary space!
int quickSelectIterative(std::vector<int>& nums, int targetIdx) {
    int low = 0;
    int high = static_cast<int>(nums.size()) - 1;

    while (low <= high) {
        if (low == high) return nums[low];

        int p = partitionQuickSelect(nums, low, high);
        if (p == targetIdx) {
            return nums[p];
        } else if (p > targetIdx) {
            high = p - 1; // Narrow search to left partition
        } else {
            low = p + 1;  // Narrow search to right partition
        }
    }
    return -1; // Valid targetIdx is always found
}

int findKthLargest(std::vector<int>& nums, int k) {
    int targetIdx = static_cast<int>(nums.size()) - k;
    return quickSelectIterative(nums, targetIdx);
}
```

- **Time Complexity:** Average $O(n)$, Worst-Case $O(n^2)$ (mitigated with randomized pivot).
- **Space Complexity:** **Strictly $O(1)$ auxiliary memory** (zero recursion call stack).

---

## 7. Common Interview Traps & Pitfalls

1. **Recursive vs. Iterative QuickSelect Memory:**  
   Recursive QuickSelect uses $O(n)$ stack space in the worst case ($O(\log n)$ average). Only the iterative implementation strictly guarantees $O(1)$ auxiliary memory.
2. **Assuming Selection Sort is Stable:**  
   Conventional swap-based Selection Sort is unstable (`[4a, 4b, 1]`).
3. **The `<=` Comparator Bug in C++:**  
   Never write `a <= b` inside a custom comparator. It violates Irreflexivity and causes Undefined Behavior under the C++ standard.
4. **Counting Sort Memory Overflow:**  
   Assuming Counting Sort is always $O(n)$. If the input array has values from $1$ to $10^9$, allocating an array of size $10^9$ will cause an Out-Of-Memory error. Also beware of `maxVal - minVal + 1` overflowing a 32-bit `int` when values are extreme; use `long long`.
5. **Integer Overflow in Midpoint Calculation:**  
   Writing `mid = (low + high) / 2` can overflow 32-bit signed integers if `low + high > INT_MAX`. Always write `mid = low + (high - low) / 2`.

---

## 8. Mock Interview: Interactive Technical Quiz

Test your recall independently before checking the answers below.

### Questions
1. **[Q1]** You have an array of 1,000,000 pointers to large objects. Swapping pointers is fast, but comparisons involve an expensive database network call. Which comparison-based sorting algorithm minimizes the number of comparisons?
2. **[Q2]** Can QuickSort be made stable? If so, what is the trade-off?
3. **[Q3]** Why does `std::make_heap` run in $O(n)$ while inserting $n$ elements into an empty `std::priority_queue` runs in $O(n \log n)$?
4. **[Q4]** An interviewer asks you to sort an array where every element is at most 5 positions away from its sorted location. What algorithm do you propose and what is its complexity?
5. **[Q5]** What happens if you run Lomuto partition on an array where all elements are identical?

---

### Answer Key

- **[A1 Answer]:** Merge Sort. It is guaranteed to perform at most $\approx n \log_2 n$ comparisons. (Selection sort minimizes swaps to $N-1$, but still executes $\approx n^2/2$ comparisons).
- **[A2 Answer]:** Yes, by either pairing each element with its original index `(arr[i], i)` at the cost of $O(n)$ extra memory, or using an out-of-place stable partitioning routine with auxiliary arrays. Both require $O(n)$ space, sacrificing QuickSort's primary advantage of in-place execution.
- **[A3 Answer]:** `std::make_heap` uses bottom-up `sift-down` starting from the lowest internal nodes; the majority of nodes are at the bottom where height is small, summing to $\sum \frac{n}{2^{h+1}} O(h) = O(n)$. Successive insertion uses top-down `sift-up`, where every newly added leaf node may travel all the way up to the root ($O(\log n)$), summing to $\Theta(n \log n)$.
- **[A4 Answer]:** Use a Min-Heap of size $k + 1 = 6$. Insert the first 6 elements. Extract the minimum to index 0, insert the 7th element, and repeat. Time complexity: $O(n \log k) = O(n \log 5) = O(n)$ linear time! Auxiliary space: $O(k) = O(1)$.
- **[A5 Answer]:** Lomuto partition checks `if (arr[j] <= pivot)`. When all elements equal the pivot, the condition is `true` for **every single element**. Lomuto performs a swap on every iteration and places the pivot at `high`, splitting the array into $n-1$ and $0$ elements. This triggers the catastrophic **$O(n^2)$ worst-case runtime!**

---

## 9. Interview Readiness Self-Checklist

Before your interview, ensure you can:
- [ ] Write bug-free Lomuto, Hoare, and 3-Way DNF partitions on a whiteboard without syntax errors.
- [ ] Implement Merge Sort with a pre-allocated scratch buffer and explain why stability is preserved.
- [ ] Explain why bottom-up `Build-Heap` is $O(n)$ while successive insertions take $O(n \log n)$.
- [ ] State the "20 Questions" information-theoretic lower bound concept ($\Omega(n \log n)$).
- [ ] Write a modern C++ lambda comparator with multi-key tie-breaking conforming to Strict Weak Ordering.
- [ ] Implement iterative QuickSelect with strict $O(1)$ auxiliary space.
- [ ] Call `std::partial_sort` and `std::nth_element` appropriately to optimize Top-$k$ interview questions.
- [ ] Explain Introsort and External Merge Sort at an architectural system level.
