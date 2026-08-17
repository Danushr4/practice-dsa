# Backtracking

## 1. Backtracking Fundamentals

First understand **why Backtracking exists**.

Many problems ask you to find **all valid configurations** or **one valid configuration** from a large space of possibilities:

```text
[1, 2, 3]
```

How many subsets?

```text
{}
{1}
{2}
{3}
{1,2}
{1,3}
{2,3}
{1,2,3}
```

That's `2^n` possibilities.

How many permutations of `[1, 2, 3]`?

```text
1 2 3
1 3 2
2 1 3
2 3 1
3 1 2
3 2 1
```

That's `n!` possibilities.

Brute force means:

```text
Generate every possibility
Check if valid
Keep the good ones
```

That works conceptually, but is often wasteful because many branches are doomed early.

Example — generating all 4-digit PINs:

```text
0000
0001
...
9999
```

10,000 possibilities.

But if the rule is:

```text
No two adjacent digits may be equal
```

Then:

```text
0011  ← invalid immediately after second 1
```

You don't need to explore `0012`, `0013`, ... from that dead branch in the same way you would if you had no pruning.

Backtracking is:

```text
Systematic search
+
Recursive decision making
+
Undoing choices (backtrack)
+
Pruning invalid branches early
```

Core idea:

> Build a solution incrementally. When a partial choice cannot lead to a valid complete solution, **abandon that branch** and try another.

```text
Backtracking = DFS on an implicit decision tree
             + explicit undo (unchoose)
             + constraint checking (prune)
```

Don't learn:

> Backtracking is just recursion.

Learn:

> Backtracking is **organized exhaustive search** where you explore choices in a tree, commit to a choice, recurse, then **restore state** so other branches remain valid.

---

# 2. The Problem Backtracking Solves

Suppose you need all subsets of `[1, 2, 3]`.

You could write nested loops for fixed sizes:

```text
size 0: {}
size 1: {1}, {2}, {3}
size 2: {1,2}, {1,3}, {2,3}
size 3: {1,2,3}
```

But what about subsets of a 20-element array?

Nested loops don't scale.

A uniform recursive model handles all sizes:

```text
At index i, either:
  include nums[i]
  exclude nums[i]
```

That is a **decision tree**.

```text
                    []
                   /  \
               [1]      []
              /  \     /  \
          [1,2] [1]  [2]  []
          ...
```

Backtracking walks this tree.

Compare approaches:

| Approach | Idea | Typical issue |
| -------- | ---- | ------------- |
| Brute force | Generate all, then filter | No early exit |
| Backtracking | Build + prune + undo | Must manage state carefully |
| DP | Overlapping subproblems + optimal substructure | Not for "list all solutions" unless counting |
| Greedy | One local best choice | May miss valid global configurations |

Backtracking shines when:

```text
You need all solutions, or one solution with structure
Choices are sequential
Constraints can reject partial builds early
The search space is exponential but prunable
```

---

# 3. The Three Pillars: Choice Space, Constraints, Goal

Every backtracking problem decomposes into three parts.

## Choice Space

What decision can you make at each step?

Examples:

```text
Subsets:     include current element? yes/no
Permutation: which unused element goes next?
N-Queens:    which column in this row?
Sudoku:      which digit 1-9 in this empty cell?
Word Search: move up/down/left/right?
```

If you cannot name the choice space, you cannot write the algorithm.

---

## Constraints

What makes a partial or complete solution invalid?

Examples:

```text
N-Queens:    no two queens attack
Sudoku:      row/col/box uniqueness
Combination: sum must equal target
Word Search: letters must match, no reuse of same cell
Parentheses: never more ')' than '(' in prefix
```

Constraints are your **pruning hooks**.

The earlier a constraint can fire, the faster the search.

---

## Goal

What are you trying to produce?

```text
All valid subsets
One valid permutation
Any valid N-Queens board
Count of valid paths
Shortest valid path (sometimes)
Lexicographically smallest valid answer
```

Be precise:

```text
"Find all"     → collect every leaf that satisfies goal
"Find one"     → return on first success
"Count"        → increment counter, maybe no storage
"Find best"    → often DP, but backtracking + pruning works for small n
```

---

# 4. The Universal Template: Choose → Explore → Unchoose

This is the heart of backtracking.

```text
backtrack(state):
    if goal_reached(state):
        record solution
        return

    for each choice in choices(state):
        if not valid(choice, state):
            continue            // prune

        apply(choice, state)    // CHOOSE

        backtrack(state)        // EXPLORE

        undo(choice, state)     // UNCHOOSE
```

Visually:

```text
         state
           |
        CHOOSE choice A
           |
        explore deeper
           |
        UNCHOOSE A
           |
        CHOOSE choice B
           |
        explore deeper
           |
        UNCHOOSE B
```

The **unchoose** step is what separates backtracking from naive DFS that mutates global state permanently.

---

## Python Template

```python
def backtrack(path, options):
    if is_complete(path):
        result.append(path[:])   # copy, not reference
        return

    for choice in options:
        if not is_valid(choice, path):
            continue

        path.append(choice)       # choose
        backtrack(path, next_options(choice))
        path.pop()                # unchoose
```

---

## C++ Template

```cpp
vector<vector<int>> result;
vector<int> path;

void backtrack(/* state */) {
    if (isGoal(/* state */)) {
        result.push_back(path);
        return;
    }

    for (int choice : choices) {
        if (!isValid(choice)) continue;

        path.push_back(choice);   // choose
        backtrack(/* updated state */);
        path.pop_back();          // unchoose
    }
}
```

---

## Critical Rules

### 1. Copy when saving solutions

```python
result.append(path[:])   # correct
result.append(path)     # wrong — path mutates later
```

### 2. Undo every mutation

If you:

```text
mark visited
add to set
swap elements
modify board cell
```

you must reverse it after the recursive call.

### 3. Prune before recursing

```text
if not valid: continue
```

Don't recurse into obviously dead branches.

### 4. Define base case clearly

```text
index == n
all cells filled
path length == target
remaining sum == 0
```

---

# 5. The Decision Tree Mental Model

Backtracking is DFS on a **decision tree** you usually never build explicitly.

