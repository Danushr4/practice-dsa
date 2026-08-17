# Trees — Complete Deep-Understanding Syllabus

Trees are one of the most important data structures in computer science because they model **hierarchical relationships** and enable **logarithmic-time** operations when structured correctly.

After Arrays, Linked Lists, Stacks/Queues, and Hash Tables, trees are the natural next step because they combine:

```text
Pointers / References   (from Linked Lists)
Recursion               (from Divide-and-Conquer thinking)
Ordered Search          (from Binary Search intuition)
Hierarchical Modeling   (new capability)
```

But don't stop at `TreeNode` and `BST`. You should understand **why trees exist**, **how each variant maintains its invariants**, and **which pattern fits which problem**.

---

# 1. Start with the problem Trees solve

Suppose you have a company's org chart:

```text
CEO
├── VP Engineering
│   ├── Backend Lead
│   └── Frontend Lead
└── VP Sales
    ├── Regional Manager A
    └── Regional Manager B
```

Or a file system:

```text
/
├── home
│   ├── user
│   │   ├── documents
│   │   └── downloads
│   └── admin
└── etc
```

Or an expression:

```text
       +
      / \
     *   5
    / \
   3   4
```

These are **not** flat lists.

An array forces you to encode hierarchy awkwardly:

```text
[CEO, VP_Eng, Backend, Frontend, VP_Sales, ...]
```

Who reports to whom? You need extra metadata.

A tree models this naturally:

```text
Each node
   ↓
has children
   ↓
forms a hierarchy
```

And when values are **ordered**, trees can search in:

```text
O(log n)
```

instead of:

```text
O(n)
```

That's the core promise of trees.

---

# 2. Tree Fundamentals — Know These Cold

Before any variant (BST, heap, trie), understand the universal vocabulary.

## Node

The basic building block:

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};
```

Conceptually:

```text
TreeNode
┌─────────────┐
│ val         │
├─────────────┤
│ left        │ ──────>
├─────────────┤
│ right       │ ──────>
└─────────────┘
```

---

## Root

The topmost node. Entry point to the entire tree.

```text
        8        ← ROOT
       / \
      3   10
```

No parent above the root.

```text
root == nullptr   →   empty tree
```

---

## Parent, Child, Sibling

```text
        8
       / \
   parent  parent
      ↓       ↓
      3       10
     / \      \
  child child  child
    1   6      14
```

- `3` and `10` are **children** of `8`
- `8` is the **parent** of `3` and `10`
- `3` and `10` are **siblings**

---

## Leaf (External Node)

A node with **no children**.

```text
        8
       / \
      3   10
     / \   \
    1   6   14
    ↑   ↑    ↑
  LEAVES
```

Leaves are where many recursive base cases live.

---

## Internal Node

Any node that is **not** a leaf.

```text
8, 3, 10 are internal nodes
```

---

## Edge

A link between parent and child.

```text
n nodes  →  n - 1 edges   (in a tree)
```

This is a fundamental tree identity.

---

## Depth of a Node

Number of edges from **root** to that node.

```text
        8          depth 0
       / \
      3   10       depth 1
     / \   \
    1   6   14     depth 2
```

```text
depth(root) = 0
```

Some textbooks count nodes instead of edges. **Be consistent** in interviews — state your convention.

---

## Height of a Node

Number of edges on the **longest path** from that node down to a leaf.

```text
        8          height 2
       / \
      3   10       height 1
     / \   \
    1   6   14     height 0
```

```text
height(leaf) = 0
```

---

## Height of the Tree

Height of the **root**.

```text
height(tree) = height(root)
```

For the tree above:

```text
height = 2
```

---

## Level

Depth + 1 (when root is level 1).

```text
Level 1:  8
Level 2:  3, 10
Level 3:  1, 6, 14
```

Level-order traversal processes nodes level by level.

---

## Subtree

A node plus **all its descendants**.

```text
        8
       / \
      3   10
     / \
    1   6
```

The subtree rooted at `3`:

```text
      3
     / \
    1   6
```

Many tree problems are really:

> Solve for left subtree, solve for right subtree, combine.

---

## Degree

Number of children a node has.

In a **binary tree**, degree is 0, 1, or 2.

```text
8  → degree 2
3  → degree 2
10 → degree 1
1  → degree 0
```

---

## Forest

A collection of trees.

```text
Tree A     Tree B
  1          5
 / \          \
2   3          7
```

Not one connected structure — multiple roots.

---

## Key Properties of a Tree

```text
1. Connected (every node reachable from root)
2. Acyclic (no cycles)
3. Exactly one path between any two nodes
4. n nodes → n - 1 edges
```

If any of these break, you don't have a tree — you have a **graph**.

---

# 3. Why Trees Exist

## Hierarchical Data Is Everywhere

```text
File systems
DOM / UI component trees
Organization charts
Decision trees
Parse trees / ASTs
Game skill trees
Taxonomies
```

Trees are the natural model when data has **parent-child structure**.

---

## Ordered Trees Enable Fast Search

Compare searching a sorted array:

```text
[1, 3, 6, 8, 10, 14, 20]
```

Binary search:

```text
O(log n)
```

A Binary Search Tree (BST) embeds that same decision at every node:

```text
        8
       / \
      3   10
     / \   \
    1   6   14
```

At each node:

```text
go left if smaller
go right if larger
```

```text
Search: O(h)   where h = height
```

If the tree is balanced:

```text
h = O(log n)
```

If the tree is a degenerate chain:

```text
1
 \
  2
   \
    3
     \
      4
