# Complexity Analysis — Complete Deep-Understanding Syllabus

Complexity analysis is the language computer scientists use to compare algorithms **independently of hardware, language, and constant factors**. Before you can reason about whether a hash table beats a sorted array, whether merge sort beats bubble sort, or whether your graph algorithm will survive at scale, you need a disciplined way to count how work grows as input size grows. This topic is Phase 1 in your DSA map because every structure and algorithm you study afterward will be judged through this lens.

---

# 1. Start with the problem Complexity Analysis solves

Suppose you need to check whether a number exists in a list of `n` elements.

### Approach A — scan every element

```text
[45, 93, 12, 71, 28, 34, ...]
 ↓   ↓   ↓   ↓   ↓   ↓
check each one
```

Worst case: inspect all `n` elements.

### Approach B — sorted array + binary search

```text
sorted array → compare middle → discard half → repeat
```

Each step eliminates half the remaining search space.

On your laptop, both feel instant for `n = 1,000`. But what about `n = 100,000,000`?

Complexity analysis answers:

> How does the amount of work grow when `n` grows?

Not:

> How fast did it run on my machine yesterday?

Understand:

- Why "it felt fast" is not enough
- Why we compare growth rates, not exact seconds
- Why input size `n` is the independent variable
- Why we care about scaling behavior

---

# 2. Why Complexity Analysis exists

Brute-force thinking says: try everything until something works.

Useful for correctness. Dangerous for scale.

Example: find duplicate in an array.

### Brute force — all pairs

```text
for i in 0..n-1
    for j in i+1..n-1
        if arr[i] == arr[j] → duplicate
```

```text
n = 5 → pairs: (0,1)(0,2)(0,3)(0,4)(1,2)(1,3)(1,4)(2,3)(2,4)(3,4)
```

Roughly `n(n-1)/2` comparisons. As `n` doubles, work roughly quadruples.

### Better — hash set

```text
for each x:
    if set contains x → duplicate
    insert x
```

One pass. Expected O(1) lookups.

```text
Brute force:  O(n²)
Hash set:     O(n) expected
```

Without complexity analysis you might optimize the wrong thing:

```text
"Rewrite the inner loop in assembly"   ← micro-optimization
"You are checking all pairs"           ← real bottleneck
```

Understand:

- Complexity compares **algorithmic ideas**, not CPU clock speed
- It reveals bottlenecks hardware cannot fix forever
- It helps choose the right data structure before coding
- It explains why some problems feel impossible at large `n`

---

# 3. The Five Main Notations

You must understand all of these — not just Big O.

## Big O — upper bound

```text
f(n) = O(g(n))
```

For large enough `n`, `f(n)` grows no faster than a constant multiple of `g(n)`.

Informally: **at most about g(n)**.

```text
3n² + 10n + 5  =  O(n²)
```

## Big Ω — lower bound

```text
f(n) = Ω(g(n))
```

`f(n)` grows at least as fast as `g(n)` (up to constant factor).

Informally: **at least about g(n)**.

```text
n² + n  =  Ω(n²)
```

## Big Θ — tight bound

```text
f(n) = Θ(g(n))   means   f(n) = O(g(n)) AND f(n) = Ω(g(n))
```

Informally: **exactly about g(n)** up to constants.

```text
5n + 20  =  Θ(n)
```

## little-o — strictly slower

```text
f(n) = o(g(n))   means   f(n)/g(n) → 0 as n → ∞
```

Example: `n = o(n²)`.

## little-ω — strictly faster

Less common in introductory DSA. Example: `n² = ω(n)`.

## Visual relationship

```text
        growth rate
            ↑
            |     O(n²)  ───────── upper bound
            |        ╱
            |  work ╱
            |      ╱
            |  Ω(n²) ───────── lower bound
            └──────────────────→ n
```

Interview shorthand often says "O(n)" when meaning "Θ(n)". Know the difference.

Understand:

- `O` = worst-case ceiling
- `Ω` = growth floor
- `Θ` = tight bound
- `o` / `ω` = strict inequality in growth

---

# 4. Growth Rates — the hierarchy

Memorize the ordering, but understand **why** each step is worse.

```text
O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(n³) < O(2^n) < O(n!)
```

```text
n = 1,000,000

O(n)       → ~1,000,000 ops          → feasible
O(n²)      → ~10¹² ops               → usually infeasible
O(2^n)     → astronomically impossible for large n
```

