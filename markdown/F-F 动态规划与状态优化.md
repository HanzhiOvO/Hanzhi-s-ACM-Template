## F 动态规划与状态优化

### 13 动态规划基础与状态设计

背包、序列、区间、状压、轮廓线、数位、概率期望和小状态 MDP 放在这里，优先解决“状态怎么设”。
#### 0/1 背包

> **赛时先看**
> - **题目信号**：题面说"每个物品选/不选一次""每种最多用一次"，容量上限 `W` 到几十万，问容量限制下的最大价值。看到"每个物品只有一次机会"就是 0/1 背包。
> - **本质**：容量维倒序滚动数组：外层遍历物品，内层容量从大到小推，保证 `dp[j-w]` 还是上一轮没选过当前物品的旧值，实现"每件最多选一次"。
> - **复杂度判定**：`O(nW)` 时间、`O(W)` 空间；`W ≤ 1e5~1e6` 常用，`W` 上千万或更大就要换做法（价值维、倍增等）。
> - **维护的量**：`dp[j]`（容量不超过 j 时的最大价值）。
> - **接法**：把每个物品的花费放进 `weight[i]`，收益放进 `value[i]`，容量是 `W`；调用后 `dp[j]` 表示容量不超过 `j` 的最大收益，通常答案是 `dp[W]`。
> - **警告**：0/1 背包容量必须倒序循环；改成正序就变成完全背包。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个物品（第 i 个花费 `w[i]`、价值 `v[i]`，每个最多选一次），容量 `W`，求最大总价值。

```cpp
vector<int> weight(n + 1);
vector<i64> value(n + 1);
for (int i = 1; i <= n; ++i) cin >> weight[i] >> value[i];
auto dp = zero_one_knapsack(W, weight, value);  // 1. 调用：物品数组 1-indexed
cout << dp[W] << '\n';                          // 2. 取结果：容量 W 的最大价值
```

样例：物品 `(2,3) (3,4) (4,5)`，`W = 5` -> 输出 `7`（选前两个）。

**传参要求（照这个传不会错）：**

- `zero_one_knapsack(W, weight, value)`：`W` = 容量上限；`weight/value` 必须 1-indexed（长度 `n+1`，`[0]` 不用）。
- 返回值：`vector<i64> dp`；`dp[j]` = 容量不超过 `j` 的最大价值，答案取 `dp[W]`。
- 每个物品只能选一次；可重复选换 `complete_knapsack`；每件有限个换 `multiple_knapsack`。
- 物品数量多但 `W` 巨大时 O(nW) 会超时，考虑价值维或二进制优化。


**不会用就照抄：**

```cpp
// dp[j]：容量不超过 j 的最大价值；外层物品、内层容量倒序
vector<i64> dp(W + 1, 0);
for (auto [w, v] : items)
    for (int j = W; j >= w; --j)
        dp[j] = max(dp[j], dp[j - w] + v);
```

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `zero_one_knapsack` 函数。
2. 构造：`weight[i]`、`value[i]` 按题面填好，`W` 是容量上限。
3. 调用：`auto dp = zero_one_knapsack(W, weight, value);`
4. 取结果：答案通常就是 `dp[W]`（容量不超过 W 的最大价值）。

**改造点（按题目改这几处）：**
- 价值可能为负：初值全 0 会把负价值吞掉，把 `dp` 初值改成 `-INF`（如 `LLONG_MIN / 4`），只有 `dp[0] = 0`。
- 要求恰好装满：同样把初值改成 `-INF`、`dp[0] = 0`，答案取 `dp[W]`，不可达状态保持 `-INF`。
- 需要输出方案：加一个 `pre[j]` 数组记录 `dp[j]` 最后被哪个物品更新，算完后倒序还原选中的物品。
- `W` 很大（1e6+）：空间 `O(W)` 吃紧时先看内存限制；`W` 上亿考虑价值维背包或其他做法。


**API / 入口函数（赛时只认这里列的名字）：**

- `zero_one_knapsack(int W, const vector<int>& weight, const vector<i64>& value)` -> 0/1 背包 返回 `vector<i64>`。
**核心逻辑（改代码时别破坏）：**

- 每件物品只能用一次，所以容量必须倒序，保证本轮不会再次使用当前物品。

```cpp
// 维护的量：dp[j] = 容量不超过 j 时的最大价值。
// 不变量：处理完前 i 个物品后，dp[j] 只由前 i 个物品构成，且每件最多用一次。
vector<i64> zero_one_knapsack(int W, const vector<int>& weight, const vector<i64>& value) {
    vector<i64> dp(W + 1, 0);
    for (int i = 0; i < (int)weight.size(); ++i) {
        // 容量倒序：dp[j - weight[i]] 还是上一轮的旧值，当前物品不会被重复选
        for (int j = W; j >= weight[i]; --j) {
            dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
        }
    }
    return dp;
}
```
#### 完全背包

> **赛时先看**
> - **题目信号**：硬币不限量、材料可重复使用。
> - **本质**：每个物品可选无限次。
> - **接法**：硬币、材料、技能可重复使用时用完全背包。正序循环代表当前物品可以被重复使用；如果你把它写成倒序，就会错误地变成每种只能选一次。
> - **复杂度判定**：`O(nW)`。
> - **维护的量**：`dp[j]`（容量不超过 j 时的最大价值）。
> - **警告**：完全背包容量正序循环。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 种物品（第 i 种花费 `w[i]`、价值 `v[i]`，每种不限量），容量 `W`，求最大总价值。

```cpp
vector<int> weight(n + 1);
vector<i64> value(n + 1);
for (int i = 1; i <= n; ++i) cin >> weight[i] >> value[i];
auto dp = complete_knapsack(W, weight, value);  // 1. 调用：物品数组 1-indexed
cout << dp[W] << '\n';                          // 2. 取结果：容量 W 的最大价值
```

样例：物品 `(2,3) (3,4) (4,5)`，`W = 6` -> 输出 `9`（三个 `(2,3)`）。

**传参要求（照这个传不会错）：**

- `complete_knapsack(W, weight, value)`：`W` = 容量上限；`weight/value` 必须 1-indexed（长度 `n+1`，`[0]` 不用）。
- 返回值：`vector<i64> dp`；`dp[j]` = 容量不超过 `j` 的最大价值，答案取 `dp[W]`。
- 每种物品可取任意次；只能取一次换 `zero_one_knapsack`；每种有限个换 `multiple_knapsack`。

**不会用就照抄：**

```cpp
for (auto [w, v] : items)
    for (int j = w; j <= W; ++j)
        dp[j] = max(dp[j], dp[j - w] + v);
```

- **完全背包容量正序**；和 0/1 背包最核心的区别就是循环方向。


**API / 入口函数（赛时只认这里列的名字）：**

- `complete_knapsack(int W, const vector<int>& weight, const vector<i64>& value)` -> 完全背包 返回 `vector<i64>`。
**核心逻辑（改代码时别破坏）：**

- 每件物品可无限次，所以容量正序，让本轮新状态继续转移到更大容量。

```cpp
vector<i64> complete_knapsack(int W, const vector<int>& weight, const vector<i64>& value) {
    vector<i64> dp(W + 1, 0);
    for (int i = 0; i < (int)weight.size(); ++i) {
        for (int j = weight[i]; j <= W; ++j) {
            dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
        }
    }
    return dp;
}
```
#### 多重背包：二进制拆分

> **赛时先看**
> - **题目信号**：物品数量有限但可能很大。
> - **本质**：每种物品最多选 `cnt[i]` 次。
> - **接法**：每种物品给了数量 `cnt[i]` 时用这个模板。它把 `cnt` 拆成 `1,2,4,...` 若干包，再跑 0/1 背包；题目容量很大、数量也很大时，这个版本可能不够快，翻“多重背包单调队列优化”。
> - **复杂度判定**：`O(W * sum log cnt[i])`。
> - **维护的量**：`dp[j]`（容量不超过 j 时的最大价值）；辅助 `nw/nv`（把 `cnt[i]` 按 `1,2,4,...` 拆出的新物品包）。
> - **警告**：拆分后按 0/1 背包倒序做。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 种物品（第 i 种重量 `w[i]`、价值 `v[i]`、最多 `cnt[i]` 个），容量 `W`，求最大总价值。

```cpp
vector<int> weight(n + 1), cnt(n + 1);
vector<i64> value(n + 1);
for (int i = 1; i <= n; ++i) cin >> weight[i] >> value[i] >> cnt[i];
auto dp = multiple_knapsack(W, weight, value, cnt);  // 1. 调用：三个数组 1-indexed
cout << dp[W] << '\n';                               // 2. 取结果：容量 W 的最大价值
```

样例：物品 `(1,2,2) (2,3,2)`（重量，价值，个数），`W = 5` -> 输出 `8`（`(1,2)` 一个 + `(2,3)` 两个）。

**传参要求（照这个传不会错）：**

- `multiple_knapsack(W, weight, value, cnt)`：`W` = 容量上限；三个数组等长、1-indexed（长度 `n+1`，`[0]` 不用）。
- 返回值：`vector<i64> dp`；`dp[j]` = 容量不超过 `j` 的最大价值，答案取 `dp[W]`。
- `cnt[i]` = 第 i 种物品最多能拿的个数；内部拆成 `1,2,4,...` 再跑 0/1 背包。
- `W` 和 `cnt[i]` 都很大、`O(W log cnt)` 过不了时，换 `multiple_knapsack_monotone_queue`。

```cpp
vector<i64> multiple_knapsack(int W, vector<int> weight, vector<i64> value, vector<int> cnt) {
    vector<int> nw;
    vector<i64> nv;
    for (int i = 0; i < (int)weight.size(); ++i) {
        int c = cnt[i];
        for (i64 k = 1; c > 0; k <<= 1) {
            int take = (int)min(k, (i64)c);
            nw.push_back((int)((i64)weight[i] * take));
            nv.push_back(value[i] * (i64)take);
            c -= take;
        }
    }
    return zero_one_knapsack(W, nw, nv);
}
```
#### 多重背包单调队列优化：数量很大、容量也大

> **赛时先看**
> - **题目信号**：多重背包，`W` 很大、物品数量上限也大，且题目需要 `O(nW)` 级别而非 `O(nW log count)`；重量为正整数。
> - **本质**：每种物品最多取 `count[i]` 个，重量/价值均为整数；二进制拆分的 `log count` 仍然不够快时，把同余类转化为滑动窗口最大值。
> - **复杂度判定**：`O(nW)` 时间、`O(W)` 空间。
> - **维护的量**：`dp[j]`（容量不超过 j 时的最大价值）；同余类滑窗 `candidates`（存 `(t, previous[r+t*w] - t*v)`，值单调递减）。
> - **警告**：同一类物品更新时必须从旧数组 `previous` 转移，不能原地污染；对固定余数 `r`，位置 `r + t*w` 的候选窗口只允许前 `count` 步。若题目是“恰好容量”，初始值和不可达状态应改为 `-INF`，不能直接用全零。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 种物品（第 i 种重量 `w[i]`、价值 `v[i]`、最多 `count[i]` 个），容量 `W`，求最大总价值。

```cpp
vector<int> weight(n + 1), count(n + 1);
vector<i64> value(n + 1);
for (int i = 1; i <= n; ++i) cin >> weight[i] >> value[i] >> count[i];
auto dp = multiple_knapsack_monotone_queue(W, weight, value, count);  // 1. 调用：数组 1-indexed
cout << dp[W] << '\n';                                                // 2. 取结果：容量 W 的最大价值
```

样例：物品 `(2,3,3) (3,4,2)`（重量，价值，个数），`W = 6` -> 输出 `9`（三个 `(2,3)`）。

**传参要求（照这个传不会错）：**

- `multiple_knapsack_monotone_queue(W, weight, value, count)`：`W` = 容量上限；三个数组等长、1-indexed（长度 `n+1`，`[0]` 不用）。
- 返回值：`vector<i64> dp`；`dp[j]` = 容量不超过 `j` 的最大价值，答案取 `dp[W]`。
- 要求重量是正整数（`w >= 1`）；要“恰好装满”时把 `dp` 初值改成 `-INF`、只留 `dp[0] = 0`。

```cpp
vector<i64> multiple_knapsack_monotone_queue(
    int W, const vector<int>& weight, const vector<i64>& value, const vector<int>& count
) {
    vector<i64> dp(W + 1, 0); // 至多容量 W，允许不装满。
    for (int item = 0; item < (int)weight.size(); ++item) {
        int w = weight[item], c = count[item];
        i64 v = value[item];
        vector<i64> previous = dp;
        for (int remainder = 0; remainder < w && remainder <= W; ++remainder) {
            deque<pair<int, i64>> candidates; // (t, previous[r+t*w] - t*v)，值单调递减。
            for (int t = 0, position = remainder; position <= W; ++t, position += w) {
                i64 key = previous[position] - (i64)t * v;
                while (!candidates.empty() && candidates.back().second <= key) {
                    candidates.pop_back();
                }
                candidates.push_back({t, key});
                while (candidates.front().first < t - c) candidates.pop_front();
                dp[position] = candidates.front().second + (i64)t * v;
            }
        }
    }
    return dp;
}
```

#### 分组背包

> **赛时先看**
> - **题目信号**：题面说“每组选一个/最多一个”，如套餐、课程类别。
> - **本质**：物品分成若干组，每组最多选一个。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O（总物品数 * W)`。
> - **维护的量**：`dp[j]`（容量不超过 j 时的最大价值）；`old`（上一组处理完的 dp 快照，保证每组最多选一个）。
> - **警告**：每组内要从上一组状态转移，容量倒序或复制旧数组。


**最小完整示例（先抄这一段就能跑）：**

题目：`m` 组物品，第 i 组是若干 `(w, val)` 对，每组最多选一个，容量 `W`，求最大总价值。

```cpp
vector<vector<pair<int, i64>>> groups(m);
for (int i = 0; i < m; ++i) {
    int k; cin >> k;
    groups[i].resize(k);
    for (auto& [w, val] : groups[i]) cin >> w >> val;
}
auto dp = group_knapsack(W, groups);   // 1. 调用：groups 0-indexed
cout << dp[W] << '\n';                 // 2. 取结果：容量 W 的最大价值
```

样例：组 1 `{(2,3),(3,4)}`、组 2 `{(2,5)}`，`W = 4` -> 输出 `8`（组 1 选 `(2,3)`，组 2 选 `(2,5)`）。

**传参要求（照这个传不会错）：**

- `group_knapsack(W, groups)`：`W` = 容量上限；`groups[i]` 是第 i 组的所有物品 `{w, val}` 对，0-indexed。
- 返回值：`vector<i64> dp`；`dp[j]` = 容量不超过 `j` 的最大价值，答案取 `dp[W]`。
- 每组最多选一个；若要求每组“至少选一个”，把转移里的 `dp[j]` 换成只从 `old[j-w]+val` 更新（去掉跳过整组的选项）。

```cpp
vector<i64> group_knapsack(int W, const vector<vector<pair<int, i64>>>& groups) {
    vector<i64> dp(W + 1, 0);
    for (auto group : groups) {
        vector<i64> old = dp;
        for (auto [w, val] : group) {
            for (int j = w; j <= W; ++j) {
                dp[j] = max(dp[j], old[j - w] + val);
            }
        }
    }
    return dp;
}
```
- **核心逻辑**：实现用 `old` 数组保存上一组结果，正序更新 `dp`；一组里最多选一个。
#### 二维费用背包

> **赛时先看**
> - **题目信号**：每个物品消耗两类资源。
> - **本质**：有两个容量限制，如重量和体积。
> - **复杂度判定**：`O(nAB)`。
> - **维护的量**：`dp[a][b]`（资源一不超过 a、资源二不超过 b 时的最大价值）。
> - **警告**：0/1 情况两个维度都倒序。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个物品（第 i 个耗资源一 `ca[i]`、资源二 `cb[i]`、价值 `val[i]`，各至多一次），资源一上限 `A`、资源二上限 `B`，求最大总价值。

```cpp
vector<int> ca(n + 1), cb(n + 1);
vector<i64> val(n + 1);
for (int i = 1; i <= n; ++i) cin >> ca[i] >> cb[i] >> val[i];
auto dp = two_cost_knapsack(A, B, ca, cb, val);  // 1. 调用：三个数组 1-indexed
cout << dp[A][B] << '\n';                        // 2. 取结果：两类资源都不超 A、B
```

样例：物品 `(1,1,3) (1,1,4) (1,2,5)`（费用一，费用二，价值），`A = 2, B = 2` -> 输出 `7`（选前两个）。

**传参要求（照这个传不会错）：**

- `two_cost_knapsack(A, B, ca, cb, val)`：`A/B` = 两类资源的容量上限；三个物品数组等长、1-indexed（长度 `n+1`，`[0]` 不用）。
- 返回值：`vector<vector<i64>> dp`；`dp[a][b]` = 资源一 ≤ a、资源二 ≤ b 的最大价值，答案取 `dp[A][B]`。
- 每件最多选一次（两维都倒序）；可重复选就把两维都改正序。
- 注意空间：`(A+1)*(B+1)` 个 i64，`A*B` 很大时先看内存限制。

```cpp
vector<vector<i64>> two_cost_knapsack(int A, int B,
                                           const vector<int>& ca,
                                           const vector<int>& cb,
                                           const vector<i64>& val) {
    vector<vector<i64>> dp(A + 1, vector<i64>(B + 1, 0));
    for (int i = 0; i < (int)val.size(); ++i) {
        for (int a = A; a >= ca[i]; --a) {
            for (int b = B; b >= cb[i]; --b) {
                dp[a][b] = max(dp[a][b], dp[a - ca[i]][b - cb[i]] + val[i]);
            }
        }
    }
    return dp;
}
```

#### 背包方案数

> **赛时先看**
> - **题目信号**：问有多少种选法，常见硬币、子集和。
> - **本质**：统计凑出容量/金额的方案数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(nW)`。
> - **维护的量**：`dp[j]`（凑出恰好 j 的方案数，`dp[0] = 1`）。
> - **警告**：0/1 背包倒序，完全背包正序；按题意处理取模。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 种硬币（第 i 种面值 `w[i]`，可重复用），凑出金额 `W` 有多少种方案，模 `mod`。

