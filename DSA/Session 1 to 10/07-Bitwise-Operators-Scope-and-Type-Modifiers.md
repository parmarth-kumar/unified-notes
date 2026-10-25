# Session 07 — Bitwise Operators, Scope, and Type Modifiers

**Lesson:** [Bitwise Operators, Data Type Modifiers & more](https://www.youtube.com/watch?v=r-u4uh3QvsQ) · **Prerequisites:** binary place values, fixed width, integer arithmetic.

[Previous](./06-Binary-Number-System-and-Conversions.md) · [Course map](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md#course-map) · [Next](./08-Arrays-Linear-Search-and-Two-Pointers.md)

**Core:** AND, OR, XOR, NOT, shifts, precedence, local/global scope, type modifiers. **Interview extensions:** bit masks, power of two, clear-lowest-set-bit, bit count, signed/unsigned pitfalls.

**Navigate:** [Operators](#operators) · [Masks](#masks) · [Algorithms](#algorithms) · [Scope and types](#types) · [Revision](#revision) · [Questions](#questions).

<a id="operators"></a>
## 1. Bitwise operations work position by position

Logical operators interpret an operand as a truth value. Bitwise operators combine the binary digits of integer operands. Do not substitute one family for the other.

| Bit a | Bit b | `a & b` AND | `a \| b` OR | `a ^ b` XOR |
|---:|---:|---:|---:|---:|
| 0 | 0 | 0 | 0 | 0 |
| 0 | 1 | 0 | 1 | 1 |
| 1 | 0 | 0 | 1 | 1 |
| 1 | 1 | 1 | 1 | 0 |

For 5 = `0101` and 3 = `0011`:

```text
AND   0101 & 0011 = 0001 = 1
OR    0101 | 0011 = 0111 = 7
XOR   0101 ^ 0011 = 0110 = 6
```

Leading zeros here are diagram padding. A **mask** is a bit pattern constructed to select or change particular positions.

### Logical versus bitwise: a concrete difference

`2 && 4` is true because both values are nonzero. `2 & 4` is zero because `0010` and `0100` have no shared set positions. `&&` and `||` short-circuit; built-in `&`, `|`, and `^` do not supply that short-circuit guard.

The symbol `&` is context-dependent: in `int& x` it declares a reference; in `a & b` it is binary bitwise AND; in `&x` it takes an address. Learn the surrounding syntax rather than assigning one meaning to the character.

### NOT must include a width and a type

`~x` flips all bits in the promoted operand type. If x is an unsigned w-bit value without padding, `~x` equals `(2^w - 1) - x`. Thus `~5U` is not the four-bit value `1010`; it flips the entire unsigned integer width.

On the ordinary two's-complement signed model, `~5` is -6. Keep that representation assumption explicit in C++17. Also, `~` applied to a small type such as `unsigned char` may first promote it to int, so the result is not necessarily an 8-bit unsigned object.

## 2. Shifts: move positions, respect the boundaries

For unsigned integers:

- `x << k` shifts left, inserts zeros at the low end, and discards high bits outside the width.
- `x >> k` shifts right and inserts zeros at the high end.
- Right shift is `floor(x / 2^k)`.
- Left shift corresponds to multiplication by `2^k` **modulo the unsigned range**; it is an exact unwrapped product only if no significant high bits are lost.

For example `5U << 1` is 10, and `5U >> 1` is 2.

The shift count must satisfy `0 <= k < width of the promoted left operand`. Negative counts and counts at least that width are UB. Casting the result afterwards does not repair an invalid shift.

**C++17 signed shifts:** left shifting a negative signed value is UB; other signed left shifts also have representability rules. Right shifting a negative signed value is implementation-defined in C++17. Do not teach it as universally identical to division or guaranteed sign extension. These rules are in the primary [C++17 shift-operator clause](https://timsong-cpp.github.io/cppwp/n4659/expr.shift). For interview masks, unsigned types make the intended behavior much clearer.

`1LL << k` changes the left operand's type, but it is still signed. `1ULL << k` is an unsigned long-long mask, subject to its width. Shifts do not accept arbitrary huge counts just because a machine instruction seems to handle them.

## 3. Precedence: use parentheses to show intent

```text
if (n & 1 == 0)       // WRONG: parsed as n & (1 == 0)
if ((n & 1U) == 0U)   // clear intended bitwise comparison for unsigned n
```

Equality binds more tightly than bitwise AND. `^` is XOR, not exponentiation. The safest learning practice is to parenthesise the bitwise expression before comparing it. Readability is more useful than recalling every precedence-table row in an interview.

<a id="masks"></a>
## 4. Get, set, clear, and toggle a bit

Bits are numbered from zero at the right. A one-bit mask at position k is `1U << k`, for a valid k in the unsigned-int width.

| Operation | Formula | Reason |
|---|---|---|
| Test bit k | `(x & mask) != 0` | Only a shared 1 survives |
| Set bit k | `x \| mask` | OR forces selected bit to 1 |
| Clear bit k | `x & ~mask` | AND with 0 clears selected bit |
| Toggle bit k | `x ^ mask` | XOR with 1 reverses selected bit |

<!-- code: bit_masks -->
```cpp
#include <limits> // numeric_limits<unsigned int>::digits
#include <stdexcept> // out_of_range
using namespace std;

unsigned int maskAt(int k) {
    const int width = numeric_limits<unsigned int>::digits;
    if (k < 0 || k >= width) {
        throw out_of_range("bit index outside unsigned int");
    }
    return 1U << k;
}

bool getBit(unsigned int x, int k) {
    return (x & maskAt(k)) != 0U;
}

unsigned int setBit(unsigned int x, int k) {
    return x | maskAt(k);
}

unsigned int clearBit(unsigned int x, int k) {
    return x & ~maskAt(k);
}

unsigned int toggleBit(unsigned int x, int k) {
    return x ^ maskAt(k);
}
```

### Dry run: x = 10 (`1010`), k = 2

Mask is `0100`. Test: `1010 & 0100 = 0000`, so false. Set: `1010 | 0100 = 1110`, or 14. Clear leaves 10 because the bit was already zero. Toggle produces 14; toggling the same bit again returns 10.

The other positions remain unchanged. This local reasoning is the proof: with a zero mask bit, OR/XOR preserve the original bit, while the complemented clear-mask has one and preserves it with AND.

All four operations take `O(1)` time and space on a fixed-width word. Big-integer masks spanning many machine words have a different cost.

<a id="algorithms"></a>
## 5. XOR properties: cancellation rather than magic

For any fixed-width value x:

- `x ^ x = 0` because each equal bit pair gives zero.
- `x ^ 0 = x` because XOR with zero preserves each bit.
- XOR is commutative and associative, so the order and grouping can change without changing the result.

Together, these allow duplicate pairs to cancel in any order. Session 09 uses them to solve Single Number. The assumption about counts matters: two different unpaired values would leave their XOR, not either original value.

Avoid XOR-swap tricks in ordinary code. They are harder to read and fail if both references identify the same object. Use `swap` with `<utility>` when a swap is intended.

## 6. Clear the lowest set bit

For positive unsigned x, subtracting one changes its lowest 1 to 0 and turns all lower trailing zeros into ones. ANDing the two values preserves the higher bits and removes that lowest set bit:

```text
x       = 1011000
x - 1   = 1010111
x&(x-1) = 1010000
```

This observation yields both of the following algorithms.

<!-- code: bit_algorithms -->
```cpp
using namespace std;

bool isPowerOfTwo(int n) {
    return n > 0 && ((n & (n - 1)) == 0);
}

int countSetBits(unsigned int x) {
    int count = 0;
    while (x != 0U) {
        x &= x - 1U;
        ++count;
    }
    return count;
}
```

### Power of two

A positive power of two has exactly one set bit. Removing that bit gives zero. Conversely, a positive number with more than one set bit remains nonzero after removing only one. The `n > 0` condition excludes zero and negatives and short-circuits before `n - 1` for the minimum int. One is a power of two because `1 = 2^0`.

Time and auxiliary space are `O(1)` for the fixed-width test. Compare with repeatedly dividing by two, which takes `O(log n)` steps in the numeric-value model.

### Count set bits: dry run for 13

| x before | Binary | x after `x & (x-1)` | Count afterwards |
|---:|---|---:|---:|
| 13 | 1101 | 12 (1100) | 1 |
| 12 | 1100 | 8 (1000) | 2 |
| 8 | 1000 | 0 | 3 |

The loop runs once for every set bit: `Theta(s)` steps for s set bits, with a constant-time zero case, and `O(1)` space. With a fixed w-bit type, there are at most w iterations, so this is also bounded constant time relative to unrelated input sizes. It is not necessarily one CPU cycle.

### Optional: isolate the lowest set bit

For unsigned x, `x & (0U - x)` keeps only the lowest set bit, or zero if x is zero. Unsigned subtraction is defined modulo the range. For 12 (`1100`), the result is 4 (`0100`). Be careful with the common signed spelling `x & -x`: negating a signed minimum value is not safe.

<a id="types"></a>
## 7. Scope: names belong to regions

A global name is declared outside functions. A block-local name is usable inside its scope. An inner declaration can **shadow** an outer name: the same spelling then refers to the inner object within that region.

<!-- code: scope_demo -->
```cpp
#include <iostream> // cout
using namespace std;

int value = 10;

int main() {
    int value = 20;
    {
        int value = 30;
        cout << value << ' ';
    }
    cout << value << ' ' << ::value << '\n';
    return 0;
}
```

Output is `30 20 10`. `::value` explicitly selects the global name. This demonstrates lookup; avoid gratuitous shadowing and global mutable state in algorithms because they make dependencies harder to track.

**Scope is not storage location.** A local automatic scalar commonly lives in a register or call frame; a local `static` object has static storage duration. A global object does not disappear when a function returns. Separate name visibility, lifetime, and the implementation's memory layout.

## 8. Type modifiers and portable assumptions

| Type spelling | Required minimum width/range capacity | Common implementation |
|---|---|---|
| `short` / `short int` | At least 16 bits of required integer capacity | Often 16-bit |
| `int` / `signed int` | At least 16 bits | Often 32-bit |
| `long` / `long int` | At least 32 bits | Often 32-bit on Windows, 64-bit on many Unix 64-bit systems |
| `long long` | At least 64 bits | Often 64-bit |
| `unsigned int` | Corresponding unsigned modular integer | Often 32-bit |

Type sizes follow nondecreasing storage requirements from short through long long, but several can be equal. Do not assume `long` is always 64-bit because the operating system is 64-bit. These are width/range statements; literal byte counts also depend on `CHAR_BIT`. See [fundamental types](https://timsong-cpp.github.io/cppwp/n4659/basic.fundamental).

Ordinary `int` is signed, but plain `char` can be signed or unsigned depending on implementation. `signed char`, `unsigned char`, and plain `char` are distinct types. `unsigned double` and `long long float` are not valid combinations.

Integer literal suffixes include `U`, `LL`, and `ULL`. They help specify arithmetic intent. A suffix does not make an arbitrary large number fit. `<cstdint>` offers fixed-width types such as `uint32_t` **when that exact-width type is available**; `<limits>` lets generic code query actual properties.

### Signed/unsigned comparison trap

When an int is compared with the corresponding unsigned int, the signed operand is converted to unsigned. Thus `-1 < 1U` is false: -1 becomes the maximum unsigned value. This is one reason to be deliberate with vector sizes, which use an unsigned size type. Avoid “fixes” that merely hide warnings without checking the valid range.

<a id="revision"></a>
## 9. Rapid revision and teach-back

- AND selects shared bits; OR combines; XOR marks differences; NOT flips the promoted width.
- Logical operators operate on truth and short-circuit; bitwise operators combine positions.
- Create masks with a suitable unsigned left operand and a checked shift count.
- `x & (x-1)` removes the lowest set bit for positive unsigned x.
- A positive power of two has exactly one set bit; zero must be excluded.
- A bit count removes one set bit per iteration.
- Parenthesise bitwise expressions before equality tests.
- Scope, lifetime, and physical memory location are separate ideas.
- C++17 negative signed shift behavior needs explicit qualification.

**Teach back:** derive a clear-bit mask, prove power-of-two detection, and explain why a mask should use the intended width before shifting.

## 10. Sources and practice links

Local note: `02_bitwise_number_systems/02_bitwise_operators_and_modifiers.md`. Code reviewed: `07-binary-number-system/06-data-type-modifiers.cpp` and `15-bit-manipulation/11-count-set-bits.cpp`. Corrections include implementation-defined negative right shift in C++17 and checking width before shifting.

Official practice: [231 — Power of Two](https://leetcode.com/problems/power-of-two/) and [191 — Number of 1 Bits](https://leetcode.com/problems/number-of-1-bits/). The supplied functions cover their core ideas; adapt the platform's wrapper/signature as required. Session 09 develops [136 — Single Number](https://leetcode.com/problems/single-number/).

<a id="questions"></a>
## 11. Revision questions

1. Compute `5 & 3`, `5 | 3`, `5 ^ 3`, and `2 && 4`. Explain each at bit/truth level. [Review](#operators)
2. Why is `~5U` not simply binary `1010`? What promotion issue arises with unsigned char? [Review](#operators)
3. State valid shift counts. Are `1U << -1` and shifting by the left type's full width valid? [Review](#operators)
4. In C++17, what happens when you left shift a negative signed value or right shift one? [Review](#operators)
5. Repair `n & 1 == 0` and explain its original parse. [Review](#operators)
6. **Dry run:** for x = 13 and k = 1, test, set, clear, and toggle that bit. [Review](#masks)
7. **Implementation:** create a checked one-bit mask and use it to clear a bit. [Review](#masks)
8. Prove that XOR pairs cancel even when duplicate values are separated. [Review](#algorithms)
9. Prove `x & (x-1)` clears exactly the lowest set bit. [Review](#algorithms)
10. Why does the power-of-two test need `n > 0`? Is 1 a power of two? [Review](#algorithms)
11. Trace countSetBits on 40 and on 0. Give its iteration count in terms of set bits. [Review](#algorithms)
12. Why can signed `x & -x` be unsafe at a type's minimum? Give the unsigned idea. [Review](#algorithms)
13. Predict the scope-demo output. Distinguish scope and lifetime. [Review](#types)
14. Is long always 64-bit? Is char always signed? Explain literal suffixes U and ULL. [Review](#types)
15. Explain why `-1 < 1U` is false and why mixing index types needs care. [Review](#types)

<details>
<summary>Answer key</summary>

1. 1, 7, 6, true. AND shares ones, OR combines ones, XOR retains differences, logical AND sees two nonzero values.
2. The full promoted unsigned-int width is inverted. Unsigned char may promote to int before NOT.
3. Nonnegative and smaller than the promoted left width; both examples are UB.
4. Negative signed left shift is UB; negative signed right shift is implementation-defined.
5. `(n & 1U) == 0U` for unsigned n; original is `n & (1 == 0)`.
6. 13 = 1101, mask 0010: false, 15, 13, 15.
7. Section 4; check k against numeric_limits<unsigned int>::digits before forming `1U << k`.
8. Commutativity/associativity regroup equal pairs; x^x = 0 and x^0 = x.
9. Subtraction flips the lowest 1 to 0 and lower zeros to ones; AND removes that 1 and leaves higher positions unchanged.
10. Zero also satisfies `0 & (0-1) == 0` under appropriate modular arithmetic; negatives are outside the positive-power contract. One is 2^0.
11. 40 = 101000 -> 100000 -> 0, count 2. Zero returns 0 without iterations. One iteration per set bit.
12. The positive negation of a signed minimum cannot fit. Use unsigned modular subtraction `x & (0U-x)`.
13. 30 20 10. Visibility of names versus existence of objects.
14. No and no. U requests an unsigned literal type; ULL requests unsigned long long.
15. The signed int converts to unsigned int, producing a large value. Use compatible types and check bounds before conversion.

</details>
