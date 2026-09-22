# Stacks, Queues & Deques — Complete Deep-Understanding Syllabus

## 1. Start with the core idea

An array or linked list lets you access/manipulate data in many ways.

A stack or queue intentionally limits that.

### Stack

**LIFO — Last In, First Out**

```text
push 10
push 20
push 30

TOP
 ↓
30
20
10
```

Remove:

```text
pop() -> 30
```

Think:

> The most recently added item is the first one removed.

### Queue

**FIFO — First In, First Out**

```text
FRONT                 BACK
  ↓                     ↓
10 → 20 → 30 → 40
```

Remove:

```text
dequeue() -> 10
```

Think:

> The oldest item leaves first.



### Deque

**Double-ended queue**

```text
FRONT                BACK
  ↓                    ↓
10 ↔ 20 ↔ 30 ↔ 40
```

You can insert/delete from **both ends**.

---



# 2. Stack Fundamentals

Understand:

- `push`
- `pop`
- `top/peek`
- `empty`
- `size`

Typical complexity:


| Operation | Complexity     |
| --------- | -------------- |
| Push      | O(1) amortized |
| Pop       | O(1)           |
| Top       | O(1)           |
| Empty     | O(1)           |


Implement a stack using:

- Dynamic array
- Linked list

Then understand the trade-off between them.

---



# 3. Why stacks exist

Stacks naturally model **nested work**.

Example:

```text
function A
   calls B
      calls C
```

Execution:

```text
A
A B
A B C
```

C finishes first, then B, then A.

That's LIFO.

So stacks naturally appear in:

- Function calls
- Recursion
- Parsing
- Undo
- Backtracking
- DFS
- Expression evaluation

---



# 4. Stack Pattern 1 — Basic LIFO State

Use a stack when you need to remember prior states and process the most recent one first.

Examples:

- Browser back
- Undo
- Directory traversal
- Expression processing

Ask:

> Do I need to return to the most recently encountered unfinished thing?

If yes, a stack is a strong candidate.

---



# 5. Stack Pattern 2 — Matching / Nested Structure

Classic example:

```text
({[]})
```

When you see an opening symbol:

```text
(
{
[
```

push it.

When you see a closing symbol:

```text
)
}
]
```

compare against the top.

This pattern applies to:

- Parentheses
- HTML/XML-like nesting
- Parser states
- Nested scopes

Deep idea:

> A stack validates properly nested structure because the most recent opening must close first.

---



# 6. Stack Pattern 3 — Expression Evaluation

Learn:

- Infix
- Prefix
- Postfix

Example:

```text
2 + 3 * 4
```

Understand operator precedence.

Then postfix:

```text
2 3 4 * +
```

Evaluation naturally uses a stack.

Learn:

- Operand stack
- Operator stack
- Basic infix → postfix idea
- Postfix evaluation

You don't need to become a compiler expert here.

---



# 7. Stack Pattern 4 — Monotonic Stack

This is one of the most important advanced array patterns, but it belongs conceptually with stacks.

A monotonic stack maintains elements in increasing or decreasing order.

Example:

```text
Array:
2 1 5 3 4
```

Typical questions:

- Next greater element
- Next smaller element
- Previous greater
- Previous smaller

Instead of comparing every element with every later element:

```text
O(n²)
```

a monotonic stack can often reduce it to:

```text
O(n)
```

because every element is typically pushed and popped at most once.

Learn:

- Increasing stack
- Decreasing stack
- Store values vs indices
- Why total work is O(n)

Representative problems:

- Next Greater Element
- Daily Temperatures
- Stock Span
- Largest Rectangle in Histogram
- Trapping Rain Water with stack approach

Do not memorize templates. Understand:

> The stack stores unresolved elements waiting for a future element that satisfies some relationship.

---



# 8. Stack Pattern 5 — DFS / Explicit Recursion Simulation

Recursive DFS:

```cpp
dfs(node);
```

implicitly uses the call stack.

You can convert it into:

```text
stack.push(start)
```

then repeatedly:

```text
pop
process
push neighbors
```

