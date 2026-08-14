# Matrix / 2D Array Traversal

The core idea is:

> A matrix is still an array, but every element now has two coordinates: **row and column**.

Given:

```text
1  2  3
4  5  6
7  8  9
```

We access values as:

```cpp
matrix[row][col]
```

So:

```text
matrix[0][0] = 1
matrix[1][2] = 6
matrix[2][1] = 8
```

The first thing to master is not spiral traversal. It's **coordinate reasoning**.

---



# 1. Rows, Columns, and Dimensions

Suppose:

```text
rows = m
cols = n
```

Valid coordinates are:

```text
0 <= row < m
0 <= col < n
```

For:

```text
1 2 3 4
5 6 7 8
9 0 1 2
```

you have:

```text
rows = 3
cols = 4
```

This distinction matters because many bugs come from assuming a square matrix.

---



# 2. Row Traversal

The simplest traversal:

```text
→ → →
→ → →
→ → →
```

Code:

```cpp
for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
        process(matrix[r][c]);
    }
}
```

Order:

```text
1 2 3
4 5 6
7 8 9
```

Time:

```text
O(rows * cols)
```

Because every cell is visited once.

---



# 3. Column Traversal

Now reverse the loop order:

```cpp
for (int c = 0; c < cols; c++) {
    for (int r = 0; r < rows; r++) {
        process(matrix[r][c]);
    }
}
```

Traversal:

```text
↓  ↓  ↓

1  2  3
4  5  6
7  8  9
```

Order:

```text
1 4 7 2 5 8 3 6 9
```

Same asymptotic complexity:

```text
O(rows * cols)
```

But potentially different practical performance because of memory layout/cache behavior.

---



# 4. Row-major memory layout

In languages such as C/C++ for ordinary contiguous multidimensional arrays, rows are typically laid out consecutively.

Conceptually:

```text
Matrix:

1 2 3
4 5 6
7 8 9
```

Memory resembles:

```text
1 2 3 4 5 6 7 8 9
```

So row traversal tends to have good locality:

```text
1 → 2 → 3 → 4 → 5 ...
```

Column traversal jumps more:

```text
1 → 4 → 7 → 2 ...
```

This is a good place to connect DSA with actual machine behavior.

Don't overdo CPU-cache theory yet, but understand:

> Two O(mn) algorithms can still have noticeably different real-world performance.

---



# 5. Rectangular vs Square Matrix

Always distinguish:

### Rectangular

```text
2 x 4

1 2 3 4
5 6 7 8
```



### Square

```text
3 x 3

1 2 3
4 5 6
7 8 9
```

Some operations work for both:

- Row traversal
- Column traversal
- Spiral traversal
- Rectangular transpose into another matrix

Some in-place operations generally require square matrices:

- In-place transpose
- In-place 90° rotation

That's important.

---



# 6. Main Diagonal

For a square matrix:

```text
1 2 3
4 5 6
7 8 9
```

Main diagonal:

```text
1
  5
    9
```

Coordinates satisfy:

```text
row == col
```

So:

```cpp
for (int i = 0; i < n; i++)
    process(matrix[i][i]);
```

---



# 7. Anti-Diagonal

Anti-diagonal:

```text
    3
  5
7
```

Coordinates:

```text
(row, n - 1 - row)
```

So:

```cpp
matrix[i][n - 1 - i]
```

This coordinate relationship is worth understanding rather than memorizing blindly.

---



# 8. General diagonal reasoning

For cells:

```text
(r, c)
```

Cells on the same `\` diagonal often share:

```text
r - c
```

Cells on the same `/` diagonal often share:

```text
r + c
```

Example:

```text
Coordinates

(0,0) (0,1) (0,2)
(1,0) (1,1) (1,2)
(2,0) (2,1) (2,2)
```

Main diagonal:

```text
(0,0)
(1,1)
(2,2)
```

all have:

```text
r - c = 0
```

Anti-diagonal:

```text
(0,2)
(1,1)
(2,0)
```

all have:

```text
r + c = 2
```

This becomes useful in matrix problems, chessboard problems, and geometry-like reasoning.

---



# 9. Boundary Traversal

Suppose:

```text
1  2  3  4
5  6  7  8
9 10 11 12
```

Boundary means:

```text
1  2  3  4
5        8
9 10 11 12
```

You can think in four segments:

```text
top row
right column
bottom row
left column
```

But be careful about duplicate corners.

That's the main edge-case challenge.

---



# 10. Boundary conditions

A cell lies on the outer boundary if:

```text
row == 0
OR
row == rows - 1
OR
col == 0
OR
col == cols - 1
```

This simpler definition is useful if you only need to **identify** boundary cells.

If you need them in clockwise order, use directional traversal.

---



# 11. Spiral Traversal

Classic matrix pattern.

Given:

```text
1  2  3  4
5  6  7  8
9 10 11 12
```

Spiral order:

```text
1 2 3 4
      ↓
      8
      ↓
      12