Visual for `n = 32`:

```text
O(1)       |█|
O(log n)   |██|
O(n)       |████████|
O(n log n) |████████████████|
O(n²)      |████████████████████████████████|
```

Understand:

- Constants matter in practice, but growth rate dominates eventually
- Logarithmic factors are extremely powerful
- Exponential/factorial growth explodes quickly
- Polynomial vs exponential is a major dividing line

---

# 5. Types of Complexity Analysis

## Time complexity

How primitive operations grow with input size: comparisons, assignments, index accesses, hash probes, pointer moves.

## Space complexity

How much **extra** memory beyond the input (auxiliary space).

```text
in-place reversal     → O(1) extra
merge sort            → O(n) extra
recursive DFS         → O(V) stack depth
```

```text
Total space ≠ Auxiliary space
```

Input storage is usually not counted as algorithm overhead unless the problem asks for total space.

## Best / Average / Worst case

Same algorithm, different inputs.

Linear search for `x`:

```text
Best:    x is first        → O(1)
Average: random position    → O(n)
Worst:   absent or last    → O(n)
```

Quicksort:

```text
Average: good pivots        → O(n log n)
Worst:   bad pivots always  → O(n²)
```

When someone says "O(n log n)", ask: worst? average? amortized?

## Amortized complexity

Some operations are expensive rarely, cheap usually.

```text
dynamic array push_back
```

Most pushes O(1). Occasionally resize + copy everything → O(n).

Over many inserts, average cost per insert is **O(1) amortized**.

```text
Average case:  over random inputs
Amortized:     over a sequence, spreading rare costly events
```

Understand:

- Time and space are separate analyses
- Best/average/worst answer different questions
- Amortized ≠ average-case (though both feel similar)

---

# 6. Core Concepts — how to count work

## Count primitive operations

Pick a consistent unit and stick with it.

```cpp
for (int i = 0; i < n; i++) {
    sum += arr[i];
}
```

```text
n iterations × constant work each → Θ(n)
```

## Identify the dominant term

```text
f(n) = 3n² + 100n + 50
```

For large `n`, `3n²` dominates → `O(n²)`.

```text
n = 1,000,000

3n²  = 3,000,000,000,000
100n = 100,000,000
50   = negligible
```

## Drop constant factors

```text
5n and n both are O(n)
```

Big O allows constant multiples. We care about growth **shape**, not exact coefficients.

Constants matter when `n` is always tiny or two algorithms share the same class.

## Nested work multiplies, sequential work adds

```text
nested:   n × n       → n²
nested:   n × log n   → n log n
sequential: O(n) + O(n²) → O(n²)   (keep max)
```

Understand:

- Multiply nested loops
- Add sequential phases, keep dominant term
- Drop constants and lower-order terms in final answer
- Don't drop lower-order terms **during** analysis — only at the end

---

# 7. Common Complexity Classes with Examples

## O(1) — constant

```text
arr[i] access, swap, stack push/pop, hash lookup average
```

```cpp
int getFirst(const vector<int>& v) { return v[0]; }
// Time O(1), Space O(1)
```

## O(log n) — logarithmic

Problem size shrinks by constant factor each step.

```text
binary search, balanced BST ops, heap push/pop
```

```text
n = 16 → 16 → 8 → 4 → 2 → 1   (4 = log₂16 steps)
```

```cpp
int binarySearch(const vector<int>& a, int target) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == target) return mid;
        if (a[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}
// Time O(log n), Space O(1)
```

## O(n) — linear

```text
find max, one-pass count, linked list traversal, BFS/DFS → O(V+E)
```

## O(n log n) — linearithmic

Divide in half repeatedly + linear work per level.

```text
merge sort, heap sort, sort-then-scan
```

```text
         n
      n/2  n/2
    ...split...
levels: log n × O(n) per level = O(n log n)
```

## O(n²) — quadratic

Nested loops over same data.

```text
bubble/selection/insertion sort, all pairs, naive duplicate check
```

```cpp
bool hasDuplicate(const vector<int>& a) {
    int n = (int)a.size();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] == a[j]) return true;
    return false;
}
// Time O(n²), Space O(1)
```

Triangle loop `j = i+1..n-1` is still O(n²): ~n(n-1)/2 iterations.

