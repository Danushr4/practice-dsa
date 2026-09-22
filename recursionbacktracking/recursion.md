# Recursion — Deep Understanding

At the highest level:  
solve a problem using solutions to smaller instances of the same problem

A recursive algorithm normally has this structure:

```cpp
ReturnType solve(State x) {
    if (base_case(x))
        return base_answer;

    // transform x into a smaller problem
    return combine(x, solve(smaller(x)));
}
```

But that syntax hides the important part.

The computer does **not understand recursion as a magical concept**.

At runtime, recursion becomes: function calls + stack frames + control flow

That is our starting point.

---



# 1. Call Stack and Stack Frames

Consider:

```cpp
int factorial(int n) {
    if (n == 0)
        return 1;

    return n * factorial(n - 1);
}
```

Call:

```cpp
factorial(3);
```

Conceptually:

```text
factorial(3)
    needs factorial(2)

factorial(2)
    needs factorial(1)

factorial(1)
    needs factorial(0)

factorial(0)
    returns 1
```

Then execution unwinds:

```text
factorial(0) = 1

factorial(1)
= 1 * factorial(0)
= 1

factorial(2)
= 2 * factorial(1)
= 2

factorial(3)
= 3 * factorial(2)
= 6
```

There are therefore **two phases**:

```text
Descending phase
3 → 2 → 1 → 0

Ascending / unwinding phase
0 → 1 → 2 → 3
```

This distinction is extremely important.

## What happens physically?

Every function call needs information such as:

```text
parameters
local variables
return address
saved registers
previous stack-frame information
temporary values
```

The runtime stores this information in a **stack frame**.

Conceptually:

```text
           Call Stack

        ┌───────────────┐
        │ factorial(0)  │ ← current
        │ n = 0         │
        ├───────────────┤
        │ factorial(1)  │
        │ n = 1         │
        ├───────────────┤
        │ factorial(2)  │
        │ n = 2         │
        ├───────────────┤
        │ factorial(3)  │
        │ n = 3         │
        ├───────────────┤
        │ main()        │
        └───────────────┘
```

The important property is:

$$
\boxed{\text{Call stack = LIFO}}
$$

Last In, First Out.

`factorial(0)` was called last and therefore returns first.

---



# Stack frame mental model

Imagine:

```cpp
int f(int x) {
    int y = x * 2;
    return y + f(x - 1);
}
```

Each invocation gets its **own** `x` **and** `y`.

They are not shared.

For example:

```text
f(3)

frame #1
x = 3
y = 6

f(2)

frame #2
x = 2
y = 4

f(1)

frame #3
x = 1
y = 2
```

This explains a common beginner question:

> If the same function is executing recursively, how can it have different variable values?

Because these are different **function invocations**, with separate stack frames.

Same function code.

Different runtime state.

Think:

```text
CODE
f()
↓
shared by everybody

STATE
frame for f(3)
frame for f(2)
frame for f(1)
↓
different for every invocation
```

---



# Return address

Consider:

```cpp
int foo(int n) {
    int x = foo(n - 1);
    return x + 10;
}
```

When `foo(n - 1)` finishes, the CPU must know:

> Where should execution continue?

It has to resume approximately here:

```cpp
int x = foo(n - 1);
//        ↑
// recursive call finishes

return x + 10;
// resume here
```

The stack frame preserves information needed to continue execution.

Therefore recursion is effectively saying:

> Suspend this computation, execute another function invocation, then resume this computation later.

That idea becomes extremely important for trees, DFS, backtracking, interpreters, compilers, parsers, etc.

---



# A much better mental model

Don't think:

> function calls itself

Think:

> The current computation is suspended while a smaller computation is solved.

For:

```cpp
return n * factorial(n - 1);
```

`factorial(3)` is conceptually holding:

```text
3 * ???
```

while waiting for `factorial(2)`.

Then:

```text
3 * (2 * ???)
```

Then:

```text
3 * (2 * (1 * ???))
```

Then:

```text
3 * (2 * (1 * 1))
```

The call stack stores these unfinished computations.

That is one of the deepest ways to understand recursion.

---



# 2. Base Case and Recursive Case

Every well-defined recursion needs progress toward termination.

Usually:

```cpp
if (smallest_problem)
    return answer;

return solve(smaller_problem);
```

These are:

### Base case

A problem that can be answered directly.

### Recursive case

Reduce the problem into one or more smaller instances.

Example:

```cpp
int sum(int n) {
    if (n == 0)
        return 0;

    return n + sum(n - 1);
}
```

