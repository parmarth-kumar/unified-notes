# Sorting Algorithms — Active Practice & Problem Solving

> **Module:** `unified-notes/DSA/Sorting/practice.md`  
> **Purpose:** Hands-on code tracing, debugging challenges, algorithmic implementation tasks, and progressive hint-driven problem solving.  
> **Rules:** Attempt every challenge independently before consulting the **Solutions & Explanations** section at the bottom.

---

## 1. Tracing & Dry-Run Challenges

### Challenge 1.1: Bubble Sort Early-Termination Trace
Given array $A = [3, 1, 2, 4, 5, 6]$:
1. Trace the inner loop comparison pairs for Pass 0. Does any swap occur?
2. Trace the inner loop comparison pairs for Pass 1. Does any swap occur?
3. State the exact value of the `swapped` boolean flag after Pass 1 and determine the total number of comparisons performed by an optimized Bubble Sort.

---

### Challenge 1.2: Lomuto vs. Hoare Partition Comparison
Given array $A = [10, 80, 30, 90, 40, 50, 70]$:
1. **Lomuto Partition:** Using the last element (`70`) as the pivot, trace the movements of pointers $i$ and $j$, list every swap executed, and identify the final returned pivot index.
2. **Hoare Partition:** Using the middle element (`90` at index 3) as the pivot, trace the movements of pointers $i$ and $j$, list every swap executed, and identify the final split index.

---

### Challenge 1.3: Stable Counting Sort Prefix Sum Tracing
Given array $A = [4, -2, 0, 4, -2, 1]$:
1. Compute `minVal`, `maxVal`, and the range $k$.
2. Write down the frequency array `count`.
3. Compute the prefix sum cumulative count array.
4. Show the step-by-step backward iteration ($i = 5$ down to $0$) demonstrating where each element is placed in the output array.

---

## 2. Debugging Challenges (Spot the Bug)

### Bug 2.1: The Fatal C++ Comparator
A developer writes the following comparator to sort student structures by score in descending order:
```cpp
struct Student {
    int id;
    int score;
};

// Intended: Sort by score descending; if equal, by id ascending
bool compareStudents(const Student& a, const Student& b) {
    if (a.score != b.score) {
        return a.score >= b.score; // BUG HERE?
    }
    return a.id <= b.id;           // BUG HERE?
}

void sortStudents(std::vector<Student>& v) {
    std::sort(v.begin(), v.end(), compareStudents);
}
```
**Questions:**
1. What axiom of Strict Weak Ordering is violated?
2. What failure may occur when passed to `std::sort` under the C++ standard?
3. Write the corrected comparator.

---

### Bug 2.2: The Subtly Broken Heapify
The following C++ function attempts to restore the Max-Heap property:
```cpp
void brokenHeapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i;      // LINE A
    int right = 2 * i + 1; // LINE B

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        // LINE C: Recursion missing?
    }
}
```
**Questions:**
1. What indexing convention error exists on Line A and Line B for a 0-indexed C++ array?
2. What critical algorithmic step is missing inside the `if (largest != i)` block?

---

### Bug 2.3: In-Place Counting Sort Data Loss
A student implements Counting Sort for a list of employee records sorted by department ID:
```cpp
struct Employee {
    int deptId;
    std::string name;
};

void sortEmployees(std::vector<Employee>& emp) {
    int maxDept = 100;
    std::vector<int> count(maxDept + 1, 0);

    for (const auto& e : emp) {
        count[e.deptId]++;
    }

    int idx = 0;
    for (int d = 0; d <= maxDept; ++d) {
        while (count[d] > 0) {
            emp[idx].deptId = d; // Overwrite deptId
            count[d]--;
            idx++;
        }
    }
}
```
**Questions:**
1. Why does this code cause silent data corruption?
2. Which sorting property is completely violated?

---

### Bug 2.4: The Bubble Sort Off-by-One Boundary Trap
A student implements the inner loop of Bubble Sort as follows:
```cpp
void brokenBubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        // Notice the loop termination condition:
        for (int j = 0; j < n - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}
```
**Questions:**
1. What runtime bug occurs during execution when $i = 0$?
2. Why is the `- 1` in `j < n - i - 1` mathematically necessary?

---

## 3. Implementation Challenges

