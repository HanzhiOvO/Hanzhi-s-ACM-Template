## C 树、图论与网络流

### 05 树上问题

树题通常是图论、数据结构和 DP 的混合题。本章按直径、LCA、剖分、欧拉序、树 DP、点分治、虚树的比赛翻阅顺序整理。
#### 树的直径

> **赛时先看**
> - **题目信号**：树上最长路径、最远距离、通信延迟最大值。
> - **本质**：求树上最远两点距离。
> - **接法**：树上最长路径直接 `tree_diameter(g)`。无权树把每条边权设为 `1`；带权树用真实边权。若题目还要输出直径端点，保留两次 `farthest` 的返回点即可。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`g`（邻接表，边权 `{v, w}`）、`dist`（起点到各点距离）。
> - **警告**：带权树要累加边权；两次 DFS/BFS。
> - **约定**：顶点需从 1 开始编号；否则把起点改成实际第一个点。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点的带权树（顶点从 1 开始编号），求直径长度。

```cpp
int n = 3;                                   // 样例输入，抄题时换成你的输入
vector<vector<pair<int, i64>>> g(n + 1);     // 1. 邻接表：顶点从 1 开始编号
for (int i = 1; i < n; ++i) { int u, v; i64 w; cin >> u >> v >> w; g[u].push_back({v, w}); g[v].push_back({u, w}); }  // 2. 加无向带权边
cout << tree_diameter(g) << '\n';            // 3. 直接调用，返回直径长度
```

样例：链 `1-2-3`，边权 `1, 2`；直径 -> `3`。

**传参要求（照这个传不会错）：**

- `g`：邻接表，`g[u]` 存 `{v, w}`；顶点从 **1** 开始编号（函数用 `g.size()-1` 当点数）。
- `tree_diameter(g)`：返回 `i64` 直径长度；内部自动做两次 `farthest`，无需 build。
- `farthest(s, g)`：返回 `pair<int, i64>`：`first` 是离 `s` 最远的点，`second` 是最远距离；要输出直径端点就保留这两次返回值。
- 无权树把所有边权传 `1` 即可。


**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
pair<int, i64> farthest(int s, const vector<vector<pair<int, i64>>>& g) {
    int n = (int)g.size() - 1;
    vector<i64> dist(n + 1, -1);
    queue<int> q;
    q.push(s);
    dist[s] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto [v, w] : g[u]) {
            if (dist[v] != -1) continue;
            dist[v] = dist[u] + w;
            q.push(v);
        }
    }
    int id = s;
    for (int i = 1; i <= n; ++i) if (dist[i] > dist[id]) id = i;
    return {id, dist[id]};
}

i64 tree_diameter(const vector<vector<pair<int, i64>>>& g) {
    auto [a, _] = farthest(1, g);
    auto [b, diam] = farthest(a, g);
    return diam;
}
```
#### LCA：倍增

> **赛时先看**
> - **题目信号**：树不修改，反复问"两点路径/最近公共祖先/距离/向上跳 k 步"。看到"多次树上路径、祖先、距离询问"，先想 LCA 倍增。
> - **本质**：用倍增表把"沿树向上走"压缩成 `O(log n)` 次跳跃：`up[k][u]` 预存 `u` 的 `2^k` 级祖先，任意祖先关系与路径询问按二进制位逐段跳拼出来。
> - **复杂度判定**：预处理 `O(n log n)`，查询 `O(log n)`；`n,q` 到 2e5 轻松过。若树静态且询问可全量离线，Tarjan 离线 LCA 更省一个 log；树会改则换树链剖分。
> - **维护的量**：`depth`（深度）、`up`（倍增祖先表）、`dist`（根到点的带权距离）。
> - **接法**：先 `LCA solver(n)`，每条无向边 `add_edge(u,v,w)`，所有边加完 `build(root)` 一次。问最近公共祖先调用 `lca(u,v)`；问距离调用 `distance(u,v)`；问向上跳调用 `jump(u,k)`。如果边无权，`w` 默认用 `1`，距离就是边数。
> - **警告**：根的父亲设成根（`dfs(root, root)`），防止倍增跳出树外；`LOG` 要覆盖 `n`（模板已自动算好，别手改）；`build` 前不要查询。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点的树，`q` 次询问两点最近公共祖先（LCA）和距离。

```cpp
LCA solver(n);                       // 1. 结构体定义：LCA(点数 n)
for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    solver.add_edge(u, v);           // 2. 加边（无向边加一次即可）
}
solver.build(1);                     // 3. 建树：根从 1 开始；必须先 build 再查询
while (q--) {
    int u, v;
    cin >> u >> v;
    cout << solver.lca(u, v) << '\n';       // 最近公共祖先
    cout << solver.distance(u, v) << '\n';  // 树上距离（边数）
}
```

样例：链 `1-2-3`；`lca(1,3)` -> `1`；`distance(1,3)` -> `2`。

**传参要求（照这个传不会错）：**

- `LCA(n)`：构造；点编号 `1..n`。
- `add_edge(u, v, w=1)`：加无向边；边带权时传 `w`（`distance` 按带权距离算）。
- `build(root)`：所有边加完后调用一次；`root` 是树根；build 前查询是错的。
- `lca(u, v)`：返回最近公共祖先（`int`）。
- `distance(u, v)`：返回 `u` 到 `v` 的带权距离（`i64`）。
- `jump(u, k)`：从 `u` 向上跳 `k` 步，越界返回 0。
- 边数很多时 `LOG` 模板已自动算好，别手改。


**不会用就照抄：**

```cpp
LCA solver(n);
solver.add_edge(u, v);   // 每条树边都加
solver.build(root);      // 所有边加完以后只 build 一次
cout << solver.lca(u, v) << '\n';
```

- 顺序固定：`构造 -> add_edge -> build(root) -> lca()`。
- 节点通常是 1..n；`build` 前不要查询。


**API / 入口函数（赛时只认这里列的名字）：**

- `LCA solver(n)` -> 初始化 1..n 的树。
- `solver.add_edge(u,v,w)` -> 加无向边；无权树可省略 `w`。
- `solver.build(root)` -> 所有边加入后预处理；查询前必须调用。
- `solver.lca(u,v)` -> 最近公共祖先。
- `solver.jump(u,k)` -> 从 `u` 向上跳 `k` 条边。
- `solver.distance(u,v)` -> 按边权求两点距离。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `struct LCA`（含 `dfs`/`build`/`jump`/`lca`/`distance`）。
2. 构造：`LCA solver(n);`，`n` 是树的点数。
3. 加边：每条树边 `solver.add_edge(u, v, w);`，无权树可省略 `w`（默认 1）。
4. 预处理：所有边加完后 `solver.build(root);`，只调用一次。
5. 查询：`solver.lca(u, v)` / `solver.distance(u, v)` / `solver.jump(u, k)`。
6. 取结果：返回值直接用；距离用 `i64` 接收。

**改造点（按题目改这几处）：**
- 根的选择：不指定时默认 `root = 1`；题面要求"以 x 为根"就把 `x` 传进 `build`。
- `LOG` 大小：模板按 `n` 自动算好，不用改；只有 `n` 极大（1e6+）时留意 `(1 << LOG)` 别溢出。
- 边权 `w`：无权树省略（默认 1，距离即边数）；带权树传真实边权，`distance` 自动返回带权和。
- 编号：模板按 1..n；题给 0..n-1 就全部下标 +1 再用。
- 只问"u 是否为 v 的祖先"：判断 `lca(u,v) == u` 即可。

**核心逻辑（改代码时别破坏）：**

- `up[k][u]` 是 `u` 的 `2^k` 级祖先。
- 先把两点拉到同深度，再从大到小尝试一起跳，最后父亲就是 LCA。
**改板时先认这几个量：**

- `g`：邻接表。
- `up`：倍增祖先表。
- `depth`：深度。
- `dist`：距离。

```cpp
// 维护的量：depth[u] = u 的深度；up[k][u] = u 向上 2^k 步的祖先；dist[u] = 根到 u 的带权距离。
// 不变量：depth + up 支撑 lca 的"拉平再齐跳"；dist 供 distance 用前缀差求路径长。
struct LCA {
    int n, LOG;
    vector<vector<pair<int, i64>>> g;
    vector<vector<int>> up;
    vector<int> depth;
    vector<i64> dist;

    LCA(int n = 0) { if (n) init(n); }

    void init(int n_) {
        n = n_;
        LOG = 1;
        while ((1 << LOG) <= n) LOG++; // LOG 满足 2^LOG > n，覆盖所有跳跃步长
        g.assign(n + 1, {});
        up.assign(LOG, vector<int>(n + 1));
        depth.assign(n + 1, 0);
        dist.assign(n + 1, 0);
    }

    void add_edge(int u, int v, i64 w = 1) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    void dfs(int u, int p) {
        up[0][u] = p;
        for (int k = 1; k < LOG; ++k) up[k][u] = up[k - 1][up[k - 1][u]]; // 2^k 祖先 = 2^(k-1) 祖先的 2^(k-1) 祖先
        for (auto [v, w] : g[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dist[v] = dist[u] + w; // 累加根到点的带权距离，供 distance 查询
            dfs(v, u);
        }
    }

    void build(int root = 1) {
        dfs(root, root); // 根的父亲设成根自己，防止倍增跳出树外
    }

    int jump(int u, int steps) const {
        for (int k = 0; k < LOG; ++k) {
            if ((steps >> k) & 1) u = up[k][u]; // steps 按二进制位拆成若干次 2^k 跳
        }
        return u;
    }

    int lca(int a, int b) const {
        if (depth[a] < depth[b]) swap(a, b);
        a = jump(a, depth[a] - depth[b]); // 先把深的拉到与浅的同一深度
        if (a == b) return a;
        for (int k = LOG - 1; k >= 0; --k) {
            if (up[k][a] != up[k][b]) { // 从大到小试跳，跳后仍不同才跳，保证不越过 LCA
                a = up[k][a];
                b = up[k][b];
            }
        }
        return up[0][a]; // 两指针的父亲就是 LCA
    }

    i64 distance(int a, int b) const {
        int c = lca(a, b);
        return dist[a] + dist[b] - 2 * dist[c]; // 路径长 = 两段根距离之和减两倍 LCA 距离
    }
};
```
#### Tarjan 离线 LCA：并查集批量求最近公共祖先

> **赛时先看**
> - **题目信号**：树不修改、询问可以全部先读入、只需要最终一次性输出；`n,q` 很大，想避免倍增的 `O(q log n)`，或题目本身就是 DFS 回溯场景。
> - **本质**：一棵静态树（或森林）上的所有 LCA 询问都已提前读入时，以并查集离线求每一对点的最近公共祖先。
> - **接法**：给一棵 `n <= 5e5` 的静态树和 `q` 个“求 `u,v` 的 LCA”询问，所有询问先输入、后统一输出。对边调用 `add_edge`，每个询问保存 `id = add_query(u,v)`，`auto ans = solver.solve(root)` 后输出 `ans[id]`。如果题目混有在线改根、修改边或动态查询，则回到倍增、树链剖分或 LCT，不能用这份离线板子。
> - **复杂度判定**：`O((n + q) alpha(n))`，其中 `alpha` 是反阿克曼函数；空间 `O(n+q)`。
> - **维护的量**：`g`（邻接表）、`query[u]`（挂在 u 上的询问 `{另一端， 编号}`）、`answer`（答案数组）、`dsu/ancestor`（并查集与代表元祖先）。
> - **警告**：这是离线算法，必须先 `add_query` 再 `solve`；并查集的合并发生在“孩子子树 DFS 完成”之后；无根森林里不同连通块的询问答案保持 `-1`；深链很深时要留意递归栈。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点的树，`q` 个"求 `u,v` 的 LCA"询问全部先读入、最后统一输出。

```cpp
int n = 3, q = 2;                            // 样例输入，抄题时换成你的输入
TarjanOfflineLCA solver(n);                  // 1. 结构体定义：TarjanOfflineLCA(点数 n)
for (int i = 1; i < n; ++i) { int u, v; cin >> u >> v; solver.add_edge(u, v); }  // 2. 加无向树边
vector<int> id(q);
for (int i = 0; i < q; ++i) { int u, v; cin >> u >> v; id[i] = solver.add_query(u, v); }  // 3. 询问先登记
vector<int> ans = solver.solve(1);           // 4. 登记完再 solve，一次性出全部答案
for (int i = 0; i < q; ++i) cout << ans[id[i]] << '\n';   // 5. 按编号取答案
```

样例：链 `1-2-3`；询问 `(1,3)` -> `1`；`(2,3)` -> `2`。

**传参要求（照这个传不会错）：**

- `TarjanOfflineLCA(n)`：构造；点编号 `1..n`。
- `add_edge(u, v)`：加无向边。
- `add_query(u, v)`：登记一条询问，返回编号 `id`（从 0 开始）；**必须先 add_query 再 solve**。
- `solve(root=1)`：全部 add_query 后调用一次；剩余点自动当森林；返回 `vector<int>`，`ans[id]` 是该询问的 LCA，不同连通块为 `-1`。
- 在线查询不能用这份板子（倍增 LCA 才支持在线）。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `solve(int root = 1)` -> 优先选 root；剩余点也允许组成森林。 返回 `vector<int>`。
**改板时先认这几个量：**

- `g`：邻接表。
- `query`：{另一个端点， 询问编号}。

```cpp
struct TarjanOfflineLCA {
    int n;
    vector<vector<int>> g;
    vector<vector<pair<int, int>>> query; // {另一个端点, 询问编号}。
    vector<int> dsu, ancestor, component, answer;
    vector<char> done;

    explicit TarjanOfflineLCA(int n_)
        : n(n_), g(n_ + 1), query(n_ + 1) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int add_query(int u, int v) {
        int id = (int)answer.size();
        answer.push_back(-1);
        query[u].push_back({v, id});
        query[v].push_back({u, id});
        return id;
    }

    int find(int x) { return dsu[x] == x ? x : dsu[x] = find(dsu[x]); }

    void dfs(int u, int parent, int component_id) {
        dsu[u] = u;
        ancestor[u] = u;
        component[u] = component_id;
        for (int v : g[u]) {
            if (v == parent) continue;
            dfs(v, u, component_id);
            dsu[find(v)] = u;      // 把已经处理完的儿子集合合并到 u 的集合中。
            ancestor[find(u)] = u; // u 所在集合的代表元对应 u 的答案祖先。
        }
        done[u] = 1;
        for (auto [v, id] : query[u]) {
            if (done[v] && component[v] == component[u]) {
                answer[id] = ancestor[find(v)];
            }
        }
    }

    // 优先选 root；剩余点也允许组成森林。
    vector<int> solve(int root = 1) {
        dsu.resize(n + 1);
        iota(dsu.begin(), dsu.end(), 0);
        ancestor.assign(n + 1, 0);
        component.assign(n + 1, -1);
        done.assign(n + 1, 0);
        fill(answer.begin(), answer.end(), -1);

        int component_id = 0;
        if (1 <= root && root <= n) dfs(root, 0, component_id++);
        for (int u = 1; u <= n; ++u) {
            if (!done[u]) dfs(u, 0, component_id++);
        }
        return answer;
    }
};
```

典题模型：给一棵 `n <= 5e5` 的静态树和 `q` 个“求 `u,v` 的 LCA”询问，所有询问先输入、后统一输出。对边调用 `add_edge`，每个询问保存 `id = add_query(u,v)`，`auto ans = solver.solve(root)` 后输出 `ans[id]`。如果题目混有在线改根、修改边或动态查询，则回到倍增、树链剖分或 LCT，不能用这份离线板子。
#### 长链剖分：O(1) 第 k 级祖先

> **赛时先看**
> - **题目信号**：题目专门大量问“第 `k` 个祖先/向上跳 `k` 步”，`q` 很大，树不修改；或需要在复杂树上 DP 中频繁取祖先。普通 LCA、只问少量祖先时优先使用更直观的倍增。
> - **本质**：在静态有根树上反复询问节点 `u` 向父亲走 `k` 条边后的祖先。相比倍增的 `O(log n)`，预处理后单次查询 `O(1)`。
> - **接法**：给定根为 `1` 的树，`q <= 1e6` 次询问 `(u,k)`，要求第 `k` 级祖先。建边后 `solver.build(1)`，每次输出 `solver.kth_ancestor(u,k)`；如果题目把根编号看作自己的祖先，遇到返回 `0` 时按题意改成根或 `-1`。
> - **复杂度判定**：预处理 `O(n log n)`，空间 `O(n log n)`（倍增表），每次合法查询 `O(1)`。
> - **维护的量**：`parent/depth`（父亲与深度）、`up`（倍增祖先表）、`heavy/top`（长链重儿子与链顶）、`down/above`（长链向下节点与链顶上方祖先）。
> - **警告**：`k=0` 答案就是自身；`k > depth[u]` 时本模板返回 `0`；长链按“向下最长”儿子选择，不是重链剖分按子树大小选择；根的父亲为 `0`。


**最小完整示例（先抄这一段就能跑）：**

题目：根为 `1` 的树，`q` 次询问 `(u,k)` 求第 `k` 级祖先。

```cpp
int n = 4, q = 2;                            // 样例输入，抄题时换成你的输入
LongChainKthAncestor solver(n);              // 1. 结构体定义：LongChainKthAncestor(点数 n)
for (int i = 1; i < n; ++i) { int u, v; cin >> u >> v; solver.add_edge(u, v); }  // 2. 加无向边
solver.build(1);                             // 3. 建树：根从 1 开始；必须先 build 再查询
while (q--) { int u, k; cin >> u >> k; cout << solver.kth_ancestor(u, k) << '\n'; }  // 4. 第 k 级祖先
```

样例：链 `1-2-3-4`；`kth_ancestor(4,2)` -> `2`；`kth_ancestor(2,5)` -> `0`。

**传参要求（照这个传不会错）：**

- `LongChainKthAncestor(n)`：构造；点编号 `1..n`。
- `add_edge(u, v)`：加无向边。
- `build(root=1)`：所有边加完后调用一次；`root` 是树根；build 前查询是错的。
- `kth_ancestor(u, k)`：返回 `u` 的第 `k` 级祖先（`int`）；`k=0` 返回自身，越过根返回 `0`（越界情况按题意自行改）。
- 只问少量祖先且 `q` 不大时，直接用更直观的倍增 LCA 的 `jump` 也行。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `build(int root = 1)` -> 完成建树或预处理
- `init(int n_)` -> 初始化/清空结构
- `kth_ancestor(int u, int k)` -> 如果第 k 级祖先越过根节点，返回 0。
**改板时先认这几个量：**

- `up`：倍增祖先表。
- `parent`：树上父节点。
- `heavy`：长链重儿子。
- `top`：长链链顶。
- `g`：邻接表。

```cpp
struct LongChainKthAncestor {
    int n, LOG;
    vector<vector<int>> g, up;
    vector<int> parent, depth, longest, heavy, top;
    vector<vector<int>> down, above;

    explicit LongChainKthAncestor(int n_ = 0) {
        if (n_) init(n_);
    }

