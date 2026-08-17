# Dynamic Programming

## 1. Dynamic Programming Fundamentals

First understand **why Dynamic Programming exists**.

Many problems are naturally expressed as recursion:

```text
solve(problem)
    ↓
break into smaller subproblems
    ↓
combine answers
```

Example — Fibonacci:

```text
fib(5)
 ↓
fib(4) + fib(3)
 ↓
fib(3) + fib(2) + fib(2) + fib(1)
```

Notice:

```text
fib(3) computed multiple times
fib(2) computed many times
```

That is the first key idea:

> **Overlapping subproblems**

The second key idea:

> **Optimal substructure**

If the best answer to the whole problem is built from the best answers to its subproblems, you can reuse those subproblem answers instead of recomputing them.

```text
DP = Recursion
   + Overlapping subproblems
   + Optimal substructure
   + Remembering subproblem results
```

Without overlapping subproblems, plain recursion may be fine.

Without optimal substructure, you cannot safely combine subproblem answers into a global optimum.

---

# 2. The Problem DP Solves

Suppose you want the minimum cost path in a grid.

Brute recursion explores many paths repeatedly:

```text
start → right → down → ...
start → down → right → ...
```

Different routes may revisit the same cell with the same remaining objective.

If reaching cell `(i, j)` with the same constraints always has the same optimal cost, then:

```text
cost(i, j) = min(
    cost(i-1, j) + grid[i][j],
    cost(i, j-1) + grid[i][j]
)
```

Once `cost(i, j)` is known, never recompute it.

That is DP.

Compare:

| Approach | Idea | Typical issue |
| -------- | ---- | ------------- |
| Brute recursion | Try all possibilities | Exponential recomputation |
| Memoization | Cache subproblem results | Top-down, lazy |
| Tabulation | Fill table bottom-up | Bottom-up, often clearer order |

---

# 3. Two Required Properties

## Overlapping Subproblems

The same subproblem appears many times in the recursion tree.

```text
                    fib(5)
                   /      \
              fib(4)        fib(3)
             /    \        /    \
        fib(3) fib(2) fib(2) fib(1)
```

`fib(3)` and `fib(2)` repeat.

Recognition clue:

> Does the recursion tree contain duplicate nodes?

If yes, memoization or tabulation helps.

If no — for example merge sort subarrays are disjoint — DP caching is usually unnecessary.

---

## Optimal Substructure

The optimal solution to the problem contains optimal solutions to subproblems.

Example — shortest path:

```text
If shortest path from A to C goes through B,
then the A→B portion must itself be shortest.
```

Counterexample — longest simple path in a general graph:

```text
Longest path A → ... → B → ... → C
```

The subpath inside may not be longest by itself because revisiting nodes matters.

So not every recursive problem is DP.

---

# 4. Memoization vs Tabulation

Both store subproblem answers.

They differ in **direction** and **control flow**.

## Memoization — Top-Down

Start from the original problem.

Recursively solve subproblems.

Before computing, check cache.

After computing, store result.

```cpp
unordered_map<int, int> memo;

int fib(int n) {
    if (n <= 1) return n;
    if (memo.count(n)) return memo[n];
    memo[n] = fib(n - 1) + fib(n - 2);
    return memo[n];
}
```

Mental model:

```text
Recursion tree
     +
Cache lookup/store
```

Advantages:

```text
Only computes needed subproblems
Often easier to write from recursive definition
Natural for sparse state spaces
```

Disadvantages:

```text
Recursion stack overhead
Cache lookup overhead
Sometimes harder to optimize space
```

---

## Tabulation — Bottom-Up

Determine dependency order.

Fill base cases first.

Iterate until full answer is known.

```cpp
int fib(int n) {
    if (n <= 1) return n;
    vector<int> dp(n + 1);
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}
```

Mental model:

```text
Table
 ↓
Fill in valid order
 ↓
Answer at target cell/index
```

Advantages:

```text
No recursion stack
Often easier to reason about iteration order
Often easier to compress space to O(1) or O(n)
```

Disadvantages:

```text
Must figure out evaluation order
May compute unused states
```

---

## When to Prefer Which

```text
State space sparse / hard to order
        ↓
Memoization

State space dense / regular grid
        ↓
Tabulation

Need maximum performance + tight space
        ↓
Tabulation + space compression
```

Often both are equivalent in time.

Choose based on clarity and constraints.

---

# 5. The DP Design Framework

For every DP problem, walk through this pipeline.

## Step 1 — Define the State

What uniquely identifies a subproblem?

Examples:

```text
i                     → first i elements
i, j                  → two prefixes / grid cell
i, j, k               → three dimensions
l, r                  → interval [l, r]
mask                  → subset bitmask
state in machine      → buy/sell/hold/cooldown
```

Ask:

> What information must I know to continue solving from here?

Too little state → wrong answer.

Too much state → slow.

---

## Step 2 — Define the Recurrence

How does `dp[state]` relate to smaller states?

Example — climbing stairs:

```text
dp[i] = dp[i-1] + dp[i-2]
```

Example — 0/1 knapsack:

```text
dp[i][w] = max(
    dp[i-1][w],
    dp[i-1][w - weight[i]] + value[i]   if w >= weight[i]
)
```

The recurrence is the heart of DP.

---

## Step 3 — Identify Base Cases

```text
dp[0] = ...
dp[i][0] = ...
empty interval = ...
```

Base cases must be correct.

Most DP bugs are wrong bases or wrong transition boundaries.

---

## Step 4 — Determine Computation Order