```cpp
vector<int> coin(n + 1);
for (int i = 1; i <= n; ++i) cin >> coin[i];
auto dp = complete_count(W, coin, mod);  // 1. 调用：硬币不限量用 complete_count（正序）
cout << dp[W] << '\n';                   // 2. 取结果：凑出恰好 W 的方案数模 mod
```

样例：硬币 `{1,2,5}`，`W = 5` -> 输出 `4`（5、2+2+1、2+1+1+1、1+1+1+1+1）。

**传参要求（照这个传不会错）：**

- `complete_count(W, coin, mod)`：`W` = 目标金额；`coin` 1-indexed（长度 `n+1`，`[0]` 不用）；`mod` = 取模数。
- 返回值：`vector<i64> dp`；`dp[j]` = 凑出恰好 `j` 的方案数模 mod，答案取 `dp[W]`。
- 硬币不限量：用 `complete_count`；每枚只能用一次（子集和）：用 `zero_one_count(W, coin, mod)`。
- 循环顺序“外层硬币、内层金额”数的是组合（顺序不区分）；交换两层就变成排列计数，结果不同。

```cpp
vector<i64> zero_one_count(int W, const vector<int>& weight, i64 mod) {
    vector<i64> dp(W + 1, 0);
    dp[0] = 1;
    for (int w : weight) {
        for (int j = W; j >= w; --j) {
            dp[j] = (dp[j] + dp[j - w]) % mod;
        }
    }
    return dp;
}

vector<i64> complete_count(int W, const vector<int>& coin, i64 mod) {
    vector<i64> dp(W + 1, 0);
    dp[0] = 1;
    for (int w : coin) {
        for (int j = w; j <= W; ++j) {
            dp[j] = (dp[j] + dp[j - w]) % mod;
        }
    }
    return dp;
}
```
#### 整数分拆：把 n 写成若干正整数之和

> **赛时先看**
> - **题目信号**：硬币面值恰为 `1..n`、每种可无限取、组合顺序不区分；这正是完全背包的“先枚举面值，再枚举和”。
> - **本质**：计算无序正整数分拆数，例如 `4 = 4 = 3+1 = 2+2 = 2+1+1 = 1+1+1+1`，共 5 种。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n^2)` 时间、`O(n)` 空间。
> - **维护的量**：`dp[x]`（x 的无序正整数分拆数，`dp[0] = 1`）。
> - **警告**：循环顺序决定是否把排列重复计数。必须外层枚举 `part`，内层正序枚举 `sum`；`dp[0]=1` 表示“凑成 0 的空方案”。


**最小完整示例（先抄这一段就能跑）：**

题目：把 `n` 写成若干个正整数的和（顺序不区分），求分拆数模 `mod`。

```cpp
int n, mod;
cin >> n >> mod;
auto dp = integer_partition_counts(n, mod);  // 1. 调用：mod 可省略（默认全局 modn）
cout << dp[n] << '\n';                       // 2. 取结果：n 的分拆数模 mod
```

样例：`n = 4` -> 输出 `5`（4、3+1、2+2、2+1+1、1+1+1+1）。

**传参要求（照这个传不会错）：**

- `integer_partition_counts(n, mod = modn)`：`n` = 要拆的数；`mod` = 取模数，缺省用全局 `modn`。
- 返回值：`vector<int> dp`（长度 `n+1`）；`dp[x]` = 整数 x 的无序分拆数（模 mod），答案取 `dp[n]`。
- 等价于“面值 `1..n` 各无限个的完全背包”；外层 `part`、内层正序 `sum` 的顺序不能换，换了会把排列也算进去。
- `n` 很大时 `O(n^2)` 会超时；带额外限制（每部分不超过 k、必须恰好 m 个等）需自己改造。

- `sum`：当前正在凑的总和。
- `dp`：DP 状态。

```cpp
vector<int> integer_partition_counts(int n, int mod = modn) {
    vector<int> dp(n + 1);
    dp[0] = 1 % mod;
    for (int part = 1; part <= n; ++part) {
        for (int sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
            if (dp[sum] >= mod) dp[sum] -= mod;
        }
    }
    return dp; // dp[x] 是 x 的无序正整数分拆数。
}
```
#### 有上界的方案计数：分糖果 / 分球到各人

> **赛时先看**
> - **题目信号**：每个人分到的数量有上限，物品/糖果必须刚好分完；`n` 约百、`total` 可到 `1e5`，朴素枚举每人取多少会超时。
> - **本质**：有 `n` 个位置，第 `i` 个取 `0..limit[i]` 个，恰好凑总和 `total`，求方案数模 `mod`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n * total)` 时间、`O(total)` 空间。
> - **维护的量**：`dp[s]`（前若干个人分完、恰好凑出 s 的方案数）；`window_sum`（滑动窗口内 `dp` 的和，即当前人可取范围的转移和）。
> - **警告**：转移是 `next[s] = sum(dp[s-x], 0<=x<=limit)`，用滑动窗口维护而非原地更新；窗口左端移出时要补回模数。经典练习：AtCoder Educational DP `M - Candies`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个人，第 i 人最多拿 `limit[i]` 个糖果，恰好分完 `total` 个，方案数模 `mod`。

```cpp
vector<int> limit(n);
for (int i = 0; i < n; ++i) cin >> limit[i];
int ans = count_bounded_sum_ways(limit, total, mod);  // 1. 调用：limit 0-indexed
cout << ans << '\n';                                  // 2. 取结果：方案数模 mod
```

样例：`limit = {2, 1, 2}`，`total = 3` -> 输出 `5`。

**传参要求（照这个传不会错）：**

- `count_bounded_sum_ways(limit, total, mod = mod7)`：`limit` 0-indexed，`limit[i]` = 第 i 个人最多能拿的个数；`total` = 必须恰好凑出的总和；`mod` = 取模数，缺省用全局 `mod7`。
- 返回值：`int`，恰好凑出 `total` 的方案数模 mod。
- 每人“恰好分完”隐含每人都要参与：本模板允许每人拿 `0..limit[i]` 个，也就是允许有人拿 0 个；要求每人至少 1 个时先给 `total` 减掉 `n`。

- `sum`：当前枚举的总和。
- `dp`：DP 状态。

```cpp
int count_bounded_sum_ways(const vector<int>& limit, int total, int mod = mod7) {
    vector<int> dp(total + 1);
    dp[0] = 1 % mod;
    for (int upper : limit) {
        vector<int> next(total + 1);
        int window_sum = 0;
        for (int sum = 0; sum <= total; ++sum) {
            window_sum += dp[sum];
            if (window_sum >= mod) window_sum -= mod;
            if (sum - upper - 1 >= 0) {
                window_sum -= dp[sum - upper - 1];
                if (window_sum < 0) window_sum += mod;
            }
            next[sum] = window_sum;
        }
        dp.swap(next);
    }
    return dp[total];
}
```
#### 依赖背包：树上选课骨架

> **赛时先看**
> - **题目信号**：课程先修、附件依赖、树形依赖选择。
> - **本质**：选某个物品前必须选它的父物品。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：常见 `O(nW^2)`。
> - **维护的量**：`dp[u]`（以 u 为根的子树、容量不超过 j 的最大价值）；`sz[u]`（子树已占容量）；`child/cost/value`（依赖树与物品数据）。
> - **警告**：合并子树时容量倒序，避免重复使用同一子树。构造后需手动填写 `cost/value/child` 再对根调用 `dfs`；森林要对每个根分别调用。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 门课，第 i 门要先修 `fa[i]`（0 表示无先修），花费 `cost[i]`、价值 `value[i]`，容量 `W`，求最大总价值。

```cpp
DependentKnapsack dk(n, W);                       // 1. 建对象：n 个节点、容量 W
vector<int> fa(n + 1);
for (int i = 1; i <= n; ++i) {
    cin >> fa[i] >> dk.cost[i] >> dk.value[i];    // 2. 手动填 cost/value（1-indexed）
    if (fa[i]) dk.child[fa[i]].push_back(i);      // 3. 填依赖：i 挂到 fa[i] 下
}
int ans = 0;
for (int r = 1; r <= n; ++r)                      // 4. 森林：每个根（fa[r]==0）各 dfs 一次
    if (fa[r] == 0) {
        dk.dfs(r);
        for (int j = 0; j <= W; ++j) ans = max(ans, dk.dp[r][j]);  // 5. 取各根 dp 的最大值
    }
cout << ans << '\n';
```

样例：`(fa,cost,value)=(0,2,5) (1,2,6) (1,3,7)`，`W = 4` -> 输出 `11`（选 1 和 2）。

**传参要求（照这个传不会错）：**

- 构造 `DependentKnapsack dk(n, W)`：`n` = 节点数（1-indexed），`W` = 容量上限；成员全部公开，直接填。
- 填 `dk.cost[i]`、`dk.value[i]`；依赖关系用 `dk.child[fa].push_back(i)`；无先修的节点是根。
- 调用 `dk.dfs（根）`：**每个根都要调用一次**，子节点在递归里自动处理；答案对所有根取 `dp[根][j]` 的最大值。
- `cost[i] > W` 的节点实际选不了，但 `dp[u][j]` 会保持 `-INF`，不影响其他分支。

- `sz`：集合/子树大小。
- `dp`：DP 状态。

```cpp
struct DependentKnapsack {
    int n, W;
    vector<vector<int>> child;
    vector<int> cost, value, sz;
    vector<vector<i64>> dp;

    DependentKnapsack(int n, int W) : n(n), W(W), child(n + 1), cost(n + 1), value(n + 1), sz(n + 1), dp(n + 1) {}

    void dfs(int u) {
        dp[u].assign(W + 1, -(1LL << 60));
        for (int j = cost[u]; j <= W; ++j) dp[u][j] = value[u];
        sz[u] = cost[u];
        for (int v : child[u]) {
            dfs(v);
            vector<i64> ndp = dp[u];
            for (int j = W; j >= cost[u]; --j) {
                for (int k = 0; k + j <= W; ++k) {
                    ndp[j + k] = max(ndp[j + k], dp[u][j] + dp[v][k]);
                }
            }
            dp[u].swap(ndp);
        }
    }
};
```
#### LIS 最长严格上升子序列

> **赛时先看**
> - **题目信号**：题面问"最长严格递增子序列长度""按原顺序挑数、要求递增、最多挑几个"，`n ≤ 1e5+`；n 一大就不能 O(n²) 暴力，直接想 `lis_strict`。
> - **本质**：`d` 维护"长度为 len 的上升子序列的最小末尾值"（`d[len-1]`）；新数 `x` 二分找它该替换哪个长度，要么把最小末尾变小给后面留余地，要么把最长链延长一格。
> - **复杂度判定**：`O(n log n)` 时间、`O(n)` 空间；n 到 1e6 也能过。
> - **维护的量**：`d`（`d[len-1]` = 长度为 len 的严格上升子序列的最小末尾）。
> - **接法**：题面说"按原顺序选一些数，要求严格递增，问最多选几个"就调用 `lis_strict(a)`；它只返回长度，不返回选了哪些位置。
> - **警告**：严格上升用 `lower_bound`；非降用 `upper_bound`；`d` 是辅助结构，本身不是答案子序列。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，求最长严格上升子序列的长度（`n <= 1e5`）。

```cpp
vector<int> a(n + 1);
for (int i = 1; i <= n; ++i) cin >> a[i];
int len = lis_strict(a);      // 1. 调用：a 需 1-indexed（a[0] 不用）
cout << len << '\n';          // 2. 取结果：最长严格上升子序列长度
```

样例：`a = [3,1,4,1,5,9,2,6]` -> 输出 `4`（如 1,4,5,9 或 3,4,5,9）。

**传参要求（照这个传不会错）：**

- `lis_strict(a)`：`a` 1-indexed（长度 `n+1`）；返回 `int` 最长**严格上升**长度。
- 只返回长度，不返回选了哪些数；要方案翻"LIS：恢复"节，要计数翻"LIS 计数"节。
- 非降（允许相等）：把实现里的 `lower_bound` 改成 `upper_bound`。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `lis_strict` 函数。
2. 构造：把原数组直接放进 `a`。
3. 调用：`int len = lis_strict(a);`
4. 取结果：`len` 就是最长严格上升子序列长度，直接输出。

**改造点（按题目改这几处）：**
- 严格 / 非降：严格递增用 `lower_bound`；允许相等（非降）把 `lower_bound` 改成 `upper_bound`。
- 要输出方案：翻本节"LIS：恢复一组最长严格上升子序列"（`longest_increasing_subsequence`）。
- 要数方案数：翻本节"LIS 计数"（树状数组版，按原下标区分方案）。
- 元素类型不是 int：把 `vector<int>` 换成 `vector<i64>` 等对应类型即可。


```cpp
// 维护的量：d[len-1] = 长度为 len 的严格上升子序列的最小末尾值。
// 不变量：d 严格递增，这是二分（lower_bound）能用的前提。
int lis_strict(const vector<int>& a) {
    vector<int> d;
    for (int x : a) {
        // 严格上升找第一个 >= x 的位置替换；改 upper_bound 即允许相等（非降）
        auto it = lower_bound(d.begin(), d.end(), x);
        if (it == d.end()) d.push_back(x);
        else *it = x;
    }
    return (int)d.size();
}
```
#### LIS：恢复一组最长严格上升子序列

> **赛时先看**
> - **题目信号**：题目不仅问最长长度，还要求输出选了哪些元素；或答案要沿着一条严格递增链继续处理。
> - **本质**：除了长度，还要输出一组方案、记录链上的原下标，或把 LIS 当作后续构造的骨架。
> - **接法**：需要输出一条递增链时用这个模板。`result.indices` 是原数组下标，默认 `0-based`；如果题目要求输出位置从 `1` 开始，输出时 `+1`。如果只要长度，用上一小节的 `lis_strict` 更简洁。
> - **复杂度判定**：`O(n log n)`，额外空间 `O(n)`。
> - **维护的量**：`tails_value/tails_index`（每个长度的最小结尾值及其原下标）；`previous[i]`（i 在最优链上的前驱下标）。
> - **警告**：`tails_value[len - 1]` 只表示“长度为 len 的最小结尾值”，它本身不是答案；恢复必须额外保存每个位置的前驱 `previous[i]`。严格上升用 `lower_bound`，非降子序列改成 `upper_bound`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数（`n <= 1e5`），输出一组最长严格上升子序列（长度 + 选中的值）。

```cpp
vector<i64> a(n);
for (int i = 0; i < n; ++i) cin >> a[i];        // a 是 0-indexed
auto res = longest_increasing_subsequence(a);   // 1. 调用
cout << res.length << '\n';                     // 2. 取结果：最长长度
for (i64 x : res.values) cout << x << ' ';      // 3. 取结果：选中的元素值
cout << '\n';                                   // 下标要 1-based 输出时 res.indices 各项 +1
```

样例：`a = [3,1,4,1,5,9,2,6]` -> `length = 4`，一组答案 `{1,4,5,9}`（下标 1,2,4,5）。

**传参要求（照这个传不会错）：**

- `longest_increasing_subsequence(a)`：`a` 0-indexed（内部从 0 遍历到 `size()-1`），元素类型 `i64`，可为负。
- 返回值 `LISResult`：`length` = 最长长度；`indices` = 被选元素在**原数组中的下标（0-based）**，递增；`values` = 对应元素值。
- 严格上升用 `lower_bound`；非降（允许相等）把 `lower_bound` 改成 `upper_bound`。
- 只问长度用上一节 `lis_strict` 更简洁；`a` 为空时返回 `length = 0`。

```cpp
struct LISResult {
    int length = 0;
    vector<int> indices; // 原数组中被选元素的下标，递增。
    vector<i64> values;
};

LISResult longest_increasing_subsequence(const vector<i64>& a) {
    int n = (int)a.size();
    vector<i64> tails_value;
    vector<int> tails_index, previous(n, -1);

    for (int i = 0; i < n; ++i) {
        int pos = (int)(lower_bound(tails_value.begin(), tails_value.end(), a[i])
                        - tails_value.begin());
        if (pos > 0) previous[i] = tails_index[pos - 1];
        if (pos == (int)tails_value.size()) {
            tails_value.push_back(a[i]);
            tails_index.push_back(i);
        } else {
            tails_value[pos] = a[i];
            tails_index[pos] = i;
        }
    }

    LISResult result;
    result.length = (int)tails_value.size();
    for (int at = result.length == 0 ? -1 : tails_index.back();
         at != -1; at = previous[at]) {
        result.indices.push_back(at);
    }
    reverse(result.indices.begin(), result.indices.end());
    for (int index : result.indices) result.values.push_back(a[index]);
    return result;
}
```
#### LIS 计数：最长严格上升子序列有多少条