Example: `[1, 2, 3]` subsets.

```text
                         []
                    /           \
               take 1             skip 1
                /                    \
             [1]                      []
           /    \                   /    \
      take 2  skip 2           take 2  skip 2
        /        \               /        \
    [1,2]        [1]          [2]         []
     / \          / \          / \         / \
   ... ...      ... ...      ... ...     ... ...
```

Each root-to-leaf path = one subset.

Key insight:

> You are not "trying random combinations." You are walking a tree with a disciplined order.

That order matters for:

```text
Avoiding duplicates
Ensuring complete coverage
Implementing pruning
```

---

# 6. Subsets (Power Set)

Problem: return all subsets of `nums`.

## Approach A — Index-based include/exclude

```text
At index i:
  branch 1: include nums[i]
  branch 2: exclude nums[i]
```

```python
def subsets(nums):
    result = []
    path = []

    def backtrack(i):
        if i == len(nums):
            result.append(path[:])
            return

        path.append(nums[i])      # include
        backtrack(i + 1)
        path.pop()                # uninclude

        backtrack(i + 1)          # exclude

    backtrack(0)
    return result
```

Decision tree for `[1,2]`:

```text
              []
           /      \
        [1]        []
       /   \      /   \
   [1,2] [1]  [2]    []
```

---

## Approach B — Iterative over start index

Common pattern when building combinations/subsets without reuse:

```python
def backtrack(start):
    result.append(path[:])

    for i in range(start, len(nums)):
        path.append(nums[i])
        backtrack(i + 1)    # next element must be after i
        path.pop()
```

This avoids duplicate subsets like `[2,1]` when input order is `[1,2]`.

---

## Complexity

```text
Time:  O(n * 2^n)   — 2^n subsets, each copy up to length n
Space: O(n)         — recursion depth, excluding output
```

---

# 7. Permutations

Problem: all orderings of `nums`.

Choice space at each step:

```text
Pick any element not yet used
```

```text
[1,2,3]

        []
    /    |    \
  [1]   [2]   [3]
  / \   / \   / \
 ...
```

---

## Approach — Used array / set

```python
def permute(nums):
    result = []
    path = []
    used = [False] * len(nums)

    def backtrack():
        if len(path) == len(nums):
            result.append(path[:])
            return

        for i in range(len(nums)):
            if used[i]:
                continue
            used[i] = True
            path.append(nums[i])
            backtrack()
            path.pop()
            used[i] = False

    backtrack()
    return result
```

---

## Permutations with duplicates

Input: `[1, 1, 2]`

Without care, you generate duplicate permutations.

Fix:

```text
1. Sort nums
2. Skip nums[i] if nums[i] == nums[i-1] and not used[i-1]
```

Why?

```text
Same value must be used in left-to-right order
Prevents symmetric duplicate branches
```

---

## Complexity

```text
Time:  O(n * n!)
Space: O(n)
```

---

# 8. Combinations

Problem: choose `k` elements from `n`.

```text
C(n, k) = n! / (k! * (n-k)!)
```

Template:

```python
def combine(n, k):
    result = []
    path = []

    def backtrack(start):
        if len(path) == k:
            result.append(path[:])
            return

        # pruning: not enough elements left
        need = k - len(path)
        remaining = n - start + 1
        if remaining < need:
            return

        for i in range(start, n + 1):
            path.append(i)
            backtrack(i + 1)
            path.pop()

    backtrack(1)
    return result
```

Decision tree for `C(4,2)`:

```text
start=1
  pick 1 → pick 2 → [1,2]
  pick 1 → pick 3 → [1,3]
  pick 1 → pick 4 → [1,4]
  pick 2 → pick 3 → [2,3]
  pick 2 → pick 4 → [2,4]
  pick 3 → pick 4 → [3,4]
```

---

## Subsets vs Combinations vs Permutations

| Problem | Order matters? | Reuse allowed? | Fixed size? |
| ------- | -------------- | -------------- | ----------- |
| Subsets | No | No | No |
| Combinations | No | No | Yes (k) |
| Permutations | Yes | No | Yes (n) |
| Combination Sum | No | Maybe | No |

Recognition:

```text
"All subsets"           → subsets
"Choose k"              → combinations
"All arrangements"      → permutations
"Pick numbers summing to target" → combination sum
```

---

# 9. Combination Sum Family

## Combination Sum I

```text
Reuse same element allowed
Input: candidates distinct
```

```python
def combinationSum(candidates, target):
    result = []
    path = []

    def backtrack(start, remaining):
        if remaining == 0:
            result.append(path[:])
            return
        if remaining < 0:
            return

        for i in range(start, len(candidates)):
            path.append(candidates[i])
            backtrack(i, remaining - candidates[i])  # i, not i+1 — reuse
            path.pop()

    backtrack(0, target)
    return result
```

---

## Combination Sum II

```text
Each element used once
Input may contain duplicates
Avoid duplicate combinations
```

Key steps:

```text
1. Sort
2. i > start and candidates[i] == candidates[i-1] → skip
3. backtrack(i + 1, ...)
```

---

## Combination Sum III

```text
Choose k numbers from 1..9 summing to n
```

Adds two constraints:

```text
len(path) == k
sum(path) == n
```

Prune when:

```text
len(path) > k
sum(path) > n
not enough numbers left to reach k
```

---

# 10. Pattern 1 — Include / Exclude (Subsets)

The simplest backtracking pattern.

```text
At each position:
  take it
  leave it
```

When you see:

```text
"All subsets"
"All subarrays/subsequences with property" (sometimes)
"Generate all possibilities of picking elements"
```

Use:

```text
index-based recursion
or start-index loop
```

Example problems:

```text
Subsets
Subsets II
Letter Case Permutation (variant)
```

---

# 11. Pattern 2 — Permutations (Choose Unused)

```text
Build path of length n
Each step picks one unused element
```

When you see:

```text
"All arrangements"
"All orderings"
"Schedule n tasks in all orders"
```

Watch for:

```text
Duplicates in input → sort + skip rule
Constraint between neighbors → prune in loop
```

Example problems:

```text
Permutations
Permutations II
Beautiful Arrangement
```

---

# 12. Pattern 3 — Combinations (Choose k, Forward Only)

```text
Choose elements from a range
Never go backward → avoids duplicate combos
```

When you see:

```text
"Choose k from n"
"All teams of size k"
"All groups"
```

Pruning:

```text
if remaining elements < still needed: stop
if current sum already too large: stop
```

Example problems:

```text
Combinations
Combinations II
Combination Sum III
```

---

# 13. Pattern 4 — Target Search (Combination Sum)

```text
Build a collection whose aggregated value hits target
May or may not allow reuse
```

When you see:

```text
"Find all ways to sum to target"
"Coin change — list all combinations"
"Partition into groups with sum"
```

Template signals:

```text
remaining budget
start index to avoid permutations of same multiset
sort + break early when candidate too large
```

Example problems:

```text
Combination Sum
Combination Sum II
Combination Sum IV (counting — often DP, but backtracking works for small input)
Split Array into Fibonacci Sequence
```

---

# 14. Pattern 5 — Constraint Placement (N-Queens)

Place one item per row/column/region with conflict rules.

```text
N-Queens:
  one queen per row
  no two queens share column or diagonal
```

Board state:

```text
. Q . .
. . . Q
Q . . .
. . Q .
```

Row-by-row choice:

```text
for each row:
  try each column
  if safe:
    place queen
    recurse next row
    remove queen
```

Safety check:

```text
Same column
Same diagonal: row - col constant
Same anti-diagonal: row + col constant
```

Optimized tracking:

```python
cols = set()
diag1 = set()   # row - col
diag2 = set()   # row + col
```

---

## N-Queens Template Sketch

```python
def solveNQueens(n):
    result = []
    board = [['.'] * n for _ in range(n)]

    def is_safe(r, c):
        return c not in cols and (r - c) not in diag1 and (r + c) not in diag2

    def backtrack(row):
        if row == n:
            result.append([''.join(row) for row in board])
            return

        for col in range(n):
            if not is_safe(row, col):
                continue
            board[row][col] = 'Q'
            cols.add(col); diag1.add(row - col); diag2.add(row + col)
            backtrack(row + 1)
            board[row][col] = '.'
            cols.remove(col); diag1.remove(row - col); diag2.remove(row + col)

    backtrack(0)
    return result
```

When you see:

```text
"Place non-attacking"
"Fill grid with one per row/col"
"Schedule with no conflicts"
```

Think:

```text
Constraint placement backtracking
Track occupied columns/diagonals/regions
```

---

# 15. Pattern 6 — Fill Empty Cells (Sudoku)

Sudoku is constraint placement on a grid.

```text
Find next empty cell
Try digits 1-9
Check row, column, 3x3 box
Recurse or backtrack
```

```text
. 5 . | . 2 .
. . . | . . 6
. . 1 | . . .

───+───+───
. . . | 4 . .
. 3 . | . . .
. . . | . 8 .

───+───+───
2 . . | . . .
. . 4 | . 7 .
. . . | . 3 .
```

---

## Sudoku Solver Pattern

```python
def solveSudoku(board):
    def is_valid(r, c, ch):
        for i in range(9):
            if board[r][i] == ch or board[i][c] == ch:
                return False
        br, bc = 3 * (r // 3), 3 * (c // 3)
        for i in range(br, br + 3):
            for j in range(bc, bc + 3):
                if board[i][j] == ch:
                    return False
        return True

    def backtrack():
        for r in range(9):
            for c in range(9):
                if board[r][c] != '.':
                    continue
                for d in '123456789':
                    if not is_valid(r, c, d):
                        continue
                    board[r][c] = d
                    if backtrack():
                        return True
                    board[r][c] = '.'
                return False
        return True

    backtrack()
```

---

## Optimizations

```text
Precompute empty cells list
Track row/col/box bitmasks
Choose most constrained empty cell first (MRV heuristic)
```

When you see:

```text
"Fill grid satisfying local rules"
"Valid Sudoku configuration"
"Latin square"
```

Think:

```text
Cell-by-cell assignment
Heavy constraint checking
Often one solution, not all
```

---

# 16. Pattern 7 — Grid Path Search (Word Search)

Explore paths on a 2D board.

```text
b o a r d
f l o a t
```

Find `"boat"`:

```text
b → o → a → t
```

Choices:

```text
up, down, left, right
```

Constraints:

```text
in bounds
not visited in current path
next char matches
```

---

## Word Search Template

```python
def exist(board, word):
    rows, cols = len(board), len(board[0])

    def backtrack(r, c, idx):
        if idx == len(word):
            return True
        if r < 0 or c < 0 or r >= rows or c >= cols:
            return False
        if board[r][c] != word[idx]:
            return False

        temp = board[r][c]
        board[r][c] = '#'           # mark visited
        found = (
            backtrack(r + 1, c, idx + 1) or
            backtrack(r - 1, c, idx + 1) or
            backtrack(r, c + 1, idx + 1) or
            backtrack(r, c - 1, idx + 1)
        )
        board[r][c] = temp            # unmark
        return found

    for r in range(rows):
        for c in range(cols):
            if backtrack(r, c, 0):
                return True
    return False
```

---

## Word Search II (Multiple Words)

Naive: run Word Search per word.

Better:

```text
Trie of words
DFS on board with trie navigation
Prune when trie node has no children
Remove completed words from trie
```

When you see:

```text
"Path on grid"
"Connect adjacent cells"
"Snake-like sequence in matrix"
```

Think:

```text
DFS + visited marking + restore
Multi-start from each cell
```

---

# 17. Pattern 8 — Partitioning (Palindrome Partitioning)

Split a string into parts satisfying a property.

```text
"aab"

a | a | b
aa | b
```

Each cut = choice.

```text
Try all prefix lengths from current index
If prefix is valid palindrome → recurse on suffix
```

---

## Palindrome Partitioning Template

```python
def partition(s):
    result = []
    path = []

    def is_palindrome(sub):
        return sub == sub[::-1]

    def backtrack(start):
        if start == len(s):
            result.append(path[:])
            return

        for end in range(start + 1, len(s) + 1):
            prefix = s[start:end]
            if not is_palindrome(prefix):
                continue
            path.append(prefix)
            backtrack(end)
            path.pop()

    backtrack(0)
    return result
```

