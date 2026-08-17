# Greedy & Divide-and-Conquer — Complete Deep-Understanding Syllabus

Greedy algorithms and Divide-and-Conquer are two of the most important algorithmic paradigms in computer science. They sit at **Phase 10** in your DSA map — after Dynamic Programming — because you need recursion, sorting, and DP intuition before you can judge **when to make a local choice** versus **when to split and combine subproblems** versus **when to memoize overlapping substructure**.

Don't learn these as separate tricks. Learn:

> Under what structural conditions does a locally optimal choice lead to a globally optimal solution?

and:

> When does breaking a problem into independent subproblems and combining their answers work better than brute force?

This document covers both paradigms deeply: theory, proof techniques, classic algorithms, at least **12 patterns**, pattern recognition, edge cases, representative problems, a problem-solving framework, and a complete checklist.

---

# 1. Start with the problem Greedy solves

Suppose you have meetings:

```text
Meeting A: [1, 4]
Meeting B: [3, 5]
Meeting C: [0, 6]
Meeting D: [5, 7]
Meeting E: [8, 9]
Meeting F: [5, 9]
```

You want the **maximum number of non-overlapping meetings** in one room.

### Brute force

Try every subset of meetings. Check whether any two overlap.

```text
2^n subsets
```

For `n = 30`, that's over a billion subsets.

### Dynamic Programming

Sort by end time. Define:

```text
dp[i] = max meetings using first i meetings (by end time)
```

Transition considers whether to include meeting `i` or not.

Works. But there's a simpler idea.

### Greedy insight

Always pick the meeting that **finishes earliest** among those still compatible.

```text
Sort by end time
Pick earliest-finishing compatible meeting
Repeat
```

Why does this work?

Because finishing early **leaves the most room** for future meetings. You're not exploring all subsets — you're making one decisive local choice at each step.

That's the essence of greedy:

> At each step, make the choice that looks best **right now**, without revisiting past decisions.

---

# 2. What is a Greedy Algorithm?

A greedy algorithm builds a solution **incrementally**, one piece at a time. At each step it chooses the option that appears locally optimal, hoping (or proving) that local optimality implies global optimality.

```text
Input
  ↓
Sort / prioritize (often)
  ↓
Repeat:
    pick best feasible local choice
    commit (never undo)
  ↓
Output
```

Key properties:

| Property | Meaning |
| -------- | ------- |
| Greedy choice property | A globally optimal solution can be reached by making a locally optimal choice |
| Optimal substructure | An optimal solution contains optimal solutions to subproblems |
| No backtracking | Once chosen, a decision is final |

Contrast with brute force:

```text
Brute force: explore all possibilities
Greedy:      commit early, prune aggressively
```

Contrast with DP:

```text
DP:     explore multiple subproblems, combine overlapping results
Greedy: one path forward, no memoization needed (when correct)
```

Understand:

- Greedy is not "pick the maximum every time"
- Greedy is not always correct
- Greedy often needs a **proof** or a **counterexample hunt**
- Greedy solutions are usually simpler and faster than DP when they work

---

# 3. Why Greedy Sometimes Works

Greedy succeeds when the problem has the right mathematical structure.

## Greedy Choice Property

There exists a greedy choice that is **safe** — meaning some optimal solution can be transformed to include that greedy choice without worsening the result.

Example (Activity Selection):

> Among all compatible meetings, picking the one that ends earliest never hurts optimality.

## Optimal Substructure

After making a greedy choice, the remaining subproblem is of the **same form**, and an optimal solution to the whole problem equals the greedy choice plus an optimal solution to the remainder.

```text
Optimal(S) = greedy_choice(S) + Optimal(remaining(S))
```

Not every problem has these properties.

```text
0/1 Knapsack → greedy by value/weight ratio FAILS
Coin change with arbitrary denominations → greedy FAILS in general
```

That's why Phase 10 comes after DP: you need to compare paradigms.

---

# 4. Proof Techniques for Greedy

You cannot rely on intuition alone. Train these proof styles.

## A. Exchange Argument

**Idea:** Suppose an optimal solution `OPT` differs from the greedy solution `G`. Show you can **exchange** elements of `OPT` to match `G` without making the solution worse.

### Template

```text
1. Let G be the greedy solution
2. Let OPT be any optimal solution
3. Find first position where G and OPT differ
4. Show you can modify OPT to include G's choice
5. Prove modified solution is still feasible and not worse
6. Repeat until OPT = G
```

### Example — Activity Selection

Greedy rule: pick compatible meeting with **earliest finish time**.

Suppose `OPT` picks meeting `x` first, but greedy picks meeting `g` first (`finish(g) ≤ finish(x)`).

If `g` and `x` are compatible with the same future set, replacing `x` with `g` in `OPT`:

- Still feasible (finishes no later)
- Does not reduce count of meetings

Therefore an optimal solution exists that starts with the greedy choice. Apply inductively.

```text
OPT:  x → ...
G:    g → ...

finish(g) ≤ finish(x)
⇒ replacing x with g frees equal or more time
```

## B. Staying Ahead

**Idea:** Show that after each greedy step, the greedy partial solution is **at least as good as** any other partial solution that made the same number of choices.

### Template

```text
1. Compare greedy partial solution G_k after k steps
2. Compare any other partial solution A_k after k steps
3. Prove invariant: G_k is "ahead" of A_k by some measure
4. At the end, greedy is optimal
```

### Example — Interval Scheduling (same problem, different framing)

After `k` choices, greedy has finished no later than any other schedule with `k` meetings.

Measure: **earliest finish time of the k-th selected meeting**.

Greedy minimizes this finish time at every step, so it can always fit at least as many future meetings.

## C. Cut-and-Paste / Contradiction

**Idea:** Assume greedy is not optimal. Derive a contradiction by modifying the supposed optimal solution.

```text
Assume ∃ optimal OPT better than greedy G
Show OPT can be transformed into G without loss
Contradiction
```

## D. Matroid Theory (advanced but useful concept)

Some problems are instances of **matroids** (e.g., minimum spanning tree). On a matroid, greedy by weight works.

You don't need full matroid theory for interviews, but know:

```text
MST (Kruskal/Prim) → greedy on matroid structure
```

## E. When you cannot prove it

If you cannot prove greedy works, try to **break it**:

```text
Find small counterexample
```

Example — Coin Change:

```text
Coins: [1, 3, 4]
Amount: 6

Greedy (largest first): 4 + 1 + 1 = 3 coins
Optimal:                3 + 3     = 2 coins
```

Counterexamples are as valuable as proofs.

---

# 5. When Greedy Works vs When It Fails

## Works well when

```text
Scheduling with compatibility constraints (finish early)
Fractional knapsack (divisible items)
Huffman coding (merge least frequent)
MST edge selection (safe edge / cut property)
Some interval problems (after sorting)
```

## Fails when

```text
Choices interact in non-local ways
You need to compare combinations, not single items
0/1 (indivisible) knapsack
General coin change
Longest path in general graphs
TSP
```

## Quick diagnostic

Ask:

```text
1. Can I sort or prioritize input meaningfully?
2. Does a locally best choice never block a better global arrangement?
3. After choosing, is the remaining problem the same type?
4. Can I exchange any optimal solution to include my greedy pick?
```

If any answer is shaky, try DP or brute force on small inputs to hunt counterexamples.

---