## O(n³) and higher polynomials

```text
three nested loops, naive matrix multiply, Floyd-Warshall O(V³)
```

## O(2^n) — exponential

```text
all subsets, naive recursive fibonacci, brute-force TSP
```

Each element: include or exclude → 2^n subsets. `n=40` is ~1 trillion.

## O(n!) — factorial

```text
all permutations, brute-force TSP orderings
```

`n=20` is already enormous.

Understand:

- Know a concrete example for each class
- Know which classes are feasible at interview-scale `n`
- Polynomial vs exponential is the biggest practical divide

---

# 8. Recurrence Relations — basics

Recursive algorithms are described by recurrences `T(n) = cost at size n`.

## Common forms

```text
T(n) = T(n-1) + O(1)        → O(n)        linear recursion
T(n) = T(n/2) + O(1)        → O(log n)    halving
T(n) = 2T(n/2) + O(n)       → O(n log n)  merge sort
T(n) = T(n-1) + T(n-2) + O(1) → O(2^n)    naive fibonacci
```

## Recursion tree — merge sort

```text
T(n) = 2T(n/2) + cn

Level 0:        cn
Level 1:    cn/2    cn/2         → cn total
Level 2:  cn/4 cn/4 cn/4 cn/4     → cn total
...
log n levels × cn per level = cn log n
```

## Master Theorem — intro

For `T(n) = aT(n/b) + f(n)` where `a ≥ 1, b > 1`:

Compare `f(n)` with `n^(log_b a)`.

```text
Case 1: f(n) smaller  → T(n) = Θ(n^(log_b a))      leaves dominate
Case 2: f(n) same rate → T(n) = Θ(n^(log_b a) log n) balanced
Case 3: f(n) larger   → T(n) = Θ(f(n))              root dominates
```

Examples:

```text
T(n) = 2T(n/2) + n   → Θ(n log n)    (Case 2)
T(n) = T(n/2) + n    → Θ(n)          (Case 3)
T(n) = 4T(n/2) + n   → Θ(n²)         (Case 1)
```

Don't treat it as magic. Draw the tree to verify.

Understand:

- Recurrences describe recursive algorithms
- Halving → log depth
- Multiple branches multiply work per level
- Memoization collapses exponential → polynomial

---

# 9. Amortized Analysis — deep dive

## Dynamic array resizing

```text
capacity: 1 → 2 → 4 → 8 → ...
```

Single worst push: O(n) when resize copies everything.

Total copying over n pushes: `1 + 2 + 4 + ... + n/2 < 2n`

```text
Amortized cost per push: O(1)
```

```text
pushes: |█|█|█|████|█|████████|
         ↑ occasional expensive resize spread over many cheap pushes
```

## Hash table resizing

When load factor exceeds threshold → bigger table + rehash all keys.

Single insert may cost O(n), but doubling strategy gives **O(1) amortized** per insert.

```text
worst single op:     O(n)
expected per op:     O(1)
amortized over seq:  O(1)
```

These are different statements.

## Aggregate method

If `n` operations cost total O(n), amortized per op is O(1).

Understand:

- One operation can exceed the amortized bound
- Doubling strategy is the classic proof pattern
- Amortized ≠ average-case formally

---

# 10. Space Complexity Patterns

## O(1) auxiliary — in-place

```text
two pointers, iterative binary search, iterative list reversal
```

## O(n) auxiliary

```text
frequency array, merge sort temp, BFS queue, hash map
```

## O(depth) — recursion stack

```text
balanced tree DFS:  O(log n)
skewed tree DFS:    O(n)
```

## Graph storage

```text
adjacency list:   O(V + E)
adjacency matrix: O(V²)
```

## Output space

```text
return all subsets → output is 2^n, can't beat that
return sorted copy → O(n)
```

Distinguish auxiliary working space from required output space.

## Time-space trade-off

```text
memoization, hash maps, prefix arrays
```

More space often buys less time.

---

# 11. The Most Important Complexity Analysis Patterns

## Pattern 1 — Single Loop Over Input

```text
for i = 0 to n-1: constant work
→ O(n)
```

Examples: linear search, sum, one-pass counting.

---

## Pattern 2 — Nested Loops on Same Input

```text
for i: for j: constant work
→ O(n²)
```

Watch hidden inner loops and triangle variants (`j = i+1..n-1`).

