# Graphs

## 1. Graph Fundamentals

First understand **why graphs exist**.

Many real-world relationships are **not linear** and **not hierarchical**.

```text
ARRAY

[10][20][30][40]
```

Linear order. Index `i` is next to index `i+1`.

```text
TREE

        10
       /  \
     20    30
    /  \
  40    50
```

Hierarchical. Each node has one parent (except root). No cycles.

```text
GRAPH

    10 ─── 20
     \     /
      \   /
       30
       / \
     40   50
```

General relationships. Nodes can connect in arbitrary ways. Cycles allowed. Multiple paths between nodes allowed.

A graph is:

```text
G = (V, E)

V = set of vertices (nodes)
E = set of edges (connections)
```

Conceptually:

```text
Vertex / Node
┌─────────────┐
│ id / value  │
└─────────────┘

Edge
┌─────────────────────┐
│ connects two nodes  │
│ may have weight     │
│ may be directed     │
└─────────────────────┘
```

Understand:

- What is a vertex?
- What is an edge?
- What is a path?
- What is a cycle?
- What is connectivity?
- What is a connected component?
- What is a subgraph?
- What is a directed vs undirected edge?
- What is a weighted vs unweighted graph?
- Why do graphs model networks, maps, dependencies, and social structures?

---

# 2. Why Graphs Exist

Compare problems that arrays and trees cannot model cleanly.

**Social network:**

```text
Alice ─── Bob
  │         │
  │         │
 Carol ─── Dave
```

Bob knows Carol. Alice knows Bob. There is no single "parent" or "root" that explains all relationships.

**City road map:**

```text
A ──5── B
│       │
3       2
│       │
C ──1── D
```

Multiple routes from A to D. Edge weights represent distance or time.

**Task dependencies:**

```text
Compile → Link → Run
   ↑
 Build
```

Some tasks must happen before others. This is not a tree if multiple tasks depend on the same prerequisite.

**State machine:**

```text
State1 → State2 → State3
   ↑         │
   └─────────┘
```

Transitions can form cycles.

Arrays give:

```text
sequential access
contiguous memory
simple indexing
```

Trees give:

```text
hierarchical structure
parent-child relationships
no cycles (usually)
```

Graphs give:

```text
arbitrary relationships
cycles
multiple paths
direction
weights
```

So don't learn:

> Graphs are just trees with cycles.

Learn:

> When the relationship between entities is a network, dependency, flow, or map — you need a graph.

---

# 3. Core Graph Terminology

You should understand all major terms before touching algorithms.

## Vertices and Edges

```text
Vertices (V):  {0, 1, 2, 3}
Edges (E):     {(0,1), (1,2), (2,3), (3,0)}
```

An edge connects two vertices.

```text
0 ─── 1
```

---

## Directed vs Undirected

**Undirected:** relationship is mutual.

```text
A ─── B

A connected to B
B connected to A
```

**Directed:** relationship has direction.

```text
A ──→ B

A can go to B
B cannot necessarily go to A
```

Directed graph (digraph):

```text
0 → 1 → 2
↑       │
└───────┘
```

---

## Weighted vs Unweighted

**Unweighted:** edges are just connections.

```text
A ─── B ─── C
```

**Weighted:** edges carry cost, distance, time, capacity.

```text
A ──5── B ──3── C
```

Weights matter for:

- Shortest path
- Minimum spanning tree
- Network flow

---

## Degree

In an undirected graph, **degree** of a vertex = number of edges touching it.

```text
    1
    │
2 ─ 0 ─ 3
    │
    4

degree(0) = 4
```

In a directed graph:

```text
in-degree  = edges coming IN
out-degree = edges going OUT
```

```text
A → B → C
↑       │
└───────┘

in-degree(C) = 1
out-degree(C) = 1
in-degree(A) = 1
out-degree(A) = 1
```

---

## Path, Cycle, Connected

**Path:** sequence of vertices connected by edges.

```text
0 → 1 → 2 → 3
```

**Simple path:** no repeated vertices.

**Cycle:** path that starts and ends at same vertex.

```text
0 → 1 → 2 → 0
```

**Connected (undirected):** every vertex reachable from every other vertex.

```text
CONNECTED

0 ─ 1 ─ 2

NOT CONNECTED

0 ─ 1    2 ─ 3
```

**Strongly connected (directed):** every vertex reachable from every other vertex **following edge directions**.

---

## Adjacent, Neighbors

Vertices connected by an edge are **adjacent**.

Neighbors of vertex `v` = all vertices directly connected to `v`.

```text
Graph:

  1 ─ 2
  │
  3

Neighbors of 1: {2, 3}
```

---

## Subgraph

A subgraph uses a subset of vertices and edges from the original graph.

```text
Original:     0─1─2─3

Subgraph:     0─1    3
```

---

## Sparse vs Dense

**Sparse:** few edges relative to possible edges.

```text
V = 1000, E = 2000
```

**Dense:** many edges.

```text
V = 1000, E = 500000
```

For `V` vertices, maximum edges in undirected graph (no self-loops):

```text
E_max = V * (V - 1) / 2
```

Representation choice depends heavily on sparsity.

---

# 4. Graph Representations

This is foundational. **Wrong representation makes problems painful.**

## Adjacency Matrix

`adj[i][j] = 1` (or weight) if edge from `i` to `j` exists.

```text
Graph:

  0 ─ 1
  │
  2

Matrix (undirected):

     0  1  2
  0 [0  1  1]
  1 [1  0  0]
  2 [1  0  0]
```

For weighted graph, store weight instead of 1.

```cpp
vector<vector<int>> adj(n, vector<int>(n, 0));

// undirected edge u-v
adj[u][v] = 1;
adj[v][u] = 1;

// directed edge u -> v
adj[u][v] = 1;
```

**Space:** O(V²)

**Check edge (u, v):** O(1)

**List neighbors of u:** O(V)

**Add edge:** O(1)

Best when:

- Graph is dense
- Need frequent edge-existence queries
- V is small (≤ a few hundred)

---

## Adjacency List

Each vertex stores list of neighbors (or `(neighbor, weight)` pairs).

```text
Graph:

  0 ─ 1
  │
  2

Adjacency list:

0 → [1, 2]
1 → [0]
2 → [0]
```

```cpp
vector<vector<int>> adj(n);

// undirected
adj[u].push_back(v);
adj[v].push_back(u);

// directed
adj[u].push_back(v);

// weighted
vector<vector<pair<int,int>>> adj(n);
adj[u].push_back({v, weight});
```

