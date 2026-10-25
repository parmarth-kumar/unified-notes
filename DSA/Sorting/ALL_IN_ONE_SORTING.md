# All-In-One Sorting Master Guide (Complete DSA & Interview Notes)

> **Module:** `unified-notes/DSA/Sorting/ALL_IN_ONE_SORTING.md`
> **Language:** Hinglish (Strictly Roman English script — No Devanagari characters used)
> **Target Audience:** First-year students to Google SDE I / II / Senior interview candidates.
> **How to use this guide:**
> Ye complete master guide hai. Isme basic intuition se lekar advanced mathematical proofs, full compilable C++ code, dry runs, 14 major conceptual doubts, LeetCode classic problems, C++ STL mastery, aur 15-minute quick revision sheet sab kuch ek single file me consolidated hai.

---

## Table of Contents

- [Module 0: Core Foundation &amp; Mathematics (Sabse Pehle Ye Samjho)](#module-0-core-foundation--mathematics)
  - [0.1 Sorting kya hoti hai aur kyu zaroori hai?](#01-sorting-kya-hoti-hai-aur-kyu-zaroori-hai)
  - [0.2 The 5 Core Questions: Har Sorting Algorithm se ye 5 sawaal poocho!](#02-the-5-core-questions)
  - [0.3 Friendly Math Guide: Big-O, Theta, Omega intuitively samjho](#03-friendly-math-guide)
  - [0.4 Checking whether an array is sorted (`isSortedAscending`)](#04-checking-whether-an-array-is-sorted)
  - [0.5 Inversions kya hote hain aur sorting se unka kya connection hai?](#05-inversions-kya-hote-hain)
- [Part 1: Course-Taught Elementary Sorts (Level 1 Foundation)](#part-1-course-taught-elementary-sorts)
  - [1. Bubble Sort (Paani ke bubble jaisa concept)](#1-bubble-sort)
  - [2. Selection Sort (Minimum dhoondo aur front pe rakho)](#2-selection-sort)
  - [3. Insertion Sort (Taash / Playing cards haath me lagana)](#3-insertion-sort)
  - [4. Counting Sort (Non-comparison frequency sorting)](#4-counting-sort)
- [Part 2: Fast &amp; Divide-and-Conquer Sorting Algorithms (Level 1 &amp; 2 Core)](#part-2-fast--divide-and-conquer-sorts)
  - [5. Merge Sort (Divide, Conquer &amp; Linear Merge)](#5-merge-sort)
  - [6. Quick Sort (Lomuto, Hoare, Bounded Stack, aur 3-Way Partition)](#6-quick-sort)
  - [7. Heap Sort (Max-Heap &amp; The $O(n)$ Build-Heap Insight)](#7-heap-sort)
- [Part 3: Non-Comparison &amp; Advanced Sorting Algorithms (Level 2 Deep Dive)](#part-3-non-comparison--advanced-sorts)
  - [8. Radix Sort (Postal Machine LSD Digit Sorting)](#8-radix-sort)
  - [9. Bucket Sort (Uniform Distribution &amp; Scatter-Gather)](#9-bucket-sort)
  - [10. Shell Sort (Diminishing Gap Sort &amp; Knuth Sequence)](#10-shell-sort)
- [Part 4: Practical C++ STL Sorting Mastery (Interview Goldmine)](#part-4-practical-c-stl-sorting-mastery)
  - [11. C++ STL Tools: `sort`, `stable_sort`, `partial_sort`, `nth_element`](#11-c-stl-tools)
  - [12. Custom Comparators, Lambdas, &amp; The Fatal `<=` Bug (Strict Weak Ordering)](#12-custom-comparators--strict-weak-ordering)
- [Part 5: Theoretical Limits &amp; System-Level Real-World Sorting](#part-5-theoretical-limits--system-level-sorting)
  - [13. Comparison Sorting Lower Bound: $\Omega(n \log n)$ Proof (20 Questions Game)](#13-comparison-sorting-lower-bound)
  - [14. Real-World Systems: External Sort (10 TB on 32 GB RAM), Linked Lists, &amp; $k$-Sorted Arrays](#14-real-world-systems)
- [Part 6: Classic Course &amp; LeetCode Sorting Problems](#part-6-classic-course--leetcode-sorting-problems)
  - [15. Descending Order Sorting &amp; Character Sorting](#15-descending-order-sorting)
  - [16. Sort Colors / Dutch National Flag (LeetCode 75)](#16-sort-colors--dutch-national-flag)
  - [17. Merge Sorted Array into Padded Array (LeetCode 88)](#17-merge-sorted-array-into-padded-array)
  - [18. Next Permutation (LeetCode 31)](#18-next-permutation)
  - [19. Merge Intervals (LeetCode 56)](#19-merge-intervals)
  - [20. Count Inversions in an Array ($O(n \log n)$ Merge Sort Approach)](#20-count-inversions-in-an-array)
  - [21. Kth Largest Element in an Array (QuickSelect / `nth_element`)](#21-kth-largest-element)
- [Part 7: The Master Doubt Log (All 14 Common Doubts Resolved)](#part-7-the-master-doubt-log)
- [Part 8: 15-Minute Revision Sheet &amp; Master Comparison Table](#part-8-15-minute-revision-sheet)
- [Part 9: Interview Spoken Q&amp;A + Practice Challenges &amp; Solutions](#part-9-interview-spoken-qa--practice-challenges)

---

# Module 0: Core Foundation & Mathematics

<a id="01-sorting-kya-hoti-hai-aur-kyu-zaroori-hai"></a>

## 0.1 Sorting kya hoti hai aur kyu zaroori hai?

Socho agar ek dictionary ke saare panno (pages) ko mix karke randomly phenk diya jaye, aur tumhe "Algorithm" shabd ka matlab dhoondna ho. Tumhe page 1 se lekar aakhiri page tak har ek word check karna padega — ise kehte hain **Linear Search ($O(n)$)**.

Lekin kyunki real dictionary **alphabetically sorted** hoti hai, tum direct beech ka panna kholte ho, hazaron irrelevant words ko ek jhatke me skip karte ho, aur seconds me apna word dhoond lete ho — ise kehte hain **Binary Search ($O(\log n)$)**.

> **Definition:**Sorting ka matlab hota hai elements ke collection (numbers, strings, objects) ko ek specific logical sequence me arrange karna:
>
> - **Ascending (Non-decreasing) Order:** Chote se bada (`arr[i] <= arr[i+1]`). Example: `[1, 2, 2, 5]`. Yahan equal numbers allow hote hain.
> - **Descending (Non-increasing) Order:** Bade se chota (`arr[i] >= arr[i+1]`). Example: `[5, 2, 2, 1]`.

### Ek Perfect Sort ki Shart:

Sorting ke baad array me saare original elements intact hone chahiye, including duplicate copies. Agar input tha `[2, 1, 2, 2]` aur output aaya `[1, 2, 2]`, to ye sort galat hai kyunki ek element gayab ho gaya!

### Computer Science me Sorting kyu zaroori hai?

1. **Fast Searching:** Binary search $O(\log n)$ me kaam karta hai, par condition ye hai ki data pehle se sorted hona chahiye.
2. **Duplicate Detection:** Unsorted array me duplicates dhoondne ke liye Hash Set lagta hai ($O(n)$ extra memory). Agar array sorted hai, to saare duplicate elements ek doosre ke bagal me baithe hote hain (`arr[i] == arr[i+1]`), to bina extra space ke $O(1)$ memory me duplicate pakda ja sakta hai.
3. **Data Summarization & Extremes:** Sorted array me minimum hamesha index 0 par aur maximum hamesha index `n-1` par milta hai.

---

<a id="02-the-5-core-questions"></a>

## 0.2 The 5 Core Questions: Har Sorting Algorithm se ye 5 sawaal poocho!

Jab bhi koi interviewer ya exam tumse kisi naye sorting algorithm ke baare me pooche, to bas ye 5 core questions dhyan me rakho:

```text
                                 5 CORE QUESTIONS
                                        │
     ┌──────────────────┬───────────────┼───────────────┬──────────────────┐
     ▼                  ▼               ▼               ▼                  ▼
1. Is it fast?     2. Is it        3. Does it need 4. Does it         5. How does it
   (Time              STABLE?         extra memory?   ADAPT?             decide order?
   Complexity)     (Keep equal     (IN-PLACE vs.   (Faster if         (Comparison vs.
                    order?)         Out-of-Place)   already sorted?)   Non-comparison)
```

### 1. Time Complexity (Kitna time lega?)

Algorithm best, average, aur worst case me kitne operations karta hai?

- Best case: Array pehle se sorted ho to kya jaldi rukta hai?
- Worst case: Reverse sorted array par kitna bura behave karta hai?

### 2. Stability (Equal elements ka order preserve rehta hai?)

Agar do records ka sorting key identical hai, to kya sort hone ke baad bhi unka original relative order maintain rehta hai?

- **Example:** Maan lo student database me list pehle se Name ke basis pe sorted hai. Ab hum use Marks ke basis pe sort karte hain. Agar Rahul (Marks 90) aur Rohan (Marks 90) the, aur pehle Rahul tha fir Rohan, to Marks sort ke baad bhi Rahul Rohan ke aage hi rehna chahiye. Agar haan, to algorithm **Stable** hai. Agar nahi, to **Unstable**.

### 3. Auxiliary Space (In-Place vs Out-of-Place)

- **Strictly In-Place:** Extra memory $O(1)$ ho (input array ke andar hi swaps ho rahe hon, koi alag temporary vector na bane).
- **Note on QuickSort:** QuickSort ko practical terms me in-place kaha jata hai kyunki ye koi auxiliary array nahi banata, lekin theoretically recursion call stack memory leta hai ($O(\log n)$ to $O(n)$).

### 4. Adaptability (Already sorted data pe fayda leta hai?)

Kya algorithm existing order ko recognize karke apna kaam kam kar leta hai?

- Bubble Sort aur Insertion Sort **Adaptive** hote hain — already sorted array milte hi $O(n)$ me terminate ho jate hain.
- Selection Sort **Non-Adaptive** hota hai — array chahe kitna bhi sorted ho, wo har baar pure array ko scan karke minimum dhoondta hai aur $O(n^2)$ time leta hai.

### 5. Comparison-based vs Non-Comparison

- **Comparison-based:** Elements ko aapas me compare karte hain (`a < b` ya `a > b`). (Example: Bubble, Selection, Insertion, Merge, Quick, Heap). Inka theoretical worst-case lower bound $\Omega(n \log n)$ hota hai.
- **Non-Comparison:** Keys ki internal properties (digits, frequencies) use karte hain bina direct comparison ke. (Example: Counting Sort, Radix Sort, Bucket Sort). Ye linear time $O(n)$ achieve kar sakte hain!

---

<a id="03-friendly-math-guide"></a>

## 0.3 Friendly Math Guide: Big-O, Theta, Omega intuitively samjho

- **Big-O ($O$):** Worst-case upper bound ("isse zyada bura time nahi lagega").
- **Big-Omega ($\Omega$):** Lower bound ("kam se kam itna kaam to karna hi padega").
- **Big-Theta ($\Theta$):** Tight bound ("exact growth rate yahi hai").

### Growth Rates ka Hierarchy:

$$
O(1) < O(\log n) < O(n) < O(n \log n) < O(n^2) < O(2^n) < O(n!)
$$

Agar $n = 1,000,000$ (10 lakh numbers):

- $O(n)$ operations $\approx 10^6$ ops $\approx 1$ millisecond.
- $O(n \log_2 n)$ operations $\approx 10^6 \times 20 \approx 2 \times 10^7$ ops $\approx 0.05$ seconds.
- $O(n^2)$ operations $\approx (10^6)^2 = 10^{12}$ ops $\approx 16$ minute se zyada!

Isliye interviews me $O(n^2)$ ko $O(n \log n)$ me optimize karna itna critical hota hai.

---

<a id="04-checking-whether-an-array-is-sorted"></a>

## 0.4 Checking whether an array is sorted (`isSortedAscending`)

Kisi bhi sorting algorithm ko test karne ke liye sabse pehle hume ek reliable checker function chahiye jo bataye ki array ascending order me sorted hai ya nahi:

```cpp
#include <vector>
using namespace std;

// Array ko linearly scan karo. Agar koi bhi pair arr[i] > arr[i+1] mil jaye,
// to iska matlab array sorted nahi hai.
bool isSortedAscending(const vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1){
        return true; // Empty ya 1-element array hamesha sorted hota hai
    }
    for(int i = 0; i < n - 1; ++i){
        if(arr[i] > arr[i + 1]){
            return false;
        }
    }
    return true;
}
```

---

<a id="05-inversions-kya-hote-hain"></a>

## 0.5 Inversions kya hote hain aur sorting se unka kya connection hai?

> **Definition:**
> Ek array me pair of indices $(i, j)$ ko ek **Inversion** kehte hain agar:
>
> $$
> i < j \quad \text{aur} \quad arr[i] > arr[j]
> $$

- Fully sorted array `[1, 2, 3, 4, 5]` me inversions = **0**.
- Reverse sorted array `[5, 4, 3, 2, 1]` me maximum inversions = $\frac{n(n-1)}{2}$.
- **Key Insight for Insertion Sort:** Adjacent swapping ya shifting har step par exact **1 inversion** ko eliminate karta hai. Isliye Insertion Sort ka running time directly $\Theta(n + I)$ hota hai, jahan $I$ total number of inversions hai!

---

# Part 1: Course-Taught Elementary Sorts

<a id="1-bubble-sort"></a>

## 1. Bubble Sort (Paani ke bubble jaisa concept)

### Core Intuition

Jaise paani ke andar jo halka bubble hota hai wo float karke upar surface pe aa jata hai, waise hi Bubble Sort me har pass me array ka **sabse bada element** adjacent swaps hote-hote array ke aakhiri (end) position par pahunch jata hai!

```text
Initial:  [ 5 , 1 , 4 , 2 , 8 ]
Pass 0:   5 aur 1 compare hue -> Swap -> [ 1 , 5 , 4 , 2 , 8 ]
          5 aur 4 compare hue -> Swap -> [ 1 , 4 , 5 , 2 , 8 ]
          5 aur 2 compare hue -> Swap -> [ 1 , 4 , 2 , 5 , 8 ]
          5 aur 8 compare hue -> No swap -> [ 1 , 4 , 2 , 5 | 8 ] (8 apni final jagah aa gaya!)
```

### Important Questions:

1. **Outer loop kahan tak chalega?**$i = 0$ se $n - 2$ tak (total $n - 1$ passes). Kyunki agar $n - 1$ elements sahi jagah baith gaye, to aakhiri bacha hua 1 element automatically sahi jagah hoga.
2. **Inner loop limit $n - 1 - i$ kyu hoti hai?**Pass $i$ ke baad aakhiri ke $i$ elements already sorted suffix me baith chuke hote hain. Unhe baar-baar re-check karna bekaar ka time waste hai.
3. **`swapped` boolean flag kyu zaroori hai?**
   Agar kisi pass me ek bhi swap nahi hua, iska matlab saare adjacent elements already sahi order me hain (`arr[j] <= arr[j+1]`). Toh aage ke saare passes skip karke turant `break` kar do! Isse already sorted array par time complexity $O(n^2)$ se gir kar **$O(n)$** ho jati hai!

### Complete C++ Implementation:

```cpp
#include <vector>
#include <utility> // for swap
using namespace std;

void bubbleSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1){
        return;
    }

    // Outer loop: total n - 1 passes
    for(int i = 0; i < n - 1; ++i){
        bool swapped = false;

        // Inner loop: har pass me unsorted portion me adjacent elements compare karo
        for(int j = 0; j < n - 1 - i; ++j){
            if(arr[j] > arr[j + 1]){
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // Agar is pure pass me ek bhi swap nahi hua, array already sorted hai!
        if(!swapped){
            break;
        }
    }
}
```

### Complexity & Properties:

- **Best Case Time:** $O(n)$ (jab array already sorted ho aur `swapped` flag break kar de).
- **Average & Worst Case Time:** $O(n^2)$ (reverse sorted me har pair swap hoga: $\frac{n(n-1)}{2}$ comparisons).
- **Auxiliary Space:** $O(1)$ (Strictly in-place).
- **Stability:** **Stable** (kyunki hum swap sirf tab karte hain jab `arr[j] > arr[j+1]` ho; agar dono equal hain, to swap nahi karte, relative order safe rehta hai).

---

<a id="2-selection-sort"></a>

## 2. Selection Sort (Minimum dhoondo aur front pe rakho)

### Core Intuition

Selection Sort ka simple funda hai:

1. Pure unsorted part me ghumo aur **sabse chote (minimum)** element ka index dhoondo.
2. Us minimum element ko unsorted part ke pehle position ke saath **swap** kardo.
3. Ab sorted prefix ka size 1 se badh gaya. Ye step repeat karte jao!

```text
Initial:  [ 64 , 25 , 12 , 22 , 11 ]
Pass 0:   Minimum element is 11 (index 4). Swap with arr[0] (64).
          Result: [ 11 | 25 , 12 , 22 , 64 ]
Pass 1:   Minimum in remaining is 12 (index 2). Swap with arr[1] (25).
          Result: [ 11 , 12 | 25 , 22 , 64 ]
Pass 2:   Minimum in remaining is 22 (index 3). Swap with arr[2] (25).
          Result: [ 11 , 12 , 22 | 25 , 64 ]
Pass 3:   Minimum in remaining is 25 (index 3). Swap with arr[3] (25) -> Self swap.
          Result: [ 11 , 12 , 22 , 25 | 64 ] -> Done!
```

### Complete C++ Implementation:

```cpp
#include <vector>
#include <utility> // for swap
using namespace std;

void selectionSort(vector<int>& arr){
    int n = static_cast<int>(arr.size()); //static case kya h aur kyun use kiya?
    if(n <= 1){
        return;
    }

    // Outer loop: pehle n - 1 positions ke liye sahi element select karo // kyun isse kya hoga?
    for(int i = 0; i < n - 1; ++i){ 
        int minIdx = i;

        // Inner loop: i se lekar aakhiri tak sabse chota element dhoondo
        for(int j = i + 1; j < n; ++j){
            if(arr[j] < arr[minIdx]){
                minIdx = j;
            }
        }

        // Har pass me sirf aur sirf EK swap hota hai!
        if(minIdx != i){
            swap(arr[i], arr[minIdx]);
        }
    }
}
```

### Critical Interview Traps:

1. **Selection Sort Unstable kyu hai? (Concrete Counterexample):**Log sochte hain ki hum to sirf minimum select kar rahe hain, to instability kaise aayi? Instability minimum dhoondne se nahi, balki **long-range swap** se aati hai!*Counterexample:* `arr = [4a, 4b, 1]`

   - Pass 0 par minimum element `1` mila (index 2).
   - Humne `arr[0]` (`4a`) ko `arr[2]` (`1`) ke saath swap kar diya.
   - Array ban gaya: `[1, 4b, 4a]`.
   - Dekho kya hua: Original array me `4a` pehle tha aur `4b` baad me. Ab `4b` pehle aa gaya aur `4a` baad me! Relative order toot gaya $\implies$ **Unstable!**
2. **Selection Sort Non-Adaptive kyu hai?**
   Array chahe already sorted ho `[1, 2, 3, 4, 5]`, inner loop tab bhi har pass me aage ke saare elements ko scan karke verify karega ki koi chota to nahi hai. Total comparisons hamesha fixed hote hain: $\frac{n(n-1)}{2}$.
   Isliye Best, Average, Worst teeno cases me time complexity **$\Theta(n^2)$** hi rehti hai!

---

<a id="3-insertion-sort"></a>

## 3. Insertion Sort (Taash / Playing cards haath me lagana)

### Core Intuition

Socho tum taash (playing cards) khel rahe ho. Jab tumhe ek naya card milta hai, to tum apne haath me pehle se sorted cards ko dekhte ho:

- Agar naya card chota hai, to bade cards ko right me khiskate (shift karte) jate ho.
- Jaise hi sahi jagah milti hai, naye card ko wahan drop (insert) kar dete ho!

```text
Hand:      [ 2 , 4 , 5 ]   Naya Card aaya: 3
Step 1:    Save key = 3.
Step 2:    5 > 3 -> 5 ko right shift karo -> [ 2 , 4 , 5 , 5 ]
Step 3:    4 > 3 -> 4 ko right shift karo -> [ 2 , 4 , 4 , 5 ]
Step 4:    2 < 3 -> Stop! Spot mil gaya. Spot index 1 par key (3) daal do:
Hand:      [ 2 , 3 , 4 , 5 ] -> Sorted!
```

### Shifting vs Swapping (Shifting kyu fast hai?):

Beginners galti se `swap(arr[j], arr[j-1])` baar-baar call karte hain.

- Ek swap 3 assignments karta hai (temporary variable ke through) aur array me 2 writes karta hai.
- **Shifting** me hum `key = arr[i]` ko ek baar save karte hain, fir elements ko sirf right copy karte hain (`arr[j+1] = arr[j]`), aur aakhiri me `key` ko single write karte hain! Agar element ko $t$ positions peeche jana hai, to swap karega $2t$ writes, jabki shifting karegi sirf $t + 1$ writes. Isliye shifting significantly faster hoti hai.

### Complete C++ Implementation:

```cpp
#include <vector>
using namespace std;

void insertionSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1){
        return;
    }

    // Index 1 se start karo kyunki single element (index 0) already sorted hota hai
    for(int i = 1; i < n; ++i){
        int key = arr[i]; // Naya card jisko insert karna hai
        int j = i - 1;    // Sorted hand me peeche chalna shuru karo

        // Jab tak peeche wale elements key se bade hain, unhe right shift karo
        while(j >= 0 && arr[j] > key){
            arr[j + 1] = arr[j];
            --j;
        }

        // Khali hui jagah par key ko insert kardo
        arr[j + 1] = key;
    }
}
```

### Complexity & Real-World Importance:

- **Best Case Time:** **$O(n)$** (Already sorted array par while loop pehli hi condition me fail ho jata hai, zero shifts!).
- **Worst Case Time:** $O(n^2)$ (Reverse sorted array par har element ko poora left tak shift karna padta hai).
- **Auxiliary Space:** $O(1)$ (In-place).
- **Stability:** **Stable** (`arr[j] > key` strict check hai; equal element aate hi shift ruk jata hai).
- **Standard Libraries kyu use karti hain?**
  Small arrays ($n \le 16$ ya $32$) par Insertion Sort ka overhead zero hota hai (no recursion, cache-friendly linear memory access). Isliye production sorting libraries (jaise C++ `std::sort` / Introsort, Python Timsort) partition chota hote hi Insertion Sort par switch kar jati hain!

---

<a id="4-counting-sort"></a>

## 4. Counting Sort (Non-comparison frequency sorting)

### Core Intuition

Ab tak humne dekha ki elements ko aapas me compare karke sort kiya jata hai. Lekin agar hume pata ho ki array ke numbers ek chote range me hain (jaise test scores 0 se 100), to bina compare kiye hum unki **frequency count** karke direct sort kar sakte hain!

### Way 1: Simple Frequency Overwrite (Sirf Primitives ke liye)

1. Har number ki frequency count karo.
2. Frequency array par loop chala ke original array me overwrite kardo.
   *Khasiyat:* Easy to write, $O(n + k)$ time, $O(k)$ extra space.
   *Limitation:* Ye satellite data (objects / records) ka original identity preserve nahi karta.

### Way 2: Stable Prefix-Sums + Backward Traversal (Standard Production Method)

Jab elements structs ya objects hon (jaise Student jisme Marks aur Name dono hon), tab hum prefix sums aur backward traversal use karte hain:

1. `count[x]` frequency store karta hai.
2. Prefix sum calculate karo: `count[i] += count[i-1]`. Ab `count[x]` batata hai ki $x$ se chote ya barabar total kitne elements hain.
3. Original array ko **peeche se (backwards, $n-1$ down to 0)** iterate karo, aur element ko `output[--count[arr[i]]]` par place karo.
4. Peeche se iterate karne ki wajah se jo duplicate element input me baad me aaya tha, wo output me bhi baad me hi place hota hai $\implies$ **Guaranteed Stability!**

### Handling Negative Numbers & Range Overflow:

Agar array me negative numbers hon (jaise `[-5, 3, -2]`), to direct negative index array me use nahi ho sakta.
Solution: `minVal` dhoondo, aur har element ka offset index banao: `arr[i] - minVal`.
Range check: `range = maxVal - minVal + 1`. Range hamesha `long long` me compute karo taaki agar `maxVal = INT_MAX` aur `minVal = INT_MIN` ho to signed integer overflow na ho!

### Complete C++ Implementation:

```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <climits>
using namespace std;

// Way 1: Primitive values overwrite (O(k) extra space)
void countingSortValues(vector<int>& arr){
    if(arr.size() <= 1) return;
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    long long range = static_cast<long long>(maxVal) - minVal + 1;
    if(range > 1000000) {
        throw invalid_argument("Range too large for Counting Sort");
    }
    vector<int> count(range, 0);
    for(int x : arr) count[x - minVal]++;
    int idx = 0;
    for(int i = 0; i < range; ++i){
        while(count[i] > 0){
            arr[idx++] = i + minVal;
            count[i]--;
        }
    }
}

// Way 2: Full Stable Counting Sort with Prefix Sums & Backward Traversal
void countingSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    long long range = static_cast<long long>(maxVal) - minVal + 1;
    if(range > 1000000){
        throw invalid_argument("Range too large for Counting Sort");
    }

    vector<int> count(range, 0);
    vector<int> output(n);

    // 1. Frequency count
    for(int x : arr){
        count[x - minVal]++;
    }

    // 2. Prefix sums (cumulative count)
    for(int i = 1; i < range; ++i){
        count[i] += count[i - 1];
    }

    // 3. Backward traversal to preserve stability
    for(int i = n - 1; i >= 0; --i){
        int val = arr[i];
        int pos = count[val - minVal] - 1;
        output[pos] = val;
        count[val - minVal]--;
    }

    arr = move(output);
}
```

### Complexity & Constraints:

- **Time Complexity:** $\Theta(n + k)$ across Best, Average, Worst cases (jahan $k = \text{range}$).
- **Auxiliary Space:** $O(n + k)$ for stable version.
- **Kab fail hota hai?** Agar $n = 5$ ho aur elements hon `[1, 2, 10^9]`, to $k = 10^9$. Counting Sort $10^9$ size ka vector allocate karne ki koshish karega aur memory overflow ya TLE ho jayega! Isliye Counting Sort sirf tabhi use hota hai jab range $k = O(n)$ ho.

---

# Part 2: Fast & Divide-and-Conquer Sorts

<a id="5-merge-sort"></a>

## 5. Merge Sort (Divide, Conquer & Linear Merge)

### Core Intuition

Merge Sort ka mantra hai: **Divide and Conquer**.

1. **Divide:** Array ko beech se kaat kar do barabar hisson me baant do (`mid = low + (high - low) / 2`).
2. **Conquer:** Left half aur Right half dono ko recursively sort karo.
3. **Combine (Merge):** Do alag-alag sorted halves ko 2-pointer approach use karke linearly merge kar do!

```text
                  [ 5 , 2 , 4 , 1 , 3 ]
                        /       \
               [ 5 , 2 ]         [ 4 , 1 , 3 ]
                /     \            /        \
             [ 5 ]   [ 2 ]      [ 4 ]     [ 1 , 3 ]
               \     /            \        /     \
               [ 2 , 5 ]          [ 4 ]  [ 1 ]   [ 3 ]
                  \                  \     \     /
                   \                  \   [ 1 , 3 ]
                    \                  \   /
                     \              [ 1 , 3 , 4 ]
                      \              /
                   [ 1 , 2 , 3 , 4 , 5 ]
```

### The 2-Pointer Linear Merge Walkthrough:

Maan lo do halves hain: Left `[2, 5]` aur Right `[1, 3, 4]`.

- Pointer $i$ Left par, pointer $j$ Right par:
  1. $2 > 1 \implies$ Pick `1`, $j++$. Output: `[1]`.
  2. $2 < 3 \implies$ Pick `2`, $i++$. Output: `[1, 2]`.
  3. $5 > 3 \implies$ Pick `3`, $j++$. Output: `[1, 2, 3]`.
  4. $5 > 4 \implies$ Pick `4`, $j++$. Output: `[1, 2, 3, 4]`.
  5. Right khatam! Left me bacha `5`, copy kardo. Output: `[1, 2, 3, 4, 5]`.
     Har element ko sirf ek baar touch kiya $\implies$ Merge step takes **pure linear time $O(n)$**.

### Stability Rule:

Merge step me hum likhte hain:

```cpp
if (arr[i] <= arr[j]) { temp[k++] = arr[i++]; }
```

Yahan `<=` lagana critical hai! Agar left half aur right half dono me identical element aa jaye, to hum hamesha **Left half** wale ko pehle uthate hain, jisse input ka original order preserve rehta hai $\implies$ **Stable!**

### Memory Optimization (Single Upfront Allocation):

Beginners har recursive call ke andar naya `vector<int> temp` allocate karte hain. Isse $O(n \log n)$ unnecessary dynamic allocations hoti hain aur code slow ho jata hai.
**Pro-Tip:** Main function me ek hi baar `temp` vector allocate karo aur use recursion me pass karo!

### Complete C++ Implementation:

```cpp
#include <vector>
using namespace std;

void merge(vector<int>& arr, int low, int mid, int high, vector<int>& temp){
    int i = low;
    int j = mid + 1;
    int k = low;

    while(i <= mid && j <= high){
        if(arr[i] <= arr[j]){ // '<=' preserves stability!
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while(i <= mid) temp[k++] = arr[i++];
    while(j <= high) temp[k++] = arr[j++];

    for(int p = low; p <= high; ++p){
        arr[p] = temp[p];
    }
}

void mergeSortInternal(vector<int>& arr, int low, int high, vector<int>& temp){
    if(low >= high) return;

    int mid = low + (high - low) / 2;

    mergeSortInternal(arr, low, mid, temp);
    mergeSortInternal(arr, mid + 1, high, temp);

    // Optional optimization: agar already sorted order me jude hain to merge skip karo
    if(arr[mid] <= arr[mid + 1]){
        return;
    }

    merge(arr, low, mid, high, temp);
}

void mergeSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;
    vector<int> temp(n); // Single upfront allocation
    mergeSortInternal(arr, 0, n - 1, temp);
}
```

### Complexity Proof ($O(n \log n)$ kyu hota hai?):

1. **Har level par kaam:** Har recursion level par array ke saare $n$ elements merge step me participate karte hain $\implies$ Work per level = $O(n)$.
2. **Total levels:** Har step par size aadhi ho rahi hai ($n \to n/2 \to n/4 \dots 1$) $\implies$ Total levels = $\log_2 n$.
3. **Total Work:** $\text{Levels} \times \text{Work per level} = n \times \log_2 n = \mathbf{O(n \log n)}$.

- Ye bound **Best, Average, aur Worst** sabhi cases me guaranteed rehta hai!
- **Auxiliary Space:** $O(n)$ temporary buffer $+ O(\log n)$ recursion call stack.

---

<a id="6-quick-sort"></a>

## 6. Quick Sort (Lomuto, Hoare, Bounded Stack, aur 3-Way Partition)

### Core Intuition

Merge Sort divide blindly karta hai aur saara mushkil kaam merge ke time karta hai.**Quick Sort iska ulta hai — ye saara mehnat ka kaam shuru me (partition me) karta hai!**

1. **Pick a Pivot:** Array me se kisi ek element ko "pivot" chuno.
2. **Partition:** Array ko rearrange karo taaki pivot se chote elements uske left me chale jayein, aur bade elements uske right me.
3. **Pivot is Placed!** Partition hote hi pivot element apni 100% final sorted position par baith jata hai! Use dubara touch karne ki zaroorat nahi hoti.
4. **Recurse:** Ab pivot ke left sub-array aur right sub-array par yahi logic recursively chalao.

---

### Scheme 1: Lomuto Partition (Single Pointer Walking)

Lomuto scheme aamtaur par last element ko pivot maanti hai:

- Pointer $i$ maintain karta hai boundary of elements $\le \text{pivot}$. Start $i = \text{low} - 1$.
- Pointer $j$ scan karta hai $\text{low}$ se $\text{high} - 1$ tak.
- Agar `arr[j] <= pivot`, to $i++$ karo aur `swap(arr[i], arr[j])`.
- Aakhiri me `swap(arr[i+1], arr[high])` karke pivot ko beech me baitha do. Return $i+1$.

```cpp
#include <vector>
#include <utility>
using namespace std;

int partition(vector<int>& arr, int low, int high){
    int pivot = arr[high]; // Last element is pivot
    int i = low - 1;

    for(int j = low; j < high; ++j){
        if(arr[j] <= pivot){
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1; // Pivot ka final index
}
```

---

### Scheme 2: Hoare Partition (Two Pointers Moving Inward)

Hoare partition me do pointers dono ends se inward move karte hain:

- Left pointer aage badhta hai jab tak use element $\ge \text{pivot}$ na mile.
- Right pointer peeche aata hai jab tak use element $\le \text{pivot}$ na mile.
- Agar wo cross nahi hue, to unhe swap karke aage badhao.
- **Fayda:** Lomuto se lagbhag aadhi (50% fewer) swaps karta hai.
- **Bada Trap:** Hoare partition ka returned index **pivot ka final index nahi hota**, balki wo ek **split boundary** hota hai! Isliye recursive call ranges `[low, split]` aur `[split + 1, high]` hoti hain (Lomuto jaisa `split - 1` mat likhna, varna infinite recursion ho jayega).

```cpp
#include <vector>
#include <utility>
using namespace std;

int hoarePartition(vector<int>& arr, int low, int high){
    int pivot = arr[low + (high - low) / 2];
    int i = low - 1;
    int j = high + 1;
    while(true){
        do { ++i; } while(arr[i] < pivot);
        do { --j; } while(arr[j] > pivot);
        if(i >= j) return j;
        swap(arr[i], arr[j]);
    }
}
```

---

### Stack Overflow Prevention: Strict $O(\log n)$ Stack Guarantee

Standard QuickSort me agar partitions unbalanced hon (jaise sorted array pe pivot last element ho), to recursion call stack ki depth $n$ ho jati hai, jisse program **Stack Overflow (Crash)** ho jata hai!

> [!IMPORTANT]
> **Interview Secret: Recurse on Smaller Partition, Loop on Larger!**
> C++ automatic tail-call optimization guarantee nahi karta. Lekin agar hum hamesha **chote sub-array** par recursion call karein aur **bade sub-array** ko ek simple `while` loop se handle karein, to stack frames maximum $O(\log_2 n)$ hi ban sakte hain!

```cpp
#include <vector>
using namespace std;

int partition(vector<int>& arr, int low, int high); // Lomuto partition

void quickSortLogarithmicStack(vector<int>& arr, int low, int high){
    while(low < high){
        int pIdx = partition(arr, low, high);

        // Always recurse on the smaller partition; loop on the larger!
        if(pIdx - low < high - pIdx){
            quickSortLogarithmicStack(arr, low, pIdx - 1);
            low = pIdx + 1; // Loop on larger right partition
        } else {
            quickSortLogarithmicStack(arr, pIdx + 1, high);
            high = pIdx - 1; // Loop on larger left partition
        }
    }
}
```

---

### Scheme 3: 3-Way Partitioning (Dutch National Flag) for Duplicate Keys

Agar array me bohot saare duplicate keys hon (jaise `[7, 7, 7, 7, 7]`), to standard Lomuto partition unbalanced split deta hai aur time complexity $O(n^2)$ ho jati hai.
Iska ultimate solution hai **Dijkstra's 3-Way Partitioning**:
Array ko 3 parts me baant do: `[ < pivot | == pivot | > pivot ]`.
Equal elements ek hi pass me final ho jate hain aur unhe dobara recurse karne ki zaroorat hi nahi hoti! All-equal array par runtime $O(n^2)$ se gir kar **$\Theta(n)$** ho jata hai!

```cpp
#include <vector>
#include <utility>
using namespace std;

void quickSort3Way(vector<int>& arr, int low, int high){
    if(low >= high) return;

    int pivot = arr[low + (high - low) / 2];
    int lt = low;  // arr[low ... lt-1] < pivot
    int gt = high; // arr[gt+1 ... high] > pivot
    int i = low;   // arr[lt ... i-1] == pivot

    while(i <= gt){
        if(arr[i] < pivot){
            swap(arr[lt++], arr[i++]);
        } else if(arr[i] > pivot){
            swap(arr[i], arr[gt--]); // Note: Do NOT increment i! incoming element re-check hoga
        } else {
            i++;
        }
    }

    // Sirf strictly smaller aur strictly greater regions ko recurse karo
    quickSort3Way(arr, low, lt - 1);
    quickSort3Way(arr, gt + 1, high);
}
```

---

<a id="7-heap-sort"></a>

## 7. Heap Sort (Max-Heap & The $O(n)$ Build-Heap Insight)

### Core Intuition

Heap Sort ko ek tournament bracket ki tarah samjho:

1. Pure array ko ek **Max-Heap** me organize karo: Har parent apne children se bada ya barabar hota hai. Is structure ka champion (sabse bada element) hamesha root par (index 0) baithta hai.
2. Root element (`arr[0]`) ko utha kar array ke aakhiri position (`arr[i]`) par swap kardo.
3. Ab remaining elements par `heapify` (sift-down) chalao taaki agla sabse bada number root par aa jaye.
4. Ye step repeat karte jao jab tak poora array sort na ho jaye!

### Binary Heap ka Array Representation:

Bina kisi pointer ya linked node ke, simple 0-indexed array me complete binary tree store hota hai:

- Index $i$ ka **Left Child:** $2i + 1$
- Index $i$ ka **Right Child:** $2i + 2$
- Index $i$ ka **Parent:** $\lfloor(i - 1) / 2\rfloor$
- Last non-leaf parent node: index $\lfloor n / 2 \rfloor - 1$.

### Complete C++ Implementation:

```cpp
#include <vector>
#include <utility>
using namespace std;

// Sifts down element at index i (Iterative version avoids recursion call stack!)
void heapify(vector<int>& arr, int n, int i){
    while(i < n / 2){ // Sirf parents ke children hote hain
        int left = 2 * i + 1;
        int right = left + 1;
        int largest = left;

        if(right < n && arr[right] > arr[left]){
            largest = right;
        }

        if(arr[i] >= arr[largest]){
            break; // Max-heap property satisfied
        }

        swap(arr[i], arr[largest]);
        i = largest; // Downward continue karo bina recursion ke
    }
}

void heapSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    // Phase 1: Build Max-Heap in O(n) time
    for(int i = n / 2 - 1; i >= 0; --i){
        heapify(arr, n, i);
    }

    // Phase 2: Extract maximum one-by-one in O(n log n)
    for(int i = n - 1; i > 0; --i){
        swap(arr[0], arr[i]); // Maximum ko end me bhejo
        heapify(arr, i, 0);   // Bache hue heap ko fix karo
    }
}
```

### The $O(n)$ Build-Heap Mathematical Proof:

> **Question:** Har element ko heapify karne me $O(\log n)$ lagta hai, to $n$ elements ka heap banana $O(n \log n)$ kyu nahi hota? $O(n)$ kyu hota hai?
> **Answer:** Tree me aadhe nodes ($\approx n/2$) sabse neeche leaves par hote hain jinki height 0 hoti hai (zero swaps!). Sirf ek single root node poori height $\log n$ travel kar sakti hai.
> Total work is given by:
>
> $$
> \sum_{h=0}^{\log n} \frac{n}{2^{h+1}} \times O(h) = O\left(n \sum_{h=0}^{\infty} \frac{h}{2^h}\right) = O(n \times 2) = \mathbf{O(n)}
> $$
>
> Ye Taylor series converge karti hai, isliye Build-Heap pure linear time $O(n)$ leta hai!

---

# Part 3: Non-Comparison & Advanced Sorts

<a id="8-radix-sort"></a>

## 8. Radix Sort (Postal Machine LSD Digit Sorting)

### Core Intuition

Purane jamane me dak khane (post office) me mechanical sorting machines 5-digit PIN codes ko kaise sort karti thi?

- Pehle unhone **ones digit** (0 se 9) ke basis par chitthiyan alag 10 box me baant li.
- Fir unhe bina order bigade ikattha kiya, aur **tens digit** ke basis par sort kiya.
- Fir **hundreds digit**, aur aakhiri me highest digit tak gaye.
- Highest digit khatam hote hi poori mail perfect numerical order me sort ho chuki thi!

> [!CRITICAL] Critical
> **Kyu har digit pass ka STABLE hona strictly mandatory hai?**Agar hum `[21, 13, 12]` ko sort karein:
>
> - Ones digit pass ke baad: `[21, 12, 13]` (kyunki 1 < 2 < 3).
> - Ab jab tens digit pass chalega, to `12` aur `13` dono ka tens digit `1` hai (tie!). Agar tens pass **Stable** nahi hoga, to wo `13` ko `12` ke aage phenk dega aur ones digit ki mehnat barbad ho jayegi! Stable pass tie hone par purana order barkarar rakhta hai.

### Complete C++ Implementation:

```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>
using namespace std;

void radixCountingSort(vector<int>& arr, long long exp){
    int n = static_cast<int>(arr.size());
    vector<int> output(n);
    int count[10] = {0};

    for(int i = 0; i < n; ++i){
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    for(int i = 1; i < 10; ++i){
        count[i] += count[i - 1];
    }

    for(int i = n - 1; i >= 0; --i){
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    arr = move(output);
}

void radixSort(vector<int>& arr){
    for(int x : arr){
        if(x < 0){
            throw invalid_argument("Radix Sort requires non-negative integers.");
        }
    }
    if(arr.size() <= 1) return;

    int maxVal = *max_element(arr.begin(), arr.end());
    // Safe long long exp prevents 32-bit int overflow when maxVal is near INT_MAX
    for(long long exp = 1; maxVal / exp > 0; exp *= 10){
        radixCountingSort(arr, exp);
    }
}
```

- **Time Complexity:** $O(d \cdot (n + b))$ jahan $d$ total digits hain aur $b$ base hai ($b = 10$).
- **Auxiliary Space:** $O(n + b)$.
- **Stability:** **Stable**.

---

<a id="9-bucket-sort"></a>

## 9. Bucket Sort (Uniform Distribution & Scatter-Gather)

### Core Intuition

Maan lo tumhe floating-point numbers sort karne hain jo $[0.0, 1.0)$ ke beech uniform distributed hain (jaise student test percentages):

1. **Scatter:** $n$ khali buckets banao. Har element ko uske value ke hisab se sahi bucket me daal do: `idx = floor(n * val)`.
2. **Sort:** Har individual bucket ko chote size ke liye **Insertion Sort** se sort karo.
3. **Gather:** Buckets ko sequentially jod kar original array me daal do!

### Complete C++ Implementation:

```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>
using namespace std;

void bucketSort(vector<double>& arr){
    int n = static_cast<int>(arr.size());
    for(double val : arr){
        if(!isfinite(val) || val < 0.0 || val >= 1.0){
            throw invalid_argument("Values must be finite and in [0.0, 1.0)");
        }
    }
    if(n <= 1) return;

    vector<vector<double>> buckets(n);

    // 1. Scatter
    for(double val : arr){
        int idx = min(static_cast<int>(n * val), n - 1);
        buckets[idx].push_back(val);
    }

    // 2. Sort individual buckets with stable Insertion Sort
    for(auto& bucket : buckets){
        for(size_t i = 1; i < bucket.size(); ++i){
            double key = bucket[i];
            size_t j = i;
            while(j > 0 && bucket[j - 1] > key){
                bucket[j] = bucket[j - 1];
                --j;
            }
            bucket[j] = key;
        }
    }

    // 3. Gather
    int k = 0;
    for(int i = 0; i < n; ++i){
        for(double val : buckets[i]){
            arr[k++] = val;
        }
    }
}
```

- **Time Complexity:** Average Case $O(n)$ jab data uniformly distributed ho. Worst Case $O(n^2)$ agar saare elements ek hi single bucket me gir jayein aur wo bucket reverse-sorted ho.
- **Auxiliary Space:** $O(n + B)$ jahan $B$ number of buckets hai.

---

<a id="10-shell-sort"></a>

## 10. Shell Sort (Diminishing Gap Sort & Knuth Sequence)

### Core Intuition

Standard Insertion Sort reverse-sorted array par bohot slow hota hai kyunki elements sirf 1 step at a time move karte hain.
**Shell Sort lambi chhalang (large leaps) allow karta hai!**
Ye pehle ek bade gap $h$ (jaise $h = 4$) par baithe elements ko sort karta hai, fir gap ko kam karta hai ($h = 2$), aur aakhiri me normal Insertion Sort ($h = 1$) chalata hai. Kyunki pehle ke passes ne long-distance inversions ko already eliminate kar diya hota hai, aakhiri pass me array nearly-sorted hota hai aur linear time me complete ho jata hai!

### Knuth's Formula:

Knuth sequence gaps use karti hai: $h = 3h + 1 \implies 1, 4, 13, 40, 121 \dots$
Is sequence ke saath Shell Sort ki worst-case time complexity $O(n^2)$ se gir kar **$O(n^{3/2})$** ho jati hai!

```cpp
#include <vector>
using namespace std;

void shellSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    // Calculate maximum initial Knuth gap
    int h = 1;
    while(h < n / 3){
        h = 3 * h + 1;
    }

    // Shrink gap until h reaches 1
    while(h >= 1){
        for(int i = h; i < n; ++i){
            int temp = arr[i];
            int j = i;
            while(j >= h && arr[j - h] > temp){
                arr[j] = arr[j - h];
                j -= h;
            }
            arr[j] = temp;
        }
        h /= 3;
    }
}
```

---

# Part 4: Practical C++ STL Sorting Mastery

<a id="11-c-stl-tools"></a>

## 11. C++ STL Tools: `sort`, `stable_sort`, `partial_sort`, `nth_element`

Google aur top tech companies ke interviews me bina wajah full sort likhne ke bajaye STL tools ko smartly use karna chahiye:

| STL Function          | Internally kya use karta hai?        | Time Complexity                                | Extra Space   | Use Case                                                              |
| --------------------- | ------------------------------------ | ---------------------------------------------- | ------------- | --------------------------------------------------------------------- |
| `std::sort`         | Introsort (Quick + Heap + Insertion) | $O(n \log n)$ Guaranteed                     | $O(\log n)$ | General sorting jab stability zaroori na ho                           |
| `std::stable_sort`  | Adaptive MergeSort                   | $O(n \log n)$ (Low mem pe $O(n \log^2 n)$) | $O(n)$      | Jab equal elements ka order strictly preserve karna ho                |
| `std::partial_sort` | Heap-based selection                 | $O(n \log k)$                                | $O(1)$      | Jab 10 lakh me se sirf Top-$k$ elements sorted order me chahiye hon |
| `std::nth_element`  | QuickSelect                          | $O(n)$ Average                               | $O(1)$      | Median ya $k^{\text{th}}$ smallest element nikalne ke liye          |

### Production Demo Code:

```cpp
#include <vector>
#include <algorithm>
#include <functional> // for greater<int>()
using namespace std;

void sortingLibraryDemo(){
    vector<int> a = {5, 2, 8, 1, 9};
    sort(a.begin(), a.end()); // [1, 2, 5, 8, 9] (Unstable)

    vector<int> b = {5, 2, 8, 1, 9};
    stable_sort(b.begin(), b.end()); // [1, 2, 5, 8, 9] (Stable)

    // Top-3 smallest elements ko front me sort karo
    vector<int> c = {10, 2, 8, 6, 7, 3, 1, 5};
    partial_sort(c.begin(), c.begin() + 3, c.end());
    // Indices [0, 1, 2] par exact [1, 2, 3] baith gaye, baaki peeche unsorted

    // QuickSelect: 4th smallest element (index 3) dhundo in O(n) average
    vector<int> d = {10, 2, 8, 6, 7, 3, 1, 5};
    nth_element(d.begin(), d.begin() + 3, d.end());
    // d[3] exact 4th smallest number ban gaya! Left wale <= d[3], Right wale >= d[3]
}

void ascendingAndDescendingDemo(){
    int arr[] = {3, 1, 7, 5, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    sort(arr, arr + n);                   // Ascending: [1, 3, 4, 5, 7]
    sort(arr, arr + n, greater<int>{});   // Descending: [7, 5, 4, 3, 1]
}
```

---

<a id="12-custom-comparators--strict-weak-ordering"></a>

## 12. Custom Comparators, Lambdas, & The Fatal `<=` Bug (Strict Weak Ordering)

### Multi-Key Sorting with Lambdas

Maan lo candidates ko sort karna hai: Score high to low; agar Score barabar ho, to ID low to high:

```cpp
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Candidate {
    string name;
    int score;
    int id;
};

void sortCandidates(vector<Candidate>& candidates){
    sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b){
        if(a.score != b.score){
            return a.score > b.score; // High score pehle
        }
        return a.id < b.id;           // Low ID pehle
    });
}
```

---

### The Fatal Bug: `return a <= b` kyu crash kar deta hai?

> [!CAUTION]
> **Kabhi bhi comparator me `<=` ya `>=` return mat karna!**
> C++ standard library demands **Strict Weak Ordering**.
> Comparator `comp(a, b)` ka matlab hota hai: *"Kya `a` ko strictly `b` ke aage aana chahiye?"*
> Rule 1 (Irreflexivity): Koi bhi element khud se pehle nahi aa sakta! Yani `comp(x, x)` hamesha **`false`** hona chahiye.
>
> Agar tum `return a <= b;` likhte ho, to jab dono elements equal honge to `comp(x, x)` return karega `true`.
> Isse C++ standard library ke partition loops boundary check cross karke memory ke bahar read/write karne lagte hain, jisse **Segmentation Fault (Crash)** ya silent data corruption ho jata hai!
> **Golden Rule:** Ascending ke liye hamesha `<` use karo, Descending ke liye `>`.

---

# Part 5: Theoretical Limits & System-Level Sorting

<a id="13-comparison-sorting-lower-bound"></a>

## 13. Comparison Sorting Lower Bound: $\Omega(n \log n)$ Proof (20 Questions Game)

### The "20 Questions Game" Intuition:

Socho tum ek 20 Questions game khel rahe ho jisme har sawaal ka jawab sirf "Yes" ya "No" ho sakta hai:

- 1 sawaal 2 possibilities distinguish kar sakta hai ($2^1$).
- 20 sawaal maximum $2^{20} = 1,048,576$ possibilities ko distinguish kar sakte hain.

Ab socho $n$ distinct numbers ko kitne alag-alag scrambled tareeqon me arrange kiya ja sakta hai?
Answer: **$n!$** (n factorial permutations).
Comparison sort jab bhi do elements ko compare karta hai (`arr[i] < arr[j]`), use sirf ek binary Yes/No information milti hai.
Sabhi $n!$ leaves ko reach karne ke liye binary decision tree ki height kam se kam honi chahiye:

$$
\text{Height} \ge \log_2(n!)
$$

Stirling's Approximation se:

$$
\log_2(n!) = n \log_2 n - n \log_2 e + O(\log n) = \mathbf{\Omega(n \log n)}
$$

**Conclusion:** Duniya ka koi bhi comparison-based algorithm arbitrary array ko worst case me $\Omega(n \log n)$ se tez sort nahi kar sakta! Counting Sort aur Radix Sort is limit ko isliye beat kar paate hain kyunki wo elements ko aapas me compare nahi karte, balki integer arithmetic aur digit addressing use karte hain.

---

<a id="14-real-world-systems"></a>

## 14. Real-World Systems: External Sort, Linked Lists, & $k$-Sorted Arrays

### 14.1 External Sorting (10 Terabytes data on 32 GB RAM)

Agar interview me Google interviewer pooche: *"Tumhare paas 10 TB data hai disk par, aur machine me sirf 32 GB RAM hai. Kaise sort karoge?"*

**Step 1: Run Generation (In-Memory Chunks)**
RAM ka safe buffer rakho (e.g., 20 GB). 10 TB data me se 20 GB ka chunk read karo, RAM me `std::sort` chalao, aur sorted "run" ko temporary file me disk par likh do.
Total runs banenge: $\frac{10\text{ TB}}{20\text{ GB}} = 500$ sorted files on disk.

**Step 2: $k$-Way Merge using Min-Heap**Ab sabhi 500 sorted files ke liye disk se buffered read streams open karo. Ek **Min-Heap of size 500** banao jisme har file ka pehla element daal do.

- Heap se minimum nikalo, output buffer me likho.
- Jis file se wo minimum aaya tha, us file ka agla element heap me push karo.
- Output buffer bharte hi use disk par sequentially flush kardo.
  Har record ko merge karne me $O(\log k) = O(\log 500) \approx 9$ operations lagte hain!

---

### 14.2 Linked Lists par Merge Sort King kyu hai?

- Linked lists me array jaisa $O(1)$ random indexing (`arr[i]`) nahi hota. Isliye QuickSort aur HeapSort linked lists par bohot inefficient hote hain.
- **Merge Sort is perfect for Linked Lists:**
  1. Midpoint slow & fast pointer technique se $O(n)$ me nikalta hai.
  2. Do sorted lists ko merge karte waqt sirf node pointers (`next`) change hote hain — koi $O(n)$ auxiliary array buffer allocate karne ki zaroorat nahi padti!
  3. Iterative bottom-up Merge Sort linked list ko **$O(1)$ auxiliary space** me sort kar deta hai!

---

### 14.3 $k$-Sorted (Nearly Sorted) Arrays

Agar array me har element apni sahi sorted position se maximum $k$ doori par ho:

- **Solution:** Ek **Min-Heap of size $k + 1$** banao.
- Pehle $k + 1$ elements ko heap me daalo.
- Root pop karo aur index 0 par daalo, agla element heap me push karo. Repeat!
- **Complexity:** Time **$O(n \log k)$**, Space **$O(k)$**. Agar $k = 5$ constant ho, to ye pure **$O(n)$ linear time** me sort ho jata hai!

```cpp
#include <vector>
#include <queue>
using namespace std;

void sortKSortedArray(vector<int>& arr, int k){
    int n = static_cast<int>(arr.size());
    if(n <= 1 || k <= 0) return;

    priority_queue<int, vector<int>, greater<int>> minHeap;
    int write = 0;

    for(int i = 0; i < n && i <= k; ++i){
        minHeap.push(arr[i]);
    }

    for(int i = k + 1; i < n; ++i){
        arr[write++] = minHeap.top();
        minHeap.pop();
        minHeap.push(arr[i]);
    }

    while(!minHeap.empty()){
        arr[write++] = minHeap.top();
        minHeap.pop();
    }
}
```

---

# Part 6: Classic Course & LeetCode Sorting Problems

<a id="15-descending-order-sorting"></a>

## 15. Descending Order Sorting & Character Sorting

Array ko descending order me sort karne ke liye:

- Bubble Sort: Swap karo jab `arr[j] < arr[j+1]`.
- Selection Sort: Scan me **maximum** element dhoondo.
- Insertion Sort: Shift karo jab `arr[j] < key`.

```cpp
#include <string>
using namespace std;

// Character string ko descending order me sort karna
void insertionSortDescendingChars(string& text){
    int n = static_cast<int>(text.size());
    for(int i = 1; i < n; ++i){
        char key = text[i];
        int j = i - 1;
        while(j >= 0 && text[j] < key){
            text[j + 1] = text[j];
            --j;
        }
        text[j + 1] = key;
    }
}
```

---

<a id="16-sort-colors--dutch-national-flag"></a>

## 16. Sort Colors / Dutch National Flag (LeetCode 75)

**Problem:** Array me sirf `0`, `1`, aur `2` hain. Inhe in-place single pass me $O(n)$ time aur $O(1)$ space me sort karo.

### 3-Pointer Algorithm:

Hum array ko 4 zones me divide karte hain:
`[ 0s zone | 1s zone | Unknown zone | 2s zone ]`

- `low`: 0s zone ke baad ka agla index.
- `mid`: Current inspecting element in unknown zone.
- `high`: 2s zone ke pehle ka index.

| Value at `arr[mid]` | Action                                             | Kyu?                                                                                                                                                                                           |
| --------------------- | -------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `0`                 | `swap(arr[low], arr[mid])`, `low++`, `mid++` | Low se jo element aayega wo guarantee `1` hoga (kyunki mid use pehle scan kar chuka hai), isliye dono pointers aage badh sakte hain                                                          |
| `1`                 | `mid++`                                          | 1s zone extend ho gaya, bas aage badho                                                                                                                                                         |
| `2`                 | `swap(arr[mid], arr[high])`, `high--`          | **CRITICAL:** High se jo naya element swap hoke mid par aaya hai, wo unknown hai (wo 0 bhi ho sakta hai ya 1 bhi). Isliye `mid++` MAT karna! Next iteration me use dubara inspect karo |

```cpp
#include <vector>
#include <utility>
using namespace std;

void sortColors(vector<int>& nums){
    int low = 0;
    int mid = 0;
    int high = static_cast<int>(nums.size()) - 1;

    while(mid <= high){
        if(nums[mid] == 0){
            swap(nums[low++], nums[mid++]);
        } else if(nums[mid] == 1){
            mid++;
        } else { // nums[mid] == 2
            swap(nums[mid], nums[high--]);
            // Do NOT increment mid here!
        }
    }
}
```

---

<a id="17-merge-sorted-array-into-padded-array"></a>

## 17. Merge Sorted Array into Padded Array (LeetCode 88)

**Problem:** `nums1` ka size $m + n$ hai (pehla $m$ sorted data hai, aakhiri $n$ khali padding zeroes hain). `nums2` ka size $n$ hai (sorted). Dono ko in-place merge karo.

### The Backward-Fill Trick:

Agar hum aage se merge karna shuru karenge, to `nums1` ke unread original elements overwrite ho jayenge!
**Trick:** Peeche se (`write = m + n - 1`) fill karna shuru karo. Sabse bada element hamesha sabse peeche aayega. Kyunki `write` pointer hamesha unread `i` pointer se aage rehta hai (`write = i + j + 1 > i`), koi bhi unread data kabhi overwrite nahi hoga!

```cpp
#include <vector>
#include <stdexcept>
#include <cstddef>
using namespace std;

void mergeSortedArrays(vector<int>& nums1, int m, const vector<int>& nums2, int n){
    if(m < 0 || n < 0 || &nums1 == &nums2 ||
       static_cast<long long>(m) + n != static_cast<long long>(nums1.size()) ||
       static_cast<size_t>(n) != nums2.size()){
        throw invalid_argument("Invalid sizes or aliased vectors");
    }

    int i = m - 1;
    int j = n - 1;
    int write = m + n - 1;

    while(j >= 0){
        if(i >= 0 && nums1[i] > nums2[j]){
            nums1[write--] = nums1[i--];
        } else {
            nums1[write--] = nums2[j--];
        }
    }
}
```

---

<a id="18-next-permutation"></a>

## 18. Next Permutation (LeetCode 31)

**Problem:** Given an array, arrange numbers into lexicographically next greater permutation in $O(n)$ time and $O(1)$ space. Agar array already highest permutation ho (e.g. `[3, 2, 1]`), to reverse karke lowest banao.

### The 3-Step Intuitive Algorithm:

1. **Find Pivot from Right:** Peeche se scan karo aur pehla index $i$ dhoondo jahan `nums[i] < nums[i+1]`. Iske aage ka pura suffix non-increasing (descending) hota hai.Agar aisa koi index na mile, matlab pura array descending hai $\implies$ Simply reverse kardo aur return `false`.
2. **Find Successor to Swap:** Suffix me peeche se dhoondo wo element jo `nums[i]` se strictly bada ho, aur unhe swap kardo.
3. **Reverse the Suffix:** Swap ke baad bhi suffix descending rehta hai. Next permutation banane ke liye suffix ko minimum (ascending) banana hai. Descending array ko ascending banane ke liye sort karne ki zaroorat nahi hoti, **sirf reverse karna hi kafi hota hai ($O(n)$)**!

```cpp
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

bool nextPermutation(vector<int>& nums){
    int n = static_cast<int>(nums.size());
    if(n < 2) return false;

    // Step 1: Find pivot
    int pivot = n - 2;
    while(pivot >= 0 && nums[pivot] >= nums[pivot + 1]){
        --pivot;
    }

    if(pivot < 0){
        reverse(nums.begin(), nums.end());
        return false; // Wrapped to smallest
    }

    // Step 2: Find rightmost element strictly greater than pivot
    int successor = n - 1;
    while(nums[successor] <= nums[pivot]){
        --successor;
    }

    swap(nums[pivot], nums[successor]);

    // Step 3: Reverse suffix
    reverse(nums.begin() + pivot + 1, nums.end());
    return true;
}
```

---

<a id="19-merge-intervals"></a>

## 19. Merge Intervals (LeetCode 56)

```cpp
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> mergeIntervals(vector<vector<int>>& intervals){
    if(intervals.empty()) return {};

    // Step 1: Sort by start time
    sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b){
        return a[0] < b[0];
    });

    vector<vector<int>> merged;
    merged.push_back(intervals[0]);

    // Step 2: Linear merge
    for(size_t i = 1; i < intervals.size(); ++i){
        if(intervals[i][0] <= merged.back()[1]){
            merged.back()[1] = max(merged.back()[1], intervals[i][1]);
        } else {
            merged.push_back(intervals[i]);
        }
    }
    return merged;
}
```

---

<a id="20-count-inversions-in-an-array"></a>

## 20. Count Inversions in an Array ($O(n \log n)$ Merge Sort Approach)

Jab bhi Right half ka element Left half ke element se chota nikalta hai (`arr[j] < arr[i]`), iska matlab Left half ke current element `arr[i]` se lekar `arr[mid]` tak ke **saare bache hue elements** us `arr[j]` se bade hain!
Toh hum direct count me add kar dete hain: `inversions += (mid - i + 1)`.

```cpp
#include <vector>
using namespace std;

long long mergeAndCount(vector<int>& arr, int low, int mid, int high, vector<int>& temp){
    int i = low;
    int j = mid + 1;
    int k = low;
    long long invCount = 0;

    while(i <= mid && j <= high){
        if(arr[i] <= arr[j]){
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            // arr[i...mid] saare elements arr[j] se bade hain!
            invCount += (mid - i + 1);
        }
    }

    while(i <= mid) temp[k++] = arr[i++];
    while(j <= high) temp[k++] = arr[j++];
    for(int p = low; p <= high; ++p) arr[p] = temp[p];

    return invCount;
}

long long mergeSortAndCount(vector<int>& arr, int low, int high, vector<int>& temp){
    long long invCount = 0;
    if(low < high){
        int mid = low + (high - low) / 2;
        invCount += mergeSortAndCount(arr, low, mid, temp);
        invCount += mergeSortAndCount(arr, mid + 1, high, temp);
        invCount += mergeAndCount(arr, low, mid, high, temp);
    }
    return invCount;
}

long long countInversions(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return 0;
    vector<int> temp(n);
    return mergeSortAndCount(arr, 0, n - 1, temp);
}
```

---

<a id="21-kth-largest-element"></a>

## 21. Kth Largest Element in an Array (QuickSelect / `nth_element`)

Array me $k^{\text{th}}$ largest element dhoondna ascending order me index `n - k` dhoondne ke barabar hai:

```cpp
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

int findKthLargest(vector<int>& nums, int k){
    int n = static_cast<int>(nums.size());
    if(k < 1 || k > n){
        throw invalid_argument("k out of bounds");
    }
    int targetIdx = n - k;
    nth_element(nums.begin(), nums.begin() + targetIdx, nums.end());
    return nums[targetIdx];
}
```

---

# Part 7: The Master Doubt Log

Yahan wo 14 sabse common doubts diye gaye hain jisme top students aur interview candidates confuse hote hain:

### Doubt 01: Comparison sort $O(n \log n)$ se tez kyu nahi chal sakta?

**Answer:** Kyunki kisi bhi comparison sort me decision binary hota hai (`<` ya `>`). $n$ distinct elements ke $n!$ permutations hote hain. Decision tree ke saare leaves cover karne ke liye tree ki height $\ge \log_2(n!)$ honi hi padegi, jo ki Stirling approximation se $\Omega(n \log n)$ aati hai.

### Doubt 02: Selection sort minimum uthata hai to bhi unstable kyu ho jata hai?

**Answer:** Instability minimum dhoondne se nahi, balki **long-range swap** se aati hai!
Jab hum `[4a, 4b, 1]` me minimum `1` ko `arr[0]` (`4a`) se swap karte hain, to array `[1, 4b, 4a]` ban jata hai. Yahan `4a` jump karke `4b` ke peeche chala gaya, jisse original relative order reverse ho gaya!

### Doubt 03: Insertion sort me shift karna swap karne se better kyu hai?

**Answer:** Both asymptotic $O(n^2)$ hain, lekin swapping me har adjacent step 3 variable assignments aur 2 array writes karta hai. Shifting me `key` ek baar save hoti hai, elements right copy hote hain ($1$ write each), aur aakhiri me `key` single write hoti hai. $t$ positions shift hone me swapping karegi $2t$ writes, jabki shifting karegi sirf $t+1$ writes!

### Doubt 04: Counting sort me prefix sums aur backward loop kyu lagate hain? Direct frequency overwrite kyu nahi?

**Answer:** Primitive integers ke liye frequency overwrite kafi hai. Lekin agar data records ya structs hon (e.g. Student objects with marks and roll number), to direct overwrite original student objects ka satellite data lose kar dega. Prefix sums index batate hain, aur backward traversal duplicate scores wale students ka original relative arrival order preserve rakhta hai.

### Doubt 05: Build-Heap $O(n)$ kyu hai jabki $n$ inserts $O(n \log n)$ lete hain?

**Answer:** Tree ke aadhe nodes ($\approx n/2$) leaves par hote hain jinki height 0 hoti hai (0 work). Sirf single root node poori height $\log n$ travel kar sakti hai. Series $\sum \frac{h}{2^h}$ converge karke constant $2$ ban jati hai, isliye total work bounded by $O(n)$ hota hai.

### Doubt 06: C++ comparator me `a <= b` likhne se Undefined Behavior kyu hota hai?

**Answer:** ISO C++ standard demands Strict Weak Ordering. Iska Rule 1 hai Irreflexivity (`comp(x, x) == false`). Agar tum `a <= b` likhoge to equal values par `comp(x, x)` true return karega. STL ke internal partitioning loops pivot se compare karte waqt rukenge nahi aur memory out-of-bounds cross karke crash ya data corruption kar denge.

### Doubt 07: QuickSort duplicate elements pe $O(n^2)$ kyu ho jata hai aur 3-way partition isse kaise theek karta hai?

**Answer:** Jab saare elements identical hon (`[5, 5, 5, 5]`), to standard Lomuto partition pivot ke barabar elements ko ek hi side phenkta hai, jisse split $0$ aur $n-1$ ka banta hai. Dijkstra ka 3-Way Partition array ko 3 hisson me baant deta hai: `[ < | == | > ]`. Saare identical elements ek hi pass me final ho jate hain aur subsequent recursion me skip ho jate hain, runtime ho jata hai $\Theta(n)$!

### Doubt 08: QuickSort call stack leta hai to in-place kyu kehlata hai, aur log stack kaise ensure karein?

**Answer:** By convention, "in-place" ka matlab hota hai ki sorting ke liye koi $O(n)$ auxiliary array buffer allocate nahi kiya gaya. Lekin recursion stack space leta hai. Agar hum hamesha **smaller partition** par recurse karein aur **larger partition** par `while` loop chalaayein, to recursive depth mathematically maximum $O(\log_2 n)$ hi ho sakti hai.

### Doubt 09: Counting sort me cumulative count 5 ka matlab output index 4 kyu hota hai?

**Answer:** 0-indexed arrays me agar kisi element ke pehle aur barabar total 5 elements hain, to unme sabse aakhiri element index $5 - 1 = 4$ par hi baithega!

### Doubt 10: DNF me 0 move karne ke baad `mid++` karte hain, par 2 move karne ke baad kyu nahi?

**Answer:** `low` pointer ke peeche ka pura region already process ho chuka hota hai (wo sirf 1s the). Isliye jab hum `arr[mid]` ko `arr[low]` se swap karte hain, to mid par aane wala element guaranteed `1` hota hai, jisko dubara check karne ki zaroorat nahi hoti (`mid++`). Lekin `high` se swap hone ke baad jo element mid par aata hai wo unknown region se aata hai — wo 0, 1 ya 2 kuch bhi ho sakta hai! Isliye `mid` ko wahi rok kar dubara inspect karna padta hai.

### Doubt 11: Hoare partition ka returned index pivot ka final position hota hai ya split boundary?

**Answer:** Wo sirf ek **split boundary** hota hai! Pivot element us index par ho bhi sakta hai aur nahi bhi. Isliye Hoare partition ke baad recursive ranges `[low, split]` aur `[split + 1, high]` hoti hain.

### Doubt 12: Same vector me merge karne se auxiliary space $O(1)$ ho jati hai kya?

**Answer:** Nahi! Merge step me agar tum temporary buffer nahi banaoge to unread elements overwrite ho jayenge. Chahe buffer alag function me ho ya class member ho, Merge Sort ko array elements hold karne ke liye linear $O(n)$ memory chahiye hi hoti hai.

### Doubt 13: Next Permutation me suffix ko sort karne ke bajaye reverse kyu karte hain?

**Answer:** Kyunki swap ke pehle suffix already strictly descending (non-increasing) tha. Jab hum pivot ko suffix ke just bade element se swap karte hain, tab bhi bacha hua suffix strictly descending hi rehta hai. Kisi bhi descending array ko ascending banane ke liye sort karne ki zaroorat nahi hoti, use bas 2 pointers se **reverse** kardo ($O(n)$ linear work)!

### Doubt 14: Loop ek hi dikh raha ho to kya algorithm guarantee linear $O(n)$ hota hai?

**Answer:** Bilkul nahi! Loop ke andar index kaise change ho raha hai wo matter karta hai. DNF me single while loop isliye linear hai kyunki har step par unexamined range `high - mid + 1` exact 1 se decrease hoti hai. Agar loop ke andar index reset ho raha ho (jaise `i = 0`), to single loop bhi $O(n^2)$ ho sakta hai.

---

# Part 8: 15-Minute Revision Sheet

## Master Comparison Table

| Algorithm                     | Best Time            | Average Time    | Worst Time      | Space         | Stable? | In-Place? | Adaptive?           | Best Used When                                    |
| ----------------------------- | -------------------- | --------------- | --------------- | ------------- | ------- | --------- | ------------------- | ------------------------------------------------- |
| **Bubble Sort**         | $O(n)$             | $O(n^2)$      | $O(n^2)$      | $O(1)$      | Yes     | Yes       | Yes (with flag)     | Small arrays, education                           |
| **Selection Sort**      | $\Theta(n^2)$      | $\Theta(n^2)$ | $\Theta(n^2)$ | $O(1)$      | No      | Yes       | No                  | Memory write cost is high (only $n$ swaps)      |
| **Insertion Sort**      | $O(n)$             | $O(n^2)$      | $O(n^2)$      | $O(1)$      | Yes     | Yes       | Yes                 | Nearly-sorted data, small arrays ($n \le 32$)   |
| **Counting Sort**       | $\Theta(n+k)$      | $\Theta(n+k)$ | $\Theta(n+k)$ | $O(n+k)$    | Yes     | No        | No                  | Small integer range $k = O(n)$                  |
| **Merge Sort**          | $O(n \log n)$      | $O(n \log n)$ | $O(n \log n)$ | $O(n)$      | Yes     | No        | No (Yes with skip)  | Guaranteed time required, Linked Lists            |
| **Quick Sort (Lomuto)** | $O(n \log n)$      | $O(n \log n)$ | $O(n^2)$      | $O(\log n)$ | No      | Practical | No                  | Fast cache-friendly array sort                    |
| **Quick Sort (3-Way)**  | $O(n)$ (all equal) | $O(n \log n)$ | $O(n^2)$      | $O(\log n)$ | No      | Practical | Yes (on duplicates) | Array has heavy duplicate values                  |
| **Heap Sort**           | $O(n \log n)$      | $O(n \log n)$ | $O(n \log n)$ | $O(1)$      | No      | Yes       | No                  | Guaranteed $O(n \log n)$ with zero extra memory |
| **Radix Sort**          | $O(d(n+b))$        | $O(d(n+b))$   | $O(d(n+b))$   | $O(n+b)$    | Yes     | No        | No                  | Fixed-length integers / strings                   |
| **Bucket Sort**         | $O(n+k)$           | $O(n)$        | $O(n^2)$      | $O(n)$      | Yes     | No        | Yes                 | Uniformly distributed floating point numbers      |
| **Shell Sort**          | $O(n \log n)$      | Depends on gap  | $O(n^{3/2})$  | $O(1)$      | No      | Yes       | Yes                 | Medium arrays without recursion overhead          |

---

## One-Liner Invariants (Exam me bolne ke liye):

- **Bubble Sort:** Pass $i$ ke baad index $n - 1 - i$ par remaining elements ka maximum element final baith jata hai.
- **Selection Sort:** Pass $i$ ke pehle pehle $i$ positions par array ke $i$ smallest elements final sorted order me hote hain.
- **Insertion Sort:** Pass $i$ ke pehle pehle $i$ elements aapas me sorted hote hain, par baad ke elements unhe right shift kar sakte hain.
- **Merge Sort:** Dono sorted halves ke front elements me se jo chota hai, wo pooray remaining elements ka absolute minimum hai.
- **Quick Sort (Lomuto):** Partition ke baad returned index par pivot baith chuka hota hai; left wale sab $\le$, right wale sab $>$.
- **Heap Sort:** Active prefix max-heap maintain karta hai; extracted elements aakhiri me sorted suffix banate hain.
- **DNF:** Chaar regions: `0s [0...low-1]`, `1s [low...mid-1]`, `unknown [mid...high]`, `2s [high+1...n-1]`.

---

# Part 9: Interview Spoken Q&A + Practice Challenges

### Q1: "Quick Sort aur Merge Sort me se real-world arrays ke liye Quick Sort kyu prefer hota hai?"

**Answer:**
Quick Sort in-place array elements ko partition karta hai jisse CPU L1/L2 cache locality bohot high rehti hai aur zero dynamic memory allocation lagti hai. Merge Sort ko har step par auxiliary buffer me copy karna padta hai jisse memory bandwidth double consume hoti hai. Lekin agar stability chahiye ya data Linked List me ho, to Merge Sort hamesha jeet ta hai.

### Q2: "Can you sort an array of $n$ elements in $O(n)$ time?"

**Answer:**
Agar algorithm comparison-based hai to answer is NO (Lower bound is $\Omega(n \log n)$). Lekin agar keys restricted range me hain (jaise integers with small $k$, ya fixed digit length), to Counting Sort ya Radix Sort se $O(n)$ me sort kiya ja sakta hai.

---

### Practice Challenge: Spot the Bug!

Ek developer ne struct sort karne ke liye ye comparator likha:

```cpp
struct Player {
    int id;
    int score;
};

// Intended: Sort by score descending; if equal, by id ascending
bool badComparator(const Player& a, const Player& b){
    if(a.score >= b.score){ // BUG HERE!
        return true;
    }
    return a.id < b.id;
}
```

**Bug Explanation:**
Line `if(a.score >= b.score) return true;` me `>=` likha hai. Agar do players ka score equal ho, to ye function `comp(a, b)` aur `comp(b, a)` dono ko `true` bol dega!
Aur sabse khatarnak: `comp(a, a)` bhi `true` return karega, jo C++ Strict Weak Ordering ke **Irreflexivity rule** ko tod deta hai. Ye code production me `std::sort` ke andar memory access out-of-bounds karke **Crash / Undefined Behavior** karega!
**Correction:**

```cpp
bool goodComparator(const Player& a, const Player& b){
    if(a.score != b.score){
        return a.score > b.score; // Strict comparison!
    }
    return a.id < b.id;           // Strict comparison!
}
```

---

*Created as part of the Unified Notes Learning System.*
