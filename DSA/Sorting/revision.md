# Sorting Algorithms — Quick Revision & High-Density Recall Sheet

> **Module:** `unified-notes/DSA/Sorting/revision.md`  
> **Use Case:** 15-minute refresher before technical interviews or exams.  
> **Companions:** Read after [`theory.md`](./theory.md); drill with [`practice.md`](./practice.md).

---

## 1. Golden Rules & Invariants Summary

1. **Comparison Lower Bound:** Any comparison-based sorting algorithm requires $\ge \lceil\log_2(n!)\rceil = \Omega(n \log n)$ comparisons in the worst case (the "20 Questions" information barrier).
2. **Stability Axiom:** Relative order of identical keys is preserved.
   - Stable: Bubble Sort, Insertion Sort, Merge Sort, Stable Counting Sort, Radix Sort, TimSort.
   - Unstable: Selection Sort, Quick Sort, Heap Sort, Shell Sort.
3. **Inversion Principle:** Insertion Sort takes $O(n + I)$ time, where $I$ is the number of inversions. Ideal for nearly-sorted data ($O(n)$ if $I = O(n)$).
4. **Minimum Writes Property:** Selection Sort makes at most $N - 1$ swaps across its entire execution ($O(n)$ memory writes). Ideal when write operations degrade storage media (Flash/EEPROM).
5. **Linear Time Conditions:** Counting Sort and Radix Sort achieve $O(n)$ only when key range $k \le O(n)$ or digit count $d = O(1)$.

---

## 2. Master Algorithmic Complexity Table

| Algorithm | Best Time | Average Time | Worst Time | Auxiliary Space | Stable? | In-Place? | Adaptive? |
|---|---|---|---|---|---|---|---|
| **Bubble Sort** | $O(n)$ | $O(n^2)$ | $O(n^2)$ | $O(1)$ | **Yes** | **Yes** | **Yes** |
| **Selection Sort** | $O(n^2)$ | $O(n^2)$ | $O(n^2)$ | $O(1)$ | **No** | **Yes** | **No** |
| **Insertion Sort** | $O(n)$ | $O(n^2)$ | $O(n^2)$ | $O(1)$ | **Yes** | **Yes** | **Yes** |
| **Counting Sort** | $O(n + k)$ | $O(n + k)$ | $O(n + k)$ | $O(n + k)$ | **Yes\*** | **No** | **No** |
| **Merge Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(n)$ buffer | **Yes** | **No** | **No** |
| **Quick Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(n^2)$ | $O(\log n)$ stack\*\* | **No** | **Yes** | **No** |
| **Heap Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(1)$ | **No** | **Yes** | **No** |
| **Radix Sort (LSD)**\*\*\* | $O(d(n + b))$ | $O(d(n + b))$ | $O(d(n + b))$ | $O(n + b)$ | **Yes** | **No** | **No** |
| **Bucket Sort**† | $O(n)$ | $O(n)$ | $O(n^2)$ | $O(n + k)$ | **Subroutine-dep.§** | **No** | **No** |
| **Shell Sort**‡ | $O(n \log n)$ | Sequence-dep. | $O(n^{1.5})$ or $O(n^2)$ | $O(1)$ | **No** | **Yes** | **Partially** |

- *\*With stable prefix sums & output array (frequency-overwrite is unstable and only for scalar keys).*  
- *\*\*Requires explicit recursion on smaller partition and loop over larger partition in C++.*  
- *\*\*\*Precondition: Non-negative integers (or keys mapped to positive digits).*  
- *†Precondition: Uniformly distributed in $[0.0, 1.0)$ or normalized.*  
- *‡Complexity strictly depends on gap sequence: Shell $\Theta(n^2)$, Knuth/Hibbard $O(n^{1.5})$, Sedgewick $O(n^{1.33})$. No universal average exists.*  
- *§Bucket Sort is only stable if a stable algorithm (e.g. std::stable_sort or Insertion Sort) is used within buckets. Using std::sort within buckets is unstable.*

---

## 3. Critical Code Patterns (Syntax Memory Triggers)

### 3.1 Bubble Sort (Adaptive Swapped Flag)
```cpp
for (int i = 0; i < n - 1; ++i) {
    bool swapped = false;
    for (int j = 0; j < n - 1 - i; ++j) {
        if (arr[j] > arr[j + 1]) {
            swap(arr[j], arr[j + 1]);
            swapped = true;
        }
    }
    if (!swapped) break;
}
```

### 3.2 Selection Sort (Minimum Index Swap)
```cpp
for (int i = 0; i < n - 1; ++i) {
    int minIdx = i;
    for (int j = i + 1; j < n; ++j) {
        if (arr[j] < arr[minIdx]) minIdx = j;
    }
    if (minIdx != i) swap(arr[i], arr[minIdx]);
}
```

### 3.3 Insertion Sort (Shift Method)
```cpp
for (int i = 1; i < n; ++i) {
    int key = arr[i], j = i - 1;
    while (j >= 0 && arr[j] > key) {
        arr[j + 1] = arr[j];
        j--;
    }
    arr[j + 1] = key;
}
```