Mathematically:

$$
S(n)=
\begin{cases}
0 & n=0
n+S(n-1) & n>0
\end{cases}
$$

Notice that recursive algorithms can often be described naturally as mathematical recurrence definitions.

---



# Three conditions for correct recursion

A useful rule:

$$
\boxed{\text{Base + Progress + Correct Combination}}
$$

## 1. Base condition

There must be some state where recursion stops.

```cpp
if (n == 0)
    return;
```



## 2. Progress

Every recursive call must move toward the base case.

Good:

```cpp
solve(n - 1);
```

Bad:

```cpp
solve(n);
```

Potentially bad:

```cpp
solve(n + 1);
```

if your termination condition is `n == 0`.

## 3. Correct combination

The recursive result must be used correctly.

Factorial:

```cpp
return n * factorial(n - 1);
```

because mathematically:

$$
n! = n(n-1)!
$$

---



# Base case is not always `n == 0`

For binary search:

```cpp
if (left > right)
    return -1;
```

For trees:

```cpp
if (node == nullptr)
    return;
```

For strings:

```cpp
if (index == str.size())
    return;
```

For graph DFS:

```cpp
if (visited[node])
    return;
```

So conceptually:

$$
\boxed{\text{Base case = state requiring no further decomposition}}
$$

---



# 3. Thinking Recursively

Suppose you need the sum:

```text
[5, 2, 7, 9]
```

Imperative thinking:

```cpp
sum = 0;
for each number:
    sum += number;
```

Recursive thinking asks:

> If someone magically gives me the answer for the smaller problem, how can I construct my answer?

For:

```text
sum([5,2,7,9])
```

Suppose you already know:

```text
sum([2,7,9])
```

Then:

```text
sum([5,2,7,9])
=
5 + sum([2,7,9])
```

Therefore:

```cpp
int sum(vector<int>& a, int i) {
    if (i == a.size())
        return 0;

    return a[i] + sum(a, i + 1);
}
```

This is perhaps the most important recursive reasoning technique:

$$
\boxed{\text{Trust that recursion solves the smaller problem}}
$$

You don't mentally execute all recursive calls while designing the function.

Instead:

1. Define what `solve(x)` promises.
2. Assume `solve(smaller(x))` fulfills that promise.
3. Use it to solve `x`.

This is sometimes called the **recursive leap of faith**.

---



# 4. Recurrence Relations

Now we move from execution to mathematical analysis.

Suppose:

```cpp
void f(int n) {
    if (n == 0)
        return;

    doConstantWork();
    f(n - 1);
}
```

Let:

$$
T(n)
$$

represent runtime for input size `n`.

Each invocation does:

```text
constant work       → c
recursive problem   → T(n-1)
```

Therefore:

$$
T(n)=T(n-1)+c
$$

and:

$$
T(0)=d
$$

Expand:

$$
T(n)=T(n-1)+c
$$

$$
=T(n-2)+2c
$$

$$
=T(n-3)+3c
$$

Eventually:

$$
=T(0)+nc
$$

Therefore:

$$
\boxed{T(n)=\Theta(n)}
$$

---



# Recurrence is not recursion

Important distinction:

**Recursion** is an implementation/control-flow technique.

**Recurrence relation** is a mathematical equation describing how a quantity depends on smaller values.

Example:

```cpp
f(n) {
    f(n / 2);
}
```

Runtime recurrence:

$$
T(n)=T(n/2)+O(1)
$$

Result:

$$
T(n)=O(\log n)
$$

---



# Common recurrence patterns

You should eventually recognize these almost instantly.

### Linear recursion

```cpp
f(n - 1);
```

$$
T(n)=T(n-1)+O(1)
$$

$$
\boxed{O(n)}
$$

---



### Halving

```cpp
f(n / 2);
```

$$
T(n)=T(n/2)+O(1)
$$

$$
\boxed{O(\log n)}
$$

Example: binary search.

---



### Two half-sized recursive calls

```cpp
f(n / 2);
f(n / 2);
```

with linear combine work:

$$
T(n)=2T(n/2)+O(n)
$$

$$
\boxed{O(n\log n)}
$$

Merge sort.

---



### Two `n-1` recursive calls

```cpp
f(n - 1);
f(n - 1);
```

$$
T(n)=2T(n-1)+O(1)
$$

$$
\boxed{O(2^n)}
$$

Very different from:

$$
2T(n/2)
$$

This distinction matters tremendously.

---



# 5. Recursive Tree Visualization

A recursion tree shows **all function invocations**.

