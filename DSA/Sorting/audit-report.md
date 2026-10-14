# Sorting Content Review — 14 October 2026

**Destination:** `Unified-Notes-collab/DSA/Sorting`.

The existing package covered ten sorting algorithms but had missing course problems, unclear explanations, conflicting complexity claims, and an inaccurate source inventory. This review checks the sorting material in the two folders supplied by the user and improves the six learning files, code verification, and existing visualizer.

## Coverage checklist used before editing

- Definitions, order with duplicates, item preservation, glossary, loop boundaries, correctness, and complexity assumptions.
- Bubble, Selection, Insertion, both Counting methods, ascending/descending exercises, and sorted-array checking.
- DNF / Sort Colors, merging into padding, and Next Permutation from the source course.
- Merge, Lomuto / Hoare / three-way Quick Sort, Heap, Radix, Bucket, and Shell.
- C++ sorting calls, comparators, selection tools, memory and stability guarantees.
- Interview coding patterns, dry runs, edge cases, progressive hints, and separate answer keys.
- Consistency among theory, revision, doubts, interview preparation, practice, and the visualizer.

<a id="source-inventory"></a>
## Source inventory — files actually read

Source paths below are relative to the stated source folder. They are attribution, not workspace navigation links. Both source folders were treated as read-only.

### Folder 1: `C:\Users\kunal\Desktop\C++ & DSA`

| File | Useful content | Correction / destination |
|---|---|---|
| `11-sorting/01_arr_is_sorted.cpp` | Linear neighbour check | Added pure boolean check in theory 0.7; source computes `n1` using the other array's `sizeof` |
| `11-sorting/bubble-sort.cpp` | Adjacent-swap loops | Source sorts descending and has no early exit; theory distinguishes those choices |
| `11-sorting/selection-sort.cpp` | Minimum index and final swap | Retained logic; explained sorted prefix, instability, and swap versus write counts |
| `11-sorting/insertion-sort.cpp` | For/while swap methods and shift method | Explained saved key, shift state, stability, and $O(n+I)$ time |
| `11-sorting/counting-sort.cpp` | Frequency counting and value reconstruction | Fixed negatives with offset, range allocation/overflow assumptions, empty input, and reconstruction limits |
| `11-sorting/builtin-sort.cpp` | Raw-array ascending/descending STL calls | Added excluded endpoint and explicit `<functional>` header; explained `sizeof` only for actual arrays |
| `11-sorting/practice-questions/q1.cpp` | Descending character exercise | Source comparison produces ascending output; added correct descending Insertion Sort and practice answer |
| `11-sorting/practice-questions/q2.cpp` | Four descending integer sorts | Added comparison-direction table and exact expected output; source count array has hardcoded bounds |
| `20-divide-and-conquer/01-merge-sort.cpp` | Recursive split and stable merge | Retained single-buffer version in Unified Notes; clarified inclusive boundaries and stack space |

### Folder 2: `C:\Users\kunal\Desktop\notes\C++ Notes`

