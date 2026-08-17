# Advanced Data Structures — Complete Deep-Understanding Syllabus

Advanced data structures are what you reach for when **arrays, hash maps, heaps, and basic trees are no longer enough** for the query/update pattern the problem demands.

After Arrays, Linked Lists, Hash Tables, Trees, Heaps, Graphs, and Dynamic Programming foundations, Phase 12 introduces structures that answer questions like:

> "I need range queries and point/range updates on a static or dynamic array — fast."

> "I need min/max over a sliding or arbitrary range — repeatedly."

> "I need to merge groups and ask whether two elements are connected — millions of times."

> "I need order statistics or predecessor/successor on a dynamic set."

These are not interview gimmicks. They appear in competitive programming, systems design (interval indexing, union of resources), databases (index structures), and anywhere **query workloads have structure**.

---

# 1. Start with the problem Advanced DS solve

Suppose you have an array:

```text
[5, 2, 8, 1, 9, 3, 7, 4]
```

And you must answer **many** queries of the form:

```text
What is the sum of elements from index 2 to 6?
```

Brute force:

```text
for i in [l, r]:
    ans += arr[i]
```

Per query:

```text
O(n)
```

If there are `q` queries:

```text
O(n × q)
```

That breaks when `n, q` are large (e.g. 10⁵ each).

### Prefix sums help — but only partially

Build:

```text
prefix[i] = sum(arr[0..i-1])
```

Range sum `[l, r]`:

```text
prefix[r+1] - prefix[l]
```

Build: `O(n)`
Query: `O(1)`
Update one element: `O(n)` — must rebuild or shift prefix

Prefix sums are perfect when the array is **static** (no updates).

But many problems require:

```text
Update arr[i]
Query sum/min/max on [l, r]
```

Both must be fast.

That's where **Segment Trees, Fenwick Trees, Sparse Tables, Sqrt Decomposition**, and related structures enter.

---

# 2. The Core Question for Every Problem

Before implementing anything, ask:

```text
What operation?
  ├── Point update / Range update
  ├── Range query (sum, min, max, gcd, xor, count, ...)
  ├── Static vs Dynamic
  └── Offline vs Online

How many queries?
  ├── Few → brute force or sort + sweep may suffice
  └── Many → need preprocessing structure

Can I sort / reorder?
  ├── Yes → offline tricks, coordinate compression, Mo's algorithm
  └── No → online structure required

Is the array static?
  ├── Yes → Sparse Table, prefix sums, sometimes segment tree over static
  └── No → Fenwick, Segment Tree, Sqrt Decomposition, balanced BST

What is the monoid / associative operation?
  ├── Sum, min, max, gcd, xor → segment tree / BIT / sparse table
  └── Non-invertible → harder; may need different structure
```

Memorizing implementations without this decision tree leads to using a Segment Tree where a Fenwick Tree would be simpler, or a Sparse Table where updates are required.

---

# 3. Complexity Landscape — Know This Table Cold

Assume array size `n`, number of queries/updates `q`.

```text
Structure              Build          Query           Update          Notes
─────────────────────────────────────────────────────────────────────────────────
Brute force            O(1)           O(n)            O(1)            Baseline
Prefix sum             O(n)           O(1)            O(n)            Static only
Sparse Table (RMQ)     O(n log n)     O(1)            ✗               Static min/max
Fenwick Tree (BIT)     O(n)           O(log n)        O(log n)        Sum; some extensions
Segment Tree           O(n)           O(log n)        O(log n)        Flexible monoid
Lazy Segment Tree      O(n)           O(log n)        O(log n)        Range updates
Sqrt Decomposition     O(n)           O(√n)           O(√n)           Simple, versatile
DSU (Union-Find)       O(n)           O(α(n)) find    O(α(n)) union   Connectivity
Treap / Splay          O(n) shuffled  O(log n)        O(log n)        Dynamic order
PBDS ordered set       O(n log n)     O(log n)          O(log n)        C++ only; order stats
```

`α(n)` = inverse Ackermann — effectively constant for any realistic `n`.

When `q` is ~10⁵ and you need `O(log n)` per operation, total ~10⁵ × 17 ≈ 1.7×10⁶ — fine.

When you use `O(n)` per query: 10⁵ × 10⁵ = 10¹⁰ — not fine.

---

# 4. Segment Tree — Fundamentals

A segment tree is a **binary tree over array intervals**.

Each node represents a segment `[L, R]` and stores an aggregate of that segment (sum, min, max, gcd, etc.).

## Visual intuition

Array:

```text
Index:  0   1   2   3   4   5
Value: [1,  3,  5,  7,  9, 11]
```

Segment tree (sum):

```text
                    [0,5] = 36
                   /          \
            [0,2]=9            [3,5]=27
           /      \            /      \
      [0,1]=4  [2,2]=5   [3,4]=16  [5,5]=11
      /    \
  [0,0]=1 [1,1]=3
```

Leaf nodes = single array elements.
Internal nodes = combine children.

## Why it works

The array is partitioned hierarchically:

```text
[0, n-1]
  ├── [0, mid]
  └── [mid+1, n-1]
```

Any query range `[l, r]` decomposes into **O(log n)** canonical nodes — disjoint segments that exactly cover `[l, r]` without overlap.

That is the segment tree's superpower: **logarithmic range queries on a dynamic array** for any associative, invertible-or-splittable operation.

## Node representation

```cpp
struct SegTree {
    int n;
    vector<long long> tree;  // 4*n is safe for 1-indexed implicit tree

    SegTree(const vector<int>& a) {
        n = (int)a.size();
        tree.assign(4 * n, 0);
        build(a, 1, 0, n - 1);
    }

    void build(const vector<int>& a, int node, int L, int R) {
        if (L == R) {
            tree[node] = a[L];
            return;
        }
        int mid = (L + R) / 2;
        build(a, node * 2, L, mid);
        build(a, node * 2 + 1, mid + 1, R);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];  // combine
    }
};
```

Common indexing: **1-based node index** in array representation; children at `2*node` and `2*node+1`.

## Combine function

The combine operation must match your query:

```text
Sum:     tree[node] = left + right
Min:     tree[node] = min(left, right)
Max:     tree[node] = max(left, right)
GCD:     tree[node] = gcd(left, right)
XOR:     tree[node] = left ^ right
```

It must be **associative**. Commutativity helps intuition but isn't always required.

---

# 5. Segment Tree — Range Query

Query sum on `[ql, qr]`:

```cpp
long long query(int node, int L, int R, int ql, int qr) {
    if (qr < L || R < ql) return 0;           // no overlap
    if (ql <= L && R <= qr) return tree[node]; // full cover
    int mid = (L + R) / 2;
    return query(node * 2, L, mid, ql, qr)
         + query(node * 2 + 1, mid + 1, R, ql, qr);
}
```

Three cases:

```text
1. No overlap     → return identity (0 for sum, INF for min)
2. Full cover     → return node aggregate
3. Partial overlap → recurse both children, combine
```

Complexity: `O(log n)` — at most two nodes per tree level.

## Walk-through

