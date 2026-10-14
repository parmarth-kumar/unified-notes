# Sorting Algorithms — Google SDE Interview Preparation Guide

> **Module:** `unified-notes/DSA/Sorting/interview-prep.md`  
> **Target Audience:** Google SDE I / II / Senior candidates, competitive programmers, and technical interviewees.  
> **How to use:** Try saying each short model answer in your own words. For a coding problem, explain the approach before writing code, then test the boundary cases.

---

## 1. Fundamental Conceptual Questions

### Q1: What is a stable sort, and why does it matter?
**Spoken answer:** A stable sort keeps the original order of records with equal sorting keys. If students are already sorted by name, a stable sort by grade keeps names in order within each grade.

**Follow-up:** For several separate stable sorts, process the secondary key first, then the primary key. An unstable sort may change tie order; it does not always change it.

---

### Q2: Why is there a comparison-sorting lower bound?
**Spoken answer:** For `n` distinct keys, there are `n!` possible orders. A binary comparison gives one of two answers. A decision tree needs at least `n!` leaves, so some input needs at least `ceil(log2(n!))` comparisons. This is $\Omega(n\log n)$ worst time.

**Follow-up:** This does not forbid a linear best case. Counting and Radix Sort use integer keys or digits, so the comparison-only argument does not apply to them. Their own bounds still depend on range, base, and digit count.

---

### Q3: Why is Selection Sort considered non-adaptive and unstable?
**Model Spoken Answer:**  
> "Selection Sort is non-adaptive because it always scans the entire unsorted suffix to find the minimum element, performing exactly $\frac{n(n-1)}{2}$ comparisons regardless of whether the input is already sorted, reversed, or random.  
> The conventional swap-based implementation is unstable because long-range swaps move elements over identical keys. For example, sorting `[4a, 4b, 1]` swaps `4a` with `1`, producing `[1, 4b, 4a]`, which reverses the original relative order of `4a` and `4b`."

---

## 2. Intermediate Conceptual Questions

### Q4: Is Quick Sort always faster than Merge or Heap Sort?
**Spoken answer:** No. Quick Sort is often fast on arrays and avoids a linear output buffer. Merge Sort gives stability and a worst-time guarantee. Iterative Heap Sort gives a worst-time guarantee with constant extra memory. Actual speed depends on data, comparison costs, implementation, and hardware.

**Follow-up:** Merge Sort also scans memory sequentially. Do not claim it always has poor cache locality. For linked lists, merging changes links without a buffer, but recursive calls still use stack memory.

---

### Q5: How do you avoid Quick Sort's bad time and large stack?
**Spoken answer:** Solve these separately. Recurse on the smaller side and loop on the larger to bound stack memory to $O(\log n)$. For a worst-time guarantee, use an introspective depth limit and fall back to Heap Sort. Random pivots improve expected time but do not remove the quadratic worst case.

**Follow-up:** Median-of-three is a useful heuristic, not a guarantee. For many duplicate keys, use three-way partitioning; randomising ordinary Lomuto does not fix all-equal input.

---

### Q6: What does the C++ standard promise for sort and stable_sort?
**Spoken answer:** `sort` orders the whole range in $O(n\log n)$ comparisons; it does not guarantee stability. `stable_sort` preserves tie order. Its comparison bound is $O(n\log n)$ with enough extra memory, otherwise $O(n\log^2 n)$.

