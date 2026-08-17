# Searching & Sorting — Complete Deep-Understanding Syllabus

Searching and Sorting are not separate tricks. They are two sides of the same idea:

> **How do I organize information so that finding, comparing, and processing it becomes cheaper?**

After Arrays, Linked Lists, Stacks/Queues, and Hash Tables, you already have multiple lookup strategies:

```text
Unsorted array     → O(n) scan
Sorted array       → O(log n) binary search
Hash table         → expected O(1) lookup
Balanced BST       → O(log n) with ordering
```

This block teaches the **ordered-search** and **comparison-based ordering** machinery that appears everywhere later — in Trees, Graphs, Greedy, and Divide-and-Conquer.

Don't learn Searching as only "find a number in a sorted array."

Learn:

> **Maintain a shrinking candidate region using a monotonic property.**

Don't learn Sorting as only "put numbers in order."

Learn:

> **Transform a messy problem into one where neighbors, intervals, duplicates, and greedy choices become obvious.**

---

# 1. Start with the problem Searching solves

Suppose you have:

```text
[12, 45, 7, 89, 23, 56, 34, 91]
```

And you need:

```text
Does 56 exist?
```

With no extra structure, you scan:

```text
12 → 45 → 7 → 89 → 23 → 56 ← found
```

Worst case:

```text
Time:  O(n)
Space: O(1)
```

That is **linear search**.

It is not "naive" in a bad sense. It is the correct baseline when:

- Data is unsorted
- Data is small
- You only search once
- You cannot afford preprocessing
- Input is a stream

But if you will search many times, preprocessing may pay off.

```text
Unsorted once + many lookups   → consider hashing or sorting
Sorted once + many lookups     → binary search
Need ordering/range queries    → trees or sorted arrays
```

---

# 2. Linear Search

## Core idea

Check every element until you find the target or exhaust the collection.

```text
arr = [4, 9, 1, 8, 2]
target = 8

i = 0 → 4
i = 1 → 9
i = 2 → 1
i = 3 → 8  ✓
```

## Implementation skeleton

```cpp
int linearSearch(const vector<int>& arr, int target) {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}
```

## Complexity

```text
Best:    O(1)   target is first element
Average: O(n)
Worst:   O(n)   target absent or last
Space:   O(1)
```

## When linear search is the right answer

```text
Input size is tiny
Input is unsorted and only queried once
You need first match in arbitrary order
You're scanning once anyway (no extra pass cost)
Memory is too tight for auxiliary structures
```

## Variants worth knowing

### Search with sentinel

Useful in low-level/array-heavy code:

```text
Append target to end
Scan until found
Guaranteed to stop
```

### Search on linked list

Same O(n), but no index jumping.

### Search with early exit condition

Example: "find any pair summing to target" can stop once found.

### Sentinel pattern recognition

```text
"I must examine every element anyway"
        ↓
Linear scan
```

---

# 3. The Power of Ordering

Suppose the same data is sorted:

```text
[7, 12, 23, 34, 45, 56, 89, 91]
```

Now you can eliminate half the search space at each step.

```text
Looking for 56

       [7, 12, 23, 34, 45, 56, 89, 91]
                         ↑
                      middle = 45

56 > 45 → search right half

                 [56, 89, 91]
                    ↑
                 middle = 89

56 < 89 → search left half

                 [56]
                  ↑
                 found
```

Complexity drops from O(n) to O(log n).

But ordering has a cost:

```text
Sorting once:  O(n log n) typically
Then search:   O(log n) each time
```

So ask:

> **Will I search enough times to justify sorting?**

Also ask:

> **Can I maintain sorted order during inserts?**

That trade-off connects directly to Trees and Balanced BSTs later.

---

# 4. Binary Search Fundamentals

Binary search is one of the most important algorithms in computer science.

But beginners often memorize a template without understanding the invariant.

## The real idea

Maintain a search interval `[lo, hi]` such that:

```text
If the answer exists, it is always inside [lo, hi]
```

At each step:

1. Inspect the middle
2. Use a monotonic property to discard half
3. Repeat until the interval is resolved

```text
lo                          hi
 ↓                           ↓
[1, 3, 5, 7, 9, 11, 13, 15]

discard left or right half based on comparison/property
```

## Two common loop styles

### Style A — exact match, return index or -1

```cpp
int binarySearch(const vector<int>& arr, int target) {
    int lo = 0, hi = arr.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}
```

### Style B — lower bound / insertion point

```cpp
int lowerBound(const vector<int>& arr, int target) {
    int lo = 0, hi = arr.size();  // half-open [lo, hi)
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}
```

Both are valid. **Pick one family and understand it deeply.**

## Overflow-safe midpoint

Never write only:

```cpp
int mid = (lo + hi) / 2;  // can overflow in some languages
```

Prefer:

```cpp
int mid = lo + (hi - lo) / 2;
```

## Complexity

```text
Time:  O(log n)
Space: O(1) iterative
Space: O(log n) recursive due to call stack
```

---

# 5. Binary Search — Exact Match

## Problem

Find index of `target` in sorted array, or return -1.

```text
arr = [1, 3, 5, 7, 9]
target = 7 → index 3
target = 4 → -1
```

## Invariant

```text
If target exists, its index is in [lo, hi]
```

## Walkthrough

```text
arr = [1, 3, 5, 7, 9, 11]
target = 9

lo=0 hi=5 mid=2 val=5  → go right
lo=3 hi=5 mid=4 val=9  → found
```

## Recognition clue

```text
Sorted array
Need exact presence/index
No duplicates special handling
```

---

# 6. Binary Search — First Occurrence

When duplicates exist, "find 5" is ambiguous.

