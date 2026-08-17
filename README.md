# practice-dsa

A structured, deep-understanding DSA study repo — not a LeetCode grind list. Each topic has a full syllabus (`notes.md`) built around **why → theory → patterns → problems → checklist**.

**Start here:** [map.md](map.md) — the master learning roadmap for all 14 phases.

---

## Quick Start

1. Open **[map.md](map.md)** and follow phases **in order** (Complexity → Arrays → … → Advanced Graphs).
2. For each phase, open that topic's **`notes.md`**.
3. Read **Fundamentals → Patterns → Pattern Recognition** before jumping to problems.
4. Solve the **representative problems** listed at the end of each `notes.md`.
5. Use the **Complete Checklist** at the bottom to verify you've covered everything.

```text
map.md  →  topic/notes.md  →  patterns  →  practice problems  →  checklist ✓
```

---

## Learning Path (14 Phases)

| # | Topic | Notes |
|---|-------|-------|
| 1 | Complexity Analysis | [complexity-analysis/notes.md](complexity-analysis/notes.md) |
| 2 | Arrays & Strings | [array/notes.md](array/notes.md) |
| 3 | Linked Lists | [linkedlist/notes.md](linkedlist/notes.md) |
| 4 | Stacks, Queues, Deques | [stack-queue-deque/notes.md](stack-queue-deque/notes.md) |
| 5 | Hash Tables | [hashmap/notes.md](hashmap/notes.md) |
| 6 | Trees | [trees/notes.md](trees/notes.md) |
| 7 | Searching & Sorting | [search-sort/notes.md](search-sort/notes.md) |
| 8 | Graphs | [graphs/notes.md](graphs/notes.md) |
| 9 | Dynamic Programming | [dynamic-programming/notes.md](dynamic-programming/notes.md) |
| 10 | Greedy & Divide-and-Conquer | [greedy-divide-conquer/notes.md](greedy-divide-conquer/notes.md) |
| 11 | Backtracking | [backtracking/notes.md](backtracking/notes.md) |
| 12 | Advanced Data Structures | [advanced-data-structures/notes.md](advanced-data-structures/notes.md) |
| 13 | String Algorithms | [string-algorithms/notes.md](string-algorithms/notes.md) |
| 14 | Advanced Graph Algorithms | [advanced-graph-algorithms/notes.md](advanced-graph-algorithms/notes.md) |

Spend **2–4 weeks each** on Trees and Graphs if your goal is mastery, not just interview prep.

---

## How Every `notes.md` Is Structured

Every topic follows the same syllabus template so you always know where to look:

| Section | What it gives you |
|---------|-------------------|
| **Motivation** | What problem this topic solves and why it exists |
| **Fundamentals** | Core concepts, definitions, ASCII diagrams |
| **Why it exists** | Trade-offs vs alternatives |
| **Types / Variants** | Singly vs doubly, BST vs heap, etc. |
| **Core Operations** | Implement-from-scratch operations + complexity |
| **Patterns (numbered)** | The main algorithmic patterns with diagrams and code |
| **Pattern Recognition** | Signal → pattern map ("when you see X, use Y") |
| **Edge Cases** | What breaks your code |
| **Implementation Skills** | Meta skills (draw pointers, maintain invariants) |
| **Problems to Practice** | Curated list grouped by pattern — not random grind |
| **For EVERY [Topic] Problem** | Numbered question framework for any new problem |
| **Complete Checklist** | ASCII tree to tick off coverage |

### Per-topic revision order

For each topic, revise in this order:

1. **Motivation** — What problem does it solve?
2. **Theory** — Core ideas and invariants
3. **Implementation** — Write it from scratch
4. **Complexity** — Time and space analysis
5. **Variants** — Related structures or algorithms
6. **Applications** — Where it's used in real systems
7. **Problems** — Solve a handful of representative problems

---

## How to Study a Topic (Workflow)

### Step 1 — Read for understanding (don't code yet)

- Read sections 1–8 in the topic's `notes.md`
- Draw the ASCII diagrams on paper
- Answer the "Understand:" bullet lists out loud

### Step 2 — Learn the patterns

- Work through each numbered **Pattern** section
- For each pattern, note: *when to use it*, *invariant*, *complexity*
- Study the **Pattern Recognition** section — this is what you train in interviews

### Step 3 — Implement from scratch

- Build the data structure or algorithm without looking at notes
- Compare your version to the notes; fix gaps
- Use the C++ reference implementations in `array/` where available

### Step 4 — Solve representative problems

- Pick problems from the **Problems to Practice** section (grouped by pattern)
- Apply the **For EVERY [Topic] Problem** framework before coding
- After solving, check against the **Complete Checklist**

### Step 5 — Self-test for mastery

Ask yourself:

