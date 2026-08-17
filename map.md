# DSA Learning Map

| Phase | Topic | Notes | Why first? |
| ----- | ----- | ----- | ---------- |
| 1 | Complexity Analysis | [notes.md](complexity-analysis/notes.md) | Everything else depends on it. |
| 2 | Arrays & Strings | [notes.md](array/notes.md) | Foundation for memory layout and many algorithms. |
| 3 | Linked Lists | [notes.md](linkedlist/notes.md) | Understand pointers/references and dynamic memory. |
| 4 | Stacks, Queues, Deques | [notes.md](stack-queue-deque/notes.md) | Learn restricted-access data structures. |
| 5 | Hash Tables | [notes.md](hashmap/notes.md) | Introduces hashing, collisions, and average vs worst-case complexity. |
| 6 | Trees | [notes.md](trees/notes.md) | Binary Trees → BST → AVL → Red-Black → Heaps → Tries |
| 7 | Searching & Sorting | [notes.md](search-sort/notes.md) | Binary Search, Merge, Quick, Heap, Counting, Radix |
| 8 | Graphs | [notes.md](graphs/notes.md) | Representation → BFS → DFS → Topological Sort → MST → Shortest Paths |
| 9 | Dynamic Programming | [notes.md](dynamic-programming/notes.md) | Once recursion and graphs feel comfortable. |
| 10 | Greedy & Divide-and-Conquer | [notes.md](greedy-divide-conquer/notes.md) | Compare when they work versus DP. |
| 11 | Backtracking | [notes.md](backtracking/notes.md) | Build on recursion fundamentals. |
| 12 | Advanced Data Structures | [notes.md](advanced-data-structures/notes.md) | Segment Tree, Fenwick Tree, Sparse Table, etc. |
| 13 | String Algorithms | [notes.md](string-algorithms/notes.md) | KMP, Z, Rabin-Karp, Suffix Arrays/Trees |
| 14 | Advanced Graph Algorithms | [notes.md](advanced-graph-algorithms/notes.md) | SCC, Network Flow, Matching, LCA |

### For each topic, revise in this order:

1. **Motivation** – What problem does it solve?
2. **Theory** – Core ideas and invariants.
3. **Implementation** – Write it from scratch.
4. **Complexity** – Time and space analysis.
5. **Variants** – Related structures or algorithms.
6. **Applications** – Where it's used in real systems.
7. **Problems** – Solve a handful of representative problems.

For example, for **Hash Tables**:

- Why not just use arrays?
- How does hashing work?
- Separate chaining vs. open addressing.
- Implement your own hash table.
- Analyze load factor and resizing.
- See how languages like Java, C++, and Python implement maps.
- Solve a few problems involving frequency counting, sets, and caching.

### If your goal is mastery rather than interviews

After revising a topic, spend time answering questions like:

- Why was this invented?
- What are its limitations?
- What alternatives exist?
- How is it implemented in real programming languages or databases?
- What are the practical trade-offs?

That level of questioning turns DSA from a collection of algorithms into a way of thinking.

For someone aiming for deep understanding, I'd spend **2–4 weeks on trees and graphs each**, because they form the conceptual foundation for many advanced areas in computer science.

### Array pattern deep-dives

The array syllabus links to focused pattern files:

| Pattern | File |
| ------- | ---- |
| Two Pointers | [twoptr.md](array/twoptr.md) |
| Sliding Window | [slidingwindow.md](array/slidingwindow.md) |
| Prefix Sum + Hash Map | [prefixsum-hashmap.md](array/prefixsum-hashmap.md) |
| Sort + Scan | [sortscan.md](array/sortscan.md) |
| Binary Search (in-place) | [binarysearch-inplace.md](array/binarysearch-inplace.md) |
| 2D Matrix Traversal | [matrix_2dTra.md](array/matrix_2dTra.md) |
