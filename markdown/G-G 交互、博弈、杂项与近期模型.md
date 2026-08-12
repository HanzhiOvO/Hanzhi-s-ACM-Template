## G 交互、博弈、杂项与近期模型

### 15 交互、通信与特殊评测

2019 年以后越来越常见的交互、通信、特殊评测和新奇构造模型放在这里。先看协议和信息量，再看 checker 与构造自检。
#### 2019-2026 XCPC 题源审查清单

> **赛时先看**
> - **题目信号**：题面不是单纯套传统算法，而是出现“输出任意合法方案”“special judge”“construct any”“checker”“通信”“交互”“两份程序”“给出一个满足性质的对象”“评测只检查性质”等描述。
> - **本质**：把 2019 年以后区域赛、EC-Final、World Finals、CCPC 网络赛/分站/总决赛、邀请赛里容易漏进板子的模型集中记录，后续补题时按这个清单反查。
> - **复杂度判定**：这是审查清单，不是单个算法；比赛时按题面关键词跳到下面对应模板。
> - **警告**：不要把“有趣”误当“能在比赛乱搞”。正式赛只使用题面允许的输出自由度；所谓 checker 思维是为了做构造、自检和边界验证，不是攻击评测系统。


- 2019-2026 公开题源优先查 ICPC Archives、ICPC World Finals Past Problems、Codeforces Gym、QOJ/Universal Cup、XCPCIO、VJudge 题单、CCPC 官网和站点公开题解。
- 看到 `any valid output`：先写本地 checker，再写构造；先让 checker 能抓自己的错误。
- 看到交互/通信：先算信息量下界，再看查询次数/通信容量是否暗示二分、分治、编码、哈希摘要或纠错。
- 看到“几何构造 + special judge”：优先考虑标准构型、组合块拼接、低维投影、数值容差和本地验证。
- 看到“题面形式怪，但数据很小”：考虑打表、搜索构造、SAT/2-SAT、最大流可行性、随机化找方案。

参考来源：ICPC Archives、ICPC World Finals Past Problems、XCPCIO、VJudge 2018-2024/2025 题单整理、Codeforces Gym 104022、USACO Guide Interactive and Communication Problems。

#### 特殊评测与 checker 思维：输出任意合法方案

> **赛时先看**
> - **题目信号**：题面说“如果有多种答案输出任意一种”“It can be shown that a solution exists”“special judge”“print any valid arrangement/model/list”。
> - **本质**：处理构造题、几何输出题、方案恢复题和特殊评测题。核心不是猜 checker 漏洞，而是把题面性质翻译成本地检查器，保证自己输出真的满足条件。
> - **复杂度判定**：取决于本地 checker，常见 `O(n^2)` 或 `O(m log n)`；只在本地调试使用，不提交或只保留轻量断言。
> - **警告**：赛场上不要利用未公开系统漏洞；允许利用的是题面给出的自由度。浮点 special judge 要严格按题面容差验证，整数构造要检查范围、重复、连通性、度数、计数。


- 第一步：把输出约束逐条列成 `check_*` 函数。
- 第二步：对样例输出、手造边界、随机小数据跑 checker。
- 第三步：构造程序每输出一个方案前，在本地 `#ifdef LOCAL` 调 checker。
- 第四步：如果 checker 比构造还难，先写暴力枚举小规模和 checker 互相对拍。

#### 本地 checker 骨架：构造题输出自检

> **赛时先看**
> - **题目信号**：构造题 WA 但不知道哪条约束错；题面输出不是唯一答案；样例输出只是一个可行方案。
> - **本质**：为“输出任意合法方案”写本地验证器。可以检查整数、排列、范围、不重复、图边、几何距离等约束。
> - **接法**：把自己的输出保存成字符串或文件，用 `LocalChecker ck(out)` 读取；每读一个值都用 `read_int/read_double`，随后 `require` 检查性质。提交前可以用 `#ifdef LOCAL` 包住。
> - **复杂度判定**：按检查逻辑而定；本骨架读输出文本并提供通用断言。
> - **维护的量**：`ss`（剩余待读的输出流）；`errors`（已收集的违规信息，`finish` 时统一打印）。
> - **警告**：本地 checker 的容差和题面必须一致；下标、范围、重复、输出行数要先检查，再检查高级性质。


**最小完整示例（先抄这一段就能跑）：**

题目：构造题输出 `n` 个数的排列，把程序生成的输出字符串交给 checker 逐条检查。

```cpp
LocalChecker ck(out);                    // 1. 用程序构造出的输出文本初始化
for (int i = 0; i < n; ++i) {            // 2. 逐个读入并核对范围
    ck.read_int(1, n, "p[i]");           //    值必须在 [1, n]
}
ck.require(cnt == n, "数量不对");        // 3. 按题面性质任意加 require
ck.finish();                             // 4. 收尾：查多余 token、汇总报错
```

样例：输出 `3 1 2` 通过；输出 `3 3 2` 报错“排列中元素重复或缺失”。

**传参要求（照这个传不会错）：**

- `LocalChecker ck(out)`：`out` = 待自检的完整输出字符串（包含所有行）。
- `read_int(low, high, name)`：读一个整数；`low/high` = 允许闭区间；`name` 只用于报错文案；缺值或越界会记录错误并返回 `low`。
- `read_double(low, high, name)`：同上，读浮点数。
- `read_permutation(n, low = 1)`：读 `n` 个数，须是 `[low, low+n-1]` 的一个排列，返回 `vector<int>`。
- `require(ok, message)`：`ok` 为 false 时把 `message` 记入错误。
- `finish()`：最后必调；流里还有多余 token 或有任何错误则 `assert(false)` 并打印全部错误。


```cpp
struct LocalChecker {
    stringstream ss;
    vector<string> errors;

    LocalChecker(const string& output) : ss(output) {}

    void require(bool ok, const string& message) {
        if (!ok) errors.push_back(message);
    }

    long long read_int(long long low, long long high, const string& name) {
        long long x;
        if (!(ss >> x)) {
            errors.push_back("缺少整数：" + name);
            return low;
        }
        require(low <= x && x <= high, name + " 超出范围");
        return x;
    }

    double read_double(double low, double high, const string& name) {
        double x;
        if (!(ss >> x)) {
            errors.push_back("缺少浮点数：" + name);
            return low;
        }
        require(low <= x && x <= high, name + " 超出范围");
        return x;
    }

    vector<int> read_permutation(int n, int low = 1) {
        vector<int> p(n), seen(n, 0);
        for (int i = 0; i < n; ++i) {
            p[i] = (int)read_int(low, low + n - 1, "permutation item");
            int id = p[i] - low;
            if (0 <= id && id < n) seen[id]++;
        }
        for (int i = 0; i < n; ++i) require(seen[i] == 1, "排列中元素重复或缺失");
        return p;
    }

    void finish() {
        string extra;
        if (ss >> extra) errors.push_back("输出末尾有多余 token：" + extra);
        if (!errors.empty()) {
            for (auto& e : errors) cerr << "[checker] " << e << '\n';
            assert(false);
        }
    }
};
```
#### 典题记录：2020 ICPC 银川 H Absolute Space

> **赛时先看**
> - **题目信号**：题面说“create any possible model”，输出浮点坐标，checker 只检查距离关系和范围。
> - **本质**：记录一种非常典型的“构造对象 + special judge 检查性质”的题型。题目要求给定 `n <= 10`，输出不超过 100 个三维点，使每个点恰好有 `n` 个距离为 1 的邻点，且任意两点距离至少 `0.01`。
> - **接法**：先把构造出的点扔进 `check_absolute_space(points, n)`；如果是打表构造，每个 `n` 的表都跑一遍；如果是随机/搜索构造，找到方案后固定输出，别把随机留到提交里。
> - **复杂度判定**：本地验证 `O(m^2)`，其中 `m <= 100`。
> - **维护的量**：`p`（构造出的点集，长度 `m`）；`need_degree`（每个点应有的距离 1 邻点数）。
> - **警告**：不要用肉眼看图当证明；必须按题面容差检查“距离在 `(0.999999, 1.000001)` 的邻点数”。浮点输出位数要足够，点间最小距离限制也要检查。


**最小完整示例（先抄这一段就能跑）：**

题目：构造 `<=100` 个三维点（坐标 `[-100,100]`），使每个点恰有 `n` 个距离约 1 的邻点且点距 `>=0.01`。

```cpp
vector<Point3D> points = my_build(n);           // 1. 自己的构造函数产出点集
if (!check_absolute_space(points, n)) {         // 2. 本地校验
    /* 重新构造或换一组表 */
}
cout << points.size() << '\n';                  // 3. 校验通过再按题面格式输出
```

样例：`n=1` 时两个相距 1 的点通过；三点成边长为 1 的三角形时每个点度数为 2 也通过。

**传参要求（照这个传不会错）：**

- `check_absolute_space(p, need_degree)`：`p` = 点集，长度 `m` 须满足 `1 <= m <= 100`；`need_degree` = 每个点应有的距离 1 邻点数。
- 校验内容：坐标均在 `[-100,100]`；任意两点距离 `>= 0.01`；每点邻点数（距离在 `(0.999999, 1.000001)` 内）恰为 `need_degree`。
- 返回值：全部满足返回 `true`，任一违反返回 `false`。
- `dist3(a, b)`：返回两点欧氏距离（已开根号）。


```cpp
struct Point3D {
    double x, y, z;
};

double dist3(Point3D a, Point3D b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

bool check_absolute_space(const vector<Point3D>& p, int need_degree) {
    int m = (int)p.size();
    if (m < 1 || m > 100) return false;
    for (int i = 0; i < m; ++i) {
        if (p[i].x < -100 || p[i].x > 100) return false;
        if (p[i].y < -100 || p[i].y > 100) return false;
        if (p[i].z < -100 || p[i].z > 100) return false;
    }
    for (int i = 0; i < m; ++i) {
        int degree = 0;
        for (int j = 0; j < m; ++j) {
            if (i == j) continue;
            double d = dist3(p[i], p[j]);
            if (d < 0.01) return false;
            if (0.999999 < d && d < 1.000001) degree++;
        }
        if (degree != need_degree) return false;
    }
    return true;
}
```
#### 多数关系构造：McGarvey 投票序列

> **赛时先看**
> - **题目信号**：题目要求输出很多个排列；每条约束形如“`a` 在超过半数排列中排在 `b` 前”；约束之间允许成环。
> - **本质**：给定若干约束 `a` 必须在多数投票中战胜 `b`，构造一组排列作为投票，使每条指定边都满足多数关系。适合“输出若干个排列，使指定 pair 的相对顺序超过一半”的构造题。
> - **接法**：把题面约束读成 `wins.push_back({a,b})`，调用 `mcgarvey_votes(n, wins)`，输出返回的每个排列。如果题面限制票数，先检查 `2*m` 是否不超过限制。
> - **复杂度判定**：`O(mn)` 生成，其中 `n` 是候选数，`m` 是指定胜负关系数。若 `n <= 50, m <= n(n-1)/2`，输出 `2m` 个排列很安全。
> - **维护的量**：`votes`（生成的票序列，每行一个排列）；`wins`（指定胜负的约束对列表）。
> - **警告**：边是有向关系，不要求传递；构造的票数为偶数时，“超过一半”对指定边是 `2` 票优势，对未指定边两个方向各一票、相互抵消。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个候选人，给定 `m` 条“`a` 在多数票中排在 `b` 前”的约束，输出若干排列作为投票。

```cpp
vector<pair<int,int>> wins = {{1, 3}, {3, 2}};   // 1. 收集约束：1 胜 3、3 胜 2
auto votes = mcgarvey_votes(n, wins);            // 2. 生成 2*m 张票
for (auto& v : votes) {                          // 3. 每行输出一个排列
    for (int x : v) cout << x << ' ';
    cout << '\n';
}
```

样例：`n=3`、约束 `(1,3)` 得两张票 `1 3 2`、`2 1 3`，指定边 2:0 胜出。

**传参要求（照这个传不会错）：**

- `mcgarvey_votes(n, wins)`：`n` = 候选人数，编号 `1..n`；`wins` = 约束列表，每个 `(a,b)` 表示 a 须在多数票中排在 b 前，`a/b` 必须在 `[1,n]`。
- 返回值：`vector<vector<int>>`，每行一个排列；总票数 = `2*m`（约束为空时返回一个 `1..n` 的排列）。
- 若题面限票数，先确认 `2*m` 不超限。


```cpp
vector<vector<int>> mcgarvey_votes(int n, const vector<pair<int, int>>& wins) {
    vector<vector<int>> votes;
    for (auto [a, b] : wins) {
        vector<int> rest;
        for (int x = 1; x <= n; ++x) {
            if (x != a && x != b) rest.push_back(x);
        }

        // 第一张票：a 在 b 前，其他人按正序放后面。
        vector<int> first;
        first.push_back(a);
        first.push_back(b);
        for (int x : rest) first.push_back(x);

        // 第二张票：其他人反序放前面，最后仍然保持 a 在 b 前。
        // 这样除 (a,b) 外的相对贡献两两抵消。
        vector<int> second;
        for (int i = (int)rest.size() - 1; i >= 0; --i) second.push_back(rest[i]);
        second.push_back(a);
        second.push_back(b);

        votes.push_back(first);
        votes.push_back(second);
    }
    if (votes.empty()) {
        vector<int> id(n);
        iota(id.begin(), id.end(), 1);
        votes.push_back(id);
    }
    return votes;
}
```
#### 镜面反射网格：状态图 + 0-1 BFS

> **赛时先看**
> - **题目信号**：网格里有 `.`、`#`、`/`、`\`、起点和方向；问能否逃出、最少删除几个镜子、最少改几个格子。
> - **本质**：处理小球/光线在网格中按方向运动，遇到 `/`、`\`、`#` 改变方向，并且可以付出代价删除镜子或改变格子的题。
> - **接法**：把每个状态设为 `(r,c,dir)`；代价 0 的自然反射走队首，代价 1 的“删除镜子后直行”走队尾；最后取所有逃出边界的最小代价。
> - **复杂度判定**：状态数 `4*n*m`，0-1 BFS 为 `O(nm)`。
> - **维护的量**：`dist[r][c][d]`（状态 `(r,c,方向)` 的最短代价，0-1 BFS 的核心表）；`dr/dc`（四个方向的位移）。
> - **警告**：状态必须包含方向；遇到边界通常直接逃出；遇到墙是反弹还是不能走，要按题面调整。若题目要求输出具体操作方案，需要在本模板基础上加前驱恢复。


**最小完整示例（先抄这一段就能跑）：**

题目：`n*m` 网格含 `#`（墙、反弹）与 `/`、`\`（镜子），从 `(sr,sc)` 任意方向出发，求最少删几个镜子能逃出边界。

```cpp
MirrorGrid01BFS solver(grid);                 // 1. 传入 vector<string> 网格
int ans = solver.shortest_destroy_to_escape(sr, sc); // 2. 起点坐标（0-indexed）
cout << (ans >= (int)1e9 ? -1 : ans) << '\n'; // 3. INF 表示逃不出去
```

样例：`2x2` 全是 `.`，起点 `(0,0)`：原方向直行 0 代价逃出，答案为 0。

**传参要求（照这个传不会错）：**

- `MirrorGrid01BFS(grid)`：`grid` = 网格字符数组（`vector<string>`），自动取 `n = 行数`、`m = 列数`。
- `shortest_destroy_to_escape(sr, sc)`：`sr/sc` = 起点，0-indexed，范围 `[0,n-1] x [0,m-1]`。
- 返回值：逃出边界所需最少删除的镜子数；无法逃出返回 `INF = 1e9`。
- 方向约定：`0 上 / 1 右 / 2 下 / 3 左`；遇 `#` 原路反弹代价 0，遇镜子反射代价 0、删镜直穿代价 1。


**改板时先认这几个量：**

- `dist`：每个方向状态的最短代价。
- `cur`：当前状态的最短代价。

```cpp
struct MirrorGrid01BFS {
    int n, m;
    vector<string> grid;
    const int INF = 1e9;
    int dr[4] = {-1, 0, 1, 0};
    int dc[4] = {0, 1, 0, -1};

    MirrorGrid01BFS(vector<string> grid_) : grid(grid_) {
        n = (int)grid.size();
        m = n ? (int)grid[0].size() : 0;
    }

    int reflect(int dir, char ch) const {
        if (ch == '#') return dir ^ 2;          // 整块墙：原路反弹。
        static int slash[4] = {1, 0, 3, 2};
        static int backslash[4] = {3, 2, 1, 0};
        if (ch == '/') return slash[dir];
        if (ch == '\\') return backslash[dir];
        return dir;
    }

    int shortest_destroy_to_escape(int sr, int sc) {
        vector<vector<array<int, 4>>> dist(n, vector<array<int, 4>>(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                dist[i][j] = {INF, INF, INF, INF};
            }
        }
        deque<tuple<int, int, int>> dq;
        for (int d = 0; d < 4; ++d) {
            dist[sr][sc][d] = 0;
            dq.push_back({sr, sc, d});
        }
        int ans = INF;
        while (!dq.empty()) {
            auto [r, c, d] = dq.front();
            dq.pop_front();
            int cur = dist[r][c][d];
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) {
                ans = min(ans, cur);
                continue;
            }
            char ch = grid[nr][nc];
            if (ch == '#') {
                int nd = reflect(d, ch);
                if (cur < dist[r][c][nd]) {
                    dist[r][c][nd] = cur;
                    dq.push_front({r, c, nd});
                }
            } else if (ch == '/' || ch == '\\') {
                int nd = reflect(d, ch);
                if (cur < dist[nr][nc][nd]) {
                    dist[nr][nc][nd] = cur;
                    dq.push_front({nr, nc, nd});
                }
                if (cur + 1 < dist[nr][nc][d]) {
                    dist[nr][nc][d] = cur + 1;
                    dq.push_back({nr, nc, d}); // 删除镜子后按原方向穿过。
                }
            } else {
                if (cur < dist[nr][nc][d]) {
                    dist[nr][nc][d] = cur;
                    dq.push_front({nr, nc, d});
                }
            }
        }
        return ans;
    }
};
```
#### 计数构造工具：三角数和矩形数贪心拆分

> **赛时先看**
> - **题目信号**：题面给一个很大的 `K`，要求构造恰好 `K` 个“矩形/路径/子序列/交点/逆序对”，并保证存在解。
> - **本质**：很多构造题要求输出一个图/网格/排列，使某类子结构数量恰好为 `K`。常见套路是找一种独立块，其贡献是 `C(x,2)`、`C(x,2)*C(y,2)` 或三角数，然后把 `K` 拆成若干块贡献之和。
> - **接法**：先写 `choose2_floor(k)` 找最大 `x` 使 `C(x,2) <= k`；如果一个块贡献 `C(h,2)*C(w,2)`，可以枚举较小维度再贪心另一维。
> - **复杂度判定**：贪心拆分一般 `O(sqrt K)` 或 `O（块数 * log K)`。
> - **维护的量**：`blocks`（拆分出的独立块大小列表，每块贡献 `C(x,2)`）；`k`（剩余待拆的贡献）。
> - **警告**：块和块之间必须互不影响；网格构造通常要用空行/空列隔开；输出尺寸上限要边构造边检查。


**最小完整示例（先抄这一段就能跑）：**

题目：构造若干互不相交的块，使某类子结构总数为 `K`；每块贡献 `C(x,2)`。

```cpp
i64 K = 11;
vector<i64> blocks = split_into_triangular_numbers(K); // 1. 拆成 {5,2}：10+1=11
auto [h, w] = best_rectangle_block(K, 100);           // 2. 矩形块：贡献 C(h,2)*C(w,2)
// 3. 按 blocks / (h,w) 画块，块之间用空行/空列隔开
```

样例：`K=11` 拆成 `{5,2}`（`C(5,2)=10`、`C(2,2)=1`，合计 11）。

**传参要求（照这个传不会错）：**

- `choose2(x)`：返回 `C(x,2) = x*(x-1)/2`。
- `choose2_floor(limit)`：返回最大的 `x` 使 `C(x,2) <= limit`（`limit=0` 时返回 1）。
- `split_into_triangular_numbers(k)`：`k` = 需要的总贡献（正整数）；返回若干 `x`，满足 `sum(C(x,2)) == k`。
- `best_rectangle_block(k, max_side)`：`k` = 总贡献、`max_side` = 允许的最大边长；返回 `{h, w}` 使 `C(h,2)*C(w,2) <= k` 且尽量大；无可行块返回 `{0,0}`。


```cpp
i64 choose2(i64 x) {
    return x * (x - 1) / 2;
}

i64 choose2_floor(i64 limit) {
    i64 l = 0, r = 2;
    while (choose2(r) <= limit) r <<= 1;
    while (l + 1 < r) {
        i64 mid = (l + r) >> 1;
        if (choose2(mid) <= limit) l = mid;
        else r = mid;
    }
    return l;
}

vector<i64> split_into_triangular_numbers(i64 k) {
    vector<i64> blocks;
    while (k > 0) {
        i64 x = choose2_floor(k);
        blocks.push_back(x);
        k -= choose2(x);
    }
    return blocks; // 每个 x 代表一个贡献 C(x,2) 的独立块。
}

pair<i64, i64> best_rectangle_block(i64 k, i64 max_side) {
    pair<i64, i64> best = {0, 0};
    i64 best_value = 0;
    for (i64 h = 2; h <= max_side; ++h) {
        i64 a = choose2(h);
        if (a > k) break;
        i64 w = min(max_side, choose2_floor(k / a));
        i64 value = a * choose2(w);
        if (value > best_value) {
            best_value = value;
            best = {h, w};
        }
    }
    return best;
}
```
#### 近年构造题自检：随机验证与边界打印

> **赛时先看**
> - **题目信号**：构造函数返回方案；题面有范围、唯一性、连通性、数量等一堆输出约束。
> - **本质**：构造题最怕“想法对但某个边界输出坏了”。本模板提供一个轻量思路：生成方案后立即调用 checker，小规模再用随机测试跑很多次。
> - **接法**：把 `build_one_case` 和 `check_one_case` 换成题目自己的构造与检查；提交前删除或包住 `stress_construct`。
> - **复杂度判定**：本地调试使用，不提交或用 `#ifdef LOCAL` 包住。
> - **维护的量**：`rng`（随机源）；`input/output`（每个 case 的输入与方案，供 checker 验证）。
> - **警告**：随机测试要覆盖最小值、最大值、空约束、全约束、奇偶边界、浮点容差边界。


**最小完整示例（先抄这一段就能跑）：**

题目：构造题本地自检——随机生成小规模输入，构造方案后立刻用 checker 验证。

```cpp
auto build = [&](mt19937& rng) { return Case::random(rng); }; // 1. 随机造小输入
auto check = [&](Case in, Case::Output out) { return my_check(in, out); }; // 2. 校验
stress_construct(1000, build, check);   // 3. 跑 1000 轮，失败即 assert
```

样例：1000 轮全过说明随机小数据下构造合法；失败时 stderr 打印 case 编号与输入/输出。

**传参要求（照这个传不会错）：**

- `stress_construct(rounds, build_one_case, check_one_case)`：`rounds` = 随机测试轮数。
- `build_one_case(rng)`：接收 `mt19937&`，返回输入结构体；该结构体需有 `solve()`（产出方案）和 `debug_print(ostream&)`（出错时打印）。
- `check_one_case(input, output)`：接收输入与 `input.solve()` 的结果，返回 `bool` 表示方案是否合法。
- 失败时：`cerr` 打印“构造自检失败”与 case 编号、输入、输出，然后 `assert(false)`。
- 提交前把调用删除或包进 `#ifdef LOCAL`。


```cpp
template <class Build, class Check>
void stress_construct(int rounds, Build build_one_case, Check check_one_case) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    for (int tc = 1; tc <= rounds; ++tc) {
        auto input = build_one_case(rng);
        auto output = input.solve();
        if (!check_one_case(input, output)) {
            cerr << "构造自检失败，case = " << tc << '\n';
            input.debug_print(cerr);
            output.debug_print(cerr);
            assert(false);
        }
    }
}
```
#### 交互题总策略：协议、次数与故障处理

> **赛时先看**
> - **题目信号**：题面出现 `interactive`、`query`、`flush`、`You may ask at most Q queries`、`print ?`、`print ! answer`。
> - **本质**：给交互题建立统一检查表。交互题不是普通输入输出，程序要一边输出询问、一边读评测器回答。
> - **复杂度判定**：核心复杂度看询问次数；读到限制后先估算 `log n`、`n log n`、`sqrt n`、`n` 哪个量级能过。
> - **警告**：每次询问后必须刷新输出；读到非法回答或 `-1` 要立即退出；不要提前读不存在的输入；不要把调试输出写到标准输出。


- 查询次数约等于 `log n`：优先二分、三分、分治。
- 查询次数约等于 `n log n`：可能是交互排序、分治恢复排列、归并。
- 查询次数约等于 `n` 或 `2n`：考虑每个元素一次/两次信息，或者利用异或、前缀、差分。
- 回答可能有噪声：重复询问取多数，或设计纠错码。
- 本地调试：写一个模拟 interactor，用函数代替真实查询。

#### 交互题通用协议：query / answer / flush

> **赛时先看**
> - **题目信号**：题面让你输出 `? ...` 得到回答，最后输出 `! ...`。
> - **本质**：封装交互题常见输出格式，减少忘记 `flush` 和查询次数超限的问题。
> - **接法**：把 `ask_int` 的输出内容改成题目要求的 query 格式；把 `answer` 改成题目最终答案格式。
> - **复杂度判定**：每次查询 `O（输出长度 + 读入回答）`。
> - **维护的量**：`query_count`（已用询问次数）；`query_limit`（询问上限，超限即 assert）。
> - **警告**：`endl` 会刷新但慢，`'\n' << flush` 更明确；读到 `-1` 通常代表格式错或次数爆了，必须 `exit(0)`。


**最小完整示例（先抄这一段就能跑）：**

题目：交互题每次输出 `? x y` 并读回答，最后输出 `! ans`，询问上限 100 次。

```cpp
InteractiveIO io(100);               // 1. 传询问上限，超限自动 assert
int resp = io.ask_int({x, y});       // 2. 输出 "? x y"（内部已 flush）并读回答
io.answer(ans);                      // 3. 输出 "! ans" 并结束程序
```

样例：`ask_int({1, 2})` 输出 `? 1 2`；回答 `-1` 时程序内部直接 `exit(0)`。

**传参要求（照这个传不会错）：**

- `InteractiveIO(limit = INT_MAX)`：`limit` = 询问上限；每次 `ask_int` 前断言 `query_count < query_limit`。
- `ask_int(args)`：`args` = 放进 `?` 后的整数列表；返回评测器回答的整数；读到 `-1` 或输入结束自动 `exit(0)`。
- `answer(value)`：输出 `! value`（任意可打印类型）并 `exit(0)`。
- 每次询问内部已 `flush`，不要再手动刷新。


```cpp
struct InteractiveIO {
    int query_count = 0;
    int query_limit = INT_MAX;

    InteractiveIO(int limit = INT_MAX) : query_limit(limit) {}

    int ask_int(const vector<int>& args) {
        assert(query_count < query_limit);
        ++query_count;
        cout << "?";
        for (int x : args) cout << ' ' << x;
        cout << '\n' << flush; // 交互题必须刷新输出。

        int response;
        if (!(cin >> response)) exit(0); // 评测器结束或协议错误。
        if (response == -1) exit(0);     // 多数交互题用 -1 表示非法询问。
        return response;
    }

    template <class T>
    void answer(const T& value) {
        cout << "! " << value << '\n' << flush;
        exit(0);
    }
};
```
#### 交互二分：隐藏下标或阈值

> **赛时先看**
> - **题目信号**：交互题允许你问 `? mid`，评测器回答"答案在左边还是右边"（大小/是否可行）；题面询问上限接近 `ceil(log2 n)`。
> - **本质**：隐藏对象有单调性质（答案 ≤ mid 是否成立随 mid 单调），用 `O(log n)` 次询问把答案区间对半压缩。
> - **复杂度判定**：`O(log n)` 次查询；若上限是 `log2 n` 的两倍左右，可能是"先二分定位 + 一次确认"，检查是否真的要多问。
> - **维护的量**：二分边界 `l/r`；每次询问后按回答收缩一半。
> - **接法**：把 `predicate(mid)` 换成一次真实 `ask`，含义是"答案是否 <= mid"，然后 `interactive_first_true(n, ask)`。
> - **警告**：交互二分不要多问一次确认；边界 `[l,r]` 写成闭区间更稳；如果回答含噪声，套多数投票（本册交互容错节）。


**最小完整示例（先抄这一段就能跑）：**

题目：交互题，评测器藏了一个答案在 `[1, n]`，每次问 `? mid` 回答 `0/1`（1 表示答案 <= mid），最多问 `ceil(log2 n)` 次。

```cpp
auto ask = [&](int mid) {
    cout << "? " << mid << '\n' << flush;   // 1. 输出询问（必须 flush）
    int resp;
    cin >> resp;                              // 2. 读回答
    return resp == 1;                         // 含义：答案 <= mid
};
int ans = interactive_first_true(n, ask);     // 3. 调用：得到最小可行答案
cout << "! " << ans << '\n' << flush;        // 4. 输出最终答案
```

样例：答案藏为 42，`n = 100`；交互器会返回 1 当且仅当 `mid >= 42`，程序输出 `! 42`。

**传参要求（照这个传不会错）：**

- `interactive_first_true(n, predicate)`：`n` = 答案上界（答案范围 `[1, n]`）。
- `predicate(mid)`：你的询问封装，返回 bool = "答案是否 <= mid"；必须单调。
- 返回值：最小的满足条件的整数，直接输出。
- 每次询问后必须 `flush`（`'\n' << flush`）；回答 `-1` 表示询问非法，立即 `exit(0)`。
- 问"最大可行值"时把判定方向反过来并自行改成上取中位数二分。

**抄板清单（照着做就行）：**
1. 抄哪段：`interactive_first_true` 模板函数。
2. 构造：写 `auto ask = [&](int mid) { ... 真实询问，返回 bool ... };`（bool 含义 = "答案 <= mid"）。
3. 调用：`int ans = interactive_first_true(n, ask);`
4. 取结果：`ans` 即最小可行下标；记得 `flush` 由你的询问函数负责。

**改造点（按题目改这几处）：**
- 询问格式：`cout << "? " << mid << '\n' << flush;` 后 `cin >> resp;`，把回答转成 bool。
- 问最大值：把判定改成"答案 >= mid"，用 `max_true` 版二分（取上中位 `(l+r+1)/2`）。
- 回答是 0/1 之外的比较：把 `resp >= mid` 之类的条件折进判定函数。
- 有噪声：对同一个 mid 重复问奇数遍取多数。

**核心逻辑（改代码时别破坏）：**

- 判定必须单调：`ask(mid)` 为真时答案 ≤ mid，否则答案 > mid。
- 闭区间 `[l, r]`，`l` 必须是一个"答案 ≤ l"必假的下界。

**改板时先认这几个量：**

- `predicate(mid)`：真实询问的封装，返回"答案是否 <= mid"。

```cpp
// 交互二分：在 [1, n] 里找最小的 x 使 predicate(x) 为真；predicate 单调。
// predicate(mid) 内部必须完成一次真实的交互询问（输出 ? 并读入回答）。
template <class Predicate>
int interactive_first_true(int n, Predicate predicate) {
    int l = 1, r = n;
    while (l < r) {
        int mid = (l + r) >> 1; // 闭区间下取中，保证不会死循环。
        if (predicate(mid)) r = mid; // 答案 <= mid，收缩右边界。
        else l = mid + 1;            // 答案 > mid，收缩左边界。
    }
    return l;
}
```
#### 交互比较排序：用查询当比较器

> **赛时先看**
> - **题目信号**：题面允许询问 `compare(a,b)`；询问上限接近 `n log n`；最终要输出一个排列。
> - **本质**：评测器能回答两个元素的大小、优先级、祖先关系或相对顺序时，用稳定排序/归并排序恢复隐藏排列。
> - **接法**：把 `less_than(a,b)` 换成真实查询；如果回答“a 是否排在 b 前”，归并时直接用。
> - **复杂度判定**：`O(n log n)` 次比较询问。
> - **维护的量**：`a`（待排序的元素列表）；`less_than`（比较器，内部封装一次真实询问）。
> - **警告**：C++ `sort` 的比较器有过多副作用或不满足严格弱序时会出事；交互比较更推荐自己写归并排序，询问次数稳定。


**最小完整示例（先抄这一段就能跑）：**

题目：交互题允许问两个元素谁在前（约 `n log n` 次），要求恢复隐藏排列。

```cpp
auto less_than = [&](int x, int y) {   // 1. 把真实询问封装成 bool 比较器
    cout << "? " << x << ' ' << y << '\n' << flush;
    int r; cin >> r;                   //    r==1 表示 x 排在 y 前
    return r == 1;
};
vector<int> perm = interactive_merge_sort(a, less_than); // 2. 归并排序恢复
for (int x : perm) cout << x << ' ';   // 3. 输出排列
```

样例：`n=4`、初始 `{4,2,3,1}`，按真实比较恢复为 `{1,2,3,4}`。

**传参要求（照这个传不会错）：**

- `interactive_merge_sort(a, less_than)`：`a` = 初始元素列表（元素类型随意）；返回按 `less_than` 排好序的 `vector<int>`。
- `less_than(x, y)`：返回 `bool` = “x 应排在 y 前”；每次调用必须恰好完成一次真实询问；须是严格弱序。
- 询问次数：稳定 `O(n log n)`，不会像 `std::sort` 那样最坏退化。


```cpp
template <class Less>
vector<int> interactive_merge_sort(vector<int> a, Less less_than) {
    if ((int)a.size() <= 1) return a;
    int mid = (int)a.size() / 2;
    vector<int> left(a.begin(), a.begin() + mid);
    vector<int> right(a.begin() + mid, a.end());
    left = interactive_merge_sort(left, less_than);
    right = interactive_merge_sort(right, less_than);

    vector<int> res;
    int i = 0, j = 0;
    while (i < (int)left.size() || j < (int)right.size()) {
        if (j == (int)right.size()) res.push_back(left[i++]);
        else if (i == (int)left.size()) res.push_back(right[j++]);
        else if (less_than(left[i], right[j])) res.push_back(left[i++]);
        else res.push_back(right[j++]);
    }
    return res;
}
```
#### 交互容错：重复询问多数投票

> **赛时先看**
> - **题目信号**：题面出现 probability、lie、malfunction、random response、回答不一定可靠。
> - **本质**：处理评测器可能随机出错、隐藏对象可能有噪声、或题面允许若干次错误回答的交互模型。
> - **接法**：把 `ask_once` 换成一次真实查询；若返回值只有 0/1，用 `majority_binary_query`。
> - **复杂度判定**：把原查询次数乘以重复次数 `repeat`。
> - **维护的量**：`ones` / `cnt`（重复询问中各回答的计数，用多数/众数当最终答案）。
> - **警告**：`repeat` 必须是奇数；如果询问上限很紧，不能无脑重复，要用纠错码或贝叶斯更新。


**最小完整示例（先抄这一段就能跑）：**

题目：交互回答可能随机出错，对同一问题重复问奇数遍取多数作为最终回答。

```cpp
auto ask_once = [&]() { return ask_q() == 1 ? 1 : 0; };  // 1. 单次真实查询转 0/1
int vote = majority_binary_query(ask_once, 3);  // 2. 重复 3 遍取 0/1 多数
int val  = majority_value_query(ask_once, 5);   // 3. 或重复 5 遍取众数
```

样例：三次回答 `1,0,1` -> 多数为 1；五次回答 `2,5,2,3,2` -> 众数为 2。

**传参要求（照这个传不会错）：**

- `majority_binary_query(ask_once, repeat = 3)`：`ask_once()` = 一次真实询问的封装，返回 0 或 1；`repeat` 必须是奇数；返回 0/1 多数。
- `majority_value_query(ask_once, repeat = 5)`：`ask_once()` 返回任意整数值；返回 `repeat` 次中出现次数最多的值。
- 两个函数都不再修改你的询问封装；返回值都是 `int`。


```cpp
template <class AskOnce>
int majority_binary_query(AskOnce ask_once, int repeat = 3) {
    assert(repeat % 2 == 1);
    int ones = 0;
    for (int i = 0; i < repeat; ++i) ones += ask_once();
    return ones * 2 > repeat;
}

template <class AskOnce>
int majority_value_query(AskOnce ask_once, int repeat = 5) {
    map<int, int> cnt;
    int best_value = 0, best_count = -1;
    for (int i = 0; i < repeat; ++i) {
        int x = ask_once();
        if (++cnt[x] > best_count) {
            best_count = cnt[x];
            best_value = x;
        }
    }
    return best_value;
}
```
#### 通信题信息量估算与设计清单