**Space:** O(V + E)

**Check edge (u, v):** O(degree(u)) — can use hash set per node for O(1)

**List neighbors of u:** O(degree(u))

**Add edge:** O(1)

Best when:

- Graph is sparse
- Need to traverse neighbors often (BFS, DFS, Dijkstra)
- V is large, E is moderate

This is the **default representation for most interview and algorithm problems**.

---

## Edge List

Simply store all edges.

```cpp
vector<pair<int,int>> edges;       // unweighted
vector<tuple<int,int,int>> edges;  // weighted (u, v, w)
```

**Space:** O(E)

Useful for:

- Kruskal's MST (sort edges)
- Bellman-Ford
- Building adjacency list from input

```text
edges = [(0,1), (0,2), (1,2)]
```

---

## Comparison

```text
REPRESENTATION COMPARISON

                    Matrix          List            Edge List
Space               O(V²)           O(V+E)          O(E)
Add edge            O(1)            O(1)            O(1)
Remove edge         O(1)            O(deg)          O(E)
Check edge          O(1)            O(deg)          O(E)
Traverse neighbors  O(V)            O(deg)          O(E)
Best for            dense/small V   sparse/large V  Kruskal/Bellman-Ford
```

---

## Building Graph from Input

Common patterns:

**Edge list input:**

```text
n = 4, edges = [[0,1],[1,2],[2,3],[3,0]]
```

```cpp
int n = 4;
vector<vector<int>> adj(n);
for (auto& e : edges) {
    int u = e[0], v = e[1];
    adj[u].push_back(v);
    adj[v].push_back(u);  // if undirected
}
```

**Grid as graph:**

```text
Grid cell (i,j) ↔ neighbors (i±1,j), (i,j±1)
```

Each cell is a vertex. Edges to adjacent cells.

```cpp
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};
```

**Implicit graph:**

Vertices are states. Edges are transitions.

```text
"000" → "100" → "110" → ...
```

No explicit adjacency list stored — generate neighbors on the fly.

---

# 5. Graph Traversal Foundations

Almost every graph algorithm is built on **BFS** or **DFS**.

Both need:

```text
1. A way to visit vertices
2. A way to track visited vertices
3. A way to explore neighbors
```

```cpp
vector<bool> visited(n, false);
```

Or for large graphs / special cases:

```cpp
unordered_set<int> visited;
```

**Golden rule:** mark visited **when you enqueue/push**, not when you dequeue/pop — depending on pattern. For standard BFS/DFS on graphs, mark when you **first discover** the node to avoid revisiting.

---

# 6. Breadth-First Search (BFS)

BFS explores **layer by layer** from a start vertex.

Uses a **queue**.

```text
Start at 0

Layer 0:  0
Layer 1:  1, 2
Layer 2:  3, 4

Graph:

    1
    │
0 ─ 2 ─ 3
    │
    4

BFS order: 0 → 2 → 1 → 4 → 3
```

## BFS Template

```cpp
vector<int> bfs(int start, vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    vector<int> order;
    queue<int> q;

    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);

        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return order;
}
```

## BFS Visualization

```text
Queue evolution:

Start: q=[0], visited={0}

Pop 0, push neighbors 1,2:
q=[1,2], visited={0,1,2}

Pop 1, push neighbor 3:
q=[2,3], visited={0,1,2,3}

Pop 2, push neighbor 4:
q=[3,4], visited={0,1,2,3,4}

Pop 3: no new neighbors
Pop 4: no new neighbors
```

## BFS Properties

- Finds **shortest path in unweighted graphs** (minimum number of edges)
- Explores all vertices in same connected component from start
- Time: O(V + E)
- Space: O(V) for queue and visited

## BFS on Grid

```text
Grid:

S . #
. . .
# . G

S = start, G = goal, # = wall
```

```cpp
int bfsGrid(vector<vector<char>>& grid, pair<int,int> start) {
    int rows = grid.size(), cols = grid[0].size();
    queue<pair<int,int>> q;
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    q.push(start);
    visited[start.first][start.second] = true;
    int dist = 0;

    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            auto [r, c] = q.front(); q.pop();
            if (grid[r][c] == 'G') return dist;

            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols
                    && !visited[nr][nc] && grid[nr][nc] != '#') {
                    visited[nr][nc] = true;
                    q.push({nr, nc});
                }
            }
        }
        dist++;
    }
    return -1;
}
```

Process **level by level** when you need distance in unweighted graph.

---

# 7. Depth-First Search (DFS)

DFS explores **as deep as possible** before backtracking.

Uses **recursion** or an **explicit stack**.

```text
Graph:

    1
    │
0 ─ 2 ─ 3
    │
    4

DFS from 0 (go left first): 0 → 2 → 1 → 4 → 3
```

## DFS Recursive Template

```cpp
void dfs(int u, vector<vector<int>>& adj, vector<bool>& visited) {
    visited[u] = true;
    // process u

    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs(v, adj, visited);
        }
    }
}
```

## DFS Stack Template

```cpp
void dfsStack(int start, vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    stack<int> st;
    st.push(start);

    while (!st.empty()) {
        int u = st.top();
        st.pop();
        if (visited[u]) continue;
        visited[u] = true;

        for (int v : adj[u]) {
            if (!visited[v]) st.push(v);
        }
    }
}
```

Note: stack-based DFS order may differ from recursive DFS due to push order.

## DFS Visualization (Recursive)

```text
At 0: visit 0, go to neighbor 2
At 2: visit 2, go to neighbor 1
At 1: visit 1, no unvisited neighbors, return
Back at 2: go to neighbor 4
At 4: visit 4, return
Back at 2: go to neighbor 3
At 3: visit 3, return
```

## DFS Properties

- Uses recursion depth up to O(V) — watch stack overflow on large graphs
- Natural for: cycle detection, topological sort, connected components, path finding with backtracking
- Time: O(V + E)
- Space: O(V) for visited + O(V) recursion stack worst case

---

# 8. BFS vs DFS — When to Use Each

```text
QUESTION                          BFS                    DFS
Shortest path (unweighted)        YES                    NO (not shortest)
Level-by-level processing         YES                    Awkward
Explore all paths / backtrack     NO                     YES
Cycle detection                   Both work              Both work
Topological sort                  Kahn's (BFS)           DFS post-order
Connected components              Both                   Both
Maze / grid shortest steps        YES                    NO
Detect if path exists             Both                   Both
Tree/graph cloning                Both                   DFS is natural
```

