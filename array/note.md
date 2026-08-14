### 1. Array fundamentals — understand the structure

#### About arrays

* [Arrays — CS50 Harvard University](https://cs50.harvard.edu/)
* [Arrays (video)](https://www.youtube.com/)
* [UC Berkeley CS61B — Linear and Multi-Dim Arrays (video)](https://www.youtube.com/) (start from 15m 32s)
* [Dynamic Arrays (video)](https://www.youtube.com/)
* [Jagged Arrays (video)](https://www.youtube.com/)

#### Implement a vector (mutable array with automatic resizing)

Practice coding using arrays and pointers, and pointer math to jump to an index instead of using indexing.

* New raw data array with allocated memory
* Can allocate `int` array under the hood, just not use its features
* Start with 16, or if the starting number is greater, use power of 2 — 16, 32, 64, 128

**API**

| Method | Description |
| --- | --- |
| `size()` | Number of items |
| `capacity()` | Number of items it can hold |
| `is_empty()` | Whether the vector is empty |
| `at(index)` | Returns the item at a given index; blows up if index out of bounds |
| `push(item)` | Append to end |
| `insert(index, item)` | Inserts item at index, shifts that index's value and trailing elements to the right |
| `prepend(item)` | Can use `insert` above at index 0 |
| `pop()` | Remove from end, return value |
| `delete(index)` | Delete item at index, shifting all trailing elements left |
| `remove(item)` | Looks for value and removes index holding it (even if in multiple places) |
| `find(item)` | Looks for value and returns first index with that value; -1 if not found |
| `resize(new_capacity)` | Private function |

**Resizing rules**

* When you reach capacity, resize to double the size
* When popping an item, if the size is 1/4 of capacity, resize to half

**Time**

* O(1) to add/remove at end (amortized for allocations for more space), index, or update
* O(n) to insert/remove elsewhere

**Space**

* Contiguous in memory, so proximity helps performance
* Space needed = (array capacity, which is >= n) × size of item; even if 2n, still O(n)

Before patterns, understand what an array actually is:

* Contiguous memory
* Index → memory address calculation
* Fixed-size arrays
* Dynamic arrays (`vector`, `ArrayList`)
* Size vs capacity
* Resizing/reallocation
* Why random access is **O(1)**
* Why middle insertion/deletion is **O(n)**
* Amortized **O(1)** append
* Cache locality
* 1D vs 2D/multidimensional arrays
* Row-major vs column-major layout
* Arrays vs linked structures

You don't need to go extremely deep into CPU architecture yet—just understand **why arrays behave the way they do**.

### 2. String fundamentals

Strings deserve their own foundation:

* String as a sequence of characters
* Mutable vs immutable strings
* Character encoding basics
  * ASCII
  * Unicode
  * UTF-8 / UTF-16 at a conceptual level
* String concatenation cost
* `StringBuilder` / mutable buffers
* Substrings and their potential costs
* Lexicographical comparison
* Character/frequency tables

Advanced string algorithms like KMP, Z-algorithm, tries and suffix arrays can wait for the dedicated **String Algorithms** topic later.

### 3. Array & String operations + complexity

You should be able to reason about:

| Operation               |               Typical complexity |
| ----------------------- | -------------------------------: |
| Access `a[i]`           |                             O(1) |
| Update                  |                             O(1) |
| Search unsorted         |                             O(n) |
| Insert at end           | O(1) amortized for dynamic array |
| Insert beginning/middle |                             O(n) |
| Delete beginning/middle |                             O(n) |
| Copy                    |                             O(n) |
| Reverse                 |                             O(n) |
| Sort                    |              commonly O(n log n) |

More importantly, understand **why**, rather than memorizing the table.

---

# 4. Problem-solving patterns

Your original list is correct, but I'd expand it slightly.

### A. Traversal / Linear Scan

Learn:

* Single traversal
* Reverse traversal
* Simultaneous tracking
* Min/max
* Counting
* Running state

The fundamental idea is:

> "Can I obtain the answer while looking at each element once?"

---

### B. Two Pointers

Learn the major variants:

**Opposite direction**

```text
L → [............] ← R
```

Examples:

* Reverse array/string
* Palindrome
* Two Sum on sorted input

**Same direction**

```text
slow →
fast ----->
```

Examples:

* Remove duplicates
* Move zeroes
* In-place filtering

Also understand why two pointers can sometimes replace an O(n²) nested-loop solution with O(n).

---

### C. Sliding Window

Learn both:

**Fixed window**

```text
[  window  ] →
```

Examples:

* Maximum sum of `k` consecutive elements
* Average of every `k` elements

**Variable window**

```text
L → [ window ] → R
```

Examples:

* Longest substring without repeating characters
* Minimum-length subarray satisfying a condition

The important part isn't memorizing the template. Understand:

> **When is it valid to expand/shrink the window without reconsidering everything we've already processed?**

---

### D. Prefix Sum

Learn:

* 1D prefix sum
* Range sum queries
* Prefix frequency/counts
* Suffix sums
* 2D prefix sum basics

Then add an important related technique:

### Difference Arrays

Prefix sums answer:

> "What is the cumulative value up to here?"

Difference arrays are useful for:

> "Apply updates across ranges efficiently."

These two concepts complement each other.

---

### E. Hashing / Frequency Counting

Learn:

* Hash set
* Hash map
* Frequency array
* Character frequency tables
* Value → index mapping
* Duplicate detection
* Grouping
* Complement lookup

For example, the important idea behind Two Sum isn't "memorize Two Sum."

It's:

```text
Current value = x
Need = target - x

Have I seen `need` before?
```

That's the reusable pattern.

---

### F. Sorting + Scan

Learn how sorting can transform a problem.

```text
Unstructured data
      ↓
     Sort
      ↓
Ordered structure
      ↓
Easy traversal
```

Examples:

* Merge intervals
* Remove/find duplicates
* Pair problems
* Grouping
* Overlap detection

And always ask:

> Is spending O(n log n) on sorting worth making the rest of the problem easier?

---

### G. Binary Search

Go beyond:

> Find `x` in sorted array.

Learn:

* Standard binary search
* First occurrence
* Last occurrence
* Lower bound
* Upper bound
* Rotated sorted arrays
* Binary search on answer

The deeper concept is:

> **Binary search works on a monotonic search space**, not merely "sorted arrays."

---

### H. In-place Array Manipulation

I'd explicitly add this to your original list.

Learn:

* Swap-based manipulation
* Reversal
* Partitioning
* Compaction
* Rotation
* Overwriting using read/write pointers

Examples:

```text
Move Zeroes
Remove Duplicates
Rotate Array
Partition Array
```

This teaches how to reduce O(n) extra space to **O(1)**.

---

### I. Matrix / 2D Array Traversal

Also missing from the original list.

Learn:

* Row traversal
* Column traversal
* Diagonals
* Boundary traversal
* Spiral traversal
* Matrix transpose
* Matrix rotation
* Neighbor/direction traversal

You don't need graph-style grid BFS/DFS yet. Save that for graphs.

---

# 5. Pattern recognition

This is probably the **most important part of your week**.

When you see a problem, train yourself to notice clues:

```text
Contiguous subarray/substring
        ↓
Sliding Window / Prefix Sum

Sorted array
        ↓
Two Pointers / Binary Search

Repeated lookup
        ↓
Hash Map / Set

Range sum queries
        ↓
Prefix Sum

Range updates
        ↓
Difference Array

Need O(1) extra memory
        ↓
In-place / Two Pointers

Pairs in sorted data
        ↓
Two Pointers

Monotonic yes/no condition
        ↓
Binary Search

2D grid/matrix
        ↓
Matrix traversal
```

But treat these as **clues**, not rigid rules.

---

# 6. Complexity analysis for every problem

For every solution you write, ask:

**Time**

```text
O(1)?
O(log n)?
O(n)?
O(n log n)?
O(n²)?
```

**Space**

```text
O(1)?
O(n)?
```

Then ask:

> Why?

That's much more valuable than simply writing `"Time: O(n)"` below the solution.

---

# 7. Trade-offs

For every approach compare alternatives.

For example:

```text
Two Sum
```

Brute force:

```text
Time  O(n²)
Space O(1)
```

Sorting + two pointers:

```text
Time  O(n log n)
Space depends on sorting
```

Hash map:

```text
Expected Time  O(n)
Space          O(n)
```

Then understand **why you'd choose one over another**.

That's the kind of DSA thinking you're trying to develop.

---

# Your complete Arrays & Strings week

So I'd update your checklist to:

```text
ARRAYS & STRINGS
│
├── Fundamentals
│   ├── Memory layout
│   ├── Static vs dynamic arrays
│   ├── Size/capacity/resizing
│   ├── Cache locality
│   ├── Complexity
│   └── 2D memory layout
│
├── Strings
│   ├── Representation
│   ├── Mutability
│   ├── Encoding basics
│   ├── Concatenation
│   └── StringBuilder/buffers
│
├── Patterns
│   ├── Traversal
│   ├── Two Pointers
│   ├── Sliding Window
│   ├── Prefix/Suffix Sum
│   ├── Difference Array
│   ├── Hashing/Frequency Counting
│   ├── Sorting + Scan
│   ├── Binary Search
│   ├── In-place Manipulation
│   └── Matrix/2D Traversal
│
└── For EVERY pattern
    ├── Why does it exist?
    ├── When should I recognize it?
    ├── How does it work?
    ├── Implement it
    ├── Prove/understand correctness
    ├── Time complexity
    ├── Space complexity
    ├── Trade-offs
    └── Solve representative problems
```