```

```text
h = O(n)   →   BST is useless
```

This is why **balance** matters.

---

## Trees vs Arrays

| Need | Array | Tree |
| ---- | ----- | ---- |
| Random access by index | O(1) | Not natural |
| Hierarchical structure | Awkward | Natural |
| Sorted search | O(log n) if sorted array | O(log n) if balanced BST |
| Insert/delete sorted | O(n) shifts | O(log n) if balanced |
| Cache locality | Excellent | Moderate (pointer chasing) |

---

## Trees vs Hash Tables

| Need | Hash Table | Balanced BST |
| ---- | ---------- | ------------ |
| Exact lookup | Expected O(1) | O(log n) |
| Sorted order | No | Yes |
| Range queries (min, max, between) | Awkward | Natural |
| Predecessor/successor | Awkward | O(log n) |
| Worst-case guarantee | Can degrade | O(log n) guaranteed |

Ask:

> Do I need ordering, or just fast membership?

---

## Trees vs Linked Lists

A linked list is a **degenerate tree**:

```text
1 → 2 → 3 → 4 → NULL
```

Each node has at most one child (in one direction).

Trees generalize this to **branching structure**.

---

# 4. Types of Trees — The Full Landscape

You should understand all major forms, from general to specialized.

```text
TREE FAMILY
│
├── General Tree (any number of children)
│
├── Binary Tree (at most 2 children: left, right)
│   │
│   ├── Full Binary Tree (0 or 2 children)
│   ├── Complete Binary Tree (filled left-to-right, last level may be partial)
│   ├── Perfect Binary Tree (all internal nodes have 2 children, all leaves same depth)
│   │
│   └── Binary Search Tree (BST)
│       │
│       └── Balanced BSTs
│           ├── AVL Tree
│           └── Red-Black Tree
│
├── Heap (priority queue tree)
│   ├── Min-Heap
│   └── Max-Heap
│
└── Trie (prefix tree)
```

We'll go through each in depth.

---

# 5. Binary Tree

The most common interview tree type.

## Definition

Each node has **at most two** children: `left` and `right`.

```text
        1
       / \
      2   3
     / \
    4   5
```

No ordering requirement.

---

## Shapes Matter

### Skewed (degenerate)

```text
1
 \
  2
   \
    3
     \
      4
```

Height = n - 1. Worst case for many algorithms.

### Balanced (ideal)

```text
        4
       / \
      2   6
     / \ / \
    1  3 5  7
```

Height ≈ log n.

### Complete

```text
        1
       / \
      2   3
     / \ /
    4  5 6
```

All levels filled except possibly the last, filled left to right.

Heaps use this shape.

---

## Core Binary Tree Operations

### Create / Build

```text
Insert nodes with left/right pointers
```

### Traverse

See Section 11 — this is fundamental.

### Search (general, unordered)

```text
Must visit nodes until found

Time:  O(n)
Space: O(h) recursive / O(n) iterative with stack
```

### Height

```text
height(node):
    if node == null: return -1 (or 0, pick convention)
    return 1 + max(height(left), height(right))
```

### Count nodes

```text
count(node):
    if node == null: return 0
    return 1 + count(left) + count(right)
```

### Mirror / Invert

```text
        1                1
       / \      →       / \
      2   3            3   2
```

Swap left and right recursively.

---

# 6. Binary Search Tree (BST)

## The Invariant

For every node:

```text
all values in left subtree  < node.val
all values in right subtree > node.val
```

Usually **no duplicates**, or duplicates go to one side consistently.

```text
        8
       / \
      3   10
     / \   \
    1   6   14
       / \
      4   7
```

---

## Why the Invariant Matters

It gives you **direction**.

At node `8`, searching for `6`:

```text
6 < 8  →  go left
```

At node `3`:

```text
6 > 3  →  go right
```

At node `6`:

```text
found
```

You eliminated half the remaining tree at each step — **if** the tree is balanced.

---

## BST Search

```cpp
TreeNode* search(TreeNode* root, int target) {
    if (!root || root->val == target) return root;
    if (target < root->val) return search(root->left, target);
    return search(root->right, target);
}
```

```text
Time:  O(h)
Space: O(h) recursive
```

---

## BST Insert

```text
        8
       / \
      3   10
```

Insert `6`:

```text
8 → left (3)
3 → right (6 > 3)
insert as right child of 3
```

Result:

```text
        8
       / \
      3   10
       \
        6
```

Always insert at a **leaf position** maintaining the invariant.

```text
Time:  O(h)
```

---

## BST Delete

Three cases:

### Case 1 — Leaf

```text
Delete 1 from:
      3
     / \
    1   6
```

Simply remove it.

### Case 2 — One child

```text
Delete 10 from:
        8
       / \
      3   10
           \
            14
```

Replace `10` with its child `14`.

### Case 3 — Two children

```text
Delete 8 from:
        8
       / \
      3   10
         / \
        9   14
```

Find **inorder successor** (smallest in right subtree) or **inorder predecessor** (largest in left subtree).

Successor of `8` is `9`:

```text
        9
       / \
      3   10
           \
            14
```

Then delete the successor from its original position (Case 1 or 2).

```text
Time:  O(h)
```

---

## BST Min / Max

```text
Minimum: go left until null
Maximum: go right until null

Time: O(h)
```

---

## Inorder Traversal Gives Sorted Order

```text
        8
       / \
      3   10
     / \   \
    1   6   14
```

Inorder: `1, 3, 6, 8, 10, 14`

This is one of the most important BST facts.

---

## BST Complexity

| Operation | Average (balanced) | Worst (skewed) |
| --------- | ------------------ | -------------- |
| Search    | O(log n)           | O(n)           |
| Insert    | O(log n)           | O(n)           |
| Delete    | O(log n)           | O(n)           |
| Min/Max   | O(log n)           | O(n)           |
| Inorder   | O(n)               | O(n)           |

The worst case is why we need **balanced** BSTs.

---

# 7. Balanced BST — AVL Tree

## The Problem AVL Solves

Inserting sorted data into a BST:

```text
1
 \
  2
   \
    3
     \
      4
```

Height = O(n). AVL guarantees:

```text
height = O(log n)
```

---

## AVL Invariant

For every node, the **balance factor**:

```text
balance = height(left) - height(right)
```

Must be in `{-1, 0, +1}`.

If balance becomes `±2`, **rotate** to fix.

---

## Rotations — The Core Mechanism

### Right Rotation (LL case)

```text
Before:          After:
    30              20
   /               / \
  20      →      10   30
 /
