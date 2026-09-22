# Sliding Window

## 1. What is a "window"?

A window is simply a **contiguous portion** of an array or string.

For:

```text
[2, 1, 5, 1, 3, 2]
```

this is a window:

```text
  [1, 5, 1]
```

because the elements are adjacent.

This is **not** a window:

```text
2, 5, 3
```

because we're skipping elements.

So the first recognition clue is:

> **subarray / substring / consecutive / contiguous**

Those words should make you consider Sliding Window.

---

# 2. Why Sliding Window exists

Suppose:

> Find the maximum sum of any 3 consecutive elements.

Array:

```text
[2, 1, 5, 1, 3, 2]
```

Brute force calculates:

```text
2 + 1 + 5 = 8
    1 + 5 + 1 = 7
        5 + 1 + 3 = 9
            1 + 3 + 2 = 6
```

Imagine `k` were 1,000.

For every starting position, recomputing all `k` elements would cost:

```text
O(nk)
```

But consecutive windows overlap heavily.

Look:

```text
Window 1:

[2 1 5] 1 3 2

Window 2:

2 [1 5 1] 3 2
```

Most information didn't change.

Only:

```text
2 leaves
1 enters
```

So instead of recomputing:

```text
1 + 5 + 1
```

we can take the previous sum:

```text
8
```

subtract outgoing:

```text
8 - 2 = 6
```

add incoming:

```text
6 + 1 = 7
```

That's Sliding Window.

---



# 3. The fundamental idea

You maintain information about:

```text
[L ........ R]
```

When the window moves:

```text
old element leaves
new element enters
```

Instead of rebuilding everything, **update the state incrementally**.

That's the central idea.

---



# 4. Two major types

You should master:

```text
Sliding Window
│
├── Fixed-size window
│
└── Variable-size window
```

They look similar but solve different kinds of problems.

---



# 5. Fixed-Size Sliding Window

Here the problem tells you the window size.

Usually:

```text
k
```

Examples:

- Maximum sum of `k` consecutive elements
- Average of every `k` elements
- Number of something in every length-`k` substring
- Maximum vowels in substring of length `k`

Window size always remains:

```text
R - L + 1 = k
```

---



# 6. Fixed Window Example

Find maximum sum of 3 consecutive values:

```text
[2, 1, 5, 1, 3, 2]
```

First window:

```text
L     R
↓     ↓
2  1  5  1  3  2

sum = 8
```

Now slide.

Remove `2`, add `1`:

```text
   L     R
   ↓     ↓
2  1  5  1  3  2

sum = 8 - 2 + 1
    = 7
```

Again:

```text
      L     R
      ↓     ↓
2  1  5  1  3  2

sum = 7 - 1 + 3
    = 9
```

Again:

```text
         L     R
         ↓     ↓
2  1  5  1  3  2

sum = 9 - 5 + 2
    = 6
```

Answer:

```text
9
```

---



# 7. Fixed Window Template

Conceptually:

```cpp
int left = 0;
int sum = 0;

for (int right = 0; right < n; right++) {

    sum += a[right];

    if (right - left + 1 == k) {

        // use current window

        sum -= a[left];
        left++;
    }
}
```

But don't memorize this yet.

Understand the lifecycle:

```text
ADD new right element
       ↓
window reaches required size
       ↓
PROCESS answer
       ↓
REMOVE left element
       ↓
move left
       ↓
continue
```

---



# 8. Why Fixed Sliding Window is O(n)

A common mistake is seeing:

```text
left
right
```

and thinking O(n²).

But each element:

- enters the window once
- leaves the window once

So an element undergoes at most constant work.

Total:

```text
roughly 2n operations
```

Therefore:

```text
O(n)
```

Same reasoning you saw with Two Pointers.

---



# 9. Fixed Window State

The window doesn't have to maintain only a sum.

You might maintain:

```text
sum
count
frequency map
number of zeros
number of vowels
distinct count
```

For example, given:

```text
"abciiidef"
```

find the maximum vowels in any substring of length 3.

Instead of recounting three characters each time, maintain:

```text
vowelCount
```

When right enters:

```text
if vowel:
    vowelCount++
```

When left leaves:

```text
if vowel:
    vowelCount--
```

Same pattern.

---



# 10. Variable-Size Sliding Window

Now things get more interesting.

The window size isn't fixed.

Instead, there is a **condition**.

Example:

> Find the shortest subarray whose sum is at least 7.

