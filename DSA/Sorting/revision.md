# Sorting — 15-Minute Revision

Read this after [theory](./theory.md). Say each rule aloud, then check it on a small example. For exercises, open [practice](./practice.md).

## 1. The words you must know

- **Sorted:** Neighbouring pairs follow the required order, and every original item is still present. Ascending order allows equal values.
- **Stable:** Records with equal sorting keys keep their original order. Equal ages do not mean equal names.
- **Auxiliary space:** Extra memory, including recursive call frames.
- **Strictly in-place:** Uses $O(1)$ auxiliary space. Quick Sort is often called in-place by convention; still report its stack cost.
- **Adaptive:** Uses existing order to do less work. Selection Sort still scans every remaining item.
- **Invariant:** A statement that stays true at the same point in every loop.
- **Inversion:** `i < j` and `arr[i] > arr[j]`. Equal values do not form an inversion.

## 2. Complexity table — match the version to the claim

`n` = item count, `k` = Counting Sort range `max - min + 1`, `d` = digit count, `b` = radix base, `B` = bucket count. Average cases need an input model. Bounds describe growth, not exact seconds.

| Algorithm in our notes | Best time | Average / expected time | Worst time | Extra space | Stable? |
|---|---|---|---|---|---|
| Bubble with early exit | $\Theta(n)$ | $O(n^2)$ | $\Theta(n^2)$ | $O(1)$ | Yes, strict swap test |
| Selection with swaps | $\Theta(n^2)$ | $\Theta(n^2)$ | $\Theta(n^2)$ | $O(1)$ | No |
| Insertion with shifts | $\Theta(n)$ | $O(n^2)$ | $\Theta(n^2)$ | $O(1)$ | Yes, strict shift test |
| Counting: stable output | $\Theta(n+k)$ | $\Theta(n+k)$ | $\Theta(n+k)$ | $O(n+k)$ | Yes |
| Counting: rebuild values | $\Theta(n+k)$ | $\Theta(n+k)$ | $\Theta(n+k)$ | $O(k)$ | Record identity not retained |
| Merge, without skip check | $\Theta(n\log n)$ | $\Theta(n\log n)$ | $\Theta(n\log n)$ | $O(n)$ buffer + $O(\log n)$ stack | Yes, left first on ties |
| Quick, bounded-stack Lomuto | $\Theta(n\log n)$ | $O(n\log n)$ for random distinct-key order | $\Theta(n^2)$ | $O(\log n)$ stack | No |
| Quick, three-way code | $\Theta(n)$ for all equal | $O(n\log n)$ under suitable random input/pivots | $O(n^2)$ | $O(n)$ worst stack in our recursive version | No |
| Heap, iterative sift-down | $\Theta(n)$ possible for all equal | $O(n\log n)$ | $\Theta(n\log n)$ | $O(1)$ | No |
| Radix, stable LSD | $O(d(n+b))$ | $O(d(n+b))$ | $O(d(n+b))$ | $O(n+b)$ | Yes |
| Bucket, insertion inside buckets | $\Theta(n+B)$ | $O(n)$ if independent uniform values and $B=n$ | $O(n^2)$ | $O(n+B)$ | Yes with stable insertion and input-order append |
| Shell, Knuth gaps | $\Theta(n\log n)$ on sorted input | No simple general formula | $O(n^{3/2})$ | $O(1)$ | No |

The table covers nontrivial valid inputs. Empty and one-item comparison-sort inputs need constant work. Radix and Bucket validate their restricted value domains even on one item.

