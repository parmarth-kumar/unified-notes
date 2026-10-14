"""Compile and test the current complete C++ implementations in the notes.

Run: python DSA/Sorting/verify_notes.py
Requires g++ and node on PATH. Generated files live in a temporary directory.
Broken debugging examples and revision fragments are deliberately excluded.
"""

from pathlib import Path
import re
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent


def extract(filename, declaration):
    blocks = re.findall(r"```cpp\s*\n(.*?)\n```", (ROOT / filename).read_text(encoding="utf-8"), re.S)
    matches = [block for block in blocks if declaration in block]
    if len(matches) != 1:
        raise RuntimeError(f"Expected one code block for {filename}: {declaration}; found {len(matches)}")
    return matches[0]


SPECS = [
    ("theory.md", "bool isSortedAscending(", []),
    ("theory.md", "void bubbleSort(", []),
    ("theory.md", "void selectionSort(", []),
    ("theory.md", "void insertionSort(", []),
    ("theory.md", "void countingSortValues(", []),
    ("theory.md", "void countingSort(", []),
    ("theory.md", "void mergeSort(", []),
    ("theory.md", "int partition(", []),
    ("theory.md", "void quickSortLogarithmicStack(", [("theory.md", "int partition(")]),
    ("theory.md", "int hoarePartition(", []),
    ("theory.md", "void quickSort3Way(", [("theory.md", "int partition(")]),
    ("theory.md", "void heapSort(", []),
    ("theory.md", "void radixSort(", []),
    ("theory.md", "void bucketSort(", []),
    ("theory.md", "void shellSort(", []),
    ("theory.md", "void sortingLibraryDemo(", []),
    ("theory.md", "void ascendingAndDescendingDemo(", []),
    ("theory.md", "void sortCandidates(", []),
    ("theory.md", "void insertionSortDescendingChars(", []),
    ("theory.md", "void mergeSortedArrays(", []),
    ("theory.md", "bool nextPermutation(", []),
    ("interview-prep.md", "void demoPartialSort(", []),
    ("interview-prep.md", "vector<vector<int>> mergeIntervals(", []),
    ("interview-prep.md", "void sortColors(", []),
    ("interview-prep.md", "long long countInversions(", []),
    ("interview-prep.md", "int findKthLargest(", []),
    ("practice.md", "void sortStringAlphabetically(", []),
    ("practice.md", "void sortKSortedArray(", []),
    ("practice.md", "long long countInversionsFenwick(", []),
]