Array `[1, 3, 5, 7, 9, 11]`, query sum `[1, 4]`:

```text
Start at [0,5]
  Partial → go to [0,2] and [3,5]

[0,2]: partial → [0,1] and [2,2]
  [0,1]: partial → [0,0] skip, [1,1] full → 3
  [2,2]: full → 5

[3,5]: partial → [3,4] and [5,5]
  [3,4]: full → 16
  [5,5]: outside qr → skip

Answer: 3 + 5 + 16 = 24
```

---

# 6. Segment Tree — Point Update

Update `arr[pos] = val`:

```cpp
void update(int node, int L, int R, int pos, long long val) {
    if (L == R) {
        tree[node] = val;
        return;
    }
    int mid = (L + R) / 2;
    if (pos <= mid)
        update(node * 2, L, mid, pos, val);
    else
        update(node * 2 + 1, mid + 1, R, pos, val);
    tree[node] = tree[node * 2] + tree[node * 2 + 1];
}
```

Walk down to the leaf, update, recompute ancestors on the way back.

Complexity: `O(log n)`.

## Iterative variant (often faster)

Some prefer iterative segment trees with size = power of 2:

```text
Build leaves at indices [n..2n-1]
Internal nodes at [1..n-1]
```

Update: set leaf, propagate up.

Query: standard iterative range query with two pointers.

Learn the recursive version first — it teaches the interval logic. Switch to iterative for speed/contest templates.

---

# 7. Segment Tree — Variants Beyond Sum

## Range Minimum Query (RMQ)

```cpp
tree[node] = min(tree[node*2], tree[node*2+1]);
// identity for no-overlap: INF
```

## Range Maximum Query

Same with `max` and identity `-INF`.

## Range GCD

```cpp
tree[node] = std::gcd(tree[node*2], tree[node*2+1]);
// identity: 0 (gcd(x,0)=x)
```

## Count / Frequency segment tree

Leaves store counts; combine = sum of counts.
Useful for "how many elements in [l,r] are ≤ x" with coordinate compression + offline or merge-sort tree.

## Segment tree on other domains

Not only arrays:

```text
Segment tree on indices with values in nodes
Segment tree on time axis for sweep-line
Segment tree of vectors (merge sort tree)
2D segment tree (rare; often too slow)
```

The **pattern** is always: partition domain, store aggregate, combine, recurse.

---

# 8. Lazy Propagation — Range Updates

Point update is easy. What about:

```text
Add v to every element in [l, r]
```

Naive segment-tree approach: call point update `r-l+1` times → `O(n log n)` worst case.

**Lazy propagation** defers work.

## The lazy idea

Each node stores:

```text
tree[node]  = aggregate for segment (correct if lazy is pushed)
lazy[node]  = pending update to apply to entire segment
```

When you must update range `[ql, qr]`:

```text
If node segment fully inside [ql,qr]:
    Apply update to tree[node]
    Record in lazy[node] (merge pending lazy)
    Return — don't recurse yet

If partial overlap:
    push(node)  — propagate lazy to children first
    recurse
    pull(node)  — recompute tree[node] from children
```

## Range add + range sum example

```cpp
struct LazySeg {
    int n;
    vector<long long> tree, lazy;

    void push(int node, int L, int R) {
        if (lazy[node] == 0) return;
        int len = R - L + 1;
        tree[node] += lazy[node] * len;
        if (L != R) {
            lazy[node * 2]     += lazy[node];
            lazy[node * 2 + 1] += lazy[node];
        }
        lazy[node] = 0;
    }

    void rangeAdd(int node, int L, int R, int ql, int qr, long long v) {
        push(node, L, R);
        if (qr < L || R < ql) return;
        if (ql <= L && R <= qr) {
            lazy[node] += v;
            push(node, L, R);
            return;
        }
        int mid = (L + R) / 2;
        rangeAdd(node * 2, L, mid, ql, qr, v);
        rangeAdd(node * 2 + 1, mid + 1, R, ql, qr, v);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }
};
```

## Lazy propagation invariants

Understand these cold:

```text
1. Before querying or updating a node, push its lazy tag to children
2. lazy[node] means "this entire segment still needs this pending update"
3. When combining lazy tags, order matters for non-commutative ops
4. Full cover → update node + set lazy; don't descend unnecessarily
5. After children change, recompute parent aggregate (pull)
```

## Common lazy operations

```text
Range add + range sum          → lazy += delta
Range set + range sum          → lazy assign; careful with "set" overriding "add"
Range add + range min          → more complex; need max+second max or different design
Range affine (ax+b) + sum      → lazy (mul, add) pairs
```

**Range set** is trickier: a new set overwrites pending add. Define semantics clearly.

## When lazy propagation is needed

```text
"Add v to subarray"                    → lazy add
"Set all in subarray to v"             → lazy assign
"Multiply range by v"                  → lazy mul
Flip bits in range                     → lazy xor / toggle
```

If updates are **only point updates**, lazy is unnecessary.

---

# 9. Fenwick Tree (Binary Indexed Tree — BIT)

The Fenwick Tree is a **compact, elegant** structure for prefix-based aggregates, especially **sum**.

## Why it exists

Segment tree uses ~`4n` nodes and recursive logic.

Fenwick tree uses **`n` elements** and simple bit tricks.

For pure **point update + prefix sum / range sum**, BIT is often:

```text
Less code
Less memory
Same O(log n) complexity
```

## Core idea

Maintain array `bit[1..n]` (1-indexed) such that:

```text
bit[i] covers a certain range of responsibility
```

Responsibility pattern uses **lowest set bit** (`i & -i`):

```text
i = 12 (1100)
i & -i = 4 (0100)

bit[12] covers range of length 4 ending at 12
```

Visual (n=8):

```text
bit[1] covers [1,1]
bit[2] covers [1,2]
bit[3] covers [3,3]
bit[4] covers [1,4]
bit[5] covers [5,5]
bit[6] covers [5,6]
bit[7] covers [7,7]
bit[8] covers [1,8]
```

## Implementation

```cpp
struct Fenwick {
    int n;
    vector<long long> bit;

    Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void add(int i, long long delta) {  // 1-indexed i
        for (; i <= n; i += i & -i)
            bit[i] += delta;
    }

    long long prefix(int i) {  // sum of [1..i]
        long long s = 0;
        for (; i > 0; i -= i & -i)
            s += bit[i];
        return s;
    }

    long long rangeSum(int l, int r) {  // [l,r] inclusive, 1-indexed
        return prefix(r) - prefix(l - 1);
    }
};
```

Build from array:

```cpp
for (int i = 1; i <= n; i++)
    add(i, a[i-1]);
```

Or O(n) build with clever loop — learn after basic version.

## Point update

Change `arr[i]` by `delta`:

```text
add(i, delta)
```

Set `arr[i] = v` if you know old value:

```text
add(i, v - old)
```

## Complexity

```text
Update:  O(log n)
Query:   O(log n) for prefix; O(log n) for range sum
Space:   O(n)
```

## BIT vs Segment Tree

