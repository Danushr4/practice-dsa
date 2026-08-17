# Linked Lists

## 1. Linked List Fundamentals

First understand **why linked lists exist**.

An array stores elements contiguously:

```text
ARRAY

Address:
1000    1004    1008    1012

[10]    [20]    [30]    [40]
```

A linked list does not require contiguous memory:

```text
LINKED LIST

1000             8400             3200

[10 | 8400] ---> [20 | 3200] ---> [30 | NULL]
```

Each node contains:

```cpp
struct Node {
    int value;
    Node* next;
};
```

Conceptually:

```text
Node
┌─────────────┐
│ data        │
├─────────────┤
│ next        │ ──────>
└─────────────┘
```

Understand:

- What is a node?
- What is a reference/pointer?
- What does `head` represent?
- What does `tail` represent?
- What does `nullptr/null` mean?
- Why aren't nodes contiguous?
- How do we reach the nth node?
- Why is random access O(n)?
- Why can insertion/deletion be O(1) once the relevant node is known?

---



# 2. Why Linked Lists Exist

Compare an array:

```text
[10][20][30][40]
```

Insert `15` after `10`:

```text
[10][15][20][30][40]
```

Potentially many elements must move.

With a linked list:

```text
10 → 20 → 30 → 40
```

Create:

```text
15
```

Change links:

```text
10 → 15 → 20 → 30 → 40
```

No shifting of the later elements.

But there is a trade-off.

Arrays give excellent:

```text
random access
cache locality
compact storage
```

Linked lists give flexible:

```text
node insertion/deletion
non-contiguous allocation
structural rearrangement
```

So don't learn:

> Linked List is better for insertion.

Learn:

> Under what conditions is linked-list insertion actually cheaper?

For example, finding the insertion point may itself cost **O(n)**.

---



# 3. Types of Linked Lists

You should understand all major forms.

## Singly Linked List

```text
HEAD
 ↓
10 → 20 → 30 → 40 → NULL
```

Each node knows only its successor.

```cpp
Node {
    value
    next
}
```

---



## Doubly Linked List

```text
NULL ← 10 ⇄ 20 ⇄ 30 ⇄ 40 → NULL
```

Node:

```cpp
Node {
    value
    prev
    next
}
```

Understand why the extra pointer makes some operations easier but increases memory usage and bookkeeping.

---



## Circular Linked List

```text
      ┌─────────────────┐
      ↓                 │
10 → 20 → 30 → 40 ─────┘
```

There is no traditional `NULL` end.

Useful for understanding cyclic structures, round-robin behavior, and some queue implementations.

---



# 4. Core Linked List Operations

Implement these yourself.

Start with a singly linked list.

### Traversal

```text
HEAD
 ↓
10 → 20 → 30 → NULL
↑
current
```

Move:

```text
current = current->next
```

until:

```text
current == nullptr
```

Complexity:

```text
Time  O(n)
Space O(1)
```

---



## Search

```text
10 → 20 → 30 → 40
          ↑
       target?
```

Worst case:

```text
O(n)
```

Unlike an array, you cannot directly jump to index 3.

---



# 5. Insertion

Learn every important case.

### Insert at beginning

Before:

```text
HEAD
 ↓
20 → 30 → 40
```

Create `10`:

```text
10
```

Then:

```text
10.next = head
head = 10
```

Result:

```text
HEAD
 ↓
10 → 20 → 30 → 40
```

O(1).

---



### Insert at end

Without a tail pointer:

```text
10 → 20 → 30 → NULL
```

You must traverse to the end.

```text
O(n)
```

With a maintained tail pointer:

```text
HEAD                 TAIL
 ↓                     ↓
10 → 20 → 30 → 40 → NULL
```

Appending can be O(1).

This teaches an important DSA lesson:

> Complexity often depends on what metadata your data structure maintains.

---



### Insert after a known node

```text
10 → 20 → 40
     ↑
   known
```

Insert 30:

```text
30.next = 20.next
20.next = 30
```

Result:

```text
10 → 20 → 30 → 40
```

O(1).

But **finding** `20` **may have cost O(n)**.

---



# 6. Deletion

Learn:

- Delete head
- Delete tail
- Delete by value
- Delete known node / node after known predecessor
- Delete at position
- Doubly linked list deletion

Example:

```text
10 → 20 → 30 → 40
     ↑
   delete
```

Conceptually:

