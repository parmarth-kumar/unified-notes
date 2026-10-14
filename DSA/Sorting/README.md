# Sorting — Learn, Practise, and Revise

Sorting means arranging items in a chosen order while keeping every original item. This package combines the sorting lessons from your two source folders with additional explanations and interview practice.

**Start here:** [Theory](./theory.md) explains each idea in simple English, with examples, code, and reasons. Do not try to finish every algorithm in one sitting.

## What to learn first

You need arrays, indices, loops, functions, and basic `vector` use for the first four sorts. Learn recursion before Merge and Quick Sort. Learn the array representation of a heap before Heap Sort. The theory begins with a short English glossary and complexity guide.

| Stage | What to study | What you should be able to do |
|---|---|---|
| 1: Foundation | Sorted order, stability, space, indices; Bubble, Selection, Insertion | Trace a pass, explain the loop bounds, and write each sort |
| 2: Count and partition | Counting, descending order, DNF / Sort Colors | Explain negative offsets, range costs, and why DNF rechecks `mid` |
| 3: Course array problems | Merge Sorted Arrays, Next Permutation | Fill padding safely; explain pivot, replacement, and suffix reversal |
| 4: Faster sorts | Merge, Quick / Lomuto / Hoare / three-way, Heap | Explain correctness, worst time, and stack memory |
| 5: Further depth | Radix, Bucket, Shell, STL tools, lower bounds | State assumptions and choose the right tool |
| 6: Interview extensions | Inversions, kth largest, intervals, k-displaced arrays, external sorting | Explain an approach, implement it, and test boundary cases |

The extra sorts are included for completeness. They do not all need equal study time now.

## Open the right file

| File | Use it for |
|---|---|
| [theory.md](./theory.md) | Full lessons, glossary, dry runs, C++ implementations, correctness and complexity |
| [sorting-visualizer.html](./sorting-visualizer.html) | All ten sorts plus DNF, three Quick variants, both Counting modes, and live working-state panels |
| [practice.md](./practice.md) | Trace, debug, and code; hints and separate answers |
| [doubts.md](./doubts.md) | Resolve common confusions with examples |
| [interview-prep.md](./interview-prep.md) | Spoken answers, coding approaches, follow-ups, and mock questions |
| [revision.md](./revision.md) | Recall rules and tables after you have studied the lessons |
| [audit-report.md](./audit-report.md) | Source inventory, corrections, actual validation, and limits of this review |
| [verify_notes.py](./verify_notes.py) | Compile and test the implementations extracted from the current Markdown |
| [verify_all_sorts.cpp](./verify_all_sorts.cpp) | Existing standalone C++ verification suite |
| [verify_visualizer.cjs](./verify_visualizer.cjs) | Check every visualizer algorithm, variant, snapshot, counter, and input rule |

The visualizer supports Bubble, Selection, Insertion, Counting, Merge, Quick, Heap, Radix, Bucket, Shell, and Sort Colors / DNF. It shows merge/output buffers, a heap tree, radix digit counts, buckets, gap groups, and partition regions. Quick Sort has Lomuto, Hoare, and three-way choices. Counting has frequency/item-list reconstruction and stable prefix sums.

Use **Load example** when a sort needs a different type of input. Bucket accepts decimals in `[0, 1)`; Radix needs non-negative integers; DNF needs only `0`, `1`, and `2`. Other sorts use whole integers. Custom input supports up to 15 items and `[]` for empty input. Counting displays value ranges of at most 40.

Play, pause, next, previous, solve, reset, and speed controls work with every choice. **Instant bar movement** turns movement transitions off while keeping the state and explanations. System reduced-motion preferences are respected. Original IDs help you inspect stability. Complexity labels exclude saved animation snapshots and rendering memory.

## Topic checklist — mark only after you can explain it

- [ ] Check ascending and descending order; keep duplicates; handle empty arrays.
- [ ] Explain stability using records with equal keys and different IDs.
- [ ] Separate input, output, buffer, and recursive stack memory.
- [ ] Bubble: adjacent swaps, final suffix, correct inner limit, early exit.
- [ ] Selection: minimum scan, final prefix, swap count, instability counterexample.
- [ ] Insertion: save key, shift, correct insertion index, sorted prefix, inversion count.
- [ ] Counting: both value reconstruction and stable placement, negatives, cumulative indices, large-range rejection.
- [ ] Implement descending sorts and descending character Insertion Sort.
- [ ] DNF: four regions, three actions, stopping rule, stability limitation.
- [ ] Merge Sorted Arrays: valid lengths, actual padding, backward fill, tie handling.
- [ ] Next Permutation: lexicographic order, non-increasing suffix, duplicate values, wraparound.
- [ ] Merge Sort: recursion, linear merge, stability, buffer reuse, optional skip check.
- [ ] Quick Sort: Lomuto vs Hoare boundaries, pivot choices, bad splits, bounded stack, three-way duplicates.
- [ ] Heap Sort: parent/child indices, bottom-up build, iterative sift-down, sorted suffix.
- [ ] Radix: LSD vs MSD, stable digit passes, non-negative precondition, digit/base costs.
- [ ] Bucket: finite range, distribution assumption, bucket-sort subroutine and stability.
- [ ] Shell: gap groups, final gap 1, Knuth gaps, sequence-dependent time.
- [ ] STL: raw arrays/vectors, exclusive end, comparators, `sort`, `stable_sort`, `partial_sort`, `nth_element`, `is_sorted`.
- [ ] Explain the worst-case comparison lower bound and its assumptions.
- [ ] Solve inversions, kth largest, merge intervals, and k-displaced arrays.
- [ ] Explain recursive vs iterative list Merge Sort and external multi-round merging.

## Study routine

For each algorithm:

1. Read the plain explanation and work one example by hand.
2. Draw its sorted/unknown regions. Explain what stays true after a pass.
3. Write the code without looking. Check every index and loop endpoint.
4. Try empty, one-item, sorted, reverse, duplicate, and negative inputs where allowed.
5. Attempt the matching practice questions before opening answers.
6. Say the time, extra memory, stability, and assumptions in your own words.

Return to the [revision sheet](./revision.md) for recall, then do the mock questions in [interview preparation](./interview-prep.md). Keep new questions in [doubts](./doubts.md). Related topics to study next are binary search, heaps, recursion, linked lists, and Fenwick trees; those topic folders do not yet exist in this workspace.

## Source folders inspected

- `C:\Users\kunal\Desktop\C++ & DSA`: all eight `.cpp` files under `11-sorting`, plus `20-divide-and-conquer/01-merge-sort.cpp`.
- `C:\Users\kunal\Desktop\notes\C++ Notes`: all six Markdown files under `06_sorting_algorithms`, Merge / Quick / Inversion lessons in `12_recursion_and_backtracking`, and the sorting/comparator sections of the complete STL lesson.

See the [file-by-file source map](./audit-report.md#source-inventory). The source files contain useful teaching examples and some mistakes; corrected explanations belong here. Neither reference folder was modified.

## Run the checks

From the repository root in PowerShell, with Python, Node.js, and `g++` installed:

```powershell
python DSA/Sorting/verify_notes.py
g++ -std=c++17 -O2 -Wall -Wextra DSA/Sorting/verify_all_sorts.cpp -o DSA/Sorting/verify_all_sorts.exe
if ($LASTEXITCODE -eq 0) { & .\DSA\Sorting\verify_all_sorts.exe }
```

The Python command also runs the JavaScript visualizer checks. These are correctness checks, not performance benchmarks. The [audit report](./audit-report.md#validation) records only checks actually run for this review.
