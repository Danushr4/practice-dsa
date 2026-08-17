# Arrays & Strings

Phase 2 from your learning map: **Arrays & Strings** — foundation for memory layout and many algorithms.

Before patterns, understand what an array actually is. Strings are closely related: a string is typically a sequence of characters backed by array-like storage, with extra conventions around encoding and mutability.

---

# 1. Array Fundamentals — Understand the Structure

First understand **why arrays exist**.

An array stores elements in **contiguous memory**:

```text
ARRAY

Address:
1000    1004    1008    1012    1016

[10]    [20]    [30]    [40]    [50]
index:   0       1       2       3       4
```

Each element occupies a fixed amount of memory. For `int` (4 bytes on many systems):

```text
index 0 → address 1000
index 1 → address 1004
index 2 → address 1008
```

The formula:

```text
address(i) = base_address + i × element_size
```

That is why **random access is O(1)** — no traversal required.

Conceptually:

```text
Array
┌─────────────────────────────────────┐
│ base pointer / reference            │
├─────────────────────────────────────┤
│ length / size                       │
├─────────────────────────────────────┤
│ contiguous block of elements        │
│ [e0][e1][e2][e3]...                 │
└─────────────────────────────────────┘
```

Understand:

- What is contiguous memory?
- What is an index?
- What is `base_address`?
- Why is `a[i]` O(1)?
- What is cache locality?
- Why do CPUs love arrays?
- What is size vs capacity for dynamic arrays?
- Why is middle insertion O(n)?

---

# 2. Why Arrays Exist

Compare finding element `40` in:

```text
[10][20][30][40][50]
```

You can jump directly to index 3 if you know the index.

Compare a linked structure where you must walk:

```text
10 → 20 → 30 → 40 → 50
```

Arrays trade **flexible structural change** for **fast indexed access** and **memory locality**.

Arrays give excellent:

```text
random access      O(1)
cache locality     excellent
compact storage    no per-element pointers
SIMD-friendly      contiguous layout
```

Arrays are weaker when:

```text
frequent insert/delete in middle   O(n) shifts
unknown final size upfront         need dynamic resizing
non-uniform element sizes          awkward fit
```

So don't learn:

> Array is always better.

Learn:

> Under what conditions does array layout win?

---

# 3. Static vs Dynamic Arrays

## Static Array

Fixed size at compile time (C/C++):

```cpp
int arr[5] = {10, 20, 30, 40, 50};
```

```text
Size: 5 (fixed)
Cannot grow without new allocation
```

## Dynamic Array

Growable array (C++ `vector`, Java `ArrayList`, Python `list`):

```text
size     = number of elements currently stored
capacity = allocated slots (may be larger than size)
```

Example:

```text
size = 3, capacity = 8

[10][20][30][ _ ][ _ ][ _ ][ _ ]
```

Append when capacity is full:

```text
1. Allocate new block (often 2× capacity)
2. Copy all existing elements
3. Append new element
4. Free old block
```

### Amortized O(1) Append

Single append can cost O(n) when resizing.

But doubling strategy means total copy work over n appends is O(n).

So average per append:

```text
O(1) amortized
```

### Shrinking (optional policy)

Some implementations shrink when size drops below capacity/4 to avoid wasting memory.

Understand the trade-off:

```text
Frequent resize → more copy cost
Rare resize     → more wasted capacity
```

---

# 4. Implement a Vector (Practice Exercise)

Practice coding using arrays and pointer math.

Reference implementation lives in:

```text
array/arrayList.cpp
```

**API to implement:**

| Method | Description |
| --- | --- |
| `size()` | Number of items |
| `capacity()` | Number of slots allocated |
| `is_empty()` | Whether empty |
| `at(index)` | Return item at index; error if out of bounds |
| `push(item)` | Append to end |
| `insert(index, item)` | Insert at index, shift right |
| `prepend(item)` | Insert at index 0 |
| `pop()` | Remove from end, return value |
| `delete(index)` | Delete at index, shift left |
| `remove(item)` | Remove first occurrence of value |
| `find(item)` | First index with value, or -1 |
| `resize(new_capacity)` | Private resize helper |

**Resizing rules (classic):**

- When `size == capacity`, resize to **2× capacity**
- When popping and `size == capacity/4`, resize to **half** (optional)

**Time:**

- O(1) access, update at index
- O(1) amortized push/pop at end
- O(n) insert/delete elsewhere

**Space:**

- Contiguous → proximity helps performance
- O(capacity) memory; capacity ≥ size

---

# 5. Memory Layout — 1D and 2D

## 1D Array

```text
[a0][a1][a2][a3][a4]
```

Single contiguous block.

## 2D Array (Matrix)

Logical view:

```text
1  2  3
4  5  6
7  8  9
```

Physical layout is still **one contiguous block** in most languages.

### Row-Major (C, C++, Java, Python)

```text
1 2 3 4 5 6 7 8 9
```

Formula:

```text
index(row, col) = row × cols + col
```

### Column-Major (Fortran, some linear algebra libs)

```text
1 4 7 2 5 8 3 6 9
```

Formula:

```text
index(row, col) = col × rows + row
```

Why this matters:

```text
Row-major + row-by-row traversal → cache-friendly
Column-major + row-by-row traversal → cache-unfriendly
```

Detailed matrix patterns: see `array/matrix_2dTra.md`.

---

# 6. Arrays vs Linked Structures

| Property | Array | Linked List |
| --- | --- | --- |
| Random access | O(1) | O(n) |
| Search unsorted | O(n) | O(n) |
| Insert at front | O(n) | O(1) |
| Insert at end | O(1) amortized | O(1) with tail |
| Insert middle | O(n) | O(1) if node known |
| Delete middle | O(n) | O(1) if node known |
| Cache locality | Excellent | Poor |
| Extra memory | Minimal | Pointer per node |
| Contiguous | Yes | No |

Arrays are the default for:

```text
numeric computation
index-based algorithms
sorting
binary search
prefix sums
```

Linked structures win when:

```text
frequent front/middle structural edits
unknown size with no resize amortization concern
stable iterators during mutation (with care)
```