    void init(int n_) {
        n = n_;
        LOG = 1;
        while ((1 << LOG) <= n) ++LOG;
        g.assign(n + 1, {});
        up.assign(LOG, vector<int>(n + 1, 0));
        parent.assign(n + 1, 0);
        depth.assign(n + 1, 0);
        longest.assign(n + 1, 0);
        heavy.assign(n + 1, 0);
        top.assign(n + 1, 0);
        down.assign(n + 1, {});
        above.assign(n + 1, {});
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs1(int u, int p) {
        parent[u] = p;
        up[0][u] = p;
        for (int j = 1; j < LOG; ++j) up[j][u] = up[j - 1][up[j - 1][u]];
        longest[u] = 1;
        heavy[u] = 0;
        for (int v : g[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs1(v, u);
            if (longest[v] > longest[heavy[u]]) heavy[u] = v;
        }
        longest[u] += longest[heavy[u]];
    }

    void dfs2(int u, int chain_top) {
        top[u] = chain_top;
        if (u == chain_top) {
            // 长链上的节点，按从浅到深排列。
            for (int x = u; x; x = heavy[x]) down[chain_top].push_back(x);
            // O(1) 查询公式最多只会用到这么多级祖先。
            for (int x = parent[u]; x && (int)above[chain_top].size() < longest[u]; x = parent[x]) {
                above[chain_top].push_back(x);
            }
        }
        if (heavy[u]) dfs2(heavy[u], chain_top);
        for (int v : g[u]) {
            if (v != parent[u] && v != heavy[u]) dfs2(v, v);
        }
    }

    void build(int root = 1) {
        depth[root] = 0;
        dfs1(root, 0);
        dfs2(root, root);
    }

    // 如果第 k 级祖先越过根节点，返回 0。
    int kth_ancestor(int u, int k) const {
        if (k < 0 || k > depth[u]) return 0;
        if (k == 0) return u;
        int bit = 31 - __builtin_clz(k);
        u = up[bit][u];
        k -= 1 << bit;
        if (k == 0) return u;

        int chain_top = top[u];
        int inside = depth[u] - depth[chain_top];
        if (k <= inside) return down[chain_top][inside - k];
        return above[chain_top][k - inside - 1];
    }
};
```

典题模型：给定根为 `1` 的树，`q <= 1e6` 次询问 `(u,k)`，要求第 `k` 级祖先。建边后 `solver.build(1)`，每次输出 `solver.kth_ancestor(u,k)`；如果题目把根编号看作自己的祖先，遇到返回 `0` 时按题意改成根或 `-1`。
#### 树链剖分 HLD

> **赛时先看**
> - **题目信号**：多次修改或查询树上两点路径，不只是求 LCA。
> - **本质**：树上路径查询/修改，配合线段树。
> - **接法**：HLD 本身只负责把树上路径拆成若干段，真正维护和/最大值/赋值的是外面的线段树。通常先按 `dfn[u]` 把点权放进数组建线段树；路径查询时循环跳重链，每段查询 `[dfn[top], dfn[u]]`；子树查询直接查 `[dfn[u], dfn[u]+sz[u]-1]`。
> - **复杂度判定**：预处理 `O(n)`，每条路径拆成 `O(log n)` 段。
> - **维护的量**：`parent/depth`（父亲与深度）、`sz`（子树大小）、`heavy`（重儿子）、`top`（链顶）、`dfn/rid`（DFS 序与逆映射）。
> - **警告**：`dfn` 后子树是连续区间；路径段交给线段树处理。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点的树，若干次"路径上每个点加 `x`"，最后问点 `u` 的值（区间加/单点查）。

```cpp
int n = 3, u = 2;                            // 样例输入，抄题时换成你的输入
HLD hld(n);                                  // 1. 结构体定义：HLD(点数 n)
for (int i = 1; i < n; ++i) { int u, v; cin >> u >> v; hld.add_edge(u, v); }  // 2. 加无向边
hld.build(1);                                // 3. 建树：根从 1 开始；必须先 build 再操作
vector<i64> diff(n + 2);                     // 4. 点权按 dfn 存的差分数组（区间加/单点查）
auto add_path = [&](int u, int v, i64 x) {   // 5. 路径每个点 +x
    hld.for_path(u, v, [&](int l, int r) { diff[l] += x; diff[r + 1] -= x; });  // 6. 路径拆成 O(log n) 段 dfn 区间
};
for (int i = 1; i <= n; ++i) diff[i] += diff[i - 1];  // 7. 前缀和还原点权
cout << diff[hld.dfn[u]] << '\n';            // 8. 点 u 当前值
```

样例：链 `1-2-3`；路径 `(1,3)` 加 `5` 后，点 `2` 的值 -> `5`。

**传参要求（照这个传不会错）：**

- `HLD(n)`：构造；点编号 `1..n`。
- `add_edge(u, v)`：加无向边。
- `build(root=1)`：所有边加完后调用一次；必须先 build，`dfn/sz` 才有值。
- `for_path(a, b, visit)`：把路径 `a-b`（含两端点）拆成 `O(log n)` 段，每段回调 `visit(l, r)`，`[l, r]` 是 dfn 区间。
- `subtree_range(u)`：返回 `{dfn[u], dfn[u]+sz[u]-1}`，子树是连续区间，直接交给线段树。
- `lca(a, b)`：返回最近公共祖先（`int`）。
- 点权按 `dfn[u]` 放进线段树数组；`dfn`/`rid` 是公开成员。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `build(int root = 1)` -> 完成建树或预处理
- `init(int n_)` -> 初始化/清空结构
- `lca(int a, int b)` -> 最近公共祖先 返回 `int`。
**改板时先认这几个量：**

- `g`：邻接表。
- `parent`：树上父节点。
- `sz`：子树大小。
- `dfn`：DFS 序时间戳。
- `depth`：深度。

```cpp
struct HLD {
    int n, timer = 0;
    vector<vector<int>> g;
    vector<int> parent, depth, heavy, sz, top, dfn, rid;

    HLD(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        parent.assign(n + 1, 0);
        depth.assign(n + 1, 0);
        heavy.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        top.assign(n + 1, 0);
        dfn.assign(n + 1, 0);
        rid.assign(n + 1, 0);
        timer = 0;
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs1(int u, int p) {
        parent[u] = p;
        sz[u] = 1;
        heavy[u] = 0;
        for (int v : g[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs1(v, u);
            sz[u] += sz[v];
            if (!heavy[u] || sz[v] > sz[heavy[u]]) heavy[u] = v;
        }
    }

    void dfs2(int u, int tp) {
        top[u] = tp;
        dfn[u] = ++timer;
        rid[timer] = u;
        if (heavy[u]) dfs2(heavy[u], tp);
        for (int v : g[u]) {
            if (v != parent[u] && v != heavy[u]) dfs2(v, v);
        }
    }

    void build(int root = 1) {
        depth[root] = 0;
        dfs1(root, 0);
        dfs2(root, root);
    }

    int lca(int a, int b) const {
        while (top[a] != top[b]) {
            if (depth[top[a]] < depth[top[b]]) b = parent[top[b]];
            else a = parent[top[a]];
        }
        return depth[a] < depth[b] ? a : b;
    }

    template <class F>
    void for_path(int a, int b, F visit) const {
        while (top[a] != top[b]) {
            if (depth[top[a]] < depth[top[b]]) swap(a, b);
            visit(dfn[top[a]], dfn[a]);
            a = parent[top[a]];
        }
        if (depth[a] > depth[b]) swap(a, b);
        visit(dfn[a], dfn[b]);
    }

    pair<int, int> subtree_range(int u) const {
        return {dfn[u], dfn[u] + sz[u] - 1};
    }
};
```
#### 树上主席树：路径点权第 k 小

> **赛时先看**
> - **题目信号**：树上多次问“两点路径中第 k 小/有多少值不超过 x”，点权或边权固定，`n,q` 常到 `2e5`。经典模型是 SPOJ `COT - Count on a tree`。
> - **本质**：点权静态、不修改的树上路径顺序统计。询问 `u-v` 路径上所有**节点权值**的第 `k` 小；边权问题把一条边的权放在较深端节点即可。
> - **接法**：每个节点有整数权值，不修改；`q` 次询问 `(u,v,k)` 求路径上的第 `k` 小点权。读取权值到 1-indexed 数组 `a`，调用 `solver.set_values(a)`、`solver.build(1)`，答案是 `solver.kth_value(u,v,k)`。要问“路径上不超过 `x` 的点数”时，可在同样四个版本的线段树上做前缀计数。
> - **复杂度判定**：离散化后预处理 `O(n log n)`，每次路径第 k 小 `O(log n)`，空间 `O(n log n)`。
> - **维护的量**：`values/pos`（点权与离散化下标）、`root[u]`（根到 u 路径的主席树版本）、`up/depth`（倍增祖先与深度）、`tr`（线段树节点池）。
> - **警告**：本模板统计的是路径两端都包含的节点；组合版本时是 `root[u] + root[v] - root[lca] - root[parent(lca)]`，不能减两次 `root[lca]`；`k` 从 `1` 开始；权值相同会被离散到同一位置，返回原始权值。
> - **约定**：a 使用 1-indexed，a[1]..a[n] 是各点点权


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点、点权固定不修改；`q` 次询问 `(u,v,k)` 求路径上点权第 `k` 小。

```cpp
int n = 3, q = 1;                            // 样例输入，抄题时换成你的输入
vector<i64> a = {0, 3, 1, 2};                // 样例输入，抄题时换成你的输入（1-indexed 点权 a[1..n]）
TreePathKth solver(n);                       // 1. 结构体定义：TreePathKth(点数 n)
for (int i = 1; i < n; ++i) { int u, v; cin >> u >> v; solver.add_edge(u, v); }  // 2. 加无向边
solver.set_values(a);                        // 3. 传 1-indexed 点权数组 a[1..n]
solver.build(1);                             // 4. 建树：根从 1 开始；必须先 build 再查询
while (q--) { int u, v, k; cin >> u >> v >> k; cout << solver.kth_value(u, v, k) << '\n'; }  // 5. 路径第 k 小
```

样例：链 `1-2-3`，点权 `{3,1,2}`；`kth_value(1,3,2)` -> `2`。

**传参要求（照这个传不会错）：**

- `TreePathKth(n)`：构造；点编号 `1..n`。
- `add_edge(u, v)`：加无向边。
- `set_values(a)`：传 1-indexed 点权数组 `a[1..n]`；内部自动离散化；必须在 build 之前调用。
- `build(tree_root=1)`：set_values 且加完边后调用一次；build 前查询是错的。
- `kth_value(u, v, k)`：返回 `u-v` 路径上第 `k` 小的**原始权值**（`i64`）；`k` 从 1 开始；路径含两端点。
- 边权题：把每条边权放到较深端节点即可。


**API / 入口函数（赛时只认这里列的名字）：**

- `set_values(const vector<i64>& a)` -> 读入 1-indexed 点权 a[1..n]。
- `build(int tree_root = 1)` -> 建树/预处理；查询前必须调用。
- `kth_value(int u, int v, int k)` -> 查询 u-v 路径第 k 小的实际值，k 从 1 开始。 返回 `i64`。
**改板时先认这几个量：**

- `up`：倍增祖先表。
- `tr`：树节点池（节点数组）。

```cpp
struct TreePathKth {
    struct Node {
        int left = 0, right = 0, sum = 0;
    };

    int n, LOG, value_count;
    vector<vector<int>> g, up;
    vector<int> depth, root, pos;
    vector<i64> values, coordinate;
    vector<Node> tr;

    explicit TreePathKth(int n_ = 0) {
        if (n_) init(n_);
    }

    void init(int n_) {
        n = n_;
        LOG = 1;
        while ((1 << LOG) <= n) ++LOG;
        g.assign(n + 1, {});
        up.assign(LOG, vector<int>(n + 1, 0));
        depth.assign(n + 1, 0);
        root.assign(n + 1, 0);
        pos.assign(n + 1, 0);
        values.assign(n + 1, 0);
        coordinate.clear();
        tr.assign(1, {}); // 0 号版本表示空版本。
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // a 使用 1-indexed，a[1]..a[n] 是各点点权。
    void set_values(const vector<i64>& a) {
        values = a;
        coordinate.assign(a.begin() + 1, a.end());
        sort(coordinate.begin(), coordinate.end());
        coordinate.erase(unique(coordinate.begin(), coordinate.end()), coordinate.end());
        value_count = (int)coordinate.size();
        for (int u = 1; u <= n; ++u) {
            pos[u] = (int)(lower_bound(coordinate.begin(), coordinate.end(), values[u]) - coordinate.begin()) + 1;
        }
        tr.assign(1, {});
        tr.reserve((n + 5) * (LOG + 1));
    }

    int insert(int previous, int l, int r, int x) {
        int current = (int)tr.size();
        tr.push_back(tr[previous]);
        ++tr[current].sum;
        if (l == r) return current;
        int mid = (l + r) >> 1;
        if (x <= mid) tr[current].left = insert(tr[previous].left, l, mid, x);
        else tr[current].right = insert(tr[previous].right, mid + 1, r, x);
        return current;
    }

    void dfs(int u, int p) {
        up[0][u] = p;
        for (int j = 1; j < LOG; ++j) up[j][u] = up[j - 1][up[j - 1][u]];
        root[u] = insert(root[p], 1, value_count, pos[u]);
        for (int v : g[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }

    void build(int tree_root = 1) {
        assert(value_count > 0);
        depth[tree_root] = 0;
        dfs(tree_root, 0);
    }

    int lca(int u, int v) const {
        if (depth[u] < depth[v]) swap(u, v);
        int gap = depth[u] - depth[v];
        for (int j = 0; j < LOG; ++j) if (gap >> j & 1) u = up[j][u];
        if (u == v) return u;
        for (int j = LOG - 1; j >= 0; --j) {
            if (up[j][u] != up[j][v]) u = up[j][u], v = up[j][v];
        }
        return up[0][u];
    }

    int kth_index(int ru, int rv, int rlca, int rparent, int k) const {
        int l = 1, r = value_count;
        while (l < r) {
            int left_count = tr[tr[ru].left].sum + tr[tr[rv].left].sum
                           - tr[tr[rlca].left].sum - tr[tr[rparent].left].sum;
            int mid = (l + r) >> 1;
            if (k <= left_count) {
                ru = tr[ru].left;
                rv = tr[rv].left;
                rlca = tr[rlca].left;
                rparent = tr[rparent].left;
                r = mid;
            } else {
                k -= left_count;
                ru = tr[ru].right;
                rv = tr[rv].right;
                rlca = tr[rlca].right;
                rparent = tr[rparent].right;
                l = mid + 1;
            }
        }
        return l;
    }

    i64 kth_value(int u, int v, int k) const {
        int w = lca(u, v);
        int pw = up[0][w];
        int count = tr[root[u]].sum + tr[root[v]].sum - tr[root[w]].sum - tr[root[pw]].sum;
        assert(1 <= k && k <= count);
        return coordinate[kth_index(root[u], root[v], root[w], root[pw], k) - 1];
    }
};
```

典题模型：每个节点有整数权值，不修改；`q` 次询问 `(u,v,k)` 求路径上的第 `k` 小点权。读取权值到 1-indexed 数组 `a`，调用 `solver.set_values(a)`、`solver.build(1)`，答案是 `solver.kth_value(u,v,k)`。要问“路径上不超过 `x` 的点数”时，可在同样四个版本的线段树上做前缀计数。
#### Euler 序 + 树状数组：子树加、单点查

> **赛时先看**
> - **题目信号**：操作只针对“某个节点的整个子树”，不涉及任意两点路径。
> - **本质**：树上子树整体加值，查询某个点当前值。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：DFS `O(n)`，每次操作 `O(log n)`。
> - **维护的量**：`tin/tout`（DFS 进出时间戳）、`fw`（差分树状数组，下标为时间戳）。
> - **警告**：DFS 序中一个子树是连续区间 `[tin[u], tout[u]]`；必须先 dfs 再操作，否则 tin/tout 全 0。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点的树，`q` 次操作：子树整体加 `x` / 问点 `u` 当前值。

```cpp
int n = 3;                                   // 样例输入，抄题时换成你的输入
SubtreeAddPointQuery solver(n);              // 1. 结构体定义：SubtreeAddPointQuery(点数 n)
for (int i = 1; i < n; ++i) { int u, v; cin >> u >> v; solver.add_edge(u, v); }  // 2. 加无向边
solver.dfs(1, 0);                            // 3. 必须先 dfs 出 tin/tout 再操作
solver.subtree_add(2, 5);                    // 4. 点 2 的整个子树 +5
cout << solver.point_query(3) << '\n';       // 5. 点 3 当前值
```

样例：链 `1-2-3`；`subtree_add(2,5)` 后 `point_query(3)` -> `5`。

**传参要求（照这个传不会错）：**

- `SubtreeAddPointQuery(n)`：构造；点编号 `1..n`。
- `add_edge(u, v)`：加无向边。
- `dfs(root, 0)`：必须最先调用一次（如 `dfs(1, 0)`），否则 `tin/tout` 全 0。
- `subtree_add(u, val)`：给点 `u` 的整个子树加 `val`（内部对 `[tin[u], tout[u]]` 区间加）。
- `point_query(u)`：返回点 `u` 当前值（`i64`），即 `tin[u]` 处前缀和。


**API / 入口函数（赛时只认这里列的名字）：**

- `dfs(root)` -> 必须先调用，否则 tin/tout 全 0。
- `subtree_add(u, val)` -> 给 u 的子树整体加 val。
- `point_query(x)` -> 查询点 x 的当前值。
**改板时先认这几个量：**

- `bit`：Fenwick 内部树状数组。
- `g`：邻接表。

```cpp
template <class T>
struct Fenwick {
    int n;
    vector<T> bit;
    Fenwick(int n = 0) { init(n); }
    void init(int n_) { n = n_; bit.assign(n + 1, T{}); }
    void add(int i, T v) { for (; i <= n; i += i & -i) bit[i] += v; }
    T sum(int i) const { T r{}; for (; i > 0; i -= i & -i) r += bit[i]; return r; }
};

struct SubtreeAddPointQuery {
    int n, timer = 0;
    vector<vector<int>> g;
    vector<int> tin, tout;
    Fenwick<i64> fw;

    SubtreeAddPointQuery(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        timer = 0;
        g.assign(n + 1, {});
        tin.assign(n + 1, 0);
        tout.assign(n + 1, 0);
        fw.init(n + 2);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        tin[u] = ++timer;
        for (int v : g[u]) if (v != p) dfs(v, u);
        tout[u] = timer;
    }

    void subtree_add(int u, i64 val) {
        fw.add(tin[u], val);
        fw.add(tout[u] + 1, -val);
    }

    i64 point_query(int u) const {
        return fw.sum(tin[u]);
    }
};
```
#### Euler 序 + 树状数组：子树和查询

> **赛时先看**
> - **题目信号**：树上“子树和”，单点权值会变化。
> - **本质**：点权修改，查询某个子树的权值和。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：DFS `O(n)`，每次操作 `O(log n)`。
> - **维护的量**：`tin/tout`（DFS 进出时间戳）、`val[u]`（点 u 当前权值）、`fw`（按时间戳存点权的树状数组）。
> - **警告**：把点 `u` 的权值放在 `tin[u]` 位置，子树就是连续区间；必须先 dfs 再操作，否则 tin/tout 全 0。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点带权的树，`q` 次操作：改点权 / 问子树点权和。

```cpp
int n = 3;                                   // 样例输入，抄题时换成你的输入
SubtreeSum solver(n);                        // 1. 结构体定义：SubtreeSum(点数 n)
for (int i = 1; i < n; ++i) { int u, v; cin >> u >> v; solver.add_edge(u, v); }  // 2. 加无向边
solver.dfs(1, 0);                            // 3. 必须先 dfs（dfs 时把初始点权放进 BIT）
solver.set_value(2, 10);                     // 4. 点 2 权值改为 10（必须在 dfs 之后）
solver.set_value(3, 4);
cout << solver.subtree_sum(1) << '\n';       // 5. 点 1 子树点权和
```

样例：链 `1-2-3`，初始全 0；改点权为 `10, 4` 后 `subtree_sum(1)` -> `14`。

**传参要求（照这个传不会错）：**

- `SubtreeSum(n)`：构造；点编号 `1..n`。
- `add_edge(u, v)`：加无向边。
- `dfs(root, 0)`：必须最先调用（如 `dfs(1, 0)`），dfs 时按当前 `val` 初始化 BIT；之后才能改权/查询。
- `set_value(u, v)`：把点 `u` 权值改为 `v`；必须在 dfs 之后调用。
- `subtree_sum(u)`：返回点 `u` 子树所有点权和（`i64`）。


**API / 入口函数（赛时只认这里列的名字）：**

- `dfs(root)` -> 必须先调用，否则 tin/tout 全 0。
- `set_value(x, v)` -> 把点 x 的权值设为 v；必须在 dfs() 之后调用，因为树状数组在 dfs 中初始化。
- `subtree_sum(x)` -> 查询点 x 的子树的点权和。
**改板时先认这几个量：**

- `bit`：Fenwick 内部树状数组。
- `g`：邻接表。

```cpp
// 与上一节的 Fenwick 同名功能重复，这里改名避免跨节照抄冲突。
template <class T>
struct FenwickRangeSum {
    int n;
    vector<T> bit;
    FenwickRangeSum(int n = 0) { init(n); }
    void init(int n_) { n = n_; bit.assign(n + 1, T{}); }
    void add(int i, T v) { for (; i <= n; i += i & -i) bit[i] += v; }
    T sum(int i) const { T r{}; for (; i > 0; i -= i & -i) r += bit[i]; return r; }
    T range_sum(int l, int r) const { return sum(r) - sum(l - 1); }
};

struct SubtreeSum {
    int n, timer = 0;
    vector<vector<int>> g;
    vector<int> tin, tout;
    FenwickRangeSum<i64> fw;
    vector<i64> val;

    SubtreeSum(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        timer = 0;
        g.assign(n + 1, {});
        tin.assign(n + 1, 0);
        tout.assign(n + 1, 0);
        val.assign(n + 1, 0);
        fw.init(n + 2);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        tin[u] = ++timer;
        fw.add(tin[u], val[u]);
        for (int v : g[u]) if (v != p) dfs(v, u);
        tout[u] = timer;
    }

    void set_value(int u, i64 new_val) {
        fw.add(tin[u], new_val - val[u]);
        val[u] = new_val;
    }

    i64 subtree_sum(int u) const {
        return fw.range_sum(tin[u], tout[u]);
    }
};
```
#### 树上差分：路径加，点统计

> **赛时先看**
> - **题目信号**：很多路径 `[u,v]`，最后统一问点/边贡献。
> - **本质**：多次给树上路径加 1，最后统计每个点被多少条路径经过。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：`O((n+q) log n)`，最后 DFS 汇总。
> - **维护的量**：`diff`（差分数组，1-indexed）、`g`（邻接表）、`up`（倍增祖先表，借 LCA 的公开成员）。
> - **警告**：点差分：`cnt[u]++ cnt[v]++ cnt[lca]-- cnt[parent[lca]]--`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点的树，`q` 条路径，每条路径上的点计数 +1，最后输出每个点的覆盖次数。

依赖：LCA（倍增）节 struct，抄板时一起抄上。

```cpp
int n = 3, q = 1;                            // 样例输入，抄题时换成你的输入
LCA lca_solver(n);                           // 1. 建 LCA 倍增（提供 lca 与 up）
vector<vector<int>> g(n + 1);                // 2. 另存一份邻接表供 DFS 汇总
for (int i = 1; i < n; ++i) { int u, v; cin >> u >> v; lca_solver.add_edge(u, v); g[u].push_back(v); g[v].push_back(u); }
lca_solver.build(1);                         // 3. 必须先 build 再差分
vector<i64> diff(n + 1);
for (int i = 0; i < q; ++i) { int u, v; cin >> u >> v; add_path_point(u, v, diff, lca_solver); }  // 4. 每条路径一次点差分
collect_tree_diff(1, 0, g, diff);            // 5. 自底向上汇总，diff[u] = u 的覆盖次数
```

样例：链 `1-2-3`；路径 `(1,3)` 加一次 -> `diff[2]` -> `1`。

**传参要求（照这个传不会错）：**

- `add_path_point(u, v, diff, solver)`：对路径 `u-v` 做点差分（内部用 `solver.lca(u,v)` 与 `solver.up[0][lca]`）；`diff` 是 1-indexed 的 `vector<i64>`。
- `collect_tree_diff(u, p, g, diff)`：从根 DFS 汇总子节点差分到父亲；`g` 必须是 `vector<vector<int>>` 邻接表；汇总完 `diff[u]` 就是点 u 被覆盖次数。
- 必须先把 `LCA` build 好再调用 `add_path_point`；`up` 是 LCA 的公开成员。
- 边统计版：`diff[u]++ diff[v]++ diff[lca] -= 2`，最后汇总边贡献。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_path_point(int u, int v, vector<i64>& diff, const LCA& solver)` -> 需要 LCA 的 lca(u,v) 和 parent[0][x]。
**改板时先认这几个量：**

- `diff`：差分数组（最后 DFS 汇总）。
- `g`：邻接表。
- `up`：倍增祖先表。

```cpp
// 需要 LCA 的 lca(u,v) 和 parent[0][x]。
void add_path_point(int u, int v, vector<i64>& diff, const LCA& solver) {
    int w = solver.lca(u, v);
    diff[u]++;
    diff[v]++;
    diff[w]--;
    int pw = solver.up[0][w];
    if (pw != w) diff[pw]--;
}

void collect_tree_diff(int u, int p, const vector<vector<int>>& g, vector<i64>& diff) {
    for (int v : g[u]) {
        if (v == p) continue;
        collect_tree_diff(v, u, g, diff);
        diff[u] += diff[v];
    }
}
```
#### 树上路径交

> **赛时先看**
> - **题目信号**：多条树上路径之间的公共部分。
> - **本质**：判断两条树上路径是否相交，或求路径交的端点辅助。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：使用 LCA 后 `O(log n)`。
> - **维护的量**：无自有成员；只用 `LCA` 的 `lca` 与 `distance`。
> - **警告**：需要一个 `is_on_path(x,a,b)` 判断点是否在路径上。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点的树，`q` 组询问：路径 `a-b` 与路径 `c-d` 是否相交。

依赖：LCA（倍增）节 struct，抄板时一起抄上。

```cpp
int n = 4, q = 1;                            // 样例输入，抄题时换成你的输入
LCA solver(n);                               // 1. 建 LCA 倍增：构造 + 加边
for (int i = 1; i < n; ++i) { int u, v; cin >> u >> v; solver.add_edge(u, v); }  // 2. 加无向边
solver.build(1);                             // 3. 必须先 build 再查询
while (q--) { int a, b, c, d; cin >> a >> b >> c >> d;
    cout << (path_intersect(solver, a, b, c, d) ? "YES" : "NO") << '\n'; }  // 4. 两路径是否相交
```

样例：链 `1-2-3-4`；`(1,2)` 与 `(3,4)` -> 不相交；`(1,3)` 与 `(2,4)` -> 相交。

**传参要求（照这个传不会错）：**

- `path_intersect(solver, a, b, c, d)`：判断路径 `a-b` 与 `c-d` 是否相交，返回 `bool`；内部先取两条路径的 LCA 再判断。
- `is_on_path(solver, x, a, b)`：判断点 `x` 是否在路径 `a-b` 上（距离等式 `dist(a,x)+dist(x,b)==dist(a,b)`），返回 `bool`。
- `solver` 必须先 build；无权树 `distance` 就是边数。


**API / 入口函数（赛时只认这里列的名字）：**

- `is_on_path(const LCA& solver, int x, int a, int b)` -> 依赖 LCA 结构中的 lca(a,b) 和 distance(a,b)。 返回 `bool`。

```cpp
// 依赖 LCA 结构中的 lca(a,b) 和 distance(a,b)。
bool is_on_path(const LCA& solver, int x, int a, int b) {
    return solver.distance(a, x) + solver.distance(x, b) == solver.distance(a, b);
}

bool path_intersect(const LCA& solver, int a, int b, int c, int d) {
    int x = solver.lca(a, b);
    int y = solver.lca(c, d);
    return is_on_path(solver, x, c, d) || is_on_path(solver, y, a, b);
}
```
#### 树形 DP：最大独立集

> **赛时先看**
> - **题目信号**：父子不能同时选；答案由子树合并。
> - **本质**：树上选择问题的基础模型。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`w[u]`（点权，默认 1）、`dp[u][0/1]`（不选/选 u 的子树最大值）、`g`（邻接表）。
> - **警告**：先定义清楚 `dp[u][0/1]`，再写转移。
> - **读答案**：答案在公开成员 `dp[u][0/1]` 中。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点带权树，父子不能同时选，求最大点权和（点权默认 1 时即最大独立集大小）。

```cpp
int n = 3;                                   // 样例输入，抄题时换成你的输入
vector<i64> w = {0, 5, 1, 4};                // 样例输入，抄题时换成你的输入（1-indexed 点权 w[1..n]）
TreeIndependentSet solver(n);                // 1. 结构体定义：TreeIndependentSet(点数 n)
for (int i = 1; i < n; ++i) { int u, v; cin >> u >> v; solver.add_edge(u, v); }  // 2. 加无向边
for (int i = 1; i <= n; ++i) solver.w[i] = w[i];   // 3. 赋点权（不赋默认全 1）
solver.dfs(1, 0);                            // 4. 根为 1 跑一遍树形 DP
cout << max(solver.dp[1][0], solver.dp[1][1]) << '\n';  // 5. 答案：根选/不选取 max
```

样例：链 `1-2-3`，点权 `{5,1,4}` -> 选 1 和 3，答案 `9`。

**传参要求（照这个传不会错）：**

- `TreeIndependentSet(n)`：构造；点编号 `1..n`。
- `add_edge(u, v)`：加无向边。
- `w[u]`：公开成员点权，默认 `1`；要在 dfs 之前赋好。
- `dfs(root, 0)`：从根调用一次即可（如 `dfs(1, 0)`）。
- `dp[u][0/1]`：公开成员，`dp[u][0]` 不选 u、`dp[u][1]` 选 u 的子树最大值；答案 `max(dp[root][0], dp[root][1])`。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `dp`：DP 状态。

```cpp
struct TreeIndependentSet {
    int n;
    vector<vector<int>> g;
    vector<array<i64, 2>> dp;
    vector<i64> w;

    TreeIndependentSet(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        dp.assign(n + 1, {0, 0});
        w.assign(n + 1, 1);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        dp[u][0] = 0;
        dp[u][1] = w[u];
        for (int v : g[u]) {
            if (v == p) continue;
            dfs(v, u);
            dp[u][0] += max(dp[v][0], dp[v][1]);
            dp[u][1] += dp[v][0];
        }
    }
};
```
#### 树上最大权匹配：每个点至多选一条邻边

> **赛时先看**
> - **题目信号**：配对只能发生在树的相邻点之间；每个点最多参与一次配对；问最多配对数、最大收益的边集合或树上的不相交边。
> - **本质**：从树边中选若干条两两不共端点的边，使总边权最大，并恢复选边。所有边权设为 `1` 就是最大匹配边数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O(n)` 时间、`O(n)` 空间；实现用迭代后序，链状树也不会递归爆栈。
> - **维护的量**：`best[u]`（u 自由时的最优值）、`blocked[u]`（u 已和父亲匹配时的最优值）、`chosen_child[u]`（恢复选边用）、`edges`（0-based 选边结果）。
> - **警告**：选边 `(u,v)` 时，`v` 不能再和自己的儿子配对，所以增益是 `edge_weight + blocked[v] - best[v]`。负权边可以不选；若题目强制恰好选若干条边，需要额外加维度，不能直接套。
> - **约定**：vector<pair<int, int>> edges; // 0-based 顶点编号


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点的带权树（顶点从 0 开始编号），每个点至多选一条邻边，求最大匹配权值和选边。

```cpp
int n = 3;                                   // 样例输入，抄题时换成你的输入
vector<vector<pair<int, i64>>> g(n);         // 1. 邻接表：顶点从 0 开始编号
for (int i = 0; i < n - 1; ++i) { int u, v; i64 w; cin >> u >> v >> w; g[u].push_back({v, w}); g[v].push_back({u, w}); }  // 2. 加无向带权边
TreeMatchingResult res = tree_maximum_weight_matching(g);  // 3. 一次调用出答案
cout << res.max_weight << '\n';              // 4. 最大匹配权值和
for (auto [u, v] : res.edges) cout << u << ' ' << v << '\n';  // 5. 选中的边（0-based）
```

样例：链 `0-1-2`，边权 `{5,3}` -> 选 `(0,1)`，权值 `5`。

**传参要求（照这个传不会错）：**

- `g`：邻接表 `g[u] = {v, w}`；顶点是 **0-based**（从 0 开始，不是 1），点数 = `g.size()`。
- `tree_maximum_weight_matching(g)`：一次调用返回 `TreeMatchingResult`；无需 build。
- `res.max_weight`：最大匹配权值和（`i64`）。
- `res.edges`：选中的边列表（`pair<int,int>`，0-based）；所有边权设为 1 就是最大匹配边数。
- 负权边自动不选；链状树也安全（迭代后序，不爆栈）。


**改板时先认这几个量：**

- `edges`：0-based 顶点编号。
- `best`：u 自由时的最优值。
- `blocked`：u 已和父亲匹配时的最优值。

```cpp
struct TreeMatchingResult {
    i64 max_weight = 0;
    vector<pair<int, int>> edges; // 0-based 顶点编号。
};

TreeMatchingResult tree_maximum_weight_matching(
    const vector<vector<pair<int, i64>>>& graph
) {
    int n = (int)graph.size();
    if (n == 0) return {};
    vector<int> parent(n, -1), order = {0};
    for (int i = 0; i < (int)order.size(); ++i) {
        int u = order[i];
        for (auto [v, weight] : graph[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            order.push_back(v);
        }
    }

    // blocked[u]：u 已经和父亲匹配，不能再选子边；best[u]：u 自由时最优。
    vector<i64> blocked(n), best(n);
    vector<int> chosen_child(n, -1);
    for (int index = n - 1; index >= 0; --index) {
        int u = order[index];
        i64 best_gain = 0;
        for (auto [v, weight] : graph[u]) {
            if (parent[v] != u) continue;
            blocked[u] += best[v];
            best_gain = max(best_gain, weight + blocked[v] - best[v]);
        }
        best[u] = blocked[u] + best_gain;
        if (best_gain > 0) {
            for (auto [v, weight] : graph[u]) {
                if (parent[v] == u && weight + blocked[v] - best[v] == best_gain) {
                    chosen_child[u] = v;
                    break;
                }
            }
        }
    }

    TreeMatchingResult result;
    result.max_weight = best[0];
    vector<pair<int, bool>> stack = {{0, false}}; // bool: 本点是否已经与父亲匹配。
    while (!stack.empty()) {
        auto [u, matched_to_parent] = stack.back();
        stack.pop_back();
        int picked = matched_to_parent ? -1 : chosen_child[u];
        for (auto [v, weight] : graph[u]) {
            if (parent[v] != u) continue;
            if (v == picked) {
                result.edges.push_back({u, v});
                stack.push_back({v, true});
            } else {
                stack.push_back({v, false});
            }
        }
    }
    return result;
}
```
#### 基环树最大点权独立集：剥叶 + 树 DP + 环 DP

> **赛时先看**
> - **题目信号**：题目给 `n` 点 `n` 边，或每个点都连向/依赖一个点；图不是树但每个连通块只有一个环；出现“不能同时选相邻点、上司与下属不能同时参加、骑士互相冲突”等约束。
> - **本质**：选一些点使任意相邻两点不能同时选，最大化点权和。输入可以有多个连通块，但要求每个连通块都恰好有一个环。
> - **复杂度判定**：`O(n)` 时间、`O(n)` 空间。
> - **维护的量**：`weight[u]`（点权）、`in_cycle`（是否在环上）、`take/skip`（选/不选该点的树部分 DP 值）。
> - **警告**：只支持"每个连通块恰好一个环"；自环不能当环处理；点权可为负，答案允许一个点也不选，初值别设成负无穷。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点 `n` 条边的无向图（每个连通块恰有一个环），点带权，相邻点不能同时选，求最大点权和。

```cpp
int n = 3;                                   // 样例输入，抄题时换成你的输入
vector<i64> weight(n);                       // 1. 点权：顶点从 0 开始编号
for (int i = 0; i < n; ++i) cin >> weight[i];
vector<pair<int, int>> edges(n);             // 2. 边数 = 点数（每个连通块恰一个环）
for (int i = 0; i < n; ++i) { int u, v; cin >> u >> v; edges[i] = {u, v}; }  // 3. 无向冲突边，0-based
cout << maximum_weight_independent_set_unicyclic_forest(weight, edges) << '\n';  // 4. 最大点权和
```

样例：三角形 `0-1-2`（3 条边），点权 `{1,2,3}` -> 只能选一个点，答案 `3`。

**传参要求（照这个传不会错）：**

- `weight`：点权数组，**0-based**（`weight[0..n-1]`）；题面从 1 编号就减一。
- `edges`：无向边列表，长度必须等于 `n`（每个连通块恰有一个环）；不支持自环，平行边长度 2 的环支持。
- `maximum_weight_independent_set_unicyclic_forest(weight, edges)`：返回 `i64` 最大点权和；多连通块自动累加；无需 build。
- 点权可为负，全负时答案 `0`（允许一个点不选）。
- 若原题是有向依赖边，先确认能否当作无向冲突边。


**改板时先认这几个量：**

- `in_cycle`：点是否在环上。
- `take`/`skip`：选/不选该点的 DP 值。
- `parent`：树边父亲。

状态：

- 剥掉度数不超过 `1` 的点，剩余点就是环。
- 对每个环点向外的树，`take[u]` 表示选 `u`，`skip[u]` 表示不选 `u`。有 `take[u] = w[u] + sum(skip[v])`，`skip[u] = sum(max(take[v], skip[v]))`。
- 环上首尾相邻，分别强制首点不选/选，在线性链 DP 中转移，取两种情况最大值。

- 这份函数处理的是**每个连通块**各有一个环，不要求整图连通；因此总边数等于总点数。
- 点权可以是负数，答案允许一个点也不选；不要把初值设成最小负数。
- 平行边形成的长度 `2` 环也能处理；自环会改变独立集定义，模板明确不支持。
- 若原题是有向函数图，先确认冲突关系是否应看成无向边；若是“沿有向边选点”的约束，转移含义可能不同。

```cpp
// 每个连通块恰有一个环的无向图上，求最大点权独立集。
// 支持多个连通块与长度为 2 的平行边环；不支持自环。
i64 maximum_weight_independent_set_unicyclic_forest(
    const vector<i64>& weight, const vector<pair<int, int>>& edges
) {
    int n = (int)weight.size();
    if (n == 0) return 0;
    assert((int)edges.size() == n); // 每个连通块恰有一个环时，边数等于点数。

    vector<vector<pair<int, int>>> graph(n);
    vector<int> degree(n);
    for (int id = 0; id < n; ++id) {
        auto [u, v] = edges[id];
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        graph[u].push_back({v, id});
        graph[v].push_back({u, id});
        ++degree[u];
        ++degree[v];
    }

    // 剥掉所有叶子，剩下的节点恰好是每个连通块的环。
    queue<int> que;
    vector<char> removed(n);
    for (int u = 0; u < n; ++u) if (degree[u] <= 1) que.push(u);
    while (!que.empty()) {
        int u = que.front();
        que.pop();
        if (removed[u]) continue;
        removed[u] = true;
        for (auto [v, id] : graph[u]) {
            if (removed[v]) continue;
            if (--degree[v] == 1) que.push(v);
        }
    }
    vector<char> in_cycle(n);
    for (int u = 0; u < n; ++u) in_cycle[u] = !removed[u];

    // 从所有环点向外建根，后序计算环外森林的 take / skip DP。
    vector<int> parent(n, -1), order;
    for (int root = 0; root < n; ++root) if (in_cycle[root]) {
        for (auto [v, id] : graph[root]) {
            if (!in_cycle[v]) {
                assert(parent[v] == -1);
                parent[v] = root;
                order.push_back(v);
            }
        }
    }
    for (int index = 0; index < (int)order.size(); ++index) {
        int u = order[index];
        for (auto [v, id] : graph[u]) {
            if (v == parent[u] || in_cycle[v]) continue;
            assert(parent[v] == -1);
            parent[v] = u;
            order.push_back(v);
        }
    }

    vector<i64> skip(n), take(n);
    auto calculate_tree_state = [&](int u) {
        take[u] = weight[u];
        skip[u] = 0;
        for (auto [v, id] : graph[u]) if (parent[v] == u) {
            take[u] += skip[v];
            skip[u] += max(skip[v], take[v]);
        }
    };
    for (int index = (int)order.size() - 1; index >= 0; --index) {
        calculate_tree_state(order[index]);
    }
    for (int u = 0; u < n; ++u) if (in_cycle[u]) calculate_tree_state(u);

    // 把每个环拆为链：首点不选 / 首点选两种情况，避免首尾同时选。
    const i64 NEG = -(1LL << 60);
    vector<char> cycle_seen(n);
    i64 answer = 0;
    for (int start = 0; start < n; ++start) {
        if (!in_cycle[start] || cycle_seen[start]) continue;
        vector<int> cycle;
        int u = start, previous_edge = -1;
        do {
            cycle.push_back(u);
            cycle_seen[u] = true;
            int next = -1, next_edge = -1;
            for (auto [v, edge_id] : graph[u]) {
                if (in_cycle[v] && edge_id != previous_edge) {
                    next = v;
                    next_edge = edge_id;
                    break;
                }
            }
            assert(next != -1);
            previous_edge = next_edge;
            u = next;
        } while (u != start);

        auto solve_chain = [&](bool first_is_taken) {
            i64 previous_skip = first_is_taken ? NEG : skip[cycle[0]];
            i64 previous_take = first_is_taken ? take[cycle[0]] : NEG;
            for (int i = 1; i < (int)cycle.size(); ++i) {
                int v = cycle[i];
                i64 current_skip = max(previous_skip, previous_take) + skip[v];
                i64 current_take = previous_skip + take[v];
                previous_skip = current_skip;
                previous_take = current_take;
            }
            return first_is_taken ? previous_skip : max(previous_skip, previous_take);
        };
        answer += max(solve_chain(false), solve_chain(true));
    }
    return answer;
}
```
#### 典题：骑士互斥 / 环外树挂在一个环上

> **赛时先看**
> - **题目信号**：每个点有收益、冲突双方不能同时选；关系图每个连通块恰有一个环，或题面给出一个环和若干向外树枝。
> - **本质**：把“角色互斥、上司和下属不能同时选、骑士互相冲突”等题面，映射成基环树上的最大点权独立集。
> - **复杂度判定**：调用上节函数后为 `O(n)` 时间、`O(n)` 空间。
> - **维护的量**：`weight`（点权，下标 `0..n-1`）；`edges`（无向冲突边，长度 `n`）；返回值 `i64`。
> - **警告**：必须先确认关系是无向“互斥边”；有向依赖边不能直接当作无向冲突。若输入有多个连通块，所有连通块的答案都要累加。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个角色各有收益，`n` 条无向互斥边（每个连通块恰有一个环），求互不冲突的最大总收益。

依赖：基环树最大点权独立集 节函数 maximum_weight_independent_set_unicyclic_forest，抄板时一起抄上。

```cpp
int n = 3;                                   // 样例输入，抄题时换成你的输入
vector<i64> weight(n);
for (int i = 0; i < n; ++i) cin >> weight[i];   // weight[i] = 角色 i 的收益
vector<pair<int, int>> edges(n);
for (int i = 0; i < n; ++i) {
    int u, v; cin >> u >> v;
    edges[i] = {u - 1, v - 1};                  // 1-based 读入，存成 0-based
}
i64 ans = maximum_weight_independent_set_unicyclic_forest(weight, edges);
cout << ans << '\n';                            // 最大总收益
```

样例：`3` 点环，收益 `{5, 2, 8}` -> `ans = 8`。

**传参要求（照这个传不会错）：**

- `weight`：`vector<i64>` 点权，下标 `0..n-1`；点权可为负，全负时答案 `0`（可以一个都不选）。
- `edges`：`vector<pair<int,int>>` 无向冲突边，长度必须等于 `n`（边数等于点数）；不支持自环，平行边（长度 2 的环）支持。
- 返回值：`i64` 最大点权独立集权值和；多连通块自动累加。


模型：每个角色有收益，若两人存在冲突边则不能同时选。每个连通块恰有一个环，求最大总收益。

输入改写：点权放入 `weight[0..n-1]`，每条无向冲突边放入 `edges`；调用 `maximum_weight_independent_set_unicyclic_forest(weight, edges)`。若题目从 `1` 开始编号，读入后减一。

手算检查：

- 纯 `3` 环的答案是三个点权中最大的一个，不能误选两个端点。
- 环点挂一棵树时，先把该树的 `take/skip` 合到环点，不能把环外点再拿去做一遍环 DP。
- 点权全负时答案应为 `0`，因为可以不选任何点。

经典练习：洛谷 P2607《骑士》。它是“环 + 挂树”模型的代表；做题时先确认题目给出的关系转成无向冲突边后是否允许平行边，再按题面处理。

#### 换根 DP：所有点到其他点距离和

> **赛时先看**
> - **题目信号**：对树上每个点求全局值；从父亲换到儿子可快速更新。
> - **本质**：每个点作为根都要答案。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`sz[u]`（子树大小）；`dp[u]`（以 u 为根时子树内各点到 u 的距离和）；`ans[u]`（以 u 为根的全局距离和，即最终答案）。
> - **警告**：换根前保存现场，递归回来恢复。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点树，对每个点 `u` 求所有点到 `u` 的距离和。

```cpp
int n = 3;                                   // 样例输入，抄题时换成你的输入
RerootDistanceSum sol;
sol.init(n);                        // 1. 初始化，点编号 1..n
for (int i = 1; i < n; ++i) {
    int u, v; cin >> u >> v;
    sol.add_edge(u, v);             // 2. 加无向边
}
vector<i64> ans = sol.solve();      // 3. ans[u] = 以 u 为根的距离和
```

样例：链 `1-2-3` -> `ans = {3, 2, 3}`。

**传参要求（照这个传不会错）：**

- 下标：点编号 1..n，`init(n_)` 自动开 `n+1` 的数组。
- `init(int n_)`：先调用；`n_` 为点数，重复调用即清空重来。
- `add_edge(int u, int v)`：每条无向边调一次，内部自动补双向。
- `solve()`：加完全部边后调用，无需参数；返回 `vector<i64>`，`ans[u]` 直接输出。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
- `solve()` -> 执行主算法并返回答案
**改板时先认这几个量：**

- `g`：邻接表。
- `sz`：集合/子树大小。

```cpp
struct RerootDistanceSum {
    int n;
    vector<vector<int>> g;
    vector<int> sz;
    vector<i64> dp, ans;

    RerootDistanceSum(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        sz.assign(n + 1, 0);
        dp.assign(n + 1, 0);
        ans.assign(n + 1, 0);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs1(int u, int p) {
        sz[u] = 1;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs1(v, u);
            sz[u] += sz[v];
            dp[u] += dp[v] + sz[v];
        }
    }

    void dfs2(int u, int p) {
        ans[u] = dp[u];
        for (int v : g[u]) {
            if (v == p) continue;
            i64 du = dp[u], dv = dp[v];
            int su = sz[u], sv = sz[v];

            dp[u] -= dp[v] + sz[v];
            sz[u] -= sz[v];
            dp[v] += dp[u] + sz[u];
            sz[v] += sz[u];

            dfs2(v, u);

            dp[u] = du; dp[v] = dv;
            sz[u] = su; sz[v] = sv;
        }
    }

    vector<i64> solve() {
        dfs1(1, 0);
        dfs2(1, 0);
        return ans;
    }
};
```
#### DSU on Tree

> **赛时先看**
> - **题目信号**：每个节点都问“它的子树里……”；需要保留重儿子信息。
> - **本质**：统计每个子树内颜色出现次数、众数、满足条件数量。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n log n)` 或均摊 `O(n)` 级别。
> - **维护的量**：`color[u]`（点 u 的颜色，手动赋值）；`cnt[c]`（当前桶内颜色 c 的出现次数）；`best`（当前众数出现次数）；`ans[u]`（u 子树内众数出现次数）。
> - **警告**：轻儿子算完清空，重儿子保留；`add_subtree` 要跳过重儿子。
> - **读答案**：答案在公开成员 `ans[u]` 中。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点树，每个点有颜色，对每个点求其子树内出现次数最多的颜色出现了几次。

```cpp
DsuOnTree d(5, 10);                 // 1. 初始化：点数、颜色值上界
d.add_edge(1, 2); d.add_edge(2, 3);
d.add_edge(3, 4); d.add_edge(3, 5); // 2. 加无向边
d.color[1] = 1; d.color[2] = 2; d.color[3] = 2;
d.color[4] = 2; d.color[5] = 3;     // 3. 赋颜色（范围 1..max_color）
d.dfs_size(1, 0);                   // 4. 先算子树大小/重儿子
d.dfs(1, 0, true);                  // 5. 跑 DSU on Tree，答案进 d.ans
```

样例：上例 -> `d.ans[1]=3`（颜色 2 全树出现 3 次），`d.ans[3]=2`。

**传参要求（照这个传不会错）：**

- 下标：点编号 1..n；颜色值 1..max_color。
- `init(int n_, int max_color)`：先调用；`max_color` 是颜色最大值（`cnt` 开到 `max_color+1`）。
- `add_edge(int u, int v)`：每条无向边调一次。
- `color[u]`：建完图后逐个赋值颜色，必须在 `dfs` 之前。
- `dfs_size(1, 0)` 与 `dfs(1, 0, true)`：两步都要从根调用一次；返回值为 void，结果读公开成员 `ans[u]`。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_, int max_color)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `sz`：集合/子树大小。

```cpp
struct DsuOnTree {
    int n;
    vector<vector<int>> g;
    vector<int> color, sz, heavy, ans;
    vector<int> cnt;
    int best = 0;

    DsuOnTree(int n = 0, int max_color = 0) { init(n, max_color); }

    void init(int n_, int max_color) {
        n = n_;
        g.assign(n + 1, {});
        color.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        heavy.assign(n + 1, 0);
        ans.assign(n + 1, 0);
        cnt.assign(max_color + 1, 0);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs_size(int u, int p) {
        sz[u] = 1;
        heavy[u] = 0;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs_size(v, u);
            sz[u] += sz[v];
            if (!heavy[u] || sz[v] > sz[heavy[u]]) heavy[u] = v;
        }
    }

    void add_node(int u) {
        best = max(best, ++cnt[color[u]]);
    }

    void add_subtree(int u, int p, int banned) {
        add_node(u);
        for (int v : g[u]) {
            if (v != p && v != banned) add_subtree(v, u, banned);
        }
    }

    void clear_subtree(int u, int p) {
        cnt[color[u]]--;
        for (int v : g[u]) {
            if (v != p) clear_subtree(v, u);
        }
    }

    void dfs(int u, int p, bool keep) {
        for (int v : g[u]) {
            if (v != p && v != heavy[u]) dfs(v, u, false);
        }
        if (heavy[u]) dfs(heavy[u], u, true);
        add_subtree(u, p, heavy[u]);
        ans[u] = best;
        if (!keep) {
            clear_subtree(u, p);
            best = 0;
        }
    }
};
```
#### 树的重心

> **赛时先看**
> - **题目信号**：需要把树尽量均匀地分开；树上分治。
> - **本质**：找删除后最大连通块最小的点，点分治前置。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`sz[u]`（子树大小）；`mx`（删 u 后最大连通块大小，dfs 内局部变量）；`centroids`（重心集合，1 或 2 个）；`best`（当前最小的最大块）。
> - **警告**：树可能有两个重心。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点树，求所有重心。

```cpp
TreeCentroid tc;
tc.init(4);                         // 1. 初始化，点编号 1..n
tc.add_edge(1, 2);                  // 2. 加无向边
tc.add_edge(2, 3);
tc.add_edge(2, 4);
vector<int> c = tc.solve();         // 3. 重心集合
```

样例：`1-2, 2-3, 2-4`（星形）-> `c = {2}`；链 `1-2-3-4` 有两个重心 `c = {2, 3}`。

**传参要求（照这个传不会错）：**

- 下标：点编号 1..n，DFS 从 1 出发（树须连通）。
- `init(int n_)`：先调用，清空结构。
- `add_edge(int u, int v)`：每条无向边调一次。
- `solve()`：加完全部边后调用；返回 `vector<int>`，含 1 或 2 个重心编号。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
- `solve()` -> 执行主算法并返回答案
**改板时先认这几个量：**

- `g`：邻接表。
- `sz`：集合/子树大小。
- `mx`：区间最大值。

```cpp
struct TreeCentroid {
    int n;
    vector<vector<int>> g;
    vector<int> sz, centroids;
    int best;

    TreeCentroid(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        sz.assign(n + 1, 0);
        centroids.clear();
        best = n + 1;
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        sz[u] = 1;
        int mx = 0;
        for (int v : g[u]) {
            if (v == p) continue;
            dfs(v, u);
            sz[u] += sz[v];
            mx = max(mx, sz[v]);
        }
        mx = max(mx, n - sz[u]);
        if (mx < best) {
            best = mx;
            centroids = {u};
        } else if (mx == best) {
            centroids.push_back(u);
        }
    }

    vector<int> solve() {
        dfs(1, 0);
        return centroids;
    }
};
```
#### 点分治骨架

> **赛时先看**
> - **题目信号**：树上路径统计，直接从每个点 DFS 会超时；路径跨过某个分治重心。
> - **本质**：统计树上路径类问题，如距离不超过 `K` 的点对数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：常见 `O(n log n)`。
> - **维护的量**：`sz[u]`（当前连通块的子树大小）；`dead[u]`（该点已被当作重心删除的标记）；`g` 为带权邻接表，存 `（终点， 边权）`。
> - **警告**：每层分治要标记删除重心；统计时先减去同一子树内部贡献，再加全局贡献。


**最小完整示例（先抄这一段就能跑）：**

题目：统计距离不超过 `K` 的点对数（统计逻辑写在 `solve_at_centroid`）。

```cpp
CentroidDecomposition cd;
cd.init(5);                         // 1. 初始化，点编号 1..n
cd.add_edge(1, 2, 1);               // 2. 加无向带权边 (u, v, w)
cd.add_edge(2, 3, 1);
cd.add_edge(3, 4, 1);
cd.add_edge(3, 5, 1);
cd.decompose(1);                    // 3. 分治入口，答案在 solve_at_centroid 里统计
```

样例：链 `1-2-3`：点对距离 `{(1,2):1, (2,3):1, (1,3):2}`，在 `solve_at_centroid(c)` 中收集距离后计数。

**传参要求（照这个传不会错）：**

- 下标：点编号 1..n。
- `init(int n_)`：先调用。
- `add_edge(int u, int v, int w = 1)`：无向带权边；不传 `w` 时默认边权 1。
- `decompose(int entry)`：加完边后从任意点（一般 1）调用启动分治；每层自动把重心 `c` 传进 `solve_at_centroid(c)`，统计逻辑写在这里。
- 返回值：无；结果在你写的统计逻辑里记录。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, int w = 1)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `sz`：集合/子树大小。

```cpp
struct CentroidDecomposition {
    int n;
    vector<vector<pair<int, int>>> g;
    vector<int> sz, dead;

    CentroidDecomposition(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        sz.assign(n + 1, 0);
        dead.assign(n + 1, 0);
    }

    void add_edge(int u, int v, int w = 1) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    int calc_size(int u, int p) {
        sz[u] = 1;
        for (auto [v, w] : g[u]) {
            if (v != p && !dead[v]) sz[u] += calc_size(v, u);
        }
        return sz[u];
    }

    int find_centroid(int u, int p, int total) {
        for (auto [v, w] : g[u]) {
            if (v != p && !dead[v] && sz[v] * 2 > total) return find_centroid(v, u, total);
        }
        return u;
    }

    void collect_dist(int u, int p, int d, vector<int>& ds) {
        ds.push_back(d);
        for (auto [v, w] : g[u]) {
            if (v != p && !dead[v]) collect_dist(v, u, d + w, ds);
        }
    }

    void solve_at_centroid(int c) {
        // 在这里写“经过 c 的路径”的统计逻辑。
        // 常见做法：对每个子树 collect_dist，先查询全局桶，再把该子树距离加入桶。
    }

    void decompose(int entry) {
        int total = calc_size(entry, 0);
        int c = find_centroid(entry, 0, total);
        dead[c] = 1;
        solve_at_centroid(c);
        for (auto [v, w] : g[c]) {
            if (!dead[v]) decompose(v);
        }
    }
};
```
#### 树上路径计数：点分治统计恰好 K 条边

> **赛时先看**
> - **题目信号**：树上所有点对/路径计数，朴素枚举两端点是 `O(n^2)`；条件只与路径长度或长度可聚合属性有关；题目给出一个 `K`，问有多少对距离刚好为 `K`。
> - **本质**：统计无权树上距离（边数）恰好为 `K` 的无序点对数。可作为“路径长度不超过 K”“路径权值等于 K”的点分治入门母版。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n log n)` 时间、`O(n)` 空间。下面是 0-indexed 无权树版本，组件扫描迭代完成，只有深度 `O(log n)` 的分治递归。
> - **维护的量**：`subtree_size[u]`（组件子树大小）；`frequency[d]`（当前分治层已处理子树中深度 d 的节点数桶）；`blocked[u]`（重心删除标记）；`answer`（累计点对数）。
> - **警告**：`K = 0` 时答案为 0（每对点距离为正，不把"同一个点"计入）；图必须 0-indexed；点对计数上限是 `n*(n-1)/2`，用 `i64`。
> - **约定**：0-indexed 无权树：统计距离（边数）恰为 K 的无序点对数量


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `0-indexed` 无权树，统计距离（边数）恰为 `K` 的无序点对数。

```cpp
CentroidExactLengthPathCounter counter(4);  // 1. 0-indexed，4 个点
counter.add_edge(0, 1);                     // 2. 加无向边
counter.add_edge(1, 2);
counter.add_edge(1, 3);
i64 ans = counter.count_paths_exactly(2);   // 3. 距离恰为 2 的点对数
```

样例：星形（中心 1，叶子 0/2/3）-> `ans = 3`（三个叶子两两配对）。

**传参要求（照这个传不会错）：**

- 下标：0-indexed，点 0..n-1；`graph` 开 `n` 个。
- `init(int n_)`（或构造时直接传）：先调用；重复调用即清空。
- `add_edge(int u, int v)`：无向边，要求 `0 <= u,v < n` 且 `u != v`。
- `count_paths_exactly(int k)`：加完边后调用一次；`k <= 0` 或 `k >= n` 直接返回 0；返回 `i64` 点对数（`K=0` 返回 0，端点相同的对不计）。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `graph`：邻接表。
- `subtree_size`：子树大小。
- `frequency`：每个深度的出现次数桶。
- `blocked`：分治中被删除的重心标记。

核心：对每层分治重心 `c`，维护已处理子树中每个深度出现次数。遍历一棵新子树的深度 `d` 时，累加已经出现的 `K-d`，这样只统计“路径经过 c 且两端来自不同子树”的点对。子树内部点对留给递归层处理。

- `frequency[0]=1` 表示重心本身，因此自动统计“一个端点是重心”的路径；不要额外加一次。
- 一定要先查询当前子树、再把当前子树深度加入桶，否则会把同一子树内部路径提前计入，之后重复计算。
- 本块统计的是不同端点的无序对，`K=0` 返回 `0`。若题目允许端点相同，需要单独加 `n`。
- 带权边不能直接以深度做数组下标；小权值可改桶，大权值常用“收集距离 + 排序双指针”统计不超过某阈值。

```cpp
// 0-indexed 无权树：统计距离（边数）恰为 K 的无序点对数量。
// 仅分治过程递归，深度 O(log n)；组件遍历和距离收集均为迭代。
struct CentroidExactLengthPathCounter {
    int n = 0, target = 0;
    vector<vector<int>> graph;
    vector<int> parent, subtree_size, frequency;
    vector<char> blocked;
    i64 answer = 0;

    explicit CentroidExactLengthPathCounter(int n_ = 0) { init(n_); }

    void init(int n_) {
        n = n_;
        graph.assign(n, {});
        parent.assign(n, -1);
        subtree_size.assign(n, 0);
        blocked.assign(n, false);
    }

    void add_edge(int u, int v) {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    i64 count_paths_exactly(int k) {
        if (n == 0 || k <= 0 || k >= n) return 0;
        target = k;
        answer = 0;
        fill(blocked.begin(), blocked.end(), false);
        frequency.assign(target + 1, 0);
        decompose(0);
        return answer;
    }

private:
    int find_centroid(int entry) {
        vector<int> order = {entry};
        parent[entry] = -1;
        for (int index = 0; index < (int)order.size(); ++index) {
            int u = order[index];
            for (int v : graph[u]) {
                if (blocked[v] || v == parent[u]) continue;
                parent[v] = u;
                order.push_back(v);
            }
        }

        for (int index = (int)order.size() - 1; index >= 0; --index) {
            int u = order[index];
            subtree_size[u] = 1;
            for (int v : graph[u]) {
                if (!blocked[v] && parent[v] == u) subtree_size[u] += subtree_size[v];
            }
        }

        int total = (int)order.size();
        int centroid = entry, largest_part = total;
        for (int u : order) {
            int largest = total - subtree_size[u];
            for (int v : graph[u]) {
                if (!blocked[v] && parent[v] == u) largest = max(largest, subtree_size[v]);
            }
            if (largest < largest_part) {
                largest_part = largest;
                centroid = u;
            }
        }
        return centroid;
    }

    vector<int> collect_depths(int start, int par) const {
        struct Node { int u, parent, depth; };
        vector<Node> stack = {{start, par, 1}};
        vector<int> depths;
        while (!stack.empty()) {
            Node current = stack.back();
            stack.pop_back();
            if (current.depth > target) continue;
            depths.push_back(current.depth);
            for (int v : graph[current.u]) {
                if (!blocked[v] && v != current.parent) {
                    stack.push_back({v, current.u, current.depth + 1});
                }
            }
        }
        return depths;
    }

    void decompose(int entry) {
        int centroid = find_centroid(entry);
        blocked[centroid] = true;

        vector<int> touched = {0};
        frequency[0] = 1; // 重心本身。
        for (int v : graph[centroid]) {
            if (blocked[v]) continue;
            vector<int> depths = collect_depths(v, centroid);
            for (int depth : depths) answer += frequency[target - depth];
            for (int depth : depths) {
                if (frequency[depth]++ == 0) touched.push_back(depth);
            }
        }
        for (int depth : touched) frequency[depth] = 0;

        for (int v : graph[centroid]) if (!blocked[v]) decompose(v);
    }
};
```

典题：CSES Fixed-Length Paths I；将树读成 0-indexed 后，输出 `counter.count_paths_exactly(K)`。CSES Fixed-Length Paths II 把条件改为路径长度在 `[K1,K2]`，需要把本块的单深度桶换成前缀和/Fenwick 查询，分治“跨子树统计”的结构不变。
#### 动态点分治：点开关，查询最近标记点

> **赛时先看**
> - **题目信号**：树不变；操作是点颜色翻转、点加入/删除集合；查询离某点最近的特殊点。经典模型类似 QTREE5。
> - **本质**：树上动态维护一批“被标记/变黑”的点，支持单点开关，查询某个点到最近标记点的距离。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：建树 `O(n log n)`，每次修改/查询 `O(log^2 n)`，如果用堆加懒删除可做到 `O(log n log n)` 量级。
> - **维护的量**：`up[u]`（点 u 到每个点分祖先重心的 `（重心， 距离）` 列表）；`bag[c]`（重心 c 处所有已激活点到 c 的距离 multiset）；`active[u]`（u 当前是否点亮）。
> - **警告**：每个点要记录它到所有点分树祖先重心的距离；删除时用 `multiset.find` 删除一个距离；没有标记点时返回 `-1`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点树，`q` 次操作：开关点 / 查询某点到最近亮点距离。

```cpp
DynamicCentroidDecomposition dc;
dc.init(5);                         // 1. 初始化，点编号 1..n
dc.add_edge(1, 2);                  // 2. 加无向边（可带权）
dc.add_edge(2, 3);
dc.add_edge(3, 4); dc.add_edge(3, 5);
dc.build(1);                        // 3. 先建点分树，之后才能开关/查询
dc.toggle(2);                       // 4. 点亮 2
i64 d = dc.query_nearest(4);        // 5. 4 到最近亮点的距离
```

样例：上述 5 点树，点亮 2 后 -> `query_nearest(4) = 2`；无亮点时返回 `-1`。

**传参要求（照这个传不会错）：**

- 下标：点编号 1..n。
- 顺序：`init(n_)` -> `add_edge(u, v, w = 1)` -> `build(root = 1)`；**必须先 build** 再开关/查询。
- `toggle(int u)` / `turn_on(int u)` / `turn_off(int u)`：开关点，重复调用安全。
- `query_nearest(int u)`：返回 `i64` 最近亮点距离；无亮点返回 `-1`。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, i64 w = 1)` -> 加入一条边
- `build(int root = 1)` -> 完成建树或预处理
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `up`：up[u] 存所有祖先点分中心及 u 到它的距离。
- `bag`：bag[c] 存所有已激活点到点分中心 c 的距离。

```cpp
struct DynamicCentroidDecomposition {
    int n;
    vector<vector<pair<int, i64>>> g;
    vector<int> sz, dead, active;
    vector<vector<pair<int, i64>>> up; // up[u] 存所有祖先点分中心及 u 到它的距离。
    vector<multiset<i64>> bag;         // bag[c] 存所有已激活点到点分中心 c 的距离。

    DynamicCentroidDecomposition(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        sz.assign(n + 1, 0);
        dead.assign(n + 1, 0);
        active.assign(n + 1, 0);
        up.assign(n + 1, {});
        bag.assign(n + 1, {});
    }

    void add_edge(int u, int v, i64 w = 1) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    int calc_size(int u, int p) {
        sz[u] = 1;
        for (auto [v, w] : g[u]) {
            if (v != p && !dead[v]) sz[u] += calc_size(v, u);
        }
        return sz[u];
    }

    int find_centroid(int u, int p, int total) {
        for (auto [v, w] : g[u]) {
            if (v != p && !dead[v] && sz[v] * 2 > total) {
                return find_centroid(v, u, total);
            }
        }
        return u;
    }

    void collect(int u, int p, i64 d, int c) {
        up[u].push_back({c, d});
        for (auto [v, w] : g[u]) {
            if (v != p && !dead[v]) collect(v, u, d + w, c);
        }
    }

    void decompose(int entry) {
        int total = calc_size(entry, 0);
        int c = find_centroid(entry, 0, total);
        dead[c] = 1;
        collect(c, 0, 0, c);
        for (auto [v, w] : g[c]) {
            if (!dead[v]) decompose(v);
        }
    }

    void build(int root = 1) {
        decompose(root);
    }

    void turn_on(int u) {
        if (active[u]) return;
        active[u] = 1;
        for (auto [c, d] : up[u]) bag[c].insert(d);
    }

    void turn_off(int u) {
        if (!active[u]) return;
        active[u] = 0;
        for (auto [c, d] : up[u]) {
            auto it = bag[c].find(d);
            if (it != bag[c].end()) bag[c].erase(it);
        }
    }

    void toggle(int u) {
        if (active[u]) turn_off(u);
        else turn_on(u);
    }

    i64 query_nearest(int u) const {
        const i64 INF = (1LL << 60);
        i64 ans = INF;
        for (auto [c, d] : up[u]) {
            if (!bag[c].empty()) ans = min(ans, d + *bag[c].begin());
        }
        return ans == INF ? -1 : ans;
    }
};
```
#### 虚树 Virtual Tree

> **赛时先看**
> - **题目信号**：多次询问给出若干关键点，要在树上按路径关系 DP。
> - **本质**：给一小批关键点，在原树上构造只包含关键点和 LCA 的压缩树。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(k log k)`。
> - **维护的量**：`tin[u]`、`depth[u]`（自备 DFS 序与深度，需先赋值）；`lca(a,b)`（自备 LCA 回调）；`build` 返回的 `vt`（虚树邻接表，节点按排序去重后的 `nodes` 顺序编号）。
> - **警告**：关键点按 DFS 序排序；相邻点的 LCA 加入集合；用栈连边。
> - **依赖**：需自备倍增/HLD 的 `tin`、`depth`、`lca`；本模板的 `LCA` 结构没有成员 `tin`，可改用 HLD 的 `dfn` 或自行记录。


**最小完整示例（先抄这一段就能跑）：**

题目：给 `k` 个关键点，建出只含关键点与两两 LCA 的虚树。

依赖：LCA（倍增）节 struct，抄板时一起抄上。

```cpp
LCA solver(5);                               // 样例输入，抄题时换成你的输入（链 1-2-3-4-5）
for (int i = 1; i < 5; ++i) solver.add_edge(i, i + 1);
solver.build(1);
vector<int> tin = {0, 1, 2, 3, 4, 5};        // 样例输入，抄题时换成你的输入（DFS 序）
vector<int> depth = {0, 0, 1, 2, 3, 4};      // 样例输入，抄题时换成你的输入（深度）
VirtualTree vt;
vt.tin = tin; vt.depth = depth;                         // 1. 来自自备 LCA/HLD
vt.lca = [&](int a, int b) { return solver.lca(a, b); };// 2. 绑定 LCA 回调
vector<vector<int>> vt_edges = vt.build({2, 4, 5});     // 3. 传关键点列表
```

样例：链 `1-2-3-4-5`（tin 即编号）-> `build({2,4,5})` 得节点 `{2,4,5}`，边 `2-4`、`4-5`。

**传参要求（照这个传不会错）：**

- 下标：与自备 LCA 的点编号一致（0 或 1 开头均可）。
- 先准备：赋值 `tin`、`depth` 两个成员，并把 `lca` 绑定成 `function<int(int,int)>`；本模板不含 LCA 实现。
- `build(vector<int> nodes)`：传关键点编号，可乱序、可重复；返回 `vt`，其中下标 i 对应 `nodes`（排序去重后）第 i 个点，`vt` 里存的邻接也是虚树内下标。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(vector<int> nodes)` -> 完成建树或预处理 返回 `vector<vector<int>>`。

```cpp
struct VirtualTree {
    vector<int> tin, depth;
    function<int(int,int)> lca;

    bool by_dfn(int a, int b) const { return tin[a] < tin[b]; }

    vector<vector<int>> build(vector<int> nodes) {
        sort(nodes.begin(), nodes.end(), [&](int a, int b) { return tin[a] < tin[b]; });
        int k = (int)nodes.size();
        for (int i = 0; i + 1 < k; ++i) nodes.push_back(lca(nodes[i], nodes[i + 1]));
        sort(nodes.begin(), nodes.end(), [&](int a, int b) { return tin[a] < tin[b]; });
        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

        vector<vector<int>> vt(nodes.size());
        unordered_map<int, int> id;
        for (int i = 0; i < (int)nodes.size(); ++i) id[nodes[i]] = i;

        vector<int> st;
        for (int u : nodes) {
            if (st.empty()) {
                st.push_back(u);
                continue;
            }
            while (!st.empty() && lca(st.back(), u) != st.back()) st.pop_back();
            vt[id[st.back()]].push_back(id[u]);
            st.push_back(u);
        }
        return vt;
    }
};
```
#### 虚树例题：关键点两两路径总长度

> **赛时先看**
> - **题目信号**：每次询问给 `k` 个点，`sum k` 较大但不能每对处理。
> - **本质**：给一批关键点，求它们在树上诱导路径的总边长。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(k log k)`。
> - **维护的量**：无结构体；只维护局部 `sum`（关键点按 DFS 序排成环后相邻点距离和，除以 2 即答案）。
> - **警告**：把关键点按 DFS 序排成环，答案是相邻距离和的一半。
> - **依赖**：需自备倍增/HLD 的 `tin`、`depth`、`lca`；本模板的 `LCA` 结构没有成员 `tin`，可改用 HLD 的 `dfn` 或自行记录。


**最小完整示例（先抄这一段就能跑）：**

题目：每次给 `k` 个关键点，求覆盖它们的最小 Steiner 树边长（关键点两两路径并的总长）。

依赖：自备 LCA 结构（需 build(n, edges)、lca、distance 与 DFS 序数组 dfn），抄板时一起抄上。

```cpp
int n = 5;                                   // 样例输入，抄题时换成你的输入（链 1-2-3-4-5）
vector<pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 4}, {4, 5}};  // 样例输入，抄题时换成你的输入
vector<int> dfn = {0, 1, 2, 3, 4, 5};        // 样例输入，抄题时换成你的输入（DFS 序数组）
LCA solver;                            // 自备 LCA，需有 distance(a,b)
solver.build(n, edges);                // 预处理
vector<int> tin = dfn;                 // DFS 序数组
i64 len = marked_steiner_tree_length({2, 4, 5}, solver, tin);
```

样例：链 `1-2-3-4-5` 选 `{2,4,5}` -> `len = 3`（路径并 `2-3-4` 与 `4-5`）。

**传参要求（照这个传不会错）：**

- `nodes`：关键点编号列表，可乱序、可重复（内部先排序去重）。
- `solver`：自备 LCA 结构，必须实现 `distance(a, b)`（返回树上两点距离）。
- `tin`：DFS 序数组，`tin[u]` 与 `solver` 的点编号一致。
- 返回值：`i64` 最小 Steiner 树总边长；`k = 1` 或空列表时返回 0。


```cpp
i64 marked_steiner_tree_length(vector<int> nodes, const LCA& solver, const vector<int>& tin) {
    sort(nodes.begin(), nodes.end(), [&](int a, int b) { return tin[a] < tin[b]; });
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
    i64 sum = 0;
    int k = (int)nodes.size();
    for (int i = 0; i < k; ++i) {
        int a = nodes[i], b = nodes[(i + 1) % k];
        sum += solver.distance(a, b);
    }
    return sum / 2;
}
```
#### 函数图倍增

> **赛时先看**
> - **题目信号**：状态转移固定 `next[x]`，问走很多步后到哪里。
> - **本质**：每个点出度为 1，快速跳 `k` 步。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：预处理 `O(n log K)`，查询 `O(log K)`。
> - **维护的量**：`up[k][i]`（点 i 跳 `2^k` 步到达的点）；`n`（点数）；`LOG`（倍增层数）；`max_k`（步数上界）。
> - **警告**：`up[0][i] = f[i]`；查询的 `steps` 不能超过构造时传入的 `max_k`。


**最小完整示例（先抄这一段就能跑）：**

题目：每个点出度为 1（`nxt[i]` 是下一步），问从 `x` 走 `steps` 步到哪个点。

```cpp
vector<int> nxt = {0, 2, 3, 1};     // 1-based：nxt[1]=2, nxt[2]=3, nxt[3]=1
FunctionalGraphJump fg(nxt, 1e18);  // 2. 建表，步数上界 1e18
int to = fg.jump(1, 5);             // 3. 从 1 走 5 步
```

样例：环 `1->2->3->1` -> `jump(1, 5) = 3`。

**传参要求（照这个传不会错）：**

- `nxt`：长度 `n+1` 的 `vector<int>`，`nxt[i]` 为点 i 的下一步（点编号 1..n，`nxt[0]` 不参与）。
- `max_k`：查询步数最大值，只影响建表层数；**`jump` 的 `steps` 不能超过它**（否则 assert 失败）。
- `jump(int x, i64 steps)`：`x` 为起点（1..n），`steps` 为步数；返回 `int` 走完后的点编号。


**API / 入口函数（赛时只认这里列的名字）：**

- `jump(int x, i64 steps)` -> 向上跳若干级祖先 返回 `int`。
**改板时先认这几个量：**

- `up`：倍增祖先表。
- `nxt`：转移/子节点。

```cpp
struct FunctionalGraphJump {
    int n, LOG;
    i64 max_k;
    vector<vector<int>> up;

    FunctionalGraphJump(const vector<int>& nxt, i64 max_k) : max_k(max_k) {
        n = (int)nxt.size() - 1;
        LOG = 1;
        while ((1LL << LOG) <= max_k) LOG++;
        up.assign(LOG, vector<int>(n + 1));
        for (int i = 1; i <= n; ++i) up[0][i] = nxt[i];
        for (int k = 1; k < LOG; ++k) {
            for (int i = 1; i <= n; ++i) up[k][i] = up[k - 1][up[k - 1][i]];
        }
    }

    int jump(int x, i64 steps) const {
        assert(steps <= max_k); // steps 不能超过构造时的 max_k。
        for (int k = 0; steps; ++k, steps >>= 1) {
            if (steps & 1) x = up[k][x];
        }
        return x;
    }
};
```
#### 树哈希：无根树同构判定

> **赛时先看**
> - **题目信号**：节点没有固定编号含义；只关心树的形状；题目询问两棵树能否重标号后相同。
> - **本质**：快速判断两棵无根树是否同构，或对大量树形结构分组。先找树中心，再把中心作为根求子树哈希。
> - **接法**：给出很多棵树，统计互不同构的树形数量。
> - **复杂度判定**：找中心和计算哈希都为 `O(n log n)`，`log n` 来自对子哈希排序。
> - **维护的量**：无结构体；`TreeHash = pair<ui64,ui64>`（一棵子树/一棵无根树的哈希对），用 `==` 判同构。
> - **警告**：单哈希可能碰撞；这里用两组 `uint64_t`，对需要完全确定性的题可改成 AHU 括号规范表示或离散化子树类型。


**最小完整示例（先抄这一段就能跑）：**

题目：判断两棵无根树是否同构（可重标号）。

```cpp
int n = 4;                              // 样例输入，抄题时换成你的输入
vector<vector<int>> g1(n), g2(n);       // 0-indexed 无向邻接表
g1[0].push_back(1); g1[1].push_back(0); // 样例：4 点链 0-1-2-3
g1[1].push_back(2); g1[2].push_back(1);
g1[2].push_back(3); g1[3].push_back(2);
g2[0].push_back(1); g2[1].push_back(0); // 样例：4 点星形（中心 0）
g2[0].push_back(2); g2[2].push_back(0);
g2[0].push_back(3); g2[3].push_back(0);
TreeHash h1 = unrooted_tree_hash(g1);   // 1. 整棵树的哈希
TreeHash h2 = unrooted_tree_hash(g2);
bool same = (h1 == h2);                 // 2. 哈希相等即同构
```

样例：4 点链 `0-1-2-3` 与 4 点星形（中心 0）-> 哈希不同（不同构）。

**传参要求（照这个传不会错）：**

- 下标：0-indexed，`g[u]` 存 u 的邻居（无向，每边存两次）。
- `unrooted_tree_hash(g)`：直接传整棵树的邻接表，内部自动找树中心再求哈希；返回 `TreeHash`（两个 `ui64`）。
- `rooted_hash(u, p, g)` / `combine_hash(child)` / `splitmix64(x)`：内部辅助函数，一般无需外部直接调用。
- 返回值：两个 `TreeHash` 相等即同构；要分组统计时直接用 `TreeHash` 作 `map`/`set` 的 key。


典题模型：给出很多棵树，统计互不同构的树形数量。

```cpp
using TreeHash = pair<ui64, ui64>;

ui64 splitmix64(ui64 x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

TreeHash combine_hash(vector<TreeHash> child) {
    sort(child.begin(), child.end());
    ui64 a = 0x123456789abcdef0ULL;
    ui64 b = 0xfedcba9876543210ULL;
    for (auto [x, y] : child) {
        a = splitmix64(a ^ (x + 0x9e3779b97f4a7c15ULL));
        b = splitmix64(b ^ (y + 0xbf58476d1ce4e5b9ULL));
    }
    return {splitmix64(a + child.size()), splitmix64(b + child.size())};
}

TreeHash rooted_hash(int u, int p, const vector<vector<int>>& g) {
    vector<TreeHash> child;
    for (int v : g[u]) {
        if (v != p) child.push_back(rooted_hash(v, u, g));
    }
    return combine_hash(child);
}

vector<int> tree_centers(const vector<vector<int>>& g) {
    int n = (int)g.size();
    if (n == 1) return {0};
    vector<int> deg(n);
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        deg[i] = (int)g[i].size();
        if (deg[i] <= 1) q.push(i);
    }
    int remain = n;
    while (remain > 2) {
        int layer = (int)q.size();
        remain -= layer;
        while (layer--) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) if (--deg[v] == 1) q.push(v);
        }
    }
    vector<int> center;
    while (!q.empty()) {
        center.push_back(q.front());
        q.pop();
    }
    return center;
}

TreeHash unrooted_tree_hash(const vector<vector<int>>& g) {
    auto c = tree_centers(g);
    if (c.size() == 1) return rooted_hash(c[0], -1, g);
    return combine_hash({rooted_hash(c[0], c[1], g), rooted_hash(c[1], c[0], g)});
}
```

### 06 图论基础：遍历、最短路与生成树

先放邻接表、BFS、最短路、差分约束、生成树、Kruskal 重构树、树形图和生成树计数。
#### 图的邻接表

> **赛时先看**
> - **题目信号**：`n,m <= 1e5/1e6`，边比较稀疏。
> - **本质**：大多数图论题的存图方式。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：建图 `O(m)`。
> - **维护的量**：`g[u]`（点 u 的所有出边终点，无权图）；`wg[u]`（点 u 的 `（终点， 边权）` 出边，有权图）。
> - **警告**：无向图要加两次边；有权图用 `pair<int,i64>`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 边，读边建好 `g` / `wg` 后，所有图论模板直接拿来用。

```cpp
int n, m;
cin >> n >> m;
vector<vector<int>> g(n + 1);              // 1. 无权图邻接表
vector<vector<pair<int, i64>>> wg(n + 1);  // 2. 带权图邻接表 (终点, 边权)
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    g[u].push_back(v); wg[u].push_back({v, w});        // 3. 有向边
    g[v].push_back(u); wg[v].push_back({u, w});        //    无向图补反向边
}
```

样例：`n=3, m=2` 边 `1-2(5), 2-3(5)` -> `g[1]={2}`，`wg[1]={{2,5}}`。

**传参要求（照这个传不会错）：**

- 下标：点编号 1..n，数组开 `n+1`；题给 0..n-1 就把数组开 `n` 个、编号直接用。
- `g[u].push_back(v)`：无权有向边 `u -> v`；无向图再 `g[v].push_back(u)`。
- `wg[u].push_back({v, w})`：带权有向边 `u -> v`，权 `w` 用 `i64`；无向图再 `wg[v].push_back({u, w})`。
- 返回值：无；建好的 `g` / `wg` 直接传给 BFS / Dijkstra 等模板。


**API / 入口函数（赛时只认这里列的名字）：**

- `push_back(v)` -> 无权图。 返回 `g[u].`。

```cpp
int n, m;
vector<vector<int>> g(n + 1);
vector<vector<pair<int, i64>>> wg(n + 1);

// 无权图。
g[u].push_back(v);
g[v].push_back(u);

// 带权图。
wg[u].push_back({v, w});
wg[v].push_back({u, w});
```
#### BFS 最短步数

> **赛时先看**
> - **题目信号**：每条边代价相同；问最少走几步。
> - **本质**：无权图最短路、网格最短步数、多源扩散。
> - **接法**：如果题面问“最少走几步/最少操作次数”，并且每一步代价都一样，就把每个状态当点、每次合法操作当边。起点放进 `starts`；返回的 `dist[x]` 是最少步数，`-1` 表示到不了。网格题把 `(x,y)` 编号或直接开二维 `dist`。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`dist[v]`（起点到 v 的最少步数，`-1` 表示还没到过）；`starts`（全部起点，先入队并设距离 0）。
> - **警告**：多源 BFS 把所有起点先入队并设距离 0。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 条无权边，多起点，问每个点最少几步可达。

```cpp
int n, m;
cin >> n >> m;
vector<vector<int>> g(n + 1);              // 1. 建无权邻接表，点编号 1..n
for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);                     // 无向图补反向边
}
vector<int> starts = {1};                  // 2. 起点列表，多源就多塞几个
auto dist = bfs(n, g, starts);             // 3. 调用：dist[v] = 最少步数
for (int v = 1; v <= n; ++v) cout << dist[v] << " \n"[v == n];
```

样例：`1-2, 2-3, 1-3`，起点 `{1}` -> 输出 `0 1 1`。

**传参要求（照这个传不会错）：**

- `bfs(n, g, starts)`：`n` = 点数；`g` = 无权邻接表（1-indexed，与 `n` 配套）；`starts` = 起点编号列表（至少 1 个）。
- 返回值：`vector<int> dist`，`dist[v]` = 离最近起点的最少步数；`-1` = 不可达。
- 只适用于每条边代价都是 1；边带权换 Dijkstra / 0-1 BFS。


**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
vector<int> bfs(int n, const vector<vector<int>>& g, vector<int> starts) {
    vector<int> dist(n + 1, -1);
    queue<int> q;
    for (int s : starts) {
        dist[s] = 0;
        q.push(s);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (dist[v] != -1) continue;
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
    return dist;
}
```
#### 网格 BFS