← 11 10 9
↑
5

then inner:
6 7
```

Full:

```text
1,2,3,4,8,12,11,10,9,5,6,7
```

---



# 12. Spiral = shrinking boundaries

Don't think of spiral traversal as a magical special algorithm.

Maintain four boundaries:

```text
top
bottom
left
right
```

Initially:

```text
top    = 0
bottom = rows - 1
left   = 0
right  = cols - 1
```

Then:

```text
Traverse top       left → right
top++

Traverse right     top → bottom
right--

Traverse bottom    right → left
bottom--

Traverse left      bottom → top
left++
```

Repeat.

The important idea is:

> Process one outer layer, then shrink the active rectangle.

---



# 13. Spiral invariant

At every point:

> Everything outside `[top..bottom] x [left..right]` has already been processed.

So you're repeatedly solving a smaller rectangle.

This is the real reasoning behind spiral traversal.

---



# 14. Spiral edge cases

Be careful with:

### One row

```text
1 2 3 4
```



### One column

```text
1
2
3
4
```



### Odd dimensions

```text
3 x 3
```

Eventually there may be one central cell.

### Rectangular matrices

```text
2 x 5
```

That's why after shrinking boundaries you often need checks such as:

```text
top <= bottom
left <= right
```

before traversing the opposite side.

---



# 15. Matrix Transpose

Transpose swaps rows and columns.

Original:

```text
1 2 3
4 5 6
```

Transpose:

```text
1 4
2 5
3 6
```

Mathematically:

```text
A[r][c]
```

moves to:

```text
Aᵀ[c][r]
```

That's the key relationship.

---



# 16. Rectangular transpose

For:

```text
rows x cols
```

the transpose has:

```text
cols x rows
```

So:

```text
2 x 3
```

becomes:

```text
3 x 2
```

Usually you need another matrix.

Example:

```cpp
transpose[c][r] = matrix[r][c];
```

Complexity:

```text
Time  O(rows * cols)
Space O(rows * cols)
```

---



# 17. In-place transpose

For a square matrix:

```text
1 2 3
4 5 6
7 8 9
```

swap:

```text
matrix[r][c]
↔
matrix[c][r]
```

But don't swap every pair twice.

Use only one triangular half:

```cpp
for (int r = 0; r < n; r++) {
    for (int c = r + 1; c < n; c++) {
        swap(matrix[r][c], matrix[c][r]);
    }
}
```

Result:

```text
1 4 7
2 5 8
3 6 9
```

Extra space:

```text
O(1)
```

---



# 18. Matrix Rotation — 90° clockwise

Original:

```text
1 2 3
4 5 6
7 8 9
```

Rotate:

```text
7 4 1
8 5 2
9 6 3
```

A beautiful in-place approach for square matrices:

```text
Transpose
   +
Reverse every row
```

First transpose:

```text
1 4 7
2 5 8
3 6 9
```

Then reverse each row:

```text
7 4 1
8 5 2
9 6 3
```

Done.

This is exactly the kind of pattern composition you want to learn.

---



# 19. Why transpose + reverse works

Original coordinate:

```text
(r,c)
```

After a 90° clockwise rotation:

```text
(c, n - 1 - r)
```

Transpose performs:

```text
(r,c) → (c,r)
```

Then reversing rows effectively performs:

```text
(c,r) → (c,n-1-r)
```

Combine them:

```text
(r,c)
→
(c,r)
→
(c,n-1-r)
```

That's the rotation.

This explanation is better than memorizing two operations.

---



# 20. 90° counter-clockwise

Similarly, you can compose transformations differently.

For example:

```text
Transpose
+
Reverse each column
```

or use an equivalent sequence.

Again, understand the coordinate mapping instead of collecting templates.

---



# 21. 180° rotation

For:

```text
1 2 3
4 5 6
7 8 9
```

180°:

```text
9 8 7
6 5 4
3 2 1
```

Conceptually:

```text
(r,c)
→
(n-1-r, n-1-c)
```

You can implement through swaps or repeated reversals depending on representation.

---



# 22. Neighbor Traversal

This is extremely important even before graphs.

Suppose you're at:

```text
(r,c)
```

Four orthogonal neighbors:

```text
      up
       ↑
