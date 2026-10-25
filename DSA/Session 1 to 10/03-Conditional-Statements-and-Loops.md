# Session 03 — Conditional Statements and Loops

**Lesson:** [Conditional Statements & Loops](https://www.youtube.com/watch?v=qR9U6bKxJ7g) · **Prerequisites:** variables, operators, input/output.

[Previous](./02-Variables-Data-Types-and-Operators.md) · [Course map](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md#course-map) · [Next](./04-Patterns-and-Nested-Loops.md)

**Core:** `if`, `else if`, ternary, `while`, `for`, `do-while`, `break`, `continue`, nested loops, sums and prime checks. **Extensions:** `switch`, loop invariants, safe square-root bounds, operation-count reasoning.

**Navigate:** [Branching](#branching) · [Loops](#loops) · [Worked algorithms](#worked) · [Complexity](#complexity) · [Revision](#revision) · [Questions](#questions).

<a id="branching"></a>
## 1. Conditions select what executes

A condition is an expression interpreted as true or false. An `if` tests it once when execution reaches that statement. A loop tests a condition repeatedly.

```text
if (condition) {
    actionA;
} else {
    actionB;
}
```

For mutually exclusive categories, use an `if / else if / else` chain. Conditions are considered from top to bottom, and only the first matching branch executes. Several independent `if` statements can all execute.

### Ordering conditions matters

For grade thresholds, check the highest first: `score >= 90`, then `score >= 80`, then `score >= 70`. If `score >= 70` comes first, a score of 95 is captured before the higher-grade branch can run. Validate an allowed range separately if the contract requires it.

### Character classification

For common ASCII-compatible environments, test `ch >= 'a' && ch <= 'z'` for an English lowercase letter and similarly for uppercase. This uses readable character literals rather than magic numbers. Full international text requires a richer model than one `char`. C++ guarantees contiguous decimal digits, but alphabetic contiguity is not a universal encoding guarantee.

### Ternary operator

`condition ? valueWhenTrue : valueWhenFalse` is an expression. For example, `int larger = (a > b) ? a : b;` selects a value. Only the selected branch is evaluated. Use it for simple choices; deep nesting becomes difficult to review.

### Switch and fallthrough — extension

A `switch` dispatches on an integral/enumeration value, such as an integer menu choice. Case labels are constant expressions. It does not directly dispatch on `string` or floating-point values.

<!-- code: switch_demo -->
```cpp
#include <iostream> // cout
using namespace std;

int main() {
    int option = 2;
    switch (option) {
        case 1:
            cout << "Add\n";
            break;
        case 2:
            cout << "Search\n";
            break;
        default:
            cout << "Invalid option\n";
    }
    return 0;
}
```

Without a `break`, execution continues into subsequent labelled statements: **fallthrough**. Sometimes it is intentional; C++17 has `[[fallthrough]];` to mark that intent. A `break` exits the nearest enclosing loop **or switch**. Do not assume it exits a surrounding function.

The language does not promise a particular machine implementation or complexity for a switch. A compiler may choose a jump table, comparisons, or another strategy. Likewise, an `if` chain need not translate literally into a linear chain of instructions.

<a id="loops"></a>
## 2. Loops are controlled repetition

Every ordinary counting loop has an initial state, a continuation test, a body, and a change that moves toward termination.

| Loop | Order | Minimum body executions | Natural use |
|---|---|---:|---|
| `while` | Test → body → test again | 0 | Repeat while a state remains valid |
| `for` | Initialise once → test → body → update → test | 0 | Counted traversal |
| `do-while` | Body → test → body again | 1 | Action that must happen once |

```text
for (initialisation; condition; update) {
    body;
}

do {
    body;
} while (condition); // trailing semicolon is required here
```

For array indices later, a half-open interval `[0, n)` means 0 is included and n is excluded: `i = 0; i < n; ++i`. It contains exactly n integers when `n >= 0`. For values from 1 through n, use an inclusive upper bound: `i = 1; i <= n; ++i`, with a representable update.

### Break, continue, and return

| Statement | Destination |
|---|---|
| `break` | After the nearest enclosing loop or switch |
| `continue` in `for` | The loop's update expression, then condition |
| `continue` in `while` | The condition; later statements in the body are skipped |
| `continue` in `do-while` | The condition at the bottom |
| `return` | Back to the caller, ending the current function |

A frequent infinite-loop bug:

```text
int i = 1;
while (i <= 5) {
    if (i == 3) {
        continue; // i stays 3 forever
    }
    ++i;
}
```

Repair the progress rule on every path. For a simple counter, a `for` loop keeps the update in one place and makes this harder to overlook. A nested `break` ends only the innermost enclosing construct; leaving several loops may need a flag, a helper function with `return`, or another explicit design.

### A full control-flow trace

<!-- code: continue_trace -->
```cpp
#include <iostream> // cout
using namespace std;

int main() {
    int sum = 0;
    for (int i = 1; i <= 7; ++i) {
        if (i % 2 == 0) {
            continue;
        }
        if (i == 7) {
            break;
        }
        sum += i;
    }
    cout << sum << '\n';
    return 0;
}
```

| `i` | Action | `sum` after action | Next control point |
|---:|---|---:|---|
| 1 | Add 1 | 1 | Update |
| 2 | Continue | 1 | Update, not directly the body |
| 3 | Add 3 | 4 | Update |
| 4 | Continue | 4 | Update |
| 5 | Add 5 | 9 | Update |
| 6 | Continue | 9 | Update |
| 7 | Break | 9 | After loop; no update |

Output is 9. Moving `sum += i` before the break would change the result; statement order is part of the algorithm.

<a id="worked"></a>
## 3. Sum odd numbers through n

**Contract:** `0 <= n <= 1000000`. Sum the odd integers in `[1, n]`.

<!-- code: sum_odds -->
```cpp
#include <stdexcept> // invalid_argument
using namespace std;

long long sumOdds(int n) {
    if (n < 0 || n > 1000000) {
        throw invalid_argument("n must be in [0, 1000000]");
    }
    long long sum = 0;
    for (int value = 1; value <= n; ++value) {
        if (value % 2 != 0) {
            sum += value;
        }
    }
    return sum;
}
```

`throw` reports a violated contract using an exception; later sessions use it for invalid inputs too. A caller can catch it, or supply valid input as promised. It is not part of the summation idea.

For `n = 6`, the running sums after values 1 through 6 are `1, 1, 4, 4, 9, 9`. The invariant is: before processing `value`, sum contains exactly the odd numbers smaller than `value` in the range.

**Improvement:** start at 1 and increase by 2, so even numbers are never visited. That roughly halves the iterations but remains `Theta(n)` time. Or let `k = (n + 1) / 2`, the number of odd integers in the range; the first k odd numbers sum to `k^2`. The formula takes `O(1)` fixed-width work. Promote before multiplying and respect the stated limits.

## 4. Prime checking: derive the improvement

A prime is greater than 1 and has no positive divisors other than 1 and itself. Negative numbers, 0, and 1 are not prime; 2 is prime.

The simple approach tries every divisor from 2 to `n - 1`, returning false as soon as one divides evenly. This is correct but has `O(n)` worst-case trial count.

If `n = a * b` and both a and b were greater than `sqrt(n)`, their product would exceed n. Therefore any composite n has a factor at most `sqrt(n)`. Checking that smaller range is sufficient. **Equality matters:** for 49, the divisor 7 must be checked.

Instead of `divisor * divisor <= n`, use `divisor <= n / divisor` for positive values. It expresses the same boundary without overflowing the product.

<!-- code: is_prime -->
```cpp
using namespace std;

bool isPrime(int n) {
    if (n < 2) {
        return false;
    }
    for (int divisor = 2; divisor <= n / divisor; ++divisor) {
        if (n % divisor == 0) {
            return false;
        }
    }
    return true;
}
```

There are no standard-library facilities here, so no header is needed. The `using` line is kept consistent with the teaching style.

### Dry runs

| n | Tests | Result |
|---:|---|---|
| 1 | Rejected before loop | false |
| 2 | `2 <= 2/2` is false | true |
| 25 | 2 fails, 3 fails, 4 fails, 5 divides | false |
| 29 | 2, 3, 4, 5 fail; 6 exceeds boundary | true |

For 29, at divisor 5 the condition is `5 <= 29/5`, or `5 <= 5`. At divisor 6 it is `6 <= 4`, false. Integer division is safe here because n and divisor are positive.

**Correctness:** before each candidate divisor, none of the smaller tested integers divides n. If one divides, n is composite. If the range ends without one, the factor-pair argument excludes all remaining possibilities. Worst-case time is `O(sqrt(n))`, auxiliary space `O(1)`. This is an upper bound for trial division, not a claim that no faster primality algorithm exists.

To print primes through N, call the function on each candidate from 2 through N. This simple composition uses at most `O(N sqrt(N))` work. A sieve is a later improvement for finding many primes together.

## 5. More useful drills

### Decimal digits

For a nonnegative integer, `n % 10` extracts the last digit and `n /= 10` removes it. For 507, extracted digits are 7, 0, 5. A `while (n > 0)` loop runs zero times for zero; if the task is to count digits, zero still has **one** decimal digit. A `do-while` naturally handles that case. Session 05 builds a sum-of-digits function.

### Leap year — branching exercise

A Gregorian leap year is divisible by 400, or divisible by 4 and not by 100. The Boolean condition is `(year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)`. Test 2000, 1900, 2024, and 2023. State the calendar and allowed year range; divisibility by 4 alone is insufficient.

### Nested loops

The inner loop executes afresh for each outer iteration. `for i = 1..3` containing `for j = 1..2` visits `(1,1), (1,2), (2,1), (2,2), (3,1), (3,2)`.

The loops control independent dimensions. Reusing `i` as a newly declared inner variable **shadows** the outer name; it is confusing but does not automatically create an infinite loop. Updating the outer counter accidentally is a different bug.

<a id="complexity"></a>
## 6. Count work instead of counting loop keywords

| Loop shape | Total body work | Tight time bound under constant-cost body |
|---|---|---|
| One visit to each of n elements | n | `Theta(n)` |
| n outer visits, m inner visits each | n*m | `Theta(nm)` |
| Inner loop visits 1, 2, ..., n times | `n(n+1)/2` | `Theta(n^2)` |
| Positive value repeatedly divided by 2 | About `log2(n)` | `Theta(log n)` for n > 1 |
| Two separate n-element loops | 2n | `Theta(n)` |

Two nested loops are not automatically quadratic. For example, an inner loop with exactly 5 iterations makes `5n` visits. A loop that repeatedly scans a growing range can have a triangular sum. Always identify what changes and how often.

**Numeric value versus input length:** `O(sqrt(n))` prime testing is measured in the value n. The number of bits needed to encode n is about `log2(n)`. Be clear about what n means when discussing number-theory algorithms.

<a id="revision"></a>
## 7. Pitfalls, rapid revision, and teach-back

- Use braces; a stray semicolon after `if`/`for`/`while` can create an empty body.
- In an `else if` ladder, first match wins; independent `if`s are different.
- `while` can run zero times; `do-while` runs at least once.
- Trace `continue` differently in `for` and `while`.
- Explain a progress measure: increasing index, shrinking range, or decreasing positive value.
- Prime check: reject n < 2; check through the square-root boundary **including it**; avoid an overflowing product.
- More efficient constant factors do not always change asymptotic complexity.
- Do not assert that an unsafe access or arithmetic operation must crash.

**Teach back:** prove the prime function, explain the `n / divisor` boundary, and trace `continue` without running the program. Then derive the sum of odd numbers formula.

## 8. Sources

Local note: `01_cpp_basics/03_conditional_statements_and_loops.md`. Code reviewed: `03-conditional-statements/if-else_if-else.cpp` and `04-loops/14_check_for_prime_optimized.cpp`. Source roots are recorded in Session 01. The prime example improves the older `i * i` condition; switch code-generation and shadowing claims have been qualified.

For C++ statement definitions, consult the primary [iteration statements](https://timsong-cpp.github.io/cppwp/n4659/stmt.iter) and [jump statements](https://timsong-cpp.github.io/cppwp/n4659/stmt.jump) clauses. The algorithmic proofs above are independent explanations.

<a id="questions"></a>
## 9. Revision questions

1. When can multiple independent `if` bodies run, and how does an `else if` chain differ? [Review](#branching)
2. Why should grade thresholds normally be checked from highest to lowest? [Review](#branching)
3. Compare `while`, `for`, and `do-while`; include their minimum execution counts. [Review](#loops)
4. Trace the complete `continue_trace` program without executing it. What changes if addition is moved before the break? [Review](#loops)
5. In a `while`, why can `continue` prevent termination? Repair the example. [Review](#loops)
6. What do `break`, `continue`, and `return` leave? What if a switch is inside a loop? [Review](#loops)
7. Give the sum of odd numbers through 9, 10, and 0. Derive an `O(1)` formula. [Review](#worked)
8. Why are 1 and negative numbers not prime? Why is 2 accepted by a zero-iteration divisor loop? [Review](#worked)
9. Prove the square-root bound and explain why `< sqrt(n)` would fail for a perfect square. [Review](#worked)
10. **Implementation:** write `isPrime` without multiplication in the loop condition. Test 0, 1, 2, 4, 25, 29. [Review](#worked)
11. **Debug:** a program prints “prime” each time `n % divisor != 0`. Explain the logical error. [Review](#worked)
12. How many visits occur for an inner loop `1..i` nested inside `i = 1..n`? What about a fixed inner range of length 5? [Review](#complexity)
13. Why does counting decimal digits need special handling for zero? [Review](#worked)
14. **Implementation:** classify Gregorian leap years. Give a case disproving “divisible by 4 is enough”. [Review](#worked)
15. **Interview:** state an invariant and a termination measure for a loop you wrote. How are they different? [Review](#revision)

<details>
<summary>Answer key</summary>

1. Each independent condition is tested; a ladder stops at its first true branch.
2. A lower threshold would capture higher scores prematurely.
3. Pretest, pretest with explicit update, posttest; minimum 0, 0, 1.
4. Original result 9. Moving addition immediately before the `i == 7` check, after the even-value continue, adds 7 and produces 16.
5. It skips the update in the body. Ensure progress before every continue, or use an appropriate for-loop update.
6. Nearest loop/switch; current iteration; current function. A switch's break leaves only that switch.
7. `25, 25, 0`; k odds where `k = (n + 1) / 2`, sum `k*k` with safe arithmetic.
8. Definition requires n > 1. For 2, there are no candidate divisors in the required range.
9. A composite has a factor <= sqrt(n); equality is essential for 25's factor 5.
10. Section 4; outputs false, false, true, false, false, true.
11. Failing one divisibility test eliminates only that divisor; return true only after all required tests fail.
12. `n(n+1)/2`, quadratic; `5n`, linear.
13. The usual while-loop makes no visits for zero, but its written representation has one digit. Use an initial special case or do-while.
14. Divisible by 400 or by 4 but not 100. 1900 is not a leap year; 2000 is.
15. Invariant says what remains true and supports correctness. Progress measure explains why execution reaches termination.

</details>
