# Hash Tables / Hashing 

Hashing is one of the most important DSA topics because it introduces a powerful idea:

> Instead of searching through data to find something, compute where it should be.

That is why hash tables can often turn an **O(n) lookup into expected O(1)**.

But don't stop at `unordered_map` / `HashMap`. You should understand **how a hash table actually works internally**.

---

# 1. Start with the problem Hash Tables solve

Suppose you have:

```text
[45, 93, 12, 71, 28, 34, ...]
```

And frequently need to ask:

```text
Does 71 exist?
```

With an unsorted array:

```text
45
93
12
71 ← found
```

Potentially:

```text
O(n)
```

Could we somehow calculate where `71` should be stored?

That's the core idea behind hashing.

```text
key
 ↓
hash function
 ↓
table index
 ↓
value
```

---



# 2. Direct Addressing — understand this first

Imagine all keys are guaranteed to be between `0` and `99`.

You could create:

```text
table[100]
```

If key `42` exists:

```text
table[42] = true
```

Lookup:

```text
table[42]
```

O(1).

Beautiful.

But suppose your keys are:

```text
123456789
982734821
734812934
```

Creating an array large enough for every possible key would waste enormous amounts of memory.

So we need to map a **large key space** into a **smaller table**.

That's hashing.

---



# 3. Hash Function

A hash function converts:

```text
key
 ↓
hash value
 ↓
bucket/index
```

For a simple integer example:

```text
index = key % table_size
```

If table size = `7`:

```text
10 % 7 = 3
17 % 7 = 3
24 % 7 = 3
```

All three map to bucket 3.

And now we have the fundamental problem:

# Collision

Different keys can map to the same location.

genui{"computing_data_structures_indexing_learning_block_staging":{"type_id":"HASH_TABLE_COLLISIONS"}}

Understanding **why collisions are unavoidable and how we handle them** is the heart of hash tables.

---



# 4. What makes a good hash function?

A good hash function should ideally be:

- Deterministic
- Fast to compute
- Distribute keys evenly
- Minimize clustering
- Use information from the whole key

Deterministic means:

```text
hash("cat")
```

must produce the same logical hash for that table operation, rather than behaving randomly on each lookup.

Conceptually:

```text
Poor distribution

0: ███████████
1:
2: ██
3:
4: ███████
```

Better:

```text
0: ███
1: ████
2: ███
3: ████
4: ███
```

You do **not** need cryptographic hashing here.

That's a different subject.

---



# 5. Hash Table Anatomy

Understand these terms:

```text
Hash Table
│
├── Key
├── Value
├── Hash Function
├── Bucket
├── Capacity
├── Size
├── Collision
└── Load Factor
```

Example:

```text
Key: "Alice"
Value: 91

hash("Alice")
       ↓
       4

Table:

0
1
2
3
4 → ("Alice", 91)
5
6
```

A map stores:

```text
key → value
```

A set conceptually stores:

```text
key → existence
```

---



# 6. Hash Map vs Hash Set

Know this clearly.

### Hash Set

Stores unique keys.

```text
{10, 20, 30}
```

Typical operations:

```text
insert(key)
contains(key)
erase(key)
```

Use when you care:

> Has this value appeared?

---



### Hash Map

Stores key/value pairs.

```text
"John" → 82
"Anna" → 91
"Sam"  → 77
```

Typical operations:

```text
put(key, value)
get(key)
contains(key)
erase(key)
```

Use when you care:

> What information is associated with this key?

---



# 7. Collision Resolution

This is essential.

There are two major families you should understand deeply.

## A. Separate Chaining

Each table bucket stores multiple entries.

```text
table[3]
   ↓
[10] → [17] → [24]
```

because:

```text
10 % 7 = 3
17 % 7 = 3
24 % 7 = 3
```

Historically this may be represented using linked lists, though real implementations can use other bucket structures.

Lookup `17`:

```text
hash(17)
   ↓
bucket 3
   ↓
search entries in bucket
```

Understand:

- Insert
- Search
- Delete
- Bucket chains
- Expected bucket length

---



# 8. Open Addressing

Instead of storing multiple objects in one bucket, every key stays directly inside the table.

Collision?

Search for another slot.

Example:

```text
index = hash(key)

[ ][ ][ ][10][ ][ ][ ]
```

Insert 17.

It also wants index 3.

Try next slot:

```text
[ ][ ][ ][10][17][ ][ ]
```

This is:

## Linear Probing

```text
h(k)
h(k)+1
h(k)+2
...
```