> **赛时先看**
> - **题目信号**：`n*m` 网格，每步上下左右移动且代价相同。
> - **本质**：迷宫、网格最短步数、连通块。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(nm)`。
> - **维护的量**：`dist[x][y]`（最近起点到 `(x,y)` 的最少步数，`-1` 表示没到过）；`grid[x][y] == '#'` 视为墙不可走。
> - **警告**：多源 BFS 将所有起点先入队。


**最小完整示例（先抄这一段就能跑）：**

题目：`n*m` 网格（`'.'` 可走、`'#'` 墙），多起点，问终点最少几步可达。

```cpp
int n, m;
cin >> n >> m;
vector<string> grid(n);                    // 1. 读网格，下标 (行 0..n-1, 列 0..m-1)
for (auto& s : grid) cin >> s;
vector<pair<int, int>> starts = {{0, 0}};  // 2. 起点 (行, 列)，多源就多塞几个
auto dist = grid_bfs(grid, starts);        // 3. 调用：dist[x][y] = 最少步数
cout << dist[n - 1][m - 1] << '\n';        // 4. -1 = 到不了
```

样例：3x3 网格 `... / .#. / ...`，起点 `(0,0)` 终点 `(2,2)` -> 输出 `4`。

**传参要求（照这个传不会错）：**