Optimization:

```text
Precompute palindrome[i][j] table in O(n²)
Then O(1) palindrome checks
```

When you see:

```text
"Split string into valid parts"
"Partition array into k groups"
"Cut string minimally"
```

Think:

```text
Choose cut position
Validate segment
Recurse on remainder
```

---

# 18. Pattern 9 — Valid Construction (Generate Parentheses)

Build a string character by character with prefix constraints.

```text
n = 3

((()))
(()())
(())()
()(())
()()()
```

At each step:

```text
Can add '(' if open < n
Can add ')' if close < open
```

```python
def generateParenthesis(n):
    result = []

    def backtrack(path, open_count, close_count):
        if len(path) == 2 * n:
            result.append(''.join(path))
            return

        if open_count < n:
            path.append('(')
            backtrack(path, open_count + 1, close_count)
            path.pop()

        if close_count < open_count:
            path.append(')')
            backtrack(path, open_count, close_count + 1)
            path.pop()

    backtrack([], 0, 0)
    return result
```

Key idea:

```text
Constraint is on PREFIX, not final string only
Never generate invalid prefixes
```

Similar problems:

```text
IP Addresses (add dots with valid segments)
Restore IP Addresses
Binary strings with no consecutive ones (variant)
```

---

# 19. Pattern 10 — Cartesian Product (Letter Combinations)

```text
digits = "23"
```

```text
2 → abc
3 → def

ad ae af bd be bf cd ce cf
```

Tree:

```text
         ""
       /  |  \
      a   b   c
     /|\ /|\ /|\
    d e f ...
```

```python
def letterCombinations(digits):
    if not digits:
        return []

    mapping = {
        '2': 'abc', '3': 'def', '4': 'ghi', '5': 'jkl',
        '6': 'mno', '7': 'pqrs', '8': 'tuv', '9': 'wxyz'
    }
    result = []
    path = []

    def backtrack(i):
        if i == len(digits):
            result.append(''.join(path))
            return
        for ch in mapping[digits[i]]:
            path.append(ch)
            backtrack(i + 1)
            path.pop()

    backtrack(0)
    return result
```

When you see:

```text
"All strings formed by picking one from each group"
"Decode ways building string"
"Multiply choices across stages"
```

---

# 20. Pattern 11 — Path Collection on Graph/Tree

Collect all root-to-leaf paths.

```text
       1
      / \
     2   3
    / \
   4   5
```

Paths:

```text
1→2→4
1→2→5
1→3
```

```python
def backtrack(node, path):
    if not node:
        return
    path.append(node.val)
    if not node.left and not node.right:
        result.append(path[:])
    else:
        backtrack(node.left, path)
        backtrack(node.right, path)
    path.pop()
```

Variants:

```text
Path Sum II
Sum Root to Leaf Numbers
Binary Tree Paths
All Paths From Source to Target (DAG/graph)
```

---

# 21. Pattern 12 — State-Space Search (General DFS)

Some problems don't look like subsets/perms but are still backtracking.

```text
Open the lock with minimum turns
Word Ladder (BFS usually, but path listing uses backtracking)
Maze solving
Knight tour
```

State:

```text
(position, visited set, steps so far, ...)
```

Transition:

```text
all legal moves from current state
```

Goal:

```text
reach target state
or enumerate all goal states
```

When state space is large:

```text
BFS for shortest path
Backtracking for all paths or deep structural search
```

---

# 22. Pruning Techniques

Pruning is what makes backtracking practical.

Without pruning, you are just brute force with extra steps.

---

## 1. Feasibility Pruning

Reject choice immediately if it violates constraints.

```text
N-Queens: column occupied
Parentheses: close > open
Combination Sum: remaining < 0
```

---

## 2. Bound Pruning

Stop when partial solution cannot beat current best.

```text
Branch and bound
Traveling Salesman (conceptual)
Subset sum exceeding target
```

```python
if current_sum > target:
    return
```

---

## 3. Ordering Pruning

Process choices in an order that fails faster.

```text
Sort candidates ascending → break when too large
Try most constrained cell first (Sudoku MRV)
```

---

## 4. Duplicate Avoidance Pruning

```text
Sort + skip equal neighbors
Use start index to only move forward
Use set of visited for grids, not global permanence
```

For subsets with duplicates:

```python
if i > start and nums[i] == nums[i-1]:
    continue
```

---

## 5. Remaining-Count Pruning

```text
Need k elements, only m left → impossible
```

```python
need = k - len(path)
remaining = n - i
if remaining < need:
    return
```

---

## 6. Early Success Exit

When only one solution needed:

```python
if backtrack():
    return True
```

Don't collect all leaves.

---

## 7. Trie / Prefix Pruning

Word Search II:

```text
If current board path is not a prefix in trie → stop
```

---

## 8. Bitmask Pruning

For small `n ≤ 20`:

```text
Represent used elements as bitmask
Fast subset/permutation enumeration
```

---

# 23. Backtracking vs DFS vs Brute Force vs DP

This comparison is essential.

---

## Backtracking vs DFS

They are closely related.

```text
DFS = graph/tree traversal mechanism
Backtracking = DFS + undo + goal/constraints on decision tree
```

Every backtracking algorithm uses DFS.

Not every DFS is backtracking.

Example — standard graph DFS counting reachable nodes:

```text
DFS yes
No unchoose needed if only marking visited permanently
```

Example — Word Search:

```text
DFS on grid
Must unmark cell → backtracking
```

Rule of thumb:

```text
If you mutate shared state and need to restore it → backtracking
If exploration is monotonic (never revisit) → plain DFS
```

---

## Backtracking vs Brute Force

Brute force:

```text
Generate all possibilities
Filter at the end
```

Backtracking:

```text
Generate incrementally
Abandon invalid partial builds
Often same worst-case, better typical case
```

Example — permutations of 10 elements:

```text
Brute force: 10! complete strings, then check
Backtracking: still 10! leaves, but can prune with constraints
```