> **赛时先看**
> - **题目信号**：题目问最长递增链的数量、最长嵌套方案数，值域很大但只涉及大小比较；要同时维护“最长长度”和“达到该长度的计数”。
> - **本质**：求 LIS 的长度及“按原下标不同”计算的方案数。
> - **接法**：如果只问 LIS 长度，翻本节上一个小节 `lis_strict`；如果问“最长递增链有多少条”，用这个树状数组版本。读每个数 `x` 时，只能接在所有 `< x` 的最优链后面，所以查询 `rank-1`；得到新链后更新 `rank`。
> - **复杂度判定**：离散化加树状数组，`O(n log n)`。
> - **维护的量**：`tree`（Fenwick 每个节点存 `LISCount{length, ways}`，按排名合并最优）；`values`（离散化后的值，用于把 `x` 转成排名）。
> - **警告**：本实现的计数按下标区分，值相同但位置不同的子序列算不同方案。严格上升必须查询排名 `< rank`，不能查 `<= rank`；空数组按唯一空子序列返回 `{0,1}`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，求最长严格上升子序列的长度和条数（模 `mod`）。

```cpp
vector<i64> a(n);
for (int i = 0; i < n; ++i) cin >> a[i];
auto res = count_longest_increasing_subsequences(a, mod);  // 1. 调用：a 0-indexed
cout << res.length << ' ' << res.ways << '\n';             // 2. 取结果：长度 与 方案数模 mod
```

样例：`a = [1,3,2,4]` -> 输出 `3 2`（1,2,4 与 1,3,4）。

**传参要求（照这个传不会错）：**

- `count_longest_increasing_subsequences(a, mod = modn)`：`a` 0-indexed（长度 `n`），元素 `i64`、可为负可重复；`mod` = 取模数，缺省用全局 `modn`。
- 返回值 `LISCount{length, ways}`：`length` = LIS 长度；`ways` = 达到该长度的方案数模 mod。
- 计数按下标区分：值相同但位置不同算不同方案；`a` 为空返回 `{0, 1}`。
- 只问长度用 `lis_strict`；`FenwickLISCount` 的 `merge/query/update` 是内部方法，不要直接调用。

**API / 入口函数（赛时只认这里列的名字）：**

- `count_longest_increasing_subsequences(a, mod = modn)` -> 主入口：求 LIS 长度及方案数，返回 `LISCount{length, ways}`。内部 `FenwickLISCount` 的 merge/query/update 只是内部方法，不要直接调。

```cpp
struct LISCount {
    int length = 0;
    int ways = 0;
};

struct FenwickLISCount {
    int n, mod;
    vector<LISCount> tree;

    FenwickLISCount(int n, int mod) : n(n), mod(mod), tree(n + 1) {}

    LISCount merge(LISCount a, LISCount b) const {
        if (a.length != b.length) return a.length > b.length ? a : b;
        if (a.length == 0) return {0, 0}; // 查询空前缀时，稍后人为补一条空链。
        return {a.length, (a.ways + b.ways) % mod};
    }

    void update(int index, LISCount value) {
        for (; index <= n; index += index & -index) tree[index] = merge(tree[index], value);
    }

    LISCount query(int index) const { // [1, index] 的最优 (长度, 方案数)
        LISCount result;
        for (; index > 0; index -= index & -index) result = merge(result, tree[index]);
        return result;
    }
};

LISCount count_longest_increasing_subsequences(const vector<i64>& a, int mod = modn) {
    if (a.empty()) return {0, 1 % mod};
    vector<i64> values = a;
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());

    FenwickLISCount bit((int)values.size(), mod);
    for (i64 x : a) {
        int rank = (int)(lower_bound(values.begin(), values.end(), x) - values.begin()) + 1;
        LISCount best = bit.query(rank - 1); // 严格小于 x。
        int ways = best.length == 0 ? 1 % mod : best.ways;
        bit.update(rank, {best.length + 1, ways});
    }
    return bit.query((int)values.size());
}
```
#### 概念辨析：子段、子序列与子串

> **赛时先看**
> - **题目信号**：数组题若要求选中的元素下标连续，是**子段**；若只要求保留原相对顺序、可以删除中间元素，是**子序列**。字符串中的连续片段叫**子串**，和数组子段同样要求连续。
> - **本质**：避免把名称相近、但约束完全不同的模型套错。
> - **接法**：读到“连续”两个字，先想子段/子串；读到“删除若干但相对顺序不变”，先想子序列。数组连续最大和翻 Kadane；字符串连续匹配翻 KMP/哈希/SAM；不连续公共部分翻 LCS/序列自动机。这个判断错了，后面代码再对也会 WA。
> - **复杂度判定**：这是术语判断，不需要额外算法；读题时先确认“连续”与“可删除”的含义，再选对应模板。
> - **警告**：Kadane 解决的是**最大子段和**（Maximum Subarray / 最大连续子数组和），不是最大子序列和。若题目允许任意跳过元素、只要求和最大：允许空选时取所有正数之和；必须非空且全为非正时取最大元素。只有附加长度、数量、单调性或其他约束时，才会形成真正的子序列 DP。为了照顾常见口头误称，网页搜索“最大子序列和”仍会带你来到本节，但打印和展示标签只使用正确术语“最大子段和”。

#### 带权区间选择：最大权不相交任务

> **赛时先看**
> - **题目信号**：任务、演出、订单、预约、机器排程等都有时间区间和收益；允许一个任务恰好在另一个结束时开始。
> - **本质**：从很多有开始、结束、收益的任务中选若干个，不重叠地取得最大总收益，并恢复选中的任务。
> - **接法**：把每个任务读成 `{start,end,weight,id}`，全部丢给 `maximum_weight_non_overlapping_intervals`。如果题目区间是闭区间 `[l,r]` 且两个任务不能共享端点，就把兼容条件理解成 `previous.end < current.start`，可以通过把 `end` 预处理成 `r+1` 转成半开区间。
> - **复杂度判定**：排序加二分为 `O(n log n)`。
> - **维护的量**：`dp[i]`（前 i 个按结束时间排序的任务的最大收益）；`previous_count[i]/take[i]`（记录每个任务的兼容前驱与“是否选它”，用于恢复方案）。
> - **警告**：这里统一使用半开区间 `[start, end)`，所以 `previous.end <= current.start` 才兼容。不要把“按开始时间贪心”错当成带权版本；带权时贪心通常不成立。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个任务，每个有开始、结束、收益，选互不重叠的任务使总收益最大，并输出选中的任务编号。

```cpp
vector<WeightedInterval> intervals(n);
for (int i = 0; i < n; ++i) {
    cin >> intervals[i].start >> intervals[i].end >> intervals[i].weight;
    intervals[i].id = i + 1;                                  // 1. 记住原题编号
}
auto res = maximum_weight_non_overlapping_intervals(intervals);  // 2. 调用：0-indexed
cout << res.max_weight << '\n';                               // 3. 取结果：最大总收益
for (int id : res.chosen_ids) cout << id << ' ';              // 4. 取结果：选中的任务编号
cout << '\n';
```

样例：任务 `(1,3,2) (2,5,4) (4,6,4)` -> 输出 `6`，选中 `1 3`。

**传参要求（照这个传不会错）：**

- `maximum_weight_non_overlapping_intervals(intervals)`：`intervals` 0-indexed，每个元素填 `start/end/weight/id` 四个字段。
- 返回值 `WeightedIntervalResult`：`max_weight` = 最大总收益；`chosen_ids` = 选中的 `id` 列表（按输入顺序，升序）。
- 区间是**半开** `[start, end)`：上一个的 `end <= 下一个的 start` 才兼容；题面给闭区间 `[l,r]` 且端点不能共享时，把 `end` 存成 `r+1`。
- 只问最大值不必填 `id`（保持默认 0 即可）；要恢复方案则必须给每个任务填不同的 `id`。带权问题贪心不成立，必须用这个 DP。

- `id`：原题的任务编号，便于恢复答案。
- `dp`：DP 状态。

```cpp
struct WeightedInterval {
    i64 start, end, weight;
    int id; // 原题的任务编号，便于恢复答案。
};

struct WeightedIntervalResult {
    i64 max_weight = 0;
    vector<int> chosen_ids;
};

WeightedIntervalResult maximum_weight_non_overlapping_intervals(
    vector<WeightedInterval> intervals
) {
    sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) {
        if (a.end != b.end) return a.end < b.end;
        return a.start < b.start;
    });
    int n = (int)intervals.size();
    vector<i64> ends(n);
    for (int i = 0; i < n; ++i) ends[i] = intervals[i].end;

    vector<i64> dp(n + 1, 0); // 前 i 个按结束时间排序的任务的最优收益。
    vector<int> previous_count(n + 1);
    vector<char> take(n + 1, false);
    for (int i = 1; i <= n; ++i) {
        // 前 i-1 个任务中，结束时间 <= 当前开始时间的任务数量。
        int compatible = (int)(upper_bound(ends.begin(), ends.begin() + i - 1,
                                            intervals[i - 1].start) - ends.begin());
        previous_count[i] = compatible;
        i64 use_current = dp[compatible] + intervals[i - 1].weight;
        if (use_current > dp[i - 1]) {
            dp[i] = use_current;
            take[i] = true;
        } else {
            dp[i] = dp[i - 1];
        }
    }

    WeightedIntervalResult result;
    result.max_weight = dp[n];
    for (int i = n; i > 0;) {
        if (take[i]) {
            result.chosen_ids.push_back(intervals[i - 1].id);
            i = previous_count[i];
        } else {
            --i;
        }
    }
    reverse(result.chosen_ids.begin(), result.chosen_ids.end());
    return result;
}
```
#### 最大子段和：无限制、环形、恰好 K 与长度区间 [L,R]

> **赛时先看**
> - **题目信号**：题面出现“最大连续子数组和 / 最大连续收益 / 最大连续得分”，元素可正可负。若要求“连续”，用本节；若允许跳过元素，则是 LIS、背包或其他“子序列”问题，不能套 Kadane。长度恰好为 `K` 时用定长滑动窗口；长度范围为 `[L,R]` 时，对每个右端点减去可行左端点中的最小前缀和。
> - **本质**：在数组中选一个**非空连续子段**，使元素和最大。给出无限制长度的 Kadane、环形数组、长度恰好为 `K`、长度至多为 `K`，以及长度限制在 `[L,R]` 的完整版本。
> - **维护的量**：只读原数组 `a`；长度区间版另维护前缀和 `prefix` 与存左端点下标的单调队列 `candidates`；恰好 K 版维护定长窗口和 `window`。
> - **接法**：无限制长度用 `maximum_subarray_sum_nonempty`；环形数组用 `maximum_circular_subarray_sum_nonempty`；恰好 `K` 个连续元素用 `maximum_subarray_sum_exactly_k`；长度在 `[L,R]` 用 `maximum_subarray_sum_length_between`；如果题目说“最多 K/至少 K”，分别把区间写成 `[1,K]` 或 `[K,n]`。所有函数都默认必须选非空连续段。
> - **复杂度判定**：所有函数均为 `O(n)` 时间。无限制、环形、恰好 `K` 使用 `O(1)` 额外空间；长度区间版使用 `O(n)` 前缀和与 `O(R-L+1)` 单调队列空间。
> - **警告**：以下均要求选非空子段，且长度参数须满足 `1 <= K <= n` 或 `1 <= L <= R <= n`。全负环形数组不能靠 `total - min_subarray` 凑答案（那会选到空段），必须直接返回最大元素。前缀和 `prefix[i]` 表示前 `i` 个元素之和；固定右端点 `right` 时，子段 `[left, right)` 的和是 `prefix[right] - prefix[left]`。

**最小完整示例（先抄这一段就能跑）：**

- 题目：数组 `[-2, 1, -3, 4, -1, 2, 1, -5, 4]` 的最大连续子段和家族。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    vector<i64> a = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    i64 p = maximum_subarray_sum_nonempty(a);              // 无限制：6
    i64 c = maximum_circular_subarray_sum_nonempty(a);     // 环形：6
    i64 k = maximum_subarray_sum_exactly_k(a, 3);          // 恰好 3 个：5
    i64 b = maximum_subarray_sum_length_between(a, 2, 4);  // 长度 [2,4]：6
    cout << p << " " << c << " " << k << " " << b << "\n";
```

- 样例数值：`6 6 5 6`。

**传参要求（照这个传不会错）：**

- `maximum_subarray_sum_nonempty(a)`：`a` 原数组（下标 `0..n-1`，允许负数，非空）；返回 `i64` 最大非空连续子段和。
- `maximum_circular_subarray_sum_nonempty(a)`：同 `a`；返回环形最大子段和；全负时直接返回最大元素（不能靠 `total - min` 凑）。
- `maximum_subarray_sum_exactly_k(a, k)`：`k` 窗口长度，要求 `1 <= k <= n`；返回恰好 `k` 个连续元素的最大和。
- `maximum_subarray_sum_length_between(a, min_len, max_len)`：`min_len`/`max_len` 长度上下界，要求 `1 <= min_len <= max_len <= n`；返回长度在 `[min_len, max_len]` 的最大和。
- `maximum_subarray_sum_at_most_k(a, k)` / `maximum_subarray_sum_at_least_k(a, k)`：内部就是 `[1,k]` / `[k,n]` 的 `length_between` 特例；要求 `1 <= k <= n`。

**API / 入口函数（赛时只认这里列的名字）：**

- `maximum_subarray_sum_nonempty(const vector<i64>& a)` -> 无限制长度的 Kadane，必选非空子段。 返回 `i64`。
- `maximum_circular_subarray_sum_nonempty(const vector<i64>& a)` -> 环形数组最大子段和，必选非空子段。 返回 `i64`。
- `maximum_subarray_sum_length_between(const vector<i64>& a, int min_len, int max_len)` -> 长度范围 [min_len, max_len] 的单调队列版，at_least/at_most 内部都复用它。 返回 `i64`。
- `maximum_subarray_sum_at_least_k(const vector<i64>& a, int k)` -> “长度至少 K”也常见：范围 [K, n]。 返回 `i64`。
- `maximum_subarray_sum_at_most_k(const vector<i64>& a, int k)` -> “长度至多 K”就是长度范围 [1, K] 的特例。 返回 `i64`。
- `maximum_subarray_sum_exactly_k(const vector<i64>& a, int k)` -> 恰好选 K 个连续元素：窗口 [right-K, right) 唯一确定。 返回 `i64`。

检索提示：若网页里输入“最大子序列和”，本节也会被命中以纠正这个常见口头名称；正式术语和打印标签仍应写作“最大子段和”。

```cpp
i64 maximum_subarray_sum_nonempty(const vector<i64>& a) {
    assert(!a.empty());
    i64 current = a[0], answer = a[0];
    for (int i = 1; i < (int)a.size(); ++i) {
        current = max(a[i], current + a[i]);
        answer = max(answer, current);
    }
    return answer;
}

i64 maximum_circular_subarray_sum_nonempty(const vector<i64>& a) {
    assert(!a.empty());
    i64 total = accumulate(a.begin(), a.end(), 0LL);
    i64 max_end = a[0], max_sum = a[0];
    i64 min_end = a[0], min_sum = a[0];
    for (int i = 1; i < (int)a.size(); ++i) {
        max_end = max(a[i], max_end + a[i]);
        max_sum = max(max_sum, max_end);
        min_end = min(a[i], min_end + a[i]);
        min_sum = min(min_sum, min_end);
    }
    if (max_sum < 0) return max_sum; // 否则 total - min_sum 会错误地选空段。
    return max(max_sum, total - min_sum);
}

// 恰好选 K 个连续元素：窗口 [right-K, right) 唯一确定。
i64 maximum_subarray_sum_exactly_k(const vector<i64>& a, int k) {
    int n = (int)a.size();
    assert(1 <= k && k <= n);
    i64 window = accumulate(a.begin(), a.begin() + k, 0LL);
    i64 answer = window;
    for (int right = k; right < n; ++right) {
        window += a[right] - a[right - k];
        answer = max(answer, window);
    }
    return answer;
}

// 选的连续子段长度在 [min_len, max_len] 中。
i64 maximum_subarray_sum_length_between(
    const vector<i64>& a, int min_len, int max_len
) {
    int n = (int)a.size();
    assert(1 <= min_len && min_len <= max_len && max_len <= n);
    vector<i64> prefix(n + 1);
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + a[i];

    deque<int> candidates; // 前缀和单调递增；存可作为左端点的下标。
    i64 answer = -(1LL << 62);
    for (int right = 1; right <= n; ++right) {
        // 新加入 left = right - min_len，保证长度不短于 min_len。
        int entering = right - min_len;
        if (entering >= 0) {
            while (!candidates.empty() && prefix[candidates.back()] >= prefix[entering]) {
                candidates.pop_back();
            }
            candidates.push_back(entering);
        }
        // left < right - max_len 时，长度已经超过 max_len。
        while (!candidates.empty() && candidates.front() < right - max_len) {
            candidates.pop_front();
        }
        if (!candidates.empty()) {
            answer = max(answer, prefix[right] - prefix[candidates.front()]);
        }
    }
    return answer;
}