| File | Useful content | Correction / destination |
|---|---|---|
| `06_sorting_algorithms/00_master_index.md` | Sorting course scope | Reconciled the checklist; identified omitted lecture 26 problems |
| `06_sorting_algorithms/01_bubble_selection_insertion_sort.md` | Basic sorts, traces, invariants | Corrected claims equating swaps with writes and fixed library thresholds as universal rules |
| `06_sorting_algorithms/02_dnf_sort_colors.md` | Four-region invariant, trace, movement rules | Added theory lesson; clarified the `low == mid` self-swap case and meaning of one pass |
| `06_sorting_algorithms/03_merge_sorted_arrays_and_next_permutation.md` | Backward merge and lexicographic permutation | Added full missing lessons, checked functions, traces, exercises, interview summaries; suffix may contain equal values |
| `06_sorting_algorithms/topic_interview_questions.md` | Stability, DNF, array/list choices | Corrected total Quick Sort and recursive list Merge Sort memory claims |
| `06_sorting_algorithms/topic_revision.md` | Comparison matrix and recall patterns | Reconciled variant-specific stack, time, domain, and stability caveats |
| `12_recursion_and_backtracking/10_merge_sort_algorithm.md` | Merge proof, recurrence, boundary skip optimisation | Added correctness argument and optional linear-best-case variant; avoided absolute cache/list claims |
| `12_recursion_and_backtracking/11_quick_sort_algorithm.md` | Lomuto, pivot strategies, QuickSelect | Added Hoare implementation and boundaries; random pivots do not remove worst cases or all-equal Lomuto degeneration |
| `12_recursion_and_backtracking/12_count_inversions_problem.md` | Merge counting and integer limits | Added explicit pair trace and 64-bit total; completed coordinate-compression alternative in practice |
| `07_cpp_stl/01_cpp_stl_complete_containers_and_algorithms.md` — sorting/comparator sections | Iterator range, raw STL usage, multi-key lambda | Distinguished contracts from implementation choices; expanded comparator rules |

The review inspected 19 relevant local source files or explicitly identified sections. Searches also located related mentions elsewhere; this report does not claim every file in either large course folder was reviewed. The supplied source folders do not contain the old numbered filenames or source HTML previously attributed in this package; that attribution was removed. The master `instruction.md` has historical repository paths, while this review uses the user's supplied paths.

## Changes made

| File | Main improvements |
|---|---|
| `README.md` | Clear staged study path, accurate syllabus/source inventory, learning checklist, verification commands |
| `theory.md` | Glossary, boundaries, sorted check, loop correctness, value-only Counting, Hoare, iterative Heap, missing course lessons, precise library and complexity claims |
| `practice.md` | Beginner traces, descending drills, merge/permutation tasks, complete Hoare answer, accurate comparator/heap bug answers, bounded k-window code, actual Fenwick solution |
| `interview-prep.md` | Shorter spoken answers, coding assumptions, approach comparisons, dry runs, follow-ups, missing course problems, guarded three-way iterative QuickSelect |
| `revision.md` | Simple recall rules, version-specific complexity matrix, course problem patterns, separate self-test answers |
| `doubts.md` | Retained eight resolved topics, corrected explanations, six new doubts with examples and linked lessons |
| `sorting-visualizer.html` | Correct swap/memory labels, strict input validation, empty-array entry, cursor-based Counting item lists, accurate range-comparison count, previous-to-start, custom-array preservation, and speed-label fixes |
| `verify_notes.py` | Extracts and compiles current Markdown code; checks links and correctness against standard-library and brute-force answers |
| `verify_all_sorts.cpp` | Keeps the standalone suite consistent with corrected Heap, Radix, Bucket, and QuickSelect implementations; adds Hoare verification |

### Important technical corrections

- Stable Counting cumulative counts are lengths, not zero-based indices.
- Selection uses at most `n-1` **swaps**, not `n-1` array writes. It is not a universal minimum-write algorithm.
- Shifting is not guaranteed to be three times faster than swapping.
- Recursive heapify does not use constant total space. The teaching implementation is now iterative.
- Early-stopping Heap Sort can be linear on all-equal input; its unconditional tight $\Theta(n\log n)$ best-case claim was replaced with the correct variant-specific bound.
- Bucket Sort's worst time and stability depend on the inner sort. The teaching code now uses stable Insertion Sort consistently with its quadratic worst bound and rejects non-finite values.
- Smaller-side Quick Sort recursion controls stack memory, not worst time. Three-way recursive Quick Sort still needs its own stack analysis.
- Library guarantees do not mandate Introsort, a threshold of 16, or an exact portable extra-space bound.
- Merge Sort on lists needs a recursive-stack caveat; pointer rewiring alone does not describe total memory.
- `>=` guarded by unequal scores is valid in the comparator exercise. The `<=` tie branch causes the actual self-comparison violation.
- The practice coordinate-compression challenge now has compression/Fenwick hints and an actual solution; the merge alternative remains available.
- Unsupported historical compiler/benchmark/log claims were removed from the README. Tests are correctness checks, not performance measurements.