10
```

```text
        y                x
       / \              / \
      x   C    →       A   y
     / \                  / \
    A   B                B   C
```

### Left Rotation (RR case)

Mirror of right rotation.

### Left-Right (LR case)

Rotate left on child, then right on node.

### Right-Left (RL case)

Rotate right on child, then left on node.

---

## AVL Insert Flow

```text
1. Standard BST insert
2. Walk back up to root
3. Update heights
4. Check balance factor at each ancestor
5. If |balance| > 1, perform appropriate rotation
```

---

## AVL Complexity

| Operation | Time |
| --------- | ---- |
| Search    | O(log n) |
| Insert    | O(log n) |
| Delete    | O(log n) |

Guaranteed worst-case O(log n) — stricter balance than Red-Black.

---

## AVL Trade-off

```text
Pros:
  Tighter balance → shorter height → faster lookups
  Worst-case O(log n) guaranteed

Cons:
  More rotations on insert/delete
  More bookkeeping (balance factors)
  Slightly more expensive updates
```

Used when **lookup-heavy** workloads dominate.

---

# 8. Balanced BST — Red-Black Tree (Conceptual)

## The Idea

A self-balancing BST with **color rules** instead of strict height balance.

Each node is **red** or **black**.

## Red-Black Invariants

```text
1. Every node is red or black
2. Root is black
3. All leaves (NIL) are black
4. Red node cannot have red children (no two consecutive reds)
5. Every path from node to descendant leaf has same number of black nodes
```

These rules guarantee:

```text
height ≤ 2 * log₂(n + 1)
```

So operations remain O(log n).

---

## Why Red-Black Over AVL?

```text
AVL:        stricter balance, more rotations on write
Red-Black:  looser balance, fewer rotations on insert/delete
```

Red-Black trees are used in:

```text
C++ std::map, std::set
Java TreeMap, TreeSet
Linux kernel rbtree
```

When **insert/delete-heavy** workloads dominate, Red-Black often wins.

---

## Fixups After Insert/Delete

```text
Insert:
  1. BST insert, color new node red
  2. If parent is red, violate rule 4 → recolor and/or rotate
  3. Cases: uncle red, uncle black (zig, zig-zag)

Delete:
  1. BST delete
  2. If double-black violation → recolor and/or rotate
```

You don't need to memorize every case for interviews unless specifically asked. Understand:

```text
Rotations + recoloring restore invariants
Height stays O(log n)
```

---

## AVL vs Red-Black Summary

| Property | AVL | Red-Black |
| -------- | --- | --------- |
| Balance strictness | Stricter | Looser |
| Lookup | Slightly faster | Slightly slower |
| Insert/Delete | More rotations | Fewer rotations |
| Height | ≤ 1.44 log₂(n+2) - 0.328 | ≤ 2 log₂(n+1) |
| Typical use | Lookup-heavy | General-purpose ordered map |

---

# 9. Heap (Priority Queue Tree)

## Not a BST

A heap is a **complete binary tree** with a **heap property**.

Do not confuse heap ordering with BST ordering.

---

## Min-Heap Property

```text
parent.val ≤ child.val   (for every parent-child pair)
```

```text
        1
       / \
      3   2
     / \ / \
    7  4 5  6
```

Root = minimum.

---

## Max-Heap Property

```text
parent.val ≥ child.val
```

Root = maximum.

---

## Complete Binary Tree Shape

```text
        1
       / \
      2   3
     / \ /
    4  5 6
```

Filled left to right, level by level.

This shape enables **array representation**.

---

## Core Heap Operations

### Peek (get min/max)

```text
return root

Time: O(1)
```

### Insert

```text
1. Add to end of array (next open position in complete tree)
2. Bubble up (sift up) until heap property restored

Time: O(log n)
```

Example — insert `0` into min-heap:

```text
Before:          After bubble up:
    1                0
   / \              / \
  3   2      →     1   2
 / \ / \          / \ / \
7  4 5  6        7  4 5  6
```

### Extract Min (or Max)

```text
1. Save root
2. Move last element to root
3. Bubble down (sift down) until heap property restored

Time: O(log n)
```

### Heapify (build heap from array)

```text
Bottom-up sift down from last non-leaf to root

Time: O(n)   ← not O(n log n)!
```

This is a classic interview insight.

---

## Heap Use Cases

```text
Priority Queue
Top K elements
Merge K sorted lists
Dijkstra's algorithm
Huffman coding
Median from data stream (with two heaps)
Task scheduling
```

---

## Heap Complexity

| Operation | Time |
| --------- | ---- |
| Peek      | O(1) |
| Insert    | O(log n) |
| Extract   | O(log n) |
| Heapify   | O(n) |
| Search arbitrary | O(n) |

Heaps are **not** for general search. They're for **extremes**.

---

# 10. Trie (Prefix Tree)

## The Problem Tries Solve

Hash tables find exact keys fast.

But what about:

```text
All words starting with "app"
```

```text
"app", "apple", "application", "apricot"
```

Scanning every word is O(total characters).

A trie makes prefix queries efficient.

---

## Structure

Each node represents a **character** (or edge represents a character).

```text
(root)
  └── a
       └── p
            └── p (end) ← "app"
                 └── l
                      └── e (end) ← "apple"
```

```text
        (root)
       /  |  \
      a   b   c
      |   |
      p   a
      |   |
      p   t
     /    |
   (end) (end)
```

---

## Trie Node

```cpp
struct TrieNode {
    TrieNode* children[26];  // or unordered_map<char, TrieNode*>
    bool isEnd;
};
```

---

## Core Trie Operations

### Insert

```text
"cat":
  c → a → t (mark isEnd)

Time: O(L)   L = word length
```

### Search (exact)

```text
Follow characters, check isEnd at last node

