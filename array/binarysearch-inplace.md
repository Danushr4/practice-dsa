# Binary Search

The beginner version is:

> Find `x` in a sorted array.

The deeper version is:

> **If the search space has a monotonic property, repeatedly discard half of it.**

That is the real idea.

## 1. Standard Binary Search

Suppose:

```text
[1, 3, 5, 7, 9, 11, 13]
```

Target:

```text
9
```

Instead of scanning left to right, choose the middle:

```text
L           M           R
↓           ↓           ↓
1  3  5  7  9  11  13
```

If:

```text
a[M] < target
```

then everything left of `M` can be discarded.

If:

```text
a[M] > target
```

then everything right of `M` can be discarded.

Each step removes roughly half the candidate region.

That gives:

```text
O(log n)
```

genui{"computing_algorithms_search_traversal_learning_block_staging":{"type_id":"BINARY_SEARCH"}}

---



## 2. The invariant

This is the part to understand deeply.

At every step:

> **If the target exists, it must still lie inside the current search interval.**

For an inclusive interval:

```text
[L, R]
```

you maintain that invariant while shrinking it.

That's more important than memorizing:

```cpp
while (left <= right)
```

---



## 3. Why binary search needs order

Suppose:

```text
[7, 1, 9, 3, 5]
```

Middle = `9`.

Target = `5`.

Can you safely decide whether to search left or right?

No.

Because values aren't ordered.

So binary search requires something like:

```text
false false false true true true
```

or:

```text
small small small large large large
```

There must be a boundary you can reason about.

---



# 4. First Occurrence

Suppose:

```text
[1, 2, 2, 2, 4, 7]
```

Target:

```text
2
```

Normal binary search may return any of the `2`s.

But maybe you need:

```text
first 2
```

The trick is:

> Finding a match does not necessarily mean we're done.

When:

```text
a[mid] == target
```

record it as a candidate, then keep searching left.

Conceptually:

```text
Found target
     ↓
Could there be an earlier one?
     ↓
search left half
```

---



# 5. Last Occurrence

Same idea, but after finding the target:

```text
search right
```

So:

```text
First occurrence:
match → continue left

Last occurrence:
match → continue right
```

The deeper idea is:

> Binary search can locate a **boundary**, not just an exact value.

---



# 6. Lower Bound

Lower bound means:

> First position where value is **>= target**.

Example:

```text
[1, 3, 3, 5, 8]
```

Target:

```text
3
```

Lower bound = index of first `3`.

Target:

```text
4
```

Lower bound = index of `5`.

This is extremely useful.

Think of the predicate:

```text
a[i] >= target
```

Across sorted data:

```text
false false false true true
```

Binary search finds the first `true`.

---



# 7. Upper Bound

Upper bound means:

> First position where value is **> target**.

For:

```text
[1, 3, 3, 5, 8]
```

Target:

```text
3
```

Upper bound points to:

```text
5
```

So:

```text
count(target)
=
upper_bound(target)
-
lower_bound(target)
```

For `3`:

```text
3 appears twice
```

This is a useful example of turning duplicate counting into boundary finding.

---



# 8. Think in predicates

This is the deepest Binary Search mindset.

Instead of thinking:

> Find 7.

Think:

> Find the first position where some condition becomes true.

Example:

```text
index:      0 1 2 3 4 5
condition:  F F F T T T
```

Find boundary:

```text
          ↑
      first true
```

Or:

```text
T T T T F F F
```

Find last true.

This formulation makes Binary Search much easier to generalize.

---



# 9. Binary Search on Answer

This is where Binary Search becomes much more powerful.

Suppose a problem asks:

> What is the minimum capacity needed to satisfy some requirement?

You may not have a sorted array at all.

But candidate answers might behave like:

```text
capacity:

1   → impossible
2   → impossible
3   → impossible
4   → possible
5   → possible
6   → possible
```

That's monotonic:

```text
F F F T T T
```

So binary search can find:

```text
first possible answer
```

This is Binary Search on Answer.

---



# 10. Example intuition — Minimum capacity

Imagine shipping packages within `D` days.

