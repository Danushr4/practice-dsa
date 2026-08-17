# String Algorithms

Phase 13 from your roadmap (`map.md`).

After Arrays & Strings fundamentals, Hash Tables, Trees (including Tries), and Searching & Sorting, you are ready for **specialized string matching and indexing algorithms**.

These algorithms exist because naive string operations often hide expensive work:

```text
"Find pattern P in text T"
"Find all occurrences"
"Find longest repeated substring"
"Match many patterns at once"
"Find all palindromic substrings efficiently"
```

A single `str.find()` call in a library may use Boyer-Moore, Two-Way, or SIMD — but you should understand **what is happening underneath** and **when to reach for which technique**.

---

# 1. String Matching Fundamentals

## What Is the Problem?

Given:

```text
Text:    T = "ababcababa"
Pattern: P = "aba"
```

Find all starting indices `i` where:

```text
T[i .. i + |P| - 1] == P
```

Answer:

```text
indices: 0, 5, 7
```

Visually:

```text
T: a b a b c a b a b a
   ↑       ↑     ↑
   0       5     7

P: a b a
```

---

## Core Vocabulary

Understand these terms before any algorithm:

```text
|s|           length of string s
substring     contiguous block T[i..j]
subsequence   characters in order, not necessarily contiguous
prefix        T[0..i]
suffix        T[i..n-1]
alphabet Σ    set of possible characters (e.g. lowercase a-z)
occurrence    one match position
overlap       matches that share characters
```

Example:

```text
T = "banana"

substring:  "nan"     (indices 2..4)
subsequence: "bnn"    (indices 0, 2, 5 — skip letters)
prefix:      "ban"
suffix:      "ana"
```

---

## The Naive Question

Why not just compare character by character everywhere?

Because for text length `n` and pattern length `m`:

```text
Brute force tries O(n - m + 1) positions
Each comparison costs up to O(m)

Worst case: O(n * m)
```

If:

```text
T = "aaaaaa...a"   (length n)
P = "aaa...ab"     (length m, last char different)
```

Almost every position compares nearly all `m` characters.

That worst case is exactly what smarter algorithms avoid.

---

## Comparison-Based vs Hash-Based vs Index-Based

Three families:

```text
Comparison-based
├── Brute force
├── KMP
├── Z-algorithm
└── Boyer-Moore (good to know; not deep focus here)

Hash-based
└── Rabin-Karp (rolling hash)

Index-based
├── Trie
├── Suffix Array
├── Suffix Tree
└── Suffix Automaton
```

Also:

```text
Specialized
├── Manacher's (palindromes)
└── Aho-Corasick (multi-pattern)
```

Don't memorize names first.

Memorize **what information each algorithm reuses** when a mismatch happens.

---

## Complexity Landscape

| Algorithm | Preprocess | Search | Space | Notes |
| --------- | ---------- | ------ | ----- | ----- |
| Brute force | O(1) | O(n·m) worst | O(1) | Baseline |
| Rabin-Karp | O(m) | O(n+m) average*, O(n·m) worst | O(1) | *with good hash |
| KMP | O(m) | O(n+m) | O(m) | Guaranteed linear |
| Z-algorithm | O(n) | O(n+m) via Z + scan | O(n) | Good for prefix structure |
| Trie | O(total pattern len) | O(n·L) per char walk | O(total chars) | Multi-pattern prefix |
| Suffix Array | O(n log n) or O(n) | O(m log n) with binary search | O(n) | Static text indexing |
| Suffix Tree | O(n) | O(m) | O(n) | Powerful but heavy |
| Manacher's | O(n) | — | O(n) | All palindromes |
| Aho-Corasick | O(total pattern len) | O(n + matches) | O(states) | Multi-pattern full scan |

The right choice depends on:

```text
One pattern or many?
Static text or streaming?
Need all occurrences or first?
Need prefix/suffix structure?
Interview or production?
```

---

# 2. Brute Force String Matching

Start here. Implement it cleanly.

## Algorithm

For each start position `i` in `T`:

```text
Compare T[i + k] with P[k] for k = 0 .. m-1
Stop early on mismatch
If all match → record i
```

```cpp
vector<int> bruteForceSearch(const string& T, const string& P) {
    int n = T.size(), m = P.size();
    vector<int> matches;
    if (m == 0 || m > n) return matches;

    for (int i = 0; i <= n - m; i++) {
        int k = 0;
        while (k < m && T[i + k] == P[k]) k++;
        if (k == m) matches.push_back(i);
    }
    return matches;
}
```

---

## Walkthrough

```text
T: a b a b c a b a b a
P: a b a

i=0: a=a, b=b, a=a → match at 0
i=1: b≠a → fail fast
i=2: a=a, b≠b → fail
i=3: b≠a → fail
i=4: c≠a → fail
i=5: a=a, b=b, a=a → match at 5
i=6: b≠a → fail
i=7: a=a, b=b, a=a → match at 7
```

---

## Why Brute Force Still Matters

1. It defines correctness.
2. It is fine for tiny inputs.
3. Every optimized algorithm is "brute force with skipped work."
4. Interviews sometimes expect you to start here, then optimize.

---

## When Brute Force Is Enough

```text
n and m both small (e.g. < 100)
Only one quick check needed
Pattern appears rarely and early mismatch is common
You already know |P| is 1
```

---

## Brute Force Limitations

```text
Repeating characters destroy performance
Overlapping matches re-compare shared prefix characters
No reuse of partial match information
```

That "partial match information" is exactly what KMP and Z exploit.

---

# 3. Rabin-Karp — Rolling Hash

## Core Idea

Instead of comparing `m` characters at every position, compare **hash values** in O(1) amortized time per shift.

If hashes equal → verify with direct comparison (avoid false positives).