- `grid_bfs(grid, starts)`：`grid` = 行字符串数组（0-indexed）；`starts` = 起点坐标 `（行， 列）` 列表。
- 返回值：`vector<vector<int>> dist`，`dist[x][y]` = 最近起点到 `(x,y)` 的最少步数；`-1` = 墙或不可达。
- 只认 `'#'` 为墙，其余字符一律可走；障碍字符不同时改 `grid[nx][ny] == '#'` 这一处。


```cpp
vector<vector<int>> grid_bfs(const vector<string>& grid, vector<pair<int, int>> starts) {
    int n = (int)grid.size(), m = (int)grid[0].size();
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<pair<int, int>> q;
    for (auto [x, y] : starts) {
        dist[x][y] = 0;
        q.push({x, y});
    }
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            if (grid[nx][ny] == '#') continue;
            if (dist[nx][ny] != -1) continue;
            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx, ny});
        }
    }
    return dist;
}
```
#### 0-1 BFS

> **赛时先看**
> - **题目信号**：代价只有两种，通常是“不改变方向代价 0，改变方向代价 1”。
> - **本质**：边权只有 0 和 1 的最短路。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`dist[v]`（源点 s 到 v 的最短路）；双端队列 `dq`（0 权边从前面推、1 权边从后面推，保证出队距离单调）。
> - **警告**：权 0 边推到 deque 前面，权 1 边推到后面。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 边，边权只有 0 或 1，求 `s` 到各点最短路。

```cpp
vector<vector<pair<int, int>>> g(n + 1);   // 1. 建图 (终点, 边权)，边权只能 0/1，点编号 1..n
for (int i = 0; i < m; ++i) {
    int u, v, w;
    cin >> u >> v >> w;
    g[u].push_back({v, w});
    g[v].push_back({u, w});                // 无向图补反向边
}
auto dist = zero_one_bfs(n, g, 1);         // 2. 调用：dist[v] = 1 到 v 的最短路
cout << (dist[t] >= 1e9 ? -1 : dist[t]) << '\n'; // 3. 1e9(INF) = 不可达
```

样例：`1-2(0), 2-3(1), 1-3(1)`，`s=1, t=3` -> 输出 `1`。

**传参要求（照这个传不会错）：**

- `zero_one_bfs(n, g, s)`：`n` = 点数；`g` = 邻接表 `vector<vector<pair<int,int>>>`，边权是 `int` 且只能 0/1（1-indexed，与 `n` 配套）；`s` = 源点。
- 返回值：`vector<int> dist`，`dist[v]` = s 到 v 的最短路；`1e9`（INF）= 不可达。
- 边权出现 `>1` 的值不能用（正确性只在 0/1 成立）；网格转向问题用本目录"0-1 BFS 网格"版。


**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
vector<int> zero_one_bfs(int n, const vector<vector<pair<int, int>>>& g, int s) {
    const int INF = 1e9;
    vector<int> dist(n + 1, INF);
    deque<int> dq;
    dist[s] = 0;
    dq.push_front(s);
    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();
        for (auto [v, w] : g[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    return dist;
}
```
#### 0-1 BFS 网格：转向次数最少

> **赛时先看**
> - **题目信号**：问最少转弯次数、镜子数、方向状态。
> - **本质**：网格中移动，直走代价 0，转向代价 1。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(nm * 4)`。
> - **维护的量**：`dist[x][y][d]`（以方向 d 进入 `(x,y)` 的最少转弯数）；答案取终点四个方向的最小值。
> - **警告**：状态是 `(x,y,dir)`，起点四个方向距离都可设 0。


**最小完整示例（先抄这一段就能跑）：**

题目：网格从 `s` 走到 `t`，直走代价 0、转向代价 1，问最少转向次数。

```cpp
int n, m;
cin >> n >> m;
vector<string> grid(n);                    // 1. 读网格，0-indexed，'#' 为墙
for (auto& s : grid) cin >> s;
pair<int, int> s = {0, 0};                 // 2. 起点 (行, 列)
pair<int, int> t = {n - 1, m - 1};         //    终点 (行, 列)
int ans = min_turns_grid(grid, s, t);      // 3. 调用：直接得到最少转向次数
cout << (ans >= 1e9 ? -1 : ans) << '\n';   // 4. 1e9(INF) = 到不了
```

样例：3x3 全可走网格，`s=(0,0), t=(2,2)` -> 输出 `1`（先直走到边再转向一次）。

**传参要求（照这个传不会错）：**

- `min_turns_grid(grid, s, t)`：`grid` = 行字符串数组（0-indexed）；`s`/`t` = 起点/终点 `（行， 列）`。
- 返回值：`int`，s 到 t 的最少转弯次数（直走 0、转向 1）；`1e9`（INF）= 不可达。
- 起点四个方向都当初始方向（距离 0），不用特判起步方向。
- 若起点就是终点，返回 `0`，不用单独判。


**改板时先认这几个量：**

- `dist`：三维距离 `dist[x][y][dir]`。

```cpp
int min_turns_grid(const vector<string>& grid, pair<int,int> s, pair<int,int> t) {
    int n = grid.size(), m = grid[0].size();
    const int INF = 1e9;
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    vector dist(n, vector(m, array<int,4>{INF, INF, INF, INF}));
    deque<tuple<int,int,int>> dq;
    for (int d = 0; d < 4; ++d) {
        dist[s.first][s.second][d] = 0;
        dq.push_back({s.first, s.second, d});
    }
    while (!dq.empty()) {
        auto [x, y, dir] = dq.front();
        dq.pop_front();
        int cur = dist[x][y][dir];
        for (int nd = 0; nd < 4; ++nd) {
            int nx = x + dx[nd], ny = y + dy[nd];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m || grid[nx][ny] == '#') continue;
            int w = (nd == dir ? 0 : 1);
            if (dist[nx][ny][nd] > cur + w) {
                dist[nx][ny][nd] = cur + w;
                if (w == 0) dq.push_front({nx, ny, nd});
                else dq.push_back({nx, ny, nd});
            }
        }
    }
    return *min_element(dist[t.first][t.second].begin(), dist[t.first][t.second].end());
}
```
#### Dijkstra

> **赛时先看**
> - **题目信号**：从单个源点出发求到各点（或到 `t`）的最短距离，边权非负；看到"单源最短路 + 边权非负"，无脑 Dijkstra。有负边（或负环）立刻停用这个模板。
> - **本质**：维护"已定最短路的点集"：每次取出 `dist` 最小者，非负边权保证它一旦出堆就定型，再用它松弛邻边；重复 n 轮即得全部最短路。
> - **复杂度判定**：`O((n+m) log n)`；`n,m` 到 2e5 轻松过，稀疏图 `n` 到 1e6 也可；边权全为 1 用 BFS 更短；稠密图可换朴素 O(n^2) 版。
> - **维护的量**：`dist`（源点到各点当前最短路）、小根堆 `priority_queue`（存候选 `（距离， 点）`）。
> - **接法**：先建 `vector<vector<pair<int,i64>>> g(n+1)`，每条有向边 `g[u].push_back({v,w})`，无向边再反向加一次；调用 `auto dist = dijkstra(n,g,s)`。问 `s` 到 `t` 就输出 `dist[t]`，若仍为 `LINF` 说明不可达。
> - **警告**：有负边不能用（贪心定型失效）；堆里旧状态用 `if (d != dist[u]) continue` 跳过，否则会重复扩展。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 条有向边（边权非负），从 `s` 出发到 `t` 的最短路。

```cpp
vector<vector<pair<int, i64>>> g(n + 1);   // 1. 建图：邻接表 (终点, 边权)
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    g[u].push_back({v, w});                // 无向边就再 push 一次 {u, w}
}
auto dist = dijkstra(n, g, s);             // 2. 调用：返回 dist[1..n]
if (dist[t] >= LINF) cout << -1 << '\n';   // 3. 不可达
else cout << dist[t] << '\n';
```

样例：`1->2(2), 2->3(3), 1->3(5)`；`s=1, t=3` -> 输出 `5`。

**传参要求（照这个传不会错）：**

- `dijkstra(n, g, s)`：`n` = 点数；`g` = 邻接表 `vector<vector<pair<int, i64>>>`（0/1-indexed 均可，但要和 `n` 配套）；`s` = 源点。
- 返回值：`vector<i64> dist`，`dist[v]` 是 `s` 到 `v` 的最短路；仍为 `LINF` 表示不可达。
- **边权必须非负**；有负边换 Bellman-Ford/SPFA。
- 需要输出路径：另开 `pre[v]` 记录每个点从哪来，跑完沿 `pre` 倒推。


**不会用就照抄：**

```cpp
vector<vector<pair<int,i64>>> g(n + 1);
g[u].push_back({v, w});           // 无向图再补 g[v].push_back({u,w})
auto dist = dijkstra(n, g, s);
cout << dist[t] << '\n';
```

- **只能保证非负边权**。有负边不要硬套。
- 距离用 `i64`；不可达通常是 `LINF`。


**API / 入口函数（赛时只认这里列的名字）：**

- `dijkstra(n,g,s)` -> `g[u]` 存 `{v,w}`；返回 `dist[1..n]`，源点为 `s`。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `dijkstra` 函数。
2. 构造：建 `vector<vector<pair<int,i64>>> g(n + 1);`。
3. 加边：有向边 `g[u].push_back({v, w});`；无向边再补 `g[v].push_back({u, w});`。
4. 调用：`auto dist = dijkstra(n, g, s);`。
5. 取结果：输出 `dist[t]`；仍为 `LINF` 即不可达。

**改造点（按题目改这几处）：**
- 0/1-indexed：模板按 1..n 建数组（大小 n+1）；题给 0..n-1 就把数组建 `n` 个（大小 n），编号 0..n-1 直接用。
- 图规模：点/边极大时保持 vector 邻接表（内存 O(n+m)）；边数接近 n^2 的稠密图换朴素 O(n^2) 版。
- 需要路径恢复：加 `vector<int> pre(n+1)`，松弛成功时记 `pre[v] = u`，最后从 `t` 沿 `pre` 倒着走回 `s` 即最短路。
- 多源：建虚拟源点连 0 边，或对每个起点各跑一次取 min。

**核心逻辑（改代码时别破坏）：**

- 堆里存 `（当前距离， 点）`；每次只扩展还等于 `dist[u]` 的最新状态。
- 只有非负边权时，“当前最小 dist 出堆即可定型”的贪心才成立。
**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
// 维护的量：dist[u] = 源点 s 到 u 的最短路；小根堆 pq 存候选 (距离, 点)。
// 不变量：出堆时 d == dist[u] 的点立即定型；非负边权保证它之后不会再被改小。
vector<i64> dijkstra(int n, const vector<vector<pair<int, i64>>>& g, int s) {
    vector<i64> dist(n + 1, LINF);
    priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq;
    dist[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue; // 过期状态（已被更短路径覆盖），跳过
        for (auto [v, w] : g[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w; // 松弛成功才入堆，维护 dist 最小性
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```
#### 线段树优化建图：点到区间 / 区间到点最短路

> **赛时先看**
> - **题目信号**：题面不断给“一个点能到一整段编号连续的点”或“一整段点能到一个点”，直接逐点连边会达到 `O(nq)`；通常要求最后跑 Dijkstra。经典题是 Codeforces 786B `Legacy`。
> - **本质**：有 `n` 个原始点，边除了 `u -> v` 外，还可能是 `u -> [l,r]` 或 `[l,r] -> v`；边权非负，求单源最短路。
> - **接法**：`n,q <= 1e5`，操作是加普通有向边、从点 `u` 到编号区间 `[l,r]` 的边、从 `[l,r]` 到点 `v` 的边，最后问源点 `s` 到所有点。创建 `SegmentTreeGraph graph(n)`；普通边调用 `add_edge(u,v,w)`，其余两类分别调用 `add_point_to_range`、`add_range_to_point`，最后只输出 `graph.dijkstra(s)[1..n]`。
> - **复杂度判定**：建两棵线段树产生 `O(n)` 个辅助点和零边；每条区间边拆成 `O(log n)` 条边。总图规模 `O(n + q log n)`，之后 Dijkstra 为 `O((n + q log n) log(n + q))`。
> - **维护的量**：`out_node/in_node`（线段树节点对应的辅助点编号）；`g`（原始点 + 辅助点的邻接表）；`tot`（当前总点数）；辅助点不可见，只影响内部图规模。
> - **警告**：点到区间与区间到点的辅助边方向相反，必须用两棵树；只适用于所有真实边权非负；返回的距离数组包含辅助点，答案只读取原始点编号 `1..n`；调用 `add_point_to_range` / `add_range_to_point` 前保证 `1 <= ql <= qr <= n`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n,q <= 1e5`，三类操作（普通边 / 点连区间 / 区间连点），边权非负，问源点 `s` 到各点最短路。

```cpp
SegmentTreeGraph graph(n);               // 1. 建树，原图点 1..n
for (int i = 0; i < q; ++i) {
    int op;
    cin >> op;
    if (op == 1) { int u, v; i64 w; cin >> u >> v >> w; graph.add_edge(u, v, w); }                    // 2. 普通边
    if (op == 2) { int u, l, r; i64 w; cin >> u >> l >> r >> w; graph.add_point_to_range(u, l, r, w); } // 3. u -> [l,r]
    if (op == 3) { int l, r, v; i64 w; cin >> l >> r >> v >> w; graph.add_range_to_point(l, r, v, w); } // 4. [l,r] -> v
}
auto dist = graph.dijkstra(s);           // 5. 调用：只读原始点 1..n
cout << (dist[t] >= LINF ? -1 : dist[t]) << '\n';
```

样例：`n=3`，操作 `2: 1 -> [2,3] 权 4`，`s=1, t=3` -> 输出 `4`。

**传参要求（照这个传不会错）：**

- `SegmentTreeGraph graph(n)`：`n` = 原始点数，原始点编号 1..n（1-indexed）。
- `add_edge(u, v, w)`：普通有向边 `u -> v`，权 `w`（i64）。
- `add_point_to_range(u, ql, qr, w)`：点 `u` 到区间内每个点，权 `w`；要求 `1 <= ql <= qr <= n`。
- `add_range_to_point(ql, qr, v, w)`：区间内每个点到 `v`，权 `w`；要求 `1 <= ql <= qr <= n`。
- `dijkstra(s)`：返回 `vector<i64>`（含辅助点），答案只读下标 `1..n`；`LINF` = 不可达。
- 无向边 = 正反各调一次 `add_edge`；区间边没有"无向"版本，分两次调。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, i64 w)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
struct SegmentTreeGraph {
    struct Edge {
        int to;
        i64 w;
    };

    int n, tot;
    vector<int> out_node, in_node;
    vector<vector<Edge>> g;

    explicit SegmentTreeGraph(int n_) { init(n_); }

    void init(int n_) {
        n = n_;
        tot = n; // 原图点编号为 1..n。
        out_node.assign(4 * n + 5, 0);
        in_node.assign(4 * n + 5, 0);
        g.assign(9 * n + 5, {});
        build(1, 1, n);
    }

    int new_node() { return ++tot; }

    void add_edge(int u, int v, i64 w) { g[u].push_back({v, w}); }

    void build(int p, int l, int r) {
        out_node[p] = new_node(); // 从区间节点连向区间内每个原图点。
        in_node[p] = new_node();  // 从区间内每个原图点连向区间节点。
        if (l == r) {
            add_edge(out_node[p], l, 0);
            add_edge(l, in_node[p], 0);
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid);
        build(p << 1 | 1, mid + 1, r);

        add_edge(out_node[p], out_node[p << 1], 0);
        add_edge(out_node[p], out_node[p << 1 | 1], 0);
        add_edge(in_node[p << 1], in_node[p], 0);
        add_edge(in_node[p << 1 | 1], in_node[p], 0);
    }

    void add_point_to_range(int u, int ql, int qr, i64 w) {
        add_point_to_range(1, 1, n, u, ql, qr, w);
    }

    void add_point_to_range(int p, int l, int r, int u, int ql, int qr, i64 w) {
        if (ql <= l && r <= qr) {
            add_edge(u, out_node[p], w);
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) add_point_to_range(p << 1, l, mid, u, ql, qr, w);
        if (qr > mid) add_point_to_range(p << 1 | 1, mid + 1, r, u, ql, qr, w);
    }

    void add_range_to_point(int ql, int qr, int v, i64 w) {
        add_range_to_point(1, 1, n, ql, qr, v, w);
    }

    void add_range_to_point(int p, int l, int r, int ql, int qr, int v, i64 w) {
        if (ql <= l && r <= qr) {
            add_edge(in_node[p], v, w);
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) add_range_to_point(p << 1, l, mid, ql, qr, v, w);
        if (qr > mid) add_range_to_point(p << 1 | 1, mid + 1, r, ql, qr, v, w);
    }

    vector<i64> dijkstra(int source) const {
        vector<i64> dist(tot + 1, LINF);
        priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq;
        dist[source] = 0;
        pq.push({0, source});
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;
            for (const auto& e : g[u]) {
                if (d <= LINF - e.w && d + e.w < dist[e.to]) {
                    dist[e.to] = d + e.w;
                    pq.push({dist[e.to], e.to});
                }
            }
        }
        return dist;
    }
};
```

典题模型：`n,q <= 1e5`，操作是加普通有向边、从点 `u` 到编号区间 `[l,r]` 的边、从 `[l,r]` 到点 `v` 的边，最后问源点 `s` 到所有点。创建 `SegmentTreeGraph graph(n)`；普通边调用 `add_edge(u,v,w)`，其余两类分别调用 `add_point_to_range`、`add_range_to_point`，最后只输出 `graph.dijkstra(s)[1..n]`。
#### Bellman-Ford

> **赛时先看**
> - **题目信号**：边权可能为负；`n,m` 不大；差分约束。
> - **本质**：有负边的单源最短路、判断源点可达负环。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O(nm)`。
> - **维护的量**：`dist[v]`（源点 s 到 v 的当前最短路）；`edges`（边列表，每轮全量扫一遍松弛）。
> - **警告**：第 `n` 轮还能松弛，说明存在可达负环。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 边（边权可为负），从 `s` 求最短路并判负环。

```cpp
vector<Edge> edges;                      // 1. 只存边列表，不用建邻接表，点编号 1..n
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    edges.push_back({u, v, w});
}
vector<i64> dist;
if (!bellman_ford(n, edges, s, dist)) cout << "neg cycle\n"; // 2. false = 有可达负环
else cout << (dist[t] == (1LL << 62) ? -1 : dist[t]) << '\n'; // 3. INF = 不可达
```

样例：`1-2(-1), 2-3(2), 1-3(10)`，`s=1, t=3` -> 输出 `1`。

**传参要求（照这个传不会错）：**

- `bellman_ford(n, edges, s, dist)`：`n` = 点数；`edges` = 边列表 `Edge{u,v,w}`（1-indexed，权可为负）；`s` = 源点；`dist` 传一个空 `vector<i64>`，函数负责赋值。
- 返回值：`true` = 无负环，`dist[v]` 即最短路；`false` = 有从 s 可达的负环（此时 dist 无效）。
- `dist[v] == (1LL << 62)`（INF）= 从 s 不可达。
- 无向负权边等价于两条反向有向边 = 负环，不要建。


```cpp
struct Edge {
    int u, v;
    i64 w;
};

bool bellman_ford(int n, const vector<Edge>& edges, int s, vector<i64>& dist) {
    const i64 INF = (1LL << 62);
    dist.assign(n + 1, INF);
    dist[s] = 0;
    for (int i = 1; i <= n - 1; ++i) {
        bool changed = false;
        for (auto [u, v, w] : edges) {
            if (dist[u] == INF) continue;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                changed = true;
            }
        }
        if (!changed) break;
    }
    for (auto [u, v, w] : edges) {
        if (dist[u] != INF && dist[v] > dist[u] + w) return false;
    }
    return true;
}
```
#### Floyd

> **赛时先看**
> - **题目信号**：`n <= 500`，多次询问任意两点最短路。
> - **本质**：全源最短路。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n^3)`。
> - **维护的量**：`d[i][j]`（i 到 j 的当前最短路，跑完即全源最短路）；每轮枚举中转点 k 做松弛。
> - **警告**：初始化 `d[i][i]=0`，无边设为 INF，跳过 INF 相加。


**最小完整示例（先抄这一段就能跑）：**

题目：`n <= 500`，多次询问任意两点最短路，边权可为负（无负环）。

```cpp
const i64 INF = (1LL << 62);
vector<vector<i64>> d(n + 1, vector<i64>(n + 1, INF)); // 1. 邻接矩阵，1-indexed
for (int i = 1; i <= n; ++i) d[i][i] = 0;
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    d[u][v] = min(d[u][v], w);           // 2. 有向边；重边取最小；无向边再补 d[v][u]
}
floyd(d, n);                             // 3. 调用：原地更新 d
cout << (d[s][t] >= INF / 2 ? -1 : d[s][t]) << '\n'; // 4. INF/2 以上 = 不可达
```

样例：`1->2(2), 2->3(3), 1->3(10)`，`s=1, t=3` -> 输出 `5`。

**传参要求（照这个传不会错）：**

- `floyd(d, n)`：`d` = `n+1` 行 `n+1` 列矩阵（1-indexed），`d[i][i]=0`、无边为 `INF`；`n` = 点数。
- 函数原地修改 `d`；跑完 `d[i][j]` = i 到 j 的最短路；`d[i][j] >= INF/2` = 不可达。
- 有负环时跑完 `d[i][i] < 0`；要判断负环就检查这一条。
- `n` 到 1000 都勉强（O(n^3) 约 1e9），更大换 Johnson / 多次 Dijkstra。


```cpp
void floyd(vector<vector<i64>>& d, int n) {
    const i64 INF = (1LL << 62);
    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            if (d[i][k] == INF) continue;
            for (int j = 1; j <= n; ++j) {
                if (d[k][j] == INF) continue;
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
            }
        }
    }
}
```
#### min-plus 矩阵快速幂：恰好 / 至多 k 条边最短路

> **赛时先看**
> - **题目信号**：题面强调“恰好走 `k` 次转移/航班/边”，`k` 极大（如 `1e18`），但点数较小，常见 `n <= 60~150`；也常和自动机、有限状态 DP 联动。
> - **本质**：在加权有向图中，求每一对点之间**恰好经过 `k` 条边**的最短路。把普通矩阵乘法的“乘法+加法”替换为“加法+取最小值”，再二进制快速幂。
> - **接法**：城市之间可乘坐航班，必须恰好乘 `k` 班，问 `s` 到 `t` 的最低票价，`k <= 1e18` 而城市数只有几十。初始化 `MinPlusMatrix trans(n)`，将每条航班写入 `trans.a[u][v]`，`min_plus_power(trans,k).a[s][t]` 即为答案；若值仍至少为 `LINF/2` 则不可达。
> - **复杂度判定**：朴素 min-plus 乘法 `O(n^3)`，快速幂总计 `O(n^3 log k)`，空间 `O(n^2)`。
> - **维护的量**：`a[i][j]`（i 到 j 恰好走"当前步数"的最短路）；快速幂中 `base` 每轮自乘翻倍、`ans` 按二进制位累计。
> - **警告**：这不是普通最短路，`k=0` 时只有 `i -> i` 的答案为 `0`；平行边只保留最小权；“至多 `k` 条边”是在每个点加一条权值 `0` 自环后，仍然求恰好 `k` 条边；要保证任何合法路径的绝对权值和远小于 `LINF`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 城航班，恰好乘 `k` 班（`k <= 1e18`），问 `s` 到 `t` 最低票价。

```cpp
MinPlusMatrix trans(n);                  // 1. n x n 全 LINF，下标 0..n-1（0-indexed）
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    trans.a[u][v] = min(trans.a[u][v], w); // 2. 有向边；平行边只留最小权
}
MinPlusMatrix res = min_plus_power(trans, k); // 3. 恰好 k 条边的转移矩阵
i64 ans = res.a[s][t];
cout << (ans >= LINF / 2 ? -1 : ans) << '\n'; // 4. 不可达
```

样例：`n=3`，边 `0->1(1), 1->2(2), 0->2(5)`，`k=2, s=0, t=2` -> 输出 `3`（0->1->2）。

**传参要求（照这个传不会错）：**

- `MinPlusMatrix trans(n)`：`n` = 点数，下标 0..n-1；初值全 `LINF`。
- `trans.a[u][v] = w`：有向边 `u -> v` 权 `w`；平行边手动 `min`；无向边再写 `trans.a[v][u] = w`。
- `min_plus_power(trans, k)`：返回"恰好 k 条边"的矩阵，答案取 `.a[s][t]`；`>= LINF/2` = 不可达。
- 要"至多 k 条边"：先 `trans.a[i][i] = min(trans.a[i][i], 0LL)`（0 权自环）再跑同样的代码。
- `k=0`：答案 `0` 当且仅当 `s == t`，否则不可达。


```cpp
struct MinPlusMatrix {
    int n;
    vector<vector<i64>> a;

    explicit MinPlusMatrix(int n_, i64 init = LINF) : n(n_), a(n_, vector<i64>(n_, init)) {}

    static MinPlusMatrix identity(int n) {
        MinPlusMatrix res(n);
        for (int i = 0; i < n; ++i) res.a[i][i] = 0;
        return res;
    }
};

MinPlusMatrix min_plus_multiply(const MinPlusMatrix& x, const MinPlusMatrix& y) {
    int n = x.n;
    MinPlusMatrix z(n);
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            if (x.a[i][k] >= LINF / 2) continue;
            for (int j = 0; j < n; ++j) {
                if (y.a[k][j] >= LINF / 2) continue;
                z.a[i][j] = min(z.a[i][j], x.a[i][k] + y.a[k][j]);
            }
        }
    }
    return z;
}

MinPlusMatrix min_plus_power(MinPlusMatrix base, i64 exp) {
    MinPlusMatrix ans = MinPlusMatrix::identity(base.n);
    while (exp > 0) {
        if (exp & 1) ans = min_plus_multiply(ans, base);
        base = min_plus_multiply(base, base);
        exp >>= 1;
    }
    return ans;
}

// 示例：对每条费用为 w 的有向边 u -> v：
// 示例：trans.a[u][v] = min(trans.a[u][v], w);
// 示例：exactly_k = min_plus_power(trans, k);
// 若要求至多 k 条边，先把 trans.a[i][i] 与 0 取 min。
```

典题模型：城市之间可乘坐航班，必须恰好乘 `k` 班，问 `s` 到 `t` 的最低票价，`k <= 1e18` 而城市数只有几十。初始化 `MinPlusMatrix trans(n)`，将每条航班写入 `trans.a[u][v]`，`min_plus_power(trans,k).a[s][t]` 即为答案；若值仍至少为 `LINF/2` 则不可达。
#### Johnson 全源最短路

> **赛时先看**
> - **题目信号**：`n` 较大、`m` 较稀疏，Floyd 过不了；有负边。
> - **本质**：稀疏有向图全源最短路，允许负边但不能有负环。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O(n m log n)`。
> - **维护的量**：`h[v]`（势能，把负权边重标成非负用）；`dist[s][v]`（s 到 v 的真实最短路，跑完要减回 `h[v]-h[s]`）。
> - **警告**：先用 SPFA/Bellman-Ford 求势能 `h`；重标边权 `w+h[u]-h[v]` 必须非负。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 较大、`m` 稀疏、边权可为负（无负环），求所有点对最短路。

```cpp
vector<JEdge> edges;                     // 1. 边列表 {u, v, w}，点编号 1..n
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    edges.push_back({u, v, w});
}
auto dist = johnson(n, edges);           // 2. 调用：dist[s][v] = s 到 v 的真实最短路
if (dist.empty()) { cout << "neg cycle\n"; return; } // 3. 空数组 = 有负环
cout << (dist[s][t] == (1LL << 60) ? -1 : dist[s][t]) << '\n'; // 4. INF = 不可达
```

样例：`1-2(-1), 2-3(2), 1-3(10)`，`s=1, t=3` -> 输出 `1`。

**传参要求（照这个传不会错）：**

- `johnson(n, edges)`：`n` = 点数；`edges` = 边列表 `JEdge{u,v,w}`（1-indexed，权可为负）。
- 返回值：`vector<vector<i64>>`（`n+1` x `n+1`），`dist[s][v]` = s 到 v 的真实最短路；`(1LL << 60)`（INF）= 不可达。
- 返回**空数组** = 图中存在负环，无最短路定义。
- 无向带负权边 = 两条反向有向边 = 负环，直接判无解。


**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
struct JEdge { int u, v; i64 w; };

vector<vector<i64>> johnson(int n, vector<JEdge> edges) {
    const i64 INF = (1LL << 60);
    vector<i64> h(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        bool changed = false;
        for (auto e : edges) {
            if (h[e.v] > h[e.u] + e.w) {
                h[e.v] = h[e.u] + e.w;
                changed = true;
            }
        }
        if (!changed) break;
        if (i == n) return {}; // 负环。
    }
    vector<vector<pair<int, i64>>> g(n + 1);
    for (auto e : edges) {
        g[e.u].push_back({e.v, e.w + h[e.u] - h[e.v]});
    }
    vector<vector<i64>> dist(n + 1, vector<i64>(n + 1, INF));
    for (int s = 1; s <= n; s++) {
        priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<>> pq;
        dist[s][s] = 0;
        pq.push({0, s});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[s][u]) continue;
            for (auto [v, w] : g[u]) {
                if (dist[s][v] > d + w) {
                    dist[s][v] = d + w;
                    pq.push({dist[s][v], v});
                }
            }
        }
        for (int v = 1; v <= n; v++) {
            if (dist[s][v] < INF) dist[s][v] += h[v] - h[s];
        }
    }
    return dist;
}
```
#### SPFA 与负环判定

> **赛时先看**
> - **题目信号**：边权可能为负；需要判断是否存在负环。
> - **本质**：含负边图的最短路、判断负环、差分约束。
> - **复杂度判定**：平均较快，最坏 `O(nm)`。
> - **维护的量**：`dist[v]`（s 到 v 的当前最短路）；`cnt[v]`（v 入队次数，`>= n` 即有负环）；`inq[v]`（v 是否在队列里）。
> - **警告**：SPFA 会被卡；能用 Dijkstra/Bellman-Ford 时优先用更稳的。


**最小完整示例（先抄这一段就能跑）：**

题目：边权可为负，求 `s` 到各点最短路并判断 s 可达的负环。

```cpp
vector<vector<pair<int, i64>>> g(n + 1); // 1. 邻接表 (终点, 边权)，点编号 1..n
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    g[u].push_back({v, w});
    g[v].push_back({u, w});              // 无向图补反向边
}
vector<i64> dist;
if (!spfa(n, g, s, dist)) cout << "neg cycle\n"; // 2. false = 有可达负环
else cout << (dist[t] == (1LL << 62) ? -1 : dist[t]) << '\n'; // 3. INF = 不可达
```

样例：`1-2(-1), 2-3(2), 1-3(10)`，`s=1, t=3` -> 输出 `1`。

**传参要求（照这个传不会错）：**

- `spfa(n, g, s, dist)`：`n` = 点数；`g` = 邻接表 `vector<vector<pair<int,i64>>>`（1-indexed，权可为负）；`s` = 源点；`dist` 传空 `vector<i64>`，函数负责赋值。
- 返回值：`true` = 无负环，`dist[v]` 即最短路；`false` = 有从 s 可达的负环。
- `dist[v] == (1LL << 62)`（INF）= 从 s 不可达。
- 只判"s 可达"的负环；要判全图负环先加超级源连所有点（见差分约束）。


**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
bool spfa(int n, const vector<vector<pair<int, i64>>>& g, int s,
          vector<i64>& dist) {
    const i64 INF = (1LL << 62);
    dist.assign(n + 1, INF);
    vector<int> inq(n + 1, 0), cnt(n + 1, 0);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    inq[s] = 1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = 0;
        for (auto [v, w] : g[u]) {
            if (dist[u] != INF && dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = 1;
                    if (++cnt[v] >= n) return false; // 存在从源点可达的负环。
                }
            }
        }
    }
    return true;
}
```
#### 差分约束