```text
[1, 2, 5, 5, 5, 8, 9]
         ↑
    first 5 is here (index 2)
```

## Goal

Return the **leftmost** index where `arr[i] == target`.

## Key idea

When `arr[mid] == target`, don't stop immediately.

Ask:

> **Could an equal value exist to the left?**

If yes, keep searching left half.

```cpp
int firstOccurrence(const vector<int>& arr, int target) {
    int lo = 0, hi = arr.size() - 1;
    int ans = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            ans = mid;
            hi = mid - 1;   // keep looking left
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}
```

## Alternative via lower bound

```text
first occurrence = lowerBound(target)
verify arr[index] == target
```

This is often cleaner.

---

# 7. Binary Search — Last Occurrence

```text
[1, 2, 5, 5, 5, 8, 9]
               ↑
          last 5 is here (index 4)
```

## Goal

Return the **rightmost** index where `arr[i] == target`.

## Key idea

When equal, search right half.

```cpp
int lastOccurrence(const vector<int>& arr, int target) {
    int lo = 0, hi = arr.size() - 1;
    int ans = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            ans = mid;
            lo = mid + 1;   // keep looking right
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}
```

## Count of target in sorted array

```text
count = lastOccurrence(target) - firstOccurrence(target) + 1
```

or using bounds:

```text
count = upperBound(target) - lowerBound(target)
```

---

# 8. Lower Bound and Upper Bound

These are among the most reusable binary search APIs.

## Lower bound

First index `i` such that `arr[i] >= target`.

```text
arr = [1, 2, 4, 4, 5, 8]
target = 4 → index 2
target = 3 → index 2 (insertion point)
target = 9 → index 6 (past end)
```

## Upper bound

First index `i` such that `arr[i] > target`.

```text
arr = [1, 2, 4, 4, 5, 8]
target = 4 → index 4
```

## Why they matter

```text
first occurrence  = lowerBound
last occurrence   = upperBound - 1
insert position   = lowerBound
count of target   = upperBound - lowerBound
```

## C++ standard library

```cpp
auto it1 = lower_bound(arr.begin(), arr.end(), x);
auto it2 = upper_bound(arr.begin(), arr.end(), x);
```

Know what they return. Implement them yourself at least once.

## Half-open interval template

Many engineers prefer:

```text
Search space: [lo, hi)
Loop while lo < hi
```

This reduces off-by-one bugs for bounds.

```cpp
int upperBound(const vector<int>& arr, int target) {
    int lo = 0, hi = arr.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] <= target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}
```

---

# 9. Binary Search in Rotated Sorted Array

A sorted array was rotated at some pivot.

```text
Original: [1, 2, 3, 4, 5, 6, 7]
Rotated:  [4, 5, 6, 7, 1, 2, 3]
```

`target = 1` should be found.

## Why normal binary search breaks

The entire array is not sorted, but **one half is always sorted**.

## Core observation

At any `mid`, at least one of `[lo, mid]` or `[mid, hi]` is sorted.

```text
[4, 5, 6, 7, 1, 2, 3]
 lo     mid        hi

left half [4,5,6,7] is sorted
```

## Decision logic

1. Identify sorted half
2. Check if target lies in that sorted half's value range
3. Discard the other half otherwise

```cpp
int searchRotated(const vector<int>& arr, int target) {
    int lo = 0, hi = arr.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;

        if (arr[lo] <= arr[mid]) {          // left sorted
            if (arr[lo] <= target && target < arr[mid]) hi = mid - 1;
            else lo = mid + 1;
        } else {                             // right sorted
            if (arr[mid] < target && target <= arr[hi]) lo = mid + 1;
            else hi = mid - 1;
        }
    }
    return -1;
}
```

## Variants

- Find minimum in rotated sorted array
- Handle duplicates (may degrade toward O(n) in worst case)
- Search in rotated array with duplicates requires extra care

## Recognition clue

```text
"Sorted but broken at one pivot"
        ↓
Identify sorted half each step
```

---

# 10. Search Invariants

This is the deepest part of searching.

A **search invariant** is a statement that remains true throughout the algorithm.

## Example — exact match

```text
Invariant: if target exists, index is in [lo, hi]
```

Every update to `lo` or `hi` must preserve this.

If you cannot state the invariant, you will get infinite loops or wrong answers.

## Example — lower bound

```text
Invariant: answer index is in [lo, hi)
All indices < lo are too small
All indices >= hi are too large
```

## How to debug binary search

When your BS code fails:

1. Write the invariant explicitly
2. Check whether `lo` and `hi` updates preserve it
3. Check empty input
4. Check single-element input
5. Check target smaller than all / larger than all
6. Check duplicates
7. Check off-by-one in `<=` vs `<`

## Common bug patterns

```text
Infinite loop        → hi not shrinking / wrong mid update
Off-by-one           → closed vs half-open mismatch
Wrong half discarded → inverted comparison
```

## Practice discipline

For every binary search solution, write one sentence:

> **"I am maintaining that the answer lies in ___ because ___."**

That habit separates memorization from mastery.

---

# 11. Monotonic Search Spaces — Binary Search on Answer

This is where binary search becomes a general technique, not just an array trick.

## The shift

Instead of searching for an index in an array, search for a **value** in a numeric answer space.

```text
Candidate answers: [low, high]
Property: if x works, then any x' >= x also works   (monotone)
```

or the reverse monotone direction.

## Template

```text
while low < high:
    mid = average candidate answer
    if feasible(mid):
        move toward better feasible answers
    else:
        discard mid and one side
return best feasible
```

## Classic example — square root floor

Find largest `m` such that `m*m <= n`.

```text
low=0 high=n
feasible(m): m*m <= n
```

## Classic example — minimum capacity to ship packages in D days