---

# 7. String Fundamentals

A string is a **sequence of characters** — conceptually an array of chars with string semantics.

```text
"hello"
 h e l l o
 0 1 2 3 4
```

## Mutable vs Immutable

### Immutable (Java `String`, Python `str`)

```text
s = "hello"
s = s + " world"   // often creates NEW string
```

Concatenation may cost O(n) per operation.

Use `StringBuilder` / `list` + `join` for many appends.

### Mutable (C `char[]`, C++ `std::string`)

Can modify in place when buffer has room.

```cpp
std::string s = "hello";
s[0] = 'H';  // "Hello"
```

## Character Encoding Basics

Computers store characters as numbers.

### ASCII

```text
'A' = 65
'a' = 97
'0' = 48
```

7-bit, 128 characters. Fine for English.

### Unicode

Abstract code points for all scripts:

```text
U+0041 = 'A'
U+1F600 = 😀
```

### UTF-8

Variable-length encoding (1–4 bytes per code point).

```text
ASCII chars → 1 byte
many others → 2–4 bytes
```

### UTF-16

Often 2 bytes per code unit; surrogate pairs for supplementary chars.

**Interview default:** treat string problems as sequences of characters unless encoding complexity is explicit.

## String Operations — Complexity

| Operation | Typical complexity |
| --- | --- |
| Access `s[i]` | O(1) |
| Length | O(1) if stored |
| Compare | O(n) |
| Concatenate immutable | O(n) new allocation |
| Substring | O(k) copy of k chars |
| Find char | O(n) naive |

## Lexicographical Comparison

Compare character by character:

```text
"apple" < "banana"  because 'a' < 'b'
"abc" < "abd"       because same prefix, then 'c' < 'd'
```

## Character / Frequency Tables

For lowercase English letters:

```cpp
int freq[26] = {0};
freq[s[i] - 'a']++;
```

Fixed small alphabet → array beats hash map.

Advanced string algorithms (KMP, Z, suffix structures) → Phase 13 String Algorithms.

---

# 8. Core Array & String Operations + Complexity

Know the **reasoning**, not just the table.

| Operation | Typical complexity | Why |
| --- | --- | --- |
| Access `a[i]` | O(1) | Address arithmetic |
| Update `a[i]` | O(1) | Direct write |
| Search unsorted | O(n) | May scan all |
| Search sorted | O(log n) | Binary search |
| Insert at end (dynamic) | O(1) amortized | Resize occasionally |
| Insert at front/middle | O(n) | Shift elements |
| Delete front/middle | O(n) | Shift elements |
| Delete at end | O(1) | No shift |
| Copy array | O(n) | Visit each element |
| Reverse | O(n) | Swap pairs |
| Sort | O(n log n) typical | Comparison sort |

For every solution you write, ask:

```text
Time:  O(?) — count dominant operations
Space: O(?) — extra structures, recursion stack
Why:   explain the bound
```

---

# 9. Reference — Detailed Pattern Files

This `notes.md` is the **master syllabus**. Deep dives live in sibling files:

| File | Topic |
| --- | --- |
| `array/twoptr.md` | Two Pointers — opposite, same direction, gap |
| `array/slidingwindow.md` | Fixed & variable sliding window |
| `array/prefixsum-hashmap.md` | Prefix sum, difference array, hash combos |
| `array/matrix_2dTra.md` | 2D traversal, spiral, diagonals, rotation |
| `array/sortscan.md` | Sort + scan, intervals, merging |
| `array/binarysearch-inplace.md` | Binary search variants, lower/upper bound |
| `array/note.md` | Original outline and resources |
| `array/arrayList.cpp` | Dynamic array implementation |
| `array/searchSort/search.cpp` | Search algorithms |
| `array/searchSort/sort.cpp` | Sort implementations |
| `array/linearScan/code.cpp` | Linear scan examples |

Read this file first for the map. Drill each pattern file for mastery.

---

# 10. The Most Important Array & String Patterns

These patterns appear constantly. For each: **why it exists**, **when to recognize it**, **how it works**, **complexity**, **trade-offs**, **problems**.

---

## Pattern 1 — Linear Scan / Traversal

The foundation of everything.

```text
i →
[2][4][1][7][3]
```

Single pass:

```cpp
for (int i = 0; i < n; i++) {
    process(a[i]);
}
```

Variants:

```text
forward scan
reverse scan
track running min/max
track running sum/count
simultaneous state (e.g. best so far)
```

The fundamental question:

> Can I obtain the answer while looking at each element once?

Examples:

- Find maximum
- Count evens
- Running average
- Single-pass validation

Complexity:

```text
Time  O(n)
Space O(1) extra (excluding input)
```

When NOT enough:

```text
need all pairs        → O(n²) or better pattern
need subarray property → sliding window / prefix sum
need past lookup       → hash map
```

---

## Pattern 2 — Two Pointers

Maintain two indices and move them intelligently.

**Deep dive:** `array/twoptr.md`

### Variant A — Opposite Direction

```text
L → [............] ← R
```

Start at both ends; move based on comparison.

```cpp
int left = 0, right = n - 1;
while (left < right) {
  // compare a[left], a[right]
  // move left++ or right-- or both
}
```

Works when:

```text
sorted array + pair sum
palindrome check
container with water style maximization
```

Examples:

- Two Sum II (sorted)
- Valid Palindrome
- Reverse String / Array
- 3Sum (outer loop + two pointers)

### Variant B — Same Direction (Slow / Fast)

```text
slow →
fast ----->
```

```cpp
int slow = 0;
for (int fast = 0; fast < n; fast++) {
  // maybe write at slow, then slow++
}
```

Works when:

```text
in-place filtering
remove duplicates
move zeroes
partition-like compaction
```

Examples:

- Remove Duplicates from Sorted Array
- Move Zeroes
- Remove Element

### Variant C — Two Arrays / Merge Style

```text
A: i →
B: j →
```

Examples:

- Merge Sorted Array
- Intersection of Two Arrays II

### Why Two Pointers Beat Nested Loops