```text
Use BIT when:
  ├── Prefix sum / range sum
  ├── Point update
  ├── Inversion count (with coordinate compression)
  ├── Simple frequency on compressed coords
  └── You want minimal code

Use Segment Tree when:
  ├── Range min / max / gcd / custom monoid
  ├── Range update + range query (lazy)
  ├── Non-invertible operations needing more than prefix trick
  └── Combining multiple different queries on same array
```

## BIT extensions (know they exist)

```text
2D BIT                    → point update, rectangle sum
BIT with range add        → difference trick on BIT
BIT for max               → possible but awkward; segment tree easier
Multiple BITs             → range add + range sum using two BITs
```

### Range add, range sum with two BITs

Trick: maintain BIT of values and BIT of "correction" so prefix of correction encodes range adds.

Learn when you encounter that problem class — don't memorize prematurely.

---

# 10. Sparse Table — Static RMQ in O(1)

Sparse Table answers **range min/max (or any idempotent operation)** on a **static array** with:

```text
Build:  O(n log n)
Query:  O(1)
Update: not supported (rebuild)
```

## When to use

```text
Array never changes (or changes very rarely)
Need many min/max/gcd queries on arbitrary ranges
n, q up to ~10⁶ — O(1) query matters
```

## Core idea — overlapping ranges

For RMQ with `min`, the operation is **idempotent**:

```text
min(min(a,b), b) works with overlapping intervals
```

Precompute `st[j][i]` = min of subarray starting at `i` with length `2^j`:

```text
st[0][i] = arr[i]
st[j][i] = min(st[j-1][i], st[j-1][i + 2^(j-1)])
```

Query `[l, r]`:

```text
k = floor(log2(r - l + 1))
ans = min(st[k][l], st[k][r - 2^k + 1])
```

Two overlapping blocks of length `2^k` cover `[l, r]`.

## Implementation

```cpp
struct SparseTable {
    int n, LOG;
    vector<vector<int>> st;

    SparseTable(const vector<int>& a) {
        n = (int)a.size();
        LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        st.assign(LOG, vector<int>(n));
        for (int i = 0; i < n; i++) st[0][i] = a[i];
        for (int j = 1; j < LOG; j++)
            for (int i = 0; i + (1 << j) <= n; i++)
                st[j][i] = min(st[j][i], st[j][i + (1 << (j - 1))]);
    }

    int query(int l, int r) {  // min on [l,r], 0-indexed
        int k = 31 - __builtin_clz(r - l + 1);
        return min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
```

## Works for

```text
min, max          → idempotent
gcd               → idempotent
bitwise AND/OR    → idempotent if overlapping merge is valid
```

## Does NOT work for

```text
sum               → overlapping double-counts
product           → same
```

For sum on static array → prefix sums.

## Sparse Table vs Segment Tree for RMQ

```text
Static, many queries:     Sparse Table wins (O(1) query)
Dynamic updates:          Segment Tree
Memory:                   Sparse Table O(n log n) vs SegTree O(n)
```

---

# 11. Disjoint Set Union (DSU / Union-Find)

> **Note:** Full DSU treatment lives in `graphs/notes.md` (Phase 11). This section covers what you need in the advanced-structures context and applications that pair with range structures.

DSU maintains a partition of elements into **disjoint sets** with:

```text
find(x)         → representative of x's set
union(a, b)     → merge sets containing a and b
connected?      → find(a) == find(b)
```

Amortized: nearly `O(1)` per operation with path compression + union by rank/size.

## Implementation (review)

```cpp
struct DSU {
    vector<int> parent, rank_;

    DSU(int n) : parent(n), rank_(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);  // path compression
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank_[a] < rank_[b]) swap(a, b);
        parent[b] = a;
        if (rank_[a] == rank_[b]) rank_[a]++;
        return true;
    }
};
```

## Why it's "advanced structures"

DSU isn't a range query structure — it's a **dynamic connectivity** structure.

It belongs here because problems often combine:

```text
DSU + sorting edges (Kruskal)
DSU + offline queries (process removals in reverse)
DSU + grid / percolation
DSU on tree (path compression on tree structure)
DSU with extra state (component size, parity, timestamps)
```

## Advanced DSU patterns

### Rollback DSU

Undo last `k` unions — stack of changes. Useful in divide-and-conquer offline queries.

### DSU with component size

```cpp
vector<int> sz;
// unite: sz[rootA] += sz[rootB]
```

### DSU on tree / parent array

When graph is a forest with known parent pointers — specialized union.

### Offline connectivity

Queries: "Is u connected to v at time t?"

Process edges **backwards** if edges are only deleted; or use rollback DSU with segment tree over time.

## DSU vs BFS/DFS for connectivity

```text
Few queries, build graph once     → DFS/BFS
Many union/find online            → DSU
Undirected dynamic connectivity   → DSU (unless deletions — harder)
```

---

# 12. Square Root Decomposition (Sqrt Decomposition)

Sqrt decomposition splits an array into **blocks of size ~√n**.

```text
n = 9, block size B = 3

Blocks:
[0,1,2]  [3,4,5]  [6,7,8]

blockAgg[0] = combine(arr[0..2])
blockAgg[1] = combine(arr[3..5])
blockAgg[2] = combine(arr[6..8])
```

## Operations

```text
Point update:     O(1) arr + O(1) rebuild block aggregate
Range query:      O(√n) — partial blocks at ends + full blocks in middle
Range update:     O(√n) with lazy tags per block (optional)
```

## When to use

```text
Problem is "almost" brute force but needs slight speedup
Range queries with unusual combine that doesn't fit seg tree easily
Mo's algorithm uses block ordering (√n bucket sort on queries)
You want simpler code than segment tree for contests
n ≤ 10⁴ sometimes passes with √n per query
```

## Template sketch — range sum

```cpp
int n, B;
vector<long long> arr, block;
vector<long long> lazy;  // optional

void rebuild(int b) {
    long long s = 0;
    for (int i = b * B; i < min(n, (b + 1) * B); i++)
        s += arr[i];
    block[b] = s;
}

long long query(int l, int r) {
    long long ans = 0;
    while (l <= r && l % B != 0) ans += arr[l++];
    while (l + B - 1 <= r) {
        ans += block[l / B];
        l += B;
    }
    while (l <= r) ans += arr[l++];
    return ans;
}
```

## Sqrt decomposition variants

```text
Static RMQ per block + preprocessing → O(√n) query
Heavy-light style block updates
Sqrt on queries (Mo's algorithm) for offline range frequency
```

## Sqrt vs Segment Tree

```text
Sqrt: simpler, O(√n) — good when constraints are loose (n,q ≤ 3×10⁴)
SegTree: O(log n) — needed for larger constraints
```

---

# 13. Treap and Splay Tree — Conceptual Introduction

These are **randomized or self-adjusting binary search trees** that guarantee `O(log n)` expected (Treap) or amortized (Splay) operations without explicit red-black rules.

You rarely implement from scratch in interviews unless asked — but you should understand **what problem they solve**.

## The problem

Standard BST degrades to a chain if insertions are sorted:

```text
1 → 2 → 3 → 4 → 5   (O(n) per op)
```

