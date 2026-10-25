# Session 08 — Arrays, Linear Search, and Two Pointers

**Lesson:** [Array Data Structure — Part 1](https://www.youtube.com/watch?v=8wmn7k1TTcI) · **Prerequisites:** loops, functions, types, basic references.

[Previous](./07-Bitwise-Operators-Scope-and-Type-Modifiers.md) · [Course map](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md#course-map) · [Next](./09-Vectors-and-Single-Number.md)

**Core:** declaration, initialisation, memory layout, traversal, min/max, function parameters, linear search, reversal. **Extensions:** empty-array contracts, first/last occurrence, second distinct largest, sum/product, intersections, correctness and lower bounds.

**Navigate:** [Array model](#model) · [Function parameters](#parameters) · [Core algorithms](#algorithms) · [Extensions](#extensions) · [Revision](#revision) · [Questions](#questions).

<a id="model"></a>
## 1. One sequence, one element type, contiguous storage

A built-in array `int scores[5]` contains five int elements stored consecutively. Its **extent** is the number of elements, and it is fixed for that array object. The valid zero-based indices are `0..4`.

```text
index        0       1       2       3       4
value       10      20      30      40      50
byte addr  1000    1004    1008    1012    1016  (illustrative 4-byte int)
```

The address of element i is the base address plus `i * sizeof(element)`. This fixed arithmetic enables constant-time indexed access in the usual RAM model: you do not scan preceding elements. It does not promise an identical number of nanoseconds for every real access; caches and memory systems affect physical latency.

### Declaration and initialisation

```text
int a[4] = {10, 20, 30, 40}; // all initialised
int b[] = {10, 20, 30};      // extent deduced as 3
int c[5] = {7, 8};          // remaining elements are zero: 7,8,0,0,0
int d[5] = {};              // all zero
int e[5];                   // ordinary local elements not initialised
```

Reading an uninitialised element of e is generally UB in C++17. The type of a literal does not change the array's declared element type; the initialiser must be valid for that type.

Built-in array bounds ordinarily require a positive compile-time constant expression. `int a[n]` with runtime input n is a variable-length array extension in some compilers, **not standard C++17**. Built-in zero-length arrays are also nonstandard. Use `vector<int>` for a runtime-sized or empty sequence in the next session.

**Fixed extent is not static storage duration.** A local automatic array has fixed extent too. Arrays may be local, global, members, or dynamically allocated; “array” does not automatically mean stack storage.

### Size and traversal

At a scope where a is an actual array object, `sizeof(a) / sizeof(a[0])` gives its element count. `sizeof` measures bytes, not elements. In C++17, `size(a)` from `<iterator>` can also recover an actual array's extent.

Traverse with `i < n`, never `i <= n`. `a[n]` is one past the last element and must not be read or written. A one-past pointer can serve as a range boundary, but dereferencing it is invalid. UB can appear to work, corrupt memory, or crash; none is guaranteed.

<a id="parameters"></a>
## 2. Passing an array: pointer adjustment, not a reference

In a parameter list, `void f(int a[], int n)` is adjusted to `void f(int* a, int n)`. The function receives a pointer value. A **pointer** identifies an object/address; here it points to the first element. It is copied by value, but both caller and callee refer through it to the same underlying elements.

```text
caller array: [10, 20, 30]
               ^
pointer copy --+  function's a points to these same elements
```

Thus assigning `a[0] = 99` changes the caller's first element. Reassigning the local pointer does not replace the caller's array. This differs from an `int&` alias.

The extent is not carried by an ordinary pointer. Inside f, `sizeof(a)` is pointer size, not array size. Writing `int a[100]` in that parameter position still does not enforce or preserve the extent. Pass n explicitly, or use a type that retains size.

<!-- code: array_parameter -->
```cpp
#include <iostream> // cout
using namespace std;

void changeFirst(int a[], int n) {
    if (n > 0) {
        a[0] = 99;
    }
}

int main() {
    int values[] = {10, 20, 30};
    int n = static_cast<int>(sizeof(values) / sizeof(values[0]));
    changeFirst(values, n);
    cout << values[0] << ' ' << n << '\n';
    return 0;
}
```

Output: `99 3`. The explicit cast is safe because this small known extent fits int. For arbitrary container sizes, do not narrow without checking the range.

Use `const int a[]` in a read-only function: it adjusts to `const int*` and prevents modifying elements through that pointer. Arrays are not pointers as types; they merely convert to pointers in many expressions. `sizeof` on the original array is an important counterexample. See the primary [parameter adjustment rule](https://timsong-cpp.github.io/cppwp/n4659/dcl.fct).

All pointer-and-length functions below require `n >= 0` and a pointer to at least n valid elements when n > 0. For n = 0, a null pointer is accepted because they perform no element access. A false length can make any implementation unsafe; the function cannot infer a raw allocation's true extent.

<a id="algorithms"></a>
## 3. Linear search: first occurrence

**Task:** find the first index whose element equals target, or return -1 if absent. The array need not be sorted.

<!-- code: linear_search -->
```cpp
using namespace std;

int linearSearch(const int a[], int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (a[i] == target) {
            return i;
        }
    }
    return -1;
}
```

For `[8, 3, 8, -2]` and target 8, return 0 immediately. For target -2, test indices 0, 1, 2, 3, then return 3. For target 5, test all four and return -1. Empty input returns -1.

**Invariant:** before index i, no earlier element equals target. A match is therefore the first one. If the loop ends, every allowed index has failed.

Best-case time is `Theta(1)`; worst-case `Theta(n)`; auxiliary space `O(1)`. Average-case claims require a distribution; if a successful match is equally likely at each position, expected comparisons are `(n+1)/2`.

In an unrestricted unsorted array, worst-case linear search is optimal for exact membership: if you leave one element uninspected, that element could be the target. Binary search needs ordering or another exploitable monotonic property and belongs to a later lesson.

## 4. Minimum and maximum with an empty-input policy

For nonempty input, initialise from the **first actual element**, not zero. Starting maximum at zero fails for all-negative input.

Use an optional result when empty input has no meaningful min/max. `optional<T>` is either a T value or no value (`nullopt`). This is a small C++17 library feature, not a new algorithmic idea.

<!-- code: min_max -->
```cpp
#include <optional> // optional and nullopt
using namespace std;

struct MinMax {
    int minimum;
    int maximum;
    int minIndex;
    int maxIndex;
};

optional<MinMax> findMinMax(const int a[], int n) {
    if (n == 0) {
        return nullopt;
    }
    MinMax result{a[0], a[0], 0, 0};
    for (int i = 1; i < n; ++i) {
        if (a[i] < result.minimum) {
            result.minimum = a[i];
            result.minIndex = i;
        }
        if (a[i] > result.maximum) {
            result.maximum = a[i];
            result.maxIndex = i;
        }
    }
    return result;
}
```

`struct` groups named fields into one result. On `[4, -2, 4, 9, -2]`, the min is -2 at index 1 and max is 9 at index 3. Strict `<`/`>` comparisons keep the **first** occurrence of a tied extreme. Using `<=`/`>=` would keep the last.

**Invariant:** after processing a prefix, fields describe the min/max and first extreme indices in that prefix. Each new value can replace an extreme or leave it unchanged. One traversal gives `Theta(n)` time and `O(1)` auxiliary/result space.

## 5. Reverse in place using two pointers

Reversal preserves every element but reverses its position: old index i moves to `n - 1 - i`. It is not sorting; `[3,1,2]` reverses to `[2,1,3]`.

A **two-pointer technique** maintains two indices representing positions or boundaries. Here, start at the ends, swap the pair, and move inward.

<!-- code: reverse_array -->
```cpp
#include <utility> // swap
using namespace std;

void reverseArray(int a[], int n) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        swap(a[left], a[right]);
        ++left;
        --right;
    }
}
```

### Detailed dry run

| Before action | left | right | Array after action |
|---|---:|---:|---|
| Initial | 0 | 4 | `[1, 2, 3, 4, 5]` |
| Swap ends | 0 | 4 | `[5, 2, 3, 4, 1]` |
| Next pair | 1 | 3 | `[5, 4, 3, 2, 1]` |
| Stop condition | 2 | 2 | Unchanged; centre already correct |

**Invariant:** everything strictly left of left and strictly right of right is already in its final reversed position. Swapping the next pair grows the finalised region. The unprocessed interval shrinks by two, so termination follows.

There are `floor(n/2)` swaps: `Theta(n)` time for growing n and `O(1)` auxiliary space. Swapping through all n indices would reverse the reversal and restore the original order.

Empty input sets right to -1 and skips the loop; one element has left == right and also skips it. These are safe because this API deliberately uses a signed length/index. With an unsigned size, `size - 1` at zero would underflow; choose the loop carefully rather than copying this expression blindly.

<a id="extensions"></a>
## 6. Essential array drills and their contracts

### Sum and product

Sum starts at 0, the additive identity. Product starts at 1, the multiplicative identity. State whether an empty product should be 1 or invalid in your application. For sum, a `long long` accumulator handles common bounds such as n <= 10⁵ and |a[i]| <= 10⁹. Product grows much faster: even a few large values can exceed 64 bits. A zero does not retroactively make an already-overflowing prefix multiplication safe.

### Swap the minimum and maximum positions

Use `findMinMax`, then swap the two recorded indices if a result exists. With duplicates, state which occurrences to choose; the provided result uses the first. If all values are equal, both indices are zero and swapping is harmless. Time `O(n)`, extra space `O(1)`.

### Second largest **distinct** value

Clarify distinctness: `[5,5,3]` has second distinct largest 3, while “second by sorted position” is 5. Keep two optional candidates so the actual minimum int can remain a valid value. A magic sentinel such as -1 fails when values can be negative.

<!-- code: second_largest -->
```cpp
#include <optional> // optional and nullopt
using namespace std;

optional<int> secondLargestDistinct(const int a[], int n) {
    optional<int> largest;
    optional<int> second;
    for (int i = 0; i < n; ++i) {
        int value = a[i];
        if (!largest || value > *largest) {
            second = largest;
            largest = value;
        } else if (value != *largest && (!second || value > *second)) {
            second = value;
        }
    }
    return second;
}
```

`*largest` accesses the contained value only after its existence is established. Short-circuiting ensures an empty optional is not dereferenced. For `[4,4,2,7,7,5]`, candidate pairs `(largest,second)` are `(4,none)`, `(4,none)`, `(4,2)`, `(7,4)`, `(7,4)`, `(7,5)`. If fewer than two distinct values exist, return no value. Time `O(n)`, auxiliary space `O(1)`.

### Values occurring exactly once

Clarify “unique”: it can mean distinct values printed once, or only values whose frequency is exactly one. For `[2,2,3]`, distinct values are 2 and 3; exactly-once values contain only 3.

A beginner solution counts each value by scanning the array, then prints it only if its count is one. Time `O(n²)`, extra working space `O(1)` excluding output. A frequency map is a later optimisation. XOR is not a general frequency-count replacement; Session 09 requires specific pair-count assumptions.

### Intersection of two unsorted arrays

First decide whether multiplicities matter. For `[1,1,2]` and `[1,1,1]`, the set intersection is `[1]`; a multiset intersection contains two ones.

For a **set intersection** beginner solution: for each value in the first array, skip it if it appeared earlier there; otherwise search for it in the second and output it once on a match. Worst-case time `O(n² + nm)`, constant scalar working space excluding output. Merely nesting a search in both arrays may emit duplicates. Sorting/two pointers and hashing are later improvements with their own costs.

## 7. Operations and trade-offs

| Operation on n elements | Time | Assumption |
|---|---|---|
| Access/update known index | O(1) | Valid index |
| Traverse | Theta(n) | Visit every element |
| Unsorted membership | O(n) worst case | No auxiliary index |
| Min/max or reversal | Theta(n) | Nonempty policy stated for extrema |
| Insert/delete in middle of a logical array | O(n) shifts | Capacity exists and order preserved |

A built-in array's extent does not change. “Insert” means shifting values inside allocated capacity while maintaining a separate logical length, not growing the actual array object.

Contiguous access often has good **spatial locality**: nearby elements are likely to be fetched together by caches. It is a useful performance tendency, not a universal promise that every array traversal beats every linked-list workload or a fixed latency claim.

<a id="revision"></a>
## 8. Rapid revision and teach-back

- Built-in array: fixed extent, contiguous elements, zero-based valid indices.
- Array type is different from pointer type; decay/parameter adjustment explains shared element mutation.
- Pass a valid length; `sizeof` on a pointer does not recover extent.
- Read-only input: `const int a[]`; mutable input when modification is intended.
- Linear search returns first match; -1 is unambiguous because valid indices are nonnegative.
- Initialise extrema from actual data; choose an explicit empty-input result.
- Two-pointer reversal finalises one pair per iteration and stops before crossing.
- Distinct versus repeated values is a problem-contract decision.

**Teach back:** draw the array/pointer relationship, prove reversal with a finalised-region invariant, then explain why -1 is a safe missing-index sentinel but an unsafe missing-value sentinel.

## 9. Sources

Local note: `03_arrays_and_vectors/01_array_data_structure_part1.md`. Code reviewed: `09-arrays-part-1/04_array_by_refrensing.cpp` and `09-arrays-part-1/07_reverse_arr_2_pointer.cpp`. The function title in the older material described references; the actual pointer-passing mechanism is corrected here.

Primary C++ references: [array declarators](https://timsong-cpp.github.io/cppwp/n4659/dcl.array), [array-to-pointer conversion](https://timsong-cpp.github.io/cppwp/n4659/conv.array), and [function parameter adjustment](https://timsong-cpp.github.io/cppwp/n4659/dcl.fct). Advanced drills are additions to the core lesson and do not assume sorting or hash maps already learned.

<a id="questions"></a>
## 10. Revision questions

1. Explain contiguous storage and derive the address of element i. Does O(1) mean identical physical latency? [Review](#model)
2. Give the contents of `int a[5] = {4,7};`. Contrast with `int a[5];` as an ordinary local declaration. [Review](#model)
3. Why are `a[n]`, a runtime-sized built-in array, and a zero-length built-in array problematic in standard C++17? [Review](#model)
4. Why does `sizeof(a)/sizeof(a[0])` work for an actual array but fail in an ordinary array parameter? [Review](#parameters)
5. Is `void f(int a[])` pass by reference? Explain how changes to an element still affect the caller. [Review](#parameters)
6. **Implementation:** linear search returning first occurrence; give present, absent, duplicate, and empty tests. [Review](#algorithms)
7. Prove worst-case linear search requires examining every element without more structure. [Review](#algorithms)
8. Why does maximum initialised to zero fail? How should empty min/max input be represented? [Review](#algorithms)
9. On `[5,-1,5,-1]`, which extreme indices does findMinMax return? How would last-occurrence policy change comparisons? [Review](#algorithms)
10. **Dry run:** reverse `[2,7,4,9]`. State the invariant and exact number of swaps. [Review](#algorithms)
11. Why does swapping all n symmetric pairs undo the reversal? What happens for n = 0 in the supplied signed-index API? [Review](#algorithms)
12. **Implementation:** second distinct largest for `[5,5,3]`, `[-5,-2,-2]`, and `[7,7]`. Avoid magic sentinels. [Review](#extensions)
13. Distinguish distinct values and values occurring once. Can XOR solve arbitrary frequency questions? [Review](#extensions)
14. Explain set versus multiset intersection and why a naive nested comparison can duplicate output. [Review](#extensions)
15. What does insertion mean if a built-in array's extent cannot grow? [Review](#extensions)
16. **Interview:** compare input storage, auxiliary space, and output space for reversal versus making a reversed copy. [Review](#revision)

<details>
<summary>Answer key</summary>

1. Consecutive elements; base + i*sizeof(element). Constant model work, not identical hardware latency.
2. 4,7,0,0,0. The uninitialised local array has elements that must not be read as if valid values had been stored.
3. a[n] is out of bounds; runtime extents and zero extents are nonstandard built-in array forms. Use vector for runtime/empty sequences.
4. Array sizeof includes all elements; a parameter is adjusted to a pointer and sizeof measures that pointer.
5. A pointer is passed by value; it refers to the same caller-owned elements. It is not an int-array reference parameter.
6. Section 3; first match index, otherwise -1. Empty needs no access.
7. An uninspected element could still contain the target.
8. On [-4,-2], zero is not even an input value and incorrectly exceeds every candidate. Return optional/no-result or reject empty input explicitly.
9. Min index 1 and max index 0. Use <= and >= for last-occurrence selection.
10. Swap 0/3 -> [9,7,4,2], then 1/2 -> [9,4,7,2]. Two swaps; outer regions already final.
11. Each pair is swapped twice. For zero, right=-1 and condition fails without element access.
12. 3; -5; no value. Section 6 uses optional candidates.
13. [2,2,3] gives distinct 2,3 but exactly-once 3. No; XOR only preserves bit-parity information, not full frequencies.
14. One instance per shared value versus min of counts; repeated matches can output the same value multiple times.
15. Shift within spare physical capacity and change a separate logical length; extent remains fixed.
16. In-place reversal uses O(1) extra storage. A new reversed array uses O(n) output storage; both require O(n) time.

</details>