This teaches:

> Recursion is often just an implicit stack.

Very important conceptually.

---



# 9. Queue Fundamentals

Understand:

- `enqueue/push`
- `dequeue/pop`
- `front`
- `back`
- `empty`
- `size`

Queue:

```text
FRONT               BACK
  ↓                   ↓
10 → 20 → 30 → 40
```

Enqueue:

```text
10 → 20 → 30 → 40 → 50
```

Dequeue:

```text
20 → 30 → 40 → 50
```

Typical operations:


| Operation | Complexity |
| --------- | ---------- |
| Enqueue   | O(1)       |
| Dequeue   | O(1)       |
| Front     | O(1)       |
| Back      | O(1)       |


---



# 10. Queue Implementations

Implement using:

- Linked list
- Circular array

Do **not** implement a queue by repeatedly deleting index 0 from a normal array if that forces shifting.

That can become:

```text
O(n)
```

per dequeue.

This is why circular buffers are important.

---



# 11. Circular Queue

Suppose capacity = 5.

Instead of physically shifting elements:

```text
[10][20][30][40][ ]
```

maintain:

```text
front
rear
```

When rear reaches the end:

```text
rear = (rear + 1) % capacity
```

The array logically wraps around.

Understand:

- Front index
- Rear index
- Size
- Full vs empty state
- Modulo arithmetic
- Why circular buffers avoid shifting

This is a very useful systems concept.

---



# 12. Why queues exist

Queues model **arrival order**.

Examples:

- Task scheduling
- Print jobs
- Request processing
- Message queues
- BFS
- Buffers
- Producer/consumer systems

Ask:

> Should work be processed in the same order it arrives?

If yes, queue.

---



# 13. Queue Pattern 1 — BFS

This is the most important queue pattern.

Suppose a tree:

```text
        A
      /   \
     B     C
    / \   / \
   D   E F   G
```

BFS visits:

```text
A
B C
D E F G
```

Queue:

```text
[A]

pop A
push B,C

[B,C]

pop B
push D,E

[C,D,E]
```

This naturally processes by **distance/layer**.

Deep idea:

> A queue preserves frontier order.

That's why BFS finds shortest paths in unweighted graphs.

---



# 14. Queue Pattern 2 — Level Order Processing

Whenever a problem says:

- level by level
- layer by layer
- minimum number of steps
- nearest
- shortest unweighted distance

think queue/BFS.

Examples:

- Binary Tree Level Order Traversal
- Minimum depth
- Rotting Oranges
- Shortest path in grid
- Multi-source BFS

---



# 15. Queue Pattern 3 — Multi-source BFS

Instead of one starting node:

```text
A
```

start from several:

```text
A, B, C
```

Put all of them into the queue initially.

This is useful when asking:

> What is the nearest source for every location?

Examples:

- Nearest zero
- Rotting oranges
- Distance from nearest facility
- Spread simulations

---



# 16. Queue Pattern 4 — Worklist Processing

A queue is also a general-purpose **worklist**.

Pattern:

```text
put initial work in queue

while queue not empty:
    take one item
    process it
    maybe add more work
```

This appears in:

- Graph algorithms
- Compilers
- Dependency processing
- Event systems
- Task pipelines

---



# 17. Deque Fundamentals

Deque means:

> Double-ended queue.

Operations:

```text
push_front
push_back

pop_front
pop_back

front
back
```

All typically O(1).

Think of it as:

```text
STACK + QUEUE abilities
```

from both ends.

---



# 18. Why Deques Exist

Sometimes FIFO isn't enough.

Sometimes LIFO isn't enough.

You need both ends.

Examples:

- Sliding window optimization
- 0-1 BFS
- Work stealing
- Palindrome processing
- Scheduling policies
- Maintaining candidate sets

---



# 19. Deque Pattern 1 — Monotonic Queue

This is one of the most important deque patterns.

Problem:

> Find maximum in every sliding window of size `k`.

Naive:

```text
For every window:
    scan k elements
```

Complexity:

```text
O(nk)
```

With a monotonic deque:

```text
O(n)
```

