# Session 04 — Patterns and Nested Loops

**Lesson:** [Patterns](https://www.youtube.com/watch?v=rga_q2N7vU8) · **Prerequisites:** loops, counters, character output.

[Previous](./03-Conditional-Statements-and-Loops.md) · [Course map](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md#course-map) · [Next](./05-Functions-and-Problem-Decomposition.md)

**Core:** squares, triangles, number/character patterns, reverse triangles, Floyd's triangle, pyramids, hollow diamond, butterfly. **Extensions:** coordinate predicates, exact output checks, complexity from output size.

**Navigate:** [Method](#method) · [Pattern family](#family) · [Full implementations](#implementations) · [Revision](#revision) · [Questions](#questions).

<a id="method"></a>
## 1. Derive a row; do not memorise a picture

Patterns train you to translate a visual specification into index relationships. The important transferable skills are loop bounds, resetting versus preserving state, symmetry, and precise output. Treat them as foundation exercises, then move to array problems after you can derive unfamiliar variations.

Use **one-based rows** `r = 1..n` and one-based positions `c` within a row throughout this file. Later arrays use zero-based indices; do not mix the conventions accidentally.

For every pattern:

1. Count the total rows.
2. For row r, derive the number of leading spaces and visible symbols.
3. Derive each symbol's value: constant, row-based, column-based, or global counter.
4. Print the newline after the entire row.

For geometry examples, one star occupies **one character column** and every space is one blank. Number patterns use spaces between values for readability. If a number becomes two digits, it occupies two columns; alignment then needs explicit formatting. Mixing `"* "` with single-space alignment without accounting for its width distorts a picture.

## 2. The reset-location rule

If a counter is initialised **inside** the outer loop, it restarts each row. If initialised **before** the outer loop, it persists across rows.

| Desired output | Expression/state |
|---|---|
| Every cell is `*` | Print a constant |
| Row r repeats r | Print r inside row |
| Each row restarts `1 2 3 ...` | Print c |
| Values continue across rows | Counter outside outer loop |
| Each row restarts `A B C ...` | Use alphabet position c - 1 |
| Values reverse within a row | Start at r and decrease |

Using `alphabet[c - 1]` from an explicit alphabet string makes the character mapping clear without depending on alphabetic character codes being contiguous. Limit such examples to 26 letters unless the specification defines wrapping.

<a id="family"></a>
## 3. Core pattern catalogue

The following outputs use n = 4. First reproduce the row formulas, then the output, and only then code it.

### Square patterns

```text
Column number       Row number          Continuous number
1 2 3 4             1 1 1 1             1 2 3 4
1 2 3 4             2 2 2 2             5 6 7 8
1 2 3 4             3 3 3 3             9 10 11 12
1 2 3 4             4 4 4 4             13 14 15 16
```

All have n rows and n values per row. The inner expression determines the difference: c, r, or `counter++`. Star squares and alphabet squares use the same boundaries.

### Increasing triangles

```text
Stars       Repeated row    Increasing value    Decreasing value
*           1               1                   1
**          2 2             1 2                 2 1
***         3 3 3           1 2 3               3 2 1
****        4 4 4 4         1 2 3 4             4 3 2 1
```

Each row has r symbols. Print `'*'`, r, c, or `r - c + 1` respectively. The last formula starts at r for c = 1 and reaches 1 at c = r.

### Floyd's triangle and its alphabet variant

```text
1                  A
2 3                B C
4 5 6              D E F
7 8 9 10           G H I J
```

Keep the counter outside the row loop. A numeric Floyd's triangle has `n(n+1)/2` values. For a non-wrapping alphabet version, that count must be at most 26.

### Reverse character triangle

```text
A
B A
C B A
D C B A
```

Row r starts at the r-th letter and decreases toward A. For position c, use alphabet position `r - c`. Do not confuse this with one continuing alphabet counter; they have different state.

### Inverted and right-aligned triangles

```text
Left inverted       Right aligned       Shifted inverted row values
****                   *                1 1 1 1
***                   **                  2 2 2
**                   ***                    3 3
*                   ****                      4
```

- Left inverted: `n - r + 1` stars, no leading spaces.
- Right aligned: `n - r` spaces, r stars.
- Shifted inverted values: `r - 1` blank **value cells**, then `n - r + 1` copies of r. If each value uses a two-character cell, the blank cell must also be two characters.

### Solid and numeric pyramids

```text
Solid              Numeric
   *                  1
  ***                121
 *****              12321
*******            1234321
```

Both have `n - r` leading spaces and width `2r - 1`. A numeric pyramid prints ascending `1..r`, then descending `r-1..1`. Start the second segment at `r-1` so the centre is not repeated. Use n <= 9 for this single-digit layout.

### Hollow rectangle and grid predicates — extension

For rows r = 1..rows and columns c = 1..cols, print a star when `r == 1 || r == rows || c == 1 || c == cols`, otherwise a space. This is a **coordinate predicate**: a condition deciding what belongs at a cell.

Other one-based square-grid predicates:

- Main diagonal: `r == c`.
- Secondary diagonal: `r + c == n + 1`.
- Lower triangle: `c <= r`.
- Upper triangle: `c >= r`.

The formulas change if you switch to zero-based indices; for example, the secondary diagonal becomes `r + c == n - 1`.

<a id="implementations"></a>
## 4. Complete implementation: the number-pattern family

The functions below print rows for 1 <= n <= 9. Keeping output routines in functions is preview syntax for Session 05. `void` means no value is returned.

<!-- code: basic_patterns -->
```cpp
#include <iostream> // cout
#include <string> // alphabet string
using namespace std;

void numberSquare(int n) {
    for (int r = 1; r <= n; ++r) {
        for (int c = 1; c <= n; ++c) {
            cout << c << (c == n ? '\n' : ' ');
        }
    }
}

void floydTriangle(int n) {
    int value = 1;
    for (int r = 1; r <= n; ++r) {
        for (int c = 1; c <= r; ++c) {
            cout << value++ << (c == r ? '\n' : ' ');
        }
    }
}

void reverseCharacterTriangle(int n) {
    const string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int r = 1; r <= n; ++r) {
        for (int c = 1; c <= r; ++c) {
            cout << alphabet[r - c] << (c == r ? '\n' : ' ');
        }
    }
}

void numericPyramid(int n) {
    for (int r = 1; r <= n; ++r) {
        for (int space = 0; space < n - r; ++space) {
            cout << ' ';
        }
        for (int value = 1; value <= r; ++value) {
            cout << value;
        }
        for (int value = r - 1; value >= 1; --value) {
            cout << value;
        }
        cout << '\n';
    }
}
```

**Floyd dry run:** before row 1 the counter is 1. Row 1 consumes one number and leaves 2. Row 2 consumes 2 and 3 and leaves 4. Row 3 consumes 4, 5, 6 and leaves 7. Reinitialising the counter to 1 inside the outer loop would produce a different triangle.

## 5. Hollow diamond: derive both halves

Define n as the number of rows from the top to the widest row, inclusive. This version has `2n - 1` total rows, so the widest row occurs once.

For n = 4:

```text
   *
  * *
 *   *
*     *
 *   *
  * *
   *
```

Let **level** increase from 1 to n, then decrease from n-1 to 1. At a given level:

- Leading spaces: `n - level`.
- If level is 1: print one star.
- Otherwise: print one star, `2 * level - 3` inside spaces, then one star.

Why `2 * level - 3`? The corresponding solid row has width `2 * level - 1`. Two boundary stars occupy two columns; the remaining width is `2 * level - 3`. This formula is negative at level 1, so treat the tip separately.

<!-- code: hollow_diamond -->
```cpp
#include <iostream> // cout
using namespace std;

// Contract: 1 <= n <= 100.
void printDiamondRow(int n, int level) {
    for (int space = 0; space < n - level; ++space) {
        cout << ' ';
    }
    cout << '*';
    if (level > 1) {
        for (int space = 0; space < 2 * level - 3; ++space) {
            cout << ' ';
        }
        cout << '*';
    }
    cout << '\n';
}

void hollowDiamond(int n) {
    for (int level = 1; level <= n; ++level) {
        printDiamondRow(n, level);
    }
    for (int level = n - 1; level >= 1; --level) {
        printDiamondRow(n, level);
    }
}
```

### Dry run for n = 3

| Level sequence | Leading spaces | Inside spaces | Printed row, with dots showing spaces |
|---:|---:|---:|---|
| 1 | 2 | Tip | `..*` |
| 2 | 1 | 1 | `.*.*` |
| 3 | 0 | 3 | `*...*` |
| 2 | 1 | 1 | `.*.*` |
| 1 | 2 | Tip | `..*` |

Dots are for the trace only; the function prints blanks. For n = 1, the first loop prints one star and the second loop runs zero times.

## 6. Butterfly: choose the centre convention

This version also uses `2n - 1` rows and includes the widest row once. Some exercises use 2n rows with a duplicated centre; both are valid specifications. Match the requested output.

For n = 4:

```text
*      *
**    **
***  ***
********
***  ***
**    **
*      *
```

At level k, print k stars, `2(n-k)` spaces, and k stars. Total width is always `k + 2(n-k) + k = 2n`. At the centre the two star segments touch, forming 2n stars.

<!-- code: butterfly -->
```cpp
#include <iostream> // cout
using namespace std;

// Contract: 1 <= n <= 100.
void printButterflyRow(int n, int level) {
    for (int star = 0; star < level; ++star) {
        cout << '*';
    }
    for (int space = 0; space < 2 * (n - level); ++space) {
        cout << ' ';
    }
    for (int star = 0; star < level; ++star) {
        cout << '*';
    }
    cout << '\n';
}

void butterfly(int n) {
    for (int level = 1; level <= n; ++level) {
        printButterflyRow(n, level);
    }
    for (int level = n - 1; level >= 1; --level) {
        printButterflyRow(n, level);
    }
}
```

For n = 1, the result is `**` on one row: two wings of one star each. If your desired tip is one star, that is a different geometry and needs a different formula.

## 7. Complexity and boundary checks

For n-by-n squares, there are n² cells. Increasing triangles contain `1 + 2 + ... + n = n(n+1)/2` values. Both take `Theta(n²)` output operations. Solid pyramids contain `1 + 3 + ... + (2n-1) = n²` stars, plus spaces. Butterfly and hollow-diamond layouts also print a quadratic number of character positions once blanks are counted.

“Hollow” does not make the **text output** linear: the blanks are output too. These functions keep only scalar counters and a fixed alphabet string, giving `O(1)` auxiliary space. Building all rows in a growing string would instead use output storage proportional to the generated text.

For unrestricted numeric patterns, the number of characters per value grows with its digit count. The usual `O(n²)` statement counts value-output operations or assumes bounded-width values; literal character output can cost more. This file's small-n contracts avoid that ambiguity.

Test n = 1, 2, and 4. Verify row count, leading spaces, centre duplication, absence of unintended blank rows, and newline placement. Derive behavior for n = 0 if the problem allows it; these function contracts start at 1.

<a id="revision"></a>
## 8. Common doubts and rapid revision

| Confusion | Resolution |
|---|---|
| “My triangle repeats 1 on every row.” | You printed a constant or reset a counter; choose c, r, or a persistent counter deliberately. |
| “The bottom half is one row too tall.” | You probably repeated the widest level; start at n-1 for a single centre. |
| “The diamond tip has two stars.” | Level 1 needs one boundary star, not two coincident boundaries printed twice. |
| “Nested loops always need quadratic extra memory.” | These loops print directly; counters take constant extra memory. |
| “Reusing a loop variable name automatically causes an infinite loop.” | A new inner declaration shadows the outer name. It is confusing, but termination depends on the actual updates. |

**Formula sheet:** triangle r; inverted n-r+1; right alignment n-r; solid pyramid 2r-1; hollow interior 2r-3 except at tip; butterfly gap 2(n-r).

**Teach back:** given one row number, explain every segment's width before writing a loop. Then derive the same pattern with zero-based rows to prove you understand the relationships.

## 9. Sources

Local note: `01_cpp_basics/04_patterns.md`. Code reviewed: `05-patterns/diamond.cpp` and `05-patterns/butterfly.cpp`. The older diamond source included unrelated incomplete declarations; those were not reused. The implementations here explicitly choose a single widest row and consistent character widths. The picture-to-formula derivations and additional practice are original instructional additions.

<a id="questions"></a>
## 10. Revision questions

1. Give the four-step method for deriving a pattern. [Review](#method)
2. Explain how printing r, c, or a persistent counter changes a square. [Review](#family)
3. For n = 5 and row r = 3, give counts for an increasing triangle, inverted triangle, and right-aligned triangle. [Review](#family)
4. Why does Floyd's counter belong outside the row loop? What is the first value of row 4? [Review](#implementations)
5. Derive `r - c + 1` for a reverse number triangle and `r - c` for its alphabet index. [Review](#family)
6. **Implementation:** print a numeric palindrome pyramid for n = 4. Explain why the centre is not repeated. [Review](#implementations)
7. For hollow diamond n = 5, level 3, how many leading and interior spaces are printed? [Review](#implementations)
8. **Dry run:** draw the hollow diamond and butterfly for n = 2, preserving spaces. [Review](#implementations)
9. Explain the difference between a 2n-row and a 2n-1-row butterfly. [Review](#implementations)
10. **Debug:** a star consumes two output columns (`"* "`) while alignment assumes one. Why is the shape distorted? [Review](#method)
11. **Implementation:** derive a hollow rectangle using a cell predicate. What happens with one row or one column? [Review](#family)
12. Give the secondary-diagonal predicate with one-based and zero-based indices. [Review](#family)
13. Why is a hollow diamond still quadratic in text-output work? Distinguish output memory from working counters. [Review](#revision)
14. **Teach-back challenge:** invent a variation by reversing one segment or changing row order. Derive its formulas before coding. [Review](#method)

<details>
<summary>Answer key</summary>

1. Rows; segment counts per row; value/expression per symbol; newline after row.
2. Repeated row values; repeated column sequence; values continuing globally.
3. 3 stars; 3 stars; 2 leading spaces then 3 stars.
4. State must survive between rows; row 4 starts at 7.
5. At c = 1 start at r; subtract one for each step. A zero-based alphabet index is one less than the numeric value.
6. Section 4; ascending 1..r and descending r-1..1.
7. 2 leading spaces, 3 interior spaces.
8. Diamond rows: ` *`, `* *`, ` *`. Butterfly rows: `*  *`, `****`, `*  *`.
9. Widest row twice versus once; start the lower level at n versus n-1.
10. Printed token width and blank width disagree; count actual character columns or use equal-width cells.
11. Star if first/last row or first/last column. Every cell is boundary in a one-row or one-column rectangle.
12. `r + c == n + 1`; `r + c == n - 1`.
13. Interior and leading spaces must still be emitted; working storage is constant, while storing the whole output requires quadratic space for these layouts.
14. Judge your derivation by n = 1, 2, and a middle row; match all symbol and space counts, not just the outline.

</details>