> **赛时先看**
> - **题目信号**：变量之间是差值限制，如 `x_i - x_j <= c`。
> - **本质**：求一组不等式 `x[v] <= x[u] + w` 是否有解，并给出一组解。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：建图后 Bellman-Ford/SPFA。
> - **维护的量**：`g`（差分约束图：边 `u -> v` 权 `w` 表示 `x[v] <= x[u] + w`）；`n`（变量个数）。
> - **警告**：约束 `x[v] <= x[u] + w` 建边 `u -> v` 权 `w`；超级源连所有点权 0。


**最小完整示例（先抄这一段就能跑）：**

题目：给 `m` 条约束 `x[v] - x[u] <= w`，求一组可行解或判定无解。

```cpp
DiffConstraint dc(n);                    // 1. 建空图，变量编号 1..n
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 w;
    cin >> u >> v >> w;
    dc.add_constraint(u, v, w);          // 2. 约束 x[v] - x[u] <= w
}
vector<i64> x(n + 1);
if (!dc.solve(x)) cout << "no solution\n"; // 3. false = 无解
else for (int v = 1; v <= n; ++v) cout << x[v] << " \n"[v == n]; // 4. 一组可行解
```

样例：约束 `x2 - x1 <= 3, x3 - x2 <= 1` -> 输出一组解 `0 3 4`。

**传参要求（照这个传不会错）：**

- `DiffConstraint dc(n)`：`n` = 变量个数，编号 1..n（1-indexed）。
- `add_constraint(u, v, w)`：约束 `x[v] - x[u] <= w`（建边 `u -> v` 权 `w`）；`>= w` 的约束改写为 `add_constraint(v, u, -w)`。
- `solve(x)`：`x` 传长度为 `n+1` 的 `vector<i64>`，函数填一组解；返回 `true` = 有解，`false` = 无解（负环）。
- `x[v] - x[u] == w`：拆成 `<= w` 和 `>= w` 两条约束。


**API / 入口函数（赛时只认这里列的名字）：**

- `init(int n_)` -> 初始化/清空结构
- `solve(vector<i64>& x)` -> 执行主算法并返回答案

```cpp
struct DiffConstraint {
    int n;
    vector<vector<pair<int, i64>>> g;

    DiffConstraint(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
    }

    void add_constraint(int u, int v, i64 w) {
        // 差分约束：x[v] <= x[u] + w。
        g[u].push_back({v, w});
    }

    bool solve(vector<i64>& x) {
        vector<vector<pair<int, i64>>> gg = g;
        gg.push_back({});
        int s = n + 1;
        gg.resize(n + 2);
        for (int i = 1; i <= n; ++i) gg[s].push_back({i, 0});
        return spfa(n + 1, gg, s, x);
    }
};
```
#### 最短路与次短路

> **赛时先看**
> - **题目信号**：题目问“第二短路径”“严格比最短长的最短路”。
> - **本质**：求从源点到每个点的最短和严格次短距离。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O((n+m) log n)`。
> - **维护的量**：`dist[v][0]`（最短路）、`dist[v][1]`（严格次短路），1-indexed，不可达为 `INF`。
> - **警告**：次短必须严格大于最短；相等路径计数是另一类问题。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// n=4 个点（1..4），s=1；边：1-2 权1，2-3 权2，1-3 权4
vector<vector<pair<int, i64>>> g(5);
g[1].push_back({2, 1}); g[2].push_back({1, 1});
g[2].push_back({3, 2}); g[3].push_back({2, 2});
g[1].push_back({3, 4}); g[3].push_back({1, 4});
auto dist = dijkstra_second(4, g, 1);
// 到 3：最短 dist[3][0]=3（1-2-3），次短 dist[3][1]=4（1-3）
```

**传参要求（照这个传不会错）：**
- `n`：点数，顶点编号 1..n。
- `g`：邻接表，`g[u]` 存 `{v, w}` 表示边 u→v 权 w，1-indexed，边权非负。
- `s`：源点，范围 `[1, n]`。
- 返回 `vector<array<i64,2>>`：`dist[v][0]` 最短路、`dist[v][1]` 严格次短路；不可达为 `INF = (1LL<<62)`。


**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
vector<array<i64, 2>> dijkstra_second(int n, const vector<vector<pair<int, i64>>>& g, int s) {
    const i64 INF = (1LL << 62);
    vector<array<i64, 2>> dist(n + 1, {INF, INF});
    priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq;
    dist[s][0] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u][1]) continue;
        for (auto [v, w] : g[u]) {
            i64 nd = d + w;
            if (nd < dist[v][0]) {
                dist[v][1] = dist[v][0];
                dist[v][0] = nd;
                pq.push({nd, v});
            } else if (dist[v][0] < nd && nd < dist[v][1]) {
                dist[v][1] = nd;
                pq.push({nd, v});
            }
        }
    }
    return dist;
}
```
#### 所有最短路条数

> **赛时先看**
> - **题目信号**：问“最短路径有多少条”，边权非负。
> - **本质**：求最短路长度同时统计最短路数量。
> - **复杂度判定**：`O((n+m) log n)`。
> - **维护的量**：`dist[v]`（最短路长度）、`cnt[v]`（最短路条数，对 `mod` 取模），1-indexed。
> - **警告**：相同最短距离时累加方案数。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// n=4 个点（1..4），s=1；边：1-2 权1，2-3 权1，1-3 权2
vector<vector<pair<int, i64>>> g(5);
g[1].push_back({2, 1}); g[2].push_back({1, 1});
g[2].push_back({3, 1}); g[3].push_back({2, 1});
g[1].push_back({3, 2}); g[3].push_back({1, 2});
auto [dist, cnt] = dijkstra_count_paths(4, g, 1, (i64)1e9 + 7);
// 到 3：dist[3]=2，cnt[3]=2（1-2-3 与 1-3 两条）
```

**传参要求（照这个传不会错）：**
- `n`：点数，顶点编号 1..n。
- `g`：邻接表，`g[u]` 存 `{v, w}`，1-indexed，边权非负。
- `s`：源点，范围 `[1, n]`。
- `mod`：方案数取模的模数。
- 返回 `{dist, cnt}`：`dist[v]` 最短路长（不可达为 `INF`），`cnt[v]` 最短路条数 % `mod`。


**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
pair<vector<i64>, vector<i64>> dijkstra_count_paths(
    int n,
    const vector<vector<pair<int,i64>>>& g,
    int s,
    i64 mod
) {
    const i64 INF = (1LL << 62);
    vector<i64> dist(n + 1, INF), cnt(n + 1, 0);
    priority_queue<pair<i64,int>, vector<pair<i64,int>>, greater<pair<i64,int>>> pq;
    dist[s] = 0;
    cnt[s] = 1;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            i64 nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                cnt[v] = cnt[u];
                pq.push({nd, v});
            } else if (nd == dist[v]) {
                cnt[v] = (cnt[v] + cnt[u]) % mod;
            }
        }
    }
    return {dist, cnt};
}
```
#### Karp：最小平均权环

> **赛时先看**
> - **题目信号**：长期平均成本、重复执行一段操作、找平均收益最高/平均代价最低的循环；题面会暗示“循环一次的平均值”。
> - **本质**：求有向图中平均边权最小的环，即最小化 `环总权 / 环边数`。
> - **接法**：任务可反复转移，求长期每步最小平均耗时；或在收益图中找最大平均收益环（将边权取负）。
> - **复杂度判定**：`O(nm)`，空间 `O(n^2)`；适合 `n,m` 在几千量级或更小的图。
> - **维护的量**：`dp[len][v]`（恰好走 len 条边到达 v 的最小总权，0-based），所有答案只由环决定。
> - **警告**：它和最短路不同，答案只由环决定；初始化 `dp[0][v]=0` 才能枚举图中任意连通部分的环。无环图返回 `INF`。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// n=3 个点（0..2）：0->1 权1，1->2 权1，2->0 权2
vector<MeanCycleEdge> edges = {{0, 1, 1}, {1, 2, 1}, {2, 0, 2}};
long double ans = minimum_mean_cycle(3, edges);
// 唯一环 0->1->2->0，平均权 (1+1+2)/3 = 4/3
```

**传参要求（照这个传不会错）：**
- `n`：顶点数，编号 0..n-1（0-based）。
- `edges`：每条 `{u, v, w}`，u、v ∈ [0,n)，w 为有向边权。
- 返回最小平均环权（`long double`）；无环返回 `INF = 1e100L`。找最大平均收益环时把 w 全部取负再调用。
> - **约定**：顶点 0-based。


典题模型：任务可反复转移，求长期每步最小平均耗时；或在收益图中找最大平均收益环（将边权取负）。

```cpp
struct MeanCycleEdge {
    int u, v;
    long double w;
};

long double minimum_mean_cycle(int n, const vector<MeanCycleEdge>& edges) {
    const long double INF = 1e100L;
    vector<vector<long double>> dp(n + 1, vector<long double>(n, INF));
    for (int v = 0; v < n; ++v) dp[0][v] = 0; // 超级源连向所有点，边权 0
    for (int len = 1; len <= n; ++len) {
        for (const auto& e : edges) {
            if (dp[len - 1][e.u] < INF / 2) {
                dp[len][e.v] = min(dp[len][e.v], dp[len - 1][e.u] + e.w);
            }
        }
    }
    long double ans = INF;
    for (int v = 0; v < n; ++v) if (dp[n][v] < INF / 2) {
        long double worst = -INF;
        for (int len = 0; len < n; ++len) if (dp[len][v] < INF / 2) {
            worst = max(worst, (dp[n][v] - dp[len][v]) / (n - len));
        }
        ans = min(ans, worst);
    }
    return ans; // INF 表示没有环
}
```
#### 最小生成树：Kruskal

> **赛时先看**
> - **题目信号**：题面问“铺路/建网/连通所有点/最小费用”；边较少。
> - **本质**：无向图连接所有点的最小总代价。
> - **接法**：把所有候选连接方案都放进 `edges`，每条是 `{u,v,cost}`；调用 `auto [cost, used] = kruskal(n, edges)`。只有 `used == n - 1` 时所有点才被连通，否则按题意输出 `impossible` 或特殊值。MST 解决的是“选边把所有点连起来”，不是“从 A 到 B 的最短路”。
> - **复杂度判定**：`O(m log m)`。
> - **维护的量**：`cost`（已选边总权）、`used`（已选边数）；内部 `dsu` 判两点是否已连通。
> - **警告**：用并查集；最终选边数必须是 `n-1`，否则图不连通。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// n=4 个点（1..4），边：1-2 权1，2-3 权2，3-4 权3，1-4 权10
vector<MstEdge> edges = {{1, 2, 1}, {2, 3, 2}, {3, 4, 3}, {1, 4, 10}};
auto [cost, used] = kruskal(4, edges);
// cost = 6，used = 3 == n-1，图连通
```

**传参要求（照这个传不会错）：**
- `n`：点数，顶点编号 1..n。
- `edges`：每条 `{u, v, w}` 表示 u-v 无向边权 w，u、v ∈ [1,n]。
- 返回 `pair<i64, int>`：`cost` 为 MST 总权，`used` 为选边数；`used == n-1` 才连通，否则按题意输出失败标记。


**API / 入口函数（赛时只认这里列的名字）：**

- `kruskal(n, edges)` -> 主入口；返回 `{cost, used}`，`used == n-1` 表示连通。
**改板时先认这几个量：**

- `sz`：集合/子树大小。
- `dsu`：内部并查集实例（按大小合并，用于生成树判定）。

```cpp
struct DSU {
    vector<int> p, sz;
    DSU(int n = 0) { init(n); }
    void init(int n) { p.resize(n + 1); sz.assign(n + 1, 1); iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b];
        return true;
    }
};

struct MstEdge {
    int u, v;
    i64 w;
};

pair<i64, int> kruskal(int n, vector<MstEdge> edges) {
    sort(edges.begin(), edges.end(), [](const MstEdge& a, const MstEdge& b) {
        return a.w < b.w;
    });
    DSU dsu(n);
    i64 cost = 0;
    int used = 0;
    for (auto e : edges) {
        if (dsu.unite(e.u, e.v)) {
            cost += e.w;
            used++;
        }
    }
    return {cost, used}; // used == n-1 表示最小生成树已经连通。
}
```
#### 最小生成树：Prim

> **赛时先看**
> - **题目信号**：`n` 较小、边很多、给的是矩阵。
> - **本质**：稠密图 MST，或用邻接矩阵更方便时。
> - **接法**：如果题目给的是完整代价矩阵 `w[i][j]`，直接用这个矩阵版 Prim；不存在的边设为 `INF`，`w[i][i]=0`。返回 `-1` 说明无法连通。边很少时 Kruskal 更短更稳。
> - **复杂度判定**：矩阵版 `O(n^2)`。
> - **维护的量**：`dist[v]`（v 到已选集合的最小边权）、`used[v]`（是否已加入 MST），1-indexed。
> - **警告**：每轮找未加入集合且距离最小的点；找不到说明不连通。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// n=3 个点（1..3），代价矩阵：1-2 权5，2-3 权2，1-3 权7
vector<vector<i64>> w(4, vector<i64>(4, (1LL << 62)));
for (int i = 1; i <= 3; i++) w[i][i] = 0;
w[1][2] = w[2][1] = 5; w[2][3] = w[3][2] = 2; w[1][3] = w[3][1] = 7;
i64 ans = prim_dense(w, 3);
// ans = 7（选 1-2 与 2-3）
```

**传参要求（照这个传不会错）：**
- `w`：`(n+1)×(n+1)` 对称代价矩阵 `w[i][j]`，1-indexed；不存在的边设为 `INF`，`w[i][i]=0`。
- `n`：点数。
- 返回 MST 总权 `i64`；图不连通返回 `-1`。


**改板时先认这几个量：**

- `dist`：到已选集合的最小边权。
- `used`：是否已加入 MST。

```cpp
i64 prim_dense(const vector<vector<i64>>& w, int n) {
    const i64 INF = (1LL << 62);
    vector<i64> dist(n + 1, INF);
    vector<int> used(n + 1, 0);
    dist[1] = 0;
    i64 ans = 0;
    for (int it = 1; it <= n; ++it) {
        int u = -1;
        for (int i = 1; i <= n; ++i) {
            if (!used[i] && (u == -1 || dist[i] < dist[u])) u = i;
        }
        if (u == -1 || dist[u] == INF) return -1;
        used[u] = 1;
        ans += dist[u];
        for (int v = 1; v <= n; ++v) {
            if (!used[v]) dist[v] = min(dist[v], w[u][v]);
        }
    }
    return ans;
}
```
#### 最小 Steiner 树：终端集状压 DP + 多源 Dijkstra

> **赛时先看**
> - **题目信号**：图上只有很少的关键城市/机器/宝石必须连通，题目允许经过其他点；终端数 `k` 很小（常见 `k <= 11~13`），但总点数 `n` 可以较大。
> - **本质**：在非负边权无向图中，连接所有指定终端点，允许额外经过非终端点，求最小总边权。额外经过的点就是 Steiner 点。
> - **接法**：`k <= 12` 个指定城市必须接入同一条管网，其他城市允许作为中转且每条道路只能付一次钱。建无向图后，把这 `k` 个城市传给 `minimum_cost` 即可；若某个点集不连通，返回值会保持为 `INF`，要按题意输出 `-1` 或其他失败标记。
> - **复杂度判定**：`O(3^k n + 2^k (n + m) log n)`，空间 `O(2^k n)`。边权必须非负；`k` 是决定能否使用的关键，而不是 `n`。
> - **维护的量**：`g`（无向邻接表）；`dp[mask][v]`（连通终端集 mask 且树根在 v 的最小代价）。
> - **警告**：这不是“终端之间跑 MST”，因为最优解可能使用额外 Steiner 点；所有终端可以重复时应先去重；模板只返回代价，若题目要求具体边集，需要额外记录“子集划分”和 Dijkstra 前驱。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// n=4 个点（0..3），终端 {0,2}；边：0-1 权1，1-2 权1，2-3 权10，0-3 权1
SteinerTree st(4);
st.add_edge(0, 1, 1); st.add_edge(1, 2, 1);
st.add_edge(2, 3, 10); st.add_edge(0, 3, 1);
i64 ans = st.minimum_cost({0, 2});
// ans = 2（走 0-1-2，中间点 1 是 Steiner 点）
```

**传参要求（照这个传不会错）：**
- `SteinerTree st(n)`：`n` 为点数，顶点编号 0..n-1（0-based）。
- `add_edge(u, v, w)`：加一条 u-v 无向边，权 w 非负。
- `minimum_cost(terminals)`：终端编号 ∈ [0,n) 且互不相同；返回最小总代价 `i64`，不连通时保持 `INF`，输出前转 `-1`。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, i64 w)` -> 加入一条边
- `minimum_cost(const vector<int>& terminals)` -> terminals 中的终端点必须互不相同，且编号范围为 [0,n)。 返回 `i64`。
**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。
- `dp`：DP 状态。

```cpp
struct SteinerTree {
    static constexpr i64 INF = (1LL << 62);

    int n;
    vector<vector<pair<int, i64>>> g;

    explicit SteinerTree(int n_) : n(n_), g(n_) {}

    void add_edge(int u, int v, i64 w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    void multi_source_dijkstra(vector<i64>& dist) const {
        priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq;
        for (int v = 0; v < n; ++v) {
            if (dist[v] < INF) pq.push({dist[v], v});
        }
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;
            for (auto [v, w] : g[u]) {
                if (d > INF - w) continue;
                if (d + w < dist[v]) {
                    dist[v] = d + w;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    // terminals 中的终端点必须互不相同，且编号范围为 [0,n)。
    i64 minimum_cost(const vector<int>& terminals) const {
        int k = (int)terminals.size();
        if (k <= 1) return 0;
        int full = 1 << k;
        vector<vector<i64>> dp(full, vector<i64>(n, INF));
        for (int i = 0; i < k; ++i) dp[1 << i][terminals[i]] = 0;

        for (int mask = 1; mask < full; ++mask) {
            if ((mask & (mask - 1)) != 0) {
                for (int sub = (mask - 1) & mask; sub; sub = (sub - 1) & mask) {
                    int other = mask ^ sub;
                    if (sub > other) continue; // 同一种集合划分不需要重复合并两次。
                    for (int v = 0; v < n; ++v) {
                        if (dp[sub][v] >= INF || dp[other][v] >= INF) continue;
                        dp[mask][v] = min(dp[mask][v], dp[sub][v] + dp[other][v]);
                    }
                }
            }
            // 把合并后的树根沿任意一条最短路转移。
            multi_source_dijkstra(dp[mask]);
        }
        return *min_element(dp.back().begin(), dp.back().end());
    }
};
```

典题模型：`k <= 12` 个指定城市必须接入同一条管网，其他城市允许作为中转且每条道路只能付一次钱。建无向图后，把这 `k` 个城市传给 `minimum_cost` 即可；若某个点集不连通，返回值会保持为 `INF`，要按题意输出 `-1` 或其他失败标记。
#### 次小生成树

> **赛时先看**
> - **题目信号**：最小生成树之外，还问第二小、是否唯一。
> - **本质**：求严格次小生成树或判断 MST 是否唯一。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：Kruskal `O(m log m)`，再枚举非树边 `O(m log n)`。
> - **维护的量**：`mst`（MST 总权）、`second`（严格次小总权）；`helper.mx[k][u]`（倍增表中路径最大边权）。
> - **警告**：严格次小要求替换后权值必须大于 MST；非严格次小可以允许相等。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// n=4 个点（1..4），边：1-2 权1，2-3 权2，3-4 权3，1-4 权4
vector<Edge> edges = {{1, 2, 1}, {2, 3, 2}, {3, 4, 3}, {1, 4, 4}};
auto [mst, second] = strict_second_mst(4, edges);
// mst = 6，second = 7（MST 中 1-2 换 1-4：2+3+4）
```

**传参要求（照这个传不会错）：**
- `n`：点数，顶点编号 1..n。
- `edges`：每条 `{u, v, w}` 表示 u-v 无向边权 w，u、v ∈ [1,n]。
- 返回 `pair<i64, i64>`：`{mst, second}`；不存在严格次小（或图不连通）时 `second = INF = (1LL<<62)`。


**API / 入口函数（赛时只认这里列的名字）：**

- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `init(int n)` -> 初始化/清空结构
- `init(int n_)` -> 初始化/清空结构
- `unite(int a, int b)` -> 合并两个集合 返回 `bool`。
- `strict_second_mst(int n, vector<Edge> edges)` -> 返回 {mst_weight, strict_second_mst_weight}；不存在严格次小则 second = INF。
**改板时先认这几个量：**

- `sz`：集合/子树大小。
- `depth`：深度。
- `up`：倍增祖先表。
- `mx`：区间最大值。

```cpp
struct Edge {
    int u, v;
    i64 w;
    bool in_mst = false;
};

struct DSU {
    vector<int> p, sz;
    DSU(int n = 0) { init(n); }
    void init(int n) { p.resize(n + 1); sz.assign(n + 1, 1); iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b];
        return true;
    }
};

struct SecondMST {
    int n, LOG;
    vector<vector<pair<int, i64>>> tree;
    vector<int> depth;
    vector<vector<int>> up;
    vector<vector<i64>> mx;

    SecondMST(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        tree.assign(n + 1, {});
        depth.assign(n + 1, 0);
        up.assign(LOG, vector<int>(n + 1, 1));
        mx.assign(LOG, vector<i64>(n + 1, 0));
    }

    void dfs(int u, int p) {
        up[0][u] = p;
        for (int k = 1; k < LOG; ++k) {
            up[k][u] = up[k - 1][up[k - 1][u]];
            mx[k][u] = max(mx[k - 1][u], mx[k - 1][up[k - 1][u]]);
        }
        for (auto [v, w] : tree[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            mx[0][v] = w;
            dfs(v, u);
        }
    }

    i64 max_on_path(int a, int b) {
        i64 ans = 0;
        if (depth[a] < depth[b]) swap(a, b);
        int diff = depth[a] - depth[b];
        for (int k = 0; k < LOG; ++k) if (diff >> k & 1) {
            ans = max(ans, mx[k][a]);
            a = up[k][a];
        }
        if (a == b) return ans;
        for (int k = LOG - 1; k >= 0; --k) {
            if (up[k][a] != up[k][b]) {
                ans = max(ans, mx[k][a]);
                ans = max(ans, mx[k][b]);
                a = up[k][a];
                b = up[k][b];
            }
        }
        ans = max(ans, mx[0][a]);
        ans = max(ans, mx[0][b]);
        return ans;
    }
};

// 返回 {mst_weight, strict_second_mst_weight}；不存在严格次小则 second = INF。
pair<i64, i64> strict_second_mst(int n, vector<Edge> edges) {
    const i64 INF = (1LL << 62);
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });
    DSU dsu(n);
    SecondMST helper(n);
    i64 mst = 0;
    int used = 0;
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            e.in_mst = true;
            mst += e.w;
            used++;
            helper.tree[e.u].push_back({e.v, e.w});
            helper.tree[e.v].push_back({e.u, e.w});
        }
    }
    if (used != n - 1) return {INF, INF};
    helper.dfs(1, 1);

    i64 second = INF;
    for (auto e : edges) {
        if (e.in_mst) continue;
        i64 mx = helper.max_on_path(e.u, e.v);
        i64 candidate = mst + e.w - mx;
        if (candidate > mst) second = min(second, candidate);
    }
    return {mst, second};
}
```
#### Kruskal 重构树

> **赛时先看**
> - **题目信号**：多次询问两点在边权不超过 `x` 时是否连通，或 MST 路径最大边。
> - **本质**：MST 合并过程建树，回答路径最大边权阈值、按边权连通的查询。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：建树 `O(m log m)`，查询配合 LCA。
> - **维护的量**：`parent`（并查集父）、`val[x]`（节点权值，新点取合并边权）、`tree`（重构树儿子列表，新点编号 n+1..tot）。
> - **警告**：新建节点权值为当前合并边权；原点是叶子。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// 原点点 1..3；边：1-2 权2，2-3 权5
struct E { int u, v; i64 w; };
KruskalTree kt(3);
kt.build(vector<E>{{1, 2, 2}, {2, 3, 5}});
// 新点 4（权2）、5（权5）；查询两点在边权<=t 下是否连通：找 LCA 权值<=t
```

**传参要求（照这个传不会错）：**
- `KruskalTree kt(n)`：`n` 为原点数，编号 1..n。
- `build(edges)`：`edges` 元素需含 `u/v/w` 字段（w 为边权），按边权升序自动建树。
- `find(x)`：并查集代表元查询。
- 建完后原点（1..n）是叶子，新点 `val[x]` = 合并边权，`tot` 为节点总数。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(vector<Edge> edges)` -> 完成建树或预处理
- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `init(int n_)` -> 初始化/清空结构

```cpp
struct KruskalTree {
    int n, tot;
    vector<int> parent;
    vector<i64> val;
    vector<vector<int>> tree;

    KruskalTree(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        tot = n;
        parent.resize(2 * n + 5);
        val.assign(2 * n + 5, 0);
        tree.assign(2 * n + 5, {});
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }

    template <class Edge>
    void build(vector<Edge> edges) {
        sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
        for (auto e : edges) {
            int ru = find(e.u), rv = find(e.v);
            if (ru == rv) continue;
            int x = ++tot;
            val[x] = e.w;
            tree[x].push_back(ru);
            tree[x].push_back(rv);
            parent[ru] = parent[rv] = parent[x] = x;
        }
    }
};
```
#### 隐式 Kruskal 重构树：增量网格连通块吞并阈值

> **赛时先看**
> - **题目信号**：题目按权值从小到大加入元素；元素会吞并相邻连通块；询问“从某个点出发要变多大才能吞掉整个可达块”；普通 DSU 只能知道连通性，但还需要知道向上合并时的最大门槛。
> - **本质**：在线按权值非降序激活点，每次新点与相邻已激活连通块合并。合并时不显式建重构树，只维护并查集父亲、连通块大小，以及每个儿子往父亲走需要的阈值，支持查询单点到当前根路径上的最大阈值。
> - **接法**：新点权值为 `w`，新建一个真实节点；每遇到相邻连通块根 `r`，创建虚点 `rt`，令两个根指向 `rt`，并给每个旧根记录 `need = w - size[root] + 1`。查询某真实点 `u` 时先 `find(u)` 压缩，然后答案是 `max(need[u] - value[u], 0)`。
> - **复杂度判定**：每次激活检查四邻域，均摊 `O(alpha(q))`；空间 `O(q)`，最多 `q` 个原点和 `q-1` 个合并虚点。
> - **维护的量**：`parent`（并查集父）、`value[x]`（节点权值）、`need[x]`（儿子到父亲要跨过的阈值）、`comp_size[x]`（连通块大小）。
> - **警告**：`find(x)` 路径压缩时要把 `need[x]` 更新成到新根路径上的最大阈值；合并产生的新虚点才是父亲；老根的 `size` 清零后不能再拿来当当前连通块大小。该模型要求激活权值非降，否则阈值语义会坏。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// 3x3 网格，最多 9 次激活；激活权值必须非降
ImplicitKruskalGridEat g(3, 3, 9);
g.activate(1, 1, 1);
g.activate(1, 2, 2);
int rest = g.activate(2, 1, 2);          // 吞并后该连通块还能再吞 rest 格
int need = g.extra_needed_to_eat_component(2, 1); // 该格还要变大 need 才能吃满
// 本样例：rest = 3（还可吞 3 格），need = 0
```

**传参要求（照这个传不会错）：**
- `init(n_, m_, max_ops)`：网格 `n_` 行 `m_` 列，坐标 1-indexed；`max_ops` 为最大激活次数（内部按 `2*max_ops+5` 预留空间）。
- `activate(x, y, w)`：按非降权值 w 激活格子 (x,y)；返回当前连通块还能吞并的其他格子数量。
- `extra_needed_to_eat_component(x, y)`：查询该格要吃满所在连通块还需变大的最小增量，格子必须已激活。
- 警告：激活权值必须非降，否则阈值语义会坏。


**API / 入口函数（赛时只认这里列的名字）：**

- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `init(int n_, int m_, int max_ops)` -> 初始化/清空结构
- `activate(int x, int y, int w)` -> 按非降权值 w 激活格子 (x,y)。 返回当前连通块中还能吞并的其他格子数量。
**改板时先认这几个量：**

- `parent`：并查集父节点。
- `find`：带上限的路径压缩查询（find(x, need)）。

```cpp
struct ImplicitKruskalGridEat {
    int n = 0, m = 0, total = 0;
    vector<vector<int>> id;
    vector<int> parent, comp_size, value, need;
    const int dx[4] = {1, 0, -1, 0};
    const int dy[4] = {0, 1, 0, -1};

    ImplicitKruskalGridEat() = default;
    ImplicitKruskalGridEat(int n_, int m_, int max_ops) { init(n_, m_, max_ops); }

    void init(int n_, int m_, int max_ops) {
        n = n_;
        m = m_;
        total = 0;
        id.assign(n + 1, vector<int>(m + 1, 0));
        int cap = 2 * max_ops + 5;
        parent.assign(cap, 0);
        comp_size.assign(cap, 0);
        value.assign(cap, 0);
        need.assign(cap, 0);
    }

    int find(int x) {
        if (parent[x] == x) return x;
        int p = parent[x];
        int root = find(p);
        need[x] = max(need[x], need[p]);
        return parent[x] = root;
    }

    void merge_roots(int a, int b, int current_weight) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        int root = ++total;
        parent[root] = root;
        comp_size[root] = comp_size[a] + comp_size[b];
        value[root] = current_weight;
        need[root] = 0;

        parent[a] = parent[b] = root;
        need[a] = current_weight - comp_size[a] + 1;
        need[b] = current_weight - comp_size[b] + 1;
        comp_size[a] = comp_size[b] = 0;
    }

    // 按非降权值 w 激活格子 (x,y)。
    // 返回当前连通块中还能吞并的其他格子数量。
    int activate(int x, int y, int w) {
        int u = ++total;
        id[x][y] = u;
        parent[u] = u;
        comp_size[u] = 1;
        value[u] = w;
        need[u] = 0;
        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir], ny = y + dy[dir];
            if (nx < 1 || nx > n || ny < 1 || ny > m || id[nx][ny] == 0) continue;
            merge_roots(u, id[nx][ny], w);
        }
        return comp_size[find(u)] - 1;
    }

    int extra_needed_to_eat_component(int x, int y) {
        int u = id[x][y];
        assert(u != 0);
        find(u);
        return max(need[u] - value[u], 0);
    }
};
```

典题：本场 C《Fish Eating》。`activate(x,y,w)` 对应放入一条鱼后能吃掉的数量；`extra_needed_to_eat_component(x,y)` 对应把该格鱼最少变大多少才能达到最大可吃数量。
#### 朱刘算法：有向图最小树形图

> **赛时先看**
> - **题目信号**：有向图版本 MST；每个非根点要选一条入边。
> - **本质**：求以 `root` 为根，到达所有点的有向最小生成树。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：朴素实现 `O(nm)`。
> - **维护的量**：`in[v]`（每轮 v 的最小入边权）、`pre[v]`（该入边起点）、`id[v]`（缩环后的新编号）。
> - **警告**：必须检查所有点可达；缩环时边权减去入边权。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// n=3，根 1；有向边 1->2 权4，1->3 权2，3->2 权1
vector<DirectedEdge> edges = {{1, 2, 4}, {1, 3, 2}, {3, 2, 1}};
i64 ans = directed_mst(3, 1, edges);
// ans = 3（选 1->3 与 3->2）
```

**传参要求（照这个传不会错）：**
- `n`：顶点数，编号 1..n。
- `root`：根，范围 `[1, n]`。
- `edges`：每条 `{u, v, w}` 表示有向边 u→v 权 w，u、v ∈ [1,n]；自环会被忽略。
- 返回最小树形图总权 `i64`；有点不可达返回 `-1`。


```cpp
struct DirectedEdge { int u, v; i64 w; };