// “长度至多 K”就是长度范围 [1, K] 的特例。
i64 maximum_subarray_sum_at_most_k(const vector<i64>& a, int k) {
    return maximum_subarray_sum_length_between(a, 1, k);
}

// “长度至少 K”也常见：范围 [K, n]。
i64 maximum_subarray_sum_at_least_k(const vector<i64>& a, int k) {
    return maximum_subarray_sum_length_between(a, k, (int)a.size());
}
```
#### 相邻区间合并：石子合并最小代价

> **赛时先看**
> - **题目信号**：石子、文件、括号段、相邻颜色块等只能按原顺序合并；合并后的代价依赖整段权重。
> - **本质**：一排相邻块只能合并相邻两段，每次代价等于新段的总权重，求总成本最小。
> - **维护的量**：`prefix`（前缀和，供 `range_sum` 算区间代价）；`dp[l][r]`（合并闭区间 `[l,r]` 的最小代价，0-indexed）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：朴素 `O(n^3)`，空间 `O(n^2)`。若题目满足 Knuth 优化的四边形不等式/决策单调性，再翻 Knuth 模板降至 `O(n^2)`。
> - **警告**：本模板是直线，不是环。环形石子合并要把数组复制一遍并枚举长度为 `n` 的起点；转移顺序是先对所有分割取最小值，再在末尾加上整段和。
> - **约定**：`range_sum(l, r)` 表示闭区间 `[l, r]` 的元素和（前缀和实现）。

**最小完整示例（先抄这一段就能跑）：**

- 题目：石子堆重量 `[4, 1, 5, 2]`，相邻两堆才能合并，求最小总代价。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    vector<i64> weight = {4, 1, 5, 2};
    i64 ans = minimum_adjacent_merge_cost(weight);
    cout << ans << "\n";
```

- 样例数值：`24`。

**传参要求（照这个传不会错）：**

- `minimum_adjacent_merge_cost(weight)`：`weight` 各堆石子重量，下标 `0..n-1`（允许任意值）；返回 `i64` 直线合并的最小总代价；`n <= 1` 时返回 `0`。环形问题把数组复制一份，对每个长度为 `n` 的窗口取最小值。

```cpp
i64 minimum_adjacent_merge_cost(const vector<i64>& weight) {
    int n = (int)weight.size();
    if (n <= 1) return 0;
    vector<i64> prefix(n + 1);
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + weight[i];
    auto range_sum = [&](int l, int r) { // 闭区间 [l,r]。
        return prefix[r + 1] - prefix[l];
    };

    const i64 INF64 = (1LL << 62);
    vector<vector<i64>> dp(n, vector<i64>(n));
    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len <= n; ++l) {
            int r = l + len - 1;
            dp[l][r] = INF64;
            for (int mid = l; mid < r; ++mid) {
                dp[l][r] = min(dp[l][r], dp[l][mid] + dp[mid + 1][r]);
            }
            dp[l][r] += range_sum(l, r);
        }
    }
    return dp[0][n - 1];
}
```
#### 矩阵链乘：相乘顺序的最小标量乘法次数

> **赛时先看**
> - **题目信号**：`A_i` 的尺寸为 `dimension[i] x dimension[i+1]`，所有矩阵都要按原顺序相乘，但括号位置可变；代价由两个子结果的尺寸相乘决定。
> - **本质**：给定一列维度相容的矩阵，选择括号化方式，使乘法标量次数最少，并恢复一种括号方案。
> - **维护的量**：`dp[l][r]`（子链 `A_l..A_r` 的最少标量乘法次数）；`split[l][r]`（最优分割点，用来恢复括号方案）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O(n^3)` 时间、`O(n^2)` 空间，其中 `n = dimension.size()-1`。
> - **警告**：第 `mid` 个分割是 `[l, mid]` 与 `[mid + 1, r]`，乘法代价是 `dimension[l] * dimension[mid + 1] * dimension[r + 1]`。若维度和成本可能超过 `i64`，把 `i64` 改成 `i128`。

**最小完整示例（先抄这一段就能跑）：**

- 题目：矩阵尺寸 `[10, 20, 50, 1]`（A:10x20、B:20x50、C:50x1），求最优括号化。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    vector<i64> dimension = {10, 20, 50, 1};
    MatrixChainResult res = matrix_chain_multiplication(dimension);
    cout << res.min_multiplications << "\n";
    cout << res.parenthesization << "\n";
```

- 样例数值：`1200` 与 `(A1 x (A2 x A3))`。

**传参要求（照这个传不会错）：**

- `matrix_chain_multiplication(dimension)`：`dimension` 长度 `n+1`，第 `i` 个矩阵（`i = 0..n-1`）尺寸是 `dimension[i] x dimension[i+1]`；返回 `MatrixChainResult`：`.min_multiplications`（`i64` 最少标量乘法次数）、`.parenthesization`（括号方案，`A1` 表示第 1 个矩阵）。

```cpp
struct MatrixChainResult {
    i64 min_multiplications = 0;
    string parenthesization;
};

MatrixChainResult matrix_chain_multiplication(const vector<i64>& dimension) {
    int n = (int)dimension.size() - 1;
    if (n <= 0) return {0, ""};
    vector<vector<i64>> dp(n, vector<i64>(n));
    vector<vector<int>> split(n, vector<int>(n, -1));
    const i64 INF64 = (1LL << 62);

    for (int len = 2; len <= n; ++len) {
        for (int l = 0; l + len <= n; ++l) {
            int r = l + len - 1;
            dp[l][r] = INF64;
            for (int mid = l; mid < r; ++mid) {
                i64 candidate = dp[l][mid] + dp[mid + 1][r]
                    + dimension[l] * dimension[mid + 1] * dimension[r + 1];
                if (candidate < dp[l][r]) {
                    dp[l][r] = candidate;
                    split[l][r] = mid;
                }
            }
        }
    }

    function<string(int, int)> build = [&](int l, int r) -> string {
        if (l == r) return "A" + to_string(l + 1);
        int mid = split[l][r];
        return "(" + build(l, mid) + " x " + build(mid + 1, r) + ")";
    };
    return {dp[0][n - 1], build(0, n - 1)};
}
```
#### 回文串最少分割：每段都是回文

> **赛时先看**
> - **题目信号**：每一段都要回文、允许在任意位置切开，目标是段数/切割次数最少。若 `n` 只有几千，预处理回文区间后做前缀 DP 足够。
> - **本质**：把一个字符串切成若干个回文子串，求最少切割次数并恢复一种切分。
> - **维护的量**：`is_palindrome[l][r]`（`s[l..r]` 是否回文）；`parts[end]`（前 `end` 个字符的最少段数）；`previous[end]`（最优切点，用于恢复分段）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：时间 `O(n^2)`，空间 `O(n^2)`。
> - **警告**：`parts[i]` 表示前 `i` 个字符的最少“段数”，不是最少切割次数；最后答案要减一。空串这里返回 `0` 刀、空方案。

**最小完整示例（先抄这一段就能跑）：**

- 题目：`"aab"` 切成若干回文段的最少切割次数。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    string s = "aab";
    PalindromicPartitionResult res = minimum_palindromic_partition(s);
    cout << res.min_cuts << "\n";
```

- 样例数值：`1`（切一刀：`aa | b`）。

**传参要求（照这个传不会错）：**

- `minimum_palindromic_partition(s)`：`s` 原字符串（下标 `0..n-1`）；返回 `PalindromicPartitionResult`：`.min_cuts`（`int` 最少切割次数，空串为 `0`）、`.segments`（切出的回文段，按原顺序）。

```cpp
struct PalindromicPartitionResult {
    int min_cuts = 0;
    vector<string> segments;
};

PalindromicPartitionResult minimum_palindromic_partition(const string& s) {
    int n = (int)s.size();
    if (n == 0) return {0, {}};
    vector<vector<char>> is_palindrome(n, vector<char>(n));
    for (int l = n - 1; l >= 0; --l) {
        for (int r = l; r < n; ++r) {
            is_palindrome[l][r] = s[l] == s[r]
                && (r - l <= 2 || is_palindrome[l + 1][r - 1]);
        }
    }

    const int INF32 = 1e9;
    vector<int> parts(n + 1, INF32), previous(n + 1, -1);
    parts[0] = 0;
    for (int end = 1; end <= n; ++end) {
        for (int start = 0; start < end; ++start) {
            if (is_palindrome[start][end - 1] && parts[start] + 1 < parts[end]) {
                parts[end] = parts[start] + 1;
                previous[end] = start;
            }
        }
    }

    PalindromicPartitionResult result;
    result.min_cuts = parts[n] - 1;
    for (int end = n; end > 0; end = previous[end]) {
        result.segments.push_back(s.substr(previous[end], end - previous[end]));
    }
    reverse(result.segments.begin(), result.segments.end());
    return result;
}
```
#### 区间 DP 骨架

> **赛时先看**
> - **题目信号**：答案定义在 `[l,r]`；最后一步是把区间拆成两段或合并两段。
> - **本质**：合并区间、删除区间、两端收缩。
> - **维护的量**：`dp[l][r]`（闭区间 `[l,r]` 的答案，1-indexed）；`a`（原数组，`a[0]` 是占位、实际元素从下标 1 开始）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：常见 `O(n^3)`。
> - **警告**：按区间长度从小到大枚举。这是骨架：代码没有转移代价、恒返回 0，需要自己补转移代价与目标统计；直接照抄会得到无意义结果。
> - **约定**：int n = (int)a.size() - 1; // 1-based，下标从 1 开始

**最小完整示例（先抄这一段就能跑）：**

- 题目：骨架演示，`a = {占位， 1, 2, 3}`，跑完看枚举顺序（转移代价需自己补）。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    vector<int> a = {0, 1, 2, 3}; // a[0] 占位，下标从 1 开始。
    i64 ans = interval_dp_example(a);
    cout << ans << "\n";
```

- 样例数值：`0`（骨架无转移代价，恒 0）。

**传参要求（照这个传不会错）：**

- `interval_dp_example(a)`：`a` 长度 `n+1`，`a[0]` 占位、`a[1..n]` 是实际元素（代码内 `n = a.size()-1`）；返回 `i64`。骨架没有转移代价、恒返回 `dp[1][n] = 0`，必须自己补转移代价与目标统计。

```cpp
const i64 DP_INF = (1LL << 60);

i64 interval_dp_example(const vector<int>& a) {
    int n = (int)a.size() - 1; // 1-based，下标从 1 开始。
    vector<vector<i64>> dp(n + 2, vector<i64>(n + 2, 0));

    for (int len = 2; len <= n; ++len) {
        for (int l = 1; l + len - 1 <= n; ++l) {
            int r = l + len - 1;
            dp[l][r] = DP_INF;
            for (int k = l; k < r; ++k) {
                dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r]);
            }
        }
    }
    return dp[1][n];
}
```
#### Knuth 优化 DP

> **赛时先看**
> - **题目信号**：`dp[l][r] = min(dp[l][k] + dp[k][r] + w(l,r))`，且最优断点单调。
> - **本质**：区间 DP 中转移点具有单调性，常见于合并石子、最优二叉搜索树。
> - **维护的量**：`dp[l][r]`（闭区间 `[l,r]` 的最小合并代价）；`opt[l][r]`（最优断点，只扫 `[opt[l][r-1], opt[l+1][r]]`）；`pre`（前缀和算区间代价）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：从 `O(n^3)` 优化到 `O(n^2)`。
> - **警告**：需要满足四边形不等式和包含单调；区间端点定义要统一为开区间或闭区间。
> - **约定**：闭区间写法：合并石子，代价为区间和

**最小完整示例（先抄这一段就能跑）：**

- 题目：石子堆（1-indexed）`[4, 1, 5, 2]`，相邻合并的最小总代价。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    vector<int> a = {0, 4, 1, 5, 2}; // a[0] 占位，下标从 1 开始。
    i64 ans = knuth_merge_stones(a);
    cout << ans << "\n";
```

- 样例数值：`24`。

**传参要求（照这个传不会错）：**

- `knuth_merge_stones(a)`：`a` 长度 `n+1`，`a[0]` 占位、`a[1..n]` 是石子重量（代码内 `n = a.size()-1`）；返回 `i64` 最小合并代价。要求代价满足四边形不等式与包含单调，否则答案可能不是最优。

**API / 入口函数（赛时只认这里列的名字）：**

- `knuth_merge_stones(const vector<int>& a)` -> 闭区间写法：合并石子，代价为区间和。 返回 `i64`。

```cpp
// 闭区间写法：合并石子，代价为区间和。
i64 knuth_merge_stones(const vector<int>& a) {
    int n = (int)a.size() - 1;
    vector<i64> pre(n + 1);
    for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + a[i];
    auto cost = [&](int l, int r) {
        return pre[r] - pre[l - 1];
    };
    const i64 INF = (1LL << 62);
    vector dp(n + 2, vector<i64>(n + 2, 0));
    vector opt(n + 2, vector<int>(n + 2, 0));
    for (int i = 1; i <= n; i++) opt[i][i] = i;
    for (int len = 2; len <= n; len++) {
        for (int l = 1; l + len - 1 <= n; l++) {
            int r = l + len - 1;
            dp[l][r] = INF;
            int from = opt[l][r - 1];
            int to = opt[l + 1][r];
            for (int k = from; k <= to; k++) {
                i64 val = dp[l][k] + dp[k + 1][r] + cost(l, r);
                if (val < dp[l][r]) {
                    dp[l][r] = val;
                    opt[l][r] = k;
                }
            }
        }
    }
    return dp[1][n];
}
```
#### 状压 DP：TSP 骨架

> **赛时先看**
> - **题目信号**：`n <= 20`，状态是一个集合。
> - **本质**：小规模集合状态，访问/选择哪些点。
> - **维护的量**：`dp[mask][u]`（已访问集合为 `mask`、当前停在 `u` 的最短路径长度）；`w`（`w[u][v]` 是 u→v 的边权）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(2^n n^2)`。
> - **警告**：数组大小是 `1<<n`，注意内存。

**最小完整示例（先抄这一段就能跑）：**

- 题目：4 个城市、从 0 出发走完所有城市的最短路径（不要求回起点）。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    vector<vector<i64>> w = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    i64 ans = tsp_min_path(w, 0); // start 默认 0，可省略。
    cout << ans << "\n";
```

- 样例数值：`65`（`0 -> 1 -> 3 -> 2`）。

**传参要求（照这个传不会错）：**

- `tsp_min_path(w, start = 0)`：`w` 是 `n x n` 邻接矩阵（`w[u][v]` = u 到 v 的边权，下标 `0..n-1`，无边给大数）；`start` 起点城市，默认 `0`，范围 `[0, n)`；返回 `i64` 从 `start` 出发、恰好访问每个点一次的最短路径长度（不回起点）；`n` 别太大（`1<<n` 的数组要开得下，`n <= 20` 左右）。

```cpp
i64 tsp_min_path(const vector<vector<i64>>& w, int start = 0) {
    int n = (int)w.size();
    const i64 INF = (1LL << 60);
    vector<vector<i64>> dp(1 << n, vector<i64>(n, INF));
    dp[1 << start][start] = 0;

    for (int mask = 0; mask < (1 << n); ++mask) {
        for (int u = 0; u < n; ++u) {
            if (dp[mask][u] == INF) continue;
            for (int v = 0; v < n; ++v) {
                if (mask >> v & 1) continue;
                dp[mask | (1 << v)][v] = min(dp[mask | (1 << v)][v], dp[mask][u] + w[u][v]);
            }
        }
    }
    return *min_element(dp[(1 << n) - 1].begin(), dp[(1 << n) - 1].end());
}
```
#### SOS DP：子集和变换

> **赛时先看**
> - **题目信号**：状态是 bitmask；转移或查询形如 `sum f[T], T subset S`。
> - **本质**：对所有集合 `S`，快速求所有子集/超集贡献和。
> - **维护的量**：`f`（长度 `1<<n` 的数组，原地 zeta 累加）；`bit`/`s`（按位分层、按掩码枚举的循环变量）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：`O(n 2^n)`。
> - **警告**：子集和与超集和循环方向不同；数组长度是 `1 << n`。

**最小完整示例（先抄这一段就能跑）：**

- 题目：`n = 2`，`f = {0, 1, 2, 3}`（下标即掩码），求每个掩码的子集和 / 超集和。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    int n = 2;
    vector<i64> f = {0, 1, 2, 3};
    vector<i64> sub = subset_sum_transform(f, n);   // 子集和
    vector<i64> sup = superset_sum_transform(f, n); // 超集和
```

- 样例数值：`sub = {0, 1, 2, 6}`，`sup = {6, 4, 5, 3}`。

**传参要求（照这个传不会错）：**

- `subset_sum_transform(f, n)`：`f` 长度必须恰好 `1 << n`（下标 `0..2^n-1` 是掩码）；`n` 位数；返回新数组 `g`，`g[S] = sum f[T]`，`T` 遍历 `S` 的所有子集。`f` 按值传参、原数组不被破坏。
- `superset_sum_transform(f, n)`：同上；返回 `g[S] = sum f[T]`，`T` 取所有以 `S` 为子集的超集（即 `S ⊆ T`）。

**API / 入口函数（赛时只认这里列的名字）：**