```text
10 ─────────→ 30 → 40

     20
```

You're not "moving elements."

You're **rewiring links**.

This mental model is extremely important.

---



# 7. Complexity

Know the reasoning rather than memorizing this table.


| Operation               | Singly Linked List |
| ----------------------- | ------------------ |
| Access index            | O(n)               |
| Search                  | O(n)               |
| Insert at head          | O(1)               |
| Delete head             | O(1)               |
| Insert after known node | O(1)               |
| Delete after known node | O(1)               |
| Append without tail     | O(n)               |
| Append with tail        | O(1)               |


Then compare against arrays.


| Property             | Array     | Linked List |
| -------------------- | --------- | ----------- |
| Random access        | O(1)      | O(n)        |
| Search unsorted      | O(n)      | O(n)        |
| Insert front         | O(n)      | O(1)        |
| Cache locality       | Excellent | Poor        |
| Extra pointer memory | No        | Yes         |
| Contiguous memory    | Yes       | No          |


---



# 8. Memory Management

Because you're learning this deeply, don't skip this.

Especially if you're using C++.

Understand:

```cpp
Node* node = new Node();
```

and:

```cpp
delete node;
```

Understand conceptually:

- Stack vs heap
- Dynamic allocation
- Pointer ownership
- Memory leaks
- Dangling pointers
- Null pointers
- Double deletion at a conceptual level
- RAII/smart pointers later

For example:

```text
Before:

A → B → C
```

Suppose you remove B:

```text
A ─────→ C

    B
```

In a manually managed environment, B still occupies memory until it is freed.

Structural deletion and memory deallocation are related but distinct concepts.

---



# 9. The Most Important Linked List Patterns

Now we get to the equivalent of your Array patterns.

These are the patterns that naturally arise from linked lists.

## Pattern 1 — Simple Traversal

The foundation of everything.

```text
current
 ↓
10 → 20 → 30 → 40 → NULL
```

Move:

```text
current = current->next
```

Used for:

- Searching
- Counting
- Printing
- Finding max/min
- Checking properties
- Finding tail

---



# Pattern 2 — Pointer Rewiring

This is arguably **the central Linked List skill**.

You need to become comfortable changing:

```text
A → B → C
```

into:

```text
A → C

B
```

or:

```text
A → B
```

into:

```text
A → X → B
```

Many Linked List problems are fundamentally:

> Save the links you need, then reconnect pointers without losing access to the rest of the structure.

---



# Pattern 3 — Linked List Reversal

Extremely important.

Original:

```text
1 → 2 → 3 → 4 → NULL
```

Wanted:

```text
NULL ← 1 ← 2 ← 3 ← 4
```

Use three conceptual pointers:

```text
prev
current
next
```

During reversal:

```text
prev       current       next
 ↓            ↓            ↓
1    ←        2    →       3 → 4
```

Core sequence:

```cpp
next = current->next;
current->next = prev;
prev = current;
current = next;
```

Don't memorize these four lines blindly.

Understand **why** `next` **must be saved before changing** `current->next`.

Otherwise you lose the remainder of the list.

Variants later:

- Reverse entire list
- Reverse sublist
- Reverse first `k`
- Reverse in groups of `k`
- Reverse recursively

---



# Pattern 4 — Fast & Slow Pointers

One of the most important linked-list patterns.

```text
slow →
fast ---->
```

Fast moves two nodes.

Slow moves one.

```cpp
slow = slow->next;
fast = fast->next->next;
```

Used for:

### Find middle

```text
1 → 2 → 3 → 4 → 5
        ↑
       slow
```

When fast reaches the end, slow is around the middle.

### Detect cycles

Suppose:

```text
1 → 2 → 3 → 4
        ↑     ↓
        6 ← 5
```

Fast and slow eventually meet if a cycle exists.

This is **Floyd's Cycle Detection Algorithm**.

Learn:

- Why they must eventually meet
- How to detect a cycle
- How to find the cycle entry
- Cycle length conceptually

Don't merely memorize:

```cpp
slow = slow->next;
fast = fast->next->next;
```

Understand the relative-motion argument.

---



# Pattern 5 — Dummy/Sentinel Node

This is an extremely useful implementation pattern.

Suppose deletion could affect the head.

Without dummy:

```text
HEAD
 ↓
1 → 2 → 3
```

Deleting `1` requires special handling.

Instead:

```text
DUMMY → 1 → 2 → 3
```