Memoization: recursion naturally respects dependencies.

Tabulation: fill so dependencies are already computed.

```text
1D forward: i from 0 to n
2D: increasing i, j
Interval DP: increasing length, or decreasing l
```

---

## Step 5 — Extract the Answer

Sometimes answer is `dp[n]`.

Sometimes answer is `max(dp[i])` over all i.

Sometimes answer is `dp[0][n-1]`.

Read the problem carefully.

---

## Step 6 — Analyze Complexity

```text
Time  ≈ number of states × work per transition
Space ≈ number of states stored
```

Then ask:

> Can I reduce dimensions?
> Can I keep only previous row/layer?

---

# 6. 1D Dynamic Programming

State depends on one parameter, often an index.

```text
dp[i] = best answer for prefix ending at i
        or
dp[i] = best answer using first i items
```

Classic examples:

```text
Fibonacci
Climbing Stairs
House Robber
Decode Ways
Maximum Subarray (Kadane is DP)
```

Template:

```cpp
vector<int> dp(n);
dp[0] = base;

for (int i = 1; i < n; i++) {
    dp[i] = best over valid previous states;
}
return dp[n - 1]; // or max/min over dp
```

Visualization:

```text
index:  0   1   2   3   4
dp:    [b] [ ] [ ] [ ] [ ]
              ↑
        depends on earlier cells
```

---

# 7. 2D Dynamic Programming

State has two parameters.

Common forms:

## Two sequences

```text
dp[i][j] = answer for A[0..i-1] and B[0..j-1]
```

Examples: LCS, Edit Distance.

## Grid

```text
dp[i][j] = answer at cell (i, j)
```

Examples: Unique Paths, Minimum Path Sum.

## Knapsack

```text
dp[i][w] = best using first i items with capacity w
```

Table mental model:

```text
     j →
i
↓
```

Fill row by row or with rolling array.

---

# 8. State Machine DP

Some problems are not "index only."

They need **phase** or **mode**.

Example — stock with cooldown:

```text
States:
  hold    → currently holding stock
  sold    → just sold today (cooldown)
  rest    → not holding, can buy
```

Transitions:

```text
rest → buy → hold
hold → sell → sold
sold → rest
```

Each day updates all states from previous day states.

```cpp
int hold = -prices[0];
int sold = 0;
int rest = 0;

for (int i = 1; i < n; i++) {
    int prevHold = hold;
    int prevSold = sold;
    int prevRest = rest;
    hold = max(prevHold, prevRest - prices[i]);
    sold = prevHold + prices[i];
    rest = max(prevRest, prevSold);
}
```

Recognition clue:

> Problem has forbidden immediate transitions or multiple "modes" of being.

Think state machine, not just index.

---

# 9. Classic Problem — Fibonacci

The teaching problem.

Recurrence:

```text
F(0) = 0
F(1) = 1
F(n) = F(n-1) + F(n-2)
```

Naive recursion:

```text
Time:  O(2^n)   exponential
Space: O(n)     recursion depth
```

Memoization / tabulation:

```text
Time:  O(n)
Space: O(n)     or O(1) with two variables
```

Space-optimized tabulation:

```cpp
int fib(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}
```

Lesson:

> DP is not a separate algorithm. It is recursion with reuse.

---

# 10. Classic Problem — Climbing Stairs

You can climb 1 or 2 steps.

How many distinct ways to reach top?

```text
n = 4

1+1+1+1
1+1+2
1+2+1
2+1+1
2+2
```

Same recurrence as Fibonacci:

```text
ways(n) = ways(n-1) + ways(n-2)
```

State:

```text
dp[i] = ways to reach step i
```

Base:

```text
dp[0] = 1   (empty way to stay at ground)
dp[1] = 1
```

This pattern generalizes to:

```text
dp[i] = sum of dp[i - step] for all valid steps
```

Used in Decode Ways and similar counting problems.

---

# 11. Classic Problem — 0/1 Knapsack

Given weights and values, each item used **at most once**, maximize value with capacity `W`.

```text
items:
  w: [1, 3, 4, 5]
  v: [1, 4, 5, 7]
capacity W = 7
```

State:

```text
dp[i][w] = max value using items 1..i with capacity w
```

Recurrence:

```text
don't take item i:
  dp[i][w] = dp[i-1][w]

take item i (if enough capacity):
  dp[i][w] = dp[i-1][w - weight[i]] + value[i]

dp[i][w] = max(don't take, take)
```

Table sketch:

```text
capacity w: 0 1 2 3 4 5 6 7
item 0       0 0 0 0 0 0 0 0
item 1       0 1 1 1 1 1 1 1
item 2       ...
```

Space optimization — only previous row needed:

```cpp
vector<int> dp(W + 1, 0);
for (int i = 0; i < n; i++) {
    for (int w = W; w >= weight[i]; w--) {
        dp[w] = max(dp[w], dp[w - weight[i]] + value[i]);
    }
}
```

**Critical:** iterate `w` backwards when using one row.

Otherwise you use the same item more than once.

---

# 12. Classic Problem — Unbounded Knapsack

Each item can be used unlimited times.

Examples:

```text
Coin Change (minimum coins)
Rod Cutting
```

Same state idea, different iteration:

```cpp
for (int w = 1; w <= W; w++) {
    for (int i = 0; i < n; i++) {
        if (w >= weight[i]) {
            dp[w] = min(dp[w], dp[w - weight[i]] + 1);
        }
    }
}
```

Forward iteration on `w` because same item may be reused.