**BFS intuition:** "What is closest / minimum steps?"

**DFS intuition:** "Explore fully / detect structure / backtrack"

---

# 9. Cycle Detection

## Undirected Graph — DFS

If you see a visited neighbor that is **not the parent**, there is a cycle.

```text
0 ─ 1
│   │
3 ─ 2

DFS from 0:
At 1, neighbor 2 is visited and 2 ≠ parent(1) → cycle
```

```cpp
bool hasCycleUndirected(int u, int parent,
                        vector<vector<int>>& adj,
                        vector<bool>& visited) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            if (hasCycleUndirected(v, u, adj, visited)) return true;
        } else if (v != parent) {
            return true;
        }
    }
    return false;
}
```

## Undirected Graph — Union-Find

Add edges one by one. If `u` and `v` are already in same set → cycle.

```text
Add 0-1: OK
Add 1-2: OK
Add 2-0: 0 and 2 already connected → CYCLE
```

---

## Directed Graph — DFS (Three Colors)

Track:

```text
WHITE = unvisited
GRAY  = in current DFS path (active)
BLACK = fully processed
```

Cycle exists if you reach a **GRAY** node (back edge).

```text
0 → 1 → 2
↑       │
└───────┘

DFS: 0 gray → 1 gray → 2 gray → back to 0 (gray) → CYCLE
```

```cpp
bool hasCycleDirected(int u, vector<vector<int>>& adj,
                      vector<int>& color) {
  // color: 0=white, 1=gray, 2=black
    color[u] = 1;
    for (int v : adj[u]) {
        if (color[v] == 1) return true;
        if (color[v] == 0 && hasCycleDirected(v, adj, color))
            return true;
    }
    color[u] = 2;
    return false;
}
```

## Directed Graph — Kahn's Algorithm

Run topological sort. If you cannot process all vertices → cycle exists.

---

# 10. Connected Components

## Undirected Graph — DFS/BFS

Run DFS/BFS from every unvisited vertex. Each run discovers one component.

```text
Component 1:  0 ─ 1 ─ 2

Component 2:  3 ─ 4

Component 3:  5
```

```cpp
int countComponents(vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs(i, adj, visited);
            count++;
        }
    }
    return count;
}
```

## Union-Find for Components

Initialize each node as its own set. Union on each edge. Number of distinct roots = number of components.

---

## Strongly Connected Components (SCC) — Preview

For directed graphs. Every vertex in an SCC can reach every other vertex in the same SCC.

```text
0 → 1 → 2
↑       │
└───────┘

{0,1,2} is one SCC
```

Full SCC algorithms (Kosaraju, Tarjan) belong to **Advanced Graph Algorithms** (Phase 14). Know the concept here; master algorithms later.

---

# 11. Topological Sort

Order vertices in a **directed acyclic graph (DAG)** such that for every edge `u → v`, `u` comes before `v`.

```text
DAG:

Build → Compile → Link → Run

Valid topological orders:
Build, Compile, Link, Run
(same order — unique here)
```

```text
NOT a DAG (has cycle):

A → B → C → A

No valid topological order
```

Applications:

- Build systems
- Course prerequisites
- Task scheduling
- Dependency resolution

---

## Method 1 — Kahn's Algorithm (BFS)

Repeatedly remove vertices with **in-degree 0**.

```text
Graph:

5 → 0 → 1
    ↓   ↓
    2 → 3 → 4

In-degrees:
5:0  0:1  1:2  2:1  3:2  4:1

Step 1: only 5 has in-degree 0 → output 5
Step 2: 0 has in-degree 0 → output 0
Step 3: 2 has in-degree 0 → output 2
Step 4: 1,3 have in-degree 0 → output 1,3
Step 5: 4 → output 4

Order: 5, 0, 2, 1, 3, 4
```

```cpp
vector<int> topologicalSortKahn(vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> indegree(n, 0);
    for (int u = 0; u < n; u++)
        for (int v : adj[u]) indegree[v]++;

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (indegree[i] == 0) q.push(i);

    vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            indegree[v]--;
            if (indegree[v] == 0) q.push(v);
        }
    }
    return order.size() == n ? order : {}; // empty if cycle
}
```

If `order.size() < n` → graph has cycle.

---

## Method 2 — DFS Post-Order

DFS, push vertex to result **after** processing all descendants. Reverse at end (or insert at front).

```text
DFS from 5:
  go deep, finish descendants first
  post-order push: when all children done, add node

Reverse → topological order
```

```cpp
void dfsTopo(int u, vector<vector<int>>& adj,
             vector<bool>& visited, vector<int>& order) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) dfsTopo(v, adj, visited, order);
    }
    order.push_back(u);  // post-order
}

vector<int> topologicalSortDFS(vector<vector<int>>& adj) {
    int n = adj.size();
    vector<bool> visited(n, false);
    vector<int> order;
    for (int i = 0; i < n; i++)
        if (!visited[i]) dfsTopo(i, adj, visited, order);
    reverse(order.begin(), order.end());
    return order;
}
```

## Kahn vs DFS Topo

```text
Kahn (BFS)     intuitive, detects cycle by count
DFS            fewer data structures, one DFS pass
Both           O(V + E)
```

---

# 12. Union-Find (Disjoint Set Union — DSU)

Union-Find tracks **disjoint sets** with near-constant-time union and find.

Operations:

```text
makeSet(x)   create set {x}
find(x)      return representative of x's set
union(x,y)   merge sets containing x and y
```

## Structure

```cpp
class UnionFind {
    vector<int> parent, rank; // or size

public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]); // path compression
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false; // already connected

        // union by rank
        if (rank[px] < rank[py]) swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
        return true;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};
```

## Visualization

```text
Initial: each node is its own parent

0  1  2  3  4

parent: [0,1,2,3,4]

union(0,1):

    0
   / \
  0   1    2  3  4

parent: [0,0,2,3,4]

union(2,3):

  0       2
 / \     / \
0   1   2   3    4

union(1,2):

      0
    / | \
   0  1  2
          \
           3    4
```

## Path Compression

Without:

```text
find(3) walks 3 → 2 → 0
```

After path compression during find:

```text
3 → 0
2 → 0
```

Future finds are faster.

## Union by Rank / Size

Attach smaller tree under larger tree. Keeps tree shallow.

## Complexity

```text
With path compression + union by rank:

find / unite: amortized O(α(n)) ≈ O(1) practically
Space: O(n)
```

