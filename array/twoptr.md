# Two Pointers 

## 1. What is a "pointer" here?

In this pattern, "pointer" doesn't necessarily mean a C++ pointer like:

```cpp
int* p;
```

It usually means an **index representing a position**.

Given:

```text
index:  0   1   2   3   4
array: [2] [4] [6] [8] [10]
```

We might have:

```cpp
int left = 0;
int right = 4;
```

Conceptually:

```text
       L               R
       ↓               ↓
      [2] [4] [6] [8] [10]
```

So two pointers simply means:

> Maintain two positions and move them intelligently based on information discovered during the algorithm.

The word **intelligently** is the important part.

---



# 2. Why do we need Two Pointers?

Consider:

```text
Find two numbers whose sum is 10.

[1, 2, 3, 4, 6, 8]
```

The straightforward approach is:

```cpp
for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
        if (a[i] + a[j] == 10)
            return true;
    }
}
```

We're checking:

```text
1+2
1+3
1+4
1+6
1+8

2+3
2+4
2+6
2+8

3+4
3+6
3+8
...
```

Potentially:

```text
O(n²)
```

But notice something critical:

### The array is sorted.

That gives us information.

Start:

```text
L                   R
↓                   ↓
1  2  3  4  6  8

1 + 8 = 9
```

We need `10`.

Our sum is too small.

Should we move `R` left?

No.

That would make the number **smaller**.

Instead move `L` right:

```text
   L                R
   ↓                ↓
1  2  3  4  6  8

2 + 8 = 10
```

Found.

That's the heart of Two Pointers:

> Use some property of the data to prove that certain possibilities can be discarded without checking them individually.

---



# 3. The most important question

Don't ask:

> "Can I use two pointers?"

Ask:

> **"If I move one pointer, can I safely eliminate part of the search space?"**

If yes, Two Pointers may be appropriate.

That's why sorted data is so powerful.

---



# 4. Variant A — Opposite-Direction Pointers

The classic structure:

```text
L → [...................] ← R
```

Usually:

```cpp
int left = 0;
int right = n - 1;

while (left < right) {

    // inspect a[left], a[right]

    if (...) {
        left++;
    }
    else {
        right--;
    }
}
```

Both pointers move toward each other.

Common applications:

- Pair sum in sorted array
- Palindrome
- Reverse array
- Container-style problems
- Some partitioning problems

But the **reason they move** differs between problems.

---



# 5. Example — Reverse an Array

Input:

```text
1 2 3 4 5
```

Wanted:

```text
5 4 3 2 1
```

Start:

```text
L       R
↓       ↓
1 2 3 4 5
```

Swap:

```text
5 2 3 4 1
```

Move:

```text
  L   R
  ↓   ↓
5 2 3 4 1
```

Swap:

```text
5 4 3 2 1
```

Move:

```text
    L
    ↓
5 4 3 2 1
```

Done.

Code:

```cpp
while (left < right) {
    swap(a[left], a[right]);
    left++;
    right--;
}
```

Time:

```text
O(n)
```

Space:

```text
O(1)
```

Notice something:

This isn't optimizing an O(n²) algorithm.

Two pointers are simply the **natural way to express the symmetric operation**.

That's important: Two Pointers isn't always about reducing O(n²).

---



# 6. Example — Palindrome

String:

```text
racecar
```

A palindrome reads the same forward and backward.

Instead of reversing the entire string, compare corresponding characters:

```text
L           R
↓           ↓
r a c e c a r

r == r ✓
```

Move:

```text
  L       R
  ↓       ↓
r a c e c a r

a == a ✓
```

Again:

```text
    L   R
    ↓   ↓
r a c e c a r

c == c ✓
```

Eventually:

```text
      L
      ↓
r a c e c a r
```

Palindrome.

Why Two Pointers?

Because the property itself is defined symmetrically:

```text
s[0] == s[n-1]
s[1] == s[n-2]
...
```

---



# 7. Example — Two Sum on Sorted Input

Now we get to the powerful search-space elimination.

Input:

```text
[1, 2, 4, 6, 8, 11]
```

Target:

```text
10
```

Start:

```text
L                 R
↓                 ↓
1  2  4  6  8  11

1 + 11 = 12
```

Too large.

Because the array is sorted, keeping `11` and moving `L` right would only make the sum even larger.

Therefore:

```text
R--
```

Now:

```text
L              R
↓              ↓
1  2  4  6  8  11

1 + 8 = 9
```

Too small.

Keeping `1` and moving R left would only make it smaller.

Therefore:

```text
L++
```

Now:

```text
   L           R
   ↓           ↓
1  2  4  6  8  11

2 + 8 = 10
```

Found.

---



# 8. Why is that O(n), not O(n²)?

This is extremely important.

Nested loops might check roughly:

```text
n × n
```

combinations.

With Two Pointers:

```text
L → → → →
← ← ← ← R
```

`L` can move at most `n` times.

`R` can move at most `n` times.

So total pointer movements are bounded by roughly:

```text
2n
```

Therefore:

```text
O(n)
```

Not:

```text
O(n²)
```

Even though there are two variables called `left` and `right`.