Balanced trees (AVL, Red-Black) fix this with rotations and balance invariants.

**Treap** and **Splay** are alternative balancing philosophies.

## Treap = BST + Heap on random priority

Each node has:

```text
key   (BST order)
prio  (random heap priority)
```

Invariants:

```text
In-order traversal → sorted keys
Heap property on prio → min-heap or max-heap
```

Insert: BST insert by key, then rotate up until heap property restored.

With random priorities, expected height = `O(log n)`.

## Splay Tree

After every access (find/insert), **splay** the node to root via rotations.

Recently accessed nodes stay near root.

Amortized `O(log n)` per operation.

Used in some competitive programming templates for **implicit treaps** (array as tree by index).

## Implicit Treap (sequence operations)

Nodes keyed by **index in sequence**, not value.

Supports:

```text
split(t, k)           → two treaps: first k elements | rest
merge(a, b)           → concatenate
insert/delete at pos
reverse range         → lazy flag
```

This is how you implement a **dynamic array** with cut/paste/reverse in `O(log n)`.

## When Treap/Splay matter

```text
Need order statistics on dynamic set     → Treap / PBDS
Need sequence with range reverse/rotate  → Implicit treap
Implement rope / text editor problems    → Implicit treap
Interview asks "design dynamic order"    → conceptual + maybe multiset
```

For most interview problems, `std::set` / `multiset` or PBDS suffices in C++.

---

# 14. Ordered Set and Policy-Based Data Structures (PBDS)

C++ `std::set` is a balanced BST (usually red-black). It supports:

```text
insert, erase, find     O(log n)
in-order traversal      O(n)
lower_bound, upper_bound
```

It does **not** support:

```text
order_of_key(k)   → how many elements strictly less than k
find_by_order(k)  → k-th smallest (0-indexed)
```

**GNU PBDS** (`#include <ext/pb_ds/assoc_container.hpp>`) adds these.

## Example

```cpp
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template<typename T>
using ordered_set = tree<
    T, null_type, less<T>, rb_tree_tag,
    tree_order_statistics_node_update>;

ordered_set<int> os;
os.insert(5);
os.insert(2);
os.insert(8);
os.order_of_key(5);   // 1 (elements < 5: {2})
os.find_by_order(1);  // 5 (1-st smallest)
```

## When to use ordered_set

```text
Dynamic k-th smallest / order statistics
Count elements in (-inf, x) online
Inversion count variants with coordinate compression alternative
Sliding window median (with two multisets more common)
```

## PBDS vs Fenwick/Segment for order stats

```text
Values are dynamic, need order of keys     → PBDS / Treap
Fixed universe [1..n], frequency counts    → BIT / SegTree
Offline queries                            → sort + sweep
```

## Caveats

```text
PBDS is GCC-specific — not portable to all judges
For pairs, use custom comparator carefully
Duplicated keys need less_equal + order_of_key adjustment trick
```

## Java / Python equivalents

```text
Java: TreeMap + order stats manual (or TreeMultiset from Guava)
Python: sortedcontainers SortedList
```

Know your toolchain.

---

# 15. When to Use Each Structure — Decision Guide

## Master decision flowchart

```text
START
  │
  ├─ Need connectivity / merge sets?
  │     YES → DSU (see graphs/notes.md)
  │
  ├─ Array static, only range min/max/gcd?
  │     YES → Sparse Table (or prefix if sum)
  │
  ├─ Array static, range sum?
  │     YES → Prefix sum
  │
  ├─ Point update + range sum only?
  │     YES → Fenwick Tree (prefer over seg tree)
  │
  ├─ Range update + range query?
  │     YES → Lazy Segment Tree
  │
  ├─ Range min/max/gcd/xor with updates?
  │     YES → Segment Tree
  │
  ├─ Constraints loose (n ≤ 10⁴), simple logic?
  │     YES → Sqrt Decomposition or even brute
  │
  ├─ Need dynamic order / k-th element?
  │     YES → PBDS / Treap / multiset + balancing
  │
  ├─ Sequence split/merge/reverse on indices?
  │     YES → Implicit Treap
  │
  └─ Offline queries with time dimension?
        → Segment tree over time + DSU rollback, or CDQ divide-conquer
```

## Side-by-side scenarios

### Scenario A: Sum queries, point updates, n,q = 10⁵

```text
Best: Fenwick Tree
Also: Segment Tree
Avoid: Sparse Table (no updates), Sqrt (borderline)
```

### Scenario B: Min queries, no updates, q = 10⁶

```text
Best: Sparse Table O(1) query
Also: Segment Tree O(log n)
```

### Scenario C: Add 5 to every element in [l,r], query sum [l,r]

```text
Best: Lazy Segment Tree
Also: Two BIT trick
Avoid: plain prefix sum
```

### Scenario D: Count inversions in permutation

```text
Best: Fenwick Tree + process right-to-left
Also: Merge sort O(n log n)
```

### Scenario E: Kruskal MST

```text
Best: DSU
```

### Scenario F: Number of connected components after each edge addition

```text
Best: DSU with component count
```

### Scenario G: Range mode query

```text
Hard — no standard O(log n) structure
Often: Mo's algorithm O((n+q)√n) or approximate/heuristic
```

Know when a problem **doesn't** have a clean structure — don't force Segment Tree.

---

# 16. Coordinate Compression — Prerequisite Skill

Many advanced structures assume indices `1..n` on a **compact** domain.

If values are:

```text
[1000000000, 3, 999999999, 7]
```

You cannot allocate BIT of size 10⁹.

Compress:

```text
Sort unique values → map to 1..m
```

```cpp
vector<int> vals = a;
sort(vals.begin(), vals.end());
vals.erase(unique(vals.begin(), vals.end()), vals.end());
auto get = [&](int x) {
    return int(lower_bound(vals.begin(), vals.end(), x) - vals.begin()) + 1;
};
```

Use compressed index in BIT/SegTree.

This pattern appears in:

```text
Inversion count
Count of smaller elements to the right
Range distinct elements (harder)
Offline frequency queries on large value range
```

---

# 17. Offline Techniques That Pair With These Structures

Not every problem needs an online structure.

## Offline RMQ with DSU

Process queries sorted by answer; add edges/elements in order; DSU to answer connectivity.

## Mo's Algorithm

Reorder range queries in blocks of size √n to minimize pointer movement.

```text
Sort queries by (block of l, r)
Move l,r pointers with O(1) add/remove
Total: O((n + q) × √n) for many frequency problems
```

## CDQ Divide and Conquer

3D partial order, inversion variants — divide time dimension.

## Segment Tree over Time

If updates happen at known times, store modification schedule in seg tree over timeline; DFS to answer queries at leaves.

Learn these **after** mastering basic BIT and Segment Tree.

---

# 18. Advanced Pattern 1 — Range Sum with Point Update

**Signal:**

```text
Subarray sum queries
Single element updates
Large n and q
```

**Structure:** Fenwick Tree (first choice) or Segment Tree

**Template:**

```text
Build BIT from array
update(i, delta)
query(l, r) = prefix(r) - prefix(l-1)
```

**Example problems:**