<a id="validation"></a>
## Validation

Checks actually run on 14 October 2026:

| Check | Actual result |
|---|---|
| Compiler | WinLibs MinGW-W64 `g++` 16.1.0, C++17 |
| Current Markdown code | 29 selected complete code blocks compiled with their documented dependencies, `-Wall -Wextra -pedantic` |
| Combined current-note C++ tests | Passed with `-O2` and `_GLIBCXX_ASSERTIONS` enabled |
| Comparison sorts | 3,090 checks against `std::sort`, including random inputs, empty/single items, duplicates, negatives, and extreme integer values |
| Counting, kth ranks, inversions | Passed bounded-range sorting, every valid kth rank on test arrays, merge/Fenwick counts against brute force, and invalid-range/rank rejection |
| DNF and Next Permutation | 3,280 exhaustive inputs over values 0/1/2 with lengths 0 through 7; permutation results and wrap booleans matched `std::next_permutation` |
| Padded merge, Bucket, Radix | 200 random cases each; empty-side merges, finite/range restrictions, and `INT_MAX` Radix values passed |
| Large inversion count | Both methods returned `4,999,950,000` for 100,000 descending distinct integers |
| Duplicate-aware selection | 10,000 equal keys passed the three-way iterative selector |
| Stability rules | Tagged adaptations of Bubble, Insertion, Merge, and Counting kept tie order; integer-only outputs cannot establish record identity |
| Other C++ examples | Character order, k-displaced arrays including `k=INT_MAX`, candidate tie-breaking, interval edge cases, and STL demos passed |
| Existing standalone suite | All 19 printed test groups passed after implementation synchronisation; its tagged library examples are separate from the current-note adaptations |
| JavaScript | Full script parsed; 512 trace-generator cases passed for output, IDs, stable tie order where applicable, counters, empty/negative inputs, and safe integer values |
| Visualizer input/range checks | Malformed tokens, decimals, exponent notation, missing tokens, too many elements, unsafe integers, and oversized Counting ranges were rejected correctly |
| Local navigation | 109 local file links/anchors and Markdown code fences passed, including the repository README |
| Browser interaction | Verified all four algorithm selections, solve, play/automatic completion, pause, next, previous-to-start, reset, negative Counting, empty input, invalid-input rejection, range warning, custom-input preservation across algorithm switches, and keyboard speed changes |
| Browser presentation | Inspected the rendered workbench in the default app viewport; labels, bars, counters, and controls were readable |

The browser tests used the actual local page. Generator tests separately executed the extracted JavaScript functions without a browser. The initial review results above are retained as a dated record; the follow-up checks below cover the expanded visualizer and code style. Neither is a performance benchmark, memory measurement, or proof that every possible input works. Arbitrary mobile viewport sizes and assistive-technology workflows were not exhaustively tested. Complexity and correctness explanations were also reviewed against the code and primary references.

## External references checked