Suppose:

```text
[2, 3, 1, 2, 4, 3]
```

You don't know whether the answer has size:

```text
1
2
3
4
...
```

So:

```text
L → [ ........ ] ← R
```

changes size dynamically.

---



# 11. Expand and Shrink

Variable windows usually follow this philosophy:

```text
Expand R
   ↓
make the window satisfy some condition
   ↓
Shrink L while condition still allows it
   ↓
record answer
   ↓
continue expanding
```

Or for longest-window problems:

```text
Expand R
   ↓
if window becomes invalid
   ↓
shrink L until valid again
   ↓
record longest valid size
```

There are variations, but this expand/shrink relationship is the important idea.

---



# 12. Example — Minimum Subarray Sum ≥ 7

Input:

```text
[2, 3, 1, 2, 4, 3]
```

Target:

```text
7
```

Start empty.

Add 2:

```text
[2]

sum = 2
```

Not enough.

Expand:

```text
[2,3]

sum = 5
```

Still not enough.

Expand:

```text
[2,3,1]

sum = 6
```

Expand:

```text
[2,3,1,2]

sum = 8
```

Now condition is satisfied:

```text
sum >= 7
```

Window length = 4.

Can we shrink?

Remove left `2`:

```text
[3,1,2]

sum = 6
```

No longer valid.

So stop shrinking.

Continue expanding.

Add 4:

```text
[3,1,2,4]

sum = 10
```

Valid.

Shrink:

```text
[1,2,4]

sum = 7
```

Still valid!

Length = 3.

Shrink again:

```text
[2,4]

sum = 6
```

Invalid.

Continue.

Eventually:

```text
[4,3]
```

sum = 7.

Length = 2.

Answer:

```text
2
```

---



# 13. Why were we allowed to shrink?

This is your most important question.

For this specific problem, all numbers are **positive**.

That gives us monotonic behavior.

If we remove a value from the left:

```text
sum decreases
```

If we add a positive value on the right:

```text
sum increases
```

Therefore we can reason about how the condition changes predictably.

That's why Sliding Window works nicely here.

---



# 14. What happens with negative numbers?

Suppose:

```text
[5, -10, 20]
```

Now expanding right can make the sum:

```text
increase
OR
decrease
```

Shrinking left can also:

```text
increase
OR
decrease
```

Our simple monotonic reasoning disappears.

So:

> "Subarray sum problem" does **not automatically mean Sliding Window**.

This is crucial.

For example, many problems with arbitrary positive/negative values need:

```text
Prefix Sum + Hash Map
```

rather than ordinary Sliding Window.

---



# 15. The real requirement

Sliding Window becomes powerful when the window has a property that can be maintained incrementally.

You need two things:

### 1. Local updateability

When an element enters or leaves, can you update state cheaply?

Example:

```text
sum += entering
sum -= leaving
```

or:

```text
freq[c]++
freq[c]--
```



### 2. Useful directional behavior

Can expanding/shrinking tell you something predictable about validity?

For example:

```text
all positive numbers + sum constraint
```

or:

```text
number of distinct characters <= k
```

---



# 16. Longest Substring Without Repeating Characters

This is the classic variable-window string problem.

Input:

```text
"abcabcbb"
```

Wanted:

```text
"abc"
```

length:

```text
3
```

Start:

```text
[a]
```

Valid.

Expand:

```text
[ab]
```

Valid.

Expand:

```text
[abc]
```

Valid.

Expand:

```text
[abca]
```

Now:

```text
a appears twice
```

Window invalid.

So shrink from left until the duplicate is removed:

```text
a [bca]
```

Now:

```text
[bca]
```

valid again.

Continue.

---



# 17. What's the window state here?

Not a sum.

We need to know:

> Is every character unique?

So maintain:

```text
frequency map
```

or:

```text
set
```

Suppose:

```text
window = "abca"
```

Frequency:

```text
a → 2
b → 1
c → 1
```

Invalid because:

```text
a = 2
```

Remove left `a`:

```text
a → 1
```

Now valid.

Same Sliding Window idea, different state.

---



# 18. Generic variable-window shape

A conceptual version:

```cpp
int left = 0;

for (int right = 0; right < n; right++) {

    // add a[right] to window state

    while (window_is_invalid()) {

        // remove a[left] from state
        left++;
    }

    // window [left, right] is valid
    // update answer
}
```

For other problems, you might shrink while the condition **is valid**, especially minimum-window problems.