```text
Range Sum Query - Mutable (LeetCode 307)
Count of Smaller Numbers After Self (BIT)
```

---

# 19. Advanced Pattern 2 — Range Minimum/Maximum Query (Static)

**Signal:**

```text
Array doesn't change
Many min/max on [l,r]
```

**Structure:** Sparse Table

**Template:**

```text
Build st[j][i]
query(l,r) = combine(st[k][l], st[k][r-2^k+1])
```

**Example problems:**

```text
Range Minimum Query (classical)
Static RMQ in competitive programming
```

If updates exist → Segment Tree instead.

---

# 20. Advanced Pattern 3 — Range Update + Range Query (Lazy)

**Signal:**

```text
Add v to all elements in [l,r]
Query sum/min on [l,r]
```

**Structure:** Lazy Segment Tree

**Key steps:**

```text
push before recurse
apply on full cover
pull after children
```

**Example problems:**

```text
Range Module (segment tree with lazy set/add — careful)
Corporate Flight Bookings (difference array — special case of range add)
```

**Special case — only range add, query prefix:**

Difference array + prefix sum can suffice if queries are prefix-only.

---

# 21. Advanced Pattern 4 — Inversion / Smaller-Before Count

**Signal:**

```text
Count pairs i < j with arr[i] > arr[j]
Count elements smaller than x as you scan
```

**Structure:** Fenwick Tree on compressed values

**Process:**

```text
Scan right-to-left (or left-to-right depending)
At i: ans += query(compressed[i] - 1)
      update(compressed[i], 1)
```

**Example problems:**

```text
Count of Smaller Numbers After Self
Reverse Pairs (merge sort or BIT)
```

---

# 22. Advanced Pattern 5 — DSU for Connectivity / Cycle Detection

**Signal:**

```text
Are u and v in same component?
Merge groups
Detect cycle when adding undirected edge
Kruskal MST
```

**Structure:** DSU

**Template:**

```text
if (find(u) == find(v)) → same component / cycle
unite(u, v) → merge
```

**Example problems:**

```text
Number of Connected Components in Undirected Graph
Redundant Connection
Accounts Merge
Last Day Where You Can Still Cross (grid DSU)
```

See `graphs/notes.md` for full treatment.

---

# 23. Advanced Pattern 6 — Offline Queries Sorted by Threshold

**Signal:**

```text
Answer queries like "connectivity using only edges with weight ≤ W"
Edges and queries both have weights/limits
```

**Structure:** Sort edges + DSU; sort queries; process in increasing order

```text
Sort queries by W
Sort edges by weight
Add edges while edge.weight ≤ current W
Answer DSU query
```

**Example problems:**

```text
Swim in Rising Water (variant thinking)
Kruskal-related offline queries
```

---

# 24. Advanced Pattern 7 — Sqrt Decomposition / Mo's Algorithm

**Signal:**

```text
Range queries on subarray frequency, distinct count, etc.
Offline queries
O(n√n) acceptable
```

**Structure:** Block decomposition + query reordering

```text
Block size = √n
Sort queries: (l/block, r) with odd-even r optimization
Expand/contract window
```

**Example problems:**

```text
DMOJ/Gymnastics style range frequency
Some "count distinct in range" with constraints
```

---

# 25. Advanced Pattern 8 — Segment Tree with Custom Monoid

**Signal:**

```text
Range gcd
Range bitwise AND (with care — AND is idempotent, sparse table too)
Range xor sum
Combine isn't just min/max/sum
```

**Structure:** Segment Tree — plug in `combine` and `identity`

```text
identity for gcd: 0
identity for xor: 0
identity for min: INF
```

**Example problems:**

```text
Range GCD queries
XOR subarray problems with updates (less common)
```

---

# 26. Advanced Pattern 9 — Order Statistics / Dynamic Rank

**Signal:**

```text
Find k-th smallest in dynamic multiset
Count how many elements < x (online)
```

**Structure:** PBDS `ordered_set` or two multisets (for median)

```text
order_of_key(x)  → count < x
find_by_order(k) → k-th smallest
```

**Example problems:**

```text
K-th smallest in stream (harder — heap of size k more common)
Range order statistics offline → sort + BIT
```

---

# 27. Advanced Pattern 10 — Implicit Treap for Sequence

**Signal:**

```text
Reverse subarray
Insert/delete at index in sequence
Concatenate sequences
All operations must be O(log n)
```

**Structure:** Implicit treap with lazy reverse

```text
split(root, k)
merge(left, right)
lazy reverse flag on nodes
```

**Example problems:**

```text
Queue at the University (classic)
TREAP problems on Codeforces
Rope problems
```

Conceptual for interviews; implementation is long.

---

# 28. Advanced Pattern 11 — 2D Range Sum (Point Updates)

**Signal:**

```text
Matrix point update + rectangle sum query
```

**Structure:** 2D Fenwick or 2D Segment Tree

```text
2D BIT: add(x,y,delta); prefix(x,y); inclusion-exclusion for rectangle
```

Complexity: `O(log n × log m)`

**Example problems:**

```text
2D Binary Indexed Tree problems (competitive programming)
Matrix sum region queries with updates
```

Use only when constraints require it — 2D structures are easy to get wrong.

---

# 29. Advanced Pattern 12 — Difference Array vs Lazy Segment Tree

**Signal:**

```text
Multiple range additions on array
Final array or prefix query — no interleaved arbitrary range sum queries
```

**Structure:** Difference array (special case)

```text
diff[l]   += v
diff[r+1] -= v
Prefix sum diff → final array
```

Simpler than lazy segtree when updates are **only** range add and you need final state or prefix once.

**When difference array fails:**

```text
Interleaved range sum queries during updates → lazy segtree or BIT
Range set operations
```

**Example problems:**

```text
Corporate Flight Bookings
Flight capacity / interval increment
```

---

# 30. Pattern Recognition — Train This Deliberately

When you see:

```text
"Sum of subarray" + "update one element"
        ↓
Fenwick Tree or Segment Tree
```

When you see:

```text
"Sum of subarray" + static array
        ↓
Prefix sum
```

When you see:

```text
"Min/max in range" + static array
        ↓
Sparse Table
```

When you see:

```text
"Min/max in range" + updates
        ↓
Segment Tree
```

When you see:

```text
"Add v to entire range [l,r]"
        ↓
Lazy Segment Tree or difference array
```

When you see:

```text
"Connected?" / "merge sets"
        ↓
DSU
```

When you see:

```text
"Inversions" / "count smaller to the right"
        ↓
BIT + compression or merge sort
```

When you see:

```text
"K-th smallest" dynamic
        ↓
PBDS / Treap
```

When you see:

```text
"Reverse/cut/paste subarray sequence"
        ↓
Implicit Treap
```

When you see:

```text
"Many range queries" + "offline" + "√n acceptable"
        ↓
Mo's algorithm / Sqrt decomposition
```

When you see:

```text
"Values up to 10⁹" but only n elements
        ↓
Coordinate compress before BIT/SegTree
```

When you see:

```text
"Only range add, query final array once"
        ↓
Difference array — don't over-engineer
```