- [C++ sorting specification](https://eel.is/c++draft/alg.sort) and [comparison requirements](https://eel.is/c++draft/alg.sorting).
- [C++ nth element](https://eel.is/c++draft/alg.nth.element), [merge](https://eel.is/c++draft/alg.merge), and [permutation contracts](https://eel.is/c++draft/alg.permutation.generators).
- [MIT comparison lower bound and integer sorting](https://courses.csail.mit.edu/6.006/fall09/lecture_notes/lecture10.pdf).
- [Princeton Knuth-gap Shell Sort](https://algs4.cs.princeton.edu/code/javadoc/edu/princeton/cs/algs4/Shell.html) and [algorithm comparison summary](https://algs4.cs.princeton.edu/cheatsheet/).

The draft standard is used to verify the relevant ordinary algorithm contracts. Examples target C++17; newer facilities or parallel-overload behaviour are outside these examples.

## Limits and next study steps

This is a complete review of the identified sorting package against the supplied sorting sources, not a review of every DSA topic. Advanced external sorting and linked-list sorting are conceptual extensions here, not full production implementations. Specialised in-place stable merge algorithms, every Shell gap family, and median-of-medians code remain outside this course-focused package. The follow-up visualizer now covers all ten sorting algorithms plus DNF, including the stable Counting prefix-sum variant. Next Permutation and the padded two-array merge remain explained in Markdown; they are related course problems, not additional general sorting algorithms.

Recommended order: foundations → three basic sorts → Counting / DNF → padded merge / Next Permutation → Merge / Quick / Heap → other sorts and STL → practice → doubts → interview drills → revision.

To check your understanding, trace an unfamiliar five-item input, explain the invariant, write code without looking, state assumptions and memory costs, and test empty/duplicate/boundary inputs. No study-progress checkbox was marked complete by this review.


## Follow-up: Complete visualizer and course-style C++

The user's follow-up asked for the missing sorts in the visualizer and C++ code formatted like `C++ & DSA`.

- Added Merge, Quick, Heap, Radix, Bucket, Shell, and DNF to the four original choices: 11 algorithms, with 14 selectable algorithm/variant combinations.
- Added Lomuto, Hoare, and three-way Quick partitions, plus frequency/item-list and stable prefix-sum Counting modes.
- Added working-state panels for sorted halves, output buffers, heap levels, digit counts, bucket contents, and Shell index groups. Explanations and links point to the canonical theory.
- Added algorithm-aware examples and validation. Bucket accepts finite decimals in `[0,1)`; Radix accepts non-negative integers; DNF accepts only 0/1/2. Empty, duplicate, sorted, reverse, and negative presets remain available where the chosen algorithm permits them.
- Preserved play/pause, forward/backward stepping, solve, reset, speed controls, and input identity. Invalid domains show an explanation and disable sorting controls until corrected.
- Changed bar movement to `transform`, provided instant movement and system reduced-motion handling, and kept keyboard stepping instant.
- Corrected a pre-existing Insertion trace bug: shifting temporarily duplicated an item reference and hid the saved key. Every frame now keeps the full original identity list. Counting reconstruction uses an output buffer so unlifted bars do not overlap.
- Reformatted all 38 fenced C++ blocks and the standalone `.cpp` file with four-space indentation, attached braces, required headers with brief descriptions, and `using namespace std;`. Unused headers were removed. The correctness guards and existing algorithm logic were retained.
- Recorded the teaching-code style in `instruction.md` for future examples. Neither source folder was modified.

**Follow-up validation:** All 29 selected complete C++ blocks compiled; the existing C++ correctness checks and all 19 standalone test groups passed. `verify_visualizer.cjs` passed 2,823 valid cases, 138 rejected-domain cases, and 211,618 snapshots checked for identities, values, non-overlapping unlifted slots, and monotone counters. It also checked 1,093 exhaustive DNF inputs, stable tie order, exact example mechanics, strict parsers, all 11 generated lesson anchors, script syntax, and reduced-motion/transform rules.

The browser checks confirmed real outputs for every newly added algorithm, all three Quick variants, stable Counting with negative duplicate keys, merge-buffer rendering, heap levels, and the instant-movement switch (computed bar transition: `0s`). Keyboard stepping displayed Insertion Sort's saved key correctly. Invalid Bucket domains and malformed input showed explanations and disabled sorting controls. The final rendered preview was inspected in the default app viewport.

The final navigation check passed 107 static local links/anchors and Markdown code fences; the JavaScript suite separately checked all 11 generated lesson anchors. All 38 fenced C++ blocks contain no repeated `std::` prefixes. `git diff --check` passed. The browser used the localhost preview; the in-app browser does not permit `file://` navigation. The system reduced-motion preference was not forcibly emulated, and arbitrary mobile sizes and assistive-technology workflows were not exhaustively tested.