```text
hash("aba") at position i
shift window by 1
update hash in O(1) using rolling hash
```

---

## Polynomial Rolling Hash

Common form:

```text
H(s) = (s[0]*B^(m-1) + s[1]*B^(m-2) + ... + s[m-1]*B^0) mod M
```

Choose:

```text
B = base (often 256 or 31)
M = large prime (or use 64-bit and rely on overflow)
```

Example with small numbers:

```text
s = "aba"
B = 10, m = 3

H = 'a'*100 + 'b'*10 + 'a'*1   (conceptually)
```

When window slides:

```text
remove leftmost char contribution
multiply by B
add new right char
```

---

## Implementation Sketch

```cpp
vector<int> rabinKarp(const string& T, const string& P) {
    int n = T.size(), m = P.size();
    vector<int> matches;
    if (m == 0 || m > n) return matches;

    const long long B = 256;
    const long long M = 1000000007;

    long long ph = 0, th = 0, powB = 1;
    for (int i = 0; i < m; i++) {
        ph = (ph * B + P[i]) % M;
        th = (th * B + T[i]) % M;
        if (i < m - 1) powB = (powB * B) % M;
    }

    for (int i = 0; i <= n - m; i++) {
        if (ph == th) {
            if (T.compare(i, m, P) == 0)
                matches.push_back(i);
        }
        if (i < n - m) {
            th = (th - T[i] * powB % M + M) % M;
            th = (th * B + T[i + m]) % M;
        }
    }
    return matches;
}
```

Always verify on hash collision.

---

## Why Rabin-Karp Is Powerful Beyond Single Pattern

It extends naturally to:

```text
Find any of k patterns
Two-string equality checks at scale
Plagiarism detection (hash chunks)
Rolling hash on arrays (not just strings)
```

For multiple patterns, hash all patterns into a set, then scan text once.

---

## Rabin-Karp Trade-offs

Pros:

```text
Simple rolling update
Good average case
Extends to multi-pattern with one pass
Useful when alphabet is large
```

Cons:

```text
Hash collisions require verification
Worst case still O(n·m) if many spurious matches
Modulo arithmetic must be careful (negative values in C++)
Not guaranteed linear like KMP
```

---

## Collision Awareness

Two different substrings can have equal hash mod M.

That is not a bug if you verify.

For competitive programming, sometimes double-hash (two moduli) reduces collision risk.

For interviews, mention:

```text
Equal hash → possible match → confirm with direct compare
```

---

# 4. KMP — Knuth-Morris-Pratt

## The Key Insight

When a mismatch happens, brute force restarts comparison from scratch.

KMP asks:

```text
How much of the already-matched prefix is also a suffix of that prefix?
```

Then shift the pattern intelligently instead of moving text pointer back.

---

## LPS Array (Longest Proper Prefix which is also Suffix)

Also called:

```text
failure function
prefix function (related but not identical in all contexts)
pi array in some textbooks
```

For pattern `P`, `lps[k]` = length of longest **proper** prefix of `P[0..k]` that is also a suffix of `P[0..k]`.

```text
P = "ababaca"

index:  0 1 2 3 4 5 6
char:   a b a b a c a
lps:    0 0 1 2 3 0 1
```

Explanation:

```text
"ab"      → proper prefix "a" equals suffix "a" → lps=1
"aba"     → "a" → lps=1
"abab"    → "ab" → lps=2
"ababa"   → "aba" → lps=3
"ababac"  → no nontrivial match → 0
"ababaca" → "a" → lps=1
```

---

## Building LPS

Two-pointer approach:

```text
len = length of current longest border
i scans pattern left to right
if P[i] == P[len] → extend border
else if len > 0 → len = lps[len-1] (fallback)
else → lps[i] = 0
```

```cpp
vector<int> buildLPS(const string& P) {
    int m = P.size();
    vector<int> lps(m, 0);
    int len = 0;
    for (int i = 1; i < m; ) {
        if (P[i] == P[len]) {
            lps[i++] = ++len;
        } else if (len > 0) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }
    return lps;
}
```

Draw this on paper until it feels mechanical.

---

## KMP Search

Maintain:

```text
i → index in text T
j → index in pattern P
```

```text
if T[i] == P[j] → advance both
else if j > 0 → j = lps[j-1]
else → i++
```

When `j == m`, found match at `i - m`, then `j = lps[j-1]` to find overlapping matches.

```cpp
vector<int> kmpSearch(const string& T, const string& P) {
    int n = T.size(), m = P.size();
    vector<int> matches;
    if (m == 0 || m > n) return matches;

    vector<int> lps = buildLPS(P);
    int j = 0;
    for (int i = 0; i < n; ) {
        if (T[i] == P[j]) {
            i++; j++;
            if (j == m) {
                matches.push_back(i - m);
                j = lps[j - 1];
            }
        } else if (j > 0) {
            j = lps[j - 1];
        } else {
            i++;
        }
    }
    return matches;
}
```

---

## KMP Walkthrough (Conceptual)

```text
T: a b a b a b a c a
P: a b a b a c

At i=5, matched "abab", next T[5]='a' but P[4]='a' already consumed...
Better to trace on paper with lps = [0,0,1,2,3,0]
```

The invariant:

```text
After fallback, P[0..j-1] matches T[i-j .. i-1]
No need to recheck those characters
```

That is why total time is O(n + m).

---

## KMP Complexity

```text
Build LPS: O(m)
Search:    O(n)
Space:     O(m)
```

Each text character advances `i` at most once.

Each fallback reduces `j` using already computed border structure.

---

## KMP vs Brute Force — Mental Model

```text
Brute force:
  mismatch at position i → restart pattern at i+1

KMP:
  mismatch at (i, j) → keep i, move j to lps[j-1]
```

You never move `i` backward.

---

## Common KMP Mistakes