> **赛时先看**
> - **题目信号**：题面出现 Alice/Bob、encoder/decoder、send_message、receive_message、bit_set、two separate programs。
> - **本质**：通信题通常要写两个函数/两份程序：发送者看见一部分信息，接收者看见另一部分信息，通过有限通信恢复答案。
> - **复杂度判定**：先看通信容量，不先看时间复杂度。能传 `B` 位，就最多区分 `2^B` 种消息；答案空间或不确定性必须被压到这个范围内。
> - **警告**：不要把所有数据都传过去；通信顺序、编号、共享随机种子、是否有反馈通道必须按题面确认。


- 要比较/恢复数值：考虑进制分组、前缀编码、二分最高不同位。
- 要传集合：考虑 bitset、Bloom/hash 摘要、XOR 和、模数摘要。
- 有一位/少量错误：考虑奇偶校验、Hamming 码、重复码。
- 输出是交互式 API：先用普通函数模拟 Alice/Bob，再改成题面 API。

#### 通信题 bit packing：定长整数编码

> **赛时先看**
> - **题目信号**：每个数范围小，比如 `< 2^k`；要传很多个数但直接传 `int` 太浪费。
> - **本质**：把若干个小整数按固定位宽打包成 `uint64_t` 数组，适合通信容量按 bit 或 machine word 计的题。
> - **接法**：发送方 `pack_fixed_width(a, bits)`；接收方 `unpack_fixed_width(words, n, bits)`。
> - **复杂度判定**：编码/解码都是 `O（元素数 * 位宽 / 64)`。
> - **维护的量**：`w`（打包后的 64 位字数组）；`pos/id/off`（每个元素在 bit 流里的位置、所在字、字内偏移）。
> - **警告**：位宽要能覆盖最大值；跨 `uint64_t` 边界时要分两段写入；通信题里双方必须约定同一顺序和位宽。


**最小完整示例（先抄这一段就能跑）：**

题目：Alice 把 `n` 个小于 `2^bits` 的数传给 Bob，通信按 64 位字计数。

```cpp
vector<unsigned long long> a = {3, 1, 2};   // 1. 待传数据，各元素 < 2^bits
auto words = pack_fixed_width(a, 2);        // 2. 编码：压成 ceil(6/64)=1 个字
auto back  = unpack_fixed_width(words, 3, 2); // 3. 解码：还原 n=3 个元素
assert(back == a);                          // 4. 校验一致
```

样例：`a={3,1,2}`、`bits=2` -> 1 个 word（值 39），解回 `{3,1,2}` 完全一致。

**传参要求（照这个传不会错）：**

- `pack_fixed_width(a, bits)`：`a` = 待编码元素（每个须 `0 <= a[i] < 2^bits`）；`bits` 范围 `[1,63]`；返回字数组，长度 `ceil(n*bits/64)`。
- `unpack_fixed_width(w, n, bits)`：`w` = `pack_fixed_width` 的输出；`n` = 元素个数；`bits` 必须与编码方一致；返回还原的 `n` 个元素。
- 双方必须约定同一 `bits` 和元素顺序；跨 64 位边界的元素由函数自动分两段读写，无需手动处理。


```cpp
vector<unsigned long long> pack_fixed_width(const vector<unsigned long long>& a, int bits) {
    assert(1 <= bits && bits <= 63);
    int total_bits = (int)a.size() * bits;
    vector<unsigned long long> w((total_bits + 63) / 64, 0);
    unsigned long long mask = (bits == 64 ? ~0ULL : ((1ULL << bits) - 1));
    for (int i = 0; i < (int)a.size(); ++i) {
        unsigned long long x = a[i] & mask;
        int pos = i * bits;
        int id = pos >> 6;
        int off = pos & 63;
        w[id] |= x << off;
        if (off + bits > 64) w[id + 1] |= x >> (64 - off);
    }
    return w;
}

vector<unsigned long long> unpack_fixed_width(const vector<unsigned long long>& w, int n, int bits) {
    assert(1 <= bits && bits <= 63);
    vector<unsigned long long> a(n);
    unsigned long long mask = (1ULL << bits) - 1;
    for (int i = 0; i < n; ++i) {
        int pos = i * bits;
        int id = pos >> 6;
        int off = pos & 63;
        unsigned long long x = w[id] >> off;
        if (off + bits > 64) x |= w[id + 1] << (64 - off);
        a[i] = x & mask;
    }
    return a;
}
```
#### 通信题 XOR 翻一位传目标

> **赛时先看**
> - **题目信号**：有 `0/1` 状态数组；允许翻转恰好一个位置；接收者看最终状态并要恢复某个编号。
> - **本质**：发送者只能改变一个位置/一位，但接收者要得到目标编号 `target`。利用 XOR 自反性：`x ^ y ^ x = y`。
> - **接法**：当前所有为 1 的位置异或和为 `cur`，想让接收者读出 `target`，就翻 `cur ^ target` 这个位置。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`bit`（0/1 状态数组）；`cur`（所有为 1 位置的下标异或和）。
> - **警告**：位置编号必须覆盖目标范围，通常要求 `n` 是 2 的幂或至少大于目标上界；如果编号从 1 开始，要双方统一。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个 0/1 位（0-indexed），发送者只翻一位，接收者读最终状态里 1 位下标异或得到 `target`。

```cpp
vector<int> bit(8, 0); bit[1] = bit[2] = 1;   // 1. 初始状态，cur = 1^2 = 3
int pos = choose_flip_position(bit, 5);       // 2. 应翻位置 = 3^5 = 6
bit[pos] ^= 1;                                // 3. 翻这一位
int got = decode_xor_message(bit);            // 4. 接收者读出 5
assert(got == 5);
```

样例：`bit={0,1,1,0,0,0,0,0}`、`target=5` -> 翻位置 6，接收者异或得 5。

**传参要求（照这个传不会错）：**

- `xor_state(bit)`：`bit` = 0/1 数组（任意长度）；返回所有为 1 位置的下标异或和。
- `choose_flip_position(bit, target)`：`bit` = 当前状态；`target` = 想让接收者读出的编号；返回 `cur ^ target` 作为要翻转的位置。
- 前置条件：必须满足 `0 <= cur ^ target < bit.size()`（内部 `assert` 会检查），所以数组长度要大于所有可能的 target。
- `decode_xor_message(bit_after_flip)`：接收方读最终状态，返回异或和即 target。
- 下标统一 0-indexed；若题面编号从 1 开始，双方先各自减 1。


**改板时先认这几个量：**

- `bit`：0/1 状态数组（每位可翻转一次）。
- `cur`：当前所有 1 位的异或和。

```cpp
int xor_state(const vector<int>& bit) {
    int x = 0;
    for (int i = 0; i < (int)bit.size(); ++i) {
        if (bit[i]) x ^= i;
    }
    return x;
}

int choose_flip_position(vector<int> bit, int target) {
    int cur = xor_state(bit);
    int pos = cur ^ target;
    assert(0 <= pos && pos < (int)bit.size());
    return pos;
}

int decode_xor_message(const vector<int>& bit_after_flip) {
    return xor_state(bit_after_flip);
}
```
#### 通信题 Hamming(7,4)：一位纠错

> **赛时先看**
> - **题目信号**：题面有传输错误、翻转一位、最多一位 corrupted、需要恢复原消息。
> - **本质**：每 4 位数据编码成 7 位，能纠正任意 1 位错误。适合通信/交互里“可能有一位坏掉”的小块编码。
> - **接法**：发送方对每个 4-bit 块调用 `hamming74_encode`；接收方先 `hamming74_decode` 自动修一位，再拼回原消息。
> - **复杂度判定**：每 4 位一组，`O（消息长度）`。
> - **维护的量**：`b[1..7]`（1-indexed 码字位）；`syndrome`（校验子，用于定位出错位）。
> - **警告**：本模板使用 1-index 位置逻辑，校验位在 1、2、4，数据位在 3、5、6、7；返回的数据低 4 位是原消息。


**最小完整示例（先抄这一段就能跑）：**

题目：每 4 位消息编码成 7 位传输，允许 1 位翻转，接收端自动纠错还原。

```cpp
int data4 = 11;                            // 1. 4 位数据（0..15）
int code = hamming74_encode(data4);        // 2. 发送：11 -> 码字 85(1010101)
code ^= (1 << 3);                          // 3. 模拟传输中翻 1 位（第 4 位）
int got = hamming74_decode(code);          // 4. 接收：自动修 1 位并还原
assert(got == 11);
```

样例：`data4=11` -> `code=85`，翻任意 1 位后 decode 仍还原 11。

**传参要求（照这个传不会错）：**

- `hamming74_encode(data4)`：`data4` = 4 位数据（`0..15`）；返回 7 位码字（用低 7 位），按位打包成 int。
- `hamming74_decode(code7)`：`code7` = 收到的码字（可有 1 位错误）；自动定位并纠正后返回 4 位原数据（`0..15`）。
- 位置逻辑 1-indexed：校验位 1/2/4，数据位 3/5/6/7；翻转超过 1 位无法保证纠正。
- 多个 4 位块逐个调用即可，函数无内部状态。


```cpp
int hamming74_encode(int data4) {
    vector<int> b(8, 0); // 使用 1..7。
    b[3] = (data4 >> 0) & 1;
    b[5] = (data4 >> 1) & 1;
    b[6] = (data4 >> 2) & 1;
    b[7] = (data4 >> 3) & 1;
    b[1] = b[3] ^ b[5] ^ b[7];
    b[2] = b[3] ^ b[6] ^ b[7];
    b[4] = b[5] ^ b[6] ^ b[7];
    int code = 0;
    for (int i = 1; i <= 7; ++i) code |= b[i] << (i - 1);
    return code;
}

int hamming74_decode(int code7) {
    vector<int> b(8, 0);
    for (int i = 1; i <= 7; ++i) b[i] = (code7 >> (i - 1)) & 1;
    int syndrome = 0;
    if ((b[1] ^ b[3] ^ b[5] ^ b[7]) != 0) syndrome |= 1;
    if ((b[2] ^ b[3] ^ b[6] ^ b[7]) != 0) syndrome |= 2;
    if ((b[4] ^ b[5] ^ b[6] ^ b[7]) != 0) syndrome |= 4;
    if (1 <= syndrome && syndrome <= 7) b[syndrome] ^= 1;
    int data4 = 0;
    data4 |= b[3] << 0;
    data4 |= b[5] << 1;
    data4 |= b[6] << 2;
    data4 |= b[7] << 3;
    return data4;
}
```
#### 通信题随机摘要：集合一致性校验

> **赛时先看**
> - **题目信号**：Alice 和 Bob 各有一个集合/序列，不能全传，要求判断是否相同或找差异。
> - **本质**：用少量哈希摘要判断两个集合/多重集是否一致，或者定位“只差一个元素”的情况。常用于通信题、分布式校验和构造自检。
> - **接法**：双方使用同一个 `splitmix64_hash(value)` 作为元素随机权，集合摘要用 XOR；只差一个元素时，两个摘要异或可以得到差异元素的哈希。
> - **复杂度判定**：`O(n)` 计算摘要。
> - **维护的量**：`xr`（异或摘要，只差一个元素时异或差值就是该元素的哈希）；`sum`（哈希和，区分重复元素）；`sum_sq`（哈希平方和，和相同但重复次数不同也能区分）。
> - **警告**：随机哈希有极小碰撞概率：题目要求确定性时应改用多模数哈希，只有题目允许随机化才可直接用；多重集要同时维护和、平方和或计数摘要。


**最小完整示例（先抄这一段就能跑）：**

题目：Alice 和 Bob 各有一个集合（元素可重复），只传摘要判断是否相同：

```cpp
SetSketch a, b;
for (auto v : setA) a.add(v);   // 1. 双方用同一个 splitmix64_hash 作为元素权
for (auto v : setB) b.add(v);
bool same = a.probably_equal(b);   // 2. 调用：true = 两个集合大概率相同
// 只差一个元素时：a.xr ^ b.xr 就是差异元素的哈希，可直接查它是什么。
```

**传参要求（照这个传不会错）：**

- `splitmix64_hash(x)`：`x` 是元素值（`unsigned long long`，整数直接传，字符/字符串先转成整数编码）；返回该元素的随机哈希。
- `add(x)`：`x` 为 `unsigned long long`；把 `splitmix64_hash(x)` 并入三个摘要（重复加同一个值会重复计入，天然支持多重集）。
- `probably_equal(other)`：`other` 是另一个 `SetSketch`；三个摘要（`xr`、`sum`、`sum_sq`）全部相等才返回 `true`。


**API / 入口函数（赛时只认这里列的名字）：**

- `add(unsigned long long x)` -> 加入一个元素/贡献

```cpp
unsigned long long splitmix64_hash(unsigned long long x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

struct SetSketch {
    unsigned long long xr = 0;
    unsigned long long sum = 0;
    unsigned long long sum_sq = 0;

    void add(unsigned long long x) {
        unsigned long long h = splitmix64_hash(x);
        xr ^= h;
        sum += h;
        sum_sq += h * h;
    }

    bool probably_equal(const SetSketch& other) const {
        return xr == other.xr && sum == other.sum && sum_sq == other.sum_sq;
    }
};
```

### 16 博弈、随机、STL 与杂项速查

最后放博弈、随机数、日期、STL 和一些不适合塞进前面章节但比赛里常用的速查项。
#### SG 函数

> **赛时先看**
> - **题目信号**：两人轮流操作，不能操作者输；局面能拆成独立子游戏。
> - **本质**：公平组合游戏，多堆独立游戏异或。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：状态数乘转移数。
> - **维护的量**：`sg[x]`（局面 x 的 SG 值）；`nxt`（x 的所有后继 SG 值，供 mex 用）。
> - **警告**：SG 为 0 是必败态；多个子游戏 SG 异或为 0 是必败。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 局面最大 n=100，每次只能取 {1,3,4} 个；sg[x]=0 必败、非 0 必胜。
vector<int> sg = sg_take_away(100, {1, 3, 4});
bool win = (sg[5] != 0);   // 单堆 5 个：sg[5]=3 → 先手胜
// 多堆独立子游戏：把各堆 sg 异或，非 0 先手胜（SG 定理）。
// 样例：sg[7]=0 → 7 个石子先手必败。
```

**传参要求（照这个传不会错）：**

- `mex(vals)`：传入所有后继 SG 值（`vector<int>`），返回最小的未出现非负整数（`int`）。
- `sg_take_away(max_n, moves)`：`max_n` 是最大石子数（下标 1..max_n 都算，0 是必败空局）；`moves` 是每步可取数量集合（`vector<int>`，如 `{1,3,4}`）；返回 `vector<int> sg`，`sg[x] != 0` 表示石子数为 x 时先手必胜。


**改板时先认这几个量：**

- `sg`：每个局面的 SG 值数组。
- `nxt`：当前局面的后继 SG 值列表。

```cpp
int mex(vector<int> vals) {
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int g = 0;
    for (int x : vals) {
        if (x == g) g++;
        else if (x > g) break;
    }
    return g;
}

vector<int> sg_take_away(int max_n, const vector<int>& moves) {
    vector<int> sg(max_n + 1);
    for (int x = 1; x <= max_n; ++x) {
        vector<int> nxt;
        for (int mv : moves) {
            if (x >= mv) nxt.push_back(sg[x - mv]);
        }
        sg[x] = mex(nxt);
    }
    return sg;
}
```
#### Nim 游戏

> **赛时先看**
> - **题目信号**：若干堆，每次选一堆取任意正数，不能取者输。
> - **本质**：经典取石子游戏。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`x`（全部堆的异或和，非 0 即先手胜）；`piles` 只读不改。
> - **警告**：异或和为 0 是先手必败。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 各堆石子数按题面顺序填进数组，个数不限、每堆可到 1e18。
bool win = first_win_nim({3, 4, 5});   // 3^4^5=2 ≠ 0 → 先手胜
// 样例：{3,4,5} → true；{3,4,7} → 3^4^7=0 → false（先手必败）。
```

**传参要求（照这个传不会错）：**

- `first_win_nim(piles)`：`piles` 是各堆石子数的 `vector<i64>`（每堆 0..1e18 均可，堆数不限，允许 0 的空堆）；返回 `bool`：`true` 先手必胜，`false` 先手必败。


```cpp
bool first_win_nim(const vector<i64>& piles) {
    i64 x = 0;
    for (i64 v : piles) x ^= v;
    return x != 0;
}
```
#### 巴什博弈 / Bachet：一堆每次取 1..k

> **赛时先看**
> - **题目信号**：题面只有一堆；每次至少取 1 个、最多取 `k` 个；没有其他限制。
> - **本质**：一堆石子有 `n` 个，两人轮流取 `1..k` 个，不能取者输。最基础的取石子周期模型。
> - **接法**：如果先手必胜，第一步取 `n % (k+1)` 个；之后对手取 `x` 个，你取 `k+1-x` 个，把局面重新送回倍数。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：`take`（先手第一步取 `n % (k+1)` 个）；之后每回合保持"你取 + 对手取 = k+1"即可。
> - **警告**：正常规则下 `n % (k+1) == 0` 是先手必败；如果题面说"取最后一个输"，这是反常规则，不能直接套。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 一堆 n 个，每次取 1..k 个，取最后者胜；先手必胜时一步送入必败态。
bool win = first_win_bash(10, 3);     // 10 % 4 = 2 ≠ 0 → 先手胜
i64 take = bash_first_take(10, 3);    // 第一步取 2，之后对手取 x 你补 4-x
// 样例：n=10,k=3 → win=true, take=2；n=12,k=3 → win=false, take=0（必败）。
```

**传参要求（照这个传不会错）：**

- `first_win_bash(n, k)`：`n` 石子总数、`k` 每次最多可取数（`i64`，`k ≥ 1`）；返回 `bool`：`true` 先手必胜。
- `bash_first_take(n, k)`：参数同上；返回先手第一步应取数量（`i64`，范围 1..k）；返回 `0` 表示当前已是必败态、没有必胜第一步。


```cpp
bool first_win_bash(i64 n, i64 k) {
    // 正常规则：每次取 1..k，不能取者输。
    return n % (k + 1) != 0;
}

i64 bash_first_take(i64 n, i64 k) {
    // 返回先手第一步应该取多少；0 表示当前就是必败态。
    i64 take = n % (k + 1);
    return take;
}
```
#### Nim 必胜一步：把异或和打成 0

> **赛时先看**
> - **题目信号**：若干堆石子；一次选一堆取任意正数；题目问胜负并要求输出操作。
> - **本质**：不仅判断 Nim 胜负，还要输出先手第一步取哪一堆、取到多少。
> - **接法**：若 `xor_sum == 0` 无必胜一步；否则调用 `nim_winning_move`，输出堆编号和要取走的数量 `old - target`。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`xr`（全部堆异或和）；`NimMove{index, target, take}`（改哪堆、改成几、取走几个）。
> - **警告**：找到最高位后，不是随便取；要选一堆 `a[i]` 使 `(a[i] ^ xor_sum) < a[i]`，把它改成这个值。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目要输出"取哪一堆、取多少"时用；只判胜负用「Nim 游戏」那节。
NimMove mv = nim_winning_move({3, 4, 5});  // xr=2：3^2=1 < 3 → 改 a[0]
printf("取第 %d 堆，取走 %lld\n", mv.index, mv.take);
// 样例：{3,4,5} → mv = {index=0, target=1, take=2}；{3,4,7} → index=-1（必败）。
```

**传参要求（照这个传不会错）：**

- `nim_winning_move(piles)`：`piles` 各堆石子数（`vector<i64>`）；返回 `NimMove{index, target, take}`：`index` 是 0-index 堆编号（`-1` 表示当前必败、无必胜一步）；`target` 是这堆应改成的新数量（异或值替换后必小于原值）；`take = 原数 - target` 是应取走的数量，直接输出即可。


```cpp
struct NimMove {
    int index = -1;      // 0-index 堆编号；-1 表示当前是必败态。
    i64 target = 0;      // 把这一堆变成 target。
    i64 take = 0;        // 从这一堆取走 take。
};

NimMove nim_winning_move(const vector<i64>& piles) {
    i64 xr = 0;
    for (i64 x : piles) xr ^= x;
    if (xr == 0) return {};
    for (int i = 0; i < (int)piles.size(); ++i) {
        i64 target = piles[i] ^ xr;
        if (target < piles[i]) {
            return {i, target, piles[i] - target};
        }
    }
    return {}; // 理论上不会走到这里。
}
```
#### 反常 Nim / Misere Nim：取最后一个输

> **赛时先看**
> - **题目信号**：Nim 规则几乎一样，但题面写“取最后一个的人输”或“last move loses”。
> - **本质**：若干堆，每次从一堆取任意正数，但取走最后一枚石子的玩家失败。
> - **接法**：先统计 `big = count(a[i] > 1)` 和 `ones = count(a[i] == 1)`；`big == 0` 走奇偶特判，否则看总异或。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`big`（>1 的堆数）、`ones`（=1 的堆数）、`xr`（全部堆异或和）。
> - **警告**：只要存在大于 1 的堆，就按普通 Nim 的异或和判断；所有堆都为 0/1 时，奇数个 1 是先手必败，偶数个 1 是先手必胜。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 取最后一个石子的人输；其余规则与普通 Nim 相同。
bool win1 = first_win_misere_nim({1, 1, 1, 1});  // 全 1：偶数个 → 先手胜
bool win2 = first_win_misere_nim({2, 2});        // 有 >1 的堆：异或 2^2=0 → 败
// 样例：{1,1,1,1} → true；{1,1,1} → false；{2,2} → false。
```

**传参要求（照这个传不会错）：**

- `first_win_misere_nim(piles)`：`piles` 各堆石子数的 `vector<i64>`（堆数不限，值可到 1e18）；返回 `bool`：`true` 先手必胜。内部自动区分"全 0/1 奇偶特判"与"存在大堆走普通异或"两种情况，不需要手动分支。


```cpp
bool first_win_misere_nim(const vector<i64>& piles) {
    int big = 0, ones = 0;
    i64 xr = 0;
    for (i64 x : piles) {
        xr ^= x;
        if (x > 1) big++;
        if (x == 1) ones++;
    }
    if (big == 0) return ones % 2 == 0;
    return xr != 0;
}
```
#### Moore's Nim-k：一次最多操作 k 堆

> **赛时先看**
> - **题目信号**：题面明确“每次可以同时从不超过 k 堆中取石子”。
> - **本质**：普通 Nim 的推广。每回合可以选择至少 1 堆、至多 `k` 堆，每堆各取任意正数，取走最后者胜。
> - **接法**：如果任意一位计数模 `k+1` 不为 0，先手必胜；全为 0 则必败。
> - **复杂度判定**：`O(n log A)`。
> - **维护的量**：`cnt[b]`（全部堆中第 b 个二进制位为 1 的个数）；胜负只看 `cnt[b] % (k+1)` 是否全为 0。
> - **警告**：不是把所有堆异或；要对每一个二进制位统计 1 的个数，并对 `k+1` 取模。`k=1` 时退化为普通 Nim。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 每回合至少取 1 堆、至多同时操作 k 堆，每堆可各取任意正数，取最后者胜。
bool win = first_win_moore_nim_k({1, 2, 3}, 2);   // 位计数模 3 非全 0 → 先手胜
bool lose = first_win_moore_nim_k({1, 2, 3}, 1);  // k=1 退化普通 Nim：1^2^3=0 → 败
// 样例：{1,2,3},k=2 → true；{1,2,3},k=1 → false。
```

**传参要求（照这个传不会错）：**

- `first_win_moore_nim_k(piles, k)`：`piles` 各堆石子数（`vector<ui64>`，每堆可到 1e18 以上）；`k` 每回合最多同时操作的堆数（`int`，`1 ≤ k ≤ 堆数`，`k=1` 就是普通 Nim）；返回 `bool`：`true` 先手必胜。


```cpp
bool first_win_moore_nim_k(const vector<ui64>& piles, int k) {
    vector<int> cnt(64, 0);
    for (ui64 x : piles) {
        for (int b = 0; b < 64; ++b) {
            if ((x >> b) & 1ULL) cnt[b]++;
        }
    }
    for (int b = 0; b < 64; ++b) {
        if (cnt[b] % (k + 1) != 0) return true;
    }
    return false;
}
```
#### 阶梯 Nim：只看奇数层异或

> **赛时先看**
> - **题目信号**：石子不是直接消失，而是向前一个位置移动；题面像“台阶”“楼梯”“从 i 移到 i-1”。
> - **本质**：有 `n` 个位置，每次可以把第 `i>1` 堆的若干石子移到 `i-1`，或从第 1 堆取走若干，不能操作输。
> - **接法**：把所有 1-index 奇数位置的石子数异或，非 0 先手胜。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`xr`（1-index 奇数位置石子数的异或和；0-index 版看偶数下标）。
> - **警告**：如果位置从 1 开始，只异或奇数位置；如果数组从 0 开始，奇数位置对应下标偶数。
> - **约定**：a[1..n] 有效；a[0] 不用。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 1-index：a[1..n] 有效，a[0] 随便填 0 占位；石子只能往编号小的位置移。
bool win = first_win_staircase_nim_1indexed({0, 1, 2, 3});  // 1^3=2 → 先手胜
// 0-index：直接看偶数下标（等价于 1-index 的奇数位）。
bool win2 = first_win_staircase_nim_0indexed({1, 2, 3});    // 1^3=2 → 先手胜
// 样例：{0,1,2,3} → true；{0,2,0,2} → 2^2=0 → false。
```

**传参要求（照这个传不会错）：**

- `first_win_staircase_nim_1indexed(a)`：`a` 长度至少 n+1，`a[1..n]` 是每层石子数（`vector<i64>`），`a[0]` 必须存在但内容随意（代码从下标 1 起跳）；返回 `bool`：1-index 奇数位置异或非 0 即先手胜。
- `first_win_staircase_nim_0indexed(a)`：`a[0..n-1]` 直接是石子数（`vector<i64>`）；返回 `bool`：偶数下标异或非 0 即先手胜。


```cpp
bool first_win_staircase_nim_1indexed(const vector<i64>& a) {
    // a[1..n] 有效；a[0] 不用。
    i64 xr = 0;
    for (int i = 1; i < (int)a.size(); i += 2) xr ^= a[i];
    return xr != 0;
}

bool first_win_staircase_nim_0indexed(const vector<i64>& a) {
    i64 xr = 0;
    for (int i = 0; i < (int)a.size(); i += 2) xr ^= a[i];
    return xr != 0;
}
```
#### Fibonacci Nim：最小 Zeckendorf 项

> **赛时先看**
> - **题目信号**：题面出现“下一次最多取上一次的两倍”“first move cannot take all”。
> - **本质**：一堆 `n` 个石子。第一步不能取完；之后每次最多取上一次对手取走数量的 2 倍，取走最后者胜。
> - **接法**：开局若不是 Fibonacci 数，第一步取 `min_zeckendorf_part(n)`。这一步不会取完，并能把局面送到必败态。
> - **复杂度判定**：`O(log n)`。
> - **维护的量**：`f`（Zeckendorf 基 1,2,3,5,...）；`smallest`（n 的 Zeckendorf 分解最小项，即开局第一步应取量）。
> - **警告**：开局 `n` 是 Fibonacci 数时先手必败；一般状态 `(remain, quota)` 是否必胜，看 `quota` 是否不小于 `remain` 的 Zeckendorf 分解中最小 Fibonacci 项。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 开局：n 个石子，第一步不能取完，之后最多取对手上次取量的 2 倍，取最后者胜。
bool win = first_win_fibonacci_nim_start(10);  // 10 不是 Fibonacci 数 → 先手胜
i64 take = fibonacci_nim_first_take(10);       // 第一步取 Zeckendorf 最小项 2
bool mid = win_fibonacci_state(10, 3);         // 中间局面 (剩余10, 上限3) → 胜
// 样例：n=10 → win=true, take=2；n=8（Fibonacci 数）→ win=false, take=0。
```

**传参要求（照这个传不会错）：**

- `fibonacci_basis(n)`：返回 ≤ 相关上界的 Fibonacci 基（`vector<i64>`，1,2,3,5,...；内部函数，一般不用手动调）。
- `min_zeckendorf_part(n)`：返回 n 的 Zeckendorf 分解中最小项（`i64`，≥ 1）。
- `is_fibonacci_number_for_game(n)`：返回 `bool`，n 是否为（该基下的）Fibonacci 数。
- `first_win_fibonacci_nim_start(n)`：开局 n 时先手是否必胜，返回 `bool`。
- `fibonacci_nim_first_take(n)`：返回开局第一步应取数量（`i64`）；`0` 表示先手必败。
- `win_fibonacci_state(remain, quota)`：`remain` 剩余石子数、`quota` 当前最多可取的个数；返回 `bool`：`quota ≥ Zeckendorf 最小项` 时胜。


```cpp
vector<i64> fibonacci_basis(i64 n) {
    // 使用 1, 2, 3, 5, ...，避免 1 重复，适合 Zeckendorf 分解。
    vector<i64> f = {1, 2};
    while (f.back() <= n - f[(int)f.size() - 2]) {
        f.push_back(f.back() + f[(int)f.size() - 2]);
    }
    return f;
}

i64 min_zeckendorf_part(i64 n) {
    vector<i64> f = fibonacci_basis(n);
    i64 smallest = 0;
    for (int i = (int)f.size() - 1; i >= 0; --i) {
        if (f[i] <= n) {
            n -= f[i];
            smallest = f[i];
        }
    }
    return smallest;
}

bool is_fibonacci_number_for_game(i64 n) {
    vector<i64> f = fibonacci_basis(n);
    return binary_search(f.begin(), f.end(), n);
}

bool first_win_fibonacci_nim_start(i64 n) {
    return !is_fibonacci_number_for_game(n);
}

i64 fibonacci_nim_first_take(i64 n) {
    // 返回 0 表示先手必败；否则返回第一步应取数量。
    if (!first_win_fibonacci_nim_start(n)) return 0;
    return min_zeckendorf_part(n);
}

bool win_fibonacci_state(i64 remain, i64 quota) {
    // 普通中间局面：当前最多能取 quota 个。
    if (quota >= remain) return true;
    return quota >= min_zeckendorf_part(remain);
}
```
#### Wythoff 博弈：两堆 + 黄金分割

> **赛时先看**
> - **题目信号**：只有两堆；允许“取一堆”或“两堆取一样多”。
> - **本质**：两堆石子 `(a,b)`，每次可以从一堆取任意正数，或从两堆同时取相同正数，取走最后者胜。
> - **接法**：只判胜负时调用 `first_win_wythoff(a,b)`；需要输出一步时调用 `wythoff_move_to_losing`，返回的 pair 是取完后两堆数量。
> - **复杂度判定**：判断 `O(log A)`，给出一步转移 `O(log A)`。
> - **维护的量**：`d = b - a`（两堆差）；`wythoff_lower(d)`（= floor(d·phi) 的整数精确值，即必败态小堆数量）。
> - **警告**：必败态不是异或为 0，而是设 `a <= b, d = b-a`，满足 `a = floor(d * phi)`。下面用整数平方根算 `floor(d * phi)`，避免浮点误差；要求 `d <= 8e18`，足够覆盖常见 `1e18` 级题目。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 两堆 (a,b)：可任取一堆任意正数，或两堆同时取相同正数，取最后者胜。
bool win = first_win_wythoff(3, 5);        // d=2，floor(2*phi)=3=a → 必败
auto p = wythoff_move_to_losing(4, 6);     // 必胜态：一步取到必败态 {3,5}
// 样例：first_win_wythoff(3,5)=false；wythoff_move_to_losing(4,6) → {3,5}。
```

**传参要求（照这个传不会错）：**

- `first_win_wythoff(a, b)`：两堆石子数（`ui64`，顺序无所谓，内部会 swap 成 a ≤ b）；返回 `bool`：`true` 先手必胜。
- `wythoff_move_to_losing(a, b)`：参数同上；返回 `pair<ui64,ui64>`：取完后两堆的新数量（可能是两堆同减，也可能只减一堆）；已处于必败态时原样返回。
- `wythoff_p_position_with_value(value, max_d)`：找一个包含 `value` 的必败态，返回 `pair<ui64,ui64>`；找不到返回 `{ULLONG_MAX, ULLONG_MAX}`（内部函数，一般不用手动调）。
- 注意：`d = b - a` 须满足 `d ≤ 8e18`（内部有断言）。


```cpp
using u128 = __uint128_t;