Maintain candidate indices in decreasing-value order.

Example:

```text
Array:
1 3 -1 -3 5 3 6 7
```

For each window, the deque front represents the maximum candidate.

Learn:

- Why old indices must leave from front
- Why weaker candidates leave from back
- Why each index enters/leaves at most once
- Why total complexity is O(n)

Representative:

- Sliding Window Maximum

---



# 20. Deque Pattern 2 — 0-1 BFS

Later, when graphs arrive, this becomes useful.

If graph edge weights are only:

```text
0 or 1
```

you can use deque instead of Dijkstra.

If edge cost = 0:

```text
push_front
```

If edge cost = 1:

```text
push_back
```

You don't need to master this deeply yet; just know why deque becomes useful.

---



# 21. Deque Pattern 3 — Palindrome / Bidirectional Processing

For a sequence:

```text
r a c e c a r
```

Compare:

```text
front ↔ back
```

This idea can be modeled with a deque.

For strings, two pointers are usually simpler, but the deque makes the data-structure idea clear.

---



# 22. Stack vs Queue vs Deque

You should be able to choose based on behavior.


| Need                   | Structure |
| ---------------------- | --------- |
| Most recent first      | Stack     |
| Oldest first           | Queue     |
| Both ends              | Deque     |
| Nested structures      | Stack     |
| BFS / level traversal  | Queue     |
| DFS iterative          | Stack     |
| Sliding window maximum | Deque     |
| Undo                   | Stack     |
| Task arrival order     | Queue     |


---



# 23. Important Design Concept — ADT vs Implementation

This is worth understanding deeply.

A **Stack** is an abstract behavior:

```text
push/pop/top
```

It can be implemented using:

```text
Array
Linked List
```

A **Queue** is an abstract behavior:

```text
enqueue/dequeue
```

It can be implemented with:

```text
Linked List
Circular Array
Deque
```

So separate:

> **What operations does the structure promise?**

from:

> **How is it physically implemented?**

This distinction becomes very important later in software design.

---



# 24. Complexity Analysis

For every implementation, ask why.

Example: array stack.

```text
push
```

usually O(1), but occasionally resize:

```text
O(n)
```

Yet append remains:

```text
O(1) amortized
```

Queue with linked list:

```text
head + tail
```

gives O(1) enqueue/dequeue.

Queue without tail may make append O(n).

Again:

> Data structure metadata changes complexity.

---



# 25. Common Edge Cases

Always test:

### Stack

- Empty pop
- Empty top
- One element
- Many pushes
- Resize



### Queue

- Empty dequeue
- One element
- Front == rear conditions
- Circular wrap-around
- Full queue
- Reuse freed slots



### Deque

- Empty
- One item
- Push/pop same end
- Push one side/pop the other
- Circular wrap-around

---



# 26. Representative Problems

You don't need hundreds.

### Stack fundamentals

- [x] Implement Stack
- [x] Valid Parentheses
- [x] Min Stack
- [x] Evaluate Reverse Polish Notation
- [x] Simplify Path



### Monotonic Stack

- [x] Next Greater Element
- [x] Daily Temperatures
- [x] Stock Span
- [x] Largest Rectangle in Histogram



### Queue fundamentals

- [x] Implement Queue
- [x] Implement Circular Queue
- [x] Number of Recent Calls / simple stream queue



### Queue/BFS

- [ ] Binary Tree Level Order Traversal later with trees
- [ ] Rotting Oranges later with grids
- [ ] Shortest Path in Binary Matrix later with graphs/grids



### Deque

- [x] Implement Deque
- [x] Sliding Window Maximum
- [ ] Sliding Window Maximum → decreasing deque → largest at front Sliding Window Minimum → increasing deque → smallest at front

That's enough for the core pass.

---



# 27. Pattern Recognition

Train these clues.

```text
Nested / matching
        ↓
Stack
```

```text
Undo / previous state
        ↓
Stack
```

```text
Most recent unresolved element
        ↓
Stack
```

```text
Next greater / next smaller
        ↓
Monotonic Stack
```