```text
Off-by-one when recording match index (i - m, not i)
Forgetting to continue after match (overlapping occurrences)
Building lps with wrong definition (proper prefix vs any prefix)
Confusing lps[i] with lps[i-1] during search fallback
Using KMP when pattern is single character (overkill but fine)
```

---

# 5. Z-Algorithm

## What Z Gives You

For string `S`, array `Z` where:

```text
Z[i] = length of longest substring starting at i
       that matches a prefix of S
```

Special case: `Z[0] = 0` (or undefined / n depending on convention).

```text
S = "aabcaabxaaz"

index: 0 1 2 3 4 5 6 7 8 9 10
Z:     - 1 0 0 3 1 0 0 2 1 0
```

At i=4, "aab" matches prefix "aab" → Z[4]=3.

---

## Why Z Is Useful

```text
Pattern matching: compute Z on (P + "$" + T)
First occurrence of full P → some Z[i] == |P|
Prefix structure problems
Period / repetition detection
String compression insights
```

Pattern matching trick:

```text
combined = P + "#" + T

Find indices i where Z[i] == |P|
Match position in T = i - |P| - 1
```

Separator `#` must not appear in P or T (or use unused char).

---

## Z-Algorithm Construction (High Level)

Maintain window `[L, R]` = rightmost segment matching a prefix.

For each `i`:

```text
If i > R → naive expand from i
Else → Z[i] >= min(Z[i-L], R-i+1), maybe expand
Update R if new match extends further right
```

```cpp
vector<int> buildZ(const string& S) {
    int n = S.size();
    vector<int> Z(n, 0);
    int L = 0, R = 0;
    for (int i = 1; i < n; i++) {
        if (i <= R) Z[i] = min(R - i + 1, Z[i - L]);
        while (i + Z[i] < n && S[Z[i]] == S[i + Z[i]]) Z[i]++;
        if (i + Z[i] - 1 > R) {
            L = i;
            R = i + Z[i] - 1;
        }
    }
    return Z;
}
```

---

## Z vs KMP

Both achieve O(n + m) matching.

```text
KMP → failure function on pattern, stream through text
Z   → prefix similarity array on combined string
```

Many people find Z easier for "prefix of combined string" problems.

KMP feels more natural for classic "find P in T" streaming.

Learn both; choose based on problem shape.

---

## Z-Algorithm Applications

```text
Find all occurrences of P in T
Longest prefix that is also suffix at each position
Check if string is periodic
Compare internal repetitions
Some string compression / border problems
```

---

# 6. Trie Applications (Cross-Reference: Trees)

Full Trie deep dive lives in:

```text
trees/notes.md → Section 10. Trie (Prefix Tree)
```

Read that section for structure, node design, insert/search/delete, and trie vs hash table.

Here we focus on **string algorithm contexts** where tries appear.

---

## Why Tries Appear in String Algorithms

A trie indexes strings by **shared prefixes**.

```text
Dictionary:
  app
  apple
  apply
  apt

Trie shares "ap" path once
```

Operations:

```text
Insert word:     O(L)
Exact search:    O(L)
Prefix search:   O(L) + output size
```

Where `L` = key length.

---

## Trie Node (Reminder)

```cpp
struct TrieNode {
    TrieNode* children[26];  // or map<char, TrieNode*>
    bool isEnd;
    int wordCount;           // optional: prefix frequency
};
```

See `trees/notes.md` for array vs map children trade-offs.

---

## String Algorithm Problems Where Trie Wins

### 1. Autocomplete / Prefix Queries

```text
Input prefix "app"
Output all words in dictionary with that prefix
```

Trie DFS from prefix node.

---

### 2. Word Search II (Grid + Trie)

```text
Board of characters + word list
Find all words formable by adjacent paths
```

Pattern:

```text
Build trie from dictionary
DFS on grid
Prune when current path is not a trie prefix
Mark/end words when isEnd reached
```

This is **Trie + Backtracking** — covered in trees notes Pattern 9.

---

### 3. Replace Words

```text
Sentence + dictionary of roots
Replace each word with shortest matching root
```

Walk each word char-by-char in trie; stop at first `isEnd`.

---

### 4. Longest Word in Dictionary (Word by Word)

```text
Build trie
Only extend words where every prefix is also a valid word node
```

---

### 5. Maximum XOR With an Element From Array (Bit Trie)

Not strictly lowercase strings — but same structure on **binary trie** (0/1 edges).

Important: trie generalizes beyond characters.

---

### 6. Stream of Characters

```text
Queries: "did any suffix of stream end with word w?"
Reverse trie or store reversed words
```

---

## Trie vs Hash Set for Dictionary

| Need | Trie | Hash Set |
| ---- | ---- | -------- |
| Exact membership | O(L) | O(L) average |
| All words with prefix | O(L) + output | O(n · L) scan |
| Space | Often more overhead | Compact |
| Wildcard patterns | Natural with DFS | Awkward |

---

## When NOT to Use Trie

```text
Only exact lookups, no prefix queries → hash set may suffice
Very large alphabet, sparse keys → map-based trie can be heavy
Single pattern search in fixed text → KMP/Z/suffix structures better
```

---

# 7. Suffix Array Basics

## Motivation

Given static text `T`, preprocess so you can answer:

```text
Is pattern P a substring of T?
How many times does P appear?
What is the longest repeated substring?
What is the lexicographically smallest rotation?
```

Suffix array is a lighter alternative to suffix tree for many tasks.

---

## Definition

Suffix array `SA` is an array of indices sorting all suffixes of `T` lexicographically.

```text
T = "banana"

Suffixes:
0: banana
1: anana
2: nana
3: ana
4: na
5: a

Sorted order:
5: a
3: ana
1: anana
0: banana
2: nana
4: na

SA = [5, 3, 1, 0, 2, 4]
```