Time: O(L)
```

### StartsWith (prefix search)

```text
Follow characters, don't require isEnd

Time: O(L)
```

### Delete

```text
Remove end marker, prune unused nodes

Time: O(L)
```

---

## Trie vs Hash Table

| Operation | Hash Table | Trie |
| --------- | ---------- | ---- |
| Exact search | O(L) average | O(L) |
| Prefix search | O(n * L) scan | O(L) + DFS for matches |
| Space | O(total chars) | O(total chars), often more overhead |
| Ordered prefix iteration | No | Natural |

---

## Trie Applications

```text
Autocomplete
Spell checker
IP routing (longest prefix match)
Word search in grid
Dictionary problems
```

---

# 11. Tree Traversals — Master All Four

Traversal = visiting every node in a defined order.

This is the single most tested tree skill.

---

## Inorder (Left → Root → Right)

```text
        4
       / \
      2   6
     / \ / \
    1  3 5  7
```

Visit order: `1, 2, 3, 4, 5, 6, 7`

```text
        4
       / \
   [1,2,3] [5,6,7]
```

On a BST, inorder = **sorted order**.

### Recursive

```cpp
void inorder(TreeNode* root) {
    if (!root) return;
    inorder(root->left);
    visit(root);
    inorder(root->right);
}
```

### Iterative (stack)

```text
Push all left children
Pop, visit, go right
```

---

## Preorder (Root → Left → Right)

Visit order: `4, 2, 1, 3, 6, 5, 7`

```text
        4          visit 4 first
       / \
      2   6
     / \ / \
    1  3 5  7
```

Use cases:

```text
Copy/serialize tree
Prefix expression evaluation
Build tree from preorder + inorder
```

### Recursive

```cpp
void preorder(TreeNode* root) {
    if (!root) return;
    visit(root);
    preorder(root->left);
    preorder(root->right);
}
```

---

## Postorder (Left → Right → Root)

Visit order: `1, 3, 2, 5, 7, 6, 4`

```text
        4          visit 4 last
       / \
      2   6
     / \ / \
    1  3 5  7
```

Use cases:

```text
Delete tree (children before parent)
Calculate subtree sizes
Postfix expression evaluation
Bottom-up DP on trees
```

### Recursive

```cpp
void postorder(TreeNode* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    visit(root);
}
```

---

## Level-Order (BFS)

Visit level by level:

```text
Level 1:  4
Level 2:  2, 6
Level 3:  1, 3, 5, 7
```

Visit order: `4, 2, 6, 1, 3, 5, 7`

```text
        4
       / \
      2   6
     / \ / \
    1  3 5  7
```

Uses a **queue**, not the call stack.

```cpp
void levelOrder(TreeNode* root) {
    if (!root) return;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front(); q.pop();
        visit(node);
        if (node->left)  q.push(node->left);
        if (node->right) q.push(node->right);
    }
}
```

### Level-order with level separation

```text
Track queue size per level:

while (!q.empty()) {
    int levelSize = q.size();
    for (int i = 0; i < levelSize; i++) { ... }
}
```

---

## Traversal Comparison

| Traversal | Order | Typical Use | Implementation |
| --------- | ----- | ----------- | -------------- |
| Inorder   | L-Rt-R | BST sorted output | Stack / recursion |
| Preorder  | Rt-L-R | Copy, serialize | Stack / recursion |
| Postorder | L-R-Rt | Delete, bottom-up | Stack / recursion |
| Level-order | By level | BFS, shortest path in tree | Queue |

---

## Traversal Diagram — Same Tree, All Orders

```text
        8
       / \
      3   10
     / \   \
    1   6   14
```

```text
Inorder:   1, 3, 6, 8, 10, 14
Preorder:  8, 3, 1, 6, 10, 14
Postorder: 1, 6, 3, 14, 10, 8
Level:     8, 3, 10, 1, 6, 14
```

Draw this yourself until it's automatic.

---

# 12. Complexity Tables

## General Binary Tree

| Operation | Time | Space |
| --------- | ---- | ----- |
| Traverse all | O(n) | O(h) stack |
| Search (unordered) | O(n) | O(h) |
| Height | O(n) | O(h) |
| Count nodes | O(n) | O(h) |

```text
h = height
n = number of nodes
```

---

## BST

| Operation | Balanced | Skewed |
| --------- | -------- | ------ |
| Search | O(log n) | O(n) |
| Insert | O(log n) | O(n) |
| Delete | O(log n) | O(n) |
| Min/Max | O(log n) | O(n) |
| Inorder | O(n) | O(n) |

---

## Balanced BST (AVL / Red-Black)

| Operation | Time |
| --------- | ---- |
| Search | O(log n) |
| Insert | O(log n) |
| Delete | O(log n) |
| Successor/Predecessor | O(log n) |
| Range query (k results) | O(log n + k) |

---

## Heap

| Operation | Time |
| --------- | ---- |
| Peek | O(1) |
| Insert | O(log n) |
| Extract min/max | O(log n) |
| Heapify | O(n) |
| Delete arbitrary | O(n) |
| Search | O(n) |

---

## Trie

| Operation | Time | Space |
| --------- | ---- | ----- |
| Insert word | O(L) | O(L) |
| Search word | O(L) | O(1) extra |
| Prefix search | O(L) | O(1) extra |
| All words with prefix | O(L + k) | O(k) output |

```text
L = word length
k = number of matching words
```

---

## Traversal Space

| Method | Space |
| ------ | ----- |
| Recursive DFS | O(h) call stack |
| Iterative DFS | O(h) explicit stack |
| BFS level-order | O(w) queue, w = max width |
| Morris traversal | O(1) — advanced |

For a balanced tree:

```text
h = O(log n)
```

For skewed:

```text
h = O(n)
```

---

# 13. Memory and Representation

## Pointer-Based (Most Common)

```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};
```

```text
        8
       / \
      3   10