isvlaid template :  

```cpp
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int left=0;
        int sum =0;

        int ans = numeric_limits<int>::max();

        for(int right=0;right<nums.size();right++){
            sum +=nums[right];

            while(sum>=target){
                ans = min(ans , right - left + 1 );
                sum -=nums[left];
                left++;
            }

            
        }

        return ans == numeric_limits<int>::max() ? 0 : ans;
    }
};
```

So again:

> Don't blindly memorize `while (invalid)`.

Understand what you're optimizing.

---



# 19. Longest vs Shortest changes the logic

This is very important.

## Longest valid window

Example:

> Longest substring containing at most 2 distinct characters.

Philosophy:

```text
Expand as much as possible.

When invalid:
    shrink until valid.

Then measure the valid window.
```

---



## Shortest window satisfying a requirement

Example:

> Minimum-length subarray with sum >= target.

Philosophy:

```text
Expand until valid.

Once valid:
    keep shrinking while still valid
    because we're trying to minimize length.
```

So the goal influences when you update the answer.

---



# 20. Sliding Window Invariant

Just like Two Pointers, you should think in terms of an invariant.

For:

> longest substring without duplicates

an invariant might be:

```text
After the shrinking loop finishes,
window [L, R] contains no duplicates.
```

For:

> at most K distinct characters

the invariant:

```text
distinctCount <= K
```

For fixed length:

```text
window length never exceeds k
```

Understanding the invariant makes the algorithm much easier to derive.

---



# 21. Variable Window with frequency maps

A huge class of string problems combines:

```text
Sliding Window
+
Hash Map / Frequency Array
```

Suppose:

```text
"aabacbebebe"
```

and you want some condition involving distinct characters.

Maintain:

```text
freq:

a → ?
b → ?
c → ?
...
```

When `R` moves:

```text
freq[s[R]]++
```

When `L` moves:

```text
freq[s[L]]--
```

If count reaches 0:

```text
remove / decrement distinctCount
```

This is the core machinery behind many substring problems.

---



# 22. Important Sliding Window state types

Learn these.


| Problem needs          | Window state                   |
| ---------------------- | ------------------------------ |
| Sum                    | running sum                    |
| Average                | sum + length                   |
| At most K zeros        | zero count                     |
| Distinct values        | frequency map + distinct count |
| No duplicates          | set/frequency map              |
| Character requirements | frequency maps                 |
| Max/min                | often monotonic deque          |


That last one deserves attention.

---



# 23. Why max/min is harder

Suppose your window is:

```text
[8, 3, 5]
```

Maximum:

```text
8
```

Window moves:

```text
[3, 5, 4]
```

The `8` left.

What's the new maximum?

You can't determine that from only:

```text
oldMax = 8
```

You have to know more about the remaining elements.

A naive rescan costs O(k).

This is why:

> **Sliding Window Maximum → Monotonic Deque**

which you'll study with Deques.

It lets us maintain the best candidates efficiently.

---



# 24. Fixed Window vs Variable Window

Think of them this way:

### Fixed

Problem gives:

```text
exactly k elements
```

So:

```text
window size determines when L moves
```



### Variable

Problem gives:

```text
a property/constraint
```

So:

```text
window validity determines when L moves
```

This is the clean distinction.

---



# 25. Another important category: At Most K

Suppose:

> Longest substring with at most K distinct characters.

Window valid when:

```text
distinct <= K
```

Expand:

```text
R++
```

When:

```text
distinct > K
```

shrink:

```text
L++
```

until:

```text
distinct <= K
```

Then update:

```text
maxLength
```

This "at most K" form is particularly friendly to Sliding Window because it has a natural notion of becoming too large/invalid.

---



# 26. Exact K can sometimes be trickier

Suppose:

> Count subarrays containing exactly K distinct integers.

"Exactly K" doesn't always directly give you the cleanest window behavior.

A common mathematical transformation is:

```text
exactly(K)
=
atMost(K) - atMost(K-1)
```

This is an important advanced Sliding Window idea.

Don't worry if it doesn't feel obvious immediately; learn it after basic fixed/variable windows are comfortable.

### Another way to think about it

Imagine people allowed into a ride based on height:

```
people <= 180 cm
```

contains everyone:

```
150
```

160

170

180

And:

```
people <= 179 cm
```

contains:

```
150
```

160

170

Subtract the groups:

```
(<= 180) - (<= 179)
```

Who's left?