Not obviously "binary search," but:

```text
Can we ship with capacity C?
If yes for C, also yes for larger C → monotone
Binary search on C
```

## Classic example — Koko eating bananas

```text
Answer space = eating speed
Predicate: can finish in h hours with speed m?
Monotone in speed
```

## Classic example — allocate books / split array

```text
Minimize maximum sum of k subarrays
Binary search on the maximum allowed sum
Check feasibility with greedy scan
```

## Recognition clues

```text
"Minimize the maximum ..."
"Maximize the minimum ..."
"Find smallest/largest X such that condition holds"
"Can we achieve ... with limit L?"
```

## Feasibility check is the real work

Binary search on answer is usually:

```text
O(log(range) * cost_of_check)
```

If check is O(n), total is O(n log range).

## Designing the predicate

Ask:

1. What is the answer variable?
2. Is feasibility monotone?
3. What is the smallest possible answer?
4. What is the largest possible answer?
5. How do I test a candidate quickly?

If monotonicity is unclear, binary search on answer is probably the wrong tool.

---

# 12. Sorting — Why Sorting Helps

Sorting is preprocessing that reveals structure.

```text
Unsorted: [8, 3, 5, 3, 9, 1]
Sorted:   [1, 3, 3, 5, 8, 9]
```

After sorting:

```text
Duplicates become adjacent
Intervals can be merged by scanning
Two-sum style problems can use two pointers
Greedy choices often become easier
Binary search becomes available
```

## Cost-benefit

```text
One sort + one scan     → often O(n log n)
Repeated hash lookups   → maybe O(n) expected
```

Neither always wins.

Compare:

```text
Need many membership queries     → hash set may win
Need order/range/neighbors       → sorting may win
Input already nearly sorted      → insertion sort / adaptive sorts
```

## Sorting as tool vs sorting as topic

Sometimes sorting is the whole problem.

More often sorting is a **step inside a larger algorithm**.

Train both views.

---

# 13. Stability, In-Place, Adaptive

## Stability

A sort is **stable** if equal elements keep their original relative order.

```text
Before: [(3,A), (1,B), (3,C)]
After stable sort by key: [(1,B), (3,A), (3,C)]
                         A before C preserved
```

Why stability matters:

```text
Sort students by grade, then by name
Multi-key sorting via successive stable sorts
Preserve input metadata / original indices
```

## In-place vs extra memory

```text
In-place:   O(1) extra space besides input rearrangement
Out-of-place: needs auxiliary array
```

Examples:

```text
Merge sort array version   → O(n) extra
Quick sort                 → O(log n) stack typically, in-place partition
Heap sort                  → O(1) extra
Counting/radix             → extra counting buckets
```

## Adaptive

An **adaptive** sort runs faster on nearly sorted input.

```text
Insertion sort: excellent on nearly sorted data
Merge sort:     not very adaptive
Quick sort:     not really adaptive
```

## Comparison vs non-comparison sorts

```text
Comparison sorts:     lower bound Ω(n log n)
Non-comparison sorts: can do O(n) under restrictions
```

Restrictions often include:

```text
Integer keys in bounded range
Fixed digit length
Uniformly distributed floats in buckets
```

---

# 14. Comparison Sorting Lower Bound

For comparison-based sorting of `n` distinct elements:

```text
Worst-case lower bound: Ω(n log n)
```

## Intuition via decision tree

Each comparison splits possibilities.

You need enough comparisons to distinguish `n!` permutations.

```text
log2(n!) = Θ(n log n)
```

## What this means

No comparison sort can beat O(n log n) in the worst case.

So when someone claims a general comparison sort in O(n), be skeptical.

## Escape hatches

Non-comparison sorts break the assumption by using key structure:

```text
Counting sort
Radix sort
Bucket sort
```

## Practical note

Lower bound is worst-case.

Quick sort's average case is O(n log n) even though worst case is O(n²).

Engineering often cares about:

```text
average case
cache behavior
stability
memory
```

---

# 15. Bubble Sort

## Idea

Repeatedly swap adjacent out-of-order elements.

```text
[5, 1, 4, 2, 8]

pass 1: [1, 4, 2, 5, 8]
pass 2: [1, 2, 4, 5, 8]
```

Largest elements "bubble" to the end.

## Complexity

```text
Time:  O(n²)
Space: O(1)
Stable: yes
```

## Optimization

If a pass performs zero swaps, array is sorted → stop early.

That makes it adaptive in a limited sense.

## Why learn it?

Not because production systems use it.

Because it teaches:

```text
Adjacent comparison
Pass-based sorting
Invariant: after i passes, last i elements are in final position
```

## When it is actually reasonable

```text
Tiny n
Educational purposes
Some hardware/extremely simple environments
```

---

# 16. Selection Sort

## Idea

Repeatedly select the minimum of the unsorted suffix and swap it into place.

```text
[29, 10, 14, 37, 13]

select 10 → [10, 29, 14, 37, 13]
select 13 → [10, 13, 14, 37, 29]
...
```

## Complexity

```text
Time:  O(n²) always
Space: O(1)
Stable: no (typical swap implementation)
```

## Useful concept

Selection sort performs at most `n - 1` swaps.

Useful when write cost is expensive.

## Teaching value

```text
Sorted prefix grows
Unsorted suffix shrinks
Explicit "select best candidate" thinking
```

---

# 17. Insertion Sort

## Idea

Build sorted prefix by inserting each next element into its correct position.

```text
[5, 2, 4, 6, 1, 3]

[5]
[2, 5]
[2, 4, 5]
[2, 4, 5, 6]
...
```

## Complexity

```text
Best:    O(n)   already sorted
Average: O(n²)
Worst:   O(n²)
Space:   O(1)
Stable:  yes
```