Nested loops on sorted data:

```text
O(n²) — check all pairs
```

Two pointers exploit ordering:

```text
if sum too small → move left right
if sum too large → move right left
```

Only O(n) moves total.

Complexity:

```text
Time  O(n) typically
Space O(1)
```

Recognition clues:

```text
sorted input
pair in array
in-place filter
palindrome
merge two sequences
```

---

## Pattern 3 — Sliding Window

A window is a **contiguous** portion of array or string.

**Deep dive:** `array/slidingwindow.md`

```text
[2, 1, 5, 1, 3, 2]
     [1, 5, 1]   ← valid window
```

NOT a window:

```text
2, 5, 3  (skipped elements)
```

Recognition clue:

```text
subarray / substring / consecutive / contiguous
maximum/minimum length sum with constraint
```

### Fixed Window Size k

```text
[  window of k  ] →
```

Template:

```cpp
// build first window
for (int i = 0; i < k; i++) window += a[i];
best = window;
for (int i = k; i < n; i++) {
  window += a[i] - a[i - k];  // add new, remove old
  best = max(best, window);
}
```

Examples:

- Maximum Average Subarray I
- Sliding Window Maximum (needs deque — harder)

Complexity:

```text
Time  O(n)
Space O(1) or O(k) for frequency map
```

### Variable Window

Expand right until invalid; shrink left until valid.

```text
L → [ window ] → R
```

Template sketch:

```cpp
int left = 0;
for (int right = 0; right < n; right++) {
  add(a[right]);
  while (invalid) {
    remove(a[left]);
    left++;
  }
  update_answer(left, right);
}
```

Examples:

- Longest Substring Without Repeating Characters
- Minimum Size Subarray Sum
- Longest Repeating Character Replacement

Key question:

> When is it valid to expand/shrink without reconsidering everything already processed?

Answer: when window property is **monotonic** with respect to expansion/contraction.

Complexity:

```text
Time  O(n) — each element enters/leaves window once
Space O(k) or O(alphabet) for counts
```

---

## Pattern 4 — Prefix Sum

Precompute cumulative sums for fast range queries.

**Deep dive:** `array/prefixsum-hashmap.md`

Array:

```text
a:      2  4  1  7  3
prefix: 0  2  6  7 14 17   (prefix[0]=0 style)
```

Range sum L..R:

```text
sum(L, R) = prefix[R+1] - prefix[L]
```

Build: O(n). Query: O(1).

Examples:

- Range Sum Query Immutable
- Subarray Sum Equals K (combine with hash map)
- Product of Array Except Self (prefix + suffix)

### 2D Prefix Sum (basics)

```text
sum(rect) = inclusion-exclusion on four corners of prefix table
```

Useful for:

```text
count cells in rectangle
number of submatrices with sum target (harder)
```

### Prefix Frequency / Counts

Prefix of counts enables range frequency queries on static data.

---

## Pattern 5 — Difference Array

Complement to prefix sum.

Prefix sum answers:

> What is cumulative value up to index i?

Difference array answers:

> Apply many range updates efficiently.

Given initial array, range update "add v to [L, R]" many times.

Difference array `diff`:

```text
diff[L]   += v
diff[R+1] -= v   (if R+1 exists)
```

After all updates, prefix sum of `diff` yields final array.

Build diff from array:

```text
diff[i] = a[i] - a[i-1]
```

Use cases:

```text
multiple range add operations
flight booking / capacity scheduling
sweep line preprocessing
```

Complexity:

```text
O(1) per range update
O(n) to reconstruct final array
```

---

## Pattern 6 — Frequency Counting

Count occurrences of values or characters.

### Array as Frequency Table

When key range is small and known:

```cpp
int freq[26];
freq[c - 'a']++;
```

```text
Time  O(n)
Space O(alphabet) = O(1) for fixed alphabet
```

Better than hash map when:

```text
keys in [0, 1000]
lowercase letters
ASCII range
```

### Hash Map Frequency

```cpp
unordered_map<int, int> freq;
freq[x]++;
```

When keys are sparse or large.

Examples:

- Valid Anagram
- Find All Anagrams in a String
- Majority Element (array freq or Boyer-Moore)
- Top K Frequent Elements (freq + heap/bucket)

Recognition:

```text
"how many times"
"same frequency"
"anagram"
"group by count"
```

---

## Pattern 7 — Hash Map with Array (Complement / Seen-Before)

Not just frequency — store **information from past** for O(1) lookup.

Classic Two Sum insight:

```text
current = x
need    = target - x

Have I seen `need` before?
```

```cpp
unordered_map<int, int> seen; // value → index
for (int i = 0; i < n; i++) {
  int need = target - a[i];
  if (seen.count(need)) return {seen[need], i};
  seen[a[i]] = i;
}
```

**Deep dive:** `array/prefixsum-hashmap.md`

Variants:

```text
value → index           Two Sum
prefix sum → count      Subarray Sum Equals K
char → last index       sliding window anagrams
canonical key → group   group anagrams
```

Critical design question:

> Insert before check or after check?

For Two Sum: check **before** inserting current to avoid pairing element with itself incorrectly.

Complexity:

```text
Expected Time O(n)
Space       O(n)
Worst-case hash: O(n²) — rare with good hash
```

---

## Pattern 8 — Sort + Scan

Sort to bring related elements together.

```text
Unstructured data
      ↓
     Sort
      ↓
Ordered structure
      ↓
Easy linear scan
```

**Deep dive:** `array/sortscan.md`

Examples:

- Contains Duplicate (sort + check neighbors)
- Merge Intervals
- Non-overlapping Intervals
- 3Sum (sort + fix one + two pointers)

Complexity:

```text
Sort:  O(n log n)
Scan:  O(n)
Total: O(n log n)
```

Always ask:

> Is O(n log n) sort worth making the rest easy?

Compare to hash map O(n) when sorting isn't required.

When sort helps:

```text
duplicates adjacent after sort
intervals by start time
greedy interval scheduling
pair problems with ordering
```

---

## Pattern 9 — Binary Search

Beyond "find x in sorted array."

