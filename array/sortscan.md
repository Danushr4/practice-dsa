# Sorting + Scan

## 1. Why sorting changes a problem

Suppose you have:

```text
[7, 2, 9, 3, 2, 8, 7]
```

In this form, related values can be anywhere.

To find duplicates naively, you might compare many pairs.

But sort it:

```text
[2, 2, 3, 7, 7, 8, 9]
```

Suddenly duplicates become adjacent:

```text
[2, 2]  3  [7, 7]  8  9
```

Now one linear scan is enough.

So sorting performs a transformation:

```text
Unstructured relationships
          ↓
         SORT
          ↓
Related elements become nearby
          ↓
Simple local reasoning
```

That is the deeper pattern.

---



# 2. The usual complexity

Most comparison sorting costs:

```text
O(n log n)
```

Then scanning costs:

```text
O(n)
```

Total:

```text
O(n log n) + O(n)
=
O(n log n)
```

So once you've paid for sorting, an additional linear scan is usually cheap asymptotically.

---



# 3. Don't think "sorting is just arranging numbers"

Sorting gives you several useful guarantees:

```text
a[0] <= a[1] <= a[2] <= ...
```

Because of that:

- Equal values become adjacent.
- Smaller values are all on one side.
- Larger values are all on the other.
- Intervals can be processed in order.
- Two pointers become possible.
- Groups often become contiguous.
- Overlaps become locally detectable.
- Greedy choices sometimes become obvious.
- Binary search becomes possible.

So:

> **Sorting creates structure.**

---



# 4. Pattern 1 — Sort + Detect Duplicates

Unsorted:

```text
[4, 8, 2, 7, 4, 3]
```

Without extra memory, detecting the duplicate `4` isn't immediately obvious.

Sort:

```text
[2, 3, 4, 4, 7, 8]
```

Then check neighbors:

```cpp
for (int i = 1; i < n; i++) {
    if (a[i] == a[i - 1]) {
        // duplicate
    }
}
```

Complexity:

```text
Sorting: O(n log n)
Scan:    O(n)

Total:   O(n log n)
```

Compare with hashing:

```text
Hash Set:
Expected O(n)
Space O(n)
```

Sorting may be preferable if:

- Extra memory is constrained.
- You already need sorted order.
- Modifying the input is allowed.
- You want deterministic ordering afterward.

This is why you should compare approaches instead of blindly choosing HashSet.

---



# 5. Pattern 2 — Sort + Remove Duplicates

Input:

```text
[5, 2, 5, 1, 2, 8]
```

Sort:

```text
[1, 2, 2, 5, 5, 8]
```

Now unique values are easy to identify:

```text
1 | 2 2 | 5 5 | 8
```

You can combine:

```text
Sorting
+
Read/Write Two Pointers
```

to compact them:

```text
[1, 2, 5, 8, ...]
```

Notice the composition:

> **Sort + Two Pointers**

Patterns frequently combine rather than existing independently.

---



# 6. Pattern 3 — Pair Problems

Suppose:

> Find whether any pair sums to 10.

Input:

```text
[8, 1, 4, 6, 3]
```

Sort:

```text
[1, 3, 4, 6, 8]
```

Now:

```text
L           R
↓           ↓
1  3  4  6  8
```

Use Two Pointers.

So:

```text
Unsorted Pair Problem
        ↓
Sort
        ↓
Ordered Pair Problem
        ↓
Two Pointers
```

Complexity:

```text
Sort         O(n log n)
Two pointers O(n)

Total        O(n log n)
```

Compare with Hash Map:

```text
Expected O(n)
Space O(n)
```

Again, there's a trade-off.

---



# 7. Sorting can destroy original information

This is an important warning.

Suppose:

```text
index:  0  1  2  3
value: [8, 1, 5, 3]
```

After sorting:

```text
[1, 3, 5, 8]
```

You no longer directly know that:

```text
8 originally came from index 0
```

If original indices matter, sort pairs:

```text
(value, originalIndex)
```

For example:

```text
[(8,0), (1,1), (5,2), (3,3)]
```

sort by value:

```text
[(1,1), (3,3), (5,2), (8,0)]
```

This preserves both:

```text
order by value
+
original position
```

Very useful technique.

---



# 8. Pattern 4 — Merge Intervals

One of the most important Sorting + Scan problems.

Suppose:

```text
[8,10]
[1,3]
[2,6]
[15,18]
```

The intervals are unordered.

It's hard to reason locally.

Sort by start:

```text
[1,3]
[2,6]
[8,10]
[15,18]
```

Now scan left → right.

Compare:

```text
current = [1,3]
next    = [2,6]
```

Since:

```text
2 <= 3
```

they overlap.

Merge:

```text
[1,6]
```

Next:

```text
current = [1,6]
next    = [8,10]
```

Since:

```text
8 > 6
```

