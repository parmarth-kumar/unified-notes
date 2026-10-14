# Sorting Algorithms — Living Doubt Log & Conceptual Clarifications

> **Module:** `unified-notes/DSA/Sorting/doubts.md`  
> **Purpose:** Clear answers to questions that commonly cause confusion. Read the example, then explain it in your own words.
> **Workflow:** When a new question arises during study or interview prep, document it here with full technical reasoning.

---

## Doubt Log Index

- [Doubt 01: Why can't a comparison sort ever run faster than $O(n \log n)$?](#doubt-01-why-cant-a-comparison-sort-ever-run-faster-than-on-log-n)
- [Doubt 02: Why is Selection Sort fundamentally unstable if we only select the minimum?](#doubt-02-why-is-selection-sort-fundamentally-unstable-if-we-only-select-the-minimum)
- [Doubt 03: In Insertion Sort, why is shifting elements superior to swapping them?](#doubt-03-in-insertion-sort-why-is-shifting-elements-superior-to-swapping-them)
- [Doubt 04: Why does Counting Sort need prefix sums and a backward loop instead of simple frequency overwriting?](#doubt-04-why-does-counting-sort-need-prefix-sums-and-a-backward-loop-instead-of-simple-frequency-overwriting)
- [Doubt 05: Why is `Build-Heap` $O(n)$ when inserting $n$ elements takes $O(n \log n)$?](#doubt-05-why-is-build-heap-on-when-inserting-n-elements-takes-on-log-n)
- [Doubt 06: Why does writing `a <= b` in a C++ comparator cause undefined behavior in `sort`?](#doubt-06-why-does-writing-a--b-in-a-c-comparator-cause-undefined-behavior-in-stdsort)
- [Doubt 07: Why does QuickSort degrade on duplicate keys, and how does 3-way partitioning fix it?](#doubt-07-why-does-quicksort-degrade-on-duplicate-keys-and-how-does-3-way-partitioning-fix-it)
- [Doubt 08: Is QuickSort truly "in-place" if it consumes call-stack memory, and does C++ guarantee logarithmic stack usage?](#doubt-08-is-quicksort-truly-in-place-if-it-consumes-call-stack-memory-and-does-c-guarantee-logarithmic-stack-usage)
- [Doubt 09: Counting Sort counts versus indices](#doubt-09-why-does-counting-sort-count-5-mean-output-index-4)
- [Doubt 10: DNF pointer movement](#doubt-10-why-can-dnf-advance-mid-after-moving-a-0-but-not-after-moving-a-2)
- [Doubt 11: Hoare split versus pivot position](#doubt-11-is-hoares-returned-index-the-pivot-position)
- [Doubt 12: Merge storage and vector size](#doubt-12-does-merging-into-the-same-vector-always-mean-constant-space)
- [Doubt 13: Next Permutation suffix reversal](#doubt-13-why-reverse-the-next-permutation-suffix-instead-of-sorting-it)
- [Doubt 14: Count work, not written loops](#doubt-14-is-an-algorithm-always-linear-because-it-uses-one-loop)

---

### Doubt 01: Why can't a comparison sort ever run faster than $O(n \log n)$?

#### Question / Confusion
"Can't an exceptionally clever branching algorithm sort an arbitrary array in $O(n)$ time using only comparisons?"

#### Why the Confusion Occurs
Because linear sorts like Counting Sort and Radix Sort run in $O(n)$, learners often assume an equally clever comparison-based sort might exist.

#### Correct Explanation
In any comparison-based sort, the only information the algorithm can obtain about the relative ordering of elements is through pairwise comparisons ($a_i < a_j$ or $a_i > a_j$).
- There are $n!$ possible permutations of $n$ distinct elements.
- Each comparison provides at most 1 bit of information (branching into 2 possibilities: true or false).
- A binary tree with $L$ leaves must have height at least $\lceil\log_2 L\rceil$; some leaves may be shallower.
- Since there must be at least $n!$ leaves to identify every distinct permutation:
  $$\text{Depth} \ge \log_2(n!) = \Omega(n \log n)$$
This is a worst-case bound for arbitrary distinct keys. It allows a linear best case on sorted input, and does not apply to a restricted-domain method such as counting three colours. For `n = 3`, the six possible orders need at least `ceil(log2(6)) = 3` comparisons in the worst case.

#### Resolution Status
**RESOLVED**

---

### Doubt 02: Why is Selection Sort fundamentally unstable if we only select the minimum?

#### Question / Confusion
"If we are only finding the minimum element and placing it at the front, how can equal elements change their relative order?"

#### Why the Confusion Occurs
Learners assume stability issues only arise when equal elements are directly compared. In Selection Sort, the instability comes not from comparing equal elements, but from the **long-range swap** of the element currently occupying the destination index.

#### Concrete Counterexample
Consider sorting: `arr = [4a, 4b, 1]`
1. **Pass 0 ($i = 0$):**
   - The minimum element in `arr[0...2]` is `1` at index 2.
   - We swap `arr[0]` (`4a`) with `arr[2]` (`1`).
   - The resulting array is: `[1, 4b, 4a]`.
2. Notice what happened to `4a`: it jumped completely over `4b`!
   - In the input, `4a` was before `4b`.
   - In the output, `4b` is before `4a`.
Relative order is reversed. The conventional swap-based implementation is unstable. (Note: A modified Selection Sort that shifts intermediate elements right rather than performing a long-range swap can be made stable, but at the cost of requiring $O(n^2)$ data shifts instead of $O(n)$ swaps).

#### Resolution Status
**RESOLVED**

---

### Doubt 03: In Insertion Sort, why is shifting elements superior to swapping them?

#### Question / Confusion
"In both methods, the elements end up in the exact same positions. Why does shifting make such a difference in performance?"

#### Why the Confusion Occurs
Both approaches perform the same number of comparisons and achieve the same asymptotic $O(n^2)$ time complexity.

#### Correct Explanation
The mechanical difference lies in how elements are moved:
- **Swap Method:** Repeatedly swapping adjacent elements `swap(arr[j], arr[j-1])` performs multiple full variable exchanges—each swap reading and writing both elements through a temporary variable.
- **Shift Method:** Saves `key = arr[i]` once. Each step simply copies the neighboring element one position to the right (`arr[j+1] = arr[j]`). Once the correct insertion point is reached, `arr[j+1] = key` writes the key once.
For a key moved left by `t` positions, swaps write array positions about `2t` times, while shifts use `t + 1` array writes. This can reduce work; it does not guarantee a three-times speed improvement. Example: inserting `1` into `[2, 3, 4, 1]` uses three shifts and one final key write.

#### Resolution Status
**RESOLVED**

---

### Doubt 04: Why does Counting Sort need prefix sums and a backward loop instead of simple frequency overwriting?

#### Question / Confusion
"In lecture code, we just did `while (count[i] > 0) arr[idx++] = i; count[i]--;`. Why do textbooks teach the complex prefix-sum and backward traversal method, and what about integer overflow in the range calculation?"

#### Why the Confusion Occurs
Simple frequency overwriting works fine when sorting standalone primitive integers (`int arr[]`). However, real-world sorting involves **records with satellite data** (e.g., database tuples, structs, objects).

#### Correct Explanation
1. **Two Distinct Implementations:**
   - **Frequency-Overwriting:** Sufficient for plain integers. Equal integers have no visible identity, so their relative order cannot be observed. Extra space is $O(k)$ for the counts, where `k` is the value range.
   - **Prefix Sums & Output Array:** One stable method for whole records such as `Student { age, name }`. Cumulative count minus one gives the last available index. Reading records backwards and placing later equal-key records farther right preserves their input order. The output adds $O(n)$ space. Other whole-record sorting methods are possible too.
2. **Range Integer Overflow Guard:** When calculating `range = maxVal - minVal + 1`, if `minVal == INT_MIN` and `maxVal == INT_MAX`, doing the math in signed 32-bit `int` will immediately overflow! We must cast to `long long` before computing `static_cast<long long>(maxVal) - minVal + 1`, and reject impractical ranges (e.g., $> 10^7$).

#### Resolution Status
**RESOLVED**

---

### Doubt 05: Why is `Build-Heap` $O(n)$ when inserting $n$ elements takes $O(n \log n)$?

#### Question / Confusion
"A heap has $n$ elements, and each element can bubble down $O(\log n)$ levels. Why isn't `Build-Heap` $n \times \log n = O(n \log n)$?"

#### Why the Confusion Occurs
Learners incorrectly assume that *every* node can bubble down $\log n$ levels.

#### Correct Explanation
In a binary heap of size $n$:
- **Half the nodes ($\approx n/2$) are leaves** at the bottom level (height 0). They require **0** swaps!
- $\approx n/4$ nodes are at height 1, requiring at most **1** swap.
- $\approx n/8$ nodes are at height 2, requiring at most **2** swaps.
- Only **1 single node** (the root) is at height $\log n$ and can drop $\log n$ levels!

Summing operations across all nodes mathematically converges to $O(n)$.  
In contrast, successive insertion (`push()`) inserts nodes at the bottom and bubbles them up, allowing many nodes to travel toward the root. Successive insertion has $\Theta(n \log n)$ worst time, but can take linear time on favourable inputs; it does not force every node to travel to the root.

#### Resolution Status
**RESOLVED**

---

<a id="doubt-06-why-does-writing-a--b-in-a-c-comparator-cause-undefined-behavior-in-stdsort"></a>
### Doubt 06: Why does writing `a <= b` in a C++ comparator cause undefined behavior in `sort`?

#### Question / Confusion
"If I want ascending order, why can't my comparator return `true` when `a <= b`? Isn't an element less than or equal to itself?"

#### Why the Confusion Occurs
In mathematics, non-strict inequalities ($\le$) are common. However, C++ Standard Library algorithms require a **Strict Weak Ordering**, modeled after strict inequality ($<$).

#### Correct Explanation
`comp(a, b)` asks whether a belongs **strictly before** b. An item cannot belong before itself, so `comp(a, a)` must be false. `a <= a` is true, which breaks this rule.

The library assumes a valid strict weak ordering. Breaking its requirements gives undefined behaviour: a crash or wrong result is possible, and a passing example does not prove correctness. The exact failure depends on the implementation.

For ascending integers, use `<`; for descending integers, use `>`. A merge's `left <= right` is safe because it chooses which input to copy on a tie; it is not the library comparator. See [the full rules](./theory.md#12-custom-comparators--the-fatal--bug).

#### Resolution Status
**RESOLVED**

---

### Doubt 07: Why does QuickSort degrade on duplicate keys, and how does 3-way partitioning fix it?

#### Question / Confusion
"If all elements in an array are equal (e.g., `[7, 7, 7, 7, 7]`), the array is already sorted! Why does Lomuto QuickSort take worst-case $O(n^2)$ time?"

#### Why the Confusion Occurs
Learners assume equal elements make sorting easier, not harder.

#### Correct Explanation
- In Lomuto partitioning, `arr[j] <= pivot` evaluates to `true` for **every single element**.
- $i$ increments on every step, every element is swapped with itself, and the pivot is swapped with `arr[high]`.
- The split produces one partition of size $n - 1$ and one partition of size $0$! The recursion becomes $T(n) = T(n - 1) + O(n) = O(n^2)$.
- **How 3-Way DNF Fixes This:**  
  Dijkstra's 3-way partition isolates all elements equal to the pivot into a middle segment `arr[lt ... gt] == pivot`. Recursion is only called on `arr[low ... lt - 1]` and `arr[gt + 1 ... high]`. When all keys are identical, both sub-partitions are empty, and QuickSort terminates in **$O(n)$ linear time**!

#### Resolution Status
**RESOLVED**

---

### Doubt 08: Is QuickSort truly "in-place" if it consumes call-stack memory, and does C++ guarantee logarithmic stack usage?

#### Question / Confusion
"QuickSort is recursive, so it uses call stack frames. Does that mean it isn't an in-place sort? And does C++ automatically optimize the stack to $O(\log n)$?"

#### Why the Confusion Occurs
Different textbooks use slightly different definitions of "in-place", and learners often assume compiler optimization guarantees what the language standard does not.

#### Formal Classification & C++ Reality
1. **Practical Engineering Definition:** QuickSort is classified as in-place because it does not allocate auxiliary heap memory proportional to the input size ($O(n)$), requiring only memory for its recursion stack.
2. **C++ Stack Space Reality:** C++ does **NOT** guarantee automatic tail-call optimization in its language standard. If you write naive two-sided recursion, unbalanced partitions can consume up to $O(n)$ stack frames, causing a stack overflow!
3. **How to Enforce $O(\log n)$ Stack Space in C++:**  
   The programmer must explicitly write code that:
   - Recursively calls QuickSort on the **smaller** partition first.
   - Uses an explicit `while` loop (iteration) to handle the **larger** partition.
   Because the smaller partition is always at most half the size ($\le n/2$), the stack depth is $O(\log n)$, including constant overhead, regardless of compiler optimisation. This bounds memory; bad splits can still make sorting take $O(n^2)$ time.

#### Resolution Status
**RESOLVED**


---

### Doubt 09: Why does Counting Sort count 5 mean output index 4?

- **Why it is confusing:** A cumulative count looks like a position, but positions start at 0.
- **Answer:** Count 5 means five items are less than or equal to the key. They occupy indices 0 through 4. Place the last one at `count - 1`, then decrease the count.
- **Example:** Two records with key 4 go into positions 3 and 4 when their cumulative count is 5. Read the later record first and place it at 4; the earlier record then goes at 3.
- **Related lesson:** [Counting Sort](./theory.md#4-counting-sort).
- **Status:** RESOLVED.

### Doubt 10: Why can DNF advance mid after moving a 0, but not after moving a 2?

- **Why it is confusing:** Both actions use a swap.
- **Answer:** When `low < mid`, the item from `low` is a known 1. When `low == mid`, it is a self-swap of 0. Neither needs inspection again. The item coming from `high` is unknown, so inspect it before advancing.
- **Example:** In `[1, 2, 0]`, moving the 2 right brings a 0 to `mid`. Skipping it would leave `[1, 0, 2]` unsorted.
- **Related lesson:** [DNF regions and trace](./theory.md#sort-colors).
- **Status:** RESOLVED.

### Doubt 11: Is Hoare's returned index the pivot position?

- **Why it is confusing:** Lomuto returns the pivot's final index, and both are partition methods.
- **Answer:** Hoare returns the boundary between two groups. The pivot may be elsewhere. Recurse on `[low, split]` and `[split + 1, high]`.
- **Example:** For `[3, 1, 2]` with the midpoint value 1, Hoare returns 0; the two groups are `[1]` and `[3, 2]`. The right group still needs sorting.
- **Related lesson:** [Hoare partition](./theory.md#hoare-partition).
- **Status:** RESOLVED.

### Doubt 12: Does “merging into the same vector” always mean constant space?

- **Why it is confusing:** Merge Sort eventually copies its output back into the input vector.
- **Answer:** Count every temporary allocation. Our array Merge Sort uses an `n`-item buffer. The padded two-array problem uses already-created spare positions and three indices, so it needs constant extra space.
- **Example:** `reserve(6)` does not make a size-3 vector writable at index 5. Use `resize(6)` to create those positions.
- **Related lessons:** [Merge Sort](./theory.md#5-merge-sort) and [padded merge](./theory.md#merge-sorted-arrays).
- **Status:** RESOLVED.

### Doubt 13: Why reverse the Next Permutation suffix instead of sorting it?

- **Why it is confusing:** Sorting seems necessary to obtain the smallest suffix.
- **Answer:** The suffix is already non-increasing, including possible equal values. The chosen swap keeps that property. Reversing therefore makes it non-decreasing in linear time.
- **Example:** `[1, 5, 5]` swaps 1 with the rightmost 5 to give `[5, 5, 1]`, then reverses `[5, 1]` to obtain `[5, 1, 5]`.
- **Related lesson:** [Next Permutation](./theory.md#next-permutation).
- **Status:** RESOLVED.

### Doubt 14: Is an algorithm always linear because it uses one loop?

- **Why it is confusing:** We count written loops instead of how much work their bodies do.
- **Answer:** Count total work. A loop that runs `n` times and sorts `n` items each time is not linear. A nested loop can be linear if the total work is bounded: Counting Sort's reconstruction writes exactly `n` items across all buckets, plus `k` bucket checks.
- **Example:** DNF may inspect one index repeatedly, but its unknown region loses exactly one item per iteration, so it has `n` iterations.
- **Related lessons:** [Counting](./theory.md#4-counting-sort) and [DNF](./theory.md#sort-colors).
- **Status:** RESOLVED.