# 6. Classic Greedy Problems — Deep Dive

## 6.1 Activity Selection / Interval Scheduling

**Problem:** Maximum number of non-overlapping intervals in one resource.

```text
Intervals (by end):
[1,4] [3,5] [0,6] [5,7] [8,9] [5,9]

Sort by end: [1,4] [3,5] [5,7] [0,6] [5,9] [8,9]
Greedy: pick [1,4], skip [3,5], pick [5,7], skip [0,6],[5,9], pick [8,9]
Answer: 3
```

**Algorithm:**

```text
sort intervals by end time
count = 0, last_end = -∞
for each interval [s, e]:
    if s >= last_end:
        take interval
        last_end = e
        count++
return count
```

```python
def activity_selection(intervals):
    intervals.sort(key=lambda x: x[1])
    count = 0
    last_end = float('-inf')
    for start, end in intervals:
        if start >= last_end:
            count += 1
            last_end = end
    return count
```

```text
Time:  O(n log n)  — sorting
Space: O(1)        — excluding sort
```

**Why earliest finish?** Exchange argument — finishing earlier never reduces future capacity.

**Variants:**

- Weighted interval scheduling → **DP**, not greedy
- Multiple rooms → sweep line / heap
- Minimum number of rooms → sort starts/ends, sweep

---

## 6.2 Interval Scheduling — Minimum Number of Rooms

**Problem:** Minimum conference rooms for all meetings.

```text
[[0,30],[5,10],[15,20]]

Start events: 0, 5, 15
End events:   10, 20, 30

Sweep: +1 at start, -1 at end
Max concurrent = 2
```

Greedy approach: **min-heap of end times**.

```python
import heapq

def min_meeting_rooms(intervals):
    intervals.sort(key=lambda x: x[0])
    heap = []  # end times
    for start, end in intervals:
        if heap and heap[0] <= start:
            heapq.heappop(heap)
        heapq.heappush(heap, end)
    return len(heap)
```

Not pure greedy in the same sense — it's sweep + heap. But the local decision "reuse earliest-ending room" is greedy.

---

## 6.3 Fractional Knapsack

**Problem:** Items have weight and value. You can take **fractions** of items. Maximize value within capacity `W`.

**Greedy:** Sort by value/weight ratio descending. Take as much as possible from best ratio items.

```text
Items: (w=10, v=60, ratio=6), (w=20, v=100, ratio=5), (w=30, v=120, ratio=4)
W = 50

Take all of item 1 (10) → value 60, remaining 40
Take all of item 2 (20) → value 100, remaining 20
Take 20/30 of item 3 → value 80

Total value = 240
```

```python
def fractional_knapsack(capacity, items):
    # items: list of (weight, value)
    items.sort(key=lambda x: x[1] / x[0], reverse=True)
    total_value = 0.0
    for w, v in items:
        if capacity == 0:
            break
        take = min(w, capacity)
        total_value += take * (v / w)
        capacity -= take
    return total_value
```

**Why greedy works:** Exchange argument on ratios — any solution can be improved by swapping lower-ratio material for higher-ratio material until capacity is full.

**0/1 Knapsack contrast:**

```text
Same items, W = 50, but cannot split

Greedy by ratio: item1 + item2 = 160  (weight 30)
Optimal 0/1:     item2 + item3 = 220  (weight 50) if item3 taken whole

Greedy FAILS
```

---

## 6.4 Coin Change — Greedy Cases

**Problem:** Fewest coins to make amount `A` using denominations `D`.

### When greedy works

**Canonical coin systems** (e.g., US coins 1, 5, 10, 25):

```text
Greedy: always take largest coin ≤ remaining
```

Proof idea: each denomination is large enough relative to smaller ones.

### When greedy fails

```text
Coins: [1, 3, 4]
Amount: 6

Greedy: 4 + 1 + 1 = 3 coins
DP:     3 + 3     = 2 coins
```

**Rule of thumb:**

```text
Greedy coin change → only when proven or given as canonical
Otherwise → DP
```

```python
def coin_change_dp(coins, amount):
    dp = [float('inf')] * (amount + 1)
    dp[0] = 0
    for a in range(1, amount + 1):
        for c in coins:
            if c <= a:
                dp[a] = min(dp[a], dp[a - c] + 1)
    return dp[amount] if dp[amount] != float('inf') else -1
```

---

## 6.5 Huffman Coding (Concept)

**Problem:** Given character frequencies, build a prefix-free binary code minimizing expected codeword length.

**Greedy idea:** Repeatedly merge the two **least frequent** subtrees.

```text
frequencies: a=5, b=9, c=12, d=13, e=16, f=45

Merge a(5)+b(9)=14
Merge c(12)+14=26
Merge d(13)+e(16)=29
Merge 26+29=55
Merge 55+45=100
```

Use a **min-heap** of frequencies.

```text
while more than one node:
    x = pop min
    y = pop min
    push (x.freq + y.freq)
```

**Why greedy works:** Exchange argument / matroid-like structure on trees.

**Applications:**

- Compression (ZIP, JPEG building blocks)
- Merge cost minimization
- Optimal merge pattern

```text
Time: O(n log n)
```

---

## 6.6 Jump Game

**Problem:** `nums[i]` = max jump from index `i`. Can you reach the last index?

### Jump Game I (reachable?)

**Greedy:** Track farthest reachable index.

```python
def can_jump(nums):
    farthest = 0
    for i, jump in enumerate(nums):
        if i > farthest:
            return False
        farthest = max(farthest, i + jump)
    return True
```

```text
Time: O(n)
Space: O(1)
```

**Invariant (staying ahead):** If you can reach index `i`, you can reach any index `< i` that was already covered.

### Jump Game II (minimum jumps)

**Greedy:** BFS-like layers without explicit queue.

```python
def jump(nums):
    if len(nums) <= 1:
        return 0
    jumps = 0
    current_end = 0
    farthest = 0
    for i in range(len(nums) - 1):
        farthest = max(farthest, i + nums[i])
        if i == current_end:
            jumps += 1
            current_end = farthest
    return jumps
```

When you exhaust the current "jump layer," increment jumps and extend to farthest seen.

---

## 6.7 Gas Station

**Problem:** Circular route. `gas[i]` fuel at station `i`, `cost[i]` fuel to next station. Find starting index if a complete circuit exists, else -1.

**Greedy insight:**

```text
If total gas >= total cost → solution exists
If a segment fails starting at i, no start in [i..j] works (where j is failure point)
```

```python
def can_complete_circuit(gas, cost):
    if sum(gas) < sum(cost):
        return -1
    tank = 0
    start = 0
    for i in range(len(gas)):
        tank += gas[i] - cost[i]
        if tank < 0:
            start = i + 1
            tank = 0
    return start
```

**Proof sketch:** If you fail at `j` starting from `i`, starting anywhere between `i` and `j` would have had even less accumulated fuel before reaching the failure.

---

## 6.8 Assign Cookies

**Problem:** Each child wants `g[i]` size cookie. Each cookie `s[j]`. Maximize satisfied children.

**Greedy:** Sort both. Smallest cookie that satisfies smallest unsatisfied child.

```python
def find_content_children(g, s):
    g.sort()
    s.sort()
    child = cookie = 0
    while child < len(g) and cookie < len(s):
        if s[cookie] >= g[child]:
            child += 1
        cookie += 1
    return child
```