- `subset_sum_transform(vector<i64> f, int n)` -> g[S] = sum f[T], T 是 S 的子集。 返回 `vector<i64>`。
- `superset_sum_transform(vector<i64> f, int n)` -> g[S] = sum f[T], S 是 T 的子集。 返回 `vector<i64>`。
**改板时先认这几个量：**

- `f`：输入数组，原地做子集/超集 zeta 累加。
- `n`：位数，数组长度 `1 << n`。
- `s`：掩码循环变量。

```cpp
// g[S] = sum f[T], T 是 S 的子集。
vector<i64> subset_sum_transform(vector<i64> f, int n) {
    for (int bit = 0; bit < n; bit++) {
        for (int s = 0; s < (1 << n); s++) {
            if (s & (1 << bit)) f[s] += f[s ^ (1 << bit)];
        }
    }
    return f;
}

// g[S] = sum f[T], S 是 T 的子集。
vector<i64> superset_sum_transform(vector<i64> f, int n) {
    for (int bit = 0; bit < n; bit++) {
        for (int s = 0; s < (1 << n); s++) {
            if (!(s & (1 << bit))) f[s] += f[s | (1 << bit)];
        }
    }
    return f;
}
```
#### 快速子集卷积 Fast Subset Convolution

> **赛时先看**
> - **题目信号**：状态是位集；要把一个集合拆成两个互不相交部分，并累加两个函数的乘积；朴素枚举子集是 `O(3^n)`，但 `n` 已经到 18~22 左右。
> - **本质**：计算不相交子集卷积 `h[S] = sum_{T subseteq S} f[T] * g[S \ T]`：按集合大小分层，用 zeta/逆 zeta 变换把复杂度从 `O(3^n)` 降到 `O(n^2 2^n)`。
> - **维护的量**：`F[k][mask]`/`G[k][mask]`（按大小 k 分层的 zeta 表）；`H[k][mask]`（大小维卷积后的结果表）；`mod`（模数，默认 ≤ 1e9+7 保证 i64 乘法安全）。
> - **接法**：计数每个点集拆成两个独立结构的方案；连通子图 DP 中按不交集合合并；集合划分、带颜色集合的组合 DP。
> - **复杂度判定**：`O(n^2 2^n)` 时间、`O(n 2^n)` 空间。若 `n <= 17` 且常数小，朴素 `O(3^n)` 有时更短更快；本模板适合位数更大、或需要多次复用卷积实现的场景。
> - **警告**：不要把它和 OR 卷积混淆。答案中是 `S \ T`，所以两部分天然不交；若题目允许重叠，通常应考虑 OR 卷积。`__builtin_popcount(mask)` 的参数是 `unsigned int`，因此 `n` 不应大到让 `1 << n` 溢出。

**最小完整示例（先抄这一段就能跑）：**

- 题目：`n = 2`，`f = {1, 2, 4, 8}`、`g = {1, 1, 1, 1}`，求不相交子集卷积。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    i64 mod = 1000000007;
    vector<i64> f = {1, 2, 4, 8}, g = {1, 1, 1, 1};
    vector<i64> h = fast_subset_convolution(f, g, mod);
```

- 样例数值：`h = {1, 3, 5, 15}`。

**传参要求（照这个传不会错）：**

- `fast_subset_convolution(f, g, mod)`：`f`、`g` 长度必须相等且都是 `2^n`（内部用 `__builtin_ctz` 推出 `n`）；`mod` 模数，要求 `mod <= 1e9+7` 量级保证乘法不溢出；返回 `vector<i64> h`，`h[S] = sum_{T ⊆ S} f[T] * g[S \ T] mod mod`，两部分天然不交。


**改板时先认这几个量：**

- `f`、`g`：两个输入函数，长度均为 `2^n`。
- `mod`：模数，默认不大于 `1e9+7` 以保证 `i64` 乘法安全。
- `n`：位数。

`h[S] = sum_{T subseteq S} f[T] * g[S \ T]`。
这不是 OR/AND/XOR 卷积：这里要求两个子集**不交且并集正好为 `S`**。

使用：`f` 和 `g` 的长度必须都是 `2^n`，系数在 `mod` 下运算；本实现默认 `mod` 不大于 `1e9+7`，以保证 `i64` 乘法安全。

```cpp
// 返回 h[S] = sum_{T subseteq S} f[T] * g[S \ T] (mod mod)
vector<i64> fast_subset_convolution(
    const vector<i64>& f,
    const vector<i64>& g,
    i64 mod
) {
    const int N = (int)f.size();
    assert(N == (int)g.size() && N > 0 && (N & (N - 1)) == 0);
    const int n = __builtin_ctz((unsigned)N);

    vector<vector<i64>> F(n + 1, vector<i64>(N));
    vector<vector<i64>> G(n + 1, vector<i64>(N));
    vector<vector<i64>> H(n + 1, vector<i64>(N));

    // 按集合大小分层，再分别做子集 zeta 变换。
    for (int mask = 0; mask < N; ++mask) {
        int k = __builtin_popcount((unsigned)mask);
        F[k][mask] = (f[mask] % mod + mod) % mod;
        G[k][mask] = (g[mask] % mod + mod) % mod;
    }
    for (int k = 0; k <= n; ++k) {
        for (int bit = 0; bit < n; ++bit) {
            for (int mask = 0; mask < N; ++mask) if (mask & (1 << bit)) {
                F[k][mask] += F[k][mask ^ (1 << bit)];
                if (F[k][mask] >= mod) F[k][mask] -= mod;
                G[k][mask] += G[k][mask ^ (1 << bit)];
                if (G[k][mask] >= mod) G[k][mask] -= mod;
            }
        }
    }

    // 对每个 S，把“大小”这一维做普通卷积。
    for (int mask = 0; mask < N; ++mask) {
        for (int k = 0; k <= n; ++k) {
            for (int left = 0; left <= k; ++left) {
                H[k][mask] = (H[k][mask]
                    + F[left][mask] * G[k - left][mask]) % mod;
            }
        }
    }

    // 逆 zeta 变换后，取与 |S| 同层的值。
    for (int k = 0; k <= n; ++k) {
        for (int bit = 0; bit < n; ++bit) {
            for (int mask = 0; mask < N; ++mask) if (mask & (1 << bit)) {
                H[k][mask] -= H[k][mask ^ (1 << bit)];
                if (H[k][mask] < 0) H[k][mask] += mod;
            }
        }
    }

    vector<i64> h(N);
    for (int mask = 0; mask < N; ++mask) {
        h[mask] = H[__builtin_popcount((unsigned)mask)][mask];
    }
    return h;
}
```
#### 小 k 枚举 + 区间行 DP：无左移网格避障

> **赛时先看**
> - **题目信号**：障碍数量 `k <= 10~20`；每个障碍列不同；方案按 bitmask 输出；不能向左。
> - **本质**：网格只能向右/上/下走，少量障碍每个有二选一限制，对所有选择方案求最短路。
> - **维护的量**：`obs_id[c]`/`obs_row[c]`（列 c 的障碍编号与行号）；`dp[row]`（当前列走到各行的最少步数）；`ans[mask]`（绕行方案 mask 的最短路，-1 表示不可行）。
> - **复杂度判定**：`O(2^k * m * n)`。
> - **警告**：某列选择“从上方绕过”时可用行是 `[1,r-1]`，从下方绕过时是 `[r+1,n]`；从一列走到下一列时，前后两列的行号都必须在各自允许区间内。

**最小完整示例（先抄这一段就能跑）：**

- 题目：`3 x 4` 网格，唯一障碍在第 2 行第 2 列，求两个绕行方案各自的最短路。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    int n = 3, m = 4;
    vector<pair<int, int>> obstacles = {{2, 2}}; // {行,列}，列严格递增。
    vector<i64> ans = min_steps_for_all_obstacle_masks(n, m, obstacles);
```

- 样例数值：`ans = {3, 3}`（两种绕行方案都需 3 步）。

**传参要求（照这个传不会错）：**

- `min_steps_for_all_obstacle_masks(n, m, obstacles)`：`n`/`m` 网格行数/列数（1-indexed，范围 `1..n`、`1..m`）；`obstacles` 是 `{行， 列}` 列表（行、列均从 1 开始，且列坐标必须严格递增）；返回 `vector<i64>`（长度 `1<<k`，`k = obstacles.size()`）：`ans[mask]` 是“第 i 个障碍按 mask 第 i 位 0=上绕/1=下绕”的最短步数，方案不可行为 `-1`。


```cpp
vector<i64> min_steps_for_all_obstacle_masks(
    int n,
    int m,
    const vector<pair<int, int>>& obstacles // {行, 列}，列坐标严格递增。
) {
    const i64 INF = (1LL << 60);
    int k = (int)obstacles.size();
    vector<int> obs_id(m + 1, -1), obs_row(m + 1, -1);
    for (int i = 0; i < k; i++) {
        auto [r, c] = obstacles[i];
        obs_id[c] = i;
        obs_row[c] = r;
    }

    auto interval = [&](int col, int mask) -> pair<int, int> {
        int id = obs_id[col];
        if (id == -1) return {1, n};
        int r = obs_row[col];
        if (((mask >> id) & 1) == 0) return {1, r - 1};
        return {r + 1, n};
    };

    vector<i64> ans(1 << k, -1);
    for (int mask = 0; mask < (1 << k); mask++) {
        vector<i64> dp(n + 1, INF);
        auto [l1, r1] = interval(1, mask);
        if (l1 > r1) continue;
        for (int r = l1; r <= r1; r++) dp[r] = 0;

        bool ok = true;
        for (int col = 2; col <= m; col++) {
            auto [l, r] = interval(col, mask);
            if (l > r) {
                ok = false;
                break;
            }
            vector<i64> enter(n + 1, INF), ndp(n + 1, INF);
            for (int row = l; row <= r; row++) {
                if (dp[row] < INF) enter[row] = dp[row] + 1;
            }

            i64 best = INF;
            for (int row = l; row <= r; row++) {
                best = min(best, enter[row] - row);
                ndp[row] = min(ndp[row], best + row);
            }
            best = INF;
            for (int row = r; row >= l; row--) {
                best = min(best, enter[row] + row);
                ndp[row] = min(ndp[row], best - row);
            }
            dp.swap(ndp);
        }
        if (!ok) continue;
        i64 best = *min_element(dp.begin() + 1, dp.end());
        if (best < INF) ans[mask] = best;
    }
    return ans;
}
```

#### 轮廓线 DP：骨牌覆盖

> **赛时先看**
> - **题目信号**：`n*m` 网格，较小的一维 `<= 12`；每格和左/上/右/下局部相关。
> - **本质**：网格按行逐格推进，统计铺砖、连通性、障碍路径等状态压缩问题。
> - **维护的量**：`dp[mask]`（当前轮廓线状态的铺法数，`ndp` 是推进一格后的新表）；内部自动让较小维度作状态维 `m`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：骨牌覆盖约 `O(n m 2^m)`。
> - **警告**：让 `m` 为较小维度；本模板只覆盖空棋盘铺满方案，障碍格、连通性等扩展需要自己加在逐格转移的判断里。

**最小完整示例（先抄这一段就能跑）：**

- 题目：`2 x 3` 空棋盘用 1x2 骨牌铺满的方案数。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    i64 ways = domino_tiling(2, 3); // 内部自动 swap 成 n=3, m=2。
    cout << ways << "\n";
```

- 样例数值：`3`。

**传参要求（照这个传不会错）：**

- `domino_tiling(n, m)`：`n`/`m` 棋盘行数/列数（`>= 1`，函数内部自动让较小的作为状态维 `m`）；返回 `i64` 空棋盘用 1x2 骨牌铺满的方案数；要求状态维 `m` 别太大（数组长 `1<<m`，约 `m <= 20`）。

**API / 入口函数（赛时只认这里列的名字）：**

- `domino_tiling(int n, int m)` -> 统计 n*m 空棋盘用 1*2 骨牌铺满的方案数。 返回 `i64`。

```cpp
// 统计 n*m 空棋盘用 1*2 骨牌铺满的方案数。
i64 domino_tiling(int n, int m) {
    if (m > n) swap(n, m);
    vector<i64> dp(1 << m), ndp(1 << m);
    dp[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fill(ndp.begin(), ndp.end(), 0);
            for (int mask = 0; mask < (1 << m); mask++) {
                i64 ways = dp[mask];
                if (!ways) continue;
                if (mask & (1 << j)) {
                    ndp[mask ^ (1 << j)] += ways;
                } else {
                    // 竖放，向下一行占同一列。
                    ndp[mask | (1 << j)] += ways;
                    // 横放，占当前行下一列。
                    if (j + 1 < m && !(mask & (1 << (j + 1)))) {
                        ndp[mask | (1 << (j + 1))] += ways;
                    }
                }
            }
            dp.swap(ndp);
        }
    }
    return dp[0];
}
```
#### 数位 DP 骨架

> **赛时先看**
> - **题目信号**：数字范围很大，限制和每一位数字有关。
> - **本质**：统计 `[1,n]` 中数位和能被 3 整除的数的个数。
> - **维护的量**：`s`（`n` 的十进制串）；`memo`（非 tight 分支下 `(pos,sum,started)` 的记忆化）；`dfs` 的四参 `pos/sum/started/tight`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。骨架的统计条件埋在代码里：base case 是 `started && sum % 3 == 0`（数位和 mod 3 == 0），换题要改 base case 与记忆化参数。
> - **复杂度判定**：`O（位数 × 状态数 × 转移数）`；`memo` 可用数组替换 `map` 提速。
> - **警告**：`tight` 表示是否贴上界；`started` 处理前导零。

**最小完整示例（先抄这一段就能跑）：**

- 题目：`[1, 123]` 中数位和能被 3 整除的数的个数。
- 调用（抄进 `solve()` 直接跑）：

```cpp
    i64 ans = solve_digit_dp(123);
    cout << ans << "\n";
```

- 样例数值：`41`。

**传参要求（照这个传不会错）：**

- `solve_digit_dp(n)`：`n` 上界（`n >= 1`，统计 `[1, n]`）；返回 `i64` 满足“数位和 % 3 == 0”的个数。换题只改 base case（`pos == s.size()` 处的判定条件）与记忆化状态参数，`tight`/`started` 逻辑保留。

```cpp
i64 solve_digit_dp(i64 n) {
    string s = to_string(n);
    map<tuple<int, int, int>, i64> memo;

    function<i64(int, int, int, bool)> dfs = [&](int pos, int sum, int started, bool tight) -> i64 {
        if (pos == (int)s.size()) {
            return started && (sum % 3 == 0);
        }
        if (!tight) {
            auto key = make_tuple(pos, sum, started);
            if (memo.count(key)) return memo[key];
        }
        int limit = tight ? s[pos] - '0' : 9;
        i64 res = 0;
        for (int d = 0; d <= limit; ++d) {
            int ns = started || d != 0;
            res += dfs(pos + 1, (sum + (ns ? d : 0)) % 3, ns, tight && d == limit);
        }
        if (!tight) memo[make_tuple(pos, sum, started)] = res;
        return res;
    };

    return dfs(0, 0, 0, true);
}
```
#### 概率 DP：独立事件分布

> **赛时先看**
> - **题目信号**：每枚硬币、每道题、每个开关独立成功，成功概率可以不同；题目问成功个数的分布或“正面数大于反面数”。
> - **本质**：若干独立 Bernoulli 事件依次发生，求恰好发生 `k` 次、至少发生某次数，或多数/少数发生的概率。
> - **接法**：读入每枚硬币正面概率数组 `p` 后调用 `probability_more_heads_than_tails(p)`；若问恰好 `k` 次成功，对 `independent_success_distribution(p)` 的返回值取 `dp[k]`，至少 `k` 次则累加 `dp[k..n]`。
> - **复杂度判定**：`O(n^2)` 时间，`O(n)` 空间。
> - **维护的量**：`dp[j]`（已处理事件中恰有 `j` 次成功的概率，下标 `0..n`）。
> - **警告**：一维数组必须 `j` 倒序，否则同一个事件会被算多次；概率答案用浮点数，通常误差 `1e-9` 足够；事件不独立时不能使用本块。


**最小完整示例（先抄这一段就能跑）：**

三枚硬币，正面概率分别为 0.5、0.6、0.3。

```cpp
vector<long double> p = {0.5L, 0.6L, 0.3L};
vector<long double> dp = independent_success_distribution(p); // dp[k] = 恰好 k 次成功
long double more = probability_more_heads_than_tails(p);      // 正面多于反面
// 样例输出：dp = {0.14, 0.41, 0.36, 0.09}，more ≈ 0.45
```

**传参要求（照这个传不会错）：**

- `independent_success_distribution(success_prob)`：`success_prob[i]` 是第 `i` 个事件（0-based，`0 <= i < n`）的成功概率，`0 <= p <= 1`（断言）；返回长度 `n+1` 的数组，`dp[k]` = 恰好 `k` 次成功的概率（`long double`）。
- `probability_more_heads_than_tails(head_prob)`：`head_prob[i]` 是第 `i` 枚硬币（0-based）的正面概率；返回正面数严格大于反面数的概率（`long double`）。各事件必须独立。

状态：`dp[j]` 表示已处理的事件中，恰有 `j` 次成功的概率。新事件成功率为 `p` 时，倒序转移 `dp[j+1] += dp[j]*p`、`dp[j] *= 1-p`。

```cpp
vector<long double> independent_success_distribution(const vector<long double>& success_prob) {
    int n = (int)success_prob.size();
    vector<long double> dp(n + 1);
    dp[0] = 1;
    for (int i = 0; i < n; ++i) {
        long double p = success_prob[i];
        assert(0 <= p && p <= 1);
        for (int successes = i; successes >= 0; --successes) {
            dp[successes + 1] += dp[successes] * p;
            dp[successes] *= 1 - p;
        }
    }
    return dp; // dp[k] = 恰好 k 次成功的概率
}