```

Each node allocated separately (usually on heap).

```text
Pros:
  Flexible shape
  Dynamic insert/delete
  Natural for interviews

Cons:
  Pointer chasing → cache misses
  Extra memory per node (2 pointers + val)
  Memory fragmentation
```

---

## Array-Based (Implicit / Index Formula)

For a **complete binary tree** (heaps):

```text
        1
       / \
      2   3
     / \ /
    4  5 6

Array: [1, 2, 3, 4, 5, 6, _, _, ...]
Index:  0  1  2  3  4  5
```

Index relationships (0-based):

```text
parent(i)      = (i - 1) / 2
left child(i)  = 2*i + 1
right child(i) = 2*i + 2
```

```text
Pros:
  Cache-friendly
  No pointers
  Compact

Cons:
  Wastes space if tree not complete
  Insert/delete in middle is hard
  Only natural for heaps / complete trees
```

---

## When to Use Which

```text
General BST, arbitrary shape     → Pointer-based
Heap, segment tree (complete)    → Array-based
Trie                             → Pointer-based (children map/array)
N-ary tree                       → Pointer-based (vector of children)
```

---

## Serialization

Convert tree to string and back.

Common pairs:

```text
Preorder + Inorder  → unique binary tree
Postorder + Inorder → unique binary tree
Level-order (BFS)   → with null markers
```

Example BFS serialization:

```text
        1
       / \
      2   3
         /
        4

"1,2,3,null,null,4,null"
```

---

# 14. Tree Pattern 1 — DFS (Recursive Divide-and-Conquer)

The foundation of most tree problems.

## Mental Model

```text
solve(root):
    base case: root == null
    left result  = solve(root.left)
    right result = solve(root.right)
    combine
```

```text
        8
       / \
   solve   solve
    (3)     (10)
```

---

## When to Use

```text
Need subtree information
Bottom-up computation (height, diameter, max path)
Tree property validation
Path problems from root
```

---

## Template

```cpp
ReturnType dfs(TreeNode* root) {
    if (!root) return base;

    ReturnType left  = dfs(root->left);
    ReturnType right = dfs(root->right);

    // process current node
    return combine(left, right, root);
}
```

---

## Classic Examples

```text
Maximum Depth
Same Tree
Invert Binary Tree
Diameter of Binary Tree
Balanced Binary Tree
Subtree of Another Tree
```

---

# 15. Tree Pattern 2 — BFS / Level-Order

Process nodes level by level using a queue.

```text
Queue: [8]
Visit 8 → enqueue 3, 10
Queue: [3, 10]
Visit 3, 10 → enqueue children
...
```

---

## When to Use

```text
Shortest path in unweighted tree
Level-by-level processing
Right/left side view
Zigzag traversal
Connect nodes at same level
Minimum depth (first leaf at lowest level)
```

---

## Template

```cpp
void bfs(TreeNode* root) {
    if (!root) return;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int size = q.size();  // optional: per-level
        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front(); q.pop();
            // process node
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
}
```

---

## DFS vs BFS on Trees

```text
DFS:  O(h) space, natural recursion, depth-first insight
BFS:  O(w) space, level structure, shortest path to any node
```

For trees (no cycles), both visit all n nodes in O(n).

---

# 16. Tree Pattern 3 — BST Search / Exploit Ordering

Use the BST invariant to **prune** search space.

```text
target < node.val  →  only left
target > node.val  →  only right
target == node.val →  found
```

---

## When to Use

```text
Search in BST
Insert/delete in BST
Validate BST
Kth smallest (inorder)
Floor/ceiling in BST
Lowest Common Ancestor in BST
```

---

## Range Queries in BST

Find all values in `[low, high]`:

```text
        8
       / \
      3   10
     / \   \
    1   6   14
```

Range [4, 10]:

```text
At 8: in range, visit, go left and right
At 3: 3 < 4, only go right (6)
At 6: in range, visit
At 10: in range, visit, go left only
Skip 1, 14
```

```text
Time: O(log n + k)   k = results in range
```

---

# 17. Tree Pattern 4 — Inorder Traversal Tricks

Inorder on BST = sorted sequence.

## Kth Smallest

```text
Inorder, count until k

Time: O(h + k)
```

## BST Iterator

```text
Simulate inorder with stack:
  push all left
  pop, return, go right

next() in amortized O(1)
```

## Validate BST

```text
Inorder must be strictly increasing

OR pass (min, max) bounds down recursively
```

---

# 18. Tree Pattern 5 — Path Problems

Many problems ask about paths from ancestor to descendant.

## Root-to-Leaf Paths

```text
        5
       / \
      4   8
     /   / \
    11  13  4
   / \      \
  7   2      1
```

Path `5→4→11→7` sums to 26.

### Template

```cpp
void dfs(TreeNode* node, int currentSum, vector<int>& path) {
    if (!node) return;
    path.push_back(node->val);
    if (!node->left && !node->right) {
        // process complete path
    }
    dfs(node->left, ...);
    dfs(node->right, ...);
    path.pop_back();  // backtrack
}
```

---

## Path Sum Variants

```text
Has Path Sum (root to leaf equals target)
Path Sum II (return all paths)
Path Sum III (any node to any descendant)
Max Path Sum (any path, node can be "bend point")
```

Path Sum III often needs:

```text
Prefix sum on paths + hash map
```

---

# 19. Tree Pattern 6 — Lowest Common Ancestor (LCA)

LCA of two nodes `p` and `q` = deepest node that has both as descendants.

```text
        3
       / \
      5   1
     / \ / \
    6  2 0  8
      / \
     7   4