ui64 isqrt_u128(u128 x) {
    ui64 lo = 0, hi = ULLONG_MAX;
    while (lo < hi) {
        ui64 mid = lo + (hi - lo + 1) / 2;
        if ((u128)mid * mid <= x) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

ui64 wythoff_lower(ui64 d) {
    // 公式：floor(d*phi)=floor((d+sqrt(5*d*d))/2)。
    if (d == 0) return 0;
    assert(d <= 8000000000000000000ULL);
    u128 x = (u128)5 * d * d;
    ui64 s = isqrt_u128(x);
    return (ui64)(((u128)d + s) / 2);
}

bool first_win_wythoff(ui64 a, ui64 b) {
    if (a > b) swap(a, b);
    ui64 d = b - a;
    return a != wythoff_lower(d);
}

pair<ui64, ui64> wythoff_p_position_with_value(ui64 value, ui64 max_d) {
    // 找一个包含 value 的必败态；没有则返回 {ULLONG_MAX, ULLONG_MAX}。
    ui64 lo = 1, hi = max_d;
    while (lo <= hi) {
        ui64 mid = lo + (hi - lo) / 2;
        ui64 x = wythoff_lower(mid);
        if (x == value) return {x, x + mid};
        if (x < value) lo = mid + 1;
        else hi = mid - 1;
    }
    lo = 1, hi = max_d;
    while (lo <= hi) {
        ui64 mid = lo + (hi - lo) / 2;
        ui64 x = wythoff_lower(mid), y = x + mid;
        if (y == value) return {x, y};
        if (y < value) lo = mid + 1;
        else hi = mid - 1;
    }
    return {ULLONG_MAX, ULLONG_MAX};
}

pair<ui64, ui64> wythoff_move_to_losing(ui64 a, ui64 b) {
    if (a > b) swap(a, b);
    ui64 d = b - a;
    ui64 x = wythoff_lower(d), y = x + d;
    if (a == x) return {a, b};          // 已经是必败态。
    if (a > x) return {x, y};           // 两堆同时取 a-x。
    auto target = wythoff_p_position_with_value(a, d - 1);
    if (target.first != ULLONG_MAX) return target; // 只减少另一堆。
    return {0, 0};                      // 兜底；正常必胜态不会需要。
}
```
#### Multi-SG：一步把一个子游戏拆成多个子游戏

> **赛时先看**
> - **题目信号**：题面说“切一刀后左右两段继续游戏”“删掉一个点后分裂成若干块”“一个状态会变成多个独立状态”。
> - **本质**：某个堆/区间/连通块经过一步操作后，会拆成若干个互不影响的小游戏，后继 SG 是这些新子游戏 SG 的异或。
> - **接法**：把 `gen_parts(x)` 写成“状态 x 一步后可能变成哪些子状态列表”，再调用这份模板。
> - **复杂度判定**：`O（状态数 * 每个状态可拆方案数 * 每个方案块数）`。
> - **维护的量**：`sg[x]`（状态 x 的 SG 值）；`next_sg`（每种拆法对应的子游戏异或值列表）。
> - **警告**：后继不是 `sg[next]`，而是所有拆出子局面的 SG 异或；如果拆出空块，通常忽略空块。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 例：一堆 x 个，一次可把 x 拆成 (i, x-i) 两堆继续玩；gen_parts 返回所有拆法。
auto gen_parts = [](int x) {
    vector<vector<int>> parts;
    for (int i = 1; i < x; ++i) parts.push_back({i, x - i});
    return parts;
};
vector<int> sg = sg_multi_game(10, gen_parts);
// 样例：拆两堆规则下 sg[5]=0 → 5 个石子先手必败。
```

**传参要求（照这个传不会错）：**

- `mex_values(vals)`：传入后继 SG 值列表（`vector<int>`），返回最小的未出现非负整数（`int`）。
- `sg_multi_game(max_state, gen_parts)`：`max_state` 最大状态编号（下标 1..max_state 都算，0 视为必败空块）；`gen_parts(x)` 是自定义函数/仿函数：入参状态 `x`（`int`），返回 `vector<vector<int>>`，其中每个内层 `vector` 是一种拆法、元素是拆出的子状态编号（`≤ 0` 的空块自动忽略）；返回 `vector<int> sg`，`sg[x] != 0` 先手必胜。


```cpp
int mex_values(vector<int> vals) {
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int g = 0;
    for (int x : vals) {
        if (x == g) g++;
        else if (x > g) break;
    }
    return g;
}

template <class GenParts>
vector<int> sg_multi_game(int max_state, GenParts gen_parts) {
    vector<int> sg(max_state + 1, 0);
    for (int x = 1; x <= max_state; ++x) {
        vector<int> next_sg;
        for (const vector<int>& parts : gen_parts(x)) {
            int xr = 0;
            for (int p : parts) {
                if (p > 0) xr ^= sg[p];
            }
            next_sg.push_back(xr);
        }
        sg[x] = mex_values(next_sg);
    }
    return sg;
}
```
#### 树的删边游戏：Green Hackenbush 常用版

> **赛时先看**
> - **题目信号**：题面是树；操作是删除一条边/切断一个子树；被切下来的部分不再参与游戏。
> - **本质**：有根树上每次删一条边，同时删掉这条边下面的整个子树；不能操作输。
> - **接法**：以地面/根为 `root`，DFS 求 `sg[u] = xor(sg[v] + 1)`；根的 SG 非 0 先手胜。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`g`（邻接表）；`sg[u]`（以 u 为根的子树删边游戏 SG 值）。
> - **警告**：每条儿子边贡献是 `sg[child] + 1`，不是单纯 `sg[child]`。多个儿子独立，整体异或。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 树上每次删一条边并丢掉下面整个子树，不能操作者输；根 SG 非 0 先手胜。
TreeCutGame tcg(3);
tcg.add_edge(1, 2);
tcg.add_edge(1, 3);
bool win = tcg.first_win(1);   // 两个叶子 sg=0 → sg[1]=(0+1)^(0+1)=0 → 败
// 样例：星形 1-2、1-3 → first_win(1)=false；再连一条 2-4 → sg[1]=(1+1)^1=3 → true。
```

**传参要求（照这个传不会错）：**

- `TreeCutGame(n)` / `init(n_)`：`n` 是点数（编号 1..n，1-index）；构造即初始化，清空邻接表与 sg。
- `add_edge(u, v)`：加一条无向边，`u/v` 都是 1-index 节点编号。
- `first_win(root)`：`root` 是树根编号（默认 1）；内部 DFS 后返回 `bool`：`true` 表示根的 SG 非 0、先手必胜。
- `sg`（公有成员）：DFS 结束后 `sg[u]` 存每个点子树的 SG 值，需要时可以自行读取。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：邻接表。
- `parent`：DFS 的父节点参数。
- `root`：树根（从哪个点开始 DFS）。

```cpp
struct TreeCutGame {
    int n;
    vector<vector<int>> g;
    vector<int> sg;

    TreeCutGame(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        sg.assign(n + 1, 0);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int parent) {
        sg[u] = 0;
        for (int v : g[u]) {
            if (v == parent) continue;
            dfs(v, u);
            sg[u] ^= (sg[v] + 1);
        }
    }

    bool first_win(int root = 1) {
        dfs(root, 0);
        return sg[root] != 0;
    }
};
```
#### 有向图游戏：胜 / 负 / 平局三态

> **赛时先看**
> - **题目信号**：状态图不是 DAG；可以循环；题面问 Win/Lose/Draw。
> - **本质**：棋子在有向图上移动，无法移动者输；图可能有环，因此除了先手胜/负，还可能平局。
> - **接法**：把每个局面编号成点，边表示一步操作；调用 `solve_directed_graph_game`，返回 `1=Win, -1=Lose, 0=Draw`。
> - **复杂度判定**：`O(n+m)`。
> - **维护的量**：`state[u]`（1 胜 / -1 败 / 0 平局或未知）；`degree[u]`（还有几个后继的状态未定）。
> - **警告**：从终止状态反推。正常规则下出度为 0 是必败；一个状态只要能走到必败就是必胜；所有后继都是必胜才是必败；剩下未定的是平局。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 节点 0-index，g[u] 存 u 一步能走到的点；走不了的人输，有环时可能平局。
vector<vector<int>> g = {{1}, {2}, {}};   // 0 → 1 → 2，2 是死点
vector<int> state = solve_directed_graph_game(g);
// 样例：链 0→1→2 → state = {-1, 1, -1}（0 败、1 胜、2 败）。
```

**传参要求（照这个传不会错）：**

- `solve_directed_graph_game(g)`：`g` 是 `vector<vector<int>>` 邻接表，节点编号 0-index（0..n-1），`g[u]` 存 u 可一步到达的节点列表（重复边无影响）；返回 `vector<int> state`，长度 = 节点数：`state[u] = 1` 表示先手必胜，`-1` 表示先手必败，`0` 表示平局（总能在环里绕、走不到死点）。


```cpp
vector<int> solve_directed_graph_game(const vector<vector<int>>& g) {
    int n = (int)g.size();
    vector<vector<int>> rg(n);
    vector<int> degree(n), state(n, 0); // 1 表示必胜，-1 表示必败，0 表示平局或未知。
    queue<int> q;
    for (int u = 0; u < n; ++u) {
        degree[u] = (int)g[u].size();
        if (degree[u] == 0) {
            state[u] = -1;
            q.push(u);
        }
        for (int v : g[u]) rg[v].push_back(u);
    }
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int p : rg[v]) {
            if (state[p] != 0) continue;
            if (state[v] == -1) {
                state[p] = 1;
                q.push(p);
            } else {
                if (--degree[p] == 0) {
                    state[p] = -1;
                    q.push(p);
                }
            }
        }
    }
    return state;
}
```
#### 二分图博弈：起点是否为最大匹配关键点

> **赛时先看**
> - **题目信号**：题面是二分图；每步移动并删除当前点；结论和最大匹配有关。
> - **本质**：棋子在二分图上，玩家每次沿边移动到邻点，并删除刚离开的点；无路可走者输。起点若在所有最大匹配中都被匹配，先手胜，否则先手败。
> - **接法**：先建二分图，算原图最大匹配 `base`；查询起点 `v` 时，删掉 `v` 后重算，若匹配数 `< base` 则先手胜。
> - **复杂度判定**：单次查询用本模板重算一次匹配，`O(E sqrt V)`；多起点大量查询时应使用 Dulmage-Mendelsohn 分解优化。
> - **维护的量**：`g`（左部邻接表）；`ml/mr`（左右部当前匹配边）；`dist`（HK 的 BFS 分层距离）；`nL/nR`（左右部点数）。
> - **警告**：不是看“当前某个最大匹配是否匹配起点”，而是看“所有最大匹配是否都匹配起点”。等价于删掉起点后最大匹配大小是否下降。


**最小完整示例（先抄这一段就能跑）：**

题目：棋子在二分图上沿边移动并删掉刚离开的点，无路可走者输；判断从起点出发先手是否必胜。

```cpp
HopcroftKarpGame game(nL, nR);           // 左部 nL 个点、右部 nR 个点
for (int i = 0; i < m; ++i) {
    int l, r; cin >> l >> r;
    game.add_edge(l, r);                 // 左 l 连右 r，0-based
}
bool win = game.first_win_start_left(l); // 先手从左部点 l 出发
cout << (win ? "First" : "Second") << '\n';
```

样例：仅边 `左0-右0`，起点 `左0` -> `win = true`（先手胜）。

**传参要求（照这个传不会错）：**

- `init(int left_size, int right_size)`：先调用（或构造时传参），编号 `0..left_size-1` / `0..right_size-1`。
- `add_edge(int l, int r)`：`l` 范围 `[0, nL)`，`r` 范围 `[0, nR)`，0-based；每条边调一次。
- `first_win_start_left(int l)`：起点在左部，`l` 范围 `[0, nL)`；返回 `true` 先手胜、`false` 后手胜。
- `first_win_start_right(int r)`：起点在右部，`r` 范围 `[0, nR)`；返回 `true` 先手胜、`false` 后手胜。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int l, int r)` -> 加入一条边
- `init(int left_size, int right_size)` -> 初始化/清空结构
**改板时先认这几个量：**

- `g`：左部邻接表。
- `dist`：Hopcroft-Karp 的 BFS 距离（增广层数）。

```cpp
struct HopcroftKarpGame {
    int nL = 0, nR = 0;
    vector<vector<int>> g;
    vector<int> dist, ml, mr;

    HopcroftKarpGame(int left_size = 0, int right_size = 0) {
        init(left_size, right_size);
    }

    void init(int left_size, int right_size) {
        nL = left_size;
        nR = right_size;
        g.assign(nL, {});
    }

    void add_edge(int l, int r) {
        // l 的范围为 [0,nL)，r 的范围为 [0,nR)。
        g[l].push_back(r);
    }

    bool bfs(int banned_l, int banned_r) {
        queue<int> q;
        dist.assign(nL, -1);
        for (int i = 0; i < nL; ++i) {
            if (i == banned_l) continue;
            if (ml[i] == -1) {
                dist[i] = 0;
                q.push(i);
            }
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                if (v == banned_r) continue;
                int to = mr[v];
                if (to == -1) found = true;
                else if (to != banned_l && dist[to] == -1) {
                    dist[to] = dist[u] + 1;
                    q.push(to);
                }
            }
        }
        return found;
    }

    bool dfs(int u, int banned_l, int banned_r) {
        for (int v : g[u]) {
            if (v == banned_r) continue;
            int to = mr[v];
            if (to == -1 || (to != banned_l && dist[to] == dist[u] + 1 && dfs(to, banned_l, banned_r))) {
                ml[u] = v;
                mr[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

    int max_matching_without(int banned_l = -1, int banned_r = -1) {
        ml.assign(nL, -1);
        mr.assign(nR, -1);
        int matching = 0;
        while (bfs(banned_l, banned_r)) {
            for (int i = 0; i < nL; ++i) {
                if (i != banned_l && ml[i] == -1 && dfs(i, banned_l, banned_r)) {
                    matching++;
                }
            }
        }
        return matching;
    }

    bool first_win_start_left(int l) {
        int base = max_matching_without();
        return max_matching_without(l, -1) < base;
    }

    bool first_win_start_right(int r) {
        int base = max_matching_without();
        return max_matching_without(-1, r) < base;
    }
};
```
#### 随机数

> **赛时先看**
> - **题目信号**：需要打乱、随机选 pivot、Pollard-Rho。
> - **本质**：随机化算法、造数据、哈希种子。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：无额外结构；只维护全局随机引擎 `rng`（mt19937）/`rng64`（mt19937_64），种子取自系统时钟。
> - **警告**：不要每次调用都重新构造 rng。


**最小完整示例（先抄这一段就能跑）：**

题目：生成闭区间随机数，并原地打乱数组。

```cpp
int x = rand_int(1, 6);                     // 1. [1,6] 闭区间随机整数，含两端
i64 y = rand_ll(1, 1000000000000000000LL);  // 2. [1,1e18] 闭区间随机 i64
shuffle(a.begin(), a.end(), rng);           // 3. 原地打乱数组，直接传全局 rng
```

样例：`rand_int(1, 6)` 输出 1~6 中的随机一个数；每次运行结果都不同。

**传参要求（照这个传不会错）：**

- `rand_int(l, r)`：闭区间 `[l, r]` 的随机整数，含两端；返回 `int`。
- `rand_ll(l, r)`：闭区间 `[l, r]` 的随机整数，含两端；返回 `i64`（r 大到 9e18 也没问题）。
- `rng` / `rng64`：全局随机引擎，可直接传给 `shuffle(a.begin(), a.end(), rng)`；全局只有一个，不要在函数里重复构造。

```cpp
mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());
mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());

int rand_int(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

i64 rand_ll(i64 l, i64 r) {
    return uniform_int_distribution<i64>(l, r)(rng64);
}
```
#### 爬山法：邻域贪心近似搜索

> **赛时先看**
> - **题目信号**：解空间连续或离散但很大；目标函数能快速计算；题面允许近似答案（浮点、带误差、special judge），例如找平面上到若干点距离和最小的点（费马点类）。
> - **本质**：从一个初始解出发，每步在邻域内生成候选，只接受更优的解，迭代固定步数后返回最优。
> - **接法**：把 `neighbor(cur, rng)` 改成自己的扰动生成（连续解用正态/均匀小扰动，离散解用交换/翻转一位）；`score(x)` 越小越好。多随机起点各爬一遍再取全局最优，效果远好于单起点。
> - **复杂度判定**：`O（步数 * 单次评分）`。
> - **维护的量**：`best`（当前最优解，也是返回值）；`best_score`（当前最优评分，`score` 越小越好）；`rng`（随机引擎，喂给 `neighbor` 做扰动）；`climb_steps`（迭代步数，默认 10000）。
> - **警告**：会陷入局部最优；解的维度大或目标函数有很多局部极小值时先想模拟退火；评分函数必须能快速重算，否则步数开不大。


**最小完整示例（先抄这一段就能跑）：**

题目：平面上 `n` 个点，求到它们欧氏距离和最小的点（输出坐标，允许近似）。直接抄下面的费马点写法：

```cpp
vector<pair<double, double>> pts;  // 读入 n 个点的坐标
auto score = [&](const pair<double, double>& p) {
    double s = 0;
    for (auto [x, y] : pts) s += hypot(p.first - x, p.second - y);
    return s;
};
auto neighbor = [&](const pair<double, double>& cur, mt19937& rng) {
    uniform_real_distribution<double> d(-1.0, 1.0);   // 步长 1.0，可随手调
    return pair<double, double>{cur.first + d(rng), cur.second + d(rng)};
};
auto ans = hill_climb(pts[0], score, neighbor, 50000);  // 调用：爬 5e4 步
cout << fixed << setprecision(6) << ans.first << ' ' << ans.second << '\n';
```

**传参要求（照这个传不会错）：**

- `hill_climb(start, score, neighbor, climb_steps)`：
  - `start`：初始解（任意类型 `State`，结构体、`pair`、`vector` 都行）。
  - `score(x)`：目标函数，返回 `double`，**越小越好**；只用于比较，量级无所谓。
  - `neighbor(cur, rng)`：在 `cur` 附近生成一个候选解，`rng` 是 `mt19937&`，直接用它扰动，不要自己另造随机引擎。
  - `climb_steps`：迭代步数，默认 10000；时限松可加大。
  - 返回值：全程 `score` 最小的那个解（`State`）。

```cpp
// 爬山法：score 越小越好；neighbor(cur, rng) 生成一个邻域候选。
// 多起点：对若干个随机 start 各跑一次 hill_climb，取 score 最小者。
template <class State, class Score, class Neighbor>
State hill_climb(State start, Score score, Neighbor neighbor, int climb_steps = 10000) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    State best = start;
    double best_score = score(start);
    for (int step = 0; step < climb_steps; ++step) {
        State cand = neighbor(best, rng);
        double s = score(cand);
        if (s < best_score) {
            best_score = s;
            best = cand;
        }
    }
    return best;
}
```
#### 模拟退火 Simulated Annealing

> **赛时先看**
> - **题目信号**：爬山容易卡局部最优；题目允许近似解或答案带误差（浮点几何最优位置、带权点集选点、排列/划分近似优化）；题面出现"误差不超过""近似""special judge 浮点"。
> - **本质**：爬山的基础上，按温度概率接受更差的解来跳出局部最优；温度从高到低，接受概率 `exp(-delta/t)`，让搜索先"乱逛"后"收敛"。
> - **复杂度判定**：`O(log_{cooling}(T_min/T0) * 单次评分）`；评分 O(n) 时把总迭代控制在 `1e5 ~ 1e6` 级别。若题目要求精确答案（误差 1e-9 且数据特殊构造），不要用随机近似，先想三分/凸优化/精确公式。
> - **维护的量**：当前解 `cur`、当前最优 `best`、当前温度 `t`（`T0` 起按 `cooling` 衰减到 `T_min`）。
> - **接法**：把 `neighbor(cur, t, rng)` 改成自己的扰动生成（扰动半径随 `t` 缩小），`score(x)` 越小越好，然后 `simulated_annealing(start, score, neighbor, T0, T_min, cooling)`。
> - **警告**：输出必须按题面精度格式化；比赛固定随机种子可复现；多随机起点各跑一次取最优，效果远好于单起点。


**最小完整示例（先抄这一段就能跑）：**

题目：平面 `n` 个点，求到它们欧氏距离和最小的点（输出坐标，允许误差）。直接抄下面的费马点示例即可：

```cpp
vector<pair<double, double>> pts;
for (int i = 0; i < n; ++i) { double x, y; cin >> x >> y; pts.push_back({x, y}); }
AnnealPoint ans = fer_mat_point_sa(pts);      // 1. 调用：返回近似最优坐标
cout << fixed << setprecision(6) << ans.x << ' ' << ans.y << '\n';  // 2. 按题面精度输出
```

**传参要求（照这个传不会错）：**

- `simulated_annealing(start, score, neighbor, T0, T_min, cooling)`：通用模板。
  - `start`：初始解；`score(x)`：目标函数，**越小越好**。
  - `neighbor(cur, t, rng)`：生成邻域候选（扰动幅度随温度 `t` 缩小）。
  - `T0/T_min/cooling`：起止温度与降温系数（`0.98 ~ 0.999`）。
  - 返回值：过程中见过的最优解（`score` 最小者）。
- `fer_mat_point_sa(pts, restarts=8)`：费马点专用封装；`pts` 为点坐标列表。
- 输出前按题面精度 `fixed << setprecision(...)`；多起点取最优。
- 题目要求精确答案时不要用随机近似。

**抄板清单（照着做就行）：**
1. 抄哪段：`simulated_annealing` 模板函数（需要坐标扰动示例时连 `AnnealPoint`/`fer_mat_point_sa` 一起抄）。
2. 构造：定义 `score(x)`（越小越好）与 `neighbor(cur, t, rng)`（在当前解附近按温度扰动）。
3. 调用：`auto best = simulated_annealing(start, score, neighbor, 10000, 1e-8, 0.999);`
4. 取结果：`score(best)` 或 `best` 本身按题面输出；多起点取最优。

**改造点（按题目改这几处）：**
- 扰动生成：连续解用 `normal_distribution(0, t)`；离散解用"随机交换/翻转一位"，半径或步长随 `t` 缩小。
- 参数：`T0 = 1e3 ~ 1e5`、`T_min = 1e-8 ~ 1e-12`、`cooling = 0.98 ~ 0.999`（时限紧取小，时限松取大）。
- 起点：从题面给的或随机点起步；`restarts` 个起点各跑一遍取最优。
- 输出精度：`cout << fixed << setprecision(...)` 按题面位数输出。

**核心逻辑（改代码时别破坏）：**

- 接受条件：`delta <= 0` 必接受，否则以 `exp(-delta / t)` 概率接受——这是跳出局部最优的关键。
- `best` 记录的是过程中见过的最优解，不是最终 `cur`。

**改板时先认这几个量：**

- `cur`：当前解。
- `best`：历史最优解（返回值）。
- `t`：当前温度，控制扰动幅度与接受概率。

```cpp
// 模拟退火：score 越小越好；返回过程中见过的最优解。
// neighbor(cur, t, rng)：在当前温度 t 下生成邻域候选。
template <class State, class Score, class Neighbor>
State simulated_annealing(State start, Score score, Neighbor neighbor,
                          double T0 = 10000, double T_min = 1e-8, double cooling = 0.999) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> unit(0.0, 1.0);
    State cur = start, best = start;
    double cur_score = score(cur), best_score = cur_score;
    for (double t = T0; t > T_min; t *= cooling) { // 温度按几何级数下降。
        State cand = neighbor(cur, t, rng);
        double s = score(cand);
        double delta = s - cur_score;
        // 更优必接受；更差以概率 exp(-delta/t) 接受，温度越低越难接受。
        if (delta <= 0 || unit(rng) < exp(-delta / t)) {
            cur = cand;
            cur_score = s;
            if (s < best_score) { // 随时记录历史最优，避免退火后期把最优解丢掉。
                best_score = s;
                best = cand;
            }
        }
    }
    return best;
}

// 典题示例：平面 n 个点，求到它们欧氏距离和最小的近似点（费马点类）。
struct AnnealPoint {
    double x, y;
};

AnnealPoint fer_mat_point_sa(const vector<pair<double, double>>& pts, int restarts = 8) {
    auto score = [&](const AnnealPoint& p) { // 距离和：越小越好。
        double s = 0;
        for (auto [x, y] : pts) {
            double dx = p.x - x, dy = p.y - y;
            s += sqrt(dx * dx + dy * dy);
        }
        return s;
    };
    auto neighbor = [&](const AnnealPoint& p, double t, auto& rng) { // 扰动半径随温度缩小。
        normal_distribution<double> nd(0.0, t);
        return AnnealPoint{p.x + nd(rng), p.y + nd(rng)};
    };
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<i64> pick(-1000000, 1000000);
    AnnealPoint best{0, 0};
    double best_score = 1e300;
    for (int r = 0; r < restarts; ++r) { // 多随机起点取最优。
        AnnealPoint start{pick(rng) / 1e3, pick(rng) / 1e3};
        AnnealPoint got = simulated_annealing(start, score, neighbor, 1000, 1e-7, 0.998);
        double s = score(got);
        if (s < best_score) {
            best_score = s;
            best = got;
        }
    }
    return best;
}
```

典题模型：坐标范围大的几何最优位置、带权聚类中心、近似排列优化（配合邻域交换）。如果题面要求精确答案（误差 1e-9 且数据是特殊构造），不要用随机近似，先想三分、凸优化或精确公式。
#### 归并排序求逆序对

> **赛时先看**
> - **题目信号**：问至少相邻交换多少次，或逆序数量。
> - **本质**：统计逆序对，不依赖值域。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n log n)`。
> - **维护的量**：`a`（正在被排序的区间，传值进入时调用方数组不受影响）；`tmp`（合并用临时数组）；`ans`（累计的逆序对数）。
> - **警告**：相等元素不算逆序时，合并条件用 `<=`。


**最小完整示例（先抄这一段就能跑）：**

题目：数组 `[3, 1, 4, 2]` 有多少个逆序对（逆序对 = 前大后小的数对）。

```cpp
vector<i64> a = {3, 1, 4, 2};                // 1. 原数组，0-based
i64 ans = count_inversions_merge(a);         // 2. 传值调用，返回后 a 仍保持原样
cout << ans << '\n';                         // 3. 输出逆序对数
```

样例：输出 `3`（逆序对为 (3,1)、(3,2)、(4,2)）。

**传参要求（照这个传不会错）：**

- `count_inversions_merge(a)`：传整个原数组 `vector<i64>`（值传递，原数组不会被改）；返回 `i64` 逆序对数。
- `merge_count(a, tmp, l, r)`：内部递归函数，区间是 `[l, r)` 半开（左闭右开），默认不要从 solve() 直接调。
- 下标从 0 开始；相等元素不算逆序（合并条件用的是 `<=`）；n 到 1e6 都能跑。

```cpp
i64 merge_count(vector<i64>& a, vector<i64>& tmp, int l, int r) {
    if (r - l <= 1) return 0;
    int m = (l + r) >> 1;
    i64 ans = merge_count(a, tmp, l, m) + merge_count(a, tmp, m, r);
    int i = l, j = m, k = l;
    while (i < m || j < r) {
        if (j == r || (i < m && a[i] <= a[j])) tmp[k++] = a[i++];
        else {
            tmp[k++] = a[j++];
            ans += m - i;
        }
    }
    for (int t = l; t < r; ++t) a[t] = tmp[t];
    return ans;
}

i64 count_inversions_merge(vector<i64> a) {
    vector<i64> tmp(a.size());
    return merge_count(a, tmp, 0, (int)a.size());
}
```
#### 五张牌牌型比较：德州扑克基础评估

> **赛时先看**
> - **题目信号**：题目给扑克牌花色和点数，要求比较五张牌；需要枚举未知牌后的最优/必胜/平局。
> - **本质**：评估五张牌牌型并比较大小，适合小规模枚举暗牌、补牌、换牌后的胜负。
> - **接法**：把每张牌转成 `(rank, suit)`，调用 `evaluate_five_cards` 得到可比较的向量；枚举双方可能暗牌后用返回值比较。
> - **复杂度判定**：单手牌评估 `O(5 log 5)`；枚举暗牌按可选牌数量额外乘上常数。
> - **维护的量**：`PokerCard{rank, suit}`（每张牌的点数 2..14、花色 0..3）；`PokerScore{type, key}`（牌型级别 0..8 与同牌型比较键 key）。
> - **警告**：A2345 是最小顺子；同牌型比较先比主要牌，再比踢脚。题面如果有七选五，要枚举 `C(7,5)` 个五张组合取最大。


**最小完整示例（先抄这一段就能跑）：**

题目：比较两手牌：A K Q J 9 同花 vs A2345 同花顺，谁更大。

```cpp
vector<PokerCard> a = {{14, 0}, {13, 0}, {12, 0}, {11, 0}, {9, 0}};  // 1. 同花：A K Q J 9
vector<PokerCard> b = {{14, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}};     // 2. A2345 同花顺（最小顺子）
PokerScore sa = evaluate_five_cards(a);
PokerScore sb = evaluate_five_cards(b);   // 3. 分别评估
cout << (sa < sb) << '\n';                // 4. type 8 同花顺 > type 5 同花
```

样例：输出 `1`（A2345 同花顺更大，牌型级别 8 > 5）。

**传参要求（照这个传不会错）：**

- `PokerCard`：`rank` 点数 `2..14`（A 记 14、10 记 10）；`suit` 花色 `0..3`，只要能区分四种花色即可。
- `evaluate_five_cards(cards)`：传 5 张牌（`vector<PokerCard>`，顺序任意）；返回 `PokerScore`。
- `PokerScore.type`：`0`=高牌、`1`=一对、`2`=两对、`3`=三条、`4`=顺子、`5`=同花、`6`=葫芦、`7`=四条、`8`=同花顺，越大越强；`key` 从主牌到踢脚降序。
- 比较：直接写 `sa < sb` / `sa == sb`；A2345 这种最小顺子/同花顺时 `key[0]` 是 5。
- 七选五的题：枚举 `C(7,5)` 种组合各评估一次取最优。

```cpp
struct PokerCard {
    int rank = 0; // 点数范围 2..14，其中 A 记作 14。
    int suit = 0; // 说明：0..3
};

struct PokerScore {
    int type = 0;
    vector<int> key;
    bool operator<(const PokerScore& other) const {
        if (type != other.type) return type < other.type;
        return key < other.key;
    }
    bool operator==(const PokerScore& other) const {
        return type == other.type && key == other.key;
    }
};

PokerScore evaluate_five_cards(vector<PokerCard> cards) {
    sort(cards.begin(), cards.end(), [](const PokerCard& a, const PokerCard& b) {
        return a.rank < b.rank;
    });
    vector<int> ranks;
    for (auto c : cards) ranks.push_back(c.rank);

    bool flush = true;
    for (int i = 1; i < 5; ++i) flush &= cards[i].suit == cards[0].suit;

    bool straight = false;
    int straight_high = ranks[4];
    if (ranks == vector<int>{2, 3, 4, 5, 14}) {
        straight = true;
        straight_high = 5;
    } else {
        straight = true;
        for (int i = 1; i < 5; ++i) straight &= ranks[i] == ranks[i - 1] + 1;
    }
    if (straight && flush) return {8, {straight_high}};

    map<int, int, greater<int>> count;
    for (int r : ranks) count[r]++;
    vector<pair<int, int>> groups;
    for (auto [rank, cnt] : count) groups.push_back({cnt, rank});
    sort(groups.begin(), groups.end(), [](auto a, auto b) {
        if (a.first != b.first) return a.first > b.first;
        return a.second > b.second;
    });

    if (groups[0].first == 4) return {7, {groups[0].second, groups[1].second}};
    if (groups[0].first == 3 && groups[1].first == 2) return {6, {groups[0].second, groups[1].second}};
    if (flush) {
        sort(ranks.rbegin(), ranks.rend());
        return {5, ranks};
    }
    if (straight) return {4, {straight_high}};
    if (groups[0].first == 3) {
        vector<int> key{groups[0].second};
        for (auto [cnt, rank] : groups) if (cnt == 1) key.push_back(rank);
        return {3, key};
    }
    if (groups[0].first == 2 && groups[1].first == 2) {
        int high_pair = max(groups[0].second, groups[1].second);
        int low_pair = min(groups[0].second, groups[1].second);
        int kicker = groups[2].second;
        return {2, {high_pair, low_pair, kicker}};
    }
    if (groups[0].first == 2) {
        vector<int> key{groups[0].second};
        for (auto [cnt, rank] : groups) if (cnt == 1) key.push_back(rank);
        return {1, key};
    }
    sort(ranks.rbegin(), ranks.rend());
    return {0, ranks};
}
```

典题：2026 牛客多校 Round1 J《Show Hand》。先枚举法国赌神暗牌和自己的暗牌，用牌型评估器找出各自最优/次优暗牌，再做必胜/必败/平局分类。
#### Josephus 约瑟夫问题

> **赛时先看**
> - **题目信号**：经典报数出圈。
> - **本质**：环上每次数 `k` 删除一个人，求最后剩下的位置。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`ans`（当前圈长 i 时最后幸存者的 0-based 位置），从 i=2 迭代到 n 递推。
> - **警告**：这个函数返回 0-based 位置，输出 1-based 要加 1。


**最小完整示例（先抄这一段就能跑）：**

题目：7 个人围成环报数，每数到 3 出圈，问最后剩下第几个人。

```cpp
i64 n = 7, k = 3;                     // 1. 7 个人，每数到 3 出圈
i64 pos0 = josephus_zero_based(n, k); // 2. 返回 0-based 位置
cout << pos0 + 1 << '\n';             // 3. 输出 1-based 记得加 1
```

样例：输出 `4`（0-based 位置是 3，+1 后是第 4 个人）。

**传参要求（照这个传不会错）：**

- `josephus_zero_based(n, k)`：`n` 个人（n >= 1）围成环，每数到 `k` 出圈；返回最后幸存者的 0-based 位置（`0..n-1`）。
- 时间 `O(n)`，n 到 1e7 都能跑；n 到 1e9 以上时不要用本模板，要翻快跳版 Josephus。
- 输出前想清楚题面要 0-based 还是 1-based：要 1-based 就在返回值上加 1。

```cpp
i64 josephus_zero_based(i64 n, i64 k) {
    i64 ans = 0;
    for (i64 i = 2; i <= n; ++i) ans = (ans + k) % i;
    return ans;
}
```
#### 日期：基姆拉尔森公式

> **赛时先看**
> - **题目信号**：日历、星期几、日期推算。
> - **本质**：根据年月日求星期。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：无额外结构；只做一次公式换算（内部把 1、2 月转成上一年的 13、14 月再套基姆拉尔森公式）。
> - **警告**：1 月和 2 月按上一年的 13、14 月处理。


**最小完整示例（先抄这一段就能跑）：**

题目：2000 年 1 月 1 日和 2026 年 8 月 12 日分别是星期几。

```cpp
int w1 = weekday(2000, 1, 1);    // 1. 直接传年月日
int w2 = weekday(2026, 8, 12);   // 2. 1、2 月不用自己处理，函数内部会转
cout << w1 << ' ' << w2 << '\n'; // 3. 输出 0=Sunday, ..., 6=Saturday
```

样例：输出 `6 3`（2000-01-01 是星期六；2026-08-12 是星期三）。

**传参要求（照这个传不会错）：**

- `weekday(y, m, d)`：`y` 年、`m` 月（1..12）、`d` 日（按当月天数）；返回 `0=Sunday, 1=Monday, ..., 6=Saturday`。
- 1 月和 2 月自动按上一年 13、14 月处理，调用方不要自己减年。
- 任意合理公历日期都行，无特殊下界；返回 `w` 一定落在 `[0, 6]`。

**API / 入口函数（赛时只认这里列的名字）：**

- `weekday(int y, int m, int d)` -> 返回 0=Sunday, 1=Monday, ..., 6=Saturday。

```cpp
// 返回 0=Sunday, 1=Monday, ..., 6=Saturday
int weekday(int y, int m, int d) {
    if (m == 1 || m == 2) {
        m += 12;
        y--;
    }
    int c = y / 100;
    int yy = y % 100;
    int w = (yy + yy / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + d - 1) % 7;
    return (w + 7) % 7;
}
```
#### STL 常用函数速查

> **赛时先看**
> - **题目信号**：排序、去重、二分、排列、累加。
> - **本质**：比赛时避免重复造轮子。
> - **复杂度判定**：按函数而定。
> - **维护的量**：无；速查表本身不建结构，注意 `unique` 必须配 `erase` 才真正改变容器长度。
> - **警告**：`unique` 只移动元素，不会改变容器长度，后面要 `erase`。


**最小完整示例（先抄这一段就能跑）：**

题目：数组去重后二分查找，求第一个 `>= 3` 的下标。

```cpp
vector<int> a = {3, 1, 2, 1, 3};                // 1. 原数组
sort(a.begin(), a.end());
a.erase(unique(a.begin(), a.end()), a.end());   // 2. 原地去重后 a = {1,2,3}
bool ok = binary_search(a.begin(), a.end(), 2); // 3. 二分查找，找到返回 true
int pos = lower_bound(a.begin(), a.end(), 3) - a.begin();  // 4. 第一个 >=3 的 0-based 下标
```

样例：`a` 变成 `{1,2,3}`，`ok = true`，`pos = 2`。

**传参要求（照这个传不会错）：**

- `sort(a.begin(), a.end())`：升序排序；要降序加第三参 `greater<int>()`。
- `erase(unique(a.begin(), a.end()), a.end())`：原地去重，返回后容器长度才真正变小；前提是先排序。
- `binary_search(a.begin(), a.end(), x)`：`x` 是否出现（前提已排序），返回 `bool`。
- `lower_bound(a.begin(), a.end(), x) - a.begin()`：第一个 `>= x` 的 0-based 下标；`upper_bound` 是第一个 `> x` 的下标。
- `iota(a.begin(), a.end(), 0)`：从 0 开始递增填充；`accumulate(a.begin(), a.end(), 0LL)`：求和，累加初值写 `0LL` 才保 i64。
- `next_permutation / prev_permutation`：原地换成下一个/上一个字典序排列，返回 `bool` 表示是否还有；`shuffle(a.begin(), a.end(), rng)`：按全局 `rng` 原地打乱。

**API / 入口函数（赛时只认这里列的名字）：**

- `erase(unique(a.begin(), a.end()), a.end())` -> 删除 `unique` 去重后末尾的多余元素，完成原地去重。

```cpp
sort(a.begin(), a.end());
a.erase(unique(a.begin(), a.end()), a.end());

bool ok = binary_search(a.begin(), a.end(), x);
int pos1 = lower_bound(a.begin(), a.end(), x) - a.begin(); // 第一个 >= x 的位置。
int pos2 = upper_bound(a.begin(), a.end(), x) - a.begin(); // 第一个 > x 的位置。

iota(a.begin(), a.end(), 0);
i64 sum = accumulate(a.begin(), a.end(), 0LL);

next_permutation(a.begin(), a.end());
prev_permutation(a.begin(), a.end());

shuffle(a.begin(), a.end(), rng);
```

### 17 近期训练赛模型补充

近期多校、萌新赛和补题时抽出的短模型放在这里。它们不是独立大专题，但很适合赛后复盘和网络赛前快速扫一遍。
#### 赛后模型总览：2026 牛客暑期多校训练营 1

> **赛时先看**
> - **题目信号**：刚 VP / 补 2026 牛客暑期多校训练营 1，想知道每题该沉淀到哪个专题。
> - **本质**：把本场题目映射到可复用板子，补题时先从这里定位。
> - **复杂度判定**：这是索引页，不涉及算法复杂度。
> - **警告**：A/E/F/G 更偏签到、构造或观察；真正建议长期复用的是 B/C/H/I/J/K/L 的模型。


题型归档：

- A：字符串位置奇偶规则检查。
- E/F：排列差值贡献与全体加同余不变量。
- G：构造题，把二维密铺的点放到两个平面。
- B：极坐标圆弧/径向关键点建图 + Dijkstra。
- C：按权增量加入点，隐式 Kruskal 重构树维护吞并阈值。
- H：有限状态零和 MDP，值迭代到稳定后按平均收益外推大轮数。
- I：找一条“端点权不同且在同一 DSU 内”的非树边制造好环，再 BFS 定向。
- J：枚举暗牌 + 五张牌牌型比较 + 简单 minmax 分类。
- K：简单多边形耳切三角剖分，三角形对的闵可夫斯基和并集面积。
- L：AC 自动机批量收集所有询问串出现位置，再按出现位置分段统计所有覆盖区间的最大和与总和。

#### 牛客多校 Round1 具体模型索引

> **赛时先看**
> - **题目信号**：补牛客暑期多校第一场但想按题号找到对应板子；或者只记得题号，不记得模型名。
> - **本质**：把 Round1 每个值得沉淀的题目指向正文中已经归类的板子。正文仍按知识点存放模板，本节只做赛后复盘索引，避免同一份代码在打印版重复出现。
> - **复杂度判定**：索引页不涉及算法复杂度。
> - **警告**：Round1 的具体算法模板已经归入对应知识点章节，本节不要再重复复制代码；真正写题时去对应模板页抄。


题号到模板：

- B：极坐标圆弧图最短路：关键角度建图。
- C：隐式 Kruskal 重构树：增量网格连通块吞并阈值。
- H：有限状态零和 MDP：值迭代差分收敛外推。
- I：非树边造好环 + BFS 定向构造。
- J：五张牌牌型比较：德州扑克基础评估。
- K：简单多边形耳切三角剖分；三角形对闵可夫斯基和面积并可再翻“凸多边形闵可夫斯基和”和几何面积模板。
- L：AC 自动机：批量收集所有模式串出现位置；典题：子串出现覆盖的所有区间最大权和与总权和。

补题建议：先按这张表定位模板，再回题面重写“题面对象 -> 模板参数”的翻译。C/L/K 是最值得复盘的三题，分别对应动态图结构、字符串出现位置统计和计算几何拆分。

#### 赛后模型总览：2026 牛客暑期多校训练营 2

> **赛时先看**
> - **题目信号**：只记得题号或题面关键词，不确定应该翻数学、图论、构造、DP 还是随机近似。
> - **本质**：补 Round2 时先按题号定位模型，再去下方模板或全册对应专题查实现。
> - **复杂度判定**：索引页不涉及算法复杂度。
> - **警告**：J 是重实现题，不建议赛场临时复刻；C 是分类讨论题，建议用小范围暴力打表反查边界；A/G/K 是“先找结构再压缩规模”的好题。


题号到模型：

- A《Annoying Traffic》：网格期望等待，沿对角线转化为带吸收边界的随机游走，组合数求首次撞边贡献。
- B《Bitwise Maximization》：两个集合异或和之和最大化；总异或固定，剩余位用线性基最大异或。
- C《Competition: Winning Streaks》：羽毛球比分连胜长度最大/最小，按普通区、终局、超分区分段讨论。
- D《Delivering Newspapers》：固定 1 到 2 的路径，把挂树按层生长计数，标号组合 DP。
- E《Easy Puzzle》：网格画同心矩形边，偶数短边时拆成若干矩形块。
- F《Fabulous Tree》：边权等于点权差绝对值，树形 DP 维护根相对值域。
- G《GCD Graph》：质数间隙很小，远处答案只分 1/2，近处用短区间 DP。
- H《Hyperspace Pairing》：超立方删两点后按 Hamming 距离 2 配对，奇偶性判定 + 位重映射构造。
- I《Imperfect Dot Sums and Cross Sums》：允许 10% 相对误差，按查询向量方向分组，用前缀和近似回答。
- J《Just Round It》：随机位四舍五入期望，连续 4 进位链转生成函数系数，NTT 批量求。
- K《Kindergarten》：只有少量特殊边可变，普通边全同权；全源 BFS 后压缩到特殊端点 + 查询端点上跑小图 Dijkstra。
- L《Lazy Shuffling》：排列逆序对约束变成偏序图，状压 DP 计数拓扑序。
- M《Maybe Connected》：给定边数最大化“连通但未直接相连”点对，大连通块 + 孤点最优。
- N《Narrow to Median》：排序后选择一个长度 `k` 子序列替换为中位数，前缀和枚举中位位置。

#### 两集合异或和最大：总 XOR 固定位 + 线性基

> **赛时先看**
> - **题目信号**：每个元素二选一进入两个集合之一；两个集合 xor 记为 `x,y`，必有 `x xor y = total_xor`；要求最大化 `x+y`。
> - **本质**：把数组分成两个集合，使两个集合的异或和之和最大。
> - **接法**：每个数先 `a[i] &= ~total_xor`，只保留可争取的位；线性基求这些数能异或出的最大值 `best`，答案为 `total_xor + 2 * best`。
> - **复杂度判定**：`O(n log V)`。
> - **维护的量**：`basis[b]`（线性基第 b 位主元）；`total_xor`（全局异或，固定贡献）。
> - **警告**：`total_xor` 中为 1 的位对 `x+y` 贡献固定；只有 `total_xor` 中为 0 的位可通过让 `x,y` 同时为 1 多贡献一倍。


**最小完整示例（先抄这一段就能跑）：**

> 题目：把 `{2,5,6,7}` 分成两堆，最大化两堆异或和之和。

```cpp
vector<int> a = {2, 5, 6, 7};
printf("%lld\n", max_two_subset_xor_sum(a)); // total_xor=6 固定，剩余位取线性基最大
```

> 样例输出：`8`

**传参要求（照这个传不会错）：**

- `a`：原数组，元素 `0..2^30`，下标 0 起，长度任意。返回：最大 `x+y`（`long long`）。

**API / 入口函数（赛时只认这里列的名字）：**

- `insert(int x)` -> 把一个数插入线性基
- `query_max()` -> 查询线性基能异或出的最大值，返回 `int`。

```cpp
struct XorBasisMax {
    static constexpr int LOG = 30;
    int basis[LOG]{};

    void clear() {
        memset(basis, 0, sizeof(basis));
    }

    void insert(int x) {
        for (int b = LOG - 1; b >= 0; --b) {
            if (((x >> b) & 1) == 0) continue;
            if (basis[b]) x ^= basis[b];
            else {
                basis[b] = x;
                return;
            }
        }
    }

    int query_max() const {
        int res = 0;
        for (int b = LOG - 1; b >= 0; --b) {
            res = max(res, res ^ basis[b]);
        }
        return res;
    }
};

long long max_two_subset_xor_sum(vector<int> a) {
    int total_xor = 0;
    for (int x : a) total_xor ^= x;

    XorBasisMax lb;
    for (int x : a) lb.insert(x & ~total_xor);

    long long best = lb.query_max();
    return total_xor + 2LL * best;
}
```

典题：Round2 B《Bitwise Maximization》。如果题目变成“分成两堆后最大化按位 OR/AND”，这套 xor 固定位思路不能直接用。
#### 逆序约束拓扑序计数：状压 DP

> **赛时先看**
> - **题目信号**：`n <= 22`；所有约束都是若干二元大小关系；两种相反方向的偏序答案一一对应。
> - **本质**：给一个排列 `p`，统计有多少个排列 `A` 能让打乱前后逆序对差的绝对值最大。
> - **接法**：对每个逆序对 `(i,j)`，固定其中一种方向，把 `j` 作为 `i` 的前置条件。`dp[S]` 表示已经放入集合 `S` 的拓扑序数量。
> - **复杂度判定**：`O(n 2^n)`，空间 `O(2^n)`。
> - **维护的量**：`need[u]`（u 必须晚于这些元素放置的掩码）；`dp[mask]`（已放入集合 mask 的拓扑序数量）。
> - **警告**：若 `p` 本身没有逆序对，每个 `A` 都满足，答案是 `n!`；否则两个相反方向不重叠，最后乘 2。


**最小完整示例（先抄这一段就能跑）：**

> 题目：`p = {2,1,3}`，统计打乱后使逆序对差绝对值最大的排列数（1-based 排列）。

```cpp
printf("%d\n", count_lazy_shuffling_orders({2, 1, 3})); // 约束 1 排在 2 后，dp 计数乘 2
```

> 样例输出：`6`

**传参要求（照这个传不会错）：**

- `p`：1..n 的排列，下标 0 起，`n<=22`。返回：方案数 mod 998244353；`p` 已升序（无逆序对）时返回 `n!`。

**改板时先认这几个量：**

- `nxt`：转移/子节点。
- `dp`：DP 状态。

```cpp
int count_lazy_shuffling_orders(const vector<int>& p) {
    const int MOD = 998244353;
    int n = (int)p.size();

    bool identity = true;
    for (int i = 0; i < n; ++i) {
        if (p[i] != i + 1) identity = false;
    }
    if (identity) {
        long long fac = 1;
        for (int i = 1; i <= n; ++i) fac = fac * i % MOD;
        return (int)fac;
    }

    vector<int> need(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (p[i] > p[j]) need[i] |= 1 << j;
        }
    }

    int full = 1 << n;
    vector<int> dp(full, 0);
    dp[0] = 1;
    for (int mask = 0; mask < full; ++mask) {
        for (int u = 0; u < n; ++u) {
            if ((mask >> u) & 1) continue;
            if ((need[u] & mask) != need[u]) continue;
            int nxt = mask | (1 << u);
            dp[nxt] += dp[mask];
            if (dp[nxt] >= MOD) dp[nxt] -= MOD;
        }
    }
    return 2LL * dp[full - 1] % MOD;
}
```

典题：Round2 L《Lazy Shuffling》。看到 `n<=22` 且要数所有满足偏序关系的排列，直接往拓扑序状压 DP 想。
#### 中位数替换最大和：排序 + 前缀和枚举中位

> **赛时先看**
> - **题目信号**：只关心被选子序列的数值集合，不关心原下标；操作后贡献只由中位数和被替换元素原和决定。
> - **本质**：选一个长度为 `k` 的子序列，把这 `k` 个数全部替换成该子序列中位数，最大化最终数组和。
> - **接法**：排序后枚举中位位置 `i`。令 `left=(k-1)/2`，`right=k-left`，被替换原和是最小的 `left` 个数加上从 `i` 开始的 `right` 个数。
> - **复杂度判定**：排序 `O(n log n)`，枚举 `O(n)`。
> - **维护的量**：`pref[i]`（排序后前 i 个元素前缀和）；`best_delta`（替换前后最大增量）。
> - **警告**：偶数 `k=2m` 时中位数是中间两数平均，替换后总和是 `m*(a[i]+a[i+1])`；选择的较小元素应尽量小，较大元素应贴着右中位数。


**最小完整示例（先抄这一段就能跑）：**

> 题目：`{1,2,3,4}` 选长度 `k=3` 的子序列全部替换成中位数，最大化总和。

```cpp
vector<int> a = {1, 2, 3, 4};
printf("%lld\n", maximize_sum_after_median_replace(a, 3)); // 替换 {1,3,4} 成 {3,3,3} 得 11
```

> 样例输出：`11`

**传参要求（照这个传不会错）：**

- `a`：原数组（会被排序，任意 int），下标 0 起。`k`：替换长度，`1<=k<=n`。返回：替换后的最大数组和（`long long`）。


```cpp
long long maximize_sum_after_median_replace(vector<int> a, int k) {
    sort(a.begin(), a.end());
    int n = (int)a.size();

    long long total = 0;
    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        total += a[i];
        pref[i + 1] = pref[i] + a[i];
    }

    int left = (k - 1) / 2;
    int right = k - left;
    long long best_delta = LLONG_MIN;

    for (int i = left; i + right <= n; ++i) {
        long long before = pref[left] + (pref[i + right] - pref[i]);
        long long after;
        if (k & 1) after = 1LL * a[i] * k;
        else after = 1LL * (a[i] + a[i + 1]) * (k / 2);
        best_delta = max(best_delta, after - before);
    }
    return total + best_delta;
}
```

典题：Round2 N《Narrow to Median》。如果题目要求最小和，把“尽量小/大”的选择方向反过来重新推，别直接取负。
#### 最大连通非边点对：大块加孤点

> **赛时先看**
> - **题目信号**：边数给定，问最多有多少对点连通但不是相邻；连通块大小可自由设计。
> - **本质**：在 `n` 个点、`m` 条无重边无自环边的无向图中，最大化“同连通块但没有直接连边”的点对数。
> - **接法**：`m` 条边最多让 `min(n, m+1)` 个点连成一个块。该块内共有 `C(x,2)` 对点，其中 `m` 对被直接连边占掉。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：无额外结构；只算连通块大小 `x = min(n, m+1)`。
> - **警告**：最优结构是一个尽可能大的连通块加若干孤点，不是把边平均分散到多个块里。


**最小完整示例（先抄这一段就能跑）：**

> 题目：`n=5` 个点、`m=3` 条边，最大化"连通但未直接连边"的点对数。

```cpp
printf("%lld\n", max_connected_non_adjacent_pairs(5, 3)); // 4 点一块用 3 条边：C(4,2)-3
```

> 样例输出：`3`

**传参要求（照这个传不会错）：**

- `n`：点数（>=1）。`m`：无重边无自环的边数（`0<=m<=n*(n-1)/2`）。返回：最大"连通但非相邻"点对数（`long long`）。


```cpp
long long max_connected_non_adjacent_pairs(long long n, long long m) {
    long long x = min(n, m + 1);
    return x * (x - 1) / 2 - m;
}
```

典题：Round2 M《Maybe Connected》。若题目额外要求整图连通，那么 `m<n-1` 不可行，公式也要重推。
#### GCD 图最短路：质数间隙 + 容斥 + 短 DP

> **赛时先看**
> - **题目信号**：`n` 到 `1e7`；若起点附近存在一个质数，路径最多两步；真正需要 DP 的只有 `n` 前面最后一个质数到 `n` 的短间隙。
> - **本质**：在正整数图上，`u<v` 的边权为 `gcd(u,v)`，求区间内所有起点到固定 `n` 的最短路和。
> - **接法**：预处理最小质因子表 `spf`。`prefix_sum_to_target(target, up)` 返回 `sum_{x=1..up} dis(x,n)`，区间答案即 `range_sum(l, r, target)`。
> - **复杂度判定**：筛 `O(M log log M)`；单次约 `O(2^omega(n) + gap^2 log n)`，`gap` 为 `n` 与前一个质数的距离。
> - **维护的量**：`spf`（最小质因子表，判质数/分解质因子）；`dp[i]`（从 `target-i` 到 `target` 的最短路）。
> - **警告**：对小于前一个质数 `p` 的 `x`，`dis(x,n)` 为 1 当且仅当 `gcd(x,n)=1`，否则为 2；短区间 DP 要从大到小对应到 `target-i`。


**最小完整示例（先抄这一段就能跑）：**

> 题目：`1..10` 所有点到终点 `10` 的最短路之和（`u<v` 边权 `gcd(u,v)`）。

```cpp
GcdGraphDistanceSum g;
g.init_sieve(10);                       // 筛到 10，须 >= 目标与查询上界
printf("%d\n", g.range_sum(1, 10, 10)); // 质数 7 之后短间隙 DP，其余距离只可能是 1 或 2
```

> 样例输出：`14`

**传参要求（照这个传不会错）：**

- `init_sieve(max_value)`：筛上界，须 `>=` 所有 `target` 与查询上界。
- `range_sum(l, r, target)`：`l<=r`，`1<=l,r,target<=max_value`，闭区间。返回：`sum_{x=l..r} dis(x,target)`（`int`）。

**API / 入口函数（赛时只认这里列的名字）：**

- `range_sum(int l, int r, int target)` -> 查询闭区间和 返回 `int`。
**改板时先认这几个量：**

- `spf`：最小质因子表。
- `dp`：短间隙 DP 数组。

```cpp
struct GcdGraphDistanceSum {
    const int INF = 1000000000;
    vector<int> spf;

    void init_sieve(int max_value) {
        spf.resize(max_value + 1);
        iota(spf.begin(), spf.end(), 0);
        for (int i = 2; i <= max_value; ++i) {
            if (spf[i] == i) {
                for (long long j = i; j <= max_value; j += i) spf[j] = i;
            }
        }
    }

    int count_coprime_leq(int x, int up) const {
        if (up <= 0) return 0;
        vector<int> primes;
        int t = x;
        while (t > 1) {
            int p = spf[t];
            primes.push_back(p);
            while (t % p == 0) t /= p;
        }

        int ans = 0;
        int k = (int)primes.size();
        for (int mask = 0; mask < (1 << k); ++mask) {
            int cur = up;
            for (int i = 0; i < k; ++i) {
                if ((mask >> i) & 1) cur /= primes[i];
            }
            if (__builtin_popcount((unsigned)mask) & 1) ans -= cur;
            else ans += cur;
        }
        return ans;
    }

    int prefix_sum_to_target(int target, int up) const {
        if (up <= 0) return 0;

        int smaller_prime = target - 1;
        while (smaller_prime >= 2 && spf[smaller_prime] != smaller_prime) --smaller_prime;

        if (up < smaller_prime) {
            return 2 * up - count_coprime_leq(target, up);
        }

        int ans = 2 * (smaller_prime - 1) - count_coprime_leq(target, smaller_prime - 1);
        int gap = target - smaller_prime;
        vector<int> dp(gap + 1, INF);
        dp[0] = 0; // dp[i] 表示从 target-i 到 target 的最短路。

        for (int i = 1; i <= gap; ++i) {
            for (int j = 0; j < i; ++j) {
                dp[i] = min(dp[i], dp[j] + gcd(target - i, target - j));
            }
            if (target - i <= up) ans += dp[i];
        }
        return ans;
    }

    int range_sum(int l, int r, int target) const {
        return prefix_sum_to_target(target, r) - prefix_sum_to_target(target, l - 1);
    }
};
```

典题：Round2 G《GCD Graph》。这题的关键不是 Dijkstra，而是用质数把大范围压成“绝大多数只可能是 1 或 2”。
#### 超立方删两点 Hamming-2 配对

> **赛时先看**
> - **题目信号**：点是二进制向量；配对要求 Hamming 距离为偶数；允许整体异或和维度位重排。
> - **本质**：从 `0..2^n-1` 中删去两个点 `a,b`，构造剩余点的完美匹配，使每对点恰好有两个二进制位不同。
> - **接法**：先把 `a xor b` 的 1 位重排到低位，构造删 `0` 和 `2^k-1` 的标准配对，再把位映射和异或 `a` 还原。
> - **复杂度判定**：`O(2^n)`。
> - **维护的量**：`mate[x]`（标准配对表，x 的配对点）；`mapped`（把 `a xor b` 的 1 位压到低位的位重排映射）。
> - **警告**：Hamming 距离 2 不改变 popcount 奇偶性，所以删去的两个点必须 popcount 奇偶性相同；构造前可整体异或 `a`，把删点变成 `0` 和 `a xor b`。


**最小完整示例（先抄这一段就能跑）：**

> 题目：`n=2` 维超立方删掉点 `0,3`，给剩余点做 Hamming 距离 2 的完美配对。

```cpp
auto pairs = pair_hypercube_minus_two(2, 0, 3);
for (auto [u, v] : pairs) printf("(%d,%d) ", u, v); // 剩余 1、2 恰好距离 2
```

> 样例输出：`(1,2)`

**传参要求（照这个传不会错）：**

- `n`：维度，点集 `0..2^n-1`。`a,b`：删去的两个点，`0<=a,b<2^n`；`popcount(a^b)` 为奇数时返回空。返回：完美配对列表（每对 Hamming 距离 2，pair 内升序）。

**改板时先认这几个量：**

- `sz`：当前已生成的掩码集合大小。
- `bit`：遍历的维度位。
- `cur`：当前标准配对用的掩码值。

```cpp
vector<pair<int, int>> pair_hypercube_minus_two(int n, int a, int b) {
    int diff = a ^ b;
    if (__builtin_popcount((unsigned)diff) & 1) return {};

    int full = 1 << n;
    vector<int> mate(full);
    for (int x = 0; x < full; ++x) mate[x] = x ^ 3;
    mate[0] = 0;
    mate[3] = 3;

    int k = __builtin_popcount((unsigned)diff);
    for (int bits = 2; bits < k; bits += 2) {
        int cur = (1 << bits) - 1;
        mate[cur] = cur << 1;
        mate[cur << 1] = cur;
        mate[(cur << 1) ^ 3] = cur << 2;
        mate[cur << 2] = (cur << 1) ^ 3;
        mate[(cur << 2) ^ 3] = (cur << 2) ^ 3;
    }

    vector<int> mapped = {0};
    for (int bit = 0; bit < n; ++bit) {
        if ((diff >> bit) & 1) {
            int sz = (int)mapped.size();
            for (int i = 0; i < sz; ++i) mapped.push_back(mapped[i] ^ (1 << bit));
        }
    }
    for (int bit = 0; bit < n; ++bit) {
        if (((diff >> bit) & 1) == 0) {
            int sz = (int)mapped.size();
            for (int i = 0; i < sz; ++i) mapped.push_back(mapped[i] ^ (1 << bit));
        }
    }

    vector<pair<int, int>> ans;
    for (int i = 0; i < full; ++i) {
        if (mate[i] > i) ans.push_back({mapped[i] ^ a, mapped[mate[i]] ^ a});
    }
    return ans;
}
```

典题：Round2 H《Hyperspace Pairing》。如果删点 popcount 奇偶性不同，两个奇偶类剩余数量不同，直接无解。
#### 交通期望：对角线随机游走首次撞边

> **赛时先看**
> - **题目信号**：每个控制点只有横向/纵向两种状态；斜走永远不等待；最优策略会尽量靠近 `i=j` 的零等待对角线。
> - **本质**：在特殊交通网格中，从右下到左上，最小化期望等待时间。基础行走时间固定，只需求额外等待期望。
> - **接法**：若一边已经为 0，额外等待是单方向等待概率乘步数；否则把较大差值一侧映射成首次撞到边界的组合数求和。
> - **复杂度判定**：预处理组合数 `O(max(n+m))`，单次 `O(|n-m| + log MOD)`。
> - **维护的量**：`fac/ifac`（阶乘与逆元表，供 `C`）；`p1/p2`（两个方向概率，`p1=p/q`）。
> - **警告**：最后答案要加固定走边数 `2n+2m-2`；代码里先令 `n--,m--` 进入控制点坐标；`p/q` 和 `1-p/q` 的方向在两侧是对称互换的。


**最小完整示例（先抄这一段就能跑）：**

> 题目：`2x2` 网格、双向等概率（`p=q`），求右下到左上的期望总时间。

```cpp
TrafficExpectation t;
t.init(10);                       // 组合数预处理到 10（须 >= n+m）
printf("%d\n", t.solve(2, 2, 1, 1)); // 等概率无等待，只剩固定步数 2n+2m-2
```

> 样例输出：`6`

**传参要求（照这个传不会错）：**

- `init(max_n)`：组合数上界，须 `>= n+m`。
- `solve(n, m, p, q)`：`n,m` 网格行列（>=1）；`p,q` 方向概率（`1<=p<=q<MOD`，模意义可逆）。返回：期望总时间 mod 998244353（`int`）。

**API / 入口函数（赛时只认这里列的名字）：**

- `C(int n, int k)` -> 组合数 `C(n,k)`，返回 `int`。
- `init(int max_n)` -> 初始化/清空结构
- `solve(int n, int m, int p, int q)` -> 执行主算法并返回答案

```cpp
struct TrafficExpectation {
    static constexpr int MOD = 998244353;
    vector<int> fac, ifac;

    long long mod_pow(long long a, long long e) const {
        long long r = 1;
        while (e > 0) {
            if (e & 1) r = r * a % MOD;
            a = a * a % MOD;
            e >>= 1;
        }
        return r;
    }

    void init(int max_n) {
        fac.assign(max_n + 1, 1);
        ifac.assign(max_n + 1, 1);
        for (int i = 1; i <= max_n; ++i) fac[i] = 1LL * fac[i - 1] * i % MOD;
        ifac[max_n] = mod_pow(fac[max_n], MOD - 2);
        for (int i = max_n; i >= 1; --i) ifac[i - 1] = 1LL * ifac[i] * i % MOD;
    }

    int C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return 1LL * fac[n] * ifac[k] % MOD * ifac[n - k] % MOD;
    }

    int solve(int n, int m, int p, int q) const {
        long long ans = 2LL * (n + m) - 2;
        int p1 = 1LL * p * mod_pow(q, MOD - 2) % MOD;
        int p2 = (MOD + 1 - p1) % MOD;

        --n;
        --m;
        if (n == 0) return (ans + 1LL * m * p1) % MOD;
        if (m == 0) return (ans + 1LL * n * p2) % MOD;

        auto calc_side = [&](int equal_part, int extra, int keep_prob, int hit_prob) {
            long long cur = 1;
            long long res = 0;
            for (int i = 0; i <= extra; ++i) {
                res += 1LL * (extra - i) * cur % MOD * C(equal_part - 1 + i, i) % MOD;
                res %= MOD;
                cur = cur * keep_prob % MOD;
            }
            return res * mod_pow(hit_prob, equal_part + 1) % MOD;
        };

        if (n > m) ans += calc_side(m, n - m, p1, p2);
        else ans += calc_side(n, m - n, p2, p1);
        return ans % MOD;
    }
};
```

典题：Round2 A《Annoying Traffic》。如果遇到类似“二维 DP 看起来很大但有一条零代价对角线”，优先尝试按差值压缩。
#### 树上绝对差赋点权：根相对值域 DP

> **赛时先看**
> - **题目信号**：边约束是 `abs(val[u]-val[v])=w`；点权整体平移不影响合法性；只关心最大值减最小值。
> - **本质**：给树边权，为每棵子树赋点权，使每条边权等于两端点权差绝对值，求子树内点权极差最小值。
> - **接法**：`dp[u][i]` 表示设 `u` 权值为 `V`，子树最小值不低于 `V-i` 时，子树最大值超过 `V` 的最小值。
> - **复杂度判定**：`O(nW)`，其中 `W` 是最大边权，本题总规模保证可过。
> - **维护的量**：`g`（邻接表）；`dp[i]`（父权为 V、子树最低不低于 `V-i` 时，子树高出 V 的最小量）；`ans[u]`（u 子树的最小极差）。
> - **警告**：最大极差最小值不超过 `2W-1`，DP 数组长度用 `2*max_edge`；子节点可以比父亲小 `w` 或大 `w`，两种转移都要取。


**最小完整示例（先抄这一段就能跑）：**

> 题目：两个点连边权 `3`，赋点权使 `abs(val[u]-val[v])=3`，最小化整棵树极差。

```cpp
FabulousTreeDP t;
t.init(2);
t.add_edge(0, 1, 3);
vector<int> ans = t.solve();
printf("%d\n", ans[0]); // 整棵树极差最小 3；叶节点 1 的子树极差 0
```

> 样例输出：`3`

**传参要求（照这个传不会错）：**

- `init(n_)`：点数。
- `add_edge(u, v, w)`：`u,v` 节点下标 0 起（`0..n-1`），`w` 边权（>=1）。
- `solve()`：返回 `ans[u]` = 以 u 为根子树的最小极差（`vector<int>`）。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v, int w)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
- `solve()` -> 执行主算法并返回答案
**改板时先认这几个量：**

- `g`：邻接表。
- `parent`：DFS 的父节点参数。
- `dp`：DP 状态。

```cpp
struct FabulousTreeDP {
    int n = 0, bound = 0;
    vector<vector<pair<int, int>>> g;
    vector<int> ans;

    void init(int n_) {
        n = n_;
        bound = 0;
        g.assign(n, {});
    }

    void add_edge(int u, int v, int w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
        bound = max(bound, 2 * w);
    }

    vector<int> dfs(int u, int parent) {
        vector<int> dp(bound, 0);
        for (auto [v, w] : g[u]) {
            if (v == parent) continue;
            vector<int> child = dfs(v, u);
            vector<int> best_from_child(bound, bound);

            for (int i = 0; i < bound; ++i) {
                best_from_child[max(0, i - w)] = min(best_from_child[max(0, i - w)], child[i] + w);
                if (i + w < bound) {
                    best_from_child[i + w] = min(best_from_child[i + w], max(0, child[i] - w));
                }
            }
            for (int i = 1; i < bound; ++i) {
                best_from_child[i] = min(best_from_child[i], best_from_child[i - 1]);
            }
            for (int i = 0; i < bound; ++i) {
                dp[i] = max(dp[i], best_from_child[i]);
            }
        }

        for (int i = 0; i < bound; ++i) ans[u] = min(ans[u], dp[i] + i);
        return dp;
    }

    vector<int> solve() {
        if (bound == 0) bound = 1;
        ans.assign(n, bound);
        dfs(0, -1);
        return ans;
    }
};
```

典题：Round2 F《Fabulous Tree》。这类“绝对值边约束”先固定一个根值，把所有状态都变成相对根的上下界。
#### 少量特殊边动态最短路：普通图全源 BFS + 小图 Dijkstra

> **赛时先看**
> - **题目信号**：可变边很少；非特殊边权相同；查询端点很多但每次最短路只需要经过特殊边端点和查询端点。
> - **本质**：无向连通图中，大多数边权都是同一个 `T`，只有 `k<=50` 条特殊边权会被修改。每次修改后回答多组点对最短路。
> - **接法**：普通边组成的图上从每个点 BFS，距离乘 `T`。查询时在压缩点集上同时考虑完全图普通距离边和特殊边。
> - **复杂度判定**：预处理普通边全源 BFS `O(nm)`；每个点对查询在 `O(k^2)` 小图上跑朴素 Dijkstra。
> - **维护的量**：`normal_dis[u][v]`（普通边全源距离）；`special_weight[id]`（特殊边当前权值）；`special_nodes`（特殊边端点集）。
> - **警告**：压缩点集要包含所有特殊边端点以及本次的 `a,b`；普通边路径距离来自预处理 `dis[u][v]`，特殊边用当前权值。
> - **约定**：vector<Edge> edges; // 0-indexed，下标从 0 开始


**最小完整示例（先抄这一段就能跑）：**

> 题目：`3` 点 `2` 边，普通边权 `5`，边 0 设为特殊边权 `1`，求 `0` 到 `2` 最短路并动态改权。

```cpp
FewSpecialEdgesShortestPath sp;
sp.init(3, 5, {{0, 1}, {1, 2}}, {{0, 1}}); // (边 id, 特殊权)，权 0 表示普通边
printf("%lld\n", sp.query(0, 2)); // 0->1 走特殊边 1，1->2 走普通边 5
sp.update_special_edge(0, 100);   // 动态改特殊边权
printf("%lld\n", sp.query(0, 2)); // 绕回普通边 5+5=10
```

> 样例输出：`6` 和 `10`

**传参要求（照这个传不会错）：**

- `init(n_, normal_w_, all_edges, special_id_weight)`：`n_` 点数；`normal_w_` 普通边权；`all_edges` 全部边 `{u,v}` 0-indexed；`special_id_weight` 为 `(边 id, 特殊权)` 列表，特殊权 >=1（0 表示普通边）。
- `update_special_edge(id, w)`：把 id 号边（须为特殊边）权改成 `w`（>=1）。
- `query(s, t)`：`s,t` 0-indexed 点对。返回：最短路（`long long`，不可达为 `4e18`）。

**API / 入口函数（赛时只认这里列的名字）：**

- `query(int s, int t)` -> 查询 返回 `long long`。
**改板时先认这几个量：**

- `edges`：0-indexed，下标从 0 开始。
- `dist`：距离。

```cpp
struct FewSpecialEdgesShortestPath {
    struct Edge {
        int u, v;
    };

    int n = 0;
    long long normal_w = 0;
    vector<Edge> edges; // 0-indexed，下标从 0 开始。
    vector<int> special_weight;
    vector<vector<int>> special_edge_ids_at;
    vector<vector<long long>> normal_dis;
    vector<int> special_nodes;

    void init(int n_, long long normal_w_, const vector<Edge>& all_edges,
              const vector<pair<int, int>>& special_id_weight) {
        n = n_;
        normal_w = normal_w_;
        edges = all_edges;
        int m = (int)edges.size();
        special_weight.assign(m, 0);
        vector<int> is_special_endpoint(n, 0);

        for (auto [id, w] : special_id_weight) {
            special_weight[id] = w;
            is_special_endpoint[edges[id].u] = 1;
            is_special_endpoint[edges[id].v] = 1;
        }

        vector<vector<int>> normal_adj(n);
        special_edge_ids_at.assign(n, {});
        for (int id = 0; id < m; ++id) {
            auto [u, v] = edges[id];
            if (special_weight[id] == 0) {
                normal_adj[u].push_back(v);
                normal_adj[v].push_back(u);
            } else {
                special_edge_ids_at[u].push_back(id);
                special_edge_ids_at[v].push_back(id);
            }
        }

        const long long INF = (long long)4e18;
        normal_dis.assign(n, vector<long long>(n, INF));
        for (int s = 0; s < n; ++s) {
            queue<int> q;
            normal_dis[s][s] = 0;
            q.push(s);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : normal_adj[u]) {
                    if (normal_dis[s][v] == INF) {
                        normal_dis[s][v] = normal_dis[s][u] + normal_w;
                        q.push(v);
                    }
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            if (is_special_endpoint[i]) special_nodes.push_back(i);
        }
    }

    void update_special_edge(int id, int w) {
        special_weight[id] = w;
    }

    long long query(int s, int t) const {
        const long long INF = (long long)4e18;
        vector<int> nodes = special_nodes;
        nodes.push_back(s);
        nodes.push_back(t);
        sort(nodes.begin(), nodes.end());
        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

        vector<long long> dist(n, INF);
        vector<int> used(n, 0);
        dist[s] = 0;

        for (int step = 0; step < (int)nodes.size(); ++step) {
            int u = -1;
            for (int v : nodes) {
                if (!used[v] && (u == -1 || dist[v] < dist[u])) u = v;
            }
            if (u == -1 || dist[u] == INF) break;
            used[u] = 1;

            for (int v : nodes) {
                if (!used[v]) dist[v] = min(dist[v], dist[u] + normal_dis[u][v]);
            }
            for (int id : special_edge_ids_at[u]) {
                int v = edges[id].u ^ edges[id].v ^ u;
                if (!used[v]) dist[v] = min(dist[v], dist[u] + special_weight[id]);
            }
        }
        return dist[t];
    }
};
```

典题：Round2 K《Kindergarten》。少量特殊边/点的题，经常能先把“大图任意普通路径”预处理成压缩图上的完全边。
#### 允许误差的向量询问：按方向分组 + 整数前缀近似

> **赛时先看**
> - **题目信号**：题面允许相对误差；查询向量随机；表达式可写成 `|V_i||T|(|sin theta|+|cos theta|)`，方向相近时比例接近。
> - **本质**：多次查询 `sum(|V_i dot T| + |V_i cross T|)`，允许较大相对误差，且查询向量随机。
> - **接法**：每轮拿一个未回答查询作为代表方向，把和它夹角足够小的查询分到同一组。对代表方向预处理数组前缀，回答这一组。
> - **复杂度判定**：期望 `O((n+q) * 轮数）`，轮数由方向覆盖精度决定。
> - **维护的量**：`pending`（未回答查询集合）；`pref[i]`（代表方向下整数前缀和，每项乘 `2^20`）；`ans[id]`（查询答案）。
> - **警告**：浮点前缀和作差可能误差不好控；标程把每项乘 `2^20` 后存整数前缀，再按查询向量长度缩放。


**最小完整示例（先抄这一段就能跑）：**

> 题目：向量 `(1,0,0)`、`(0,1,0)`，用查询向量 `(1,1,0)` 求 `[1,2]` 的 `|dot|+|cross|` 和。

```cpp
vector<Vec3LL> vecs = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}}; // 下标从 1 起，vecs[0] 占位
vector<array<int, 5>> q = {{1, 1, 0, 1, 2}};              // {x, y, z, l, r}
auto ans = answer_random_direction_queries(vecs, q);
printf("%.6Lf\n", ans[0]); // 每项 |dot|+|cross| = 2，两项合计 4
```

> 样例输出：`4.000000`

**传参要求（照这个传不会错）：**

- `vecs`：向量数组，1-indexed（`vecs[0]` 占位不用），`vecs[i].x/y/z` 为坐标分量。
- `raw_queries`：每项 `{x,y,z,l,r}` = 查询向量分量 + 闭区间 `[l,r]`，`1<=l<=r<=n`。返回：`ans[i]` 为近似和（`long double`，允许相对误差）。


```cpp
struct Vec3LL {
    long long x, y, z;
};

long double norm_vec(Vec3LL a) {
    return sqrtl((long double)a.x * a.x + (long double)a.y * a.y + (long double)a.z * a.z);
}

long double approximate_dot_cross_sum(Vec3LL v, Vec3LL t) {
    long double dot = (long double)v.x * t.x + (long double)v.y * t.y + (long double)v.z * t.z;
    long double cx = (long double)v.y * t.z - (long double)v.z * t.y;
    long double cy = (long double)v.z * t.x - (long double)v.x * t.z;
    long double cz = (long double)v.x * t.y - (long double)v.y * t.x;
    return fabsl(dot) + sqrtl(cx * cx + cy * cy + cz * cz);
}

vector<long double> answer_random_direction_queries(
    const vector<Vec3LL>& vecs,
    const vector<array<int, 5>>& raw_queries
) {
    int n = (int)vecs.size() - 1;
    int q = (int)raw_queries.size();

    vector<int> left(q), right(q);
    vector<Vec3LL> query_vec(q);
    for (int i = 0; i < q; ++i) {
        auto [x, y, z, l, r] = raw_queries[i];
        query_vec[i] = {x, y, z};
        left[i] = l;
        right[i] = r;
    }

    vector<int> pending(q);
    iota(pending.begin(), pending.end(), 0);
    vector<long double> ans(q, 0);

    while (!pending.empty()) {
        int rep_id = pending[0];
        Vec3LL rep = query_vec[rep_id];
        vector<int> group, rest;

        for (int id : pending) {
            Vec3LL cur = query_vec[id];
            long double dot = (long double)cur.x * rep.x + (long double)cur.y * rep.y + (long double)cur.z * rep.z;
            long double cos2 = dot * dot / (norm_vec(cur) * norm_vec(cur)) / (norm_vec(rep) * norm_vec(rep));
            if (cos2 > 0.991L) group.push_back(id);
            else rest.push_back(id);
        }

        const long double SCALE = (1 << 20);
        vector<long long> pref(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            long double value = approximate_dot_cross_sum(vecs[i], rep) / norm_vec(rep);
            pref[i] = pref[i - 1] + (long long)(value * SCALE);
        }

        for (int id : group) {
            long double base = (long double)(pref[right[id]] - pref[left[id] - 1]) / SCALE;
            ans[id] = base * norm_vec(query_vec[id]);
        }
        pending.swap(rest);
    }
    return ans;
}
```

典题：Round2 I《Imperfect Dot Sums and Cross Sums》。只有题面允许误差并且数据随机时才这么写；精确题不要用近似分组。
#### 网格画同心矩形：偶短边拆块构造

> **赛时先看**
> - **题目信号**：要求输出一组边；每个点度数为 0 或 2，意味着染色边是一堆环；矩形边界天然满足点度数条件。
> - **本质**：在 `n*m` 个格子的网格图边上染色，使每个点 incident 染色边数为 0 或 2，且坏格子数不超过 5。
> - **接法**：若短边为奇数，直接一层层画同心矩形。若短边为偶数，把长方形拆成正方形和奇短边长方形，再分别画。
> - **复杂度判定**：输出边数 `O(nm)`。
> - **维护的量**：`edges`（染色边列表，每条为两个格点编号）；无其他结构。
> - **警告**：函数里的坐标是格点坐标，范围 `0..n`、`0..m`；输出点编号是 `(n+1)*(m+1)` 个格点，不是 `n*m` 个格子。
> - **约定**：return i * (m + 1) + j + 1; // 格点 (i,j) 的 1-based 编号


**最小完整示例（先抄这一段就能跑）：**

> 题目：`3x3` 网格，画出全部同心矩形边（短边为奇数直接画）。

```cpp
auto e = construct_easy_puzzle_edges(3, 3);
printf("%zu\n", e.size()); // 外圈 12 条 + 内圈 4 条，每个格点度数 0 或 2
```

> 样例输出：`16`

**传参要求（照这个传不会错）：**

- `n`：行格子数；`m`：列格子数（均 >=1）。返回：染色边列表，点编号 `i*(m+1)+j+1`，格点 `i in [0,n]`、`j in [0,m]`（1-based）。


```cpp
vector<pair<int, int>> construct_easy_puzzle_edges(int n, int m) {
    auto id = [&](int i, int j) {
        return i * (m + 1) + j + 1; // 格点 (i,j) 的 1-based 编号。
    };

    vector<pair<int, int>> edges;
    auto draw_rectangle = [&](auto&& self, int xl, int xr, int yl, int yr) -> void {
        if (xr - xl <= 0 || yr - yl <= 0) return;
        for (int i = xl; i < xr; ++i) {
            edges.push_back({id(i, yl), id(i + 1, yl)});
            edges.push_back({id(i, yr), id(i + 1, yr)});
        }
        for (int j = yl; j < yr; ++j) {
            edges.push_back({id(xl, j), id(xl, j + 1)});
            edges.push_back({id(xr, j), id(xr, j + 1)});
        }
        self(self, xl + 1, xr - 1, yl + 1, yr - 1);
    };

    if (min(n, m) & 1) {
        draw_rectangle(draw_rectangle, 0, n, 0, m);
    } else if (n > m) {
        int x = n, y = m;
        while (x > 2 * y) {
            draw_rectangle(draw_rectangle, x - y, x, 0, y);
            x -= y + 1;
        }
        if (x == y || x == y + 1) draw_rectangle(draw_rectangle, 0, x, 0, y);
        else if (x % 2 == 0) {
            draw_rectangle(draw_rectangle, 0, y, 0, y);
            draw_rectangle(draw_rectangle, y + 1, x, 0, y);
        } else {
            int v = x / 2;
            if (!(v & 1)) --v;
            draw_rectangle(draw_rectangle, 0, v, 0, y);
            draw_rectangle(draw_rectangle, v + 1, x, 0, y);
        }
    } else {
        int x = n, y = m;
        while (y > 2 * x) {
            draw_rectangle(draw_rectangle, 0, x, y - x, y);
            y -= x + 1;
        }
        if (y == x || y == x + 1) draw_rectangle(draw_rectangle, 0, x, 0, y);
        else if (y % 2 == 0) {
            draw_rectangle(draw_rectangle, 0, x, 0, x);
            draw_rectangle(draw_rectangle, 0, x, x + 1, y);
        } else {
            int v = y / 2;
            if (!(v & 1)) --v;
            draw_rectangle(draw_rectangle, 0, x, 0, v);
            draw_rectangle(draw_rectangle, 0, x, v + 1, y);
        }
    }
    return edges;
}
```

典题：Round2 E《Easy Puzzle》。构造图上度数全为偶数时，先想“输出若干个不相交或嵌套的环”。
#### 羽毛球比分连胜：普通区与超分区分段

> **赛时先看**
> - **题目信号**：得分过程只增加不减少；终止条件为“至少 k 分且领先 2 分”；普通区和 deuce 超分区规则不同。
> - **本质**：已知比赛从比分 `(x1,y1)` 到 `(x2,y2)`，求第一位选手在这段过程中的最长连续得分的最小值和最大值。
> - **复杂度判定**：`O(1)`，但分类多。
> - **维护的量**：`dx/dy`（第一/第二人得分增量）；分段后每段的 `mn/mx`（最长连胜的最小/最大值）。
> - **警告**：题解建议先写小范围暴力搜索对拍，再补分类边界；尤其是从 `(k-1,k-1)` 进入超分区时，要用某次第二个人得分把序列切成两段。


**最小完整示例（先抄这一段就能跑）：**

> 题目：比分从 `(0,0)` 到 `(10,4)`（全程未到 `k`），求第一人最长连胜的最小/最大值。

```cpp
long long dx = 10, dy = 4;
long long mn = (dx + dy) / (dy + 1); // ceil(dx/(dy+1))：dy 次对方得分把连胜隔开
long long mx = dx;                    // 最大：全部连在一起
printf("%lld %lld\n", mn, mx);
```

> 样例输出：`2 10`

**传参要求（照这个传不会错）：**

- 本节无封装函数，直接用公式分段：`dx = x2-x1`、`dy = y2-y1`（比分起点/终点，`x1<=x2`）。
- 全程未到 `k`：`mn = ceil(dx/(dy+1))`、`mx = dx`；起点已超分或穿过超分区：按「赛时先看」分支枚举中转比分合并，返回值是 `(mn, mx)`。

- 若全程没到 `k`，第一人得分 `dx` 用第二人 `dy` 个得分隔开，最小最长段为 `ceil(dx/(dy+1))`，最大为 `dx`。
- 若起点已在超分区，第一人连续 3 分通常不可能，除非最后第一人赢；最大值受总得分和领先差共同限制。
- 穿过普通区到超分区时，枚举中转比分，把前后两段的答案取 `max` 合并。

典题：Round2 C《Competition: Winning Streaks》。这题更像“分类讨论工程题”，模板价值在分段方法，不在死背所有分支。

#### 路径挂树按层生长计数 DP

> **赛时先看**
> - **题目信号**：树上两点路径固定，其他点都挂在路径某个点或挂树上；距离到路径只按层数增加；还要处理标号选择。
> - **本质**：固定树中 `dis(1,2)=i`，统计路径外挂树对某个距离频次数组的贡献均值。
> - **接法**：设 `dp0[x][y]` 为有 `x` 棵挂树、总点数 `y` 的方案数，`dp1[x][y]` 为对应答案和。每向外扩一层，枚举新一层的点数并乘组合数、父亲选择数和距离权值变化。
> - **复杂度判定**：`O(n^3)`。
> - **警告**：路径上的常数距离部分可最后整体乘 `P^{dis(1,2)}`；DP 转移中新增一层要选标号并给旧根选父亲，所以有组合数和 `x'^x`。


典题：Round2 D《Delivering Newspapers》。看到“固定主路径 + 路径外森林”的树计数，先把路径外点按到主路径的距离分层。

#### 随机四舍五入期望：连续 4 进位链与生成函数

> **赛时先看**
> - **题目信号**：四舍五入会把后缀清零，`0..3` 直接舍，`5..8` 直接进；真正复杂的是 `444...45` 这种进位链。
> - **本质**：一个不含数码 9 的数，随机选择数位四舍五入 `k` 次，求最终数字期望。
> - **复杂度判定**：题解做法 `O(n log n + n log MOD)`。
> - **警告**：最高位被操作后可能产生新的一位 `10^n`；模意义下待定系数分解可能出现重根，不能直接套普通部分分式。


- 按“操作过的最高位”分组，概率是 `i^k-(i-1)^k` 这类形式。
- 普通位 `0..3` 和 `5..8` 只需前缀数值、后缀 `10^i`。
- 连续 4 后接大数时，进位概率等价于若干几何级数乘积的系数。
- 同一段连续 4 可转成卷积：`(x+j-1)^k / x!` 与 `(-1)^x / x!`，用 NTT 批量求。
- 若最高位产生的分母在模意义下重根，要额外处理 `(1-kx)^2` 的系数。

典题：Round2 J《Just Round It》。这类题要先把“随机过程”按最后一次关键事件拆开，再把多次等待转成生成函数。

#### 赛后模型总览：2026 牛客暑期多校训练营 3

> **赛时先看**
> - **题目信号**：只记得题号或题面关键词，不确定该翻哪个算法专题。
> - **本质**：补 Round3 时先用本节定位题号对应的模型，再去下面具体板子或全册对应专题查代码。
> - **复杂度判定**：索引页不涉及算法复杂度。
> - **警告**：A/B/G/K/L 是短模型，建议直接熟练；D/H/J/M 更偏建模题，补题时要重点复盘“为什么能转成这个模型”。


题号到模型：

- A《比特掩码》：相邻二进制位状态计数，全体 `AND/OR/XOR` 后维护 1 段数量。
- B《再买一瓶》：Raney 引理 / cycle lemma，花光预算的合法随机序列计数。
- C《蛋糕店》：删支配订单、二分答案、前缀分批 DP，优化可用指针 + 单调结构 + 线段树。
- D《最长的连续的一》：DAG 拓扑序中的连续段，三组划分转最大权闭合图 / 最小割。
- E《扫雷》：两行条带构造，按列 mask DP 统计合法地雷方案数，离线搜索打表。
- F《Not Aqre 2》：窄行大列网格染色，按“颜色相等关系形状”压缩状态后矩阵快速幂。
- G《矩阵标记》：同值点按出现行压缩，前后缀列界推出可标记矩形，二维差分。
- H《季节》：周期序列线性空间，分圆多项式 / 线性递推判定最小周期上界。
- I《交换大师》：至多一次交换，受影响边局部化，按斜率集合维护历史最优。
- J《树.zip》：新父亲必须是原树祖先，约束最深祖先优先，用可并堆自底向上合并要求。
- K《转向导航》：三点叉积判左转、右转、直行。
- L《登山对决》：严格升高移动形成 DAG，按高度降序做普通图上胜负 DP。
- M《漫游者》：树上无立即回头随机游走，有向边状态期望，后序求线性关系、前序代回。

#### 位对计数：全体按位操作后维护 1 段数量

> **赛时先看**
> - **题目信号**：题目要统计“二进制里有几段连续的 1”；操作是对全部元素统一按位运算；单独改每个数会超时，但每一位的变化规则相同。
> - **本质**：维护一组整数，支持把所有数同时做 `& x`、`| x`、`^ x`，每次询问所有数二进制表示中的连续 1 段总数。
> - **接法**：对每个相邻位 `(i,i+1)` 维护四种状态数量。连续 1 段数量等于所有满足 `bit_i=1, bit_{i+1}=0` 的位置数。
> - **复杂度判定**：初始化 `O(30n)`，每次操作 `O(30 * 4)`，询问 `O(30)`。
> - **维护的量**：`cnt[i][low][high]`：第 `i` 位取 `low`、第 `i+1` 位取 `high` 的元素个数；`LOG=30`，第 30 位固定看成 0。
> - **警告**：额外补一个第 30 位为 0，用来统计最高位的 1 段结尾；如果题目值域超过 `2^30`，把 `LOG` 改大，并把第 `LOG` 位补成 0。

**最小完整示例（先抄这一段就能跑）：**
题目：`n` 个数、`q` 次全体按位运算，每次询问所有数二进制里连续 1 段的段数。
```cpp
BitRunCounter brc;
brc.init(a);                    // a 为 vector<int>，a[i] 是第 i 个数，下标 0..n-1
brc.apply_all(1, x);            // 全体 &= x；type=1/2/3 分别对应 &、|、^
long long ans = brc.answer();   // 当前所有数的二进制连续 1 段总数
```
样例：`a={3,5}` 时 `answer()=3`；`apply_all(3,2)` 后 `answer()=2`。

**传参要求（照这个传不会错）：**
- `init(const vector<int>& a)`：`a[i]` 为第 `i` 个数，下标 `0..n-1`，值域 `[0,2^30)`；无返回值。
- `apply_all(int type, int x)`：`type` 取 `1/2/3`（`&`、`|`、`^`）；`x` 与 `a[i]` 同值域，若题面值域更大需先调大 `LOG`；无返回值。
- `answer()`：无参数；返回 `long long`，为当前所有数的连续 1 段总数。


**API / 入口函数（赛时只认这里列的名字）：**

- `init(const vector<int>& a)` -> 初始化/清空结构
**改板时先认这几个量：**

- `low`：当前低位（第 i 位）的旧 bit 值。
- `nxt`：按位操作后的新状态计数数组。

```cpp
struct BitRunCounter {
    static constexpr int LOG = 30; // 处理 0..29 位；第 30 位固定看成 0。
    long long cnt[LOG][2][2]{};

    int apply_one_bit(int old_bit, int type, int x_bit) const {
        // type=1 表示 & x，type=2 表示 | x，type=3 表示 ^ x。
        if (type == 1) return old_bit & x_bit;
        if (type == 2) return old_bit | x_bit;
        return old_bit ^ x_bit;
    }

    void init(const vector<int>& a) {
        memset(cnt, 0, sizeof(cnt));
        for (int x : a) {
            for (int i = 0; i < LOG; ++i) {
                int low = (x >> i) & 1;
                int high = (i + 1 == LOG ? 0 : ((x >> (i + 1)) & 1));
                cnt[i][low][high]++;
            }
        }
    }

    void apply_all(int type, int x) {
        for (int i = 0; i < LOG; ++i) {
            long long nxt[2][2] = {};
            int xb0 = (x >> i) & 1;
            int xb1 = (i + 1 == LOG ? 0 : ((x >> (i + 1)) & 1));
            for (int a = 0; a <= 1; ++a) {
                for (int b = 0; b <= 1; ++b) {
                    int na = apply_one_bit(a, type, xb0);
                    int nb = apply_one_bit(b, type, xb1);
                    nxt[na][nb] += cnt[i][a][b];
                }
            }
            memcpy(cnt[i], nxt, sizeof(nxt));
        }
    }

    long long answer() const {
        long long res = 0;
        for (int i = 0; i < LOG; ++i) res += cnt[i][1][0];
        return res;
    }
};
```

典题：Round3 A《比特掩码》。如果题面改成只问某个区间，通常要在线段树节点里维护同样的 `cnt[i][2][2]`。
#### Raney 引理：花光预算的合法序列计数

> **赛时先看**
> - **题目信号**：有 `m` 步、其中 `w` 步是特殊事件；最终余额恰好到 0；要求中途余额始终大于 0；把序列反转取负后，每步都不超过 1。
> - **本质**：统计由两类步长组成的序列，要求总和固定，并且所有非空前缀和都为正。常见于“过程中不能破产 / 不能提前归零”的计数概率题。
> - **接法**：中奖概率 `p=a/b`，不中奖概率 `q=(b-a)/b`，答案为 `n * inv(m) * C(m,w) * p^w * q^(m-w)`。
> - **复杂度判定**：预处理组合数 `O(max m)`，单次 `O(log MOD)`。
> - **维护的量**：`fac/ifac`：`0..n` 的阶乘与阶乘逆元，供 `C(n,k)` O(1) 查询；除此之外无其它长期状态。
> - **警告**：先判断中奖次数是否唯一合法：`m < n` 或 `(m-n) % c != 0` 直接为 0；Raney 引理给的是合法线性序列数 `n / m * C(m,w)`。

**最小完整示例（先抄这一段就能跑）：**
题目：`m` 次抽奖、每次以 `a/b` 概率赢，全程余额不为负且最终恰好为 0 的合法序列概率。
```cpp
Comb998 comb;
comb.init(m);                                // 先预处理到最大步数 m 的组合数
int ans = bottle_raney_answer(n, m, c, a, b, comb);
printf("%d\n", ans);                         // 概率 mod 998244353
```
样例：`bottle_raney_answer(2,4,1,1,2,comb)` 返回 `811073537`（即 3/16 mod 998244353）。

**传参要求（照这个传不会错）：**
- `n`：最终净余额；需满足 `m>=n` 且 `(m-n)%c==0`，否则函数直接返回 0；`int`。
- `m`：总步数；`int`；`comb.init(m)` 必须先按此值预处理。
- `c`：每个特殊事件对余额的净变化量；`int`。
- `a` / `b`：特殊事件概率 `a/b`（内部自动求模逆元），不中奖概率为 `(b-a)/b`；`int`。
- `comb`：已 `init` 的 `Comb998`；函数返回 `int`：概率 mod 998244353。


**API / 入口函数（赛时只认这里列的名字）：**

- `C(int n, int k)` -> 组合数 `C(n,k)`，返回 `int`。
- `init(int n)` -> 初始化/清空结构

```cpp
struct Comb998 {
    static constexpr int MOD = 998244353;
    vector<int> fac, ifac;

    long long mod_pow(long long a, long long e) const {
        long long r = 1;
        while (e > 0) {
            if (e & 1) r = r * a % MOD;
            a = a * a % MOD;
            e >>= 1;
        }
        return r;
    }

    void init(int n) {
        fac.assign(n + 1, 1);
        ifac.assign(n + 1, 1);
        for (int i = 1; i <= n; ++i) fac[i] = 1LL * fac[i - 1] * i % MOD;
        ifac[n] = mod_pow(fac[n], MOD - 2);
        for (int i = n; i >= 1; --i) ifac[i - 1] = 1LL * ifac[i] * i % MOD;
    }

    int C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return 1LL * fac[n] * ifac[k] % MOD * ifac[n - k] % MOD;
    }
};

int bottle_raney_answer(int n, int m, int c, int a, int b, const Comb998& comb) {
    const int MOD = Comb998::MOD;
    if (m < n || (m - n) % c != 0) return 0;
    int win = (m - n) / c;
    if (win < 0 || win > m) return 0;

    long long inv_b = comb.mod_pow(b, MOD - 2);
    long long p = 1LL * a * inv_b % MOD;
    long long q = 1LL * (b - a) * inv_b % MOD;

    long long ans = n % MOD;
    ans = ans * comb.mod_pow(m, MOD - 2) % MOD;
    ans = ans * comb.C(m, win) % MOD;
    ans = ans * comb.mod_pow(p, win) % MOD;
    ans = ans * comb.mod_pow(q, m - win) % MOD;
    return (int)ans;
}
```

典题：Round3 B《再买一瓶》。看到“合法循环移位个数正好等于总和”的味道，优先想 Raney 引理。
#### DAG 连续全 1 段：三组划分转最大权闭合图

> **赛时先看**
> - **题目信号**：要求在某个拓扑序中出现“连续的一段”；边约束使得段内集合必须序凸；点分为“段前、段中、段后”三类。
> - **本质**：在 DAG 的所有拓扑序里，最大化某个连续段内满足条件的点数，并构造达到最优的拓扑序。
> - **接法**：令 `s_i = [group_i >= 1]`，`t_i = [group_i >= 2]`，则 `s_i >= t_i`。最大化组 1 数量就是最大化 `s_i - t_i`，用最大权闭合图表达点权和蕴含约束。
> - **复杂度判定**：建最大权闭合图后跑最大流。若源点正权总和为 `O(n)`，Dinic 通常足够。
> - **维护的量**：每个点的一对状态 `s_i/t_i`（是否属于段内/段后），以及最大权闭合图网络（源点→正权点、负权点→汇点、蕴含边容量无穷）与跑完流后的残量网络。
> - **警告**：对每条边 `u -> v`，三组编号必须满足 `group[u] <= group[v]`；标号为 0 的点不能分到组 1；最后还要按组号优先做一次拓扑排序来构造序列。


建模清单：

- 点权：选择 `s_i` 给 `+1`，选择 `t_i` 给 `-1`，于是只有 `s_i=1,t_i=0` 贡献 1。
- 自身约束：`t_i -> s_i`。
- 边约束：`u -> v` 推出 `s_u -> s_v` 和 `t_u -> t_v`。
- 标号 0：禁止组 1，可加无限容量约束逼迫 `s_i` 与 `t_i` 同时选或同时不选。
- 残量网络源点侧读出变量取值，再按 `group=0,1,2` 做拓扑排序。

典题：Round3 D《最长的连续的一》。如果目标从“点数最大”改成“权值最大”，把 `+1/-1` 换成对应点权即可。

#### 两行扫雷条带 DP：按列 mask 统计方案数

> **赛时先看**
> - **题目信号**：高度很小，数字约束只依赖相邻几列；题目要求构造“恰好 x 种方案”的局面，`x` 范围不大，可以搜索后打表。
> - **本质**：给一个两行扫雷条带，第一行全是 `*`，第二行由数字和 `*` 组成，统计有多少种合法地雷分布。可以作为构造题的离线搜索校验器。
> - **接法**：枚举第二行字符串，用下面函数算方案数；搜到 `x=1..300` 的短条带后，比赛代码直接输出打表结果。
> - **复杂度判定**：宽度为 `w` 时 `O(w * 4^3)`，实际就是常数很小的线性 DP。
> - **维护的量**：`dp[older][prev]`：前两列掩码分别为 `older`/`prev` 时的合法方案数；每列枚举 `cur` 后滚动成 `ndp`。
> - **警告**：第二行是数字时，该格一定不是雷；数字只看左、中、右三列里的相邻格，自己的格子不计入。

**最小完整示例（先抄这一段就能跑）：**
题目：两行条带、第二行 `bottom` 由数字/`*` 组成，统计合法地雷分布方案数。
```cpp
string bottom = "0*0";
long long cnt = count_minesweeper_strip(bottom);  // bottom 下标 0..w-1，可含 '*' 与数字
printf("%lld\n", cnt);                            // 方案数，可能很大用 long long
```
样例：`count_minesweeper_strip("0*0")=1`（所有格无雷，只有 1 种）；`("1*1")=3`。

**传参要求（照这个传不会错）：**
- `bottom`：第二行字符串，长度 `w`，下标 `0..w-1`；数字 `'0'..'9'` 表示该格必须无雷且周围雷数恰为其值，`'*'` 表示该格任意（第一行不读入，函数自动枚举）；返回 `long long`：合法方案数。


**改板时先认这几个量：**

- `cur`：当前列的掩码枚举值。
- `dp`：DP 状态。

```cpp
long long count_minesweeper_strip(const string& bottom) {
    int w = (int)bottom.size();

    auto is_digit = [&](int col) {
        return 0 <= col && col < w && isdigit((unsigned char)bottom[col]);
    };
    auto top_mine = [](int mask) { return mask & 1; };
    auto bottom_mine = [](int mask) { return (mask >> 1) & 1; };

    auto digit_ok = [&](int col, int left_mask, int mid_mask, int right_mask) {
        if (!is_digit(col)) return true;
        if (bottom_mine(mid_mask)) return false;
        int mines = 0;
        mines += top_mine(left_mask) + top_mine(mid_mask) + top_mine(right_mask);
        mines += bottom_mine(left_mask) + bottom_mine(right_mask);
        return mines == bottom[col] - '0';
    };

    long long dp[4][4] = {};
    dp[0][0] = 1; // 还没放任何列时，虚拟的第 -2、第 -1 列都是空列。

    for (int col = 0; col <= w; ++col) {
        long long ndp[4][4] = {};
        for (int older = 0; older < 4; ++older) {
            for (int prev = 0; prev < 4; ++prev) {
                if (!dp[older][prev]) continue;
                for (int cur = 0; cur < 4; ++cur) {
                    if (col == w && cur != 0) continue; // 最右侧虚拟空列。
                    if (col < w && is_digit(col) && bottom_mine(cur)) continue;
                    if (col >= 1 && !digit_ok(col - 1, older, prev, cur)) continue;
                    ndp[prev][cur] += dp[older][prev];
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    long long ans = 0;
    for (int a = 0; a < 4; ++a) ans += dp[a][0];
    return ans;
}
```

典题：Round3 E《扫雷》。这个板子本身是“计数器”，不是最终构造；构造题要在赛前或程序启动时搜索字符串并保存答案。
#### 矩阵同值分割：出现行前后缀列界 + 二维差分

> **赛时先看**
> - **题目信号**：矩阵按值分组；同一个值在若干行出现；需要找相邻出现行之间能被覆盖的行列区域。
> - **本质**：对矩阵中每个相同值独立处理，判断哪些矩形会被该值的上下两部分出现位置共同覆盖，最后用二维差分累计被标记次数。
> - **接法**：对某个值的出现行 `row[0..k-1]`，做 `pref_min_col[t]` 和 `suf_max_col[t]`。每个分割 `t` 若 `pref_min_col[t] < suf_max_col[t+1]`，就在差分数组中给矩形 `[row[t], row[t+1]] * [L, R]` 加一。
> - **复杂度判定**：设矩阵大小为 `n*m`，总复杂度 `O(nm log(nm) + nm)`，若值域可直接桶排可做到近似 `O(nm)`。
> - **维护的量**：`pos[value][row]`：该值在每行的最左列 `mn` 与最右列 `mx`；每行出现行序列的前缀最小列 `pref_min`、后缀最大列 `suf_max`；累计覆盖次数的二维差分 `diff`。
> - **警告**：每个值只需要记每一行的最左和最右出现列；分割点在相邻出现行之间；题解条件是 `L < R`，不是 `L <= R`。

**最小完整示例（先抄这一段就能跑）：**
题目：1-indexed 矩阵，统计每个格子被"同值上/下两部分的出现位置"共同覆盖的次数。
```cpp
vector<vector<int>> cover = mark_same_value_rectangles(a);
// a 是 (n+1)x(m+1)，有效数据在 a[1..n][1..m]；cover[i][j] 为 (i,j) 的覆盖次数
if (cover[i][j] > 0) { /* (i,j) 至少被一个值覆盖 */ }
```
样例：`a[1..2][1..2]` 全为 `1` 时，`cover[1][1]=cover[1][2]=cover[2][1]=cover[2][2]=1`。

**传参要求（照这个传不会错）：**
- `a`：`(n+1) x (m+1)` 的矩阵，下标 `1..n` 行、`1..m` 列有效（函数内部从 `a[1]` 开始读，`a[0]` 行与各行的 0 列不读）；返回 `vector<vector<int>>`：与 `a` 同尺寸的 `cover`，`cover[i][j]` 是 1-indexed 的覆盖次数。


**改板时先认这几个量：**

- `mx`：该行最右出现列。
- `mn`：该行最左出现列。

```cpp
vector<vector<int>> mark_same_value_rectangles(const vector<vector<int>>& a) {
    int n = (int)a.size() - 1;
    int m = (int)a[1].size() - 1;

    // value -> vector of (row, min_col, max_col)。先临时用 map 合并同一行。
    map<int, map<int, pair<int, int>>> pos;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            auto& p = pos[a[i][j]][i];
            if (p.first == 0) p = {j, j};
            else {
                p.first = min(p.first, j);
                p.second = max(p.second, j);
            }
        }
    }

    vector<vector<int>> diff(n + 3, vector<int>(m + 3, 0));
    auto add_rect = [&](int x1, int y1, int x2, int y2) {
        diff[x1][y1]++;
        diff[x2 + 1][y1]--;
        diff[x1][y2 + 1]--;
        diff[x2 + 1][y2 + 1]++;
    };

    for (auto& [value, rows_map] : pos) {
        int k = (int)rows_map.size();
        if (k <= 1) continue;

        vector<int> row, mn, mx;
        for (auto& [r, p] : rows_map) {
            row.push_back(r);
            mn.push_back(p.first);
            mx.push_back(p.second);
        }

        vector<int> pref_min(k), suf_max(k);
        for (int i = 0; i < k; ++i) {
            pref_min[i] = (i == 0 ? mn[i] : min(pref_min[i - 1], mn[i]));
        }
        for (int i = k - 1; i >= 0; --i) {
            suf_max[i] = (i == k - 1 ? mx[i] : max(suf_max[i + 1], mx[i]));
        }

        for (int t = 0; t + 1 < k; ++t) {
            int L = pref_min[t];
            int R = suf_max[t + 1];
            if (L < R) add_rect(row[t], L, row[t + 1], R);
        }
    }

    vector<vector<int>> cover(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            diff[i][j] += diff[i - 1][j] + diff[i][j - 1] - diff[i - 1][j - 1];
            cover[i][j] = diff[i][j];
        }
    }
    return cover;
}
```

典题：Round3 G《矩阵标记》。如果题目只问是否被至少一个值标记，把 `cover[i][j] > 0` 当布尔值即可。
#### 一次交换最大邻差：局部贡献函数扫描

> **赛时先看**
> - **题目信号**：目标是 `sum |a[i]-a[i-1]|`；操作是交换两个元素；直接枚举交换点 `O(n^2)` 超时；每个位置只和左右邻居有关。
> - **本质**：最大化序列相邻绝对差之和，允许交换两个位置一次。非相邻交换只影响四条边，可写成两个位置局部贡献函数的组合。
> - **复杂度判定**：相邻交换 `O(n)`；非相邻交换可按有限斜率集合扫描做到 `O(n * K^2)`，其中 `K` 为局部绝对值函数拆出的斜率数量，通常是常数。
> - **维护的量**：原始答案 `base`；每个位置 `p` 的局部贡献函数 `g_p(x)=|x-a[p-1]|+|x-a[p+1]|`（端点只含一个邻居）；扫描右端点时历史左端点集合的「斜率-截距」最优值。
> - **警告**：相邻位置交换和非相邻位置交换的受影响边不同，要分开算；扫描右端点 `j` 时，历史集合只能加入 `i <= j-2`，否则会把相邻交换混进去。


- 先算原始答案 `base`。
- 枚举相邻交换，直接重算受影响的至多三条边。
- 对非相邻交换，把位置 `p` 的贡献写成 `g_p(x)=|x-a[p-1]|+|x-a[p+1]|`，端点只有一个邻居。
- `g_p(x)` 是若干条直线的最大值。枚举右端点实际放入的值，在历史左端点集合里查 `best_slope_intercept + slope * value`。

典题：Round3 I《交换大师》。这题比板子更重要的是“只重算受影响边”的习惯；很多交换/替换/删除一项的题都靠这个降维。

#### 折线路径转向判定：叉积符号

> **赛时先看**
> - **题目信号**：题面直接问 `LEFT / RIGHT / STRAIGHT`；坐标是整数；只涉及相邻三个点的方向。
> - **本质**：给连续三个点，判断从第一段走到第二段是左转、右转还是直行。
> - **接法**：按路径顺序读点，对每个 `i=2..n-1` 调用 `turn(a[i-1], a[i], a[i+1])`。
> - **复杂度判定**：每个转向 `O(1)`。
> - **维护的量**：无额外结构；只维护相邻三个点 `a,b,c` 与叉积 `z = cross(a,b,c)`。
> - **警告**：向量应写成 `b-a` 和 `c-b`；叉积为正是左转，负是右转，零是直行。坐标到 `1e9` 时叉积要用 `long long`。

**最小完整示例（先抄这一段就能跑）：**
题目：按顺序读 `n` 个点，对每三个相邻点输出转向是 `LEFT / RIGHT / STRAIGHT`。
```cpp
PointLL a, b, c;
cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y;
string d = turn_direction(a, b, c);   // 从 a→b 再到 c 的转向
cout << d << "\n";
```
样例：`a=(0,0), b=(1,0), c=(1,1)` → `LEFT`；`c=(1,-1)` → `RIGHT`；`c=(2,0)` → `STRAIGHT`。

**传参要求（照这个传不会错）：**
- `a`：路径上的前一个点；`b`：当前点；`c`：下一个点。均为 `PointLL`（`long long x, y`），坐标绝对值 `1e9` 内安全，`x,y` 需先赋值再调用；`turn_direction` 返回 `string`：`"LEFT"`/`"RIGHT"`/`"STRAIGHT"`；`cross(a,b,c)` 返回 `long long` 叉积值。


```cpp
struct PointLL {
    long long x, y;
};

long long cross(PointLL a, PointLL b, PointLL c) {
    long long x1 = b.x - a.x;
    long long y1 = b.y - a.y;
    long long x2 = c.x - b.x;
    long long y2 = c.y - b.y;
    return x1 * y2 - y1 * x2;
}

string turn_direction(PointLL a, PointLL b, PointLL c) {
    long long z = cross(a, b, c);
    if (z > 0) return "LEFT";
    if (z < 0) return "RIGHT";
    return "STRAIGHT";
}
```

典题：Round3 K《转向导航》。如果题面说“顺时针/逆时针”，注意坐标系是否仍然是数学坐标系。
#### 严格升高移动博弈：按高度反向拓扑

> **赛时先看**
> - **题目信号**：每次移动后高度严格上升，天然无环；不能移动的人输；多次询问起点胜负。
> - **本质**：网格上只能走到严格更高的相邻格，判断从每个格出发的普通博弈胜负。
> - **接法**：把每个格子看成 DAG 点。`win[u] = exists(v higher neighbor of u && !win[v])`。
> - **复杂度判定**：排序 `O(nm log(nm))`，转移 `O(nm)`，每次询问 `O(1)`。
> - **维护的量**：`win[i][j]`：从格 `(i,j)` 出发的胜负态（0 必败 / 1 必胜），按高度从高到低逐个填充。
> - **警告**：要按高度从高到低算，这样“更高邻居”的胜负已经知道；存在一个后继为必败态，当前就是必胜态。

**最小完整示例（先抄这一段就能跑）：**
题目：网格每次只能走到严格更高的相邻格，不能走者输；输出每个格的胜负态并回答起点询问。
```cpp
vector<vector<int>> win = increasing_grid_game(h);
// h 是 (n+1)x(m+1) 的 1-indexed 高度矩阵，win[i][j] 是 (i,j) 的胜负态
if (win[sx][sy]) puts("First"); else puts("Second");
```
样例：2x2 高度 `{2,1;1,0}`（1-indexed）时，`win[1][1]=0`、`win[1][2]=1`、`win[2][1]=1`、`win[2][2]=0`。

**传参要求（照这个传不会错）：**
- `h`：`(n+1) x (m+1)` 的高度矩阵，有效数据在 `h[1..n][1..m]`（函数从 `h[1]` 起读，0 行/0 列不读）；返回 `vector<vector<int>>`：与 `h` 同尺寸的 `win`，`win[i][j] ∈ {0,1}`，1-indexed，为从 `(i,j)` 出发的胜负态。


```cpp
vector<vector<int>> increasing_grid_game(const vector<vector<int>>& h) {
    int n = (int)h.size() - 1;
    int m = (int)h[1].size() - 1;
    vector<tuple<int, int, int>> cells;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) cells.push_back({h[i][j], i, j});
    }
    sort(cells.rbegin(), cells.rend()); // 高度从大到小。

    vector<vector<int>> win(n + 1, vector<int>(m + 1, 0));
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    for (auto [height, x, y] : cells) {
        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir], ny = y + dy[dir];
            if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
            if (h[nx][ny] > h[x][y] && !win[nx][ny]) {
                win[x][y] = 1;
            }
        }
    }
    return win;
}
```

典题：Round3 L《登山对决》。若移动规则从“更高”变成“更低”，排序方向反过来。
#### 树上约束重连：最深要求祖先 + 可并堆

> **赛时先看**
> - **题目信号**：所有约束都在一棵原树的祖先关系里；某个点必须挂到若干候选祖先中“最深的那个”才最优；子树需求需要向上合并。
> - **本质**：构造一棵新树，要求每个点的新父亲必须是原树严格祖先，并满足若干“v 仍为 u 的祖先”约束，同时最小化新树深度和。
> - **复杂度判定**：用左偏树 / 可并堆维护每个点的要求祖先集合，`O((n+q) log q)`。
> - **维护的量**：每个点 `x` 的要求祖先堆 `need[x]`（按原树深度为关键字，堆顶是最深必须祖先）；新父亲数组 `parent_new[x]`；DFS 回溯时儿子堆合并后的剩余要求。
> - **警告**：处理顺序是原树自底向上；如果 `need[x]` 为空，`x` 的新父亲可以直接接到根；取出最深要求祖先 `d` 后，要弹掉重复的 `d`，剩余要求并入 `need[d]`。


- 对约束 `(u,v)`，把 `v` 按原树深度作为关键字放进 `need[u]`。
- DFS 回溯到点 `x` 时，先把儿子的堆合进来。
- `need[x]` 堆顶是当前最深的必须祖先 `d`，令 `parent_new[x]=d`。
- 弹掉所有等于 `d` 的要求，剩余堆并入 `need[d]`。
- 最后按新父亲计算深度和。

典题：Round3 J《树.zip》。这类题别急着写倍增，核心是“最深约束优先”这个贪心。

#### 有向边状态随机游走：树上无立即回头期望

> **赛时先看**
> - **题目信号**：走到点 `u` 后不能立刻回到 `pre`，除非没有其他邻居；目标点吸收；树结构使每个子树只通过一条边和外界相连。
> - **本质**：在树上随机游走，状态与“当前点 + 上一个点”有关，求到目标点的期望步数。
> - **复杂度判定**：树上两遍 DFS，`O(n)`。
> - **维护的量**：每个非根点 `u` 的一次函数系数 `a_u/b_u`（把“从父亲进入 u 的期望”写成 `F_u = a_u*B_u + b_u`，其中 `B_u` 是“从 u 回父亲之后”的外部期望）；前序代回后每条有向边 `(pre -> u)` 的真实期望值。
> - **警告**：状态不是点，而是有向边 `(pre -> u)`；对子树可以先写成 `F_u = a_u * B_u + b_u`，其中 `B_u` 是从 `u` 走回父亲方向后的外部期望。


- 以目标点 `t` 为根。
- 后序处理每个非根点 `u`，把“从父亲进入 u 后的期望”表示成关于“从 u 回父亲后的期望”的一次函数。
- 根的外部期望为 0，再前序把每条边的真实期望代回。
- 起点为 `s` 时，若 `s=t` 答案为 0；否则答案是第一步 `1` 加上从相邻点状态出发的期望平均。

典题：Round3 M《漫游者》。遇到“不能回到上一点”的随机游走，先把状态改成有向边，不要只给每个点设一个期望。

#### 赛后模型总览：2026 牛客暑期多校训练营 4

> **赛时先看**
> - **题目信号**：只记得题号或题面关键词，不确定应该翻构造、DP、图论还是几何。
> - **本质**：补 Round4 时先按题号定位模型，再去下方模板或全册对应专题查实现。
> - **复杂度判定**：索引页不涉及算法复杂度。
> - **警告**：G/L 是重实现题，比赛时更需要复盘维护量是否值得；A/C/E/F/H/I/J/K 都能沉淀成相对稳定的模板。


题号到模型：

- A《Sixteen》：固定 16 阶矩阵，把行列式变成小系数线性表示，构造给定巨大整数。
- B《Quadratic Residue》：令 `p+q` 为完全平方数，同时满足两个二次剩余条件。
- C《Retest Queue》：测试点排序，子集 DP + 高维前缀和预处理“能通过当前前缀的程序代价”。
- D《The Game》：循环最小表示下的双人对抗排列构造。
- E《DPRS》：强连通带权有向图价值可只枚举边；单条边降权询问用原全源最短路 `O(m)` 回答。
- F《23 Subsequences》：合法子序列增长至少翻倍，最大长度约 60；按长度逐层求最早结束位置。
- G《Fading Memories》：区间消除长度由极值折线描述，线段树节点维护四条折线并可合并。
- H《String》：字符串按 `s+t<t+s` 排序，修改只会把最靠前非 `a` 改成 `a`，DP 状态只留全 `a` 前缀长度和后缀。
- I《Rounddog》：模式串无 border，循环串出现次数为 0/1/至少 2 三类讨论。
- J《Walk》：最短操作数下的栈式访问，BFS 全源最短路 + 区间 DP 求最小最大栈深。
- K《Decomposition Trees》：任意点分中心的点分树数量，统计根到固定点路径长度并树上背包。
- L《Geometry》：凸多边形缩放内切圆覆盖带权点，边 Voronoi 图 + 二分 + 区间扫描。

#### 16 阶行列式构造：小元素矩阵表示任意目标值

> **赛时先看**
> - **题目信号**：题面要求输出任意矩阵 / 图 / 序列，使某个代数值等于目标；目标值很大但单个元素范围很小。
> - **本质**：构造一个 `16 * 16` 的整数矩阵，元素范围在 `[-16,16]` 内，使行列式等于给定整数 `x`。
> - **接法**：直接复制 `construct_det_16(x)`。输出时先输出 `16`，再输出矩阵。
> - **复杂度判定**：每个目标常数级搜索，约 `16 * 33` 个候选。
> - **维护的量**：递归搜索中的系数数组 `coef[k]` 与剩余目标 `target`；预处理的 31 的幂 `pow31`；除此之外无其它长期结构。
> - **警告**：输入的 `x` 可能超过 `long long`，要用 `__int128` 读；构造出的矩阵元素不能越界；这是特化构造，别拿去求普通行列式。

**最小完整示例（先抄这一段就能跑）：**
题目：输入可能超 `long long` 的整数 `x`，输出一个 16×16 矩阵使行列式等于 `x`（元素范围 `[-16,16]`）。
```cpp
i128 x = read_i128_string(s);          // s 为读入的十进制串，支持负号
auto mat = construct_det_16(x);        // mat 是 16x16，元素在 [-16,16]
printf("16\n");                        // 先输出 16，再逐行输出 mat 的每个元素
```
样例：`read_i128_string("-123") = -123`；`construct_det_16(5)` 返回行列式为 5 的 16×16 矩阵。

**传参要求（照这个传不会错）：**
- `read_i128_string(const string& s)`：`s` 为十进制整数字符串，下标 `0..len-1`，可带前导 `-`；返回 `i128`（`__int128_t`）。
- `construct_det_16(i128 x)`：`x` 为目标行列式值（把上面读到的数直接传入即可，可为负或 0）；返回 `vector<vector<int>>`：16×16 矩阵，元素在 `[-16,16]`，行列式恰为 `x`。


```cpp
using i128 = __int128_t;

i128 abs_i128(i128 x) {
    return x < 0 ? -x : x;
}

i128 read_i128_string(const string& s) {
    bool neg = false;
    int p = 0;
    if (!s.empty() && s[0] == '-') neg = true, p = 1;
    i128 x = 0;
    for (; p < (int)s.size(); ++p) x = x * 10 + (s[p] - '0');
    return neg ? -x : x;
}

bool det16_dfs(int k, i128 target, const vector<i128>& pow31, vector<int>& coef) {
    if (k == 1) {
        if (-16 <= target && target <= 16) {
            coef[0] = (int)target;
            return true;
        }
        return false;
    }

    i128 weight = 16 * pow31[k - 2];
    vector<pair<i128, int>> candidate;
    for (int v = -16; v <= 16; ++v) {
        i128 rem = target - (i128)v * weight;
        if (rem % 15 != 0) continue;
        i128 nxt = rem / 15;
        i128 max_possible = 16 * pow31[k - 2];
        if (abs_i128(nxt) <= max_possible) candidate.push_back({abs_i128(nxt), v});
    }

    sort(candidate.begin(), candidate.end());
    for (auto [unused_abs, v] : candidate) {
        coef[k - 1] = v;
        if (det16_dfs(k - 1, (target - (i128)v * weight) / 15, pow31, coef)) {
            return true;
        }
    }
    return false;
}

vector<vector<int>> construct_det_16(i128 x) {
    const int N = 16;
    vector<i128> pow31(N + 1, 1);
    for (int i = 1; i <= N; ++i) pow31[i] = pow31[i - 1] * 31;

    vector<int> coef(N, 0);
    bool ok = det16_dfs(N, x, pow31, coef);
    assert(ok);

    vector<vector<int>> a(N, vector<int>(N, 0));
    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j <= i; ++j) {
            a[i][j] = ((i + j) % 2 == 0 ? 16 : -16);
        }
        a[i][i + 1] = 15;
    }

    for (int j = 0; j < N; ++j) {
        int sign = ((N + j + 1) % 2 == 0 ? 1 : -1);
        a[N - 1][j] = sign * coef[j];
    }
    return a;
}
```

典题：Round4 A《Sixteen》。构造题先找“目标量对某些变量是线性的”这一层，剩下就是小范围表示。
#### 平方和二次剩余构造：让 p+q 成为平方数

> **赛时先看**
> - **题目信号**：两个模数互相作为剩余；条件看似对称；可以把两边都改写成同一个数 `p+q`。
> - **本质**：给定正整数 `p`，构造 `x1,x2,q`，满足 `x1^2 ≡ p (mod q)` 且 `x2^2 ≡ q (mod p)`。
> - **接法**：枚举平方数 `s=x^2>p`，令 `q=s-p`。因为 `p ≡ s (mod q)` 且 `q ≡ s (mod p)`，直接取 `x1=x%q`、`x2=x%p`。
> - **复杂度判定**：从 `ceil(sqrt(p))` 往上试，实际很快。
> - **维护的量**：无额外结构；只维护当前枚举的平方根 `x` 与算出的 `q = x*x - p`，四个整除/非零条件一起判。
> - **警告**：题面要求 `1 <= x1 < q`、`1 <= x2 < p`，所以 `x % q` 和 `x % p` 不能为 0；`q` 可能到 `1e12`，用 `long long`。

**最小完整示例（先抄这一段就能跑）：**
题目：给 `p`，构造 `x1,x2,q` 使 `x1^2 ≡ p (mod q)` 且 `x2^2 ≡ q (mod p)`。
```cpp
auto [x1, x2, q] = construct_quadratic_residue(p);  // p 为 int，结果自动满足 1<=x1<q、1<=x2<p
printf("%lld %lld %lld\n", x1, x2, q);
```
样例：`construct_quadratic_residue(5)` 返回 `(3, 3, 4)`（`3^2 ≡ 5 (mod 4)` 且 `3^2 ≡ 4 (mod 5)`）。

**传参要求（照这个传不会错）：**
- `p`：给定的正整数，`int`（`q` 可能到 `1e12`，函数内部用 `long long`）；返回 `tuple<long long, long long, long long>`：`(x1 = x%q, x2 = x%p, q)`，满足两个同余式与取值范围约束。


```cpp
tuple<long long, long long, long long> construct_quadratic_residue(int p) {
    long long x = sqrt((long double)p) + 1;
    while (true) {
        long long q = x * x - p;
        if (q > 0 && q % p != 0 && x % p != 0 && x % q != 0) {
            return {x % q, x % p, q};
        }
        ++x;
    }
}
```

典题：Round4 B《Quadratic Residue》。这类构造题不要只盯着“平方剩余算法”，先看能否手造一个共同平方数。
#### 重测队列：子集 DP + 高维前缀和

> **赛时先看**
> - **题目信号**：测试点数量 `m <= 20`；要排列所有测试点；某个程序会在第一个失败点停止；状态自然是“已经排好的测试点集合”。
> - **本质**：所有程序使用同一个测试点顺序；每个程序遇到第一个不通过测试点就停止。求测试点排序使总时间最小。
> - **接法**：先把每个程序能通过的测试点集合记为 `passMask`，把它在每个测试点的耗时加到 `cost[j][passMask]`。对每个 `j` 做超集和，得到所有 `S subset passMask` 的贡献。
> - **复杂度判定**：`O(m^2 2^m + n m)`，空间 `O(m 2^m)`。
> - **维护的量**：`cost[j][S]`（已排完集合 S 后仍通过的程序在测试点 j 上的耗时和，超集和刷成 `S subset passMask` 的总和）；`dp[mask]`（排完集合 mask 的最小总耗时）。
> - **警告**：`cost[j][S]` 表示“已经排完集合 S 后，仍能通过 S 的程序跑测试点 j 的总时间”；不是只看 S 中最后一个测试点。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：m<=20 个测试点重排，n 个程序遇首个不通过点即停，求最小总耗时。
vector<vector<long long>> d = {{1, 2}, {3, 1}};  // d[i][j]：程序 i 在测试点 j 的耗时
vector<string> verdict = {"AA", "RA"};           // verdict[i][j]=='A'：程序 i 能通过测试点 j
long long ans = minimum_retest_time(d, verdict); // 最小总耗时，直接输出
// 样例：测试点 1 排在 2 前时总耗时 6，ans = 6
```

**传参要求（照这个传不会错）：**

- `d`：`n` 行 `m` 列耗时矩阵，`d[i][j]` 是程序 i 跑测试点 j 的耗时（0-indexed），值可到 1e18。
- `verdict`：`n` 个长度 `m` 的字符串，`'A'` 表示能通过该测试点，其余字符（如 `'R'`）表示不通过。
- 返回：`long long`，最优重排下的总耗时（各程序跑到首个失败点即停的时间之和）。

**改板时先认这几个量：**

- `bit`：按位遍历的循环变量。
- `dp`：DP 状态。

```cpp
long long minimum_retest_time(const vector<vector<long long>>& d, const vector<string>& verdict) {
    int n = (int)d.size();
    int m = (int)d[0].size();
    int B = 1 << m;
    vector<long long> cost((long long)m * B, 0);

    for (int i = 0; i < n; ++i) {
        int pass_mask = 0;
        for (int j = 0; j < m; ++j) {
            if (verdict[i][j] == 'A') pass_mask |= 1 << j;
        }
        for (int j = 0; j < m; ++j) {
            cost[(long long)j * B + pass_mask] += d[i][j];
        }
    }

    // 超集和：处理后 cost[j][S] = 所有 passMask 包含 S 的程序在 j 上的耗时和。
    for (int j = 0; j < m; ++j) {
        long long* a = cost.data() + (long long)j * B;
        for (int bit = 0; bit < m; ++bit) {
            for (int mask = 0; mask < B; ++mask) {
                if ((mask & (1 << bit)) == 0) a[mask] += a[mask | (1 << bit)];
            }
        }
    }

    const long long INF = (long long)4e18;
    vector<long long> dp(B, INF);
    dp[0] = 0;
    for (int mask = 0; mask < B; ++mask) {
        int rest = (B - 1) ^ mask;
        while (rest) {
            int bit = __builtin_ctz(rest);
            int nmask = mask | (1 << bit);
            dp[nmask] = min(dp[nmask], dp[mask] + cost[(long long)bit * B + mask]);
            rest &= rest - 1;
        }
    }
    return dp[B - 1];
}
```

典题：Round4 C《Retest Queue》。如果题面从 “Accepted/Rejected” 换成“能否通过某条件集合”，核心仍是 `passMask` 的超集和。
#### 循环最小排列对抗构造：The Game

> **赛时先看**
> - **题目信号**：最终评价函数是“循环移位的字典序最小表示”；排列中最小值 1 一定是最小循环移位开头；1 出现前后的选择有不同战略意义。
> - **本质**：两个玩家轮流写一个排列，最终看所有循环移位中字典序最小者，求双方最优下的最终结果。
> - **接法**：按题解结论直接构造。
> - **复杂度判定**：每组 `O(n)`。
> - **维护的量**：`ans`（构造出的最终 `f(p)` 序列）；`m = n/2`（两半分界，奇偶公式在此分叉）。
> - **警告**：偶数和奇数公式不同；输出的是最终的 `f(p)`，不是双方实际落子的原排列。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：两人轮流写排列 p，最终取字典序最小的循环移位，构造这个 f(p)。
int n = 6;                                        // 排列长度，奇偶走不同构造分支
vector<int> ans = game_lexicographically_min_rotation_result(n);
// 样例：n=6 -> {1,4,3,5,2,6}；n=5 -> {1,4,2,3,5}
```

**传参要求（照这个传不会错）：**

- `n`：排列长度，`n >= 1`；`n = 1` 时直接返回 `{1}`。
- 返回：`vector<int>`，长度 `n`，即最优博弈结果 `f(p)`，按序输出即可；不是双方实际落子的排列。

```cpp
vector<int> game_lexicographically_min_rotation_result(int n) {
    vector<int> ans;
    if (n == 1) return {1};

    if (n % 2 == 0) {
        int m = n / 2;
        ans.push_back(1);
        for (int i = 1; i <= m; ++i) {
            ans.push_back(m + i);
            if (i < m) ans.push_back(m + 1 - i);
        }
    } else {
        int m = n / 2;
        ans.push_back(1);
        for (int i = 1; i <= m; ++i) {
            ans.push_back(m + 3 - i);
            if (i == 1) ans.push_back(2);
            else ans.push_back(m + i + 1);
        }
    }
    return ans;
}
```

典题：Round4 D《The Game》。博弈构造题经常先证明某一位的上下界相同，再继续固定前缀。
#### 最短路比值只枚举边：DPRS 降边权询问

> **赛时先看**
> - **题目信号**：最大值看似要枚举所有点对，但图中边权全正；修改只有单条边降权；`n,m,q` 总量约 2000。
> - **本质**：强连通带权有向图中，价值定义为 `max dis(i,j)/dis(j,i)`。支持一次询问临时降低一条边权，求新价值。
> - **接法**：原图全源 Dijkstra 得到 `d`。询问把 `a->b` 从 `w` 降到 `x`，新距离 `dx(u,v)=min(d(u,v), d(u,a)+x+d(b,v))`，然后枚举每条边的贡献。
> - **复杂度判定**：预处理 `O(nm log n)`；每次询问 `O(m)`。
> - **维护的量**：`edges`（1-indexed 边表，`edges[0]` 占位）；`g`（邻接表）；`dis[s][t]`（原图全源最短路，`INF = 1LL<<62`）。
> - **警告**：价值只需枚举边 `e=(u,v,w)` 的 `dis(v,u)/w`；降权后任意最短路至多使用被修改边一次。
> - **约定**：vector<Edge> edges; // 1-indexed，edges[0] 不用


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：强连通带权有向图，每次把第 id 条边临时降权，求 max dis(i,j)/dis(j,i)。
DPRS solver;                                      // 先构造结构
solver.init(n, input_edges);                      // 顶点 1..n，边表从下标 1 开始
long double val = solver.query_decrease_edge(id, new_w);  // 第 id 条边降为 new_w 后查询
// 样例：三角环 1->2(10),2->3(1),3->1(1)，query_decrease_edge(1,3) 返回 4
```

**传参要求（照这个传不会错）：**

- `init(n_, input_edges)`：`n_` 为点数（顶点 `1..n_`）；`input_edges` 是 `vector<DPRS::Edge>`，下标从 1 开始（`edges[0]` 占位不用），每条边 `{u,v,w}` 权全正。
- `query_decrease_edge(id, new_w)`：`id` 为被降权边下标（`1 <= id < edges.size()`）；`new_w` 为降权后的新边权，满足 `0 < new_w < edges[id].w`。
- 返回：`long double`，降权后的最大不对称度（枚举每条边取 `dis(v,u)/w_e` 的最大值）。

**API / 入口函数（赛时只认这里列的名字）：**

- `init(int n_, const vector<Edge>& input_edges)` -> 初始化/清空结构
**改板时先认这几个量：**

- `edges`：1-indexed，edges[0] 不用。
- `g`：邻接表。

```cpp
struct DPRS {
    struct Edge {
        int u, v;
        long long w;
    };

    int n = 0;
    vector<Edge> edges; // 1-indexed，edges[0] 不用。
    vector<vector<pair<int, long long>>> g;
    vector<vector<long long>> dis;
    static constexpr long long INF = (1LL << 62);

    void init(int n_, const vector<Edge>& input_edges) {
        n = n_;
        edges = input_edges;
        g.assign(n + 1, {});
        for (int i = 1; i < (int)edges.size(); ++i) {
            g[edges[i].u].push_back({edges[i].v, edges[i].w});
        }
        dis.assign(n + 1, vector<long long>(n + 1, INF));
        for (int s = 1; s <= n; ++s) dijkstra(s);
    }

    void dijkstra(int s) {
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        dis[s][s] = 0;
        pq.push({0, s});
        while (!pq.empty()) {
            auto [du, u] = pq.top();
            pq.pop();
            if (du != dis[s][u]) continue;
            for (auto [v, w] : g[u]) {
                if (dis[s][v] > du + w) {
                    dis[s][v] = du + w;
                    pq.push({dis[s][v], v});
                }
            }
        }
    }

    long double query_decrease_edge(int id, long long new_w) const {
        int a = edges[id].u;
        int b = edges[id].v;
        long double ans = 0;
        for (int i = 1; i < (int)edges.size(); ++i) {
            int u = edges[i].u;
            int v = edges[i].v;
            long long denominator = (i == id ? new_w : edges[i].w);

            long long back = dis[v][u];
            if (dis[v][a] < INF && dis[b][u] < INF) {
                back = min(back, dis[v][a] + new_w + dis[b][u]);
            }
            ans = max(ans, (long double)back / (long double)denominator);
        }
        return ans;
    }
};
```

典题：Round4 E《DPRS》。这个结论也适合记成一句话：最大往返不对称度由某条边的反向最短路除以边权达到。
#### 23 子序列：长度很小的区间最长合法子序列

> **赛时先看**
> - **题目信号**：每一步至少翻倍，值域最多 `1e18`，所以最长长度不超过约 60；询问很多但长度层数很少。
> - **本质**：给定序列 `a`，多次询问区间 `[l,r]` 内最长子序列长度，要求相邻选择值满足 `2*b[i-1] <= b[i] <= 3*b[i-1]`。
> - **接法**：`end_len[i]` 表示以 `i` 为第一个元素、长度为当前层的合法子序列最早能在哪个位置结束。每加一层，用线段树按值域查 `[2a[i],3a[i]]` 内的最小结束位置。
> - **复杂度判定**：设最大可行长度为 `K <= 60`，预处理并回答所有询问 `O(K(n log n + q))`。
> - **维护的量**：`prev/cur[i]`（长为当前层的合法子序列在 i 处开始的最早结束位置）；`suffix_best[i]`（后缀最早结束位置的最小值）；`seg`（按值域存最小结束位置的线段树）。
> - **警告**：子序列不是子段，可以跳着选；为了保证下标递增，扫 `i` 时线段树里只能放 `j>i` 的位置。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：多次询问区间 [l,r] 内最长合法子序列，相邻元素满足 2*b[i-1] <= b[i] <= 3*b[i-1]。
vector<long long> a = {0, 1, 2, 6};               // a[0] 占位，序列从 a[1] 开始
vector<pair<int, int>> queries = {{1, 3}};        // 询问闭区间，1-indexed
vector<int> ans = answer_23_subsequence_queries(a, queries);
// 样例：a={1,2,6} 可选出 1->2->6，询问 [1,3] 得 ans[0] = 3
```

**传参要求（照这个传不会错）：**

- `a`：长度 `n+1` 的数组，`a[0]` 占位不用，真实序列在 `a[1..n]`，值可到 1e18。
- `queries`：`q` 个 `{l, r}` 闭区间，`1 <= l <= r <= n`。
- 返回：`vector<int>`，长度 `q`，`ans[id]` 是第 id 个询问的最长合法子序列长度（至少 1）。

**API / 入口函数（赛时只认这里列的名字）：**

- `init(int m)` -> 初始化/清空结构
- `query_min(int l, int r)` -> 查询闭区间最小值 返回 `int`。

```cpp
struct RangeMinPointUpdate {
    int n = 1;
    static constexpr int INF = 1000000000;
    vector<int> t;

    void init(int m) {
        n = 1;
        while (n < m) n <<= 1;
        t.assign(2 * n, INF);
    }

    void reset() {
        fill(t.begin(), t.end(), INF);
    }

    void update_min(int p, int value) {
        int x = p + n - 1;
        t[x] = min(t[x], value);
        for (x >>= 1; x; x >>= 1) t[x] = min(t[x << 1], t[x << 1 | 1]);
    }

    int query_min(int l, int r) const {
        if (l > r) return INF;
        int res = INF;
        l += n - 1;
        r += n - 1;
        while (l <= r) {
            if (l & 1) res = min(res, t[l++]);
            if (!(r & 1)) res = min(res, t[r--]);
            l >>= 1;
            r >>= 1;
        }
        return res;
    }
};

vector<int> answer_23_subsequence_queries(const vector<long long>& a, const vector<pair<int, int>>& queries) {
    int n = (int)a.size() - 1;
    int q = (int)queries.size();
    vector<long long> vals(a.begin() + 1, a.end());
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    int M = (int)vals.size();
    vector<int> pos(n + 1), left_id(n + 1), right_id(n + 1);
    for (int i = 1; i <= n; ++i) {
        pos[i] = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin() + 1;
        left_id[i] = lower_bound(vals.begin(), vals.end(), 2 * a[i]) - vals.begin() + 1;
        right_id[i] = upper_bound(vals.begin(), vals.end(), 3 * a[i]) - vals.begin();
    }

    vector<int> ans(q, 1);
    vector<int> prev(n + 2), cur(n + 2), suffix_best(n + 3);
    for (int i = 1; i <= n; ++i) prev[i] = i;

    RangeMinPointUpdate seg;
    seg.init(M);
    int max_len = min(n, 63); // 1e18 下最多约 60 层。

    for (int len = 2; len <= max_len; ++len) {
        seg.reset();
        fill(cur.begin(), cur.end(), RangeMinPointUpdate::INF);

        for (int i = n; i >= 1; --i) {
            cur[i] = seg.query_min(left_id[i], right_id[i]);
            seg.update_min(pos[i], prev[i]);
        }

        int best = RangeMinPointUpdate::INF;
        for (int i = n; i >= 1; --i) {
            best = min(best, cur[i]);
            suffix_best[i] = best;
        }
        if (best >= RangeMinPointUpdate::INF) break;

        for (int id = 0; id < q; ++id) {
            auto [l, r] = queries[id];
            if (suffix_best[l] <= r) ans[id] = len;
        }
        prev.swap(cur);
    }
    return ans;
}
```

典题：Round4 F《23 Subsequences》。如果比例条件换成 `b[i] >= c*b[i-1]`，最大长度仍可用 `log_c V` 控制。
#### 拼接字符串加前缀改 a：只留 a 前缀长度和后缀

> **赛时先看**
> - **题目信号**：允许把字符改小；最优修改一定从左到右把非 `a` 改成 `a`；剩余未修改字符串要按 `s+t < t+s` 排序。
> - **本质**：若干字符串可任意重排拼接，再把至多 `k` 个字符改成任意小写字母。对每个 `k` 求字典序最小结果。
> - **接法**：状态 `(used_cost, has_partial)` 存二元组 `（全 a 前缀长度， 后缀字符串）`。比较时先让前缀更长，再让后缀字典序更小。
> - **复杂度判定**：总长 `L <= 500` 时，直接存字符串状态 `O(nL^2)`。
> - **维护的量**：`dp[cost][used]`（状态二元组：`pref` 全 a 前缀长度 + `suf` 后缀字符串，`used` 标记是否已选部分修改串）；`answer[k]`（至多 k 次修改的最优结果）。
> - **警告**：最终结构是“若干完全变成 a 的字符串 + 至多一个部分修改字符串 + 未修改后缀”；部分修改字符串只允许选一次。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：字符串可任意重排拼接，至多改 k 个字符为 'a'，对每个 k 求字典序最小结果。
vector<string> s = {"bc", "aa"};                  // 任意小写字符串集合
vector<string> ans = best_strings_after_a_changes(s);  // ans[k]：花至多 k 次修改的最优串
// 样例：s={"bc","aa"} -> ans[0]="aabc"，ans[1]="aaac"
```

**传参要求（照这个传不会错）：**

- `s`：若干小写字符串（可重复、任意顺序），总长 `L <= 500`；按值传入即可（内部会排序，不影响调用方副本）。
- 返回：`vector<string>`，长度 `L+1`，`answer[k]`（`k = 0..L`）是至多改 k 个字符的字典序最小结果。

**改板时先认这几个量：**

- `cur`：当前处理的字符串。
- `nxt`：转移后的新状态。
- `dp`：DP 状态。

```cpp
struct StringState {
    int pref = -1;
    string suf;
    bool ok = false;
};

bool better_string_state(const StringState& a, const StringState& b) {
    if (!a.ok) return false;
    if (!b.ok) return true;
    if (a.pref != b.pref) return a.pref > b.pref;
    return a.suf < b.suf;
}

void relax_string_state(StringState& a, const StringState& b) {
    if (better_string_state(b, a)) a = b;
}

vector<string> best_strings_after_a_changes(vector<string> s) {
    sort(s.begin(), s.end(), [](const string& a, const string& b) {
        return a + b < b + a;
    });

    int L = 0;
    for (auto& x : s) L += (int)x.size();
    vector<array<StringState, 2>> dp(L + 1), ndp(L + 1);
    dp[0][0] = {0, "", true};

    for (const string& cur : s) {
        for (int j = 0; j <= L; ++j) ndp[j] = {StringState(), StringState()};

        int full_cost = 0;
        for (char c : cur) full_cost += (c != 'a');
        vector<int> pre_cost(cur.size() + 1, 0);
        for (int i = 0; i < (int)cur.size(); ++i) pre_cost[i + 1] = pre_cost[i] + (cur[i] != 'a');

        for (int cost = 0; cost <= L; ++cost) {
            for (int used = 0; used <= 1; ++used) {
                if (!dp[cost][used].ok) continue;

                if (cost + full_cost <= L) {
                    StringState nxt = dp[cost][used];
                    nxt.pref += (int)cur.size();
                    relax_string_state(ndp[cost + full_cost][used], nxt);
                }

                StringState keep = dp[cost][used];
                keep.suf += cur;
                relax_string_state(ndp[cost][used], keep);

                if (used == 0) {
                    for (int p = 0; p < (int)cur.size(); ++p) {
                        if (cur[p] == 'a') continue;
                        int add = pre_cost[p];
                        if (cost + add > L) continue;
                        StringState part;
                        part.ok = true;
                        part.pref = dp[cost][0].pref + p;
                        part.suf = cur.substr(p) + dp[cost][0].suf;
                        relax_string_state(ndp[cost + add][1], part);
                    }
                }
            }
        }
        dp.swap(ndp);
    }

    vector<string> answer(L + 1);
    string best;
    for (int k = 0; k <= L; ++k) {
        for (int used = 0; used <= 1; ++used) {
            if (!dp[k][used].ok) continue;
            string cur = string(dp[k][used].pref, 'a') + dp[k][used].suf;
            if (best.empty() || cur < best) best = cur;
        }
        answer[k] = best;
    }
    return answer;
}
```

典题：Round4 H《String》。如果题目只问某一个 `k`，也可以跑到 `L` 后取 `answer[k]`，实现更省心。
#### 无 border 循环串出现计数：Rounddog 三分类

> **赛时先看**
> - **题目信号**：模式串没有 border；环形串里某个长度为 `m` 的模式出现次数只可能属于三类：0 次、1 次、至少 2 次。
> - **本质**：给定环形字符串 `S` 和特殊模式 `T_k = "Rounddo" + k 个 'g'`，统计有多少个循环移位包含该模式。
> - **接法**：数出现次数 `cnt`，答案 `cnt=0 -> 0`，`cnt=1 -> n-m+1`，`cnt>=2 -> n`。
> - **复杂度判定**：朴素检查 `O(n * m)`，本题 `k <= 100` 可过；通用写法可替换为 KMP。
> - **维护的量**：`pattern`（`"Rounddo" + k 个 'g'`）；`doubled`（`S + S[0..m-2]`）；`cnt`（起点 `0..n-1` 中的出现次数，数到 2 就停）。
> - **警告**：只需要在 `S + S[0..m-2]` 中检查起点 `0..n-1`；如果出现两次及以上，所有循环移位都包含一次。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：环形串 S 的 n 个循环移位里有多少个包含模式 "Rounddo" + k 个 'g'。
string S = "Rounddog";                            // 环形串内容（0-indexed）
int k = 1;                                        // 模式串尾部 'g' 的个数
int ans = count_rounddog_rotations(S, k);         // 三分类：0 / n-m+1 / n
// 样例：S="Rounddog", k=1 -> 模式恰出现 1 次，ans = n-m+1 = 1
```

**传参要求（照这个传不会错）：**

- `S`：环形串内容，长度 `n`。
- `k`：模式串尾部 `'g'` 的数量，`k >= 0`；模式总长 `m = 7 + k`。
- 返回：`int`：出现 0 次返回 0；恰 1 次返回 `n - m + 1`；至少 2 次返回 `n`；`m > n` 时返回 0。

```cpp
int count_rounddog_rotations(const string& S, int k) {
    int n = (int)S.size();
    string pattern = string("Rounddo") + string(k, 'g');
    int m = (int)pattern.size();
    if (m > n) return 0;

    string doubled = S + S.substr(0, m - 1);
    int cnt = 0;
    for (int start = 0; start < n; ++start) {
        bool ok = true;
        for (int j = 0; j < m; ++j) {
            if (doubled[start + j] != pattern[j]) {
                ok = false;
                break;
            }
        }
        if (ok && ++cnt >= 2) break;
    }

    if (cnt == 0) return 0;
    if (cnt == 1) return n - m + 1;
    return n;
}
```

典题：Round4 I《Rounddog》。若换成一般模式串，先用 KMP 判断 border；有 border 时不能直接套三分类公式。
#### 栈式最短路访问：Walk 区间 DP

> **赛时先看**
> - **题目信号**：操作过程像 DFS 栈；最少操作数意味着相邻特殊点之间必须走最短路；还要优化最大栈深。
> - **本质**：用栈在无向图上移动：可压入栈顶邻点，也可弹出栈顶并访问新的栈顶。要求按序访问特殊点，在总操作数最少的前提下最小化最大栈大小。
> - **接法**：`f[u][l][r]` 表示以 `u` 为当前不可弹出的栈顶，访问 `x_l..x_r` 后回到 `u` 的最小最大相对栈深；`g[u][l][r]` 表示最后不要求回到 `u`。
> - **复杂度判定**：全源 BFS `O(nm)`；DP 约 `O(mL^2+nL^3)`，适合 `n<=200,L<=60`。
> - **维护的量**：`dis[s][t]`（全源最短路）；`f[u][l][r]`（以 u 为栈顶、访问完 `x_l..x_r` 并回到 u 的最小最大相对栈深）；`g[u][l][r]`（最后不要求回到 u 的版本）。
> - **警告**：先合并连续相同特殊点；`on(a,b;u,v)` 表示边 `u->v` 能在某条 `a` 到 `b` 的最短路上。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：无向图上用栈按序访问特殊点序列，最少操作数前提下最小化最大栈大小。
int n = 3;                                        // 顶点数，编号 1..n
vector<vector<int>> adj = {{}, {2}, {1, 3}, {2}}; // 1-indexed 邻接表，adj[0] 不用
vector<int> x = {1, 2, 3};                        // 需依次访问的特殊点
int ans = minimum_walk_stack_depth(n, adj, x);
// 样例：链 1-2-3 依次访问 1,2,3 -> ans = 3
```

**传参要求（照这个传不会错）：**

- `n`：点数，顶点编号 `1..n`。
- `adj`：`n+1` 个 vector 的邻接表（`adj[0]` 不用），无向边要双向加入。
- `x`：特殊点访问顺序，元素范围 `[1,n]`，可重复（函数会先合并连续相同点）。
- 返回：`int`，最小最大栈大小；去重后只剩 1 个特殊点时返回 1。

```cpp
int minimum_walk_stack_depth(int n, const vector<vector<int>>& adj, vector<int> x) {
    const int INF = 1000000000;

    vector<int> y;
    for (int v : x) {
        if (y.empty() || y.back() != v) y.push_back(v);
    }
    x = y;
    int L = (int)x.size();
    if (L == 1) return 1;

    vector<vector<int>> dis(n + 1, vector<int>(n + 1, INF));
    for (int s = 1; s <= n; ++s) {
        queue<int> q;
        dis[s][s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (dis[s][v] == INF) {
                    dis[s][v] = dis[s][u] + 1;
                    q.push(v);
                }
            }
        }
    }

    vector<vector<vector<int>>> f(n + 1, vector<vector<int>>(L, vector<int>(L, INF)));
    vector<vector<vector<int>>> g(n + 1, vector<vector<int>>(L, vector<int>(L, INF)));
    vector<vector<int>> order(L);
    for (int l = 0; l < L; ++l) {
        order[l].resize(n);
        iota(order[l].begin(), order[l].end(), 1);
        sort(order[l].begin(), order[l].end(), [&](int a, int b) {
            return dis[a][x[l]] < dis[b][x[l]];
        });
    }

    for (int len = 1; len <= L; ++len) {
        for (int l = 0; l + len - 1 < L; ++l) {
            int r = l + len - 1;
            for (int u = 1; u <= n; ++u) {
                if (l == r && u == x[l]) f[u][l][r] = g[u][l][r] = 1;

                for (int k = l; k < r; ++k) {
                    if (dis[x[k]][x[k + 1]] == dis[x[k]][u] + dis[u][x[k + 1]]) {
                        f[u][l][r] = min(f[u][l][r], max(f[u][l][k], f[u][k + 1][r]));
                        g[u][l][r] = min(g[u][l][r], max(f[u][l][k], g[u][k + 1][r]));
                    }
                }
            }

            for (int u : order[l]) {
                for (int v : adj[u]) {
                    if (dis[u][x[l]] == dis[v][x[l]] + 1) {
                        g[u][l][r] = min(g[u][l][r], g[v][l][r] + 1);
                        if (dis[u][x[r]] == dis[v][x[r]] + 1) {
                            f[u][l][r] = min(f[u][l][r], f[v][l][r] + 1);
                        }
                    }
                }
            }
        }
    }
    return g[x[0]][0][L - 1];
}
```

典题：Round4 J《Walk》。如果题面只要求最少操作数，不要求最小栈深，就直接拼相邻特殊点最短路即可；本题多出的难点是第二优化目标。
#### 任意点分树计数：子树路径长度背包

> **赛时先看**
> - **题目信号**：不是标准“重心分治”，中心可以任意选；不同点分树按每个点父亲是否相同区分；需要数所有递归删除点的结构。
> - **本质**：一棵树每次在当前连通块任选一个点作为分治中心，统计能产生多少棵不同的点分树。
> - **接法**：`f[u][k]` 表示只考虑原树子树 `u`，点分树根到 `u` 的路径长度恰好为 `k+1` 的方案数。儿子 `v` 贡献 `t` 个路径点时，方案数取后缀和。
> - **复杂度判定**：`O(n^2)`，空间 `O(n^2)` 级别以内，适合 `n<=3000`。
> - **维护的量**：`f[u][k]`（原树子树 u 内、点分树根到 u 路径长恰好 `k+1` 的方案数）；`sz[u]`（子树大小）；`C`（组合数表）。
> - **警告**：固定原树根为 1 后，统计的是点分树根到固定点 `v` 的路径长度；合并儿子贡献时要乘组合数，表示不同子树贡献点交错出现的顺序。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：树每次任选一点当分治中心递归删点，统计能形成多少棵不同的点分树（mod 1e9+7）。
DecompositionTreeCounter dtc;                     // 构造计数结构
dtc.init(3);                                      // 点数 3，顶点 1..3
dtc.add_edge(1, 2);                               // 加无向边（原树）
dtc.add_edge(2, 3);
int ans = dtc.solve();                            // 不同点分树数量
// 样例：链 1-2-3 -> ans = 5
```