```
exactly 180
```

Same concept:

```
<= K  -  <= K-1
```

```
   =

   == K
```

So whenever you see:

```
exactly(K) = atMost(K) - atMost(K - 1);
```

read it in English as:

> **Count everything up through K, then remove everything below K. What's left is exactly K.**

That's the core idea.

---



# 27. Minimum Window problems

A more difficult family:

> Find the smallest substring containing all required characters.

Example:

```text
S = "ADOBECODEBANC"
T = "ABC"
```

Need a window containing:

```text
A
B
C
```

The window may contain additional characters.

Basic idea:

```text
Expand R until requirements satisfied
       ↓
Shrink L while requirements remain satisfied
       ↓
record smallest
       ↓
continue
```

This combines:

```text
Sliding Window
+
Frequency Maps
+
Requirement Tracking
```

Representative problem:

**Minimum Window Substring**

Do this after you're comfortable with easier variable windows.

---



# 28. Sliding Window and Two Pointers

Remember:

```text
L ... R
```

means sliding window is often technically a **two-pointer algorithm**.

But the window carries extra meaning:

```text
Everything between L and R
is the active contiguous state.
```

Two Sum:

```text
L →       ← R
```

doesn't treat everything between them as one active object.

1. Sliding Window does.

That's why we treat Sliding Window as its own pattern.

---



# 29. Brute force → Sliding Window transformation

This is a useful way to recognize it.

Suppose brute force says:

```text
For every left:
    For every right:
        calculate something about [left ... right]
```

That's potentially:

```text
O(n²)
```

Then ask:

> When `right` increases or `left` increases, can I update the previous range's information instead of recomputing it?

If yes, Sliding Window may drastically reduce the work.

---



# 30. Why many Sliding Window algorithms are O(n)

Consider:

```text
R → → → → → →
L → → → → →
```

Right only moves forward.

Left only moves forward.

Neither pointer moves backward.

Even though `L` may sit inside a `while` loop, total movement of `L` over the entire algorithm is at most `n`.

So:

```text
R moves ≤ n
L moves ≤ n
```

Total:

```text
O(n)
```

This is a crucial amortized-style argument.

---



# 31. Don't get scared by the nested while

You may see:

```cpp
for (...) {

    while (...) {
        left++;
    }
}
```

and think:

```text
O(n²)
```

Not necessarily.

If `left` never resets and only moves forward, the inner loop executes at most `n` times **across the entire algorithm**.

So total:

```text
O(n)
```

This is one of the most important complexity insights in Sliding Window.

---



# 32. When Sliding Window does NOT work

Do not force it.

Be suspicious when:

### The problem isn't contiguous

Example:

```text
choose arbitrary subsequence
```

Sliding Window usually isn't appropriate.

### Window state can't be updated efficiently

If removing one item requires rebuilding everything, another structure may be needed.

### Expanding/shrinking has unpredictable effects

Especially with some numerical constraints containing arbitrary negative values.

### You need arbitrary historical positions

A different pattern such as prefix sums, hashing, binary search, or DP may fit better.

---



# 33. Sliding Window vs Prefix Sum

These are sometimes confused.

Suppose:

> Answer many range sum queries.

Prefix Sum is great:

```text
prefix[R] - prefix[L-1]
```

Suppose:

> Continuously process overlapping windows while optimizing something.

Sliding Window may be better.

Simple distinction:

```text
Prefix Sum:
Fast queries about arbitrary ranges.

Sliding Window:
Incrementally maintain a moving contiguous range.
```

Sometimes both can solve the same problem with different trade-offs.

---



# 34. Sliding Window vs Binary Search

Sometimes a problem asks:

> Find minimum possible size/value satisfying a monotonic condition.

You might have two options:

```text
Sliding Window
```

or:

```text
Binary Search on Answer
```

depending on the structure.

Sliding Window tends to exploit local incremental movement.

Binary search exploits global monotonicity in the answer space.

You'll understand this distinction better when you study Binary Search deeply.

---



# 35. Core problems to practice

You don't need 50.

### Fixed-size

- [x] **1. Maximum Sum Subarray of Size K** — Learn basic window maintenance.

- [x] **2. Maximum Average Subarray I** — Same pattern.

- [x] **3. Maximum Number of Vowels in a Substring of Given Length** — Learn maintaining counts instead of sums.

---



### Variable-size beginner

- [x] **4. Minimum Size Subarray Sum** — Learn expand + shrink with positive values.