Compare:

| Variant | Item usage | Capacity loop direction (1-row) |
| ------- | ---------- | --------------------------------- |
| 0/1 | once | backwards |
| unbounded | unlimited | forwards |

This distinction is interview-critical.

---

# 13. Classic Problem — Longest Common Subsequence (LCS)

```text
A = "abcde"
B = "ace"

LCS = "ace", length 3
```

State:

```text
dp[i][j] = LCS length of A[0..i-1] and B[0..j-1]
```

Recurrence:

```text
if A[i-1] == B[j-1]:
    dp[i][j] = dp[i-1][j-1] + 1
else:
    dp[i][j] = max(dp[i-1][j], dp[i][j-1])
```

Visualization:

```text
     a c e
   +---------
 a | 1 1 1
 b | 1 1 1
 c | 1 2 2
 d | 1 2 2
 e | 1 2 3
```

Base:

```text
dp[0][j] = 0
dp[i][0] = 0
```

Complexity:

```text
Time:  O(n * m)
Space: O(n * m), or O(min(n,m)) with rolling row
```

Variants:

```text
Longest Common Substring → reset to 0 on mismatch
Edit Distance → different cost on mismatch
Delete Operation for Two Strings → n + m - 2*LCS
```

---

# 14. Classic Problem — Longest Increasing Subsequence (LIS)

```text
[10, 9, 2, 5, 3, 7, 101, 18]

LIS = [2, 3, 7, 101], length 4
```

## O(n²) DP

```text
dp[i] = length of LIS ending at index i

dp[i] = 1 + max(dp[j]) for all j < i where nums[j] < nums[i]
```

## O(n log n) — patience sorting / binary search

Maintain smallest tail of each length.

```cpp
vector<int> tails;
for (int x : nums) {
    auto it = lower_bound(tails.begin(), tails.end(), x);
    if (it == tails.end()) tails.push_back(x);
    else *it = x;
}
return tails.size();
```

Know both.

The O(n²) version is pure DP.

The optimized version is important for scale.

Variants:

```text
LIS with differences at most k
Number of LIS
Russian Doll Envelopes → sort + LIS on second dimension
```

---

# 15. Classic Problem — Edit Distance (Levenshtein)

Transform `word1` into `word2` with minimum operations:

```text
insert
delete
replace
```

State:

```text
dp[i][j] = min edits for word1[0..i-1] → word2[0..j-1]
```

Recurrence:

```text
if word1[i-1] == word2[j-1]:
    dp[i][j] = dp[i-1][j-1]
else:
    dp[i][j] = 1 + min(
        dp[i-1][j],      // delete from word1
        dp[i][j-1],      // insert into word1
        dp[i-1][j-1]    // replace
    )
```

Base cases:

```text
dp[i][0] = i   (delete all i chars)
dp[0][j] = j   (insert all j chars)
```

This is one of the most important 2D DP templates.

Many string DP problems are variations.

---

# 16. Classic Problem — Coin Change

```text
coins = [1, 2, 5]
amount = 11

answer = 3   (5 + 5 + 1)
```

Minimum coins — unbounded knapsack style:

```text
dp[a] = min coins to make amount a

dp[a] = min(dp[a - c] + 1) for each coin c <= a
```

Count number of combinations:

```text
dp[a] += dp[a - c]
```

Watch wording:

```text
"number of ways" → counting DP
"minimum number" → min DP
"can you make"     → boolean DP / reachability
```

Initialize carefully:

```text
min:  dp[0] = 0, others INF
ways: dp[0] = 1
```

---

# 17. Classic Problem — Grid Paths

## Unique Paths

Move only right or down.

```text
dp[i][j] = dp[i-1][j] + dp[i][j-1]
```

## Minimum Path Sum

```text
dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1])
```

## With obstacles

If blocked, `dp[i][j] = 0` or skip.

Space trick — one row:

```cpp
vector<int> dp(cols, 0);
dp[0] = 1;
for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        if (obstacle) dp[j] = 0;
        else if (j > 0) dp[j] += dp[j-1];
    }
}
```

Variants:

```text
Dungeon Game
Maximal Square
Falling Path Sum
Cherry Pickup → harder state expansion
```

---

# 18. Interval Dynamic Programming

Subproblem defined on interval `[i, j]`.

```text
dp[l][r] = best answer for subarray/substring from l to r
```

Often fill by **increasing interval length**.

Classic — Matrix Chain Multiplication:

```text
dp[l][r] = min cost to multiply matrices from l to r
```

Classic — Burst Balloons:

```text
dp[l][r] = max coins bursting all balloons in (l, r)
```

Classic — Palindrome Partitioning / Minimum cuts:

```text
dp[l][r] = min cuts for substring s[l..r]
```

Template:

```cpp
for (int len = 1; len <= n; len++) {
    for (int l = 0; l + len - 1 < n; l++) {
        int r = l + len - 1;
        if (len == 1) dp[l][r] = base;
        else {
            dp[l][r] = ... combine smaller intervals ...
        }
    }
}
```

Recognition clue:

> "Split / merge / burst / partition a contiguous range optimally."

---

# 19. Bitmask DP — Introduction

State includes a bitmask representing a **subset**.

```text
mask bit i = 1  → element i is included / visited / used
mask bit i = 0  → not included
```

Number of states:

```text
O(2^n * n)   often
```

Classic — Traveling Salesman (TSP):

```text
dp[mask][i] = min cost to visit set mask ending at city i
```

Classic — Assignment / matching small n.

Template sketch:

```cpp
int FULL = (1 << n) - 1;
vector<vector<int>> dp(1 << n, vector<int>(n, INF));
dp[1][0] = 0; // start at city 0

for (int mask = 0; mask < (1 << n); mask++) {
    for (int u = 0; u < n; u++) {
        if (!dp[mask][u]) continue;
        for (int v = 0; v < n; v++) {
            if (mask & (1 << v)) continue;
            int next = mask | (1 << v);
            dp[next][v] = min(dp[next][v], dp[mask][u] + dist[u][v]);
        }
    }
}
```

When n ≤ 20, think bitmask.

When problem says "visit all exactly once" on small set, strong signal.

---

# 20. DP Pattern 1 — Linear 1D Recurrence

**Recognition:**

```text
Answer for position i depends on few previous positions
```

Examples:

```text
Fibonacci / Climbing Stairs
House Robber
Maximum Sum Subarray (Kadane)
Decode Ways
```

Pattern:

```cpp
for (int i = 0; i < n; i++) {
    dp[i] = combine(dp[i-1], dp[i-2], ...);
}
```

Edge cases:

```text
i = 0
i = 1
empty input
```

Often compress to O(1) space.

---

# 21. DP Pattern 2 — Take or Skip (0/1 Choice)

**Recognition:**

```text
Each item/element used at most once
```

Examples:

```text
0/1 Knapsack
Subset Sum
Partition Equal Subset Sum
Target Sum (with +/- assignment)
```

Pattern:

```text
dp[i][...] from dp[i-1][...]  (previous row)
```

Or backward capacity loop on one row.

Key invariant:

> When processing item i, `dp[w]` still represents state **before** considering item i, then we update.

---

# 22. DP Pattern 3 — Unbounded / Repeated Choice

**Recognition:**

```text
Same choice can be taken multiple times
```

Examples:

```text
Coin Change (min)
Coin Change 2 (ways)
Integer Break
Perfect Squares
```

Pattern:

```text
Forward loop on capacity/amount
dp[x] updated from dp[x - cost]
```

Difference from 0/1 is **iteration direction** and whether state index means "prefix of items."

---

# 23. DP Pattern 4 — Two Sequence 2D DP

**Recognition:**

```text
Two strings or two sequences compared / aligned
```

Examples:

```text
LCS
Edit Distance
Interleaving String
Delete Operation for Two Strings
Shortest Common Supersequence
```

Pattern:

```text
dp[i][j] from dp[i-1][j], dp[i][j-1], dp[i-1][j-1]
```

Draw the 2D table.

Trace back for reconstruction when needed.

---

# 24. DP Pattern 5 — Grid Path DP

**Recognition:**

```text
Move on matrix with local transitions
```

Examples:

```text
Unique Paths
Minimum Path Sum
Dungeon Game
Maximal Square
```

Pattern:

```text
dp[i][j] from neighbors (usually top and left)
```

Watch:

```text
obstacles
modulo for count problems
min vs max
initial row/column base cases
```

---

# 25. DP Pattern 6 — Interval DP

**Recognition:**

```text
Optimal answer on contiguous subarray/substring
Split or merge intervals
```

Examples:

```text
Burst Balloons
Matrix Chain Multiplication
Palindrome Partitioning II
Stone Game variants
```

Pattern:

```cpp
for (len = 1 to n)
  for (l = 0 to n-len)
    r = l + len - 1
    try all split points k in [l, r]
```

Complexity often O(n³).

---

# 26. DP Pattern 7 — State Machine DP

**Recognition:**

```text
Multiple modes / forbidden transitions
Buy-sell-cooldown
Rolling window constraints on actions
```

Examples:

```text
Best Time to Buy and Sell Stock with Cooldown
Best Time with Transaction Fee
Paint House (no adjacent same color)
```

Pattern:

```text
dp[day][state] or parallel variables per state
update all states each step from previous states
```

Do not collapse into one variable too early.

---

# 27. DP Pattern 8 — Prefix Optimization on DP

**Recognition:**

```text
Transition is max/min over range of prior dp values
Naive transition O(n), can optimize to O(log n) or O(1)
```

Examples:

```text
LIS with binary search
Maximum Subarray with deque (less common)
Convex hull trick (advanced)
Monotonic queue optimization on grid
```

Pattern:

> Improve transition, not state count.

Still DP if optimal substructure holds.

---

# 28. DP Pattern 9 — Counting DP

**Recognition:**

```text
"How many ways"
"Number of distinct"
```

Examples:

```text
Climbing Stairs
Decode Ways
Unique BSTs
Coin Change 2
Domino Tiling
```

Pattern:

```text
dp[i] = sum of dp[previous valid states]
```

Use `long long` or modulo when counts explode.

Initialize `dp[0]` correctly:

```text
often 1 for empty case in counting
```

---

# 29. DP Pattern 10 — Bitmask / Subset DP

**Recognition:**

```text
n is small (≤ 20)
Visit all / assign all / subset selection
```

Examples:

```text
TSP
Partition to K Equal Sum Subsets (n small)
Maximum Students Taking Exam (with bitmask rows)
```

Pattern:

```text
dp[mask] or dp[mask][i]
enumerate set bits / try adding one element
```

State count exponential — only for small n.

---

# 30. DP Pattern 11 — Tree DP

**Recognition:**

```text
DP on tree nodes
Answer for subtree
```

Examples:

```text
House Robber III
Binary Tree Maximum Path Sum
Diameter of Binary Tree (can be done without table)
```