left ← X → right
       ↓
      down
```

Coordinates:

```text
up    = (r-1, c)
down  = (r+1, c)
left  = (r, c-1)
right = (r, c+1)
```

Instead of writing four separate blocks, use direction arrays.

---



# 23. Direction arrays

Common representation:

```cpp
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};
```

Then:

```cpp
for (int d = 0; d < 4; d++) {
    int nr = r + dr[d];
    int nc = c + dc[d];
}
```

This is a reusable abstraction.

It will become extremely important once you start:

- Grid BFS
- Grid DFS
- Flood fill
- Connected components

But for now, only learn **neighbor generation and bounds checking**.

---



# 24. Bounds checking

Before accessing:

```text
matrix[nr][nc]
```

ensure:

```text
0 <= nr < rows
0 <= nc < cols
```

Otherwise:

```text
out-of-bounds access
```

This is probably the most common matrix traversal bug.

A useful helper conceptually:

```cpp
bool valid(int r, int c) {
    return 0 <= r && r < rows &&
           0 <= c && c < cols;
}
```

---



# 25. Eight-direction traversal

Sometimes diagonals count as neighbors too.

Then directions include:

```text
NW  N  NE
 W  X   E
SW  S  SE
```

That's 8 neighbors.

You can encode:

```text
(-1,-1) (-1,0) (-1,1)
( 0,-1)          ( 0,1)
( 1,-1) ( 1,0)  ( 1,1)
```

Know the concept.

Don't turn it into graph traversal yet.

---



# 26. Direction vector mindset

This connects nicely to geometry.

A move is just:

```text
current position
+
direction vector
=
neighbor position
```

For example:

```text
(r,c) + (-1,0)
```

means "move up."

That vector-based representation is a very reusable mental model.

---



# 27. Snake / Zigzag Traversal

Sometimes rows alternate direction.

Example:

```text
1 → 2 → 3
          ↓
6 ← 5 ← 4
↓
7 → 8 → 9
```

You can implement based on row parity:

```text
if row even:
    left → right

if row odd:
    right → left
```

This is not as fundamental as row/column/spiral, but it is useful traversal practice.

---



# 28. Layer / Ring Traversal

A square matrix can be viewed as nested rings:

```text
┌───────────┐
│ outer     │
│ ┌───────┐ │
│ │ inner │ │
│ └───────┘ │
└───────────┘
```

This viewpoint helps with:

- Spiral traversal
- Rotation
- Ring shifting
- Boundary algorithms

Spiral traversal is essentially:

> Process ring → shrink → process next ring.

---



# 29. In-place layer rotation

Another way to rotate a square matrix is to rotate four cells at a time.

Coordinates cycle:

```text
top
→ right
→ bottom
→ left
→ top
```

This is more complex than transpose + reverse.

Learn it eventually because it teaches in-place cyclic movement, but for the first pass:

> **Transpose + reverse is enough.**

---



# 30. Matrix search

There is one matrix problem worth understanding conceptually here.

Suppose every row and every column is sorted:

```text
1   4   7
2   5   9
3   8  12
```

Starting at the top-right:

```text
        ↓
1  4  [7]
2  5   9
3  8  12
```

If target is smaller:

```text
move left
```

If target is larger:

```text
move down
```

Why?

Because the sorted structure lets one comparison eliminate an entire row/column region.

This is essentially a 2D version of monotonic elimination.

It's useful, though you can revisit it during Searching.

---



# 31. Complexity of matrix traversal

If a matrix has:

```text
m rows
n columns
```

and you visit each cell once:

```text
O(mn)
```

Do not simplify this to:

```text
O(n²)
```

unless the matrix is explicitly:

```text
n x n
```

This distinction matters.

For a `100 x 1,000,000` matrix, O(mn) describes the real dimensions correctly.

---



# 32. Space complexity

Simple traversal:

```text
O(1)
```

extra space.

Creating transpose/output:

```text
O(mn)
```

In-place transpose for square matrix:

```text
O(1)
```

Spiral output itself may require:

```text
O(mn)
```

if you're returning a new list of all elements, even though the traversal machinery uses only O(1).

Important distinction:

> Auxiliary space vs required output space.

---



# 33. Common bugs

Matrix problems are full of indexing mistakes.

Watch for:

- `rows` vs `cols` mixed up
- Accessing `matrix[c][r]` accidentally
- Assuming square matrices
- Double-processing spiral corners
- Missing center row/column
- Out-of-bounds neighbors
- Performing transpose swaps twice
- Incorrect rotation direction
- Empty matrix handling

Always test weird dimensions.

---



# 34. Test these cases

For any matrix traversal, test:

```text
1 x 1
```

```text
1 x N
```

```text
N x 1
```

```text
2 x 2
```

```text
3 x 3
```

```text
2 x 4
```

```text
4 x 2
```

These catch most assumptions.

---



# 35. Pattern Recognition

When you see:

```text
"visit every cell"
        ↓