α = inverse Ackermann function — grows extremely slowly.

## Union-Find Use Cases

```text
Dynamic connectivity       "are x and y connected?"
Cycle detection (undirected)  adding edge to connected nodes
Kruskal's MST              group edges by component
Connected components count number of distinct roots
Percolation / grid connectivity
Network connectivity queries
```

---

# 13. Minimum Spanning Tree (MST)

Given a **connected, undirected, weighted** graph, find a spanning tree with **minimum total edge weight**.

```text
Spanning tree: connects all vertices, no cycles, exactly V-1 edges

Graph:

    1
   / \
  6   2
 /     \
0       2

MST weight = 6 + 1 + 2 = 7 (edges 0-1, 1-2, 1-3... depends on graph)
```

MST is unique if all edge weights are distinct. Otherwise multiple MSTs may exist with same weight.

**Not the same as shortest path tree.** MST minimizes total edge weight of tree spanning all nodes.

---

## Kruskal's Algorithm

1. Sort all edges by weight
2. Add edges greedily if they don't form a cycle (Union-Find)

```text
Edges sorted: (1,2,1), (1,3,2), (0,1,6), (2,3,3), (0,2,5)

Pick (1,2,1)  OK
Pick (1,3,2)  OK
Pick (0,1,6)  OK  → 3 edges, V-1 = 3 for V=4, done

MST edges: (1,2), (1,3), (0,1)
```

```cpp
int kruskal(int n, vector<tuple<int,int,int>>& edges) {
    sort(edges.begin(), edges.end(),
         [](auto& a, auto& b) { return get<2>(a) < get<2>(b); });

    UnionFind uf(n);
    int mstWeight = 0, edgeCount = 0;

    for (auto& [u, v, w] : edges) {
        if (uf.unite(u, v)) {
            mstWeight += w;
            edgeCount++;
        }
    }
    return edgeCount == n - 1 ? mstWeight : -1; // -1 if disconnected
}
```

**Time:** O(E log E) dominated by sorting

**Best when:** sparse graph, edges given as list

---

## Prim's Algorithm

Grow MST from a start vertex. Always add **minimum weight edge** connecting tree to a non-tree vertex.

Uses **priority queue** (min-heap).

```text
Start at 0

Tree: {0}
Edges to outside: (0,1,6), (0,2,5)
Pick min: (0,2,5)

Tree: {0,2}
Edges: (0,1,6), (2,1,1), (2,3,3)
Pick min: (2,1,1)

Tree: {0,2,1}
... continue until V-1 edges
```

```cpp
int prim(int n, vector<vector<pair<int,int>>>& adj) {
    vector<bool> inMST(n, false);
    priority_queue<pair<int,int>, vector<pair<int,int>>,
                   greater<pair<int,int>>> pq;
    pq.push({0, 0}); // {weight, vertex}
    int mstWeight = 0, count = 0;

    while (!pq.empty() && count < n) {
        auto [w, u] = pq.top(); pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;
        mstWeight += w;
        count++;

        for (auto [v, weight] : adj[u]) {
            if (!inMST[v]) pq.push({weight, v});
        }
    }
    return mstWeight;
}
```

**Time:** O(E log V) with binary heap

**Best when:** dense graph, adjacency list already built

## Kruskal vs Prim

```text
Kruskal    sort edges + Union-Find    great for sparse, edge list input
Prim       greedy + heap              great for dense, adjacency list
Both       produce MST                O(E log E) or O(E log V)
```

---

# 14. Shortest Paths

Find minimum cost path between vertices.

**Path cost** = sum of edge weights along path.

Critical distinction:

```text
Unweighted shortest path  → BFS
Weighted, non-negative    → Dijkstra
Weighted, negative edges  → Bellman-Ford
All pairs shortest paths  → Floyd-Warshall
```

---

## Dijkstra's Algorithm

**Single-source shortest paths** in graph with **non-negative** edge weights.

Greedy: always process the unvisited vertex with smallest known distance.

```text
Graph:

      1
     / \
    4   2
   /     \
  S       D
   \     /
    3   1
     \ /
      C

Shortest S to D: S → C → D = 3 + 1 = 4
(not S → A → D = 4 + 2 = 6)
```

```cpp
vector<int> dijkstra(int start, vector<vector<pair<int,int>>>& adj) {
    int n = adj.size();
    vector<int> dist(n, INT_MAX);
    priority_queue<pair<int,int>, vector<pair<int,int>>,
                   greater<pair<int,int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue; // stale entry

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

## Dijkstra Visualization

```text
dist: all INF except start=0

pq: {(0,S)}

Process S: update A=4, C=3
pq: {(3,C), (4,A)}

Process C (smaller): update D=4
pq: {(4,A), (4,D)}

Process A: D already 4, no improvement via A
Process D: done
```

**Time:** O((V + E) log V) with binary heap

**Does NOT work with negative edges** — greedy assumption breaks.

---

## Bellman-Ford Algorithm

Single-source shortest paths. Handles **negative edge weights**. Detects negative cycles reachable from source.

Relax all edges, repeat V-1 times.

```text
Relaxation:

if dist[u] + w(u,v) < dist[v]:
    dist[v] = dist[u] + w(u,v)
```

```cpp
bool bellmanFord(int n, int start,
                 vector<tuple<int,int,int>>& edges,
                 vector<int>& dist) {
    dist.assign(n, INT_MAX);
    dist[start] = 0;

    for (int i = 0; i < n - 1; i++) {
        for (auto& [u, v, w] : edges) {
            if (dist[u] != INT_MAX && dist[u] + w < dist[v])
                dist[v] = dist[u] + w;
        }
    }

    // check negative cycle
    for (auto& [u, v, w] : edges) {
        if (dist[u] != INT_MAX && dist[u] + w < dist[v])
            return false; // negative cycle
    }
    return true;
}
```

**Time:** O(V × E)

**Use when:** negative weights, need negative cycle detection, V is small

---

## Floyd-Warshall Algorithm

**All-pairs shortest paths.**

DP on number of intermediate vertices allowed.

```text
dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
```

```cpp
void floydWarshall(vector<vector<int>>& dist) {
    int n = dist.size();
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX
                    && dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
}
```

Initialize `dist[i][j]` from adjacency matrix. `dist[i][i] = 0`. No edge = INF.

**Time:** O(V³)

**Space:** O(V²)

**Use when:** need all pairs, V ≤ ~400-500, dense graph

---

## Shortest Path Algorithm Selection

```text
PROBLEM                              ALGORITHM           TIME
Unweighted, single source            BFS                 O(V+E)
Weighted, non-negative, single       Dijkstra            O((V+E)logV)
Weighted, negative edges             Bellman-Ford        O(VE)
All pairs                            Floyd-Warshall      O(V³)
Negative cycle detection             Bellman-Ford        O(VE)
K shortest paths / special           Variants            context-specific
```

```text
DECISION TREE