---

# 31. Edge Cases — Extremely Important

Advanced structure bugs are almost always **indexing and boundary** bugs.

## General

```text
Empty array / n = 0
Single element n = 1
Query l == r (single element range)
Query entire array [0, n-1]
Off-by-one: inclusive vs exclusive bounds
0-indexed vs 1-indexed (BIT usually 1-indexed)
```

## Segment Tree

```text
Tree size: use 4*n or 2*next_pow2
Integer overflow in sum (use long long)
Identity values: INF for min (must be > any arr[i])
Lazy push forgotten before query → wrong answer
Lazy merge order wrong for set vs add
Updating same index twice without reading old value
```

## Fenwick Tree

```text
Forgetting 1-indexing
Building with wrong size (n vs n+1)
Range update tricks need two BITs — easy to miswire
```

## Sparse Table

```text
Using for sum (wrong — overlapping)
Forgetting to rebuild after "update" attempt
log table boundaries at r - 2^k + 1
```

## DSU

```text
0-indexed vs 1-indexed nodes
Unite without find first
Not using path compression / rank → TLE
Off-by-one array size (n nodes vs n+1)
```

## Sqrt Decomposition

```text
Block size 0 when n = 0
Partial blocks at range boundaries
Lazy per block not pushed before point access
```

## PBDS

```text
Duplicate keys with less<T> — use less_equal + careful order_of_key
Not portable compilers
```

## Always test

```text
n = 1
l = r
l = 0, r = n-1
Maximum values (overflow)
All same elements
Strictly increasing / decreasing
```

---

# 32. Implementation Skills — What to Practice

## Skill 1: Write Segment Tree from memory

Recursive build, query, point update — 15 minutes target.

Then lazy range add + range sum.

## Skill 2: Write Fenwick Tree from memory

`add`, `prefix`, `rangeSum` — 5 minutes target.

## Skill 3: Sparse Table in 10 minutes

Build + query for min.

## Skill 4: DSU in 5 minutes

find with compression, unite with rank.

## Skill 5: Know when NOT to code

```text
n, q ≤ 1000 → brute force may pass
Static sum → prefix only
Only range add, final array → difference array
```

## Skill 6: Draw the segment tree for n=4

On paper — see how query decomposes.

## Skill 7: Trace lazy propagation

Update [1,3] on array size 4 — track lazy tags per node.

---

# 33. Compare Multiple Approaches

## Problem: Range sum + point update, n = 10⁵, q = 10⁵

### Brute force

```text
Time: O(n × q) — TLE
Space: O(1) extra
```

### Prefix sum + rebuild on update

```text
Time: O(n) per update — TLE
```

### Fenwick Tree

```text
Time: O(q log n) — passes
Space: O(n)
Code: short
```

### Segment Tree

```text
Time: O(q log n) — passes
Space: O(n)
Code: longer
```

**Conclusion:** Fenwick preferred for this exact pattern.

---

## Problem: Range min query, static array, q = 10⁶

### Segment Tree

```text
Query: O(log n) × 10⁶ — OK
```

### Sparse Table

```text
Query: O(1) × 10⁶ — faster, simpler per query
Build: O(n log n) once
Space: O(n log n)
```

**Conclusion:** Sparse Table wins for static RMQ.

---

## Problem: Dynamic connectivity, 10⁵ unions/finds

### BFS from scratch each query

```text
O(n) per query — TLE
```

### DSU

```text
O(α(n)) per op — passes
```

---

## Problem: Range distinct count

### Segment tree on counts

```text
Not straightforward — values matter
```

### Mo's algorithm

```text
O((n+q)√n) — typical competitive approach
```

**Conclusion:** recognize hard problems — no magic tree.

---

# 34. Problems to Practice

You don't need 200 random problems. Cover each structure and pattern.

### Fenwick Tree / BIT

- [ ] Range Sum Query - Mutable (LC 307)
- [ ] Count of Smaller Numbers After Self (LC 315)
- [ ] Reverse Pairs (LC 493)

### Segment Tree

- [ ] Range Sum Query - Mutable (also seg tree solution)
- [ ] Range Minimum Query (classical)
- [ ] My Calendar I/II/III (interval / segment tree on time)
- [ ] Count of Range Sum (harder — merge or BIT)

### Lazy Segment Tree

- [ ] Range Addition queries (competitive)
- [ ] Falling Snow / range update templates

### Sparse Table

- [ ] Static RMQ
- [ ] Maximum of Minimum Values in subarrays (conceptual)

### DSU

- [ ] Number of Provinces (LC 547)
- [ ] Redundant Connection (LC 684)
- [ ] Accounts Merge (LC 721)
- [ ] Connecting Cities With Minimum Cost (LC 1135 — Kruskal)
- [ ] Last Day Where You Can Still Cross (LC 1970)

### Sqrt / Mo

- [ ] D-Query style problems (competitive)
- [ ] Some "distinct in range" with constraints

### PBDS / Order statistics

- [ ] Order statistic tree practice (Codeforces)
- [ ] K-th number in dynamic stream (conceptual)

### Implicit Treap

- [ ] Array manipulation with reverse (advanced CF)

### Composition problems

- [ ] Count of Smaller Numbers After Self → BIT + compression
- [ ] Rectangle Area II → coordinate compress + sweep + seg tree

---

# 35. For EVERY Advanced Data Structures Problem

Use this thinking process:

```text
1. What is the sequence / domain?
   Array indices? Values? Time? Graph nodes?

2. What operations are required?
   Point update? Range update?
   Range query? What aggregate?

3. Static or dynamic?
   If static → prefix, sparse table
   If dynamic → BIT, segtree, sqrt, treap

4. Online or offline?
   Offline → sort queries, Mo's, CDQ, reverse DSU

5. What are n and q?
   Determines if O(n√n) or O(n log n) needed

6. Is coordinate compression needed?
   Value range >> n?

7. Can a simpler trick work?
   Difference array? Prefix only? Brute?

8. Which structure's combine matches the operation?
   Sum, min, gcd, xor — associative?

9. Indexing convention?
   0 vs 1 — pick one and stick to it

10. Overflow?
    Use long long for sums/products

11. Identity element for queries?
    0, INF, -INF — correct?

12. Lazy needed?
    Range updates on segments?

13. Expected complexity?
    O(q log n) typical target

14. Implement vs library?
    C++: PBDS, policy; know your judge

15. Edge cases?
    n=1, l=r, full range, empty
```

Question 7 is especially important — many problems **look** like segment tree but are difference array or prefix sum in disguise.

---

# 36. One Especially Important Mental Shift

Beginners see:

```text
"Range query" → Segment Tree immediately
```

Improve by asking:

> What exactly is being queried, and what changes between queries?

Maybe the array is **static** → you don't need any dynamic structure.

Maybe updates are **only range additions** and you only need the **final** array → difference array.

Maybe you only need **prefix** sums, not arbitrary ranges → BIT prefix only, or simple prefix array.

Maybe queries are **offline** → process in sorted order with DSU, no segtree.

Maybe the operation is **connectivity** → DSU, not a tree on an array.