- Why was this invented?
- What are its limitations?
- What alternatives exist?
- How do real languages/databases implement it?
- Can I explain *why* my solution works, not just *that* it works?

---

## Array Pattern Deep-Dives

Phase 2 (Arrays) has a master syllabus plus focused pattern files. Use **`array/notes.md`** first, then drill into specific patterns:

| Pattern | File |
|---------|------|
| Two Pointers | [array/twoptr.md](array/twoptr.md) |
| Sliding Window | [array/slidingwindow.md](array/slidingwindow.md) |
| Prefix Sum + Hash Map | [array/prefixsum-hashmap.md](array/prefixsum-hashmap.md) |
| Sort + Scan | [array/sortscan.md](array/sortscan.md) |
| Binary Search (in-place) | [array/binarysearch-inplace.md](array/binarysearch-inplace.md) |
| 2D Matrix Traversal | [array/matrix_2dTra.md](array/matrix_2dTra.md) |

Legacy index with external resource links: [array/note.md](array/note.md)

---

## C++ Reference Implementations

Some topics include hands-on C++ code to implement structures from scratch:

| File | What it implements |
|------|--------------------|
| [array/arrayList.cpp](array/arrayList.cpp) | Dynamic array (vector-like) with resize |
| [array/linearScan/code.cpp](array/linearScan/code.cpp) | Linear scan patterns |
| [array/searchSort/search.cpp](array/searchSort/search.cpp) | Search algorithms |
| [array/searchSort/sort.cpp](array/searchSort/sort.cpp) | Sort algorithms |

### Compile and run

```bash
# Dynamic array
g++ -std=c++17 -Wall -o arrayList array/arrayList.cpp && ./arrayList

# Search
g++ -std=c++17 -Wall -o search array/searchSort/search.cpp && ./search

# Sort
g++ -std=c++17 -Wall -o sort array/searchSort/sort.cpp && ./sort
```

Requires a C++17 compiler (`g++` or `clang++`).

---

## Problem-Solving Framework (Use on Every Problem)

Every `notes.md` ends with a **"For EVERY [Topic] Problem"** section. The general flow across all topics:

```text
1. What is the structure / input type?
2. What information do I need to track?
3. What pattern naturally fits?
4. What invariant must stay true?
5. Draw / trace before coding
6. Time complexity?
7. Space complexity?
8. Edge cases?
9. Could another approach work? Why is this one better?
10. Explain WHY it works
```

When stuck, jump to the **Pattern Recognition** section of the current topic — match the problem signal to a pattern.

---

## Repository Structure

```text
practice/
├── map.md                          # Master roadmap — start here
├── README.md                       # This file
│
├── complexity-analysis/notes.md    # Phase 1
├── array/
│   ├── notes.md                    # Phase 2 master syllabus
│   ├── twoptr.md                   # Pattern deep-dives
│   ├── slidingwindow.md
│   ├── prefixsum-hashmap.md
│   ├── sortscan.md
│   ├── binarysearch-inplace.md
│   ├── matrix_2dTra.md
│   ├── arrayList.cpp               # Implementations
│   └── searchSort/
│
├── linkedlist/notes.md             # Phase 3
├── stack-queue-deque/notes.md      # Phase 4
├── hashmap/notes.md                # Phase 5
├── trees/notes.md                  # Phase 6
├── search-sort/notes.md            # Phase 7
├── graphs/notes.md                 # Phase 8
├── dynamic-programming/notes.md    # Phase 9
├── greedy-divide-conquer/notes.md  # Phase 10
├── backtracking/notes.md           # Phase 11
├── advanced-data-structures/notes.md   # Phase 12
├── string-algorithms/notes.md      # Phase 13
└── advanced-graph-algorithms/notes.md  # Phase 14
```

---

## Tips

| Goal | Approach |
|------|----------|
| **Deep understanding** | Follow all 14 phases in order; implement every structure from scratch |
| **Interview prep** | Focus on Patterns + Pattern Recognition + Problems sections per topic |
| **Stuck on a problem** | Check Pattern Recognition → apply "For EVERY Problem" framework |
| **Review before interview** | Use the Complete Checklist at the bottom of each `notes.md` |
| **Pattern composition** | Many hard problems = combine 2–3 patterns you already know (see Palindrome List, Reorder List in linked lists) |

---

## Philosophy

This repo is designed to turn DSA from a collection of tricks into a way of thinking:

- **Don't memorize code** — understand invariants and why each step is necessary
- **Don't grind 500 problems** — solve representative problems per pattern
- **Do draw** — pointers, trees, and graphs become dramatically easier when visualized
- **Do ask "why"** — every structure exists because something else was too slow or too rigid

---

## Contributing

This is a personal study repo. Add your own problem solutions, notes, and implementations in the relevant topic folder.