### 3.4 Stable Counting Sort (Prefix Sums + Backward Scan)
```cpp
// 1. Safe range calculation guarding against signed 32-bit int overflow
long long range = static_cast<long long>(maxVal) - minVal + 1;
// 2. Tally frequencies
for (int x : arr) count[x - minVal]++;
// 3. Prefix sums
for (size_t i = 1; i < count.size(); ++i) count[i] += count[i - 1];
// 4. Backward scan guarantees stability
for (int i = n - 1; i >= 0; --i) {
    output[--count[arr[i] - minVal]] = arr[i];
}
```

### 3.5 Merge Sort (Linear Merge with Stability)
```cpp
while (i <= mid && j <= high) {
    temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++]; // '<=' ensures stability
}
while (i <= mid) temp[k++] = arr[i++];
while (j <= high) temp[k++] = arr[j++];
```

### 3.6 QuickSort with Bounded Logarithmic Stack Space in C++
```cpp
while (low < high) {
    int p = partition(arr, low, high);
    if (p - low < high - p) {
        quickSort(arr, low, p - 1); // Recurse on smaller half
        low = p + 1;                // Loop on larger half
    } else {
        quickSort(arr, p + 1, high);
        high = p - 1;
    }
}
```

### 3.7 HeapSort Sift-Down
```cpp
int largest = i, l = 2 * i + 1, r = 2 * i + 2;
if (l < n && arr[l] > arr[largest]) largest = l;
if (r < n && arr[r] > arr[largest]) largest = r;
if (largest != i) {
    swap(arr[i], arr[largest]);
    heapify(arr, n, largest);
}
```

### 3.8 Safe C++ Lambda Comparator (Strict Weak Ordering)
```cpp
// MUST use '<' or '>', NEVER '<=' or '>=' (violating strict weak ordering causes undefined behavior; crash is possible, not guaranteed)
auto comp = [](const auto& a, const auto& b) {
    if (a.score != b.score) return a.score > b.score; // high score first
    return a.id < b.id;                               // tie-break: low id first
};
```

### 3.9 C++ STL Selection Shortcuts
```cpp
// Top-k sorted elements in O(n log k) time:
std::partial_sort(v.begin(), v.begin() + k, v.end());

// kth element / median in O(n) average time:
// NOTE: v.begin() + k places the element that belongs at ZERO-BASED index k (which is the (k + 1)th smallest element in 1-based human terms)
std::nth_element(v.begin(), v.begin() + k, v.end());
```

---

## 4. Frequently Confused Concepts

| Concept A | Concept B | Core Distinction |
|---|---|---|
| **Bubble Sort** | **Selection Sort** | Bubble Sort constantly swaps adjacent elements throughout the pass; Selection Sort scans the whole suffix and swaps at most once per pass. |
| **Lomuto Partition** | **Hoare Partition** | Lomuto uses two forward pointers ($i, j$) and does more swaps; Hoare uses inward pointers and executes ~3x fewer swaps, handling duplicate keys much better. |
| **`std::sort`** | **`std::stable_sort`** | `std::sort` is commonly Introsort (in-place, **unstable**). `std::stable_sort` is commonly adaptive MergeSort (**stable**, uses $O(n)$ buffer if available). |
| **Heapify vs. Build-Heap** | Complexities | A single `heapify` operation is $O(\log n)$. Bottom-up `Build-Heap` on an entire array of size $n$ is **$O(n)$** linear time. |
| **Counting Sort vs. DNF** | Single-pass sorting | Counting sort takes 2 passes and extra memory; Dutch National Flag (DNF) sorts 3 discrete keys in a **single pass** in-place ($O(1)$ space). |

---

## 5. 10-Minute Rapid Self-Test

1. *Why does `[4a, 4b, 1]` prove Selection Sort is unstable?*  
   $\to$ `4a` swaps with `1`, placing `4a` after `4b` $\implies [1, 4b, 4a]$. (A modified version that shifts rather than swaps can be stable, but requires $O(n^2)$ shifts).
2. *Why is `Build-Heap` $O(n)$ instead of $O(n \log n)$?*  
   $\to$ Nodes at lower heights far outnumber nodes at the top. The summation converges to $O(n)$.
3. *How do you guarantee $O(\log n)$ stack depth in QuickSort in C++?*  
   $\to$ Recurse explicitly into the smaller partition first, and loop over the larger partition using a `while` loop (C++ does not guarantee automatic tail-call optimization).
4. *What happens if your C++ comparator returns `true` for equal values?*  
   $\to$ Violates the Irreflexivity axiom of Strict Weak Ordering, triggering Undefined Behavior under the C++ standard (which frequently causes crashes/segfaults due to missing boundary sentinels, though a crash is not guaranteed).
5. *Why is Merge Sort preferred for Linked Lists?*  
   $\to$ Sequential access requires no random indexing, and merging rewires pointers with $O(1)$ auxiliary memory.