If there is **no pruning**, backtracking complexity equals brute force.

The win is **early cut-off**.

---

## Backtracking vs Dynamic Programming

| | Backtracking | DP |
| --- | --- | --- |
| Goal | All/one configurations | Optimal value or count |
| Overlapping subproblems | Usually no (paths are distinct) | Required |
| Optimal substructure | Not required | Required |
| State | Path + choices | Table keyed by subproblem |
| Output | List of solutions | Number / best value |

Example — Fibonacci:

```text
Backtracking: meaningless
DP: natural
```

Example — all paths in grid:

```text
Backtracking: list each path
DP count paths: unique paths I/II
```

Example — combination sum:

```text
List all combos → backtracking
Count combos → DP (Combination Sum IV)
```

When DP counts and you need the actual combos:

```text
DP to count
Backtracking to reconstruct
or just backtracking if n is small
```

---

## Backtracking vs Greedy

Greedy:

```text
One choice per step, never undo
```

Backtracking:

```text
Try a choice, maybe undo
```

If problem asks for **all** solutions, greedy is insufficient.

If problem asks for **one** solution and greedy proof exists, don't backtrack.

---

# 24. Complexity Analysis

Backtracking often looks scary but follows patterns.

---

## Time

Worst case is usually exponential:

```text
Subsets:      O(2^n)
Permutations: O(n!)
Combinations: O(C(n,k))
N-Queens:     O(n!) with pruning much better in practice
Sudoku:       exponential in empty cells
```

With pruning, **average** case may be far better.

Always ask:

```text
What is branching factor b?
What is depth d?
Is pruning effective?
```

Rough upper bound:

```text
O(b^d)
```

---

## Space

```text
Recursion depth:     O(d)
Current path/board:  O(d) or O(n)
Output size:         may dominate
Auxiliary sets:      O(n) or O(n²) for constraints
```

Example — N-Queens:

```text
Recursion O(n)
Output O(n * number of solutions)
```

Don't forget output space counts toward total space in analysis interviews.

---

# 25. Implementation Pitfalls

## 1. Forgetting to undo

```python
visited.add(cell)
backtrack(...)
# missing visited.remove(cell)
```

## 2. Shallow copy bugs

```python
result.append(path)  # wrong
```

## 3. Off-by-one in indices

```text
start vs start+1
i vs i+1 for reuse/no-reuse
```

## 4. Duplicate results

```text
[1,2] and [2,1] treated as same combination
```

Fix with `start` index or sorting + skip.

## 5. Infinite recursion

```text
No progress toward base case
Wrong base case
```

## 6. Modifying input without restore

```text
board[r][c] = '#'
# must restore
```

## 7. Global mutable state

Harder to reason about.

Prefer passing state or explicit undo.

---

# 26. Pattern Recognition

Train yourself to notice these signals.

```text
"All subsets / subsequences"
        ↓
Include/Exclude or Start-index subsets
```

```text
"All permutations / arrangements"
        ↓
Choose unused + used[] / swap-based
```

```text
"Choose k from n"
        ↓
Combinations with forward-only start
```

```text
"Sum to target with picks"
        ↓
Combination Sum (+ sort/prune)
```

```text
"Place pieces with no conflicts"
        ↓
N-Queens / constraint placement
```

```text
"Fill empty cells with rules"
        ↓
Sudoku pattern
```

```text
"Path in grid using adjacent cells"
        ↓
Word Search DFS + visited restore
```

```text
"Split string into valid segments"
        ↓
Palindrome partitioning / cut positions
```

```text
"Generate valid strings step by step"
        ↓
Parentheses / prefix constraint
```

```text
"Pick one letter per group"
        ↓
Cartesian product / phone keypad
```

```text
"All root-to-leaf paths"
        ↓
Tree path backtracking
```

```text
"Need ALL solutions"
        ↓
Backtracking (unless counting only → consider DP)
```

```text
"Can reject early based on partial build"
        ↓
Backtracking with pruning
```

---

# 27. Edge Cases — Extremely Important

Backtracking code breaks on edge cases because recursion + state is fragile.

Always test:

```text
Empty input
```

```text
Single element
```

```text
All elements identical
```

```text
Duplicates in input
```

```text
Target sum = 0
```

```text
No valid solution exists
```

```text
Multiple valid solutions
```

```text
Need lexicographically smallest — order your choices
```

```text
Very large n — TLE expected, need different algorithm
```

---

## Subsets / Combinations

```text
nums = []
nums = [1]
nums = [1,1,1]
k = 0
k > n
```

---

## Permutations

```text
nums = [1]
nums with duplicates
```

---

## Combination Sum

```text
target = 0
no combination possible
candidates contains 0 (if allowed by problem)
unlimited reuse vs single use
```

---

## N-Queens

```text
n = 1 → one solution
n = 2,3 → no solution
```

---

## Word Search

```text
word longer than board cells
word length 1
repeated characters in word
same cell cannot be reused in one path
```

---

## Palindrome Partitioning

```text
single character string
entire string already palindrome
all same characters: "aaaa"
```

---

## Parentheses

```text
n = 1 → "()"
n = 0 → "" (problem dependent)
```

---

# 28. Draw the Decision Tree

For backtracking, **don't only code — draw**.

Example: `generateParenthesis(2)`

```text
                ""
               /  \
             "("  X invalid
            /
          "(("
         /   \
      "(()"  "((" invalid more ')'
      /
   "(())"
```

Suppose you place `)` too early:

```text
")("  ← pruned by close < open rule
```

Drawing prevents:

```text
Wrong base case
Missing branches
Duplicate generation
```

---

# 29. Problems to Practice

You don't need 100 random backtracking problems.

Choose problems representing each pattern.

### Fundamentals

- [ ] Subsets
- [ ] Subsets II
- [ ] Permutations
- [ ] Permutations II
- [ ] Combinations
- [ ] Combinations II

### Target / Sum

- [ ] Combination Sum
- [ ] Combination Sum II
- [ ] Combination Sum III
- [ ] Partition Equal Subset Sum (recognize — DP common, backtracking for understanding)

### Constraint Placement

