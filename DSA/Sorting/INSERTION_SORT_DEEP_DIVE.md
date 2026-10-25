# Insertion Sort — Ultimate Deep Dive Guide

> **Module:** `unified-notes/DSA/Sorting/INSERTION_SORT_DEEP_DIVE.md`  
> **Language:** Hinglish (Strictly Roman English Script — No Devanagari characters)  
> **Topic:** Insertion Sort (Intuition, Mechanics, Visual Dry Run, Edge Cases, Code, & Deep Doubts)  
> **Goal:** Insertion Sort ke har single aspect ko crystal-clear samajhna — basic intuition se lekar internal CPU-level optimization aur production interview traps tak.

---

## Table of Contents

- [1. Real-World Intuition: Taash (Playing Cards) ka Mental Model](#1-real-world-intuition)
- [2. Core Mechanics: Shifting vs Swapping (Kyu Shifting Superior Hai?)](#2-core-mechanics-shifting-vs-swapping)
- [3. Step-by-Step Visual Dry Run (Full State Trace)](#3-step-by-step-visual-dry-run)
- [4. Complete C++ Implementations](#4-complete-c-implementations)
  - [4.1 Standard In-Place Array Insertion Sort](#41-standard-in-place-array-insertion-sort)
  - [4.2 Descending Order Insertion Sort](#42-descending-order-insertion-sort)
  - [4.3 Binary Insertion Sort (Binary Search Optimization)](#43-binary-insertion-sort)
  - [4.4 Linked List Insertion Sort (LeetCode 147 Style)](#44-linked-list-insertion-sort)
- [5. Time & Space Complexity Analysis](#5-time--space-complexity-analysis)
  - [5.1 The Inversion Relation: $\Theta(n + I)$](#51-the-inversion-relation-thetan--i)
  - [5.2 Best, Average, and Worst Case](#52-best-average-and-worst-case)
  - [5.3 Space & In-Place Nature](#53-space--in-place-nature)
- [6. Stability Proof: Duplicate Elements Safe Kyu Rehte Hain?](#6-stability-proof)
- [7. Comprehensive Doubts Log (Top 8 Conceptual Confusions Resolved)](#7-comprehensive-doubts-log)
- [8. Quick Summary & Interview Recall Sheet](#8-quick-summary--interview-recall-sheet)

---

<a id="1-real-world-intuition"></a>

## 1. Real-World Intuition: Taash (Playing Cards) ka Mental Model

Socho tum doston ke saath taash (cards) khel rahe ho. Jab dealer tumhe ek-ek karke cards baant-ta hai, to tum apne haath me cards ko kaise arrange karte ho?

```text
Tumhara Left Hand (Already Sorted Cards)        Table se Naya Card Aaya
    [ 2 , 4 , 7 , 9 ]                                      5
```

1. Tum naye card `5` ko pakadte ho (ise hum algorithm me **`key`** bolte hain).
2. Ab tum apne haath me right-to-left scan karte ho:
   - `9 > 5`? Haan! To `9` ko ek slot aage (right me) khiska do.
   - `7 > 5`? Haan! To `7` ko ek slot aage khiska do.
   - `4 > 5`? Nahi! `4` chota hai. **Bas yahin ruk jao!**
3. `4` ke theek baad jo khali slot bana hai, wahan apna naya card `5` drop (insert) kardo!
4. Ab tumhara haath ban gaya: `[ 2 , 4 , 5 , 7 , 9 ]` — poora sorted!

> **Core Philosophy:**  
> Har step par array do hisson me divide hota hai:
> - **Sorted Prefix (Left Hand):** Index `0` se `i - 1` tak.
> - **Unsorted Suffix (Deck):** Index `i` se `n - 1` tak.  
> Har pass me hum unsorted suffix ke pehle element (`arr[i]`) ko uthate hain aur sorted prefix me uski exact rightful jagah par **insert** kar dete hain.

---

<a id="2-core-mechanics-shifting-vs-swapping"></a>

## 2. Core Mechanics: Shifting vs Swapping (Kyu Shifting Superior Hai?)

Bohot se beginners Insertion Sort ko galat tareeqe se samajhte hain aur adjacent elements ko baar-baar `swap(arr[j], arr[j-1])` karte rehte hain.

### Swapping Method (Beginner Mistake):
```cpp
// Inefficient Swap Approach
for(int i = 1; i < n; ++i){
    for(int j = i; j > 0 && arr[j] < arr[j - 1]; --j){
        swap(arr[j], arr[j - 1]); // Har step par full 3-step swap!
    }
}
```

### Shifting Method (Optimal Approach):
```cpp
// Optimal Shifting Approach
for(int i = 1; i < n; ++i){
    int key = arr[i]; // Card ko ek baar haath me utha lo
    int j = i - 1;
    while(j >= 0 && arr[j] > key){
        arr[j + 1] = arr[j]; // Sirf right shift (Single write)
        --j;
    }
    arr[j + 1] = key; // Sahi jagah par single write
}
```

### Shifting Swapping se Kafi Fast Kyu Hai?
Agar kisi element ko $t$ positions left khisakna hai:

| Property | Swapping Method | Shifting Method |
|---|---|---|
| **Array Reads** | $2t$ reads | $t$ reads |
| **Array Writes** | $2t$ writes (har swap 2 writes karta hai) | $t + 1$ writes ($t$ shifts + 1 final key placement) |
| **Temporary Variables** | Har step par temporary swap allocation | Sirf 1 variable `key` |
| **CPU Register Optimization** | Array me baar-baar write hone se cache dirty hoti hai | CPU registers me shift efficiently pipeline ho jata hai |

### The "Duplicate Ghost" Myth:
Shifting ke dauraan jab hum `arr[j+1] = arr[j]` karte hain, to array me temporary duplicate values dikhte hain:
- Shuruat: `[2, 4, 7, 9 | 5]` (Key = 5)
- 9 shift hua: `[2, 4, 7, 9, 9]` (Array me do baar 9 aa gaya!)
- 7 shift hua: `[2, 4, 7, 7, 9]` (Array me do baar 7 aa gaya!)
- 5 insert hua: `[2, 4, 5, 7, 9]`

Students dar jaate hain ki *"Arre 5 to gayab ho gaya aur 7 duplicate ho gaya!"*  
Lekin **koi data lost nahi hota** kyunki original value already `key` variable ke andar safe baithi hui hai!

---

<a id="3-step-by-step-visual-dry-run"></a>

## 3. Step-by-Step Visual Dry Run (Full State Trace)

Chalo ek interesting array trace karte hain jisme random numbers, duplicates, aur reverse pairs sab hain:  
**Input:** `arr = [ 7 , 3 , 5 , 8 , 2 , 5 ]` ($n = 6$)

```text
Legend:
[ ... ] = Sorted Prefix (Sorted Hand)
|       = Boundary separating Sorted and Unsorted parts
key     = Current element to be inserted
```

---

### Initial State:
- `[ 7 | 3 , 5 , 8 , 2 , 5 ]`  
  Index `0` (`7`) by definition already sorted hai (1 element ka subarray hamesha sorted hota hai).

---

### Pass 1 ($i = 1$, `key = arr[1] = 3`):
- Sorted prefix: `arr[0...0] = [ 7 ]`.
- $j = 0$: Check karo `arr[0] > key` $\implies 7 > 3$ (True!).
  - Shift $7$ to index $1$: `arr[1] = arr[0]`. Array state: `[ 7 , 7 , 5 , 8 , 2 , 5 ]`.
  - $j$ becomes $-1$. Loop stops (`j >= 0` fails).
- Insert `key` at `arr[j + 1] = arr[0] = 3`.
- **Array after Pass 1:** `[ 3 , 7 | 5 , 8 , 2 , 5 ]`

---

### Pass 2 ($i = 2$, `key = arr[2] = 5`):
- Sorted prefix: `arr[0...1] = [ 3 , 7 ]`.
- $j = 1$: Check `arr[1] > key` $\implies 7 > 5$ (True!).
  - Shift $7$ to index $2$: `arr[2] = 7`. Array state: `[ 3 , 7 , 7 , 8 , 2 , 5 ]`.
  - $j$ becomes $0$.
- $j = 0$: Check `arr[0] > key` $\implies 3 > 5$ (False!).
  - Loop stops immediately!
- Insert `key` at `arr[j + 1] = arr[1] = 5`.
- **Array after Pass 2:** `[ 3 , 5 , 7 | 8 , 2 , 5 ]`

---

### Pass 3 ($i = 3$, `key = arr[3] = 8`):
- Sorted prefix: `arr[0...2] = [ 3 , 5 , 7 ]`.
- $j = 2$: Check `arr[2] > key` $\implies 7 > 8$ (False!).
  - Loop condition pehli hi baar me fail ho gayi! Zero shifts!
- Insert `key` at `arr[j + 1] = arr[3] = 8` (Same position).
- **Array after Pass 3:** `[ 3 , 5 , 7 , 8 | 2 , 5 ]`  
  *(Notice: Agar element already bada ho, to ek hi comparison me pass khatam ho jata hai — Adaptive nature!)*

---

### Pass 4 ($i = 4$, `key = arr[4] = 2`):
- Sorted prefix: `arr[0...3] = [ 3 , 5 , 7 , 8 ]`.
- $j = 3$: $8 > 2$ $\implies$ Shift $8$ right $\implies$ `[ 3 , 5 , 7 , 8 , 8 , 5 ]`, $j = 2$.
- $j = 2$: $7 > 2$ $\implies$ Shift $7$ right $\implies$ `[ 3 , 5 , 7 , 7 , 8 , 5 ]`, $j = 1$.
- $j = 1$: $5 > 2$ $\implies$ Shift $5$ right $\implies$ `[ 3 , 5 , 5 , 7 , 8 , 5 ]`, $j = 0$.
- $j = 0$: $3 > 2$ $\implies$ Shift $3$ right $\implies$ `[ 3 , 3 , 5 , 7 , 8 , 5 ]`, $j = -1$.
- Loop ends (`j = -1`).
- Insert `key` at `arr[0] = 2`.
- **Array after Pass 4:** `[ 2 , 3 , 5 , 7 , 8 | 5 ]`

---

### Pass 5 ($i = 5$, `key = arr[5] = 5` — Note: Duplicate Key!):
- Sorted prefix: `arr[0...4] = [ 2 , 3 , 5a , 7 , 8 ]` (Maan lo pehla 5 hai $5_a$, aur current key hai $5_b$).
- $j = 4$: $8 > 5_b$ $\implies$ Shift $8$ right $\implies j = 3$.
- $j = 3$: $7 > 5_b$ $\implies$ Shift $7$ right $\implies j = 2$.
- $j = 2$: `arr[2]` is $5_a$. Check: `arr[2] > key` $\implies 5_a > 5_b$ (**False! Equal elements me strict '>' fail hota hai!**).
- **Loop stops!** $5_a$ right shift nahi hoga!
- Insert $5_b$ at `arr[j + 1] = arr[3] = 5_b`.
- **Array after Pass 5:** `[ 2 , 3 , 5a , 5b , 7 , 8 ]`  
  *(Proof of Stability: $5_a$ input me pehle tha, aur output me bhi $5_a$ $5_b$ ke pehle hi raha!)*

---

### Summary Table of Dry Run:

| Pass ($i$) | Key | Comparisons Made | Shifts Performed | Array State after Pass |
|:---:|:---:|:---:|:---:|---|
| **Start** | — | — | — | `[ 7 , 3 , 5 , 8 , 2 , 5 ]` |
| **Pass 1** | 3 | 1 | 1 | `[ 3 , 7 , 5 , 8 , 2 , 5 ]` |
| **Pass 2** | 5 | 2 | 1 | `[ 3 , 5 , 7 , 8 , 2 , 5 ]` |
| **Pass 3** | 8 | 1 | 0 | `[ 3 , 5 , 7 , 8 , 2 , 5 ]` |
| **Pass 4** | 2 | 4 | 4 | `[ 2 , 3 , 5 , 7 , 8 , 5 ]` |
| **Pass 5** | 5 | 3 | 2 | `[ 2 , 3 , 5 , 5 , 7 , 8 ]` |

---

<a id="4-complete-c-implementations"></a>

## 4. Complete C++ Implementations

<a id="41-standard-in-place-array-insertion-sort"></a>

### 4.1 Standard In-Place Array Insertion Sort

```cpp
#include <vector>
#include <iostream>
using namespace std;

void insertionSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return; // Base case: 0 ya 1 element already sorted hota hai

    // Outer loop: Hand me agla naya card uthao (index 1 se n - 1 tak)
    for(int i = 1; i < n; ++i){
        int key = arr[i]; // New card to be inserted
        int j = i - 1;    // Sorted hand me peeche chalna shuru karo

        // Condition 1: j >= 0 (Boundary safety)
        // Condition 2: arr[j] > key (Strict inequality maintains STABILITY)
        while(j >= 0 && arr[j] > key){
            arr[j + 1] = arr[j]; // Right shift
            --j;
        }

        // Drop the key into the correct opening
        arr[j + 1] = key;
    }
}
```

---

<a id="42-descending-order-insertion-sort"></a>

### 4.2 Descending Order Insertion Sort
Descending order me sabse bada element sabse aage aana chahiye:
- Change: `arr[j] < key` check karo. Jo elements key se chote hain, unhe right shift karo!

```cpp
#include <vector>
using namespace std;

void insertionSortDescending(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    for(int i = 1; i < n; ++i){
        int key = arr[i];
        int j = i - 1;

        // Shift elements that are smaller than key to the right
        while(j >= 0 && arr[j] < key){
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}
```

---

<a id="43-binary-insertion-sort"></a>

### 4.3 Binary Insertion Sort (Binary Search Optimization)

Kyunki left prefix `arr[0...i-1]` already sorted hota hai, hum linear scan ke bajaye **Binary Search** (`std::upper_bound`) use karke key ki correct insertion position $O(\log i)$ time me dhoond sakte hain!

```cpp
#include <vector>
#include <algorithm>
using namespace std;

void binaryInsertionSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    for(int i = 1; i < n; ++i){
        int key = arr[i];

        // upper_bound returns iterator to first element strictly greater than key
        // (upper_bound preserves STABILITY!)
        auto it = upper_bound(arr.begin(), arr.begin() + i, key);
        int pos = it - arr.begin();

        // Elements ko pos se i-1 tak right shift karna padega
        for(int j = i - 1; j >= pos; --j){
            arr[j + 1] = arr[j];
        }

        arr[pos] = key;
    }
}
```

> **Interview Insight:**  
> Binary Insertion Sort comparisons ko $O(n^2)$ se gira kar **$O(n \log n)$** kar deta hai, **LEKIN overall time complexity tab bhi $O(n^2)$ hi rehti hai** kyunki array elements ko physically right shift karne me abhi bhi worst case me $O(n^2)$ data movements lagte hain!

---

<a id="44-linked-list-insertion-sort"></a>

### 4.4 Linked List Insertion Sort (LeetCode 147 Style)

Linked list me array jaisa shifting ka issue nahi hota! Hum sirf pointer nodes ko detach aur re-attach kar sakte hain:

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* insertionSortList(ListNode* head){
    if(!head || !head->next) return head;

    // Dummy node points to the sorted linked list
    ListNode dummy(0);
    ListNode* curr = head;

    while(curr != nullptr){
        ListNode* nextNode = curr->next; // Save next pointer

        // Find insertion point in sorted sublist starting from dummy
        ListNode* prev = &dummy;
        while(prev->next != nullptr && prev->next->val < curr->val){
            prev = prev->next;
        }

        // Insert curr node between prev and prev->next
        curr->next = prev->next;
        prev->next = curr;

        curr = nextNode; // Move to next node in original list
    }

    return dummy.next;
}
```

---

<a id="5-time--space-complexity-analysis"></a>

## 5. Time & Space Complexity Analysis

<a id="51-the-inversion-relation-thetan--i"></a>

### 5.1 The Inversion Relation: $\Theta(n + I)$

Insertion Sort ki sabse khoobsurat mathematical property ye hai ki iska running time **array ke inversion count ($I$) ke directly proportional** hota hai:

$$\text{Running Time} = \Theta(n + I)$$

- Har single shift operation array me se exact **1 inversion** ko remove karta hai.
- Agar array nearly-sorted hai (jaise sirf 2-3 elements misplaced hain, $I = O(n)$), to Insertion Sort **pure linear time $O(n)$** me execute ho jata hai!

---

<a id="52-best-average-and-worst-case"></a>

### 5.2 Best, Average, and Worst Case

| Case | Input Condition | Comparisons | Shifts | Time Complexity |
|---|---|:---:|:---:|:---:|
| **Best Case** | Already Sorted `[1, 2, 3, 4, 5]` | $n - 1$ | $0$ | **$\Theta(n)$** |
| **Worst Case** | Reverse Sorted `[5, 4, 3, 2, 1]` | $\frac{n(n-1)}{2}$ | $\frac{n(n-1)}{2}$ | **$\Theta(n^2)$** |
| **Average Case** | Random Permutation | $\approx \frac{n(n-1)}{4}$ | $\approx \frac{n(n-1)}{4}$ | **$\Theta(n^2)$** |

---

<a id="53-space--in-place-nature"></a>

### 5.3 Space & In-Place Nature
- **Auxiliary Space:** **$O(1)$** (Strictly in-place).
- Sirf do variables use hote hain: `key` aur loop pointer `j`.

---

<a id="6-stability-proof"></a>

## 6. Stability Proof: Duplicate Elements Safe Kyu Rehte Hain?

> **Rule:**  
> Ek sorting algorithm stable tab hota hai jab duplicate keys ka relative order sort hone ke baad bhi intact rahe.

Insertion Sort me stability loop condition se aati hai:
```cpp
while(j >= 0 && arr[j] > key) // STRICT GREATER THAN!
```

### Trace:
Maan lo array hai `[... 5a ... 5b ...]`:
1. `5a` pehle se sorted prefix me baitha hua hai.
2. Jab `5b` ko `key` banakar check karte hain:
   - Loop dekhta hai: `arr[j] > key` $\implies 5_a > 5_b$.
   - Kyunki $5_a$ strictly greater nahi hai, condition **`false`** ho jati hai!
   - Shifting turant ruk jati hai.
   - `5b` ko $5_a$ ke theek aage `arr[j + 1]` par place kar diya jata hai.
3. Isliye $5_a$ hamesha $5_b$ ke pehle hi rehta hai $\implies$ **100% STABLE!**

> [!CAUTION]
> Agar kisi programmer ne galti se `arr[j] >= key` likh diya, to $5_a$ bhi right shift ho jayega aur $5_b$ uske peeche chala jayega, jisse algorithm **Unstable** ban jayega!

---

<a id="7-comprehensive-doubts-log"></a>

## 7. Comprehensive Doubts Log (Top 8 Conceptual Confusions)

---

### Doubt 01: Selection Sort vs Insertion Sort me fundamental farq kya hai? Dono me to sorted prefix banta hai!
**Answer:**  
Dono algorithms me shuruat me ek sorted prefix banta hai, lekin dono ke prefix ke nature me zameen-aasman ka farq hai:
- **Selection Sort ka prefix FINAL hota hai:**  
  Selection Sort har pass me bache hue array ka absolute minimum uthata hai. Index `0` par jo element ek baar baith gaya, wo poori life me dobara kabhi move nahi karega!
- **Insertion Sort ka prefix TENTATIVE (temporary) hota hai:**  
  Insertion Sort sirf ab tak dekhe gaye elements ko sort karta hai. Agar pass 3 me `[3, 5, 7]` sorted hai, aur pass 4 me naya card `1` aa gaya, to `3, 5, 7` teeno ko right shift hona padega! Yani Insertion Sort ka sorted prefix baad ke smaller elements ki wajah se aage khisak sakta hai.

---

### Doubt 02: `while (j >= 0 && arr[j] > key)` me `j >= 0` pehle kyu likhte hain? Kya order matter karta hai?
**Answer:**  
**Haan, order 100% matter karta hai!**  
C++ me `&&` operator **Short-Circuit Evaluation** use karta hai:
- Agar pehli condition `false` ho jaye, to C++ doosri condition ko evaluate hi nahi karta.
- Agar tum likhoge: `while (arr[j] > key && j >= 0)`:
  Jab element sabse chota hoga aur shift hote-hote $j = -1$ ho jayega, to loop sabse pehle `arr[-1]` ko access karne ki koshish karega!
- `arr[-1]` memory out-of-bounds access hai, jisse **Segmentation Fault (Crash)** ya Undefined Behavior ho sakta hai.  
Isliye `j >= 0` ko pehle likhna compulsory hai taaki $j = -1$ hote hi check ruk jaye!

---

### Doubt 03: Outer loop index `1` se kyu shuru hota hai? Index `0` se kyu nahi?
**Answer:**  
Kyunki ek single element ka array (subarray of size 1) mathematically pehle se hi sorted hota hai.  
Index 0 par koi peeche wala element nahi hota jisse use compare kiya ja sake. Isliye hum pehla naya card hamesha index `1` se uthate hain aur use index `0` ke element ke sath compare karte hain.

---

### Doubt 04: C++ `std::sort` aur Python `Timsort` $O(n^2)$ wale Insertion Sort ko kyu use karte hain?
**Answer:**  
Iske 3 zabardast real-world hardware reasons hain:
1. **Zero Recursion & Function Call Overhead:** QuickSort aur MergeSort me recursion stack frames allocate karne padte hain. Insertion Sort me sirf 2 simple integer variables hote hain.
2. **CPU Cache Locality:** Insertion Sort memory ko sequentially scan karta hai. CPU L1/L2 cache prefetching ispar 100% efficient hoti hai.
3. **Threshold for Small Arrays:** $n \le 16$ ya $32$ ke liye $c_1 n^2$ ka constant factor QuickSort ke $c_2 n \log n$ se chota hota hai!  
Isliye production introsort partitions chota hote hi Insertion Sort par switch kar jata hai.

---

### Doubt 05: Kya Binary Search use karke Insertion Sort ko $O(n \log n)$ banaya ja sakta hai?
**Answer:**  
**Nahi!**  
Binary Search se hum insertion position dhoondne ke comparisons ko $O(\log n)$ me reduce kar sakte hain, to total comparisons $O(n \log n)$ ho jayenge.  
Lekin array ek contiguous memory block hota hai. Element ko insert karne ke liye right side ke elements ko physically khiskana (shift karna) padega, jisme abhi bhi worst case me $\frac{n(n-1)}{2}$ shifts lagenge. Isliye total time tab bhi **$O(n^2)$** hi rahega.

---

### Doubt 06: Selection Sort aur Bubble Sort ke mukable Insertion Sort nearly-sorted array par kyu jeet-ta hai?
**Answer:**  
- **Selection Sort** non-adaptive hai: array chahe fully sorted ho, wo tab bhi $\frac{n(n-1)}{2}$ comparisons karega ($\Theta(n^2)$).
- **Bubble Sort** optimized flag ke sath $O(n)$ me terminate ho sakta hai, lekin pass ke dauraan adjacent swaps karta hai (3 operations per swap).
- **Insertion Sort** har inversion par sirf 1 assignment (shift) karta hai, aur sorted part me break hote hi agle pass par chala jata hai. Practically nearly-sorted data par Insertion Sort sabse fastest elementary sort hota hai.

---

### Doubt 07: Singly Linked List par Insertion Sort arrays se better kyu hota hai?
**Answer:**  
Array me insertion karte waqt elements ko physically right shift karna padta hai ($O(n)$ memory writes).  
Linked List me sirf node ke `next` pointer ko re-link karna padta hai ($O(1)$ pointer operations). Halanki position dhoondne ke liye list me linear traversal hi lagta hai, lekin data shifting ka overhead completely zero ho jata hai.

---

### Doubt 08: Kya Insertion Sort ko Online Algorithm kaha ja sakta hai?
**Answer:**  
**Haan! Insertion Sort ek true Online Sorting Algorithm hai.**  
"Online algorithm" ka matlab hota hai ki array ka saara data shuruat me ek sath available hona zaroori nahi hai. Agar data stream se ek-ek number real-time me aa raha ho, to Insertion Sort naye aane wale number ko already sorted existing elements ke beech me turant insert kar sakta hai bina pure array ko dobara sort kiye!

---

<a id="8-quick-summary--interview-recall-sheet"></a>

## 8. Quick Summary & Interview Recall Sheet

```text
┌────────────────────────────────────────────────────────────────────────┐
│                      INSERTION SORT CHEAT SHEET                        │
├──────────────────────┬─────────────────────────────────────────────────┤
│ Mental Model         │ Playing cards hand me insert karna              │
│ Mechanism            │ Shifting (NOT repeated swapping)                │
│ Best Case Time       │ O(n) — when array is already sorted             │
│ Worst Case Time      │ O(n^2) — when array is reverse sorted           │
│ Average Case Time    │ O(n^2)                                          │
│ Auxiliary Space      │ O(1) — strictly in-place                        │
│ Stability            │ STABLE (due to strict 'arr[j] > key' condition) │
│ Adaptability         │ ADAPTIVE (runtime strictly proportional to n+I) │
│ Online Property      │ YES (can sort streaming data as it arrives)     │
│ Ideal For            │ Small arrays (n <= 32) & nearly-sorted data     │
└──────────────────────┴─────────────────────────────────────────────────┘
```

### 3 Golden Rules for Coding Insertion Sort:
1. Outer loop $i = 1$ se shuru karo (`key = arr[i]`).
2. Inner while loop me `j >= 0` pehle likho taaki short-circuit evaluation out-of-bounds access roke.
3. Strict inequality `arr[j] > key` use karo taaki stability preserve rahe!

---
*Created as part of the Unified Notes Learning System.*