For capacity:

```text
10 → impossible
15 → impossible
20 → possible
25 → possible
30 → possible
```

If capacity `20` works, then a larger capacity should also work.

So:

```text
works(capacity)
```

is monotonic.

Then search:

```text
minimum capacity where works(capacity) == true
```

That's not searching an array value.

You're searching an **answer space**.

---



# 11. Other Binary Search on Answer clues

Watch for phrases like:

```text
minimum possible X such that...
maximum possible X such that...
smallest capacity...
largest minimum distance...
minimum speed...
minimum time...
```

Then ask:

> If one candidate works, do all larger candidates work?

or:

> If one candidate works, do all smaller candidates work?

If yes, Binary Search on Answer may fit.

---



# 12. Rotated Sorted Arrays

Suppose:

```text
[4,5,6,7,0,1,2]
```

It was sorted, then rotated.

Normal sorted ordering is broken globally.

But here's the key:

> At least one half around `mid` is still sorted.

Example:

```text
L       M         R
↓       ↓         ↓
4 5 6 7 0 1 2
```

Left half:

```text
4 5 6 7
```

is sorted.

Then ask:

> Is the target inside that sorted half?

If yes, search there.

Otherwise search the other half.

The deeper skill is:

> Identify which side still has enough order to eliminate possibilities.

---



# 13. Duplicates make rotated search harder

Example:

```text
[2,2,2,3,4,2]
```

Sometimes:

```text
a[left] == a[mid] == a[right]
```

and you can't tell which half is sorted confidently.

You may need to shrink boundaries.

Worst-case complexity can degrade toward:

```text
O(n)
```

So again:

> Binary Search is powerful only when the monotonic/order information is strong enough.

---



# 14. Midpoint safety

In C++ you often see:

```cpp
mid = left + (right - left) / 2;
```

instead of:

```cpp
mid = (left + right) / 2;
```

Why?

Historically, `left + right` can overflow for large integers.

So the safer form is:

```text
left + half the distance
```

Small implementation detail, useful to know.

---



# 15. Closed vs Half-Open Intervals

Two common styles:

```text
[left, right]
```

inclusive,

or:

```text
[left, right)
```

half-open.

Both are valid.

The important thing is:

> Choose one invariant and be consistent.

Many Binary Search bugs come from mixing interval conventions.

---



# 16. Binary Search failure modes

Common mistakes:

- Wrong loop condition
- Updating `left = mid` instead of `mid + 1`
- Updating `right = mid` incorrectly
- Infinite loops
- Off-by-one errors
- Returning the wrong boundary
- Assuming exact-match logic works for lower bound
- Using binary search when the predicate isn't monotonic

So for every version, write the invariant first.

---



# 17. Binary Search recognition

Think:

```text
sorted array
        ↓
Binary Search candidate
```

```text
first / last occurrence
        ↓
Boundary Binary Search
```

```text
first >= x
        ↓
Lower Bound
```

```text
first > x
        ↓
Upper Bound
```

```text
minimum feasible answer
        ↓
Binary Search on Answer
```

```text
monotonic predicate
        ↓
Binary Search candidate
```

---



# 18. Binary Search problems to practice

A good deep pass:

- [x] Binary Search
- [x] Search Insert Position
- [x] First and Last Position of Element
- [x] Lower Bound
- [x] Upper Bound
- [x] Search in Rotated Sorted Array
- [x] Find Minimum in Rotated Sorted Array
- [ ] Koko Eating Bananas
- [ ] Capacity To Ship Packages Within D Days

That is enough to understand the major forms.

---



# In-place Array Manipulation

The core idea is:

> **Modify the existing array itself instead of building another array.**

Normally you might solve:

```text
input
 ↓
create output array
 ↓
copy transformed values
```

using:

```text
O(n) extra space
```

In-place algorithms ask:

> Can I reuse the input storage?

Goal:

```text
O(1) auxiliary space
```

---



# 1. Important distinction

In-place does **not** mean:

```text
O(1) time
```

It means:

```text
small constant extra memory
```

You may still take:

```text
O(n)
O(n log n)
O(n²)
```

time.

Time and space are separate dimensions.

---



# 2. Swap-based manipulation

The simplest pattern.

Suppose:

```text
[1,2,3,4,5]
```

Reverse:

```text
swap(0,4)
swap(1,3)
```

Result:

```text
[5,4,3,2,1]
```

Only a temporary variable is required.

Space:

```text
O(1)
```

This is:

```text
Two Pointers
+
In-place Swap
```

---



# 3. In-place reversal

This becomes a reusable primitive.

For subarray:

```text
[L ... R]
```

use:

```text
L →       ← R
```

and swap inward.

This primitive later helps with:

- Rotate array
- Reverse words
- Reorder segments
- Permutation-related operations

---



# 4. Rotation

Suppose:

```text
[1,2,3,4,5,6,7]
```

Rotate right by 3:

```text
[5,6,7,1,2,3,4]
```

A naive approach creates another array.

But you can do it in-place using reversals:

```text
1. Reverse whole array
2. Reverse first k
3. Reverse remaining n-k
```

Example:

```text
Original:
1 2 3 4 5 6 7

Reverse all:
7 6 5 4 3 2 1

Reverse first 3:
5 6 7 4 3 2 1

Reverse rest:
5 6 7 1 2 3 4
```

Beautiful example of composing a primitive.

---



# 5. Compaction

Suppose:

```text
[0,1,0,3,12]
```

Want:

```text
[1,3,12,0,0]
```

You could allocate a new array.

Instead use read/write pointers.

```text
read scans
write marks next valid position
```

Process non-zero values:

```text
[1,3,12,_,_]
```

Then fill remainder with zeros:

```text
[1,3,12,0,0]
```

This is in-place compaction.

---



# 6. Remove duplicates in-place

Sorted:

```text
[1,1,2,2,3,3]
```

Maintain:

```text
write
read
```

The invariant:

> Everything before `write` contains the compacted unique output.

Eventually:

```text
[1,2,3, ...]
```

Return logical length:

```text
3
```

The physical array may still contain old values after the logical result.

That's okay.

This introduces an important distinction:

> **Physical array size vs logical valid length.**

---



# 7. Overwriting

Sometimes you don't need swaps.

You can simply overwrite positions.

Example filtering positives:

```text
[3,-1,5,-7,8]
```

Read:

```text
3  → keep
-1 → skip
5  → keep
-7 → skip
8  → keep
```

Write:

```text
[3,5,8,_,_]
```

That's often simpler and faster than repeatedly deleting elements.

---



# 8. Partitioning

Partitioning means rearranging elements into groups according to a condition.

Example:

```text
[7,2,9,4,1,8]
```

Partition around:

```text
pivot = 5
```

Wanted conceptually:

```text
[values < 5 | values >= 5]
```

Possible result:

```text
[2,4,1 | 7,9,8]
```

The exact order may not matter.

Partitioning is fundamental because it appears in:

- Quick Sort
- Quickselect
- Move Zeroes variants
- Dutch National Flag
- Filtering
- Grouping by predicate

---



# 9. Two-region invariant

A very common partition idea:

```text
[ processed-good | unknown ]
       ↑              ↑
      write          read
```

As `read` scans, if the element belongs in the "good" region:

```text
swap/write it at boundary
advance boundary
```

Invariant:

> Everything before the boundary satisfies the desired condition.

This style of thinking is extremely reusable.

---



# 10. Dutch National Flag

Suppose values are only:

```text
0, 1, 2
```

Want:

```text
0s | 1s | 2s
```

Use three regions:

```text
[0 region | 1 region | unknown | 2 region]
```

with:

```text
low
mid
high
```

This is a deeper partitioning pattern.

The key is not memorizing the exact code.

Understand:

> Maintain several regions whose meanings remain true throughout the algorithm.

This is an invariant-driven algorithm.

---



# 11. Move Zeroes — two possible in-place styles



### Style A: overwrite

First compact non-zero values:

```text
[1,3,12,_,_]
```

then fill zeros.

### Style B: swap