Negative edges?
  YES → Bellman-Ford (or check if negatives matter)
  NO  →
    All pairs?
      YES → Floyd-Warshall (small V) or V × Dijkstra
      NO  →
        Unweighted?
          YES → BFS
          NO  → Dijkstra
```

---

# 15. Complexity Summary

```text
OPERATION / ALGORITHM          TIME              SPACE
─────────────────────────────────────────────────────────
Build adjacency list           O(V + E)          O(V + E)
Build adjacency matrix         O(V²)             O(V²)
BFS                            O(V + E)          O(V)
DFS                            O(V + E)          O(V)
Connected components           O(V + E)          O(V)
Cycle detect (undirected)      O(V + E)          O(V)
Cycle detect (directed)        O(V + E)          O(V)
Topological sort               O(V + E)          O(V)
Union-Find (m operations)      O(m × α(n))       O(n)
Kruskal MST                    O(E log E)        O(V)
Prim MST                       O(E log V)        O(V)
Dijkstra                       O((V+E) log V)    O(V)
Bellman-Ford                   O(V × E)          O(V)
Floyd-Warshall                 O(V³)             O(V²)
```

---

# 16. The Most Important Graph Patterns

These are the patterns that naturally arise from graph problems.

## Pattern 1 — BFS Level Expansion (Shortest Path in Unweighted Graph)

The foundation for grid and graph shortest-distance problems.

```text
Layer 0:  S
Layer 1:  neighbors of S
Layer 2:  neighbors of layer 1 (unvisited)
...
```

```text
Grid BFS:

S . . #
. # . .
. . . G

Dist from S:
0  1  2  -
1  -  3  4
2  3  4  5

G at distance 5
```

```cpp
// process level by level for distance
while (!q.empty()) {
    int size = q.size();
    for (int i = 0; i < size; i++) {
        // pop, expand
    }
    dist++;
}
```

Used for:

- Shortest path in unweighted graph
- Minimum steps in grid/maze
- Word ladder
- Rotting oranges (multi-source BFS)

---

## Pattern 2 — DFS Exploration / Flood Fill

Explore all reachable nodes from a start point.

```text
Start DFS at (0,0):

# . .      → visit connected '.' cells
. . .      → mark visited
. # .
```

```cpp
void dfs(int r, int c, vector<vector<char>>& grid) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (grid[r][c] != '.') return;
    grid[r][c] = '#'; // mark visited
    dfs(r+1,c,grid); dfs(r-1,c,grid);
    dfs(r,c+1,grid); dfs(r,c-1,grid);
}
```

Used for:

- Count islands
- Flood fill
- Path existence
- Connected component labeling

---

## Pattern 3 — Multi-Source BFS

Start BFS from **multiple sources simultaneously**.

```text
Rotting oranges:

2 = rotten, 1 = fresh, 0 = empty

Initial queue: all rotten oranges (sources)

BFS spreads rot layer by layer
Answer: time until all fresh are rotten (or -1)
```

```cpp
queue<pair<int,int>> q;
for each cell:
    if rotten: q.push(cell), visited = true

// standard BFS from all sources at once
```

Used for:

- Rotting oranges
- 01 Matrix (distance to nearest 0)
- Walls and gates
- Fire spread simulation

---

## Pattern 4 — Topological Sort (Dependency Ordering)

When problem mentions prerequisites, dependencies, or "before/after" constraints.

```text
Courses: 0,1,2,3,4
Prereqs: 1→0, 2→0, 3→1, 3→2, 4→3

Valid order: 4,3,2,1,0 or similar satisfying all edges
```

Signal words:

```text
"prerequisite"
"dependency"
"can you finish all tasks"
"ordering of build steps"
```

Kahn's or DFS post-order.

---

## Pattern 5 — Union-Find Connectivity

Dynamic connectivity and grouping.

```text
Operations:
connect(0,1)
connect(2,3)
connected(0,2)? → false
connect(1,2)
connected(0,2)? → true
```

```text
Accounts merge:
"John@a.com" and "John@b.com" share name → same person
Union accounts that share any email
```

Used for:

- Redundant connection (cycle in undirected)
- Number of connected components (dynamic)
- Kruskal MST
- Percolation
- Friend circles / account merge

---

## Pattern 6 — Dijkstra Shortest Path (Weighted, Non-Negative)

When edges have weights and you need shortest path.

```text
Network delay time:
find max shortest path from source to all nodes
→ Dijkstra from source, return max dist
```

```text
Key invariant:
when a node is extracted from pq with dist d,
d is its final shortest distance (non-negative weights)
```

Used for:

- Network delay time
- Path with minimum effort
- Cheapest flights with k stops (modified)
- Reachable nodes within distance threshold

---

## Pattern 7 — Bellman-Ford / Shortest Path with Negatives

When graph has negative edge weights or need negative cycle detection.

```text
Graph with -2 edge:

A ──1── B
 \      /
  4    -2
   \  /
    C

Shortest A to B might use negative edge
```

Also used for:

- Currency arbitrage (negative cycle = profit loop)
- Distance with constraints (modified relaxations)

---

## Pattern 8 — Floyd-Warshall All-Pairs

When you need shortest path between **every pair** of vertices.

```text
4 nodes, dense graph:
run Floyd-Warshall once → dist[i][j] for all i,j
```

```text
Find the city with smallest number of neighbors
within distance threshold:
→ Floyd-Warshall, then count per city
```

Use when V is small enough for O(V³).

---

## Pattern 9 — Cycle Detection

Detect whether graph has a cycle.

```text
Undirected: Union-Find or DFS parent check
Directed:   three-color DFS or Kahn's topo
```

```text
Course schedule:
can finish? ↔ no cycle in prereq graph
```

---

## Pattern 10 — Connected Components

Count or label groups of connected vertices.

```text
Graph:
1─2  3─4  5