## Why insertion sort matters

```text
Excellent on small arrays
Excellent on nearly sorted data
Used inside Timsort/Introsort for small subarrays
Online algorithm: can sort as data arrives
```

## Recognition

```text
Small n
Nearly sorted input
Need stable simple in-place sort
```

---

# 18. Merge Sort

## Idea

Divide array into halves, sort each half, merge sorted halves.

```text
[38, 27, 43, 3, 9, 82, 10]

split → [38,27,43,3] [9,82,10]
split → ...
merge up
```

## Merge step

```text
Left:  [3, 27, 38, 43]
Right: [9, 10, 82]

Compare fronts, append smaller
→ [3, 9, 10, 27, 38, 43, 82]
```

## Complexity

```text
Time:  O(n log n) always
Space: O(n) for array version
Stable: yes
```

## Why merge sort is foundational

It teaches:

```text
Divide and conquer
Merging sorted sequences
Recurrence T(n) = 2T(n/2) + O(n)
```

## Applications beyond sorting

```text
Count inversions
Merge k sorted lists
External sorting
Linked list sorting
```

## Linked list merge sort

Often preferred over quick sort for linked lists because:

```text
No random access needed
Merge is natural
O(1) extra space possible with pointer manipulation
```

---

# 19. Quick Sort

## Idea

Choose a pivot, partition into `< pivot` and `> pivot`, recurse.

```text
[8, 3, 5, 1, 9, 2]

pivot = 5
[3, 1, 2] 5 [8, 9]
```

## Partitioning

Two-pointer partition is the core skill.

```text
i scans, j scans
swap out-of-place elements
place pivot
```

## Complexity

```text
Average: O(n log n)
Worst:   O(n²) bad pivot choices / adversarial input
Space:   O(log n) recursion stack average
In-place: yes
Stable:   usually no
```

## Why worst case happens

```text
Already sorted + naive pivot = unbalanced partitions
```

## Improvements

```text
Random pivot
Median-of-three
Switch to insertion sort for small subarrays
Introsort: fall back to heap sort if recursion too deep
```

## Quickselect

Partition-based algorithm to find kth smallest in average O(n).

This is more important than memorizing quick sort variants.

```text
Do not fully sort
Only recurse into side containing k
```

---

# 20. Heap Sort

## Idea

Build a max-heap, repeatedly extract maximum to end.

```text
Array representation of heap
Heapify
Swap root with end
Shrink heap
```

## Complexity

```text
Time:  O(n log n) worst case
Space: O(1) extra
Stable: no
```

## Why learn heap sort here

It introduces the heap concept before the Trees/Heaps block.

Learn enough now to understand:

```text
Heap property
Heapify
Priority queue behavior
```

Then study heaps deeply later as a data structure.

## Heap sort vs quick sort vs merge sort

```text
Merge sort: predictable O(n log n), extra memory
Quick sort: fast average, in-place, worst-case risk
Heap sort: guaranteed O(n log n), in-place, poorer locality
```

---

# 21. Counting Sort

## Idea

Count occurrences of each key, then reconstruct sorted order.

```text
[4, 2, 2, 8, 3, 3, 1]
counts: 1→1, 2→2, 3→2, 4→1, 8→1
output: [1, 2, 2, 3, 3, 4, 8]
```

## Requirements

```text
Keys in limited integer range [0, k]
```

## Complexity

```text
Time:  O(n + k)
Space: O(k)
Stable: possible with careful reconstruction
```

## When to use

```text
Small key universe
Frequencies matter
As subroutine of radix sort
```

## Recognition

```text
"Values bounded by small k"
        ↓
Counting sort or counting array
```

---

# 22. Radix Sort

## Idea

Sort by individual digits/characters from least significant digit (LSD) or most significant digit (MSD), using a stable subroutine sort.

```text
170, 45, 75, 90, 802, 24, 2, 66

LSD passes by ones, tens, hundreds...
```

## Complexity

```text
Time: O(d * (n + b))
d = number of digits/pass
b = base (10, 256, etc.)
```

## Requirements

```text
Fixed-length or variable-length keys with careful handling
Stable digit sorting
```

## Teaching value

Shows how non-comparison sorts bypass the Ω(n log n) bound.

---

# 23. Bucket Sort

## Idea

Distribute elements into buckets, sort buckets individually, concatenate.

```text
Input in [0, 1) uniformly
bucket by range
sort each bucket
concatenate
```

## Complexity

```text
Average: O(n) when uniformly distributed
Worst:   O(n²) if all in one bucket
```

## When useful

```text
Uniformly distributed floats
External sort intuition
```

## Recognition

```text
"Input uniformly spread across range"
        ↓
Bucket sort possible
```

---

# 24. Choosing the Right Sort

Use this decision thinking:

```text
1. Do I only need top k / kth element?
   → heap / quickselect, maybe not full sort

2. Do I need stability?
   → merge sort, insertion sort, counting/radix if applicable

3. Is memory tight?
   → heap sort or in-place quick sort

4. Is input nearly sorted?
   → insertion sort or adaptive hybrid

5. Are keys integers in small range?
   → counting/radix

6. Do I need guaranteed worst-case O(n log n)?
   → merge sort or heap sort

7. Is this a linked list?
   → merge sort

8. Is sorting just preprocessing for scan/two pointers?
   → library sort often fine; know complexity
```

## Language standard libraries

Know what your language uses:

```text
C++ std::sort         → Introsort (quick + heap + insertion)
Java Arrays.sort      → DualPivot Quicksort / TimSort depending on type
Python sorted         → Timsort
```

You don't need to memorize every hybrid detail, but know they are engineered for real data.

---

# 25. The Most Important Searching & Sorting Patterns