Whenever read sees non-zero:

```text
swap(a[write], a[read])
write++
```

Both can be valid.

Compare:

- Number of writes
- Stability
- Simplicity
- Whether order must be preserved

---



# 12. Stable vs unstable in-place transformations

Suppose:

```text
[3,0,1,0,2]
```

If you move zeros while preserving non-zero order:

```text
[3,1,2,0,0]
```

that's stable relative to non-zero elements.

But some partition algorithms may produce:

```text
[1,3,2,0,0]
```

which still groups correctly but changes relative order.

Always ask:

> Does relative order matter?

This affects which in-place strategy you can use.

---



# 13. In-place doesn't always mean no extra variables

You're allowed a few variables:

```text
left
right
temp
count
pivot
```

That's still:

```text
O(1) auxiliary space
```

The restriction is against storage growing with `n`.

So:

```text
vector<int> extra(n)
```

is O(n), not in-place.

---



# 14. Cyclic replacement

Rotation can also be done via cycles.

Suppose moving each index to:

```text
(i + k) % n
```

You can follow positions in a cycle, moving values around without a full extra array.

This is a more advanced in-place technique.

You don't need to master it before the reversal method, but understand the concept:

> A permutation of indices decomposes into cycles.

This becomes useful later.

---



# 15. In-place matrix operations

For square matrices, some transformations can also be done in-place:

- Transpose
- Rotate 90 degrees

For rotation:

```text
transpose
+
reverse each row
```

This is another example of composing simple in-place primitives.

You can learn this during your 2D array section.

---



# 16. In-place complexity trade-offs

Sometimes avoiding extra memory makes code:

- More complex
- Harder to reason about
- Less stable
- More write-heavy

For example:

```text
extra array
```

may give cleaner logic.

So ask:

> Is O(1) extra space actually required?

In production software, clarity may sometimes matter more than shaving O(n) temporary memory.

---



# 17. In-place recognition clues

Watch for:

```text
"modify array in-place"
        ↓
In-place manipulation
```

```text
"O(1) extra space"
        ↓
Swap / overwrite / partition
```

```text
"remove elements"
        ↓
Read / Write pointers
```

```text
"move all X to one side"
        ↓
Partition
```

```text
"rotate array"
        ↓
Reversal / cycles
```

```text
"preserve relative order"
        ↓
Stable compaction
```

---



# 18. Core problems to practice

For a strong first pass:

- [x] Reverse String / Array
- [x] Move Zeroes
- [x] Remove Element
- [x] Remove Duplicates from Sorted Array
- [x] Rotate Array
- [x] Sort Colors
- [x] Partition Array Around Pivot
- [x] Reverse Words in a String conceptually
- [ ] Rotate Image later with matrices

That's enough.

---



# 19. For every in-place problem, ask

```text
1. What output must remain in the same storage?

2. Can I overwrite values safely?

3. Do I need original values later?

4. Should I use swaps or read/write?

5. Does relative order matter?

6. What regions of the array already have correct meaning?

7. What is the invariant?

8. Can changing a value destroy information I still need?

9. Time complexity?

10. Auxiliary space?

11. Would an extra array make the algorithm significantly simpler?

12. Is O(1) space actually required?
```

That "will I destroy information I still need?" question is crucial.

It's the array equivalent of "don't lose the rest of the list" in Linked Lists.

---



# How G and H connect to your other patterns

At this point your Arrays & Strings patterns start combining:

```text
Binary Search
+
Sorting
```

```text
In-place
+
Two Pointers
```

```text
Partitioning
+
Quick Sort
```

```text
Read/Write
+
Compaction
```

```text
Rotation
+
Reversal
```

```text
Binary Search on Answer
+
Greedy/feasibility check
```

This is exactly what you want: stop seeing each pattern as an isolated trick and start seeing them as reusable building blocks.

So the two sentences to remember are:

> **Binary Search:** Find a monotonic boundary and repeatedly eliminate half the remaining search space.

> **In-place manipulation:** Reuse the input storage by maintaining meaningful regions and carefully swapping or overwriting without destroying information you still need.