**传参要求（照这个传不会错）：**

- `init(int n_)`：`n_` 为点数，顶点编号 `1..n_`；在加边前调用。
- `add_edge(int u, int v)`：加入一条无向边，`1 <= u,v <= n`。
- `solve()`：执行主算法（固定原树根 1 做 dfs），返回 `int`：模 `1e9+7` 下的点分树计数。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_edge(int u, int v)` -> 加入一条边
- `init(int n_)` -> 初始化/清空结构
- `solve()` -> 执行主算法并返回答案
**改板时先认这几个量：**

- `g`：邻接表。
- `sz`：集合/子树大小。

```cpp
struct DecompositionTreeCounter {
    static constexpr int MOD = 1000000007;
    int n = 0;
    vector<vector<int>> g;
    vector<vector<int>> f;
    vector<int> sz;
    vector<vector<int>> C;

    void addmod(int& x, long long y) {
        x = (x + y) % MOD;
    }

    void init(int n_) {
        n = n_;
        g.assign(n + 1, {});
        f.assign(n + 1, {});
        sz.assign(n + 1, 0);
        C.assign(n + 1, vector<int>(n + 1, 0));
        for (int i = 0; i <= n; ++i) {
            C[i][0] = C[i][i] = 1;
            for (int j = 1; j < i; ++j) {
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
                if (C[i][j] >= MOD) C[i][j] -= MOD;
            }
        }
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int parent) {
        sz[u] = 1;
        vector<int> dp(1, 1);

        for (int v : g[u]) {
            if (v == parent) continue;
            dfs(v, u);

            int m = sz[v];
            vector<int> suffix(m + 1, 0);
            for (int i = m - 1; i >= 0; --i) {
                suffix[i] = suffix[i + 1] + f[v][i];
                if (suffix[i] >= MOD) suffix[i] -= MOD;
            }

            // take[t]：儿子子树在通向 u 的路径上贡献 t 个点。
            vector<int> take(m + 1, 0);
            take[0] = suffix[0];
            for (int t = 1; t <= m; ++t) take[t] = suffix[t - 1];

            vector<int> ndp(dp.size() + m, 0);
            for (int a = 0; a < (int)dp.size(); ++a) {
                if (dp[a] == 0) continue;
                for (int t = 0; t <= m; ++t) {
                    if (take[t] == 0) continue;
                    long long ways = 1LL * dp[a] * take[t] % MOD * C[a + t][a] % MOD;
                    addmod(ndp[a + t], ways);
                }
            }
            dp.swap(ndp);
            sz[u] += sz[v];
        }
        f[u] = move(dp);
    }

