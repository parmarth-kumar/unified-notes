# Session 10 — Subarrays and Kadane's Algorithm

**Lesson:** [Kadane's Algorithm — Maximum Subarray Sum](https://www.youtube.com/watch?v=9IZYqostl2M) · **Prerequisites:** arrays/vectors, loops, min/max, functions and integer bounds.

[Previous](./09-Vectors-and-Single-Number.md) · [Course map](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md#course-map)

**Core:** contiguous subarrays, enumeration, maximum subarray, brute force, running sums, Kadane, all-negative input. **Interview extensions:** recurrence and proof, indices and ties, prefix-minimum interpretation, fixed-length/circular variations and limits of reuse.

**Navigate:** [Definitions](#definitions) · [Baseline solutions](#baselines) · [Kadane](#kadane) · [Indices](#indices) · [Extensions](#extensions) · [Revision](#revision) · [Questions](#questions).

<a id="definitions"></a>
## 1. Clarify the object being optimised

A **subarray** is a contiguous slice: choose a left boundary L and right boundary R, include every element from L through R. A **subsequence** preserves relative order but can skip positions. A mathematical **subset** ignores sequence order.

For `[3,-2,5]`, `[3,-2]` is a subarray. `[3,5]` is a subsequence but not a subarray. You cannot remove an inconvenient middle negative value from a subarray without changing its boundaries.

**Maximum Subarray task:** choose a **nonempty contiguous** slice whose element sum is largest. Return its sum. The official [LeetCode 53 statement](https://leetcode.com/problems/maximum-subarray/) specifies nonempty input. These notes also define how the functions reject empty input rather than inventing an answer.

### Interview clarifications

- Must the selected subarray be nonempty?
- Can values be negative or zero?
- Return the sum, indices, or actual copied elements?
- Is circular wraparound allowed? Is there a length restriction?
- What are the size/value bounds? Can the sum exceed int?
- If several intervals tie, which one should be returned?

**Contracts for all implementations here:** nonempty vector, length at most 200000, each value in `[-10^9,10^9]`. Thus any subarray sum has magnitude at most `2 * 10^14`, which fits signed 64-bit long long. Empty input is explicitly rejected. The size/value bounds are caller preconditions; the code does not rescan to validate them. Use the cubic implementation only on small test inputs; its larger domain is mathematically valid but computationally impractical.

## 2. Count and enumerate subarrays

For a fixed start L, possible ends are L through n-1: `n-L` choices. Summing over starts gives `n + (n-1) + ... + 1 = n(n+1)/2` nonempty subarrays.

For `[1,2,3]` they are `[1]`, `[1,2]`, `[1,2,3]`, `[2]`, `[2,3]`, `[3]`: six, not eight.

There are 2ⁿ choices of **positions** for a subsequence when the empty choice is allowed. Repeated values can make different position selections produce the same value sequence; 2ⁿ is not universally the count of distinct subsequence values. Similarly, the elementary 2ⁿ subset formula assumes n distinct set elements or positional selections. This distinction prevents confusing a counting formula with a unique-output count.

Two loops enumerate boundaries:

```text
for L = 0..n-1
    for R = L..n-1
        process the interval [L, R]
```

Enumerating all boundaries costs `Theta(n²)` operations. Printing **all elements** of every subarray costs `Theta(n³)` total element outputs, because one interval can contain many elements. Counting outputs matters.

<a id="baselines"></a>
## 3. Baseline 1: recompute each sum — cubic

<!-- code: max_cubic -->
```cpp
#include <algorithm> // max
#include <stdexcept> // invalid_argument
#include <vector> // vector
using namespace std;

long long maxSubarrayCubic(const vector<int>& values) {
    if (values.empty()) {
        throw invalid_argument("nonempty input required");
    }
    int n = static_cast<int>(values.size());
    long long best = values[0];
    for (int left = 0; left < n; ++left) {
        for (int right = left; right < n; ++right) {
            long long sum = 0;
            for (int i = left; i <= right; ++i) {
                sum += values[i];
            }
            best = max(best, sum);
        }
    }
    return best;
}
```

It considers every valid interval and computes its sum directly, so it cannot miss the maximum. The total additions are `n(n+1)(n+2)/6`, hence `Theta(n³)` time. Working space is `O(1)` excluding the input.

This slow, simple implementation is useful as a **reference oracle** for testing faster code on small random arrays. It also gives you a correct interview starting point before optimising.

## 4. Baseline 2: reuse the running sum — quadratic

For a fixed left boundary, the sum of `[left,right]` equals the previous sum of `[left,right-1]` plus `values[right]`. Repeatedly re-reading the same prefix is unnecessary.

<!-- code: max_quadratic -->
```cpp
#include <algorithm> // max
#include <stdexcept> // invalid_argument
#include <vector> // vector
using namespace std;

long long maxSubarrayQuadratic(const vector<int>& values) {
    if (values.empty()) {
        throw invalid_argument("nonempty input required");
    }
    int n = static_cast<int>(values.size());
    long long best = values[0];
    for (int left = 0; left < n; ++left) {
        long long sum = 0;
        for (int right = left; right < n; ++right) {
            sum += values[right];
            best = max(best, sum);
        }
    }
    return best;
}
```

Initialise sum once per **left boundary**, not once per array and not inside every right iteration. Each inner iteration now takes constant work, giving `Theta(n²)` time and `O(1)` auxiliary space. This is a running interval sum; it does not store a separate prefix-sum array.

For `[2,-3,4]`, left 0 produces sums `2,-1,3`; left 1 produces `-3,1`; left 2 produces `4`. The maximum is 4 from `[4]`.

### Separate prefix-sum method

Define `prefix[0] = 0` and `prefix[i+1] = prefix[i] + values[i]`. Then the inclusive interval sum is `prefix[R+1] - prefix[L]`. Building this array takes `O(n)` time and space; testing every pair of boundaries still takes `O(n²)` time. It is useful for many range-sum queries, but it is not yet the optimal maximum-subarray algorithm.

<a id="kadane"></a>
## 5. Derive Kadane from the best sum ending here

Do not begin with “reset when negative” as a memorised command. Define two different quantities:

- `ending`: largest sum of a **nonempty** subarray that ends at the current index.
- `best`: largest sum of **any** nonempty subarray seen so far.

At index i, a subarray ending there has only two possibilities:

1. Start at i: sum is `values[i]`.
2. Extend a subarray ending at i-1: best such sum is previous ending plus `values[i]`.

Therefore:

```text
ending_i = max(values[i], ending_(i-1) + values[i])
best_i   = max(best_(i-1), ending_i)
```

Only the best previous ending matters. Any worse subarray ending at i-1 gets the same new value added and remains no better. This is an elementary **dynamic programming** state: retain the optimal result for a smaller state and reuse it. Keeping only the latest state compresses the memory from a potential table to two scalars.

### Why discarding a negative prefix is justified

If a candidate prefix has sum P < 0, attaching it before a future suffix of sum S gives `P + S < S`. Starting at the suffix is better. But a negative **element** does not automatically mean restart: a positive running sum can absorb one negative value and remain useful. For `[4,-1,2]`, the full sum 5 exceeds either positive piece alone.

When previous ending is zero, extending and restarting tie in sum. Either works for a sum-only answer; index tie policies require a deliberate choice.

<!-- code: kadane -->
```cpp
#include <algorithm> // max
#include <stdexcept> // invalid_argument
#include <vector> // vector
using namespace std;

long long maxSubarray(const vector<int>& values) {
    if (values.empty()) {
        throw invalid_argument("nonempty input required");
    }
    long long ending = values[0];
    long long best = values[0];
    for (size_t i = 1; i < values.size(); ++i) {
        long long value = values[i];
        ending = max(value, ending + value);
        best = max(best, ending);
    }
    return best;
}
```

### Full dry run

Input: `[-2,1,-3,4,-1,2,1,-5,4]`.

| Index | Value | Previous ending | Start here | Extend | New ending | best |
|---:|---:|---:|---:|---:|---:|---:|
| 0 | -2 | Initial | -2 | — | -2 | -2 |
| 1 | 1 | -2 | 1 | -1 | 1 | 1 |
| 2 | -3 | 1 | -3 | -2 | -2 | 1 |
| 3 | 4 | -2 | 4 | 2 | 4 | 4 |
| 4 | -1 | 4 | -1 | 3 | 3 | 4 |
| 5 | 2 | 3 | 2 | 5 | 5 | 5 |
| 6 | 1 | 5 | 1 | 6 | 6 | 6 |
| 7 | -5 | 6 | -5 | 1 | 1 | 6 |
| 8 | 4 | 1 | 4 | 5 | 5 | 6 |

Answer 6 comes from `[4,-1,2,1]`, indices 3 through 6. At the end ending is 5, while best is 6; returning ending would be wrong.

### Correctness proof and complexity

Base case: at index 0 the only nonempty ending interval is `[0,0]`, and both state values are its element.

Inductive step: all intervals ending at i either start there or extend an interval ending at i-1. The recurrence chooses the largest of these exhaustive alternatives. Taking the maximum with the earlier best covers all possible right endpoints through i. Thus both invariants remain true, and the final best is the global maximum.

One pass gives `Theta(n)` time and `O(1)` auxiliary space. In general every element must be inspected: an unread value could change the result arbitrarily within the permitted input bounds. That gives a matching worst-case linear lower bound in this model.

## 6. The lecture-style reset formulation

An equivalent sum-only form carries the best nonnegative suffix for the **next** element. Its meaning differs from the nonempty `ending` state after a reset.

<!-- code: kadane_reset -->
```cpp
#include <algorithm> // max
#include <stdexcept> // invalid_argument
#include <vector> // vector
using namespace std;

long long maxSubarrayReset(const vector<int>& values) {
    if (values.empty()) {
        throw invalid_argument("nonempty input required");
    }
    long long current = 0;
    long long best = values[0];
    for (int value : values) {
        current += value;
        best = max(best, current); // record the nonempty candidate first
        if (current < 0) {
            current = 0; // prepare the next iteration
        }
    }
    return best;
}
```

For `[-5,-2,-8]`, candidates before reset are -5, -2, -8; best becomes -2. If best starts at zero, or if you record the candidate only after resetting it, the algorithm may return zero—an empty choice that violates the nonempty contract.

| Input | Correct nonempty answer | Trap exposed |
|---|---:|---|
| `[-5,-2,-8]` | -2 | Initialising best to zero |
| `[-7]` | -7 | Mishandling a single negative |
| `[0,0]` | 0 | Zero can be a real nonempty optimum |
| `[1,2,3]` | 6 | Full array can be optimal |
| `[4,-1,2]` | 5 | Resetting at every negative element |
| `[5,-100,6]` | 6 | Confusing subarray with picking all positives |
| `[1000000000,1000000000,1000000000]` | 3000000000 | Sum wider than a 32-bit int |

The wide-sum example stays within the stated element contract while showing why the accumulator needs more than 32 bits.

<a id="indices"></a>
## 7. Return the interval as well as the sum

Track the start of the current best-ending candidate. When previous ending is negative, restart at i. Otherwise extend. When the candidate strictly improves the global best, save both boundaries.

**Tie policy:** earliest left index, then earliest right index. Extending when previous ending is zero retains the earlier candidate start; updating the global result only on strict improvement retains the first winning right boundary. Candidate starts never move backwards.

<!-- code: kadane_indices -->
```cpp
#include <stdexcept> // invalid_argument
#include <vector> // vector
using namespace std;

struct SubarrayResult {
    long long sum;
    int left;
    int right;
};

SubarrayResult maxSubarrayWithIndices(const vector<int>& values) {
    if (values.empty()) {
        throw invalid_argument("nonempty input required");
    }
    long long ending = values[0];
    int candidateLeft = 0;
    SubarrayResult best{ending, 0, 0};
    int n = static_cast<int>(values.size());
    for (int i = 1; i < n; ++i) {
        if (ending < 0) {
            ending = values[i];
            candidateLeft = i;
        } else {
            ending += values[i];
        }
        if (ending > best.sum) {
            best = {ending, candidateLeft, i};
        }
    }
    return best;
}
```

For `[0,1,-1,1]`, maximum sum is 1. Both `[0,1]` at 0..1 and `[1]` at 1..1 achieve it; this policy returns 0..1. A shortest-interval policy would need different tie comparisons. For the main dry run, candidateLeft becomes 1, then 3; the saved winning interval eventually becomes 3..6.

Returning indices and sum still uses `O(1)` output and working space. Returning a copied vector of k winning elements needs `O(k)` output memory and copying time in addition to finding the interval.

<a id="extensions"></a>
## 8. Interview extension: prefix sum minus the smallest earlier prefix

With prefix sums, an interval ending before prefix position R has sum `prefix[R] - prefix[L]`, where L < R. For a fixed R, maximise this by subtracting the **smallest earlier** prefix.

<!-- code: max_prefix_min -->
```cpp
#include <algorithm> // min and max
#include <stdexcept> // invalid_argument
#include <vector> // vector
using namespace std;

long long maxSubarrayPrefixMin(const vector<int>& values) {
    if (values.empty()) {
        throw invalid_argument("nonempty input required");
    }
    long long prefix = 0;
    long long minimumEarlierPrefix = 0;
    long long best = values[0];
    for (int value : values) {
        prefix += value;
        best = max(best, prefix - minimumEarlierPrefix);
        minimumEarlierPrefix = min(minimumEarlierPrefix, prefix);
    }
    return best;
}
```

**Order matters:** evaluate the nonempty candidate before adding the current prefix to the minimum. Otherwise you allow L = R, a zero-length interval, which can incorrectly produce zero on all-negative input.

For `[-2,1,-3,4]`, prefix values are -2,-1,-4,0. The minimum earlier prefix before the last step is -4, so the ending candidate is `0 - (-4) = 4`. This is another derivation of the same linear-time optimisation, not a need to store an entire prefix array.

## 9. Variants: identify what changes before reusing code

### Exactly k elements: fixed sliding window

Ordinary Kadane can choose any positive length. If length must be exactly k, maintain the sum of the last k values instead. After the first window, remove the departing value and add the arriving value.

<!-- code: max_fixed_window -->
```cpp
#include <algorithm> // max
#include <stdexcept> // invalid_argument
#include <vector> // vector
using namespace std;

long long maxFixedWindow(const vector<int>& values, int k) {
    int n = static_cast<int>(values.size());
    if (k <= 0 || k > n) {
        throw invalid_argument("require 1 <= k <= size");
    }
    long long sum = 0;
    for (int i = 0; i < k; ++i) {
        sum += values[i];
    }
    long long best = sum;
    for (int right = k; right < n; ++right) {
        sum -= values[right - k];
        sum += values[right];
        best = max(best, sum);
    }
    return best;
}
```

For `[2,-1,3,-2,4]`, k = 2 gives window sums `1,2,1,2`; answer 2. Unrestricted Kadane gives 6, which is not a valid fixed-length answer. Time `O(n)`, auxiliary space `O(1)`.

### Circular maximum subarray — optional stretch

If wrapping is permitted, an optimal interval is either ordinary/non-wrapping, or it contains a suffix plus a prefix. A proper wrapping interval is the complement of a contiguous **minimum-sum** middle interval. Candidate value is `totalSum - minimumSubarraySum`.

Compute ordinary maximum and minimum using analogous recurrences. If all values are negative, return the ordinary maximum; subtracting the entire minimum interval would otherwise choose an empty complement and return zero. With at least one nonnegative value, compare the ordinary maximum and the wrapping candidate. Time is O(n), auxiliary space O(1).

For `[5,-3,5]`, total 7 minus minimum -3 gives 10, using the last and first 5. For `[-4,-2,-7]`, return -2 rather than zero. This outline is an extension; implement it after the standard recurrence is comfortable.

### Product, target sums, and length restrictions

- **Maximum product:** negative multiplication can turn a minimum into a maximum. Track both largest and smallest ending products; sums' reset rule does not transfer directly. Product overflow also requires its own bounds.
- **Count subarrays with sum k:** maximise and count are different tasks. A prefix-frequency method is a later solution; Kadane does not count all matches.
- **At least k or a bounded length:** the “best previous ending” state alone is insufficient. The recurrence must retain valid boundary/length information.
- **Sliding windows with negative values:** the usual “sum too big, shrink” monotonic argument can fail because removing a negative value increases the sum. Fixed-size windows above remain valid because their length, not monotonicity of sum, determines movement.

### Divide and conquer — later after recursion

Split the array into halves. The answer lies entirely left, entirely right, or crosses the split. Combine a maximum suffix of the left half with a maximum prefix of the right. Scanning to combine gives `T(n) = 2T(n/2) + O(n)`, or O(n log n) time and O(log n) recursion depth. A richer summary with total, best prefix, best suffix, and best subarray can combine in O(1), connecting this problem to segment trees later. These are follow-ups, not prerequisites for this lesson.

<a id="revision"></a>
## 10. Interview explanation, pitfalls, and rapid revision

**A concise spoken answer:** “I keep the largest sum of a nonempty subarray ending at the current position. Such an interval either starts here or extends the best interval ending one step earlier, so I take the larger of those two sums. Separately I retain the best across all endpoints. Initialising from the first element handles all-negative arrays. This uses one pass and constant extra memory.”

| Common wrong approach | Counterexample / repair |
|---|---|
| Add every positive element | `[5,-100,6]` gives 11 but no contiguous slice sums to 11 |
| Restart at each negative element | `[4,-1,2]` should give 5 |
| Initialise answer to zero | `[-3,-1]` must give -1 for nonempty contract |
| Return current ending sum | Main example ends at 5 while global best is 6 |
| Reset before recording candidate | Loses negative-only candidates |
| Sort first | Changes adjacency and solves a different problem |
| Call the two-loop running sum O(n) | It still visits all n(n+1)/2 intervals |
| Use int because each element fits int | Many elements can produce a wider sum |

**Revision ladder:** enumerate boundaries → remove repeated summation → define best-ending state → choose restart or extend → keep global best → handle negatives → add indices/tie policy.

**Teach back:** prove the recurrence in your own words, trace a mixed and an all-negative input, then explain why exactly-k and maximum-product variants need different state.

## 11. Sources and what to study next

Local note: `03_arrays_and_vectors/03_kadanes_algorithm_max_subarray.md`. Code reviewed: `10-arrays-part-2/05_maxsum_subarrays_Kadane's_Algo.cpp`. The new examples use wide sums, distinguish running accumulation from a stored prefix array, reject empty input explicitly, and state index tie rules. Official practice and source video are linked above.

After closed-book mastery, continue with later playlist sessions such as majority element, formal complexity, stock/power problems, binary search, recursion and subsequent data structures in their actual course order. The existing [sorting package](../Sorting/README.md) is available when its prerequisites are comfortable; advanced sorts there also need recursion or heaps. These ten sessions are a foundation, not the entire interview syllabus.

<a id="questions"></a>
## 12. Revision questions — finish with a mock interview

1. Define subarray, subsequence, and subset. Why is picking all positive values invalid for this task? [Review](#definitions)
2. Derive n(n+1)/2 and list all subarrays of `[2,-1,3]`. Does 2ⁿ always count distinct subsequence values? [Review](#definitions)
3. Why does printing every subarray's elements cost more than listing boundary pairs? [Review](#definitions)
4. Explain the cubic baseline and how the running-sum version removes repeated work. [Review](#baselines)
5. Distinguish a running interval sum from a stored prefix-sum array. Give the inclusive interval query formula. [Review](#baselines)
6. Define ending and best precisely. Derive both recurrences without saying only “reset when negative”. [Review](#kadane)
7. Prove that a negative-sum prefix cannot improve a later suffix. Why can a negative individual element still belong in the optimum? [Review](#kadane)
8. **Dry run:** compute both state values for `[3,-4,5,-1,2,-6,4]`. [Review](#kadane)
9. **Implementation:** write Kadane for nonempty input using long long. State its contract, time, space, and correctness argument. [Review](#kadane)
10. Explain the order of updates in the reset formulation. Test `[-5,-2,-8]`, `[-7]`, and `[0]`. [Review](#kadane)
11. Why is returning ending instead of best wrong? Supply a small counterexample. [Review](#kadane)
12. **Implementation:** return indices and sum. State tie policy and explain the case previous ending = 0. [Review](#indices)
13. For `[0,1,-1,1]`, what interval does the provided earliest-boundary policy return? What if shortest were required? [Review](#indices)
14. Derive the prefix-minimum solution. Why update best before admitting the current prefix as a possible minimum? [Review](#extensions)
15. **Implementation:** maximum sum of exactly k values. Trace `[2,-1,3,-2,4]` with k = 2. [Review](#extensions)
16. **Stretch:** explain circular maximum subarray and the all-negative exception. [Review](#extensions)
17. Why do maximum product and counting sum-k subarrays require different state or algorithms? [Review](#extensions)
18. Give an input where the largest sum needs wider storage than one element. Explain why sorting first invalidates this problem. [Review](#revision)
19. How would you test the linear solution against a trustworthy baseline? Include negatives, ties, zeros, and boundary sizes. [Review](#baselines)
20. **Mock interview, 20–30 minutes:** clarify the problem; explain a baseline; derive Kadane; implement it; trace an all-negative case; add indices; discuss exactly-k and circular follow-ups. Assess explanation as well as output correctness. [Review](#revision)

<details>
<summary>Answer key and mock-interview rubric</summary>

1. Contiguous slice; ordered position selection with skips; unordered set selection. Positive values may be separated by required negative values.
2. For each start, n-L ends. Six intervals: [2], [2,-1], [2,-1,3], [-1], [-1,3], [3]. Repeated values can duplicate subsequence outputs.
3. Boundary pairs are quadratic; emitting each interval's length gives n(n+1)(n+2)/6 element outputs, cubic.
4. Recompute every candidate sum; then keep sum while extending the same start. Theta(n³) becomes Theta(n²).
5. Running sum uses scalar state per start. Stored prefixes cost O(n) space; query `[L,R]` is prefix[R+1]-prefix[L].
6. Best nonempty sum ending at i; best over all endpoints through i. `ending=max(x,oldEnding+x)`, `best=max(oldBest,ending)`.
7. P<0 implies P+S<S. `[4,-1,2]` retains its negative element and totals 5.
8. ending: 3,-1,5,4,6,0,4. best: 3,3,5,5,6,6,6. Winning sum 6 from [5,-1,2].
9. Section 5. One pass, O(1) working space; first-element initialisation and exhaustive start/extend induction.
10. Record current candidate before zeroing it; answers -2,-7,0.
11. `[5,-10]` ends with -5 in recurrence form but best is 5.
12. Section 7. Restart only when previous ending<0, extend on zero to preserve earliest start; strict global improvement preserves earliest winning end.
13. 0..1 with sum 1. Shortest could select 1..1 (earliest among shortest); change tie handling explicitly.
14. Maximise prefix[R]-earlierPrefix[L] using smallest earlier prefix; using the current prefix first admits an empty interval.
15. Section 9; sums 1,2,1,2, answer 2.
16. Compare ordinary maximum with total-minus-minimum-middle; all-negative input must use ordinary maximum to exclude an empty complement.
17. Products swap maximum/minimum roles under negative factors; counting needs all matching boundary frequencies, not just the single best state.
18. Three 10^9 values sum to 3*10^9. Sorting changes which values are adjacent.
19. Enumerate every interval on many small arrays as the oracle; compare all sum methods, verify returned boundaries and their sum, check tie policy separately, reject empty input, and test wide accumulators.
20. Ready if you state the contract, derive the two states, justify the recurrence, write correct boundaries, handle negatives/zero, explain complexity, and distinguish follow-up requirements. If you can only reproduce memorised code, revisit the derivation before marking the topic mastered.

</details>