3 components
```

Variants:

- Undirected: DFS/BFS or Union-Find
- Directed (SCC): advanced — Kosaraju/Tarjan later

Used for:

- Number of provinces
- Largest component size
- Graph valid tree? (connected + E = V-1)

---

## Pattern 11 — Bipartite Check (Graph Coloring)

Can vertices be colored with 2 colors so no adjacent vertices share color?

```text
Bipartite:

0 ─ 1
│   │
3 ─ 2

Color: 0=Red, 1=Blue, 2=Red, 3=Blue → OK
```

```text
Not bipartite (odd cycle):

0 ─ 1
│ ╲ │
3 ─ 2
```

BFS/DFS: assign colors level by level. If neighbor has same color → not bipartite.

Equivalent to: no odd-length cycle.

Used for:

- Is graph bipartite?
- Possible to partition into two groups with constraints

---

## Pattern 12 — Backtracking on Graph (All Paths / Hamiltonian-style)

DFS with path tracking, undo (backtrack) when dead end.

```text
All paths from 0 to 3:

0 → 1 → 3
0 → 2 → 3
```

```cpp
void dfs(int u, int target, vector<int>& path,
         vector<bool>& visited, vector<vector<int>>& adj) {
    if (u == target) { record path; return; }
    for (int v : adj[u]) {
        if (!visited[v]) {
            visited[v] = true;
            path.push_back(v);
            dfs(v, target, path, visited, adj);
            path.pop_back();
            visited[v] = false; // backtrack
        }
    }
}
```

Used for:

- All paths source to target
- Permutation-style graph problems
- Constraint satisfaction on graphs

---

## Pattern 13 — MST Pattern (Minimum Connection Cost)

Connect all nodes with minimum total edge weight.

```text
Min cost to connect all points:
complete graph of points with Manhattan/Euclidean edge weights
→ Prim or Kruskal
```

Signal:

```text
"connect all"
"minimum cost to make connected"
"spanning"
```

---

## Pattern 14 — Grid as Implicit Graph

Treat each cell as vertex. Edges to adjacent cells.

```text
Word search:

grid of letters, find word by adjacent cells

Each cell (r,c) → neighbors (r±1,c), (r,c±1)
DFS with visited / path tracking
```

Many "matrix" problems are graph problems in disguise.

---

## Pattern 15 — In-Degree / Out-Degree Analysis

Count edges coming in or going out.

```text
Eventual safe states:
nodes that eventually reach a terminal cycle-free path

Reverse graph + nodes with out-degree 0 (terminal)
peel layers like Kahn's from the end
```

```text
Minimum height trees:
peel leaves layer by layer (remove nodes with degree 1)
→ topological peel on tree
```

---

# 17. Pattern Composition

Advanced problems combine patterns.

```text
Course Schedule II
    Cycle detection + Topological sort

Word Ladder
    BFS on implicit graph (word neighbors)

Network Delay Time
    Dijkstra + max of distances

Accounts Merge
    Union-Find + graph of emails

Critical Connections
    Bridges (DFS) — advanced, but DFS-based

Cheapest Flights Within K Stops
    Bellman-Ford k relaxations OR modified Dijkstra
```

Notice:

You're no longer learning a new magical algorithm for each problem.

You're **recognizing which graph pattern applies and which algorithm implements it**.

---

# 18. Pattern Recognition

Train yourself to notice these signals.

```text
"Shortest path, unweighted / minimum steps"
        ↓
BFS (level-by-level)
```

```text
"Shortest path, weighted, non-negative"
        ↓
Dijkstra
```

```text
"Negative edge weights"
        ↓
Bellman-Ford
```

```text
"All pairs shortest paths, small V"
        ↓
Floyd-Warshall
```

```text
"Prerequisites / dependencies / ordering"
        ↓
Topological Sort
```

```text
"Can finish all tasks / is DAG?"
        ↓
Cycle detection in directed graph
```

```text
"Connected / same group / merge groups"
        ↓
Union-Find or DFS components
```

```text
"Minimum cost connect all nodes"
        ↓
MST — Kruskal or Prim
```

```text
"Grid / maze / islands"
        ↓
Grid as graph + BFS/DFS
```

```text
"Multiple sources spreading simultaneously"
        ↓
Multi-source BFS
```

```text
"Redundant edge / cycle on add"
        ↓
Union-Find
```

```text
"Partition into two groups, no conflict"
        ↓
Bipartite check (2-coloring)
```

```text
"All paths / enumerate routes"
        ↓
DFS + Backtracking
```

```text
"Implicit states and transitions"
        ↓
BFS/DFS on implicit graph
```

That last one is particularly important.

Many problems never give you an explicit edge list. The graph is **in the state space**.

---

# 19. Edge Cases — Extremely Important for Graphs

Always test mentally:

## Empty and Single Vertex

```text
n = 0        → handle gracefully
n = 1        → single node, no edges
```

## Disconnected Graph

```text
BFS/DFS from one node does not visit all
→ loop over all vertices or Union-Find
```

## Self-Loops

```text
0 → 0

Cycle in directed. In undirected, usually ignored or handled.
```

## Duplicate Edges

```text
0-1 appears twice
→ adjacency list may have duplicate neighbors; Union-Find still works
```

## Negative Weights

```text
Dijkstra FAILS
→ use Bellman-Ford
```

## Negative Cycles

```text
Shortest path undefined (can go around cycle forever decreasing cost)
→ Bellman-Ford detects
```

## Large Weights / Overflow

```text
dist + weight > INT_MAX
→ use long long
```

## Directed vs Undirected Mistake

```text
Adding edge only one way when graph is undirected
→ wrong connectivity
```

## 1-Indexed vs 0-Indexed

```text
Input nodes 1..n, code uses 0..n-1
→ convert or size n+1
```

## Grid Boundaries

```text
(r, c) out of bounds
→ always check before visiting
```

## BFS Visited Timing

```text
Mark visited when enqueueing (standard)
vs when dequeuing (may enqueue same node multiple times)
→ prefer mark on enqueue for efficiency
```

## DFS Stack Overflow

```text
Deep graph, V = 100000
→ recursive DFS may crash; use iterative stack or increase limit
```

## Tree vs Graph

```text
Tree: V-1 edges, no cycles, connected
Graph: may have cycles, multiple edges
```

## Complete Graph

```text
V nodes, V*(V-1)/2 edges
→ Floyd-Warshall or dense Dijkstra may be appropriate
```

---

# 20. Important Implementation Skill: Draw the Graph

For graphs, **don't try to do everything mentally initially**.

Suppose:

```text
Edges: 0-1, 1-2, 2-0, 1-3