    int solve() {
        dfs(1, 0);
        int ans = 0;
        for (int x : f[1]) addmod(ans, x);
        return ans;
    }
};
```

典题：Round4 K《Decomposition Trees》。看到“递归任选一个点删除形成树”时，可以先固定一个原树点，统计分治树根到它的路径。
#### 区间消除长度：极值折线线段树

> **赛时先看**
> - **题目信号**：合法删除区间 `[l,r]` 要求内部所有值都在两个端点值之间；询问区间答案和最小值、最大值第一次/最后一次出现位置有关。
> - **本质**：维护序列单点修改和区间查询，查询区间经过“端点包住内部值即可删除内部”的最小剩余长度。
> - **复杂度判定**：题解做法 `O((n+q) log^2 n)`。
> - **维护的量**：线段树节点维护四条 LOW/HIGH 折线 `F(L,alpha)`、`F(L,gamma)`、`F(beta,R)`、`F(delta,R)`，外加区间极值首末位置 `firstMin/lastMax/firstMax/lastMin`。
> - **警告**：区间答案可由两种极值方向取最小：`firstMin <= lastMax` 和 `firstMax <= lastMin`；每个线段树节点不是只存答案，还要存四条折线。


- 对区间 `[L,R]`，记最小值 `mn`、最大值 `mx`。
- `alpha=firstMin`，`beta=lastMax`，`gamma=firstMax`，`delta=lastMin`。
- 若 `alpha<=beta`，候选为 `F(L,alpha)+F(beta,R)`。
- 若 `gamma<=delta`，候选为 `F(L,gamma)+F(delta,R)`。
- 线段树节点维护 `F(L,alpha)`、`F(L,gamma)`、`F(beta,R)`、`F(delta,R)` 四条 LOW/HIGH 折线。
- 合并左右儿子时，用可持久化 Treap 支持查找可替代前缀、切掉前缀、拼接序列。

典题：Round4 G《Fading Memories》。这个模板工程量很大，适合作为“知道该怎么维护”的复盘卡；真赛场复制请优先参考本场 `stds/G.cpp`。

#### 凸多边形缩放内切圆覆盖：边 Voronoi + 二分

> **赛时先看**
> - **题目信号**：圆必须完全位于凸多边形内；圆半径由到所有边的最小距离决定；缩放只会把中轴线和半径同比例放大。
> - **本质**：求最小缩放系数 `lambda`，使凸多边形 `lambda A` 内存在一个圆，覆盖给定带权点总权至少 `W`。
> - **复杂度判定**：预处理边 Voronoi 约 `O(n^2 log n)`；每次二分检查约 `O(nm log m)`。
> - **维护的量**：边 Voronoi 的中轴线段（端点 `a,b` 及端点最大内切圆半径 `ra,rb`）；二分变量 `lambda`；检查时按 `t in [0,1]` 维护带权点的覆盖事件。
> - **警告**：原点必须严格在凸多边形内部，这样每条边半平面可写成 `u·x <= h` 且 `h>0`；固定 `lambda` 时只需考虑极大内切圆的圆心，它一定在边 Voronoi / 中轴线上。


- 对每条边 `i` 建它的 Voronoi cell：同时满足在原凸多边形内，以及 `d_i(x) <= d_j(x)`。
- 对每个 cell 的边界，如果来自 `d_i=d_j`，就是一段中轴线。
- 记录中轴线段两端点 `a,b` 和端点最大内切圆半径 `ra,rb`。
- 二分 `lambda`。对每条中轴线段参数 `t in [0,1]`，圆心和半径都是一次函数。
- 对每个带权点解一个关于 `t` 的二次不等式，得到覆盖区间；扫描事件看权值和是否达到 `W`。

典题：Round4 L《Geometry》。如果凸多边形变成圆或矩形，边 Voronoi 可以大幅简化；先看特殊形状再上半平面交。

#### 赛后模型总览：2026 河南萌新联赛第（一）场

> **赛时先看**
> - **题目信号**：刚 VP / 补 2026 河南萌新联赛第（一）场；题目明显是基础模型训练，但赛场上容易因为边界和实现细节翻车。
> - **本质**：把本场 13 题映射到可复用板子，后续补题时先从这里定位模型。
> - **复杂度判定**：这是索引页，不涉及算法复杂度。
> - **警告**：A/I 更偏纯模拟或签到；真正建议长期复用的是 B/C/D/E/F/G/H/J/K/L/M 的模型。


题型归档：

- A《萌新大富翁》：复杂规则模拟，维护行动队列、玩家状态、地产归属和破产释放。
- B《缝缝补补》：二阶差分 gcd，判断“加减同一个 x 后可变等差数列”。
- C《最大公约数》：统计 `gcd(l,r) == min(l,r)` 的子数组。
- D《走迷宫》：BFS 状态分层，位置 + 是否已经破墙。
- E《完美灵丝》：恰好 K 种不同元素 + 极差不超过 D，双指针和单调队列。
- F《闯关游戏》：小根堆 Kahn，输出字典序最小拓扑序。
- G《强迫症》：区间减一到 0 的最少次数，累加正差分。
- H《神秘整数》：所有数模 k 余数相同，转化为差值 gcd 的约数。
- I《AI 训练》：整数模拟 `x = x * 9 / 10`。
- J《灵魂之塔》：删除一个元素后相邻奇偶交替。
- K《填魔法》：网格相邻不同染色，逐格 m 进制状态 DP。
- L《小明的丹药 easy》：权值 1/2 的两侧可达和最大公共值。
- M《小明的丹药 hard》：权值 2/3 的两侧可达和最大公共值，等价于小权值有界可达性。

#### 二阶差分 gcd：加减同一个 x 变等差数列

> **赛时先看**
> - **题目信号**：操作是“单点加减同一个 `x` 的任意倍”；最终要求等差数列；问最大 `x` 或判断某个 `x` 是否可行。
> - **本质**：每个位置都能加减若干次同一个正整数 `x`，要求把序列变成等差数列，求最大可行 `x`。
> - **接法**：如果固定 `x`，要求相邻差分在模 `x` 意义下全相同；移项后就是所有二阶差分都被 `x` 整除。最大 `x` 就是二阶差分绝对值的 gcd。
> - **复杂度判定**：`O(n log V)`，只做 gcd。
> - **维护的量**：`g`（所有二阶差分绝对值的 gcd）；循环变量 `d2`（当前位置的二阶差分 `a[i]-2a[i-1]+a[i-2]`）。
> - **警告**：不是对一阶差分直接求 gcd，而是对二阶差分 `a[i]-2a[i-1]+a[i-2]` 求 gcd；若 gcd 为 `0`，说明原序列已经是等差数列，本题输出 `-1`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：每个位置可加减同一个 x 的任意倍，求把序列变等差数列的最大 x。
vector<i64> a = {1, 2, 5};                        // 序列（任意长度）
i64 g = second_difference_gcd_for_arithmetic_mod(a);
cout << (g == 0 ? -1 : g) << '\n';                // g==0 说明原序列已是等差数列
// 样例：a={1,2,5} -> g=2；a={1,2,3} -> g=0，输出 -1
```