wrapping around the table.

Understand:

- Probe sequence
- Collision
- Clustering
- Wrap-around
- Lookup
- Insertion

---



# 9. Other probing strategies

Know conceptually:

### Linear probing

```text
h(k), h(k)+1, h(k)+2...
```

Simple, but can cause:

> Primary clustering.

---



### Quadratic probing

Probe offsets grow quadratically.

Conceptually:

```text
h(k)+1²
h(k)+2²
h(k)+3²
```

Helps reduce some clustering.

---



### Double hashing

Use another hash function to determine probe step.

```text
index =
h1(key) + i × h2(key)
```

You don't need to implement every variation initially.

But understand **why probing strategy matters**.

---



# 10. Deletion in Open Addressing

This is an excellent deep-understanding topic.

Suppose:

```text
A → slot 3
B collides → slot 4
```

Table:

```text
[ ][ ][ ][A][B][ ]
```

Searching B:

```text
slot 3 → A
slot 4 → B
```

Now delete A.

If you simply make slot 3 empty:

```text
[ ][ ][ ][EMPTY][B][ ]
```

When searching B, the algorithm might stop at the empty slot and incorrectly conclude:

```text
B doesn't exist.
```

So open-addressed tables often need a special marker:

```text
DELETED / TOMBSTONE
```

Result:

```text
[ ][ ][ ][DELETED][B][ ]
```

This allows probing to continue.

This is a very important example where deletion is **not simply erase the data**.

---



# 11. Load Factor

One of the most important hash-table concepts.

```text
load factor =
number of stored entries / number of buckets
```

Usually represented as:

```text
α = n / m
```

where:

```text
n = number of elements
m = table capacity
```

Example:

```text
8 elements
10 buckets

load factor = 0.8
```

As the table becomes crowded:

```text
more collisions
more probing
longer bucket chains
```

Performance deteriorates.

---



# 12. Resizing / Rehashing

When load factor crosses some implementation-dependent threshold:

```text
Old table

[ ][A][ ][B][C][D]
```

allocate a larger table:

```text
[ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
```

Then insert existing elements into the new table using the new capacity.

This is:

> Rehashing.

Important:

You usually can't simply copy each element to the identical index because:

```text
hash(key) % old_capacity
```

and:

```text
hash(key) % new_capacity
```

may produce different bucket positions.

---



# 13. Complexity

Typical expected behavior:


| Operation | Average / Expected | Worst |
| --------- | ------------------ | ----- |
| Insert    | O(1)               | O(n)  |
| Search    | O(1)               | O(n)  |
| Delete    | O(1)               | O(n)  |


That word **expected/average** is extremely important.

Do not say:

> Hash map lookup is always O(1).

It isn't.

Worst case:

```text
All keys collide
```

and you can effectively get:

```text
O(n)
```

depending on collision strategy and implementation.

---



# 14. Why expected O(1)?

This is worth understanding.

With a good hash function and controlled load factor, entries distribute reasonably evenly.

Suppose:

```text
100 elements
200 buckets
```

Then average occupancy is small.

So after computing the bucket:

```text
hash(key)
```

you examine only a small expected number of candidates.

Therefore:

```text
Expected O(1)
```

This assumption depends on good distribution and appropriate resizing.

---



# 15. Amortized Complexity

Rehashing can cost:

```text
O(n)
```

So why is insertion commonly described as O(1)?

Because rehashing happens occasionally.

Imagine:

```text
insert
insert
insert
insert
EXPENSIVE RESIZE
insert
insert
insert
...
```

Spread the occasional expensive resize over many inexpensive insertions.

That's **amortized analysis**.

This connects directly to what you learned with dynamic arrays.

---



# 16. Hashing Pattern 1 — Frequency Counting

Probably the most common pattern.

Input:

```text
a b a c a b
```

Create:

```text
a → 3
b → 2
c → 1
```

Pattern:

```cpp
for each x:
    frequency[x]++
```

Used for:

- Character counts
- Word counts
- Duplicate frequencies
- Majority-like problems
- Anagrams

Recognition clue:

> How many times does each thing occur?

Think:

```text
Hash Map / Frequency Array
```

---



# 17. Hashing Pattern 2 — Membership / Seen Set

Problem asks:

> Have I seen this before?

Example:

```text
[7, 3, 9, 3]
```

Traverse:

```text
seen = {}

7 → add
3 → add
9 → add
3 → already exists!
```

Useful for:

- Duplicate detection
- Unique elements
- Cycle/state detection
- Visited states