long double probability_more_heads_than_tails(const vector<long double>& head_prob) {
    vector<long double> dp = independent_success_distribution(head_prob);
    long double answer = 0;
    for (int heads = (int)head_prob.size() / 2 + 1;
         heads <= (int)head_prob.size(); ++heads) {
        answer += dp[heads];
    }
    return answer;
}
```

典题模型：AtCoder DP Contest I - Coins。读入每枚硬币正面概率后调用 `probability_more_heads_than_tails(p)`；若问至少 `k` 次成功，则累加 `dp[k..n]`。
#### 期望 DP：含自环的移项公式

> **赛时先看**
> - **题目信号**：题面出现“随机抽到空位置就重来”“本轮可能没有任何进展”“直到完成为止的平均次数”，并能把未来状态写成 `E`。
> - **本质**：随机过程每轮付出代价，问到终点的期望轮数/期望代价；当前状态可能以一定概率仍留在原状态。
> - **复杂度判定**：消掉一个状态自身的转移是 `O(1)`；整题复杂度取决于其余状态是否能按拓扑顺序 DP，或是否需要高斯消元。
> - **维护的量**：`E[s]`（当前状态期望）、自环概率 `p_self`、本轮代价 `step_cost`、其余状态贡献 `other`。
> - **警告**：`p_self = 1` 且尚未终止意味着期望可能无穷；若转移之间不再能按拓扑顺序求值，不能硬套本公式，应翻 E 章「吸收马尔可夫链期望方程」小节的高斯消元模板；不要漏掉本轮已经花掉的代价（公式里的 `cost`）。


**最小完整示例（先抄这一段就能跑）：**

抽卡：每轮花 1 次，`1/3` 概率抽到重复卡留在原状态（自环），`2/3` 概率进入下一阶段（该阶段期望还需 4 次）。

```cpp
long double ans = expectation_remove_self_loop(1.0L, 1.0L / 3, (2.0L / 3) * 4.0L);
// 样例输出：ans ≈ 5.5（= (1 + 8/3) / (1 - 1/3)）
```

**传参要求（照这个传不会错）：**

- `step_cost`：本轮已经花掉的代价（轮数/次数），非负。
- `self_probability`：留在原状态的概率 `p_self`，范围 `0 <= p_self < 1`（断言）；等于 1 且未终止时期望可能无穷。
- `other`：其余状态的贡献和 `sum(p_i * E[to_i])`，先用题目的 DP 把所有非自环转移算好再传入。
- 返回：消去自环后的期望 `(step_cost + other) / (1 - self_probability)`（`long double`）。

公式：若 `E[s] = cost + p_self * E[s] + sum(p_i * E[to_i])`，则

`E[s] = (cost + sum(p_i * E[to_i])) / (1 - p_self)`。

```cpp
// 已知所有其他状态的期望贡献 other = sum(p_i * E[to_i]) 时，消掉自环。
long double expectation_remove_self_loop(
    long double step_cost, long double self_probability, long double other
) {
    assert(0 <= self_probability && self_probability < 1);
    return (step_cost + other) / (1 - self_probability);
}
```

#### 典题：随机吃寿司到空盘的期望步数

> **赛时先看**
> - **题目信号**：对象同质、状态只依赖于“还剩 1、2、3 个的对象分别有多少”，随机选到已经完成的对象会形成自环。这个模型同样适用于随机抽盒子、随机处理不同剩余容量任务。
> - **本质**：有 `n` 个盘子，每轮等概率选一个盘子；若盘中有寿司则吃掉一个。给定每盘初始 `1/2/3` 个寿司，求全部吃完的期望轮数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n^3)` 时间与空间；`n=300` 时 `double` 数组约 220 MB，确认内存限制后再使用。
> - **维护的量**：`E[a][b][c]`（还剩 `1/2/3` 个寿司的盘子数分别为 `a,b,c` 时吃完的期望轮数，`a+b+c <= n`）。
> - **警告**：分子是总盘数 `n`，不是 `s`；循环顺序要让右侧状态先计算；这里用 `double` 节省内存，若 `n` 更小可改 `long double`。


**最小完整示例（先抄这一段就能跑）：**

三个盘子，初始寿司数分别为 1、2、3。

```cpp
vector<int> plates = {1, 2, 3};
double ans = expected_sushi_steps(plates);
// 样例输出：ans ≈ 11.0（AtCoder DP Contest J 第 1 个样例）
```

**传参要求（照这个传不会错）：**

- `plates`：`plates[i]` 是第 `i` 个盘子（0-based）的初始寿司数，只允许 `1/2/3`（断言）；盘子数 `n` 任意，但状态空间 `O(n^3)`，`n` 大时注意内存。
- 返回：全部吃完的期望轮数（`double`）；想要更高精度可把函数内的 `double` 换成 `long double`。

状态：`E[a][b][c]`，分别有 `a,b,c` 个盘子还剩 `1,2,3` 个寿司。非空盘数为 `s=a+b+c`，移项后：

`E[a][b][c] = (n + aE[a-1][b][c] + bE[a+1][b-1][c] + cE[a][b+1][c-1]) / s`。

```cpp
double expected_sushi_steps(const vector<int>& plates) {
    int n = (int)plates.size();
    vector<int> count(4);
    for (int x : plates) {
        assert(1 <= x && x <= 3);
        count[x]++;
    }

    int width = n + 1;
    auto index = [width](int a, int b, int c) -> size_t {
        return ((size_t)a * width + b) * width + c;
    };
    vector<double> dp((size_t)width * width * width, 0);
    auto get = [&](int a, int b, int c) -> double& {
        return dp[index(a, b, c)];
    };

    for (int c = 0; c <= n; ++c) {
        for (int b = 0; b + c <= n; ++b) {
            for (int a = 0; a + b + c <= n; ++a) {
                int nonempty = a + b + c;
                if (nonempty == 0) continue;
                double value = n;
                if (a) value += a * get(a - 1, b, c);
                if (b) value += b * get(a + 1, b - 1, c);
                if (c) value += c * get(a, b + 1, c - 1);
                get(a, b, c) = value / nonempty;
            }
        }
    }
    return get(count[1], count[2], count[3]);
}
```

典题：AtCoder DP Contest J - Sushi。直接读入盘子数组后输出 `expected_sushi_steps(a)`；状态压缩的关键是只统计剩余数量的频数，不区分盘子编号。
#### 树上随机游走：到根的期望步数

> **赛时先看**
> - **题目信号**：图是一棵树、每一步均匀随机走边、目标是固定根。树边会来回走，但不需要高斯消元。
> - **本质**：无向树上每步等概率走向一个相邻点，求每个点首次走到根的期望步数。
> - **接法**：建无向树邻接表（0-based）后调用 `expected_hit_root_on_uniform_tree(graph, root)`，返回各点首次走到根的期望步数。
> - **复杂度判定**：`O(n)` 时间和空间。
> - **维护的量**：`parent`（DFS 父节点）、`order`（遍历序）、`subtree_size`（子树大小）、`expectation`（答案数组）。
> - **警告**：这是**无向树且每条边等概率**的特例；如果边有转移概率、图有环或目标不止一个，转为期望方程/高斯消元。


**最小完整示例（先抄这一段就能跑）：**

链 `0-1-2`，根为 `0`。

```cpp
vector<vector<int>> g(3);
g[0].push_back(1); g[1].push_back(0);
g[1].push_back(2); g[2].push_back(1);
vector<long double> ans = expected_hit_root_on_uniform_tree(g, 0);
// 样例输出：ans = {0, 3, 4}（链上深度 d 的点为 d^2，可自查）
```

**传参要求（照这个传不会错）：**

- `graph`：无向树邻接表（0-based），`graph[u]` 存 `u` 的所有邻居；必须是**树**（否则断言失败），点数为 `graph.size()`。
- `root`：目标根节点下标，默认 `0`，范围 `0 <= root < n`（断言）。
- 返回：长度 `n` 的 `long double` 数组，`ans[u]` = 从 `u` 首次随机游走到 `root` 的期望步数。

**改板时先认这几个量：**

- `parent`：DFS 时记录的树上父节点。
- `order`：遍历顺序（BFS 序）。
- `subtree_size`：子树大小。
- `expectation`：答案数组，各点首次到根的期望步数。

结论：设 `sz[u]` 是以 `root` 为根时 `u` 子树大小，则从 `u` 首次走到父亲的期望步数为 `2*sz[u]-1`。所以 `E[root]=0`，`E[u]=E[parent[u]]+2*sz[u]-1`。

```cpp
vector<long double> expected_hit_root_on_uniform_tree(
    const vector<vector<int>>& graph, int root = 0
) {
    int n = (int)graph.size();
    assert(0 <= root && root < n);
    vector<int> parent(n, -1), order{root};
    for (int i = 0; i < (int)order.size(); ++i) {
        int u = order[i];
        for (int v : graph[u]) {
            if (v == parent[u]) continue;
            assert(parent[v] == -1); // 这里要求输入是一棵树。
            parent[v] = u;
            order.push_back(v);
        }
    }
    assert((int)order.size() == n);

    vector<int> subtree_size(n, 1);
    for (int i = n - 1; i > 0; --i) {
        subtree_size[parent[order[i]]] += subtree_size[order[i]];
    }

    vector<long double> expectation(n);
    for (int i = 1; i < n; ++i) {
        int u = order[i];
        expectation[u] = expectation[parent[u]] + 2 * subtree_size[u] - 1;
    }
    return expectation;
}
```

典题模型：给一棵无向树和根 `0`，每次随机走到相邻点，问从各点回到根的平均步数。建邻接表后调用本函数；链上深度 `d` 的点答案为 `d^2`，可用来手算检查。
#### 均匀集卡与线性期望速查

> **赛时先看**
> - **题目信号**：问“平均多少次”，总量能拆成很多指示变量之和，或已经收集 `i` 种时，得到新种类的概率为 `(kinds-i)/kinds`。
> - **本质**：从 `kinds` 种等概率卡片中反复抽，问集齐全部种类的期望抽取次数；或随机排列、随机染色中，求很多局部事件数量之和的期望。
> - **接法**：抽卡类型等概率时直接调用 `uniform_coupon_collector_expectation(k)`；随机排列的期望逆序对数调用 `expected_inversions_of_uniform_permutation(n)`。
> - **复杂度判定**：两者均为 `O(kinds)` 或 `O(1)`。
> - **维护的量**：无状态结构；只用标量 `kinds`（卡牌种类数）或 `n`（排列长度）。
> - **警告**：集卡公式要求每一种卡出现概率相同且独立；非均匀抽取不能把总种类数直接代入。线性期望虽然不要求各指示变量独立，但每个指示变量的期望与取值都要算对。


**最小完整示例（先抄这一段就能跑）：**

6 种等概率卡片集齐的期望次数；长度 4 的随机排列期望逆序对数。

```cpp
long double cards = uniform_coupon_collector_expectation(6);
long double invs = expected_inversions_of_uniform_permutation(4);
// 样例输出：cards ≈ 14.7（= 6*(1+1/2+...+1/6)），invs = 3（= 4*3/4）
```

**传参要求（照这个传不会错）：**

- `uniform_coupon_collector_expectation(kinds)`：`kinds` 是卡牌种类数，`kinds >= 0`（断言），要求每种卡抽取概率相等；返回集齐全部种类的期望抽取次数（`long double`）。
- `expected_inversions_of_uniform_permutation(n)`：`n` 是排列长度（`i64`），`n >= 0`（断言）；返回均匀随机排列的期望逆序对数 `n*(n-1)/4`（`long double`）。

结论：均匀集卡期望为 `kinds * (1 + 1/2 + ... + 1/kinds)`；随机排列的期望逆序对数为 `n*(n-1)/4`。线性期望不需要事件互相独立。

```cpp
long double uniform_coupon_collector_expectation(int kinds) {
    assert(kinds >= 0);
    long double answer = 0;
    for (int collected = 0; collected < kinds; ++collected) {
        answer += (long double)kinds / (kinds - collected);
    }
    return answer;
}

long double expected_inversions_of_uniform_permutation(i64 n) {
    assert(n >= 0);
    return (long double)n * (n - 1) / 4;
}
```

典题模型：抽卡类型等概率时直接调用 `uniform_coupon_collector_expectation(k)`；若各类概率不相等，不能代此公式，需要按状态做期望 DP 或容斥。随机排列的每一对元素以 `1/2` 概率构成逆序，因此即使所有对之间相关，期望仍可逐对相加。
#### 有限状态零和 MDP：值迭代差分收敛外推

> **赛时先看**
> - **题目信号**：状态数几十到几百；每一步 Alice 取 `max`、Bob 取 `min`，之后随机转移；`k` 可到 `1e9` 或更大；矩阵快速幂不容易，因为有策略选择的非线性 `max/min`。
> - **本质**：小状态、超大轮数的随机零和博弈。先做足够多轮值迭代，若有限状态链不可约且非周期，`V_t(s)-V_{t-1}(s)` 会趋于同一个长期平均收益 `g`，大轮数可用 `V_T(s)+(k-T)g` 外推。
> - **接法**：把一轮最优转移写成 `bellman(state, previous_values)`；跑 `warmup` 轮；用所有状态的平均差分估计 `gain`；若 `k<=warmup` 直接查表，否则外推。
> - **复杂度判定**：`O(T0 * 状态数 * 单次 Bellman 代价）` 预处理，每个询问 `O(1)`。
> - **维护的量**：`dp[t][s]`（第 `t` 轮状态 `s` 的值）、`gain`（平均差分估计的长期收益）、`warmup`（预热轮数）。
> - **警告**：这是数值外推模板，不是严格适用于所有 MDP。要有强连通/不可约、非周期等稳定性；`T0` 要足够大并用样例或误差要求检验。输出一般用 `long double`。


**最小完整示例（先抄这一段就能跑）：**

2 个状态：状态 0 每轮稳定 +1 分，状态 1 每轮 +0 分，无随机转移。

```cpp
struct Bell {
    long double operator()(int s, const vector<long double>& prev) const {
        return s == 0 ? prev[0] + 1.0L : prev[1] + 0.0L;
    }
};
AverageRewardValueIteration<Bell> it;
it.init(2, 1000);                          // 状态数 2，预热 1000 轮
it.run(Bell{});
long double v = it.value(1000000000LL, 0); // 第 1e9 轮状态 0 的值
// 样例输出：v ≈ 1e9（每轮稳定收益 1，外推值与真实值一致）
```

**传参要求（照这个传不会错）：**

- `init(states, rounds)`：`states` 为状态数（下标 `0..states-1`）；`rounds` 为预热轮数 `warmup`，要足够大（如 `1e4`）。
- `run(bellman)`：`bellman(s, prev)` 给出第 `t` 轮状态 `s` 的最优转移值，`prev` 是第 `t-1` 轮全部状态的值（0-based）；跑完后内部自动把所有状态末轮差分平均成 `gain`。
- `value(rounds, state)`：`rounds` 为轮数（`long long`，可到 `1e18`）；`state` 为状态下标，`0 <= state < states`（断言）；返回 `long double`：`rounds <= warmup` 直接查表，否则 `dp[warmup][state] + (rounds - warmup) * gain` 外推。

**API / 入口函数（赛时只认这里列的名字）：**

- `init(int states, int rounds)` -> 初始化/清空结构。
- `run(bellman)` -> 跑 `warmup` 轮值迭代，并用所有状态的平均差分估计长期收益 `gain`。
- `value(long long rounds, int state)` -> 查询第 `rounds` 轮状态 `state` 的值：`rounds <= warmup` 直接查表，否则用 `gain` 线性外推。 返回 `long double`。

```cpp
template <class Bellman>
struct AverageRewardValueIteration {
    int state_count = 0;
    int warmup = 0;
    vector<vector<long double>> dp;
    long double gain = 0;

    AverageRewardValueIteration() = default;
    AverageRewardValueIteration(int states, int rounds) { init(states, rounds); }

    void init(int states, int rounds) {
        state_count = states;
        warmup = rounds;
        dp.assign(warmup + 1, vector<long double>(state_count, 0));
        gain = 0;
    }

    void run(Bellman bellman) {
        for (int t = 1; t <= warmup; ++t) {
            for (int s = 0; s < state_count; ++s) {
                dp[t][s] = bellman(s, dp[t - 1]);
            }
        }
        gain = 0;
        for (int s = 0; s < state_count; ++s) {
            gain += dp[warmup][s] - dp[warmup - 1][s];
        }
        gain /= state_count;
    }

    long double value(long long rounds, int state) const {
        assert(0 <= state && state < state_count);
        if (rounds <= warmup) return dp[(int)rounds][state];
        return dp[warmup][state] + (rounds - warmup) * gain;
    }
};
```

典题：本场 H《Rock-Paper-Scissors Master》。双方手牌状态仅 `10*10=100`，Bellman 转移枚举 Alice 出牌、Bob 出牌和双方补牌；预处理 `T0=10000` 后对 `k>10000` 线性外推。