Now even the original head has a predecessor.

This simplifies:

- Delete nodes
- Merge lists
- Partition list
- Build output lists
- Remove nth node
- Many insertion operations

Important lesson:

> Dummy nodes eliminate edge-case branching by giving the head a predecessor.

---



# Pattern 6 — Two Lists / Merge Pointers

Example:

```text
A: 1 → 4 → 7

B: 2 → 3 → 8
```

Merge:

```text
1 → 2 → 3 → 4 → 7 → 8
```

Typically maintain:

```text
dummy
tail
a
b
```

Used for:

- Merge two sorted lists
- Merge sort on linked lists
- List intersection-style reasoning
- Building result lists

---



# Pattern 7 — Gap Between Two Pointers

Very useful.

Instead of fast moving 2× slow, maintain a fixed gap.

Example: find kth node from the end.

```text
1 → 2 → 3 → 4 → 5 → NULL
```

Move `fast` ahead by `k`.

Then:

```text
slow →
fast →
```

move both together.

When fast reaches the end:

```text
slow
```

is positioned relative to the kth-from-end target.

Used for:

- kth node from end
- Remove nth node from end
- Fixed-distance node relationships

---



# Pattern 8 — Intersection of Two Linked Lists

Consider:

```text
A: 1 → 2 ─┐
           ↓
           8 → 9 → 10
           ↑
B: 5 → 6 ─┘
```

You want the shared node.

A beautiful pointer technique is:

```text
pointerA traverses A then B
pointerB traverses B then A
```

Both eventually travel the same total distance.

This teaches something deeper than one LeetCode trick:

> Different traversal paths can be normalized by equalizing total distance.

---



# Pattern 9 — Recursive Linked List Processing

Linked lists naturally have a recursive structure:

```text
node
 ↓
[value | next]
         ↓
      another list
```

You can think:

```text
List = Node + Remaining List
```

Learn recursive versions of:

- Traversal
- Reverse
- Merge
- Delete/search

But always analyze call-stack cost.

For example:

```text
Iterative reverse

Time  O(n)
Space O(1)
```

versus recursive reverse:

```text
Time  O(n)
Space O(n) call stack
```

---



# Pattern 10 — In-place Structural Transformation

Linked Lists are excellent for teaching this.

Example:

```text
1 → 2 → 3 → 4 → 5
```

could be transformed into something like:

```text
1 → 5 → 2 → 4 → 3
```

without allocating another list.

Typical process:

```text
Find middle
     ↓
Split
     ↓
Reverse second half
     ↓
Merge/interleave
```

This teaches how **multiple simple patterns compose into a larger algorithm**.

Very important.

---



# 10. Sorting a Linked List

Understand why linked lists change the choice of sorting algorithm.

### Merge Sort

Very natural for linked lists.

```text
List
 ↓
Find middle
 ↓
Split

Left       Right
 ↓           ↓
Sort        Sort
  \         /
     Merge
```

Complexity:

```text
O(n log n)
```

Learn:

- Find middle with slow/fast
- Split
- Recursive merge sort
- Merge sorted lists

This is a great example of combining:

```text
Fast/Slow
+
Recursion
+
Merge
```

---



# 11. Pattern Recognition

This is the part you should train deliberately.

When you see:

```text
Need middle
      ↓
Fast + Slow
```

When you see:

```text
Cycle
      ↓
Fast + Slow / Floyd
```

When you see:

```text
Nth node from end
      ↓
Two pointers with gap
```

When you see:

```text
Reverse links
      ↓
prev/current/next
```

When you see:

```text
Head may change
      ↓
Dummy node
```

When you see:

```text
Two sorted linked lists
      ↓
Merge pointers
```

When you see:

```text
Intersection
      ↓
Pointer path/distance normalization
```

When you see:

```text
Palindrome linked list
      ↓
Find middle
+
Reverse half
+
Compare
```

When you see:

```text
Reorder list
      ↓
Middle
+
Reverse
+
Merge
```

Notice the last two.

You're no longer learning a new magical algorithm.

You're **composing patterns you already understand**.

That's where DSA starts becoming powerful.

---



# 12. Edge Cases — Extremely Important for Linked Lists

Linked-list code breaks easily because of edge cases.

Always test:

```text
Empty list

NULL
```

Single node:

```text
1 → NULL
```

Two nodes:

```text
1 → 2 → NULL
```