**Deep dive:** `array/binarysearch-inplace.md`

Core requirement:

> **Monotonic predicate** over search space.

```text
false false false true true true
              ↑
           boundary
```

Variants:

```text
exact match
first occurrence
last occurrence
lower bound (first >= x)
upper bound (first > x)
rotated sorted array
binary search on answer (min/max feasible value)
```

Template (lower bound style):

```cpp
int lo = 0, hi = n; // or n-1 depending on variant
while (lo < hi) {
  int mid = lo + (hi - lo) / 2;
  if (predicate(mid)) hi = mid;
  else lo = mid + 1;
}
return lo;
```

Examples:

- Search Insert Position
- Find First and Last Position
- Search in Rotated Sorted Array
- Koko Eating Bananas (BS on answer)
- Minimum Capacity to Ship Packages (BS on answer)

Complexity:

```text
Time  O(log n) per search on n elements
Space O(1)
```

Recognition:

```text
sorted or monotonic
find boundary of yes/no
minimize maximum / maximize minimum
```

---

## Pattern 10 — In-Place Manipulation

Reduce space to O(1) by overwriting using read/write pointers.

Techniques:

```text
swap-based reversal
partition (elements < pivot left, >= right)
compaction (write index slow, read index fast)
rotation via reversal trick
cycle-based placement (each element moves to unique slot)
```

Examples:

- Reverse Array / String
- Rotate Array (reverse segments)
- Move Zeroes
- Remove Duplicates
- Sort Colors (see Dutch National Flag)

Read pointer / write pointer:

```cpp
int write = 0;
for (int read = 0; read < n; read++) {
  if (should_keep(a[read])) {
    a[write] = a[read];
    write++;
  }
}
// new logical size = write
```

Complexity:

```text
Time  O(n)
Space O(1)
```

Caution:

```text
don't lose data you still need
handle overwrite order carefully
clarify if extra space allowed
```

---

## Pattern 11 — Matrix / 2D Array Traversal

Matrix is array with two coordinates.

**Deep dive:** `array/matrix_2dTra.md`

```text
matrix[row][col]
0 <= row < rows
0 <= col < cols
```

Traversal types:

```text
row by row
column by column
diagonals
boundary
spiral
BFS/DFS on grid (graphs phase — preview)
```

Row-major cache tip:

```cpp
for (int r = 0; r < rows; r++)
  for (int c = 0; c < cols; c++)
    process(matrix[r][c]);
```

Examples:

- Matrix Diagonal Sum
- Spiral Matrix
- Rotate Image (transpose + reverse rows)
- Set Matrix Zeroes
- Search a 2D Matrix (binary search on flattened or row/col logic)

Complexity:

```text
Time  O(rows × cols) for full traversal
Space O(1) for in-place transforms
```

Recognition:

```text
grid / matrix / 2D
neighbors up/down/left/right
paths in grid
```

---

## Pattern 12 — Kadane's Algorithm (Maximum Subarray)

Find maximum sum of contiguous subarray.

```text
[-2, 1, -3, 4, -1, 2, 1, -5, 4]
              ↑────────────↑
              sum = 6
```

Key idea:

```text
at each index, either extend previous subarray or start new at i
```

```cpp
int best = a[0], cur = a[0];
for (int i = 1; i < n; i++) {
  cur = max(a[i], cur + a[i]);
  best = max(best, cur);
}
```

Why it works:

```text
if cur + a[i] < a[i], previous segment hurts — drop it
```

Variants:

```text
maximum subarray sum
maximum product subarray (track min and max)
maximum circular subarray
longest turbulent subarray
```

Complexity:

```text
Time  O(n)
Space O(1)
```

Recognition:

```text
contiguous subarray
maximum/minimum sum/product
```

---

## Pattern 13 — Dutch National Flag (Three-Way Partition)

Sort array with few distinct values in O(n) — typically 0, 1, 2.

```text
0 0 1 2 1 0 2
↓       ↑     ↓
low    mid   high
```

Invariant:

```text
[0..low-1]     = 0
[low..mid-1]   = 1
[mid..high]    = unknown
[high+1..n-1]  = 2
```

```cpp
int low = 0, mid = 0, high = n - 1;
while (mid <= high) {
  if (a[mid] == 0) swap(a[low++], a[mid++]);
  else if (a[mid] == 1) mid++;
  else swap(a[mid], a[high--]);
}
```

Generalizes to **quickselect partition** step in quicksort.

Examples:

- Sort Colors
- Wiggle Sort II (related partitioning ideas)

Complexity:

```text
Time  O(n)
Space O(1)
```

Recognition:

```text
three categories
in-place sort of small alphabet
partition around pivot
```

---

## Pattern 14 — Cyclic Rotation / Modular Indexing

Array treated as circular.

```text
rotate right by k:

[1,2,3,4,5]  k=2  →  [4,5,1,2,3]
```

Techniques:

### Reversal Trick

```text
reverse whole array
reverse first k
reverse rest
```

### Modular index

```cpp
int idx = (i + k) % n;
```

Examples:

- Rotate Array
- Rotate Image (90° as transpose + flip)
- Circular Array Loop (graph + fast/slow preview)

Caution:

```text
k may be larger than n → k %= n
k = 0 edge case
```

Complexity:

```text
Reversal: O(n) time, O(1) space
New array copy: O(n) time, O(n) space
```

---

## Pattern 15 — Interval Merge / Sweep

Intervals as array of pairs `[start, end]`.

**Overlap with:** `array/sortscan.md`

Sort by start:

```text
[1,3] [2,6] [8,10] [15,18]
```

Merge overlapping:

```text
[1,6] [8,10] [15,18]
```

Template:

```cpp
sort(intervals.begin(), intervals.end());
vector<pair<int,int>> merged;
for (auto& in : intervals) {
  if (merged.empty() || in.first > merged.back().second)
    merged.push_back(in);
  else
    merged.back().second = max(merged.back().second, in.second);
}
```

Examples:

- Merge Intervals
- Insert Interval
- Meeting Rooms II (sweep line + min heap)
- Non-overlapping Intervals (greedy)