Pattern:

```cpp
pair<int,int> dfs(Node* node) {
    if (!node) return {0, 0};
    auto [leftWith, leftWithout] = dfs(node->left);
    auto [rightWith, rightWithout] = dfs(node->right);
    int with = node->val + leftWithout + rightWithout;
    int without = max({leftWith, leftWithout, rightWith, rightWithout});
    return {with, without};
}
```

Post-order traversal naturally solves subtree states.

---

# 31. DP Pattern 12 — Digit DP (Brief Intro)

**Recognition:**

```text
Count numbers in range [L, R] with digit constraints
```

Examples:

```text
Numbers At Most N Digit Sum
Count Numbers with Unique Digits (related)
```

Pattern:

```text
dp[pos][tight][...] on digit position
tight = whether still matching upper bound prefix
```

Advanced topic — know it exists for "count in range" problems.

---

# 32. How to Identify DP Problems

Ask these questions:

## Q1 — Can I define recursive subproblems?

If natural recursion exists, continue.

## Q2 — Do subproblems repeat?

Draw recursion tree or reason about state collision.

## Q3 — Does optimal substructure hold?

Can I build global optimum from local optima?

## Q4 — Is brute recursion exponential?

DP often reduces to polynomial in state count.

## Q5 — Are choices sequential or with constraints?

```text
sequential decisions + reuse → DP candidate
```

## Q6 — Is it optimization or counting on structured choices?

```text
min/max + structured choices → DP candidate
count ways + structured choices → DP candidate
```

---

## Strong Signals

```text
"minimum number of ..."
"maximum profit ..."
"how many ways ..."
"longest / shortest ..."
"can you partition ..."
"with capacity / limit ..."
"two strings / sequences ..."
"grid paths ..."
"burst / merge / split interval ..."
"small n, visit all ..."
```

---

## Weak or Negative Signals

```text
Greedy works with exchange argument → maybe not DP
All subproblems disjoint → divide and conquer, not DP
Need global non-local structure without state → harder
n huge but no clear compact state → may not be DP
```

---

## DP vs Greedy vs Divide and Conquer

| Technique | When | Example |
| --------- | ---- | ------- |
| Divide & Conquer | Independent subproblems | Merge sort |
| Greedy | Local choice is globally safe | Activity selection |
| DP | Overlap + optimal substructure | Knapsack |

Phase 10 in your roadmap compares Greedy and DP deliberately.

When unsure, try greedy proof.

If greedy fails, look for DP state.

---

# 33. Pattern Recognition

Train deliberate recognition.

```text
"Depends on previous few indices"
        ↓
1D Linear DP
```

```text
"Use item once"
        ↓
0/1 Knapsack / Take-Skip
```

```text
"Reuse same item"
        ↓
Unbounded Knapsack
```

```text
"Two strings"
        ↓
2D Sequence DP
```

```text
"Grid movement"
        ↓
Grid DP
```

```text
"Contiguous range optimal"
        ↓
Interval DP
```

```text
"Buy / sell / cooldown / states"
        ↓
State Machine DP
```

```text
"How many ways"
        ↓
Counting DP
```

```text
"n ≤ 20, all subsets"
        ↓
Bitmask DP
```

```text
"Tree subtree answer"
        ↓
Tree DP
```

```text
"Count numbers with digit property"
        ↓
Digit DP
```

Notice:

> Advanced problems compose patterns.

Example — Cherry Pickup:

```text
Grid DP + state expansion (two agents)
```

Example — Russian Doll Envelopes:

```text
Sort + LIS
```

---

# 34. Edge Cases — Extremely Important for DP

DP bugs often come from boundaries, not logic.

Always test:

```text
Empty input
```

```text
Single element
```

```text
n = 1
m = 1
```

```text
All zeros
```

```text
All same values
```

```text
Impossible case → return -1 or 0 correctly
```

```text
Large answer → modulo or long long
```

```text
Capacity / amount = 0
```

```text
Target sum = 0
```

```text
Both strings empty
```

```text
One string empty
```

```text
Grid 1x1
```

```text
Only obstacles in grid
```

```text
Negative numbers (if allowed) — min path, kadane
```

```text
Overflow in intermediate DP values
```

---

## Boundary Index Errors

Common mistakes:

```text
dp[i-1] when i = 0
dp[i][j-1] when j = 0
wrong loop start/end
off-by-one in prefix definition
```

Define clearly:

```text
dp[i] means first i elements (0..i-1)
OR
dp[i] means ending at index i
```

Stick to one convention per problem.

---

## Initialization Mistakes

```text
min DP: start with INF, dp[0] = 0
max DP: start with 0 or -INF carefully
counting: dp[0] = 1 often
boolean: dp[0] = true for empty reachability
```

Wrong init → subtle wrong answers, not crashes.

---

# 35. Space Optimization

DP memory often reducible.

## 1D rolling

```text
only need dp[i-1] and dp[i-2]
→ two variables
```

## 2D knapsack

```text
only need previous item row
→ one dimension array, backward loop
```

## 2D grid

```text
only need previous row
→ vector<int> dp(cols)
```

## 2D sequence

```text
if only dp[i][j] needed from previous row/col
→ rolling row O(min(n,m))
```

Rule:

> Identify dependency direction, keep only required layer.

---

# 36. Complexity Analysis

General formula:

```text
Time  = O(states × transitions per state)
Space = O(states)   or optimized
```

Examples:

| Problem | States | Transitions | Time |
| ------- | ------ | ----------- | ---- |
| Fibonacci | n | O(1) | O(n) |
| 0/1 Knapsack | n × W | O(1) | O(nW) |
| LCS | n × m | O(1) | O(nm) |
| LIS | n | O(n) naive | O(n²) |
| Interval DP | n² | O(n) splits | O(n³) |
| Bitmask TSP | 2^n × n | O(n) | O(n² 2^n) |

Pseudo-polynomial:

```text
O(nW) depends on numeric value W, not just input size
```

Important for theory, less for interviews.

---

# 37. Reconstructing the Solution

Sometimes need actual path/items, not just optimal value.

Techniques:

## Traceback on table

```cpp
if (dp[i][j] == dp[i-1][j]) {
  // came from i-1, j
} else {
  // took item i
}
```

## Store parent pointers

```text
parent[i][j] = which transition won
```

## Reverse build

For LCS, follow matches diagonally.

For knapsack, walk items backward.

Practice at least one reconstruction per major pattern.

---

# 38. DP vs Memoization in Production

Interview DP is algorithmic.

Production "memoization" often means caching expensive function results.

Same idea:

```text
key → computed result
```

Examples:

```text
API response cache
Fibonacci-style repeated queries
Dynamic programming in route planning (with constraints)
```

Difference:

> Algorithmic DP state is structured and bounded by problem parameters.

> Production cache keys may be arbitrary and need eviction (LRU).

Conceptual link helps, but don't confuse hash-map memo with DP table design.

---

# 39. Common Mistakes

```text
1. Using forward capacity loop in 0/1 knapsack one-row
2. Wrong base cases
3. Confusing "first i items" vs "ending at i"
4. Forgetting modulo on counting problems
5. Returning dp[n] when answer is max over all dp[i]
6. Not handling impossible → INF left in answer
7. Recursion depth overflow without memo on large n
8. Building greedy when DP is required
9. State too small → invalid optimal substructure assumption
10. State too large → TLE/MLE
```

---

# 40. Implement DP Yourself — Learning Path

## Version 1 — Fibonacci

```text
naive recursion
memoization
tabulation
O(1) space
```

## Version 2 — Climbing Stairs / Decode Ways

```text
counting 1D
```

## Version 3 — 0/1 Knapsack

```text
2D table
1D optimized
```

## Version 4 — LCS + Edit Distance

```text
2D string DP
traceback
```

## Version 5 — Coin Change both variants

```text
min and counting
unbounded
```

## Version 6 — Interval DP

```text
Burst Balloons or Matrix Chain
```

## Version 7 — Bitmask TSP (small n)

```text
subset enumeration
```

Once you've implemented these, DP pattern recognition becomes much faster.

---

# 41. Representative Problems

You do **not** need 100 DP problems.

Choose problems representing each pattern.

## 1D Linear

- [ ] Climbing Stairs
- [ ] House Robber
- [ ] House Robber II (circular — twist)
- [ ] Maximum Subarray
- [ ] Decode Ways

## 0/1 Knapsack family

- [ ] Partition Equal Subset Sum
- [ ] Target Sum
- [ ] Ones and Zeroes
- [ ] Last Stone Weight II

## Unbounded

- [ ] Coin Change
- [ ] Coin Change 2
- [ ] Integer Break
- [ ] Perfect Squares

## Two Sequence

- [ ] Longest Common Subsequence
- [ ] Edit Distance
- [ ] Delete Operation for Two Strings
- [ ] Shortest Common Supersequence
- [ ] Interleaving String

## LIS family

- [ ] Longest Increasing Subsequence
- [ ] Russian Doll Envelopes
- [ ] Number of Longest Increasing Subsequence

## Grid

- [ ] Unique Paths
- [ ] Unique Paths II
- [ ] Minimum Path Sum
- [ ] Dungeon Game
- [ ] Maximal Square
- [ ] Cherry Pickup (advanced)

## Interval

- [ ] Burst Balloons
- [ ] Matrix Chain Multiplication
- [ ] Palindrome Partitioning II
- [ ] Stone Game

## State Machine

- [ ] Best Time to Buy and Sell Stock
- [ ] Best Time with Cooldown
- [ ] Best Time with Transaction Fee
- [ ] Paint House

## Tree DP

- [ ] House Robber III
- [ ] Binary Tree Maximum Path Sum

## Bitmask

- [ ] Shortest Path Visiting All Nodes
- [ ] Partition to K Equal Sum Subsets

## Counting / Math DP

- [ ] Unique Binary Search Trees
- [ ] Domino and Tromino Tiling

That's a strong first deep pass.

---
# 42. For EVERY Dynamic Programming Problem

Use this thinking process:

```text
1. What is the subproblem?
   Define it in words precisely.

2. What is the state?
   What parameters uniquely identify it?

3. What is the recurrence?
   Write it before coding.

4. What are base cases?
   List them explicitly.

5. Is it min, max, count, or boolean?

6. Memoization or tabulation?
   Which is clearer here?

7. What is computation order for tabulation?

8. What is the final answer index?
   dp[n]? max(dp)? dp[0][n-1]?

9. Time complexity?
   states × transition cost

10. Space complexity?
    Can I optimize?

11. Do I need to reconstruct solution?

12. Edge cases?
    empty, single, zero capacity, impossible

13. Can greedy work instead?
    If yes, prove why. If no, why not?

14. Does optimal substructure actually hold?
```

Question 1 and 2 are the hardest.

Most interview time should go there, not typing loops.

---

# 43. Compare Multiple Approaches

Example — Climbing Stairs