---

## 6.9 Partition Labels

**Problem:** Partition string into as many parts as possible so each letter appears in at most one part.

**Greedy:** Record last occurrence of each char. Extend current partition end to max last occurrence seen.

```python
def partition_labels(s):
    last = {c: i for i, c in enumerate(s)}
    start = end = 0
    result = []
    for i, c in enumerate(s):
        end = max(end, last[c])
        if i == end:
            result.append(end - start + 1)
            start = i + 1
    return result
```

---

## 6.10 Task Scheduler / Rearrange String

**Problem:** Tasks with cooldown `n`. Minimum time to complete all.

**Greedy:** Schedule most frequent task first, leave cooldown slots.

```text
Count frequencies → max heap
Each round: pick up to (n+1) most frequent tasks (with cooldown)
```

Related: **Reorganize String** — same frequency-greedy idea with feasibility check.

---

## 6.11 Minimum Spanning Tree (Greedy on Graphs)

**Kruskal:** Sort edges by weight. Add edge if it doesn't form a cycle.

**Prim:** Grow tree from a start node, always add cheapest edge to tree.

Both are greedy algorithms with cut-property proofs.

```text
Cut property: minimum weight edge crossing a cut is safe
```

This connects Greedy to Graphs (Phase 8).

---

## 6.12 Dijkstra's Shortest Path (Greedy + Heap)

**Greedy choice:** Always settle the unvisited vertex with smallest tentative distance.

Works with **non-negative** weights.

```text
Not greedy on general graphs with negative edges → use Bellman-Ford
```

---

# 7. Greedy Pattern 1 — Sort Then Scan

The most common greedy template.

```text
Sort input by some key
Scan linearly, making greedy decisions
```

**Examples:**

- Activity selection (sort by end)
- Fractional knapsack (sort by ratio)
- Assign cookies (sort both arrays)
- Meeting rooms (sort by start)

```python
def sort_then_scan(items, key, process):
    items.sort(key=key)
    result = process(items)
    return result
```

**Recognition signal:**

```text
"Maximum number of ..."
"Minimum number of ..."
"Schedule ..."
"Assign ..."
        ↓
Try sorting + one pass
```

---

# 8. Greedy Pattern 2 — Earliest Finish / Latest Start

Interval problems often reduce to one of:

```text
Max non-overlapping → sort by END, greedy take
Min rooms/resources  → sort by START, sweep/heap
```

```text
Timeline
|----A----|
      |--B--|
           |---C---|

End-sorted greedy: take A, skip B, take C
```

**Edge case:** Touching intervals `[1,2]` and `[2,3]` — clarify if `end == start` counts as overlap. Usually **allowed** (non-overlapping if `start >= last_end`).

---

# 9. Greedy Pattern 3 — Two Heaps / Median Greedy

For streaming median or balancing two halves:

```text
Max-heap for lower half
Min-heap for upper half
Balance sizes
```

Not always labeled "greedy," but the local balance rule is greedy.

**Examples:**

- Find Median from Data Stream
- IPO (use max-heap for capital, min-heap for affordable projects)

---

# 10. Greedy Pattern 4 — Farthest Reachable / Jump Layers

```text
Track farthest index reachable
Optionally count layers (jumps)
```

**Examples:**

- Jump Game I & II
- Video stitching (minimum clips to cover [0, T])

```python
def min_clips(time, clips):
    clips.sort()
    end = 0
    clips_used = 0
    i = 0
    while end < time:
        farthest = end
        while i < len(clips) and clips[i][0] <= end:
            farthest = max(farthest, clips[i][1])
            i += 1
        if farthest == end:
            return -1
        clips_used += 1
        end = farthest
    return clips_used
```

---

# 11. Greedy Pattern 5 — Merge Least / Most (Huffman-style)

```text
Repeatedly combine two smallest (or largest) elements
Use min-heap or priority queue
```

**Examples:**

- Huffman coding
- Minimum cost to connect sticks
- Merge stones (some variants)

```python
import heapq

def min_cost_connect_sticks(sticks):
    heapq.heapify(sticks)
    cost = 0
    while len(sticks) > 1:
        a = heapq.heappop(sticks)
        b = heapq.heappop(sticks)
        cost += a + b
        heapq.heappush(sticks, a + b)
    return cost
```

---

# 12. Greedy Pattern 6 — Local Fail-Skip Reset

When a partial segment cannot work, skip entire segment of starts.

**Examples:**

- Gas station
- Some sliding window with monotonic feasibility

```text
If tank < 0 at i starting from s:
    no valid start in [s..i]
    reset start to i+1
```

---

# 13. Greedy Pattern 7 — Frequency / Count Greedy

When answer depends on character or task frequencies:

```text
Count frequencies
Greedily schedule most frequent first
Or check feasibility: max_freq <= (n + cooldown) / ...
```

**Examples:**

- Task Scheduler
- Reorganize String
- Partition Array Such That Maximum Difference Is K (sort + group)

---

# 14. Greedy Pattern 8 — Remove / Keep with Stack or Greedy Scan

```text
Scan left to right
Maintain structure (stack, result string)
Remove element if local condition improves future
```

**Examples:**

- Remove K Digits (monotonic stack — greedy keep smaller leading digits)
- Create Maximum Number (greedy + stack)
- Valid Parenthesis removal variants

```python
def remove_k_digits(num, k):
    stack = []
    for d in num:
        while k and stack and stack[-1] > d:
            stack.pop()
            k -= 1
        stack.append(d)
    return ''.join(stack[:len(stack) - k]).lstrip('0') or '0'
```

---

# 15. Greedy Pattern 9 — Difference Array / Sweep Line Greedy

For overlapping intervals counting coverage:

```text
Events: +1 at start, -1 at end
Sort events, sweep max
```

**Examples:**

- Meeting rooms II
- Car pooling
- My Calendar variants (sometimes segment tree, but sweep for counting)

---

# 16. Greedy Pattern 10 — Exchange Greedy on Arrays

When you can swap or exchange to improve:

```text
Boats to Save People: sort, two pointers (lightest + heaviest)
```

```python
def num_rescue_boats(people, limit):
    people.sort()
    i, j = 0, len(people) - 1
    boats = 0
    while i <= j:
        if people[i] + people[j] <= limit:
            i += 1
        j -= 1
        boats += 1
    return boats
```

Greedy: pair heaviest with lightest when possible.

---

# 17. Greedy Pattern 11 — Prefix / Suffix Greedy

Precompute greedy best from left and right.

**Examples:**

- Candy (two passes: left-to-right, right-to-left)
- Trapping Rain Water (sometimes two-pointer greedy)

```python
def candy(ratings):
    n = len(ratings)
    candies = [1] * n
    for i in range(1, n):
        if ratings[i] > ratings[i - 1]:
            candies[i] = candies[i - 1] + 1
    for i in range(n - 2, -1, -1):
        if ratings[i] > ratings[i + 1]:
            candies[i] = max(candies[i], candies[i + 1] + 1)
    return sum(candies)
```

Not pure one-pass greedy — but each pass is locally greedy with a combine step.

---

# 18. Greedy Pattern 12 — Safe Edge / Cut Property (Graphs)

```text
MST: add minimum weight edge that doesn't create cycle
Shortest path (non-neg): settle closest vertex
```

Recognize graph problems where **local edge/vertex choice is provably safe**.