- [ ] N-Queens
- [ ] N-Queens II (count)
- [ ] Valid Sudoku (validation, not full solve)
- [ ] Sudoku Solver

### Grid / Path

- [ ] Word Search
- [ ] Word Search II
- [ ] Unique Paths III (backtracking acceptable for small grids)
- [ ] Robot Room Cleaner (conceptual)

### String Partitioning / Construction

- [ ] Palindrome Partitioning
- [ ] Palindrome Partitioning II (DP optimization)
- [ ] Generate Parentheses
- [ ] Letter Combinations of a Phone Number
- [ ] Restore IP Addresses

### Tree Paths

- [ ] Binary Tree Paths
- [ ] Path Sum II
- [ ] Sum Root to Leaf Numbers

### Harder / Compositional

- [ ] Remove Invalid Parentheses
- [ ] Expression Add Operators
- [ ] Word Pattern II
- [ ] Matchsticks to Square
- [ ] Split Array into Fibonacci Sequence
- [ ] Beautiful Arrangement
- [ ] Gray Code (backtracking or formula)

That is enough to understand the major backtracking patterns deeply.

---
# 30. For EVERY Backtracking Problem

Use this thinking process:

```text
1. What am I building?
   Path? Board? String? Partition?

2. What is one decision at each step?
   Include/exclude? Pick unused? Place queen? Move direction?

3. What is the base case / goal?
   Length n? All rows filled? Index at end? Sum == target?

4. What constraints apply to PARTIAL solutions?
   Can I prune early?

5. What state must be restored after recursion?
   path pop, unmark visited, clear board cell

6. How do I avoid duplicate solutions?
   start index, sort+skip, used ordering rule

7. Do I need all solutions or one?
   Collect all vs return True on first hit

8. What is branching factor and depth?
   Rough complexity estimate

9. Can DP or BFS be better?
   Count only? Shortest path?

10. Edge cases?
    empty, single element, duplicates, impossible
```

Question 4 is especially important.

Many students write backtracking that only checks validity at the leaves.

That is brute force disguised as recursion.

---

# 31. Compare Multiple Approaches

Example: **Subsets**

### Iterative bitmask

```text
for mask in 0..2^n-1:
  build subset from bits

Time:  O(n * 2^n)
Space: O(1) extra besides output
```

### Backtracking

```text
include/exclude recursion

Time:  O(n * 2^n)
Space: O(n) recursion
```

### Cascading iterative

```text
result = [[]]
for num in nums:
  result += [subset + [num] for subset in result]

Time:  O(n * 2^n)
Space: O(n * 2^n)
```

Don't just conclude:

> Backtracking is the only way.

Ask:

* Need only count? → `2^n` formula
* Need iterative? → bitmask or cascade
* Need prune by custom constraint? → backtracking shines

---

Example: **Word Search**

### Backtracking DFS

```text
Try all paths with undo

Time:  O(m * n * 4^L) rough
Space: O(L) recursion
```

### BFS from each cell

```text
Possible but heavy for existence check
Backtracking is natural
```

---

Example: **Combination Sum — count ways**

### Backtracking

```text
List all combos
```

### DP

```text
dp[t] = sum of dp[t - c] for each coin c
Count ways to make target
```

Use the right tool:

```text
List → backtracking
Count → DP often better
```

---

# 32. One Especially Important Mental Shift

When beginners see:

```text
nums = [1, 2, 3]
target = 5
```

they think:

> Try all subsets and check sums.

As you improve, ask:

> What decision at each step shrinks the remaining problem?

Maybe:

```text
Pick next number from index i
```

Maybe:

```text
Include or exclude current index
```

Maybe:

```text
Place queen in row r, column c
```

Maybe:

```text
Cut string after position i
```

Backtracking is not about syntax.

It is about **modeling the problem as a sequence of choices with undo**.

The deeper skill:

```text
Choice space
+
Constraints
+
Goal
+
Undo discipline
```

Once that model is clear, the code is mostly template.

---

# 33. Swap-Based Permutations (Alternative)

Another permutation technique — in-place swap:

```python
def permute(nums):
    result = []

    def backtrack(start):
        if start == len(nums):
            result.append(nums[:])
            return
        for i in range(start, len(nums)):
            nums[start], nums[i] = nums[i], nums[start]
            backtrack(start + 1)
            nums[start], nums[i] = nums[i], nums[start]

    backtrack(0)
    return result
```

Choose:

```text
path + used[]  → clearer for beginners
swap-based     → O(1) extra space besides output
```

Both are backtracking.

---

# 34. Reuse vs Non-Reuse — Decision Table

| Problem type | Next index after picking i | Why |
| ------------ | -------------------------- | --- |
| Combinations | i + 1 | each element once |
| Combination Sum I | i | reuse allowed |
| Combination Sum II | i + 1 | each element once |
| Permutations | any unused | order matters |
| Subsets (index) | i + 1 for include branch | standard |

Getting this wrong creates:

```text
duplicate combinations
missing combinations
infinite recursion
```

---

# 35. When NOT to Use Backtracking

Recognize when another paradigm fits better.

```text
"Shortest path in unweighted graph"     → BFS
"Minimum cost with optimal substructure" → DP
"Pick locally best"                      → Greedy (if proven)
"Count ways with overlapping subproblems" → DP
"Existence in huge state space"          → maybe meet-in-middle, math, or pruning + heuristics
```

Backtracking is correct but may TLE:

```text
n = 25 for permutation with heavy pruning maybe ok
n = 50 naive subsets → 2^50 impossible
```

Always estimate search space before coding.

---

# 36. Branch and Bound (Extension)

Backtracking + best-so-far bound.

```text
Traveling Salesman (conceptual)
Subset sum to minimize difference
```

Pattern:

```python
if current_cost >= best_cost:
    return  # prune
```

This is backtracking's optimization cousin.

Useful when:

```text
You need optimal solution
But full enumeration is too large
A good bound eliminates most branches
```

---

# 37. Meet-in-the-Middle (Contrast)

For some subset problems with `n ≈ 40`:

```text
Split into two halves
Enumerate all subset sums of each half
Combine with hash map
```

Not backtracking in pure form, but solves similar "subset" search faster.

