# Advanced Graph Algorithms

> **Phase 14** from [map.md](../map.md). Prerequisite: **Graphs (Phase 8)** — see [graphs/notes.md](../graphs/notes.md) for BFS, DFS, Union-Find, shortest paths, MST, and core patterns before diving here.

## 1. Advanced Graph Fundamentals

First understand **why advanced graph algorithms exist**.

Phase 8 graphs cover traversal, connectivity, shortest paths, and MST. Many real problems ask deeper structural questions:

```text
PHASE 8 (Graphs)                    PHASE 14 (Advanced)
────────────────────────────────────────────────────────────
Are A and B connected?              Which directed clusters are mutually reachable? (SCC)
Shortest path                       Maximize flow through a network
Bipartite check (2-coloring)        Maximum matching in bipartite graph
DFS exploration                     Bridges that disconnect the graph
Tree DFS                            LCA of two nodes in a tree
DAG topological sort                2-SAT via implication graph + SCC
```

A graph is still:

```text
G = (V, E)

V = vertices
E = edges (may be directed, weighted, capacitated)
```

But the **questions** change:

```text
Advanced Question Types
┌─────────────────────────────────────────────┐
│ Structural decomposition (SCC, bridges)       │
│ Optimization on flows and matchings           │
│ Tree path queries (LCA, HLD)                  │
│ Logic satisfiability (2-SAT)                  │
│ Euler tours and degree constraints            │
└─────────────────────────────────────────────┘
```

Understand:

- What is a **strongly connected component** vs undirected connected component?
- What is a **bridge** (cut edge) vs **articulation point** (cut vertex)?
- What is **network flow**, **capacity**, **augmenting path**, **max-flow min-cut**?
- What is **bipartite matching** and how does it relate to flow?
- What is **LCA** and why do trees need specialized algorithms?
- What is **2-SAT** and how does SCC solve it?
- When does an **Euler path/circuit** exist?
- What is **heavy-light decomposition** used for?

---

# 2. Why Advanced Graph Algorithms Exist

Compare problems that Phase 8 tools handle poorly or cannot handle.

**Directed mutual reachability:**

```text
0 → 1 → 2
↑       │
└───────┘
3 → 4

SCCs: {0,1,2} and {3} and {4}
```

DFS component count on undirected graph does not answer "can every node in a group reach every other node in the same group" for directed edges.

**Network reliability:**

```text
A ─── B ─── C
      │
      D

Bridge: B-C — removing it disconnects C from the rest
Articulation: B — removing B disconnects the graph
```

**Resource allocation:**

```text
Source S ──cap 10──> [network] ──> Sink T

How much can flow from S to T without exceeding edge capacities?
```

**Assignment:**

```text
Workers: W1, W2, W3
Jobs:    J1, J2, J3

Each worker can do some jobs. Assign maximum jobs with one worker per job.
→ Bipartite matching
```

**Tree queries:**

```text
        1
       / \
      2   3
     / \
    4   5

LCA(4, 5) = 2
LCA(4, 3) = 1
```

Phase 8 BFS from both nodes is O(n) per query. LCA preprocesses for O(log n) or O(1) per query.

**Boolean constraints:**

```text
(x₁ OR x₂) AND (NOT x₁ OR x₃) AND (NOT x₂ OR NOT x₃)

Can we assign true/false to each variable satisfying all clauses?
→ 2-SAT via implication graph
```

Phase 8 gives:

```text
BFS/DFS traversal
Shortest paths
Union-Find connectivity
Topological sort on DAGs
MST
Basic bipartite coloring
```

Advanced algorithms give:

```text
Directed structural decomposition (SCC)
Critical edge/vertex detection
Flow and cut optimization
Maximum matching
Fast tree ancestry queries
Satisfiability of 2-CNF formulas
Euler tour existence and construction
Path queries on trees (HLD)
```

So don't learn:

> Advanced graph algorithms are harder versions of DFS.

Learn:

> Each advanced algorithm answers a **specific structural or optimization question** that basic traversal cannot answer efficiently.

---

# 3. Prerequisites from Phase 8

Before Phase 14, you should be comfortable with:

```text
From graphs/notes.md
├── Adjacency list construction
├── BFS and DFS (visited arrays, recursion/stack)
├── Directed vs undirected handling
├── Topological sort (Kahn's, DFS post-order)
├── Union-Find (find, unite, path compression)
├── Dijkstra (for flow-related shortest path ideas)
├── Bipartite check via 2-coloring (Pattern 11)
├── Connected components (undirected)
└── Complexity: O(V + E) traversal baseline
```

Cross-reference map:

| Phase 8 concept | Advanced extension |
| --- | --- |
| Connected components | Strongly connected components (directed) |
| DFS exploration | Low-link DFS for bridges/APs |
| Bipartite 2-coloring | Bipartite maximum matching |
| Topological sort on DAG | SCC condensation → topo on meta-graph |
| Shortest path | Min-cut as dual of max-flow |
| Tree DFS | LCA, Euler tour, HLD |

If any Phase 8 item is shaky, revisit [graphs/notes.md](../graphs/notes.md) before implementing Tarjan or Edmonds-Karp.

---

# 4. Strongly Connected Components (SCC)

## Definition

In a **directed** graph, an SCC is a maximal set of vertices such that for every pair `u, v` in the set, there exists a path from `u` to `v` and from `v` to `u`.

```text
Graph:

0 → 1 → 2
↑       │
└───────┘

3 → 4

SCCs:
  {0, 1, 2}
  {3}
  {4}
```

Note: `{3, 4}` is NOT an SCC — 4 cannot reach 3.

## Why SCC Matters

- **Condensation graph**: shrink each SCC to one node → get a DAG
- **2-SAT**: satisfiability reduces to "no variable implies its negation in same SCC"
- **Dependency analysis**: circular dependency detection in directed systems
- **Web graph**: communities with mutual reachability

## Kosaraju's Algorithm

Two-pass DFS. Elegant and easy to implement.

**Pass 1**: DFS on original graph, push vertices to stack on finish (post-order stack).

**Pass 2**: DFS on **transpose graph** (reverse all edges), pop from stack, each DFS tree is one SCC.