---

# 19. Greedy Complexity Patterns

| Pattern | Typical Time | Typical Space |
| ------- | ------------ | ------------- |
| Sort + scan | O(n log n) | O(1) or O(n) |
| Heap greedy | O(n log n) | O(n) |
| One-pass scan | O(n) | O(1) |
| Two-pointer on sorted | O(n log n) | O(1) |
| Frequency count + heap | O(n log k) | O(k) |

Always state whether sorting dominates.

---

# 20. Divide-and-Conquer — Start with the Problem

Suppose you want to sort a million numbers.

### Merge Sort idea

```text
Split array in half
Sort each half (recursively)
Merge two sorted halves
```

```text
[38, 27, 43, 3, 9, 82, 10]
        split
[38, 27, 43, 3]     [9, 82, 10]
    split                   split
[38, 27] [43, 3]      [9, 82] [10]
  ... base cases ...
merge back up
[3, 9, 10, 27, 38, 43, 82]
```

Instead of one giant O(n²) bubble sort, you solve **smaller independent subproblems** and combine.

That's Divide-and-Conquer:

```text
DIVIDE    → break into smaller subproblems of same type
CONQUER   → solve subproblems recursively (base case direct)
COMBINE   → merge subproblem answers into full answer
```

---

# 21. The Divide-and-Conquer Paradigm

```text
function solve(problem):
    if problem is small enough:
        return base_case(problem)

    parts = divide(problem)
    results = []
    for part in parts:
        results.append(solve(part))

    return combine(results)
```

Three steps must each be analyzable:

| Step | Question |
| ---- | -------- |
| Divide | How many subproblems? What size? |
| Conquer | Recursion depth? Base case cost? |
| Combine | How expensive is merging? |

**When D&C shines:**

```text
Problem naturally splits into independent halves
Combining is cheaper than solving from scratch
Subproblems do not overlap (or overlap handled differently → DP)
```

**When D&C struggles:**

```text
Subproblems overlap heavily → DP/memoization better
Combine step is expensive
Divide does not reduce problem size meaningfully
```

---

# 22. Merge Sort — Canonical D&C

```python
def merge_sort(arr):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])
    return merge(left, right)

def merge(left, right):
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    result.extend(left[i:])
    result.extend(right[j:])
    return result
```

**Recurrence:**

```text
T(n) = 2T(n/2) + O(n)
```

**Master Theorem → O(n log n)**

```text
Space: O(n) for merge buffer (O(log n) recursion stack)
```

**Stable sort:** Yes (if merge prefers left on tie).

**Properties to understand:**

- Not in-place (typical implementation)
- Predictable O(n log n) worst case
- Excellent for linked lists (natural merge)
- External sorting (disk blocks)

---

# 23. Quicksort — Divide with Smart Pivot

```text
Choose pivot
Partition: elements < pivot | pivot | elements > pivot
Recursively sort left and right partitions
```

```python
def quicksort(arr, lo, hi):
    if lo >= hi:
        return
    p = partition(arr, lo, hi)
    quicksort(arr, lo, p - 1)
    quicksort(arr, p + 1, hi)

def partition(arr, lo, hi):
    pivot = arr[hi]
    i = lo
    for j in range(lo, hi):
        if arr[j] <= pivot:
            arr[i], arr[j] = arr[j], arr[i]
            i += 1
    arr[i], arr[hi] = arr[hi], arr[i]
    return i
```

**Recurrence (balanced pivot):**

```text
T(n) = 2T(n/2) + O(n)  →  O(n log n)
```

**Worst case (bad pivot every time):**

```text
T(n) = T(n-1) + O(n)  →  O(n²)
```

**Mitigations:**

- Random pivot
- Median-of-three
- Introsort (switch to heapsort when depth too large)

**vs Merge Sort:**

| | Merge Sort | Quicksort |
| - | ---------- | --------- |
| Worst time | O(n log n) | O(n²) |
| Average time | O(n log n) | O(n log n) |
| Extra space | O(n) typical | O(log n) stack |
| Stable | Yes | No (typical) |
| Cache behavior | Good | Often excellent |

---

# 24. Binary Search — D&C on Answer Space

Binary search is divide-and-conquer on a **sorted structure** or **monotonic predicate**.

```text
search space [lo, hi]
while lo <= hi:
    mid = (lo + hi) // 2
    if predicate(mid):
        adjust search range
```

```python
def binary_search(arr, target):
    lo, hi = 0, len(arr) - 1
    while lo <= hi:
        mid = (lo + hi) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            lo = mid + 1
        else:
            hi = mid - 1
    return -1
```

**Recurrence:**

```text
T(n) = T(n/2) + O(1)  →  O(log n)
```

**Two major uses:**

1. **Index search** in sorted array
2. **Answer search** — find minimum/maximum value satisfying condition

Examples of (2):

- Koko Eating Bananas
- Capacity to Ship Packages
- Split Array Largest Sum
- Minimum Days to Make Bouquets

**Template for answer BS:**

```python
def min_feasible(lo, hi, feasible):
    while lo < hi:
        mid = (lo + hi) // 2
        if feasible(mid):
            hi = mid
        else:
            lo = mid + 1
    return lo
```

Recognize monotonicity:

```text
If x works → all x' > x work   (minimize x)
If x works → all x' < x work   (maximize x)
```

---

# 25. Maximum Subarray — Divide and Conquer Version

**Kadane's** is the standard O(n) greedy/DP solution. Know the **D&C** version too.

**Idea:** Max subarray is entirely in left, entirely in right, or **crossing** the middle.

```python
def max_subarray_dc(arr, lo, hi):
    if lo == hi:
        return arr[lo]
    mid = (lo + hi) // 2
    left_max = max_subarray_dc(arr, lo, mid)
    right_max = max_subarray_dc(arr, mid + 1, hi)
    cross_max = max_crossing_sum(arr, lo, mid, hi)
    return max(left_max, right_max, cross_max)

def max_crossing_sum(arr, lo, mid, hi):
    left_sum = float('-inf')
    s = 0
    for i in range(mid, lo - 1, -1):
        s += arr[i]
        left_sum = max(left_sum, s)
    right_sum = float('-inf')
    s = 0
    for i in range(mid + 1, hi + 1):
        s += arr[i]
        right_sum = max(right_sum, s)
    return left_sum + right_sum
```

```text
T(n) = 2T(n/2) + O(n)  →  O(n log n)
```

Compare:

```text
Kadane:  O(n) time, O(1) space — prefer in practice
D&C:     O(n log n) — teaches combine step
```

---

# 26. Closest Pair of Points

**Problem:** Given `n` points in plane, find pair with minimum Euclidean distance.

**Brute force:** O(n²)

**D&C:**

```text
1. Sort points by x
2. Split into left and right halves
3. Recursively find closest in each half
4. Combine: check points near the dividing line (strip)
```

```text
T(n) = 2T(n/2) + O(n)  →  O(n log² n) or O(n log n) with careful merge
```

**Key insight:** Only O(n) points in the strip need checking because if closest cross-distance is `d`, at most 6 points per rectangle of size 2d × d (geometric packing argument).

This is a classic "combine step needs clever geometry" example.

---

# 27. Strassen's Matrix Multiplication (Introduction)

Standard matrix multiply: O(n³) for n×n matrices.