TESTS = r'''
#include <cassert>
#include <climits>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
using namespace std;

long long bruteInversions(const vector<int>& a) {
    long long result = 0;
    for (size_t i = 0; i < a.size(); ++i)
        for (size_t j = i + 1; j < a.size(); ++j) result += a[i] > a[j];
    return result;
}

template<class Function> void rejects(Function action) {
    bool caught = false;
    try { action(); } catch (const invalid_argument&) { caught = true; }
    assert(caught);
}

int main() {
    mt19937 random(20261014);
    vector<vector<int>> cases = {{}, {42}, {1,2,3}, {3,2,1}, {7,7,7},
        {-5,0,-5,2}, {INT_MIN,INT_MAX,0}, {INT_MIN,INT_MIN}, {INT_MAX,INT_MAX}};
    for (int trial = 0; trial < 300; ++trial) {
        vector<int> a(random() % 51);
        for (auto& x : a) x = static_cast<int>(random() % 31) - 15;
        cases.push_back(a);
    }
    vector<function<void(vector<int>&)>> sorts = {
        bubbleSort, selectionSort, insertionSort, mergeSort, heapSort, shellSort,
        [](auto& a) { quickSort(a,0,static_cast<int>(a.size())-1); },
        [](auto& a) { quickSortLogarithmicStack(a,0,static_cast<int>(a.size())-1); },
        [](auto& a) { quickSortHoare(a,0,static_cast<int>(a.size())-1); },
        [](auto& a) { quickSort3Way(a,0,static_cast<int>(a.size())-1); }
    };
    size_t sortChecks = 0;
    for (const auto& original : cases) {
        auto expected = original;
        sort(expected.begin(),expected.end());
        assert(isSortedAscending(original) == is_sorted(original.begin(),original.end()));
        for (auto algorithm : sorts) {
            auto a = original;
            algorithm(a);
            assert(a == expected);
            ++sortChecks;
        }
        if (original.empty() || static_cast<long long>(expected.back()) - expected.front() < 1000) {
            auto a = original;
            countingSort(a);
            assert(a == expected);
            a = original;
            countingSortValues(a);
            assert(a == expected);
            a = original;
            countingSortValues(a,true);
            auto reverseExpected = expected;
            reverse(reverseExpected.begin(),reverseExpected.end());
            assert(a == reverseExpected);
        }
        auto copy = original;
        auto inversions = bruteInversions(original);
        assert(countInversions(copy) == inversions && copy == expected);
        assert(countInversionsFenwick(original) == inversions);
        for (int k = 1; k <= static_cast<int>(original.size()); ++k) {
            auto a = original;
            assert(findKthLargest(a,k) == expected[expected.size()-k]);
        }
        // A k >= size covers every possible displacement, including INT_MAX.
        auto a = original;
        sortKSortedArray(a,INT_MAX);
        assert(a == expected);
        a = expected;
        sortKSortedArray(a,0);
        assert(a == expected);
    }
    cout << "PASS: " << sortChecks << " comparison-sort checks; counting, kth ranks, and two inversion methods\n";

    // Exhaust all small duplicate-valued arrays, testing the full permutation contract.
    size_t permutations = 0;
    for (int n = 0; n <= 7; ++n) {
        int combinations = 1;
        for (int i = 0; i < n; ++i) combinations *= 3;
        for (int mask = 0; mask < combinations; ++mask) {
            vector<int> a(n);
            int code = mask;
            for (auto& x : a) { x = code % 3; code /= 3; }
            auto expected = a;
            bool result = next_permutation(expected.begin(),expected.end());
            assert(nextPermutation(a) == result && a == expected);
            a = expected;
            sortColors(a);
            assert(is_sorted(a.begin(),a.end()));
            auto before = expected;
            sort(before.begin(),before.end());
            assert(a == before);
            ++permutations;
        }
    }
    cout << "PASS: " << permutations << " exhaustive Next Permutation and DNF inputs\n";

    for (int trial = 0; trial < 200; ++trial) {
        vector<int> left(random()%21), right(random()%21);
        for (auto& x : left) x = static_cast<int>(random()%21)-10;
        for (auto& x : right) x = static_cast<int>(random()%21)-10;
        sort(left.begin(),left.end()); sort(right.begin(),right.end());
        auto expected = left;
        expected.insert(expected.end(),right.begin(),right.end());
        sort(expected.begin(),expected.end());
        int m = static_cast<int>(left.size()), n = static_cast<int>(right.size());
        left.resize(m+n,999); // Padding is not identified by zero values.
        mergeSortedArrays(left,m,right,n);
        assert(left == expected);
        vector<double> floating(random()%31);
        for (auto& x : floating) x = (random()%1000)/1000.0;
        auto floatExpected = floating;
        sort(floatExpected.begin(),floatExpected.end());
        bucketSort(floating);
        assert(floating == floatExpected);
        vector<int> radix(random()%31);
        for (auto& x : radix) x = static_cast<int>(random()%static_cast<unsigned>(INT_MAX));
        auto radixExpected = radix;
        sort(radixExpected.begin(),radixExpected.end());
        radixSort(radix);
        assert(radix == radixExpected);
    }
    vector<int> largestRadix = {INT_MAX,0,INT_MAX-1,1000000000};
    radixSort(largestRadix);
    assert(is_sorted(largestRadix.begin(),largestRadix.end()));
    cout << "PASS: 200 random padded merges, bucket inputs, and radix inputs; INT_MAX radix\n";

    for (int m = 0; m <= 3; ++m) for (int n = 0; n <= 3; ++n) {
        vector<int> left(m,0), right(n,0);
        left.resize(m+n,123);
        mergeSortedArrays(left,m,right,n);
        assert(left == vector<int>(m+n,0));
    }
    rejects([] { vector<int> a={INT_MIN,INT_MAX}; countingSort(a); });
    rejects([] { vector<int> a={0,20000000}; countingSortValues(a); });
    rejects([] { vector<int> a={-1}; radixSort(a); });
    rejects([] { vector<int> a={0,-1}; radixSort(a); });
    for (double value : {1.0,-0.1,numeric_limits<double>::infinity(),numeric_limits<double>::quiet_NaN()})
        rejects([=] { vector<double> a={value}; bucketSort(a); });
    rejects([] { vector<int> a; findKthLargest(a,1); });
    rejects([] { vector<int> a={1}; findKthLargest(a,0); });
    rejects([] { vector<int> a={1}; findKthLargest(a,2); });
    rejects([] { vector<int> a={1}; quickSelectIterative(a,-1); });
    rejects([] { vector<int> a; sortKSortedArray(a,-1); });
    rejects([] { vector<int> a={1},b={2}; mergeSortedArrays(a,1,b,1); });
    rejects([] { vector<int> a={1}; mergeSortedArrays(a,0,a,1); });
    cout << "PASS: domain, range, size, and rank rejection cases; empty-side merges\n";

    vector<int> descending(100000);
    for (int i = 0; i < 100000; ++i) descending[i] = 100000-i;
    assert(countInversionsFenwick(descending) == 4999950000LL);
    assert(countInversions(descending) == 4999950000LL);
    vector<int> equalKeys(10000,7);
    assert(findKthLargest(equalKeys,5000) == 7);
    vector<int> nearSorted={2,1,4,3,6,5};
    sortKSortedArray(nearSorted,1);
    assert(is_sorted(nearSorted.begin(),nearSorted.end()));
    cout << "PASS: 64-bit inversion totals, 10,000 equal-key selection, k=1 displacement\n";

    for (string text : {string{},string{"a"},string{"fbaecd"},string{"zzaabb"}}) {
        string expected = text;
        sort(expected.begin(),expected.end(),greater<char>{});
        auto a = text; insertionSortDescendingChars(a); assert(a == expected);
        sort(expected.begin(),expected.end());
        a = text; sortStringAlphabetically(a); assert(a == expected);
    }
    vector<Candidate> candidates={{"A",90,2},{"B",80,1},{"C",90,1}};
    sortCandidates(candidates);
    assert(candidates[0].name=="C" && candidates[1].name=="A" && candidates[2].name=="B");
    vector<vector<int>> intervals={{8,10},{1,3},{2,6},{9,12}};
    assert(mergeIntervals(intervals) == vector<vector<int>>({{1,6},{8,12}}));
    intervals={{1,10},{2,3},{10,12}};
    assert(mergeIntervals(intervals) == vector<vector<int>>({{1,12}}));
    intervals={}; assert(mergeIntervals(intervals).empty());
    intervals={{1,1}}; assert(mergeIntervals(intervals)==intervals);
    demoPartialSort(); demoNthElement(); sortingLibraryDemo(); ascendingAndDescendingDemo();
    cout << "PASS: character order, candidate tie-breaking, intervals, and STL demos\n";
    verifyTaggedStability();
}
'''