These patterns appear constantly in interviews and real problem solving.

---

## Pattern 1 — Linear Scan / Exhaustive Search

**When:** no order, single pass, small input, or must inspect all.

```text
Find max in unsorted array
Check if any element satisfies condition
First missing positive with constraints
```

Pattern:

```cpp
for (int x : arr) {
    if (condition(x)) ...
}
```

Recognition:

```text
"No preprocessing allowed or helpful"
        ↓
Linear scan
```

---

## Pattern 2 — Binary Search Exact Match

**When:** sorted array, need presence/index.

```text
Search Insert Position (with bounds framing)
Classic binary search
```

Recognition:

```text
"Sorted array" + "find target"
        ↓
Exact match BS
```

---

## Pattern 3 — Binary Search on Bounds (First/Last/Lower/Upper)

**When:** duplicates, ranges, insertion position, frequency in sorted array.

```text
First and last position of element
Count occurrences in sorted array
Find leftmost feasible index
```

Recognition:

```text
"First/last/index where ..."
        ↓
lowerBound / upperBound
```

---

## Pattern 4 — Identify Sorted Half in Rotated Array

**When:** rotated sorted structure.

```text
Search in rotated sorted array
Find minimum in rotated sorted array
```

Recognition:

```text
"Rotated sorted"
        ↓
One half always sorted
```

---

## Pattern 5 — Binary Search on Answer (Monotone Predicate)

**When:** minimize/maximize answer with yes/no feasibility.

```text
Koko eating bananas
Ship packages within D days
Split array largest minimum sum
Aggressive cows / minimum maximum distance
Painter's partition
```

Template:

```text
define predicate(mid)
binary search answer space
return best feasible
```

Recognition:

```text
"Minimum maximum" / "Maximum minimum"
"Smallest X such that possible"
        ↓
BS on answer
```

---

## Pattern 6 — Sort + Scan

**When:** after sorting, one linear pass solves the problem.

```text
Contains duplicate (sort and check neighbors)
Merge intervals preprocessing
Assign cookies / matching with greediness after sort
Non-overlapping intervals
```

Workflow:

```text
sort
scan adjacent / linear
```

Recognition:

```text
"After ordering, answer is local/neighbor-based"
        ↓
Sort + scan
```

---

## Pattern 7 — Sort + Two Pointers

**When:** pair/triplet sum, container with most water style on sorted data.

```text
Two sum II (sorted input)
3Sum
4Sum
Boats to save people
```

Workflow:

```text
sort if needed
left = 0, right = n-1
move pointers based on comparison
```

Recognition:

```text
"Pair/triplet in sorted array"
        ↓
Two pointers
```

---

## Pattern 8 — Interval Merging After Sort by Start

**When:** overlapping intervals.

```text
Merge intervals
Insert interval
Meeting rooms II (after sorting starts/ends)
```

Workflow:

```text
sort by start
keep current merged interval
if overlap → extend
else → push and start new
```

Recognition:

```text
"Overlapping intervals"
        ↓
Sort by start + merge scan
```

---

## Pattern 9 — Duplicate Detection via Sorting or Hashing

**When:** detect repeated values.

```text
Contains duplicate
Find all duplicates in array
```

Approaches:

```text
Hash set: O(n) time, O(n) space
Sort:     O(n log n) time, O(1) extra if in-place allowed
```

Recognition:

```text
"Any duplicate?"
        ↓
Set or sort neighbors
```

---

## Pattern 10 — Partitioning (Quick Sort / Quickselect)

**When:** rearrange around pivot, find kth element, Dutch flag style.

```text
Kth largest element
Sort colors
Partition array by predicate
```

Core skill:

```text
move elements < pivot left, > pivot right
```

Recognition:

```text
"Rearrange around threshold" / "kth order statistic"
        ↓
Partition / quickselect
```

---

## Pattern 11 — Merge Sorted Sequences

**When:** combine ordered outputs.

```text
Merge sorted arrays
Merge k sorted lists
Merge intervals (sorted starts)
Count inversions with merge sort
```

Recognition:

```text
"Two/many sorted things"
        ↓
Merge pattern
```

---

## Pattern 12 — Counting / Bucket for Bounded Keys

**When:** key range is small or digits are bounded.

```text
Sort characters in string
Height checker with counting
Custom sort on limited alphabet
Radix sort problems
```

Recognition:

```text
"Keys in small range / fixed alphabet"
        ↓
Counting array / radix
```

---

## Pattern 13 — Greedy After Sorting

**When:** sorting makes greedy choice safe.

```text
Activity selection / non-overlapping intervals
Minimum arrows to burst balloons
Largest number from array of strings (custom comparator)
Task scheduler with cooling (sometimes)
```

Workflow:

```text
sort by finish time / start / custom comparator
greedy pick
```

Recognition:

```text
"Choose maximum compatible set"
        ↓
Sort + greedy
```

---

## Pattern 14 — Coordinate Compression (Sort + Map to Ranks)

**When:** large value range but only n distinct values matter.

```text
Count smaller numbers after self
Range sum query after discretization
```

Workflow:

```text
copy and sort unique values
map values to ranks
use BIT/segment tree or array on ranks
```

Recognition:

```text
"Huge coordinates, few actual values"
        ↓
Sort + compress
```

---

# 26. Pattern Recognition

Train yourself to notice these signals.

```text
"Sorted array" + "find exact"
        ↓
Binary search exact
```

```text
"First/last position" / "insert index"
        ↓
Lower/upper bound
```

```text
"Rotated sorted"
        ↓
Sorted-half binary search
```

```text
"Minimize maximum" / "can we do with limit X?"
        ↓
Binary search on answer
```

```text
"Neighbors matter after ordering"
        ↓
Sort + scan
```

