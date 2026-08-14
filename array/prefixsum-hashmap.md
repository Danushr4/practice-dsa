# Prefix Sum

The central idea is:

> Precompute cumulative information once, so repeated range queries become cheap.

Suppose:

```text
[2, 4, 1, 7, 3]
```

A normal range sum query like:

```text
sum from index 1 to 3
```

could scan:

```text
4 + 1 + 7
```

That costs O(length of range).

If you have many such queries, repeated scanning becomes expensive.

## 1. Build a prefix sum

Define:

```text
prefix[i] = sum of elements from index 0 through i
```

For:

```text
array:   2  4  1  7  3
prefix:  2  6  7 14 17
```

Now:

```text
sum(L..R)
=
prefix[R] - prefix[L-1]
```

For `L = 1`, `R = 3`:

```text
prefix[3] - prefix[0]
= 14 - 2
= 12
```

which is:

```text
4 + 1 + 7
```

So:

- Building prefix array: O(n)
- Each range-sum query: O(1)

That's the trade-off:

> Spend O(n) preprocessing and O(n) extra memory to make future queries O(1).

---



# 2. A cleaner prefix representation

In practice, this form is often easier:

```text
prefix[0] = 0
prefix[i+1] = prefix[i] + a[i]
```

For:

```text
a = [2,4,1,7,3]
```

you get:

```text
prefix = [0,2,6,7,14,17]
```

Then:

```text
sum(L..R)
=
prefix[R+1] - prefix[L]
```

This avoids special handling when `L = 0`.

That's an important implementation detail.

---



# 3. Why prefix subtraction works

Suppose:

```text
prefix[R]
```

contains:

```text
a[0] + a[1] + ... + a[R]
```

and:

```text
prefix[L-1]
```

contains:

```text
a[0] + ... + a[L-1]
```

Subtract:

```text
everything before L cancels
```

leaving:

```text
a[L] + ... + a[R]
```

This cancellation idea is the real concept.

---



# 4. Prefix Sum is not only about sums

"Prefix sum" is often used broadly for **prefix accumulation**.

You can maintain:

- Count of vowels up to position i
- Count of zeros
- Count of a character
- XOR prefix
- Number of positives
- Number of events

Example string:

```text
"abacaba"
```

Suppose you want:

> How many `a`s appear between L and R?

Build:

```text
prefixA[i]
=
number of 'a' characters before i
```

Then range count is again:

```text
prefixA[R+1] - prefixA[L]
```

So the deeper pattern is:

> Precompute cumulative state so range state can be obtained by subtraction/difference.

---



# 5. Prefix Frequency / Counts

Suppose characters are only lowercase English letters.

You could build:

```text
prefix[i][26]
```

where:

```text
prefix[i][c]
```

means:

> number of character `c` among the first `i` characters.

Then for any substring:

```text
frequency of c in [L,R]
=
prefix[R+1][c] - prefix[L][c]
```

This is useful when you need many substring frequency queries.

---



# 6. Prefix XOR

Important variation.

For XOR:

```text
prefix[i+1] = prefix[i] XOR a[i]
```

Then:

```text
xor(L..R)
=
prefix[R+1] XOR prefix[L]
```

Why?

Because:

```text
x XOR x = 0
```

so the common prefix cancels.

This becomes useful in several bitwise problems later.

---



# 7. Suffix Sums

Prefix:

```text
prefix[i]
=
information from beginning → i
```

Suffix:

```text
suffix[i]
=
information from i → end
```

Example:

```text
a = [2,4,1,7]
```

Suffix sums:

```text
[14,12,8,7]
```

Useful when a problem needs information about:

```text
left side
+
right side
```

For example:

- Pivot index
- Product except self style reasoning
- Left/right cumulative comparisons
- Partition problems

---



# 8. Prefix + Suffix Pattern

Suppose you want an index where:

```text
sum left == sum right
```

Instead of recalculating both sides each time:

```text
prefix information
+
total sum / suffix information
```

lets you test each position in O(1).

Total becomes O(n), not O(n²).

---



# 9. 2D Prefix Sum

Now imagine a matrix:

```text
1 2 3
4 5 6
7 8 9
```

You want:

> Sum of any rectangular region.

Without preprocessing, each query may scan many cells.

2D prefix sum stores cumulative sums over rectangles.

Define:

```text
prefix[r][c]
=
sum of rectangle from (0,0) to (r-1,c-1)
```

Then rectangle queries can be answered using inclusion-exclusion.

Conceptually:

```text
wanted rectangle
=
big prefix
- top strip
- left strip
+ overlap
```

Formula with half-open indexing:

```text
sum(r1..r2, c1..c2)
=
P[r2+1][c2+1]
- P[r1][c2+1]
- P[r2+1][c1]
+ P[r1][c1]
```

Don't memorize the signs blindly.

Understand:

- subtract top
- subtract left
- their overlap was subtracted twice
- add overlap back once

---



# 10. When Prefix Sum is useful

Recognition clues:

```text
many range sum queries
        ↓
Prefix Sum
```

```text
subarray cumulative property
        ↓
Potential Prefix Sum
```

```text
count something in many substrings/ranges
        ↓
Prefix Counts
```

```text
left information + right information
        ↓
Prefix + Suffix
```

```text
rectangle sum queries
        ↓
2D Prefix Sum
```

---



# 11. Prefix Sum + Hash Map

This is one of the most important combinations.

Suppose:

> Count subarrays whose sum equals K.

For a subarray `i..j`:

```text
prefix[j] - prefix[i] = K
```

Rearrange:

```text
prefix[i] = prefix[j] - K
```

So while scanning current prefix `P`, ask:

```text
How many previous prefixes equal P - K?
```

That's a hash lookup.

So:

```text
Prefix Sum
+
Hash Map
```

turns a potentially O(n²) subarray search into expected O(n).

This connection is extremely important.

---



# 12. Difference Arrays

Now the inverse perspective.

Prefix sum is great for:

> Querying ranges.

Difference arrays are great for:

> Updating ranges.

Suppose:

```text
a = [0,0,0,0,0,0]
```

You want to add `+5` to indices:

```text
1 through 4
```

Naively:

```text
a[1] += 5
a[2] += 5
a[3] += 5
a[4] += 5
```

That's O(length of range).

With a difference array:

```text
diff[L] += value
diff[R+1] -= value
```

So:

```text
diff[1] += 5
diff[5] -= 5
```

Later, take the prefix sum of `diff`.

Result:

```text
[0,5,5,5,5,0]
```

So a range update becomes O(1).

---



# 13. Why Difference Arrays work

Think of `diff[i]` as:

> How much does the value change when entering index i?

If:

```text
+5 starts at L
```

record:

```text
diff[L] += 5
```

If it stops after R:

```text
diff[R+1] -= 5
```

Then cumulative sum reconstructs the final values.

This is the duality:

```text
Difference Array
   ↓ prefix sum
Actual Array
```

and conceptually:

```text
Actual Array
   ↓ differences
Difference Array
```

---



# 14. Multiple Range Updates

This is where difference arrays become powerful.

Suppose there are 100,000 updates like:

```text
add x to [L,R]
```

Naively updating every element might become enormous.

With differences:

```text
for each update:
    diff[L] += x
    diff[R+1] -= x
```

Each update O(1).

Then one final prefix pass:

```text
O(n)
```

Total:

```text
O(number_of_updates + n)
```

instead of potentially:

```text
O(number_of_updates × range_length)
```

---



# 15. Prefix Sum vs Difference Array

Think:


| Need                            | Technique              |
| ------------------------------- | ---------------------- |
| Many range queries              | Prefix Sum             |
| Many range updates              | Difference Array       |
| Final values after updates      | Prefix over Difference |
| Count/sum in arbitrary interval | Prefix                 |
| Rectangle sums                  | 2D Prefix              |


The complement is:

> Prefix compresses past values into cumulative information. Difference compresses range changes into boundary events.

---



# Prefix Sum Checklist

```text
PREFIX SUM
│
├── 1D Prefix
│   ├── Build cumulative array
│   ├── Range queries
│   └── O(n) preprocessing + O(1) query
│
├── Prefix Counts
│   ├── Character counts
│   ├── Zero/one counts
│   └── Frequency by range
│
├── Variants
│   ├── Prefix XOR
│   ├── Prefix min/max concept
│   └── Suffix accumulation
│
├── 2D Prefix
│   ├── Rectangle preprocessing
│   └── Inclusion-exclusion
│
├── Combinations
│   ├── Prefix + Hash Map
│   ├── Prefix + Suffix
│   └── Prefix + binary search sometimes
│
└── Difference Arrays
    ├── Boundary changes
    ├── O(1) range update
    └── Prefix to reconstruct result
```