```

```text
LCA(5, 1) = 3
LCA(5, 4) = 5
LCA(6, 4) = 5
```

---

## General Binary Tree LCA

```cpp
TreeNode* lca(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode* left  = lca(root->left, p, q);
    TreeNode* right = lca(root->right, p, q);
    if (left && right) return root;
    return left ? left : right;
}
```

```text
Time:  O(n)
Space: O(h)
```

---

## BST LCA — Use Ordering

```text
if p.val < root.val && q.val < root.val  → go left
if p.val > root.val && q.val > root.val  → go right
else → root is LCA
```

```text
Time: O(h)
```

---

# 20. Tree Pattern 7 — Build Tree from Traversals

Given traversal orders, reconstruct the unique tree.

## Preorder + Inorder

```text
Preorder: first element = root
Inorder:  root splits left/right subtrees
```

```text
Preorder: [3, 9, 20, 15, 7]
Inorder:  [9, 3, 15, 20, 7]

Root = 3
Left inorder:  [9]       → left subtree
Right inorder: [15, 20, 7] → right subtree
```

Use hash map for inorder indices → O(n) total.

---

## Postorder + Inorder

```text
Postorder: last element = root
```

Same split logic, build from bottom up.

---

# 21. Tree Pattern 8 — Heap / Priority Queue

When you need **repeated min/max** access.

## Top K Frequent

```text
Count frequencies → min-heap of size k

Time: O(n log k)
```

## Merge K Sorted Lists

```text
Min-heap of k list heads

Time: O(N log k)   N = total elements
```

## Find Median from Data Stream

```text
Max-heap (lower half) + min-heap (upper half)
Balance sizes

Insert: O(log n)
Median: O(1)
```

---

## Two-Heap Pattern

```text
        [max-heap]  |  [min-heap]
        lower half  |  upper half
              median here
```

---

# 22. Tree Pattern 9 — Trie / Prefix Search

For string sets with prefix queries.

## Word Search II (Grid + Trie)

```text
Build trie from dictionary
DFS on grid, prune when prefix not in trie
```

## Replace Words

```text
Insert roots into trie
For each word, find shortest prefix root
```

## Design Add and Search Words

```text
Trie with '.' wildcard → DFS on children at wildcard
```

---

# 23. Tree Pattern 10 — Morris Traversal / Threaded Tree (Advanced)

Inorder in O(1) space without stack.

Uses **threaded links** (temporary right pointers to inorder successor).

Good to know exists. Less commonly required in interviews.

---

# 24. Tree Pattern 11 — Serialize / Deserialize

Convert tree ↔ string.

```text
BFS with nulls:  "1,2,3,null,null,4,null"
Preorder with # : "1,2,#,#,3,4,#,#,#,#"
```

Key insight: traversal order determines reconstruction strategy.

---

# 25. Tree Pattern 12 — Subtree / Structure Comparison

```text
Is Subtree:     for each node, check if tree rooted there matches
Same Tree:      recursive compare
Flip Equivalence: compare with optional swap
```

Often combine DFS on main tree + helper to compare.

---

# 26. Tree Pattern 13 — Global vs Local State in DFS

Some problems need **passing information down** (top-down):

```text
Root to leaf with target sum: pass remaining sum
Validate BST: pass (min, max) bounds
```

Some need **aggregating from children** (bottom-up):

```text
Diameter: max of (left height + right height)
Max path sum: max through current node
```

Recognize which direction information flows.

```text
Top-down:    root → leaves (preorder style)
Bottom-up:   leaves → root (postorder style)
```

---

# 27. Tree Pattern 14 — Binary Tree to Linked List / Flatten

In-place restructuring.

## Flatten to Linked List (preorder)

```text
        1                1
       / \                \
      2   5        →       2
     / \   \                \
    3   4   6                3
                              \
                               4
                                \
                                 5
                                  \
                                   6
```

Often solved with recursion or Morris-like threading.

---

# 28. Tree Pattern 15 — Monotonic Stack/Deque on Tree (Advanced)

Less common but appears in "next greater node in binary tree" style problems.

```text
DFS order + stack to find next greater element per node
```

---

# 29. Pattern Recognition

Train yourself to notice these signals.

```text
"Subtree" / "left and right"
        ↓
DFS recursive divide-and-conquer
```

```text
"Level by level" / "each level" / "minimum depth"
        ↓
BFS / level-order
```

```text
"Sorted" / "BST" / "kth smallest" / "inorder"
        ↓
BST property / inorder traversal
```

```text
"Range" / "between low and high"
        ↓
BST pruning
```

```text
"Root to leaf path"
        ↓
DFS + backtracking
```

```text
"Any path" / "through node" / "max path sum"
        ↓
Postorder DFS + global max
```

```text
"Lowest common ancestor"
        ↓
Recursive LCA (general) or BST ordering
```

```text
"Build tree from traversal"
        ↓
Preorder/postorder root + inorder split
```

```text
"Top K" / "Kth largest" / "median stream"
        ↓
Heap
```

```text
"Prefix of words" / "autocomplete" / "dictionary"
        ↓
Trie
```

```text
"Serialize" / "deserialize"
        ↓
Preorder or BFS with null markers
```

```text
"Right side view" / "visible nodes"
        ↓
BFS last per level OR DFS with depth tracking
```

```text
"Count nodes in complete tree"
        ↓
Math + BST-like indexing (O(log²n))
```

Notice how many "hard" problems are **pattern composition**:

```text
Word Search II = Trie + Grid DFS + Backtracking
Serialize/Deserialize = Traversal + Parsing
Binary Tree Cameras = DFS + Greedy on tree
```

---

# 30. Edge Cases — Extremely Important for Trees

Tree code breaks on edge cases. Always test:

```text
Empty tree

null
```

Single node:

```text
  5
```

Two nodes:

```text
  5
 /
3
```

Skewed tree:

```text
1
 \
  2
   \
    3
```

Perfectly balanced:

```text
      4
     / \
    2   6
   / \ / \
  1  3 5  7
```

Only left children:

```text
    5
   /
  3
 /
1
```

Only right children:

```text
1
 \
  3
   \
    5