**传参要求（照这个传不会错）：**

- `a`：序列（任意长度），元素可为任意整数（函数内部取二阶差分的绝对值求 gcd）。
- 返回：`i64`，最大可行 `x`（二阶差分绝对值的 gcd）；为 `0` 表示原序列已是等差数列，按题面输出 `-1`。

```cpp
i64 second_difference_gcd_for_arithmetic_mod(const vector<i64>& a) {
    i64 g = 0;
    for (int i = 2; i < (int)a.size(); ++i) {
        // 二阶差分为 0 表示局部已经完全符合等差趋势。
        i64 d2 = a[i] - 2LL * a[i - 1] + a[i - 2];
        g = gcd(g, llabs(d2));
    }
    return g; // g == 0 时，整个序列已经是等差数列。
}

// 用法：
// 示例：i64 g = second_difference_gcd_for_arithmetic_mod(a);
// 示例：cout << (g == 0 ? -1 : g) << '\n';
```

典题：2026 河南萌新联赛第（一）场 B《缝缝补补》。
#### 区间 gcd 等于区间最小值计数

> **赛时先看**
> - **题目信号**：题面同时定义区间 gcd 和区间最小值，并问二者相等；数组元素为正数；`n` 到 `5e5/1e6`，不能枚举所有区间。
> - **本质**：统计正整数数组中满足 `gcd(a[l..r]) == min(a[l..r])` 的连续子数组数量。
> - **接法**：对每个位置 `i` 作为最左最小值，先用单调栈求它能管辖的区间范围；再用 gcd ST 表二分出向左/向右最多能扩到哪里且所有数仍被 `a[i]` 整除，贡献为左右选择数乘积。
> - **复杂度判定**：建 gcd ST 表 `O(n log n)`，每个位置二分左右边界 `O(log n)`，总 `O(n log n)`。
> - **维护的量**：`st`（gcd 稀疏表）；`prev_le/next_less`（单调栈给出的左右管辖界）；`left_good/right_good`（二分出的、gcd 仍等于 `a[i]` 的最远扩展点）。
> - **警告**：`gcd <= min` 恒成立，但相等要求区间内所有数都能被最小值整除；最小值重复时必须避免重复计数。这里把每个区间归给“最左侧的最小值位置”：左边界用前一个 `<= a[i]`，右边界用后一个 `< a[i]`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：统计正整数数组中满足 gcd(a[l..r]) == min(a[l..r]) 的连续子数组数量。
vector<int> a = {2, 4, 6};                        // 数组（0-indexed）
i64 ans = count_subarrays_gcd_equal_min(a);
// 样例：a={2,4,6} -> 满足的子数组有 [2],[4],[6],[2,4],[2,4,6] 共 5 个，ans = 5
```

**传参要求（照这个传不会错）：**

- `a`：正整数数组（0-indexed，可为空）；内部 GcdSparseTable 同样按 0-indexed 建表与查询。
- 返回：`i64`，满足 `gcd(a[l..r]) == min(a[l..r])` 的子数组总数（可能超出 int 范围，用 i64）。

**API / 入口函数（赛时只认这里列的名字）：**

- `build(const vector<int>& a)` -> 完成建树或预处理
- `query(int l, int r)` -> 查询 返回 `int`。

```cpp
struct GcdSparseTable {
    int n = 0;
    vector<int> lg;
    vector<vector<int>> st;