---

## Building Suffix Array

Naive:

```text
Generate all suffixes
Sort

O(n² log n) if comparing suffixes directly
```

Better:

```text
O(n log² n) doubling method
O(n log n) with efficient counting sort steps
O(n) skew / SA-IS (advanced; know it exists)
```

For learning, implement O(n log² n) version first.

---

## Pattern Matching with Suffix Array + Binary Search

Once `SA` built:

```text
Binary search on suffixes comparing P with T[SA[mid] ..]
O(m log n) per query
```

Two binary searches can find equal range → all occurrences.

---

## LCP Array (Longest Common Prefix)

`LCP[i]` = longest common prefix between suffix at `SA[i-1]` and `SA[i]`.

```text
Enables:
  Longest repeated substring
  Number of distinct substrings
  Pattern occurrence clustering
```

Often built with Kasai's algorithm in O(n) given SA.

---

## Suffix Array Use Cases

```text
Bioinformatics sequence search (with FM-index variants)
Competitive programming substring problems on static string
Building block for suffix tree (some constructions)
Lexicographic problems on cyclic strings
```

---

## Suffix Array Limitations

```text
Static text — updates are expensive
Pattern search O(m log n), not O(m) like suffix tree
LCP/RMQ structures add implementation complexity
O(n) construction is non-trivial
```

---

# 8. Suffix Tree / Automaton — Intro

## Suffix Tree (Conceptual)

Compact trie containing **all suffixes** of `T`.

```text
Every substring appears as path from root
Each internal path labeled with substring chunk
Total size O(n) for alphabet O(1) — surprising but true
```

Visually (simplified):

```text
T = "banana"

(root)
 ├── b → anana...
 ├── a → nana..., na..., a...
 └── n → ana..., a...
```

Edges store `(start, end)` indices into `T`, not full copied strings.

---

## What Suffix Tree Enables

```text
Find P in T:          O(m)
Longest repeated:     O(n)
Longest common:       O(n) with two strings concatenated
Count distinct subs:  O(n)
Many "all occurrences" queries efficiently
```

---

## Why You Usually Learn It Conceptually First

```text
Implementation is long (Ukkonen's algorithm)
Many edge cases in active point / suffix links
Interview rarely requires full code
Understanding what it represents matters more
```

Know:

```text
It is the "Swiss army knife" of string indexing
Production libraries use simpler structures or hybrid indexes
Suffix array + LCP often substitutes in contests
```

---

## Suffix Automaton (Brief)

Also called DAWG (Directed Acyclic Word Graph).

```text
States represent end positions of substrings
Transitions on characters
Also O(n) size, O(m) pattern search
Different construction, sometimes easier than suffix tree in contests
```

Properties:

```text
Minimal automaton recognizing all substrings of T
Each state has longest length / link to suffix link state
Powerful for counting distinct substrings, longest common substring variants
```

If suffix tree feels heavy, read suffix automaton next — many CP competitors prefer it.

---

## Suffix Tree vs Suffix Array vs Automaton

| Structure | Build | Space | Query P in T | Implementation |
| --------- | ----- | ----- | ------------ | -------------- |
| Suffix Array | O(n log n) typical | O(n) | O(m log n) | Moderate |
| Suffix Tree | O(n) | O(n) | O(m) | Hard |
| Suffix Automaton | O(n) | O(n) | O(m) | Medium-Hard |

---

# 9. Manacher's Algorithm — Palindromes

## Problem

Find all palindromic substrings or longest palindromic substring in O(n).

Naive:

```text
Check every center
Expand around center

O(n²) time
```

Manacher:

```text
O(n) time
O(n) space
```

---

## Key Idea — Exploit Symmetry

When scanning left to right, if palindrome centered at `C` with radius `R` is known, mirror position `i'` inside that palindrome may reuse palindrome length information.

Transform string to avoid even/odd center cases:

```text
"s = "aba"

Transformed: ^ # a # b # a # $
```

Every palindrome in transformed string has odd length with center at a `#` or character.

---

## Manacher Array

`P[i]` = radius (number of chars to left/right excluding center) of palindrome centered at `i`.

```text
For "aba" transformed, center at middle 'b' has P = 1 → palindrome "aba"
```

---

## Algorithm Sketch

```text
Maintain center C, right boundary R of rightmost palindrome
For each i:
  mirror = 2*C - i
  if i < R: P[i] = min(R - i, P[mirror])
  try expand while chars match
  if i + P[i] > R: update C, R
```

```cpp
string preprocess(const string& s) {
    string t = "^";
    for (char c : s) { t += '#'; t += c; }
    t += "#$";
    return t;
}

int manacher(const string& s) {
    string t = preprocess(s);
    int n = t.size();
    vector<int> P(n, 0);
    int C = 0, R = 0, maxLen = 0;
    for (int i = 1; i < n - 1; i++) {
        int mirror = 2 * C - i;
        if (i < R) P[i] = min(R - i, P[mirror]);
        while (t[i + 1 + P[i]] == t[i - 1 - P[i]]) P[i]++;
        if (i + P[i] > R) { C = i; R = i + P[i]; }
        maxLen = max(maxLen, P[i]);
    }
    return maxLen;  // radius in transformed string
}
```

Map back to original string indices carefully.

---

## Manacher Applications

```text
Longest Palindromic Substring
Count palindromic substrings (with adjustments)
Some DP optimizations on palindrome structure
```

---

## When Simpler Methods Suffice

```text
Expand around center O(n²) is fine for n ≤ 2000 in many contests
DP palindrome table O(n²) for "is s[i..j] palindrome?" queries
Manacher shines when O(n) required on large n
```

---

# 10. Aho-Corasick — Multi-Pattern Matching

## Problem

Given:

```text
Text T
Dictionary of k patterns P1, P2, ..., Pk
```

Find all occurrences of **any** pattern in T in one scan.

Naive:

```text
Run KMP k times → O(k · (n + m))
Or trie walk restarting each position → can degrade badly
```

Aho-Corasick:

```text
Build trie of patterns
Add failure links (like KMP lps, but on trie)
Scan T once in O(n + matches + alphabet work)
```

---

## Structure

```text
Trie nodes for all patterns
Each node has:
  children
  failure link (fallback if char not found)
  output list (patterns ending here)
```

Failure link of node = longest proper suffix of current prefix that is also a trie prefix.

---

## Construction Phases

### Phase 1 — Build Trie

Insert all patterns normally.

---

### Phase 2 — BFS Failure Links

```text
Root's failure = root
For depth 1 nodes, failure = root
For deeper nodes:
  follow parent's failure until child char exists or hit root
  set failure link
  merge output from failure target (dictionary suffix patterns)
```

---

### Phase 3 — Scan Text

```text
node = root
for each char c in T:
  while node has no c child and node != root:
      node = node.fail
  if c child exists: node = child
  report all patterns in node.output
```

---

## Walkthrough (Small Example)

Patterns:

```text
he
she
his
hers
```

Text:

```text
"ushers"
```

Trie finds "she", "he", "hers" at appropriate positions depending on output propagation.

Draw the trie + failure links on paper — this algorithm clicks visually.

---

## Complexity

```text
Build: O(total pattern length · alphabet) with array children
Scan:  O(n + number of matches + transitions)
Space: O(total pattern length)
```

---

## Aho-Corasick Applications

```text
Virus scanning / intrusion detection (multiple signatures)
Keyword filtering in logs
Bioinformatics motif search
Word break with large dictionary (combined with DP)
Search engine token matching (simplified view)
```

---

## Aho-Corasick vs Repeated KMP

```text
Few long patterns, small k → repeated KMP acceptable
Many patterns or large dictionary → Aho-Corasick
Need prefix dictionary while scanning stream → Aho-Corasick
```

---

# 11. The Most Important String Algorithm Patterns

These are the recurring **problem-solving patterns** beyond raw algorithm implementations.

---

## Pattern 1 — Single Pattern Matching (Linear Scan)

Signal:

```text
One pattern P, one text T
Need all or first occurrence
Guaranteed O(n + m) required
```

Reach for:

```text
KMP or Z-algorithm
```

```cpp
// Z trick
string combined = P + "#" + T;
vector<int> Z = buildZ(combined);
for (int i = P.size() + 1; i < combined.size(); i++)
    if (Z[i] == P.size()) { /* match at i - P.size() - 1 */ }
```

---

## Pattern 2 — Rolling Hash Window

Signal:

```text
Fixed-length substring comparisons while sliding
"Find repeated substring of length k"
"Compare hash of every window"
```

Reach for:

```text
Rabin-Karp rolling hash
Also appears in array problems (max avg subarray of len k variant with hash)
```

Watch modulo negatives in C++.

---

## Pattern 3 — Prefix Function / Border Reuse

Signal:

```text
Overlapping occurrences matter
Need period or repetition structure
"Shortest string s.t. s repeats to form T"
```

Reach for:

```text
KMP lps on T
or Z-array on T
```

Period length:

```text
n - lps[n-1]  (when n - lps[n-1] divides n)
```

---

## Pattern 4 — Z-Array on Combined String

Signal:

```text
Compare every suffix of T to prefix of something
Pattern + separator + text
"How long does suffix at i match prefix of S?"
```

Reach for:

```text
Z-algorithm
```

---

## Pattern 5 — Trie Prefix Pruning

Signal:

```text
Many words
Prefix must remain valid while building candidate
Grid word search
Autocomplete
```

Reach for:

```text
Trie (+ DFS/backtracking)
See trees/notes.md
```

---

## Pattern 6 — Multi-Pattern Dictionary Scan

Signal:

```text
Large dictionary
Scan one text for any of thousands of keywords
```

Reach for:

```text
Aho-Corasick
```

---

## Pattern 7 — Suffix Index on Static Text

Signal:

```text
Many queries on same unchanged T
Substring existence / ranking / repeated structure
n up to 1e5, m moderate
```

Reach for:

```text
Suffix array (+ binary search)
Suffix tree / automaton if O(m) per query needed
```

---

## Pattern 8 — Palindrome Structure (Manacher / Expand)

Signal:

```text
Longest palindromic substring
Count palindromes
Palindrome partitioning optimization
```

Reach for:

```text
Manacher O(n)
Expand around center O(n²) if n small
DP table for boolean queries
```

---

## Pattern 9 — Two-Pointer on Sorted Suffixes / Strings

Signal:

```text
Lexicographic comparison of many suffixes
Merge k strings optimally (LeetCode "Minimum Addition to Make String Sorted" variants)
```

Reach for:

```text
Suffix array
Priority queue merging
Sometimes greedy on string structure
```

---

## Pattern 10 — Character Frequency / Anagram Window

Signal:

```text
Permutation of P in T
Fixed alphabet size
All anagrams in sliding window
```

Reach for:

```text
Fixed-size frequency array (26 counts)
Sliding window with diff counter
Not pure "string algo" but constant companion
```

```text
Need window size == |P|
Track how many char frequencies match
```

---

## Pattern 11 — String Hash as Equality Proxy

Signal:

```text
Compare substrings quickly many times
Avoid O(m) strcmp repeatedly
```

Reach for:

```text
Prefix hash array
H[i] = hash of T[0..i-1]
Get hash of T[l..r] in O(1)
```

```cpp
// prefix hash with double mod for safety
hash(l, r) = (H[r+1] - H[l] * powB[r-l+1]) mod M
```

---

## Pattern 12 — Greedy String Construction

