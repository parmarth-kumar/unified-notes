/**
 * ============================================================================
 * UNIFIED NOTES — MASTER SORTING TEST HARNESS (C++17)
 * ============================================================================
 * File: sorting_test_harness.cpp
 * Language: C++ (with Hinglish comments in Roman English script)
 * 
 * KAISE CHALANA HAI (How to run):
 * 1. Compile:
 *    g++ -std=c++17 -O2 sorting_test_harness.cpp -o sorting_test_harness.exe
 * 2. Run:
 *    .\sorting_test_harness.exe
 * 
 * AAP ISME KYA KAR SAKTE HO:
 * - Section 1 me saare sorting algorithms already implemented hain.
 * - Aap kisi bhi function ko delete karke apna khud ka code likh sakte ho.
 * - Test harness automatically aapke code ko `std::sort` ke saath compare karega,
 *   edge cases check karega, mismatch par error dikhayega, aur speed benchmark karega!
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <chrono>
#include <random>
#include <iomanip>
#include <stdexcept>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ============================================================================
// ✍️ SECTION 1: APNE SORTING ALGORITHMS YAHAN LIKHO / EDIT KARO
// ============================================================================

// 1. Bubble Sort (Adjacent swaps, with swapped flag early exit)
void myBubbleSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    for(int i = 0; i < n - 1; ++i){
        bool swapped = false;
        for(int j = 0; j < n - 1 - i; ++j){
            if(arr[j] > arr[j + 1]){
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if(!swapped) break;
    }
}

// 2. Selection Sort (Minimum find karo aur swap karo)
void mySelectionSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

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

// 3. Insertion Sort (Key element ko peeche shift karke sahi jagah insert karo)
void myInsertionSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    for(int i = 1; i < n; ++i){
        int key = arr[i];
        int j = i - 1;
        while(j >= 0 && arr[j] > key){
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// 4. Merge Sort (Divide & Conquer with Linear 2-Pointer Merge)
namespace InternalMergeSort {
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

    void sortHelper(vector<int>& arr, int low, int high, vector<int>& temp){
        if(low >= high) return;
        int mid = low + (high - low) / 2;
        sortHelper(arr, low, mid, temp);
        sortHelper(arr, mid + 1, high, temp);
        if(arr[mid] <= arr[mid + 1]) return; // Skip merge if already ordered
        merge(arr, low, mid, high, temp);
    }
}

void myMergeSort(vector<int>& arr){
    if(arr.size() <= 1) return;
    vector<int> temp(arr.size());
    InternalMergeSort::sortHelper(arr, 0, static_cast<int>(arr.size()) - 1, temp);
}

// 5. Quick Sort (Lomuto Partition with bounded stack)
namespace InternalQuickSort {
    int partition(vector<int>& arr, int low, int high){
        int pivot = arr[high];
        int i = low - 1;
        for(int j = low; j < high; ++j){
            if(arr[j] <= pivot){
                ++i;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    void sortHelper(vector<int>& arr, int low, int high){
        while(low < high){
            int pIdx = partition(arr, low, high);
            // Smaller partition pe recurse karo taaki stack bounded to O(log n) rahe
            if(pIdx - low < high - pIdx){
                sortHelper(arr, low, pIdx - 1);
                low = pIdx + 1;
            } else {
                sortHelper(arr, pIdx + 1, high);
                high = pIdx - 1;
            }
        }
    }
}

void myQuickSort(vector<int>& arr){
    if(arr.size() <= 1) return;
    InternalQuickSort::sortHelper(arr, 0, static_cast<int>(arr.size()) - 1);
}

// 6. Quick Sort 3-Way (Dijkstra's Dutch National Flag for heavy duplicates)
namespace InternalQuickSort3Way {
    void sortHelper(vector<int>& arr, int low, int high){
        if(low >= high) return;
        int pivot = arr[low + (high - low) / 2];
        int lt = low, gt = high, i = low;
        while(i <= gt){
            if(arr[i] < pivot) swap(arr[lt++], arr[i++]);
            else if(arr[i] > pivot) swap(arr[i], arr[gt--]);
            else ++i;
        }
        sortHelper(arr, low, lt - 1);
        sortHelper(arr, gt + 1, high);
    }
}

void myQuickSort3Way(vector<int>& arr){
    if(arr.size() <= 1) return;
    InternalQuickSort3Way::sortHelper(arr, 0, static_cast<int>(arr.size()) - 1);
}

// 7. Heap Sort (Max-Heap with iterative sift-down)
namespace InternalHeapSort {
    void siftDown(vector<int>& arr, int n, int i){
        while(i < n / 2){
            int left = 2 * i + 1;
            int right = left + 1;
            int largest = left;
            if(right < n && arr[right] > arr[left]) largest = right;
            if(arr[i] >= arr[largest]) break;
            swap(arr[i], arr[largest]);
            i = largest;
        }
    }
}

void myHeapSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    // Phase 1: Build Max-Heap in O(n)
    for(int i = n / 2 - 1; i >= 0; --i){
        InternalHeapSort::siftDown(arr, n, i);
    }

    // Phase 2: Extract maximum one by one
    for(int i = n - 1; i > 0; --i){
        swap(arr[0], arr[i]);
        InternalHeapSort::siftDown(arr, i, 0);
    }
}

// 8. Counting Sort (Handles negative numbers with minVal offset)
void myCountingSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    long long range = static_cast<long long>(maxVal) - minVal + 1;

    if(range > 5000000){
        // Range too large for memory
        sort(arr.begin(), arr.end());
        return;
    }

    vector<int> count(range, 0);
    vector<int> output(n);

    for(int x : arr) count[x - minVal]++;
    for(size_t i = 1; i < count.size(); ++i) count[i] += count[i - 1];

    for(int i = n - 1; i >= 0; --i){
        int val = arr[i];
        output[count[val - minVal] - 1] = val;
        count[val - minVal]--;
    }
    arr = move(output);
}

// 9. Radix Sort (LSD Base 10, non-negative integers only)
void myRadixSort(vector<int>& arr){
    if(arr.size() <= 1) return;
    int n = static_cast<int>(arr.size());

    // Radix precondition check: non-negative
    for(int x : arr){
        if(x < 0){
            // Fallback for negatives in general test harness
            sort(arr.begin(), arr.end());
            return;
        }
    }

    int maxVal = *max_element(arr.begin(), arr.end());
    vector<int> output(n);

    for(long long exp = 1; maxVal / exp > 0; exp *= 10){
        int count[10] = {0};
        for(int i = 0; i < n; ++i){
            int digit = (arr[i] / exp) % 10;
            count[digit]++;
        }
        for(int i = 1; i < 10; ++i) count[i] += count[i - 1];
        for(int i = n - 1; i >= 0; --i){
            int digit = (arr[i] / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }
        arr = output;
    }
}

// 10. Shell Sort (Knuth Gap Sequence: 1, 4, 13, 40...)
void myShellSort(vector<int>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    int h = 1;
    while(h < n / 3) h = 3 * h + 1;

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

// 11. Bucket Sort (For double values in [0.0, 1.0))
void myBucketSort(vector<double>& arr){
    int n = static_cast<int>(arr.size());
    if(n <= 1) return;

    vector<vector<double>> buckets(n);
    for(double x : arr){
        int idx = min(static_cast<int>(n * x), n - 1);
        buckets[idx].push_back(x);
    }

    for(auto& b : buckets){
        for(size_t i = 1; i < b.size(); ++i){
            double key = b[i];
            size_t j = i;
            while(j > 0 && b[j - 1] > key){
                b[j] = b[j - 1];
                --j;
            }
            b[j] = key;
        }
    }

    int k = 0;
    for(int i = 0; i < n; ++i){
        for(double x : buckets[i]){
            arr[k++] = x;
        }
    }
}


// ============================================================================
// 🧪 SECTION 2: TEST HARNESS ENGINE & BENCHMARKING SYSTEM
// ============================================================================

// Terminal Color Helper
namespace TermColor {
    void enableVirtualTerminal(){
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if(hOut != INVALID_HANDLE_VALUE){
            DWORD dwMode = 0;
            if(GetConsoleMode(hOut, &dwMode)){
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }
    const string RESET   = "\033[0m";
    const string RED     = "\033[31m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN    = "\033[36m";
    const string BOLD    = "\033[1m";
}

// Test Case Representation
struct TestCase {
    string name;
    vector<int> input;
};

// Generates Standard & Tricky Edge Cases
vector<TestCase> generateTestSuites(bool allowNegatives = true){
    vector<TestCase> suites;

    // 1. Edge Case: Empty Array
    suites.push_back({"Empty Array []", {}});

    // 2. Edge Case: Single Element
    suites.push_back({"Single Element [42]", {42}});

    // 3. Edge Case: Two Elements Sorted & Inverted
    suites.push_back({"Two Elements Sorted [10, 20]", {10, 20}});
    suites.push_back({"Two Elements Inverted [20, 10]", {20, 10}});

    // 4. Already Sorted Array
    suites.push_back({"Already Sorted [1, 2, 3, 4, 5, 6, 7]", {1, 2, 3, 4, 5, 6, 7}});

    // 5. Strictly Reverse Sorted Array
    suites.push_back({"Reverse Sorted [9, 8, 7, 6, 5, 4, 3, 2, 1]", {9, 8, 7, 6, 5, 4, 3, 2, 1}});

    // 6. Heavy Duplicates & Plateaus
    suites.push_back({"Heavy Duplicates [4, 1, 4, 2, 1, 4, 2, 3]", {4, 1, 4, 2, 1, 4, 2, 3}});

    // 7. All Elements Identical
    suites.push_back({"All Identical [7, 7, 7, 7, 7, 7]", {7, 7, 7, 7, 7, 7}});

    // 8. Negative & Zero Values
    if(allowNegatives){
        suites.push_back({"Mixed Negatives [-8, 4, 0, -2, 9, -15, 3]", {-8, 4, 0, -2, 9, -15, 3}});
        suites.push_back({"Only Negatives [-1, -5, -2, -9, -3]", {-1, -5, -2, -9, -3}});
    } else {
        suites.push_back({"Positive Integers [105, 42, 9, 87, 0, 314]", {105, 42, 9, 87, 0, 314}});
    }

    // 9. Random Array (50 elements)
    mt19937 rng(1337);
    vector<int> randArr(50);
    uniform_int_distribution<int> dist(allowNegatives ? -100 : 0, 500);
    for(int& x : randArr) x = dist(rng);
    suites.push_back({"Random Array (Size 50)", randArr});

    // 10. Large Stress Test (2,000 elements)
    vector<int> stressArr(2000);
    uniform_int_distribution<int> stressDist(allowNegatives ? -10000 : 0, 10000);
    for(int& x : stressArr) x = stressDist(rng);
    suites.push_back({"Stress Test Array (Size 2,000)", stressArr});

    return suites;
}

// Pretty print a small vector
void printVectorSlice(const vector<int>& v, size_t maxCount = 8){
    cout << "[";
    for(size_t i = 0; i < min(v.size(), maxCount); ++i){
        cout << v[i] << (i + 1 < min(v.size(), maxCount) ? ", " : "");
    }
    if(v.size() > maxCount) cout << ", ... (" << v.size() << " items)";
    cout << "]";
}

// Function pointer typedef for sorting algorithms
using SortFunction = void (*)(vector<int>&);

// Result of running a single sorting algorithm
struct TestSummary {
    string name;
    int passed = 0;
    int total = 0;
    double totalTimeMs = 0.0;
};

// Runner function for any integer sorting algorithm
TestSummary runTestSuite(const string& algoName, SortFunction func, bool allowNegatives = true){
    using namespace TermColor;
    cout << "\n" << BOLD << CYAN << "=======================================================" << RESET << "\n";
    cout << BOLD << " TESTING: " << YELLOW << algoName << RESET << "\n";
    cout << BOLD << CYAN << "=======================================================" << RESET << "\n";

    auto suites = generateTestSuites(allowNegatives);
    TestSummary summary;
    summary.name = algoName;
    summary.total = static_cast<int>(suites.size());

    for(const auto& test : suites){
        vector<int> userArr = test.input;
        vector<int> expectedArr = test.input;

        // Ground truth via std::sort
        sort(expectedArr.begin(), expectedArr.end());

        // Measure execution time
        auto start = chrono::high_resolution_clock::now();
        func(userArr);
        auto end = chrono::high_resolution_clock::now();
        double elapsedUs = chrono::duration<double, micro>(end - start).count();
        summary.totalTimeMs += (elapsedUs / 1000.0);

        // Verification check
        if(userArr == expectedArr){
            summary.passed++;
            cout << "  " << GREEN << "[PASS]" << RESET << " " << left << setw(38) << test.name 
                 << " (" << fixed << setprecision(1) << elapsedUs << " us)\n";
        } else {
            cout << "  " << RED << BOLD << "[FAIL]" << RESET << " " << test.name << "\n";
            cout << "         " << RED << "Input:    " << RESET; printVectorSlice(test.input); cout << "\n";
            cout << "         " << GREEN << "Expected: " << RESET; printVectorSlice(expectedArr); cout << "\n";
            cout << "         " << RED << "Actual:   " << RESET; printVectorSlice(userArr); cout << "\n";

            // Identify first mismatch
            for(size_t i = 0; i < min(userArr.size(), expectedArr.size()); ++i){
                if(userArr[i] != expectedArr[i]){
                    cout << "         " << YELLOW << "-> First Mismatch at index " << i 
                         << " (Expected " << expectedArr[i] << ", Got " << userArr[i] << ")" << RESET << "\n";
                    break;
                }
            }
        }
    }

    cout << "-------------------------------------------------------\n";
    if(summary.passed == summary.total){
        cout << BOLD << GREEN << " [RESULT] ALL " << summary.total << " TESTS PASSED! " 
             << "(Total time: " << fixed << setprecision(2) << summary.totalTimeMs << " ms)" << RESET << "\n";
    } else {
        cout << BOLD << RED << " [RESULT] " << summary.passed << "/" << summary.total 
             << " Tests Passed. (" << (summary.total - summary.passed) << " Failed)" << RESET << "\n";
    }
    return summary;
}

// Special runner for Bucket Sort (Doubles in [0.0, 1.0))
void runBucketSortTest(){
    using namespace TermColor;
    cout << "\n" << BOLD << CYAN << "=======================================================" << RESET << "\n";
    cout << BOLD << " TESTING: " << YELLOW << "Bucket Sort (Doubles in [0.0, 1.0))" << RESET << "\n";
    cout << BOLD << CYAN << "=======================================================" << RESET << "\n";

    vector<vector<double>> tests = {
        {},
        {0.42},
        {0.75, 0.12},
        {0.1, 0.2, 0.3, 0.4},
        {0.9, 0.7, 0.5, 0.3, 0.1},
        {0.25, 0.25, 0.25},
        {0.89, 0.56, 0.65, 0.12, 0.66, 0.34, 0.18, 0.99, 0.05}
    };

    int passed = 0;
    for(size_t i = 0; i < tests.size(); ++i){
        vector<double> userArr = tests[i];
        vector<double> expected = tests[i];
        sort(expected.begin(), expected.end());

        auto start = chrono::high_resolution_clock::now();
        myBucketSort(userArr);
        auto end = chrono::high_resolution_clock::now();
        double elapsedUs = chrono::duration<double, micro>(end - start).count();

        if(userArr == expected){
            passed++;
            cout << "  " << GREEN << "[PASS]" << RESET << " Bucket Test #" << (i + 1) 
                 << " (Size " << tests[i].size() << ") - " << fixed << setprecision(1) << elapsedUs << " us\n";
        } else {
            cout << "  " << RED << "[FAIL]" << RESET << " Bucket Test #" << (i + 1) << "\n";
        }
    }
    cout << "-------------------------------------------------------\n";
    if(passed == static_cast<int>(tests.size())){
        cout << BOLD << GREEN << " [RESULT] ALL BUCKET SORT TESTS PASSED!" << RESET << "\n";
    } else {
        cout << BOLD << RED << " [RESULT] " << passed << "/" << tests.size() << " Passed." << RESET << "\n";
    }
}

// Runs all algorithms and prints a performance comparison leaderboard
void runAllAlgorithmsBenchmark(){
    using namespace TermColor;

    vector<pair<string, SortFunction>> algorithms = {
        {"Bubble Sort (Optimized)", myBubbleSort},
        {"Selection Sort", mySelectionSort},
        {"Insertion Sort", myInsertionSort},
        {"Merge Sort", myMergeSort},
        {"Quick Sort (Lomuto Bounded)", myQuickSort},
        {"Quick Sort (3-Way DNF)", myQuickSort3Way},
        {"Heap Sort (Iterative)", myHeapSort},
        {"Counting Sort", myCountingSort},
        {"Radix Sort (LSD Base 10)", myRadixSort},
        {"Shell Sort (Knuth Gaps)", myShellSort}
    };

    vector<TestSummary> summaries;
    for(auto& p : algorithms){
        bool allowNeg = (p.first.find("Radix") == string::npos);
        summaries.push_back(runTestSuite(p.first, p.second, allowNeg));
    }
    runBucketSortTest();

    // Summary Leaderboard
    cout << "\n" << BOLD << MAGENTA << "=======================================================" << RESET << "\n";
    cout << BOLD << MAGENTA << "          🏆 SPEED BENCHMARK LEADERBOARD 🏆           " << RESET << "\n";
    cout << BOLD << MAGENTA << "=======================================================" << RESET << "\n";
    cout << left << setw(30) << "Algorithm Name" 
         << setw(16) << "Tests Passed" 
         << setw(15) << "Total Time" << "\n";
    cout << "-------------------------------------------------------\n";

    // Sort leaderboard by fastest total time
    vector<TestSummary> sortedSummaries = summaries;
    sort(sortedSummaries.begin(), sortedSummaries.end(), [](const TestSummary& a, const TestSummary& b){
        return a.totalTimeMs < b.totalTimeMs;
    });

    for(size_t rank = 0; rank < sortedSummaries.size(); ++rank){
        const auto& s = sortedSummaries[rank];
        string statusColor = (s.passed == s.total) ? GREEN : RED;
        cout << left << setw(30) << s.name 
             << statusColor << setw(16) << (to_string(s.passed) + "/" + to_string(s.total)) << RESET
             << YELLOW << fixed << setprecision(3) << s.totalTimeMs << " ms" << RESET;
        if(rank == 0) cout << BOLD << " ⚡ [FASTEST]" << RESET;
        cout << "\n";
    }
    cout << "=======================================================\n";
}

// Display Interactive CLI Menu
void showInteractiveMenu(){
    using namespace TermColor;

    while(true){
        cout << "\n" << BOLD << CYAN << "=======================================================" << RESET << "\n";
        cout << BOLD << "       UNIFIED NOTES — SORTING TEST HARNESS MENU       " << RESET << "\n";
        cout << BOLD << CYAN << "=======================================================" << RESET << "\n";
        cout << " 1. Run ALL Sorting Tests + Speed Benchmark (Sabhi check karo)\n";
        cout << " 2. Test Bubble Sort\n";
        cout << " 3. Test Selection Sort\n";
        cout << " 4. Test Insertion Sort\n";
        cout << " 5. Test Merge Sort\n";
        cout << " 6. Test Quick Sort (Lomuto Bounded)\n";
        cout << " 7. Test Quick Sort 3-Way (Duplicate Handling)\n";
        cout << " 8. Test Heap Sort\n";
        cout << " 9. Test Counting Sort\n";
        cout << "10. Test Radix Sort (LSD)\n";
        cout << "11. Test Shell Sort (Knuth)\n";
        cout << "12. Test Bucket Sort\n";
        cout << " 0. Exit (Bahir niklo)\n";
        cout << "-------------------------------------------------------\n";
        cout << BOLD << "Enter your choice (0-12): " << RESET;

        int choice = -1;
        if(!(cin >> choice)){
            break;
        }

        switch(choice){
            case 1:  runAllAlgorithmsBenchmark(); break;
            case 2:  runTestSuite("Bubble Sort", myBubbleSort); break;
            case 3:  runTestSuite("Selection Sort", mySelectionSort); break;
            case 4:  runTestSuite("Insertion Sort", myInsertionSort); break;
            case 5:  runTestSuite("Merge Sort", myMergeSort); break;
            case 6:  runTestSuite("Quick Sort (Lomuto Bounded)", myQuickSort); break;
            case 7:  runTestSuite("Quick Sort 3-Way", myQuickSort3Way); break;
            case 8:  runTestSuite("Heap Sort", myHeapSort); break;
            case 9:  runTestSuite("Counting Sort", myCountingSort); break;
            case 10: runTestSuite("Radix Sort (Non-negative)", myRadixSort, false); break;
            case 11: runTestSuite("Shell Sort", myShellSort); break;
            case 12: runBucketSortTest(); break;
            case 0:
                cout << "\n" << GREEN << "Good luck with your DSA preparation! Happy Coding :)\n" << RESET;
                return;
            default:
                cout << RED << "Invalid choice! Please choose between 0 and 12.\n" << RESET;
        }
    }
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(int argc, char* argv[]){
    TermColor::enableVirtualTerminal();

    // Agar command line argument "all" diya ho: non-interactive mode me sabhi run karo
    if(argc > 1 && string(argv[1]) == "all"){
        runAllAlgorithmsBenchmark();
        return 0;
    }

    showInteractiveMenu();
    return 0;
}