**Two pointers does NOT mean two nested loops.**

---



# 9. But why are we allowed to skip combinations?

This is the deeper question.

Suppose:

```text
L             R
↓             ↓
2  3  5  7  9
```

Target = 10.

Current:

```text
2 + 9 = 11
```

Too large.

Could `3 + 9` work?

```text
3 + 9 = 12
```

No.

Could `5 + 9` work?

Even larger.

Therefore, if:

```text
a[L] + a[R] > target
```

we can safely eliminate `a[R]` as a possible partner with `a[L]` or anything to its right of L in this candidate range.

So:

```text
R--
```

One decision eliminates many possibilities.

**That's where the speedup comes from.**

---



# 10. Variant B — Same-Direction Pointers

Now completely different behavior.

```text
slow →
fast ----->
```

Usually:

```text
[ processed ][ unexplored ]
      ↑            ↑
     slow         fast
```

Both move left → right.

But they have different jobs.

Usually:

### Fast pointer

Reads/scans elements.

### Slow pointer

Tracks where useful output/state should go.

Think:

> `fast` explores. `slow` builds or marks the valid region.

---



# 11. Example — Remove Duplicates

Sorted input:

```text
1 1 2 2 3 3 4
```

Wanted logically:

```text
1 2 3 4
```

We don't want another array.

We want **in-place modification**.

Start:

```text
slow
↓
1 1 2 2 3 3 4
  ↑
 fast
```

Fast sees:

```text
1
```

duplicate.

Skip.

Fast reaches:

```text
1 1 2 2 3 3 4
    ↑
   fast
```

`2` is new.

Move slow:

```text
  slow
    ↓
1 1 2 2 3 3 4
```

Write `2` there:

```text
1 2 2 2 3 3 4
```

Continue.

Eventually the meaningful prefix becomes:

```text
1 2 3 4 | garbage/irrelevant
```

The first four positions contain the answer.

---



# 12. The deeper idea: Read Pointer + Write Pointer

A better mental model than "slow/fast" for many array problems is:

```text
READ
WRITE
```

`read` examines every input element.

`write` marks where the next valid element belongs.

Example:

```text
Input:

0 1 0 3 12
```

Move zeroes to end.

Read scans:

```text
READ →
```

Write tracks the next place for a non-zero value:

```text
WRITE →
```

Result:

```text
1 3 12 0 0
```

This is often called:

> **In-place compaction**

---



# 13. Example — Filter values in-place

Suppose:

```text
[3, -1, 5, -7, 8, 2]
```

Keep only positive numbers.

Fast/read examines:

```text
3 ✓
-1 ✗
5 ✓
-7 ✗
8 ✓
2 ✓
```

Slow/write produces:

```text
[3, 5, 8, 2, ...]
```

Conceptually:

```cpp
int write = 0;

for (int read = 0; read < n; read++) {

    if (a[read] > 0) {
        a[write] = a[read];
        write++;
    }
}
```

Time:

```text
O(n)
```

Extra space:

```text
O(1)
```

This pattern is extremely useful.

---



# 14. There is another same-direction variant

Sometimes the pointers aren't read/write.

They maintain a **distance/gap**.

For example:

```text
slow
 ↓
1 2 3 4 5 6
      ↑
     fast
```

They remain some distance apart.

This becomes very important later for:

- Linked lists
- kth element from end
- Cycle algorithms
- Window boundaries

So don't think `slow/fast` always means the same thing.

Two pointers is a broad technique.

---



# 15. Major Two-Pointer Families

For Arrays & Strings, learn these families:


| Variant           | Shape         | Typical purpose                  |
| ----------------- | ------------- | -------------------------------- |
| Opposite ends     | `L → ... ← R` | Pair search, palindrome, reverse |
| Read/Write        | `W → R →`     | Remove/filter/compact            |
| Slow/Fast         | `S → F →`     | Different traversal speeds       |
| Two sequences     | `i →`, `j →`  | Merge/compare two arrays         |
| Window boundaries | `L → ... R →` | Becomes sliding window           |


That fourth one is also important.

---



# 16. Two Pointers Across Two Arrays

Suppose:

```text
A = [1, 4, 7]
B = [2, 3, 8]
```

Both are sorted.

Merge them.

```text
A:
i
↓
1 4 7

B:
j
↓
2 3 8
```

Compare:

```text
1 vs 2
```

Take 1.

Move only `i`.

```text
A:
  i
  ↓
1 4 7

B:
j
↓
2 3 8
```

Now compare:

```text
4 vs 2
```

Take 2.

Continue.

Result:

```text
1 2 3 4 7 8
```

Time:

```text
O(n + m)
```

This pattern appears everywhere.

---



# 17. Two Pointers vs Sliding Window

They're related, but don't treat them as identical.

Two pointers:

```text
L →             ← R
```

or:

```text
L →
      R →
```

Sliding window specifically represents a **contiguous range**:

```text
      L             R
      ↓             ↓
... [ x x x x x x ] ...
        WINDOW
```

and the algorithm expands/shrinks that range while maintaining some condition.

So:

> Sliding Window is often implemented using two pointers, but not every Two Pointer problem is a Sliding Window problem.

You'll study sliding window separately next.

---



# 18. How to recognize Two Pointers

Look for clues such as:

### Sorted input

```text
sorted array
+
pair/triplet relationship
```

Think:

```text
Two Pointers
```

---



### Opposite-side comparison

```text
Palindrome
Reverse
Symmetry
```

Think:

```text
L / R
```

---



### In-place modification

Problem says:

```text
Remove...
Move...
Filter...
Partition...
without extra array
```

Think:

```text
Read / Write pointers
```

---



### Two sorted sequences

```text
Merge
Intersection
Compare
```

Think:

```text
i / j
```

---



### Contiguous range

```text
substring
subarray
longest/shortest window
```

Think:

```text
Sliding Window
```

which often uses two pointers.

---



# 19. Don't force Two Pointers

Suppose:

```text
[7, 1, 9, 3, 4]
```

Target = 10.

Can we simply do:

```text
L → ... ← R
```

and decide which pointer to move based on the sum?

No.

Why?

Because the array isn't sorted.

If:

```text
a[L] + a[R]
```

is too large, moving left/right gives no predictable effect.

We have no monotonic information.

So Two Sum on an **unsorted array** is commonly solved using:

```text
Hash Map
```

Expected:

```text
O(n)
```

This distinction matters much more than memorizing the template.

---



# 20. Sorting + Two Pointers

Sometimes input isn't sorted, but you're allowed to sort it.

Then:

```text
Unsorted
   ↓
Sort
   ↓
Two Pointers
```

Example complexity:

```text
Sorting      O(n log n)
Two Pointers O(n)

Total        O(n log n)
```

This can still beat:

```text
O(n²)
```

But sorting may destroy original indices.

So you must think about the problem requirements.

---



# 21. The invariant

This is where your DSA understanding becomes deeper.

For each Two Pointer algorithm, ask:

> What remains guaranteed as the pointers move?

For sorted Two Sum:

```text
Everything outside [L, R]
has already been safely eliminated.
```

For Remove Duplicates:

```text
Everything before `slow`
is already the correct compacted answer.
```

For palindrome:

```text
Everything outside [L, R]
has already been verified as matching.
```

That's called an **invariant**.

Understanding the invariant is much more valuable than memorizing code.

---



# 22. Complexity reasoning

Don't say:

> "Two pointers = O(n)."

That's not universally true.

Instead ask:

### How many times can each pointer move?

Example:

```text
L → → → → →
← ← ← ← ← R
```

Each moves at most n positions.

Therefore:

```text
O(n)
```

Same direction:

```text
read  → → → → → n times
write → → →     ≤ n times
```

Total:

```text
O(n)
```

This is the correct reasoning.

---



# 23. Problems you should practice

You don't need 50.

### Opposite direction

**1. Reverse String**

Learn basic L/R movement.

**2. Valid Palindrome**

Learn symmetric comparison.

**3. Two Sum II — Input Array Is Sorted**

Learn search-space elimination.

**4. Container With Most Water**

Learn more subtle pointer-movement reasoning.

---



### Same direction / Read-Write

**5. Remove Duplicates from Sorted Array**

Learn compaction.

**6. Remove Element**

Learn filtering.

**7. Move Zeroes**

Learn stable in-place movement.

---



### Two sequences

**8. Merge Sorted Array**

Learn `i/j` traversal.

**9. Intersection of Two Sorted Arrays** conceptually.

---



### After you're comfortable

**10. 3Sum**

Very important composition:

```text
Sorting
+
Fixed first element
+
Two Pointers
```

It teaches how Two Pointers can reduce:

```text
O(n³)
```

toward:

```text
O(n²)
```

after sorting.

---



# 24. Your Two Pointers checklist

For this pattern, make sure you understand:

```text
TWO POINTERS
│
├── Core Idea
│   ├── Maintain two positions
│   ├── Move based on information
│   └── Eliminate unnecessary work
│
├── Opposite Direction
│   ├── Reverse
│   ├── Palindrome
│   └── Pair Sum
│
├── Same Direction
│   ├── Slow / Fast
│   ├── Read / Write
│   ├── Remove duplicates
│   ├── Filtering
│   └── Move zeroes
│
├── Two Sequences
│   ├── i / j
│   ├── Merge
│   └── Intersection
│
├── Understand
│   ├── Why pointer moves are safe
│   ├── What gets eliminated
│   ├── Invariant
│   ├── Why O(n)
│   └── When it DOESN'T work
│
└── Recognition
    ├── Sorted + pair relationship
    ├── Symmetry
    ├── In-place modification
    ├── Two sorted sequences
    └── Contiguous range → consider Sliding Window
```

The single most important thing to take away is:

> **Two Pointers is not "use** `left` **and** `right` **variables." It's using structure in the problem so that moving a pointer safely eliminates possibilities or maintains a useful processed region.**

Once that idea clicks, problems like Two Sum II, Remove Duplicates, Move Zeroes, Palindrome, Merge Sorted Arrays, and eventually 3Sum stop looking like unrelated tricks. They become variations of the same reasoning pattern.