Recognition:

```text
start/end pairs
overlapping ranges
schedule conflicts
```

---

## Pattern 16 — String-Specific Patterns

Beyond array patterns with char semantics.

### Palindrome

```text
two pointers from ends
or reverse half and compare
```

Examples: Valid Palindrome, Longest Palindrome Substring (expand around center)

### Anagram

```text
frequency count match
or sort both strings
```

Examples: Valid Anagram, Group Anagrams

### Substring vs Subsequence

```text
substring = contiguous
subsequence = order preserved, gaps allowed
```

Different patterns:

```text
substring → sliding window often
subsequence → two pointers / DP later
```

### Expand Around Center

For palindrome length at each center:

```cpp
expand(l, r) while s[l]==s[r]
```

O(n²) for all centers — fine for many interview sizes.

### String Builder Pattern

Many appends:

```cpp
ostringstream / StringBuilder / vector<char> + join
```

Avoid O(n²) repeated immutable concatenation.

---

# 11. Pattern Composition — Combining Ideas

Real problems combine patterns.

### Palindrome Linked List style (on arrays)

```text
reverse second half + compare
```

### 3Sum

```text
sort + outer index + two pointers
```

### Subarray Sum Equals K

```text
prefix sum + hash map (count prefix differences)
```

### Longest Substring with K Distinct

```text
variable sliding window + frequency map
```

### Search 2D Matrix

```text
binary search on rows/columns OR treat as sorted 1D
```

### Rotate Image

```text
matrix transpose + row reversal
```

### Product Except Self

```text
prefix products + suffix products (or one-pass optimized)
```

When stuck, ask:

> Which two patterns from my toolkit compose here?

That's where DSA becomes powerful — not 200 isolated tricks.

---

# 12. Pattern Recognition — Train This Deliberately

When you see a problem, notice clues:

```text
Contiguous subarray/substring
        ↓
Sliding Window / Prefix Sum / Kadane
```

```text
Sorted array
        ↓
Two Pointers / Binary Search
```

```text
Repeated lookup / "have I seen?"
        ↓
Hash Map / Set / Frequency array
```

```text
Range sum queries (static)
        ↓
Prefix Sum
```

```text
Many range updates
        ↓
Difference Array
```

```text
Need O(1) extra memory
        ↓
In-place / Two Pointers
```

```text
Pairs in sorted data
        ↓
Two Pointers
```

```text
Monotonic yes/no over index or value
        ↓
Binary Search (on array or on answer)
```

```text
2D grid/matrix
        ↓
Matrix traversal (see matrix_2dTra.md)
```

```text
Three categories 0/1/2
        ↓
Dutch National Flag
```

```text
Intervals [start, end]
        ↓
Sort + Scan / Merge
```

```text
Small fixed alphabet (a-z)
        ↓
Array frequency table (not hash)
```

```text
Maximum contiguous subarray sum
        ↓
Kadane
```

Treat these as **clues**, not rigid rules.

Sometimes multiple patterns apply — compare trade-offs.

---

# 13. Edge Cases — Extremely Important

Array and string code breaks on edge cases. Always test:

### Empty

```text
n = 0
empty string ""
return appropriate default (0, "", true, etc.)
```

### Single Element

```text
[5]
"a"
```

### Two Elements

```text
[1, 2]
"ab"
```

### All Same

```text
[7, 7, 7, 7]
```

### Already Sorted / Reverse Sorted

```text
[1,2,3,4]
[4,3,2,1]
```

### Duplicates

```text
[1, 1, 2, 2]
```

### Negative Numbers

```text
prefix sum / Kadane / product subarray
```

### Zeros

```text
move zeroes, product subarray, division
```

### Integer Overflow

```text
sum of large values
use long long
check intermediate products
```

### Off-by-One

```text
window size k vs indices 0..n-1
prefix[L-1] when L=0
binary search boundaries lo/hi
```

### String Encoding

```text
unless problem states UTF complexities, assume simple char access
```

### Matrix

```text
single row / single column
non-square matrix
```

### Rotation

```text
k = 0
k > n (use k % n)
k = n
```

### Intervals

```text
touching intervals [1,2] and [2,3] — clarify if overlap
single interval
nested intervals
```

---

# 14. Compare Multiple Approaches

For every problem, sketch alternatives.

## Example — Two Sum

### Brute force

```text
all pairs

Time  O(n²)
Space O(1)
```

### Sort + two pointers

```text
sort, then scan (loses original indices unless tracked)

Time  O(n log n)
Space O(n) for index mapping or sort
```

### Hash map

```text
seen complement

Expected Time O(n)
Space         O(n)
```

Don't just say "hash map wins."

Ask:

```text
need original indices?
memory constrained?
input already sorted?
worst-case hash concerns?
```

## Example — Contains Duplicate

```text
Brute O(n²)
Sort + scan neighbors O(n log n)
Hash set O(n) expected
Array freq if values bounded
```

## Example — Maximum Subarray

```text
Brute all subarrays O(n²) or O(n³)
Prefix sum + enumerate O(n²)
Kadane O(n)
```

Comparing approaches builds real algorithmic reasoning.

---

# 15. Trade-offs Summary

| Need | Often choose | Why |
| --- | --- | --- |
| Fast index access | Array | O(1) address math |
| Fast front insert | Linked structure | O(1) if head |
| Subarray sum many queries | Prefix sum | O(1) query |
| Many range adds | Difference array | O(1) update |
| Pair in unsorted | Hash map | O(n) |
| Pair in sorted | Two pointers | O(n), O(1) space |
| Contiguous constraint | Sliding window | O(n) |
| Sorted search | Binary search | O(log n) |
| O(1) space filter | In-place pointers | No extra array |
| Small alphabet freq | int[] | Faster than hash |
| Sparse keys | unordered_map | Space proportional to keys |
| Bring equals together | Sort | Enables scan |
| 2D neighbors | Matrix traversal | Direct coordinates |

---

# 16. For EVERY Array & String Problem

Use this thinking process:

```text
1. What is the input structure?
   1D array? string? matrix? sorted?

2. What is the output?
   index? value? count? boolean? modified array?

3. What property am I looking for?
   contiguous? pairs? range? frequency? ordering?

4. Which pattern fits?
   scan / two ptr / window / prefix / hash / sort / BS / in-place / kadane / DNF / matrix

5. Can I combine patterns?

6. What invariant do I maintain?
   window valid? pointers region? prefix definition?

7. Time complexity?
   Count operations — dominant term?

8. Space complexity?
   Extra arrays? hash? recursion stack?

9. Edge cases?
   empty, one, two, duplicates, negatives, overflow

10. Can I do better?
    Brute first, then optimize with pattern

11. Does detailed pattern file cover this?
    twoptr.md, slidingwindow.md, etc.

12. Why does it work?
    Explain correctness in words — not just code
```

Question about **invariant** is especially important for sliding window and two pointers.

---

# 17. Important Implementation Skills

## Draw Indices on Small Examples

For two pointers:

```text
L → [2][4][6][8] ← R
```

For sliding window:

```text
L → [1 5 1] → R
```

For Dutch National Flag:

```text
low mid high pointers on paper
```

## Write Brute Force First (Sometimes)

Clarifies what you're optimizing away.

## Template Then Customize

Binary search and sliding window have many boundary variants.

Customize `lo/hi` and `while` condition per problem.

## Language Details

```text
C++: vector, string, array
Java: int[], String, StringBuilder
Python: list, str (immutable), join
```

Know whether your string is mutable.

## Avoid Hidden O(n²) on Strings

```text
s = s + c   // bad in loop for immutable strings
```

---

# 18. Problems to Practice

You don't need 100 random problems. Choose representatives per pattern.

### Fundamentals

- [ ] Implement Dynamic Array (Vector)
- [ ] Reverse String / Array
- [ ] Find Max / Second Max
- [ ] Merge Two Sorted Arrays

### Linear Scan

- [ ] Best Time to Buy and Sell Stock
- [ ] Maximum Subarray (Kadane)
- [ ] Contains Duplicate (multiple approaches)

### Two Pointers

- [ ] Two Sum II
- [ ] 3Sum
- [ ] Valid Palindrome
- [ ] Remove Duplicates from Sorted Array
- [ ] Move Zeroes
- [ ] Container With Most Water

**More:** `array/twoptr.md`

### Sliding Window

- [ ] Maximum Average Subarray I
- [ ] Longest Substring Without Repeating Characters
- [ ] Minimum Size Subarray Sum
- [ ] Permutation in String
- [ ] Longest Repeating Character Replacement

**More:** `array/slidingwindow.md`

### Prefix Sum / Difference

- [ ] Range Sum Query Immutable
- [ ] Subarray Sum Equals K
- [ ] Product of Array Except Self
- [ ] Corporate Flight Bookings (difference array)

**More:** `array/prefixsum-hashmap.md`

### Hash Map + Array

- [ ] Two Sum
- [ ] Group Anagrams
- [ ] Valid Anagram
- [ ] Find All Anagrams in a String

### Sort + Scan

- [ ] Merge Intervals
- [ ] Non-overlapping Intervals
- [ ] Insert Interval
- [ ] Meeting Rooms

**More:** `array/sortscan.md`

### Binary Search

- [ ] Binary Search
- [ ] Search Insert Position
- [ ] Find First and Last Position
- [ ] Search in Rotated Sorted Array
- [ ] Koko Eating Bananas

**More:** `array/binarysearch-inplace.md`

### In-Place / Partition

- [ ] Sort Colors (Dutch National Flag)
- [ ] Rotate Array
- [ ] Remove Element
- [ ] Wiggle Sort

### Matrix

- [ ] Spiral Matrix
- [ ] Rotate Image
- [ ] Set Matrix Zeroes
- [ ] Search a 2D Matrix

**More:** `array/matrix_2dTra.md`

### String

- [ ] Longest Palindrome Substring
- [ ] Valid Anagram
- [ ] Implement strStr() (basic / KMP later)

### Composition (Harder)

- [ ] Trapping Rain Water
- [ ] Substring with Concatenation of All Words
- [ ] First Missing Positive (cycle placement)
- [ ] Maximum Product Subarray

That set covers the major array & string patterns deeply.

---

# 19. Complexity Analysis Checklist

For every solution:

**Time**

```text
O(1)     — direct access, hash lookup expected
O(log n) — binary search
O(n)     — single scan, two pointers, sliding window, kadane
O(n log n) — sort + scan
O(n²)    — nested loops, all subarrays naive
```

**Space**

```text
O(1)     — in-place pointers
O(n)     — hash map, prefix array, output array
O(k)     — window frequency, alphabet size
```

**Amortized**

```text
dynamic array append — O(1) amortized
```

Always ask **why** the bound holds.

---

# 20. Real Systems — Where Arrays Matter

Arrays aren't just interview puzzles.

```text
CPU caches       — contiguous access fast
Image buffers    — pixel rows
Database columns — columnar storage
Ring buffers     — queues in systems
Memory allocators — contiguous blocks
GPU kernels      — parallel array ops
Time series      — sequential numeric data
String processing — parsers, logs, protocols
```

Understanding layout explains:

```text
why row-major traversal matters
why resizing vectors amortizes
why StringBuilder exists
```

---

# 21. Limitations and Alternatives

Ask mastery questions:

**Why was the array invented?**

Direct mapping index → memory → fast access.

**What are limitations?**

```text
fixed or resize cost
middle insert/delete expensive
wasted capacity
sparse data inefficient
```

**What alternatives exist?**

```text
linked list — flexible structure
hash table — key-based access
tree — ordered dynamic structure
deque — efficient both ends
```

**How do languages implement dynamic arrays?**

```text
C++ vector — capacity doubling
Java ArrayList — similar
Python list — over-allocated pointer array
```

**Practical trade-offs?**

```text
cache vs pointer chasing
memory vs resize frequency
immutable strings vs builder mutation
```

---

# 22. One Especially Important Mental Shift

When beginners see:

```text
[4, 9, 1, 8, 2]
```

they think only about the input.

As you improve, ask:

> What information from everything I've already processed would make the future cheaper?

Maybe:

```text
seen values          → hash set
frequencies          → freq map or array
value → index        → hash map
prefix sum → count   → hash map
running max/min      → scalar state
window counts        → freq array in window
```

That's the deeper idea behind many array and string problems.

The array is the **stream**.

Your extra structure is the **memory of the stream**.

---

# 23. Phase 2 Learning Order (Suggested)

Align with `map.md` revision order:

```text
1. Motivation     — why arrays/strings
2. Theory         — memory, complexity, encoding
3. Implementation — vector, reverse, prefix build
4. Complexity     — every operation justified
5. Variants       — 2D, circular, immutable strings
6. Applications   — systems context
7. Problems       — pattern representatives
```

Suggested week flow:

```text
Day 1–2: Fundamentals + vector implementation
Day 3:   Linear scan + two pointers (twoptr.md)
Day 4:   Sliding window (slidingwindow.md)
Day 5:   Prefix sum + hash combos (prefixsum-hashmap.md)
Day 6:   Sort+scan + binary search
Day 7:   Matrix + in-place + kadane + DNF + review
```

For each pattern file:

```text
read theory section
implement template from scratch
solve 3–5 problems without looking
explain correctness aloud
```

---

# 24. Pattern Quick Reference Table

| Pattern | Signal | Time | Space | Deep dive |
| --- | --- | --- | --- | --- |
| Linear scan | single pass enough | O(n) | O(1) | this file §10 P1 |
| Two pointers | sorted, pairs, in-place | O(n) | O(1) | twoptr.md |
| Sliding window | contiguous subarray/string | O(n) | O(k) | slidingwindow.md |
| Prefix sum | range sums, static | O(n) build, O(1) query | O(n) | prefixsum-hashmap.md |
| Difference array | many range updates | O(1) update | O(n) | prefixsum-hashmap.md |
| Frequency count | counts, anagrams | O(n) | O(α) | this file §10 P6 |
| Hash + array | complement, seen before | O(n) exp | O(n) | prefixsum-hashmap.md |
| Sort + scan | order helps local check | O(n log n) | varies | sortscan.md |
| Binary search | monotonic predicate | O(log n) | O(1) | binarysearch-inplace.md |
| In-place | O(1) space required | O(n) | O(1) | this file §10 P10 |
| Matrix traversal | grid, 2D indices | O(rc) | O(1) | matrix_2dTra.md |
| Kadane | max subarray sum | O(n) | O(1) | this file §10 P12 |
| Dutch National Flag | 3-way partition | O(n) | O(1) | this file §10 P13 |
| Cyclic rotation | rotate k steps | O(n) | O(1) | this file §10 P14 |
| Intervals | merge/overlap | O(n log n) | O(n) | sortscan.md |
| String patterns | palindrome, anagram | varies | varies | this file §10 P16 |

---

# 25. Binary Search on Answer (Preview)

Sometimes search space isn't the array — it's a **value**.

```text
"minimum capacity to ship in D days"
"minimum eating speed"
"split array largest sum"
```

Structure:

```text
guess mid value
check if feasible(mid) — often O(n) scan
if feasible → try smaller (minimize)
else → try larger
```

Monotonic:

```text
feasible(mid) = true for all mid >= answer
```

This connects arrays (verification scan) with binary search.

Full treatment: `array/binarysearch-inplace.md`

---

# 26. Boyer-Moore Majority Vote (Bonus Pattern)

Find element appearing > n/2 times in O(n) time, O(1) space.

```cpp
int cand = 0, count = 0;
for (int x : a) {
  if (count == 0) cand = x, count = 1;
  else if (x == cand) count++;
  else count--;
}
// verify cand if majority guaranteed
```

Recognition:

```text
majority element
cancelling pairs
```

Not a hash map — elegant when majority exists.

---

# 27. Monotonic Stack/Deque (Bridge to Stacks Phase)

Some array problems use stack — preview before Phase 4.

Examples:

```text
Next Greater Element
Daily Temperatures
Sliding Window Maximum (deque)
```

Sliding window maximum:

```text
deque stores indices
front = max in current window
```

When you see:

```text
"next greater/smaller"
"window max/min"
```

Consider stack/deque — detailed in stacks phase.

---

# 28. Drawing Practice — Reverse In-Place

Input:

```text
[1][2][3][4][5]
 L               R
```

Swap and move:

```text
step: swap L,R, L++, R--

[5][2][3][4][1]
     L       R
...
result [5][4][3][2][1]
```

Invariants:

```text
elements outside [L,R] already in final position
```

Practice drawing 3 steps for every pointer algorithm until automatic.

---

# 29. Drawing Practice — Variable Window

String: `abcabcbb` — longest without repeat.

```text
expand R, track freq
when duplicate, shrink L until valid
```

Trace:

```text
R at 'c' second time → shrink L past first 'c'
```

Window invariant:

```text
all chars in [L,R] unique
```

Answer = max(R-L+1) seen.

---

# 30. Subarray vs Subsequence vs Subset

Clarify every problem:

| Term | Contiguous? | Order? |
| --- | --- | --- |
| Subarray | Yes | Yes |
| Substring | Yes | Yes |
| Subsequence | No | Yes |
| Subset | No | No (usually) |

Wrong pattern if you confuse them:

```text
subsequence → sliding window usually wrong
subarray    → two pointers on unsorted may miss
```

---

# 31. Prefix Sum — Worked Micro-Example

```text
a = [1, 2, 3, 4]

prefix = [0, 1, 3, 6, 10]

sum(1..2) = prefix[3] - prefix[1] = 6 - 1 = 5 = 2+3
```

Subarray Sum Equals K:

```text
need prefix[R] - prefix[L] = k
→ prefix[L] = prefix[R] - k

count how many earlier prefixes equal prefix[R]-k
```

Hash map stores `prefix value → count`.

---

# 32. Kadane — Worked Micro-Example