### 14 DP 优化与高级状态模型

单调队列、斜率优化、Li Chao、分治优化、SMAWK、WQS、Slope Trick 集中放在这里。
#### 单调队列优化 DP

> **赛时先看**
> - **题目信号**：`dp[i] = max(dp[j]) + val[i]`，且 `j` 在 `[i-k,i-1]`。
> - **本质**：转移从滑动窗口中取最大/最小。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`dp[i]`（前 `i` 项的最优值）；双端队列 `q`（存候选下标，按 `dp[j]` 从大到小）。
> - **警告**：先弹过期，再取队首转移。队列为空时 `dp[i]` 保持负无穷，后续状态不可达时注意别把 `NEG` 状态带进转移。
> - **约定**：`val` 为 1-indexed（长度 `n+1`），`val[0]` 不用。


**最小完整示例（先抄这一段就能跑）：**

`val = {0, 3, 5, 1}`（1-indexed），窗口 `k = 2`。

```cpp
vector<i64> val = {0, 3, 5, 1}; // val[0] 不用
vector<i64> dp = window_dp_max(val, 2);
// 样例输出：dp = {0, 3, 8, 9}
```

**传参要求（照这个传不会错）：**

- `val`：长度 `n+1`（1-indexed），`val[i]`（`1 <= i <= n`）是第 `i` 个位置的值，`val[0]` 不参与。
- `k`：转移窗口大小，`dp[i]` 只能从 `j in [i-k, i-1]` 转移；`k >= 1`。
- 返回：长度 `n+1` 的 `i64` 数组，`dp[i] = max(dp[j]) + val[i]`，`dp[0] = 0`；无合法转移的位置保持 `NEG`（不可达）。


```cpp
vector<i64> window_dp_max(const vector<i64>& val, int k) {
    int n = (int)val.size() - 1;
    const i64 NEG = -(1LL << 60);
    vector<i64> dp(n + 1, NEG);
    deque<int> q;
    dp[0] = 0;
    q.push_back(0);

    for (int i = 1; i <= n; ++i) {
        while (!q.empty() && q.front() < i - k) q.pop_front();
        if (!q.empty()) dp[i] = dp[q.front()] + val[i];
        while (!q.empty() && dp[q.back()] <= dp[i]) q.pop_back();
        q.push_back(i);
    }
    return dp;
}
```
#### 斜率优化：单调队列凸包

> **赛时先看**
> - **题目信号**：把转移式展开后，候选 `j` 变成一堆直线。
> - **本质**：转移形如 `dp[i] = min(k_j * x_i + b_j)`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部函数参数直接当题面参数。
> - **复杂度判定**：斜率单调且查询单调时 `O(n)`。
> - **维护的量**：双端队列 `q`（存按斜率递增加入的下凸包直线 `Line{k,b}`）。
> - **警告**：如果斜率或查询不单调，改用 Li Chao Tree。


**最小完整示例（先抄这一段就能跑）：**

依次加入 `y = 1x+3`、`y = 2x+1`、`y = 3x-2`，查询最小值。

```cpp
HullMin h;
h.add_line(1, 3);
h.add_line(2, 1);
h.add_line(3, -2);
i64 best = h.query(1); // x=1 处取值 4、3、1
// 样例输出：best = 1
```

**传参要求（照这个传不会错）：**

- `add_line(k, b)`：加入直线 `y = k*x + b`（`i64`）；要求斜率 `k` 按加入顺序**单调不减**（本模板维护最小值下凸包）。
- `query(x)`：查询所有已加入直线在 `x` 处的最小值（`i64`）；要求查询点 `x` **单调不减**。
- 返回：`i64` 最小值；斜率或查询不单调时结果错误，改用 Li Chao Tree。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_line(i64 k, i64 b)` -> 加入一条直线
- `query(i64 x)` -> 查询 返回 `i64`。

```cpp
struct Line {
    i64 k, b;
    i64 eval(i64 x) const { return k * x + b; }
};

bool bad(const Line& a, const Line& b, const Line& c) {
    return (i128)(b.b - a.b) * (b.k - c.k) >=
           (i128)(c.b - b.b) * (a.k - b.k);
}

struct HullMin {
    deque<Line> q;

    void add_line(i64 k, i64 b) {
        Line cur{k, b};
        while (q.size() >= 2 && bad(q[q.size() - 2], q.back(), cur)) q.pop_back();
        q.push_back(cur);
    }

    i64 query(i64 x) {
        while (q.size() >= 2 && q[0].eval(x) >= q[1].eval(x)) q.pop_front();
        return q.front().eval(x);
    }
};
```
#### Li Chao Tree

> **赛时先看**
> - **题目信号**：斜率不单调、查询 `x` 不单调，普通凸包队列不能用。
> - **本质**：动态加入直线，查询某个 `x` 处最小值。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部函数参数直接当题面参数。
> - **复杂度判定**：插入/查询 `O(log X)`。
> - **维护的量**：线段树节点 `tr[p]`（每节点存区间中点更优的一条直线，含 `lc/rc/has`）；建树值域 `[L, R]`。
> - **警告**：需要知道 `x` 的值域；若 `x` 是离散集合，可以写离散 Li Chao。


**最小完整示例（先抄这一段就能跑）：**

值域 `[-5, 5]`，加入 `y = 2x+1` 与 `y = -x+9`，查询 `x = 3` 处最小值。

```cpp
LiChao lc(-5, 5);
lc.add_line(2, 1);
lc.add_line(-1, 9);
i64 best = lc.query(3); // 2*3+1 = 7，-1*3+9 = 6
// 样例输出：best = 6
```

**传参要求（照这个传不会错）：**

- 构造 `LiChao lc(XL, XR)`：整数查询值域 `[XL, XR]`，之后所有 `x` 必须落在其中；本版维护**最小值**。
- `add_line(k, b)`：加入直线 `y = k*x + b`（`i64`），斜率任意、可乱序、可重复。
- `query(x)`：`XL <= x <= XR`；返回所有已加入直线在 `x` 处的最小值（`i64`），一条直线都没有时返回 `INF = 1LL << 62`。

**不会用就照抄：**

```cpp
LiChao lc(XL, XR);
lc.add_line(k, b);
auto best = lc.query(x);
```

- 先确认模板维护最大值还是最小值。
- `x` 的允许范围必须落在建树值域 `[XL,XR]`。


**API / 入口函数（赛时只认这里列的名字）：**

- `LiChao lc(XL,XR)` -> 建立整数 x 值域 `[XL,XR]`，本版维护最小值。
- `lc.add_line(k,b)` -> 加入直线 `y=kx+b`。
- `lc.query(x)` -> 查询所有已加入直线在 x 处的最小值。
**核心逻辑（改代码时别破坏）：**

- 每个节点保留在区间中点更优的直线；另一条只可能在左半或右半继续有机会。
- 查询沿 x 所在根到叶路径取最优值；无需斜率单调。

```cpp
struct LiChao {
    struct Line {
        i64 k, b;
        i64 get(i64 x) const { return k * x + b; }
    };

    struct Node {
        Line line;
        int lc = 0, rc = 0;
        bool has = false;
    };

    vector<Node> tr;
    i64 L, R;
    const i64 INF = (1LL << 62);

    LiChao(i64 L_, i64 R_) : L(L_), R(R_) {
        tr.push_back(Node{});
        tr.push_back(Node{});
    }

    int new_node() {
        tr.push_back(Node{});
        return (int)tr.size() - 1;
    }

    void add_line(Line nw, int p, i64 l, i64 r) {
        if (!tr[p].has) {
            tr[p].line = nw;
            tr[p].has = true;
            return;
        }
        i64 mid = (l + r) >> 1;
        Line lo = tr[p].line, hi = nw;
        if (lo.get(mid) > hi.get(mid)) swap(lo, hi);
        tr[p].line = lo;
        if (l == r) return;
        if (lo.get(l) > hi.get(l)) {
            if (!tr[p].lc) tr[p].lc = new_node();
            add_line(hi, tr[p].lc, l, mid);
        } else if (lo.get(r) > hi.get(r)) {
            if (!tr[p].rc) tr[p].rc = new_node();
            add_line(hi, tr[p].rc, mid + 1, r);
        }
    }

    i64 query(i64 x, int p, i64 l, i64 r) const {
        if (!p) return INF;
        i64 ans = tr[p].has ? tr[p].line.get(x) : INF;
        if (l == r) return ans;
        i64 mid = (l + r) >> 1;
        if (x <= mid) return min(ans, query(x, tr[p].lc, l, mid));
        return min(ans, query(x, tr[p].rc, mid + 1, r));
    }

    void add_line(i64 k, i64 b) { add_line({k, b}, 1, L, R); }
    i64 query(i64 x) const { return query(x, 1, L, R); }
};
```
#### 分治优化 DP

> **赛时先看**
> - **题目信号**：`dp[g][i] = min(prev[k] + cost(k+1,i))`，并且决策点随 `i` 不下降。
> - **本质**：分组 DP，最优决策点单调。
> - **接法**：每层分组把 `prev/cur` 准备好，调用 `compute_dc(1, n, 0, n - 1, prev, cur, cost)`；`cost(l, r)` 是闭区间组代价，写成 lambda 传入。
> - **复杂度判定**：`O(G n log n)`。
> - **维护的量**：`prev`（上一层 DP，只读）；`cur`（当前层，递归内逐段填，`cur[0]` 由外部初始化）；`opt_l/opt_r`（当前段决策点允许区间）；`mid/opt`（段中点与中点处最优决策点）。
> - **警告**：必须确认决策单调，否则会错。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 示例：n=5 个元素分 G=2 组，组代价 cost(l,r)=段长，求最小总代价。
// 真正用时把 cost 换成题目自己的闭区间代价，并保证决策点单调。
int n = 5, G = 2;
vector<i64> prev(n + 1, 0), cur(n + 1);
auto cost = [](int l, int r) { return (i64)(r - l + 1); };
for (int g = 1; g <= G; ++g) {
    fill(cur.begin(), cur.end(), (1LL << 60));
    cur[0] = 0;
    compute_dc(1, n, 0, n - 1, prev, cur, cost);
    prev.swap(cur);
}
cout << prev[n] << '\n'; // 5（每段代价加起来就是 n）
```

- 样例：输出 `5`。

**传参要求（照这个传不会错）：**

- `prev`：上一层 DP 数组（`vector<i64>`，下标 `0..n`）；`cur`：当前层输出，长度与 `prev` 相同，`cur[0]` 需外部初始化（求最小为 `0`，求最大为 `-INF`）。
- `cost(l, r)`：闭区间 `[l, r]` 的组代价，返回 `i64`；内部按 `cost(k+1, mid)` 调用。
- 入口：`compute_dc(1, n, 0, n - 1, prev, cur, cost)`，每层分组调用一次后 `prev.swap(cur)`。
- 前提：最优决策点随 `i` 单调不降，否则答案错误。


```cpp
// 维护的量：prev（上一层 DP，只读）；cur（当前层，递归中逐段填）；opt_l/opt_r（决策点允许区间）；
//          mid/opt（段中点与中点处的最优决策点）。
// 不变量：进入 compute_dc(l,r,opt_l,opt_r) 时，段内所有位置的最优决策点都落在 [opt_l,opt_r]，
//         且随下标单调不降，所以左右半段可以各自收窄决策区间。
template <class Cost>
void compute_dc(int l, int r, int opt_l, int opt_r,
                const vector<i64>& prev,
                vector<i64>& cur,
                Cost cost) {
    if (l > r) return;
    int mid = (l + r) >> 1;
    pair<i64, int> best = {(1LL << 60), -1};
    // 只扫 [opt_l, min(mid-1, opt_r)]：决策单调让每层总扫描量只有 O(n)
    for (int k = opt_l; k <= min(mid - 1, opt_r); ++k) {
        best = min(best, {prev[k] + cost(k + 1, mid), k});
    }
    cur[mid] = best.first;
    int opt = best.second;
    compute_dc(l, mid - 1, opt_l, opt, prev, cur, cost);   // 左半段决策点不超过 opt
    compute_dc(mid + 1, r, opt, opt_r, prev, cur, cost);   // 右半段决策点不小于 opt
}
```

#### SMAWK：全单调矩阵每行最优值

> **赛时先看**
> - **题目信号**：`opt[i]` 随行号不下降；转移能看成矩阵 `A[i][j]` 的每行最小值；题目进一步给出 Monge 性/四边形不等式/凸代价，或要求 min-plus/max-plus 卷积；分治优化的 `O(n log n)` 仍不够。
> - **本质**：不显式建出矩阵，在一个全单调（totally monotone）矩阵中求每一行的最优列。它是“决策单调 DP”的线性级别工具，适合 `n*m` 已经无法枚举、但可以 `O(1)` 比较两个候选的情况。
> - **接法**：把候选比较写成 `better(row, candidate, current)` 后调用 `smawk_row_minimum(n, m, better)` 获取每行最优列；典型适用场景见下方典题模型。
> - **复杂度判定**：比较次数 `O(n+m)`，其中矩阵有 `n` 行、`m` 列。前提是矩阵满足全单调性；代码不验证此前提。
> - **维护的量**：`reduced`（压缩后的候选列表，长度不超过行数）；`odd_answer`（奇数行的答案，递归求得）；`answer`（每行最优列，最终全部填满）。
> - **警告**：`better(row, candidate, current)` 必须表示“candidate 严格更优”，相等时返回 `false`，这样会保留更小列号。列编号必须按升序传入。一般的“最优决策单调”不自动推出可用 SMAWK，先确认全单调/Monge 条件。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 全单调矩阵 A[i][j] = (i-j)^2（4 行 5 列），求每行最小值的列。
auto opt = smawk_row_minimum(4, 5, [&](int i, int a, int b) {
    return (i64)(i - a) * (i - a) < (i64)(i - b) * (i - b); // 严格更优返回 true
});
for (int x : opt) cout << x << ' '; // 0 1 2 3（第 i 行最优列是 i）
```

- 样例：输出 `0 1 2 3`。

**传参要求（照这个传不会错）：**

- `smawk_row_minimum(n, m, better)`：`n` 行、`m` 列；`better(row, candidate, current)` 表示 `candidate` 列在 `row` 行严格优于 `current` 列（相等返回 `false`，会保留更小列号）；返回 `vector<int>`，第 `i` 个元素是第 `i` 行的最优列。
- 前提：矩阵必须全单调/Monge（代码不验证）；列号按升序传入。
- `smawk_rows(rows, cols, better)`：底层函数，行列都以升序 `vector<int>` 传入；一般只用 `smawk_row_minimum`。


典题模型：Monge 数组行最小值；凸费用的 min-plus 卷积；区间 DP 或分组 DP 的更高阶优化；运输/匹配代价矩阵的批量最优决策。

```cpp
// 维护的量：reduced（候选列压缩表，长度不超过行数）；odd_answer（奇数行答案，递归求得）；
//          answer（每行最优列；偶数行在相邻奇数行答案之间收窄范围后扫描）。
// 不变量：每轮调用后，任何行的最优列都仍落在 reduced 中；奇数行答案已定，偶数行只需
//         在左右相邻奇数行答案之间的列里扫描（全单调性保证答案必在该区间内）。
// better(row,candidate_col,current_col) 必须表示候选列严格更优。
// rows 和 cols 都按升序排列；返回每一行的最优列。
template <class Better>
vector<int> smawk_rows(const vector<int>& rows, const vector<int>& cols,
                       const Better& better) {
    if (rows.empty()) return {};

    // 把候选列缩到不超过行数。
    vector<int> reduced;
    for (int col : cols) {
        while (!reduced.empty()) {
            int row = rows[(int)reduced.size() - 1];
            if (better(row, col, reduced.back())) reduced.pop_back(); // 新列优于旧列才淘汰
            else break;
        }
        if ((int)reduced.size() < (int)rows.size()) reduced.push_back(col);
    }

    vector<int> odd_rows;
    for (int i = 1; i < (int)rows.size(); i += 2) odd_rows.push_back(rows[i]);
    vector<int> odd_answer = smawk_rows(odd_rows, reduced, better);

    vector<int> answer(rows.size(), -1);
    for (int i = 0; i < (int)odd_answer.size(); ++i) answer[2 * i + 1] = odd_answer[i];

    // 在相邻奇数行答案之间，只扫描必要的列范围。
    for (int i = 0; i < (int)rows.size(); i += 2) {
        int left = 0, right = (int)reduced.size() - 1;
        if (i > 0) left = (int)(lower_bound(reduced.begin(), reduced.end(), answer[i - 1]) - reduced.begin());
        if (i + 1 < (int)rows.size()) {
            right = (int)(lower_bound(reduced.begin(), reduced.end(), answer[i + 1]) - reduced.begin());
        }
        answer[i] = reduced[left];
        for (int j = left + 1; j <= right; ++j) {
            if (better(rows[i], reduced[j], answer[i])) answer[i] = reduced[j];
        }
    }
    return answer;
}

// 维护的量：rows/cols（0..n-1、0..m-1 的行列号表，供底层 smawk_rows 使用）。
// 不变量：返回第 i 行的最优列；比较全部交给 better。
template <class Better>
vector<int> smawk_row_minimum(int n, int m, Better better) {
    vector<int> rows(n), cols(m);
    iota(rows.begin(), rows.end(), 0);
    iota(cols.begin(), cols.end(), 0);
    return smawk_rows(rows, cols, better);
}

