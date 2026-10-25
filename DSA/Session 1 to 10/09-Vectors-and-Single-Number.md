# Session 09 — Vectors and Single Number

**Lesson:** [Vectors in C++ — Arrays Part 2](https://www.youtube.com/watch?v=NWg38xWYzEg) · **Prerequisites:** arrays, loops, functions, references, XOR.

[Previous](./08-Arrays-Linear-Search-and-Two-Pointers.md) · [Course map](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md#course-map) · [Next](./10-Subarrays-and-Kadanes-Algorithm.md)

**Core:** vector construction, traversal, size, capacity, push/pop, front/back/at, dynamic storage, Single Number. **Interview extensions:** amortised analysis, reserve versus resize, invalidation, safe iteration, copy costs and ownership.

**Navigate:** [Model and API](#model) · [Memory and complexity](#memory) · [Safety](#safety) · [Single Number](#single) · [Revision](#revision) · [Questions](#questions).

<a id="model"></a>
## 1. A vector is a managed, resizable sequence

`vector<T>` stores a sequence of T objects and manages their storage automatically. For ordinary element types such as int, live elements are contiguous, so valid indexed access is constant time. The `vector<bool>` specialisation can use packed bits and has different reference behavior; focus on `vector<int>` here.

The vector's size can change, but one allocation cannot stretch arbitrarily in place. When more storage is needed, the vector may allocate a new buffer, move/copy its elements, and release the old buffer. This explains both efficient growth and invalidation risks.

### Common construction forms

```text
vector<int> a;               // empty: size 0
vector<int> b = {10,20,30};  // three values
vector<int> c(4);            // four int elements, value-initialised to zero
vector<int> d(4,7);          // four copies of 7
vector<int> e{4,7};          // two elements: 4 and 7
vector<int> copy = b;        // independent copy of b's elements
```

Parentheses and braces can mean different things: `(4,7)` means count and value, `{4,7}` means the listed elements. Include `<vector>`, which declares the container.

For runtime input n, validate that n is nonnegative and within the task's memory limits before constructing `vector<int> values(n)`. A negative int converted to the vector's unsigned size type becomes a huge value. Valid language syntax does not imply a feasible allocation.

## 2. Size and capacity are different contracts

- **Size:** number of live elements, whose valid indices are `[0, size())`.
- **Capacity:** how many elements fit in the current storage without reallocation.
- Always `size() <= capacity()`.

```text
values: [10 | 20 | 30 | unused storage | unused storage]
         <--- size 3 --->
         <-------------- capacity 5 ------------------>
```

Unused capacity is **not** additional vector elements. Even if storage exists, `v[3]` is invalid when size is 3.

| Operation | Effect |
|---|---|
| `push_back(x)` | Append a live element, size increases by 1 |
| `pop_back()` | Destroy last element, size decreases by 1; requires nonempty vector |
| `front()`, `back()` | Access first/last live element; require nonempty vector |
| `v[i]` | Unchecked access; requires i < size |
| `v.at(i)` | Checked access; throws out_of_range for invalid index |
| `empty()` | Whether size is zero |
| `reserve(k)` | Ensure capacity at least k when successful; size unchanged |
| `resize(k)` | Change size to k; growing an int vector adds zeros |
| `resize(k,x)` | Growing size appends copies of x |
| `clear()` | Destroy all elements; size becomes zero; capacity retained |

`pop_back()` returns no removed value. To use the old last value, read `back()` before popping, after checking nonempty.

### Reserve does not create elements

```text
vector<int> v;
v.reserve(100);
v[0] = 7; // invalid: size is still zero
```

Either `push_back(7)` to append, or `resize(100)` to create 100 elements before indexing them. Calling `reserve` with a smaller number does not reduce capacity. `shrink_to_fit` is a **nonbinding request**, and shrinking size does not itself shrink capacity. See [C++17 vector capacity rules](https://timsong-cpp.github.io/cppwp/n4659/vector.capacity).

## 3. A complete state-changing example

<!-- code: vector_operations -->
```cpp
#include <iostream> // cout
#include <vector> // vector
using namespace std;

int main() {
    vector<int> values{10, 20, 30};
    values.reserve(10);
    values.push_back(40);
    values.pop_back();
    values.resize(5, -1);

    for (int& value : values) {
        value += 1;
    }
    values.erase(values.begin() + 1);

    for (int value : values) {
        cout << value << ' ';
    }
    cout << '\n' << values.size() << ' ' << (values.capacity() >= 10) << '\n';
    return 0;
}
```

| Operation completed | Live values | Size | Capacity information |
|---|---|---:|---|
| Construct | `[10,20,30]` | 3 | At least 3 |
| Reserve 10 | Same | 3 | At least 10 |
| Append 40 | `[10,20,30,40]` | 4 | No reallocation needed |
| Pop | `[10,20,30]` | 3 | Unchanged |
| Resize 5, -1 | `[10,20,30,-1,-1]` | 5 | Unchanged here |
| Reference traversal | `[11,21,31,0,0]` | 5 | Unchanged |
| Erase index 1 | `[11,31,0,0]` | 4 | Unchanged |

The final lines show `11 31 0 0` and `4 1`, with a trailing space after each printed element on the first line. Capacity is compared with 10 instead of predicting an implementation-specific exact value.

## 4. Traversal, copying, and function parameters

| Form | What each iteration receives | Changes original element? |
|---|---|---|
| `for (int x : values)` | Copy of the element | No |
| `for (int& x : values)` | Mutable reference to element | Yes |
| `for (const int& x : values)` | Read-only reference | No through x |

For small ints, copying an element is cheap. For large objects such as strings, `const auto&` avoids potentially expensive per-element copies when only reading.

Use `const vector<int>& values` for read-only function input. `vector<int> values` passed by value makes an independent vector copy for an ordinary lvalue argument; that costs `O(n)` element copying and additional storage. `vector<int>&` allows intentional in-place changes. Passing a const reference costs `O(1)` to establish the reference; the function's own traversal can still be `O(n)`.

`size()` returns an unsigned size type. Prefer range-for when an index is unnecessary. A forward indexed loop can use `size_t i = 0; i < values.size(); ++i`. For reverse iteration, avoid `size()-1` on an empty vector. A safe pattern is `for (size_t i = values.size(); i > 0; --i)` followed by access at `values[i-1]`.

<a id="memory"></a>
## 5. Storage ownership and automatic cleanup

A local `vector<int> v` is an ordinary local object, and it manages dynamically allocated element storage with the default allocator. When v's lifetime ends, its destructor destroys the elements and releases the owned allocation. This is **RAII**: resource ownership is tied to object lifetime, reducing manual cleanup errors.

The vector object itself can also be global, a class member, or dynamically allocated. It is not guaranteed to live on the stack. Common implementations store pointer-like bookkeeping, but neither “exactly three pointers” nor “always 24 bytes” is a portable vector guarantee.

Contrast with manually allocating `new int[n]`, where a matching `delete[]` would need correct ownership and lifetime handling. For these DSA problems, vector provides the needed runtime sizing without manual allocation code.

### Fixed array versus vector

| Property | Built-in array | vector<int> |
|---|---|---|
| Extent/size | Extent fixed for object | Size can change |
| Empty sequence | No standard zero-extent built-in array | Supported |
| Indexed access | Constant time, unchecked | Constant time; checked alternative at |
| Function input | Often decays/adjusts to pointer; explicit length | Carries size; can pass reference |
| Copy semantics | No direct whole-array assignment | Value-copy semantics |
| Storage management | Depends on declaration/allocation | Container manages owned element storage |

## 6. Why append is amortised O(1)

One append with spare capacity constructs one element and takes constant element work. An append that reallocates may move/copy all n existing elements, so its worst-case time is `O(n)`.

**Amortised** means the cost averaged over a whole sequence of operations in a worst-case sequence analysis, not an average over random inputs. Expensive reallocations are infrequent enough that N successive appends take `O(N)` total element work.

Consider an illustrative doubling strategy: capacities 1, 2, 4, 8, ... . Moving elements at reallocations costs `1 + 2 + 4 + ...`. Before N appends complete, this geometric sum is less than `2N`; adding N new-element writes still gives less than `3N` elementary element operations. For N equal to a power of two, moved old elements total N-1.

**Doubling is an illustration, not the standard's required growth factor.** Standard vector promises amortised constant-time end insertion without specifying one exact capacity sequence. See the primary [vector overview](https://timsong-cpp.github.io/cppwp/n4659/vector.overview).

If capacity grew by exactly one each append, old-element moves would be `0 + 1 + ... + (N-1) = Theta(N²)`. Repeatedly forcing tiny reserves can undermine the intended growth policy. If you know a reasonable final count, reserve it once; do not call `reserve(size()+1)` before every append.

### Complexity table for ordinary int elements

| Operation | Time | Explanation |
|---|---|---|
| Indexed access, front, back, size, empty | O(1) | Direct bookkeeping/access |
| Append | Amortised O(1), worst O(n) | Possible reallocation |
| Pop last | O(1) | Destroy one int |
| Insert/erase in middle | O(n) worst case | Shift later elements |
| Copy vector | O(n) | Copy elements |
| Traverse | Theta(n) | Visit every element |
| Clear | O(n) general element model | Destroy each element; trivial destruction may be optimised |

The vector owns O(capacity) element storage; most growth discussions assume a reasonable geometric policy with capacity O(n). Calling `reserve(K)` for K much larger than n intentionally allocates beyond that assumption. During reallocation, old and new buffers can coexist temporarily.

<a id="safety"></a>
## 7. Invalidation: saved element addresses can expire

An **iterator** identifies a position in a container; `begin()` refers to the first element and `end()` to the position after the last. Never dereference end. Pointers and references can also refer to vector elements.

- If a vector reallocates, all existing element pointers, references, and iterators are invalidated.
- An append without reallocation preserves references/iterators to existing elements, but the old end iterator is invalidated.
- Erasing an element invalidates references/iterators at or after the erased position, even if no allocation changes.
- `clear()` ends all element lifetimes, even though capacity remains.

These are correctness rules from [vector modifiers](https://timsong-cpp.github.io/cppwp/n4659/vector.modifiers), not merely performance advice.

```text
int& first = v[0];
v.push_back(9); // may reallocate
use(first);     // unsafe if reallocation occurred
```

A safer design keeps an index and reacquires `v[index]` after growth, provided the same logical position still makes sense. Erasing earlier elements changes position meanings, so even an index is not a universal identity.

Do not append while using an ordinary range-for over the same vector. Range-for saves iteration boundaries; even without reallocation the old end is invalidated. Use an intentional indexed traversal over a fixed original size, with capacity/reference issues accounted for, or build a separate output vector.

<a id="single"></a>
## 8. Single Number: from counting to XOR

**Problem contract:** a nonempty integer array has one value appearing once; every other value appears exactly twice. Return that single value. The official [LeetCode 136 statement](https://leetcode.com/problems/single-number/) additionally asks for linear time and constant extra space.

Clarify the frequency guarantee before choosing an algorithm. “One unique value” without a count rule can mean several different problems.

### Evolving the approach

1. For each value, count matches by scanning all values. A count of one identifies the answer: `O(n²)` time, `O(1)` scalar extra storage.
2. A frequency table records counts: typically expected `O(n)` time with hashing and `O(n)` extra storage; hash-table details are a later topic.
3. XOR all elements: duplicate pairs cancel, leaving the unpaired value. `O(n)` time, `O(1)` extra storage.

<!-- code: single_number -->
```cpp
#include <stdexcept> // invalid_argument
#include <vector> // vector
using namespace std;

// Caller guarantees one once-occurring value and all others exactly twice.
int singleNumber(const vector<int>& values) {
    if (values.empty()) {
        throw invalid_argument("Single Number requires nonempty input");
    }
    int answer = 0;
    for (int value : values) {
        answer ^= value;
    }
    return answer;
}
```

### Dry run for [4, 1, 2, 1, 2]

| Read value | Old answer | XOR | New answer |
|---:|---:|---|---:|
| 4 | 0 | 0000 ^ 0100 | 4 |
| 1 | 4 | 0100 ^ 0001 | 5 |
| 2 | 5 | 0101 ^ 0010 | 7 |
| 1 | 7 | 0111 ^ 0001 | 6 |
| 2 | 6 | 0110 ^ 0010 | 4 |

**Invariant:** answer equals the XOR of the prefix processed so far. At completion, regroup equal pairs using associativity and commutativity. Each pair becomes zero; XORing zero preserves the remaining value. The intermediate answer need not be an element of the input.

**Tests:** `[7] -> 7`, `[0,8,8] -> 0`, `[-3,2,2] -> -3`, and separated pairs as above. This is not a “find a nonzero value” operation; zero is a valid answer.

### What the algorithm does not establish

The code rejects empty input but does **not validate the complete frequency promise**. For `[1,2,3]`, it returns 0 even though no input value is zero and the problem contract is violated. For values repeated three times, pairs no longer cancel as intended. For two singletons, XOR returns their combination, not both identities. Such variants need different algorithms and belong to later bit-manipulation work.

Worst-case linear time is optimal under this input model: an unread value can change the singleton. Constant extra space follows from a fixed number of scalar accumulators; the supplied vector is input space, not newly allocated working memory.

<a id="revision"></a>
## 9. Rapid revision and teach-back

- Live index range is determined by size, never capacity.
- Reserve changes capacity; resize changes size.
- Exact capacity growth and vector-object byte size are implementation details.
- Append is amortised O(1), with an O(n) reallocating operation possible.
- Pass a const reference for read-only access without an element copy.
- Reallocation invalidates element handles; erasing also shifts/invalidate positions.
- XOR cancellation requires the stated frequency pattern.
- Returning an XOR is not proof that the input satisfied that pattern.

**Teach back:** explain vector growth using an expanding bookshelf analogy, then replace the analogy with size/capacity and invalidation rules. Derive Single Number from per-bit XOR identities without saying “it is a trick”.

## 10. Sources

Local note: `03_arrays_and_vectors/02_vectors_in_cpp_part2.md`. Code reviewed: `14-vectors/vector-push-pop-capacity.cpp`. Corrections distinguish illustrative doubling from guaranteed behavior and separate the vector object's placement from its dynamically managed element buffer. Primary container rules and official practice are linked beside the corresponding discussion.

<a id="questions"></a>
## 11. Revision questions

1. Compare `vector<int> a(3,5)` and `vector<int> b{3,5}`. [Review](#model)
2. Define size and capacity. Which one controls whether `v[i]` is valid? [Review](#model)
3. **Debug:** reserve 100 on an empty vector, then write v[0]. Why is this invalid? Give two repairs. [Review](#model)
4. Predict the live values after each operation in the full vector example. Can exact capacity be predicted portably? [Review](#model)
5. What preconditions do front, back, and pop_back require? Does pop_back return the removed value? [Review](#model)
6. Compare `[]` and `at`. Does invalid `[]` guarantee an exception or crash? [Review](#model)
7. Explain range-for by value, mutable reference, and const reference. [Review](#model)
8. Why pass `const vector<int>&` for reading? What is the copy cost of an ordinary by-value lvalue argument? [Review](#model)
9. Prove amortised constant-time append with a geometric-growth example. Distinguish amortised and average case. [Review](#memory)
10. Does the standard require capacity doubling or a 24-byte vector object? [Review](#memory)
11. When does append invalidate all element references? What changes even if it does not reallocate? [Review](#safety)
12. Why can erase invalidate positions without reallocation? Why is append during range-for unsafe? [Review](#safety)
13. State Single Number's frequency promise and derive XOR cancellation. [Review](#single)
14. **Dry run:** solve `[6,2,6,4,2]` and `[0,-1,-1]`. [Review](#single)
15. **Implementation:** write the XOR solution, stating time, auxiliary space, and input assumptions. [Review](#single)
16. Does a returned result prove the frequency promise? Give a counterexample. [Review](#single)
17. **Interview:** explain why a vector is usually preferable to manual new[] for these problems. What still needs attention? [Review](#revision)

<details>
<summary>Answer key</summary>

1. Three copies of 5; two elements 3 and 5.
2. Live element count versus available slots before reallocation. Size controls valid access.
3. Reserve creates storage, not elements. Append with push_back, or resize before valid indexed writes.
4. Table in Section 3; final [11,31,0,0], size 4, capacity >=10. Exact growth is not a portable prediction.
5. Nonempty vector. Pop returns void; read back before popping if needed.
6. Unchecked precondition versus range-checked exception. Invalid [] is UB, not a guaranteed crash/exception.
7. Copy cannot update original; mutable reference can; const reference is read-only through that alias.
8. No element copy on entry. Ordinary by-value copying costs O(n) time and extra element storage.
9. Old-element moves form a geometric sum O(N), plus N writes. Amortised analyses a whole operation sequence without a random-input assumption.
10. Neither.
11. Reallocation invalidates all; old end is invalid even without reallocation.
12. Later elements shift and positions/lifetimes change. Range-for's saved end becomes invalid on append.
13. One singleton, all others twice. Regroup pairs, use x^x=0 and x^0=x.
14. 4 and 0.
15. Section 8; Theta(n) traversal, O(1) extra space; full count promise is supplied by caller.
16. No. [1,2,3] produces zero even though it violates the promise.
17. Automatic lifetime-based cleanup, size tracking, resizing and value semantics. Bounds, invalidation, capacity limits, and copy costs still matter.

</details>