Consider naive Fibonacci:

```cpp
int fib(int n) {
    if (n <= 1)
        return n;

    return fib(n - 1) + fib(n - 2);
}
```

For `fib(5)`:

```text
                         fib(5)
                      /          \
                 fib(4)          fib(3)
                /     \          /     \
           fib(3)    fib(2)  fib(2)   fib(1)
           /   \      /  \    /   \
      fib(2) fib(1) f1   f0  f1   f0
      /   \
    f1    f0
```

Immediately notice repeated work:

```text
fib(3)
fib(2)
fib(2)
fib(2)
...
```

The recursion tree reveals the problem.

---



# Tree height vs number of calls

This is a critical distinction.

For Fibonacci:

```text
recursion depth ≈ n
```

but:

```text
number of calls ≈ exponential
```

So:

$$
\text{stack space} = O(n)
$$

while:

$$
\text{time} = O(2^n)
$$

approximately.

Time complexity and recursion depth are **not the same thing**.

---



# 6. Time Complexity of Recursion

People often make this mistake:

> recursion has O(n) complexity because it goes n levels deep.

Wrong.

Depth tells you approximately **simultaneously active stack frames**.

Runtime depends on the **total amount of work across all calls**.

Consider:

```cpp
void f(int n) {
    if (n == 0)
        return;

    f(n - 1);
    f(n - 1);
}
```

Recursion depth:

$$
O(n)
$$

But number of calls:

```text
level 0: 1
level 1: 2
level 2: 4
level 3: 8
...
```

Total:

$$
1+2+4+\dots+2^n
$$

Geometric series:

$$
2^{n+1}-1
$$

Therefore:

$$
\boxed{O(2^n)}
$$

Yet stack space remains:

$$
\boxed{O(n)}
$$

because the calls are not all alive simultaneously.

---



# Three questions for recursion complexity

Always ask:

### 1. How many recursive calls per invocation?

Example:

```cpp
f(n - 1);
```

one.

versus:

```cpp
f(n - 1);
f(n - 1);
```

two.

### 2. How much does the problem shrink?

```text
n → n - 1
```

versus

```text
n → n / 2
```



### 3. How much non-recursive work happens per call?

Example:

```cpp
for (int i = 0; i < n; ++i)
    ...
```

means each node may itself do O(n) work.

---



# Example

```cpp
void f(int n) {
    if (n <= 1)
        return;

    for (int i = 0; i < n; i++)
        cout << i;

    f(n / 2);
}
```

Recurrence:

$$
T(n)=T(n/2)+O(n)
$$

Expand:

$$
n+n/2+n/4+n/8+\dots
$$

Geometric series:

$$
<2n
$$

Therefore:

$$
\boxed{T(n)=O(n)}
$$

A recursion with \log n depth can still have O(n) runtime.

---



# 7. Space Complexity of Recursion

Suppose:

```cpp
f(n) {
    if (n == 0)
        return;

    f(n - 1);
}
```

Maximum simultaneously active calls:

```text
f(n)
f(n-1)
f(n-2)
...
f(0)
```

So:

$$
\boxed{O(n)\text{ auxiliary stack space}}
$$

Now:

```cpp
f(n) {
    if (n <= 1)
        return;

    f(n / 2);
}
```

Depth:

```text
n
n/2
n/4
...
1
```

Number of divisions until 1:

$$
\log_2 n
$$

Therefore:

$$
\boxed{O(\log n)\text{ stack space}}
$$

---



# Important subtlety: branching does not automatically increase stack space

Consider:

```cpp
f(n) {
    f(n - 1);
    f(n - 1);
}
```

The first recursive subtree finishes before the second begins.

The stack doesn't contain the entire recursion tree.

It contains only the **current path**.

So:

```text
recursion tree width → influences total work
maximum tree depth   → influences stack memory
```

A useful approximation:

$$
\boxed{\text{Space} \approx \text{maximum recursion depth} \times \text{frame size}}
$$

---



# 8. Tail Recursion

Consider factorial:

```cpp
int factorial(int n) {
    if (n == 0)
        return 1;

    return n * factorial(n - 1);
}
```

Is this tail recursive?

No.

Why?

Because after:

```cpp
factorial(n - 1)
```

returns, we still need:

```cpp
n * result
```

There is unfinished work.

Call stack conceptually stores:

```text
3 * ???
2 * ???
1 * ???
```

---

A **tail recursive call** is the final operation performed by the function.

Example:

```cpp
int factorialTail(int n, int acc) {
    if (n == 0)
        return acc;

    return factorialTail(n - 1, n * acc);
}
```