no overlap.

Final:

```text
[1,6]
[8,10]
[15,18]
```

---



# 9. Why sorting makes interval merging possible

This deserves careful thought.

After sorting by start:

```text
start1 <= start2 <= start3 ...
```

Suppose you're maintaining:

```text
current merged interval = [L,R]
```

The next interval begins at `nextStart`.

Only two major cases exist:

```text
nextStart <= R
        ↓
overlap
```

or:

```text
nextStart > R
        ↓
no overlap
```

Because starts are ordered, you don't need to compare the current interval against every remaining interval.

Sorting converted a global relationship into a local one.

That's the deeper idea.

---



# 10. Pattern 5 — Overlap Detection

Suppose meeting intervals:

```text
[5,7]
[1,3]
[2,4]
[8,10]
```

Question:

> Are any meetings overlapping?

Sort:

```text
[1,3]
[2,4]
[5,7]
[8,10]
```

Now compare neighboring intervals.

```text
[1,3]
[2,4]
```

Since:

```text
2 < 3
```

overlap exists.

You don't need to compare every pair.

Naive:

```text
O(n²)
```

Sorting + scan:

```text
O(n log n)
```

---



# 11. Pattern 6 — Grouping Related Values

Suppose:

```text
["cat", "dog", "cat", "bird", "dog", "cat"]
```

Sort:

```text
["bird", "cat", "cat", "cat", "dog", "dog"]
```

Now groups are contiguous.

You can scan:

```text
bird | cat cat cat | dog dog
```

and calculate:

- Frequencies
- Group lengths
- Number of distinct values
- Largest group

Hashing is often faster asymptotically for pure counting, but sorting gives ordered groups.

---



# 12. Pattern 7 — Find Closest Elements

Suppose:

> Find the pair with the smallest absolute difference.

Unsorted:

```text
[20, 3, 15, 8, 10]
```

Naively compare all pairs:

```text
O(n²)
```

Sort:

```text
[3, 8, 10, 15, 20]
```

Now here's the key insight:

> The closest pair must be adjacent in sorted order.

Why?

Suppose:

```text
a < b < c
```

Then:

```text
c - a
```

cannot be smaller than both:

```text
b - a
```

and:

```text
c - b
```

So simply scan adjacent differences:

```text
8 - 3  = 5
10 - 8 = 2  ← minimum
15 - 10 = 5
20 - 15 = 5
```

Answer:

```text
(8,10)
```

This is a beautiful example of sorting eliminating an enormous search space.

---



# 13. Pattern 8 — Three Sum

You've already seen Two Pointers.

Now compose it with sorting.

Problem:

```text
Find triples where:

a + b + c = 0
```

Naive:

```text
three nested loops
=
O(n³)
```

Instead:

```text
Sort
```

Then fix one number:

```text
a[i]
```

and solve:

```text
b + c = -a[i]
```

with Two Pointers.

Conceptually:

```text
Sort
 ↓
Fix one value
 ↓
Two Sum on remaining sorted range
```

Complexity:

```text
Sort: O(n log n)

For each i:
    Two pointers O(n)

Total: O(n²)
```

So you turn:

```text
O(n³)
```

into:

```text
O(n²)
```

This is an important example of pattern composition.

---



# 14. Pattern 9 — Sort by Something Other Than Value

Sorting doesn't always mean:

```text
sort numbers ascending
```

You may sort objects by:

```text
start time
end time
length
priority
x-coordinate
y-coordinate
frequency
custom key
```

Example intervals:

```cpp
sort(intervals.begin(), intervals.end(),
     [](auto& a, auto& b) {
         return a.start < b.start;
     });
```

So one of your most important questions should be:

> **What ordering would make the problem easier?**

Not merely:

> Should I sort?

---



# 15. Custom Ordering

Suppose tasks:

```text
Task A: deadline 8
Task B: deadline 3
Task C: deadline 5
```

Maybe sorting by deadline exposes a greedy solution:

```text
B → C → A
```

Or intervals might be sorted by:

```text
end time
```

instead of start time.

Different ordering can expose different structures.

This becomes extremely important in Greedy Algorithms later.

---



# 16. Sort + Greedy Scan

A very important future pattern:

```text
Sort
 ↓
Scan left → right
 ↓
Make locally optimal choices
```

Example:

> Select maximum number of non-overlapping meetings.

If you sort meetings by **finish time**, choosing the earliest-finishing available meeting leads to the classic greedy solution.

Don't study greedy deeply yet, but recognize that sorting is often the preprocessing step that makes greedy reasoning possible.

---



# 17. Sort + Binary Search

Another combination:

```text
Sort
 ↓
For every element
 ↓
Binary search for another value
```

Example conceptual pair search:

```text
For each x:
    search target-x
```

Complexity:

```text
Sort           O(n log n)
n searches     O(n log n)

Total          O(n log n)
```