**Strassen (1969):** Divide each matrix into four n/2 × n/2 blocks.

```text
Normal block multiply: 8 recursive multiplications

Strassen: 7 recursive multiplications + 18 additions/subtractions
```

```text
T(n) = 7T(n/2) + O(n²)
```

**Master Theorem:**

```text
a=7, b=2, f(n)=n²
log_b(a) = log₂(7) ≈ 2.81 > 2
⇒ T(n) = O(n^log₂(7)) ≈ O(n^2.81)
```

**Practical note:**

```text
Constant factors and numerical stability limit real-world use
Theoretical importance > everyday implementation
Shows divide-and-conquer can beat naive combine
```

Understand the idea; you rarely implement Strassen in interviews.

---

# 28. Fast Exponentiation (Power) — D&C

Compute `x^n` efficiently.

```python
def power(x, n):
    if n == 0:
        return 1
    half = power(x, n // 2)
    if n % 2 == 0:
        return half * half
    else:
        return half * half * x
```

**Iterative (preferred):**

```python
def power_iter(x, n):
    result = 1
    base = x
    exp = n
    while exp > 0:
        if exp & 1:
            result *= base
        base *= base
        exp >>= 1
    return result
```

```text
T(n) = T(n/2) + O(1)  →  O(log n) multiplications
```

**Applications:**

- Modular exponentiation (cryptography)
- Matrix exponentiation for linear recurrences (Fibonacci in O(log n))

---

# 29. Count of Inversions — Merge Sort Combine

**Problem:** Count pairs `(i, j)` where `i < j` and `arr[i] > arr[j]`.

During merge, when taking from right half before left exhausted:

```text
inversions += remaining elements in left half
```

```python
def sort_and_count(arr):
    if len(arr) <= 1:
        return arr, 0
    mid = len(arr) // 2
    left, inv_l = sort_and_count(arr[:mid])
    right, inv_r = sort_and_count(arr[mid:])
    merged, inv_m = merge_count(left, right)
    return merged, inv_l + inv_r + inv_m
```

```text
Time: O(n log n)
```

Classic example where **combine step carries non-trivial work**.

---

# 30. The Master Theorem

For recurrences of the form:

```text
T(n) = aT(n/b) + f(n)

where:
  a ≥ 1  (number of subproblems)
  b > 1  (factor by which size shrinks)
  f(n)    (divide + combine cost)
```

Let:

```text
c = log_b(a)
```

## Case 1 — Leaves dominate

```text
f(n) = O(n^(c - ε)) for some ε > 0

⇒ T(n) = Θ(n^c)
```

Example:

```text
T(n) = 2T(n/2) + O(1)     (merge sort merge if only counted at leaves — actually merge is O(n))
Better example: T(n) = 2T(n/2) + O(n^0.5)  →  Θ(n)
```

## Case 2 — Balanced

```text
f(n) = Θ(n^c log^k n)

⇒ T(n) = Θ(n^c log^(k+1) n)
```

Examples:

```text
T(n) = 2T(n/2) + O(n)       →  Θ(n log n)   [Merge Sort, Quick Sort balanced]
T(n) = 2T(n/2) + O(n log n) →  Θ(n log² n)
```

## Case 3 — Root dominates

```text
f(n) = Ω(n^(c + ε)) for some ε > 0
AND regularity: af(n/b) ≤ kf(n) for some k < 1

⇒ T(n) = Θ(f(n))
```

Example:

```text
T(n) = 2T(n/2) + O(n²)  →  Θ(n²)
```

## Strassen application

```text
T(n) = 7T(n/2) + O(n²)
c = log₂(7) ≈ 2.81
f(n) = n² = O(n^(2.81 - ε))
⇒ T(n) = Θ(n^2.81)
```

## Binary search application

```text
T(n) = T(n/2) + O(1)
a=1, b=2, c=0
f(n)=1 = Θ(n^0 log^0 n)
⇒ T(n) = Θ(log n)
```

## When Master Theorem does NOT apply

```text
T(n) = T(n-1) + O(n)         → arithmetic series → O(n²)
T(n) = 2T(n-1) + O(1)        → O(2^n)
Non-constant a/b across levels
f(n) not polynomially related to n^c
```

Use **recursion tree** or **substitution method** then.

---

# 31. Recursion Tree Method (Supplement)

Draw levels:

```text
Merge Sort T(n) = 2T(n/2) + n

Level 0:        n          work = n
Level 1:    n/2   n/2      work = n
Level 2:  n/4 n/4 n/4 n/4   work = n
...
log n levels, each level total work n

⇒ O(n log n)
```

For:

```text
T(n) = 3T(n/2) + n

Level k work ≈ n * (3/2)^k
Grows until (3/2)^k ≈ n → k ≈ log_{3/2} n
Total ≈ n * (3/2)^k → roughly O(n^log_{3/2}3)
```

Master Theorem is faster when it applies.

---

# 32. D&C Pattern 1 — Split in Half, Merge

```text
Divide array/list into two halves
Recurse on both
Merge sorted/combined result
```

**Examples:**

- Merge sort
- Merge k sorted lists (with heap — hybrid)
- Inversion count

---

# 33. D&C Pattern 2 — Partition Around Pivot

```text
Choose pivot/key
Partition into less / equal / greater
Recurse only on needed side (binary search) or both (quicksort)
```

**Examples:**

- Quicksort
- Quickselect (kth smallest) — average O(n)
- Dutch National Flag

```python
def quickselect(arr, k):
    pivot = arr[len(arr) // 2]
    left = [x for x in arr if x < pivot]
    mid = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    if k <= len(left):
        return quickselect(left, k)
    elif k <= len(left) + len(mid):
        return pivot
    else:
        return quickselect(right, k - len(left) - len(mid))
```

---

# 34. D&C Pattern 3 — Binary Search on Monotonic Predicate

```text
Identify monotonic feasible function
Binary search answer space
```

**Examples:**

- Classic sorted array search
- Minimize maximum (split array, ship capacity)
- Maximize minimum (aggressive cows, magnetic force)

---

# 35. D&C Pattern 4 — Cross-Boundary Combine

Answer may span the divide boundary.

```text
Solve left
Solve right
Solve crossing case explicitly
Take max/min/sum of three
```

**Examples:**

- Maximum subarray (D&C)
- Closest pair of points
- Maximum gap (with constraints)

---

# 36. D&C Pattern 5 — Reduce Problem Size by Constant Factor

```text
T(n) = aT(n-b) + f(n)
```

Not Master Theorem standard form, but still D&C spirit.

**Examples:**

- Euclidean GCD: T(n) = T(n mod m) + O(1)
- Some tree traversals

---

# 37. D&C Pattern 6 — Tree Recursion

Binary trees are natural D&C structures.

```text
solve(node):
    left = solve(node.left)
    right = solve(node.right)
    return combine(node, left, right)
```

**Examples:**

- Tree height
- Count nodes
- Validate BST (with range)
- Diameter of binary tree
- Serialize/deserialize

```text
T(n) = 2T(n/2) + O(1) on balanced tree → O(n)
On skewed tree → O(n) still but recursion depth O(n)
```

---

# 38. D&C Pattern 7 — Geometry / Plane Sweep Hybrid

```text
Sort by one dimension
Divide
Combine checks only local region near split
```

**Examples:**

- Closest pair
- Line intersection (advanced)

---

