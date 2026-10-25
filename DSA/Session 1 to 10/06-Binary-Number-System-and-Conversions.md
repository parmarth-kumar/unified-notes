# Session 06 — Binary Number System and Conversions

**Lesson:** [Binary Number System](https://www.youtube.com/watch?v=xpy5NXiBFvA) · **Prerequisites:** integer division, remainder, loops, functions.

[Previous](./05-Functions-and-Problem-Decomposition.md) · [Course map](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md#course-map) · [Next](./07-Bitwise-Operators-Scope-and-Type-Modifiers.md)

**Core:** positional notation, decimal↔binary, common values, binary addition, one's/two's complement. **Extensions:** string-based conversion, validation and overflow checks, fixed width, hexadecimal, complexity.

**Navigate:** [Representation](#representation) · [Conversions](#conversions) · [Signed bits](#signed) · [Revision](#revision) · [Questions](#questions).

<a id="representation"></a>
## 1. Value and written representation are different

Thirteen is a quantity. `13` in decimal and `1101` in binary are different representations of that quantity. An ordinary integer object already stores a value in its machine representation; converting it to a binary **string** produces a human-readable sequence of `'0'` and `'1'` characters.

```text
13          decimal integer literal: value thirteen
0b1101      C++ binary integer literal: also value thirteen
"1101"      text: four digit characters, not an integer value
1101        decimal integer literal: value one thousand one hundred one
```

`cout << 0b1101` normally prints `13`, because integer output defaults to decimal formatting. A literal's spelling does not force its later display format.

## 2. Positional notation and powers

In base B, each digit d at position i contributes `d * B^i`, counting from i = 0 at the right. A valid digit is in `[0, B-1]`. The total value is the sum of those contributions.

For `1101₂`:

| Position i | 3 | 2 | 1 | 0 |
|---|---:|---:|---:|---:|
| Weight `2^i` | 8 | 4 | 2 | 1 |
| Digit | 1 | 1 | 0 | 1 |
| Contribution | 8 | 4 | 0 | 1 |

Total = 13. Here `₂` labels the base. It is explanatory notation, not C++ syntax. In C++, `^` is XOR, **not exponentiation**.

| Decimal | Binary | Decimal | Binary |
|---:|---|---:|---|
| 0 | 0 | 8 | 1000 |
| 1 | 1 | 9 | 1001 |
| 2 | 10 | 10 | 1010 |
| 3 | 11 | 12 | 1100 |
| 4 | 100 | 15 | 1111 |
| 5 | 101 | 16 | 10000 |
| 6 | 110 | 31 | 11111 |
| 7 | 111 | 32 | 100000 |

Powers of two have one set bit; a **set bit** is a digit equal to 1. For positive n, its minimal binary representation has `floor(log2(n)) + 1` bits. Zero is conventionally written with one digit, `0`.

Only the rightmost weight, 1, is odd; every larger power of two is even. Therefore the last bit determines parity: 1 for odd and 0 for even.

<a id="conversions"></a>
## 3. Decimal to binary: remainders arrive backwards

For nonnegative n, division gives `n = 2 * quotient + remainder`, with remainder 0 or 1. That remainder tells whether the lowest bit is set. Dividing n by 2 removes that bit's position; repeat for the remaining quotient.

### Dry run: decimal 42

| n before division | Quotient `n / 2` | Remainder `n % 2` | Digits collected so far |
|---:|---:|---:|---|
| 42 | 21 | 0 | 0 |
| 21 | 10 | 1 | 01 |
| 10 | 5 | 0 | 010 |
| 5 | 2 | 1 | 0101 |
| 2 | 1 | 0 | 01010 |
| 1 | 0 | 1 | 010101 |

These are **least significant first**. Reverse `010101` to get `101010`. Reversal is required because ordinary written notation places the most significant digit first.

### Why not build a decimal integer containing the binary digits?

A teaching method stores digit 0 or 1 at decimal place values `1, 10, 100, ...`. For 42 it returns the decimal integer 101010 as a picture of its bits. It is not the integer value 42, loses leading zeros, and exhausts decimal integer range quickly. Even the final `power *= 10` can overflow when its value will no longer be used.

Use a string for textual digits. The following code accepts the complete range of `unsigned long long`; it intentionally represents **nonnegative** values only.

<!-- code: to_binary -->
```cpp
#include <algorithm> // reverse
#include <string> // string
using namespace std;

string toBinary(unsigned long long n) {
    if (n == 0) {
        return "0";
    }
    string digits;
    while (n != 0) {
        digits.push_back((n % 2 == 0) ? '0' : '1');
        n /= 2;
    }
    reverse(digits.begin(), digits.end());
    return digits;
}
```

`string` is a sequence of characters. `push_back` appends one character; `reverse` reverses a range, here the entire string from `begin()` through the position before `end()`. This preview is enough to follow the conversion; vectors introduce related operations in Session 09.

For b produced bits, time is `O(b)` including reversal. The returned output uses `O(b)` space, while other scalar working storage is `O(1)`. Do not claim total extra memory is constant while storing b output characters. With a fixed machine type, b has a fixed maximum; the b-based analysis explains the scaling idea.

Avoid prepending a character to the front each time: repeated shifting/copying can make the construction quadratic in b. Appending then reversing keeps the work linear.

## 4. Binary to decimal: accumulate from the left

If the bits processed so far have value v, appending bit d changes the value to `2v + d`. The old digits all shift up one place, and the new bit occupies the ones place. This is the same reason decimal `37` followed by digit 4 becomes `10 * 37 + 4 = 374`.

### Dry run: text "101010"

| Character | Old value | Calculation | New value |
|---|---:|---|---:|
| 1 | 0 | 2*0+1 | 1 |
| 0 | 1 | 2*1+0 | 2 |
| 1 | 2 | 2*2+1 | 5 |
| 0 | 5 | 2*5+0 | 10 |
| 1 | 10 | 2*10+1 | 21 |
| 0 | 21 | 2*21+0 | 42 |

At every step, value equals the value of the prefix already processed. This invariant proves the algorithm.

### Validate before updating

<!-- code: parse_binary -->
```cpp
#include <limits> // numeric_limits
#include <stdexcept> // invalid_argument and overflow_error
#include <string> // string
using namespace std;

unsigned long long parseBinary(const string& bits) {
    if (bits.empty()) {
        throw invalid_argument("binary text must not be empty");
    }
    unsigned long long value = 0;
    const auto limit = numeric_limits<unsigned long long>::max();
    for (char ch : bits) {
        if (ch != '0' && ch != '1') {
            throw invalid_argument("only 0 and 1 are allowed");
        }
        unsigned long long digit = static_cast<unsigned long long>(ch - '0');
        if (value > (limit - digit) / 2) {
            throw overflow_error("binary value exceeds unsigned long long");
        }
        value = value * 2 + digit;
    }
    return value;
}
```

`const string&` reads the original string without copying it. The expression `value <= (limit - digit) / 2` is exactly the condition needed for `2 * value + digit <= limit`, rearranged so the **check itself** cannot overflow. The multiplication happens only after that guard.

Leading zeros are accepted: `"000101"` means 5. Empty text, `"1021"`, spaces, minus signs, and a `0b` prefix are rejected by this deliberately simple grammar. The parser processes digit text, not an entire C++ literal. For b input characters it takes `O(b)` time and `O(1)` auxiliary space, excluding the supplied input.

## 5. Binary addition

| A bit | B bit | Carry in | Sum bit | Carry out |
|---:|---:|---:|---:|---:|
| 0 | 0 | 0 | 0 | 0 |
| 0 | 1 | 0 | 1 | 0 |
| 1 | 1 | 0 | 0 | 1 |
| 1 | 1 | 1 | 1 | 1 |

In general `total = aBit + bBit + carry`, sum bit is `total % 2`, new carry is `total / 2`. Example: `0111 + 0001 = 1000`, since the carry propagates through three ones. A fixed-width representation can run out of bits; the interpretation of that event depends on signed versus unsigned arithmetic.

<a id="signed"></a>
## 6. Fixed-width signed values and two's complement

First specify the width and interpretation. The bit pattern `11111011` means 251 as an unsigned 8-bit value and -5 as an 8-bit two's-complement signed value. The bits alone do not say which interpretation applies.

For a w-bit two's-complement number, bit weights are:

```text
-2^(w-1), 2^(w-2), ..., 4, 2, 1
```

The highest bit has a negative weight. With w = 8, `11111011` is `-128 + 64 + 32 + 16 + 8 + 2 + 1 = -5`.

### Encode -5 using eight bits

```text
+5                00000101
invert eight bits 11111010  (one's complement operation)
add one           11111011  (-5 in two's complement)
```

Equivalently, the unsigned value of the bit pattern representing -x is `2^w - x` for positive x within the signed magnitude range. For x = 5, `256 - 5 = 251`, whose unsigned bits are `11111011`.

**Width matters:** inverting `101` as a three-bit pattern gives `010`; inverting its eight-bit padded form gives `11111010`. Do not remove the width while reasoning about complement operations.

### Range and the minimum-value trap

For w-bit two's complement, the signed range is `-2^(w-1) .. 2^(w-1)-1`. There is one zero, not a positive and negative zero. Eight bits therefore give -128 through 127. The minimum value has no representable positive counterpart in that signed width. Negating it, or taking its integer absolute value in the same type, is unsafe.

Two's-complement hardware can discard a carry beyond the fixed width: `11111011 + 00000101` leaves `00000000` in eight bits. This helps explain hardware arithmetic. It **does not** make signed overflow defined in C++.

### C++17 qualification

Most modern machines use two's complement, and it is the model used for these diagrams. C++17 still permits other signed representations; later language standards tightened representation requirements. Use unsigned values for portable bit manipulation and do not infer C++ signed-overflow rules from a hardware diagram. The [C++17 fundamental-type specification](https://timsong-cpp.github.io/cppwp/n4659/basic.fundamental) makes this historical distinction explicit.

## 7. Hexadecimal, octal, and useful connections

Hexadecimal is base 16, using `0..9` and `A..F` for 10..15. One hex digit maps to exactly four binary digits because 16 = 2⁴. For example, `0x2A` = `0010 1010₂` = 42. Octal is base 8; each digit corresponds to three bits.

In C++ source, `42`, `0b101010`, and `0x2A` represent the same value. A leading zero denotes an octal literal: `010` is eight, not ten. Avoid padding integer literals with leading zeros unless octal is intended.

Binary notation and byte order are different concepts. **Endianness** concerns the order of bytes in a multi-byte object; it does not reverse the normal written bit string produced by `toBinary`.

<a id="revision"></a>
## 8. Rapid revision and teach-back

- Representation changes notation, not the underlying quantity.
- Decimal to binary: repeated `% 2`, `/ 2`, reverse collected digits.
- Binary text to value: `value = 2 * value + digit`, validating grammar and range first.
- Zero needs explicit representation `"0"` in the conversion-to-text function.
- Powers of two have one set bit; positive n needs `floor(log2(n)) + 1` bits.
- Complement reasoning needs a width; signed interpretation needs a representation.
- Two's complement has one extra negative value; minimum-value negation is a trap.
- `^` is XOR in C++, not a power operator.

**Teach back:** derive both conversion algorithms from positional weights. Encode and decode -12 in eight-bit two's complement. Explain why a string is a better output than a decimal integer pretending to contain binary digits.

## 9. Sources

Local note: `02_bitwise_number_systems/01_binary_number_system.md`. Code reviewed: `07-binary-number-system/03-convert-decimal-to-binary.cpp`. Both used the decimal-digit construction as a teaching method; this file explains its limits and supplies a safe string alternative. Primary language rules are linked above; conversion derivations, dry runs, and guards are original explanations.

<a id="questions"></a>
## 10. Revision questions

1. Distinguish `13`, `0b1101`, `"1101"`, and `1101`. What does ordinary integer output print for `0b1101`? [Review](#representation)
2. Convert decimal 37 and 50 to binary by division. Explain the reverse order of remainders. [Review](#conversions)
3. Convert `101101₂` to decimal by weights and by the left-to-right recurrence. [Review](#conversions)
4. **Implementation:** write decimal-to-binary text conversion. Handle zero and give output-space complexity. [Review](#conversions)
5. What goes wrong with returning an int whose decimal digits look like binary? [Review](#conversions)
6. **Implementation:** parse a nonempty binary string with digit validation and an overflow guard. Derive the guard. [Review](#conversions)
7. What should the supplied parser do with `"000101"`, `"102"`, `""`, and `"0b101"`? [Review](#conversions)
8. Add `1111₂` and `1₂`. How many bits does the exact result need? [Review](#conversions)
9. Why does the last bit determine parity? How many bits are needed for positive 32? [Review](#representation)
10. Encode -12 in eight-bit two's complement, then decode it using signed weights. [Review](#signed)
11. Interpret `11111111` as unsigned and as signed eight-bit two's complement. [Review](#signed)
12. Why can the smallest signed two's-complement integer not be safely negated in the same type? [Review](#signed)
13. Does C++17 universally require two's complement? Does hardware wraparound make signed overflow defined? [Review](#signed)
14. Convert `0x2A` to binary and decimal. What is the C++ literal `010`? [Review](#signed)
15. **Interview:** compare conversion to a text representation with interpreting fixed-width negative bit patterns. What assumptions must be stated? [Review](#revision)

<details>
<summary>Answer key</summary>

1. Values 13 and 13; digit text; decimal 1101. Ordinary output of the binary literal is 13.
2. 37 = `100101`, 50 = `110010`. Division reveals low-order bits first.
3. 32+8+4+1 = 45; prefix values 1,2,5,11,22,45.
4. Section 3; zero returns `"0"`. O(b) time and returned text, O(1) other working storage.
5. It represents a different numerical quantity, cannot preserve leading zeros, and overflows after relatively few digits; updating its decimal place value can overflow too.
6. Section 4; require `value <= (limit-digit)/2` before `value*2+digit`.
7. 5; invalid digit exception; empty-input exception; invalid-character exception.
8. `10000₂`, five bits.
9. All other weights are even. 32 = 100000 uses six bits.
10. +12 = 00001100, invert to 11110011, add one -> 11110100. Signed weights give -128+64+32+16+4 = -12.
11. 255 and -1.
12. Its magnitude is one greater than the maximum positive value.
13. No and no. Use explicit representation assumptions and the language's arithmetic rules.
14. `00101010₂`, 42; octal 010 is decimal 8.
15. The conversion functions use nonnegative unsigned values and minimal digit text; signed diagrams require a fixed width and a signed representation such as two's complement.

</details>