### Naive recursion

```text
Time:  O(2^n)
Space: O(n) stack
```

### Memoization

```text
Time:  O(n)
Space: O(n)
```

### Tabulation

```text
Time:  O(n)
Space: O(n) or O(1)
```

### Matrix exponentiation (advanced)

```text
Time:  O(log n)
Space: O(1)
```

Don't only memorize:

> Tabulation O(n)

Ask when log solutions exist (linear recurrences).

---

Example — Subset Sum

### Brute force all subsets

```text
Time:  O(2^n)
Space: O(n)
```

### DP knapsack style

```text
Time:  O(n * sum)
Space: O(sum)
```

### Meet in the middle (advanced, large n)

Different tradeoff.

Understanding multiple approaches deepens DP, not just one trick.

---

# 44. One Especially Important Mental Shift

Beginners see:

```text
[1, 2, 5], amount = 11
```

and think only about coins forward.

Improve by asking:

> What smaller amounts do I already know how to solve?

```text
dp[6] known → helps dp[11]
dp[10] known → helps dp[11]
```

DP is **backward dependency on smaller solved states**.

Same shift for strings:

```text
dp[i][j] depends on smaller prefixes
```

Same for intervals:

```text
dp[l][r] depends on dp[l][k] and dp[k][r]
```

You're building a **table of solved subproblems**, not simulating full construction each time.

---

# 45. Connection to Phase 8 (Graphs)

Some DP problems are shortest paths on implicit graphs.

```text
Grid path = DAG shortest path
```

```text
State machine = graph on (index, mode)
```

```text
Bitmask TSP = shortest path on state graph
```

If graph is DAG, DP and topological DP ordering align.

If graph has cycles but state is expanded to make DAG (e.g. add day index), still DP.

Unified view:

> DP = shortest path / path counting on structured state graph, often with topo order.

---

# 46. Connection to Phase 10 (Greedy & Divide-and-Conquer)

From `map.md` roadmap:

After DP, you compare when Greedy and Divide-and-Conquer work **instead** of DP.

| Problem | Greedy? | DP? |
| ------- | ------- | --- |
| Activity Selection | Yes | No need |
| 0/1 Knapsack | No | Yes |
| Coin Change (unbounded, min coins) | Yes if canonical coin system | DP always works |
| LIS | No simple greedy | Yes |
| Merge Sort | D&C | Subproblems don't overlap |

Building this comparison table is how DP becomes judgment, not templates.

---

# 47. Modular Arithmetic in Counting DP

When answer is huge:

```text
dp[i] = (dp[i] + dp[j]) % MOD
```

Watch:

```text
subtract under modulo: (a - b + MOD) % MOD
overflow before modulo in multiplication problems
```

Problems:

```text
Unique Paths (large grids)
Domino Tiling
Count sorted arrays
```

---

# 48. Boolean DP / Reachability

State is true/false.

```text
dp[i][w] = can we achieve sum w using first i items?
```

Transition:

```text
dp[i][w] = dp[i-1][w] OR dp[i-1][w - weight[i]]
```

Often enough to return `dp[n][target]`.

Subset Sum, Partition, Target Sum variants.

---

# 49. Kadane's Algorithm as DP

Maximum subarray sum:

```text
cur = max(nums[i], cur + nums[i])
best = max(best, cur)
```

Interpretation:

```text
cur = best sum ending at i
```

This is 1D DP with O(1) space.

Shows DP is broader than explicit `dp[]` arrays.

---

# 50. Palindrome DP Sub-Template

Many problems need:

```text
isPalindrome[l][r]
```

Fill:

```cpp
for (int len = 1; len <= n; len++) {
    for (int l = 0; l + len - 1 < n; l++) {
        int r = l + len - 1;
        if (len == 1) pal[l][r] = true;
        else if (len == 2) pal[l][r] = s[l] == s[r];
        else pal[l][r] = s[l] == s[r] && pal[l+1][r-1];
    }
}
```

Used in:

```text
Longest Palindromic Substring
Palindromic Substrings count
Palindrome Partitioning II
```

Interval + palindrome check composition.

---

# 51. Stock Problems — Unified State Machine View

| Problem | States / Rules |
| ------- | -------------- |
| I | one transaction |
| II | unlimited transactions |
| III | at most 2 transactions |
| IV | at most k transactions |
| Cooldown | must rest after sell |
| Fee | pay fee on sell |

Generalize:

```text
dp[i][k][hold]   or optimized versions
```

Start from simple state machine, add parameters as problem demands.

---

# 52. Partition Problems

```text
array → contiguous parts with property
```

Examples:

```text
Palindrome Partitioning II  (min cuts)
Split Array Largest Sum
Maximum Subarray Sum after one split
```

Often:

```text
dp[i] = best for prefix ending at i
OR interval dp on [l,r]
```

---

# 53. Expected Value DP (Brief)

Some problems ask expected value:

```text
dp[i] = expected steps / value from state i
```

Linear equations or recurrence with probability.

Less common in basic interviews, but exists.

---

# 54. Debugging DP

When answer is wrong:

```text
1. Print small table for sample input
2. Verify base row/column manually
3. Check one transition by hand
4. Confirm index meaning (inclusive vs exclusive)
5. Confirm loop bounds
6. Test smallest n,m,W
7. Compare memo vs tab — should match
```

Small hand-drawn table beats staring at code.

---

# 55. Interview Strategy

```text
1. Brute recursion in words (30 sec)
2. Identify repeated states
3. Define state + recurrence aloud
4. Base cases
5. Complexity
6. Code tabulation or memo
7. Mention space optimization if time
```