Nested row/column loops
```

```text
"main/secondary diagonal"
        ↓
Coordinate relationship
```

```text
"outer boundary"
        ↓
Boundary traversal
```

```text
"spiral"
        ↓
Four shrinking boundaries
```

```text
"transpose"
        ↓
Swap row/column coordinates
```

```text
"rotate 90°"
        ↓
Transpose + Reverse
```

```text
"adjacent cells"
        ↓
Direction arrays + bounds check
```

```text
"grid connected regions"
        ↓
STOP — that's Graph BFS/DFS later
```

That last distinction is important for keeping your learning organized.

---



# 36. Core problems to practice

For this Arrays pass, I'd choose:

- Matrix row/column traversal from scratch
- Diagonal Sum
- Spiral Matrix
- Spiral Matrix II conceptually
- Transpose Matrix
- Rotate Image
- Set Matrix Zeroes
- Search a 2D Matrix / sorted matrix search
- Reshape Matrix conceptually

For neighbor traversal, write a small exercise that visits valid neighbors of each cell.

No need for Number of Islands yet—that belongs with Graphs.

---



# 37. Set Matrix Zeroes — useful in-place reasoning

Problem:

```text
1 1 1
1 0 1
1 1 1
```

Because `(1,1)` is zero, entire row and column become zero:

```text
1 0 1
0 0 0
1 0 1
```

Naively, maintain:

```text
rowsToZero[]
colsToZero[]
```

Extra:

```text
O(m+n)
```

More advanced:

> Reuse first row and first column as marker storage.

This is a good in-place matrix exercise because it combines:

- Traversal
- State marking
- Careful overwrite ordering
- O(1) auxiliary space

Do it after basic traversal.

---



# 38. Matrix traversal invariant examples

For row traversal:

> Every cell before `(r,c)` in row-major order has already been processed.

For spiral:

> Everything outside the active boundaries is already processed.

For transpose:

> Everything in the processed triangular region has already been swapped exactly once.

For rotation:

> Each transformation preserves the intended coordinate mapping.

These invariants help you reason instead of debug blindly.

---



# Complete Matrix / 2D Array Checklist

```text
MATRIX / 2D ARRAYS
│
├── Foundations
│   ├── Rows / Columns
│   ├── matrix[r][c]
│   ├── Bounds
│   ├── Rectangular vs Square
│   └── Row-major locality basics
│
├── Basic Traversal
│   ├── Row-wise
│   ├── Column-wise
│   ├── Zigzag
│   └── Full O(mn) traversal
│
├── Coordinate Patterns
│   ├── Main diagonal
│   ├── Anti-diagonal
│   ├── r-c diagonal grouping
│   └── r+c diagonal grouping
│
├── Boundary Patterns
│   ├── Outer boundary
│   ├── Layers / Rings
│   └── Spiral traversal
│
├── Transformations
│   ├── Transpose
│   ├── In-place square transpose
│   ├── 90° clockwise rotation
│   ├── 90° counter-clockwise concept
│   └── 180° rotation concept
│
├── Neighbor Traversal
│   ├── 4 directions
│   ├── 8 directions
│   ├── Direction arrays
│   └── Bounds checking
│
├── In-place Matrix Work
│   ├── Swap cells
│   ├── Use rows/columns as markers
│   └── Avoid destroying needed state
│
└── For Every Problem
    ├── What are rows and cols?
    ├── Is it square or rectangular?
    ├── What does (r,c) represent?
    ├── What order should I traverse?
    ├── What are the valid boundaries?
    ├── What coordinate relationship matters?
    ├── What is the invariant?
    ├── O(mn) or something smaller?
    ├── Auxiliary vs output space?
    └── Test 1x1, 1xN, Nx1, rectangular cases
```

The sentence to remember is:

> **2D array problems become much easier when you stop seeing a matrix as a picture and start seeing it as coordinates, boundaries, direction vectors, and transformations between** `(row, col)` **positions.**

And yes: stop at **neighbor generation** for now. Once you start asking “which cells are reachable/connected?” or “visit an entire region,” you've crossed into **Graphs → grid BFS/DFS**, which you'll study later.