Odd number:

```text
1 → 2 → 3 → 4 → 5
```

Even number:

```text
1 → 2 → 3 → 4
```

Also consider:

- Deleting head
- Deleting tail
- Target not present
- Duplicate values
- Entire list removed
- Cycle
- Head changes after operation

---



# 13. Important Implementation Skill: Draw the Pointers

For linked lists, **don't try to do everything mentally initially**.

Suppose:

```text
prev     curr     next
 ↓        ↓        ↓
A        B        C → D
```

Then draw what happens after every pointer modification.

For example:

```text
curr.next = prev
```

produces:

```text
A ← B       C → D
    ↑       ↑
   curr    next
```

This prevents the classic mistake:

```text
"I changed the pointer and lost the rest of my list."
```

Linked Lists become dramatically easier when you visualize them.

---



# 14. Problems to Practice

You don't need 100 random Linked List problems.

Choose problems representing each concept.

### Fundamentals

- Implement Singly Linked List
- Implement Doubly Linked List
- Insert/Delete/Search
- Find length



### Reversal

- Reverse Linked List
- Reverse Linked List II
- Reverse Nodes in K Group — later/harder



### Fast/Slow

- Middle of Linked List
- Linked List Cycle
- Linked List Cycle II
- Happy Number as a conceptual application of Floyd



### Two-pointer gap

- Remove Nth Node From End



### Dummy node

- Remove Linked List Elements
- Remove Nth Node From End
- Merge Two Sorted Lists



### Composition

- Palindrome Linked List
- Reorder List



### Structural relationships

- Intersection of Two Linked Lists



### Sorting

- Sort List

That is enough to understand the major Linked List patterns deeply.

---



# 15. For EVERY Linked List Problem

Use the same learning process we established for Arrays.

Ask:

**1. What is the structure?**

```text
Singly?
Doubly?
Circular?
Cycle?
```

**2. What information do I need?**

```text
previous?
current?
next?
head?
tail?
middle?
```

**3. What pattern naturally fits?**

```text
Traversal?
Reversal?
Fast/slow?
Gap?
Dummy?
Merge?
```

**4. Which pointers will change?**

Draw them.

**5. Can changing a pointer make me lose access to part of the list?**

This question is crucial.

**6. Complexity?**

```text
Time?
Space?
```

**7. Edge cases?**

```text
NULL
one node
two nodes
head changes
tail changes
```

---



# Your Complete Linked Lists Checklist

```text
LINKED LISTS
│
├── Fundamentals
│   ├── Why Linked Lists exist
│   ├── Node
│   ├── Head / Tail
│   ├── Pointer / Reference
│   ├── Non-contiguous memory
│   ├── Array vs Linked List
│   └── Complexity
│
├── Types
│   ├── Singly Linked List
│   ├── Doubly Linked List
│   └── Circular Linked List
│
├── Core Operations
│   ├── Traverse
│   ├── Search
│   ├── Insert head
│   ├── Insert tail
│   ├── Insert middle
│   ├── Delete head
│   ├── Delete tail
│   └── Delete node
│
├── Memory
│   ├── Allocation
│   ├── Deallocation
│   ├── Ownership basics
│   ├── Memory leaks
│   ├── Dangling pointers
│   └── Stack vs Heap basics
│
├── Patterns
│   ├── Traversal
│   ├── Pointer Rewiring
│   ├── Reversal
│   ├── Fast & Slow Pointers
│   ├── Floyd Cycle Detection
│   ├── Dummy/Sentinel Node
│   ├── Merge Pointers
│   ├── Two Pointers with Gap
│   ├── Intersection
│   ├── Recursion
│   └── In-place Transformation
│
├── Pattern Composition
│   ├── Palindrome
│   │      Middle + Reverse + Compare
│   │
│   ├── Reorder
│   │      Middle + Reverse + Merge
│   │
│   └── Merge Sort
│          Middle + Split + Sort + Merge
│
├── Analysis
│   ├── Time Complexity
│   ├── Space Complexity
│   ├── Iterative vs Recursive
│   └── Array vs List trade-offs
│
└── For EVERY Problem
    ├── Draw the list
    ├── Mark pointers
    ├── Identify invariant
    ├── Identify pattern
    ├── Track pointer changes
    ├── Check lost references
    ├── Handle edge cases
    ├── Analyze time
    ├── Analyze space
    └── Explain WHY it works
```