Signal:

```text
Remove duplicates lexicographically smallest
Reorganize string with constraints (no adjacent equal)
```

Reach for:

```text
Stack + greedy
Count remaining chars
Not matching algorithm but string DP/greedy family
```

---

# 12. Pattern Composition

Advanced problems combine patterns.

## Example A — Word Search II

```text
Trie from dictionary
Grid DFS with prefix pruning
Backtracking with visited marking
```

From trees notes.

---

## Example B — Shortest Palindrome (KMP)

```text
Find longest suffix of S matching prefix of reverse(S)
Use KMP on rev(S) + "#" + S
Answer = reverse(remaining prefix) + S
```

---

## Example C — Distinct Echo Substrings

```text
Z-array or rolling hash
Check substrings of even length where halves equal
```

---

## Example D — Multi-Search Replacement

```text
Aho-Corasick to find all pattern ends
Sort matches by position
Rebuild string or use interval marking
```

---

Notice:

```text
You're not learning infinite algorithms.
You're reusing:
  borders (KMP)
  prefix similarity (Z)
  rolling hash
  trie pruning
  suffix indexing
```

---

# 13. Pattern Recognition

Train yourself to map problem phrases to tools.

```text
"Find pattern in text once"
        ↓
KMP / Z / Rabin-Karp / library find
```

```text
"Many patterns, one text"
        ↓
Aho-Corasick
```

```text
"Dictionary prefix while scanning"
        ↓
Trie
```

```text
"Static text, many substring queries"
        ↓
Suffix array / suffix tree / automaton
```

```text
"Longest palindrome"
        ↓
Manacher or expand-around-center
```

```text
"Compare all windows of length k"
        ↓
Rolling hash
```

```text
"Overlap / period / border"
        ↓
KMP lps or Z on string
```

```text
"Grid + word list"
        ↓
Trie + DFS
```

```text
"Lexicographically smallest with constraint"
        ↓
Greedy + stack / suffix structure
```

```text
"Count distinct substrings"
        ↓
Suffix array + LCP or suffix automaton
```

```text
"Anagram in window"
        ↓
Frequency array sliding window (from array patterns)
```

```text
"Repeated DNA / periodic string"
        ↓
KMP period or Z-array
```

That last mapping saves time — don't jump to suffix tree when KMP lps suffices.

---

# 14. Edge Cases — Extremely Important for Strings

String code fails on boundaries more than almost any other topic.

Always test:

---

## Empty and Minimal

```text
T = ""
P = ""
P longer than T
P length 1
T length 1
Both single char equal / not equal
```

---

## Pattern at Boundaries

```text
Match at index 0
Match at index n - m (last possible)
Match only at end
Match only at start
```

---

## Overlapping Matches

```text
T = "aaaa"
P = "aa"

Matches at 0, 1, 2 — not just 0 and 2
KMP must continue after reporting match
```

---

## Separator in Z / Combined Strings

```text
P = "a#b", T contains "#"
Choose separator not in alphabet (null char trick, or unique sentinel)
```

---

## Hash Collisions

```text
All same character strings
Verify after hash match
Consider double hashing in adversarial CP
```

---

## Modulo Arithmetic

```text
(th - T[i] * powB % M + M) % M   // avoid negative in C++
Overflow: use long long
```

---

## Unicode / Multibyte

```text
These notes assume byte/char or fixed alphabet
UTF-8 breaks naive char indexing
Real systems need normalization (NFC/NFD)
Interview usually ASCII lowercase a-z
```

---

## Case Sensitivity

```text
Explicitly convert or compare case-folded
Turkish I problem in production i18n
```

---

## Palindrome Even vs Odd Length

```text
Manacher preprocessing handles both
Expand-around-center needs two cases OR transformed string
```

---

## Trie Memory

```text
26-child array sparse for short words → wasteful
map<char, Node*> for large alphabet
Delete/prune unused nodes in advanced problems
```

---

## Suffix Array Off-by-One

```text
SA gives start indices
LCP size n-1 or n depending on definition
Last suffix has no next suffix in LCP
```

---

## Aho-Corasick Output Propagation

```text
Pattern "a" and "aba" both match at some positions
Failure link output merging must include suffix patterns
```

---

# 15. Important Implementation Skills

## Skill 1 — Draw the String and Indices

```text
index: 0 1 2 3 4 5
T:     a b a b c a
P:           a b c

Always mark i, j, window [L,R]
```

---

## Skill 2 — Implement LPS by Hand

Until you can build lps for arbitrary P without code, KMP will feel magical instead of mechanical.

---

## Skill 3 — Trace One Full KMP Mismatch Recovery

Pick non-trivial P like `"ababac"`.

---

## Skill 4 — Prefix Hash Template

Memorize one clean prefix hash struct for O(1) substring hash queries.

---

## Skill 5 — Know When NOT to Implement Suffix Tree

Reach for suffix array + binary search or Z/KMP first.

---

## Skill 6 — Separator Convention

Standardize:

```text
P + "#" + T     for matching
rev(S) + "#" + S   for palindrome-prefix tricks
```

---

# 16. Compare Multiple Approaches

Example: **Find all occurrences of P in T**

### Brute force

```text
Time:  O(n·m)
Space: O(1)
Simple, bad worst case
```

### Rabin-Karp

```text
Time:  O(n+m) average, O(n·m) worst
Space: O(1)
Good for multiple pattern hashes
```

### KMP

```text
Time:  O(n+m)
Space: O(m)
Guaranteed linear, best default for single pattern
```

### Z-algorithm

```text
Time:  O(n+m)
Space: O(n+m) for combined string
Elegant when already using Z elsewhere
```

### Boyer-Moore (awareness)

