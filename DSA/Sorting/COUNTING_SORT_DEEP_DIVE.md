# Counting Sort — Ultimate Deep Dive Guide

> **Module:** `unified-notes/DSA/Sorting/COUNTING_SORT_DEEP_DIVE.md`  
> **Language:** Hinglish (Strictly Roman English Script — No Devanagari characters)  
> **Topic:** Counting Sort (Non-Comparison Sorting, Frequency Mapping, Stable Prefix Sums, Negative Offsets, & Deep Doubts)  
> **Goal:** Counting Sort ke mechanics, satellite data preservation, stability proofs, aur range limitations ko first principles se samajhna.

---

## Table of Contents

- [1. Real-World Intuition: Election Ballot Box ka Mental Model](#1-real-world-intuition)
- [2. Comparison vs Non-Comparison: Lower Bound $\Omega(n \log n)$ Kaise Break Hua?](#2-comparison-vs-non-comparison)
- [3. The Two Variations: Overwrite vs Stable Prefix Sums](#3-the-two-variations)
  - [Variation 1: Simple Frequency Overwrite (Primitive Values)](#variation-1-simple-frequency-overwrite)
  - [Variation 2: Stable Prefix Sums & Backward Traversal (Production Standard)](#variation-2-stable-prefix-sums)
- [4. Step-by-Step Visual Dry Run (Full State Trace with Negatives & Duplicates)](#4-step-by-step-visual-dry-run)
- [5. Complete C++ Implementations](#5-complete-c-implementations)
  - [5.1 Way 1: Primitive Value Overwriting ($O(k)$ Extra Space)](#51-way-1-primitive-value-overwriting)
  - [5.2 Way 2: Stable Production Counting Sort with Offset & Overflow Guard](#52-way-2-stable-production-counting-sort)
  - [5.3 Way 3: Sorting Custom Structs / Satellite Data (Real-World Objects)](#53-way-3-sorting-custom-structs)
- [6. Time & Space Complexity Analysis](#6-time--space-complexity-analysis)
  - [6.1 Why $\Theta(n + k)$?](#61-why-thetan--k)
  - [6.2 The Range Disaster: Kab Counting Sort Buri Tarah Fail Hota Hai?](#62-the-range-disaster)
- [7. Comprehensive Doubts Log (Top 8 Conceptual Confusions)](#7-comprehensive-doubts-log)
- [8. Quick Summary & Interview Recall Sheet](#8-quick-summary--interview-recall-sheet)

---

<a id="1-real-world-intuition"></a>

## 1. Real-World Intuition: Election Ballot Box ka Mental Model

Socho tumhare college me Class Representative (CR) ka election ho raha hai. Total 5 candidates khade hain, jinhe candidate number `1, 2, 3, 4, 5` diya gaya hai.  
Total 1,000 students ne vote diya aur har voter ne slip par candidate number likhkar ek peti me daal diya.

Tumhe in 1,000 voting slips ko sort karna hai:
- **Agar tum Bubble ya Merge sort lagate:**  
  Tum do-do slips ko compare karte: *"Kya slip A ka candidate slip B se chota hai?"* Isme hazaron comparisons lagte.
- **Counting Sort ka Tareeqa (No Comparisons!):**  
  1. Tum table par 5 boxes rakh dete ho jinpar label laga hai: `Box 1`, `Box 2`, `Box 3`, `Box 4`, `Box 5`.
  2. Tum slips ko ek-ek karke uthate ho aur bina kisi doosri slip se compare kiye, use uske matching box me drop kar dete ho (Tally count!).
  3. Jab saari slips khatam ho jayein, to tum Box 1 ki saari slips nikalte ho, fir Box 2 ki, fir Box 3 ki...
  4. Poori voting slips bina kisi aapas ke comparison ke **perfect order me sort ho gayi!**

---

<a id="2-comparison-vs-non-comparison"></a>

## 2. Comparison vs Non-Comparison: Lower Bound $\Omega(n \log n)$ Kaise Break Hua?

Hume pata hai ki comparison-based sorting algorithms (jaise Merge Sort, Quick Sort, Heap Sort) worst case me mathematically $\Omega(n \log n)$ se tez nahi chal sakte (kyunki har pairwise comparison sirf 1 bit information deta hai, aur $n!$ permutations cover karne ke liye $\log_2(n!) \approx n \log_2 n$ comparisons zaroori hain).

### Counting Sort is lower bound ko kaise tod deta hai?
Counting Sort numbers ko aapas me compare **karta hi nahi**!  
Ye direct **Array Indexing** (`count[x]++`) use karta hai:
- Har number directly memory address (index) ban jata hai.
- RAM me index lookup $O(1)$ direct hardware addressing operation hota hai.
- Is wajah se algorithm ka running time comparisons par nahi, balki do cheezon par depend karta hai:
  1. $n$: Total elements kitne hain array me.
  2. $k$: Numbers ka range kitna bada hai ($\text{maxVal} - \text{minVal} + 1$).
- Overall Time Complexity: **$\mathbf{\Theta(n + k)}$**!  
Agar $k = O(n)$ ho (jaise 10,000 numbers jinki values 1 se 50,000 ke beech hain), to ye **pure linear time $O(n)$** me execute hota hai!

---

<a id="3-the-two-variations"></a>

## 3. The Two Variations: Overwrite vs Stable Prefix Sums

Students aksar confuse hote hain kyunki college lectures me ek simple version padhaya jata hai, jabki textbooks aur technical interviews me doosra complex version expect kiya jata hai.

<a id="variation-1-simple-frequency-overwrite"></a>

### Variation 1: Simple Frequency Overwrite (Primitive Integers Only)
1. Har number ki frequency count karo: `count[x]++`.
2. Original array par pointer rakho (`idx = 0`), aur `count` array me iterate karke values direct overwrite kardo:
```cpp
int idx = 0;
for(int val = 0; val < range; ++val){
    while(count[val] > 0){
        arr[idx++] = val + minVal;
        count[val]--;
    }
}
```
- **Fayda:** Extra space sirf $O(k)$ lagti hai (no separate output array needed).
- **Nuksan:** Ye satellite data (objects / records) ka original identity destroy kar deta hai. Agar array me `Student { name: "Rahul", marks: 95 }` tha, to tum marks to rewrite kar doge par "Rahul" gayab ho jayega!

---

<a id="variation-2-stable-prefix-sums"></a>

### Variation 2: Stable Prefix Sums & Backward Traversal (Production Standard)
Agar elements complex records hon ya hume Radix Sort ke liye **Guaranteed Stability** chahiye ho, to hum 3 steps follow karte hain:
1. **Frequency Count:** `count[x]` me har value ki count bharo.
2. **Cumulative Prefix Sum:** `count[i] += count[i-1]`.  
   *Iska physical matlab:* `count[x]` ab ye batata hai ki $x$ se chote ya barabar total kitne elements hain. Yani output array me $x$ ka aakhiri slot index `count[x] - 1` hoga!
3. **Backward Traversal ($n - 1$ down to $0$):**  
   Original array ko peeche se iterate karo. Element ko uske sahi index par place karo aur `count` ko decrement karo:
   `output[--count[arr[i] - minVal]] = arr[i];`

> **Kyu Backward Traversal?**  
> Agar input me do duplicate elements the: $4_a$ pehle aur $4_b$ baad me.  
> Jab hum peeche se scan karenge, to hume pehle $4_b$ milega. Wo rightmost available position par place hoga.  
> Fir jab $4_a$ aayega, to count kam hone ki wajah se wo $4_b$ ke theek left me place hoga.  
> Output me order bana rahega: $4_a$ pehle, $4_b$ baad me $\implies$ **Guaranteed Stability!**

---

<a id="4-step-by-step-visual-dry-run"></a>

## 4. Step-by-Step Visual Dry Run (Full State Trace with Negatives & Duplicates)

Chalo ek aisa array trace karte hain jisme negative numbers, zeroes, aur duplicates sab hon!  
**Input:** `arr = [ 4a , -2a , 0 , 4b , -2b , 1 ]` ($n = 6$)

---

### Step 1: Range aur Offset Calculate Karo
- $\text{minVal} = -2$
- $\text{maxVal} = 4$
- $\text{range} = \text{maxVal} - \text{minVal} + 1 = 4 - (-2) + 1 = \mathbf{7}$.
- Offset formula: $\text{bucket} = \text{val} - \text{minVal} = \text{val} - (-2) = \text{val} + 2$.

Value se bucket mapping:
- Value `-2` $\to$ Bucket $0$
- Value `-1` $\to$ Bucket $1$
- Value `0` $\to$ Bucket $2$
- Value `1` $\to$ Bucket $3$
- Value `2` $\to$ Bucket $4$
- Value `3` $\to$ Bucket $5$
- Value `4` $\to$ Bucket $6$

---

### Step 2: Frequency Array (`count`) Banao
Size 7 ka array initialize karo zeroes ke saath:
```text
Bucket Index:    [ 0    1    2    3    4    5    6  ]
Mapped Value:    [-2   -1    0    1    2    3    4  ]
Count:           [ 0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ]
```

Array elements scan karte hain:
1. `arr[0] = 4a` $\to$ bucket $4 - (-2) = 6$ $\implies$ `count[6]++`
2. `arr[1] = -2a` $\to$ bucket $-2 - (-2) = 0$ $\implies$ `count[0]++`
3. `arr[2] = 0` $\to$ bucket $0 - (-2) = 2$ $\implies$ `count[2]++`
4. `arr[3] = 4b` $\to$ bucket $4 - (-2) = 6$ $\implies$ `count[6]++`
5. `arr[4] = -2b` $\to$ bucket $-2 - (-2) = 0$ $\implies$ `count[0]++`
6. `arr[5] = 1` $\to$ bucket $1 - (-2) = 3$ $\implies$ `count[3]++`

**Resulting Frequency Array:**
```text
Bucket:  0   1   2   3   4   5   6
Count: [ 2 , 0 , 1 , 1 , 0 , 0 , 2 ]
```

---

### Step 3: Prefix Sum (Cumulative Count) Calculate Karo
Formula: `count[i] = count[i] + count[i-1]` (for $i = 1$ to $6$):
- $i = 1$: $count[1] = 0 + 2 = 2$
- $i = 2$: $count[2] = 1 + 2 = 3$
- $i = 3$: $count[3] = 1 + 3 = 4$
- $i = 4$: $count[4] = 0 + 4 = 4$
- $i = 5$: $count[5] = 0 + 4 = 4$
- $i = 6$: $count[6] = 2 + 4 = 6$

**Prefix Sum Array:**
```text
Bucket:           0   1   2   3   4   5   6
Cumulative Count: [ 2 , 2 , 3 , 4 , 4 , 4 , 6 ]
```
> **Meaning:**  
> - Bucket 0 (value `-2`) ke liye count hai 2 $\implies$ `-2` values indices 0 and 1 par aayengi.
> - Bucket 6 (value `4`) ke liye count hai 6 $\implies$ `4` values index 5 par end hongi.

---

### Step 4: Backward Traversal ($i = 5$ down to $0$) and Output Placement
Khali output array of size 6: `[ _ , _ , _ , _ , _ , _ ]`

#### Iteration 1 ($i = 5$, `arr[5] = 1`):
- Bucket: $1 - (-2) = 3$.
- `count[3]` is `4`.
- Output position: `count[3] - 1 = 4 - 1 = 3`.
- `output[3] = 1`.
- Decrement `count[3]` from 4 to 3.
- Output state: `[ _ , _ , _ , 1 , _ , _ ]`

#### Iteration 2 ($i = 4$, `arr[4] = -2b`):
- Bucket: $-2 - (-2) = 0$.
- `count[0]` is `2`.
- Output position: `count[0] - 1 = 2 - 1 = 1`.
- `output[1] = -2b`.
- Decrement `count[0]` from 2 to 1.
- Output state: `[ _ , -2b , _ , 1 , _ , _ ]`

#### Iteration 3 ($i = 3$, `arr[3] = 4b`):
- Bucket: $4 - (-2) = 6$.
- `count[6]` is `6`.
- Output position: `count[6] - 1 = 6 - 1 = 5`.
- `output[5] = 4b`.
- Decrement `count[6]` from 6 to 5.
- Output state: `[ _ , -2b , _ , 1 , _ , 4b ]`

#### Iteration 4 ($i = 2$, `arr[2] = 0`):
- Bucket: $0 - (-2) = 2$.
- `count[2]` is `3`.
- Output position: `count[2] - 1 = 3 - 1 = 2`.
- `output[2] = 0`.
- Decrement `count[2]` from 3 to 2.
- Output state: `[ _ , -2b , 0 , 1 , _ , 4b ]`

#### Iteration 5 ($i = 1$, `arr[1] = -2a`):
- Bucket: $-2 - (-2) = 0$.
- `count[0]` is now `1`!
- Output position: `count[0] - 1 = 1 - 1 = 0`.
- `output[0] = -2a`.
- Decrement `count[0]` from 1 to 0.
- Output state: `[ -2a , -2b , 0 , 1 , _ , 4b ]`  
*(Notice: $-2_a$ index 0 par gaya aur $-2_b$ index 1 par $\implies$ Relative order preserved!)*

#### Iteration 6 ($i = 0$, `arr[0] = 4a`):
- Bucket: $4 - (-2) = 6$.
- `count[6]` is now `5`!
- Output position: `count[6] - 1 = 5 - 1 = 4`.
- `output[4] = 4a`.
- Decrement `count[6]` from 5 to 4.
- Output state: `[ -2a , -2b , 0 , 1 , 4a , 4b ]`  
*(Notice: $4_a$ index 4 par gaya aur $4_b$ index 5 par $\implies$ Relative order preserved!)*

---

### Final Result:
$$\mathbf{[-2_a, -2_b, 0, 1, 4_a, 4_b]}$$
Poora array perfectly sorted hai aur 100% stable hai!

---

<a id="5-complete-c-implementations"></a>

## 5. Complete C++ Implementations

<a id="51-way-1-primitive-value-overwriting"></a>

### 5.1 Way 1: Primitive Value Overwriting ($O(k)$ Extra Space)

```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

void countingSortValues(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());

    // Safe 64-bit calculation
    long long range = static_cast<long long>(maxVal) - minVal + 1;
    if(range > 5000000){
        throw invalid_argument("Range too large for Counting Sort memory allocation.");
    }

    vector<int> count(range, 0);

    // 1. Tally counts
    for(int x : arr){
        count[x - minVal]++;
    }

    // 2. Overwrite back into original array
    int idx = 0;
    for(int i = 0; i < range; ++i){
        while(count[i] > 0){
            arr[idx++] = i + minVal;
            count[i]--;
        }
    }
}
```

---

<a id="52-way-2-stable-production-counting-sort"></a>

### 5.2 Way 2: Stable Production Counting Sort with Offset & Overflow Guard

```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>
using namespace std;

void countingSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());

    // 64-bit calculation prevents signed 32-bit overflow (e.g. INT_MAX - INT_MIN + 1)
    long long range = static_cast<long long>(maxVal) - minVal + 1;
    if(range > 5000000){
        throw invalid_argument("Range too large for Counting Sort memory allocation.");
    }

    vector<int> count(range, 0);
    vector<int> output(n);

    // Phase 1: Frequency Count
    for(int x : arr){
        count[x - minVal]++;
    }

    // Phase 2: Prefix Sums (Cumulative Frequency)
    for(size_t i = 1; i < count.size(); ++i){
        count[i] += count[i - 1];
    }

    // Phase 3: Backward Traversal (Guarantees Stability)
    for(int i = n - 1; i >= 0; --i){
        int val = arr[i];
        int bucket = val - minVal;
        output[count[bucket] - 1] = val;
        count[bucket]--;
    }

    arr = move(output); // Move assignment prevents copying overhead
}
```

---

<a id="53-way-3-sorting-custom-structs"></a>

### 5.3 Way 3: Sorting Custom Structs / Satellite Data (Real-World Objects)

```cpp
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Student {
    string name;
    int marks; // Range 0 to 100
};

void sortStudentsByMarks(vector<Student>& students){
    int n = static_cast<int>(students.size());
    if(n <= 1) return;

    // Marks range is 0 to 100
    int range = 101;
    vector<int> count(range, 0);
    vector<Student> output(n);

    for(const auto& s : students){
        count[s.marks]++;
    }

    for(int i = 1; i < range; ++i){
        count[i] += count[i - 1];
    }

    // Backward traversal preserves student arrival order for equal marks!
    for(int i = n - 1; i >= 0; --i){
        int m = students[i].marks;
        output[count[m] - 1] = students[i];
        count[m]--;
    }

    students = move(output);
}
```

---

<a id="6-time--space-complexity-analysis"></a>

## 6. Time & Space Complexity Analysis

<a id="61-why-thetan--k"></a>

### 6.1 Why $\Theta(n + k)$?
Algorithm me 3 distinct loops chalte hain:
1. Input array scan karke frequency count karna: takes $O(n)$ time.
2. `count` array me prefix sums calculate karna: takes $O(k)$ time (jahan $k = \text{range}$).
3. Backward pass me elements ko output array me place karna: takes $O(n)$ time.

Total Time:
$$T(n, k) = O(n) + O(k) + O(n) = \mathbf{\Theta(n + k)}$$
Ye time **Best, Average, aur Worst** teeno cases me identical rehta hai!

---

<a id="62-the-range-disaster"></a>

### 6.2 The Range Disaster: Kab Counting Sort Buri Tarah Fail Hota Hai?

Counting Sort tabhi use karna chahiye jab:
$$k = O(n)$$

#### Catastrophic Failure Example:
Maan lo array me sirf 4 elements hain:
`arr = [ 1 , 5 , 2 , 1000000000 ]` ($n = 4$)
- Yahan $k = 10^9 - 1 + 1 = 1,000,000,000$.
- Counting Sort $10^9$ integers ka vector banane ki koshish karega ($\approx 4\text{ GB RAM}$!).
- Sirf 4 elements sort karne ke liye 4 GB memory consume hogi aur loop 1 billion times chalega!
- **Result:** Out of Memory (Memory Limit Exceeded) ya Time Limit Exceeded (TLE).  
Aise cases me standard $O(n \log n)$ QuickSort ya MergeSort lagbhag 1 microsecond me sort kar dega!

---

<a id="7-comprehensive-doubts-log"></a>

## 7. Comprehensive Doubts Log (Top 8 Conceptual Confusions)

---

### Doubt 01: Prefix sum array kyu banate hain? Direct frequency overwrite kyu nahi karte?
**Answer:**  
Primitive `int` array sort karne ke liye direct frequency overwrite bilkul sahi hai. Lekin real-world programming me hum objects/structs sort karte hain (jaise Employee jisme ID, Salary, Department ho).  
Direct overwrite sirf number likhta hai, object ke baaki fields ko recreate nahi kar sakta. Prefix sum array har key ke liye **exact output slot index** calculate kar deta hai jisse poora ka poora original object satellite data ke sath sahi slot me transfer ho jata hai.

---

### Doubt 02: Backward traversal ($n - 1$ down to $0$) kyu zaroori hai? Forward loop kyu galat hai?
**Answer:**  
Forward loop chalane se array **UNSTABLE** ho jata hai!  
*Dekho kaise:*  
Maan lo do duplicate numbers hain: $5_a$ (index 1 par) aur $5_b$ (index 4 par).  
Prefix sum batata hai ki 5 ka aakhiri index hai 3.  
- **Forward loop me:** Pehle $5_a$ aayega aur wo aakhiri slot index 3 par baith jayega. Baad me $5_b$ aayega aur wo slot 2 par baithega. Isse output me $5_b$ pehle aa gaya aur $5_a$ baad me! (Order reverse ho gaya).
- **Backward loop me:** Pehle $5_b$ aata hai aur wo aakhiri slot 3 par baithta hai. Fir $5_a$ aata hai aur wo slot 2 par baithta hai. Order safe rehta hai ($5_a$ before $5_b$) $\implies$ **Stable!**

---

### Doubt 03: Cumulative count value 5 ka matlab output index 4 kyu hota hai?
**Answer:**  
Kyunki C++ me arrays **0-indexed** hote hain!  
Agar prefix sum kehta hai ki $x$ se chote ya barabar total 5 elements hain, to 0-based indexing me 5 elements ke slots honge: `0, 1, 2, 3, 4`.  
Inme sabse aakhiri slot hamesha $5 - 1 = 4$ hoga! Isliye hum `output[count[val] - 1]` likhte hain.

---

### Doubt 04: Negative numbers ko Counting Sort me kaise handle karte hain?
**Answer:**  
C++ vector me negative index `count[-5]` invalid hota hai. Isliye hum pure array ka **`minVal`** dhoondte hain aur har element se `minVal` subtract karke ek non-negative offset banate hain:
$$\text{bucket} = \text{arr}[i] - \text{minVal}$$
Agar `minVal = -10` hai, to `-10` ban jayega `-10 - (-10) = 0`, `-9` ban jayega `1`, aur `0` ban jayega `10`. Saare numbers safely 0 ya positive range me map ho jate hain!

---

### Doubt 05: `range = maxVal - minVal + 1` me `long long` kyu compulsory hai?
**Answer:**  
32-bit signed integers me:
- `INT_MAX = 2,147,483,647`
- `INT_MIN = -2,147,483,648`  
Agar array me dono values present hon, to `maxVal - minVal` banega:
$$2,147,483,647 - (-2,147,483,648) = 4,294,967,295$$
Ye number standard 32-bit signed `int` ki maximum capacity ($2 \times 10^9$) se double hai! Agar `int` use karoge to signed overflow hoga jo C++ me **Undefined Behavior** hai. Isliye `static_cast<long long>(maxVal) - minVal + 1` likhna zaroori hai.

---

### Doubt 06: Kya Counting Sort in-place ho sakta hai?
**Answer:**  
Stable Counting Sort **kabhi in-place nahi ho sakta**. Usse $O(n)$ size ka separate output array chahiye hi hota hai.  
Halanki primitive numbers ke liye frequency overwrite version in-place ho sakta hai, lekin usme stability aur satellite data ka loss ho jata hai.

---

### Doubt 07: Radix Sort Counting Sort par kyu depend karta hai?
**Answer:**  
Radix Sort LSD (Least Significant Digit) algorithm numbers ko digit-by-digit sort karta hai (ones digit, fir tens digit, fir hundreds digit).  
Har digit pass me sirf 10 possible values hoti hain ($0$ to $9$). To har digit pass ko sort karne ke liye **Counting Sort sabse ideal algorithm** hota hai ($k = 10$). Aur kyunki agle digit pass me pichle digit ka order maintain rehna zaroori hai, isliye Counting Sort ki **Stability property** Radix Sort ko correctly kaam karne ke liye strictly mandatory hoti hai!

---

### Doubt 08: Agar array me values $1$ se $10^9$ ke beech hon, to kya linear time sorting possible hai?
**Answer:**  
Normal Counting Sort wahan fail ho jayega. Lekin do alternatives hain:
1. **Coordinate Compression (Hashing):** Agar $n$ chota hai, to values ko unke rank ($0$ to $n-1$) me map kar do.
2. **Radix Sort:** Agar numbers non-negative 32-bit ya 64-bit integers hon, to Radix Sort unhe $b = 256$ (byte-by-byte) ke base me sirf 4 ya 8 stable Counting Sort passes me sort kar deta hai!

---

<a id="8-quick-summary--interview-recall-sheet"></a>

## 8. Quick Summary & Interview Recall Sheet

```text
┌────────────────────────────────────────────────────────────────────────┐
│                       COUNTING SORT CHEAT SHEET                        │
├──────────────────────┬─────────────────────────────────────────────────┤
│ Paradigm             │ Non-Comparison Sort (Direct Index Addressing)   │
│ Time Complexity      │ Theta(n + k) (Best = Avg = Worst)               │
│ Auxiliary Space      │ O(n + k) (Stable version), O(k) (Overwrite)     │
│ Stability            │ STABLE (Backward traversal + prefix sums)       │
│ Adaptability         │ NON-ADAPTIVE (always scans entire range)        │
│ Negative Numbers     │ Supported via 'arr[i] - minVal' offset          │
│ Major Constraint     │ Efficient ONLY when range k = O(n)              │
│ Superpower           │ Subroutine for Radix Sort (k = 10 or 256)       │
└──────────────────────┴─────────────────────────────────────────────────┘
```

---
*Created as part of the Unified Notes Learning System.*