```text
Level-by-level
        ↓
Queue / BFS
```

```text
Minimum steps in unweighted graph/grid
        ↓
Queue / BFS
```

```text
Arrival order
        ↓
Queue
```

```text
Need both ends
        ↓
Deque
```

```text
Sliding window max/min
        ↓
Monotonic Deque
```

Again: these are clues, not rigid rules.

---



# 28. For EVERY Problem

Use the same deep-learning framework:

```text
1. What behavior do I need?
   LIFO?
   FIFO?
   Both ends?

2. Why does that behavior fit the problem?

3. Which data structure gives it?

4. What should I store?
   Values?
   Indices?
   States?
   Nodes?

5. What invariant must remain true?

6. What enters the structure?

7. When does it leave?

8. Can each item be pushed/popped only once?

9. Time complexity?

10. Space complexity?

11. Edge cases?

12. Could another structure solve it?
    Why is this one better?
```

That "what should I store?" question is especially important.

For example, monotonic stack problems often require storing **indices rather than values** because you need both:

```text
value relationship
+
distance/position
```

---



# Complete Checklist

```text
STACKS → QUEUES → DEQUES
│
├── Stack
│   ├── LIFO
│   ├── Push / Pop / Top
│   ├── Array implementation
│   ├── Linked List implementation
│   ├── Complexity
│   │
│   ├── Patterns
│   │   ├── Nested / Matching
│   │   ├── Expression Evaluation
│   │   ├── DFS / Recursion Simulation
│   │   └── Monotonic Stack
│   │
│   └── Applications
│       ├── Undo
│       ├── Function Call Stack
│       ├── Parsing
│       └── Backtracking
│
├── Queue
│   ├── FIFO
│   ├── Enqueue / Dequeue
│   ├── Linked List implementation
│   ├── Circular Array implementation
│   ├── Front / Rear
│   ├── Wrap-around
│   ├── Complexity
│   │
│   └── Patterns
│       ├── BFS
│       ├── Level Order
│       ├── Multi-source BFS
│       └── Worklist Processing
│
├── Deque
│   ├── Push Front
│   ├── Push Back
│   ├── Pop Front
│   ├── Pop Back
│   ├── Complexity
│   │
│   └── Patterns
│       ├── Monotonic Deque
│       ├── Sliding Window Max/Min
│       └── 0-1 BFS concept
│
├── Concepts
│   ├── ADT vs Implementation
│   ├── Array vs Linked implementation
│   ├── Amortized Complexity
│   ├── Invariants
│   └── Choosing LIFO/FIFO/Both
│
└── For Every Problem
    ├── Identify required access order
    ├── Choose structure
    ├── Decide what to store
    ├── Define invariant
    ├── Trace push/pop
    ├── Analyze complexity
    ├── Check edge cases
    └── Explain WHY
```

One thing I'd explicitly emphasize: **do not postpone monotonic stack/deque** as some "advanced LeetCode trick." They are fundamental patterns arising directly from Stack/Deque behavior, so this is the right time to learn them.

After you complete this block, your natural next major topic is **Hash Tables / Hashing**, followed by **Trees**.

STACK Extra :  

The key to **monotonic stacks** is not memorizing “increasing stack = X” or “decreasing stack = Y.” Instead, build the mental model around **unresolved elements**.

Suppose:

```text
nums = [2, 1, 5, 3, 4]
```

For **Next Greater Element**, when you see `2`, you don't yet know its answer. So `2` becomes unresolved. Then `1` is also unresolved. When `5` appears, suddenly it resolves both:

```text
5 > 1
5 > 2
```

So:

```text
2 -> 5
1 -> 5
```

The stack is essentially a waiting room.

---



## 1. The core invariant

For Next Greater Element, scan left → right and keep indices whose answers haven't been found yet.

```text
nums:
index   0  1  2  3  4
value   2  1  5  3  4
```

Start:

```text
stack = []
```

See `2`:

```text
nothing waiting

push 2

stack:
[2]
```

See `1`.

Does `1` resolve `2`?

```text
1 > 2? no
```