def tagged_stability_code():
    """Adapt scalar code to carry IDs; keep its comparisons and placement order.

    Scalar duplicates have no observable identity. These adaptations test the
    stable movement rules rather than pretending integer equality proves them.
    """
    prefix = r'''
#include <cassert>
#include <iostream>
struct Tagged {
    int key = 0;
    int id = 0;
    operator int() const { return key; }
    bool operator==(const Tagged& other) const { return key == other.key && id == other.id; }
};
'''
    pieces = []
    for declaration in ["void bubbleSort(", "void insertionSort(", "void mergeSort(", "void countingSort("]:
        code = extract("theory.md", declaration)
        # Headers must stay outside the per-algorithm namespace.
        code = re.sub(r"^#include[^\n]*\n", "", code, flags=re.M)
        if declaration == "void countingSort(":
            code = code.replace("vector<int>& arr", "vector<Tagged>& arr")
            code = code.replace("vector<int> output", "vector<Tagged> output")
        else:
            code = code.replace("vector<int>", "vector<Tagged>")
            code = code.replace("int key = arr[i]", "Tagged key = arr[i]")
        namespace = declaration.split()[1].split("(")[0]
        pieces.append(f"namespace tagged_{namespace} {{\n{code}\n}}")
    ending = r'''
void verifyTaggedStability() {
    vector<Tagged> input = {{3,0},{1,1},{3,2},{2,3},{1,4},{3,5}};
    auto expected = input;
    stable_sort(expected.begin(),expected.end(),[](const auto& a, const auto& b) {return a.key < b.key;});
    auto a = input; tagged_bubbleSort::bubbleSort(a); assert(a == expected);
    a = input; tagged_insertionSort::insertionSort(a); assert(a == expected);
    a = input; tagged_mergeSort::mergeSort(a); assert(a == expected);
    a = input; tagged_countingSort::countingSort(a); assert(a == expected);
    cout << "PASS: tagged adaptations of Bubble, Insertion, Merge, and Counting preserve tie order\n";
}
'''
    return prefix + "\n".join(pieces) + ending