- [x] **5. Longest Substring Without Repeating Characters** — Learn window + set/frequency.

---



### Variable-size intermediate

- [x] **6. Longest Repeating Character Replacement** — Learn a more subtle validity condition.

- [x] **7. Fruit Into Baskets** — Essentially: at most 2 distinct values.

- [x] **8. Max Consecutive Ones III** — Learn: at most K violations.

---



### Advanced

- [x] **9. Minimum Window Substring** — Learn requirements/frequency matching.

- [ ] **10. Subarrays with K Different Integers** — Learn: exactly K = atMost(K) - atMost(K-1).
- [ ] 11. **[862. Shortest Subarray with Sum at Least K](https://leetcode.com/problems/shortest-subarray-with-sum-at-least-k/)**

Don't jump to these before the basics.

---



# 36. Recognition clues

Train yourself to react to wording.

```text
"consecutive"
"contiguous"
"subarray"
"substring"
        ↓
Potential Sliding Window
```

Then:

```text
"of size K"
"exactly K positions"
        ↓
Fixed Window
```

And:

```text
"longest"
"shortest"
"at most"
"at least"
"without repeating"
        ↓
Potential Variable Window
```

Then ask the real question:

> Can validity/state be maintained as L and R only move forward?

If yes, you're much more confident.

---



# 37. A strong problem-solving checklist

When you encounter a possible Sliding Window problem, ask:

```text
1. Is the answer a contiguous range?

2. Fixed size or variable size?

3. What exactly does my window represent?

4. What state must I maintain?
   Sum?
   Count?
   Frequency?
   Distinct count?

5. What happens when R enters?

6. What happens when L leaves?

7. What makes the window valid?

8. What makes it invalid?

9. When should I expand?

10. When should I shrink?

11. When should I update the answer?
    Before shrinking?
    During shrinking?
    After shrinking?

12. Why is discarding the left element safe?

13. Can L and R both move only forward?

14. Time complexity?

15. What breaks if negative values / duplicates / etc. appear?
```

Questions 7–12 are where actual Sliding Window understanding happens.

---



# 38. The invariant is everything

For each problem, write one sentence before coding.

For example:

### Longest substring without repeating characters

> After shrinking, `[L,R]` contains no repeated characters.



### At most K distinct

> After shrinking, `[L,R]` contains at most K distinct values.



### Fixed K window

> Whenever I evaluate the answer, `[L,R]` contains exactly K elements.



### Minimum sum ≥ target

> While shrinking, the current window still satisfies the target condition; stop once it no longer does.

If you can state that clearly, your implementation becomes much less mysterious.

---



# Your complete Sliding Window checklist

```text
SLIDING WINDOW
│
├── Foundation
│   ├── Contiguous range
│   ├── L / R boundaries
│   ├── Incremental state updates
│   └── Why overlapping ranges matter
│
├── Fixed Window
│   ├── Window size = K
│   ├── Running sum
│   ├── Running count
│   ├── Frequency state
│   └── Add right / remove left
│
├── Variable Window
│   ├── Expand right
│   ├── Check condition
│   ├── Shrink left
│   ├── Restore/maintain validity
│   └── Update answer appropriately
│
├── Common State
│   ├── Sum
│   ├── Frequency Map
│   ├── Hash Set
│   ├── Distinct Count
│   ├── Violation Count
│   └── Monotonic Deque for max/min
│
├── Common Families
│   ├── Fixed K
│   ├── At Most K
│   ├── Longest valid
│   ├── Shortest satisfying
│   ├── No duplicates
│   ├── Character requirements
│   └── Exactly K via transformations
│
├── Deep Understanding
│   ├── Window invariant
│   ├── Why expanding is safe
│   ├── Why shrinking is safe
│   ├── Why discarded positions needn't return
│   ├── Why both pointers only move forward
│   └── Why total complexity is often O(n)
│
└── Recognition
    ├── Subarray / substring
    ├── Consecutive / contiguous
    ├── Fixed K → fixed window
    ├── Longest/shortest → variable candidate
    ├── At most K → strong candidate
    └── Arbitrary negatives / non-contiguous → reconsider
```

The single sentence I want you to remember is:

> **Sliding Window works when a contiguous range can be updated incrementally as its boundaries move, and moving those boundaries lets us safely discard old possibilities without needing to revisit them.**

That's the reasoning. The `left/right/while` code is just the implementation of that reasoning.