So `1` also waits:

```text
stack:
[2, 1]
```

Notice something important:

```text
2 > 1
```

So from bottom → top, the stack is **decreasing**.

Now see `5`.

```text
5 > 1
```

Therefore:

```text
1's next greater = 5
```

Pop `1`.

Now:

```text
5 > 2
```

Therefore:

```text
2's next greater = 5
```

Pop `2`.

Then push `5`:

```text
stack:
[5]
```

See `3`:

```text
3 > 5? no
```

Push:

```text
[5, 3]
```

See `4`:

```text
4 > 3
```

Resolve `3`:

```text
3 -> 4
```

But:

```text
4 > 5? no
```

So push `4`.

Final stack:

```text
[5, 4]
```

These were never resolved.

Therefore:

```text
value:        2  1  5  3  4
next greater:5  5 -1  4 -1
```

---



# 2. Why the stack becomes monotonic

This is the part worth understanding deeply.

After processing each element, imagine we had:

```text
[2, 1]
```

and `5` arrived.

Keeping `1` would make no sense because its question:

```text
"what is the first greater value after me?"
```

has just been answered.

Likewise for `2`.

So resolved elements disappear.

What's left consists only of elements for which the current number wasn't sufficient.

That naturally creates an ordered stack.

For Next Greater:

```text
while current > stack top:
    resolve stack top
```

Anything smaller than `current` disappears.

Therefore what's left is generally decreasing:

```text
large
 ↓
small
```

Hence the term:

> **monotonic decreasing stack**

---



# 3. The real template

Conceptually, most monotonic-stack algorithms are doing this:

```text
for each current element:

    while stack isn't empty
          and current resolves stack.top:

        unresolved = stack.pop()

        record answer for unresolved

    stack.push(current)
```

That's much more useful than memorizing a particular Java/Python template.

Ask:

> **What elements are currently waiting for an answer?**

and:

> **Can the current element answer their question?**

That's the pattern.

---



# 4. Why indices are usually better than values

Suppose:

```text
nums = [2, 1, 5]
```

For basic Next Greater Element, storing values works:

```text
stack = [2, 1]
```

But consider Daily Temperatures.

```text
temperatures = [73, 74, 75, 71, 69, 72]
```

The problem doesn't ask:

```text
"What warmer temperature comes next?"
```

It asks:

```text
"How many days until a warmer temperature?"
```

If day `3` gets resolved by day `5`:

```text
days waited = 5 - 3 = 2
```

Therefore you need indices.

Store:

```text
stack = [indices]
```

Then access values using:

```text
temperatures[stack.top]
```

This is why **storing indices is usually the safest default**.

An index gives you both:

```text
value    -> nums[index]
position -> index
```

A value only gives you the value.

---



# 5. The four fundamental questions

There are four basic relationship queries.

```text
                 direction

              previous      next
             ---------------------
greater       prev greater  next greater

smaller       prev smaller  next smaller
```

The useful distinction is whether you want to resolve:

```text
past elements using current
```

or determine:

```text
current's relationship with past elements
```

---



## Next Greater Element

Question:

```text
For every element, find the first larger element to its right.
```

Use unresolved previous elements.

```text
while nums[i] > nums[stack.top]:
    resolve stack.top
```

Stack tends to be:

```text
decreasing
```

Example:

```text
[2,1,5,3,4]

2 -> 5
1 -> 5
5 -> none
3 -> 4
4 -> none
```

---



## Next Smaller Element

Question:

```text
find first smaller element to the right
```

Current resolves larger waiting elements:

```text
while nums[i] < nums[stack.top]:
    resolve stack.top
```

Stack tends to be:

```text
increasing
```

---



## Previous Greater Element

Here the interpretation changes slightly.

When processing `nums[i]`, we want the closest previous greater value.

Before using the stack:

```text
remove elements that cannot possibly be the answer
```

If they are:

```text
<= current
```

they aren't greater.

So:

```text
while stack not empty
      and nums[stack.top] <= nums[i]:

    stack.pop()
```

Now:

```text
stack.top
```

is the nearest previous greater element.