Call:

```cpp
factorialTail(4, 1)
```

becomes:

```text
factorialTail(4, 1)
factorialTail(3, 4)
factorialTail(2, 12)
factorialTail(1, 24)
factorialTail(0, 24)
```

There is nothing to do after the recursive call returns.

---



# Tail Call Optimization

Some languages/compilers can transform:

```cpp
return f(x);
```

into something roughly equivalent to:

```cpp
x = newX;
goto function_start;
```

That means the existing frame can theoretically be reused.

Then:

$$
O(n) \text{ recursive stack}
$$

can potentially become:

$$
O(1)
$$

This is called:

**Tail Call Optimization — TCO**

But don't assume it exists.

In practical C++ code, the compiler may optimize some tail calls, but the language does not give you a universal guarantee that every tail-recursive program becomes constant-stack execution.

So for correctness involving huge recursion depth:

> Do not rely blindly on tail-call optimization in C++.

---



# Tail recursion vs normal recursion

Normal:

```cpp
return n + f(n - 1);
```

Pending operation:

```text
+
```

Tail:

```cpp
return f(n - 1, accumulator + n);
```

No pending operation.

A useful test:

> After the recursive call returns, does this invocation have anything left to do?

If yes → not tail recursive.

If no → potentially tail recursive.

---



# 9. Direct vs Indirect Recursion



## Direct recursion

Function directly calls itself.

```cpp
void A(int n) {
    if (n <= 0)
        return;

    A(n - 1);
}
```

Call graph:

```text
A → A
```

---



## Indirect recursion

A calls B, B eventually calls A.

```cpp
void A(int n) {
    if (n <= 0)
        return;

    B(n - 1);
}

void B(int n) {
    if (n <= 0)
        return;

    A(n - 1);
}
```

Graph:

```text
A → B
↑   ↓
└───┘
```

This matters in real systems.

For example parsers may naturally contain mutually recursive functions:

```text
parseExpression()
    ↓
parseTerm()
    ↓
parseFactor()
    ↓
parseExpression()
```

depending on grammar structure.

This is often called **mutual recursion** when two or more functions participate.

---



# 10. Multiple Recursion

Multiple recursion means a function creates more than one recursive call.

Example:

```cpp
void f(int n) {
    if (n <= 0)
        return;

    f(n - 1);
    f(n - 1);
}
```

or naturally:

```cpp
void traverse(Node* root) {
    if (!root)
        return;

    traverse(root->left);
    traverse(root->right);
}
```

The second example is extremely important.

Binary trees are structurally recursive:

```text
Tree =
    Node
      left subtree
      right subtree
```

So the algorithm mirrors the data structure.

This reveals why recursion is so natural for trees.

---



# Structural recursion

If data is recursively defined, recursive algorithms often become the most natural solution.

A binary tree is:

$$
Tree = Empty
$$

or:

$$
Tree = Node(value, Tree, Tree)
$$

Therefore traversal naturally becomes:

```cpp
void visit(Node* node) {
    if (!node)
        return;

    visit(node->left);
    visit(node->right);
}
```

This idea generalizes to:

```text
directory trees
UI component trees
DOM trees
ASTs
scene graphs
CSG trees
BVHs
dependency graphs
```

For CAD/software engineering, you'll encounter this constantly in hierarchical geometric and scene structures.

---



# 11. Recursion → Iteration Conversion

One of the most important things to understand:

$$
\boxed{\text{Recursion usually hides a stack}}
$$

Therefore many recursive algorithms can be converted to iteration using an **explicit stack**.

Recursive DFS:

```cpp
void dfs(Node* node) {
    if (!node)
        return;

    process(node);

    for (Node* child : node->children)
        dfs(child);
}
```

Equivalent conceptual iterative form:

```cpp
void dfs(Node* root) {
    stack<Node*> st;
    st.push(root);

    while (!st.empty()) {
        Node* node = st.top();
        st.pop();

        process(node);

        for (Node* child : node->children)
            st.push(child);
    }
}
```

The recursive version uses:

```text
implicit runtime stack
```

The iterative version uses:

```text
explicit programmer-controlled stack
```

This equivalence is foundational.

---



# Simple recursion → loop

Consider:

```cpp
void countdown(int n) {
    if (n == 0)
        return;

    cout << n;
    countdown(n - 1);
}
```

Equivalent:

```cpp
void countdown(int n) {
    while (n != 0) {
        cout << n;
        n--;
    }
}
```