- Bubble **without** early exit has $\Theta(n^2)$ best time.
- Merge with the optional boundary skip check has $O(n)$ best time on sorted data.
- Recursive heapify uses $O(\log n)$ stack; our iterative version avoids it.
- Ordinary two-sided Quick Sort can use $O(n)$ worst stack. Random pivots improve expected time, not the worst-case guarantee.
- Bucket using `sort` inside buckets has $O(n\log n)$ worst time and no stability guarantee. Do not apply insertion-bucket bounds to that version.
- Shell bounds depend on the exact gaps. The [Knuth implementation](https://algs4.cs.princeton.edu/code/javadoc/edu/princeton/cs/algs4/Shell.html) uses `1, 4, 13, 40, ...`.

## 3. What each algorithm keeps true

| Algorithm | Remember this |
|---|---|
| Bubble | After pass `i`, position `n - 1 - i` holds the largest remaining value. The sorted suffix is final |
| Selection | Before pass `i`, the first `i` positions hold the smallest `i` values in final order |
| Insertion | Before pass `i`, the first `i` items are sorted. Later items may move this prefix |
| Stable Counting | Cumulative count says how many items are `<= key`; subtract 1 for an output index |
| Merge | The smaller front item of the two sorted halves is the smallest remaining item |
| Lomuto | Returned index is the pivot's final position |
| Hoare | Returned index is a split boundary; the pivot need not be there |
| Heap | Active prefix is a max-heap; extracted values form a final sorted suffix |
| LSD Radix | Each stable pass keeps the order of already-processed lower digits |
| DNF | `[0s | 1s | unknown | 2s]`; one item leaves the unknown region per iteration |

## 4. Short code reminders

These are fragments, not standalone programs. [Theory](./theory.md) contains full definitions and headers.

```cpp
// Bubble, ascending: reset swapped=false every outer pass
for(int j = 0; j < n - 1 - i; ++j){
    if(arr[j] > arr[j + 1]){
        swap(arr[j], arr[j + 1]);
        swapped = true;
    }
}
// Selection: scan first, swap once afterwards
if(arr[j] < arr[minIdx]){
    minIdx = j;
}
// Insertion: save key before shifting; check the index first
while(j >= 0 && arr[j] > key){
    arr[j + 1] = arr[j];
    --j;
}
arr[j + 1] = key;
// Counting: calculate in a wider type, then guard range before allocating
long long range = static_cast<long long>(maxVal) - minVal + 1;
// After counts and prefix sums, scan INPUT backwards:
output[--count[arr[i] - minVal]] = arr[i];
```

| Ascending | Descending |
|---|---|
| Bubble swaps on `>` | Bubble swaps on `<` |
| Selection chooses minimum | Selection chooses maximum |
| Insertion shifts on `>` | Insertion shifts on `<` |
| Counting visits low buckets first | Counting visits high buckets first |

Reversing stable ascending output reverses equal-key records too. Use a descending rule directly if stability matters.

```cpp
// Lomuto recurse ranges: [low, p-1] and [p+1, high]
// Hoare recurse ranges:  [low, s]   and [s+1, high]
// Smaller-side recursion + loop on larger side bounds Quick Sort stack to O(log n).
// Iterative heapify follows the larger child; no recursive call stack.
// DNF: move 2 to high, decrease high, DO NOT advance mid.
```

## 5. C++ library rules

| Need | Tool | Remember |
|---|---|---|
| Full order | `sort(first, last)` | $O(n\log n)$ comparison guarantee; stability not guaranteed |
| Stable full order | `stable_sort` | $O(n\log n)$ comparisons with enough memory; otherwise $O(n\log^2 n)$ |
| Smallest `k` sorted | `partial_sort(begin, begin+k, end)` | `0 <= k <= n`; rest is not sorted |
| kth smallest | `nth_element(begin, begin+k-1, end)` | `1 <= k <= n`; average linear comparisons; neither side fully sorted |
| kth largest | `nth_element(begin, begin+n-k, end)` | Duplicates count as separate positions |
| Check current order | `is_sorted` | Empty and single-item ranges return true |
| Sorted raw array | `sort(arr, arr+n)` | Excluded end is one past the last item |

These functions do not promise a particular internal algorithm or exact portable memory bound. Introsort and a threshold such as 16 are implementation choices. See [C++ sorting guarantees](https://eel.is/c++draft/alg.sort) and [nth-element requirements](https://eel.is/c++draft/alg.nth.element).

`comp(a, b)` means **a comes before b**. Require false for self-comparison, no contradictory directions, transitive ordering, and consistent ties. Checking only `comp(x, x)` does not prove all these rules.

```cpp
// Score descending, ID ascending for ties
auto comp = [](const auto& a, const auto& b){
    if(a.score != b.score){
        return a.score > b.score;
    }
    return a.id < b.id;
};
```

`return a <= b` is invalid as a sorting comparator. Merge's `left <= right` is a different decision: it selects the left item on a tie and preserves stability.

## 6. Course problem patterns

| Problem | Steps | Time / extra space |
|---|---|---|
| Sort Colors | Swap 0 left, accept 1, swap 2 right and recheck incoming item | $O(n)$ / $O(1)$ |
| Merge into padded nums1 | Start at both valid ends; fill output from the back | $O(m+n)$ / $O(1)$ |
| Next Permutation | Rightmost rise; swap with rightmost strictly greater value; reverse suffix | $O(n)$ / $O(1)$ |
| Inversion count | Merge Sort; when right wins, add remaining left count `mid-i+1` | $O(n\log n)$ / $O(n)$ |
| k-displaced array | Min-heap of up to `k+1` items | $O(n\log(k+2))$ / $O(\min(n,k+1))$ |

Next Permutation's suffix is **non-increasing**, not necessarily strictly decreasing. No pivot means reverse the whole range and report wraparound. Merge padding must exist as vector **size**, not just capacity.

## 7. Recall checks — answer before opening

1. Why is `[4a, 4b, 1]` a counterexample to Selection Sort stability?
2. Why does cumulative count 5 lead to index 4?
3. Why does smaller-side Quick Sort recursion not guarantee fast sorting?
4. Why can DNF keep `mid` unchanged and still finish in linear time?
5. Why is `[1, 5, 5]` followed by `[5, 1, 5]`?
6. Why is sorting unnecessary to find one maximum?
7. What are the two requirements of correct sorting besides the time bound?
8. What differs between recursive and iterative list Merge Sort space?

<details>
<summary>Answer key</summary>

1. Swapping `4a` with `1` gives `[1, 4b, 4a]`.
2. Counts describe a length; zero-based final index is length minus one.
3. It bounds stack depth. Bad partitions can still produce quadratic total scans.
4. Moving a 2 decreases `high`; moving a 0 or 1 increases `mid`. The unknown region shrinks every time.
5. The rightmost rise is `1 < 5`; increase 1 to 5, then minimise the suffix.
6. A linear scan finds the maximum in $O(n)$ without arranging all other values.
7. Output must follow the order and keep exactly the original items, including duplicates.
8. Recursive list Merge Sort uses $O(\log n)$ stack; iterative bottom-up can use $O(1)$ auxiliary space.

</details>