```

Duplicate values (BST):

```text
Define policy: left or right?
```

Also consider:

```text
- Root is the answer
- Answer is a leaf
- Answer is deep in one subtree
- Both subtrees null
- One child null
- Negative values
- Integer overflow in path sum
- p == q in LCA
- p or q not in tree
- Very deep tree → stack overflow (use iterative)
```

---

# 31. Important Implementation Skill: Draw the Tree and Recursion Stack

For trees, **don't do everything mentally initially**.

## Draw the Tree

```text
        8
       / \
      3   10
     / \   \
    1   6   14
```

Mark:

```text
- current node
- left/right return values
- what you're combining
```

---

## Draw the Recursion Stack

For `maxDepth(8)`:

```text
Call maxDepth(8)
  Call maxDepth(3)
    Call maxDepth(1)
      Call maxDepth(null) → 0
      Call maxDepth(null) → 0
    Return 1
    Call maxDepth(6)
      ...
    Return 2
  Return 3
  Call maxDepth(10)
    ...
  Return 2
Return 3
```

---

## Draw Pointers During Mutation

For inserting into BST:

```text
        8
       / \
      3   10
       \
        6  ← new node
```

Show parent pointer:

```text
parent(6) = 3
3.right = 6
```

For deletion with two children:

```text
Before:           After:
    8                9
   / \              / \
  3   10    →     3   10
     / \               \
    9   14              14
```

---

## Iterative Traversal — Draw the Stack

Inorder on tree above:

```text
Stack: push 8, 3, 1
Pop 1, visit
Pop 3, visit, push 6
Pop 6, visit
Pop 8, visit, push 10, push 14
...
```

This prevents losing track of where you are.

---

# 32. Problems to Practice — Grouped by Pattern

You don't need 100 random tree problems. Choose representatives.

## Fundamentals

- [ ] Maximum Depth of Binary Tree
- [ ] Same Tree
- [ ] Invert Binary Tree
- [ ] Count Complete Tree Nodes
- [ ] Symmetric Tree

## DFS — Divide and Conquer

- [ ] Diameter of Binary Tree
- [ ] Balanced Binary Tree
- [ ] Subtree of Another Tree
- [ ] Count Good Nodes in Binary Tree

## BFS / Level-Order

- [ ] Binary Tree Level Order Traversal
- [ ] Binary Tree Right Side View
- [ ] Average of Levels in Binary Tree
- [ ] Minimum Depth of Binary Tree
- [ ] Populating Next Right Pointers in Each Node

## BST

- [ ] Search in a BST
- [ ] Insert into a BST
- [ ] Delete Node in a BST
- [ ] Validate Binary Search Tree
- [ ] Kth Smallest Element in a BST
- [ ] Lowest Common Ancestor of a BST

## Path Problems

- [ ] Path Sum
- [ ] Path Sum II
- [ ] Path Sum III
- [ ] Binary Tree Maximum Path Sum
- [ ] Sum Root to Leaf Numbers

## LCA

- [ ] Lowest Common Ancestor of a Binary Tree

## Build Tree

- [ ] Construct Binary Tree from Preorder and Inorder
- [ ] Construct Binary Tree from Inorder and Postorder

## Heap

- [ ] Kth Largest Element in an Array
- [ ] Top K Frequent Elements
- [ ] Merge K Sorted Lists
- [ ] Find Median from Data Stream

## Trie

- [ ] Implement Trie
- [ ] Design Add and Search Words Data Structure
- [ ] Word Search II

## Serialization

- [ ] Serialize and Deserialize Binary Tree

## Advanced Composition

- [ ] Binary Tree Maximum Path Sum
- [ ] Count Univalue Subtrees
- [ ] House Robber III
- [ ] Binary Tree Cameras

That is enough to understand the major tree patterns deeply.

---
# 33. For EVERY Tree Problem

Use the same learning process we established for Arrays and Linked Lists.

Ask:

**1. What is the tree type?**

```text
General binary tree?
BST?
N-ary?
Implicit (heap array)?
Trie?
```

**2. What traversal fits?**

```text
Preorder (top-down)?
Inorder (sorted)?
Postorder (bottom-up)?
Level-order (BFS)?
```

**3. What information flows where?**

```text
Top-down (pass params to children)?
Bottom-up (aggregate from children)?
Both?
```

**4. What is the recursive structure?**

```text
solve(left) + solve(right) + combine?
Or single-child recursion?
```

**5. Can I use BST/heap/trie properties to prune?**

```text
Ordering?
Parent-child heap property?
Prefix?
```

**6. What is the base case?**

```text
null node?
leaf node?
single node?
```

**7. Do I need global state?**

```text
max diameter so far?
best path sum?
count?
```

**8. Iterative or recursive?**

```text
Recursion depth = tree height
Skewed tree may overflow stack
```

**9. Complexity?**

```text
Time:  O(n)? O(h)? O(n log n)?
Space: O(h)? O(w)? O(n)?
```

**10. Edge cases?**

```text
null
single node
skewed
duplicates (BST)
overflow
```

**11. Draw it.**

Always draw the tree and at least one recursion step before coding.

---

# 34. Compare Multiple Approaches

Example:

## Kth Smallest in BST

### Inorder with array

```text
Inorder → store all → return arr[k-1]

Time:  O(n)
Space: O(n)
```

### Inorder iterative (early stop)

```text
Stop when count == k

Time:  O(h + k)
Space: O(h)
```

### BST iterator

```text
Amortized O(1) per next()