Know the limit:

```text
Backtracking great for n ≤ 20-25 depending on constraints
Meet-in-the-middle for larger n, specific sum problems
```

---

# 38. Real-World Connections

Backtracking appears beyond puzzles.

```text
Constraint Satisfaction Problems (CSP)
Scheduling with restrictions
Configuration solvers
Game move generators (chess puzzles small boards)
Compiler instruction selection (simplified)
Test case generation
SMT solvers (far more advanced)
```

Sudoku and N-Queens are toy versions of **CSP solvers**.

Production systems add:

```text
Variable ordering heuristics
Constraint propagation
Arc consistency
No good — forward checking
```

You don't need those for interviews, but knowing they exist deepens understanding.

---

# 39. Constraint Propagation Preview

Basic backtracking:

```text
Try value, check validity, recurse
```

Smarter CSP:

```text
When you assign a cell, immediately eliminate inconsistent values from neighbors
```

For Sudoku:

```text
Maintain sets of possible digits per cell
When board[r][c] = d, remove d from row/col/box peers
```

This reduces branching dramatically.

Interview level:

```text
Usually explicit is_valid() is enough
Mention propagation as optimization if asked
```

---

# 40. Iterative Backtracking (Advanced)

Recursion is natural, but you can simulate with a stack:

```text
stack holds (state, next_choice_index)
```

Useful when:

```text
Recursion depth limits
Performance critical code
Explicit control needed
```

Conceptually identical.

Most interview solutions use recursion.

---

# 41. Debugging Backtracking

When output is wrong:

```text
1. Log enter/exit with path state
2. Verify base case triggers at right time
3. Check undo mirrors choose exactly
4. Print decision tree for small input by hand
5. Test n=1 and n=2 manually
6. Verify duplicate skip conditions
```

Common bug pattern:

```text
Correct on small inputs
Duplicates on larger inputs with repeated values
→ sorting/skip issue
```

---

# 42. Lexicographic Order

Some problems require returning results in lex order.

```text
Generate Parentheses — natural order if try '(' before ')'
Combinations — iterate start ascending
Permutations — sort nums first
```

If you need smallest valid answer only:

```text
Try choices in sorted order
Return first success
```

---

# 43. Multi-Constraint Problems

Some problems combine patterns.

**Matchsticks to Square:**

```text
Combinations + target sum + 4 buckets
Prune when stick too large or bucket overflow
```

**Expression Add Operators:**

```text
Partition indices + operator choices + evaluate with precedence handling
```

Decompose:

```text
What are sequential decisions?
What is pruned early?
```

---

# 44. Counting with Backtracking

If asked for count only:

```python
count = 0

def backtrack(...):
    global count
    if goal:
        count += 1
        return
    ...
```

Often DP is cleaner for counts.

Use backtracking count when:

```text
Constraints are complex
State is not obvious for DP
Input is small
```

N-Queens II is a clean counting backtracking problem.

---

# 45. Memory of Paths vs Bitmasks

For `n ≤ 20`, bitmask DP/backtracking hybrid is common.

```text
mask represents which elements used
```

Subset enumeration:

```python
for mask in range(1 << n):
    ...
```

Backtracking with bitmask:

```python
def backtrack(mask):
    for i in range(n):
        if mask & (1 << i): continue
        backtrack(mask | (1 << i))
```

Useful in competitive programming.

---

# 46. The Backtracking Framework (Unified)

```text
BACKTRACKING FRAMEWORK
│
├── 1. Define STATE
│      What changes as you recurse?
│      path, index, row, col, remaining, visited
│
├── 2. Define CHOICES(state)
│      All options at this step
│
├── 3. Define VALID(choice, state)
│      Constraint check — prune if false
│
├── 4. APPLY(choice, state)     ← CHOOSE
│      Mutate state
│
├── 5. RECURSE(state)           ← EXPLORE
│      Go deeper
│
├── 6. REVERT(choice, state)    ← UNCHOOSE
│      Undo mutation exactly
│
└── 7. BASE CASE
       Goal reached → record / return
```

Memorize the framework, not 50 problem solutions.

---

# 47. Walkthrough — Full Trace

Problem: subsets of `[1,2]`

```python
def subsets(nums):
    result = []
    path = []

    def backtrack(start):
        result.append(path[:])

        for i in range(start, len(nums)):
            path.append(nums[i])
            backtrack(i + 1)
            path.pop()

    backtrack(0)
    return result
```

Trace:

```text
backtrack(0)
  append [] → result = [[]]
  i=0: append 1, path=[1]
    backtrack(1)
      append [1] → result = [[], [1]]
      i=1: append 2, path=[1,2]
        backtrack(2)
          append [1,2] → result = [[], [1], [1,2]]
          loop end
        pop 2, path=[1]
      loop end
    pop 1, path=[]
  i=1: append 2, path=[2]
    backtrack(2)
      append [2] → result = [..., [2]]
    pop 2
  loop end
```

Final:

```text
[[], [1], [1,2], [2]]
```

Tracing small examples builds confidence for harder problems.

---

# 48. Walkthrough — N-Queens n=4

One solution:

```text
. Q . .
. . . Q
Q . . .
. . Q .
```

Row 0: try col 0,1,2,3...

```text
Row0 col1 → valid
Row1 col3 → valid
Row2 col0 → valid
Row3 col2 → valid → solution
```

Backtrack removes queens and tries next column at row 3, etc.

For n=4, there are 2 solutions.

Pruning eliminates most of 4^4 = 256 naive placements.

---

# 49. Interview Communication Tips

When solving backtracking live:

```text
1. State choice space clearly
2. State constraints / prune rules
3. Write template skeleton first
4. Handle base case
5. Mention time/space roughly
6. Test empty / small case
```

Say:

> "I'll use backtracking because we need all valid configurations and we can prune invalid partial builds."

Not:

> "I'll use recursion."

---

# 50. Relationship to Phase 9 (DP) and Phase 8 (Graphs)

From your learning map:

```text
Phase 8  Graphs      → DFS/BFS machinery
Phase 9  DP          → overlapping subproblems, optimal counts
Phase 11 Backtracking → enumerate / construct with undo
```