i64 directed_mst(int n, int root, vector<DirectedEdge> edges) {
    const i64 INF = (1LL << 60);
    i64 ans = 0;
    while (true) {
        vector<i64> in(n + 1, INF);
        vector<int> pre(n + 1, -1);
        for (auto e : edges) {
            if (e.u != e.v && e.w < in[e.v]) {
                in[e.v] = e.w;
                pre[e.v] = e.u;
            }
        }
        in[root] = 0;
        for (int i = 1; i <= n; i++) if (in[i] == INF) return -1;
        int cnt = 0;
        vector<int> id(n + 1, 0), vis(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            ans += in[i];
            int v = i;
            while (vis[v] != i && !id[v] && v != root) {
                vis[v] = i;
                v = pre[v];
            }
            if (v != root && !id[v]) {
                id[v] = ++cnt;
                for (int u = pre[v]; u != v; u = pre[u]) id[u] = cnt;
            }
        }
        if (!cnt) break;
        for (int i = 1; i <= n; i++) if (!id[i]) id[i] = ++cnt;
        vector<DirectedEdge> ne;
        for (auto e : edges) {
            int u = id[e.u], v = id[e.v];
            i64 w = e.w;
            if (u != v) w -= in[e.v];
            ne.push_back({u, v, w});
        }
        root = id[root];
        n = cnt;
        edges.swap(ne);
    }
    return ans;
}
```
#### Stoer-Wagner 全局最小割

> **赛时先看**
> - **题目信号**：不是指定 s-t 最小割，而是全局最小割；点数通常几百以内。
> - **本质**：无向带权图求任意划分两侧割边权最小值。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n^3)`。
> - **维护的量**：`dis[j]`（j 到当前已选集合的总边权）、`v`（剩余点集）、`w`（每轮合并后收缩的权矩阵）。
> - **警告**：图必须是无向图；每轮把最后加入的两个点合并；图不连通时返回 0（最小割为 0），按题意自行判断。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// 3 个点（0..2），边：0-1 权2，1-2 权3，0-2 权5
vector<vector<i64>> w(3, vector<i64>(3, 0));
w[0][1] = w[1][0] = 2; w[1][2] = w[2][1] = 3; w[0][2] = w[2][0] = 5;
i64 ans = stoer_wagner(w);
// ans = 5（割 {0,2}|{1}：2+3）
```

**传参要求（照这个传不会错）：**
- `w`：n×n 对称权矩阵（0-based 编号），`w[i][j]` 为 i-j 边权，无边为 0，重边直接累加。
- 返回全局最小割 `i64`；图不连通时返回 0。


```cpp
i64 stoer_wagner(vector<vector<i64>> w) {
    int n = (int)w.size();
    vector<int> v(n);
    iota(v.begin(), v.end(), 0);
    const i64 INF = (1LL << 62);
    i64 ans = INF;
    while (n > 1) {
        vector<i64> dis(n, 0);
        vector<int> used(n, 0);
        int prev = -1, sel = -1;
        for (int i = 0; i < n; i++) {
            sel = -1;
            for (int j = 0; j < n; j++) {
                if (!used[j] && (sel == -1 || dis[j] > dis[sel])) sel = j;
            }
            if (i == n - 1) {
                ans = min(ans, dis[sel]);
                for (int j = 0; j < n; j++) {
                    w[prev][j] += w[sel][j];
                    w[j][prev] += w[j][sel];
                }
                v.erase(v.begin() + sel);
                for (int j = 0; j < n; j++) w[j].erase(w[j].begin() + sel);
                w.erase(w.begin() + sel);
                n--;
                break;
            }
            used[sel] = 1;
            prev = sel;
            for (int j = 0; j < n; j++) if (!used[j]) dis[j] += w[sel][j];
        }
    }
    return ans;
}
```
#### Matrix-Tree 定理：生成树计数

> **赛时先看**
> - **题目信号**：题面问生成树个数；`n` 不大但边多；答案取模。
> - **本质**：求无向图生成树数量，或带权生成树总权。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n^3)`。
> - **维护的量**：`lap`（1-indexed 拉普拉斯矩阵）、`a`（删第 1 行第 1 列后的 n-1 阶子式）。
> - **警告**：构造拉普拉斯矩阵后删掉任意一行一列求行列式；重边要累加；下面 `det_mod` 版本要求 `mod` 为质数。

**最小完整示例（先抄这一段就能跑）：**
```cpp
// n=3 个点（1..3）：1-2 两条重边，2-3 一条
vector<pair<int, int>> edges = {{1, 2}, {1, 2}, {2, 3}};
i64 ans = count_spanning_tree(3, edges, (i64)1e9 + 7);
// ans = 2（必含 2-3，再在两条 1-2 重边里任选一条）
```

**传参要求（照这个传不会错）：**
- `count_spanning_tree(n, edges, mod)`：`n` 个点编号 1..n；`edges` 为 `{u, v}` 无向边，重边算多条；返回生成树个数 % `mod`。
- `det_mod(a, mod)`：`a` 为 n-1 阶模矩阵，`mod` 必须是质数；返回 det % `mod`。
- 用法：1-indexed 建拉普拉斯矩阵，删第 1 行第 1 列，子式行列式即答案。
> - **约定**：拉普拉斯矩阵用 1-indexed，删第 1 行第 1 列。


```cpp
i64 det_mod(vector<vector<i64>> a, i64 mod) {
    int n = (int)a.size();
    i64 ans = 1;
    for (int i = 0; i < n; i++) {
        int pivot = i;
        while (pivot < n && a[pivot][i] % mod == 0) pivot++;
        if (pivot == n) return 0;
        if (pivot != i) {
            swap(a[pivot], a[i]);
            ans = (mod - ans) % mod;
        }
        i64 inv = 1, base = (a[i][i] % mod + mod) % mod, e = mod - 2;
        while (e) {
            if (e & 1) inv = inv * base % mod;
            base = base * base % mod;
            e >>= 1;
        }
        ans = ans * ((a[i][i] % mod + mod) % mod) % mod;
        for (int j = i + 1; j < n; j++) {
            i64 factor = (a[j][i] % mod + mod) % mod * inv % mod;
            for (int k = i; k < n; k++) {
                a[j][k] = (a[j][k] - factor * a[i][k]) % mod;
            }
        }
    }
    return (ans % mod + mod) % mod;
}

i64 count_spanning_tree(int n, const vector<pair<int, int>>& edges, i64 mod) {
    vector lap(n + 1, vector<i64>(n + 1, 0));
    for (auto [u, v] : edges) {
        lap[u][u]++; lap[v][v]++;
        lap[u][v]--; lap[v][u]--;
    }
    vector a(n - 1, vector<i64>(n - 1, 0));
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) a[i - 1][j - 1] = lap[i][j];
    }
    return det_mod(a, mod);
}
```

### 07 图论进阶：连通性、拓扑与结构

拓扑、DAG、支配树、强连通/双连通、仙人掌、2-SAT、欧拉路、传递闭包和最大团放在这里。
#### 拓扑排序

> **赛时先看**
> - **题目信号**：有向边表示先后顺序；问是否存在合法顺序。
> - **本质**：DAG 顺序、依赖关系、课程先修、DAG DP。
> - **接法**：边 `u -> v` 表示 `u` 必须在 `v` 前完成；把所有入度为 `0` 的点入队。返回的 `order` 是一种合法顺序；如果 `order.size() < n`，说明依赖里有环，DAG DP 不能直接做。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`indeg`（每个点的入度）与 `order`（已排出的拓扑序）。
> - **警告**：结果长度小于 `n` 说明有环。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 门课有先后依赖，边 `u->v` 表示先修 `u` 再修 `v`，求一种合法学习顺序。

```cpp
int n = 4;                              // 样例输入，抄题时换成你的输入
vector<vector<int>> g(n + 1);           // 1-based 邻接表
g[1].push_back(2);                      // 样例边：1->2、2->3、3->4
g[2].push_back(3);
g[3].push_back(4);
vector<int> order = topo_sort(n, g);           // 1-based 邻接表
if ((int)order.size() < n) cout << "有环，无合法顺序\n";
else for (int u : order) cout << u << ' ';     // 按序输出即可
```

样例：`n=4`，边 `1->2、2->3、3->4`，输出 `1 2 3 4`。

**传参要求（照这个传不会错）：**

- `n`：顶点数；下标 `1..n`。
- `g`：邻接表，`g[u]` 存从 `u` 出发的边终点 `v`，范围 `1..n`。
- 返回值 `order`：一种合法拓扑序；`order.size() < n` 说明有环（无合法顺序）。

**改板时先认这几个量：**

- `g`：邻接表。
- `indeg`：入度。

```cpp
vector<int> topo_sort(int n, const vector<vector<int>>& g) {
    vector<int> indeg(n + 1), order;
    for (int u = 1; u <= n; ++u) {
        for (int v : g[u]) indeg[v]++;
    }
    queue<int> q;
    for (int i = 1; i <= n; ++i) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }
    return order;
}
```
#### DAG 最长路

> **赛时先看**
> - **题目信号**：有依赖顺序，边可以有权，图无环。
> - **本质**：有向无环图上的最长路径/最大收益。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`dp[v]`（到 `v` 的最长距离，`NEG` 表示不可达）、`indeg`（入度）。
> - **警告**：普通图最长路很难；只有 DAG 可以这样做。


**最小完整示例（先抄这一段就能跑）：**

题目：DAG 上每条边有权，求从某个入度为 0 的点出发到各点的最长距离。

```cpp
int n = 3;                              // 样例输入，抄题时换成你的输入
vector<vector<pair<int, i64>>> g(n + 1); // 1-based 邻接表，g[u] 存 {v, w}
g[1].push_back({2, 5});                 // 样例边：1->2(5)、2->3(4)
g[2].push_back({3, 4});
int v = 3;                              // 样例询问：dp[3] = 9
vector<i64> dp = dag_longest_path(n, g);   // g[u] 存 {v, w}，w 为 i64 权
cout << dp[v] << '\n';                     // v 的最长路；NEG 表示不可达
```

样例：`n=3`，边 `1->2(5)、2->3(4)`，`dp[3]=9`。

**传参要求（照这个传不会错）：**

- `n`：顶点数；下标 `1..n`。
- `g`：邻接表，`g[u]` 存 `{v, w}`（终点、边权），编号 `1..n`，权用 `i64`。
- 返回值 `dp`：`dp[v]` 为到 `v` 的最长距离；入度为 0 的点为 `0`；不可达为 `NEG = -(1LL<<60)`。

**改板时先认这几个量：**

- `g`：邻接表。
- `dp`：DP 状态。
- `indeg`：入度。

```cpp
vector<i64> dag_longest_path(int n, const vector<vector<pair<int, i64>>>& g) {
    vector<int> indeg(n + 1);
    for (int u = 1; u <= n; ++u) {
        for (auto [v, w] : g[u]) indeg[v]++;
    }
    queue<int> q;
    for (int i = 1; i <= n; ++i) if (indeg[i] == 0) q.push(i);

    const i64 NEG = -(1LL << 60);
    vector<i64> dp(n + 1, NEG);
    for (int i = 1; i <= n; ++i) if (indeg[i] == 0) dp[i] = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto [v, w] : g[u]) {
            if (dp[u] != NEG) dp[v] = max(dp[v], dp[u] + w);
            if (--indeg[v] == 0) q.push(v);
        }
    }
    return dp;
}
```
#### 非树边造好环 + BFS 定向构造

> **赛时先看**
> - **题目信号**：题目要求输出每条无向边的方向；只要每个弱连通块/基环森林里的环满足某个性质即可；存在一条非树边能制造关键环；其它边只需保证能流向这个环。
> - **本质**：在无向连通图中先找一条满足性质的非树边，让最终有向图至少包含一个“好环”，再把其余边按 BFS 深度定向成指向汇点的 DAG。
> - **接法**：把“不坏的边”先用于建生成结构；遇到一条端点满足好性质且已经在同一 DSU 内的边，就把它留作特殊非树边 `(s,t)`；从 `s` BFS，其余边按深度从大到小指向小深度，特殊边强制 `s <- t` 或按题意反向。
> - **复杂度判定**：并查集找边 `O(m alpha(n))`，BFS 定向 `O(n+m)`。
> - **维护的量**：`dsu`（挑“不坏的边”建生成结构、找出特殊非树边 `(s,t)`）；`depth[u]`（从 `s` BFS 的深度，定向依据：深指向浅）；特殊边 `(s,t)` 单独记下、最后强制反向。
> - **警告**：先用 DSU 选择“可作为额外环边”的边；BFS 定向时深度小的点应成为汇点方向；最后要对特殊非树边反向，确保环里包含关键差异。若没有合格非树边，应输出无解。


典题：本场 I《Combination of Two Nice Problems》。先用相等坐标边尽量连通，找到一条端点坐标不同且会成环的非树边；从其中一端 BFS，对其他边按层次定向，特殊边反向，使唯一环含有不同坐标点。

#### 支配树：Lengauer-Tarjan

> **赛时先看**
> - **题目信号**：控制流图、必须经过的检查点、删去一个点会让哪些点从起点不可达、所有路径公共关键节点。
> - **本质**：在有向图从起点 `s` 出发时，求每个点的直接支配者。若到达 `v` 的每条路径都必须经过 `u`，则 `u` 支配 `v`。
> - **接法**：给出有向图和起点，询问“必须经过某点才能到达”的支配关系，或对支配树做子树统计。
> - **复杂度判定**：接近线性，常写为 `O((n+m) alpha(n))`。
> - **维护的量**：`idom[v]`（`v` 的直接支配者）、`sdom/dom`（半支配者/支配者，按 DFS 序坐标维护）。
> - **警告**：只对从根可达的节点有定义；返回 `idom[v] = -1` 表示不可达，根自己的直接支配者置为根。
> - **约定**：图 0-based。


**最小完整示例（先抄这一段就能跑）：**

题目：有向图问“删掉某点后哪些点从起点不可达”，即求每个点的直接支配者。

```cpp
int n = 3;                              // 样例输入，抄题时换成你的输入
vector<vector<int>> g(n);               // 0-based 邻接表
g[0].push_back(1);                      // 样例边：0->1、0->2、1->2
g[0].push_back(2);
g[1].push_back(2);
int root = 0;                           // 样例源点
DominatorTree dt(g);                  // g 为 0-based 邻接表
vector<int> idom = dt.build(root);    // root 为源点（0-based）
for (int v = 0; v < n; ++v) cout << idom[v] << ' '; // 直接支配者，-1=不可达
```

样例：`n=3`，边 `0->1、0->2、1->2`，`idom = {0,0,0}`（`idom[0]=0` 表示根支配自己）。

**传参要求（照这个传不会错）：**

- 构造 `DominatorTree(graph)`：`graph` 是 0-based 邻接表，大小 `n`。
- `build(int root)`：`root` 为源点，0-based，范围 `0..n-1`。
- 返回值 `idom`（长度 `n`）：`idom[v]` 是 `v` 的直接支配者；`idom[root] = root`；`idom[v] = -1` 表示 `v` 从 `root` 不可达。

**API / 入口函数（赛时只认这里列的名字）：**

- `build(int root)` -> 完成建树或预处理 返回 `vector<int>`。
**改板时先认这几个量：**

- `dsu`：Lengauer-Tarjan 的 DSU 辅助数组（路径压缩 + 半支配者合并）。
- `root`：`build(root)` 的源点。
- `g`：邻接表。
- `depth`：深度。

典题模型：给出有向图和起点，询问“必须经过某点才能到达”的支配关系，或对支配树做子树统计。

```cpp
struct DominatorTree {
    int n, timer = 0;
    vector<vector<int>> g, rg, bucket;
    vector<int> arr, rev, par, sdom, dom, dsu, label;

    DominatorTree(const vector<vector<int>>& graph) : n((int)graph.size()), g(graph) {}

    void dfs(int u) {
        arr[u] = timer;
        rev[timer] = u;
        label[timer] = sdom[timer] = dsu[timer] = timer;
        ++timer;
        for (int v : g[u]) {
            if (arr[v] != -1) continue;
            dfs(v);
            par[arr[v]] = arr[u];
        }
    }

    int find_set(int u, int depth = 0) {
        if (u == dsu[u]) return depth ? -1 : u;
        int v = find_set(dsu[u], depth + 1);
        if (v < 0) return u;
        if (sdom[label[dsu[u]]] < sdom[label[u]]) label[u] = label[dsu[u]];
        dsu[u] = v;
        return depth ? v : label[u];
    }

    vector<int> build(int root) {
        arr.assign(n, -1);
        rev.assign(n, -1);
        par.assign(n, -1);
        sdom.assign(n, 0);
        dom.assign(n, 0);
        dsu.assign(n, 0);
        label.assign(n, 0);
        timer = 0;
        dfs(root);

        int T = timer;
        rg.assign(T, {});
        bucket.assign(T, {});
        for (int u = 0; u < n; ++u) if (arr[u] != -1) {
            for (int v : g[u]) if (arr[v] != -1) rg[arr[v]].push_back(arr[u]);
        }

        for (int i = T - 1; i >= 0; --i) {
            for (int v : rg[i]) sdom[i] = min(sdom[i], sdom[find_set(v)]);
            if (i) bucket[sdom[i]].push_back(i);
            for (int v : bucket[i]) {
                int y = find_set(v);
                dom[v] = (sdom[y] == sdom[v] ? sdom[v] : y);
            }
            if (i) dsu[i] = par[i];
        }
        for (int i = 1; i < T; ++i) {
            if (dom[i] != sdom[i]) dom[i] = dom[dom[i]];
        }

        vector<int> idom(n, -1);
        idom[root] = root;
        for (int i = 1; i < T; ++i) idom[rev[i]] = rev[dom[i]];
        return idom;
    }
};
```
#### DAG 最小路径覆盖

> **赛时先看**
> - **题目信号**：有向无环图，问最少链/路径覆盖全部点。
> - **本质**：用最少条路径覆盖 DAG 上所有点。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：建二分图后最大匹配，答案 `n - matching`。
> - **维护的量**：无持久状态；内部维护二分图最大匹配数，答案即 `n - matching`。
> - **警告**：只适用于 DAG；每个原点拆成左点和右点。依赖第 08 章第 66 节 `HopcroftKarp`，抄板时须一并抄上。


**最小完整示例（先抄这一段就能跑）：**

题目：DAG 上选尽量少的路径覆盖所有点，求最少路径条数。

依赖：08 章 HopcroftKarp struct，抄板时一起抄上。

```cpp
int n = 3;                              // 样例输入，抄题时换成你的输入
vector<vector<int>> dag(n + 1);         // 1-based 邻接表
dag[1].push_back(2);                    // 样例边：1->2
int ans = minimum_path_cover_dag(n, dag);  // dag 为 1-based 邻接表
cout << ans << '\n';                        // 最少路径条数
```

样例：`n=3`，边 `1->2`，答案 `2`（路径 `1->2` 与 `3`）。

**传参要求（照这个传不会错）：**

- `n`：顶点数；下标 `1..n`。
- `dag`：邻接表，`dag[u]` 存 `u` 可直接到达的后继 `v`，范围 `1..n`。
- 返回值：`int`，最小路径覆盖数（= `n -` 最大匹配）。
- 依赖：须一并抄第 08 章 `HopcroftKarp`，否则无法编译。


```cpp
int minimum_path_cover_dag(int n, const vector<vector<int>>& dag) {
    HopcroftKarp hk(n, n);
    for (int u = 1; u <= n; ++u) {
        for (int v : dag[u]) hk.add_edge(u, v);
    }
    return n - hk.max_matching();
}
```
#### Tarjan 强连通分量 SCC

> **赛时先看**
> - **题目信号**：有向图中问“互相到达”“环依赖”“缩点后 DAG”。
> - **本质**：有向图缩点、互相可达关系。
> - **接法**：先 `SCC scc(n)`，所有有向边调用 `add_edge(u,v)`，最后 `scc.run()`。如果题目问两个点是否互相可达，看 `scc.comp[u] == scc.comp[v]`；如果要缩点建 DAG，就把原边 `(u,v)` 中 `comp[u] != comp[v]` 的部分连到新图。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`dfn/low`（时间戳/lowlink）、`comp[u]`（`u` 所属 SCC 编号）、`comp_cnt`。
> - **警告**：`comp[u]` 相同表示同一强连通分量。


**最小完整示例（先抄这一段就能跑）：**

题目：有向图问 `u`、`v` 是否互相可达（同一 SCC），并统计缩点个数。

```cpp
int n = 2;                              // 样例输入，抄题时换成你的输入
int u = 1, v = 2;                       // 样例查询点
SCC scc(n);                 // n 为点数，编号 1..n
scc.add_edge(u, v);         // 每条有向边都加
scc.add_edge(v, u);         // 样例边：1->2、2->1
int cnt = scc.run();        // 返回强连通分量个数
if (scc.comp[u] == scc.comp[v]) cout << "互相可达\n";
```

样例：`n=2`，边 `1->2、2->1`，`run()` 返回 `1`，`comp[1]==comp[2]`。

**传参要求（照这个传不会错）：**

- `SCC(n)` / `init(n_)`：`n` 为点数，编号 `1..n`。
- `add_edge(int u, int v)`：加一条有向边，`u`、`v` 范围 `1..n`。
- `run()`：返回 `comp_cnt`（SCC 个数），并填好 `comp`。
- `comp[u]`：`u` 所属 SCC 编号，范围 `1..comp_cnt`；相等即互相可达。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `dfn`：DFS 序时间戳。
- `low`：lowlink / 最早可达时间戳。

```cpp
struct SCC {
    int n, timer = 0, comp_cnt = 0;
    vector<vector<int>> g;
    vector<int> dfn, low, st, in_st, comp;

    SCC(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        in_st.assign(n + 1, 0);
        comp.assign(n + 1, 0);
        st.clear();
        timer = comp_cnt = 0;
    }
    void add_edge(int u, int v) { g[u].push_back(v); }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        st.push_back(u);
        in_st[u] = 1;
        for (int v : g[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_st[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            comp_cnt++;
            while (true) {
                int x = st.back();
                st.pop_back();
                in_st[x] = 0;
                comp[x] = comp_cnt;
                if (x == u) break;
            }
        }
    }

    int run() {
        for (int i = 1; i <= n; ++i) if (!dfn[i]) dfs(i);
        return comp_cnt;
    }
};
```
#### 割点与桥

> **赛时先看**
> - **题目信号**：网络脆弱性、关键道路、删点/删边后是否断开。
> - **本质**：无向图中找删掉后影响连通性的点/边。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`dfn/low`（时间戳/lowlink）、`is_cut[u]`（割点标记）、`bridges`（所有桥的端点对）。
> - **警告**：无向边要带边编号，避免把反向边当父边跳过。
> - **读答案**：答案在公开成员 `is_cut[u]` 与 `bridges` 中。


**最小完整示例（先抄这一段就能跑）：**

题目：无向图问删掉哪些点（或边）会使图不连通，输出所有割点与桥。

```cpp
int n = 3;                              // 样例输入，抄题时换成你的输入
int u = 1, v = 2;                       // 样例边起点/终点
CutBridge cb(n);                    // n 为点数，编号 1..n
cb.add_edge(u, v);                  // 无向边加一次即可
cb.add_edge(2, 3);                  // 样例：三角形 1-2-3-1
cb.add_edge(3, 1);
cb.run();                           // 跑完答案直接可读
if (cb.is_cut[u]) cout << "u 是割点\n";
for (auto [a, b] : cb.bridges) cout << a << ' ' << b << '\n';
```

样例：三角形 `1-2-3-1`，无割点、`bridges` 为空。

**传参要求（照这个传不会错）：**

- `CutBridge(n)` / `init(n_)`：`n` 为点数，编号 `1..n`。
- `add_edge(int u, int v)`：加一条无向边，`u`、`v` 范围 `1..n`，重边也安全。
- `run()`：无返回值；执行后结果就绪。
- `is_cut[u]`：`1` 表示 `u` 是割点；`bridges`：所有桥 `{u,v}` 的列表。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `dfn`：DFS 序时间戳。
- `low`：lowlink / 最早可达时间戳。

```cpp
struct CutBridge {
    int n, timer = 0, edge_id = 0;
    vector<vector<pair<int, int>>> g;
    vector<int> dfn, low, is_cut;
    vector<pair<int, int>> bridges;

    CutBridge(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        is_cut.assign(n + 1, 0);
        bridges.clear();
        timer = edge_id = 0;
    }
    void add_edge(int u, int v) {
        g[u].push_back({v, edge_id});
        g[v].push_back({u, edge_id++});
    }
    void dfs(int u, int parent_edge) {
        dfn[u] = low[u] = ++timer;
        int child = 0;
        for (auto [v, id] : g[u]) {
            if (id == parent_edge) continue;
            if (!dfn[v]) {
                child++;
                dfs(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) bridges.push_back({u, v});
                if (parent_edge != -1 && low[v] >= dfn[u]) is_cut[u] = 1;
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (parent_edge == -1 && child > 1) is_cut[u] = 1;
    }
    void run() {
        for (int i = 1; i <= n; ++i) if (!dfn[i]) dfs(i, -1);
    }
};
```
#### 边双连通分量 e-DCC

> **赛时先看**
> - **题目信号**：询问桥、删一条边后的连通性、桥树直径、至少加多少边使边双连通。
> - **本质**：无向图删掉桥后得到边双连通分量，可缩点成桥树。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`is_bridge[id]`（边是否为桥）、`comp[u]`（`u` 所属边双编号）、`comp_cnt`。
> - **警告**：无向边要用边编号排除反向边；重边不是桥。


**最小完整示例（先抄这一段就能跑）：**

题目：无向图删掉所有桥后分成几个连通块，输出每个点所属边双。

```cpp
int n = 4;                              // 样例输入，抄题时换成你的输入
int u = 1, v = 2;                       // 样例边起点/终点
EdgeBCC eb(n);                  // n 为点数，编号 1..n
eb.add_edge(u, v);              // 无向边加一次即可
eb.add_edge(2, 3);              // 样例：边 1-2、2-3、3-1、3-4
eb.add_edge(3, 1);
eb.add_edge(3, 4);
int cnt = eb.build();           // 返回边双个数
for (int u = 1; u <= n; ++u) cout << eb.comp[u] << ' ';
```

样例：`n=4`，边 `1-2、2-3、3-1、3-4`，`build()` 返回 `2`（`{1,2,3}` 与 `{4}`，`3-4` 是桥）。

**传参要求（照这个传不会错）：**

- `EdgeBCC(n)` / `init(n_)`：`n` 为点数，编号 `1..n`。
- `add_edge(int u, int v)`：加一条无向边，`u`、`v` 范围 `1..n`，重边自动处理。
- `build()`：返回 `comp_cnt`（边双个数），并填好 `comp`。
- `comp[u]`：`u` 所属边双编号，范围 `1..comp_cnt`。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `build()` -> 完成建树或预处理 返回 `int`。
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `dfn`：DFS 序时间戳。
- `low`：lowlink / 最早可达时间戳。

```cpp
struct EdgeBCC {
    int n, timer = 0, comp_cnt = 0;
    vector<vector<pair<int, int>>> g;
    vector<int> dfn, low, is_bridge, comp;
    vector<pair<int, int>> edges;

    EdgeBCC(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        comp.assign(n + 1, 0);
        is_bridge.clear();
        edges.clear();
        timer = comp_cnt = 0;
    }

    void add_edge(int u, int v) {
        int id = (int)edges.size();
        edges.push_back({u, v});
        is_bridge.push_back(0);
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }

    void tarjan(int u, int in_edge) {
        dfn[u] = low[u] = ++timer;
        for (auto [v, id] : g[u]) {
            if (!dfn[v]) {
                tarjan(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) is_bridge[id] = 1;
            } else if (id != in_edge) {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void dfs_comp(int u) {
        comp[u] = comp_cnt;
        for (auto [v, id] : g[u]) {
            if (comp[v] || is_bridge[id]) continue;
            dfs_comp(v);
        }
    }

    int build() {
        for (int i = 1; i <= n; i++) if (!dfn[i]) tarjan(i, -1);
        for (int i = 1; i <= n; i++) if (!comp[i]) {
            comp_cnt++;
            dfs_comp(i);
        }
        return comp_cnt;
    }
};
```
#### 点双连通分量 v-DCC 与圆方树

> **赛时先看**
> - **题目信号**：询问割点、点双、任意两点路径是否必须经过某些割点；建圆方树做树上问题。
> - **本质**：无向图按割点分解点双连通分量，常用于经过割点的路径统计。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`bcc`（所有点双的顶点集合）、`cut[u]`（割点标记）、`bcc_cnt`。
> - **警告**：点双分量会共享割点；根节点割点判定是子树数大于 1。


**最小完整示例（先抄这一段就能跑）：**

题目：无向图列出所有点双连通分量，并标记割点（后续可建圆方树）。

```cpp
int n = 3;                              // 样例输入，抄题时换成你的输入
int u = 1, v = 2;                       // 样例边起点/终点
VertexBCC vb(n);                // n 为点数，编号 1..n
vb.add_edge(u, v);              // 无向边加一次即可
vb.add_edge(2, 3);              // 样例：边 1-2、2-3
vb.build();                     // 预处理完成
if (vb.cut[u]) cout << "u 是割点\n";
for (auto& c : vb.bcc) { /* c 是一个点双，割点会出现在多个点双里 */ }
```

样例：`n=3`，边 `1-2、2-3`，`cut[2]=1`，`bcc = {{2,1},{2,3}}`。

**传参要求（照这个传不会错）：**

- `VertexBCC(n)` / `init(n_)`：`n` 为点数，编号 `1..n`。
- `add_edge(int u, int v)`：加一条无向边，`u`、`v` 范围 `1..n`。
- `build()`：无返回值；执行后 `bcc`、`cut` 就绪。
- `cut[u]`：`1` 表示 `u` 是割点；`bcc`：每个元素是一个点双（割点会在多个点双中出现）；`bcc_cnt = bcc.size()`。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `build()` -> 完成建树或预处理
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `bcc`：所有点双分量。
- `cut`：割点标记。
- `dfn`：DFS 序时间戳。
- `low`：lowlink / 最早可达时间戳。

```cpp
struct VertexBCC {
    int n, timer = 0, bcc_cnt = 0;
    vector<vector<int>> g, bcc;
    vector<int> dfn, low, cut, st;

    VertexBCC(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        cut.assign(n + 1, 0);
        bcc.clear(); st.clear();
        timer = bcc_cnt = 0;
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void tarjan(int u, int root) {
        dfn[u] = low[u] = ++timer;
        st.push_back(u);
        int child = 0;
        for (int v : g[u]) {
            if (!dfn[v]) {
                child++;
                tarjan(v, root);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u]) {
                    if (u != root || child > 1) cut[u] = 1;
                    vector<int> cur;
                    while (true) {
                        int x = st.back();
                        st.pop_back();
                        cur.push_back(x);
                        if (x == v) break;
                    }
                    cur.push_back(u);
                    bcc.push_back(cur);
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    void build() {
        for (int i = 1; i <= n; i++) {
            if (!dfn[i]) {
                st.clear();
                tarjan(i, i);
                if (g[i].empty()) bcc.push_back({i});
            }
        }
        bcc_cnt = (int)bcc.size();
    }
};
```
#### 仙人掌图找环

> **赛时先看**
> - **题目信号**：题面保证 “each edge appears in at most one cycle”；`m` 可能大于 `n-1` 但结构接近树。
> - **本质**：无向连通图中每条边最多属于一个简单环，提取所有环，后续可建圆方树、做树形 DP 或距离压缩。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n+m+环长总和）`；仙人掌中环长总和 `O(m)`。
> - **维护的量**：`dep`（深度/访问标记）、`parent/parent_edge`（父点与连向父的边 id）、`cycles`（每个环的边 id 列表）。
> - **警告**：无向边要用边编号跳过父边；只在遇到返祖边 `dep[v] < dep[u]` 时收集一次环。


**最小完整示例（先抄这一段就能跑）：**

题目：仙人掌图（每条边至多在一个环中），提取所有环，按环长处理。

```cpp
int n = 3;                              // 样例输入，抄题时换成你的输入
int u = 1, v = 2;                       // 样例边起点/终点
CactusCycles cc(n);                         // n 为点数，编号 1..n
cc.add_edge(u, v);                          // 无向边加一次即可
cc.add_edge(2, 3);                          // 样例：边 1-2、2-3、3-1
cc.add_edge(3, 1);
vector<vector<int>> cyc = cc.find_cycles(); // 每个环是边 id 列表
for (auto& c : cyc) cout << c.size() << '\n'; // 环长（边数）
```

样例：`n=3`，边 `0:1-2、1:2-3、2:3-1`，`find_cycles()` 返回 `{{0,1,2}}`。

**传参要求（照这个传不会错）：**

- 构造 `CactusCycles(n)`：`n` 为点数，编号 `1..n`。
- `add_edge(int u, int v)`：加一条无向边，`u`、`v` 范围 `1..n`；边 id 从 `0` 起自动分配。
- `find_cycles()`：返回 `cycles`，每个元素是一个环的边 id 列表（id 0-based，按环上顺序）。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
**改板时先认这几个量：**

- `g`：邻接表。
- `cycles`：每个环用边编号列表表示。

```cpp
struct CactusCycles {
    struct Edge {
        int to;
        int id;
    };

    int n = 0, edge_count = 0;
    vector<vector<Edge>> g;
    vector<int> dep, parent, parent_edge;
    vector<vector<int>> cycles; // 每个环用边编号列表表示。

    CactusCycles(int n = 0) : n(n), g(n + 1), dep(n + 1), parent(n + 1), parent_edge(n + 1, -1) {}

    void add_edge(int u, int v) {
        int id = edge_count++;
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }

    void dfs(int u, int pe) {
        for (auto [v, id] : g[u]) {
            if (id == pe) continue;
            if (!dep[v]) {
                dep[v] = dep[u] + 1;
                parent[v] = u;
                parent_edge[v] = id;
                dfs(v, id);
            } else if (dep[v] < dep[u]) {
                vector<int> cyc{id};
                int x = u;
                while (x != v) {
                    cyc.push_back(parent_edge[x]);
                    x = parent[x];
                }
                cycles.push_back(cyc);
            }
        }
    }

    vector<vector<int>> find_cycles() {
        cycles.clear();
        for (int s = 1; s <= n; s++) {
            if (!dep[s]) {
                dep[s] = 1;
                dfs(s, -1);
            }
        }
        return cycles;
    }
};
```
#### 2-SAT

> **赛时先看**
> - **题目信号**：每个对象二选一；条件形如“选 A 就必须选 B”“A 和 B 至少一个”。
> - **本质**：布尔变量约束可行性。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`g`（蕴含图）、`comp`（SCC 编号）、`assignment`（求得的 0/1 赋值）。
> - **警告**：`x` 和 `not x` 在同一 SCC 中则无解。
> - **约定**：return 2 * x + (val ? 0 : 1); // x 是 0-based 下标


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个布尔变量，条件形如“`a` 为真则 `b` 为真”“`a` 或 `b` 至少一个”，求一组可行赋值。

```cpp
int n = 1;                              // 样例输入，抄题时换成你的输入
int a = 0, b = 0;                       // 样例变量（0-based 下标）
TwoSAT ts(n);                       // n 个变量，下标 0..n-1
ts.imply(a, true, b, false);        // a=true => b=false
ts.add_or(a, false, b, true);       // (a=false) 或 (b=true)，即 a 假则 b 真
vector<int> assign;
if (ts.solve(assign)) for (int x : assign) cout << x; // 输出 0/1 赋值
else cout << "无解\n";
```

样例：`n=1`，`add_or(0,true,0,true)`（即 `x∨x`），输出 `1`。

**传参要求（照这个传不会错）：**

- `TwoSAT(n)` / `init(n_)`：`n` 为变量个数；变量 `x` 用 0-based 下标 `0..n-1`。
- `id(int x, bool val)`：`x` 0-based；`val=true` 得 `2x`，`val=false` 得 `2x+1`。
- `imply(a, va, b, vb)`：加蕴含边 `(a=va) => (b=vb)`。
- `add_or(a, va, b, vb)`：加两个蕴含边，表示 `(a=va) 或 (b=vb)` 至少一个成立。
- `solve(assignment)`：有解返回 `true` 并写入 `assignment[x]`（0/1）；无解返回 `false`。

**API / 入口函数（赛时只认这里列的名字）：**

- `init(int n_)` -> 初始化/清空结构
- `solve(vector<int>& assignment)` -> 执行主算法并返回答案
**改板时先认这几个量：**

- `g`：邻接表。
- `dfn`：DFS 序时间戳。
- `low`：lowlink / 最早可达时间戳。

```cpp
struct TwoSAT {
    int n;
    vector<vector<int>> g;
    vector<int> dfn, low, comp, st, in_st;
    int timer = 0, comp_cnt = 0;