    GcdSparseTable() = default;
    explicit GcdSparseTable(const vector<int>& a) { build(a); }

    void build(const vector<int>& a) {
        n = (int)a.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
        st.assign(lg[n] + 1, vector<int>(n));
        st[0] = a;
        for (int k = 1; k < (int)st.size(); ++k) {
            int len = 1 << k;
            for (int i = 0; i + len <= n; ++i) {
                st[k][i] = gcd(st[k - 1][i], st[k - 1][i + (len >> 1)]);
            }
        }
    }

    int query(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        int k = lg[r - l + 1];
        return gcd(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

i64 count_subarrays_gcd_equal_min(const vector<int>& a) {
    int n = (int)a.size();
    GcdSparseTable gst(a);

    vector<int> prev_le(n), next_less(n), stk;
    for (int i = 0; i < n; ++i) {
        // 栈顶保留离 i 最近的 <= a[i] 的位置，用来保证 i 是最左最小值。
        while (!stk.empty() && a[stk.back()] > a[i]) stk.pop_back();
        prev_le[i] = stk.empty() ? -1 : stk.back();
        stk.push_back(i);
    }

    stk.clear();
    for (int i = n - 1; i >= 0; --i) {
        // 右侧只遇到严格更小才停止；相等的最小值仍归给更左的位置。
        while (!stk.empty() && a[stk.back()] >= a[i]) stk.pop_back();
        next_less[i] = stk.empty() ? n : stk.back();
        stk.push_back(i);
    }

    i64 answer = 0;
    for (int i = 0; i < n; ++i) {
        int v = a[i];

        int lo = prev_le[i] + 1, hi = i, left_good = i;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            if (gst.query(mid, i) == v) {
                left_good = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }

        lo = i, hi = next_less[i] - 1;
        int right_good = i;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            if (gst.query(i, mid) == v) {
                right_good = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        answer += 1LL * (i - left_good + 1) * (right_good - i + 1);
    }
    return answer;
}
```

典题：2026 河南萌新联赛第（一）场 C《最大公约数》。
#### 分层状态 BFS：一次破墙网格最短路

> **赛时先看**
> - **题目信号**：每一步代价相同；题面说“有一次机会”改变通行规则；到达同一个格子时，是否已经用过机会会影响后续路径。
> - **本质**：网格最短路中有一次特殊通行机会，例如最多破一面墙、最多用一次钥匙、最多走一次特殊格。
> - **接法**：状态写成 `(x,y,used)`。走普通格保持 `used`；走墙且 `used==0` 时转成 `used=1`；答案取终点两层距离的较小值。
> - **复杂度判定**：状态数翻倍，`O(nm)`。
> - **维护的量**：`dist[x][y][used]`：到 `(x,y)` 且破墙状态为 `used`（0 未破 / 1 已破）的最短步数；`sx,sy` / `tx,ty`：起点/终点坐标。
> - **警告**：访问数组必须开第三维 `used`；不能用一个二维 `dist` 把“还没破墙”和“已经破墙”的状态合并。

**最小完整示例（先抄这一段就能跑）：**
网格里含 `'S'` 起点、`'T'` 终点、`'#'` 墙，最多破一面墙，求起点到终点的最短步数：

```cpp
vector<string> grid = {"S.#", "...", "#T."};
int ans = shortest_path_break_one_wall(grid);  // 入口，返回最短步数
cout << ans << '\n';                           // 样例输出：3
```

**传参要求（照这个传不会错）：**
- `grid`：`n` 行字符串（`0-indexed`），字符只含 `'S'/'T'/'#'/'.'`，保证恰好一个 `'S'` 一个 `'T'`；`'#'` 墙每格至多破一次。
- 返回值：`int`，`S` 到 `T` 的最短步数；不可达返回 `-1`。


```cpp
int shortest_path_break_one_wall(const vector<string>& grid) {
    int n = (int)grid.size();
    int m = (int)grid[0].size();
    const int INF = 0x3f3f3f3f;
    vector dist(n, vector(m, array<int, 2>{INF, INF}));
    queue<tuple<int, int, int>> q;

    int sx = -1, sy = -1, tx = -1, ty = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 'S') sx = i, sy = j;
            if (grid[i][j] == 'T') tx = i, ty = j;
        }
    }

    dist[sx][sy][0] = 0;
    q.push({sx, sy, 0});
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    while (!q.empty()) {
        auto [x, y, used] = q.front();
        q.pop();
        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir], ny = y + dy[dir];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;

            int nused = used;
            if (grid[nx][ny] == '#') {
                if (used) continue;
                nused = 1;
            }
            if (dist[nx][ny][nused] != INF) continue;
            dist[nx][ny][nused] = dist[x][y][used] + 1;
            q.push({nx, ny, nused});
        }
    }

    int answer = min(dist[tx][ty][0], dist[tx][ty][1]);
    return answer == INF ? -1 : answer;
}
```

典题：2026 河南萌新联赛第（一）场 D《走迷宫》。
#### 恰好 K 种且极差不超过 D 的子数组计数

> **赛时先看**
> - **题目信号**：右端点右移时，种类数和极差都只会更不满足；左端点右移可以修复约束；问连续区间数量。
> - **本质**：统计连续子数组，要求不同值个数恰好为 `K`，且区间最大值与最小值之差不超过 `D`。
> - **接法**：写 `count_at_most(k)` 统计“不同值不超过 `k` 且极差不超过 `D`”的区间数。窗口合法后，每个右端点贡献 `right-left+1`。
> - **复杂度判定**：哈希表期望 `O(n)`，两个单调队列总 `O(n)`。
> - **维护的量**：`freq`（窗口内各值出现次数）、`distinct`（当前不同值个数）、`maxq/minq`（窗口最大/最小值的单调队列，存下标）、`left/right`（窗口左右边界）。
> - **警告**：恰好 `K` 不要硬维护一个窗口，改成 `atMost(K) - atMost(K-1)`；收缩左端点时，频次数组和两个单调队列都要同步过期。

**最小完整示例（先抄这一段就能跑）：**
统计一个数组中“不同值个数恰好为 `K` 且最大值减最小值不超过 `D`”的连续子数组个数：

```cpp
vector<i64> a = {1, 2, 3, 4};
i64 ans = count_subarrays_exactly_k_distinct_and_range(a, 2, 3);  // K=2, D=3
cout << ans << '\n';   // 样例输出：3
```

**传参要求（照这个传不会错）：**
- `a`：原数组，`0-indexed`，可空。
- `k`：要求的恰好种类数；`k <= 0` 时返回 `0`（内部用 atMost 相减实现）。
- `max_diff`：极差上界；`max_diff < 0` 时返回 `0`。
- 返回值：`i64`，满足条件的子数组个数。


```cpp
i64 count_subarrays_at_most_k_distinct_and_range(
    const vector<i64>& a, int k, i64 max_diff
) {
    if (k < 0 || max_diff < 0) return 0;
    unordered_map<i64, int> freq;
    freq.reserve(a.size() * 2 + 1);
    freq.max_load_factor(0.7F);

    deque<int> maxq, minq;
    int left = 0, distinct = 0;
    i64 answer = 0;

    for (int right = 0; right < (int)a.size(); ++right) {
        if (++freq[a[right]] == 1) ++distinct;

        while (!maxq.empty() && a[maxq.back()] <= a[right]) maxq.pop_back();
        while (!minq.empty() && a[minq.back()] >= a[right]) minq.pop_back();
        maxq.push_back(right);
        minq.push_back(right);

        while (distinct > k || a[maxq.front()] - a[minq.front()] > max_diff) {
            if (--freq[a[left]] == 0) {
                freq.erase(a[left]);
                --distinct;
            }
            if (maxq.front() == left) maxq.pop_front();
            if (minq.front() == left) minq.pop_front();
            ++left;
        }

        answer += right - left + 1;
    }
    return answer;
}

i64 count_subarrays_exactly_k_distinct_and_range(
    const vector<i64>& a, int k, i64 max_diff
) {
    return count_subarrays_at_most_k_distinct_and_range(a, k, max_diff)
         - count_subarrays_at_most_k_distinct_and_range(a, k - 1, max_diff);
}
```

典题：2026 河南萌新联赛第（一）场 E《完美灵丝》。

#### 字典序最小拓扑排序

> **赛时先看**
> - **题目信号**：边 `u -> v` 表示 `u` 必须在 `v` 前；题面明确要求“编号尽量小”“字典序最小”。
> - **本质**：有向依赖图中输出一组合法顺序，并要求字典序最小；若有环则无解。
> - **接法**：Kahn 算法的队列换成 `priority_queue<int, vector<int>, greater<int>>`。最终序列长度小于 `n` 说明有环。
> - **复杂度判定**：`O((n+m) log n)`。
> - **维护的量**：`indeg[v]`（`v` 当前剩余入度）、小根堆 `pq`（当前入度为 0 的候选点）、`order`（已确定的输出顺序）。
> - **警告**：普通队列只能得到任意拓扑序；要字典序最小必须把所有当前入度为 0 的点放进小根堆。

**最小完整示例（先抄这一段就能跑）：**
`n` 个点 `1-indexed`，边 `u -> v` 表示 `u` 必须先于 `v`，输出字典序最小的拓扑序：

```cpp
int n = 4;
vector<vector<int>> g = {{}, {2, 3}, {4}, {4}, {}};  // 1->2, 1->3, 2->4, 3->4
vector<int> order = lexicographically_smallest_topo(n, g);
for (int x : order) cout << x << ' ';   // 样例输出：1 2 3 4
```

**传参要求（照这个传不会错）：**
- `n`：点数，编号从 `1` 到 `n`。
- `g`：邻接表，`g[u]` 存 `u` 的所有出边 `v`，顶点编号 `1..n`。
- 返回值：`vector<int>` 字典序最小拓扑序；若 `order.size() < n` 说明图有环。


**改板时先认这几个量：**

- `g`：邻接表。
- `indeg`：入度。

```cpp
vector<int> lexicographically_smallest_topo(int n, const vector<vector<int>>& g) {
    vector<int> indeg(n + 1), order;
    for (int u = 1; u <= n; ++u) {
        for (int v : g[u]) ++indeg[v];
    }

    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 1; i <= n; ++i) {
        if (indeg[i] == 0) pq.push(i);
    }

    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        order.push_back(u);
        for (int v : g[u]) {
            if (--indeg[v] == 0) pq.push(v);
        }
    }
    return order; // order.size() < n 表示有环。
}
```

典题：2026 河南萌新联赛第（一）场 F《闯关游戏》。
#### 区间减一到 0 的最少操作次数

> **赛时先看**
> - **题目信号**：操作是“选一段连续区间，整体加/减 1”；所有数非负；目标全变成 0。
> - **本质**：每次把一个连续区间内所有正数同时减少 `1`，求把整个非负数组变成 `0` 的最少操作数。
> - **接法**：把每个高度看成很多层。若 `a[i] > a[i-1]`，多出来的层必须从位置 `i` 新开操作；否则前面开的操作可以延续过来。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`previous`（上一个数的高度）、`answer`（已累计的“新开操作”层数，即所有正上升量之和）。
> - **警告**：答案可能超过 `int`；只累加正的上升量，不是累加所有差分绝对值。

**最小完整示例（先抄这一段就能跑）：**
非负数组，每次选一个连续区间整体减 `1`，求把整个数组变全 `0` 的最少操作次数：

```cpp
vector<i64> a = {2, 1, 3, 2};
i64 ans = min_operations_decrement_intervals_to_zero(a);
cout << ans << '\n';   // 样例输出：4
```

**传参要求（照这个传不会错）：**
- `a`：非负整数数组，`0-indexed`，可空。
- 返回值：`i64`，最少操作次数（可能超过 `int`）。


```cpp
i64 min_operations_decrement_intervals_to_zero(const vector<i64>& a) {
    i64 answer = 0;
    i64 previous = 0;
    for (i64 current : a) {
        if (current > previous) answer += current - previous;
        previous = current;
    }
    return answer;
}
```

典题：2026 河南萌新联赛第（一）场 G《强迫症》。
#### 所有数模 k 余数相同：枚举差值 gcd 的约数

> **赛时先看**
> - **题目信号**：题面出现“所有数除以 `k` 的余数相同”；要求输出所有合法 `k`。
> - **本质**：给一组整数，找所有 `k > 1`，使得它们对 `k` 取模后的余数相同。
> - **接法**：求 `g = gcd(|a_i - a_0|)`，所有 `g` 的大于 `1` 的约数就是答案。
> - **复杂度判定**：求 gcd `O(n log V)`，枚举约数 `O(sqrt(g))`。
> - **维护的量**：`g`（所有数与 `a[0]` 差的绝对值 gcd）、`answer`（`g` 的大于 `1` 的约数，升序去重）。
> - **警告**：条件不是 `k` 整除所有数，而是 `k` 整除所有差值；若差值 gcd 为 `1` 则无解。若所有数都相同，合法 `k` 无限多，需要按题面另行特判。

**最小完整示例（先抄这一段就能跑）：**
给一组整数，输出所有 `k > 1`，使这些数对 `k` 取模的余数全部相同：

```cpp
vector<i64> a = {2, 5, 8};
vector<i64> ks = same_remainder_moduli(a);
for (i64 k : ks) cout << k << ' ';   // 样例输出：3
```

**传参要求（照这个传不会错）：**
- `a`：整数数组（可为负，内部取绝对值），`0-indexed`，非空；传的是拷贝，不改原数组。
- 返回值：`vector<i64>`，升序去重后的所有合法 `k`；差值 gcd `<= 1` 时返回空。


```cpp
vector<i64> same_remainder_moduli(vector<i64> a) {
    i64 g = 0;
    for (int i = 1; i < (int)a.size(); ++i) {
        g = gcd(g, llabs(a[i] - a[0]));
    }
    if (g <= 1) return {};

    vector<i64> answer;
    for (i64 d = 2; d * d <= g; ++d) {
        if (g % d != 0) continue;
        answer.push_back(d);
        if (d * d != g) answer.push_back(g / d);
    }
    answer.push_back(g);
    sort(answer.begin(), answer.end());
    answer.erase(unique(answer.begin(), answer.end()), answer.end());
    return answer;
}
```

典题：2026 河南萌新联赛第（一）场 H《神秘整数》。
#### 删除一个元素后相邻奇偶交替

> **赛时先看**
> - **题目信号**：要求恰好删除一个元素；剩余序列有相邻条件；条件能用前缀合法、后缀合法和中间桥接判断。
> - **本质**：统计删掉一个位置后，剩余序列是否满足相邻奇偶性全部不同。
> - **接法**：`pre[i]` 维护 `0..i` 是否交替，`suf[i]` 维护 `i..n-1` 是否交替。枚举删除位置 `i`，只需检查左段、右段和 `i-1` 与 `i+1` 是否能接上。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`p[i]`（`a[i]` 的奇偶性 0/1）、`pre[i]`（前缀 `0..i` 是否交替）、`suf[i]`（后缀 `i..n-1` 是否交替）。
> - **警告**：删除首尾时没有桥接边；长度 `0/1` 的剩余序列应视为合法。

**最小完整示例（先抄这一段就能跑）：**
恰好删除一个元素，使剩余序列相邻元素奇偶性全部不同，统计可删的位置个数：

```cpp
vector<i64> a = {1, 2, 3, 4};
int ans = count_deletions_make_parity_alternating(a);
cout << ans << '\n';   // 样例输出：2
```

**传参要求（照这个传不会错）：**
- `a`：整数数组（可为负，奇偶性按 `|a[i]|` 判），`0-indexed`。
- 返回值：`int`，可删除的位置个数；`n == 1` 时返回 `1`。


```cpp
int count_deletions_make_parity_alternating(const vector<i64>& a) {
    int n = (int)a.size();
    if (n == 1) return 1;

    vector<int> p(n);
    for (int i = 0; i < n; ++i) p[i] = (int)(llabs(a[i]) & 1);

    vector<char> pre(n, true), suf(n, true);
    for (int i = 1; i < n; ++i) {
        pre[i] = pre[i - 1] && (p[i] != p[i - 1]);
    }
    for (int i = n - 2; i >= 0; --i) {
        suf[i] = suf[i + 1] && (p[i] != p[i + 1]);
    }

    int answer = 0;
    for (int i = 0; i < n; ++i) {
        bool left_ok = (i == 0) || pre[i - 1];
        bool right_ok = (i == n - 1) || suf[i + 1];
        bool bridge_ok = (i == 0 || i == n - 1) || (p[i - 1] != p[i + 1]);
        if (left_ok && right_ok && bridge_ok) ++answer;
    }
    return answer;
}
```

典题：2026 河南萌新联赛第（一）场 J《灵魂之塔》。
#### 网格相邻不同染色：逐格 m 进制状态 DP

> **赛时先看**
> - **题目信号**：列数很小 `q <= 8`，颜色数小 `m <= 4`，行数可到 `100`；每个格子的合法性只依赖左边和上边。
> - **本质**：统计 `p*q` 网格染色方案，要求上下左右相邻格颜色不同，且首行、末行可能被固定。
> - **接法**：先把固定首行编码成初始状态；从第 2 行第 1 列开始逐格填。每填一个格子，就把状态中对应列替换成新颜色。最后读取固定末行状态的方案数。
> - **复杂度判定**：`O(p * q * m * m^q)`，空间 `O(m^q)`。
> - **维护的量**：`power[col]`（`m^col` 位权）、`total_states`（状态总数 `m^q`）、`dp[state]`（最近一行压缩状态 `state` 的方案数）、`head/tail`（首/末行编码）。
> - **警告**：状态是最近一行的颜色，按 `m` 进制压缩；逐格转移时，当前格的上方颜色是旧状态的本列，左方颜色是已经更新过的前一列。`p == 1` 时直接判断首行是否等于末行。

**最小完整示例（先抄这一段就能跑）：**
`p*q` 网格、`m` 种颜色（编号 `0..m-1`），相邻上下左右不同色且首末行固定，统计方案数：

```cpp
GridColoringFixedRows solver(2, 2, 3);      // p=2 行, q=2 列, 3 种颜色
int ans = solver.count({0, 1}, {1, 0});     // 首行 [0,1]，末行 [1,0]
cout << ans << '\n';                        // 样例输出：1
```

**传参要求（照这个传不会错）：**
- 构造 `GridColoringFixedRows(p, q, colors)`：`p` 行数、`q` 列数（需很小，如 `<= 8`）、`colors` 颜色数（如 `<= 4`）。
- `count(first_row, last_row)`：首行/末行颜色数组，长度必须等于 `q`，且各自内部相邻颜色不能相同（否则断言失败）。
- 返回值：`int`，模 `998244353` 的方案数；`p == 1` 时首行等于末行返回 `1`，否则返回 `0`。


**改板时先认这几个量：**

- `up`：当前格上方的颜色（旧状态本列）。
- `dp`：DP 状态。

```cpp
struct GridColoringFixedRows {
    static constexpr int MOD = 998244353;
    int p = 0, q = 0, colors = 0, total_states = 0;
    vector<int> power;

    GridColoringFixedRows(int p_, int q_, int colors_)
        : p(p_), q(q_), colors(colors_), power(q_ + 1, 1) {
        for (int i = 1; i <= q; ++i) power[i] = power[i - 1] * colors;
        total_states = power[q];
    }

    int get_digit(int state, int col) const {
        return state / power[col] % colors;
    }

    int set_digit(int state, int col, int value) const {
        int old = get_digit(state, col);
        return state + (value - old) * power[col];
    }

    int encode(const vector<int>& row) const {
        int state = 0;
        for (int col = 0; col < q; ++col) state += row[col] * power[col];
        return state;
    }

    bool valid_row(const vector<int>& row) const {
        for (int col = 1; col < q; ++col) {
            if (row[col] == row[col - 1]) return false;
        }
        return true;
    }

    int count(const vector<int>& first_row, const vector<int>& last_row) const {
        assert((int)first_row.size() == q && (int)last_row.size() == q);
        assert(valid_row(first_row) && valid_row(last_row));

        int head = encode(first_row);
        int tail = encode(last_row);
        if (p == 1) return head == tail ? 1 : 0;

        vector<int> dp(total_states), ndp(total_states);
        dp[head] = 1;

        for (int row = 2; row <= p; ++row) {
            for (int col = 0; col < q; ++col) {
                fill(ndp.begin(), ndp.end(), 0);
                for (int state = 0; state < total_states; ++state) {
                    if (!dp[state]) continue;
                    int up = get_digit(state, col);
                    int left = (col == 0 ? -1 : get_digit(state, col - 1));
                    for (int color = 0; color < colors; ++color) {
                        if (color == up || color == left) continue;
                        int next_state = set_digit(state, col, color);
                        ndp[next_state] += dp[state];
                        if (ndp[next_state] >= MOD) ndp[next_state] -= MOD;
                    }
                }
                dp.swap(ndp);
            }
        }
        return dp[tail];
    }
};
```

典题：2026 河南萌新联赛第（一）场 K《填魔法》。
#### 两种小权值物品凑最大公共和

> **赛时先看**
> - **题目信号**：题面只有两种小权值，例如 `1/2` 或 `2/3`；问“两边一样多且总和最大”；数量上限不大，或权值很小可以枚举目标和。
> - **本质**：左右两侧各有两种权值的物品，每种有数量上限；要求两侧凑出相同元素和，且这个公共和最大。
> - **接法**：先写 `can_make_with_two_weights(sum, count1, weight1, count2, weight2)` 判断单侧是否可达；从两边总容量较小值往下枚举第一个两侧都可达的公共和。
> - **复杂度判定**：枚举目标和 `O(min(total_left,total_right) * log W)`，本模板里 `W` 极小，实际近似线性。
> - **维护的量**：`g`（两种权值的 gcd）、`low/high`（第一种物品用量的可行区间）、`upper`（两侧总容量较小值，枚举起点）。
> - **警告**：答案不一定是两边总容量的较小值，因为奇偶性 / 模数可达性会卡住。例如权值只有 `2` 时凑不出奇数。

**最小完整示例（先抄这一段就能跑）：**
两侧各有两种权值的物品（各给个数与权值），各取若干使两边元素和相等，求最大公共和：

```cpp
i64 ans = maximum_equal_sum_two_sides(
    3, 2, 2, 1,    // 左侧：3 个权值 2，2 个权值 1
    2, 2, 3, 1);   // 右侧：2 个权值 2，3 个权值 1