```text
"Pair sum in sorted data"
        ↓
Two pointers
```

```text
"Overlapping intervals"
        ↓
Sort by start + merge
```

```text
"Duplicates"
        ↓
Hash set OR sort neighbors
```

```text
"kth largest/smallest"
        ↓
Heap OR quickselect
```

```text
"Small key universe"
        ↓
Counting sort / frequency array
```

```text
"Need stable order among equals"
        ↓
Stable sort (merge / insertion)
```

```text
"Only top k needed"
        ↓
Maybe avoid full sort → heap
```

Notice how often the workflow is:

```text
Preprocess (sort / count / compress)
        ↓
Simple scan / BS / two pointers
```

That composition is the real skill.

---

# 27. Edge Cases — Extremely Important

Searching and sorting code fails on boring edge cases.

Always test:

## Searching edge cases

```text
Empty array
Single element
Two elements
Target smaller than all
Target larger than all
Target in middle
All elements equal
Many duplicates
Rotated array with no rotation
Rotated array with full rotation
Answer space low == high
Predicate never true
Predicate always true
Integer overflow in mid or answer bounds
```

## Sorting edge cases

```text
Empty array
Single element
Already sorted
Reverse sorted
All equal
Many duplicates
Negative numbers
Mixed signs
Very large range with few elements (counting sort trap)
Stability required with duplicate keys
```

## Off-by-one traps

```text
lo <= hi  vs  lo < hi
hi = n-1  vs  hi = n
mid = lo + (hi-lo)/2
upperBound - 1 for last occurrence
```

## Input mutation

```text
Is in-place sort allowed?
Does binary search require original order preserved?
Do indices need to refer to original positions? → store pairs/index map
```

---

# 28. Implement Yourself

For deep understanding, implement these from scratch.

## Searching

```text
linearSearch
binarySearchExact
lowerBound
upperBound
firstOccurrence
lastOccurrence
searchRotated
binarySearchOnAnswer template with custom predicate
```

## Sorting

```text
bubbleSort
selectionSort
insertionSort
mergeSort
quickSort + partition
heapSort OR understand heapify enough for sort
countingSort
```

## Merging utility

```text
mergeTwoSortedArrays
```

Once you can implement bounds-based binary search cleanly and partition in quick sort without looking up code, most "search/sort" problems become pattern recognition rather than syntax struggle.

---

# 29. Representative Problems

You do **not** need 100 searching/sorting problems.

Choose representatives for each concept.

## Linear search / basics

- [ ] Find maximum in unsorted array
- [ ] Search in unsorted matrix with sorted rows conceptually later

## Binary search exact / bounds

- [ ] Binary Search
- [ ] Search Insert Position
- [ ] Find First and Last Position of Element in Sorted Array
- [ ] Sqrt(x) / integer square root

## Rotated / modified arrays

- [ ] Search in Rotated Sorted Array
- [ ] Find Minimum in Rotated Sorted Array
- [ ] Search a 2D Matrix (if sorted structure)

## Binary search on answer

- [ ] Koko Eating Bananas
- [ ] Capacity To Ship Packages Within D Days
- [ ] Split Array Largest Sum
- [ ] Aggressive Cows style problems
- [ ] Minimum Number of Days to Make m Bouquets

## Sorting fundamentals

- [ ] Implement Merge Sort
- [ ] Implement Quick Sort
- [ ] Sort an Array
- [ ] Sort Colors (Dutch national flag / counting)

## Sort + scan / intervals

- [ ] Merge Intervals
- [ ] Insert Interval
- [ ] Non-overlapping Intervals
- [ ] Meeting Rooms / Meeting Rooms II

## Sort + two pointers

- [ ] Two Sum II
- [ ] 3Sum
- [ ] 3Sum Closest
- [ ] Container With Most Water (not always sort-first, but two-pointer family)

## Duplicates / frequency

- [ ] Contains Duplicate
- [ ] Find All Duplicates in an Array
- [ ] Majority Element (sort or Boyer-Moore later)

## Selection / heap / quickselect

- [ ] Kth Largest Element in an Array
- [ ] Top K Frequent Elements (heap; sorting also works)

## Custom sort / greedy

- [ ] Largest Number
- [ ] Reorganize String
- [ ] Assign Cookies

## Non-comparison / bounded keys

- [ ] Sort Characters By Frequency
- [ ] Relative Sort Array

That's enough for a first deep pass.

---
# 30. For EVERY Searching / Sorting Problem

Use this thinking process:

```text
1. Is the data already sorted or partially sorted?

2. Am I searching for:
   - an index?
   - a value?
   - a feasibility threshold?
   - a count?

3. If binary search:
   - What is the invariant?
   - Closed [lo, hi] or half-open [lo, hi)?
   - What do I do when arr[mid] == target?

4. If binary search on answer:
   - What is the answer variable?
   - Is the predicate monotone?
   - What are low/high bounds?
   - How do I check feasibility in O(?)

5. Would sorting reveal structure?
   - neighbors
   - intervals
   - greedy order
   - two pointers

6. Do equal elements matter?
   - stability
   - first vs last occurrence

7. Can I avoid full sort?
   - hash set
   - heap for top k
   - quickselect for kth
   - counting for bounded keys

8. What is time complexity?
9. What is space complexity?
10. What edge cases break naive code?
11. Is in-place required?
12. Are original indices needed after sorting?
```

Question 11 is especially important.

If the problem asks for indexes in the original array, sorting the array directly may destroy information unless you sort pairs `(value, index)`.

---

# 31. Compare Multiple Approaches

Example:

## Contains Duplicate

### Brute force pairs

```text
Time:  O(n²)
Space: O(1)
```

### Sort and scan neighbors