Then push `i`.

---



## Previous Smaller Element

Symmetrically:

```text
while stack not empty
      and nums[stack.top] >= nums[i]:

    stack.pop()
```

Then:

```text
stack.top
```

is the nearest previous smaller element.

---



# 6. One extremely useful distinction

There are two ways you should think about stack popping.

### Pattern A — current element resolves old elements

Used heavily for:

```text
Next Greater
Next Smaller
Daily Temperatures
```

Think:

```text
"Who has the current element answered?"
```

Example:

```text
stack: [73, 71, 69]

current = 72
```

`72` resolves:

```text
69
71
```

but not:

```text
73
```

---



### Pattern B — remove useless candidates for current

Used heavily for:

```text
Previous Greater
Previous Smaller
Histogram boundary calculations
```

Think:

```text
"Which old elements cannot be the answer for current?"
```

That distinction removes a lot of monotonic-stack confusion.

---



# 7. Why the algorithm is O(n)

You'll often see:

```text
for i in range(n):
    while stack:
        ...
```

and think:

```text
for + while = O(n²)
```

But nested loops do **not automatically mean O(n²)**.

Consider every index individually.

An index can be:

```text
pushed once
```

and later:

```text
popped once
```

That's it.

For `n` elements:

```text
<= n pushes
<= n pops
```

Therefore total stack operations are bounded by approximately:

```text
2n
```

So:

```text
O(n)
```

This is called **amortized analysis**.

For example:

```text
1 2 3 4 5
```

When each new element arrives, it may pop something.

But once something gets popped:

```text
it is gone forever
```

It can't cause work again.

That's the critical reason.

genui{"learning_viz":{"type_id":"BIG_O_TIME_COMPLEXITY"}}

The stack approach replaces repeated future comparisons with a structure where each element participates only a constant number of times.

---



# 8. Daily Temperatures

This is probably the cleanest problem for learning the pattern.

Suppose:

```text
[73, 74, 75, 71, 69, 72, 76, 73]
```

Question:

> How many days until a warmer temperature?

Use indices:

```text
stack = indices of days waiting for a warmer day
```

Start:

```text
day 0 = 73

stack:
[0]
```

Day 1:

```text
74 > 73
```

Therefore day `0` is resolved:

```text
answer[0] = 1 - 0 = 1
```

Push `1`.

Day 2:

```text
75 > 74
```

Resolve day 1:

```text
answer[1] = 2 - 1 = 1
```

Push `2`.

Eventually:

```text
day 4 = 69
```

Stack might contain:

```text
75, 71, 69
```

Then day 5:

```text
72
```

arrives.

It resolves:

```text
69
71
```

but not:

```text
75
```

Exactly the unresolved-element model.

---



# 9. Stock Span

Stock Span sounds different but is closely related.

Suppose:

```text
prices = [100, 80, 60, 70, 60, 75, 85]
```

For each day, determine how many consecutive previous days had price:

```text
<= today's price
```

Consider:

```text
today = 75
```

Previous prices:

```text
60
70
60
```

are all ≤ 75.

But:

```text
80 > 75
```

stops the span.

So what you're really looking for is:

> **Previous Greater Element**

If previous greater is at index `j`:

```text
span = i - j
```

If none exists:

```text
span = i + 1
```

This illustrates something important:

> Many problems don't say “find previous greater element,” but structurally that's exactly what they require.

Pattern recognition matters more than the problem's vocabulary.

---



# 10. Largest Rectangle in Histogram

This is where monotonic stacks become much more powerful.

Example:

```text
heights = [2, 1, 5, 6, 2, 3]
```

For each bar, imagine treating its height as the rectangle height.

You want to know:

```text
How far left can this height extend?
How far right can this height extend?
```

It can extend until encountering a bar:

```text
strictly smaller than it
```

So each bar cares about:

```text
Previous Smaller Element
Next Smaller Element
```

Suppose:

```text
height = 5
index = 2
```

Array:

```text
2 1 5 6 2 3
    ^
```

Previous smaller:

```text
1 at index 1
```