cout << ans << '\n';   // 样例输出：7
```

**传参要求（照这个传不会错）：**
- 左侧四参：`left_a`（权值 `left_weight_a` 的物品个数）、`left_weight_a`、`left_b`（权值 `left_weight_b` 的个数）、`left_weight_b`；右侧四参同理；均为非负 `i64`。
- 返回值：`i64`，两侧可达的最大公共元素和；从总容量较小值往下找第一个两侧都可凑出的值，没有则返回 `0`。
- 辅助函数 `can_make_with_two_weights(target, cnt_a, weight_a, cnt_b, weight_b)`：判断单侧能否凑出 `target`，一般不用直接调。


**改板时先认这几个量：**

- `g`：两种权值的 gcd。
- `low/high`：第一种物品使用数量的可行区间。

```cpp
i64 exgcd_for_inverse(i64 a, i64 b, i64& x, i64& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    i64 x1, y1;
    i64 g = exgcd_for_inverse(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return g;
}

i64 inverse_mod_coprime(i64 a, i64 mod) {
    i64 x, y;
    i64 g = exgcd_for_inverse(a, mod, x, y);
    assert(g == 1);
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

bool can_make_with_two_weights(i64 target, i64 cnt_a, i64 weight_a, i64 cnt_b, i64 weight_b) {
    if (target < 0) return false;
    i64 total = cnt_a * weight_a + cnt_b * weight_b;
    if (target > total) return false;

    // 设使用 x 个 weight_a，则 target - weight_a*x 必须由 weight_b 凑出。
    i64 low = max<i64>(0, (target - cnt_b * weight_b + weight_a - 1) / weight_a);
    i64 high = min(cnt_a, target / weight_a);
    if (low > high) return false;

    i64 g = gcd(weight_a, weight_b);
    if (target % g != 0) return false;

    i64 mod = weight_b / g;
    if (mod == 1) return true;

    i64 a = weight_a / g;
    i64 t = target / g;
    i64 need = t % mod * inverse_mod_coprime(a % mod, mod) % mod;

    // 找到区间 [low, high] 中第一个满足 x == need (mod) 的 x。
    i64 first = low + (need - low % mod + mod) % mod;
    return first <= high;
}

i64 maximum_equal_sum_two_sides(
    i64 left_a, i64 left_weight_a, i64 left_b, i64 left_weight_b,
    i64 right_a, i64 right_weight_a, i64 right_b, i64 right_weight_b
) {
    i64 upper = min(left_a * left_weight_a + left_b * left_weight_b,
                    right_a * right_weight_a + right_b * right_weight_b);
    for (i64 target = upper; target >= 0; --target) {
        if (can_make_with_two_weights(target, left_a, left_weight_a, left_b, left_weight_b) &&
            can_make_with_two_weights(target, right_a, right_weight_a, right_b, right_weight_b)) {
            return target;
        }
    }
    return 0;
}
```

典题：2026 河南萌新联赛第（一）场 L/M《小明的丹药》。easy 调用权值 `2/1`，hard 调用权值 `3/2`，最终输出 `2 * maximum_equal_sum_two_sides(...)`。
#### 赛前题源索引：码蹄杯 2022-2026 真题集

> **赛时先看**
> - **题目信号**：准备码蹄杯国赛，想从近年真题中抽出常见算法和题型。
> - **本质**：赛前先知道这几年题库覆盖了什么，再按模型扫模板。
> - **复杂度判定**：人工扫题用；不是代码模板。
> - **警告**：官方题单能确认题号、标题、难度和提交统计；若没有公开题解或完整题面，不要把标题猜测当原题结论。


出处：码蹄集官方真题集抓取于 2026-07-29，官方题库接口为 `/exam-back/pc/queryMatiBeiQuestionBank.do` 与 `/exam-back/pc/queryOjProblemByTreeId.do`。

| 年份 | 官方题库 ID | 题量 | 赛前优先扫的关键词 |
| --- | --- | ---: | --- |
| 2026 | `C98C14523F069FECB0DEED64F00CEAB0` | 63 | 图论判定、树上计数、构造、博弈、字符串/序列 DP |
| 2025 | `305EE97B0D5E361DE6A28CD18C929AF0` | 90 | 括号序列、LCM、lowbit、二分图染色、回文、树上加和、期望、路径 |
| 2024 | `C2CBD34082148550EF198C50D10DBDC7` | 73 | 二分答案、迷宫、区间数据处理、字符串、图论、异或、构造 |
| 2023 | `16A92C42378232DEB56179D9C70DC45C` | 54 | 最短路、组合/计数、密码学、循环、图搜索、博弈 |
| 2022 | `C448715ED43BEA9D2D47CED523050945` | 80 | 项链/循环串、马走日、迷宫、水渠规划、铺砖、剪刀石头布 |

赛前扫题顺序：

1. 先扫本章下面这些短模型，因为它们和官方题单标题或公开题面强相关。
2. 再翻主目录中的对应大章：二分图染色在图论，括号和回文在字符串/DP，区间维护在数据结构，低位运算在位运算。
3. 如果现场读题发现只是换皮，不要急着改模板核心；先改输入转换、建图条件、状态定义和输出。

#### 最大平均子段：长度至少 L 的二分答案

> **赛时先看**
> - **题目信号**：题面问“平均值最大”“胜率/密度/单位收益最大”，同时对子段长度有下界；答案是实数或需要扩大若干倍后输出整数。
> - **本质**：求一个连续子段的最大平均值，且子段长度必须至少为 `L`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n log 精度）`，常用 60 次二分。
> - **维护的量**：`pre[i]`（`a[0..i-1] - avg` 的前缀和）、`best_left_prefix`（允许的最左前缀最小值）、二分边界 `lo/hi`。
> - **警告**：检查函数不是直接看原数组和，而是把每个数减去猜测平均值 `mid`，判断是否存在长度至少 `L` 的子段使变换后和非负；`pre_min` 只能取当前位置左侧 `L` 个位置及更早的最小前缀。

**最小完整示例（先抄这一段就能跑）：**
求长度至少为 `L` 的连续子段的最大平均值（实数，60 次二分收敛）：

```cpp
vector<i64> a = {1, 2, 3, 4};
double ans = max_average_subarray_at_least_len(a, 2);  // 长度至少 2
cout << fixed << setprecision(6) << ans << '\n';       // 样例输出：3.500000
```

**传参要求（照这个传不会错）：**
- `a`：非空整数数组，`0-indexed`。
- `min_len`：子段长度下界，要求 `1 <= min_len <= n`（不满足会断言）。
- `iterations`：二分迭代次数，默认 `60`。
- 返回值：`double` 最大平均值；要求平均值乘 `1000` 向下取整输出时用 `(long long)(ans * 1000 + 1e-8)`。


出处：码蹄杯 2024 官方题单 `MC0304 拔河`；公开题解将其归为二分最大平均值模型。

```cpp
bool exists_average_at_least(const vector<i64>& a, int min_len, double avg) {
    int n = (int)a.size();
    vector<double> pre(n + 1, 0.0);

    // 把“平均值 >= avg”改写成“存在一段 b[i]=a[i]-avg 的和 >= 0”。
    for (int i = 1; i <= n; ++i) {
        pre[i] = pre[i - 1] + (double)a[i - 1] - avg;
    }

    double best_left_prefix = 0.0;
    for (int r = min_len; r <= n; ++r) {
        // 子段至少 min_len，所以左端点最多到 r-min_len+1。
        // 前缀下标取到 r-min_len，表示子段 [l,r] 的 l-1。
        best_left_prefix = min(best_left_prefix, pre[r - min_len]);
        if (pre[r] - best_left_prefix >= -1e-12) return true;
    }
    return false;
}

double max_average_subarray_at_least_len(const vector<i64>& a, int min_len, int iterations = 60) {
    assert(!a.empty());
    assert(1 <= min_len && min_len <= (int)a.size());

    double lo = (double)*min_element(a.begin(), a.end());
    double hi = (double)*max_element(a.begin(), a.end());

    for (int it = 0; it < iterations; ++it) {
        double mid = (lo + hi) / 2.0;
        if (exists_average_at_least(a, min_len, mid)) lo = mid;
        else hi = mid;
    }
    return lo;
}
```

怎么套题：如果题目要求输出平均值乘 `1000` 后向下取整，就令 `ans = max_average_subarray_at_least_len(a, L)`，最后输出 `(long long)(ans * 1000 + 1e-8)`。如果长度是“恰好 `L`”，不用二分，直接滑动窗口；如果长度是“不超过 `L`”，这个模板不能直接套，要换成前缀最大/最小的窗口维护。
#### 差分统计区间覆盖次数：重排后最大询问和

> **赛时先看**
> - **题目信号**：题面给若干 `[l,r]`，每个位置被问到的次数不同；原数组元素可以重新排列；目标是总贡献最大或最小。
> - **本质**：有很多区间求和询问，但允许重排原数组，要求所有询问和总和最大。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O((n+q) + n log n)`。
> - **维护的量**：`diff[i]`（覆盖次数差分：`diff[l]+=1`、`diff[r+1]-=1`）、`cnt[i]`（位置 `i` 被询问次数）、`ans`（`i128` 加权和）。
> - **警告**：区间是 1-indexed 时差分要开 `n+2`；最大值把大数放到高次数位置，最小值把大数放到低次数位置；乘法可能超过 `i64` 时用 `i128`。
> - **约定**：a 使用 1-indexed：a[1..n] 是原数组，a[0] 不用

**最小完整示例（先抄这一段就能跑）：**
数组可任意重排，`q` 个区间询问 `[l,r]`，重排后所有询问区间和的总和最大：

```cpp
vector<i64> a = {0, 1, 2, 3};                 // 1-indexed：a[1..3] 为 1,2,3
vector<pair<int, int>> q = {{1, 3}, {1, 3}};  // 两次全区间询问
i128 ans = max_total_query_sum_after_rearrange(a, q);
cout << to_string_i128(ans) << '\n';          // 样例输出：12
```

**传参要求（照这个传不会错）：**
- `a`：`1-indexed`，`a[0]` 不用，`a[1..n]` 是原数组；函数内部会排序，请传拷贝。
- `queries`：`{l, r}` 列表，要求 `1 <= l <= r <= n`（不满足会断言）。
- 返回值：`i128` 最大总和；输出必须用 `to_string_i128`。


出处：码蹄杯 2024 官方题单 `MC0351 区间询问和`；公开题解模型为差分统计覆盖次数 + 排序重排。

```cpp
string to_string_i128(i128 x) {
    if (x == 0) return "0";
    bool neg = x < 0;
    if (neg) x = -x;
    string s;
    while (x > 0) {
        s.push_back(char('0' + x % 10));
        x /= 10;
    }
    if (neg) s.push_back('-');
    reverse(s.begin(), s.end());
    return s;
}

i128 max_total_query_sum_after_rearrange(vector<i64> a, const vector<pair<int, int>>& queries) {
    // a 使用 1-indexed：a[1..n] 是原数组，a[0] 不用。
    int n = (int)a.size() - 1;
    vector<i64> diff(n + 2, 0), cnt(n + 1, 0);

    for (auto [l, r] : queries) {
        // 每个询问 [l,r] 会让区间内每个位置的贡献次数 +1。
        assert(1 <= l && l <= r && r <= n);
        diff[l] += 1;
        diff[r + 1] -= 1;
    }

    for (int i = 1; i <= n; ++i) cnt[i] = cnt[i - 1] + diff[i];

    sort(a.begin() + 1, a.end());
    sort(cnt.begin() + 1, cnt.end());

    i128 ans = 0;
    for (int i = 1; i <= n; ++i) {
        ans += (i128)a[i] * cnt[i];
    }
    return ans;
}
```

怎么套题：如果题目说“数组不能重排，只问所有区间总和”，就只保留差分统计次数，再算 `sum a[i]*cnt[i]`，不要排序。如果要求总和最小，把 `a` 升序、`cnt` 降序配对即可。
#### 二分图染色：距离冲突两组分配

> **赛时先看**
> - **题目信号**：题面出现“所有点必须分完”“属于同一方的两点必须满足距离/关系限制”“只有两方可选”；冲突关系是无向的。
> - **本质**：把点分给两个人/两组，要求同组内任意两点不能冲突。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：朴素建边 `O(n^2)`，染色 `O(n+m)`；若 `n` 很大再考虑网格哈希或扫描线优化建边。
> - **维护的量**：`color`（每个点的分组，`-1` 未分组、`0/1` 分别两方）；`g`（冲突边邻接表，1-based）；`dist2`（距离平方，与 `limit = d*d` 比较决定是否连边）。
> - **警告**：如果同组要求距离 `> d`，冲突边条件是距离 `<= d`；坐标到 `1e9` 时平方必须用 `i128`；图不连通也要逐块染色。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个哨岗坐标，分给两方，同组欧氏距离必须 `> d`，问能否分完（`MC0421 分配哨岗`）：

```cpp
vector<MatijiPoint> pts;
for (int i = 0; i < n; ++i) { i64 x, y; cin >> x >> y; pts.push_back({x, y}); }
vector<int> color;
bool ok = split_points_into_two_groups_by_distance(pts, d, color);  // 1. 调用
cout << (ok ? "YES" : "NO") << '\n';   // 2. true 输出 YES，false 输出 NO
```

**传参要求（照这个传不会错）：**

- `split_points_into_two_groups_by_distance(points, d, color)`：`points` 是坐标列表（0-based，`points[i]` 对应输入第 `i+1` 个点）；`d` 是“同组距离必须大于”的阈值（`i64`）；`color` 传出分组结果（1-based 下标，`-1` 未分组、`0/1` 两方）；返回 `bool`：能分成两组返回 `true` 且 `color` 有效，否则返回 `false`。
- `color_conflict_graph(n, g, color)`：给现成的冲突图染色；`n` 是点数、`g` 是 1-based 邻接表；返回 `bool` 同上。非距离类冲突题（同色不能同姓/同班/同边相邻）只自己建 `g` 再调它，染色函数不改。


**改板时先认这几个量：**

- `g`：邻接表。
- `dist`：距离。

出处：码蹄杯 2025 官方题单 `MC0421 分配哨岗`；题面为两方分配哨岗且同组欧氏距离必须大于 `d`。

```cpp
struct MatijiPoint {
    i64 x, y;
};

bool color_conflict_graph(int n, const vector<vector<int>>& g, vector<int>& color) {
    // color[u] = -1 表示未分组；0/1 分别表示两方。
    color.assign(n + 1, -1);

    for (int start = 1; start <= n; ++start) {
        if (color[start] != -1) continue;

        queue<int> que;
        color[start] = 0;
        que.push(start);

        while (!que.empty()) {
            int u = que.front();
            que.pop();

            for (int v : g[u]) {
                if (color[v] == -1) {
                    color[v] = color[u] ^ 1;
                    que.push(v);
                } else if (color[v] == color[u]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool split_points_into_two_groups_by_distance(const vector<MatijiPoint>& points, i64 d, vector<int>& color) {
    int n = (int)points.size();
    vector<vector<int>> g(n + 1);
    i128 limit = (i128)d * d;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            i128 dx = (i128)points[i].x - points[j].x;
            i128 dy = (i128)points[i].y - points[j].y;
            i128 dist2 = dx * dx + dy * dy;

            // 同组要求“距离必须大于 d”。
            // 所以 dist <= d 的两点不能同组，必须连冲突边。
            if (dist2 <= limit) {
                int u = i + 1;
                int v = j + 1;
                g[u].push_back(v);
                g[v].push_back(u);
            }
        }
    }
    return color_conflict_graph(n, g, color);
}
```

怎么套题：`points[i]` 按输入顺序存第 `i+1` 个哨岗；函数返回 `true` 就输出 `YES`，返回 `false` 就输出 `NO`。如果题目不是距离，而是“同色不能同姓/同班/同边相邻”，只改建 `g` 的条件，染色函数不改。
#### 最小公倍数安全累积：上界剪枝

> **赛时先看**
> - **题目信号**：题面出现周期共同对齐、若干事件同时发生、`lcm` 可能很大、答案超过 `1e18` 或超过某个 `limit` 就不需要继续。
> - **本质**：多个数求 LCM，同时判断是否已经超过题目上界，避免 `i64` 溢出。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n log V)`。
> - **维护的量**：`out`（当前累积 LCM）、`limit`（上界）、`g`（`gcd(a,b)`）、`reduced`（`a/g`，先除再乘防溢出）。
> - **警告**：LCM 必须先除 `gcd` 再乘；`a/g > limit/b` 时已经超过上界，不要真的乘；若数组里可能有 `0`，要按题意单独定义。

**最小完整示例（先抄这一段就能跑）：**
给一组数求 LCM，一旦超过上界 `limit` 立即返回 `false`，避免 `i64` 溢出：

```cpp
vector<i64> v = {6, 10, 15};
i64 ans;
bool ok = lcm_all_limited(v, (i64)1e18, ans);
cout << ok << ' ' << ans << '\n';   // 样例输出：1 30
```

**传参要求（照这个传不会错）：**
- `lcm_all_limited(values, limit, out)`：`values` 任意个整数（含 `0`，此时 `out=0` 且返回 `true`）、`limit >= 0`；成功返回 `true` 且 `out` 为累积 LCM，超限返回 `false` 且 `out` 无效。
- `lcm_limited(a, b, limit, out)`：单步合并两个数；`a/b` 内部取绝对值，含 `0` 时 `out=0` 返回 `true`。


出处：码蹄杯 2025 官方题单 `MC0406 最小公倍数`；该模型也是周期题常用安全工具。

```cpp
bool lcm_limited(i64 a, i64 b, i64 limit, i64& out) {
    // 返回 true：lcm(a,b) 没有超过 limit，并写入 out。
    // 返回 false：lcm(a,b) 已经超过 limit，out 的值不要再用。
    assert(limit >= 0);
    a = llabs(a);
    b = llabs(b);
    if (a == 0 || b == 0) {
        out = 0;
        return true;
    }

    i64 g = std::gcd(a, b);
    i64 reduced = a / g;
    if (reduced > limit / b) return false;

    out = reduced * b;
    return out <= limit;
}

bool lcm_all_limited(const vector<i64>& values, i64 limit, i64& out) {
    out = 1;
    for (i64 x : values) {
        i64 next_lcm = 0;
        if (!lcm_limited(out, x, limit, next_lcm)) return false;
        out = next_lcm;
    }
    return true;
}
```

怎么套题：如果题目只关心 LCM 是否超过 `1e18`，调用 `lcm_all_limited(a, (i64)4e18, ans)`；如果题目给了目标周期 `M`，把 `limit` 设成 `M`，一旦返回 `false` 就说明后续不会再等于 `M` 以内的答案。
#### lowbit 拆分与最低位博弈速查

> **赛时先看**
> - **题目信号**：题面直接出现 `lowbit`、`x & -x`、二进制最低位、每次加/减最低位、按能被 `2^k` 整除分类。
> - **本质**：围绕最低位的 `1` 做分层统计、拆数、模拟，或处理每步只能按 `lowbit(x)` 改变数值的小游戏。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：单次最低位查询 `O(1)`；把一个数按 lowbit 不断拆到 0 的复杂度是 `O(popcount(x))`。
> - **维护的量**：`parts`（按 lowbit 拆出的 2 的幂列表）、`lb`（当前最低位 `x & -x`）；博弈只依赖 `popcount(x)` 的奇偶。
> - **警告**：`lowbit(0)=0`，不能再拿去做除数；有符号负数的位运算容易出坑，模板统一用 `ui64`；“每步减 lowbit”这个博弈只有在题面明确如此时才可用。

**最小完整示例（先抄这一段就能跑）：**
lowbit 速查：取最低位、最低位指数、按 lowbit 拆成 2 的幂、每步只能减 lowbit 的博弈胜负：

```cpp
ui64 x = 13;                        // 13 = 0b1101
cout << lowbit_u64(x) << '\n';      // 样例输出：1
cout << lowbit_exponent(x) << '\n'; // 样例输出：0
vector<ui64> parts = split_by_lowbit(x);              // [1, 4, 8]
cout << first_win_subtract_lowbit_game(x) << '\n';    // 样例输出：1（3 个二进制 1，先手赢）
```

**传参要求（照这个传不会错）：**
- `lowbit_u64(x)`：返回最低位的 2 的幂；`x=0` 时返回 `0`（不能当除数）。
- `lowbit_exponent(x)`：返回最低位指数 `e`（`0..63`）；`x=0` 时返回 `-1`。
- `split_by_lowbit(x)`：返回从低位到高位的 2 的幂列表；`x=0` 返回空。
- `first_win_subtract_lowbit_game(x)`：每步只能把 `x` 变成 `x-lowbit(x)`，返回先手是否必胜（`popcount` 为奇数）；仅当每步选择唯一时可用。


出处：码蹄杯 2025 官方题单 `MC0407 lowbit神功`、`MC0437 lowbit博弈`；码蹄杯 2024 官方题单 `MC0362 异或`。

```cpp
ui64 lowbit_u64(ui64 x) {
    return x & (~x + 1);
}

int lowbit_exponent(ui64 x) {
    // 返回 lowbit(x) = 2^e 中的 e。
    // x=0 没有最低位的 1，返回 -1 方便外面判掉。
    if (x == 0) return -1;
    return __builtin_ctzll(x);
}

vector<ui64> split_by_lowbit(ui64 x) {
    // 把 x 拆成若干个 2 的幂，顺序从低位到高位。
    // 例如 x=13(1101) -> [1,4,8]。
    vector<ui64> parts;
    while (x) {
        ui64 lb = lowbit_u64(x);
        parts.push_back(lb);
        x -= lb;
    }
    return parts;
}

bool first_win_subtract_lowbit_game(ui64 x) {
    // 仅适用于规则：每一步必须把 x 改成 x-lowbit(x)，不能操作者输。
    // 每一步恰好删除一个二进制 1，所以总步数等于 popcount(x)。
    return (__builtin_popcountll(x) & 1) != 0;
}
```

怎么套题：如果题面是树状数组式 `i += lowbit(i)` 或 `i -= lowbit(i)`，翻树状数组；如果题面是按 `lowbit` 给贡献，先用 `lowbit_exponent(x)` 分到 `0..63` 层；如果是博弈，必须先确认每步选择是否唯一，唯一才可以用上面的奇偶结论。
#### 括号序列：线段树维护总和与前缀最小值

> **赛时先看**
> - **题目信号**：`(` 记为 `+1`，`)` 记为 `-1`；合法条件是总和为 `0` 且任意前缀和不小于 `0`；有单点修改、翻转或区间询问。
> - **本质**：动态修改括号串后，判断整串或某个区间是否为合法括号序列。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：建树 `O(n)`，单点修改和区间查询 `O(log n)`。
> - **维护的量**：`s`（1-indexed 括号串副本）；`tr[p]`（线段树第 p 号节点区间的 `BracketNode{sum, min_pref}`：括号和与最小前缀和）。
> - **警告**：判断子串合法时，不能直接看全局前缀；要查询区间内部的相对 `sum/min_prefix`；合并两个节点时右段最小前缀要加左段总和。
> - **约定**：str 使用普通 0-indexed 字符串；模板内部转成 1-indexed；把第 pos 个字符改成 c，pos 是 1-indexed；查询子串 [l,r] 的相对总和和相对最小前缀，l/r 是 1-indexed


**最小完整示例（先抄这一段就能跑）：**

题目：串 `(()())((`，翻转第 8 个字符后问 `[1,8]` 是否合法括号序列。

```cpp
BracketSegTree seg;
seg.build("(()())((");                 // 1. 建树，传普通 0-indexed 字符串
seg.flip(8);                           // 2. 翻转第 8 个括号（1-indexed）
cout << seg.valid_range(1, 8) << '\n'; // 3. 问 [1,8] 是否合法（1-indexed）
```

样例：输出 `1`（翻转后串变 `(()())()`，合法）。

**传参要求（照这个传不会错）：**

- `build(str)`：传 0-indexed 括号串（只含 `'('` / `')'`）；无返回值，建完树才能查。
- `flip(pos)`：把第 `pos` 个括号翻转（`'('` <-> `')'`），`pos` 是 1-indexed（`1..n`）；`set_char(pos, c)`：把第 `pos` 个字符改成 `c`，同样 1-indexed。
- `query(l, r)`：`l/r` 为 1-indexed（`1 <= l <= r <= n`），返回 `BracketNode{sum, min_pref}`（区间相对总和、相对最小前缀）。
- `valid_range(l, r)`：问子串 `[l,r]` 是否合法括号序列，返回 `bool`；`valid_whole()`：问整串是否合法。
- 判断区间合法性用的是区间内部的"相对"最小前缀，不能拿全局前缀来比。

**API / 入口函数（赛时只认这里列的名字）：**

- `build(const string& str)` -> 完成建树或预处理
- `query(int l, int r)` -> 查询 返回 `BracketNode`。
**改板时先认这几个量：**

- `tr`：线段树节点数组。
- `sum`：区间括号和（`(` 为 +1、`)` 为 -1）。

出处：码蹄杯 2025 官方题单 `MC0402 括号序列`；括号序列也是 XCPC 常见动态维护模型。

```cpp
struct BracketNode {
    int sum = 0;        // 这一段 '(' 记 +1、')' 记 -1 后的总和。
    int min_pref = 0;   // 从这一段左端开始走，过程中出现过的最小前缀和。
};

BracketNode merge_bracket_node(const BracketNode& left, const BracketNode& right) {
    BracketNode res;
    res.sum = left.sum + right.sum;
    res.min_pref = min(left.min_pref, left.sum + right.min_pref);
    return res;
}

struct BracketSegTree {
    int n = 0;
    string s;
    vector<BracketNode> tr;

    int bracket_value(char c) {
        return c == '(' ? 1 : -1;
    }

    void build(const string& str) {
        // str 使用普通 0-indexed 字符串；模板内部转成 1-indexed。
        n = (int)str.size();
        s = " " + str;
        tr.assign(4 * n + 4, {});
        if (n == 0) return;
        build(1, 1, n);
    }

    void build(int p, int l, int r) {
        if (l == r) {
            int v = bracket_value(s[l]);
            tr[p] = {v, min(0, v)};
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid);
        build(p << 1 | 1, mid + 1, r);
        tr[p] = merge_bracket_node(tr[p << 1], tr[p << 1 | 1]);
    }

    void set_char(int pos, char c) {
        // 把第 pos 个字符改成 c，pos 是 1-indexed。
        assert(1 <= pos && pos <= n);
        s[pos] = c;
        set_char(1, 1, n, pos, c);
    }

    void flip(int pos) {
        // 把第 pos 个括号翻转：'(' <-> ')'。
        assert(1 <= pos && pos <= n);
        set_char(pos, s[pos] == '(' ? ')' : '(');
    }

    void set_char(int p, int l, int r, int pos, char c) {
        if (l == r) {
            int v = bracket_value(c);
            tr[p] = {v, min(0, v)};
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) set_char(p << 1, l, mid, pos, c);
        else set_char(p << 1 | 1, mid + 1, r, pos, c);
        tr[p] = merge_bracket_node(tr[p << 1], tr[p << 1 | 1]);
    }

    BracketNode query(int l, int r) {
        // 查询子串 [l,r] 的相对总和和相对最小前缀，l/r 是 1-indexed。
        assert(1 <= l && l <= r && r <= n);
        return query(1, 1, n, l, r);
    }

    BracketNode query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(p << 1, l, mid, ql, qr);
        if (ql > mid) return query(p << 1 | 1, mid + 1, r, ql, qr);
        return merge_bracket_node(
            query(p << 1, l, mid, ql, qr),
            query(p << 1 | 1, mid + 1, r, ql, qr)
        );
    }

    bool valid_range(int l, int r) {
        BracketNode res = query(l, r);
        return res.sum == 0 && res.min_pref >= 0;
    }

    bool valid_whole() {
        return n == 0 || (tr[1].sum == 0 && tr[1].min_pref >= 0);
    }
};
```

怎么套题：读入字符串 `s` 后 `BracketSegTree seg; seg.build(s);`。翻转第 `pos` 个括号调用 `seg.flip(pos)`；问 `[l,r]` 是否合法调用 `seg.valid_range(l,r)`。如果题目只问静态最长合法括号，翻 D 章 09「括号序列合法性与最长合法括号」的栈/DP 模板。
#### 恢复回文与回文子序列：区间 DP 可恢复方案

> **赛时先看**
> - **题目信号**：题面说可把 `?` 替换成字母、要求正反相同；或者允许删除字符，求最长仍为回文的子序列。
> - **本质**：处理两类常见回文题：把含 `?` 的字符串恢复成回文；或求最长回文子序列并恢复一个方案。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：问号恢复 `O(n)`；最长回文子序列恢复 `O(n^2)` 时间和空间。
> - **维护的量**：问号恢复无需结构；LPS 用 `dp[i][j]`（子串 `s[i..j]` 的最长回文子序列长度）与 `left/right`（恢复方案的左右两半）。
> - **警告**：回文子串必须连续，回文子序列可以跳过字符；`?` 恢复和 LPS 是两个不同模型，不要混用。


**最小完整示例（先抄这一段就能跑）：**

题目：把 `a?c?a` 恢复成回文；并求 `bbbab` 的最长回文子序列及一个方案。

```cpp
cout << restore_question_mark_palindrome("a?c?a") << '\n';  // 1. 问号恢复，成对 ? 默认填 'a'
auto [len, pal] = longest_palindromic_subsequence_with_answer("bbbab");  // 2. LPS 长度 + 方案
cout << len << ' ' << pal << '\n';  // 3. 输出
```

样例：第一行 `aacaa`；第二行 `4 bbbb`。

**传参要求（照这个传不会错）：**

- `restore_question_mark_palindrome(s, fill = 'a')`：`s` 是含 `?` 的原串（0-indexed）；`fill` 是成对 `?` 的填充字符；返回恢复后的回文串；无解返回空串 `""`。
- `longest_palindromic_subsequence_with_answer(s)`：传原串（0-indexed）；返回 `pair<int, string>`：`first` = 最长回文子序列长度，`second` = 一个具体方案；空串返回 `{0, ""}`。
- 要字典序最小的恢复结果，`fill` 用 `'a'` 即可（题面要求别的字母再改）。
- n <= 5000 时 LPS 的 O(n^2) 也扛得住；只问长度、不恢复方案的话翻主章节的一维压缩版。


出处：码蹄杯 2025 官方题单 `MC0408 恢复回文`、`MC0429 回文子序列`。

```cpp
string restore_question_mark_palindrome(string s, char fill = 'a') {
    int n = (int)s.size();
    for (int l = 0, r = n - 1; l <= r; ++l, --r) {
        if (s[l] != '?' && s[r] != '?' && s[l] != s[r]) return "";
        if (s[l] == '?' && s[r] == '?') {
            s[l] = s[r] = fill;
        } else if (s[l] == '?') {
            s[l] = s[r];
        } else if (s[r] == '?') {
            s[r] = s[l];
        }
    }
    return s;
}

pair<int, string> longest_palindromic_subsequence_with_answer(const string& s) {
    int n = (int)s.size();
    if (n == 0) return {0, ""};

    vector<vector<int>> dp(n, vector<int>(n, 0));
    for (int i = n - 1; i >= 0; --i) {
        dp[i][i] = 1;
        for (int j = i + 1; j < n; ++j) {
            if (s[i] == s[j]) dp[i][j] = dp[i + 1][j - 1] + 2;
            else dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
        }
    }

    string left, right;
    int l = 0, r = n - 1;
    while (l <= r) {
        if (l == r) {
            left.push_back(s[l]);
            break;
        }
        if (s[l] == s[r] && dp[l][r] == dp[l + 1][r - 1] + 2) {
            left.push_back(s[l]);
            right.push_back(s[r]);
            ++l;
            --r;
        } else if (dp[l + 1][r] >= dp[l][r - 1]) {
            ++l;
        } else {
            --r;
        }
    }

    reverse(right.begin(), right.end());
    return {dp[0][n - 1], left + right};
}
```

怎么套题：`restore_question_mark_palindrome` 返回空串表示无解；如果题目要求字典序最小，默认把一对 `??` 填成 `'a'` 就是常见做法。LPS 函数会返回长度和一个具体子序列；如果 `n` 到 `5000` 只要长度，翻主章节的一维压缩版。
#### 网格寻路：一次破障 BFS

> **赛时先看**
> - **题目信号**：题面是迷宫、地图、入口出口；有墙/障碍；允许一次特殊操作穿过或破坏墙；边权都是 1。
> - **本质**：在网格中从起点走到终点，允许最多破坏一次障碍，求最短步数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O(nm)`，因为每个格子只有“没用破障 / 已用破障”两个状态。
> - **维护的量**：`dist[x][y][used]`（每个格子"未破障/已破障"两种状态的最短步数，初始 INF）；`GridState{x, y, used, dist}`（BFS 队列元素）。
> - **警告**：访问状态必须包含 `used`，只用 `vis[x][y]` 会错；起点和终点坐标要统一成 0-indexed；如果特殊操作不止一次，把第二维改成 `0..K`。
> - **约定**：本节的 `shortest_path_break_one_wall_from(grid, start, target)` 需要显式传入 0-indexed 起点终点；读入地图时把 `S`/`T` 记成坐标即可。若题面允许破 `K` 次墙，把 `array<int,2>` 改成 `vector<int>(K+1, INF)`，状态里的 `used` 范围改成 `0..K`


**最小完整示例（先抄这一段就能跑）：**

题目：3x3 迷宫，`S` 在 (0,0)、`T` 在 (2,2)，中间全是墙，最多破 1 次墙，求最短步数。

```cpp
vector<string> grid = {"S.#", "###", "##T"};  // 1. 地图，'#' 是墙
int step = shortest_path_break_one_wall_from(grid, {0, 0}, {2, 2});  // 2. 0-indexed 起点终点
cout << step << '\n';                          // 3. 输出最短步数，不可达输出 -1
```

样例：输出 `4`（S -> (0,1) -> 破墙(1,1) -> (2,1) -> T）。

**传参要求（照这个传不会错）：**

- `shortest_path_break_one_wall_from(grid, start, target)`：`grid` 为 `vector<string>`（n 行 m 列，`'#'` 是墙、其余可走）；`start`/`target` 是 0-indexed 的 `{x, y}`。
- 返回最短步数 `int`；走不到返回 `-1`。
- 只允许破 1 次墙；要破 K 次，把 `array<int,2>` 改成 `vector<int>(K + 1, INF)`，`used` 范围改成 `0..K`。
- 起点终点用 0-indexed：读入时把 `S`/`T` 记成坐标再传入。

**改板时先认这几个量：**

- `dist`：每个 `(x,y,used)` 状态的最短步数。
- `cur`：当前弹出的 BFS 状态。

出处：码蹄杯 2025 官方题单 `MC0404 寻找出口`、`MC0469 寻找隐秘路径`；码蹄杯 2022 官方题单 `MC0171 迷宫`。

```cpp
struct GridState {
    int x, y, used, dist;
};

// 带显式起点/终点参数的版本；与上方从 S/T 自动识别的版本二选一即可。
int shortest_path_break_one_wall_from(const vector<string>& grid, pair<int, int> start, pair<int, int> target) {
    const int INF = 1e9;
    static const int dx[4] = {1, -1, 0, 0};
    static const int dy[4] = {0, 0, 1, -1};
    int n = (int)grid.size();
    int m = (int)grid[0].size();
    vector<vector<array<int, 2>>> dist(n, vector<array<int, 2>>(m, {INF, INF}));
    queue<GridState> que;

    auto inside = [&](int x, int y) {
        return 0 <= x && x < n && 0 <= y && y < m;
    };

    dist[start.first][start.second][0] = 0;
    que.push({start.first, start.second, 0, 0});

    while (!que.empty()) {
        GridState cur = que.front();
        que.pop();
        if (cur.dist != dist[cur.x][cur.y][cur.used]) continue;
        if (make_pair(cur.x, cur.y) == target) return cur.dist;

        for (int dir = 0; dir < 4; ++dir) {
            int nx = cur.x + dx[dir];
            int ny = cur.y + dy[dir];
            if (!inside(nx, ny)) continue;

            int next_used = cur.used;
            if (grid[nx][ny] == '#') {
                if (next_used == 1) continue;
                next_used = 1;
            }

            if (dist[nx][ny][next_used] > cur.dist + 1) {
                dist[nx][ny][next_used] = cur.dist + 1;
                que.push({nx, ny, next_used, cur.dist + 1});
            }
        }
    }
    return -1;
}
```

怎么套题：读入地图时把起点 `S` 和终点 `T` 记录成 0-indexed 坐标，并把它们当普通可走格处理。若题面允许破 `K` 次墙，把 `array<int,2>` 改成 `vector<int>(K+1, INF)`，状态里的 `used` 范围改成 `0..K`。
