# Unified-Notes

> Central workspace for learning computer science fundamentals from first principles, deep conceptual clarity, technical interview preparation (Google SDE level), and interactive visual aids.

---

## Repository Structure

```
unified-notes/
├── instruction.md          # Master learning system guidelines & architecture rules
└── DSA/
    └── Sorting/            # Complete Sorting Algorithms Learning Package
        ├── README.md       # Topic syllabus, navigation & attribution
        ├── theory.md       # First-principles conceptual theory & C++ implementations
        ├── practice.md     # Tracing challenges, edge-case debugging & drills
        ├── interview-prep.md # Google SDE interview Q&A bank & coding problems
        ├── revision.md     # High-density recall sheet & complexity matrix
        ├── doubts.md       # Living doubt log & conceptual misconceptions
        ├── sorting-visualizer.html # Interactive visualizer & telemetry HUD
        ├── audit-report.md        # Source coverage, corrections, and actual validation
        ├── verify_notes.py        # Compile/test code extracted from the current notes
        ├── verify_all_sorts.cpp    # Standalone C++ correctness checks
        └── verify_visualizer.cjs  # All visualizer algorithms and variants
```

---

## Topics

### Data Structures & Algorithms (DSA)
- [**Sorting Algorithms**](./DSA/Sorting/README.md) — Simple-English lessons covering ten sorts, C++ tools, DNF, Merge Sorted Arrays, Next Permutation, and interview practice. Includes an interactive [Sorting Visualizer](./DSA/Sorting/sorting-visualizer.html) and a [source review](./DSA/Sorting/audit-report.md).

---

## Learning Architecture

Each topic in `unified-notes` follows a structured 6-file learning framework:

1. **`README.md`** — Topic navigation, learning objectives, prerequisites, syllabus checklist, and study sequence.
2. **`theory.md`** — Deep first-principles explanations, mathematical proofs, invariants, and modern C++ implementations.
3. **`interview-prep.md`** — Google SDE interview questions, architectural trade-offs, and tricky follow-ups.
4. **`practice.md`** — Active problem solving, tracing exercises, and spot-the-bug diagnostic challenges.
5. **`revision.md`** — 15-minute high-density summary tables, complexity matrices, and rapid self-tests.
6. **`doubts.md`** — Living log tracking resolved student confusions, edge cases, and counterexamples.