Next smaller:

```text
2 at index 4
```

Therefore `5` can occupy:

```text
indices 2..3
```

Width:

```text
4 - 1 - 1 = 2
```

Area:

```text
5 * 2 = 10
```

General relationship:

```text
width = rightSmallerIndex - leftSmallerIndex - 1
```

This problem is one of the strongest reasons to truly understand monotonic stacks rather than memorize Next Greater Element.

---



# 11. Trapping Rain Water

A stack-based interpretation is different again.

Consider:

```text
[4, 2, 0, 3, 2, 5]
```

A lower bar waits for a sufficiently high bar on the right to create a container.

When a taller bar arrives, you pop lower bars and calculate water using:

```text
left boundary
bottom
right boundary
```

The stack again represents unresolved structure.

But now the resolution isn't merely:

```text
next greater = current
```

Instead:

```text
current closes a basin
```

This demonstrates the generality of the idea:

> A monotonic stack doesn't have to literally answer “next greater.” It can maintain unresolved geometric relationships.

---



# 12. `<` vs `<=` matters

This is one of the most common interview bugs.

Suppose:

```text
[2, 2, 3]
```

Do equal values count as greater?

Normally:

```text
No.
```

So for Next **Strictly Greater**:

```text
current > stackTop
```

not:

```text
current >= stackTop
```

But other problems need duplicates collapsed.

For example, when finding boundaries in Histogram problems, choices such as:

```text
>
>=
```

determine which equal-height bar owns a particular range.

There isn't one universally correct inequality.

Always translate the English requirement:

```text
greater       >
greater/equal >=
smaller       <
smaller/equal <=
```

Then design the stack condition from that.

---



# 13. How to recognize a monotonic-stack problem

Look for language like:

```text
next
previous
nearest
first
greater
smaller
warmer
higher
lower
until
span
boundary
```

especially combined with:

```text
for every element...
```

and a brute-force solution resembling:

```text
for each i:
    scan left/right until condition is found
```

That is a major monotonic-stack signal.

For example:

```text
for each temperature:
    search forward until warmer
```

Brute force:

```text
O(n²)
```

Ask:

> Can unresolved earlier positions wait in a stack until a future value resolves them?

If yes, investigate a monotonic stack.

---



# 14. But don't use it merely because the array is ordered

A common mistake is:

```text
"I need next something, therefore monotonic stack."
```

Not necessarily.

Monotonic stacks work particularly well when failed candidates can be **permanently discarded**.

Suppose current element makes some older element irrelevant not just now, but for all future work.

That's when the stack becomes powerful.

For Next Greater:

```text
old = 2
current = 5
```

Once `5` resolves `2`, we never need `2` again.

That irreversible elimination is what gives you linear time.

---



# 15. A better problem-solving checklist

When encountering one of these problems, don't start with:

```text
Should I use increasing or decreasing stack?
```

Start with:

```text
1. What answer does each position need?

2. Which positions currently don't know their answer?

3. Can the current element resolve any of them?

4. If something gets popped, why can I safely forget it forever?

5. Do I need the value, index, distance, or boundary?

6. How should equal values behave?
```

Only **after** answering those should you ask:

```text
What monotonic order naturally results?
```

That prevents template memorization.

---



# 16. The deepest mental model

You can view an ordinary stack as remembering:

```text
unfinished nested work
```

Examples:

```text
parentheses
function calls
DFS
expression parsing
```

A monotonic stack remembers:

```text
unfinished ordered relationships
```

For example:

```text
index 7:
"I'm waiting for someone greater than me."

index 8:
"I'm also waiting."

index 9:
"I'm also waiting."

current index 10 arrives:
"I can resolve some of you."
```

Those it resolves disappear.

Those it can't resolve remain.

That is why this sentence is the right one to internalize:

> **The stack stores unresolved elements waiting for a future element that satisfies some relationship.**

Once that becomes intuitive, **Daily Temperatures, Stock Span, Next Greater/Smaller, Histogram, and eventually Trapping Rain Water stop feeling like five unrelated tricks. They become variations of the same idea.**