Advanced structures are **tools in a toolkit**, not a hammer for every range-shaped nail.

The skill is:

```text
Recognize the operation algebra (monoid)
Recognize static vs dynamic
Recognize online vs offline
Choose the simplest structure that meets constraints
```

---

# 37. Segment Tree — Full Reference Template (Sum + Lazy Add)

Study this as a single unit after you understand the pieces.

```cpp
struct SegLazy {
    int n;
    vector<long long> t, lz;

    SegLazy(int n_) : n(n_), t(4 * n_, 0), lz(4 * n_, 0) {}

    void build(const vector<int>& a, int v, int tl, int tr) {
        if (tl == tr) { t[v] = a[tl]; return; }
        int tm = (tl + tr) / 2;
        build(a, v*2, tl, tm);
        build(a, v*2+1, tm+1, tr);
        t[v] = t[v*2] + t[v*2+1];
    }

    void apply(int v, int tl, int tr, long long add) {
        t[v] += add * (tr - tl + 1);
        lz[v] += add;
    }

    void push(int v, int tl, int tr) {
        if (lz[v] == 0 || tl == tr) return;
        int tm = (tl + tr) / 2;
        apply(v*2, tl, tm, lz[v]);
        apply(v*2+1, tm+1, tr, lz[v]);
        lz[v] = 0;
    }

    void upd(int v, int tl, int tr, int l, int r, long long add) {
        if (l > r) return;
        if (l == tl && r == tr) { apply(v, tl, tr, add); return; }
        push(v, tl, tr);
        int tm = (tl + tr) / 2;
        upd(v*2, tl, tm, l, min(r, tm), add);
        upd(v*2+1, tm+1, tr, max(l, tm+1), r, add);
        t[v] = t[v*2] + t[v*2+1];
    }

    long long qry(int v, int tl, int tr, int l, int r) {
        if (l > r) return 0;
        if (l == tl && r == tr) return t[v];
        push(v, tl, tr);
        int tm = (tl + tr) / 2;
        return qry(v*2, tl, tm, l, min(r, tm))
             + qry(v*2+1, tm+1, tr, max(l, tm+1), r);
    }
};
```

Trace one `upd` and one `qry` on paper with `n=4`.

---

# 38. Fenwick Tree — Full Reference Template

```cpp
struct BIT {
    int n;
    vector<long long> f;

    BIT(int n_) : n(n_), f(n_ + 1, 0) {}

    void add(int i, long long v) {
        for (; i <= n; i += i & -i) f[i] += v;
    }

    long long sum(int i) const {
        long long s = 0;
        for (; i > 0; i -= i & -i) s += f[i];
        return s;
    }

    long long rangeSum(int l, int r) const {
        return sum(r) - sum(l - 1);
    }

    // optional: O(n) build
    void build(const vector<int>& a) {
        for (int i = 1; i <= n; i++) f[i] += a[i-1];
        for (int i = 1; i <= n; i++) {
            int j = i + (i & -i);
            if (j <= n) f[j] += f[i];
        }
    }
};
```

Memorize `i += i & -i` and `i -= i & -i`.

---

# 39. Common Mistakes and Debugging

```text
Wrong answer on large inputs only     → overflow (use long long)
TLE                                   → wrong complexity class; missing path compression
RE (runtime error)                    → segtree size; out of bounds index
WA on specific test                   → off-by-one; 0 vs 1 index
Lazy segtree WA                       → forgot push before query
BIT WA                                → 1-indexed input treated as 0-indexed
Sparse table WA on sum                  → used overlapping technique wrongly
DSU WA                                → components counted wrong on self-loop
```

### Debug checklist

```text
Print small case tree arrays
Compare brute force for n ≤ 200
Stress test random vs brute
```

---

# 40. Relationship to Other Phases

```text
Phase 5  Arrays        → prefix sum, difference array foundations
Phase 6  Hash Maps     → coordinate compression, frequency
Phase 8  Trees         → BST order; treap is a tree
Phase 11 Graphs        → DSU primary home
Phase 12 THIS          → range structures + order structures
Phase 14 Adv Graphs    → LCA sometimes uses sparse table / euler tour + RMQ
```

LCA (Lowest Common Ancestor) often uses:

```text
Euler tour + RMQ (sparse table or segtree)
Binary lifting
```

You'll meet that in Advanced Graph Algorithms.

---

# 41. Real-World Intuition (Brief)

```text
Segment trees / BIT     → database index ranges, cumulative metrics
DSU                   → network connectivity, image percolation, Kruskal
Sparse table          → static RMQ in embedded analytics
Sqrt decomposition    → cache-friendly blocked arrays
Balanced BST / PBDS   → ordered indexes in memory
```

You won't implement lazy propagation in production daily — but **the idea of deferring batch updates** appears everywhere (lazy evaluation, write buffers, segment-level locking).

---

# Complete Advanced Data Structures Checklist

```text
ADVANCED DATA STRUCTURES
│
├── Foundations
│   ├── Static vs dynamic array operations
│   ├── Online vs offline queries
│   ├── Associative operations / monoids
│   ├── Coordinate compression
│   ├── Difference array (range add shortcut)
│   └── Complexity target: O(q log n)
│
├── Segment Tree
│   ├── Interval decomposition idea
│   ├── Build O(n)
│   ├── Point update O(log n)
│   ├── Range query O(log n)
│   ├── Combine function (sum, min, max, gcd, xor)
│   ├── Identity elements
│   ├── Iterative vs recursive
│   ├── Lazy propagation
│   │   ├── push / pull
│   │   ├── Range add + range sum
│   │   └── Range set (concept)
│   └── When seg tree over BIT
│
├── Fenwick Tree (BIT)
│   ├── 1-indexing
│   ├── add / prefix / rangeSum
│   ├── i & -i insight
│   ├── Inversion count pattern
│   ├── O(n) build (optional)
│   └── 2D BIT (awareness)
│
├── Sparse Table
│   ├── Static RMQ O(1)
│   ├── Idempotent operations only
│   ├── Build O(n log n)
│   ├── Does NOT support sum
│   └── vs Segment Tree for static min
│
├── Disjoint Set Union (DSU)
│   ├── find + path compression
│   ├── union by rank/size
│   ├── Connectivity / cycle detection
│   ├── Kruskal MST
│   ├── Offline + sort by threshold
│   ├── Component size / count
│   └── Full detail in graphs/notes.md
│
├── Sqrt Decomposition
│   ├── Block size √n
│   ├── Range query O(√n)
│   ├── Point update O(√n)
│   ├── Mo's algorithm awareness
│   └── When constraints allow simpler approach
│
├── Treap / Splay (Conceptual)
│   ├── BST + heap priority
│   ├── Implicit treap (sequence)
│   ├── split / merge
│   ├── Range reverse lazy
│   └── When std::set / PBDS enough
│
├── Ordered Set / PBDS
│   ├── order_of_key
│   ├── find_by_order
│   ├── GCC / portability caveats
│   └── Dynamic order statistics
│
├── Core Patterns
│   ├── Range sum + point update (BIT)
│   ├── Static RMQ (Sparse Table)
│   ├── Range update + query (Lazy Seg)
│   ├── Inversion / smaller-before (BIT)
│   ├── DSU connectivity
│   ├── Offline threshold + DSU
│   ├── Mo's / sqrt on queries
│   ├── Custom monoid seg tree
│   ├── Order statistics (PBDS)
│   ├── Implicit treap sequence
│   ├── 2D BIT (point update, rect query)
│   └── Difference array vs lazy
│
├── Pattern Recognition
│   ├── Query/update type → structure map
│   ├── Static → prefix / sparse
│   ├── Dynamic sum → BIT first
│   ├── Dynamic min/max → seg tree
│   ├── Range add only → difference array
│   ├── Connectivity → DSU
│   └── Compress coordinates when needed
│
├── Edge Cases
│   ├── n = 1, l = r, full range
│   ├── 0-index vs 1-index
│   ├── Overflow (long long)
│   ├── Lazy push forgotten
│   ├── BIT sizing
│   └── DSU indexing
│
├── Implement Yourself
│   ├── Segment tree (point update, sum query)
│   ├── Lazy segment tree (range add)
│   ├── Fenwick tree
│   ├── Sparse table (RMQ)
│   ├── DSU
│   └── Sqrt decomposition (basic)
│
├── Comparisons
│   ├── BIT vs Segment Tree
│   ├── Sparse Table vs Segment Tree
│   ├── Sqrt vs log structures
│   ├── DSU vs BFS/DFS
│   ├── PBDS vs BIT for order stats
│   └── Brute force when n,q small
│
└── For EVERY Problem
    ├── Identify operations and aggregates
    ├── Static or dynamic?
    ├── Online or offline?
    ├── n, q constraints
    ├── Simpler trick available?
    ├── Pick structure + indexing convention
    ├── Handle edge cases and overflow
    ├── Analyze time and space
    └── Trace small example by hand
```

