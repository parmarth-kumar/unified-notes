# Merge Sort — Ultimate Deep Dive Guide

> **Module:** `unified-notes/DSA/Sorting/MERGE_SORT_DEEP_DIVE.md`  
> **Language:** Hinglish (Strictly Roman English Script — No Devanagari characters)  
> **Topic:** Merge Sort (Divide & Conquer, 2-Pointer Linear Merge, Stability Mechanics, Buffer Optimization, & Deep Doubts)  
> **Goal:** Merge Sort ke mathematical recurrence, recursion trees, memory allocations, linked list sorting, aur interview problems ko deeply master karna.

---

## Table of Contents

- [1. Real-World Intuition: Divide & Conquer aur Two-Pointer Zipper](#1-real-world-intuition)
- [2. The 3 Phases of Merge Sort: Divide, Conquer, Combine](#2-the-3-phases-of-merge-sort)
- [3. Step-by-Step Visual Dry Run (Full Recursion Tree & Linear Merge)](#3-step-by-step-visual-dry-run)
  - [3.1 Full Recursion Tree Walkthrough](#31-full-recursion-tree-walkthrough)
  - [3.2 2-Pointer Linear Merge Procedure (Detailed Pointer Movement)](#32-2-pointer-linear-merge-procedure)
- [4. Complete C++ Implementations & Production Optimizations](#4-complete-c-implementations)
  - [4.1 Standard Recursive Merge Sort with Single Upfront Buffer Allocation](#41-standard-recursive-merge-sort)
  - [4.2 The Boundary-Skip Optimization ($O(n)$ Best Case)](#42-the-boundary-skip-optimization)
  - [4.3 Iterative (Bottom-Up) Merge Sort (Non-Recursive)](#43-iterative-bottom-up-merge-sort)
  - [4.4 Linked List Merge Sort ($O(1)$ Auxiliary Buffer Space)](#44-linked-list-merge-sort)
  - [4.5 Counting Inversions in $O(n \log n)$ via Merge Sort](#45-counting-inversions-via-merge-sort)
- [5. Time & Space Complexity Proofs](#5-time--space-complexity-analysis)
  - [5.1 Recursion Tree Method Proof: Why $O(n \log n)$ Always?](#51-recursion-tree-proof)
  - [5.2 Auxiliary Space Breakdown: Buffer vs Call Stack](#52-auxiliary-space-breakdown)
- [6. Stability Proof: `<= vs <` ka Khel](#6-stability-proof)
- [7. Comprehensive Doubts Log (Top 8 Conceptual Confusions)](#7-comprehensive-doubts-log)
- [8. Quick Summary & Interview Recall Sheet](#8-quick-summary--interview-recall-sheet)

---

<a id="1-real-world-intuition"></a>

## 1. Real-World Intuition: Divide & Conquer aur Two-Pointer Zipper

Socho ek teacher ke paas 500 exam answer sheets aayi hain jinhe Roll Number wise sort karna hai:
- Agar akela teacher 500 sheets ko ek-ek karke sort karega, to bohot time lagega.
- Teacher ne smart strategy lagayi:
  1. Usne 500 sheets ko 250-250 ke do groups me baant diya.
  2. Apne do assistants ko bola: *"Tum dono apne-apne 250 sheets ko sort kar lao."*
  3. Jab dono assistants apne-apne piles sort karke le aaye, to teacher ne dono piles ko table par aamne-saamne rakha.
  4. Teacher ne dono piles ke top card ko dekha, jo chota tha use naye pile me rakh diya.
  5. Ye kaam ek **Zipper (Chain)** ki tarah chalta gaya aur kuch hi minutes me saari 500 sheets sort ho gayi!

> **Core Philosophy:**  
> Badi problem ko do barabar aadhi problems me todo (**Divide**).  
> Dono choti problems ko recursively solve karo (**Conquer**).  
> Fir do sorted tukdon ko 2 pointers se ek linear scan me jod do (**Combine / Merge**).

---

<a id="2-the-3-phases-of-merge-sort"></a>

## 2. The 3 Phases of Merge Sort: Divide, Conquer, Combine

Har recursive step par Merge Sort 3 cheezein karta hai:

```text
                             [ 8 , 4 , 5 , 7 , 1 , 3 , 6 , 2 ]
                                            │
                          1. DIVIDE (Cut down the middle)
                                            ▼
                  [ 8 , 4 , 5 , 7 ]                   [ 1 , 3 , 6 , 2 ]
                          │                                   │
                    2. CONQUER (Recursively sort both smaller halves)
                          ▼                                   ▼
                  [ 4 , 5 , 7 , 8 ]                   [ 1 , 2 , 3 , 6 ]
                          │                                   │
                    3. COMBINE (2-Pointer Linear Merge in O(n))
                          └─────────────────┬─────────────────┘
                                            ▼
                             [ 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 ]
```

### The Safe Midpoint Formula:
Beginners galti se likhte hain: `int mid = (low + high) / 2;`  
Agar array bohot bada ho (jaise $2 \times 10^9$ elements), to `low + high` signed 32-bit integer limit cross karke **Negative Number** ban jata hai, jisse program crash ho jata hai!  
**Always write:**
```cpp
int mid = low + (high - low) / 2;
```
Ye mathematically identical hai par kabhi overflow nahi hota!

---

<a id="3-step-by-step-visual-dry-run"></a>

## 3. Step-by-Step Visual Dry Run (Full Recursion Tree & Linear Merge)

<a id="31-full-recursion-tree-walkthrough"></a>

### 3.1 Full Recursion Tree Walkthrough
Maan lo input array hai: `arr = [ 38 , 27 , 43 , 3 , 9 , 82 , 10 ]` ($n = 7$)

```text
Level 0:                         [ 38, 27, 43, 3, 9, 82, 10 ]
                                         /          \
Level 1:                   [ 38, 27, 43, 3 ]      [ 9, 82, 10 ]
                              /        \             /      \
Level 2:                 [ 38, 27 ]   [ 43, 3 ]   [ 9, 82 ]  [ 10 ]
                          /    \       /    \      /    \       │
Level 3 (Base Case):   [38]   [27]   [43]   [3]  [9]   [82]   [10]
```

Ab neeche se upar merging shuru hoti hai:
- `[38]` aur `[27]` merge hoke banate hain: `[27, 38]`
- `[43]` aur `[3]` merge hoke banate hain: `[3, 43]`
- `[9]` aur `[82]` merge hoke banate hain: `[9, 82]`
- `[10]` already single hai: `[10]`

Agla Level:
- `[27, 38]` aur `[3, 43]` merge hoke banate hain: `[3, 27, 38, 43]`
- `[9, 82]` aur `[10]` merge hoke banate hain: `[9, 10, 82]`

Final Top Level:
- `[3, 27, 38, 43]` aur `[9, 10, 82]` merge hoke banate hain:  
  **`[3, 9, 10, 27, 38, 43, 82]`** $\implies$ Perfectly Sorted!

---

<a id="32-2-pointer-linear-merge-procedure"></a>

### 3.2 2-Pointer Linear Merge Procedure (Detailed Pointer Movement)
Chalo step-by-step dekhte hain ki do sorted halves kaise merge hote hain:  
- **Left Half:** `[ 27 , 38 ]` (indices $i = 0$ to $1$)  
- **Right Half:** `[ 3 , 43 ]` (indices $j = 2$ to $3$)  
- **Temporary Output Buffer:** `temp` (pointer $k$)

```text
Initial:
Left:   [ 27 , 38 ]       Right:  [ 3 , 43 ]       temp: [ _ , _ , _ , _ ]
          ▲                         ▲                      ▲
          i                         j                      k

Step 1:
Compare arr[i] (27) aur arr[j] (3):
3 is smaller!
temp[k++] = arr[j++];
Left:   [ 27 , 38 ]       Right:  [ 3 , 43 ]       temp: [ 3 , _ , _ , _ ]
          ▲                             ▲                      ▲
          i                             j                      k

Step 2:
Compare arr[i] (27) aur arr[j] (43):
27 is smaller!
temp[k++] = arr[i++];
Left:   [ 27 , 38 ]       Right:  [ 3 , 43 ]       temp: [ 3 , 27 , _ , _ ]
               ▲                        ▲                           ▲
               i                        j                           k

Step 3:
Compare arr[i] (38) aur arr[j] (43):
38 is smaller!
temp[k++] = arr[i++];
Left:   [ 27 , 38 ]       Right:  [ 3 , 43 ]       temp: [ 3 , 27 , 38 , _ ]
                    ▲                   ▲                                ▲
                    i                   j                                k

Step 4 (Left half is exhausted!):
Left half khatam ho gaya (i > mid).
Ab Right half me jo kuch bacha hai (43), use direct copy kar do:
temp[k++] = arr[j++];
temp ban gaya: [ 3 , 27 , 38 , 43 ]

Step 5 (Copy Back):
temp se wapas original array arr me copy kar do. Done!
```

---

<a id="4-complete-c-implementations"></a>

## 4. Complete C++ Implementations & Production Optimizations

<a id="41-standard-recursive-merge-sort"></a>

### 4.1 Standard Recursive Merge Sort with Single Upfront Buffer Allocation

> [!IMPORTANT]
> **Performance Tip:**  
> Beginners har recursive call ke andar `vector<int> temp(high - low + 1)` allocate karte hain. Isse $O(n \log n)$ baar dynamic heap allocation hoti hai jo code ko 3x slow kar deti hai.  
> **Best Practice:** Main function me ek hi baar `vector<int> temp(n)` allocate karo aur reference se pass karo!

```cpp
#include <vector>
using namespace std;

void merge(vector<int>& arr, int low, int mid, int high, vector<int>& temp){
    int i = low;     // Pointer for left sorted half
    int j = mid + 1; // Pointer for right sorted half
    int k = low;     // Pointer for temporary buffer

    while(i <= mid && j <= high){
        // Strict '<=' guarantees STABILITY (ties pick left half first!)
        if(arr[i] <= arr[j]){
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    // Copy remaining elements from left half (if any)
    while(i <= mid){
        temp[k++] = arr[i++];
    }

    // Copy remaining elements from right half (if any)
    while(j <= high){
        temp[k++] = arr[j++];
    }

    // Copy merged result back into original array
    for(int p = low; p <= high; ++p){
        arr[p] = temp[p];
    }
}

void mergeSortInternal(vector<int>& arr, int low, int high, vector<int>& temp){
    if(low >= high){
        return; // Base case: 0 or 1 element is trivially sorted
    }

    int mid = low + (high - low) / 2;

    mergeSortInternal(arr, low, mid, temp);      // Sort left half
    mergeSortInternal(arr, mid + 1, high, temp); // Sort right half

    merge(arr, low, mid, high, temp);            // Merge both halves
}

void mergeSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    // Single upfront allocation to avoid repetitive dynamic heap allocations
    vector<int> temp(n);
    mergeSortInternal(arr, 0, n - 1, temp);
}
```

---

<a id="42-the-boundary-skip-optimization"></a>

### 4.2 The Boundary-Skip Optimization ($O(n)$ Best Case)

Standard Merge Sort already-sorted array par bhi $O(n \log n)$ time leta hai kyunki wo har sub-array ko merge karne lagta hai.  
Lekin agar Left half ka sabse bada element (`arr[mid]`) Right half ke sabse chote element (`arr[mid+1]`) se chota ya barabar ho:
$$\text{arr}[mid] \le \text{arr}[mid + 1]$$
Iska matlab dono halves already perfectly arranged hain! Unhe merge karne ki bilkul zaroorat nahi hai!

```cpp
void mergeSortOptimized(vector<int>& arr, int low, int high, vector<int>& temp){
    if(low >= high) return;

    int mid = low + (high - low) / 2;

    mergeSortOptimized(arr, low, mid, temp);
    mergeSortOptimized(arr, mid + 1, high, temp);

    // ✨ Boundary Skip Optimization:
    // Agar left ka last element <= right ka first element hai, to merge skip kar do!
    if(arr[mid] <= arr[mid + 1]){
        return;
    }

    merge(arr, low, mid, high, temp);
}
```
Is simple 2-line check se already sorted array par Merge Sort ka Best Case time **$O(n)$** ban jata hai!

---

<a id="43-iterative-bottom-up-merge-sort"></a>

### 4.3 Iterative (Bottom-Up) Merge Sort (Non-Recursive)

Recursion use kiye bina hum bottom-up tree simulate kar sakte hain:
- Shuru me 1-1 element ke pairs merge karo ($size = 1$).
- Fir 2-2 elements ke pairs merge karo ($size = 2$).
- Fir 4-4 ($size = 4$), aur aise double karte jao jab tak $size \ge n$ na ho jaye!
- **Fayda:** Zero recursion call stack overhead ($O(1)$ stack memory)!

```cpp
#include <vector>
#include <algorithm>
using namespace std;

void mergeSortIterative(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    vector<int> temp(n);

    // Subarray size: 1, 2, 4, 8, 16...
    for(int sz = 1; sz < n; sz *= 2){
        for(int low = 0; low < n - sz; low += 2 * sz){
            int mid = low + sz - 1;
            int high = min(low + 2 * sz - 1, n - 1);
            merge(arr, low, mid, high, temp);
        }
    }
}
```

---

<a id="44-linked-list-merge-sort"></a>

### 4.4 Linked List Merge Sort ($O(1)$ Auxiliary Buffer Space)

LeetCode 148 (Sort List): Linked List par Merge Sort kyu sabse best sorting algorithm hai?  
1. Midpoint dhoondne ke liye **Slow & Fast Pointers** use hote hain.
2. Merge step me **koi auxiliary array buffer allocate nahi karna padta**! Hum direct pointers re-link karte hain ($O(1)$ space).

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Merges two sorted linked lists using dummy head
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2){
    ListNode dummy(0);
    ListNode* tail = &dummy;

    while(l1 && l2){
        if(l1->val <= l2->val){
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    tail->next = (l1 ? l1 : l2);
    return dummy.next;
}

ListNode* sortList(ListNode* head){
    if(!head || !head->next) return head;

    // Slow and Fast pointer technique to find middle
    ListNode* slow = head;
    ListNode* fast = head->next;
    while(fast && fast->next){
        slow = slow->next;
        fast = fast->next->next;
    }

    ListNode* rightHead = slow->next;
    slow->next = nullptr; // Cut list into two halves

    ListNode* leftSorted = sortList(head);
    ListNode* rightSorted = sortList(rightHead);

    return mergeTwoLists(leftSorted, rightSorted);
}
```

---

<a id="45-counting-inversions-via-merge-sort"></a>

### 4.5 Counting Inversions in $O(n \log n)$ via Merge Sort

Inversion ka matlab hota hai $i < j$ aur $arr[i] > arr[j]$.  
Merge step me jab Right half ka element Left half ke element se chota hota hai (`arr[j] < arr[i]`), to Left half ke current index $i$ se lekar $mid$ tak ke **saare elements** $arr[j]$ se bade hote hain!  
Isliye hum ek hi step me add kar dete hain:
$$\text{inversions} += (mid - i + 1)$$

```cpp
long long mergeAndCount(vector<int>& arr, int low, int mid, int high, vector<int>& temp){
    int i = low, j = mid + 1, k = low;
    long long invCount = 0;

    while(i <= mid && j <= high){
        if(arr[i] <= arr[j]){
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            // arr[i...mid] saare bache hue elements arr[j] se bade hain!
            invCount += (mid - i + 1);
        }
    }
    while(i <= mid) temp[k++] = arr[i++];
    while(j <= high) temp[k++] = arr[j++];
    for(int p = low; p <= high; ++p) arr[p] = temp[p];

    return invCount;
}

long long countInversionsRec(vector<int>& arr, int low, int high, vector<int>& temp){
    long long count = 0;
    if(low < high){
        int mid = low + (high - low) / 2;
        count += countInversionsRec(arr, low, mid, temp);
        count += countInversionsRec(arr, mid + 1, high, temp);
        count += mergeAndCount(arr, low, mid, high, temp);
    }
    return count;
}
```

---

<a id="5-time--space-complexity-analysis"></a>

## 5. Time & Space Complexity Analysis

<a id="51-recursion-tree-proof"></a>

### 5.1 Recursion Tree Method Proof: Why $O(n \log n)$ Always?

Merge Sort ki recurrence relation hoti hai:
$$T(n) = 2T(n/2) + O(n)$$

Chalo ise Recursion Tree se visually dekhein:

```text
Level 0:                         T(n)                       Work = n
                               /      \
Level 1:                 T(n/2)        T(n/2)               Work = n/2 + n/2 = n
                         /    \        /    \
Level 2:             T(n/4)  T(n/4)  T(n/4)  T(n/4)         Work = 4 * (n/4) = n
                       ...     ...     ...     ...
Level k:               T(1)    T(1)    ...    T(1)          Work = n * 1 = n
```

1. **Har Level par Work:** Har level par chahe kitne bhi tukde bane hon, unke saare elements merge step me ek baar process hote hain $\implies$ Work per level = **$n$**.
2. **Total Levels (Height of Tree):** Har step par array size half ho raha hai:
   $$n \to \frac{n}{2} \to \frac{n}{4} \dots \to 1 \implies \text{Total Levels} = \log_2 n$$
3. **Total Work:**
   $$\text{Total Work} = \text{Levels} \times \text{Work per level} = n \times \log_2 n = \mathbf{O(n \log n)}$$

Ye guarantee **Best Case, Average Case, aur Worst Case** sabhi inputs ke liye valid hai!

---

<a id="52-auxiliary-space-breakdown"></a>

### 5.2 Auxiliary Space Breakdown: Buffer vs Call Stack
Merge Sort ki total auxiliary space hoti hai:
$$\text{Auxiliary Space} = O(n) + O(\log n) = \mathbf{O(n)}$$
- **Temporary Buffer:** $O(n)$ space merge step me elements ko copy karne ke liye chahiye hoti hai.
- **Recursion Call Stack:** Binary tree ki height $\log_2 n$ hoti hai, to call stack maximum $O(\log n)$ memory frames hold karta hai.

---

<a id="6-stability-proof"></a>

## 6. Stability Proof: `<= vs <` ka Khel

Merge Sort ki stability ka raaz is line me chhupa hai:
```cpp
if (arr[i] <= arr[j]) {
    temp[k++] = arr[i++];
} else {
    temp[k++] = arr[j++];
}
```

### Trace:
Maan lo Left half me hai `5a` aur Right half me hai `5b`:
- Dono values equal hain (`5a == 5b`).
- Kyunki condition `<= (less than or equal to)` hai, to check evaluate hota hai **`true`**!
- Iska matlab **Left half wala element (`5a`) pehle output me place hota hai**!
- `5b` uske baad place hota hai.
- Is wajah se jo element input me pehle tha wo output me bhi pehle rehta hai $\implies$ **Guaranteed 100% Stability!**

> [!CAUTION]
> Agar galti se `arr[i] < arr[j]` likh diya (bina equals ke), to equal values aane par `else` branch chal jayega aur right half wala `5b` pehle copy ho jayega, jisse stability toot jayegi!

---

<a id="7-comprehensive-doubts-log"></a>

## 7. Comprehensive Doubts Log (Top 8 Conceptual Confusions)

---

### Doubt 01: QuickSort ke mukable MergeSort arrays par slow kyu feel hota hai jabki dono $O(n \log n)$ hain?
**Answer:**  
Asymptotic complexity same hone ke bawajood hardware level par QuickSort faster hota hai:
1. **Memory Bandwidth & Copying:** Merge Sort har pass me elements ko temporary array me copy karta hai, aur fir wapas original array me copy karta hai (double memory writes). QuickSort elements ko in-place direct swap karta hai.
2. **CPU Cache Locality:** QuickSort partitioning me CPU cache line prefetching bohot high hoti hai kyunki wo continuous block ko read/write karta hai. Merge Sort ka auxiliary buffer cache pollution karta hai.

---

### Doubt 02: Linked Lists par Merge Sort QuickSort se kyu jeet-ta hai?
**Answer:**  
Iske do bohot bade reasons hain:
1. **$O(1)$ Extra Space:** Linked list me merging ke waqt kisi naye buffer array ki zaroorat nahi padti. Hum sirf node ke `next` pointer ko re-wire karte hain.
2. **Sequential Access:** Linked lists me random indexing (`arr[i]`) impossible hoti hai ($O(n)$ traverse karna padta hai). QuickSort pivot placement aur partition ke liye random access chahta hai, jabki Merge Sort purely sequential access chahta hai jo linked list ke liye 100% natural hai.

---

### Doubt 03: `temp` vector ko `merge` ke andar allocate kyu nahi karna chahiye?
**Answer:**  
Agar tum `merge` function ke andar `vector<int> temp(high - low + 1)` likhte ho:
- Har recursive call me OS dynamic memory allocate karta hai (`malloc` / `new`).
- $n$ elements ke liye total $n - 1$ merges hote hain.
- $O(n)$ alag-alag heap allocations aur deallocations CPU memory allocator par massive overhead dalti hain.
- Main function me ek hi baar `vector<int> temp(n)` allocate karke reuse karne se ye overhead completely eliminate ho jata hai.

---

### Doubt 04: Kya Merge Sort ko arrays me $O(1)$ auxiliary space me implement kiya ja sakta hai?
**Answer:**  
Theoretically haan ("In-Place Merge Sort"), lekin practically ye inefficient hota hai:
- Simple in-place merge karne ke liye elements ko right-shift karna padta hai, jisse merge step $O(n)$ se badhkar $O(n^2)$ ho jata hai, aur poora algorithm $O(n^2)$ ban jata hai!
- Advanced $O(n \log n)$ In-Place algorithms (jaise Block Sort / GrailSort) exist karte hain jo array ke kuch hisse ko internal buffer ki tarah use karte hain, lekin unka implementation itna complex aur constant factor itna high hota hai ki production me koi use nahi karta.

---

### Doubt 05: Inversion count problem me `invCount += (mid - i + 1)` kyu hota hai?
**Answer:**  
Kyunki Left half `arr[low ... mid]` already sorted order me hota hai.  
Agar kisi point par `arr[j] < arr[i]` nikalta hai, to kyunki Left half sorted hai, `arr[i]` ke aage baithe hue saare elements (`arr[i+1], arr[i+2], ... arr[mid]`) bhi guarantee `arr[j]` se bade honge!  
Index $i$ se index $mid$ tak total kitne elements hain? Exactly $\mathbf{mid - i + 1}$ elements. To hume har element ko individually count karne ki zaroorat nahi hai, hum direct formula se saare inversions add kar lete hain!

---

### Doubt 06: External Sorting (10 TB data on 32 GB RAM) me Merge Sort kyu use hota hai?
**Answer:**  
Kyunki 10 TB data RAM me fit nahi ho sakta. Hum disk se 20-20 GB ke chunks read karke sort karte hain aur disk par 500 sorted files (runs) bana lete hain.  
Ab in 500 files ko ek sath merge karne ke liye hum **$k$-Way Merge Sort** use karte hain (ek Min-Heap of size 500 ke through). Sequential streaming read/write disk I/O ke liye sabse fast pattern hota hai.

---

### Doubt 07: Merge Sort ka Best Case $O(n \log n)$ hota hai ya $O(n)$?
**Answer:**  
- **Standard Merge Sort:** Hamesha **$\Theta(n \log n)$** hota hai kyunki wo pehle se sorted array ko bhi poora divide karega aur fir har level par merge loop chalayega.
- **Optimized Merge Sort (with Boundary Skip):** Agar hum check karein `if (arr[mid] <= arr[mid+1]) return;`, to already sorted array par saare merge steps skip ho jate hain aur runtime **$O(n)$** ho jata hai!

---

### Doubt 08: C++ standard library me `std::sort` aur `std::stable_sort` me kya relation hai?
**Answer:**  
- `std::sort`: Ye Introsort use karta hai (QuickSort + HeapSort + InsertionSort). Ye fast hota hai par **Unstable** hota hai.
- `std::stable_sort`: Ye Adaptive **MergeSort** variant (Timsort jaisa) use karta hai. Ye guaranteed **Stable** hota hai aur equal elements ka original arrival order preserve rakhta hai.

---

<a id="8-quick-summary--interview-recall-sheet"></a>

## 8. Quick Summary & Interview Recall Sheet

```text
┌────────────────────────────────────────────────────────────────────────┐
│                        MERGE SORT CHEAT SHEET                          │
├──────────────────────┬─────────────────────────────────────────────────┤
│ Paradigm             │ Divide and Conquer                              │
│ Best Case Time       │ O(n log n) [O(n) with boundary skip check]      │
│ Worst Case Time      │ O(n log n) (Guaranteed!)                        │
│ Average Case Time    │ O(n log n)                                      │
│ Auxiliary Space      │ O(n) buffer + O(log n) call stack               │
│ Stability            │ STABLE (due to '<=' condition in merge)         │
│ Adaptability         │ NON-ADAPTIVE by default (Adaptive with skip)    │
│ Linked Lists         │ Undisputed King (O(1) extra buffer space)       │
│ Recurrence Relation  │ T(n) = 2T(n/2) + O(n)                           │
│ Key Applications     │ External Sorting, Inversion Count, Stable Sort  │
└──────────────────────┴─────────────────────────────────────────────────┘
```

---
*Created as part of the Unified Notes Learning System.*