### Level 1 (Easy): In-Place Char Array Sort
Write a C++ function `void sortStringAlphabetically(std::string& s)` that sorts a lowercase ASCII string `s` in $O(n)$ time and $O(1)$ auxiliary space.

### Level 2 (Medium): Sort $k$-Sorted Array
Given an array of $n$ elements where each element is at most $k$ positions away from its target sorted position, sort the array in $O(n \log k)$ time and $O(k)$ auxiliary space.
- LeetCode Variant: [Nearly Sorted Array (GFG)](https://www.geeksforgeeks.org/problems/nearly-sorted-1587115620/1)

### Level 3 (Hard): Inversion Counting with Coordinate Compression
Given an array `arr` with $10^5$ arbitrary 32-bit integers (including negative numbers and large integers up to $10^9$), count the total number of inversions $(i, j)$ where $i < j$ and $arr[i] > arr[j]$.
- LeetCode 315 Variant: [Count of Smaller Numbers After Self](https://leetcode.com/problems/count-of-smaller-numbers-after-self/)

---

## 4. Progressive Hints

### Hints for Level 2 (Sort $k$-Sorted Array)
- **Hint 1:** Because every element is at most $k$ steps away from its sorted index, the element that belongs at index $0$ must currently reside somewhere in indices $0$ through $k$.
- **Hint 2:** Can you use a data structure of fixed size $k + 1$ that efficiently extracts the minimum element in $O(\log k)$?
- **Hint 3:** Use a `std::priority_queue<int, std::vector<int>, std::greater<int>>` (Min-Heap). Push the first $k + 1$ elements. Then, for every index from $0$ to $n-1$, pop the root into the array and push the next available element.

### Hints for Level 3 (Inversion Counting)
- **Hint 1:** Think divide and conquer. If you divide the array into left and right halves, inversions can be of three types: completely inside the left half, completely inside the right half, or spanning across the halves.
- **Hint 2:** If both halves are already sorted, how can two pointers identify how many elements in the left half are greater than an element in the right half in $O(1)$ time?
- **Hint 3:** In the merge step: if `arr[i] > arr[j]`, then because the left subarray is sorted, every element from index `i` to `mid` is also strictly greater than `arr[j]`. You can add `(mid - i + 1)` directly to your count!

---

## 5. Solutions & Explanations

<details>
<summary><b>Click to expand Solution for Challenge 1.1 (Bubble Sort Trace)</b></summary>

1. **Pass 0:**
   - Compare `(3, 1)`: $3 > 1 \implies$ swap $\to [1, 3, 2, 4, 5, 6]$.
   - Compare `(3, 2)`: $3 > 2 \implies$ swap $\to [1, 2, 3, 4, 5, 6]$.
   - Compare `(3, 4)`: $3 < 4 \implies$ no swap.
   - Compare `(4, 5)`: $4 < 5 \implies$ no swap.
   - Compare `(5, 6)`: $5 < 6 \implies$ no swap.
   - `swapped = true`. $6$ settled at index 5.
2. **Pass 1:**
   - Compare `(1, 2)`: no swap.
   - Compare `(2, 3)`: no swap.
   - Compare `(3, 4)`: no swap.
   - Compare `(4, 5)`: no swap.
   - `swapped = false`.
3. **Termination:**
   - Loop breaks immediately after Pass 1 because `swapped == false`.
   - Total comparisons: $5$ (Pass 0) $+ 4$ (Pass 1) $= 9$ comparisons.
</details>

<details>
<summary><b>Click to expand Solution for Challenge 1.2 (Lomuto vs. Hoare Trace)</b></summary>

Given: `A = [10, 80, 30, 90, 40, 50, 70]`

1. **Lomuto Partition (pivot = 70):**
   - $i = -1$.
   - $j = 0$ (`10`): $10 \le 70 \implies i=0$, swap `arr[0], arr[0]` (no-op).
   - $j = 1$ (`80`): $80 > 70 \implies$ no swap.
   - $j = 2$ (`30`): $30 \le 70 \implies i=1$, swap `arr[1]` (`80`) with `arr[2]` (`30`). Array: `[10, 30, 80, 90, 40, 50, 70]`.
   - $j = 3$ (`90`): $90 > 70 \implies$ no swap.
   - $j = 4$ (`40`): $40 \le 70 \implies i=2$, swap `arr[2]` (`80`) with `arr[4]` (`40`). Array: `[10, 30, 40, 90, 80, 50, 70]`.
   - $j = 5$ (`50`): $50 \le 70 \implies i=3$, swap `arr[3]` (`90`) with `arr[5]` (`50`). Array: `[10, 30, 40, 50, 80, 90, 70]`.
   - Final swap: swap `arr[i+1]` (`80`) with pivot `arr[6]` (`70`).
   - Final Array: `[10, 30, 40, 50, 70, 90, 80]`. Returns index $4$.

2. **Hoare Partition (pivot = arr[3] = 90):**
   - $i = -1, j = 7$.
   - Advance $i$: stops at `arr[1] = 80` (since $10 < 90, 80 < 90$... wait: `arr[i] < pivot`: $10 < 90, 80 < 90, 30 < 90$, stops at $i=3$ where $90 \ge 90$).
   - Decrement $j$: stops at $j=6$ where `arr[6] = 70 \le 90$.
   - Swap `arr[3]` (`90`) and `arr[6]` (`70`).
   - Pointers meet and return split index.
</details>

<details>
<summary><b>Click to expand Solution for Challenge 1.3 (Counting Sort Trace)</b></summary>

Given: $A = [4, -2, 0, 4, -2, 1]$
1. `minVal = -2`, `maxVal = 4`. Range $k = 4 - (-2) + 1 = 7$.
   Indices in count array: $0 \to -2, 1 \to -1, 2 \to 0, 3 \to 1, 4 \to 2, 5 \to 3, 6 \to 4$.
2. **Frequency array `count`:**
   `count[0]=2` (-2), `count[1]=0` (-1), `count[2]=1` (0), `count[3]=1` (1), `count[4]=0` (2), `count[5]=0` (3), `count[6]=2` (4).
   `count = [2, 0, 1, 1, 0, 0, 2]`.
3. **Prefix sums:**
   `count = [2, 2, 3, 4, 4, 4, 6]`.
4. **Backward scan:**
   - $i = 5, A[5] = 1 \implies \text{idx} = 1 - (-2) = 3$. Target $= \text{count}[3] - 1 = 4 - 1 = 3$. Output[3] = 1. `count[3]` becomes 3.
   - $i = 4, A[4] = -2 \implies \text{idx} = 0$. Target $= \text{count}[0] - 1 = 2 - 1 = 1$. Output[1] = -2. `count[0]` becomes 1.
   - $i = 3, A[3] = 4 \implies \text{idx} = 6$. Target $= \text{count}[6] - 1 = 6 - 1 = 5$. Output[5] = 4. `count[6]` becomes 5.
   - $i = 2, A[2] = 0 \implies \text{idx} = 2$. Target $= \text{count}[2] - 1 = 3 - 1 = 2$. Output[2] = 0. `count[2]` becomes 2.
   - $i = 1, A[1] = -2 \implies \text{idx} = 0$. Target $= \text{count}[0] - 1 = 1 - 1 = 0$. Output[0] = -2. `count[0]` becomes 0.
   - $i = 0, A[0] = 4 \implies \text{idx} = 6$. Target $= \text{count}[6] - 1 = 5 - 1 = 4$. Output[4] = 4. `count[6]` becomes 4.
   Final `Output = [-2, -2, 0, 1, 4, 4]`. Stability strictly preserved!
</details>

<details>
<summary><b>Click to expand Solution for Bug 2.1 (Comparator Fix)</b></summary>

1. **Axiom Violated:** **Irreflexivity**. The rule states `comp(x, x)` must evaluate to `false`. Using `>=` or `<=` causes `comp(x, x)` to evaluate to `true`.
2. **Failure:** Violates Strict Weak Ordering, which triggers **Undefined Behavior (UB)** under the ISO C++ standard. In common standard library implementations (like libstdc++), partitioning loops omit boundary checks to maximize speed, relying on `comp(pivot, pivot) == false` as an in-buffer sentinel. When it returns `true`, the pointer can advance past allocated memory bounds. While a segmentation fault is a common symptom on many platforms, **a crash is possible, not guaranteed**; other manifestations include silent memory corruption, invalid sorting order, or infinite loops.
3. **Corrected Comparator:**
```cpp
bool compareStudents(const Student& a, const Student& b) {
    if (a.score != b.score) {
        return a.score > b.score; // Strict '>'
    }
    return a.id < b.id;           // Strict '<'
}
```
</details>

<details>
<summary><b>Click to expand Solution for Bug 2.2 (Heapify Fix)</b></summary>

1. **Index error:** In 0-indexed C++ arrays, the left child is `2 * i + 1` and the right child is `2 * i + 2`. Using `2 * i` is 1-indexed formula and creates an infinite loop when $i = 0$ ($2 \times 0 = 0$).
2. **Missing recursion:** When an element is swapped down, it can violate the heap property in the newly formed subtree. It must recursively call `brokenHeapify(arr, n, largest);`.
</details>

<details>
<summary><b>Click to expand Solution for Bug 2.3 (Counting Sort Bug)</b></summary>

1. **Data Corruption:** Overwriting `emp[idx].deptId = d` only updates the department ID, leaving the employee's `name` field untouched from whatever record was originally at `idx`. Employee records are completely decoupled and scrambled!
2. **Property Violated:** **Stability and Data Integrity**. In-place frequency overwrite can only sort primitive scalars; composite records require the stable output-array prefix-sum implementation.
</details>

<details>
<summary><b>Click to expand Solution for Bug 2.4 (Bubble Sort Boundary Trap)</b></summary>

1. **Runtime Error:** When $i = 0$, the inner loop condition `j < n - i` allows $j$ to reach $n - 1$. Inside the loop body, `arr[j + 1]` attempts to read `arr[n]`. In standard 0-indexed vectors of size $n$, valid elements reside at indices $0 \dots n - 1$. Accessing index $n$ is an out-of-bounds memory violation, triggering Undefined Behavior (and commonly a segmentation fault).
2. **Mathematical Necessity:** Every comparison inspects the adjacent pair `(arr[j], arr[j + 1])`. The upper index `j + 1` must satisfy $j + 1 \le \text{last valid index}$. For pass $i$, the last unsorted index is $n - 1 - i$. Therefore, $j + 1 \le n - 1 - i \iff j \le n - 2 - i \iff j < n - i - 1$. The extra `- 1` guarantees the lookahead `j + 1` never exceeds array bounds.
</details>

<details>
<summary><b>Click to expand Solution for Implementation Level 1 (Char Sort)</b></summary>

```cpp
#include <string>
#include <vector>

void sortStringAlphabetically(std::string& s) {
    int count[26] = {0};
    for (char c : s) {
        count[c - 'a']++;
    }
    int idx = 0;
    for (int i = 0; i < 26; ++i) {
        while (count[i] > 0) {
            s[idx++] = static_cast<char>('a' + i);
            count[i]--;
        }
    }
}
```
- **Time Complexity:** $O(n + 26) = O(n)$.
- **Space Complexity:** $O(26) = O(1)$ auxiliary.
</details>

<details>
<summary><b>Click to expand Solution for Implementation Level 2 (k-Sorted Array)</b></summary>

```cpp
#include <vector>
#include <queue>

void sortKSortedArray(std::vector<int>& arr, int k) {
    int n = arr.size();
    if (n <= 1 || k == 0) return;

    // Min-heap to maintain window of size k + 1
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    // Push first min(n, k + 1) elements
    int windowSize = std::min(n, k + 1);
    for (int i = 0; i < windowSize; ++i) {
        minHeap.push(arr[i]);
    }

    int targetIdx = 0;
    for (int i = k + 1; i < n; ++i) {
        arr[targetIdx++] = minHeap.top();
        minHeap.pop();
        minHeap.push(arr[i]);
    }

    // Drain remaining elements
    while (!minHeap.empty()) {
        arr[targetIdx++] = minHeap.top();
        minHeap.pop();
    }
}
```
- **Time Complexity:** $O(n \log k)$.
- **Space Complexity:** $O(k)$ auxiliary heap memory.
</details>

<details>
<summary><b>Click to expand Solution for Implementation Level 3 (Inversion Counting)</b></summary>

See verified C++ implementation in [`interview-prep.md`](./interview-prep.md#problem-3-count-inversions-in-an-array).
</details>