This one doesn't need an explicit stack because there is **no pending work**.

That's essentially why tail recursion is easy to transform into iteration.

---



# Non-tail recursion needs saved state

Consider:

```cpp
void printAscending(int n) {
    if (n == 0)
        return;

    printAscending(n - 1);
    cout << n;
}
```

Call:

```cpp
printAscending(3);
```

Output:

```text
1 2 3
```

The function must remember:

```text
after recursion:
print 1
print 2
print 3
```

That pending state is naturally stored in call frames.

To convert it iteratively, you need to preserve equivalent information:

```cpp
stack<int> st;

while (n > 0) {
    st.push(n);
    n--;
}

while (!st.empty()) {
    cout << st.top();
    st.pop();
}
```

The stack didn't disappear.

You just moved responsibility from the runtime to yourself.

---



# Recursive frame ≈ explicit state object

This becomes even clearer with complex algorithms.

A recursive frame may implicitly store:

```text
node
current child index
local accumulated result
what instruction to execute after return
```

When converting that recursion to iteration, you might create:

```cpp
struct Frame {
    Node* node;
    int childIndex;
    int state;
    Result partial;
};
```

Then:

```cpp
stack<Frame> frames;
```

This is how interpreters, parsers, graph algorithms and production systems sometimes eliminate deep recursion.

Very important engineering insight:

$$
\boxed{\text{Recursion is an implicit state machine}}
$$

A stack frame is essentially the suspended state of one invocation.

---



# 12. Iteration → Recursion

Loops can often become recursion too.

Iteration:

```cpp
for (int i = 0; i < n; i++)
    process(i);
```

Recursive equivalent:

```cpp
void loop(int i, int n) {
    if (i == n)
        return;

    process(i);

    loop(i + 1, n);
}
```

The loop state:

```text
i
```

becomes a recursive parameter.

Generally:

```text
loop variables
↓
recursive arguments
```

and:

```text
loop termination
↓
base case
```

But just because transformation is possible doesn't mean recursion is preferable.

Use the representation that expresses the problem most clearly and safely.

---



# 13. Stack Overflow

Every recursive call normally consumes some stack memory.

Suppose each frame requires approximately F bytes and the available thread stack is S.

Very roughly:

$$
\text{maximum depth} \approx \frac{S}{F}
$$

The real value varies due to:

```text
compiler
optimization
architecture
ABI
debug/release mode
local variables
register spilling
OS/thread configuration
```

So there is no universal:

> C++ recursion limit = X

Unlike Python, native C++ typically has no language-level fixed recursion count. You're constrained primarily by stack memory and generated code.

---

Example:

```cpp
void recurse(long long n) {
    if (n == 0)
        return;

    recurse(n - 1);
}
```

Calling:

```cpp
recurse(100000000);
```

is likely disastrous.

Eventually:

```text
stack
│ frame
│ frame
│ frame
│ frame
│ frame
│ ...
↓
guard page / unavailable memory
```

Then you may get:

```text
stack overflow
segmentation fault
access violation
```

depending on platform.

---



# Why tree recursion can be dangerous

Consider DFS on a tree with one million nodes.

Balanced tree:

$$
depth \approx \log_2(10^6)\approx20
$$

Fine.

But a degenerate tree:

```text
1
 \
  2
   \
    3
     \
      ...
```

has:

$$
depth = 10^6
$$

Recursive DFS can stack overflow.

Same number of nodes.

Completely different stack risk.

Therefore:

$$
\boxed{\text{recursion safety depends on depth, not merely input size}}
$$

This is production-engineering relevant.

---



# 14. Memoization Introduction

Consider Fibonacci again:

```cpp
int fib(int n) {
    if (n <= 1)
        return n;

    return fib(n - 1) + fib(n - 2);
}
```

The recursive tree repeats the same states:

```text
fib(5)
├── fib(4)
│   ├── fib(3)
│   └── fib(2)
└── fib(3)          ← repeated
    ├── fib(2)      ← repeated
    └── fib(1)
```

The problem has:

### Overlapping subproblems

The same input is solved repeatedly.

Instead, save answers:

```cpp
vector<long long> memo(n + 1, -1);

long long fib(int n) {
    if (n <= 1)
        return n;

    if (memo[n] != -1)
        return memo[n];

    memo[n] =
        fib(n - 1) +
        fib(n - 2);

    return memo[n];
}
```

Now each state:

```text
fib(0)
fib(1)
fib(2)
...
fib(n)
```

is computed once.

There are only:

$$
n+1
$$

unique states.

Therefore runtime becomes:

$$
\boxed{O(n)}
$$

instead of exponential.

Memory:

```text
memo table = O(n)
call stack = O(n)
```

so:

$$
\boxed{O(n)}
$$

overall auxiliary space.

---



# Memoization is caching function states

Think more generally.

Suppose:

$$
f(state)
$$

is expensive.

If repeated calls occur:

```text
f(A)
f(B)
f(A)
f(C)
f(B)
```

we can store:

```text
cache[A] = answerA
cache[B] = answerB
```

Then later calls become lookups.

This transforms:

```text
recursion tree
```

into something closer to:

```text
state graph / DAG
```

That's the conceptual bridge from recursion to dynamic programming.

---



# Recursion vs Memoization vs DP

Do not merge these concepts.

**Recursion**

A control-flow technique.

```text
solve via calls to smaller problems
```

**Memoization**

Cache previously computed states.

```text
state → answer
```

**Dynamic programming**

A broader algorithmic technique for problems with appropriate structure, usually exploiting:

```text
overlapping subproblems
optimal substructure
```

Top-down DP often means:

```text
recursion + memoization
```

Bottom-up DP often means:

```text
iteration + table
```

Example:

Top-down:

```cpp
fib(n) → fib(n-1), fib(n-2)
```

Bottom-up:

```cpp
dp[0] = 0;
dp[1] = 1;

for (int i = 2; i <= n; ++i)
    dp[i] = dp[i-1] + dp[i-2];
```

We'll study this much more deeply when you reach DP.

---



# 15. Order of Work Matters in Recursion

This is an extremely important concept.

Compare:

```cpp
void f(int n) {
    if (n == 0)
        return;

    cout << n << " ";
    f(n - 1);
}
```

For `3`:

```text
3 2 1
```

Now:

```cpp
void f(int n) {
    if (n == 0)
        return;

    f(n - 1);
    cout << n << " ";
}
```

Output:

```text
1 2 3
```

Only one line moved.

But why does behavior reverse?

First:

```text
WORK
↓
RECURSE
```

Work occurs during **descent**.

Second:

```text
RECURSE
↓
WORK
```

Work occurs during **unwinding**.

This is fundamental for tree traversal.

---



# Preorder / inorder / postorder are recursion placement

For binary tree:

```cpp
void traverse(Node* node) {
    if (!node)
        return;

    process(node);           // preorder

    traverse(node->left);

    process(node);           // inorder position

    traverse(node->right);

    process(node);           // postorder position
}
```

The location of work relative to recursive calls determines traversal semantics.

That insight is much deeper than memorizing:

```text
preorder = root-left-right
inorder = left-root-right
postorder = left-right-root
```

Those orders emerge naturally from the call structure.

---



# 16. Recursion and Mathematical Induction

Recursion and induction are closely related.

A recursive algorithm often says:

> Assume the algorithm works for a smaller instance; use that fact to solve the larger instance.

Mathematical induction says:

1. prove base case;
2. assume true for smaller case;
3. prove current case.

Suppose:

```cpp
int sum(int n) {
    if (n == 0)
        return 0;

    return n + sum(n - 1);
}
```

Claim:

$$
sum(n)=\frac{n(n+1)}2
$$

Base:

$$
sum(0)=0
$$

correct.

Inductive assumption:

$$
sum(n-1)=\frac{(n-1)n}{2}
$$

Then:

$$
sum(n)=n+sum(n-1)
$$

$$
=n+\frac{(n-1)n}{2}
$$

$$
=\frac{n(n+1)}2
$$

So recursive correctness proofs often naturally use induction.

---



# 17. Recursion Invariants

For serious engineering, define the **contract of the recursive function**.

For example:

```cpp
bool contains(Node* node, int x)
```

Contract:

> Returns true exactly when the subtree rooted at `node` contains `x`.

Then implementation:

```cpp
bool contains(Node* node, int x) {
    if (!node)
        return false;

    if (node->value == x)
        return true;

    return contains(node->left, x) ||
           contains(node->right, x);
}
```

You don't need to manually trace every possible tree.

Assume recursively:

```text
contains(left, x)
```

correctly answers for the left subtree.

And:

```text
contains(right, x)
```

correctly answers for the right subtree.

Then combine them.

This **contract-oriented way of thinking** is far more scalable than manually simulating recursive calls.

---



# 18. Parameters Encode State

Consider:

```cpp
void search(vector<int>& a, int index)
```

Why pass `index`?

Because recursive algorithms need to encode:

> What part of the problem remains?

For example:

```text
index = 0
```

means:

```text
solve array[0...end]
```

Recursive call:

```cpp
search(a, index + 1)
```

means:

```text
solve array[1...end]
```

So recursive parameters aren't arbitrary.

They define the **state space**.

This becomes extremely important for dynamic programming and backtracking.

For DP, you often ask:

> What minimum set of variables uniquely determines a subproblem?

That is essentially asking:

> What should the recursive function's parameters be?

Example:

```cpp
solve(i, remainingCapacity)
```

means the state is:

$$
(i, capacity)
$$

This later becomes a DP table.

---



# 19. Recursion Tree vs State Graph

This distinction is worth learning now.

Naive Fibonacci generates a recursion **tree**:

```text
fib(5)
├── fib(4)
│   └── fib(3)
...
└── fib(3)
```

But logically `fib(3)` is the same state.

So the actual dependency structure is closer to:

```text
fib(5)
 ↓    ↓
fib(4) fib(3)
 ↓   ↙
fib(3)
```

Repeated tree nodes collapse into one state.

Memoization effectively performs this collapse.

Therefore:

```text
naive recursion
     ↓
execution tree

memoization
     ↓
unique state graph
```

This will become central when you study DP.

---



# 20. When Recursion Is Natural

Recursion is excellent when the problem itself has recursive structure.

Examples:

```text
Trees
AST traversal
DFS
Divide & conquer
Merge sort
Quick sort
Binary search
Recursive grammars/parsers
Backtracking
Dynamic programming
Directory traversal
Scene graphs
CSG trees
Hierarchical models
Fractals
```

For example merge sort:

```text
sort array
    ↓
sort left half
sort right half
merge
```

The problem itself decomposes recursively.

---



# 21. When Recursion Is Usually Unnecessary

For:

```cpp
for (int i = 0; i < 1'000'000; ++i)
    process(i);
```

writing:

```cpp
processRecursively(0);
```

probably gives no conceptual benefit and introduces stack risk.

So don't adopt:

> recursion is more elegant, therefore use recursion.

Instead:

$$
\boxed{\text{Use recursion when recursive structure makes reasoning clearer}}
$$

Use iteration when:

```text
the operation is naturally linear
depth may be dangerously large
you need precise memory control
recursive overhead matters
explicit traversal state is desirable
```

---



# 22. Function Call Overhead

A recursive call is still a function call.

Potential costs include:

```text
setting up parameters
creating/managing frame state
saving/restoring registers
control transfer
return handling
reduced optimization opportunities in some situations
```

Modern compilers may optimize heavily, so don't assume a fixed cost.

But conceptually:

```cpp
for (...) { ... }
```

can have less control-flow overhead than:

```cpp
recursiveCall(...);
```

That's one reason iterative forms can matter in performance-sensitive low-level code.

For CAD kernels, graphics, geometry algorithms, numerical code, etc., you'll eventually care not only about asymptotic complexity but also:

```text
cache locality
allocation
branch behavior
frame size
function inlining
vectorization
data layout
```

DSA Big-O is the beginning, not the end, of performance engineering.

---



# 23. Recursion Is Not Automatically Slow

This statement:

> recursion is slow

is wrong.

Compare:

### Binary search

Recursive:

$$
O(\log n)
$$

Iterative:

$$
O(\log n)
$$

same asymptotic runtime.

### Fibonacci

Naive recursive:

$$
O(2^n)
$$

But that's not because recursion itself is inherently exponential.

Memoized recursive Fibonacci:

$$
O(n)
$$

The exponential behavior comes from **recomputing overlapping states**, not from recursion as a language mechanism.

Important distinction:

$$
\boxed{\text{Algorithm structure determines complexity, not the presence of recursion alone}}
$$

---



# 24. Recursion Is Not Automatically O(n) Space

Another misconception.

Binary search:

```cpp
binarySearch(l, r)
```

reduces size in half.

Depth:

$$
O(\log n)
$$

Therefore recursive stack:

$$
O(\log n)
$$

Tree traversal on a balanced tree:

$$
O(\log n)
$$

stack.

On a skewed tree:

$$
O(n)
$$

stack.

So always analyze:

$$
\boxed{\text{maximum active call-chain length}}
$$

---



# 25. Deepest Mental Model: Recursion Is Continuation Storage

Consider:

```cpp
return A + solve(x);
```

Before calling `solve(x)`, the current function has unfinished business:

```text
When solve(x) returns:
    add A
    then return
```

That future computation must be remembered somewhere.

The stack frame stores effectively:

```text
current local state
+
what to do next
```

This idea is related to a deeper PL concept called a **continuation**:

> A representation of "what remains to be done."

You don't need formal continuation-passing style yet, but understand this:

$$
\boxed{\text{The call stack stores suspended computations}}
$$

That's why recursion can later be transformed into:

```text
explicit stacks
state machines
continuation objects
coroutines
trampolines
```

This connects DSA to compilers, runtimes, interpreters and systems programming.

---



# 26. A Complete Example

Let's analyze merge sort.

```cpp
void mergeSort(vector<int>& a, int l, int r) {
    if (l >= r)
        return;

    int mid = l + (r - l) / 2;

    mergeSort(a, l, mid);
    mergeSort(a, mid + 1, r);

    merge(a, l, mid, r);
}
```



## Base case

```cpp
l >= r
```

A zero/one-element array is already sorted.

## Recursive hypothesis

Assume:

```cpp
mergeSort(a, l, mid)
```

correctly sorts the left half.

Assume:

```cpp
mergeSort(a, mid + 1, r)
```

correctly sorts the right half.

## Combination

Merge the two sorted halves.

Therefore the entire range becomes sorted.

## Recurrence

Two subproblems each roughly half-size:

$$
2T(n/2)
$$

Merge costs:

$$
O(n)
$$

Therefore:

$$
T(n)=2T(n/2)+O(n)
$$

At each recursion-tree level:

```text
level 0             n
level 1         n/2 + n/2              = n
level 2     n/4+n/4+n/4+n/4            = n
...
```

Number of levels:

$$
\log n
$$

Work per level:

$$
n
$$

Thus:

$$
\boxed{O(n\log n)}
$$

Stack depth:

$$
\boxed{O(\log n)}
$$

Ignoring the auxiliary merge buffer, of course.

This one example combines:

```text
base case
recursive case
recursive tree
recurrence relation
time complexity
space complexity
multiple recursion
unwinding
divide & conquer
```

---



# 27. Recursion Checklist

Whenever you encounter a recursive function, train yourself to answer these questions:

1. **What exactly does this function promise to compute?**
2. **What is the state?**
3. **What is the base case?**
4. **Why is the base case correct?**
5. **How does each call move toward termination?**
6. **How many recursive calls does one invocation create?**
7. **How much does the input shrink?**
8. **What happens before recursion?**
9. **What happens after recursion returns?**
10. **What recurrence describes the runtime?**
11. **What is the recursion-tree height?**
12. **What is the total number of calls/work?**
13. **What is the maximum number of simultaneously active frames?**
14. **Could stack overflow occur?**
15. **Are subproblems repeated?**
16. **Would memoization help?**
17. **Could this be expressed iteratively?**
18. **What explicit state would replace the call stack?**
19. **Is recursion actually clearer than iteration here?**

If you can answer those for an unfamiliar recursive algorithm, you understand recursion properly.

---



# What you should be able to do before marking Recursion “complete”

For your deep-understanding DSA track, I would **not** consider recursion finished until you can independently do all of this:

```text
                         RECURSION
                             │
              ┌──────────────┼──────────────┐
              ↓              ↓              ↓
           Runtime        Reasoning        Analysis
              │              │              │
        call stack       base case       recurrence
        stack frame      progress        recursion tree
        return addr      hypothesis      time complexity
        unwinding        combination     stack complexity
              │              │              │
              └──────────────┼──────────────┘
                             ↓
                     Transformations
                             │
                ┌────────────┼────────────┐
                ↓            ↓            ↓
            tail recursion iteration  memoization
                             │
                             ↓
                         APPLICATION
                             │
           Trees / DFS / D&C / DP / Backtracking
```

And specifically, you should be able to write from scratch:

- factorial and sum recursively
- reverse/print an array recursively
- binary search recursively
- exponentiation using divide-and-conquer
- linked-list traversal/reversal recursively
- tree DFS recursively
- merge sort
- naive Fibonacci and then memoized Fibonacci
- convert recursive DFS to explicit-stack DFS
- convert at least one non-tail recursive algorithm into iteration
- derive the recurrence and stack complexity for each

The most important conceptual takeaway is:

$$
\boxed{
\text{Recursion isn't “calling yourself.”}
}
$$

It is:

$$
\boxed{
\text{decompose state → suspend current computation →
solve smaller state → unwind → combine results}
}
$$

And the **call stack is the mechanism that remembers all suspended computations**.

That's the foundation you need before moving into trees, divide-and-conquer, DFS, backtracking and dynamic programming.