Pattern:

```text
seen.contains(x)
```

---



# 18. Hashing Pattern 3 — Complement Lookup

Classic Two Sum concept.

Given:

```text
target = 10

array = [3, 5, 7, 2]
```

At `3`:

```text
need = 10 - 3 = 7
```

Ask:

```text
Have I seen 7?
```

The reusable idea is:

> Instead of searching for the matching partner every time, remember useful previous information.

This transforms many:

```text
O(n²)
```

pair searches into expected:

```text
O(n)
```

---



# 19. Hashing Pattern 4 — Value → Index Mapping

Sometimes you don't only need to know that something exists.

You need:

```text
Where did it occur?
```

Example:

```text
5 → index 0
7 → index 1
3 → index 2
```

Useful for:

- Two Sum
- Detecting distances
- Last occurrence
- First occurrence
- Index relationships

---



# 20. Hashing Pattern 5 — Grouping

Suppose:

```text
eat
tea
tan
ate
nat
bat
```

We want groups:

```text
[eat, tea, ate]
[tan, nat]
[bat]
```

Create a normalized key.

For example conceptually:

```text
eat → aet
tea → aet
ate → aet
```

Then:

```text
"aet" → [eat, tea, ate]
```

Pattern:

> Derive a canonical representation and hash by that representation.

This is a very powerful idea.

---



# 21. Hashing Pattern 6 — Prefix Sum + Hash Map

Extremely important.

Suppose you want:

> Number of subarrays with sum `k`.

Prefix sums let us transform:

```text
prefix[j] - prefix[i] = k
```

into:

```text
prefix[i] = prefix[j] - k
```

Now the question becomes:

> Have I seen prefixSum - k before?

That's a hash lookup.

This combination:

```text
Prefix Sum
+
Hash Map
```

is one of the most reusable problem-solving patterns.

Examples:

- Subarray Sum Equals K
- Zero-sum subarray
- Equal numbers of categories under transformations
- Longest subarray satisfying a cumulative condition

---



# 22. Hashing Pattern 7 — State Encoding

Sometimes the "key" isn't a number from the original problem.

You create one.

Suppose a state contains:

```text
(x, y)
```

You might conceptually store:

```text
seen[(x,y)]
```

Or:

```text
(row, column)
```

Or a normalized string.

This is crucial.

A hash table doesn't only remember raw values.

It can remember:

> A representation of the state you've already encountered.

This becomes important later in:

- Graph traversal
- Dynamic programming
- Geometry algorithms
- Simulation
- Memoization

---



# 23. Hashing Pattern 8 — Counting Pairs

Example question:

> How many pairs satisfy some relation?

Instead of comparing all:

```text
i with j
```

use previously seen frequencies.

For example:

```text
need = target - current

pairs += frequency[need]

frequency[current]++
```

Important distinction:

```text
Does pair exist?
```

versus:

```text
How many pairs exist?
```

A set may solve the first.

A frequency map may be needed for the second.

---



# 24. Hashing Pattern 9 — Deduplication

Input:

```text
5 2 5 7 2 9
```

Unique:

```text
5 2 7 9
```

Hash sets naturally represent uniqueness.

But ask:

> Do I need to preserve original order?

If yes, you may combine:

```text
array/list + hash set
```

This teaches an important lesson:

> One algorithm may combine multiple data structures because each serves a different purpose.

---



# 25. Hashing Pattern 10 — Caching / Memoization Concept

Suppose an expensive calculation has already been performed:

```text
input → result
```

Store:

```text
cache[input] = result
```

Next time:

```text
if input exists:
    return cached result
```

This idea appears later in:

- Dynamic programming
- Memoization
- Caches
- Compilers
- Geometry processing
- Applications/services

Hash maps are one of the most important foundations behind caching.

---



# 26. Strings as Hash Keys

Since you already studied strings, connect that knowledge here.

For:

```text
"apple"
```

the table cannot simply use the text as an array index.

The string gets converted into a numeric hash.

Conceptually:

```text
'a', 'p', 'p', 'l', 'e'
          ↓
       combine
          ↓
      hash value
```

Understand at a conceptual level:

- Hashing sequences
- Order matters
- Same string → same expected key behavior
- Different strings can still collide

You don't need to implement industrial-strength string hashing yet.

---



# 27. Equality + Hashing Contract

Very important when working with custom objects.

Suppose two keys are considered equal:

```text
A == B
```

Then their hashes must be compatible with that equality:

```text
hash(A) == hash(B)
```