Good for streaming kth queries
```

Don't just conclude one wins. Ask:

* Is k close to n or small?
* Multiple queries?
* Memory constrained?

---

## Kth Largest Element

### Sort

```text
Time:  O(n log n)
Space: O(1) or O(n)
```

### Min-heap of size k

```text
Time:  O(n log k)
Space: O(k)
```

### Quickselect

```text
Average: O(n)
Worst:   O(n²)
```

Trees/heaps give O(n log k) with clean code. Quickselect is faster average but trickier.

---

# 35. One Especially Important Mental Shift

When beginners see a tree problem, they think only about the current node.

As you improve, ask:

> What information does each **subtree** provide to its parent?

```text
Subtree height     → balanced check, diameter
Subtree sum        → path problems
Subtree min/max    → validate BST
Subtree match      → is subtree
Subtree path count → path sum III
```

And also:

> What information does the **ancestor path** provide to descendants?

```text
Remaining sum
Min/max bounds
Current path values
Depth
```

Tree problems are about **information flow** along edges — up or down.

---

# 36. Implement Trees Yourself

Don't just use library structures. Build from scratch.

## Version 1 — Binary Tree Node + Traversals

```text
Create nodes
Inorder, preorder, postorder (recursive + iterative)
Level-order (BFS)
```

## Version 2 — BST

```text
Search, insert, delete
Min, max
Inorder (sorted output)
```

## Version 3 — Min-Heap (array-based)

```text
Insert (sift up)
Extract min (sift down)
Heapify
```

## Version 4 — Trie

```text
Insert, search, startsWith
Delete (optional)
```

## Version 5 — AVL or Red-Black (stretch goal)

```text
Insert with rotations
Understand balance invariants
```

Once you've done those, you understand trees far beyond merely calling `TreeNode` in LeetCode.

---

# 37. Real-World Applications

```text
File systems           → general trees
Databases (indexes)    → B-trees, B+ trees (multi-way, on disk)
Priority scheduling    → heaps
Autocomplete           → tries
DOM                    → n-ary trees
Compilers (AST)        → binary/n-ary trees
Decision trees / ML    → tree structures
Git                    → DAG (not pure tree, but tree-like)
JSON/XML parsing       → tree representation
Game AI                → minimax trees
```

B-trees and B+ trees are the database industry's workhorse — generalization of balanced trees for **disk blocks**. Worth knowing they exist even if you don't implement them yet.

---

# Complete Trees Checklist

```text
TREES
│
├── Fundamentals
│   ├── Node, Root, Leaf
│   ├── Parent, Child, Sibling
│   ├── Depth, Height, Level
│   ├── Subtree, Edge count (n-1)
│   ├── Internal vs External node
│   ├── Tree properties (connected, acyclic)
│   └── Why trees exist
│
├── Binary Tree
│   ├── Definition (≤ 2 children)
│   ├── Shapes (skewed, balanced, complete, perfect)
│   ├── Traversals (in, pre, post, level)
│   ├── Height, count, mirror
│   └── Search O(n) unordered
│
├── Binary Search Tree
│   ├── Invariant (left < root < right)
│   ├── Search, insert, delete
│   ├── Three delete cases
│   ├── Min/max, successor/predecessor
│   ├── Inorder = sorted
│   └── O(h) complexity — skewed danger
│
├── Balanced BSTs
│   ├── Why balance matters
│   ├── AVL (balance factor, rotations)
│   ├── Red-Black (color invariants, concept)
│   ├── AVL vs Red-Black trade-offs
│   └── Guaranteed O(log n)
│
├── Heap
│   ├── Min-heap / max-heap property
│   ├── Complete binary tree shape
│   ├── Array representation (index formulas)
│   ├── Insert, extract, peek, heapify
│   ├── O(n) build heap
│   └── Priority queue use cases
│
├── Trie
│   ├── Prefix tree structure
│   ├── Insert, search, startsWith
│   ├── Space/time trade-offs
│   └── Autocomplete, dictionary apps
│
├── Traversals
│   ├── Inorder (L-Rt-R)
│   ├── Preorder (Rt-L-R)
│   ├── Postorder (L-R-Rt)
│   ├── Level-order / BFS
│   ├── Recursive vs iterative
│   ├── Stack vs queue
│   └── Morris O(1) space (awareness)
│
├── Memory / Representation
│   ├── Pointer-based nodes
│   ├── Array-based (heap indexing)
│   ├── Serialization / deserialization
│   └── Cache vs flexibility trade-offs
│
├── Core Patterns
│   ├── DFS recursive (divide-and-conquer)
│   ├── BFS / level-order
│   ├── BST search and pruning
│   ├── Inorder tricks (kth, validate, iterator)
│   ├── Path problems (root-leaf, any path)
│   ├── LCA (general + BST)
│   ├── Build from traversals
│   ├── Heap / top-K / two-heap
│   ├── Trie / prefix search
│   ├── Serialize / deserialize
│   ├── Subtree comparison
│   ├── Top-down vs bottom-up state
│   ├── Flatten / restructure
│   └── Pattern composition
│
├── Complexity
│   ├── O(n) traverse
│   ├── O(h) space for DFS
│   ├── O(w) space for BFS
│   ├── BST O(log n) balanced vs O(n) skewed
│   ├── Heap operations
│   └── Trie O(L) per operation
│
├── Comparisons
│   ├── Tree vs Array
│   ├── Tree vs Hash Table
│   ├── Tree vs Linked List
│   ├── BST vs Heap
│   └── AVL vs Red-Black
│
├── Implement Yourself
│   ├── Binary tree traversals
│   ├── BST operations
│   ├── Min-heap (array)
│   ├── Trie
│   └── AVL/RB (stretch)
│
└── For EVERY Problem
    ├── Identify tree type
    ├── Choose traversal
    ├── Determine info flow (up/down)
    ├── Define recursive structure
    ├── Exploit invariants (BST/heap/trie)
    ├── Handle base cases
    ├── Consider global state
    ├── Choose iterative vs recursive
    ├── Draw tree and stack
    ├── Analyze time and space
    ├── Test edge cases
    └── Explain WHY it works
```

For your **core DSA pass**, that's enough trees. You don't need to dive deeply yet into B-trees, suffix trees, segment trees, Fenwick trees, or persistent data structures.

Once you can **implement BST and heap from scratch**, explain why balance matters, traverse in all four orders (recursive and iterative), recognize DFS vs BFS vs BST vs heap vs trie patterns, and solve path/LCA/build problems confidently, you're ready for the next major block: **Searching & Sorting → Graphs → Dynamic Programming**.