Connections:

```text
Graph DFS           → traversal mechanism
Backtracking        → DFS on implicit decision tree + undo
DP                  → when subproblems repeat and you need optimum/count
Backtracking + memo → rare; only if states truly repeat
```

Example bridge:

```text
Unique Paths (count)        → DP
Unique Paths III (list all) → backtracking on small grid
Word Ladder (shortest)      → BFS
Word Ladder II (all shortest) → BFS + backtrack reconstruction
```

---

# Your Complete Backtracking Checklist

```text
BACKTRACKING
│
├── Fundamentals
│   ├── Why backtracking exists
│   ├── Systematic search with pruning
│   ├── Decision tree mental model
│   ├── Choice space
│   ├── Constraints
│   ├── Goal (all / one / count)
│   └── Choose → Explore → Unchoose
│
├── Core Templates
│   ├── Subsets (include/exclude)
│   ├── Subsets (start index)
│   ├── Permutations (used[])
│   ├── Permutations (swap)
│   ├── Combinations (forward only)
│   ├── Combination Sum (reuse)
│   ├── Combination Sum II (no reuse + dedup)
│   └── Universal backtrack skeleton
│
├── Patterns
│   ├── Pattern 1 — Include/Exclude (Subsets)
│   ├── Pattern 2 — Permutations
│   ├── Pattern 3 — Combinations
│   ├── Pattern 4 — Target Search (Combination Sum)
│   ├── Pattern 5 — Constraint Placement (N-Queens)
│   ├── Pattern 6 — Fill Empty Cells (Sudoku)
│   ├── Pattern 7 — Grid Path (Word Search)
│   ├── Pattern 8 — Partitioning (Palindrome)
│   ├── Pattern 9 — Valid Construction (Parentheses)
│   ├── Pattern 10 — Cartesian Product (Phone Keypad)
│   ├── Pattern 11 — Root-to-Leaf Paths
│   └── Pattern 12 — General State-Space Search
│
├── Pruning
│   ├── Feasibility pruning
│   ├── Bound pruning
│   ├── Ordering pruning
│   ├── Duplicate avoidance
│   ├── Remaining-count pruning
│   ├── Early success exit
│   ├── Trie/prefix pruning
│   └── Bitmask (small n)
│
├── Comparisons
│   ├── Backtracking vs DFS
│   ├── Backtracking vs Brute Force
│   ├── Backtracking vs DP
│   ├── Backtracking vs Greedy
│   ├── Backtracking vs BFS (shortest path)
│   └── When NOT to use backtracking
│
├── Complexity
│   ├── Branching factor × depth
│   ├── Subsets O(2^n)
│   ├── Perms O(n!)
│   ├── Combinations C(n,k)
│   ├── Recursion space
│   └── Output-dominated space
│
├── Pitfalls
│   ├── Forgetting undo
│   ├── Shallow copy into result
│   ├── Wrong start index (reuse)
│   ├── Duplicate combinations
│   ├── Infinite recursion
│   └── Global state without restore
│
├── Edge Cases
│   ├── Empty input
│   ├── Single element
│   ├── All duplicates
│   ├── No solution
│   ├── Target zero
│   └── Lexicographic order requirements
│
├── Implement Yourself
│   ├── Subsets
│   ├── Permutations
│   ├── Combinations
│   ├── Combination Sum
│   ├── N-Queens
│   ├── Sudoku solver
│   ├── Word Search
│   ├── Palindrome Partitioning
│   └── Generate Parentheses
│
└── For EVERY Problem
    ├── What am I building?
    ├── What is one decision per step?
    ├── What is the base case?
    ├── What partial constraints prune?
    ├── What must be undone?
    ├── How to avoid duplicates?
    ├── All solutions or one?
    ├── Complexity estimate?
    ├── DP/BFS/greedy alternative?
    ├── Draw decision tree for small input
    └── Test edge cases
```

For your **core DSA pass**, that's enough backtracking. You don't need to dive deeply yet into industrial CSP solvers, SAT/SMT engines, or full constraint propagation theory — but you should recognize that Sudoku and N-Queens are introductory faces of the same underlying idea: **search a configuration space intelligently**.

Once you can **write the universal template from memory**, explain **choose/explore/unchoose**, recognize the **12 major patterns**, and **prune instead of brute forcing to the leaves**, you're ready for advanced topics that combine search with structure — or for revisiting DP when the question asks for counts rather than constructions.

---

# Quick Reference Card

```text
TEMPLATE
────────
backtrack(state):
    if goal: record; return
    for choice in choices:
        if not valid: continue
        apply
        backtrack
        revert

SUBSETS
───────
backtrack(start):
    record path
    for i from start:
        add nums[i]
        backtrack(i+1)
        remove nums[i]

PERMUTATIONS
────────────
backtrack():
    if len(path)==n: record
    for i if not used[i]:
        mark, add, recurse, unmark, remove

COMBINATIONS
────────────
same as subsets but stop when len(path)==k
prune if not enough elements remain

COMBINATION SUM
───────────────
backtrack(start, remaining):
    if remaining==0: record
    if remaining<0: return
    for i from start:
        add candidates[i]
        backtrack(i or i+1, remaining-cand[i])
        remove

N-QUEENS
────────
for each row, try columns
track cols, diagonals
place, recurse, unplace

WORD SEARCH
───────────
mark cell, dfs 4 dirs, unmark cell

PALINDROME PARTITION
────────────────────
for end from start+1:
    if palindrome(s[start:end]):
        add, recurse(end), remove

PARENTHESES
───────────
if open<n: add '('
if close<open: add ')'
```

---

# Final Mental Model

```text
                    PROBLEM
                       |
           Can you model as sequential choices?
                  /         \
                no           yes
                |             |
           other algo    constraints on partial?
           (DP/BFS/      /              \
            greedy)    weak            strong
                        |                |
                   brute-ish         backtracking
                   backtrack          + pruning
```

Backtracking is not a trick.

It is **depth-first exploration of possibilities with discipline**:

```text
Commit
Explore
Undo
Prune
```

Master that rhythm and dozens of problems become the same song in different keys.