Otherwise your hash table can behave incorrectly.

But:

```text
hash(A) == hash(B)
```

does **not** imply:

```text
A == B
```

because collisions exist.

This matters a lot with:

- C++ custom hashers
- Java `hashCode()` / `equals()`
- C# equality/hash contracts
- Custom geometry keys

---



# 28. Mutable Keys

Another subtle but important concept.

Imagine inserting an object based on:

```text
(x=10, y=20)
```

Hash:

```text
H(10,20) → bucket 5
```

Then while it is stored, you modify it:

```text
x = 99
```

Now:

```text
H(99,20) → bucket 2
```

But the object is physically still in bucket 5.

Lookup may fail.

So hash keys generally need stable values with respect to fields participating in hashing/equality.

This is particularly important when working with geometric objects.

---



# 29. Hash Tables vs Trees

Eventually you'll encounter:

```text
HashMap
```

versus:

```text
TreeMap / ordered map
```

Hash table:

```text
Expected lookup: O(1)
Ordering: generally no sorted ordering guarantee
```

Balanced BST:

```text
Lookup: O(log n)
Sorted order: yes
```

Ask:

> Do I need fast exact lookup, or do I also need ordering/range operations?

Hashing isn't always automatically the best choice.

---



# 30. Hash Tables vs Direct Frequency Arrays

Suppose values are only:

```text
0–255
```

You might not need a hash map.

Simply use:

```text
count[256]
```

That's often:

- Simpler
- Faster
- More cache-friendly

This is important.

Don't think:

> Frequency counting = always HashMap.

Instead ask:

```text
Is the key range small/dense?
        ↓
Array

Is the key range huge/sparse?
        ↓
Hash Map
```

---



# 31. Real-world performance matters

Big-O isn't the whole story.

Hash tables involve:

- Hash computation
- Memory access
- Collisions
- Resizing
- Poor locality depending on implementation
- Allocation overhead

Arrays can sometimes outperform hash tables even when both appear O(1).

For engineering work this matters.

Always separate:

```text
Asymptotic complexity
```

from:

```text
Actual machine-level cost
```

---



# 32. Implement a Hash Table Yourself

For deep understanding, do this.

## Version 1

Integer keys + separate chaining.

Implement:

```text
insert
find
erase
contains
```

Use something like:

```text
index = key % capacity
```

---



## Version 2

Add:

```text
key → value
```

Build your own map.

---



## Version 3

Add:

```text
load factor
resize
rehash
```

---



## Version 4

Implement simple linear probing.

Add:

- Empty slot
- Occupied slot
- Deleted/tombstone slot

Once you've done those, you understand hash tables far beyond merely calling library APIs.

---



# 33. Representative Problems

You do **not** need 100 hashing questions.

## Fundamentals

- [ ] Implement Hash Set
- [ ] Implement Hash Map



## Membership

- [ ] Contains Duplicate
- [ ] Intersection of Two Arrays



## Frequency

- [ ] Valid Anagram
- [ ] First Unique Character
- [ ] Top K Frequent Elements later with heap



## Complement

- [ ] Two Sum



## Grouping

- [ ] Group Anagrams



## Prefix + Hashing

- [ ] Subarray Sum Equals K
- [ ] Contiguous Array



## Mapping

- [ ] Isomorphic Strings
- [ ] Word Pattern



## Structural/state

- [ ] Happy Number with set approach

That's plenty for your first deep pass.

---



# 34. Pattern Recognition

Train yourself to notice these signals.

```text
"Have I seen this?"
        ↓
Hash Set
```

```text
"How many times?"
        ↓
Frequency Map
```

```text
"Where did I see this?"
        ↓
Value → Index Map
```

```text
"Need complement"
        ↓
Hash Map / Set
```

```text
"Group equivalent objects"
        ↓
Canonical Key + Hash Map
```

```text
"Repeated expensive calculation"
        ↓
Cache / Memoization
```

```text
"Subarray with cumulative property"
        ↓
Prefix Sum + Hash Map
```

```text
"Visited state"
        ↓
Hash Set
```

```text
"Small fixed key range"
        ↓
Maybe Array, NOT Hash Map
```

That last one is particularly important.

---



# 35. For EVERY Hashing Problem

Use this thinking process:

```text
1. What exactly is my key?

2. What information do I need to store?
   Existence?
   Count?
   Index?
   Object?
   Collection?

3. Set or Map?

4. Could an array be better because key range is small?

5. When should I insert into the map?
   Before checking?
   After checking?

6. Do duplicates matter?

7. Does ordering matter?

8. What equality definition am I using?

9. Can collisions occur?
   Yes — always design under that assumption.

10. Expected time?

11. Worst-case time?

12. Extra space?

13. Can I trade space for time?

14. Is there another solution using sorting?
```

Question 5 is especially useful.

For Two Sum, whether you check before inserting the current element can affect whether you accidentally pair an element with itself.

---



# 36. Compare Multiple Approaches

Example:

## Contains Duplicate



### Brute force

```text
Compare every pair

Time:  O(n²)
Space: O(1)
```



### Sort

```text
Sort
Check neighbors

Time:  O(n log n)
Space: depends on sort
```



### Hash Set

```text
Track previously seen elements

Expected Time: O(n)
Space:         O(n)
```

Don't just conclude:

> Hash Set wins.

Ask:

- Is modifying input allowed?
- Is memory constrained?
- Is deterministic worst-case performance important?
- Is the input already sorted?

That's actual algorithmic reasoning.

---



# 37. One especially important mental shift

When beginners see:

```text
[4, 9, 1, 8, 2]
```

they think only about the input.

As you improve, you should ask:

> What information from everything I've already processed would make the future cheaper?

Maybe:

```text
seen values
```

Maybe:

```text
frequencies
```

Maybe:

```text
value → index
```

Maybe:

```text
prefix sum → frequency
```

That's the deeper idea behind many hash-table problems.

You're **building an auxiliary memory structure to avoid recomputation/search**.

---



# Complete Hash Tables / Hashing Checklist

```text
HASH TABLES / HASHING
│
├── Foundations
│   ├── Direct Addressing
│   ├── Key / Value
│   ├── Hash Function
│   ├── Bucket
│   ├── Hash Map
│   ├── Hash Set
│   └── Why hashing exists
│
├── Hash Functions
│   ├── Deterministic behavior
│   ├── Distribution
│   ├── Modulo mapping
│   ├── Integer hashing
│   └── String/object hashing concepts
│
├── Collisions
│   ├── Why collisions are unavoidable
│   │
│   ├── Separate Chaining
│   │   ├── Insert
│   │   ├── Search
│   │   └── Delete
│   │
│   └── Open Addressing
│       ├── Linear Probing
│       ├── Quadratic Probing concept
│       ├── Double Hashing concept
│       ├── Clustering
│       └── Tombstones
│
├── Performance
│   ├── Load Factor
│   ├── Capacity
│   ├── Resizing
│   ├── Rehashing
│   ├── Expected O(1)
│   ├── Worst-case O(n)
│   └── Amortized Analysis
│
├── Key Semantics
│   ├── Equality
│   ├── Hash contract
│   ├── Custom keys
│   └── Mutable-key danger
│
├── Core Patterns
│   ├── Frequency Counting
│   ├── Membership / Seen Set
│   ├── Complement Lookup
│   ├── Value → Index Mapping
│   ├── Grouping by Canonical Key
│   ├── Prefix Sum + Hash Map
│   ├── State Encoding
│   ├── Pair Counting
│   ├── Deduplication
│   └── Cache / Memoization
│
├── Comparisons
│   ├── Hash Table vs Array
│   ├── Hash Table vs Balanced Tree
│   ├── Hash Set vs Hash Map
│   ├── Hashing vs Sorting
│   └── Time vs Space trade-off
│
├── Implement Yourself
│   ├── Basic Hash Set
│   ├── Hash Map
│   ├── Separate Chaining
│   ├── Linear Probing
│   ├── Delete/Tombstone
│   ├── Resize
│   └── Rehash
│
└── For EVERY Problem
    ├── Determine the key
    ├── Determine stored information
    ├── Choose Set vs Map
    ├── Consider array alternative
    ├── Decide when to insert
    ├── Handle duplicates
    ├── Analyze expected complexity
    ├── Analyze worst-case complexity
    ├── Analyze space
    ├── Compare sorting alternative
    └── Explain WHY hashing helps
```

For your **core DSA pass**, that's enough hashing. You don't need to dive deeply yet into cryptographic hashes, distributed consistent hashing, Bloom filters, cuckoo hashing, perfect hashing, or industrial hash-function design.

Once you can **implement a basic hash table yourself**, explain collisions/load factor/rehashing, and recognize when a problem is really asking you to remember `existence`, `count`, `index`, or `state`, you're ready for the next major block: **Trees → Binary Trees → BST → Balanced Trees → Heaps → Tries**.