If stuck:

> Can I solve for prefix i-1 and extend?

> Can I add one dimension for constraint?

> Is there knapsack / LCS / grid structure hidden?

---

# Your Complete Dynamic Programming Checklist

```text
DYNAMIC PROGRAMMING
│
├── Fundamentals
│   ├── Why DP exists
│   ├── Recursion with reuse
│   ├── Overlapping subproblems
│   ├── Optimal substructure
│   ├── When DP does NOT apply
│   └── DP vs Greedy vs D&C
│
├── Implementation Styles
│   ├── Memoization (top-down)
│   ├── Tabulation (bottom-up)
│   ├── When to prefer each
│   └── Recursion depth / stack issues
│
├── Design Framework
│   ├── Define state
│   ├── Write recurrence
│   ├── Base cases
│   ├── Computation order
│   ├── Extract answer
│   ├── Time / space analysis
│   └── Reconstruction (optional)
│
├── DP Dimensions
│   ├── 1D DP
│   ├── 2D DP
│   ├── Interval DP
│   ├── State machine DP
│   ├── Tree DP
│   ├── Bitmask DP
│   └── Digit DP (intro)
│
├── Classic Problems
│   ├── Fibonacci / Climbing Stairs
│   ├── 0/1 Knapsack
│   ├── Unbounded Knapsack
│   ├── LCS
│   ├── LIS
│   ├── Edit Distance
│   ├── Coin Change (min + ways)
│   ├── Grid Paths
│   ├── Interval (Burst Balloons, MCM)
│   └── Bitmask TSP intro
│
├── Core Patterns
│   ├── Pattern 1 — Linear 1D Recurrence
│   ├── Pattern 2 — Take or Skip (0/1)
│   ├── Pattern 3 — Unbounded Choice
│   ├── Pattern 4 — Two Sequence 2D
│   ├── Pattern 5 — Grid Path
│   ├── Pattern 6 — Interval DP
│   ├── Pattern 7 — State Machine
│   ├── Pattern 8 — Prefix / Transition Optimization
│   ├── Pattern 9 — Counting DP
│   ├── Pattern 10 — Bitmask / Subset
│   ├── Pattern 11 — Tree DP
│   └── Pattern 12 — Digit DP (intro)
│
├── Knapsack Mastery
│   ├── 0/1 vs unbounded
│   ├── 2D table vs 1D optimized
│   ├── Backward vs forward loop
│   ├── Subset sum / partition / target sum
│   └── Pseudo-polynomial complexity
│
├── String DP Mastery
│   ├── LCS template
│   ├── Edit distance template
│   ├── Palindrome table
│   └── Traceback / reconstruction
│
├── Optimization
│   ├── Rolling array / row
│   ├── O(1) space 1D
│   ├── Binary search on LIS
│   └── Monotonic structure on transitions (awareness)
│
├── Analysis
│   ├── State count
│   ├── Transitions per state
│   ├── Pseudo-polynomial time
│   ├── Memory limits (MLE)
│   └── Modulo in counting
│
├── Identification
│   ├── Recognition signals
│   ├── Anti-signals (greedy, disjoint D&C)
│   ├── Pattern recognition map
│   └── Compose patterns (Cherry Pickup, Envelopes)
│
├── Edge Cases
│   ├── Empty / single element
│   ├── Zero capacity / amount
│   ├── Impossible → -1 / false
│   ├── Overflow / modulo
│   ├── Boundary indices
│   └── Initialization (INF, 0, 1)
│
├── Connections
│   ├── Graphs (DAG shortest path view)
│   ├── Greedy comparison (Phase 10)
│   ├── Memoization vs caching in systems
│   └── Backtracking (Phase 11) for reconstruction paths
│
└── For EVERY Problem
    ├── Define subproblem in words
    ├── Define state precisely
    ├── Write recurrence before code
    ├── List base cases
    ├── Choose memo vs tab
    ├── Confirm answer location
    ├── Analyze time and space
    ├── Optimize space if needed
    ├── Test edge cases
    ├── Reconstruct if required
    └── Explain WHY optimal substructure holds
```

---

# 56. Final Mastery Questions

After revising DP, answer without looking:

```text
Why is naive Fibonacci exponential?
What two properties make DP valid?
When is memoization better than tabulation?
Why is 0/1 knapsack one-row loop backward?
How is unbounded knapsack different?
What does dp[i][j] mean in LCS?
How do you initialize edit distance base cases?
What is the state in stock cooldown problem?
When do you use interval DP loop order?
When is bitmask DP appropriate?
What is pseudo-polynomial time?
How do you know answer is max(dp[i]) vs dp[n]?
When does greedy replace DP?
How is Kadane related to DP?
What breaks optimal substructure in longest path?
```

That level of questioning turns DP from a template list into a design skill.

---

# 57. Phase 9 Placement in Your Roadmap

From `map.md`:

> Dynamic Programming — Once recursion and graphs feel comfortable.

You need:

```text
Recursion → define subproblems
Graphs    → state transitions, DAG intuition
Complexity → state count analysis
```

Next phases:

```text
Phase 10 — Greedy & Divide-and-Conquer (contrast with DP)
Phase 11 — Backtracking (often reconstruct or explore when DP counts only)
```

Study DP **with** those comparisons in mind.

The goal is not "memorize 50 problems."

The goal is:

> Recognize structure, define state, prove reuse, implement table.

That is Phase 9 mastery.