---

## Pattern 3 — Halving Loop

```text
while n > 1: n = n / 2
→ O(log n)
```

Examples: binary search, repeated halving.

---

## Pattern 4 — Outer Linear × Inner Logarithmic

```text
for i in n: inner doubles j until n
→ O(n log n)
```

Also: n items × binary search each, sort-then-scan.

---

## Pattern 5 — Recursion Depth × Work Per Level

```text
linear recursion:     depth n × O(1)      → O(n)
merge sort:           depth log n × O(n)  → O(n log n)
```

Draw the recursion tree when unsure.

---

## Pattern 6 — Branching Recursion (Exponential)

```text
T(n) = T(n-1) + T(n-2) + O(1)   → O(2^n) without memo
```

```text
                    fib(5)
                 /          \
            fib(4)            fib(3)
           /     \           /    \
       fib(3) fib(2)     fib(2) fib(1)
```

Memoization → O(n) time, O(n) space.

---

## Pattern 7 — Sorting as Subroutine

```text
O(n log n) sort + O(n) scan = O(n log n)
```

Don't forget sorting cost in total analysis.

---

## Pattern 8 — Hash Map / Set Linear Scan

```text
for each element: O(1) avg hash op
→ O(n) time avg, O(n) space
```

Worst case with collisions: O(n²).

---

## Pattern 9 — Two Pointers / Sliding Window

Pointers move forward only — each moves at most n times total.

```text
while left < right: move one pointer
→ O(n) total movement, NOT O(n²)
```

Analyze total pointer movement, not syntax.

---

## Pattern 10 — BFS / DFS on Graph

```text
visit each vertex once, each edge constant times
→ O(V + E) time, O(V) space
```

Dense graph E ≈ V² → O(V²).

---

## Pattern 11 — Dynamic Programming

```text
states × work per transition
```

0/1 knapsack: n items × capacity W → O(nW) (pseudo-polynomial).

---

## Pattern 12 — Binary Search on Answer

Monotonic predicate over answer range + O(n) check per step.

```text
→ O(n log A)   where A = answer range size
```

Understand:

- Loop shape suggests complexity
- Hidden loops and pointer movement must be counted
- Subroutine costs must be included
- DS operations carry their own bounds

---

# 12. Pattern Recognition — signal → complexity

```text
"Single scan, constant per element"     → O(n)
"Check all pairs"                       → O(n²)
"Repeatedly discard half"               → O(log n)
"Divide in half + linear merge"         → O(n log n)
"Sort input first"                      → likely O(n log n) total
"Try all subsets"                       → O(2^n)
"Try all permutations"                  → O(n!)
"Branching factor b, depth d"           → often O(b^d)
"Graph visit each node/edge once"       → O(V + E)
"Pointers only move forward"            → maybe O(n), not O(n²)
"Memoize overlapping subproblems"       → exponential → polynomial
"Hash map membership"                   → O(n) avg time, O(n) space
"Balanced BST operations"               → O(log n) per op
```

Key distinction:

```text
unsorted array search:  O(n)
sorted + binary search: O(log n)
hash set average:       O(1)
```

---

# 13. Edge Cases and Common Mistakes

## Confusing O(n) with O(n²)

One loop → O(n). Two nested loops → O(n²). Triangle loop still O(n²).

## Ignoring hidden costs

```cpp
for (int i = 0; i < n; i++)
    result += to_string(arr[i]);  // may not be O(1) per call
```

String concatenation in a loop can be O(n²) total in some languages.

## Log base doesn't matter

```text
log₂ n, log₁₀ n, ln n — differ by constant factor only
O(log₂ n) = O(log₁₀ n)
```

## Average vs worst vs amortized

Hash table: avg O(1), worst O(n). Quicksort: avg O(n log n), worst O(n²).

## Forgetting recursion stack space

DFS on graph: O(V+E) time but O(V) stack in worst case.

## Counting input as auxiliary space

Array of size n given as input — usually not "extra" unless problem asks total space.

## Assuming two pointers is always O(n)

Only true if each pointer advances bounded total times. Analyze movement.

## Dropping lower-order terms too early

```text
n log n + n = O(n log n), NOT O(n)
```

## Misapplying Master Theorem

Not every recurrence fits cleanly. Draw the tree if unsure.

## Claiming O(1) for non-constant ops