Draw:

    0
   / \
  2   1 ─ 3
```

Mark:

```text
visited set
current queue/stack
distances
parent pointers
```

For BFS levels:

```text
Level 0: {0}
Level 1: {1, 2}
Level 2: {3}
```

For Union-Find:

```text
parent array after each union
```

This prevents classic mistakes:

```text
"I forgot an edge direction"
"I ran Dijkstra on negative weights"
"I only BFS from one node in disconnected graph"
"I used O(V²) matrix when V = 10000"
```

Graphs become dramatically easier when you visualize.

---

# 21. Problems to Practice

You don't need 100 random graph problems.

Choose problems representing each concept.

### Fundamentals

- Build adjacency list from edge list
- BFS traversal order
- DFS traversal order
- Count vertices and edges

### BFS

- Shortest path in unweighted graph
- Word Ladder
- Rotting Oranges (multi-source BFS)
- 01 Matrix
- Maze shortest path

### DFS

- Number of Islands
- Flood Fill
- Clone Graph
- Path exists

### Cycle Detection

- Course Schedule (can finish?)
- Redundant Connection

### Topological Sort

- Course Schedule II
- Alien Dictionary (harder)

### Union-Find

- Number of Connected Components (dynamic)
- Accounts Merge
- Redundant Connection
- Friend Circles

### MST

- Min Cost to Connect All Points
- Connecting Cities With Minimum Cost

### Dijkstra

- Network Delay Time
- Path With Minimum Effort
- Cheapest Flights Within K Stops (variant)

### Bellman-Ford

- Shortest path with negative edges (classic textbook)
- Distance limited paths counting (variant)

### Floyd-Warshall

- Find the City With the Smallest Number of Neighbors at a Threshold Distance

### Bipartite / Coloring

- Is Graph Bipartite?

### Backtracking

- All Paths From Source to Target

### Composition

- Course Schedule II (cycle + topo)
- Word Ladder (implicit graph BFS)
- Critical Connections (bridges — when ready for advanced)

That is enough to understand the major graph patterns deeply.

---

# 22. For EVERY Graph Problem

Use the same learning process established for Arrays, Linked Lists, and Hash Tables.

Ask:

**1. What is the graph?**

```text
Explicit edges?
Grid?
Implicit state graph?
Directed or undirected?
Weighted or unweighted?
```

**2. What is the question really asking?**

```text
Shortest path?
Connectivity?
Ordering?
Minimum spanning?
Cycle?
Count components?
Reachability?
```

**3. What pattern naturally fits?**

```text
BFS?
DFS?
Dijkstra?
Union-Find?
Topo sort?
MST?
Backtracking?
```

**4. What representation should I use?**

```text
Adjacency list (default)
Matrix (small V or dense)
Edge list (Kruskal/Bellman-Ford)
No storage (implicit)
```

**5. What do I track per vertex?**

```text
visited
distance
parent
color (white/gray/black)
in-degree
component id
```

**6. Algorithm correctness depends on what invariant?**

```text
BFS: first visit = shortest in unweighted
Dijkstra: non-negative weights, greedy extract-min
Topo: all predecessors before node
Union-Find: same root = same component
```

**7. Complexity?**

```text
Time: O(V+E)? O(E log V)? O(V³)?
Space: O(V)? O(V²)?
```

**8. Edge cases?**

```text
Disconnected
Empty
Single node
Negative weights
Cycles
1-indexed input
Overflow
```

**9. Can I draw a small example and trace?**

Always.

**10. Is the graph disguised as something else?**

```text
Grid → graph
String transformations → implicit graph
Dependency list → directed graph
```

---

# 23. Compare Multiple Approaches

Example:

## Shortest Path

### BFS

```text
Unweighted only

Time:  O(V + E)
Space: O(V)
```

### Dijkstra

```text
Non-negative weights

Time:  O((V + E) log V)
Space: O(V)
```

### Bellman-Ford

```text
Negative weights, detects negative cycles

Time:  O(V × E)
Space: O(V)
```

Don't just conclude:

> Dijkstra always wins.

Ask:

- Are weights non-negative?
- Single source or all pairs?
- How large is V?
- Are edges sparse or dense?
- Is input an edge list or adjacency structure?

---

Example:

## Connect All Points (Minimum Cost)

### Kruskal

```text
Sort all edges, Union-Find

Good when edges are explicit list
Time: O(E log E)
```

### Prim

```text
Greedy grow from source, min-heap

Good with adjacency list (dense complete graph from coordinates)
Time: O(E log V)
```

For `n` points, complete graph has E = O(n²). Both work. Prim with adjacency is often natural when edges are implicit from coordinates.

---

Example:

## Connected Components

### DFS/BFS

```text
One pass per unvisited node

Time:  O(V + E)
Space: O(V)
```

### Union-Find

```text
Union on each edge, count roots

Time:  O(E × α(V))
Space: O(V)
Better for dynamic connectivity queries
```

---

# 24. One Especially Important Mental Shift

When beginners see:

```text
edges = [[0,1],[1,2],[2,3]]
```

they think only about the edge list.

As you improve, you should ask:

> What graph am I actually building, and what property am I trying to preserve?

Maybe:

```text
Reachability        → BFS/DFS
Minimum cost        → Dijkstra / MST
Dependency order    → Topological sort
Same group          → Union-Find
Layer distance      → BFS levels
All pairs distance  → Floyd-Warshall
```

Maybe the graph is not given:

```text
Grid cell → neighbor cells
Word → words differing by one letter
State → next states from one move
Person → friends
Task → prerequisites
```

That's the deeper idea behind graph problems.

You're **modeling relationships and choosing the algorithm that answers the relationship question**.

---

# 25. Graphs vs Trees

```text
TREE                          GRAPH
────────────────────────────────────────────────
V-1 edges (if connected)      E can vary
No cycles                     Cycles allowed
Unique path between nodes     Multiple paths
Root (usually)                No root (usually)
Parent-child hierarchy        Arbitrary connections
DFS/BFS simpler               More algorithm variety
```

Every tree is a graph. Not every graph is a tree.

```text
Tree check:
  connected AND E = V - 1