```text
Time:  O(n log n)
Space: O(1) if in-place sort allowed
```

### Hash set

```text
Expected Time: O(n)
Space:         O(n)
```

Don't just conclude "hash set wins."

Ask:

```text
Is memory limited?
Is input already sorted?
Do I need deterministic worst-case?
Is modifying input allowed?
```

---

Example:

## Kth Largest Element

### Full sort descending

```text
Time: O(n log n)
```

### Heap of size k

```text
Time: O(n log k)
```

### Quickselect

```text
Average: O(n)
Worst:   O(n²) without careful pivot strategy
```

If k is small, heap is great.

If you need repeated k queries on changing data, different tools apply.

---

Example:

## Two Sum

### Unsorted

```text
Hash map: expected O(n)
```

### Sorted input

```text
Two pointers: O(n) after sort
Or already O(n) if given sorted
```

The "sorted" version is the bridge between sorting patterns and searching patterns.

---

# 32. Searching vs Hashing vs Trees

You already learned hash tables.

Now connect the trade-offs:

```text
Hash table
  expected O(1) lookup
  no order
  no easy predecessor/successor

Sorted array + BS
  O(log n) lookup
  order available
  range queries with bounds

Balanced BST
  O(log n) insert/search/delete with order
```

So when a problem needs:

```text
Only membership                → hash set
Sorted order / bounds          → sort + BS or BST
Dynamic order maintenance      → BST/heap later
```

Searching and sorting are often the **static offline** solution.

Trees are often the **dynamic ordered** solution.

---

# 33. Real-World Considerations

Big-O is not the whole story.

## Cache behavior

```text
Merge sort sequential merge can be cache-friendly
Quick sort in-place often fast in practice
Heap sort poorer locality
```

## Stability in systems

```text
Sorting records by multiple fields
Stable sort preserves prior ordering of equal keys
```

## External sorting

When data does not fit in memory:

```text
Chunk → sort chunks → k-way merge
```

Merge sort's structure matters here.

## Databases and indexes

```text
B-Trees / B+ Trees provide ordered access
Sorting concepts underpin index range scans
```

You don't need full database internals now, but know sorting/searching is not "interview-only."

---

# 34. One Especially Important Mental Shift

Beginners see:

```text
[4, 9, 1, 8, 2]
```

and think only about the raw values.

Stronger thinkers ask:

> **What structure can I impose so the rest becomes easy?**

Maybe:

```text
sort it
count frequencies
binary search on answer
compress coordinates
partition around pivot
merge intervals
```

Searching and sorting are often not the final goal.

They are **structure-building steps**.

---

# 35. Complexity Summary Tables

## Searching


| Technique            | Preconditions  | Time        | Space |
| -------------------- | -------------- | ----------- | ----- |
| Linear search        | none           | O(n)        | O(1)  |
| Binary search exact  | sorted         | O(log n)    | O(1)  |
| Lower/upper bound    | sorted         | O(log n)    | O(1)  |
| Rotated array search | rotated sorted | O(log n)*   | O(1)  |
| BS on answer         | monotone pred  | O(log R * f)| O(1)  |


\*With duplicates in rotated search, worst case may degrade.

## Sorting


| Algorithm       | Best       | Average      | Worst        | Space   | Stable |
| --------------- | ---------- | ------------ | ------------ | ------- | ------ |
| Bubble          | O(n)       | O(n²)        | O(n²)        | O(1)    | yes    |
| Selection       | O(n²)      | O(n²)        | O(n²)        | O(1)    | no     |
| Insertion       | O(n)       | O(n²)        | O(n²)        | O(1)    | yes    |
| Merge           | O(n log n) | O(n log n)   | O(n log n)   | O(n)    | yes    |
| Quick             | O(n log n) | O(n log n)   | O(n²)        | O(log n)| no     |
| Heap              | O(n log n) | O(n log n)   | O(n log n)   | O(1)    | no     |
| Counting          | O(n + k)   | O(n + k)     | O(n + k)     | O(k)    | yes    |
| Radix             | O(d(n+b))  | O(d(n+b))    | O(d(n+b))    | O(n+b)  | yes    |
| Bucket (average)  | O(n)       | O(n)         | O(n²)        | O(n)    | depends|


---

# 36. Binary Search on Answer — Worked Feasibility Example

## Problem shape

Given an array of weights and `D` days, find minimum ship capacity to ship all packages within `D` days.

## Monotone predicate

```text
can(capacity):
    simulate days needed greedily loading packages
    return days_needed <= D
```

If capacity `C` works, any capacity `> C` also works.

## Answer space

```text
low  = max(weights)          must fit largest package
high = sum(weights)          one trip carries all
```

## Why greedy check works

Load as much as possible each day without exceeding capacity.

That minimizes days for a fixed capacity.

## Pattern extracted

```text
Binary search on capacity
O(n) feasibility per step
O(n log sum) total
```

Whenever you see "minimum capacity/speed/time limit such that schedule works," test this pattern.

---

# 37. Merge Intervals — Canonical Sort + Scan

## Input

```text
[[1,3],[2,6],[8,10],[15,18]]
```

## Sort by start

```text
[[1,3],[2,6],[8,10],[15,18]]
```

## Scan

```text
current = [1,3]
[2,6] overlaps → merge to [1,6]
[8,10] no overlap → push [1,6], current=[8,10]
[15,18] no overlap → push [8,10], current=[15,18]
```

Result:

```text
[[1,6],[8,10],[15,18]]
```

## Invariant

After processing i intervals, merged list is valid for first i inputs.

## Edge cases

```text
empty list
single interval
one interval covers all
unsorted input
touching intervals [1,2],[2,3] → often considered overlapping
```