# 39. D&C Pattern 8 — Fast Power / Repeated Squaring

Halve exponent each step.

**Examples:**

- Power function
- Modular pow
- Matrix exponentiation

---

# 40. D&C Pattern 9 — Karatsuba Multiplication (Concept)

Multiply large integers faster than O(n²) schoolbook method.

```text
Split numbers into high/low parts
3 recursive multiplications instead of 4
T(n) = 3T(n/2) + O(n) → O(n^log₂3) ≈ O(n^1.58)
```

Same family as Strassen — know the pattern exists.

---

# 41. D&C Pattern 10 — Parallel Decomposition (Conceptual)

D&C maps naturally to parallel computing:

```text
Fork subproblems to workers
Join at combine
```

Merge sort and parallel prefix are textbook examples.

Understand **why** D&C is the backbone of parallel algorithms even if you don't implement threads in interviews.

---

# 42. D&C Pattern 11 — Merge Intervals from Sorted Lists

```text
Divide list of lists
Recursively merge pairs
```

Merge k sorted lists:

```text
Pairwise D&C merge: O(N log k) where N = total elements
Heap: O(N log k) as well
```

---

# 43. D&C Pattern 12 — Selection (Order Statistics)

Find kth smallest without full sort.

**Quickselect average:** O(n)

```text
Partition
Recurse only on side containing k
```

Worst case O(n²) — randomized pivot fixes in expectation.

---

# 44. Greedy vs Divide-and-Conquer vs Dynamic Programming

This is the core of Phase 10. Build a decision framework.

```text
                    ┌─────────────────┐
                    │   Problem       │
                    └────────┬────────┘
                             │
              ┌──────────────┼──────────────┐
              ▼              ▼              ▼
        Independent     Overlapping    Local choice
        subproblems     subproblems    seems safe?
              │              │              │
              ▼              ▼              ▼
         Divide &        Dynamic        Greedy
         Conquer         Programming    (if provable)
```

| Dimension | Greedy | Divide & Conquer | Dynamic Programming |
| --------- | ------ | ---------------- | ------------------- |
| Decision style | One choice, no undo | Split, solve, merge | Explore subproblems, store results |
| Subproblem overlap | N/A | Usually none | Critical feature |
| Proof | Exchange / staying ahead | Recurrence / Master | Optimal substructure + memo |
| Typical time | O(n log n), O(n) | O(n log n) common | O(n²), O(n·m) common |
| Backtracking | No | No (unless combine tries all) | Implicit via table |
| Examples | Activity selection, Huffman | Merge sort, closest pair | 0/1 knapsack, LIS |

## Same problem, different paradigms

### Maximum Subarray

```text
Greedy/Kadane:     O(n)
D&C:               O(n log n)
DP (standard):     O(n)  — dp[i] = max(nums[i], dp[i-1]+nums[i])
```

### Coin Change

```text
Greedy:  works only for canonical systems
DP:      always correct for fewest coins
```

### Interval Scheduling

```text
Unweighted max jobs:  Greedy by end time
Weighted max profit:  DP (not greedy)
```

### Matrix Chain Multiplication

```text
Greedy:  FAILS
D&C:     not natural
DP:      correct (choose split point)
```

### Merge Sorted Arrays

```text
Greedy two-pointer:  O(n+m)
D&C merge sort:      O(n log n) for full array
```

## When students confuse them

```text
"Use DP" when problem only needs sort + scan → overkill
"Use Greedy" on 0/1 knapsack → wrong
"Use D&C" when subproblems overlap heavily → exponential without memo
```

Always ask:

```text
Do subproblems overlap?
Does greedy choice have a proof?
Is combine step the bottleneck?
```

---

# 45. Pattern Recognition — Greedy

Train yourself deliberately.

```text
"Maximum non-overlapping intervals"
        ↓
Sort by end + greedy scan
```

```text
"Minimum resources to cover intervals"
        ↓
Sort by start + heap/sweep
```

```text
"Take fractions / ratios"
        ↓
Sort by ratio + greedy fill
```

```text
"Reach end / minimum steps"
        ↓
Farthest reachable / BFS layers greedy
```

```text
"Circular tour / tank"
        ↓
Total sum check + reset on failure
```

```text
"Merge lowest cost repeatedly"
        ↓
Min-heap Huffman pattern
```

```text
"Schedule by frequency"
        ↓
Count + max-heap / cooldown formula
```

```text
"Remove digits/characters for optimal number"
        ↓
Monotonic stack greedy
```

```text
"Pair light with heavy"
        ↓
Sort + two pointers
```

```text
"Safe minimum edge in graph"
        ↓
MST greedy (Kruskal/Prim)
```

```text
"Shortest path non-negative weights"
        ↓
Dijkstra greedy + heap
```

```text
"Does greedy work?"
        ↓
Try counterexample OR prove exchange argument
```

---

# 46. Pattern Recognition — Divide and Conquer

```text
"Sort entire array efficiently"
        ↓
Merge sort / Quicksort
```

```text
"Find in sorted data"
        ↓
Binary search O(log n)
```

```text
"Find optimal value satisfying condition"
        ↓
Binary search on answer (monotonic predicate)
```

```text
"Count pairs with order property"
        ↓
Merge sort inversion count
```

```text
"Subarray crossing middle matters"
        ↓
D&C max subarray
```

```text
"Geometric closest pair"
        ↓
D&C + strip combine
```

```text
"Compute x^n fast"
        ↓
Binary exponentiation
```

```text
"Kth smallest without full sort"
        ↓
Quickselect
```

```text
"Tree problem on subtrees"
        ↓
Recursive D&C on tree
```

```text
"Recurrence T(n)=aT(n/b)+f(n)"
        ↓
Master Theorem / recursion tree
```

```text
"Multiply large matrices faster"
        ↓
Strassen (theoretical)
```

---

# 47. Edge Cases — Greedy

Greedy code often passes happy paths and fails corners.

### Empty input

```text
[]
0 intervals → answer 0
```

### Single element

```text
[1] → jump reachable
One interval → take it
```

### All equal

```text
Ratios tie → any order may work; watch stability requirements
```

### Touching intervals

```text
[1,2] and [2,3]
Clarify: start == last_end → usually non-overlapping (use >=)
```

### Negative values

```text
Gas station with negative net at a station
Jump game with zeros creating traps
```

### Greedy false positive

```text
Always test:
  coins [1,3,4], amount 6
  0/1 knapsack with high ratio but large weight
```

### Integer overflow

```text
Farthest reach: i + nums[i] on large arrays
Binary search mid: use lo + (hi-lo)//2
```

### Circular arrays

```text
Gas station: modulo wrap
```

### Sort stability

```text
When equal keys need deterministic tie-break (e.g., by index)
```

---

# 48. Edge Cases — Divide and Conquer

### Small n

```text
n = 0, n = 1 base cases
```

### Already sorted / reverse sorted

```text
Quicksort worst case without random pivot
```

### Duplicates

```text
Quicksort partition with many equal elements → 3-way partition
Binary search lower/upper bound variants
```

### Integer overflow in mid

```python
mid = lo + (hi - lo) // 2  # not (lo + hi) // 2
```

### Binary search off-by-one

```text
lo <= hi  vs  lo < hi
hi = mid - 1 vs hi = mid
Depends on invariant — pick one template and stick to it
```

### Recursion depth