**Follow-up:** Introsort and merge-based sorting are common implementations. The standard does not require those exact algorithms, a threshold of 16, or one portable memory bound. See the [C++ sorting specification](https://eel.is/c++draft/alg.sort).

---

## 3. Advanced Architectural Questions

### Q7: What does strict weak ordering mean?
**Spoken answer:** The comparator answers “should a come before b?” It must be false for an item compared with itself, must not order both directions, must be transitive, and must give consistent ties. `return a <= b` breaks the self-comparison rule and makes the sort's behaviour undefined.

**Follow-up:** `comp(x, x) == false` is necessary but not sufficient. A cyclic order can pass that check and still be invalid. A crash is possible, not guaranteed. Read the [four rules and examples](./theory.md#12-custom-comparators--the-fatal--bug).

---

### Q8: How would you sort 10 TB with 32 GB RAM?
**Spoken answer:** Make sorted files, called runs, from chunks that fit in RAM with room for buffers. Then merge those runs by keeping one front record per run in a min-heap. Stream the chosen records to disk. If all run buffers or open files do not fit, merge groups in several rounds.

**Example:** Using decimal units, 20 GB chunks give about 500 runs. A merge of `r` runs uses $O(\log r)$ heap work per output record. Each full round reads and writes the dataset, so I would also estimate I/O, disk space, and buffer memory. Stable output needs stable runs and consistent tie handling.

---

## 4. Practical C++ STL Interview Tools

In Google interviews, knowing when to use standard selection and partial sorting primitives can turn an $O(n \log n)$ problem into an $O(n)$ or $O(n \log k)$ solution:

```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
using namespace std;

// Tool 1: partial_sort — Finding and sorting only Top-k elements
// About O(n log k) comparisons for k >= 2; memory depends on implementation
void demoPartialSort(){
    vector<int> v = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    // Sort only the 3 smallest elements into v[0...2]
    partial_sort(v.begin(), v.begin() + 3, v.end());
    // v[0], v[1], v[2] are now [1, 2, 3]; remaining elements are in unspecified order.
}

// Tool 2: nth_element — QuickSelect in O(n) average time
// O(n) average comparisons; exact memory is implementation-dependent
// INDEXING CONVENTION:
// v.begin() + k places the element that belongs at ZERO-BASED index k.
// In 1-based human terms, this is the (k + 1)th smallest element.
// (e.g. k = 0 finds the 1st smallest/minimum; k = 4 finds the 5th smallest / median).
void demoNthElement(){
    vector<int> v = {9, 1, 8, 2, 7, 3, 6, 4, 5};
    // Place the element that belongs at index 4 (the 5th smallest element)
    nth_element(v.begin(), v.begin() + 4, v.end());
    // v[4] is now 5. All elements before index 4 are <= 5; all elements after are >= 5.
}
```

---

## 5. Deep Comparison Bank

| Comparison | Key Decision Criteria | Winner / Recommendation |
|---|---|---|
| **Merge Sort vs. Quick Sort (Arrays)** | Memory vs. Speed | **Quick Sort** typically wins on cache speed and $O(1)$ heap memory; Merge Sort wins if stability is strictly required. |
| **Merge Sort vs. Quick Sort (Linked Lists)** | Random Access vs. Pointers | **Merge Sort** fits sequential access; $O(1)$ merge memory, $O(\log n)$ recursive stack or $O(1)$ for bottom-up iteration. |
| **Heap Sort vs. Quick Sort** | Worst-Case Guarantee vs. Cache Hits | **Quick Sort** is often faster in practice due to contiguous cache lines; Heap Sort is useful when $O(n \log n)$ must be guaranteed with strict $O(1)$ memory. |
| **Counting Sort vs. Quick Sort** | Value Range ($k$) vs. Input Size ($n$) | **Counting Sort** wins when $k \le O(n)$ (integers); Quick Sort wins when $k \gg n \log n$ or sorting non-integer types. |
| **Insertion Sort vs. Selection Sort** | Adaptivity & Writes | **Insertion Sort** wins for nearly sorted data ($O(n)$) and general use; **Selection Sort** makes at most $n-1$ swaps and $O(n)$ array writes; useful for small inputs where movement is expensive. |
| **Shell Sort vs. Insertion Sort** | Gap Leaps vs. Single Shifts | **Shell Sort** can reduce long-distance movement compared with Insertion Sort; complexity depends strictly on the gap sequence (e.g. $O(n^{1.5})$ with Knuth gaps). |

---

## 6. Core Coding Interview Problems (Google SDE Level)

### Problem 1: Merge Intervals (LeetCode 56)

#### Problem Statement
Given an array of `intervals` where `intervals[i] = [start_i, end_i]`, merge all overlapping intervals, and return an array of the non-overlapping intervals that cover all the intervals in the input.

#### Clarification Questions for Interviewer
- Are the intervals guaranteed to have `start <= end`? (Assume yes).
- Can the input array be empty? (Handle `size <= 1` gracefully).
- Can the intervals be returned in any order? (Standard is sorted by start time).

#### Approach

A starting approach is to repeatedly search for and combine overlapping pairs. It can require quadratic or worse repeated work, depending on how rescanning is done. Sorting puts intervals in a useful order so one linear scan is enough.

1. Sort intervals by start time: $O(n \log n)$.
2. Initialize `merged` list with the first interval.
3. Iterate through remaining intervals:
   - If current interval starts after the last merged interval ends (`curr.start > merged.back().end`), no overlap: append `curr`.
   - Otherwise, overlap: update `merged.back().end = max(merged.back().end, curr.end)`.

#### C++ Implementation
```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
#include <cstddef>   // for size_t, an array-size type
using namespace std;

vector<vector<int>> mergeIntervals(vector<vector<int>>& intervals){
    if(intervals.size() <= 1){
        return intervals;
    }

    // Step 1: Sort by start time
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b){ return a[0] < b[0]; });

    vector<vector<int>> merged;
    merged.push_back(intervals[0]);

    for(size_t i = 1; i < intervals.size(); ++i){
        if(intervals[i][0] <= merged.back()[1]){
            // Overlapping intervals: expand current interval
            merged.back()[1] = max(merged.back()[1], intervals[i][1]);
        } else {
            // Disjoint interval: push as new entry
            merged.push_back(intervals[i]);
        }
    }

    return merged;
}
```

- **Time Complexity:** $O(n \log n)$ due to sorting. The linear scan takes $O(n)$.
- **Space Complexity:** Output may hold $O(n)$ intervals. The scan uses $O(1)$ extra variables; also account for the sorting implementation, commonly $O(\log n)$ stack.

---

**Dry run and edge checks for intervals:** `[[1,3],[2,6],[8,10],[9,12]]` becomes `[[1,6],[8,12]]`. Nested `[[1,10],[2,3]]` stays `[[1,10]]` because we take the maximum end. Empty and one-interval inputs work. Assume every interval has exactly two endpoints with `start <= end`.

**Follow-up:** With closed intervals, `[1,4]` and `[4,5]` merge. If intervals are half-open and touching intervals should remain separate, change the overlap rule. If the input is already sorted, only the linear merge scan is needed. [Official problem](https://leetcode.com/problems/merge-intervals/).

<a id="problem-2-sort-colors--dutch-national-flag-leetcode-75"></a>
### Problem 2: Sort Colors / Dutch National Flag (LeetCode 75)

#### Problem Statement
Given an array `nums` with $n$ objects colored red, white, or blue (represented as $0, 1, 2$), sort them in-place so that objects of the same color are adjacent, with the colors in the order $0, 1, 2$. Must be solved in a single pass with $O(1)$ auxiliary space.

#### Compare approaches first

Assume every value is 0, 1, or 2, and the array length fits signed indices. General sorting costs $O(n \log n)$. Counting three values and rebuilding costs $O(n)$ time and $O(1)$ space with two scans. DNF keeps four regions and uses one classification loop. It is unstable for labelled records. The [full trace and reasons](./theory.md#sort-colors) explain each pointer move.

#### 3-Pointer Invariant Mechanics
- `nums[0 ... low - 1]` contains `0`s.
- `nums[low ... mid - 1]` contains `1`s.
- `nums[mid ... high]` contains uninspected elements.
- `nums[high + 1 ... n - 1]` contains `2`s.

#### C++ Implementation
```cpp
#include <vector>  // for dynamic arrays
#include <utility> // for swap, move, and pair
using namespace std;

void sortColors(vector<int>& nums){
    int low = 0;
    int mid = 0;
    int high = static_cast<int>(nums.size()) - 1;

    while(mid <= high){
        if(nums[mid] == 0){
            swap(nums[low], nums[mid]);
            low++;
            mid++;
        } else if(nums[mid] == 1){
            mid++;
        } else { // nums[mid] == 2
            swap(nums[mid], nums[high]);
            high--;
            // Do NOT increment mid! Element swapped from high is unexamined.
        }
    }
}
```

- **Time Complexity:** $O(n)$ strictly single-pass.
- **Space Complexity:** $O(1)$ auxiliary space.

---

**Edge checks for DNF:** Empty, one item, all zeros, all ones, all twos, and mixed values. `[2,0]` needs a second inspection of index 0 after the swap. Each iteration removes one item from the unknown region, so total time is linear.

**Follow-up:** DNF swaps whole records but does not preserve equal-colour order. A fourth category needs a revised algorithm or a counting approach. [Official problem](https://leetcode.com/problems/sort-colors/).

<a id="problem-3-count-inversions-in-an-array"></a>
### Problem 3: Count Inversions in an Array

#### Problem Statement
An inversion is a pair of indices $(i, j)$ such that $i < j$ and $arr[i] > arr[j]$. Count the total number of inversions in $O(n \log n)$ time.

#### Algorithmic Insight

Brute force checks all index pairs in $O(n^2)$ time and $O(1)$ space. Ask whether changing the input is allowed; the implementation below sorts it as a side effect. Make a copy first if the original order must remain.

Modify Merge Sort: During the merge step, when an element from the right subarray `arr[j]` is smaller than `arr[i]`, then `arr[j]` is smaller than **all** remaining elements in the left subarray from `i` to `mid`.  
Therefore, add `(mid - i + 1)` directly to the inversion count!

#### C++ Implementation
```cpp
#include <vector> // for dynamic arrays
using namespace std;

long long mergeAndCount(vector<int>& arr, int low, int mid, int high, vector<int>& temp){
    int i = low;
    int j = mid + 1;
    int k = low;
    long long count = 0;

    while(i <= mid && j <= high){
        if(arr[i] <= arr[j]){
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            count += (mid - i + 1); // Key inversion accumulator
        }
    }
    while(i <= mid){
        temp[k++] = arr[i++];
    }
    while(j <= high){
        temp[k++] = arr[j++];
    }

    for(int p = low; p <= high; ++p){
        arr[p] = temp[p];
    }
    return count;
}

long long countInversionsRec(vector<int>& arr, int low, int high, vector<int>& temp){
    long long count = 0;
    if(low < high){
        int mid = low + (high - low) / 2;
        count += countInversionsRec(arr, low, mid, temp);
        count += countInversionsRec(arr, mid + 1, high, temp);
        count += mergeAndCount(arr, low, mid, high, temp);
    }
    return count;
}

long long countInversions(vector<int>& arr){
    if(arr.size() <= 1){
        return 0;
    }
    vector<int> temp(arr.size());
    return countInversionsRec(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}
```

- **Time Complexity:** $O(n \log n)$ guaranteed.
- **Space Complexity:** $O(n)$ auxiliary space.

---

**Dry run for inversions:** Merge `[2,4]` with `[1,3,5]`. Taking 1 adds 2 pairs: `(2,1)` and `(4,1)`. Taking 3 before 4 adds 1 pair: `(4,3)`. Total 3. Equal values add zero. For 100,000 distinct descending values the maximum is `4,999,950,000`, so use `long long`.

**Follow-up:** Count without modifying the input by sorting a copy. For an alternative that preserves order, coordinate compression with a Fenwick tree is in [practice](./practice.md#fenwick-solution).

### Problem 4: Kth Largest Element in an Array (Iterative QuickSelect — LeetCode 215)

#### Problem Statement
Given an integer array `nums` and an integer `k`, return the $k^{\text{th}}$ largest element in the array. Solve in average $O(n)$ time with **strict $O(1)$ auxiliary space**.

#### Compare approaches

Assume the size fits signed indices and `1 <= k <= n`; duplicates count separately. Sorting costs $O(n \log n)$. A size-`k` min-heap costs $O(n \log(k+1))$ time and $O(k)$ space. QuickSelect needs only one partition after each step. It changes input order. A three-way partition prevents repeated work on an equal-key band.

#### Iterative Approach & Bounded Memory
The $k^{\text{th}}$ largest element is the $(n - k)^{\text{th}}$ element in 0-indexed sorted order.  
While recursive QuickSelect can consume up to $O(n)$ call-stack space in the worst case, **iterative QuickSelect** eliminates all recursive call-stack overhead by updating `low` and `high` in a `while` loop, strictly guaranteeing **$O(1)$ auxiliary space**.

#### C++ Implementation
```cpp
#include <vector>    // for dynamic arrays
#include <utility>   // for swap, move, and pair
#include <random>    // for random pivot selection
#include <stdexcept> // for reporting invalid input
#include <cstddef>   // for size_t, an array-size type
using namespace std;

// Randomised three-way partition: return the full equal-key band
pair<int, int> partitionQuickSelect(vector<int>& nums, int low, int high){
    static mt19937 rng(random_device{}());
    uniform_int_distribution<int> pick(low, high);
    int pivot = nums[pick(rng)];
    int lt = low, current = low, gt = high;
    while(current <= gt){
        if(nums[current] < pivot){
            swap(nums[lt++], nums[current++]);
        } else if(nums[current] > pivot){
            swap(nums[current], nums[gt--]);
        } else {
            ++current;
        }
    }
    return {lt, gt};
}

int quickSelectIterative(vector<int>& nums, int targetIdx){
    if(targetIdx < 0 || static_cast<size_t>(targetIdx) >= nums.size()){
        throw invalid_argument("Target index out of range");
    }
    int low = 0, high = static_cast<int>(nums.size()) - 1;
    while(low <= high){
        auto [lt, gt] = partitionQuickSelect(nums, low, high);
        if(targetIdx < lt){
            high = lt - 1;
        } else if(targetIdx > gt){
            low = gt + 1;
        } else {
            return nums[targetIdx];
        }
    }
    throw logic_error("Unreachable for a valid target");
}

int findKthLargest(vector<int>& nums, int k){
    if(k < 1 || static_cast<size_t>(k) > nums.size()){
        throw invalid_argument("Require 1 <= k <= size");
    }
    return quickSelectIterative(nums, static_cast<int>(nums.size()) - k);
}
```

- **Time Complexity:** Expected $O(n)$ under uniform random pivot selection, worst $O(n^2)$. Three-way partitioning finishes all-equal input in $O(n)$; randomised two-way Lomuto would still take quadratic time for some ranks on that input.
- **Space Complexity:** **Strictly $O(1)$ auxiliary memory** (zero recursion call stack).

---

**Dry run and edge checks for selection:** In `[3,2,1,5,6,4]`, the second largest belongs at sorted index `6-2=4`, and its value is 5. The actual pivot path is random. Check `k=1`, `k=n`, duplicates, negatives, and invalid `k`. An empty input has no kth element and is rejected.

**Follow-up:** Expected linear time is not a deterministic worst-time guarantee. An interviewer may ask for a heap with predictable bounds, or median-of-medians selection with worst-case linear time. [Official problem](https://leetcode.com/problems/kth-largest-element-in-an-array/).

### Problem 5: Merge Sorted Arrays (LeetCode 88)

**Task and assumptions:** Two arrays are individually sorted. `nums1` has `m` valid items and `n` spare positions already included in its size; `nums2` has `n` items. The vectors are distinct. Ask whether changing `nums1` is allowed and whether padding really exists.

**Approaches:** Sorting all the data together costs $O((m+n)\log(m+n))$. Forward merging into a new vector costs $O(m+n)$ time and space. With the promised padding, compare the largest remaining items and fill from the back in $O(m+n)$ time and $O(1)$ extra space.

**Dry run:** `[1,2,3,_,_,_]` and `[2,5,6]` write `6,5,3,2` into indices `5,4,3,2`. The remaining `1,2` are already placed. Result `[1,2,2,3,5,6]`.

**Implementation:** [Full checked C++ function and overwrite-safety argument](./theory.md#merge-sorted-arrays).

**Edge cases:** `m=0`, `n=0`, all equal, negative values, real zeros inside valid data. Padding is identified by lengths, not by zero values.

**Follow-ups:** Without padding, ask where the output should go; the constant-space method relies on this storage contract. `merge` requires non-overlapping output. For records, tie handling depends on writing direction.

### Problem 6: Next Permutation (LeetCode 31)

**Task and assumptions:** Return the next lexicographically larger arrangement of the same items, or the smallest arrangement if already at the largest. Duplicates are allowed. Ask whether wraparound should return a boolean; our function does.

**Approaches:** Generating and sorting all permutations can require factorial work. Instead, find the rightmost rise, increase that position with the smallest greater suffix item, then reverse the non-increasing suffix. Each step scans at most `n` items: $O(n)$ time, $O(1)$ extra space.

**Dry run:** `[1,2,5,4,3]`: pivot 2, replacement 3, swap to `[1,3,5,4,2]`, reverse suffix to `[1,3,2,4,5]`.

**Implementation:** [Full C++ function and proof of the three choices](./theory.md#next-permutation).

**Edge cases:** Empty, single item, all equal, and non-increasing input have no greater arrangement. `[1,5,5]` becomes `[5,1,5]`; the successor must be strictly greater. Sorting the whole input on every call does not solve this problem.

**Follow-up:** `next_permutation` returns false on wraparound. Starting sorted and repeatedly calling it visits distinct arrangements in lexicographic order, including inputs with duplicate values.

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
3. **[Q3]** Why does `make_heap` run in $O(n)$ while inserting $n$ elements into an empty `priority_queue` runs in $O(n \log n)$?
4. **[Q4]** An interviewer asks you to sort an array where every element is at most 5 positions away from its sorted location. What algorithm do you propose and what is its complexity?
5. **[Q5]** What happens if you run Lomuto partition on an array where all elements are identical?

---

### Answer Key

- **[A1 Answer]:** First fetch and cache each comparison key once, if the task permits; a network call per comparison is costly and may give inconsistent results. Merge Sort is a reasonable choice for predictable $O(n\log n)$ comparisons, but it is not proven to use the minimum possible comparisons among all algorithms. Selection Sort reduces swaps, not comparisons.
- **[A2 Answer]:** Yes, by either pairing each element with its original index `(arr[i], i)` at the cost of $O(n)$ extra memory, or using an out-of-place stable partitioning routine with auxiliary arrays. Both require $O(n)$ space, sacrificing QuickSort's primary advantage of in-place execution.
- **[A3 Answer]:** `make_heap` uses bottom-up `sift-down` starting from the lowest internal nodes; the majority of nodes are at the bottom where height is small, summing to $\sum \frac{n}{2^{h+1}} O(h) = O(n)$. Successive insertion uses top-down `sift-up`, where every newly added leaf node may travel all the way up to the root ($O(\log n)$), giving $\Theta(n \log n)$ worst time; favourable inputs can take less work.
- **[A4 Answer]:** Use a Min-Heap of size $k + 1 = 6$. Insert the first 6 elements. Extract the minimum to index 0, insert the 7th element, and repeat. Time complexity: $O(n \log(k+1)) = O(n \log 6) = O(n)$ linear time! Auxiliary space: $O(k) = O(1)$.
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
- [ ] Call `partial_sort` and `nth_element` appropriately to optimize Top-$k$ interview questions.
- [ ] Explain Introsort and External Merge Sort at an architectural system level.


## 10. Course problem links and teach-back

- [Merge Sorted Array](https://leetcode.com/problems/merge-sorted-array/): explain why writing from the back is safe.
- [Next Permutation](https://leetcode.com/problems/next-permutation/): explain why changing the rightmost possible position gives the next arrangement.
- [Sort an Array](https://leetcode.com/problems/sort-an-array/): implement Merge Sort or another method with suitable worst-case behaviour.

Before calling yourself ready, solve one problem from each pattern without opening its code. Then explain its invariant, time, extra space, and edge cases. The checkboxes above record your own progress; generating notes does not mark them complete.