```text
array insert middle: O(n)
unsorted search:     O(n)
singly linked delete without predecessor: O(n)
```

Know data structure operation costs first.

---

# 14. Implementation Skill — analyze any algorithm step by step

## The 8-step process

```text
1. Define n (and m, V, E, W, etc.)
2. Choose operation to count (comparisons, probes, etc.)
3. Break algorithm into phases
4. Bound each loop/recursion
5. Combine: multiply nested, add sequential (keep max)
6. Simplify to standard class (drop constants, lower terms)
7. Analyze space separately (vars, stack, aux structures, output)
8. State case type (worst / average / amortized)
```

## Worked example — find duplicate (hash set)

```cpp
bool hasDuplicate(const vector<int>& nums) {
    unordered_set<int> seen;
    for (int x : nums) {
        if (seen.count(x)) return true;
        seen.insert(x);
    }
    return false;
}
```

```text
n = nums.size()
Loop: n iterations
Inside: hash count O(1) avg, hash insert O(1) avg

Time:  O(n) average, O(n²) worst (pathological collisions)
Space: O(n)
```

## Worked example — merge sort

```text
T(n) = 2T(n/2) + O(n)
Tree: log n levels, O(n) per level

Time:  O(n log n)
Space: O(n) auxiliary
```

Practice this on every algorithm you implement.

---

# 15. Problems to Practice

Choose problems representing each reasoning mode.

### Fundamentals — counting loops

- Analyze simple for/while loops
- Compare `i++` vs `i *= 2` loop bounds
- Triangle nested loop complexity
- Sequential phases with different costs

### Logarithmic reasoning

- Binary search variants
- Find peak element by halving
- Integer square root by binary search
- Exponentiation by squaring

### Divide and conquer

- Merge sort / quicksort analysis
- Count inversions
- Draw tree for `T(n) = 3T(n/2) + n`

### Recurrences

- Fibonacci naive vs memoized
- Tower of Hanoi
- Recursive binary search

### Amortized analysis

- Dynamic array push_back sequence
- Hash table insert with resizing

### Space complexity

- DFS stack on skewed vs balanced tree
- In-place vs extra-array reversal
- BFS queue max size
- Memoized DP table size

### Integrated (analyze, don't just solve)

- Two Sum — brute vs hash map
- Contains Duplicate — three approaches
- Search rotated sorted array — O(log n)
- Kth largest — heap vs quickselect
- Number of islands — O(mn) BFS/DFS

---

# 16. For EVERY Complexity Analysis

**1. What is the input size variable?**

```text
n? m? V and E? rows × cols?
```

**2. What exact operation am I counting?**

**3. Is there a hidden loop?** (library calls, sort inside loop, string build, rehash)

**4. Are loops nested or sequential?** (multiply vs max)

**5. Does recursion create a tree?** (depth? branching? overlap?)

**6. What data structure operations are used?**

```text
array index O(1) | vector insert mid O(n) | hash avg O(1) | BST O(log n)
```

**7. Time — which case?** (best / average / worst / amortized)

**8. Auxiliary space?** (variables, stack, maps, arrays)