```text
Skewed tree / quicksort worst case → stack overflow
Use iterative BS or introsort / heap sort
```

### Non-power-of-two sizes

```text
Merge sort mid = n//2 handles odd lengths
```

### Combine step correctness

```text
Max subarray: crossing sum must include mid on both sides
Closest pair: strip filtering must not miss true minimum
```

---

# 49. Important Implementation Skills

## For Greedy

**1. Identify the sort key**

```text
End time? Start time? Ratio? Frequency?
```

**2. State the greedy rule in one sentence**

```text
"Always pick the interval that ends earliest among compatible ones."
```

**3. Try to break it before coding**

```text
2-minute counterexample search
```

**4. Write the loop invariant**

```text
"At each step, the number of selected intervals is maximal for the processed prefix."
```

## For Divide and Conquer

**1. Write the recurrence before coding**

```text
T(n) = 2T(n/2) + O(n)
```

**2. Identify base case clearly**

```text
n <= 1 return directly
```

**3. Analyze combine separately**

```text
Merge is O(n) — dominates? balanced with recursion?
```

**4. Draw recursion tree for unfamiliar recurrences**

---

# 50. Problems to Practice — Greedy

You don't need 100 random problems. Cover each pattern.

### Fundamentals / Proof

- [ ] Activity Selection (classic)
- [ ] Fractional Knapsack
- [ ] Minimum Number of Arrows to Burst Balloons (intervals)

### Intervals

- [ ] Meeting Rooms
- [ ] Meeting Rooms II
- [ ] Non-overlapping Intervals (min removals)
- [ ] Partition Labels

### Reachability / Jumps

- [ ] Jump Game
- [ ] Jump Game II
- [ ] Minimum Number of Taps to Open Water Valve

### Array Greedy

- [ ] Gas Station
- [ ] Assign Cookies
- [ ] Lemonade Change
- [ ] Queue Reconstruction by Height

### Stack Greedy

- [ ] Remove K Digits
- [ ] Create Maximum Number
- [ ] Monotonic stack variants

### Heap Greedy

- [ ] Task Scheduler
- [ ] Reorganize String
- [ ] IPO
- [ ] Minimum Cost to Connect Sticks

### Two Pointer Greedy

- [ ] Boats to Save People
- [ ] Two City Scheduling

### Graph Greedy (connect to Phase 8)

- [ ] Minimum Spanning Tree
- [ ] Dijkstra Shortest Path

### Tricky / Know When NOT Greedy

- [ ] Coin Change (DP)
- [ ] 0/1 Knapsack (DP)
- [ ] Weighted Interval Scheduling (DP)

---
# 51. Problems to Practice — Divide and Conquer

### Sorting / Selection

- [ ] Implement Merge Sort
- [ ] Implement Quicksort
- [ ] Sort an Array (LC — know both)
- [ ] Kth Largest Element (Quickselect)

### Binary Search

- [ ] Binary Search
- [ ] Search Insert Position
- [ ] Find First and Last Position
- [ ] Search in Rotated Sorted Array
- [ ] Median of Two Sorted Arrays (hard D&C)

### Binary Search on Answer

- [ ] Koko Eating Bananas
- [ ] Capacity To Ship Packages Within D Days
- [ ] Split Array Largest Sum
- [ ] Aggressive Cows (classic)

### D&C Combine

- [ ] Count of Inversions
- [ ] Maximum Subarray (D&C version)
- [ ] Reverse Pairs (merge sort variant)

### Power / Math

- [ ] Pow(x, n)
- [ ] Sqrt(x) (binary search)
- [ ] Matrix exponentiation for Fibonacci

### Geometry (stretch)

- [ ] Closest Pair of Points

### Tree D&C

- [ ] Maximum Depth of Binary Tree
- [ ] Diameter of Binary Tree
- [ ] Balanced Binary Tree
- [ ] Construct BST from Preorder (with bounds)

---
# 52. For EVERY Greedy Problem

Use this thinking process:

```text
1. What am I optimizing? (count, sum, feasibility)

2. What is the natural order? (sort key)

3. What is the greedy choice in one sentence?

4. Does a local choice ever block a better global solution?
   → try counterexample

5. Can I prove with exchange argument or staying ahead?

6. After greedy choice, is remaining subproblem same type?

7. What data structure do I need? (none, heap, stack)

8. Time complexity? (sort dominates?)

9. Space complexity?

10. Edge cases:
    empty, single, ties, touching boundaries

11. Is DP required instead? (weights, 0/1, overlapping)

12. Can I explain WHY greedy works in 30 seconds?
```

Question 4 is the most important. If you skip it, you risk AC on samples and WA on hidden tests.

---

# 53. For EVERY Divide-and-Conquer Problem

```text
1. How do I divide? (halves, pivot, predicate)

2. What is the base case?

3. What does combine do? (merge, max of three, add)

4. Write recurrence T(n)

5. Solve via Master Theorem or recursion tree

6. Is combine O(n) or worse?

7. Does overlap exist between subproblems?
   → if yes, consider DP/memo

8. Iterative equivalent? (binary search, heap sort)

9. Worst case vs average case? (quicksort)

10. Space: recursion depth + auxiliary buffers

11. Can greedy or DP be simpler?

12. Draw small example recursion tree
```

---

# 54. Compare Multiple Approaches — Example

## Problem: Maximum Non-overlapping Intervals

### Brute force

```text
Try all 2^n subsets

Time:  O(2^n)
Space: O(n)
```

### Dynamic Programming

```text
Sort by end
dp[i] = max(dp[i-1], 1 + dp[j]) where j is last compatible

Time:  O(n²) or O(n log n) with binary search for j
Space: O(n)
```

### Greedy

```text
Sort by end, pick earliest finishing compatible

Time:  O(n log n)
Space: O(1)
```

Don't just conclude "Greedy wins."

Ask:

```text
Is weighted version needed? → DP
Is online streaming? → different structure
Must we return which intervals? → greedy still works, track choices
```

---

## Problem: Kth Largest Element

### Full sort

```text
Time: O(n log n)
```

### Heap of size k

```text
Time: O(n log k)
```

### Quickselect (D&C)

```text
Average: O(n)
Worst:   O(n²)
```

Pick based on constraints:

```text
k small → heap
need repeated queries → maintain heap or BST
one-shot, memory tight → quickselect
```

---

# 55. One Especially Important Mental Shift

When beginners see a problem, they ask:

```text
"What algorithm name should I use?"
```

Better:

```text
"What structure does this problem have?"
```

Maybe:

```text
Optimal substructure without overlap → D&C
Optimal substructure with overlap → DP
Safe local choice with proof → Greedy
```

Maybe the same problem has **two** valid lenses:

```text
Merge sort: D&C paradigm
Merge step: greedy two-pointer (pick smaller front)
```

```text
Dijkstra: greedy (settle closest)
Also: relaxation resembling DP on DAG if no cycles with negative
```

You're not collecting templates. You're learning to **read problem structure**.

---

# 56. Common Greedy Mistakes

```text
1. Greedy on 0/1 knapsack because fractional works

2. Assuming coin change greedy without canonical proof

3. Wrong sort key (start instead of end for activity selection)

4. Off-by-one on interval boundaries

5. Forgetting to sort before scanning

6. Using greedy when weights exist (weighted intervals)

7. Not checking feasibility before greedy (gas station total sum)

8. Confusing "minimum removals" with "maximum kept" — related but flip logic
```