    TwoSAT(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(2 * n, {});
        dfn.assign(2 * n, 0);
        low.assign(2 * n, 0);
        comp.assign(2 * n, 0);
        in_st.assign(2 * n, 0);
        st.clear();
        timer = comp_cnt = 0;
    }

    int id(int x, bool val) const {
        return 2 * x + (val ? 0 : 1); // x 是 0-based 下标。
    }

    void imply(int a, bool va, int b, bool vb) {
        g[id(a, va)].push_back(id(b, vb));
    }

    void add_or(int a, bool va, int b, bool vb) {
        imply(a, !va, b, vb);
        imply(b, !vb, a, va);
    }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        st.push_back(u);
        in_st[u] = 1;
        for (int v : g[u]) {
            if (!dfn[v]) dfs(v), low[u] = min(low[u], low[v]);
            else if (in_st[v]) low[u] = min(low[u], dfn[v]);
        }
        if (low[u] == dfn[u]) {
            comp_cnt++;
            while (true) {
                int x = st.back();
                st.pop_back();
                in_st[x] = 0;
                comp[x] = comp_cnt;
                if (x == u) break;
            }
        }
    }

    bool solve(vector<int>& assignment) {
        for (int i = 0; i < 2 * n; ++i) if (!dfn[i]) dfs(i);
        assignment.assign(n, 0);
        for (int i = 0; i < n; ++i) {
            if (comp[2 * i] == comp[2 * i + 1]) return false;
            assignment[i] = comp[2 * i] > comp[2 * i + 1];
        }
        return true;
    }
};
```
#### 欧拉路径/回路：Hierholzer

> **赛时先看**
> - **题目信号**：题面要求“一笔画”“每条边恰好走一次”。
> - **本质**：输出经过每条边一次的路径。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`used[id]`（边是否已走）、`path`（走出的顶点序列，倒序存储）。
> - **警告**：本模板仅实现无向图版本；有向图请自行改边表。无向图要标记边 id。


**最小完整示例（先抄这一段就能跑）：**

题目：无向图求一笔画回路（每条边恰好走一次），输出顶点顺序。

```cpp
int n = 3;                              // 样例输入，抄题时换成你的输入
int u = 1, v = 3;                       // 样例边起点/终点
EulerUndirected eu(n);                  // n 为点数，编号 1..n
eu.add_edge(u, v);                      // 无向边加一次即可
eu.add_edge(1, 2);                      // 样例：三角形 1-2-3-1
eu.add_edge(2, 3);
eu.dfs(1);                              // 从起点开始走（先自行验证度数条件）
reverse(eu.path.begin(), eu.path.end()); // path 是倒序，反转后输出
for (int x : eu.path) cout << x << ' ';
```

样例：三角形 `1-2-3-1`，输出 `1 2 3 1`。

**传参要求（照这个传不会错）：**

- `EulerUndirected(n)` / `init(n_)`：`n` 为点数，编号 `1..n`。
- `add_edge(int u, int v)`：加一条无向边，`u`、`v` 范围 `1..n`；边 id 自动分配。
- `dfs(int start)`：从 `start` 开始；调用前需自行保证存在欧拉回路/路径（度数条件）。
- `path`：顶点序列，**倒序**存储，输出前要 `reverse`；无解时不会走完全部边，需自行判长度。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构

```cpp
struct EulerUndirected {
    int n, edge_id = 0;
    vector<vector<pair<int, int>>> g;
    vector<int> used, path;

    EulerUndirected(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        used.clear();
        path.clear();
        edge_id = 0;
    }

    void add_edge(int u, int v) {
        g[u].push_back({v, edge_id});
        g[v].push_back({u, edge_id});
        used.push_back(0);
        edge_id++;
    }

    void dfs(int u) {
        while (!g[u].empty()) {
            auto [v, id] = g[u].back();
            g[u].pop_back();
            if (used[id]) continue;
            used[id] = 1;
            dfs(v);
        }
        path.push_back(u);
    }
};
```
#### bitset 传递闭包

> **赛时先看**
> - **题目信号**：`n <= 3000/5000`，问很多可达性。
> - **本质**：有向图可达性闭包，比 Floyd 快很多。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n^3 / word_size)`。
> - **维护的量**：`reach[u]`（bitset：`u` 能到达的所有点）。
> - **警告**：先按拓扑序/编号逆序处理 DAG；一般图可先 SCC 缩点。


**最小完整示例（先抄这一段就能跑）：**

题目：DAG 上问所有点对可达性（`u` 能否到达 `v`），多次查询。

```cpp
int n = 3;                              // 样例输入，抄题时换成你的输入
vector<vector<int>> dag(n + 1);         // 1-based 邻接表
dag[1].push_back(2);                    // 样例边：1->2、2->3
dag[2].push_back(3);
vector<int> topo = {1, 2, 3};           // 样例拓扑序
int u = 1, v = 3;                       // 样例查询：1 可达 3
auto reach = dag_transitive_closure<5005>(n, dag, topo); // MAXN 需 >= n+1
if (reach[u][v]) cout << "u 可到达 v\n";
```

样例：`n=3`，边 `1->2、2->3`，`reach[1]={1,2,3}`、`reach[2]={2,3}`、`reach[3]={3}`。

**传参要求（照这个传不会错）：**

- `MAXN`（模板参数）：bitset 长度，必须 `>= n+1`（如 5005）。
- `n`：顶点数，编号 `1..n`。
- `dag`：邻接表，`dag[u]` 存 `u` 的后继 `v`，范围 `1..n`。
- `topo`：长度 `n` 的合法拓扑序（含全部顶点）；一般图要先 SCC 缩点成 DAG 再传。
- 返回值 `reach`：`reach[u]` 是 `bitset<MAXN>`，`reach[u][v]=1` 表示 `u` 可达 `v`。


```cpp
template <int MAXN>
vector<bitset<MAXN>> dag_transitive_closure(int n, const vector<vector<int>>& dag, const vector<int>& topo) {
    vector<bitset<MAXN>> reach(n + 1);
    for (int idx = n - 1; idx >= 0; --idx) {
        int u = topo[idx];
        reach[u][u] = 1;
        for (int v : dag[u]) reach[u] |= reach[v];
    }
    return reach;
}
```
#### 最大团：Bron-Kerbosch bitset

> **赛时先看**
> - **题目信号**：`n <= 60/100`，问最大互相相连集合。
> - **本质**：小图最大团/最大独立集。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：指数级，但剪枝强。
> - **维护的量**：`adj_clique[i]`（点 `i` 的邻接 bitset，0-indexed）、`best_clique`（全局答案：当前最大团大小）。
> - **警告**：最大独立集可在补图上求最大团；n 必须 <= MAXC，更大请改大 MAXC。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点无向图，求最大团大小（两两相连的最大点集）。

```cpp
int n, m;
cin >> n >> m;
for (int i = 0; i < n; ++i) adj_clique[i].reset();   // 1. 邻接 bitset，下标 0..n-1
for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    adj_clique[u][v] = adj_clique[v][u] = 1;
}
best_clique = 0;
bitset<MAXC> P;                                     // 2. 候选集：全部点
for (int i = 0; i < n; ++i) P[i] = 1;
bron_kerbosch(bitset<MAXC>(), P, n);                // 3. R 空集、P 全选
cout << best_clique << '\n';                        // 4. 读全局答案
```

样例：`n=4`，边 `(0,1)(1,2)(2,3)(0,2)` -> 输出 `3`（最大团 `{0,1,2}`）。

**传参要求（照这个传不会错）：**

- `MAXC`：顶点数上限常量，必须 `n <= MAXC`（默认 100，更大就改大它）。
- `adj_clique[i]`：点 `i` 的邻接 bitset（`i` 属于 `[0, n)`），先建好再调用，函数只读它。
- `bron_kerbosch(R, P, n)`：`R` 当前已选点（初始传空集 `bitset<MAXC>()`）、`P` 候选点（初始全部 `1`）、`n` 顶点数；递归过程中会修改局部的 `R/P`，按值传递即可。
- 返回值：无；答案写在全局 `best_clique`，调用完直接输出。


```cpp
const int MAXC = 100;
int best_clique;
bitset<MAXC> adj_clique[MAXC];

void bron_kerbosch(bitset<MAXC> R, bitset<MAXC> P, int n) {
    if (P.none()) {
        best_clique = max(best_clique, (int)R.count());
        return;
    }
    if ((int)R.count() + (int)P.count() <= best_clique) return;
    int pivot = -1, mx = -1;
    for (int i = 0; i < n; ++i) if (P[i]) {
        int c = (int)(P & adj_clique[i]).count();
        if (c > mx) mx = c, pivot = i;
    }
    bitset<MAXC> cand = P & ~adj_clique[pivot];
    for (int v = 0; v < n; ++v) if (cand[v]) {
        R[v] = 1;
        bron_kerbosch(R, P & adj_clique[v], n);
        R[v] = 0;
        P[v] = 0;
        if ((int)R.count() + (int)P.count() <= best_clique) return;
    }
}
```
#### 三元环计数：按度定向枚举

> **赛时先看**
> - **题目信号**：题面问三角形、三个点两两相连、长度为 3 的环；`m` 较大，不能 `O(n^3)`。
> - **本质**：统计简单无向图中的三元环数量，也可以在三角形上累加权值贡献。
> - **接法**：先按 `(degree, id)` 从小到大给每条边定向；枚举 `u -> v -> w`，检查 `u -> w` 是否存在。每个三角形只会被最小的那个点统计一次。
> - **复杂度判定**：按度定向后 `O(m sqrt m)` 级别，实际常数较小。
> - **维护的量**：`degree[u]`（原图度数，定向比较依据）；`g[u]`（按 `(degree, id)` 定向后的有向邻接表）；`mark`（标记当前枚举点的邻居）；`answer`（三元环计数）。
> - **警告**：重边和自环会破坏计数，读入时应去重或确认题目是简单图；定向规则必须全局一致，否则会重复或漏算。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 条无向边，统计三元环个数。

```cpp
int n = 3;                              // 样例输入，抄题时换成你的输入
int m = 3;                              // 样例边数
vector<pair<int, int>> edges;
for (int i = 0; i < m; ++i) {
    int u, v; cin >> u >> v;
    edges.push_back({u, v});           // 顶点编号 1..n
}
i64 ans = count_triangles(n, edges);   // 无向图三元环个数
cout << ans << '\n';
```

样例：边 `1-2、2-3、1-3` -> `ans = 1`。

**传参要求（照这个传不会错）：**

- `n`：顶点数，编号 `1..n`。
- `edges`：每条无向边一个 `pair<int,int>`，编号 `1..n`；按值传参，原数组不会被改。
- 返回值：`i64` 三元环个数。
- 注意：重边会被重复计数，读入前去重或确认是简单图；自环代码自动跳过。


```cpp
i64 count_triangles(int n, vector<pair<int, int>> edges) {
    vector<int> degree(n + 1);
    for (auto [u, v] : edges) {
        if (u == v) continue; // 简单图题一般没有自环，这里防一下。
        ++degree[u];
        ++degree[v];
    }

    vector<vector<int>> g(n + 1);
    auto less_order = [&](int a, int b) {
        if (degree[a] != degree[b]) return degree[a] < degree[b];
        return a < b;
    };

    for (auto [u, v] : edges) {
        if (u == v) continue;
        if (less_order(v, u)) swap(u, v);
        g[u].push_back(v); // 只保留从低优先级到高优先级的有向边。
    }

    vector<int> mark(n + 1, 0);
    i64 answer = 0;
    for (int u = 1; u <= n; ++u) {
        for (int v : g[u]) mark[v] = 1;
        for (int v : g[u]) {
            for (int w : g[v]) {
                if (mark[w]) ++answer;
            }
        }
        for (int v : g[u]) mark[v] = 0;
    }
    return answer;
}
```

### 08 网络流、匹配与割建模

二分图、一般图匹配、稳定匹配、最大流、费用流、上下界流、闭合图和全局/点对最小割集中放在本章。
#### 二分图染色判定

> **赛时先看**
> - **题目信号**：题面问“能否分给 A/B 两方”“能否染成两种颜色”“同组内任意两点不能满足某种冲突关系”；一旦两个对象不能放同一组，就在它们之间连一条无向边。
> - **本质**：判断一堆对象能否分成两组，使所有“不能同组”的冲突对都被分到不同组。
> - **接法**：先把题面条件翻译成“哪些点不能同组”。例如“同一方距离必须大于 `d`”，那么距离 `<= d` 的两点就是冲突边。建好冲突图后调用 `is_bipartite(n, g, color)`。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`color[u]`：点 `u` 的分组（`0/1`），`-1` 表示还没处理。
> - **警告**：图可能不连通，要从每个未染色点开始 BFS/DFS；`color[u]` 只是第 `u` 个对象被分到第 `0/1` 组，不代表输入里天然的左右部；冲突边必须双向加入。


参数怎么传：

- `n`：对象数量，默认对象编号为 `1..n`。
- `g`：冲突图邻接表，`g[u]` 里放所有“不能和 `u` 同组”的对象编号 `v`。如果 `u` 和 `v` 冲突，要同时写 `g[u].push_back(v)` 和 `g[v].push_back(u)`。
- `color`：输出数组。函数会把它改成长度 `n+1`，`color[u]=0/1` 表示对象 `u` 分到哪一组，`-1` 表示还没处理。
- 返回值：`true` 表示可以分成两组；`false` 表示某条边两端被迫同色，也就是出现了奇环，无法满足。

```cpp
bool is_bipartite(int n, const vector<vector<int>>& g, vector<int>& color) {
    // color[u] 的含义：
    // -1：点 u 还没有分组；
    // 0：点 u 分到第一组；
    // 1：点 u 分到第二组。
    // 说明：
    // 注意：这两个组的名字随便叫，可以对应题面里的“武松/施恩”“黑/白”“A/B”。
    color.assign(n + 1, -1);

    // 图可能不连通，所以不能只从 1 号点 BFS。
    // 每遇到一个还没染色的点，就把它当成一个新连通块的起点。
    for (int s = 1; s <= n; ++s) {
        if (color[s] != -1) continue;

        queue<int> q;
        color[s] = 0; // 新连通块第一个点放哪组都可以。
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            // g[u] 存的是所有和 u 有冲突、不能同组的点。
            // 所以每条边 u-v 都要求 color[v] = color[u] ^ 1。
            for (int v : g[u]) {
                if (color[v] == -1) {
                    // v 还没分组，就把它放到 u 的另一组。
                    color[v] = color[u] ^ 1;
                    q.push(v);
                } else if (color[v] == color[u]) {
                    // v 已经分组，但它和 u 同组；而 u-v 是冲突边。
                    // 说明条件互相矛盾，不存在合法分法。
                    return false;
                }
            }
        }
    }
    return true;
}
```

典型建图：如果题面说“同组内任意两点距离必须大于 `d`”，那么距离 `<= d` 的两点不能同组，要连冲突边。

```cpp
struct Point {
    i64 x, y;
};

bool can_split_points_by_distance(const vector<Point>& p, i64 d, vector<int>& color) {
    int n = (int)p.size();
    vector<vector<int>> g(n + 1);
    i128 limit = (i128)d * d;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            i128 dx = (i128)p[i].x - p[j].x;
            i128 dy = (i128)p[i].y - p[j].y;
            i128 dist2 = dx * dx + dy * dy;

            // 同组要求距离 > d。
            // 因此距离 <= d 的两点是“冲突对”，必须分到不同组。
            if (dist2 <= limit) {
                int u = i + 1;
                int v = j + 1;
                g[u].push_back(v);
                g[v].push_back(u);
            }
        }
    }

    return is_bipartite(n, g, color);
}
```

这类题的完整使用方式：

```cpp
void solve() {
    int n;
    i64 d;
    cin >> n >> d;

    vector<Point> p(n);
    for (auto& point : p) cin >> point.x >> point.y;

    vector<int> color;
    cout << (can_split_points_by_distance(p, d, color) ? "YES" : "NO") << '\n';
}
```

典题：两组分配哨岗；同一方任意两哨岗欧几里得距离必须大于 `d`。先连出所有距离 `<= d` 的冲突边，再判二分图。

出处：码蹄杯 2025 官方题单 `MC0421 分配哨岗`。
#### 二分图最大匹配 Hopcroft-Karp

> **赛时先看**
> - **题目信号**：左集合与右集合配对，求最多配多少对。
> - **本质**：大二分图匹配，每个点最多匹配一次。
> - **接法**：把对象分成左、右两类，能配对就 `add_edge(left_id,right_id)`；调用 `max_matching()` 得到最多配对数。匹配结果在 `ml[left]` 和 `mr[right]` 里，值为 `0` 表示未匹配。若左右不是天然两类，先做二分图染色，不是二分图就不能用这个模板。
> - **复杂度判定**：`O(E sqrt V)`。
> - **维护的量**：`ml[l]`（左点 `l` 配到的右点）、`mr[r]`（右点 `r` 配到的左点，`0`=未匹配）、`dist`（增广路分层）。
> - **警告**：左侧编号 `1..nL`，右侧编号 `1..nR`。


**最小完整示例（先抄这一段就能跑）：**

题目：左右各 `nL`、`nR` 个点，`m` 条可配对边，求最多配对数。

```cpp
HopcroftKarp hk(nL, nR);           // 1. 构造：左侧 nL 个、右侧 nR 个点
for (int i = 0; i < m; ++i) {
    int l, r;
    cin >> l >> r;
    hk.add_edge(l, r);             // 2. 加边：左点 l 可与右点 r 配对
}
cout << hk.max_matching() << '\n'; // 3. 返回最大匹配数
```

样例：`nL=3, nR=3`，边 `(1,1)(1,2)(2,2)(3,3)` -> 输出 `3`。

**传参要求（照这个传不会错）：**

- `init(L, R)` / 构造：`L` 左部点数、`R` 右部点数；左右都从 `1` 编号（`0` 是“未匹配”空位）。
- `add_edge(l, r)`：`l` 属于 `[1, nL]`，`r` 属于 `[1, nR]`；表示左点 `l` 可与右点 `r` 配对，加重复边无害。
- `max_matching()`：返回 `int` 最大匹配数；跑完后 `ml[l]` 是左点 `l` 配到的右点、`mr[r]` 是右点 `r` 配到的左点，值为 `0` 表示未匹配。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int l, int r)` -> 加入一条边
- `init(int L, int R)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
struct HopcroftKarp {
    int nL, nR;
    vector<vector<int>> g;
    vector<int> dist, ml, mr;

    HopcroftKarp(int L = 0, int R = 0) { init(L, R); }

    void init(int L, int R) {
        nL = L;
        nR = R;
        g.assign(nL + 1, {});
        ml.assign(nL + 1, 0);
        mr.assign(nR + 1, 0);
        dist.resize(nL + 1);
    }

    void add_edge(int l, int r) {
        g[l].push_back(r);
    }