---



# Hashing / Frequency Counting

The core idea is:

> Store useful information about what you've already processed so you don't have to search for it again.

---



# 1. Hash Set — Existence

Use a set when the question is:

> Have I seen this value?

Example:

```text
[4,7,1,7]
```

Process:

```text
seen = {}

4 → not seen → add
7 → not seen → add
1 → not seen → add
7 → already seen
```

Duplicate found.

Typical uses:

- Duplicate detection
- Membership
- Visited states
- Unique elements

---



# 2. Hash Map — Association

Use a map when you need:

```text
key → information
```

Examples:

```text
value → count
value → index
string → group
prefix sum → frequency
```

That's broader than a set.

---



# 3. Frequency Array

Not every counting problem needs a hash map.

Suppose values are characters `'a'` to `'z'`.

You can use:

```cpp
int freq[26];
```

Then:

```text
'a' → 0
'b' → 1
...
'z' → 25
```

This is often faster and simpler than hashing.

Use when:

```text
key domain is small + known + dense
```

Examples:

- lowercase letters
- digits 0–9
- ASCII characters
- bounded small integers

---



# 4. Character Frequency Table

Example:

```text
"banana"
```

Frequency:

```text
a → 3
b → 1
n → 2
```

This unlocks:

- Anagram checks
- Character counts
- Frequency comparison
- Permutation problems
- Sliding-window character constraints

---



# 5. Duplicate Detection

Without extra memory:

```text
Compare every pair
```

can be O(n²).

Hash set approach:

```text
for each x:
    if x in seen:
        duplicate
    seen.insert(x)
```

Expected:

```text
O(n)
```

Extra space:

```text
O(n)
```

This is the classic:

> trade space for time.

---



# 6. Value → Index Mapping

Sometimes existence isn't enough.

Example:

```text
[8, 3, 6, 2]
```

Store:

```text
8 → 0
3 → 1
6 → 2
2 → 3
```

Now if you need a matching value, you can immediately recover its position.

This is exactly what Two Sum often needs.

---



# 7. Complement Lookup

This is one of the most reusable hashing patterns.

Target:

```text
10
```

Current:

```text
x = 3
```

Need:

```text
10 - 3 = 7
```

So ask:

```text
Have I already seen 7?
```

That is much better than searching all previous values.

Pseudo-logic:

```cpp
for each x:
    need = target - x

    if need exists:
        answer found

    store x
```

The reusable idea is not Two Sum itself.

It's:

> Convert the relationship into "what partner do I need?", then use constant-time lookup for that partner.

---



# 8. Why insertion order matters

Suppose target = 6 and current value = 3.

If you insert `3` before checking:

```text
need = 3
```

you may accidentally match the element with itself depending on the problem.

So often:

```text
check first
then insert
```

This is why implementation order is part of the reasoning.

---



# 9. Frequency Counting

Instead of:

```text
Have I seen x?
```

ask:

```text
How many x have I seen?
```

Map:

```text
value → count
```

Example:

```text
[2,2,3,1,2,3]
```

becomes:

```text
1 → 1
2 → 3
3 → 2
```

This supports:

- Counting pairs
- Majority/frequency questions
- Anagrams
- Top-frequency problems
- Exact occurrence constraints

---



# 10. Set vs Frequency Map

This distinction matters.

If you only care:

```text
exists?
```

use a set.

If you care:

```text
how many?
```

use a map/array.

Example:

```text
[1,1,1]
```

Set knows:

```text
1 exists
```

Frequency map knows:

```text
1 appears 3 times
```

Different information.

---



# 11. Grouping

Another powerful hashing pattern:

> Compute a canonical key and group everything with the same key.

Example anagrams:

```text
eat
tea
ate
```

All can normalize to:

```text
aet
```

Map:

```text
"aet" → ["eat","tea","ate"]
```

Another key could be a frequency vector.

The deeper pattern is:

```text
object
 ↓
canonical representation
 ↓
hash key
 ↓
group
```

---



# 12. Frequency Vector as Key

For lowercase strings, instead of sorting:

```text
eat → aet
```

you can represent:

```text
a:1
e:1
t:1
others:0
```

That 26-count vector becomes a canonical representation.