```text
Original:     0→1→2, 2→0, 3→4

Transpose:    1→0, 2→1, 0→2, 4→3

Pass 1 finish order (example): 4, 3, 2, 1, 0
Pass 2 from stack: {0,1,2}, then {3}, then {4}
```

```cpp
void dfs1(int u, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& order) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) dfs1(v, adj, visited, order);
    }
    order.push_back(u); // finish time order
}

void dfs2(int u, vector<vector<int>>& radj, vector<bool>& visited, vector<int>& component) {
    visited[u] = true;
    component.push_back(u);
    for (int v : radj[u]) {
        if (!visited[v]) dfs2(v, radj, visited, component);
    }
}

vector<vector<int>> kosaraju(int n, vector<vector<int>>& adj) {
    vector<int> order;
    vector<bool> visited(n, false);
    for (int i = 0; i < n; i++) {
        if (!visited[i]) dfs1(i, adj, visited, order);
    }

    vector<vector<int>> radj(n);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) radj[v].push_back(u);
    }

    visited.assign(n, false);
    vector<vector<int>> sccs;
    for (int i = n - 1; i >= 0; i--) {
        int u = order[i];
        if (!visited[u]) {
            vector<int> comp;
            dfs2(u, radj, visited, comp);
            sccs.push_back(comp);
        }
    }
    return sccs;
}
```

**Time**: O(V + E)  
**Space**: O(V + E)

**Intuition**: First pass orders by "finish time" so that when you reverse edges, roots of SCC DAG are processed first.

## Tarjan's Algorithm

Single DFS with **low-link** values. One pass.

Maintain:

```text
dfs[u]   = discovery time
low[u]   = lowest dfs reachable from u via tree edges + one back edge
onStack[u] = u is on current DFS stack
```

SCC root: `low[u] == dfs[u]` → pop stack until u is removed.

```cpp
vector<vector<int>> tarjanSCC(int n, vector<vector<int>>& adj) {
    vector<int> disc(n, -1), low(n, -1), onStack(n, 0);
    vector<int> st;
    int timer = 0;
    vector<vector<int>> sccs;

    function<void(int)> tarjanDfs = [&](int u) {
        disc[u] = low[u] = timer++;
        st.push_back(u);
        onStack[u] = 1;

        for (int v : adj[u]) {
            if (disc[v] == -1) {
                tarjanDfs(v);
                low[u] = min(low[u], low[v]);
            } else if (onStack[v]) {
                low[u] = min(low[u], disc[v]);
            }
        }

        if (low[u] == disc[u]) {
            vector<int> comp;
            while (true) {
                int x = st.back();
                st.pop_back();
                onStack[x] = 0;
                comp.push_back(x);
                if (x == u) break;
            }
            sccs.push_back(comp);
        }
    };

    for (int i = 0; i < n; i++) {
        if (disc[i] == -1) tarjanDfs(i);
    }
    return sccs;
}
```

**Time**: O(V + E)  
**Space**: O(V)

**When to prefer**:

```text
Kosaraju   → two graphs + stack; very clear two-phase logic
Tarjan     → one pass; slightly trickier but no transpose graph
```

## Condensation Graph

Replace each SCC with a super-node. Edges between SCCs form a **DAG**.

```text
SCC A = {0,1,2}    SCC B = {3}    SCC C = {4}

Condensation:

A → B → C
```

Use condensation for:

- Topological order of SCC blocks
- Reachability queries after SCC decomposition
- 2-SAT assignment order

## Complexity

```text
Kosaraju:  O(V + E) time, O(V + E) space
Tarjan:    O(V + E) time, O(V) space
```

---

# 5. Bridges and Articulation Points

## Definitions

**Bridge (cut edge)**: edge whose removal increases the number of connected components.

**Articulation point (cut vertex)**: vertex whose removal increases the number of connected components.

```text
    0 ─── 1 ─── 2
          │
          3

Bridge: 1-2 (or 0-1 depending on full graph)
Articulation: 1
```

## Low-Link DFS (Undirected)

Same `dfs` and `low` idea as Tarjan, but for undirected graphs.

For edge `u — v`:

- **Bridge**: if `low[v] > dfs[u]` (v's subtree cannot reach u or above via back edge)
- **Articulation**: 
  - root with ≥ 2 children in DFS tree
  - non-root `u` with child `v` where `low[v] >= dfs[u]`

```cpp
void dfsBridge(int u, int parent, vector<vector<int>>& adj,
               vector<int>& dfs, vector<int>& low, int& timer,
               vector<pair<int,int>>& bridges) {
    dfs[u] = low[u] = timer++;
    for (int v : adj[u]) {
        if (dfs[v] == -1) {
            dfsBridge(v, u, adj, dfs, low, timer, bridges);
            low[u] = min(low[u], low[v]);
            if (low[v] > dfs[u]) {
                bridges.push_back({min(u, v), max(u, v)});
            }
        } else if (v != parent) {
            low[u] = min(low[u], dfs[v]);
        }
    }
}

void dfsAP(int u, int parent, vector<vector<int>>& adj,
           vector<int>& dfs, vector<int>& low, int& timer,
           vector<bool>& isAP, int& rootChildren) {
    dfs[u] = low[u] = timer++;
    int children = 0;
    for (int v : adj[u]) {
        if (dfs[v] == -1) {
            children++;
            dfsAP(v, u, adj, dfs, low, timer, isAP, rootChildren);
            low[u] = min(low[u], low[v]);
            if (parent != -1 && low[v] >= dfs[u]) {
                isAP[u] = true;
            }
        } else if (v != parent) {
            low[u] = min(low[u], dfs[v]);
        }
    }
    if (parent == -1 && children >= 2) isAP[u] = true;
}
```

**Time**: O(V + E)

## Applications

- **Critical Connections** (LeetCode 1192) — find all bridges
- Network reliability — which links/routers are critical?
- Designing redundant networks

---

# 6. Network Flow

## Flow Network

```text
Directed graph with:
  source s
  sink t
  capacity c(u,v) ≥ 0 on each edge
  flow f(u,v) ≤ c(u,v)
  flow conservation at internal nodes
```

```text
     cap 10
S ──────────> A ──cap 5──> T
              │
              └──cap 8──> T
```

**Flow conservation**: for every node except s and t, total inflow = total outflow.

**Max flow**: maximize total flow leaving source (equals total entering sink).

## Ford-Fulkerson Method

Repeatedly find an **augmenting path** from s to t in the **residual graph** and push flow.

Residual graph:

```text
Forward edge: remaining capacity c - f
Backward edge: allows undoing flow (capacity = current flow)
```

```cpp
bool bfsResidual(int s, int t, vector<vector<int>>& cap, vector<int>& parent) {
    int n = cap.size();
    fill(parent.begin(), parent.end(), -1);
    queue<int> q;
    parent[s] = s;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < n; v++) {
            if (parent[v] == -1 && cap[u][v] > 0) {
                parent[v] = u;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int fordFulkerson(vector<vector<int>>& cap, int s, int t) {
    int n = cap.size();
    int flow = 0;
    vector<int> parent(n);
    while (bfsResidual(s, t, cap, parent)) {
        int pathFlow = INT_MAX;
        int v = t;
        while (v != s) {
            int u = parent[v];
            pathFlow = min(pathFlow, cap[u][v]);
            v = u;
        }
        v = t;
        while (v != s) {
            int u = parent[v];
            cap[u][v] -= pathFlow;
            cap[v][u] += pathFlow;
            v = u;
        }
        flow += pathFlow;
    }
    return flow;
}
```

**Time**: O(E × max_flow) — bad if capacities are huge.

## Edmonds-Karp

Ford-Fulkerson with **BFS** for shortest augmenting path (fewest edges).

**Time**: O(V × E²)

```text
Prefer Edmonds-Karp when:
  capacities are large
  you need predictable polynomial bound
```

## Max-Flow Min-Cut Theorem

Maximum flow value equals minimum **s-t cut** capacity.

**Cut**: partition vertices into sets S (contains s) and T (contains t). Cut capacity = sum of capacities of edges from S to T.

```text
Min cut edges are "bottlenecks" limiting flow
After max flow, edges with cap[u][v] == 0 in residual (and original edge exists) form cut
```

Applications:

- Bipartite matching (unit capacity)
- Project selection
- Image segmentation
- Minimum vertex cover in bipartite graphs (Kőnig's theorem)

## Dinic's Algorithm (Bonus)

Level graph + blocking flow.

**Time**: O(V² × E) — often faster in practice.

Know Ford-Fulkerson / Edmonds-Karp first; Dinic is optimization when needed.

---

# 7. Bipartite Matching

## Definition

**Bipartite graph**: vertices split into L and R; edges only between L and R.

**Matching**: set of edges no two sharing a vertex.

**Maximum matching**: largest such set.

```text
L: workers W1, W2, W3
R: jobs   J1, J2, J3

W1 ─ J1
W1 ─ J2
W2 ─ J2
W3 ─ J1

Max matching size = 2 (e.g. W1-J2, W3-J1)
```

## Reduction to Max Flow

Add source connected to all L, sink from all R, capacity 1 on all edges.

```text
s → W1, W2, W3 → jobs → t

Max flow = max matching size
```

## DFS Augmenting Path (Kuhn's Algorithm)

For each left node, try to match via DFS; if matched right node, recurse to reassign.

```cpp
bool dfsMatch(int u, vector<vector<int>>& adj, vector<int>& matchR, vector<bool>& seen) {
    for (int v : adj[u]) {
        if (seen[v]) continue;
        seen[v] = true;
        if (matchR[v] == -1 || dfsMatch(matchR[v], adj, matchR, seen)) {
            matchR[v] = u;
            return true;
        }
    }
    return false;
}

int maxBipartiteMatching(vector<vector<int>>& adjL, int nL, int nR) {
    vector<int> matchR(nR, -1);
    int matching = 0;
    for (int u = 0; u < nL; u++) {
        vector<bool> seen(nR, false);
        if (dfsMatch(u, adjL, matchR, seen)) matching++;
    }
    return matching;
}
```

**Time**: O(V × E)

## Hopcroft-Karp

BFS layering + DFS blocking — **O(E × √V)**.

Use when graph is large and matching is central.

## Applications

- Job assignment
- Minimum path cover in DAG (via matching)
- Board domino / tile placement
- **Dancing Links** adjacent concept for exact cover

---

# 8. Lowest Common Ancestor (LCA)

## Definition

For nodes `u` and `v` in a **rooted tree**, LCA is the deepest node that is an ancestor of both.

```text
        1
       / \
      2   3
     / \
    4   5

LCA(4,5) = 2
LCA(4,3) = 1
```

## Naive Approaches

```text
Parent pointers up from u and v: O(depth)
DFS record paths, walk together: O(n) preprocess per query pair naive
```

## Binary Lifting

Precompute `up[k][v]` = ancestor of v after 2^k steps up.

```cpp
vector<vector<int>> buildBinaryLifting(int n, int root, vector<vector<int>>& tree) {
    int LOG = 20;
    vector<vector<int>> up(LOG, vector<int>(n, -1));
    vector<int> depth(n, 0);

    function<void(int,int)> dfs = [&](int u, int p) {
        up[0][u] = p;
        for (int k = 1; k < LOG; k++) {
            if (up[k-1][u] != -1)
                up[k][u] = up[k-1][up[k-1][u]];
        }
        for (int v : tree[u]) {
            if (v != p) {
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
        }
    };
    dfs(root, -1);
    return up;
}

int lca(int u, int v, vector<vector<int>>& up, vector<int>& depth) {
    int LOG = up.size();
    if (depth[u] < depth[v]) swap(u, v);
    int diff = depth[u] - depth[v];
    for (int k = 0; k < LOG; k++) {
        if (diff & (1 << k)) u = up[k][u];
    }
    if (u == v) return u;
    for (int k = LOG - 1; k >= 0; k--) {
        if (up[k][u] != up[k][v]) {
            u = up[k][u];
            v = up[k][v];
        }
    }
    return up[0][u];
}
```

**Preprocess**: O(n log n)  
**Query**: O(log n)

## Euler Tour + RMQ

DFS records first visit time `in[u]` and tour array of node IDs.

```text
Tour of tree above: 1, 2, 4, 2, 5, 2, 1, 3

LCA(u,v) = node with minimum in-time between in[u] and in[v] in tour
```

Use sparse table or segment tree on `in[]` values for RMQ.

**Preprocess**: O(n)  
**Query**: O(1) with O(n log n) sparse table

## Applications

- Distance in tree: `dist(u,v) = depth[u] + depth[v] - 2*depth[lca]`
- K-th ancestor queries (with binary lifting)
- Tree path queries combined with HLD

---

# 9. 2-SAT Introduction

## Problem

Given boolean variables `x₁, x₂, ..., xₙ`, each clause has **at most two literals**:

```text
(x₁ OR NOT x₂) AND (x₂ OR x₃) AND (NOT x₁ OR NOT x₃)
```

Is there a satisfying assignment?

## Implication Graph

Each variable has two nodes: `x` and `¬x`.

Clause `(a OR b)` → implications `¬a → b` and `¬b → a`.

```text
(x₁ OR NOT x₂)  →  ¬x₁ → ¬x₂,  x₂ → x₁
```

Build directed graph with 2n nodes. Run SCC.

**Unsatisfiable iff** for some `i`, `xᵢ` and `¬xᵢ` are in the same SCC.

## Assignment from SCC

Process SCCs in **reverse topological order** of condensation DAG.

If `xᵢ` not assigned, assign false to all nodes in SCC of `xᵢ`, true to negations.

```cpp
bool solve2SAT(int n, vector<pair<int,int>>& clauses, vector<bool>& assignment) {
  // n variables, clauses as (a,b) literals: positive = variable, negative = negated
  // Build implication graph, run Tarjan, check x and ¬x same SCC
  // Assign in reverse SCC order
  return true; // skeleton — implement with 2*n nodes
}
```

## Applications

- Scheduling with pairwise constraints
- Game theory win/lose assignments
- Puzzle constraints (e.g. grid with "at least one of these two cells")

---

# 10. Euler Path and Circuit

## Definitions

**Euler path**: traverse every **edge** exactly once.

**Euler circuit**: Euler path that starts and ends at same vertex.

Vertices can be revisited — edges must all be used once.

```text
Euler circuit exists:

0 ─ 1 ─ 2
      │
      0

Trail: 0-1-2-0 (every edge once, return to start)
```

## Existence Conditions

**Undirected**:

```text
Euler circuit  → all vertices even degree, connected (non-zero edges)
Euler path     → exactly 0 or 2 odd-degree vertices, connected
```

**Directed**:

```text
Euler circuit  → in-degree = out-degree for all, connected in underlying graph
Euler path     → at most one vertex with out-in = 1, one with in-out = 1, rest balanced
```

## Hierholzer's Algorithm

Start at valid vertex, DFS building circuit, splice sub-circuits.

```cpp
vector<int> eulerCircuit(int start, vector<vector<int>>& adj) {
    vector<int> stack = {start};
    vector<int> circuit;
    while (!stack.empty()) {
        int u = stack.back();
        if (adj[u].empty()) {
            circuit.push_back(u);
            stack.pop_back();
        } else {
            int v = adj[u].back();
            adj[u].pop_back();
            adj[v].pop_back(); // undirected — remove both directions
            stack.push_back(v);
        }
    }
    reverse(circuit.begin(), circuit.end());
    return circuit;
}
```

**Time**: O(E)

## Applications

- Route planning (traverse all streets)
- DNA sequencing (De Bruijn graphs)
- Drawing figure without retracing edges

**Not Hamilton**: Hamilton visits every **vertex** once — different problem.

---

# 11. Heavy-Light Decomposition (HLD) — Introduction

## Purpose

Decompose a tree into **heavy paths** so any root-to-node path crosses O(log n) chains.

Enables path queries/updates with segment tree or Fenwick on chains.

```text
Heavy child: child with largest subtree size
Light edge: to non-heavy child

Each path from root uses O(log n) light edges
```

## Outline

1. DFS compute subtree sizes, mark heavy child
2. Decompose into chains with head IDs
3. Query path by aggregating segment trees on chains

```cpp
// Skeleton — full HLD pairs with segment tree on chain positions
void dfsSize(int u, int p, vector<vector<int>>& tree,
             vector<int>& size, vector<int>& heavy) {
    size[u] = 1;
  heavy[u] = -1;
    for (int v : tree[u]) {
        if (v == p) continue;
        dfsSize(v, u, tree, size, heavy);
        size[u] += size[v];
        if (heavy[u] == -1 || size[v] > size[heavy[u]])
            heavy[u] = v;
    }
}
```

**Path query**: O(log² n) typical with segment tree per chain.

## When to Use

- Many path sum/max/min queries on static tree
- Alternative: Euler tour + segment tree for subtree; HLD for paths

Prerequisite: segment tree / Fenwick from Phase 12.

---

# 12. Complexity Summary

```text
Algorithm                      Time              Space
────────────────────────────────────────────────────────────
Kosaraju SCC                   O(V + E)          O(V + E)
Tarjan SCC                     O(V + E)          O(V)
Bridges / Articulation         O(V + E)          O(V)
Ford-Fulkerson (generic)       O(E × max_flow)   O(V²) or O(E)
Edmonds-Karp                   O(V × E²)         O(V + E)
Dinic (bonus)                  O(V² × E)         O(V + E)
Bipartite matching (DFS)       O(V × E)          O(V + E)
Hopcroft-Karp                  O(E × √V)         O(V + E)
LCA binary lifting             O(log n) query    O(n log n)
LCA Euler + RMQ                O(1) query        O(n log n)
2-SAT                          O(V + E)          O(V + E)
Euler circuit (Hierholzer)     O(E)              O(E)
HLD path query                 O(log² n)         O(n)
```

---

# 13. The Most Important Advanced Graph Patterns

These patterns naturally arise from Phase 14 problems.

## Pattern 1 — SCC Decomposition + Condensation DAG

When directed graph has mutual reachability clusters or circular dependencies.

```text
Signal words:
  "strongly connected"
  "mutual reachability"
  "circular dependency in directed graph"
  "shrink cycles to nodes"
```

```text
Steps:
1. Run Kosaraju or Tarjan
2. Build condensation DAG
3. Topo sort condensation OR answer per-SCC queries
```

Used for:

- Number of SCCs
- 2-SAT implication graph
- Minimum additions to make graph strongly connected (variant)

---

## Pattern 2 — Low-Link DFS (Bridges / Articulation Points)

When removing one edge or vertex disconnects the graph.

```text
Signal words:
  "critical connection"
  "critical router"
  "cut edge"
  "cut vertex"
  "redundant link"
```

```cpp
// Bridge condition on tree edge u-v:
if (low[v] > dfs[u])  // edge u-v is bridge

// Articulation (non-root):
if (low[v] >= dfs[u])  // u is articulation point
```

Used for:

- Critical Connections (LeetCode 1192)
- Network reliability analysis
- Tree edge vs back edge classification

---

## Pattern 3 — Max Flow via Augmenting Paths

When maximizing throughput from source to sink with capacity limits.

```text
Signal words:
  "maximum flow"
  "capacity"
  "bottleneck"
  "source and sink"
  "minimize cut"
```

```text
Residual graph:
  forward edge capacity = remaining
  backward edge allows flow cancellation
```

Used for:

- Basic max flow problems
- Bipartite matching reduction
- Project selection (min cut)

---

## Pattern 4 — Edmonds-Karp Shortest Augmenting Path

When Ford-Fulkerson might be slow due to large capacities.

```text
Always BFS in residual graph
Polynomial O(V × E²) bound
```

Prefer over naive DFS augmenting path when capacities are not small integers.

---

## Pattern 5 — Min-Cut Extraction

After max flow, identify minimum s-t cut.

```text
BFS/DFS from s in residual graph
Reachable set = S side of min cut
Edges from S to T with original capacity used = cut edges
```

Applications:

- Which edges to remove to disconnect s from t cheapest?
- Bipartite minimum vertex cover (Kőnig)

---

## Pattern 6 — Bipartite Matching via Flow or DFS

When assigning one-to-one from set L to set R.

```text
Signal words:
  "assign workers to jobs"
  "maximum matching"
  "pair up"
  "no shared endpoints"
```

Two approaches:

```text
Flow:     s → L (cap 1) → R (cap 1) → t
DFS:      Kuhn augmenting path per left node
```

Used for:

- Minimum path cover in DAG
- Board placement
- Airline seat assignment style problems

---

## Pattern 7 — Binary Lifting for LCA / K-th Ancestor

When many ancestry queries on a static tree.

```text
Precompute up[k][v] for 2^k jumps
Query LCA in O(log n)
Query k-th ancestor in O(log n)
```

```text
Distance(u,v) = depth[u] + depth[v] - 2*depth[lca(u,v)]
```

Used for:

- LCA queries
- Jump pointers on functional graphs (variant)
- Tree path aggregation with HLD companion

---

## Pattern 8 — Euler Tour + RMQ for LCA

Alternative LCA with O(1) query after O(n log n) preprocess.

```text
DFS tour records node entry times
LCA = node with min entry time in tour segment [in[u], in[v]]
```

Use when:

- Many LCA queries
- Already using Euler tour for other tree problems

---

## Pattern 9 — 2-SAT Implication Graph + SCC

When boolean clauses have at most two literals.

```text
Clause (a OR b)  →  edges ¬a→b and ¬b→a
Satisfiable iff no x and ¬x in same SCC
Assign in reverse topological order of SCC DAG
```

Used for:

- Scheduling with OR constraints
- Game outcome assignments
- Grid parity / constraint puzzles

---

## Pattern 10 — Euler Path/Circuit Degree Check + Hierholzer

When traverse every edge exactly once.

```text
Check degree conditions first (O(V))
Then Hierholzer construction (O(E))
```

```text
Do NOT confuse with Hamilton path (vertices once)
```

Used for:

- Reconstructing sequences (De Bruijn)
- Route coverage
- Valid stick figure drawing

---

## Pattern 11 — Heavy-Light Decomposition Path Query

When many path sum/max/update queries on a tree.

```text
Decompose into heavy chains
Segment tree on each chain's linearized positions
Walk up at most O(log n) chains per query
```

Combine with:

- Segment tree for range aggregate
- Lazy propagation for updates

---

## Pattern 12 — SCC + Topological Order on Condensation

When problem needs order of "blocks" in directed graph.

```text
1. Find SCCs
2. Build meta-graph (always DAG)
3. Topo sort meta-graph
4. Process components in that order
```

Used for:

- 2-SAT assignment
- Dependency resolution with cycles (handle per SCC)

---

# 14. Pattern Composition

Advanced problems often combine patterns.

```text
2-SAT
  → build implication graph
  → Tarjan SCC
  → check x and ¬x same component
  → assign via reverse topo on condensation

Bipartite min vertex cover
  → max matching (flow or DFS)
  → Kőnig: cover = matched left unmatched + matched right endpoints

Tree path sum queries
  → LCA to split path
  → HLD or Euler tour + segment tree

Critical infrastructure
  → bridges for edges
  → articulation points for nodes
  → may combine with Union-Find for counting components after removal

Project selection (profit/loss nodes)
  → build flow network with INF edges
  → max flow / min cut
  → selected projects = S side of cut
```

Notice:

You're no longer learning isolated algorithms.

You're **composing structural decomposition + optimization + tree machinery**.

---

# 15. Pattern Recognition

When you see:

```text
Directed graph, mutual reachability, cycle groups
      ↓
SCC (Kosaraju / Tarjan)
```

When you see:

```text
Remove one edge disconnects
      ↓
Bridge (low-link DFS)
```

When you see:

```text
Remove one node disconnects
      ↓
Articulation point (low-link DFS)
```

When you see:

```text
Source, sink, edge capacities, maximize throughput
      ↓
Max flow (Edmonds-Karp / Dinic)
```

When you see:

```text
After max flow, minimum bottleneck cut
      ↓
Min-cut from residual reachability
```

When you see:

```text
Two disjoint sets, pair without sharing
      ↓
Bipartite matching (flow or Kuhn DFS)
```

When you see:

```text
Tree, many LCA or ancestor queries
      ↓
Binary lifting OR Euler tour + RMQ
```

When you see:

```text
Boolean OR clauses with two literals max
      ↓
2-SAT implication graph + SCC
```

When you see:

```text
Use every edge exactly once in a trail
      ↓
Euler path/circuit (degree check + Hierholzer)
```

When you see:

```text
Many path queries on tree (sum, max, update)
      ↓
HLD + segment tree
```

When you see:

```text
Directed dependencies with cycles, need block order
      ↓
SCC + condensation topo
```

---

# 16. Edge Cases — Extremely Important

Always test mentally:

## SCC

```text
Single vertex, no edges        → one SCC
All vertices isolated          → n SCCs
Entire graph one SCC         → one SCC (strongly connected)
Self-loops                     → vertex in its own SCC handling
Disconnected directed graph    → multiple SCCs, process all starts
```

## Bridges / Articulation

```text
Single edge 0-1                → bridge; both articulation if tree
Cycle only                     → no bridges, no articulation (except tree cases)
Disconnected                   → run DFS from each component
Multiple edges between u,v     → handle carefully in undirected (not simple bridge)
Root with one child            → root is NOT articulation
```

## Network Flow

```text
No path s to t                 → flow = 0
Infinite capacity edges        → use large sentinel, not true INF in residual arithmetic
Multiple edges u→v             → sum capacities or single edge with total cap
Vertex capacities              → split vertex into in-node and out-node
Disconnected source/sink       → flow 0
Integer overflow               → use long long for flow value
```

## Bipartite Matching

```text
No edges                       → matching 0
Perfect matching possible?     → |matching| = min(|L|,|R|)
Non-bipartite graph            → cannot use bipartite matching directly (general matching harder)
Multiple edges same pair       → still works
```

## LCA

```text
u == v                         → LCA is u
One is ancestor of other       → LCA is the ancestor
Root queries                   → root has no parent; up[0][root] = -1
Forest (multiple roots)        → preprocess each tree or add super-root
```

## 2-SAT

```text
Empty clause                   → unsatisfiable
(x OR x)                       → forces x true
(x OR NOT x)                   → always satisfiable for that variable
All variables forced           → check SCC consistency
```

## Euler

```text
No edges                       → empty circuit or trivial
Disconnected but degrees OK    → must be connected for non-zero edges
Single vertex, no edges        → trivial Euler circuit
Directed vs undirected rules   → different degree conditions
```

## HLD

```text
Tree vs forest                 → handle multiple roots or super-root
Path query u to v              → split at LCA into two upward walks
Updates on edges vs nodes      → decide what segment tree stores
```

---

# 17. Important Implementation Skill: Trace the Algorithm

For advanced graph algorithms, **trace on small examples**.

## SCC Trace (Tarjan)

```text
Graph: 0→1, 1→2, 2→0, 2→3

DFS from 0:
  visit 0 (dfs=0, low=0)
  visit 1 (dfs=1, low=1)
  visit 2 (dfs=2, low=2)
  back to 0 (low[2]=0)
  visit 3 (dfs=3, low=3)
  pop SCC {3}
  pop SCC {0,1,2}
```

## Bridge Trace

```text
0-1-2 chain:

dfs[0]=0, dfs[1]=1, low[1]=1, dfs[2]=2, low[2]=2
Edge 1-2: low[2] > dfs[1]? 2 > 1 → bridge
```

## Flow Trace

```text
cap S→A=10, A→T=5

Augment path S-A-T with flow 5
Residual: S→A cap 5, A→T cap 0, backward A→S cap 5, T→A cap 5
Second augment: 5 more → total 10 but A→T limits to 5 total
```

Drawing prevents:

```text
"Forgot backward edges in residual"
"Confused low with dfs"
"Assigned SCC before popping stack"
```

---

# 18. Problems to Practice

Choose representative problems per concept.

### Strongly Connected Components

- Count SCCs
- Condensation graph construction
- Minimum edges to make graph strongly connected (concept)
- 2-SAT template problems

### Bridges and Articulation Points

- Critical Connections (LeetCode 1192)
- Articulation Points in Graph (classic)
- Network reliability counting

### Network Flow

- Maximum Flow template
- Minimum cut extraction
- Flow with vertex capacity (split node)
- Project selection (profit/cost)

### Bipartite Matching

- Assign jobs to workers
- Minimum path cover in DAG
- Grid matching (board problems)
- Bipartite matching via Hopcroft-Karp (larger inputs)

### LCA

- LCA of binary tree (LeetCode 236)
- Distance between nodes in tree
- K-th ancestor query
- LCA + binary lifting template

### 2-SAT

- Two-satisfiability template
- Scheduling with OR constraints
- Party invitation (classic)

### Euler Path/Circuit

- Reconstruct itinerary (variant)
- Valid arrangement of dominoes
- De Bruijn sequence construction (concept)

### Heavy-Light Decomposition

- Path sum queries on tree
- Path max queries
- Combined update + query on tree paths

### Composition

- 2-SAT (implication + SCC + assignment)
- Min vertex cover in bipartite (matching + Kőnig)
- Tree path queries (HLD + segment tree + LCA)

That is enough to understand major advanced graph patterns deeply.

---

# 19. For EVERY Advanced Graph Problem

Use the same learning process from Arrays, Linked Lists, Hash Tables, and Graphs.

Ask:

**1. What graph type and what structural question?**

```text
Directed? Undirected? Tree? Bipartite?
Flow network? Implication graph?
Question: SCC, cut, flow, matching, LCA, satisfiability, Euler?
```

**2. What is the problem really asking?**

```text
Decompose into blocks?
Find critical edges/vertices?
Maximize flow or matching?
Answer ancestry or path queries?
Satisfy boolean constraints?
Traverse all edges once?
```

**3. What pattern fits?**

```text
SCC / low-link / max flow / matching / LCA / 2-SAT / Euler / HLD
```

**4. What representation?**

```text
Adjacency list (most cases)
Residual capacity matrix or adjacency with caps
2n nodes for 2-SAT
Tree parent/depth arrays for LCA
Chain arrays for HLD
```

**5. What state per vertex/edge?**

```text
dfs, low, onStack (Tarjan)
parent, matchR (matching)
up[k][v] (binary lifting)
in[], tour[] (Euler LCA)
chain head, pos (HLD)
```

**6. What invariant guarantees correctness?**

```text
SCC: low[u]==dfs[u] ⟺ u is root of SCC subtree in DFS
Bridge: low[v]>dfs[u] ⟺ no back edge from v-subtree to u
Flow: augmenting path ⟺ not maximum yet (max-flow min-cut)
Matching: augmenting path increases matching size
LCA: binary lifting jumps preserve ancestor relation
2-SAT: same SCC for x and ¬x ⟺ unsatisfiable
Euler: degree parity ⟺ existence of trail
```

**7. Complexity?**

```text
O(V+E) for SCC, bridges, Euler
O(V×E²) Edmonds-Karp
O(log n) per LCA query
O(log² n) per HLD path query
```

**8. Edge cases?**

```text
Disconnected, single node, no edges
Overflow, large capacities
u==v, root cases
Non-bipartite mistaken for bipartite
```

**9. Trace small example?**

Always.

**10. Is it disguised?**

```text
Clauses → 2-SAT graph
Assignment → bipartite matching
Dependencies with cycles → SCC
Road coverage → Euler
Tree queries → LCA / HLD
Pipeline capacity → max flow
```

---

# 20. Compare Multiple Approaches

## SCC: Kosaraju vs Tarjan

### Kosaraju

```text
Two passes, transpose graph
Very intuitive
Time: O(V + E)
Space: O(V + E) for reverse graph
```

### Tarjan

```text
One DFS, low-link
No transpose graph
Time: O(V + E)
Space: O(V)
```

Ask:

- Memory for reverse graph acceptable?
- Prefer single-pass elegance?
- Already using low-link for bridges? → Tarjan SCC is natural sibling

---

## LCA: Binary Lifting vs Euler + RMQ

### Binary Lifting

```text
Query: O(log n)
Preprocess: O(n log n)
Also gives k-th ancestor
Easier to implement
```

### Euler Tour + RMQ

```text
Query: O(1) with sparse table
Preprocess: O(n log n)
Best for many LCA-only queries
Tour array useful for other tree problems
```

Ask:

- Need k-th ancestor? → binary lifting
- Only LCA, huge query count? → Euler + RMQ
- Already building Euler tour? → combine

---

## Max Flow: Ford-Fulkerson DFS vs Edmonds-Karp vs Dinic

### Ford-Fulkerson (DFS paths)

```text
Simple
Bad if capacities large: O(E × max_flow)
```

### Edmonds-Karp (BFS paths)

```text
O(V × E²) polynomial
Good default for moderate graphs
```

### Dinic

```text
O(V² × E)
Better on many practical networks
More implementation effort
```

Ask:

- Capacity values huge? → avoid naive Ford-Fulkerson
- Graph size moderate? → Edmonds-Karp often enough
- Time limit strict on flow? → Dinic

---

## Matching: DFS Kuhn vs Hopcroft-Karp vs Flow

### Kuhn DFS

```text
O(V × E)
Simple for small bipartite graphs
```

### Hopcroft-Karp

```text
O(E × √V)
Large bipartite matching
```

### Max Flow

```text
General reduction
Useful when matching is part of larger flow network
Unit capacity edges
```

---

# 21. One Especially Important Mental Shift

When beginners see:

```text
edges = [[0,1],[1,2],[2,0]]
```

they think only about cycle detection from Phase 8.

As you improve, ask:

> Is this a directed mutual reachability question (SCC), a cut structure question (bridge/AP), or a satisfiability constraint (2-SAT)?

When you see:

```text
source, sink, capacities
```

think:

> Residual graph + augmenting paths, not shortest path (unless min-cost flow variant).

When you see:

```text
tree with 100000 queries
```

think:

> Preprocess — LCA, HLD, Euler tour — not BFS per query.

When you see:

```text
(x OR y) clauses
```

think:

> Implication edges, not brute force 2^n.

Advanced graph problems are **recognizing which structural or optimization template the story hides**.

---

# 22. Advanced Graphs vs Phase 8 Graphs

```text
PHASE 8                           PHASE 14
────────────────────────────────────────────────────────
Connected components              Strongly connected components
BFS shortest path                 Max flow (different optimization)
Bipartite 2-color check           Bipartite maximum matching
DFS tree edges                    Bridges / articulation (low-link)
Topo sort on DAG                  2-SAT via SCC on implication graph
Union-Find static connectivity    Flow/cut global optimization
Single-source shortest path       Tree path queries (LCA, HLD)
```

Phase 8 builds the **traversal and basic connectivity** foundation.

Phase 14 builds **decomposition, optimization, and specialized tree/constraint machinery**.

---

# 23. Implement Advanced Algorithms Yourself

Don't skip implementation.

## Version 1 — Kosaraju SCC

Transpose graph + two DFS passes.

## Version 2 — Tarjan SCC

Single DFS, stack, low-link pop.

## Version 3 — Bridges and Articulation

Low-link on undirected graph; separate bridge vs AP conditions.

## Version 4 — Edmonds-Karp

BFS on residual, augment flow.

## Version 5 — Bipartite Matching

Kuhn DFS or flow reduction.

## Version 6 — Binary Lifting LCA

Preprocess `up`, answer queries.

## Version 7 — 2-SAT

Build implication graph, Tarjan, assign.

## Version 8 — Hierholzer Euler

Degree check + circuit construction.

## Version 9 — HLD Skeleton

Size DFS, heavy child, chain decomposition + segment tree hook.

If you can implement these from scratch and explain each invariant, advanced graph interviews become manageable.

---

# 24. Real-World Applications

```text
SCC                    → dead-code detection, web crawling clusters, circuit analysis
Bridges/AP             → network reliability, road critical links, telecom backbone
Max flow / min cut     → supply chain, airline scheduling, image segmentation
Bipartite matching     → job assignment, roommate matching, ad placement
LCA                    → versioning trees (git merge base concept), org hierarchies
2-SAT                  → hardware verification, scheduling, constraint puzzles
Euler circuits         → DNA sequencing, route inspection, CAD pen plotting
HLD                    → competitive programming tree paths; static tree analytics
```

Advanced graph algorithms power **infrastructure, allocation, and constraint systems** — not just contest puzzles.

---

# 25. Common Mistakes to Avoid

```text
1. Running undirected bridge algorithm on directed graph without adapting
   → wrong cut edges

2. Forgetting backward edges in residual graph
   → flow stuck below maximum

3. Using Dijkstra instead of max flow for matching
   → wrong problem model

4. Confusing Euler (edges once) with Hamilton (vertices once)
   → wrong existence check

5. 2-SAT with only one implication per clause
   → must add BOTH ¬a→b and ¬b→a

6. LCA on forest without super-root or multi-root handling
   → undefined parent

7. Tarjan SCC: marking off-stack incorrectly
   → wrong components

8. Kosaraju: wrong DFS order (must finish-time / reverse finish order)
   → components merged wrong

9. Hopcroft-Karp when graph is not bipartite
   → incorrect matching model

10. HLD without segment tree on chains
    → decomposition alone doesn't answer path sums
```

---

# Your Complete Advanced Graph Algorithms Checklist

```text
ADVANCED GRAPH ALGORITHMS (Phase 14)
│
├── Prerequisites (Phase 8)
│   ├── BFS / DFS
│   ├── Adjacency list
│   ├── Topological sort
│   ├── Union-Find
│   ├── Bipartite 2-coloring
│   └── See graphs/notes.md
│
├── Strongly Connected Components
│   ├── Definition (directed mutual reachability)
│   ├── Kosaraju (two-pass, transpose)
│   ├── Tarjan (low-link, one pass)
│   ├── Condensation DAG
│   └── O(V + E)
│
├── Bridges and Articulation Points
│   ├── Bridge / cut edge definition
│   ├── Articulation point definition
│   ├── Low-link DFS (undirected)
│   ├── Bridge: low[v] > dfs[u]
│   ├── AP: root children ≥ 2 or low[v] ≥ dfs[u]
│   └── O(V + E)
│
├── Network Flow
│   ├── Flow conservation, capacity constraints
│   ├── Residual graph
│   ├── Ford-Fulkerson (augmenting paths)
│   ├── Edmonds-Karp (BFS augmenting)
│   ├── Max-flow min-cut theorem
│   ├── Min-cut extraction
│   └── Dinic (bonus)
│
├── Bipartite Matching
│   ├── Definition
│   ├── Reduction to max flow
│   ├── Kuhn DFS augmenting O(V×E)
│   ├── Hopcroft-Karp O(E√V)
│   └── Kőnig theorem (min cover ↔ max matching)
│
├── Lowest Common Ancestor
│   ├── Definition
│   ├── Binary lifting (O(log n) query)
│   ├── Euler tour + RMQ (O(1) query)
│   ├── K-th ancestor
│   └── Tree distance via LCA
│
├── 2-SAT
│   ├── 2-CNF clauses
│   ├── Implication graph (2n nodes)
│   ├── SCC check x vs ¬x
│   ├── Assignment from reverse topo
│   └── Applications
│
├── Euler Path / Circuit
│   ├── Edge traversal vs Hamilton
│   ├── Degree parity conditions
│   ├── Directed vs undirected rules
│   ├── Hierholzer algorithm
│   └── O(E)
│
├── Heavy-Light Decomposition (intro)
│   ├── Heavy child / light edges
│   ├── Chain decomposition
│   ├── O(log n) chains per path
│   ├── Segment tree on chains
│   └── O(log² n) path queries
│
├── Core Patterns (12)
│   ├── SCC + condensation
│   ├── Low-link bridges
│   ├── Low-link articulation
│   ├── Max flow augmenting
│   ├── Edmonds-Karp
│   ├── Min-cut extraction
│   ├── Bipartite matching
│   ├── Binary lifting LCA
│   ├── Euler tour LCA
│   ├── 2-SAT implication SCC
│   ├── Euler degree + Hierholzer
│   └── HLD path query
│
├── Pattern Composition
│   ├── 2-SAT full pipeline
│   ├── Min vertex cover via matching
│   ├── Tree path HLD + segment tree
│   └── Project selection via cut
│
├── Pattern Recognition
│   └── Signal words → algorithm map
│
├── Edge Cases
│   ├── SCC: isolated, single SCC, self-loops
│   ├── Bridges: chains, cycles, multi-edges
│   ├── Flow: overflow, no path, vertex caps
│   ├── Matching: empty, non-bipartite
│   ├── LCA: u=v, forest
│   ├── 2-SAT: forced literals
│   └── Euler: disconnected, parity
│
├── Implement Yourself
│   ├── Kosaraju
│   ├── Tarjan SCC
│   ├── Bridges / AP
│   ├── Edmonds-Karp
│   ├── Kuhn matching
│   ├── Binary lifting LCA
│   ├── 2-SAT
│   ├── Hierholzer
│   └── HLD skeleton
│
├── For EVERY Problem
│   ├── Graph type + question
│   ├── Pattern selection
│   ├── Representation
│   ├── Per-vertex state
│   ├── Correctness invariant
│   ├── Complexity
│   ├── Edge cases
│   ├── Trace example
│   ├── Disguised graph model
│   └── Compare alternatives
│
└── Cross-References
    ├── map.md Phase 14
    ├── graphs/notes.md Phase 8 foundation
    └── Phase 12 (segment tree) for HLD
```

For your **advanced graph pass (Phase 14)**, master SCC, cuts, flow, matching, LCA, 2-SAT, Euler, and HLD intro. Once you can **decompose directed graphs into SCCs, identify bridges, run Edmonds-Karp, solve bipartite matching, answer LCA queries, reduce 2-SAT to SCC, construct Euler tours, and sketch HLD path queries**, you have the advanced graph toolkit complete.

Next blocks in [map.md](../map.md) may include deeper string algorithms, advanced data structures integration, or domain-specific optimizations — but Phase 14 closes the core **graph algorithm** arc from traversal to structural decomposition and network optimization.

---