Always clarify inclusive/exclusive boundaries in the problem.

---

# 38. 3Sum — Sort + Two Pointers Composition

## Goal

Find triplets summing to zero.

## Step 1 — sort

```text
[-4, -1, -1, 0, 1, 2]
```

## Step 2 — fix one element, two-pointer rest

```text
fix i
left = i+1
right = n-1
while left < right:
    sum = a[i] + a[left] + a[right]
```

## Step 3 — skip duplicates

```text
skip duplicate i
skip duplicate left
skip duplicate right
```

This is not a new algorithm.

It is:

```text
Sorting
+
Two pointers
+
Duplicate handling discipline
```

Many medium problems are just that.

---

# 39. Quickselect — Find Kth Element Without Full Sort

## Idea

Partition like quick sort, but recurse only on side containing k.

```text
partition around pivot
if pivotIndex == k → done
if k < pivotIndex → recurse left
else → recurse right
```

## Complexity

```text
Average: O(n)
Worst:   O(n²)
```

## Practical note

Randomized pivot helps average case.

For interviews, know heap alternative:

```text
min-heap of size k → O(n log k)
```

Choose based on constraints.

---

# 40. Stability Example — Why It Matters

Sort these by score only:

```text
(90, Alice)
(75, Bob)
(90, Carol)
```

Unstable sort by score might yield:

```text
(75, Bob)
(90, Carol)
(90, Alice)
```

Stable sort yields:

```text
(75, Bob)
(90, Alice)
(90, Carol)
```

If the problem cares about original order among ties, stability matters.

Multi-key sorting trick:

```text
Sort by name (stable)
then sort by score (stable)
```

This is radix-sort thinking in disguise.

---

# Complete Searching & Sorting Checklist

```text
SEARCHING & SORTING
│
├── Foundations
│   ├── Why searching exists
│   ├── Linear search
│   ├── Sorted vs unsorted trade-offs
│   ├── Preprocessing vs query cost
│   └── Search vs hash vs tree
│
├── Binary Search Core
│   ├── Exact match
│   ├── First occurrence
│   ├── Last occurrence
│   ├── Lower bound
│   ├── Upper bound
│   ├── Half-open interval template
│   ├── Overflow-safe mid
│   └── Invariant-based reasoning
│
├── Binary Search Variants
│   ├── Rotated sorted array
│   ├── Find minimum in rotated array
│   ├── Duplicate handling
│   └── Search in structured matrices (concept)
│
├── Binary Search on Answer
│   ├── Monotone predicate
│   ├── Define low/high
│   ├── Feasibility check
│   ├── Minimize maximum
│   ├── Maximize minimum
│   └── Complexity with check cost
│
├── Sorting Concepts
│   ├── Why sorting helps
│   ├── Stability
│   ├── In-place vs extra memory
│   ├── Adaptive sorts
│   ├── Comparison lower bound Ω(n log n)
│   └── Choosing the right sort
│
├── Elementary Sorts
│   ├── Bubble sort
│   ├── Selection sort
│   └── Insertion sort
│
├── Core Efficient Sorts
│   ├── Merge sort
│   ├── Quick sort
│   ├── Partitioning
│   ├── Quickselect
│   └── Heap sort
│
├── Non-Comparison Sorts
│   ├── Counting sort
│   ├── Radix sort
│   └── Bucket sort
│
├── Core Patterns
│   ├── Linear scan
│   ├── Binary search exact
│   ├── Bounds search (first/last)
│   ├── Rotated array sorted-half BS
│   ├── Binary search on answer
│   ├── Sort + scan
│   ├── Sort + two pointers
│   ├── Interval merging
│   ├── Duplicate detection
│   ├── Partitioning / quickselect
│   ├── Merge sorted sequences
│   ├── Counting/bucket for bounded keys
│   ├── Greedy after sort
│   └── Coordinate compression
│
├── Problem Solving
│   ├── Pattern recognition
│   ├── Edge cases
│   ├── Compare multiple approaches
│   ├── Preserve original indices when needed
│   └── Avoid unnecessary full sort
│
├── Implement Yourself
│   ├── linearSearch
│   ├── lowerBound / upperBound
│   ├── first/last occurrence
│   ├── searchRotated
│   ├── BS on answer template
│   ├── insertionSort
│   ├── mergeSort
│   ├── quickSort partition
│   ├── countingSort
│   └── merge two sorted arrays
│
└── For EVERY Problem
    ├── Is data sorted or sortable?
    ├── Index, value, or threshold?
    ├── State the BS invariant
    ├── Check monotonicity for BS on answer
    ├── Consider hash/tree alternatives
    ├── Need stability?
    ├── Need in-place?
    ├── Time complexity
    ├── Space complexity
    ├── Edge cases
    └── Explain WHY the pattern works
```

For your **core DSA pass**, this is enough Searching & Sorting depth.

Once you can:

- implement `lowerBound`/`upperBound` without hesitation,
- explain the binary search invariant in one sentence,
- recognize binary search on answer from problem wording,
- implement merge sort and quick sort partition,
- and spot when a problem is really **sort + scan** or **sort + two pointers**,

you are ready for the next major block:

```text
Trees
→ Binary Trees
→ Tree DFS/BFS
→ Binary Search Trees
→ Balanced BSTs
→ Heaps / Priority Queues
→ Tries
```

Heap Sort gives you a preview of heaps here; study heaps deeply as a data structure in the Trees block, not only as a sorting subroutine.

That progression stays clean:

```text
Arrays & Strings
        ↓
Linked Lists
        ↓
Stacks → Queues → Deques
        ↓
Hash Tables / Hashing
        ↓
Searching & Sorting
        ↓
Trees
```

Each layer gives you machinery reused in the next.