```

---

# 26. Implement Graph Algorithms Yourself

Don't skip implementation.

## Version 1 — BFS + DFS

Build adjacency list. BFS with queue. DFS recursive.

## Version 2 — Union-Find

`find` with path compression. `unite` with union by rank.

## Version 3 — Topological Sort

Kahn's algorithm with in-degree array.

## Version 4 — Dijkstra

Priority queue + distance array. Handle stale heap entries.

## Version 5 — Kruskal

Sort edges. Union-Find to detect cycles.

## Version 6 — Prim

Min-heap. Track in-MST set.

## Version 7 — Bellman-Ford

Edge list relaxation V-1 times.

## Version 8 — Floyd-Warshall

2D dist matrix. Triple loop over k, i, j.

If you can implement these from scratch and explain each line, graph interviews and advanced topics become manageable.

---

# 27. Real-World Applications

```text
Google Maps          shortest path, road networks (Dijkstra, A*)
Social networks      friend suggestions, connectivity
Internet routing     BGP, network protocols
Build systems        topological sort (Make, Bazel)
Compilers            dependency graphs, register allocation
Game AI              pathfinding (BFS, A*)
Epidemiology         spread models (BFS on contact graphs)
Circuit design       connectivity, spanning trees
Recommendation       graph-based ranking
Database query plans join ordering
Kubernetes           pod scheduling dependencies
```

Graphs are not abstract puzzles. They model **networks of things that connect**.

---

# 28. Common Mistakes to Avoid

```text
1. Using DFS for shortest path in unweighted graph
   → use BFS

2. Using Dijkstra with negative edges
   → use Bellman-Ford

3. Forgetting to handle disconnected graph
   → outer loop over all vertices

4. Wrong direction on directed edges
   → adj[u].push_back(v) only for u → v

5. Not marking visited in BFS before enqueue
   → duplicate queue entries, worse performance

6. O(V²) matrix for V = 10^5
   → use adjacency list

7. Recursion depth overflow on long path
   → iterative DFS or increase stack

8. Integer overflow in dist + weight
   → use long long

9. Confusing MST with shortest path
   → different problems

10. Building complete graph unnecessarily
    → think if implicit edge calculation is enough
```

---

# Your Complete Graphs Checklist

```text
GRAPHS
│
├── Fundamentals
│   ├── Vertex / Edge
│   ├── Directed / Undirected
│   ├── Weighted / Unweighted
│   ├── Path / Cycle
│   ├── Degree / In-degree / Out-degree
│   ├── Connected / Strongly connected
│   ├── Component / Subgraph
│   ├── Sparse / Dense
│   └── Tree vs Graph
│
├── Representations
│   ├── Adjacency List (default)
│   ├── Adjacency Matrix
│   ├── Edge List
│   ├── Grid as graph
│   ├── Implicit graph
│   └── Representation trade-offs
│
├── Traversal
│   ├── BFS
│   │   ├── Queue
│   │   ├── Level-by-level
│   │   ├── Shortest path unweighted
│   │   └── O(V + E)
│   │
│   └── DFS
│       ├── Recursive
│       ├── Iterative stack
│       ├── Exploration
│       └── O(V + E)
│
├── Core Algorithms
│   ├── Cycle Detection
│   │   ├── Undirected (DFS parent / Union-Find)
│   │   └── Directed (three-color / Kahn's)
│   │
│   ├── Connected Components
│   │   ├── DFS/BFS count
│   │   └── Union-Find
│   │
│   ├── Topological Sort
│   │   ├── Kahn's (BFS in-degree)
│   │   └── DFS post-order
│   │
│   ├── Union-Find (DSU)
│   │   ├── find + path compression
│   │   ├── unite + union by rank
│   │   └── Dynamic connectivity
│   │
│   ├── MST
│   │   ├── Kruskal (sort + Union-Find)
│   │   └── Prim (greedy + heap)
│   │
│   └── Shortest Paths
│       ├── BFS (unweighted)
│       ├── Dijkstra (non-negative)
│       ├── Bellman-Ford (negative, cycle detect)
│       └── Floyd-Warshall (all pairs)
│
├── Core Patterns
│   ├── BFS Level Expansion
│   ├── DFS Flood Fill
│   ├── Multi-Source BFS
│   ├── Topological Sort / Dependencies
│   ├── Union-Find Connectivity
│   ├── Dijkstra Weighted Shortest Path
│   ├── Bellman-Ford Negatives
│   ├── Floyd-Warshall All Pairs
│   ├── Cycle Detection
│   ├── Connected Components
│   ├── Bipartite 2-Coloring
│   ├── Backtracking All Paths
│   ├── MST Minimum Connection
│   ├── Grid as Implicit Graph
│   └── In/Out Degree Peel
│
├── Pattern Composition
│   ├── Course Schedule (cycle + topo)
│   ├── Word Ladder (implicit BFS)
│   ├── Network Delay (Dijkstra)
│   ├── Accounts Merge (Union-Find)
│   └── Cheapest Flights K Stops (Bellman-Ford variant)
│
├── Analysis
│   ├── Time Complexity Table
│   ├── Space Complexity
│   ├── Algorithm Selection
│   └── Sparse vs Dense implications
│
├── Edge Cases
│   ├── Empty / single vertex
│   ├── Disconnected
│   ├── Self-loops / duplicate edges
│   ├── Negative weights / cycles
│   ├── Overflow
│   ├── 1-indexed input
│   ├── Grid boundaries
│   └── Recursion depth
│
├── Implement Yourself
│   ├── BFS
│   ├── DFS
│   ├── Union-Find
│   ├── Kahn's Topo Sort
│   ├── Dijkstra
│   ├── Kruskal
│   ├── Prim
│   ├── Bellman-Ford
│   └── Floyd-Warshall
│
└── For EVERY Problem
    ├── Identify graph type
    ├── Identify the real question
    ├── Choose pattern + algorithm
    ├── Choose representation
    ├── Track per-vertex state
    ├── State the invariant
    ├── Trace small example
    ├── Handle edge cases
    ├── Analyze time and space
    ├── Compare alternatives
    └── Explain WHY this algorithm fits
```

For your **core DSA pass (Phase 8)**, that's enough graphs. You don't need to dive deeply yet into network flow, bipartite matching, advanced SCC (Tarjan/Kosaraju), bridge/articulation point theory, Euler/Hamilton paths, A* heuristics, or industrial graph databases.

Once you can **build adjacency lists, run BFS/DFS correctly, implement Union-Find, choose between Dijkstra/Bellman-Ford/Floyd-Warshall, run Kruskal/Prim, and recognize when a problem is a disguised graph**, you're ready for the next major block: **Dynamic Programming (Phase 9)**.
