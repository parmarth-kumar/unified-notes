#include <iostream>  // for input and output
#include <vector>    // for dynamic arrays
#include <string>    // for text
#include <algorithm> // for sorting and array helpers
#include <utility>   // for swap, move, and pair
#include <random>    // for random pivot selection
#include <stdexcept> // for reporting invalid input
#include <cmath>     // for checking decimal values
#include <cstddef>   // for size_t, an array-size type
#include <climits>   // for integer limits
#include <cassert>   // for correctness checks
using namespace std;

// ==========================================
// 1. BUBBLE SORT (Adaptive & Stable)
// ==========================================
void bubbleSort(vector<int>& arr){
    int n = arr.size();
    for(int i = 0; i < n - 1; ++i){
        bool swapped = false;
        for(int j = 0; j < n - 1 - i; ++j){
            if(arr[j] > arr[j + 1]){
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if(!swapped){
            break;
        }
    }
}

// ==========================================
// 2. SELECTION SORT (At Most n-1 Swaps & Unstable)
// ==========================================
void selectionSort(vector<int>& arr){
    int n = arr.size();
    for(int i = 0; i < n - 1; ++i){
        int minIdx = i;
        for(int j = i + 1; j < n; ++j){
            if(arr[j] < arr[minIdx]){
                minIdx = j;
            }
        }
        if(minIdx != i){
            swap(arr[i], arr[minIdx]);
        }
    }
}

// ==========================================
// 3. INSERTION SORT (Shift Method & Stable)
// ==========================================
void insertionSort(vector<int>& arr){
    int n = arr.size();
    for(int i = 1; i < n; ++i){
        int key = arr[i];
        int j = i - 1;
        while(j >= 0 && arr[j] > key){
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ==========================================
// 4. COUNTING SORT (Stable, Offset for Negatives, Overflow Guard)
// ==========================================
void countingSort(vector<int>& arr){
    if(arr.size() <= 1){
        return;
    }
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());

    // Safe 64-bit range calculation preventing 32-bit signed overflow
    long long range = static_cast<long long>(maxVal) - minVal + 1;
    if(range > 10000000){
        throw invalid_argument("Value range too large for Counting Sort memory allocation.");
    }

    vector<int> count(range, 0);
    for(int x : arr){
        count[x - minVal]++;
    }

    for(size_t i = 1; i < count.size(); ++i){
        count[i] += count[i - 1];
    }

    vector<int> output(arr.size());
    // Backward traversal preserves stability
    for(int i = static_cast<int>(arr.size()) - 1; i >= 0; --i){
        int bucket = arr[i] - minVal;
        output[count[bucket] - 1] = arr[i];
        count[bucket]--;
    }

    arr = move(output);
}

// ==========================================
// 5. MERGE SORT (Single Buffer & Stable)
// ==========================================
void mergeHelper(vector<int>& arr, int low, int mid, int high, vector<int>& temp){
    int i = low;
    int j = mid + 1;
    int k = low;

    while(i <= mid && j <= high){
        if(arr[i] <= arr[j]){ // '<=' ensures stability
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while(i <= mid){
        temp[k++] = arr[i++];
    }
    while(j <= high){
        temp[k++] = arr[j++];
    }

    for(int p = low; p <= high; ++p){
        arr[p] = temp[p];
    }
}

void mergeSortRec(vector<int>& arr, int low, int high, vector<int>& temp){
    if(low >= high){
        return;
    }
    int mid = low + (high - low) / 2;
    mergeSortRec(arr, low, mid, temp);
    mergeSortRec(arr, mid + 1, high, temp);
    mergeHelper(arr, low, mid, high, temp);
}

void mergeSort(vector<int>& arr){
    if(arr.size() <= 1){
        return;
    }
    vector<int> temp(arr.size());
    mergeSortRec(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}

// ==========================================
// 6. QUICK SORT (Lomuto, Hoare, 3-Way, & Explicit Bounded Stack)
// ==========================================
int lomutoPartition(vector<int>& arr, int low, int high){
    int pivot = arr[high];
    int i = low - 1;
    for(int j = low; j < high; ++j){
        if(arr[j] <= pivot){
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Bounded call stack QuickSort: recurses on smaller partition, loops on larger
void quickSortBoundedStack(vector<int>& arr, int low, int high){
    while(low < high){
        int p = lomutoPartition(arr, low, high);
        if(p - low < high - p){
            quickSortBoundedStack(arr, low, p - 1); // Recurse on smaller left
            low = p + 1;                            // Loop on larger right
        } else {
            quickSortBoundedStack(arr, p + 1, high); // Recurse on smaller right
            high = p - 1;                            // Loop on larger left
        }
    }
}

void quickSort3Way(vector<int>& arr, int low, int high){
    if(low >= high){
        return;
    }
    int pivot = arr[low + (high - low) / 2];
    int lt = low;
    int gt = high;
    int i = low;
    while(i <= gt){
        if(arr[i] < pivot){
            swap(arr[lt++], arr[i++]);
        } else if(arr[i] > pivot){
            swap(arr[i], arr[gt--]);
        } else {
            i++;
        }
    }
    quickSort3Way(arr, low, lt - 1);
    quickSort3Way(arr, gt + 1, high);
}

int hoarePartition(vector<int>& arr, int low, int high){
    int pivot = arr[low + (high - low) / 2]; // Save the value, not a reference
    int i = low - 1;
    int j = high + 1;
    while(true){
        do {
            ++i;
        } while(arr[i] < pivot);
        do {
            --j;
        } while(arr[j] > pivot);
        if(i >= j){
            return j;
        }
        swap(arr[i], arr[j]);
    }
}

void quickSortHoare(vector<int>& arr, int low, int high){
    if(low >= high){
        return;
    }
    int split = hoarePartition(arr, low, high);
    quickSortHoare(arr, low, split);
    quickSortHoare(arr, split + 1, high);
}

// ==========================================
// 7. HEAP SORT (0-Indexed & Sift-Down)
// ==========================================
// Sifts down an element at index i until it satisfies the Max-Heap property
void heapify(vector<int>& arr, int n, int i){
    while(i < n / 2){ // Only parents have children; avoids large-index overflow
        int left = 2 * i + 1;
        int right = left + 1;
        int largest = left;
        if(right < n && arr[right] > arr[left]){
            largest = right;
        }
        if(arr[i] >= arr[largest]){
            break;
        }
        swap(arr[i], arr[largest]);
        i = largest; // Continue downward without recursion
    }
}

void heapSort(vector<int>& arr){
    int n = arr.size();
    if(n <= 1){
        return;
    }

    // Phase 1: Build the Max-Heap in O(n) time
    for(int i = n / 2 - 1; i >= 0; --i){
        heapify(arr, n, i);
    }

    // Phase 2: Extract elements from the heap one by one in O(n log n)
    for(int i = n - 1; i > 0; --i){
        swap(arr[0], arr[i]); // Move current maximum to the end
        heapify(arr, i, 0);   // Restore heap property on reduced heap
    }
}

// ==========================================
// 8. RADIX SORT (Non-Negative Guarded & 64-bit exp Overflow Guard)
// ==========================================
void radixCountingSort(vector<int>& arr, long long exp){
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0};

    // Frequency of current digit
    for(int i = 0; i < n; ++i){
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    // Prefix sums
    for(int i = 1; i < 10; ++i){
        count[i] += count[i - 1];
    }

    // Backward loop preserves stability
    for(int i = n - 1; i >= 0; --i){
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    arr = move(output);
}

void radixSort(vector<int>& arr){
    // Verify precondition: non-negative integers only
    for(int x : arr){
        if(x < 0){
            throw invalid_argument("Radix Sort requires non-negative integers.");
        }
    }

    if(arr.size() <= 1){
        return;
    }

    int maxVal = *max_element(arr.begin(), arr.end());
    // Safe 'long long' exponent prevents signed 32-bit int overflow when maxVal is near INT_MAX
    for(long long exp = 1; maxVal / exp > 0; exp *= 10){
        radixCountingSort(arr, exp);
    }
}

// ==========================================
// 9. BUCKET SORT ([0.0, 1.0) Range Guarded)
// ==========================================
void bucketSort(vector<double>& arr){
    int n = arr.size();

    // Validate precondition: all elements must be in [0.0, 1.0)
    for(double val : arr){
        if(!isfinite(val) || val < 0.0 || val >= 1.0){
            throw invalid_argument(
                "Bucket Sort precondition violated: values must be in [0.0, 1.0).");
        }
    }

    if(n <= 1){
        return;
    }

    vector<vector<double>> buckets(n);

    // 1. Scatter into buckets
    for(double val : arr){
        int idx = min(static_cast<int>(n * val), n - 1);
        buckets[idx].push_back(val);
    }

    // 2. Sort individual buckets
    // Stable insertion sort within each bucket
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

    // 3. Gather back into arr
    int k = 0;
    for(int i = 0; i < n; ++i){
        for(double val : buckets[i]){
            arr[k++] = val;
        }
    }
}

// ==========================================
// 10. SHELL SORT (Knuth's Sequence)
// ==========================================
void shellSort(vector<int>& arr){
    int n = arr.size();
    int h = 1;
    while(h < n / 3){
        h = 3 * h + 1;
    }
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

// ==========================================
// 11. QUICKSELECT (Iterative & Strict O(1) Auxiliary Space)
// ==========================================
// Randomised three-way partition: return the full equal-key band
pair<int, int> partitionQuickSelect(vector<int>& nums, int low, int high){
    static mt19937 rng(random_device{}());
    uniform_int_distribution<int> pick(low, high);
    int pivot = nums[pick(rng)];
    int lt = low, current = low, gt = high;
    while(current <= gt){
        if(nums[current] < pivot){
            swap(nums[lt++], nums[current++]);
        } else if(nums[current] > pivot){
            swap(nums[current], nums[gt--]);
        } else {
            ++current;
        }
    }
    return {lt, gt};
}

int quickSelectIterative(vector<int>& nums, int targetIdx){
    if(targetIdx < 0 || static_cast<size_t>(targetIdx) >= nums.size()){
        throw invalid_argument("Target index out of range");
    }
    int low = 0, high = static_cast<int>(nums.size()) - 1;
    while(low <= high){
        auto [lt, gt] = partitionQuickSelect(nums, low, high);
        if(targetIdx < lt){
            high = lt - 1;
        } else if(targetIdx > gt){
            low = gt + 1;
        } else {
            return nums[targetIdx];
        }
    }
    throw logic_error("Unreachable for a valid target");
}

int findKthLargest(vector<int>& nums, int k){
    if(k < 1 || static_cast<size_t>(k) > nums.size()){
        throw invalid_argument("Require 1 <= k <= size");
    }
    return quickSelectIterative(nums, static_cast<int>(nums.size()) - k);
}

// ==========================================
// 12. COUNT INVERSIONS (Merge Sort Inversion Counter)
// ==========================================
long long mergeAndCount(vector<int>& arr, int low, int mid, int high, vector<int>& temp){
    int i = low;
    int j = mid + 1;
    int k = low;
    long long count = 0;

    while(i <= mid && j <= high){
        if(arr[i] <= arr[j]){
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            count += (mid - i + 1);
        }
    }
    while(i <= mid){
        temp[k++] = arr[i++];
    }
    while(j <= high){
        temp[k++] = arr[j++];
    }

    for(int p = low; p <= high; ++p){
        arr[p] = temp[p];
    }
    return count;
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

long long countInversions(vector<int>& arr){
    if(arr.size() <= 1){
        return 0;
    }
    vector<int> temp(arr.size());
    return countInversionsRec(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}

// ==========================================
// STABILITY STRUCTS & SUBROUTINE-DEPENDENT BUCKET SORT
// ==========================================
struct Record {
    int key;
    int id; // Tracks original input order
    bool operator==(const Record& other) const {
        return key == other.key && id == other.id;
    }
};

struct FloatRecord {
    double val;
    int id;
    bool operator==(const FloatRecord& o) const {
        return fabs(val - o.val) < 1e-9 && id == o.id;
    }
};

// Stable Bucket Sort using stable_sort within buckets
void bucketSortStable(vector<FloatRecord>& arr){
    int n = arr.size();
    if(n <= 1){
        return;
    }
    vector<vector<FloatRecord>> buckets(n);
    for(const auto& r : arr){
        int idx = min(static_cast<int>(n * r.val), n - 1);
        buckets[idx].push_back(r);
    }
    for(int i = 0; i < n; ++i){
        stable_sort(buckets[i].begin(), buckets[i].end(),
                    [](const FloatRecord& a, const FloatRecord& b){ return a.val < b.val; });
    }
    int idx = 0;
    for(int i = 0; i < n; ++i){
        for(const auto& r : buckets[i]){
            arr[idx++] = r;
        }
    }
}

// ==========================================
// COMPREHENSIVE TEST SUITE
// ==========================================
int main(){
    cout << "=====================================================" << endl;
    cout << "RUNNING THOROUGH SORTING VERIFICATION & AUDIT SUITE" << endl;
    cout << "=====================================================" << endl;

    // 1. General Integer Sorting Tests (Empty, Single, Duplicates, Negatives, Sorted, Reverse)
    vector<vector<int>> generalCases = {
        {},                                 // Empty array
        {42},                               // Single element
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},    // Already sorted
        {10, 9, 8, 7, 6, 5, 4, 3, 2, 1},    // Reverse sorted
        {7, 7, 7, 7, 7, 7, 7, 7},           // All duplicates
        {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5},  // Mixed with duplicates
        {-5, 12, -3, 0, 8, -15, 20, 1, -1}, // Negatives, zero, positives
        {100, -100, 50, -50, 0, 25, -25},   // Symmetric negative values
        {-500, -250, 0, 250, 500}           // Bounded negative range
    };

    // Extreme INT_MIN / INT_MAX boundaries for comparison-based sorts
    vector<vector<int>> extremeComparisonCases = {
        {INT_MAX, INT_MIN, 0, -1, 1, INT_MAX - 1, INT_MIN + 1},
        {INT_MIN, INT_MAX},
        {INT_MAX, INT_MAX, INT_MIN, INT_MIN}};

    auto testComparisonSort = [&](const string& name, auto sortFn){
        cout << "[TEST] " << name << " ... ";
        // Test on general cases (empty, single, sorted, reverse, duplicates, negatives)
        for(const auto& original : generalCases){
            vector<int> arr = original;
            sortFn(arr);
            vector<int> expected = original;
            sort(expected.begin(), expected.end());
            assert(arr == expected);
        }
        // Test on extreme INT_MIN / INT_MAX boundary cases
        for(const auto& original : extremeComparisonCases){
            vector<int> arr = original;
            sortFn(arr);
            vector<int> expected = original;
            sort(expected.begin(), expected.end());
            assert(arr == expected);
        }
        cout << "PASSED!" << endl;
    };

    testComparisonSort("Bubble Sort (Adaptive)", bubbleSort);
    testComparisonSort("Selection Sort", selectionSort);
    testComparisonSort("Insertion Sort (Shift)", insertionSort);
    testComparisonSort("Merge Sort (Single Buffer)", mergeSort);
    testComparisonSort("Quick Sort (Bounded Stack)", [](vector<int>& arr){
        if(!arr.empty()){
            quickSortBoundedStack(arr, 0, arr.size() - 1);
        }
    });
    testComparisonSort("Quick Sort (3-Way DNF)", [](vector<int>& arr){
        if(!arr.empty()){
            quickSort3Way(arr, 0, arr.size() - 1);
        }
    });
    testComparisonSort("Quick Sort (Hoare)", [](vector<int>& arr){
        quickSortHoare(arr, 0, static_cast<int>(arr.size()) - 1);
    });
    testComparisonSort("Heap Sort (0-indexed, iterative)", heapSort);
    testComparisonSort("Shell Sort (Knuth Sequence)", shellSort);

    // Counting Sort on general cases
    cout << "[TEST] Counting Sort (Stable with Offset) ... ";
    for(const auto& original : generalCases){
        vector<int> arr = original;
        countingSort(arr);
        vector<int> expected = original;
        sort(expected.begin(), expected.end());
        assert(arr == expected);
    }
    cout << "PASSED!" << endl;

    // 2. Precondition & Edge-Case Guard Verification
    cout << "[TEST] Counting Sort Range Overflow & Impractical Range Guards ... ";
    {
        vector<int> hugeRange = {0, 20000000}; // range 20,000,001 > 10,000,000
        bool caught1 = false;
        try {
            countingSort(hugeRange);
        } catch(const invalid_argument&){
            caught1 = true;
        }
        assert(caught1);

        vector<int> intMinMax = {INT_MIN, INT_MAX}; // 4*10^9 range > 10M
        bool caught2 = false;
        try {
            countingSort(intMinMax);
        } catch(const invalid_argument&){
            caught2 = true;
        }
        assert(caught2);
        cout << "PASSED (Correctly rejected > 10M range and INT_MIN/INT_MAX)!" << endl;
    }

    cout << "[TEST] Radix Sort Large Numbers (Near INT_MAX) & Non-Negative Precondition ... ";
    {
        vector<int> largeRadix = {2000000000, 42, 1000000000, 1999999999, 0, 7};
        vector<int> expLarge = largeRadix;
        sort(expLarge.begin(), expLarge.end());
        radixSort(largeRadix);
        assert(largeRadix == expLarge);

        vector<int> validRadix = {170, 45, 75, 90, 802, 24, 2, 66};
        vector<int> expRadix = validRadix;
        sort(expRadix.begin(), expRadix.end());
        radixSort(validRadix);
        assert(validRadix == expRadix);

        vector<int> invalidRadix = {10, -5, 20};
        bool caught = false;
        try {
            radixSort(invalidRadix);
        } catch(const invalid_argument&){
            caught = true;
        }
        assert(caught);
        cout << "PASSED (Safe 64-bit exp handled 2*10^9 & rejected negatives)!" << endl;
    }

    cout << "[TEST] Bucket Sort Range [0.0, 1.0) Precondition ... ";
    {
        vector<double> validBucket = {0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21, 0.12};
        vector<double> expBucket = validBucket;
        sort(expBucket.begin(), expBucket.end());
        bucketSort(validBucket);
        for(size_t i = 0; i < validBucket.size(); ++i){
            assert(fabs(validBucket[i] - expBucket[i]) < 1e-9);
        }

        vector<double> outOfBounds = {0.5, 1.5, 0.2};
        bool caught = false;
        try {
            bucketSort(outOfBounds);
        } catch(const invalid_argument&){
            caught = true;
        }
        assert(caught);
        cout << "PASSED (Correctly rejected values >= 1.0)!" << endl;
    }

    // 3. Multi-Key Stability Verification on Custom Records
    cout << "[TEST] Multi-Key Stability Verification ... ";
    {
        vector<Record> input = {{3, 1}, {1, 2}, {3, 3}, {2, 4}, {3, 5}, {1, 6}};

        // Test with stable_sort
        vector<Record> stableArr = input;
        stable_sort(stableArr.begin(), stableArr.end(),
                    [](const Record& a, const Record& b){ return a.key < b.key; });

        // Key 1: original ids must be 2, then 6
        assert(stableArr[0].key == 1 && stableArr[0].id == 2);
        assert(stableArr[1].key == 1 && stableArr[1].id == 6);
        // Key 2: id 4
        assert(stableArr[2].key == 2 && stableArr[2].id == 4);
        // Key 3: original ids must be 1, then 3, then 5
        assert(stableArr[3].key == 3 && stableArr[3].id == 1);
        assert(stableArr[4].key == 3 && stableArr[4].id == 3);
        assert(stableArr[5].key == 3 && stableArr[5].id == 5);
        cout << "PASSED (Relative ordering strictly preserved)!" << endl;
    }

    // 4. Bucket Sort Subroutine-Dependent Stability Verification
    cout << "[TEST] Bucket Sort Subroutine-Dependent Stability Verification ... ";
    {
        vector<FloatRecord> floatData = {{0.42, 1}, {0.15, 2}, {0.42, 3},
                                         {0.88, 4}, {0.15, 5}, {0.42, 6}};
        bucketSortStable(floatData);

        // Value 0.15: ID 2 before ID 5
        assert(fabs(floatData[0].val - 0.15) < 1e-9 && floatData[0].id == 2);
        assert(fabs(floatData[1].val - 0.15) < 1e-9 && floatData[1].id == 5);
        // Value 0.42: ID 1 before ID 3 before ID 6
        assert(fabs(floatData[2].val - 0.42) < 1e-9 && floatData[2].id == 1);
        assert(fabs(floatData[3].val - 0.42) < 1e-9 && floatData[3].id == 3);
        assert(fabs(floatData[4].val - 0.42) < 1e-9 && floatData[4].id == 6);
        // Value 0.88: ID 4
        assert(fabs(floatData[5].val - 0.88) < 1e-9 && floatData[5].id == 4);
        cout << "PASSED (stable_sort inside buckets preserves stability)!" << endl;
    }

    // 5. C++ STL Selection Functions
    cout << "[TEST] partial_sort & nth_element Verification ... ";
    {
        vector<int> pArr = {10, 2, 8, 6, 7, 3, 1, 5};
        partial_sort(pArr.begin(), pArr.begin() + 3, pArr.end());
        assert(pArr[0] == 1 && pArr[1] == 2 && pArr[2] == 3);

        vector<int> nArr = {10, 2, 8, 6, 7, 3, 1, 5};
        nth_element(nArr.begin(), nArr.begin() + 3, nArr.end());
        assert(nArr[3] == 5); // 4th smallest element
        for(int i = 0; i < 3; ++i){
            assert(nArr[i] <= nArr[3]);
        }
        for(size_t i = 4; i < nArr.size(); ++i){
            assert(nArr[i] >= nArr[3]);
        }
        cout << "PASSED!" << endl;
    }

    // 6. Iterative QuickSelect Verification (O(1) aux space & <random>)
    cout << "[TEST] Iterative QuickSelect (Strict O(1) Aux Space & <random>) Verification ... ";
    {
        vector<int> qNums = {3, 2, 1, 5, 6, 4};
        // 2nd largest in [3, 2, 1, 5, 6, 4] is 5
        int ans1 = findKthLargest(qNums, 2);
        assert(ans1 == 5);

        vector<int> qNums2 = {3, 2, 3, 1, 2, 4, 5, 5, 6};
        // 4th largest in [3, 2, 3, 1, 2, 4, 5, 5, 6] is 4
        int ans2 = findKthLargest(qNums2, 4);
        assert(ans2 == 4);

        // 1st largest (maximum)
        int ansMax = findKthLargest(qNums, 1);
        assert(ansMax == 6);

        // k == n (minimum)
        int ansMin = findKthLargest(qNums, qNums.size());
        assert(ansMin == 1);

        // Negative numbers and duplicates
        vector<int> qNeg = {-10, 0, -5, 20, 15, -10, 20};
        // Sorted: [-10, -10, -5, 0, 15, 20, 20]
        assert(findKthLargest(qNeg, 1) == 20);  // 1st largest: 20
        assert(findKthLargest(qNeg, 3) == 15);  // 3rd largest: 15
        assert(findKthLargest(qNeg, 7) == -10); // 7th largest (min): -10
        cout << "PASSED!" << endl;
    }

    // 7. Inversion Counting Verification
    cout << "[TEST] Inversion Counting (Merge Sort Algorithm) Verification ... ";
    {
        vector<int> inv1 = {2, 4, 1, 3, 5};
        assert(countInversions(inv1) == 3);

        vector<int> inv2 = {5, 4, 3, 2, 1};
        assert(countInversions(inv2) == 10);

        vector<int> inv3 = {1, 2, 3, 4, 5};
        assert(countInversions(inv3) == 0);

        vector<int> inv4 = {1};
        assert(countInversions(inv4) == 0);
        cout << "PASSED!" << endl;
    }

    // 8. Strict Weak Ordering Irreflexivity Check
    cout << "[TEST] Strict Weak Ordering Irreflexivity Check ... ";
    {
        auto validComp = [](int a, int b){ return a < b; };
        assert(validComp(42, 42) == false); // Irreflexivity holds!
        cout << "PASSED (comp(x, x) == false verified)!" << endl;
    }

    cout << "=====================================================" << endl;
    cout << "ALL VERIFICATION AND AUDIT TESTS PASSED SUCCESSFULLY!" << endl;
    cout << "=====================================================" << endl;
    return 0;
}