def check_links():
    """Check local Markdown/HTML destinations and GitHub-style heading anchors."""
    errors = []
    count = 0
    for path in [*ROOT.glob("*.md"), ROOT / "sorting-visualizer.html", ROOT.parents[1] / "README.md"]:
        text = path.read_text(encoding="utf-8")
        without_code = re.sub(r"```.*?```", "", text, flags=re.S)
        links = re.findall(r"(?<!!)\[[^\]]+\]\(([^)]+)\)", without_code)
        links += re.findall(r'href="([^"]+)"', without_code)
        for link in links:
            if re.match(r"^[a-zA-Z][\w+.-]*:", link) or link.startswith("//"):
                continue
            target, _, anchor = link.partition("#")
            destination = (path.parent / target).resolve() if target else path
            count += 1
            if not destination.is_file():
                errors.append(f"{path.name}: missing {link}")
                continue
            if anchor and destination.suffix in {".md", ".html"}:
                data = destination.read_text(encoding="utf-8")
                anchors = set(re.findall(r'<[^>]+\bid=["\']([^"\']+)', data))
                headings = re.findall(r"^#{1,6}\s+(.+)$", re.sub(r"```.*?```", "", data, flags=re.S), re.M)
                used = {}
                for heading in headings:
                    heading = re.sub(r"<[^>]*>", "", heading).lower()
                    slug = re.sub(r"[^\w\s-]", "", heading).replace(" ", "-")
                    suffix = used.get(slug, 0)
                    used[slug] = suffix + 1
                    anchors.add(slug if suffix == 0 else f"{slug}-{suffix}")
                if anchor not in anchors:
                    errors.append(f"{path.name}: missing anchor {link}")
        if path.suffix == ".md" and len(re.findall(r"^```", text, re.M)) % 2:
            errors.append(f"{path.name}: unbalanced code fences")
    if errors:
        raise RuntimeError("\n".join(errors))
    print(f"PASS: {count} local links/anchors and Markdown code fences", flush=True)


def check_visualizer():
    node = shutil.which("node")
    if not node:
        raise RuntimeError("node is required to test the visualizer generators.")
    subprocess.run([node, str(ROOT / "verify_visualizer.cjs")], check=True)


def main():
    compiler = shutil.which("g++")
    if not compiler:
        raise SystemExit("g++ is required on PATH; no code validation was performed.")
    check_links()
    check_visualizer()
    with tempfile.TemporaryDirectory(prefix="sorting-notes-") as temporary:
        directory = Path(temporary)
        snippets = []
        for number, (filename, declaration, dependencies) in enumerate(SPECS):
            snippet = extract(filename, declaration)
            code = "\n".join(extract(*dependency) for dependency in dependencies) + "\n" + snippet
            source = directory / f"snippet-{number}.cpp"
            source.write_text(code, encoding="utf-8")
            subprocess.run([compiler, "-std=c++17", "-Wall", "-Wextra", "-pedantic", "-fsyntax-only", str(source)], check=True)
            snippets.append(snippet)
        print(f"PASS: {len(snippets)} complete code blocks compile with documented dependencies", flush=True)
        source = directory / "notes-tests.cpp"
        source.write_text("\n\n".join(dict.fromkeys(snippets)) + "\n" + tagged_stability_code() + "\n" + TESTS, encoding="utf-8")
        executable = directory / "notes-tests.exe"
        subprocess.run([compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-pedantic", "-D_GLIBCXX_ASSERTIONS", str(source), "-o", str(executable)], check=True)
        subprocess.run([str(executable)], check=True, timeout=90)
    print("All current-note checks passed. These are correctness checks, not benchmarks.", flush=True)


if __name__ == "__main__":
    main()