// 以 A[i][j] 为例的调用方式：
// 示例：auto opt = smawk_row_minimum(n, m, [&](int i, int a, int b) {
// 示例：return value(i, a) < value(i, b); // strict: ties keep the smaller column.
// 示例：});
```
#### WQS 二分

> **赛时先看**
> - **题目信号**：目标是最大/最小权值，同时限制选取个数；选取个数随惩罚值单调变化。
> - **本质**：要求“恰好选 k 个”但普通 DP 多一维很慢，通过给每个选择加惩罚项把数量约束二分掉。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部函数参数直接当题面参数。
> - **复杂度判定**：`O(log V * check)`。
> - **维护的量**：`WqsResult{value, count}`（当前 λ 下的最优值与对应选取个数）；二分边界 `lo/hi`。
> - **警告**：`check(lambda)` 必须同时返回最优值和选取数量；最后答案要把惩罚项加/减回去。


**最小完整示例（先抄这一段就能跑）：**

数组 `{3, 5, 1, 8, 2}`，恰好选 2 个互不相邻元素。

```cpp
vector<i64> a = {3, 5, 1, 8, 2};
i64 ans = maximum_sum_exactly_k_non_adjacent(a, 2);
// 样例输出：ans = 13（选 a[1]=5 与 a[3]=8）
```

**传参要求（照这个传不会错）：**

- `maximum_sum_exactly_k_non_adjacent(a, k)`：**主入口**。`a` 是原数组（0-based），元素 `a[i]` 不能为 `LLONG_MIN`（断言）；`k` 为恰好选取的个数，`0 <= k <= (n+1)/2`（越界断言）；返回恰好选 `k` 个互不相邻元素的最大和（`i64`），`k = 0` 时返回 `0`。
- `check_non_adjacent(a, lambda)`：判定器，仅供主入口内部调用。`lambda` 是每个被选元素减去的惩罚；返回 `WqsResult{value, count}`：`value` 为已减惩罚的最优值，`count` 为达到该值的选取个数（同值时偏向多选，保证数量随 λ 单调）。

**API / 入口函数（赛时只认这里列的名字）：**

- `maximum_sum_exactly_k_non_adjacent(const vector<i64>& a, int k)` -> **主入口，先看这个**：恰好选 k 个互不相邻元素的最大和，内部二分惩罚并反复调用 check。 返回 `i64`。
- `check_non_adjacent(const vector<i64>& a, i64 lambda)` -> 给定 λ 的判定器，仅供 `maximum_sum_exactly_k_non_adjacent` 内部使用，不要从 `solve()` 直接调。 返回 `WqsResult`。

```cpp
struct WqsResult {
    i64 value; // 已减去 lambda * count 后的最优值。
    int count;
};

WqsResult wqs_better(WqsResult a, WqsResult b) {
    if (a.value != b.value) return a.value > b.value ? a : b;
    return a.count > b.count ? a : b; // 同值时偏向选得更多，保证数量关于 lambda 单调。
}

// 完整典型模型：从数组中恰好选 k 个互不相邻元素，最大化元素和。
// 对每个被选元素减 lambda，check 同时维护最优值和选中个数。
WqsResult check_non_adjacent(const vector<i64>& a, i64 lambda) {
    const i64 NEG = -(1LL << 60);
    WqsResult skip{0, 0};
    WqsResult take{NEG, INT_MIN / 2};
    for (i64 x : a) {
        WqsResult next_take{skip.value + x - lambda, skip.count + 1};
        WqsResult next_skip = wqs_better(skip, take);
        skip = next_skip;
        take = next_take;
    }
    return wqs_better(skip, take);
}

i64 maximum_sum_exactly_k_non_adjacent(const vector<i64>& a, int k) {
    int n = (int)a.size();
    assert(0 <= k && k <= (n + 1) / 2);
    if (k == 0) return 0;

    i64 bound = 1;
    for (i64 x : a) {
        assert(x != LLONG_MIN); // llabs(LLONG_MIN) 无法用 i64 表示。
        bound = max(bound, llabs(x) + 1);
    }
    i64 lo = -bound, hi = bound;
    while (lo < hi) {
        i64 mid = lo + (hi - lo + 1) / 2;
        if (check_non_adjacent(a, mid).count >= k) lo = mid;
        else hi = mid - 1;
    }
    WqsResult result = check_non_adjacent(a, lo);
    return result.value + lo * k; // 把恰好 k 份惩罚加回。
}
```
#### Slope Trick：分段线性凸函数 DP

> **赛时先看**
> - **题目信号**：DP 状态形如 `dp[i][x]`，`x` 是整数位置/高度/时间；转移可写成对前一层取一个区间最小值；代价是绝对值、单边折线或凸函数；直接枚举 `x` 太大。
> - **本质**：维护一元凸函数的最小值与最优点区间，特别适合不断叠加 `max(a-x,0)`、`max(x-a,0)`、`|x-a|`，以及做“相邻状态变化范围限制”的 DP。
> - **接法**：按 DP 转移逐层调用下方 API 的 `add_a_minus_x`/`add_x_minus_a`/`shift`/`prefix_min`/`suffix_min` 维护凸函数；典型应用见下方典题模型。
> - **复杂度判定**：每个加折线操作 `O(log n)`，平移和取前缀/后缀最小值 `O(1)`（清空堆除外）。空间 `O（操作数）`。
> - **维护的量**：`min_f`（当前函数全局最小值）；`L/R`（左/右折点堆，真实折点 = 堆内值 + `add_l`/`add_r`）；`add_l/add_r`（两堆整体懒平移量）。
> - **警告**：`shift(a,b)` 的语义是 `g(x)=min_{x-b <= y <= x-a} f(y)`，要求 `a<=b`。`prefix_min()` 是 `g(x)=min_{y<=x}f(y)`，而 `suffix_min()` 是 `g(x)=min_{y>=x}f(y)`，不要写反。该模板维护的是凸的分段线性函数，不能直接拿去处理任意函数。
> - **约定**：本模板维护一元凸的分段线性函数，初始代表恒为 `0` 的函数；各操作的语义见下方 API 与代码前说明。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 把序列改成非降序列的最小绝对偏差代价。
vector<i64> a = {1, 4, 3, 2, 5};
SlopeTrick st;
for (i64 x : a) {
    st.add_x_minus_a(x); // 限制当前值 >= 上一段的值
    st.prefix_min();     // 取前缀最小值，保证结果非降
}
cout << st.min_value() << '\n'; // 2（改成 1 3 3 3 5）
```

- 样例：输出 `2`。

**传参要求（照这个传不会错）：**

- `add_a_minus_x(a)`：加代价项 `f(x) += max(a-x, 0)`；`add_x_minus_a(a)`：加代价项 `f(x) += max(x-a, 0)`；两者连用即加 `|x-a|`。
- `shift(a, b)`：转移 `g(x) = min_{x-b <= y <= x-a} f(y)`，要求 `a <= b`。
- `prefix_min()`：转移 `g(x) = min_{y <= x} f(y)`；`suffix_min()`：转移 `g(x) = min_{y >= x} f(y)`；不要写反。
- `min_value()`：当前函数全局最小值；`argmin()`：所有最优 `x` 构成的闭区间（无界时端点取 `±INF`）。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_a_minus_x(i64 a)` -> 加入代价项 f(x) += max(a-x,0)。
- `add_x_minus_a(i64 a)` -> 加入代价项 f(x) += max(x-a,0)。
- `prefix_min()` -> 转移为 g(x)=min_{y<=x} f(y)。
- `shift(i64 a, i64 b)` -> 转移为 g(x)=min_{x-b<=y<=x-a} f(y)，其中 a<=b。
- `suffix_min()` -> 转移为 g(x)=min_{y>=x} f(y)。
**改板时先认这几个量：**

- `add_l`/`add_r`：左右两个堆各自的整体懒平移。
- `L`：存左折点，真实值 = raw + add_l。
- `R`：右折点。

使用：初始代表恒为 `0` 的函数。`add_a_minus_x(a)` 加 `max(a-x,0)`；`add_x_minus_a(a)` 加 `max(x-a,0)`；两者一起就是加 `|x-a|`。`min_value()` 给函数全局最小值，`argmin()` 给所有最优 `x` 构成的闭区间。

典题模型：把序列改成非降/非升，代价为绝对偏差；限制相邻高度变化不超过 `D` 的最小代价；区间最小转移 + 单点绝对值罚分；OI Wiki Slope Trick 专题的序列修复与安全高度模型。

```cpp
// 维护的量：min_f（当前分段线性凸函数的最小值）；L/R（左/右折点堆，真实折点 = 堆内值 + add_l/add_r）；
//          add_l/add_r（两个堆的整体懒平移量）。
// 不变量：函数是凸的分段线性函数；按真实值排序时 L 中所有折点 <= R 中所有折点，
//         全局最小值在闭区间 [top_l, top_r] 上取到，min_f 就是它的值。
struct SlopeTrick {
    static constexpr i64 INF = (1LL << 60);

    i64 min_f = 0;       // 当前函数的全局最小值
    i64 add_l = 0, add_r = 0; // 两个堆的整体懒平移
    priority_queue<i64> L;    // 存左折点，真实值 = raw + add_l
    priority_queue<i64, vector<i64>, greater<i64>> R; // 右折点

    bool empty_l() const { return L.empty(); }
    bool empty_r() const { return R.empty(); }
    i64 top_l() const { return L.top() + add_l; }
    i64 top_r() const { return R.top() + add_r; }
    void push_l(i64 x) { L.push(x - add_l); }
    void push_r(i64 x) { R.push(x - add_r); }
    i64 pop_l() { i64 x = top_l(); L.pop(); return x; }
    i64 pop_r() { i64 x = top_r(); R.pop(); return x; }

    // 加入代价项 f(x) += max(a-x,0)。
    void add_a_minus_x(i64 a) {
        if (!empty_r() && top_r() < a) {
            min_f += a - top_r();
            i64 r = pop_r();
            push_r(a);
            push_l(r); // 最小值上移 a-top_r，原右折点转成左折点
        } else {
            push_l(a); // 最小值区间覆盖到 a：只新增左折点，min_f 不变
        }
    }

    // 加入代价项 f(x) += max(x-a,0)。
    void add_x_minus_a(i64 a) {
        if (!empty_l() && top_l() > a) {
            min_f += top_l() - a;
            i64 l = pop_l();
            push_l(a);
            push_r(l);
        } else {
            push_r(a);
        }
    }

    void add_abs(i64 a) { // 加入绝对值代价项 f(x)+=|x-a|。
        add_a_minus_x(a);
        add_x_minus_a(a);
    }

    // 转移为 g(x)=min_{x-b<=y<=x-a} f(y)，其中 a<=b。
    void shift(i64 a, i64 b) {
        assert(a <= b);
        add_l += a; // 懒平移：真实折点 = 堆内值 + 懒标记
        add_r += b;
    }

    // 转移为 g(x)=min_{y<=x} f(y)。
    void prefix_min() { while (!R.empty()) R.pop(); }
    // 转移为 g(x)=min_{y>=x} f(y)。
    void suffix_min() { while (!L.empty()) L.pop(); }

    i64 min_value() const { return min_f; }
    pair<i64, i64> argmin() const {
        return {empty_l() ? -INF : top_l(), empty_r() ? INF : top_r()};
    }
};
```
#### 最大子段和与最大子矩阵

> **赛时先看**
> - **题目信号**：连续子数组/子矩阵最大和。这里的“子段/子数组”要求连续；允许跳过元素的是子序列，不适用 Kadane。
> - **本质**：Kadane 算法（最大子段和）和二维压缩。
> - **接法**：一维直接 `max_subarray_sum(a)`；二维把矩阵丢给 `max_submatrix_sum(mat)`；全负数组/矩阵时答案取最大元素，两者都已处理。
> - **复杂度判定**：一维 `O(n)`，二维 `O(n^2 m)`。
> - **维护的量**：一维 `cur`（以当前元素结尾的最大子段和）/`best`（历史最大）；二维另加 `col[j]`（当前上下边界间第 j 列的列和）。
> - **警告**：全负数时答案应为最大元素，不是 0。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 一维最大子段和与二维最大子矩阵和（元素可为负，全负时取最大元素）。
vector<i64> a = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
cout << max_subarray_sum(a) << '\n'; // 6
vector<vector<i64>> mat = {
    {1, -2, 3},
    {-4, 5, 6},
    {7, -8, 9}
};
cout << max_submatrix_sum(mat) << '\n'; // 18（取整个矩阵）
```

- 样例：输出 `6` 与 `18`。

**传参要求（照这个传不会错）：**

- `max_subarray_sum(a)`：`a` 非空（内部断言）；返回 `i64` 最大非空连续子段和；全负数时返回最大元素。
- `max_submatrix_sum(a)`：`a` 为非空 `vector<vector<i64>>`；返回 `i64` 最大非空子矩阵和；内部对每对上下边界跑 Kadane（`O(n^2 m)`）。
- 两者都要求“连续”，允许跳过元素的问题不适用。


```cpp
// 维护的量：cur（以当前元素结尾的最大子段和）；best（扫描至今的最大子段和）。
// 不变量：处理完 a[0..i] 后，best 就是这段的最大非空子段和；cur < 0 时前缀被丢弃、重开一段。
i64 max_subarray_sum(const vector<i64>& a) {
    assert(!a.empty());
    i64 best = a[0], cur = 0;
    for (i64 x : a) {
        cur = max(x, cur + x); // 要么从 x 重开一段，要么接上前面
        best = max(best, cur);
    }
    return best;
}

// 维护的量：col[j]（当前 [top, bot] 行区间内第 j 列的列和）；ans（历史最大子矩阵和）。
// 不变量：固定 top/bot 后，问题退化成在 col 上求一维最大子段和。
i64 max_submatrix_sum(const vector<vector<i64>>& a) {
    assert(!a.empty() && !a[0].empty());
    int n = a.size(), m = a[0].size();
    i64 ans = LLONG_MIN;
    for (int top = 0; top < n; ++top) {
        vector<i64> col(m, 0);
        for (int bot = top; bot < n; ++bot) {
            for (int j = 0; j < m; ++j) col[j] += a[bot][j]; // 把 bot 行并入当前列和
            ans = max(ans, max_subarray_sum(col));
        }
    }
    return ans;
}
```
#### 全 1 最大矩形

> **赛时先看**
> - **题目信号**：网格只含 0/1，问最大全 1 子矩形。
> - **本质**：01 矩阵中最大全 1 矩形面积。
> - **接法**：把网格按 `vector<string>` 传给 `maximal_all_one_rectangle(grid)` 一行出答案；单条直方图题也可以单独用 `largest_rectangle_histogram_zero_based(h)`。
> - **复杂度判定**：`O(nm)`。
> - **维护的量**：`h[j]`（以当前行为底边时第 j 列连续 1 的高度）；`st`（高度不减的下标栈）；`ans`（扫描中维护的最大面积）。
> - **警告**：每行当作直方图底边，用单调栈。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 01 网格中最大全 1 矩形面积。
vector<string> grid = {
    "10100",
    "10111",
    "11111",
    "10010"
};
cout << maximal_all_one_rectangle(grid) << '\n'; // 6
```

- 样例：输出 `6`（第 3 行起第 2..4 列构成 2x3 的矩形）。

**传参要求（照这个传不会错）：**

- `maximal_all_one_rectangle(grid)`：`grid` 为 `vector<string>`，每行等长、只含 `'0'`/`'1'`；返回 `int` 最大全 1 矩形面积（空网格返回 0）。
- `largest_rectangle_histogram_zero_based(h)`：`h` 为 0-indexed 柱高数组（`vector<int>`），返回 `int` 直方图最大矩形面积；单独柱状图题也能用。
- `maximal_all_one_rectangle` 内部调用 `largest_rectangle_histogram_zero_based`，两个函数一起抄。


```cpp
// 维护的量：st（高度单调不减的下标栈）；ans（当前最大矩形面积）。
// 不变量：弹出高度 h[i] 时，它的左右边界是栈内前一个元素与 i，贡献面积 h[i]*(i-left-1) 即最优。
int largest_rectangle_histogram_zero_based(const vector<int>& h) {
    vector<int> st;
    int n = h.size(), ans = 0;
    for (int i = 0; i <= n; ++i) {
        int cur = (i == n ? 0 : h[i]); // 末尾补 0 强制把栈清空，确保所有高度都被计算
        while (!st.empty() && h[st.back()] >= cur) {
            int height = h[st.back()];
            st.pop_back();
            int left = st.empty() ? -1 : st.back();
            ans = max(ans, height * (i - left - 1)); // 以 height 为高的矩形宽度
        }
        st.push_back(i);
    }
    return ans;
}

// 维护的量：h[j]（以当前行为底边时第 j 列连续 1 的高度）；ans（历史最大面积）。
// 不变量：h 就是直方图高度；逐行累加高度后套用直方图模板，答案即最大全 1 矩形。
int maximal_all_one_rectangle(const vector<string>& grid) {
    if (grid.empty()) return 0;
    int n = grid.size(), m = grid[0].size();
    vector<int> h(m, 0);
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) h[j] = (grid[i][j] == '1' ? h[j] + 1 : 0); // 遇 0 断高
        ans = max(ans, largest_rectangle_histogram_zero_based(h));
    }
    return ans;
}
```