    bool bfs() {
        queue<int> q;
        for (int i = 1; i <= nL; ++i) {
            if (!ml[i]) dist[i] = 0, q.push(i);
            else dist[i] = -1;
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                int u2 = mr[v];
                if (!u2) found = true;
                else if (dist[u2] == -1) {
                    dist[u2] = dist[u] + 1;
                    q.push(u2);
                }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : g[u]) {
            int u2 = mr[v];
            if (!u2 || (dist[u2] == dist[u] + 1 && dfs(u2))) {
                ml[u] = v;
                mr[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

    int max_matching() {
        int ans = 0;
        while (bfs()) {
            for (int i = 1; i <= nL; ++i) {
                if (!ml[i] && dfs(i)) ans++;
            }
        }
        return ans;
    }
};
```
#### 二分图最小点覆盖

> **赛时先看**
> - **题目信号**：二分图 + 覆盖所有边 + 最少点。根据 Konig 定理，大小等于最大匹配。
> - **本质**：在二分图中选最少点覆盖所有边。
> - **复杂度判定**：匹配后 `O(n+m)`。
> - **维护的量**：`visL/visR`（交替路访问标记）、`coverL/coverR`（选入最小点覆盖的左右点编号）。
> - **警告**：从左侧未匹配点出发，沿“非匹配边左到右、匹配边右到左”走；答案是“未访问左点 + 已访问右点”。必须先运行第 66 节 Hopcroft-Karp 得到 ml/mr 再调用。


**最小完整示例（先抄这一段就能跑）：**

题目：二分图已跑完 Hopcroft-Karp（得到 `ml/mr`），求最小点覆盖（覆盖点数 = 最大匹配数）。

依赖：08 章 HopcroftKarp struct，抄板时一起抄上。

```cpp
int nL = 2, nR = 2;                     // 样例输入，抄题时换成你的输入
vector<vector<int>> g(nL + 1);          // 左部邻接表，g[l] 存可达的右点
g[1].push_back(1);                      // 样例边：(1,1)(1,2)(2,1)
g[1].push_back(2);
g[2].push_back(1);
vector<int> ml = {0, 2, 1};             // 样例最大匹配：左1-右2、左2-右1
vector<int> mr = {0, 2, 1};
auto [coverL, coverR] =
    min_vertex_cover_bipartite(nL, nR, g, ml, mr); // 1. 传左右点数、左部邻接表、匹配结果
cout << coverL.size() + coverR.size() << '\n';     // 2. 覆盖点数，等于最大匹配数
for (int x : coverL) cout << x << ' ';             // 3. 左侧被选点（编号 1..nL）
for (int y : coverR) cout << y << ' ';             // 4. 右侧被选点（编号 1..nR）
```

样例：`nL=2, nR=2`，边 `(1,1)(1,2)(2,1)`，最大匹配 2 -> 覆盖点数 `2`。

**传参要求（照这个传不会错）：**

- `nL` / `nR`：左右部点数，左点编号 `1..nL`、右点编号 `1..nR`。
- `g`：左部邻接表 `g[l]` 存左点 `l` 连到的右点 `r`，必须和跑 Hopcroft-Karp 时用同一个图。
- `ml` / `mr`：Hopcroft-Karp 跑完的匹配数组，长度 `nL+1` / `nR+1`，值 `0` 表示未匹配。
- 返回值：`pair<vector<int>, vector<int>>`：`first` 是左侧选入覆盖的点（`coverL`）、`second` 是右侧选入覆盖的点（`coverR`），两边加起来是一组最小点覆盖。


```cpp
// 已有二分图 g[1..nL] -> right 1..nR，以及最大匹配 ml, mr。
pair<vector<int>, vector<int>> min_vertex_cover_bipartite(
    int nL, int nR,
    const vector<vector<int>>& g,
    const vector<int>& ml,
    const vector<int>& mr
) {
    vector<int> visL(nL + 1, 0), visR(nR + 1, 0);
    queue<int> q;
    for (int i = 1; i <= nL; ++i) {
        if (!ml[i]) {
            visL[i] = 1;
            q.push(i);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (ml[u] == v) continue; // 只沿非匹配边从左部走到右部。
            if (visR[v]) continue;
            visR[v] = 1;
            if (mr[v] && !visL[mr[v]]) {
                visL[mr[v]] = 1;
                q.push(mr[v]);
            }
        }
    }
    vector<int> coverL, coverR;
    for (int i = 1; i <= nL; ++i) if (!visL[i]) coverL.push_back(i);
    for (int i = 1; i <= nR; ++i) if (visR[i]) coverR.push_back(i);
    return {coverL, coverR};
}
```

#### Gale-Shapley：稳定匹配 / 稳定婚姻

> **赛时先看**
> - **题目信号**：不是求最大匹配或最大权匹配；题面强调志愿、偏好、录取、配对后不能存在一对人更愿意彼此选择；每个人都有候选排序。
> - **本质**：两侧个体各自给出严格偏好，求一个没有“互相都想换对象”的稳定匹配。左侧提出申请时，得到的是对左侧最优的稳定匹配。
> - **接法**：医院-住院医师、学校志愿、双边录取、偏好约束下的稳定配对；SPOJ STABLEMP 是直接模板题。
> - **复杂度判定**：`O(P + nL*nR)`，`P` 是左侧偏好表总长度；这里额外建右侧排名表，空间 `O(nL*nR)`。支持不完整偏好表与两侧人数不同。
> - **维护的量**：`match_left[x]`（左侧 `x` 最终匹配的右侧）、`next[x]`（左侧 `x` 下一次要提亲的位置）、`rank[y][x]`（右侧 `y` 对左侧 `x` 的偏好排名，越小越喜欢）。
> - **警告**：右侧排名越小代表越喜欢；左侧列出而右侧没有列出的对象视为不可接受。想让右侧最优，交换两侧输入后再把答案映回。它解决的是**稳定性**，不是“匹配数最大”或“总权值最大”。


**最小完整示例（先抄这一段就能跑）：**

题目：`nL` 个男生、`nR` 个女生各给偏好排序，求稳定匹配（返回对男生最优的）。

```cpp
int nL, nR;
cin >> nL >> nR;
vector<vector<int>> pref_left(nL), pref_right(nR);   // 1. 下标 0..nL-1 / 0..nR-1
// 2. 读偏好：每人一个 vector，按“更喜欢在前”写入对方编号
auto match = gale_shapley(pref_left, pref_right);    // 3. 返回 match_left
for (int x = 0; x < nL; ++x) cout << match[x] << ' '; // 4. 男生 x 配到的女生，-1 未匹配
```

样例：`pref_left=[[0,1],[1,0]]`、`pref_right=[[1,0],[0,1]]` -> 输出 `0 1`。

**传参要求（照这个传不会错）：**

- `pref_left[x]`：左侧 `x`（下标 `[0, nL)`）的偏好表，按“更喜欢在前”列右侧编号 `[0, nR)`。
- `pref_right[y]`：右侧 `y`（下标 `[0, nR)`）的偏好表，按“更喜欢在前”列左侧编号 `[0, nL)`；没列出的对象视为不可接受，可为空表。
- 返回值：`vector<int>` 即 `match_left[x]`：左侧 `x` 配到的右侧编号，`-1` 表示未匹配；这是对左侧最优的稳定匹配。


使用：`pref_left[x]` 与 `pref_right[y]` 都按“更喜欢在前”列出可接受对象，编号分别为 `[0,nL)` 与 `[0,nR)`。返回 `match_left[x]`，`-1` 表示未匹配。

典题模型：医院-住院医师、学校志愿、双边录取、偏好约束下的稳定配对；SPOJ STABLEMP 是直接模板题。

```cpp
// 返回左侧到右侧的匹配；两侧偏好表可以不完整。
vector<int> gale_shapley(
    const vector<vector<int>>& pref_left,
    const vector<vector<int>>& pref_right
) {
    const int nL = (int)pref_left.size();
    const int nR = (int)pref_right.size();
    const int INF = 1e9;

    // rank[y][x] 越小，说明右侧 y 越喜欢左侧 x。
    vector<vector<int>> rank(nR, vector<int>(nL, INF));
    for (int y = 0; y < nR; ++y) {
        for (int i = 0; i < (int)pref_right[y].size(); ++i) {
            rank[y][pref_right[y][i]] = i;
        }
    }

    vector<int> next(nL), match_left(nL, -1), match_right(nR, -1);
    queue<int> q;
    for (int x = 0; x < nL; ++x) q.push(x);

    auto retry = [&](int x) {
        if (next[x] < (int)pref_left[x].size()) q.push(x);
    };

    while (!q.empty()) {
        int x = q.front(); q.pop();
        if (next[x] == (int)pref_left[x].size()) continue;
        int y = pref_left[x][next[x]++];

        // 右侧 y 没把 x 列为可接受对象，直接拒绝。
        if (rank[y][x] == INF) {
            retry(x);
            continue;
        }
        int old = match_right[y];
        if (old == -1 || rank[y][x] < rank[y][old]) {
            if (old != -1) {
                match_left[old] = -1;
                retry(old);
            }
            match_left[x] = y;
            match_right[y] = x;
        } else {
            retry(x);
        }
    }
    return match_left;
}
```

#### KM：二分图最大权完美匹配

> **赛时先看**
> - **题目信号**：指派问题、每个人分配一个任务、要求总权最大且一一匹配。
> - **本质**：左右各 `n` 个点，求完美匹配最大权。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n^3)`。
> - **维护的量**：`lx/ly`（左右顶标）、`slack`（松弛量）、`match_y[y]`（右部 `y` 配到的左点）、`pre`（交替树前驱）。
> - **警告**：权值可以为负时初始化要小；若求最小权，把权值取反。题目保证有完美匹配时才可用，否则返回 -1。
> - **约定**：w 是 1-indexed 的 n+1 方阵。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 人分 `n` 个任务，`w[i][j]` 是第 `i` 人做第 `j` 事的收益，求总收益最大的完美指派。

```cpp
int n;
cin >> n;
vector<vector<i64>> w(n + 1, vector<i64>(n + 1)); // 1. 1-indexed 的 n+1 方阵
for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j) cin >> w[i][j];  // 2. 读权值
cout << km_max_weight_matching(w) << '\n';        // 3. 返回最大总收益；无完美匹配返回 -1
```

样例：`n=2`，`w[1][1]=5, w[1][2]=1, w[2][1]=2, w[2][2]=6` -> 输出 `11`。

**传参要求（照这个传不会错）：**

- `w`：`(n+1) x (n+1)` 的 `i64` 方阵，**1-indexed**：`w[i][j]` 是左点 `i`（人）配右点 `j`（任务）的权，`i/j` 属于 `[1,n]`，`w[0]` 那一行/列不用。
- 返回值：`i64` 完美匹配的最大总权；若不存在完美匹配返回 `-1`。
- 求最小权：把所有权值取反传入，答案取负即可。


**改板时先认这几个量：**

- `lx`/`ly`：左右顶标。
- `slack`：松弛量。
- `match_y`：右部点的匹配左点。

```cpp
// 题目保证有完美匹配时才可用；否则返回 -1。
i64 km_max_weight_matching(const vector<vector<i64>>& w) {
    int n = (int)w.size() - 1;
    const i64 INF = (1LL << 60);
    vector<i64> lx(n + 1), ly(n + 1), slack(n + 1);
    vector<int> match_y(n + 1), pre(n + 1);
    for (int i = 1; i <= n; i++) {
        lx[i] = w[i][1];
        for (int j = 2; j <= n; j++) lx[i] = max(lx[i], w[i][j]);
    }
    for (int root = 1; root <= n; root++) {
        vector<int> vx(n + 1), vy(n + 1);
        fill(slack.begin(), slack.end(), INF);
        int py = 0;
        match_y[0] = root;
        do {
            vy[py] = 1;
            int x = match_y[py], yy = 0;
            i64 delta = INF;
            for (int y = 1; y <= n; y++) if (!vy[y]) {
                i64 cur = lx[x] + ly[y] - w[x][y];
                if (cur < slack[y]) {
                    slack[y] = cur;
                    pre[y] = py;
                }
                if (slack[y] < delta) {
                    delta = slack[y];
                    yy = y;
                }
            }
            for (int y = 0; y <= n; y++) {
                if (vy[y]) {
                    lx[match_y[y]] -= delta;
                    ly[y] += delta;
                } else {
                    slack[y] -= delta;
                }
            }
            py = yy;
        } while (match_y[py]);
        while (py) {
            int last = pre[py];
            match_y[py] = match_y[last];
            py = last;
        }
    }
    i64 ans = 0;
    for (int y = 1; y <= n; y++) {
        if (match_y[y] == 0) return -1; // 不存在完美匹配。
        ans += w[match_y[y]][y];
    }
    return ans;
}
```
#### 一般图最大匹配 Blossom

> **赛时先看**
> - **题目信号**：图不是二分图，但要求最多匹配边数；`n` 通常几百到几千。
> - **本质**：非二分图最大匹配。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：常见实现 `O(n^3)`。
> - **维护的量**：`match[u]`（`u` 的配对点，`0`=未匹配）、`base[u]`（花基/所在花）、`p[u]`（交替树父亲）、`used`（访问标记）。
> - **警告**：缩花时 LCA 与 blossom 标记最容易写错；点编号从 1 开始。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 条无向边的**非二分图**，求最大匹配边数。

```cpp
Blossom b(n);                    // 1. 构造：顶点编号 1..n
for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    b.add_edge(u, v);            // 2. 加无向边，反向自动存好
}
cout << b.solve() << '\n';       // 3. 返回最大匹配边数
```

样例：`n=4`，边 `(1,2)(2,3)(3,4)(4,1)` -> 输出 `2`。

**传参要求（照这个传不会错）：**

- `init(n_)` / 构造：`n_` 顶点数，顶点编号 `1..n`（`0` 是空位）。
- `add_edge(u, v)`：无向边，`u/v` 属于 `[1, n]`；内部自动存双向，不需要自己加两条。
- `solve()`：返回 `int` 最大匹配边数；跑完后 `match[u]` 是与 `u` 配对的点，`0` 表示未匹配。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
- `lca(int a, int b)` -> 最近公共祖先 返回 `int`。
- `solve()` -> 执行主算法并返回答案
**改板时先认这几个量：**

- `g`：邻接表。
- `match`：匹配关系。
- `base`：花基/所在花。
- `p`：交替树父亲。

```cpp
struct Blossom {
    int n;
    vector<vector<int>> g;
    vector<int> match, p, base, q;
    vector<int> used, blossom;

    Blossom(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        match.assign(n + 1, 0);
        p.resize(n + 1); base.resize(n + 1); q.resize(n + 1);
        used.resize(n + 1); blossom.resize(n + 1);
    }
    void add_edge(int u, int v) { g[u].push_back(v); g[v].push_back(u); }

    int lca(int a, int b) {
        vector<int> used_lca(n + 1, 0);
        while (true) {
            a = base[a];
            used_lca[a] = 1;
            if (!match[a]) break;
            a = p[match[a]];
        }
        while (true) {
            b = base[b];
            if (used_lca[b]) return b;
            b = p[match[b]];
        }
    }

    void mark_path(int v, int b, int children) {
        while (base[v] != b) {
            blossom[base[v]] = blossom[base[match[v]]] = 1;
            p[v] = children;
            children = match[v];
            v = p[match[v]];
        }
    }

    int find_path(int root) {
        fill(used.begin(), used.end(), 0);
        fill(p.begin(), p.end(), 0);
        iota(base.begin(), base.end(), 0);
        int qh = 0, qt = 0;
        q[qt++] = root;
        used[root] = 1;
        while (qh < qt) {
            int v = q[qh++];
            for (int u : g[v]) {
                if (base[v] == base[u] || match[v] == u) continue;
                if (u == root || (match[u] && p[match[u]])) {
                    int cur = lca(v, u);
                    fill(blossom.begin(), blossom.end(), 0);
                    mark_path(v, cur, u);
                    mark_path(u, cur, v);
                    for (int i = 1; i <= n; i++) {
                        if (blossom[base[i]]) {
                            base[i] = cur;
                            if (!used[i]) {
                                used[i] = 1;
                                q[qt++] = i;
                            }
                        }
                    }
                } else if (!p[u]) {
                    p[u] = v;
                    if (!match[u]) return u;
                    u = match[u];
                    used[u] = 1;
                    q[qt++] = u;
                }
            }
        }
        return 0;
    }

    int solve() {
        int matching = 0;
        for (int i = 1; i <= n; i++) if (!match[i]) {
            int v = find_path(i);
            if (!v) continue;
            matching++;
            while (v) {
                int pv = p[v], nv = match[pv];
                match[v] = pv;
                match[pv] = v;
                v = nv;
            }
        }
        return matching;
    }
};
```
#### Dinic 最大流

> **赛时先看**
> - **题目信号**：源点 `s` 到汇点 `t`、边带容量，问"最多能运/选/匹配多少"；或最小割类题（最大流 = 最小割）；或二分图匹配（左部连源、右部连汇、匹配边容量 1）。
> - **本质**：在残量网络上反复做"BFS 分层 + DFS 找阻塞流"：每次沿 `level+1` 的残量边一次推多条增广路，直到残量网络够不到汇点；当前弧 `it` 跳过本轮已推不动的边，保证每层只扫一遍。
> - **复杂度判定**：理论 `O(V^2 E)`，实际在比赛图（单位容量、分层少）上远快于理论界；`n,m` 到 1e5 的常规建模放心用；只有稠密大图 + 高容量分层多且常数卡紧时才换 HLPP。
> - **维护的量**：残量网络 `g`（每条边带剩余容量 `cap`，反向边初始 0）、`level`（BFS 分层）、`it`（当前弧指针）。
> - **接法**：先确定源点 `s` 和汇点 `t`，`Dinic dinic(n)`，每条限制写成一条有容量的边 `add_edge(u,v,cap)`，所有边加完后 `max_flow(s,t)` 返回答案。如果题目是二分图匹配，也可以左边连源、右边连汇、匹配边容量为 `1`，但专门的 Hopcroft-Karp 更短。求最小割时跑完最大流后，从源点沿残量边能到的点集就是割的一侧。
> - **警告**：`add_edge` 自动建残量反边，**不要自己再手动加 0 容量反边**；无向容量边按题意加两条有向边；容量建议用 `i64`；`max_flow` 会修改残量网络，不能对同一对象换源汇重跑。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 条有向边（每条容量 `c`），求 `s` 到 `t` 的最大流。

```cpp
Dinic dinic(n);                       // 1. 结构体定义：Dinic(点数 n)
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 c;
    cin >> u >> v >> c;
    dinic.add_edge(u, v, c);          // 2. 加边：正向容量 c；反向 0 容量自动建好
}
cout << dinic.max_flow(s, t) << '\n';  // 3. 调用：返回最大流
```

样例：`1->2(3), 2->3(2), 1->3(2)`；`s=1, t=3` -> 输出 `4`。

**传参要求（照这个传不会错）：**

- `Dinic(n)`：构造；点编号 `1..n`（`Dinic` 用 `n+1` 个槽，从 1 用）。
- `add_edge(u, v, cap)`：加一条容量 `cap` 的有向边；**反向边自动建好，别再手动加**。
- `max_flow(s, t)`：返回 `i64` 最大流；会修改残量网络，不能换源汇重跑同一对象。
- 无向容量边：`add_edge(u,v,c); add_edge(v,u,c);` 两条。
- 最小割：跑完最大流后从 `s` 沿残量边可达的点集就是割的一侧；最大流值 = 最小割值。


**不会用就照抄：**

```cpp
Dinic dinic(n);
dinic.add_edge(u, v, cap);   // 有向容量边 u -> v
// ... 所有边加完
cout << dinic.max_flow(s, t) << '\n';
```

- `add_edge` 会在内部创建残量反边；**不要自己再手动加一条 0 容量反边**，除非模板明确要求。
- 无向容量边要按题意加两条有向容量边。


**API / 入口函数（赛时只认这里列的名字）：**

- `Dinic flow(n)` -> 初始化 1..n 网络。
- `flow.add_edge(u,v,cap)` -> 加入有向容量边，同时自动建残量反边。
- `flow.max_flow(s,t)` -> 所有边加完后求最大流；会修改残量网络。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `struct Dinic`（含 `init`/`add_edge`/`bfs`/`dfs`/`max_flow`）。
2. 构造：`Dinic dinic(n);`，`n` 是点编号上限。
3. 加边：每条有向容量边 `dinic.add_edge(u, v, cap);`，内部自动补残量反边。
4. 调用：`i64 ans = dinic.max_flow(s, t);`，`s/t` 从题面取。
5. 取结果：`ans` 即最大流；要最小割就再从 `s` 沿残量边跑一遍可达性。

**改造点（按题目改这几处）：**
- 0/1-indexed：模板数组大小 n+1，编号 0..n 都可用，只要最大编号 ≤ n。
- 容量类型：模板默认 `i64`；容量小也别改成 int，防相加溢出。
- 拆点建模：点有容量（每个点最多经过一次）时把点 `u` 拆成入点 `u` 与出点 `u+n`，中间连容量 = 点容量的边，原图边从出点连向入点。
- 二分图匹配：左部连源 `s`、右部连汇 `t`、匹配边容量 1，`max_flow(s,t)` 即最大匹配数。
- 多组数据：每组重新 `Dinic dinic(n)` 或先 `init(n)` 清空。

**核心逻辑（改代码时别破坏）：**

- BFS 建只沿残量正边的分层图；DFS 只走 `level+1` 的边。
- `it` 是当前弧优化：一条已经证明确实推不动的边，本轮 BFS 不再重试。
**改板时先认这几个量：**

- `g`：邻接表。
- `it`：Dinic 当前弧位置。

```cpp
// 维护的量：g = 残量网络（每条边带 to/rev/cap，cap 是剩余容量）；level = BFS 分层；it = 当前弧指针。
// 不变量：正向边与反向边容量互补增减（反向边初始 0），残量网络守恒。
struct Dinic {
    struct Edge {
        int to, rev;
        i64 cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    Dinic(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        level.resize(n + 1);
        it.resize(n + 1);
    }

    void add_edge(int u, int v, i64 cap) {
        Edge a{v, (int)g[v].size(), cap};
        Edge b{u, (int)g[u].size(), 0}; // 反向残量边初始 0，供撤销流量
        g[u].push_back(a);
        g[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& e : g[u]) {
                if (e.cap > 0 && level[e.to] == -1) { // 只沿残量 > 0 的边分层
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1; // 汇点不可达即无增广路，最大流结束
    }

    i64 dfs(int u, int t, i64 f) {
        if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); ++i) { // 当前弧：本轮推不动的边不再重试
            Edge& e = g[u][i];
            if (e.cap <= 0 || level[e.to] != level[u] + 1) continue;
            i64 ret = dfs(e.to, t, min(f, e.cap));
            if (ret) {
                e.cap -= ret;
                g[e.to][e.rev].cap += ret; // 正向减、反向加，残量守恒
                return ret;
            }
        }
        return 0;
    }

    i64 max_flow(int s, int t) {
        i64 flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0); // 每轮 BFS 后重置当前弧
            while (i64 f = dfs(s, t, (1LL << 62))) flow += f;
        }
        return flow;
    }
};
```
#### HLPP：最高标号预流推进最大流

> **赛时先看**
> - **题目信号**：标准最大流模型已经建好；`n`、`m` 较大或图较稠密；Dinic 可能卡在高容量/复杂残量网络上；题目不要求最小费用。
> - **本质**：求一般有向图最大流。当 Dinic 在稠密图、容量很大、分层次数很多时卡常，HLPP 往往更稳。
> - **接法**：大规模二分图匹配以外的容量分配、最小割建模、网格网络流、项目选择的最大流子过程；洛谷 P4722“最大流加强版”是直接模板题。
> - **复杂度判定**：最高标号选择 + GAP 优化的理论复杂度为 `O(V^2 sqrt(E))`，实际常数通常很强。空间 `O(V+E)`。
> - **维护的量**：`excess[u]`（点 `u` 的超额流）、`height[u]`（高度标号）、`bucket`（按高度分桶的待推进点）、`cur[u]`（当前弧指针）。
> - **警告**：这是**有向**加边；无向容量边通常需要分别调用 `add_edge(u,v,c)` 和 `add_edge(v,u,c)`。容量和总流量用 `i64`。本实现会修改残量网络，不能直接对同一个对象换源汇再跑一次。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 条有向容量边，求 `s` 到 `t` 的最大流（顶点从 `0` 编号）。

```cpp
HLPP flow(n);                       // 1. 构造：n 个点，编号 0..n-1
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 c;
    cin >> u >> v >> c;
    flow.add_edge(u, v, c);         // 2. 有向容量边；无向边再 add_edge(v,u,c)
}
cout << flow.maxflow(s, t) << '\n'; // 3. 返回 i64 最大流
```

样例：边 `0->1(3), 1->2(2), 0->2(2)`；`s=0, t=2` -> 输出 `4`。

**传参要求（照这个传不会错）：**

- `HLPP(n)`：构造；顶点编号 **`0..n-1`**（和 Dinic 的 1-indexed 不一样，别混用）。
- `add_edge(u, v, cap, rev_cap = 0)`：有向边 `u->v` 容量 `cap`；`rev_cap` 是反向残量初值（默认 `0`，双向容量边可传 `rev_cap = cap` 或加两次）。
- `maxflow(s, t)`：所有边加完后调用，返回 `i64` 最大流；会修改残量网络，之后不能再 `add_edge`，也不能换源汇重跑。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, i64 cap, i64 rev_cap = 0)` -> 加入一条边
- `maxflow(int s, int t)` -> 计算最大流 返回 `i64`。
- `add_flow(Edge& e, i64 f)` -> 沿残量边 e 推 f；若终点第一次获得超额流，就放进对应高度桶。
**改板时先认这几个量：**

- `g`：邻接表。
- `excess`：预流推进中的超额流。
- `cur`：当前弧/当前指针。

使用：`HLPP flow(n); flow.add_edge(u, v, cap);`，最后调用 `flow.maxflow(s, t)`。顶点从 `0` 编号。所有边必须在调用 `maxflow` **之前**加入。

典题模型：大规模二分图匹配以外的容量分配、最小割建模、网格网络流、项目选择的最大流子过程；洛谷 P4722“最大流加强版”是直接模板题。

```cpp
struct HLPP {
    struct Edge {
        int to, rev;
        i64 flow, cap; // flow 是当前流量，cap 是剩余容量
    };

    vector<vector<Edge>> g;
    vector<i64> excess;
    vector<Edge*> cur;
    vector<vector<int>> bucket;
    vector<int> height;

    explicit HLPP(int n)
        : g(n), excess(n), cur(n), bucket(2 * n), height(n) {}

    void add_edge(int u, int v, i64 cap, i64 rev_cap = 0) {
        if (u == v) return;
        g[u].push_back({v, (int)g[v].size(), 0, cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0, rev_cap});
    }

    // 沿残量边 e 推 f；若终点第一次获得超额流，就放进对应高度桶。
    void add_flow(Edge& e, i64 f) {
        Edge& back = g[e.to][e.rev];
        if (excess[e.to] == 0 && f != 0) bucket[height[e.to]].push_back(e.to);
        e.flow += f;
        e.cap -= f;
        excess[e.to] += f;
        back.flow -= f;
        back.cap += f;
        excess[back.to] -= f;
    }

    i64 maxflow(int s, int t) {
        const int n = (int)g.size();
        height.assign(n, 0);
        excess.assign(n, 0);
        bucket.assign(2 * n, {});

        // 所有边已加完后才保存指针；之后不能再 add_edge。
        for (int i = 0; i < n; ++i) cur[i] = g[i].data();
        vector<int> count(2 * n, 0);
        count[0] = n - 1;
        height[s] = n;
        excess[t] = 1; // 防止汇点进入活动桶

        // 源点所有出边先饱和，形成预流。
        for (Edge& e : g[s]) add_flow(e, e.cap);

        for (int highest = 0;;) {
            while (bucket[highest].empty()) {
                if (highest == 0) return -excess[s];
                --highest;
            }
            int u = bucket[highest].back();
            bucket[highest].pop_back();

            while (excess[u] > 0) {
                Edge* end = g[u].data() + g[u].size();
                if (cur[u] == end) {
                    // 没有可推边：重贴标签为所有残量邻居高度的最小值 + 1。
                    height[u] = 2 * n - 1;
                    for (Edge& e : g[u]) {
                        if (e.cap > 0 && height[u] > height[e.to] + 1) {
                            height[u] = height[e.to] + 1;
                            cur[u] = &e;
                        }
                    }
                    ++count[height[u]];

                    // GAP：某一层空了，夹在它与源点之间的点不可能再到汇点。
                    if (--count[highest] == 0 && highest < n) {
                        for (int v = 0; v < n; ++v) {
                            if (highest < height[v] && height[v] < n) {
                                --count[height[v]];
                                height[v] = n + 1;
                            }
                        }
                    }
                    highest = height[u];
                } else if (cur[u]->cap > 0 &&
                           height[u] == height[cur[u]->to] + 1) {
                    add_flow(*cur[u], min(excess[u], cur[u]->cap));
                } else {
                    ++cur[u];
                }
            }
        }
    }
};
```
#### 最小费用最大流

> **赛时先看**
> - **题目信号**：既有容量限制，又要费用最小或收益最大。
> - **本质**：带费用的匹配、分配、运输问题。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：SPFA 版适合中小图。
> - **维护的量**：`g`（含容量与单位费用的残量网络）、`dist`（SPFA 最短费用距离）、`pv/pe`（最短路前驱点/边）。
> - **警告**：最大收益可以把费用取负；确认是否必须跑满指定流量。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 条有向边（容量 `c`、单位费用 `w`），求 `s` 到 `t` 的最小费用流。

```cpp
MinCostMaxFlow mcmf(n);                       // 1. 构造：顶点编号 1..n
for (int i = 0; i < m; ++i) {
    int u, v, c;
    i64 w;
    cin >> u >> v >> c >> w;
    mcmf.add_edge(u, v, c, w);                // 2. 有向边：容量 c、单位费用 w
}
auto [flow, cost] = mcmf.min_cost_flow(s, t); // 3. {实际流量, 最小总费用}
cout << flow << ' ' << cost << '\n';
```

样例：边 `1->2(2,1), 2->3(2,2), 1->3(1,3)`；`s=1, t=3` -> 输出 `3 9`。

**传参要求（照这个传不会错）：**

- `init(n_)` / 构造：`n_` 顶点数，编号 `1..n`。
- `add_edge(u, v, cap, cost)`：有向边 `u->v`：容量 `cap`（int）、单位费用 `cost`（`i64`，可为负）；反向残量边自动建好，别再手动加。
- `min_cost_flow(s, t, need = INT_MAX)`：`s/t` 属于 `[1, n]`；`need` 是要送满的目标流量（默认不加限制，跑到不可增广为止）。返回 `pair<int, i64>`：`first` 实际流量、`second` 最小总费用。
- 求最大收益：费用取负传入，总费用取负即为最大收益。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, int cap, i64 cost)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

```cpp
struct MinCostMaxFlow {
    struct Edge {
        int to, rev, cap;
        i64 cost;
    };

    int n;
    vector<vector<Edge>> g;

    MinCostMaxFlow(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
    }

    void add_edge(int u, int v, int cap, i64 cost) {
        Edge a{v, (int)g[v].size(), cap, cost};
        Edge b{u, (int)g[u].size(), 0, -cost};
        g[u].push_back(a);
        g[v].push_back(b);
    }

    pair<int, i64> min_cost_flow(int s, int t, int need = INT_MAX) {
        const i64 INF = (1LL << 62);
        int flow = 0;
        i64 cost = 0;
        vector<i64> dist(n + 1);
        vector<int> inq(n + 1), pv(n + 1), pe(n + 1);

        while (flow < need) {
            fill(dist.begin(), dist.end(), INF);
            fill(inq.begin(), inq.end(), 0);
            queue<int> q;
            dist[s] = 0;
            q.push(s);
            inq[s] = 1;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                inq[u] = 0;
                for (int i = 0; i < (int)g[u].size(); ++i) {
                    Edge& e = g[u][i];
                    if (e.cap > 0 && dist[e.to] > dist[u] + e.cost) {
                        dist[e.to] = dist[u] + e.cost;
                        pv[e.to] = u;
                        pe[e.to] = i;
                        if (!inq[e.to]) q.push(e.to), inq[e.to] = 1;
                    }
                }
            }

            if (dist[t] == INF) break;
            int add = need - flow;
            for (int v = t; v != s; v = pv[v]) add = min(add, g[pv[v]][pe[v]].cap);
            for (int v = t; v != s; v = pv[v]) {
                Edge& e = g[pv[v]][pe[v]];
                e.cap -= add;
                g[v][e.rev].cap += add;
            }
            flow += add;
            cost += 1LL * add * dist[t];
        }
        return {flow, cost};
    }
};
```
#### 最小割建模提醒

> **赛时先看**
> - **题目信号**：每个对象二选一；有收益/代价；选 A 不选 B 有惩罚。
> - **本质**：把“选或不选”的代价问题转成最小割。
> - **复杂度判定**：建图后跑最大流。
> - **警告**：最大权闭合子图常用“正权源连点，负权点连汇，依赖边 INF”。


```cpp
// 最大权闭合子图建模：
// 点权 w[i] > 0: S -> i, capacity w[i], total += w[i]
// 点权 w[i] < 0: i -> T, capacity -w[i]
// 依赖：选 u 必须选 v，则 u -> v, capacity INF
// 答案 = 正权总和 - 最小割。
```

#### 最大权闭合图：最小割建模与选点恢复

> **赛时先看**
> - **题目信号**：出现“选 A 必须选 B”“做课程/项目必须先完成前置项”“得到收益但要承担依赖成本”；选择关系是有向蕴含，目标是最大化点权和。
> - **本质**：每个项目/点有正负收益；一旦选择 `u` 就必须同时选择它依赖的 `v`，求合法选择集合的最大总收益，并恢复一组选择方案。
> - **接法**：每门课有收益（可以是负数），选一门课要先选所有先修课。对每条“选 `u` 必选 `v`”调用 `require(u,v)`，对每个点调用 `set_weight`。`solve().first` 是最大净收益，`solve().second` 是一组可直接输出的选课/项目编号。
> - **复杂度判定**：建图 `O(n + d)`，之后一次 Dinic 最大流；`d` 为依赖条数。
> - **维护的量**：`weight[u]`（点权）、`dependency`（“选 `u` 必选 `v`”的依赖边）、`reach`（残量图从源可达的点，即被选中的点）。
> - **警告**：依赖 `u -> v` 应连容量 `INF` 的边 `u -> v`，不是反向；`INF` 必须严格大于所有点权绝对值总和；本实现要求权值绝对值总和小于 `2^62`；代码中的“选中点”是最终残量图从源点可达的原点。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 门课各有收益（可负），选 `u` 必须先选 `v`，求最大净收益和选课方案。

```cpp
MaximumWeightClosure mwc(n);           // 1. 构造：n 个点，编号 0..n-1
for (int i = 0; i < n; ++i) {
    i64 w;
    cin >> w;
    mwc.set_weight(i, w);              // 2. 点权：正=收益，负=成本
}
mwc.require(u, v);                     // 3. 依赖：选 u 必须选 v（0-indexed）
auto [best, chosen] = mwc.solve();     // 4. {最大净收益, 被选点编号}
cout << best << '\n';
```

样例：`n=3`，权 `[5,-2,-3]`，依赖 `1->0`、`2->0` -> 输出 `5`（选点 `{0}`）。

**传参要求（照这个传不会错）：**

- `MaximumWeightClosure(n_)`：`n_` 点数，点编号 **`0..n-1`**。
- `set_weight(u, w)`：点 `u` 的权值 `w`（`i64`，正负均可）。
- `require(u, v)`：依赖边“选 `u` 必须选 `v`”，`u/v` 属于 `[0, n)`；内部建成容量 `INF` 的边 `u -> v`。
- `solve()`：返回 `pair<i64, vector<int>>`：`first` 最大净收益、`second` 是 0-indexed 的被选点编号列表（直接输出即可）。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, i64 cap)` -> 加入一条边
- `max_flow(int s, int t)` -> 计算最大流 返回 `i64`。
- `solve()` -> 返回 {最大总权值， 一组最优选择点集}。
**改板时先认这几个量：**

- `g`：内部 Dinic 残量网络（含反向边）。
- `it`：Dinic 当前弧。
- `dependency`：选择 u 时必须同时选择 v。

```cpp
struct ClosureDinic {
    struct Edge { int to, rev; i64 cap; };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    explicit ClosureDinic(int n_) : n(n_), g(n_), level(n_), it(n_) {}

    void add_edge(int u, int v, i64 cap) {
        Edge a{v, (int)g[v].size(), cap};
        Edge b{u, (int)g[u].size(), 0};
        g[u].push_back(a);
        g[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& e : g[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    i64 dfs(int u, int t, i64 f) {
        if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); ++i) {
            Edge& e = g[u][i];
            if (e.cap == 0 || level[e.to] != level[u] + 1) continue;
            i64 got = dfs(e.to, t, min(f, e.cap));
            if (!got) continue;
            e.cap -= got;
            g[e.to][e.rev].cap += got;
            return got;
        }
        return 0;
    }

    i64 max_flow(int s, int t) {
        i64 ans = 0;
        const i64 INF = (1LL << 62);
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (i64 f = dfs(s, t, INF)) ans += f;
        }
        return ans;
    }

    vector<int> reachable_from_source(int s) const {
        vector<int> vis(n);
        queue<int> q;
        vis[s] = 1;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& e : g[u]) {
                if (e.cap > 0 && !vis[e.to]) vis[e.to] = 1, q.push(e.to);
            }
        }
        return vis;
    }
};

struct MaximumWeightClosure {

    int n;
    vector<i64> weight;
    vector<pair<int, int>> dependency; // 选择 u 时必须同时选择 v。

    explicit MaximumWeightClosure(int n_) : n(n_), weight(n_, 0) {}

    void set_weight(int u, i64 w) { weight[u] = w; }
    void require(int u, int v) { dependency.push_back({u, v}); }

    // 返回 {最大总权值, 一组最优选择点集}。
    pair<i64, vector<int>> solve() const {
        int s = n, t = n + 1;
        ClosureDinic dinic(n + 2);
        i64 positive_sum = 0;
        i128 abs_sum = 0;
        for (int u = 0; u < n; ++u) {
            positive_sum += max(0LL, weight[u]);
            abs_sum += llabs(weight[u]);
        }
        assert(abs_sum < (1LL << 62));
        i64 INF = (i64)abs_sum + 1;
        for (int u = 0; u < n; ++u) {
            if (weight[u] > 0) dinic.add_edge(s, u, weight[u]);
            if (weight[u] < 0) dinic.add_edge(u, t, -weight[u]);
        }
        for (auto [u, v] : dependency) dinic.add_edge(u, v, INF);

        i64 best = positive_sum - dinic.max_flow(s, t);
        vector<int> reach = dinic.reachable_from_source(s), chosen;
        for (int u = 0; u < n; ++u) if (reach[u]) chosen.push_back(u);
        return {best, chosen};
    }
};
```

典题模型：每门课有收益（可以是负数），选一门课要先选所有先修课。对每条“选 `u` 必选 `v`”调用 `require(u,v)`，对每个点调用 `set_weight`。`solve().first` 是最大净收益，`solve().second` 是一组可直接输出的选课/项目编号。
#### 上下界可行流 / 最大流 / 最小流

> **赛时先看**
> - **题目信号**：题面有“每条边至少/至多流多少”；点有供需平衡；要求可行方案。
> - **本质**：每条边有流量下界 `low` 和上界 `cap`，判断是否存在可行流，或求有源汇上下界最大/最小流。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：一次或两次最大流。
> - **维护的量**：`demand[i]`（点 `i` 的需求量：出边下界减、入边下界加）、`original`（所有原边记录 `{u,v,low,cap}`）。
> - **警告**：原边容量改成 `cap-low`；对点记录需求 `d[u]-=low, d[v]+=low`；超级源汇连需求边。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点 `m` 条边，每条流量范围 `[low, cap]`，判断是否存在无源汇可行流。

```cpp
LowerBoundFlowBuilder lb(n);      // 1. 构造：原图点编号 1..n（SS=n+1, TT=n+2）
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 low, cap;
    cin >> u >> v >> low >> cap;
    lb.add_edge(u, v, low, cap);  // 2. 记录边并累计 demand
}
Dinic dinic(n + 2);               // 3. 复用 Dinic
for (const auto& e : lb.original) dinic.add_edge(e.u, e.v, e.cap - e.low);
for (int i = 1; i <= n; ++i) {
    if (lb.demand[i] > 0) dinic.add_edge(lb.S, i, lb.demand[i]);
    if (lb.demand[i] < 0) dinic.add_edge(i, lb.T, -lb.demand[i]);
}
i64 f = dinic.max_flow(lb.S, lb.T); // 4. SS 出边全满 => 存在可行流
```

样例：边 `1->2(2,3)`、`2->1(1,2)` -> 可行；只有 `1->2(3,4)` -> 不可行。

**传参要求（照这个传不会错）：**

- `init(n_)` / 构造：`n_` 原图点数，编号 `1..n`；`S = n+1`、`T = n+2` 是自动预留的超级源/汇。
- `add_edge(u, v, low, cap)`：原图一条边：`u/v` 属于 `[1, n]`，下界 `low`、上界 `cap`（`i64`，`0 <= low <= cap`）；只更新 `demand` 与 `original`，不真正建图。
- 结果：本结构只负责建模；可行性/最大/最小流按代码内注释用 Dinic 补边后跑 `maxflow(S, T)`（无源汇）/ `maxflow(s,t)`（有源汇）得到。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, i64 low, i64 cap)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构

```cpp
// 需要已有 Dinic，点编号 1..n。这里给建模流程。
struct LowerBoundFlowBuilder {
    struct Edge { int u, v; i64 low, cap; };
    int n, S, T;
    vector<i64> demand;
    vector<Edge> original;

    LowerBoundFlowBuilder(int n = 0) { init(n); }
    void init(int n_) {
        n = n_;
        S = n + 1;
        T = n + 2;
        demand.assign(n + 3, 0);
        original.clear();
    }

    void add_edge(int u, int v, i64 low, i64 cap) {
        original.push_back({u, v, low, cap});
        demand[u] -= low;
        demand[v] += low;
        // 示例：Dinic.add_edge(u, v, cap - low);
    }

    // 无源汇可行流：
    // 1. 加所有原边 cap-low。
    // 公式/约定：2. demand[i] > 0: SS -> i, capacity demand[i]。
    // 公式/约定：3. demand[i] < 0: i -> TT, capacity -demand[i]。
    // 4. 跑 maxflow(SS, TT)，所有 SS 出边满流则可行。
    // 说明：
    // 有源汇 s,t 可行流：
    // 在上述基础上额外加 t -> s, INF，再判可行。
    // 说明：
    // 有源汇最大流：
    // 先求可行流，然后删除超级源汇，保留残量网络，跑 maxflow(s,t)。
    // 说明：
    // 有源汇最小流：
    // 先求可行流。若 t -> s 这条边流量为 base，则答案为
    // base - maxflow(t,s)（在删除超级源汇和该辅助边后）。
};
```
#### Gomory-Hu Tree：无向图全点对最小割

> **赛时先看**
> - **题目信号**：无向图；很多次询问不同点对的最小割/最大流；直接对每个询问跑最大流太慢。
> - **本质**：把一个无向带容量图的所有点对最小割压缩成一棵树。任意两点的最小割值，等于树上两点路径边权的最小值。
> - **接法**：给无向通信网络，多次询问两座城市之间最多能同时通过多少容量；或统计最小割至少为某个阈值的点对数。
> - **复杂度判定**：建树需要 `n-1` 次最大流；这里每次用 Dinic，因此总体为 `O(n * MaxFlow)`。建好后下面的朴素查询为 `O(n)`，大量查询可再加 LCA 二进制提升到 `O(log n)`。
> - **维护的量**：`edges`（无向容量边集合）、`tree`（Gomory-Hu 树邻接表，边权为割值）、`parent/cut`（建树中间量）。
> - **警告**：只适用于**无向**图；一条无向容量边要拆成两个相反方向、容量均为 `c` 的有向边。最大流结束后，必须从残量网络中找 `s` 可达侧来更新父亲。图不连通时最小割为 0，本模板查询返回 -1，按题意自行判断。
> - **约定**：图 0-based。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 点无向带权图，`q` 次询问任意两点的最小割。

```cpp
GomoryHu gh(n);                         // 1. 构造：顶点编号 0..n-1
for (int i = 0; i < m; ++i) {
    int u, v;
    i64 c;
    cin >> u >> v >> c;
    gh.add_undirected_edge(u, v, c);    // 2. 无向容量边（内部自动拆两条有向边）
}
gh.build();                             // 3. 建 GH 树：内部跑 n-1 次最大流
cout << gh.mincut_query(s, t) << '\n';  // 4. 查询 s,t 的最小割（0-indexed）
```

样例：`n=4`，边 `(0,1,3)(1,2,2)(2,3,4)(3,0,5)`；`mincut_query(0,2)` -> 输出 `5`。

**传参要求（照这个传不会错）：**

- `GomoryHu(n)`：`n` 顶点数，编号 **`0..n-1`**。
- `add_undirected_edge(u, v, cap)`：无向容量边，`u/v` 属于 `[0, n)`；内部自动拆成 `u->v`、`v->u` 两条容量 `cap` 的边。
- `build()`：所有边加完后调用一次；建树需要 `n-1` 次最大流。
- `mincut_query(s, t)`：返回 `i64`：`s` 与 `t` 的最小割值；`s/t` 属于 `[0, n)`；不连通返回 `-1`。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, i64 cap)` -> 加入一条边
- `build()` -> 完成建树或预处理
- `maxflow(int s, int t)` -> 计算最大流 返回 `i64`。
**改板时先认这几个量：**

- `g`：内部 Dinic 残量网络（含反向边）。
- `it`：Dinic 当前弧。

典题模型：给无向通信网络，多次询问两座城市之间最多能同时通过多少容量；或统计最小割至少为某个阈值的点对数。

```cpp
struct GH_Dinic {
    struct Edge { int to, rev; i64 cap; };
    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    explicit GH_Dinic(int n) : n(n), g(n), level(n), it(n) {}

    void add_edge(int u, int v, i64 cap) {
        Edge a{v, (int)g[v].size(), cap};
        Edge b{u, (int)g[u].size(), 0};
        g[u].push_back(a);
        g[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const Edge& e : g[u]) if (e.cap > 0 && level[e.to] == -1) {
                level[e.to] = level[u] + 1;
                q.push(e.to);
            }
        }
        return level[t] != -1;
    }

    i64 dfs(int u, int t, i64 f) {
        if (u == t) return f;
        for (int& i = it[u]; i < (int)g[u].size(); ++i) {
            Edge& e = g[u][i];
            if (e.cap <= 0 || level[e.to] != level[u] + 1) continue;
            i64 got = dfs(e.to, t, min(f, e.cap));
            if (!got) continue;
            e.cap -= got;
            g[e.to][e.rev].cap += got;
            return got;
        }
        return 0;
    }

    i64 maxflow(int s, int t) {
        i64 ans = 0, pushed;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while ((pushed = dfs(s, t, LLONG_MAX / 4))) ans += pushed;
        }
        return ans;
    }

    vector<int> residual_reachable(int s) const {
        vector<int> vis(n);
        queue<int> q;
        q.push(s);
        vis[s] = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (const Edge& e : g[u]) if (e.cap > 0 && !vis[e.to]) {
                vis[e.to] = 1;
                q.push(e.to);
            }
        }
        return vis;
    }
};

struct GomoryHu {
    struct UEdge { int u, v; i64 cap; };
    int n;
    vector<UEdge> edges;
    vector<vector<pair<int, i64>>> tree;

    explicit GomoryHu(int n) : n(n) {}
    void add_undirected_edge(int u, int v, i64 cap) { edges.push_back({u, v, cap}); }

    void build() {
        vector<int> parent(n, 0);
        vector<i64> cut(n);
        for (int s = 1; s < n; ++s) {
            int t = parent[s];
            GH_Dinic flow(n);
            for (const auto& e : edges) {
                flow.add_edge(e.u, e.v, e.cap);
                flow.add_edge(e.v, e.u, e.cap);
            }
            i64 value = flow.maxflow(s, t);
            vector<int> side = flow.residual_reachable(s);
            for (int i = s + 1; i < n; ++i) {
                if (parent[i] == t && side[i]) parent[i] = s;
            }
            if (side[parent[t]]) {
                parent[s] = parent[t];
                parent[t] = s;
                cut[s] = cut[t];
                cut[t] = value;
            } else {
                cut[s] = value;
            }
        }
        tree.assign(n, {});
        for (int v = 1; v < n; ++v) {
            tree[v].push_back({parent[v], cut[v]});
            tree[parent[v]].push_back({v, cut[v]});
        }
    }

    i64 mincut_query(int s, int t) const {
        vector<int> vis(n);
        queue<pair<int, i64>> q;
        q.push({s, LLONG_MAX / 4});
        vis[s] = 1;
        while (!q.empty()) {
            auto [u, best] = q.front(); q.pop();
            if (u == t) return best;
            for (auto [v, w] : tree[u]) if (!vis[v]) {
                vis[v] = 1;
                q.push({v, min(best, w)});
            }
        }
        return -1;
    }
};
```