---

# 57. Common D&C Mistakes

```text
1. Wrong base case (empty array not handled)

2. Mid overflow in binary search

3. Incorrect binary search boundaries (infinite loop)

4. Assuming quicksort is always O(n log n)

5. Forgetting combine step cost in recurrence

6. Using D&C when subproblems overlap (exponential time)

7. Recursion depth overflow on large n

8. Confusing binary search on index vs on answer space
```

---

# 58. Building Intuition — Worked Recognition

### Problem statement snippet → Likely approach

```text
"Find minimum time to complete all tasks with cooldown"
→ Frequency + heap greedy (Task Scheduler)

"Can you reach the last index"
→ Farthest reach greedy (Jump Game)

"Minimum number of coins" (general denominations)
→ DP, NOT greedy

"Minimum number of coins" (standard US coins)
→ Greedy

"Maximum profit scheduling jobs with deadlines and profits"
→ Greedy by deadline (Union-Find or sort) — different from weighted intervals

"Find peak element"
→ Binary search D&C on unsorted-but-structured array

"Count inversions in array"
→ Merge sort D&C combine

"Ship all packages in D days"
→ Binary search on capacity (answer space)
```

---

# 59. Real-World Connections

## Greedy in systems

```text
Load balancing: assign to least-loaded server (greedy)
Huffman coding in compression
MST in network design
Dijkstra in routing (OSPF concepts)
Cache eviction approximations (not always optimal globally)
```

## D&C in systems

```text
Merge sort in external sorting
Binary search in databases and indexes
Parallel map-reduce (split-combine)
FFT (signal processing — advanced D&C)
```

Understanding paradigms helps you recognize when production heuristics are **provably optimal** vs **engineering approximations**.

---

# 60. Implement From Scratch — Learning Path

## Greedy

```text
Version 1: Activity selection
Version 2: Fractional knapsack
Version 3: Huffman coding with heap
Version 4: Jump Game II layer greedy
Version 5: Task Scheduler
```

## Divide and Conquer

```text
Version 1: Merge sort
Version 2: Quicksort + randomized pivot
Version 3: Binary search (index + answer templates)
Version 4: Power function (recursive + iterative)
Version 5: Count inversions
Version 6: Quickselect
```

After each implementation, write the recurrence and solve it.

---

# Complete Greedy & Divide-and-Conquer Checklist

```text
GREEDY & DIVIDE-AND-CONQUER
│
├── Greedy Foundations
│   ├── What greedy means
│   ├── Greedy choice property
│   ├── Optimal substructure
│   ├── No backtracking
│   ├── When greedy works vs fails
│   └── Counterexample hunting
│
├── Greedy Proof Techniques
│   ├── Exchange argument
│   ├── Staying ahead
│   ├── Cut-and-paste / contradiction
│   └── Matroid / safe edge (concept)
│
├── Classic Greedy Problems
│   ├── Activity selection
│   ├── Interval scheduling (max)
│   ├── Meeting rooms (min resources)
│   ├── Fractional knapsack
│   ├── Coin change (greedy cases only)
│   ├── Huffman coding
│   ├── Jump Game I & II
│   ├── Gas station
│   ├── Assign cookies
│   ├── Partition labels
│   ├── Task Scheduler
│   ├── MST (Kruskal/Prim)
│   └── Dijkstra
│
├── Greedy Patterns
│   ├── Sort then scan
│   ├── Earliest finish / latest start
│   ├── Two heaps / median
│   ├── Farthest reachable / jump layers
│   ├── Merge least/most (Huffman)
│   ├── Local fail-skip reset
│   ├── Frequency / count greedy
│   ├── Stack / remove-k greedy
│   ├── Sweep line / difference events
│   ├── Exchange two-pointer
│   ├── Prefix/suffix passes
│   └── Safe edge (graphs)
│
├── Divide & Conquer Foundations
│   ├── Divide → Conquer → Combine
│   ├── Base cases
│   ├── Recurrence writing
│   ├── When D&C applies
│   └── Overlap → DP instead
│
├── Classic D&C Algorithms
│   ├── Merge sort
│   ├── Quicksort (+ pivot strategies)
│   ├── Binary search (index)
│   ├── Binary search (answer space)
│   ├── Maximum subarray (D&C)
│   ├── Closest pair of points
│   ├── Strassen (intro)
│   ├── Fast exponentiation
│   ├── Count inversions
│   └── Quickselect
│
├── Master Theorem
│   ├── Form T(n) = aT(n/b) + f(n)
│   ├── Compute c = log_b(a)
│   ├── Case 1: leaves dominate
│   ├── Case 2: balanced
│   ├── Case 3: root dominates
│   ├── Merge sort application
│   ├── Binary search application
│   ├── Strassen application
│   └── When theorem does not apply
│
├── D&C Patterns
│   ├── Split half + merge
│   ├── Partition around pivot
│   ├── Binary search monotonic
│   ├── Cross-boundary combine
│   ├── Tree recursion
│   ├── Geometry strip combine
│   ├── Fast power
│   ├── Karatsuba (concept)
│   ├── Parallel decomposition (concept)
│   ├── Merge k lists D&C
│   └── Order statistics / quickselect
│
├── Paradigm Comparison
│   ├── Greedy vs DP
│   ├── D&C vs DP
│   ├── Greedy vs D&C
│   ├── Same problem multiple lenses
│   └── Decision flowchart
│
├── Pattern Recognition
│   ├── Greedy signals
│   ├── D&C signals
│   └── When to reject greedy
│
├── Edge Cases
│   ├── Empty / single element
│   ├── Interval boundaries
│   ├── Greedy counterexamples
│   ├── BS off-by-one
│   ├── Mid overflow
│   ├── Recursion depth
│   └── Duplicates / ties
│
├── Analysis Skills
│   ├── Sorting cost in greedy
│   ├── Recurrence trees
│   ├── Master Theorem
│   ├── Average vs worst (quicksort)
│   └── Space (stack + auxiliary)
│
├── Implement Yourself
│   ├── Activity selection
│   ├── Fractional knapsack
│   ├── Huffman with heap
│   ├── Merge sort
│   ├── Quicksort
│   ├── Binary search templates
│   ├── Pow(x,n)
│   ├── Inversion count
│   └── Quickselect
│
└── For EVERY Problem
    ├── Identify optimization goal
    ├── Check overlapping subproblems
    ├── Try greedy proof or counterexample
    ├── Write D&C recurrence if applicable
    ├── Compare greedy / D&C / DP
    ├── Choose correct sort key or predicate
    ├── Analyze time and space
    ├── Test edge cases
    └── Explain WHY in 30 seconds
```

---

For your **core DSA pass**, this is enough Greedy and Divide-and-Conquer depth. You don't need to master every advanced geometric D&C algorithm, every matroid class, or industrial compression pipelines — but you **must** be able to:

1. Prove or disprove greedy with exchange arguments and counterexamples
2. Implement merge sort, quicksort, and binary search from scratch
3. Write and solve recurrences with the Master Theorem
4. Recognize when Phase 9 DP replaces greedy or naive D&C
5. Explain the same problem through multiple paradigm lenses

Once you can do that, you're ready for **Phase 11: Backtracking** — where you deliberately explore choices instead of committing greedily or splitting blindly.