```text
Time:  O(n·m) worst, often sublinear on large alphabet
Space: O(σ)
Skips from end of pattern; used in grep implementations
Good to name, not required to implement deeply here
```

---

Example: **Dictionary of many words, scan text**

### Repeated KMP

```text
O(k · (n + m_avg))
Simple if k small
```

### Trie only (restart each position)

```text
Can degenerate without failure links
Not sufficient alone for optimal scan
```

### Aho-Corasick

```text
O(n + matches + build)
Correct tool for large dictionary
```

---

Example: **Longest palindromic substring**

### Expand around center

```text
O(n²) time, O(1) space
Fine for moderate n
```

### DP

```text
O(n²) time, O(n²) space
Useful if many palindrome queries on all intervals
```

### Manacher

```text
O(n) time, O(n) space
When n is large or time limit tight
```

---

# 17. Problems to Practice

Organized by technique. Do a few from each bucket deeply — implement, trace, explain complexity.

---

## Brute Force / Baseline

* Implement strstr behavior from scratch
* Count occurrences with overlap
* Find first differing character of two strings

---

## Rabin-Karp / Rolling Hash

* Repeated DNA Sequences (length-10 windows)
* Subarrays with equal hash (array variant)
* Find substring anagrams (hash + sliding window hybrid)

---

## KMP / LPS

* Implement strStr / find index of first match
* Find all occurrences with overlap
* Shortest Palindrome (KMP on reversed string)
* Repeated Substring Pattern (period via lps)

---

## Z-Algorithm

* Find all occurrences via Z on combined string
* Longest prefix which is suffix at each position
* Count distinct echo substrings (with hash/Z hybrid)

---

## Trie (see also trees/notes.md)

* Implement Trie (Insert/Search/StartsWith)
* Replace Words
* Longest Word in Dictionary
* Word Search II
* Design Add and Search Words (wildcard DFS)

---

## Suffix Array / Advanced Index

* Count distinct substrings (conceptual with SA + LCP)
* Longest repeated substring (SA + LCP max)
* Binary search pattern in text using SA

---

## Manacher / Palindromes

* Longest Palindromic Substring
* Palindromic Substrings (count)
* Manacher vs expand — compare runtimes on same input

---

## Aho-Corasick

* Multi-pattern search (implement minimal version)
* Word break II with huge dictionary (Aho-Corasick + DP)
* Search keywords in document stream

---

## Pattern Composition / Mixed

* Shortest Palindrome
* Different ways to add parentheses — less string, but parsing adjacency
* Minimum window containing all characters of multiple strings
* Substring with concatenation of all words (hash + sliding window)

---

## Interview Classics (String Family)

* Valid Anagram / Group Anagrams (hashing)
* Longest Substring Without Repeating Characters (sliding window)
* Minimum Window Substring
* Decode Ways
* Edit Distance (DP — bridge to DP phase)

That is enough for a first deep pass through Phase 13.

---

# 18. For EVERY String Algorithm Problem

Use this thinking process — same discipline as Arrays and Linked Lists.

---

**1. What exactly am I matching or indexing?**

```text
Single pattern?
Many patterns?
Substrings, prefixes, suffixes, palindromes?
```

---

**2. Is the text static or changing?**

```text
Static → suffix array/tree/automaton may pay off
Streaming / one pass → KMP, Rabin-Karp, Aho-Corasick
```

---

**3. One pass or many queries?**

```text
One query → KMP/Z/RK often enough
Many queries on same T → preprocess
```

---

**4. What must I reuse on mismatch or shift?**

```text
Border (KMP)
Prefix match length (Z)
Hash of window (Rabin-Karp)
Trie failure (Aho-Corasick)
Nothing (brute force)
```

---

**5. Alphabet size and constraints?**

```text
Lowercase a-z → array[26]
Large / Unicode → hash map or library types
Binary → bit trie
```

---

**6. Overlapping matches?**

```text
If yes → careful KMP continuation
If no → can skip ahead by |P| after match
```

---

**7. Expected vs worst-case time?**

```text
Adversarial inputs break hash-only approaches
Need guaranteed linear → KMP, Z, Manacher
```

---

**8. Space budget?**

```text
O(1) extra → brute force, RK scan
O(m) → KMP
O(n) → Z, Manacher, suffix structures
```

---

**9. Can a simpler pattern from Arrays/Hashing solve it?**

```text
Anagram window → frequency count
Two equal substrings of len k → rolling hash
Don't over-engineer with suffix tree
```

---

**10. Edge cases?**

```text
Empty, single char, overlap, boundary match
Separator collisions, modulo negatives
```

---

**11. Draw an example.**

Strings are visual. Five seconds of indexing prevents twenty minutes of bugs.

---

**12. Explain WHY the invariant holds.**

For KMP:

```text
After fallback, matched prefix is longest border — so text alignment preserved
```

If you can't explain invariants, you don't own the algorithm yet.

---

# 19. Relationship to Other Phases

```text
Phase 2  Arrays & Strings     → character arrays, basic manipulation
Phase 5  Hash Tables           → frequency, anagram maps, string keys
Phase 6  Trees                 → Trie deep dive (trees/notes.md §10)
Phase 7  Searching & Sorting   → binary search on SA, sort suffixes
Phase 9  Dynamic Programming   → edit distance, palindrome DP, regex DP
Phase 11 Backtracking          → Word Search II with trie
Phase 13 String Algorithms     → this document
```

When stuck, ask whether the problem is really:

```text
String matching/indexing  → this phase
Window + frequency        → arrays/hashmap
Tree traversal            → trees
DP on intervals           → dynamic programming
```

---

# 20. One Especially Important Mental Shift

Don't treat string algorithms as a bag of unrelated tricks.

They all answer variants of:

```text
How much of what I already know about this text
can I reuse without re-reading characters?
```