**9. Does output size dominate?** (all subsets → can't beat 2^n)

**10. Is preprocessing included?** (prefix O(n) + query O(1); sort O(n log n) + scan O(n))

**11. Tighter than O?** (can you prove Ω and Θ?)

**12. vs brute force — what work was eliminated?**

Question 12 is crucial. Complexity is not a label — it explains **what work you stopped doing**.

---

# 17. Compare Multiple Approaches

## Search for target

### Unsorted array — linear scan

```text
Time: O(n)  Space: O(1)
```

### Sorted array — binary search

```text
Time: O(log n)  Space: O(1)
```

### Hash set preprocessing

```text
Build: O(n)  Query: O(1) avg  Space: O(n)
```

Don't just conclude "hash set wins." Ask:

```text
Static or streaming data?
How many queries q?
Memory limited?
Worst-case guarantee needed?
Already sorted?
Preprocessing allowed?
```

For `q` queries on static data:

```text
sort once:    O(n log n) + q log n
hash set:     O(n) + q
```

Large `q` → preprocessing pays off.

---

## Contains Duplicate

### Brute force pairs

```text
Time: O(n²)  Space: O(1)
```

### Sort neighbors

```text
Time: O(n log n)  Space: O(1) or O(n) per sort impl
```

### Hash set

```text
Expected Time: O(n)  Space: O(n)
```

```text
                Time          Space         Notes
Brute force     O(n²)         O(1)          tiny n only
Sort            O(n log n)    varies        may mutate input
Hash set        O(n) avg      O(n)          fastest expected
```

That's actual algorithmic reasoning.

---

# 18. One especially important mental shift

Beginners think: what does this code do line by line?

Improve by asking: **what is the shape of work as n grows?**

```text
one loop              → O(n)
loop inside loop      → O(n²)
recursion tree        → depth × branching
preprocess + queries  → pay once, query fast
amortized events      → rare spikes spread out
```

The goal is not to parrot `O(n log n)`. The goal is to see:

```text
"I eliminated a factor of n"
"I pay n log n once to avoid n² many times"
"I trade O(n) space to remove an inner loop"
```

You're learning to predict scalability before you benchmark.

---

# Your Complete Complexity Analysis Checklist

```text
COMPLEXITY ANALYSIS
│
├── Foundations
│   ├── Why complexity analysis exists
│   ├── Input size n
│   ├── Growth rate intuition
│   ├── Hardware-independent reasoning
│   └── Brute force vs improved approach
│
├── Notation
│   ├── Big O (upper bound)
│   ├── Big Ω (lower bound)
│   ├── Big Θ (tight bound)
│   ├── little-o / little-ω
│   └── O vs Θ interview shorthand
│
├── Complexity Types
│   ├── Time / Space
│   ├── Auxiliary vs total space
│   ├── Best / Average / Worst case
│   └── Amortized analysis
│
├── Core Counting
│   ├── Count primitive operations
│   ├── Nested multiply, sequential add
│   ├── Dominant term
│   ├── Drop constants & lower terms
│   └── Hidden/library costs
│
├── Standard Classes
│   ├── O(1), O(log n), O(n), O(n log n)
│   ├── O(n²), polynomials
│   └── O(2^n), O(n!)
│
├── Recurrence / Recursion
│   ├── Write recurrence from code
│   ├── Recursion tree
│   ├── Master theorem (intro)
│   └── Memoization effect
│
├── Amortized
│   ├── Dynamic array doubling
│   ├── Hash table resize/rehash
│   └── Amortized vs average vs worst
│
├── Space Patterns
│   ├── O(1) in-place
│   ├── O(n) auxiliary
│   ├── Recursion stack O(depth)
│   ├── Graph O(V+E) vs O(V²)
│   └── Output size / time-space trade-off
│
├── Core Patterns (12)
│   ├── Single loop O(n)
│   ├── Nested loops O(n²)
│   ├── Halving O(log n)
│   ├── Linear × log O(n log n)
│   ├── Recursion depth × work
│   ├── Branching → exponential
│   ├── Sort + scan
│   ├── Hash map scan
│   ├── Two pointers forward
│   ├── BFS/DFS O(V+E)
│   ├── DP states × transitions
│   └── Binary search on answer
│
├── Pattern Recognition
│   ├── All pairs → O(n²)
│   ├── Discard half → O(log n)
│   ├── All subsets → O(2^n)
│   ├── Graph traversal → O(V+E)
│   └── Memoization collapses overlap
│
├── Common Mistakes
│   ├── Hidden quadratic loops
│   ├── Ignoring preprocess cost
│   ├── Log base irrelevance
│   ├── Average vs worst confusion
│   ├── Forgetting recursion stack
│   └── Wrong DS operation costs
│
├── Analysis Workflow (8 steps)
│
├── Compare Approaches
│   ├── Brute force baseline
│   ├── Time / space trade-off
│   └── Preprocessing worth it?
│
└── For EVERY Analysis
    ├── Define n
    ├── Count what?
    ├── Hidden loops?
    ├── Nested or sequential?
    ├── Recursion tree?
    ├── DS op costs?
    ├── Which case?
    ├── Auxiliary space?
    ├── Output dominates?
    ├── Preprocessing included?
    ├── Tight Θ?
    └── WHAT work eliminated and WHY?
```

Once you can **look at an algorithm, count its work, state time and space in the right case, compare it to brute force, and explain what structural change improved the bound**, you're ready for **Arrays & Strings** — where memory layout and iteration patterns meet the complexity lens you've built.