For your **core DSA pass**, that's enough advanced data structures. You don't need to master every variant (persistent segment trees, convex hull trick, Li Chao tree, wavelet trees, centroid decomposition) until competitive programming or specialized topics demand them.

Once you can **implement Fenwick Tree and Segment Tree (with lazy propagation) from scratch, build Sparse Table for RMQ, use DSU fluently, recognize when sqrt or difference array suffices, and explain when PBDS/treap concepts apply**, you're ready for the next major block: **String Algorithms (Phase 13)**.

---

# 42. Appendix — Quick Reference Formulas

```text
Segment tree size:          ≥ 4n nodes (recursive) or 2 × 2^ceil(log2 n) (iterative)
BIT index:                  1 .. n
BIT update:                 i += i & -i
BIT query:                  i -= i & -i
Sparse table query length:    k = floor(log2(r - l + 1))
Sqrt block size:              ⌈√n⌉ or ⌊√n⌋ + 1
DSU amortized:                O(α(n)) ≈ O(1) practical
Mo's complexity:              O((n + q) × √n) typical
```

---

# 43. Appendix — Iterative Segment Tree Sketch (Sum)

For contests, some prefer iterative:

```cpp
struct IterSeg {
    int n;
    vector<long long> t;
    IterSeg(const vector<int>& a) {
        int sz = 1;
        n = (int)a.size();
        while (sz < n) sz <<= 1;
        t.assign(2 * sz, 0);
        for (int i = 0; i < n; i++) t[sz + i] = a[i];
        for (int i = sz - 1; i > 0; i--) t[i] = t[i<<1] + t[i<<1|1];
    }
    void set(int p, long long v) {
        for (t[p += n/2 ? ...] — fix: use proper size pad
    }
};
```

Build with `size` = power of 2 ≥ n; leaves at `[size..size+n-1]`.

Learn recursive first; convert when comfortable.

---

# 44. Appendix — Merge Sort Tree (Awareness)

Segment tree where each node stores **sorted list** of values in range.

Query "count elements ≤ x in [l,r]" in `O(log² n)` with binary search in nodes.

Heavy in memory — know it exists for offline/count problems.

---

# 45. Appendix — Persistent Segment Tree (Awareness)

When you need **historical versions** of the array after updates.

Path copying on update — new nodes only along root-to-leaf path.

Used in some advanced competitive problems — not core interview material.

---

# 46. Worked Micro-Example — BIT Inversion Count

Array: `[3, 1, 2]` (0-indexed values)

Compress: `1→1, 2→2, 3→3`

Process right to left:

```text
i=2, val=2: query(1)=0, smaller after=0; add(2,1)
i=1, val=1: query(0)=0, smaller after=0; add(1,1)
i=0, val=3: query(2)=2, smaller after=2; add(3,1)

Inversions = 2  (pairs (3,1), (3,2))
```

Trace `i & -i` on paper for `i=1..8` to build intuition once.

---

# 47. Worked Micro-Example — DSU Cycle Detection

Edges: `(0,1), (1,2), (2,0)`

```text
unite(0,1) → OK
unite(1,2) → OK  (same component {0,1,2})
unite(2,0) → find(2)==find(0) → CYCLE
```

For undirected graph MST/Kruskal: skip edge that would cycle.

---

# 48. Worked Micro-Example — Sparse Table

`arr = [7, 2, 3, 0, 5, 8, 1, 6]`

`st[0] = arr`

`st[1][i] = min(arr[i], arr[i+1])`

Query min on [2,5] (0-indexed): values `3,0,5,8` → min `0`

`k = floor(log2(4)) = 2`? length 4 → k=2, blocks length 4...

Actually length = 5-2+1 = 4, k=2:

```text
min(st[2][2], st[2][5-4+1]) = min(st[2][2], st[2][2]) — verify indices
```

Work through on paper — indexing discipline matters.

---

# 49. Self-Test Questions

Answer without looking at code:

```text
1. Why is Fenwick Tree often preferred over Segment Tree for sum?
2. Why can't Sparse Table handle range sum?
3. What does lazy[node] mean in lazy propagation?
4. When is difference array enough instead of lazy segtree?
5. What is the identity for range min query?
6. What operation does path compression optimize?
7. What block size for sqrt decomposition on n=10000?
8. What does order_of_key return in PBDS?
9. Static array, 10⁶ min queries — Sparse Table or SegTree?
10. Range add + range sum, interleaved — which structure?
```

If you can answer all ten clearly, Phase 12 fundamentals are solid.

---

# 50. Closing Integration

Advanced data structures complete the picture for **array-like** and **connectivity** workloads:

```text
Arrays gave you          prefix sum, two pointers, sliding window
Hash maps gave you       O(1) lookup, frequency
Trees gave you           hierarchy, BST order, heaps
Graphs gave you          BFS/DFS, shortest path, DSU
Advanced DS give you     subarray aggregates with updates, static RMQ, dynamic order
```

The through-line:

> Preprocess so each query reuses work instead of rescanning everything.

Segment Tree, Fenwick Tree, Sparse Table, DSU, Sqrt Decomposition, Treap, and PBDS are all instantiations of that principle under different operation sets.

Master the **decision layer** first. Implementations are learnable once you know **which tool** and **why**.