```text
Brute force     → reuse nothing
Rabin-Karp      → reuse hash of previous window
KMP             → reuse matched prefix border
Z               → reuse prefix match within [L,R] window
Trie            → reuse shared prefix paths
Suffix structures → reuse precomputed suffix relationships
Manacher        → reuse palindrome symmetry
Aho-Corasick    → reuse trie prefix + KMP-like fallback
```

Once you see **reuse of partial knowledge**, the field organizes itself.

---

# Your Complete String Algorithms Checklist

```text
STRING ALGORITHMS (Phase 13)
│
├── Fundamentals
│   ├── Substring vs subsequence vs prefix vs suffix
│   ├── Single vs multi-pattern matching
│   ├── Static text vs streaming
│   ├── Alphabet size implications
│   └── Complexity landscape (comparison vs hash vs index)
│
├── Core Algorithms
│   ├── Brute Force
│   │   ├── O(n·m) baseline
│   │   └── Early mismatch exit
│   │
│   ├── Rabin-Karp
│   │   ├── Polynomial rolling hash
│   │   ├── O(1) window slide
│   │   ├── Collision verification
│   │   └── Multi-pattern hash set variant
│   │
│   ├── KMP
│   │   ├── LPS / failure function
│   │   ├── Build lps O(m)
│   │   ├── Search O(n)
│   │   ├── Overlapping matches
│   │   └── Period / border problems
│   │
│   ├── Z-Algorithm
│   │   ├── Z[i] = prefix match at i
│   │   ├── [L,R] optimization window
│   │   └── P + sep + T matching trick
│   │
│   ├── Trie Applications
│   │   ├── Cross-ref trees/notes.md §10
│   │   ├── Prefix search / autocomplete
│   │   ├── Word Search II
│   │   ├── Replace Words
│   │   └── Bit trie (XOR problems)
│   │
│   ├── Suffix Array
│   │   ├── Sort suffix indices
│   │   ├── Build (naive → O(n log n))
│   │   ├── Pattern search via binary search
│   │   └── LCP array + Kasai (awareness)
│   │
│   ├── Suffix Tree / Automaton (Intro)
│   │   ├── O(n) substring indexing concept
│   │   ├── O(m) pattern query
│   │   ├── Ukkonen (conceptual)
│   │   └── Suffix automaton as alternative
│   │
│   ├── Manacher's Algorithm
│   │   ├── Transform with # separators
│   │   ├── Mirror reuse within [C,R]
│   │   └── Longest palindrome O(n)
│   │
│   └── Aho-Corasick
│       ├── Trie of all patterns
│       ├── BFS failure links
│       ├── Output propagation
│       └── Single-pass multi-pattern scan
│
├── Core Patterns
│   ├── Single pattern linear match (KMP/Z)
│   ├── Rolling hash window
│   ├── Prefix function / border reuse
│   ├── Z on combined string
│   ├── Trie prefix pruning
│   ├── Multi-pattern dictionary scan
│   ├── Suffix index on static text
│   ├── Palindrome structure (Manacher)
│   ├── Two-pointer / lexicographic merge
│   ├── Anagram frequency window
│   ├── Prefix hash for O(1) substring compare
│   └── Greedy string with stack
│
├── Pattern Composition
│   ├── Word Search II = Trie + Grid DFS
│   ├── Shortest Palindrome = KMP on reversed
│   ├── Distinct echoes = Z or rolling hash
│   └── Multi-replace = Aho-Corasick + intervals
│
├── Pattern Recognition
│   ├── "One pattern" → KMP/Z/RK
│   ├── "Many patterns" → Aho-Corasick
│   ├── "Prefix dictionary" → Trie
│   ├── "Many queries, static T" → Suffix array/tree
│   ├── "Longest palindrome" → Manacher
│   ├── "Window of len k" → Rolling hash
│   ├── "Period/overlap" → LPS / Z
│   └── "Anagram window" → Frequency array
│
├── Edge Cases
│   ├── Empty / |P|>|T|
│   ├── Match at 0 or n-m
│   ├── Overlapping occurrences
│   ├── Separator not in alphabet
│   ├── Hash collision verification
│   ├── Modulo negatives
│   ├── Even/odd palindrome centers
│   ├── Trie alphabet sparsity
│   └── Aho-Corasick suffix output merge
│
├── Analysis
│   ├── Compare brute / RK / KMP / Z
│   ├── When hash worst-case bites
│   ├── Suffix array vs tree trade-offs
│   ├── Expected vs guaranteed linear
│   └── Space vs preprocessing time
│
├── Implementation Skills
│   ├── Draw indices on paper
│   ├── Build LPS by hand
│   ├── Trace KMP mismatch recovery
│   ├── Prefix hash template
│   ├── Separator conventions
│   └── Know when NOT to build suffix tree
│
├── Practice Problems
│   ├── strStr / repeated substring pattern
│   ├── Shortest palindrome
│   ├── Implement Trie / Word Search II
│   ├── Longest palindromic substring
│   ├── Rabin-Karp window problems
│   └── Multi-pattern search
│
└── For EVERY Problem
    ├── Define matching vs indexing task
    ├── Static or streaming text?
    ├── One query or many?
    ├── What gets reused on shift/mismatch?
    ├── Check simpler array/hash solution
    ├── Handle overlap and boundaries
    ├── Analyze time and space
    ├── Test empty/single/overlap cases
    └── Explain invariant / WHY it works
```

---

Once you can **implement KMP and Rabin-Karp from scratch**, build a trie and explain when Aho-Corasick wins, describe suffix array purpose even if you don't code O(n) construction, run Manacher on a example string, and map new problems to the pattern recognition chart above — Phase 13 is solid.

Next in roadmap (`map.md`): **Phase 14 — Advanced Graph Algorithms** (SCC, Network Flow, Matching, LCA).