Two pointers might be better for some pair problems, but binary search can be useful when pointer movement doesn't fit.

---



# 18. Sort + Sweep

This becomes an important advanced pattern.

Imagine events on a line:

```text
start
end
start
start
end
...
```

Sort all events by position/time:

```text
event1
event2
event3
...
```

Then scan them while maintaining state.

This is often called:

> Sweep Line

Examples later include:

- Number of overlapping intervals
- Maximum simultaneous meetings
- Geometry problems
- Calendar scheduling
- Rectangle/event processing

For your current Arrays pass, just understand the basic concept.

---



# 19. Example — Maximum Concurrent Intervals

Suppose:

```text
[1,4]
[2,5]
[7,9]
[3,6]
```

Convert to events:

```text
1 start
4 end

2 start
5 end

7 start
9 end

3 start
6 end
```

Sort events:

```text
1 start
2 start
3 start
4 end
5 end
6 end
7 start
9 end
```

Scan while maintaining:

```text
activeCount
```

At time 3:

```text
activeCount = 3
```

That's a simple sweep-line idea built from:

```text
Sorting
+
Scanning
+
Running state
```

You'll see a lot of this in geometry/CAD algorithms later.

---



# 20. Pattern 10 — Sort + Prefix/Suffix

Patterns can combine further.

Suppose sorting gives:

```text
a[0] <= a[1] <= ... <= a[n-1]
```

Then build:

```text
prefix sums
```

or:

```text
suffix information
```

to answer aggregate questions about smaller/larger portions efficiently.

For example:

```text
sort
+
prefix sums
+
binary search
```

is a surprisingly common advanced combination.

The important lesson is:

> Individual patterns become building blocks for larger algorithms.

---



# 21. Stable vs Unstable Sorting

When using Sorting + Scan, understand whether **stability** matters.

A stable sort preserves original order among elements with equal keys.

Suppose:

```text
(A, score=10)
(B, score=5)
(C, score=10)
```

Stable sort by score:

```text
B
A
C
```

A remains before C because they had equal scores and A originally appeared first.

This can matter when you perform multiple sorts or need secondary ordering.

---



# 22. Sorting mutates data

Always ask:

> Am I allowed to change the input order?

Because:

```cpp
sort(a.begin(), a.end());
```

changes the array.

If original order matters, you may need:

```text
copy
```

or:

```text
sort (value,index) pairs
```

That changes space complexity.

---



# 23. Sorting vs Hashing

This comparison is worth mastering.

Suppose:

> Detect duplicate.



### Hash Set

```text
Expected time: O(n)
Space:         O(n)
Original order unchanged
```



### Sort + Scan

```text
Time:          O(n log n)
Extra space:   depends on sorting implementation
Input order:   changed unless copied
```

So why ever sort?

Because sorting can provide additional structure:

- Ordered output
- Nearby equal values
- Range processing
- Two-pointer capability
- Deterministic order

Algorithm choice isn't only about asymptotic time.

---



# 24. Sorting vs brute force

A common transformation is:

```text
Brute force:
compare arbitrary pairs
        ↓
        O(n²)
```

Ask:

> If I sort, will only nearby elements or monotonic pointer movements matter?

If yes:

```text
Sort          O(n log n)
Scan          O(n)
-----------------------
Total         O(n log n)
```

This is often a major improvement.

---



# 25. When sorting is NOT worth it

Don't sort automatically.

Suppose:

> Find maximum value.

You only need:

```text
O(n)
```

scan.

Sorting would cost:

```text
O(n log n)
```

unnecessarily.

Similarly:

> Does x exist?

A single unsorted scan:

```text
O(n)
```

may be cheaper than:

```text
sort + binary search
=
O(n log n)
```

if you only have one query.

So always ask:

> **What benefit am I actually buying with the sorting cost?**

---



# 26. When sorting becomes worth it

Sorting is especially attractive when it enables one or more of these:

```text
Many future searches
Adjacent relationships
Two pointers
Interval processing
Grouping
Duplicate compression
Greedy ordering
Binary search
Sweep-line processing
```

That's a better recognition rule than:

> "I see an array → maybe sort it."

---



# 27. The invariant after sorting

Just like Two Pointers and Sliding Window, think about invariants.

After sorting:

```text
everything before i <= a[i]
everything after i >= a[i]
```

For Merge Intervals:

> Everything before the current position has already been correctly merged.

For duplicate detection:

> If duplicates exist in the processed region, adjacent comparison would have detected them.

For two pointers:

> Values outside `[L,R]` can no longer form the required answer.

These invariants explain why the scan works.

---



# 28. Important Sorting + Scan families

You should recognize these.

### Adjacent comparison

```text
Sort
 ↓
compare a[i] with a[i-1]
```

Used for:

- Duplicates
- Closest pair
- Gaps
- Groups



### Ordered merging

```text
Sort by start/key
 ↓
maintain current merged result
```

Used for:

- Intervals
- Segments
- Ranges



### Sort + Two Pointers

```text
Sort
 ↓
L →       ← R
```

Used for:

- Pair sum
- 3Sum
- Pair differences
- Closest sums



### Sort + Running State

```text
Sort events
 ↓
scan
 ↓
update count/state
```

Used for:

- Overlap counting
- Sweep line
- Scheduling



### Sort + Greedy

```text
Sort by useful criterion
 ↓
choose as you scan
```

Used later extensively.

---



# 29. Recognition clues

Train yourself to notice:

```text
"overlapping intervals"
        ↓
Sort by boundary + Scan
```

```text
"duplicates"
        ↓
Hashing OR Sort + Scan
```

```text
"closest pair / minimum difference"
        ↓
Sort + Adjacent Scan
```

```text
"pair/triple sum"
        ↓
Potential Sort + Two Pointers
```

```text
"group equal/similar values"
        ↓
Sort + Scan OR Hashing
```

```text
"maximum simultaneous..."
        ↓
Sort Events + Sweep
```

```text
"non-overlapping intervals"
        ↓
Sorting often enables Greedy
```

---



# 30. Core practice problems

You don't need dozens.

For this pattern, I'd use:

- [ ] **1. Contains Duplicate — sorting solution**

Compare with hashing.

- [ ] **2. Merge Sorted Array**

Basic ordered traversal.

- [ ] **3. Merge Intervals**

Core Sort + Scan problem.

- [ ] **4. Meeting Rooms**

Overlap detection.

- [ ] **5. Minimum Difference Between Highest and Lowest of K Scores**

Sorting exposes nearby candidates.

- [ ] **6. Two Sum II**

Sorted Two Pointers.

- [ ] **7. 3Sum**

Sort + fixed element + Two Pointers.

- [ ] **8. Group/Remove duplicates from sorted input**

Adjacent grouping.

Later:

- [ ] **9. Non-overlapping Intervals**

Sort + Greedy.

- [ ] **10. Meeting Rooms II / overlapping events**

Sort + sweep concept.

---


# 31. For EVERY Sorting + Scan problem

Ask these questions:

```text
1. What is difficult because the input is unordered?

2. What relationship would sorting expose?

3. What should I sort by?
   Value?
   Start?
   End?
   Coordinate?
   Frequency?
   Custom key?

4. After sorting, what becomes local?
   Adjacent duplicates?
   Overlap?
   Closest values?
   Pair relationship?

5. What will my scan maintain?
   Previous value?
   Current interval?
   Left/right pointers?
   Running count?
   Current group?

6. What is the invariant?

7. Can I process each element only once after sorting?

8. Is O(n log n) acceptable?

9. Could hashing give O(n)?

10. Does sorting destroy information I need?

11. Should I preserve original indices?

12. Is sorting actually necessary?
```

Questions **2 and 3** are the heart of this pattern.

---



# 32. The deepest idea

The most important lesson isn't:

> "Sort first."

It's:

> **Choose an ordering that converts difficult global relationships into simple local relationships.**

Before sorting:

```text
Everything could relate to everything.
```

After the right sorting:

```text
I may only need to inspect:
neighbor
next interval
left/right boundary
current group
next event
```

That's why paying:

```text
O(n log n)
```

can be extremely worthwhile.

# Your Sorting + Scan checklist

```text
SORTING + SCAN
│
├── Core Idea
│   ├── Create order
│   ├── Expose relationships
│   └── Replace global comparisons with local reasoning
│
├── Basic Patterns
│   ├── Adjacent comparison
│   ├── Duplicate detection
│   ├── Grouping
│   └── Closest values
│
├── Intervals
│   ├── Sort by start/end
│   ├── Merge overlaps
│   ├── Detect overlaps
│   └── Maintain current interval
│
├── Compositions
│   ├── Sort + Two Pointers
│   ├── Sort + Binary Search
│   ├── Sort + Prefix Sum
│   ├── Sort + Greedy
│   └── Sort + Sweep Line
│
├── Practical Concerns
│   ├── O(n log n) sorting cost
│   ├── Stable vs unstable
│   ├── Input mutation
│   ├── Preserve original indices
│   └── Extra space
│
└── For Every Problem
    ├── Ask what order would help
    ├── Choose sorting key
    ├── Identify local relationship
    ├── Define scan state
    ├── State invariant
    ├── Compare with hashing/brute force
    └── Explain why sorting is worth its cost
```

So the sentence to remember for **Sorting + Scan** is:

> **Sorting is worth O(n log n) when the order it creates lets you replace expensive global searching/comparison with cheap local or monotonic reasoning.**

That is the actual pattern—not merely calling `sort()` before solving the problem.