```text
a = [-2, 1, -3, 4, -1, 2, 1, -5, 4]

i=0: cur=-2, best=-2
i=1: cur=1, best=1
i=2: cur=-2, best=1
i=3: cur=4, best=4
i=4: cur=3, best=4
i=5: cur=5, best=5
i=6: cur=6, best=6
i=7: cur=1, best=6
i=8: cur=5, best=6
```

Answer: 6.

---

# 33. Dutch National Flag — Worked Micro-Example

```text
[2,0,2,1,1,0]

low=0, mid=0, high=5

process mid:
2 → swap with high, high--
0 → swap with low, low++, mid++
1 → mid++
...
[0,0,1,1,2,2]
```

---

# 34. Matrix Index — Worked Example

```text
3×4 matrix, row-major

element (2,1) in:
1  2  3  4
5  6  7  8
9  0  1  2

index = 2*4 + 1 = 9 → value 0
```

Rotate 90° clockwise:

```text
transpose then reverse each row
```

---

# 35. Interview Communication Framework

When solving aloud:

```text
1. Restate problem + clarify constraints
2. Examples including edge cases
3. Brute force complexity
4. Identify pattern clue
5. Proposed optimized approach + why correct
6. Complexity
7. Code
8. Test with edge case
```

Mention trade-offs if multiple solutions exist.

---

# 36. Common Mistakes

```text
off-by-one in binary search (lo < hi vs lo <= hi)
forgetting k %= n in rotation
using hash when int[26] suffices
O(n²) string concat in loop
not handling empty input
sliding window: forget to shrink when invalid
two sum: pairing element with itself
prefix sum: wrong index when L=0
matrix: confuse rows and cols
interval merge: forget to sort by start first
```

Review mistakes after each problem — build personal bug list.

---

# 37. Mastery Questions (After Phase 2)

Answer without looking:

```text
Why is a[i] O(1)?
What is amortized O(1) append?
Row-major vs column-major?
When array beats hash for frequency?
Difference array vs prefix sum?
When two pointers on sorted array?
Fixed vs variable window?
Kadane recurrence?
Dutch National Flag invariants?
Binary search on answer vs on index?
```

If shaky → revisit specific pattern file.

---

# 38. Connections to Later Phases

```text
Phase 3 Linked Lists    — contrast with contiguous arrays
Phase 4 Stacks/Queues   — monotonic stack, deque window
Phase 5 Hash Tables     — deep hash internals
Phase 7 Sort/Search     — sort algorithms, BS mastery
Phase 8 Graphs          — grid BFS/DFS
Phase 9 DP              — subsequence, edit distance
Phase 13 String Algos   — KMP, suffix structures
```

Arrays & strings are the **hub** — most later topics assume you're comfortable here.

---

# Your Complete Arrays & Strings Checklist

```text
ARRAYS & STRINGS
│
├── Fundamentals
│   ├── Contiguous memory
│   ├── Index → address
│   ├── Static vs dynamic arrays
│   ├── Size vs capacity
│   ├── Resizing / amortized append
│   ├── Cache locality
│   ├── 1D vs 2D layout
│   ├── Row-major vs column-major
│   └── Array vs linked structure
│
├── Implement
│   ├── Dynamic array (vector)
│   └── array/arrayList.cpp reference
│
├── Strings
│   ├── Sequence of characters
│   ├── Mutable vs immutable
│   ├── ASCII / Unicode / UTF-8 basics
│   ├── Concatenation cost
│   ├── StringBuilder / buffers
│   ├── Substring cost
│   ├── Lexicographic compare
│   └── Character frequency tables
│
├── Core Complexity
│   ├── Access, search, insert, delete
│   ├── Copy, reverse, sort
│   └── Explain why for each
│
├── Patterns (16+)
│   ├── Linear Scan
│   ├── Two Pointers (opposite, same, merge)
│   ├── Sliding Window (fixed, variable)
│   ├── Prefix Sum
│   ├── Difference Array
│   ├── Frequency Counting
│   ├── Hash Map + Array
│   ├── Sort + Scan
│   ├── Binary Search (+ on answer)
│   ├── In-Place Manipulation
│   ├── Matrix / 2D Traversal
│   ├── Kadane's Algorithm
│   ├── Dutch National Flag
│   ├── Cyclic Rotation
│   ├── Interval Merge / Sweep
│   └── String-Specific (palindrome, anagram)
│
├── Deep Dive Files
│   ├── twoptr.md
│   ├── slidingwindow.md
│   ├── prefixsum-hashmap.md
│   ├── matrix_2dTra.md
│   ├── sortscan.md
│   ├── binarysearch-inplace.md
│   └── note.md (resources)
│
├── Pattern Composition
│   ├── 3Sum, Subarray Sum K, Trapping Rain Water, etc.
│   └── Identify multi-pattern problems
│
├── Pattern Recognition
│   ├── Clue → pattern map
│   └── Clues not rigid rules
│
├── Edge Cases
│   ├── empty, single, two, duplicates
│   ├── negatives, zeros, overflow
│   ├── off-by-one, rotation k
│   └── matrix dimensions
│
├── Analysis
│   ├── Compare brute / hash / sort / pointers
│   ├── Time and space every solution
│   └── Trade-offs table
│
├── Problems
│   ├── Per-pattern representatives
│   └── Composition problems
│
└── For EVERY Problem
    ├── Input structure?
    ├── Output type?
    ├── Pattern fit?
    ├── Invariant?
    ├── Draw small example
    ├── Edge cases
    ├── Time / space
    ├── Correctness why
    └── Check detailed pattern file
```

---

# 39. Final Note — From Collection to Thinking

Phase 2 is not about memorizing 50 templates.

It's about understanding:

```text
memory layout → why O(1) access
contiguity    → sliding window validity
ordering      → two pointers / binary search
past information → hash / prefix
locality after sort → scan
```

When that foundation is solid, linked lists, hash tables, trees, and graphs become comparisons against what you already know — not unrelated new topics.

Read this file for the map. Drill each pattern file until you can implement from scratch and explain why it works.

That turns DSA from a bag of tricks into a way of thinking.
