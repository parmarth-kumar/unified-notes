# Sorting Algorithms — Active Practice & Problem Solving

> **Module:** `unified-notes/DSA/Sorting/practice.md`  
> **Purpose:** Learn by doing. Trace small arrays, find bugs, then write and explain your own code.
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

Use the exact [partition implementations in theory](./theory.md#hoare-partition), starting each trace from a fresh copy of the input.
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

### Challenge 1.4: Sorted prefix versus final prefix

Start with `[4, 3, 2, 1]` and use a fresh copy for each algorithm.

1. Write the array after each Selection Sort pass and after each Insertion Sort pass.
2. Mark the processed prefix. Which algorithm can move those processed items again later?
3. Trace insertion of 1 into `[2, 3, 4, 1]`, including the saved key and temporary repeated values.

### Challenge 1.5: DNF and duplicates

Trace `[1, 2, 0]` with `low`, `mid`, and `high`. Show why advancing `mid` after swapping with `high` gives the wrong result. Then trace `[2a, 2b, 0]` to check stability, comparing only colour values.

### Challenge 1.6: Padded merge and Next Permutation

1. Merge `nums1 = [-3, 0, 4, _, _, _]`, `m=3`, with `nums2 = [-2, 0, 5]`, `n=3`. Record `i`, `j`, and `write` before each assignment. Why is the first zero real data?
2. Find the next permutation of `[1, 5, 5]`, `[2, 3, 1]`, and `[3, 2, 1]`. Identify the pivot, successor, and suffix in each case.
3. Explain why Next Permutation is not the same task as sorting.

### Challenge 1.7: Match the algorithm to the assumptions

Choose a method and state its time and extra memory:

- Find only the maximum of a million values.
- Sort students by marks while keeping arrival order for equal marks.
- Sort signed integers whose values lie between -10 and 10.
- Find the fifth largest value without needing a fully sorted array.
- Sort data larger than RAM.

## 2. Debugging Challenges (Spot the Bug)

### Bug 2.1: The Fatal C++ Comparator
A developer writes the following comparator to sort student structures by score in descending order:
```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
using namespace std;

struct Student {
    int id;
    int score;
};

// Intended: Sort by score descending; if equal, by id ascending
bool compareStudents(const Student& a, const Student& b){
    if(a.score != b.score){
        return a.score >= b.score; // BUG HERE?
    }
    return a.id <= b.id; // BUG HERE?
}

void sortStudents(vector<Student>& v){
    sort(v.begin(), v.end(), compareStudents);
}
```
**Questions:**
1. What axiom of Strict Weak Ordering is violated?
2. What failure may occur when passed to `sort` under the C++ standard?
3. Write the corrected comparator.

---

### Bug 2.2: The Subtly Broken Heapify
The following C++ function attempts to restore the Max-Heap property:
```cpp
#include <utility> // for swap, move, and pair
using namespace std;

void brokenHeapify(int arr[], int n, int i){
    int largest = i;
    int left = 2 * i;      // LINE A
    int right = 2 * i + 1; // LINE B

    if(left < n && arr[left] > arr[largest]){
        largest = left;
    }

    if(right < n && arr[right] > arr[largest]){
        largest = right;
    }

    if(largest != i){
        swap(arr[i], arr[largest]);
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
#include <vector> // for dynamic arrays
#include <string> // for text
using namespace std;

struct Employee {
    int deptId;
    string name;
};

void sortEmployees(vector<Employee>& emp){
    int maxDept = 100;
    vector<int> count(maxDept + 1, 0);

    for(const auto& e : emp){
        count[e.deptId]++;
    }

    int idx = 0;
    for(int d = 0; d <= maxDept; ++d){
        while(count[d] > 0){
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
#include <vector>  // for dynamic arrays
#include <utility> // for swap, move, and pair
using namespace std;

void brokenBubbleSort(vector<int>& arr){
    int n = arr.size();
    for(int i = 0; i < n - 1; ++i){
        // Notice the loop termination condition:
        for(int j = 0; j < n - i; ++j){
            if(arr[j] > arr[j + 1]){
                swap(arr[j], arr[j + 1]);
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

### Course drill: Descending order

Using `[3,6,2,1,8,7,4,5,3,1]`, implement descending Bubble, Selection, Insertion, and frequency Counting Sort. For each, state the one decision you changed. Then use Insertion Sort to change `"fbaecd"` to `"fedcba"`.

### Level 1 (Easy): In-Place Char Array Sort
Write a C++ function `void sortStringAlphabetically(string& s)` that sorts a lowercase ASCII string `s` in $O(n)$ time and $O(1)$ auxiliary space.

### Level 2 (Medium): Sort $k$-Sorted Array
Given an array of $n$ elements where each element is at most $k$ positions away from its target sorted position, sort the array in $O(n \log(k+2))$ time and $O(k)$ auxiliary space.
- LeetCode Variant: [Nearly Sorted Array (GFG)](https://www.geeksforgeeks.org/problems/nearly-sorted-1587115620/1)

### Level 3 (Hard): Inversion Counting with Coordinate Compression
Given an array `arr` with $10^5$ arbitrary 32-bit integers (including negative numbers and large integers up to $10^9$), count the total number of inversions $(i, j)$ where $i < j$ and $arr[i] > arr[j]$.
- LeetCode 315 Variant: [Count of Smaller Numbers After Self](https://leetcode.com/problems/count-of-smaller-numbers-after-self/)

---

## 4. Progressive Hints

### Hints for Level 2 (Sort $k$-Sorted Array)
- **Hint 1:** Because every element is at most $k$ steps away from its sorted index, the element that belongs at index $0$ must currently reside somewhere in indices $0$ through $k$.
- **Hint 2:** Can you use a data structure of fixed size $k + 1$ that efficiently extracts the minimum element in $O(\log k)$?
- **Hint 3:** Use a `priority_queue<int, vector<int>, greater<int>>` (Min-Heap). Push the first $k + 1$ elements. Then, for every index from $0$ to $n-1$, pop the root into the array and push the next available element.

### Hints for Level 3 (Inversion Counting)

- **Hint A:** Replace each distinct value with its sorted rank. Equal values must get equal ranks.
- **Hint B:** Scan from right to left; already-seen values are to the right of the current item.
- **Hint C:** A Fenwick tree stores frequency totals. Query ranks strictly smaller than the current rank, then add the current rank.

**Alternative merge approach (no compression required):**
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

2. **Hoare Partition (pivot value = 90):**
   - Start `i=-1`, `j=7`.
   - Left scan visits 10, 80, 30, then stops at `i=3` because `90 < 90` is false.
   - Right scan stops at `j=6`, value 70, because `70 > 90` is false.
   - Swap indices 3 and 6: `[10,80,30,70,40,50,90]`.
   - Next left scan passes indices 4 and 5, then stops at `i=6`, value 90.
   - Next right scan stops at `j=5`, value 50.
   - `i >= j`, so return **split 5**. Recurse on `[0..5]` and `[6..6]`.
   - The left group is not sorted yet. The returned split is not the final index of pivot 90, which is currently at 6.
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

1. **Actual invalid branch:** `return a.id <= b.id` returns true when both ID and score tie, including self-comparison. This violates irreflexivity.
2. **Check the guard:** `a.score >= b.score` is only evaluated when scores differ, so it has the same result as `a.score > b.score` there. That branch is valid as written. Use `>` for clearer intent, but the presence of `>=` alone is not proof of a bug. The complete ordering rule matters. Passing the invalid comparator to `sort` gives undefined behaviour; a crash is not guaranteed.
3. **Corrected Comparator:**
```cpp
bool compareStudents(const Student& a, const Student& b){
    if(a.score != b.score){
        return a.score > b.score; // Strict '>'
    }
    return a.id < b.id; // Strict '<'
}
```
</details>

<details>
<summary><b>Click to expand Solution for Bug 2.2 (Heapify Fix)</b></summary>

1. **Index error:** In 0-indexed C++ arrays, the left child is `2 * i + 1` and the right child is `2 * i + 2`. Using `2 * i` is a 1-indexed formula. At `i=0`, it treats the root as its own child and misses the real right child at index 2. Self-comparison with `>` is false, so this code does not automatically create an infinite loop. For `[1,2,9]`, it picks 2 instead of 9.
2. **Missing recursion:** When an element is swapped down, it can violate the heap property in the newly formed subtree. Continue sift-down from `largest`, either in a loop or with a call to the corrected heapify function. Merely calling the still-broken function does not repair its indexing.
</details>

<details>
<summary><b>Click to expand Solution for Bug 2.3 (Counting Sort Bug)</b></summary>

1. **Data Corruption:** Overwriting `emp[idx].deptId = d` only updates the department ID, leaving the employee's `name` field untouched from whatever record was originally at `idx`. Employee records are completely decoupled and scrambled!
2. **Property Violated:** **Data integrity / item preservation** fails first: the output no longer contains the original employee records. Stability is a separate question once whole records are preserved. Stable prefix-sum placement is one suitable repair; other record-moving sorts can also work.
</details>

<details>
<summary><b>Click to expand Solution for Bug 2.4 (Bubble Sort Boundary Trap)</b></summary>

1. **Runtime Error:** When $i = 0$, the inner loop condition `j < n - i` allows $j$ to reach $n - 1$. Inside the loop body, `arr[j + 1]` attempts to read `arr[n]`. In standard 0-indexed vectors of size $n$, valid elements reside at indices $0 \dots n - 1$. Accessing index $n$ is an out-of-bounds memory violation, triggering Undefined Behavior (and commonly a segmentation fault).
2. **Mathematical Necessity:** Every comparison inspects the adjacent pair `(arr[j], arr[j + 1])`. The upper index `j + 1` must satisfy $j + 1 \le \text{last valid index}$. For pass $i$, the last unsorted index is $n - 1 - i$. Therefore, $j + 1 \le n - 1 - i \iff j \le n - 2 - i \iff j < n - i - 1$. The extra `- 1` guarantees the lookahead `j + 1` never exceeds array bounds.
</details>

<details>
<summary><b>Click to expand Solution for Implementation Level 1 (Char Sort)</b></summary>

```cpp
#include <string> // for text
using namespace std;

void sortStringAlphabetically(string& s){
    int count[26] = {0};
    for(char c : s){
        count[c - 'a']++;
    }
    int idx = 0;
    for(int i = 0; i < 26; ++i){
        while(count[i] > 0){
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
#include <vector>     // for dynamic arrays
#include <algorithm>  // for sorting and array helpers
#include <functional> // for comparison and callable types
#include <queue>      // for the heap / priority queue
#include <stdexcept>  // for reporting invalid input
using namespace std;

void sortKSortedArray(vector<int>& arr, int k){
    if(k < 0){
        throw invalid_argument("k must be non-negative");
    }
    int n = static_cast<int>(arr.size());
    if(n <= 1 || k == 0){
        return; // k=0 promises the input is already sorted
    }

    // Min-heap to maintain window of size k + 1
    priority_queue<int, vector<int>, greater<int>> minHeap;

    // Push first min(n, k + 1) elements
    int windowSize = min(k, n - 1) + 1; // Avoid k+1 overflow
    for(int i = 0; i < windowSize; ++i){
        minHeap.push(arr[i]);
    }

    int targetIdx = 0;
    for(int i = windowSize; i < n; ++i){
        arr[targetIdx++] = minHeap.top();
        minHeap.pop();
        minHeap.push(arr[i]);
    }

    // Drain remaining elements
    while(!minHeap.empty()){
        arr[targetIdx++] = minHeap.top();
        minHeap.pop();
    }
}
```
- **Time Complexity:** $O(n \log(k+2))$.
- **Space Complexity:** $O(\min(n,k+1))$ heap memory. For `k=0`, the displacement promise means the input is already sorted.
</details>

<details>
<summary><b>Click to expand Solution for Implementation Level 3 (Inversion Counting)</b></summary>

The merge-based solution is in [interview preparation](./interview-prep.md#problem-3-count-inversions-in-an-array). It does not need coordinate compression. Here is the requested **coordinate-compression** alternative.

<a id="fenwick-solution"></a>
**Compression:** Sort a copy and remove repeated values. For `[-5, 10, -5, 3]`, unique values `[-5,3,10]` become ranks `1,2,3`. This changes the size of the index domain without changing relative order.

**Fenwick tree:** An array of partial frequency totals that can update a rank or sum a prefix in $O(\log n)$ time. `index & -index` gives its lowest set bit; updates move to larger covering ranges, queries move to smaller ranges.

Scan right to left. Query rank minus one to count strictly smaller values already seen; then insert the current rank. Equal values share a rank and are not counted as inversions.

```cpp
#include <vector>    // for dynamic arrays
#include <algorithm> // for sorting and array helpers
using namespace std;

long long countInversionsFenwick(const vector<int>& arr){
    vector<int> keys = arr;
    sort(keys.begin(), keys.end());
    keys.erase(unique(keys.begin(), keys.end()), keys.end());
    int ranks = static_cast<int>(keys.size());
    vector<int> tree(keys.size() + 1, 0);
    long long answer = 0;
    for(int i = static_cast<int>(arr.size()) - 1; i >= 0; --i){
        int rank =
            static_cast<int>(lower_bound(keys.begin(), keys.end(), arr[i]) - keys.begin()) + 1;
        for(int q = rank - 1; q > 0; q -= q & -q){
            answer += tree[q];
        }
        for(int u = rank; u <= ranks;){
            ++tree[u];
            int jump = u & -u;
            if(jump > ranks - u){
                break; // Avoid overflow beyond the final rank
            }
            u += jump;
        }
    }
    return answer;
}
```

For `[-5,10,-5,3]`, scan ranks `2,1,3,1`. Queries return `0,0,2,0`; answer is 2, from `(10,-5)` and `(10,3)`. Time $O(n\log n)$ including compression; extra space $O(n)$. This version leaves the original array unchanged. Array sizes must fit the signed-index convention in theory. Empty and all-equal inputs return zero.
</details>


<details>
<summary>Answers for Challenges 1.4–1.7 and the descending drill</summary>

**1.4:** Selection states: `[1,3,2,4]`, `[1,2,3,4]`, `[1,2,3,4]`. Each selected prefix is final. Insertion states: `[3,4,2,1]`, `[2,3,4,1]`, `[1,2,3,4]`. Its sorted prefix can move later. Inserting saved key 1 into `[2,3,4,1]` gives temporary `[2,3,4,4]`, `[2,3,3,4]`, `[2,2,3,4]`, then `[1,2,3,4]`.

**1.5:** Accept 1: `low=0, mid=1, high=2`. Swap 2 and 0: `[1,0,2]`, `high=1`, with `mid=1` unchanged. Process incoming 0: `[0,1,2]`, `low=1, mid=2`; stop. With tagged `[2a,2b,0]`, the end swaps can yield `[0,2b,2a]`, proving instability.

**1.6 merge:** Start `i=2,j=2,write=5`: write 5. Then `(2,1,4)`: write 4. Then `(1,1,3)`: write 0 from nums2 on a tie. Then `(1,0,2)`: write 0 from nums1. Then `(0,0,1)`: write -2. `j=-1`; stop. Final `[-3,-2,0,0,4,5]`. The first zero is valid because its index is below `m`.

**1.6 permutations:** `[1,5,5]`: pivot 0, successor 2; swap and reverse gives `[5,1,5]`. `[2,3,1]`: pivot 0, successor 1; swap to `[3,2,1]`, reverse suffix to `[3,1,2]`. `[3,2,1]`: no pivot; wrap to `[1,2,3]`. Next Permutation chooses the next arrangement, while full sorting always chooses the smallest one.

**1.7 choices:** Maximum: linear scan, $O(n)$ time / $O(1)$ space. Stable marks: stable Merge Sort or `stable_sort`, with the relevant buffer/stack costs. Range -10..10: Counting Sort, $O(n+21)$ time / $O(21)$ space for plain-value reconstruction. Fifth largest: QuickSelect, expected $O(n)$ / $O(1)$ for the iterative version. Bigger than RAM: sorted runs plus buffered external merging; costs depend on number of runs and I/O rounds.

**Descending drill:** Swap on `<` in Bubble; choose the maximum in Selection; shift values `< key` in Insertion; visit count buckets from high to low. Integer result `[8,7,6,5,4,3,3,2,1,1]`; character result `"fedcba"`. [Order decisions and full character implementation](./theory.md#descending-order); [descending Counting function](./theory.md#4-counting-sort) uses `countingSortValues(values, true)`.

</details>

## 6. Implementation checks before you finish

Use fresh inputs so a previous sort does not make later tests accidentally easy. Compare against a sorted copy and check duplicate counts. Test empty, one item, sorted, reverse, equal, negative, and extreme values where the algorithm permits them. Check invalid domain values for Radix and Bucket, and large ranges for Counting.

The [notes verification script](./verify_notes.py) compiles the actual complete code blocks and checks their results. It intentionally does not run the broken debugging examples. A passing test suite helps detect mistakes; it does not replace understanding the invariants.

## 7. Related problems after the core exercises

These are follow-up applications from the source notes. They are optional, and their full solutions belong to the relevant later topic.

| Official problem | Connection to this module |
|---|---|
| [Insertion Sort List](https://leetcode.com/problems/insertion-sort-list/) | Insert into a sorted prefix by changing links |
| [Sort List](https://leetcode.com/problems/sort-list/) | Merge Sort on a linked list; distinguish recursive and bottom-up memory |
| [Merge k Sorted Lists](https://leetcode.com/problems/merge-k-sorted-lists/) | Extend two-way merge with divide-and-conquer or a heap |
| [Squares of a Sorted Array](https://leetcode.com/problems/squares-of-a-sorted-array/) | Choose between two ends and fill output from the back |
| [Next Greater Element III](https://leetcode.com/problems/next-greater-element-iii/) | Next Permutation on digits, followed by integer-range checks |
| [Move Zeroes](https://leetcode.com/problems/move-zeroes/) | Stable compaction of nonzero values; do not assume arbitrary DNF swaps preserve their order |
| [Wiggle Sort II](https://leetcode.com/problems/wiggle-sort-ii/) | Selection and three-way partitioning with a different target arrangement |
| [Reverse Pairs](https://leetcode.com/problems/reverse-pairs/) | Count `i < j` with `a[i] > 2*a[j]`; use wide arithmetic and a separate counting step before merging |