This idea generalizes:

> Two objects belong together if some normalized state is identical.

---



# 13. Hashing + Sliding Window

This combination is extremely common.

Example:

> Longest substring without repeating characters.

Sliding Window maintains the current range.

Hash Set / frequency map tracks:

```text
which characters are inside?
```

So:

```text
Sliding Window
+
Hashing
```

solves the problem.

Another:

> Longest substring with at most K distinct characters.

Track:

```text
char → frequency
```

and:

```text
distinctCount
```

Again:

```text
Window structure + hash state
```

---



# 14. Hashing + Prefix Sum

As discussed:

```text
current prefix = P
want subarray sum K
```

Need previous:

```text
P - K
```

So store:

```text
prefix value → frequency
```

This is one of the strongest pattern compositions in Arrays.

---



# 15. Pair Counting with Frequencies

Suppose:

> Count number of pairs summing to target.

At current `x`:

```text
need = target - x
```

If:

```text
freq[need] = 3
```

then current `x` creates 3 new pairs with previous values.

So:

```text
answer += freq[need]
freq[x]++
```

This is more powerful than a simple set because multiplicity matters.

---



# 16. First / Last Occurrence Mapping

You may store:

```text
value → first index
```

or:

```text
value → last index
```

depending on the problem.

Examples:

- Longest distance between repeated values
- Earliest occurrence
- Subarray length problems
- Prefix-state problems

The map value isn't always "count."

That's why before using a hash map, always ask:

> What information exactly do I need to remember?

---



# 17. State Encoding

Suppose the relevant state is:

```text
countA - countB
```

or:

```text
(row,column)
```

or:

```text
some normalized configuration
```

You can hash **derived state**, not just raw input values.

This is a big conceptual jump.

Hash maps are often used as:

> memory of previously encountered algorithm states.

This becomes very important later in graphs, DP, geometry, and memoization.

---



# 18. When a frequency array is better than a hash map

Suppose input values are only:

```text
0..100
```

Using:

```text
int freq[101]
```

may be better than a hash map because:

- no hashing overhead
- contiguous memory
- predictable performance
- better cache behavior
- simpler implementation

So recognition should be:

```text
small dense key space
        ↓
array
```

```text
large/sparse/arbitrary keys
        ↓
hash map
```

Don't mechanically reach for `unordered_map`.

---



# 19. Hashing vs Sorting

Suppose:

> Find duplicates.

Options:

Hashing:

```text
Expected O(n)
Space O(n)
```

Sorting:

```text
O(n log n)
Potentially low extra space
```

Sorting may also give useful ordering.

So hashing is not universally better.

Ask:

- Do I need order?
- Can I modify the input?
- Is extra memory acceptable?
- Do I need deterministic worst-case behavior?

---



# 20. Pattern recognition

Train these reactions:

```text
"Have I seen this?"
        ↓
Hash Set
```

```text
"How many times?"
        ↓
Frequency Map / Array
```

```text
"Where did I see it?"
        ↓
Value → Index Map
```

```text
"What partner do I need?"
        ↓
Complement Lookup
```

```text
"Group equivalent items"
        ↓
Canonical Key + Map
```

```text
"Current range needs character counts"
        ↓
Sliding Window + Frequency Map
```

```text
"Subarray cumulative relationship"
        ↓
Prefix Sum + Hash Map
```

---



# 21. Core problems to practice

For this Arrays/Strings pass, these are enough:

- Contains Duplicate
- Valid Anagram
- Two Sum
- Intersection of Two Arrays
- Group Anagrams
- First Unique Character
- Longest Substring Without Repeating Characters
- Subarray Sum Equals K
- Isomorphic Strings
- Count pairs with target sum

The purpose isn't to collect solved problems.

It's to see the recurring information structures:

```text
existence
count
index
group
state
complement
```

---



# Combined mental model

These two topics connect beautifully:

```text
Prefix Sum
=
remember cumulative information about the past
```

```text
Hashing
=
remember searchable information about the past
```

Together:

```text
Current cumulative state
        ↓
What previous state would create the answer?
        ↓
Look that state up in a hash map
```

That is exactly why **Prefix Sum + Hash Map** is such a powerful pattern.

For your Arrays & Strings study, after Two Pointers and Sliding Window, these two are absolutely worth learning deeply before moving to Binary Search / Sorting + Scan.