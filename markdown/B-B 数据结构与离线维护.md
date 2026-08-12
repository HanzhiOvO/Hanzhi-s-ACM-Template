## B 数据结构与离线维护

### 03 基础数据结构与区间维护

从并查集、链表、树状数组、离线区间、线段树到单调结构，按比赛最常翻的维护套路排列。
#### 并查集 DSU

> **赛时先看**
> - **题目信号**：题面出现“合并”“连通”“朋友关系”“等价类”“最小生成树 Kruskal”。要动态维护“把两个集合并起来 + 判断两点是否同集合”。
> - **本质**：每个集合选一个代表元，其余点通过 parent 指针指向它；合并只改两个代表元的指向，路径压缩 + 按大小合并保证树高近似 O(log n)，所以均摊接近 O(1)。
> - **复杂度判定**：find/unite/same 均摊接近 `O(1)`；n 到 1e6 随便用。
> - **维护的量**：`parent[x]`（x 的父节点，代表元的 parent 指向自己）；`sz[x]`（x 作为根时所在集合大小）。
> - **接法**：先 `DSU dsu(n)`；每读到一条“合并/建立关系/同队伍”操作就 `dsu.unite(u,v)`；每读到“是否连通/是否同类”就 `dsu.same(u,v)`。如果题目还要求维护两点距离差、奇偶关系、敌友关系，普通 DSU 不够，要翻“带权并查集”或“关系并查集”。
> - **警告**：初始化到 `n`；路径压缩和按大小合并一起用。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点，`m` 条操作。`op=1 u v`：合并 u、v；`op=2 u v`：判断 u、v 是否连通。

```cpp
DSU dsu(n);                       // 1. 结构体定义：DSU(点数 n)，编号 1..n
while (m--) {
    int op, u, v;
    cin >> op >> u >> v;
    if (op == 1) dsu.unite(u, v);                 // 2. 调用：合并
    else cout << (dsu.same(u, v) ? "YES" : "NO") << '\n';  // 3. 调用：判连通
}
```

样例：`unite(1,2), unite(2,3)` 后 `same(1,3)` -> `YES`；`same(1,4)` -> `NO`。

**传参要求（照这个传不会错）：**

- `DSU(n)`：构造；编号范围 `0..n` 都可用（题目从 0 开始也能直接用）。
- `unite(u, v)`：合并 u、v 所在集合；无返回值（内部自动按大小合并）。
- `same(u, v)`：u、v 是否在同一集合；返回 `bool`。
- `find(x)`：x 所在集合的代表元；`size(x)`：x 所在集合大小。
- 要维护"距离差/奇偶关系/敌友"时普通 DSU 不够，翻"带权并查集""关系并查集"。


**不会用就照抄：**

```cpp
DSU dsu(n);               // 点编号 1..n
dsu.unite(u, v);           // 合并 u,v
if (dsu.same(u, v)) { }    // 是否同一集合
int s = dsu.size(u);       // u 所在集合大小
```

- `find(x)` 返回代表元；一般题里优先用 `unite/same/size`。
- 多测时每组重新构造 `DSU dsu(n)`，不要沿用上一组状态。


**API / 入口函数（赛时只认这里列的名字）：**

- `DSU dsu(n)` -> 建一个下标 `0..n` 都合法的并查集；竞赛一般只用 `1..n`。
- `dsu.unite(u,v)` -> 合并两集合；本来已连通返回 `false`，成功合并返回 `true`。
- `dsu.same(u,v)` -> 判断两点当前是否在同一集合。
- `dsu.size(u)` -> 返回 `u` 所在集合大小。
- `dsu.find(u)` -> 返回代表元；一般只有需要代表元编号时才直接调用。

**抄板清单（照着做就行）：**

1. 抄哪段：整个 `DSU` 结构体，抄到 `solve()` 外面。
2. 构造：`DSU dsu(n);`，n 取点编号上限。
3. 调用：合并 `dsu.unite(u,v)`；判连通 `dsu.same(u,v)`；查大小 `dsu.size(u)`。
4. 取结果：`unite` 返回 `true` 表示这次合并新连上了两个集合，`false` 表示本来就连通；`same` 直接给布尔值。

**改造点（按题目改这几处）：**

- 编号：点从 `0` 开始也能直接用（`0..n` 都是合法下标），不需要偏移。
- 数连通块个数：外面维护 `cnt`，初值 `n`，每次 `unite` 返回 `true` 就 `--cnt`。
- 要带边权/点关系：这是朴素并查集，换“带权并查集”节。

**核心逻辑（改代码时别破坏）：**

- `find(x)` 把点映射到集合代表元；路径压缩负责把树压扁。
- `unite(a,b)` 只合并两个代表元，按 `sz` 小并大，避免树变高。
**改板时先认这几个量：**

- `sz`：集合/子树大小。
- `parent`：并查集父节点。

```cpp
// 维护的量：parent[x]（x 的父节点，代表元的 parent 指向自己）、sz[x]（x 为根时所在集合大小）。
// 不变量：每个集合有唯一代表元；find(x) 沿 parent 走到代表元，路径压缩后树高近似 O(log n)。
struct DSU {
    vector<int> parent, sz;

    DSU(int n = 0) { init(n); }

    void init(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        // 路径压缩：沿途点直接挂到代表元下，之后 find 都是 O(1) 量级
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b); // 按大小合并：小的挂到大的树上，压低树高
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }

    bool same(int a, int b) {
        return find(a) == find(b);
    }

    int size(int x) {
        return sz[find(x)];
    }
};
```
#### 单链表：头插、尾插、删除、反转与有序去重

> **赛时先看**
> - **题目信号**：题面强调“在某节点后插入/删除”“只能沿 next 向后走”“反转链表”“删除重复结点”；若频繁按下标随机访问，应该用 `vector`，不是链表。
> - **本质**：维护只能从头向后遍历的动态序列；已知前驱节点时 `O(1)` 插入或删除。适合链式队列、链式哈希桶、按出现顺序动态增删，或题目明确要求模拟单链表。
> - **接法**：连续操作 `H x` 头插、`T x` 尾插、`I p x` 在编号 `p` 后插入、`D p` 删除 `p` 后一个节点。用 `push_front`、`push_back`、`insert_after`、`erase_after` 直接模拟；若题目只给值而不给节点编号，先遍历找到前驱，复杂度就会变为 `O(n)`。
> - **复杂度判定**：头插、尾插、已知前驱后的插删均为 `O(1)`；按值查找/删除、遍历、反转与去重为 `O(n)`。下标池总空间 `O（操作数）`，本模板删除后不复用编号。
> - **维护的量**：`nodes`（节点池，存 `value/next`）；`head/tail`（表头/表尾节点编号，`-1` 表示空表）；`length`（当前节点数）。
> - **警告**：单链表删除一个节点需要它的前驱，头结点要单独处理；删除后的旧编号不可再作为有效节点使用；反转后旧 `head` 就是新 `tail`。有序去重只适用于相同值相邻的已排序链表。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 次操作。`H x` 头插值 `x`，`T x` 尾插值 `x`，`I p x` 在编号 `p` 的节点后插值 `x`，`D p` 删除编号 `p` 后的节点；最后输出链表当前值。

```cpp
StaticSinglyLinkedList list;                // 1. 结构体定义：直接构造，无需参数
list.push_front(3);                         // 2. 调用：头插 3，返回新节点编号
int p = list.push_back(5);                  // 3. 调用：尾插 5，返回节点编号
list.insert_after(p, 7);                    // 4. 调用：在编号 p 的节点后插入 7
list.erase_after(p);                        // 5. 调用：删除 p 后的节点（7）
for (int v : list.to_vector()) cout << v << ' ';  // 6. 遍历输出
```

样例：`push_front(3)`、`push_back(5)`、`insert_after(p,7)`、`erase_after(p)` 后链表为 `[3,5]`。

**传参要求（照这个传不会错）：**

- `StaticSinglyLinkedList()`：构造；无参；空表时 `head = tail = -1`。
- `push_front(value)` / `push_back(value)`：头/尾插入；`value` 任意 int；返回新节点编号（从 0 起，编号不复用）。
- `insert_after(position, value)`：在编号 `position` 的节点后插入；要求 `0 <= position < nodes.size()`；返回新节点编号。
- `erase_front()`：删除头节点；空表返回 `false`，否则返回 `true`。
- `erase_after(position)`：删除 `position` 的后继；无后继返回 `false`；要删头结点需单独处理。
- `erase_first_value(value)`：删除第一个值等于 `value` 的节点；找到返回 `true`，否则 `false`。
- `reverse()`：原地反转链表，无返回值；`deduplicate_sorted()` 仅适用于已排序链表去重。
- `to_vector()`：返回按链表顺序的 `vector<int>` 值序列。


```cpp
struct StaticSinglyLinkedList {
    struct Node {
        int value;
        int next = -1;
    };

    vector<Node> nodes;
    int head = -1, tail = -1;
    int length = 0;

    bool empty() const { return head == -1; }

    int make_node(int value) {
        nodes.push_back({value, -1});
        return (int)nodes.size() - 1;
    }

    int push_front(int value) {
        int id = make_node(value);
        nodes[id].next = head;
        head = id;
        if (tail == -1) tail = id;
        ++length;
        return id;
    }

    int push_back(int value) {
        int id = make_node(value);
        if (tail == -1) head = tail = id;
        else {
            nodes[tail].next = id;
            tail = id;
        }
        ++length;
        return id;
    }

    int insert_after(int position, int value) {
        assert(0 <= position && position < (int)nodes.size());
        int id = make_node(value);
        nodes[id].next = nodes[position].next;
        nodes[position].next = id;
        if (tail == position) tail = id;
        ++length;
        return id;
    }

    bool erase_front() {
        if (head == -1) return false;
        head = nodes[head].next;
        if (head == -1) tail = -1;
        --length;
        return true;
    }

    bool erase_after(int position) {
        assert(0 <= position && position < (int)nodes.size());
        int erased = nodes[position].next;
        if (erased == -1) return false;
        nodes[position].next = nodes[erased].next;
        if (tail == erased) tail = position;
        --length;
        return true;
    }

    bool erase_first_value(int value) {
        if (head == -1) return false;
        if (nodes[head].value == value) return erase_front();
        for (int previous = head; nodes[previous].next != -1; previous = nodes[previous].next) {
            if (nodes[nodes[previous].next].value == value) return erase_after(previous);
        }
        return false;
    }

    void reverse() {
        int previous = -1, current = head;
        tail = head;
        while (current != -1) {
            int following = nodes[current].next;
            nodes[current].next = previous;
            previous = current;
            current = following;
        }
        head = previous;
    }

    void deduplicate_sorted() {
        for (int current = head; current != -1 && nodes[current].next != -1;) {
            int following = nodes[current].next;
            if (nodes[current].value == nodes[following].value) {
                nodes[current].next = nodes[following].next;
                if (tail == following) tail = current;
                --length;
            } else {
                current = following;
            }
        }
    }

    vector<int> to_vector() const {
        vector<int> result;
        for (int current = head; current != -1; current = nodes[current].next) {
            result.push_back(nodes[current].value);
        }
        return result;
    }
};
```

#### 双向循环链表：哨兵、O(1) 插删、移动与约瑟夫环

> **赛时先看**
> - **题目信号**：题面出现“在某元素前/后插入”“删除后仍要得到前后邻居”“把一个元素移到开头/结尾”“环形报数”；若操作只发生在两端，普通 `deque` 往往更短。
> - **本质**：维护循环顺序，并在已知节点编号时 `O(1)` 插入、删除、移动到首尾。适合 LRU 顺序、桌面牌堆/队列模拟、字符串光标两侧操作、约瑟夫环和“删除当前后继续走”的题。
> - **接法**：报数到第 `m` 人就离开，问最后留下的编号，调用 `josephus_survivor(n,m)`。若每次删除后还需向左走，删除前同时保存 `previous[current]`；若题目要求按第 `k` 个位置删除且 `n,q` 很大，链表不能快速跳第 `k` 个，应改用树状数组或平衡树。
> - **复杂度判定**：已知节点的插入、删除、移动均为 `O(1)`；按值寻找节点和完整遍历为 `O(n)`。空间 `O（操作数）`。
> - **维护的量**：`value/previous/next`（三个平行数组，节点 `0` 恒为哨兵）；`length`（链表长度）。
> - **警告**：节点 `0` 是哨兵，不存数据；空表时 `next[0]=prev[0]=0`。删除节点前先保存下一节点，避免用已脱链节点继续走；`move_to_front/back` 只能操作仍在链表中的节点。此模板也不复用已删节点编号。


**最小完整示例（先抄这一段就能跑）：**

题目：约瑟夫环：`n` 个人围一圈报数，报到 `m` 的人离队，问最后留下的人的编号。

```cpp
int n, m;
cin >> n >> m;
int survivor = josephus_survivor(n, m);  // 1. 调用：n 人，报数到 m 离队
cout << survivor << '\n';                // 2. 输出：最后留下的人的编号（1-based）
```

样例：`josephus_survivor(7, 3)` -> 输出 `4`。

**传参要求（照这个传不会错）：**

- `josephus_survivor(n, step)`：`n` = 人数（`>= 1`），`step` = 报数到第几离队（`>= 1`）；返回最后留下者的编号（1-based）。
- `push_front(x)` / `push_back(x)`：在表头/表尾插入；返回新节点编号（从 1 起，0 是哨兵，编号不复用）。
- `insert_after(pos, x)` / `insert_before(pos, x)`：在节点 `pos` 后/前插入；`pos` 必须已链接（或为哨兵 0）；返回新节点编号。
- `erase(node)`：删除节点 `node`；要求 `node` 已链接且不是哨兵 0；无返回值。
- `move_to_front(node)` / `move_to_back(node)`：把节点 `node` 移到表头/表尾；要求 `node` 仍在链表中。
- `first()` / `last()`：返回第一个/最后一个数据节点编号（空表返回 0）。
- `next_alive(node)`：返回 `node` 的后继；碰到哨兵时绕回第一个节点。

**API / 入口函数（赛时只认这里列的名字）：**

- `push_front(x)` / `push_back(x)` -> 在表头/表尾插入，返回新节点编号。
- `insert_after(pos, x)` / `insert_before(pos, x)` -> 在节点 `pos` 后/前插入，返回新节点编号。
- `erase(int node)` -> 删除节点；节点 `0` 是哨兵，不能删。
- `move_to_front(node)` / `move_to_back(node)` -> 把节点移到表头/表尾。
- `josephus_survivor(n, m)` -> 报数到第 `m` 个人离队，返回最后留下的编号。

```cpp
struct IndexDoublyCircularList {
    vector<int> value, previous, next;
    int length = 0;

    IndexDoublyCircularList() : value(1), previous(1, 0), next(1, 0) {}

    bool empty() const { return length == 0; }
    int first() const { return next[0]; }
    int last() const { return previous[0]; }
    bool linked(int node) const { return node != 0 && previous[node] != node; }

    int make_node(int x) {
        value.push_back(x);
        int id = (int)value.size() - 1;
        previous.push_back(id);
        next.push_back(id); // 单独节点：尚未接入循环链表。
        return id;
    }

    void link_between(int left, int node, int right) {
        next[left] = node;
        previous[node] = left;
        next[node] = right;
        previous[right] = node;
    }

    int insert_after(int position, int x) {
        assert(position == 0 || linked(position));
        int node = make_node(x);
        link_between(position, node, next[position]);
        ++length;
        return node;
    }

    int insert_before(int position, int x) {
        assert(position == 0 || linked(position));
        return insert_after(previous[position], x);
    }

    int push_front(int x) { return insert_after(0, x); }
    int push_back(int x) { return insert_before(0, x); }

    void erase(int node) {
        assert(linked(node));
        int left = previous[node], right = next[node];
        next[left] = right;
        previous[right] = left;
        previous[node] = next[node] = node; // 标记为脱链，防止重复删除。
        --length;
    }

    void move_to_front(int node) {
        assert(linked(node));
        int left = previous[node], right = next[node];
        next[left] = right;
        previous[right] = left;
        link_between(0, node, next[0]);
    }

    void move_to_back(int node) {
        assert(linked(node));
        int left = previous[node], right = next[node];
        next[left] = right;
        previous[right] = left;
        link_between(previous[0], node, 0);
    }

    int next_alive(int node) const {
        int result = next[node];
        return result == 0 ? next[0] : result;
    }

    vector<int> to_vector() const {
        vector<int> result;
        for (int node = next[0]; node != 0; node = next[node]) result.push_back(value[node]);
        return result;
    }
};

int josephus_survivor(int n, int step) {
    assert(n >= 1 && step >= 1);
    IndexDoublyCircularList list;
    for (int x = 1; x <= n; ++x) list.push_back(x);
    int current = list.first();
    while (list.length > 1) {
        for (int count = 1; count < step; ++count) current = list.next_alive(current);
        int following = list.next_alive(current);
        list.erase(current);
        current = following;
    }
    return list.value[list.first()];
}
```

#### 后继并查集：区间中跳过已处理位置

> **赛时先看**
> - **题目信号**：循环里总在“从 `l` 开始找下一个还没处理的位置”，每个下标最多处理一次；普通逐格扫描会因重叠区间反复访问同一批位置。
> - **本质**：初始有位置 `1..n` 都未处理；一个位置一旦处理就永久删除，要求快速找到不小于 `x` 的第一个未处理位置。适合区间染色、区间赋值、批量访问未访问节点。
> - **接法**：有若干次操作“把区间 `[l,r]` 中尚未涂色的位置涂成颜色 `c`”，每个位置只需第一次涂色。用上面的循环枚举仍存活的位置，写 `color[x]=c` 后删除它；总时间是所有位置数加查询数的近线性，而不是所有区间长度之和。
> - **复杂度判定**：初始化 `O(n)`；所有 `find` 与删除摊还 `O(alpha(n))`，整轮每个位置只删除一次。
> - **维护的量**：`parent[x]`（位置 `x` 链向的下一个存活位置；`find(x)` 即第一个存活位置）。
> - **警告**：只能永久向右删除，不能恢复；`n+1` 是哨兵，表示不存在可用位置；调用 `erase_and_next(x)` 前必须保证 `x` 当前未删除。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个位置初始都未涂色；每次把区间 `[l,r]` 中还没涂色的位置涂成颜色 `c`，每个位置只涂一次。

```cpp
SuccessorDSU dsu(n);                       // 1. 结构体定义：n 个位置，编号 1..n
for (int x = dsu.first_alive(l); x <= r; x = dsu.erase_and_next(x)) {
    color[x] = c;                          // 2. 调用：涂色后删除，x 跳到下一个未处理位置
}
```

样例：`n=6`，先涂 `[2,5]` 再涂 `[3,4]` -> 每个位置恰好涂一次，共涂 4 次。

**传参要求（照这个传不会错）：**

- `SuccessorDSU(n)`：构造；`n` = 位置数（编号 `1..n`），内部自动开 `n+1` 哨兵。
- `init(n_)`：重置为 `n_` 个位置，全部初始存活。
- `find(x)`：返回第一个 `>= x` 的存活位置（可能是 `n+1`，表示没有）。
- `first_alive(x)`：等价于 `find(x)`，返回第一个存活位置。
- `erase_and_next(x)`：删除 `x` 并返回下一个存活位置（可能 `n+1`）；要求 `x` 当前未删除。

**API / 入口函数（赛时只认这里列的名字）：**

- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `init(int n_)` -> 初始化/清空结构
- `erase_and_next(int x)` -> 删除仍存活的 x，并返回下一个存活位置，可能是 n+1。
**改板时先认这几个量：**

- `parent`：并查集父节点。

```cpp
struct SuccessorDSU {
    int n;
    vector<int> parent;

    explicit SuccessorDSU(int n_ = 0) {
        if (n_) init(n_);
    }

    void init(int n_) {
        n = n_;
        parent.resize(n + 2);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    int first_alive(int x) { return find(x); }

    // 删除仍存活的 x，并返回下一个存活位置，可能是 n+1。
    int erase_and_next(int x) {
        parent[x] = find(x + 1);
        return parent[x];
    }
};

// 把 [l,r] 中仍存活的每个下标恰好处理一次：
// 示例：for (int x = dsu.first_alive(l); x <= r; x = dsu.erase_and_next(x)) {
// 示例：process(x);
// 公式/约定：}
```

#### 可撤销并查集

> **赛时先看**
> - **题目信号**：边会删除；操作可以离线；需要回到某个历史状态。
> - **本质**：离线动态连通性、时间分治、需要撤销合并。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：合并 `O(log n)` 以内，回滚按合并次数。
> - **维护的量**：`parent`（父节点，不路径压缩）；`sz`（子树大小）；`history`（合并记录栈，回滚时弹栈复原）；`components`（当前连通块数）。
> - **警告**：不能路径压缩，否则无法回滚。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点、`m` 条边按顺序加入，过程中要回到某个历史状态再继续加边（离线动态连通性）。

```cpp
RollbackDSU dsu(n);                  // 1. 结构体定义：n 个点，编号 1..n
int snap = dsu.snapshot();           // 2. 调用：记录当前历史长度，返回快照编号
dsu.unite(u, v);                     // 3. 调用：合并 u、v 所在集合
bool ok = dsu.same(x, y);            // 4. 调用：判断 x、y 是否同一集合
dsu.rollback(snap);                  // 5. 调用：撤销 snap 之后的所有合并
```

样例：`n=3`；`snapshot()` 后 `unite(1,2)`、`unite(2,3)`，再 `rollback(snap)` -> 三个点恢复独立。

**传参要求（照这个传不会错）：**

- `RollbackDSU(n)`：构造；`n` = 点数（编号 `1..n`）。
- `init(n)`：重置为 `n` 个独立点，并清空历史栈。
- `find(x)`：返回 `x` 所在集合代表元（不路径压缩，约 `O(log n)`）。
- `unite(a, b)`：合并 `a`、`b` 所在集合；已同集合返回 `false`，否则合并成功返回 `true`。
- `snapshot()`：返回当前历史长度（int），即"快照编号"。
- `rollback(snap)`：回滚到快照 `snap` 时的状态；`snap` 必须是之前 `snapshot()` 的返回值。

**API / 入口函数（赛时只认这里列的名字）：**

- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `init(int n)` -> 初始化/清空结构
- `unite(int a, int b)` -> 合并两个集合 返回 `bool`。
- `snapshot()` -> 记录当前历史长度，返回快照编号。
- `rollback(int snap)` -> 回滚到快照 `snap` 时的状态。
**改板时先认这几个量：**

- `sz`：集合/子树大小。
- `parent`：并查集父节点。

```cpp
struct RollbackDSU {
    vector<int> parent, sz;
    vector<pair<int, int>> history;
    int components = 0;

    RollbackDSU(int n = 0) { init(n); }

    void init(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
        history.clear();
        components = n;
    }

    int find(int x) const {
        while (parent[x] != x) x = parent[x];
        return x;
    }

    int snapshot() const {
        return (int)history.size();
    }

    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) {
            history.push_back({-1, -1});
            return false;
        }
        if (sz[a] < sz[b]) swap(a, b);
        history.push_back({b, sz[a]});
        parent[b] = a;
        sz[a] += sz[b];
        components--;
        return true;
    }

    void rollback(int snap) {
        while ((int)history.size() > snap) {
            auto [b, old_size] = history.back();
            history.pop_back();
            if (b == -1) continue;
            int a = parent[b];
            parent[b] = b;
            sz[a] = old_size;
            components++;
        }
    }
};
```
#### 带权并查集

> **赛时先看**
> - **题目信号**：约束形如 `value[y] - value[x] = w`，需要判断矛盾。
> - **本质**：维护集合内点之间的相对权值，如距离差、势能差。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：均摊接近 `O(1)`。
> - **维护的量**：`parent`（父节点）；`sz`（子树大小）；`diff[x]`（`value[x]-value[parent[x]]`，压缩后即 `value[x]-value[root]`）。
> - **警告**：`diff[x]` 表示 `value[x] - value[parent[x]]`；路径压缩时要累加。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个变量，`q` 条约束 `value[y] - value[x] = w`，判断约束是否矛盾。

```cpp
WeightedDSU dsu(n);                  // 1. 结构体定义：n 个变量，编号 1..n
bool ok = dsu.unite(x, y, w);        // 2. 调用：加约束 value[y]-value[x]=w；与已知矛盾返回 false
i64 d = dsu.difference(x, y);        // 3. 调用：同一集合时返回 value[y]-value[x]
```

样例：`unite(1,2,3)`、`unite(2,3,4)` -> `difference(1,3)=7`；再 `unite(3,1,2)` 会矛盾返回 `false`。

**传参要求（照这个传不会错）：**

- `WeightedDSU(n)`：构造；`n` = 变量数（编号 `1..n`）。
- `init(n)`：重置为 `n` 个独立变量，相对值全为 0。
- `find(x)`：路径压缩查找；调用后 `diff[x]` 更新为 `value[x]-value[root]`。
- `weight(x)`：返回 `value[x]-value[root]`（根节点相对值视为 0），`i64`。
- `unite(x, y, w)`：加约束 `value[y]-value[x]=w`；不同集合则合并返回 `true`；同集合返回约束是否满足（矛盾为 `false`）。
- `difference(x, y)`：要求 `x,y` 同一集合；返回 `value[y]-value[x]`，`i64`。

**API / 入口函数（赛时只认这里列的名字）：**

- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `init(int n)` -> 初始化/清空结构
- `unite(int x, int y, i64 w)` -> 合并两个集合 返回 `bool`。
**改板时先认这几个量：**

- `sz`：集合/子树大小。
- `diff`：公式：value[x]-value[parent[x]]。

```cpp
struct WeightedDSU {
    vector<int> parent, sz;
    vector<i64> diff; // 公式：value[x]-value[parent[x]]。

    WeightedDSU(int n = 0) { init(n); }

    void init(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        diff.assign(n + 1, 0);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] == x) return x;
        int p = parent[x];
        parent[x] = find(parent[x]);
        diff[x] += diff[p];
        return parent[x];
    }

    i64 weight(int x) {
        find(x);
        return diff[x]; // 公式：value[x]-value[root]。
    }

    bool unite(int x, int y, i64 w) {
        // 加入约束：value[y] - value[x] = w。
        int rx = find(x), ry = find(y);
        i64 wx = weight(x), wy = weight(y);
        if (rx == ry) return wy - wx == w;
        if (sz[rx] < sz[ry]) {
            parent[rx] = ry;
            diff[rx] = wy - wx - w; // 公式：value[rx]-value[ry]。
            sz[ry] += sz[rx];
        } else {
            parent[ry] = rx;
            diff[ry] = wx + w - wy; // 公式：value[ry]-value[rx]。
            sz[rx] += sz[ry];
        }
        return true;
    }

    i64 difference(int x, int y) {
        // 要求同一集合；返回 value[y] - value[x]。
        return weight(y) - weight(x);
    }
};
```
#### 带权并查集例题：区间和约束判矛盾

> **赛时先看**
> - **题目信号**：区间和、前缀和差值、真假话/矛盾数。
> - **本质**：给出若干断言 `sum[l..r] = s`，判断有多少条与之前断言矛盾。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
    > - **复杂度判定**：`O(m alpha(n))`。
    > - **维护的量**：`pre[i]`（前缀和节点，编号 `i+1`）；带权并查集的 `weight`（到根的差值）。
    > - **警告**：令 `pre[i] = sum[1..i]`，约束变成 `pre[r] - pre[l-1] = s`。
    > - **约定**：`WeightedDSUExample dsu(n + 1);`，前缀节点 `0..n` 都合法；代码把 `pre[i]` 映射到编号 `i+1`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个变量，`m` 条约束 `sum[l..r] = s`（即 `pre[r] - pre[l-1] = s`），输出与前面约束矛盾的条数。

```cpp
WeightedDSUExample dsu(n + 1);        // 1. 结构体定义：前缀节点 0..n，编号映射为 i+1
int bad = 0;
while (m--) {
    int l, r;
    i64 s;
    cin >> l >> r >> s;
    int x = l;                        // 2. pre[l-1] 的编号：l-1+1
    int y = r + 1;                    // 3. pre[r] 的编号：r+1
    if (!dsu.unite(x, y, s)) bad++;   // 4. 与已知约束矛盾，计数加 1
}
cout << bad << '\n';
```

样例：`unite(1,3,6)`（`sum[1..3]=6`）、`unite(2,3,3)`（`sum[2..3]=3`）都成立；再加 `unite(2,3,5)` 与第二条矛盾返回 `false` -> `bad = 1`。

**传参要求（照这个传不会错）：**

- `WeightedDSUExample(n + 1)`：构造；`n` = 变量个数；前缀节点 `0..n` 都合法，代码把 `pre[i]` 映射到编号 `i+1`。
- `unite(x, y, w)`：加约束 `value[y]-value[x]=w`；不同集合直接合并返回 `true`，同集合返回约束是否满足（矛盾为 `false`）。
- `find(x)` / `weight(x)`：路径压缩查找与相对权值；一般题里直接 `unite` 判矛盾即可。
- `count_contradictions(n, statements)`：一行求出矛盾条数；`statements` 每个元素是 `{l, r, s}`（1-indexed 闭区间 `sum[l..r]=s`），返回 `int`。


**API / 入口函数（赛时只认这里列的名字）：**

- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `init(int n)` -> 初始化/清空结构
- `unite(int x, int y, i64 w)` -> 合并两个集合 返回 `bool`。
**改板时先认这几个量：**

- `sz`：集合/子树大小。
- `parent`：并查集父节点。
- `diff`：公式：value[x]-value[parent[x]]。

```cpp
struct WeightedDSUExample {
    vector<int> parent, sz;
    vector<i64> diff;

    WeightedDSUExample(int n = 0) { init(n); }
    void init(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        diff.assign(n + 1, 0);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] == x) return x;
        int p = parent[x];
        parent[x] = find(parent[x]);
        diff[x] += diff[p];
        return parent[x];
    }
    i64 weight(int x) { find(x); return diff[x]; }
    bool unite(int x, int y, i64 w) {
        int rx = find(x), ry = find(y);
        i64 wx = weight(x), wy = weight(y);
        if (rx == ry) return wy - wx == w;
        parent[rx] = ry;
        diff[rx] = wy - wx - w;
        return true;
    }
};

int count_contradictions(int n, const vector<tuple<int, int, i64>>& statements) {
    WeightedDSUExample dsu(n + 1); // 前缀点为 0..n；若 DSU 使用 1-based，需要整体 +1 偏移。
    int bad = 0;
    for (auto [l, r, s] : statements) {
        int x = l;     // 表示 pre[l-1] 偏移后的编号：l-1+1。
        int y = r + 1; // 表示 pre[r] 偏移后的编号：r+1。
        if (!dsu.unite(x, y, s)) bad++;
    }
    return bad;
}
```
#### 关系并查集：食物链 mod 3

> **赛时先看**
> - **题目信号**：关系有模意义，常见 `0=同类， 1=吃， 2=被吃`。
> - **本质**：维护三类循环关系，如 A 吃 B、同类等。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：均摊接近 `O(1)`。
> - **维护的量**：`parent`（父节点）；`rel[x]`（`x` 到 `parent[x]` 的关系，mod 3；`0=同类，1=吃，2=被吃`）。
> - **警告**：这里 `rel[x]` 表示 `x` 到父亲的关系；`relation(x,y)` 返回 `x` 相对 `y`。


**最小完整示例（先抄这一段就能跑）：**

题目：食物链：`n` 个动物，`q` 句话。`1 x y` 表示同类，`2 x y` 表示 `x` 吃 `y`；统计与前面已确认信息矛盾的假话数。

```cpp
ModDSU dsu(n);                       // 1. 结构体定义：n 个动物，编号 1..n
bool ok = dsu.unite(x, y, r);        // 2. 调用：加约束 x->y 关系为 r（0=同类，1=吃）
if (!ok) bad++;                      // 3. 统计：与已知信息矛盾，记为假话
```

样例：`unite(1,2,1)`（1 吃 2）、`unite(2,3,1)`（2 吃 3）-> `relation(1,3)=2`（1 被 3 吃）。

**传参要求（照这个传不会错）：**

- `ModDSU(n)`：构造；`n` = 元素数（编号 `1..n`）。
- `init(n)`：重置为 `n` 个独立元素。
- `find(x)`：路径压缩查找；压缩后 `rel[x]` 为 `x` 到根的关系（mod 3）。
- `relation(x, y)`：要求 `x,y` 同一集合；返回 `x -> y` 的关系（`0=同类，1=吃，2=被吃`）。
- `unite(x, y, r)`：加约束 `x -> y` 关系为 `r`（`r` 取 0/1/2）；不同集合则合并返回 `true`；同集合返回已有关系是否等于 `r`。

**API / 入口函数（赛时只认这里列的名字）：**

- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `init(int n)` -> 初始化/清空结构
- `unite(int x, int y, int r)` -> 合并两个集合 返回 `bool`。
**改板时先认这几个量：**

- `rel`：rel[x] 表示 x 到 parent[x] 的关系，mod 3。
- `parent`：并查集父节点。

```cpp
struct ModDSU {
    vector<int> parent, rel; // rel[x] 表示 x 到 parent[x] 的关系，mod 3。

    ModDSU(int n = 0) { init(n); }

    void init(int n) {
        parent.resize(n + 1);
        rel.assign(n + 1, 0);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] == x) return x;
        int p = parent[x];
        parent[x] = find(parent[x]);
        rel[x] = (rel[x] + rel[p]) % 3;
        return parent[x];
    }

    int relation(int x, int y) {
        // 确保同一集合后，返回 x -> y 的关系。
        find(x); find(y);
        return (rel[x] - rel[y] + 3) % 3;
    }

    bool unite(int x, int y, int r) {
        // 加入约束：x -> y 的关系为 r。
        int rx = find(x), ry = find(y);
        if (rx == ry) return relation(x, y) == r;
        parent[rx] = ry;
        rel[rx] = (r + rel[y] - rel[x] + 3) % 3;
        return true;
    }
};
```
#### 树状数组：单点加，区间和

> **赛时先看**
> - **题目信号**：数组有单点加/改；反复问 `[l,r]` 的和；或“前缀和”“逆序对计数”。看到单点改+区间和，先想 BIT。
> - **本质**：把前缀和拆成 `O(log n)` 段 lowbit 块累加：`bit[i]` 维护下标 `i` 往前 `lowbit(i)` 个位置的和，查询前缀时逐段相加。
> - **复杂度判定**：修改/查询 `O(log n)`；n 到 1e6 仍可用（常数极小）；要区间加请换“区间加区间和”双 BIT 版；要区间最值换线段树。
> - **维护的量**：`n`（长度）；`bit[i]`（第 i 个 lowbit 块的和，1-indexed）。
> - **接法**：把题目里的“当前位置贡献多少”变成 `add(pos, value)`；把“前 `r` 个的总和”变成 `sum_prefix(r)`；区间 `[l,r]` 的和就是 `range_sum(l,r)`。如果原值很大但只关心相对大小，先离散化成 `1..k` 再开树状数组；如果要找第 `k` 个，需要所有频次非负。
> - **警告**：下标从 1 开始，`add(0, ...)` 死循环；`range_sum(l,r)=prefix(r)-prefix(l-1)`；答案可能到 1e18 用 i64。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，`q` 次操作。`op=1 x y`：把 `a[x]` 加 `y`；`op=2 l r`：输出 `a[l..r]` 的和。

```cpp
Fenwick<i64> fw(n);                           // 1. 结构体定义：Fenwick<i64>(长度 n)
for (int i = 1; i <= n; ++i) fw.add(i, a[i]); // 2. 初始化：把 a[i] 加进下标 i
while (q--) {
    int op, x, y;
    cin >> op >> x >> y;
    if (op == 1) fw.add(x, y);                     // 3. 调用：a[x] += y
    else cout << fw.range_sum(x, y) << '\n';       // 4. 调用：输出 a[x..y] 的和
}
```

样例：`n=5, a=[1,2,3,4,5]`；`add(2,10)` 后查 `[1,5]` -> 输出 `25`。

**传参要求（照这个传不会错）：**

- `Fenwick<T>(n)`：构造；`n` = 下标上限（数组长度）；内部 1-indexed。
- `add(pos, delta)`：`a[pos] += delta`；要求 `1 <= pos <= n`，`pos=0` 死循环。
- `sum_prefix(r)`：`a[1..r]` 的和；要求 `0 <= r <= n`（`r=0` 返回 0）。
- `range_sum(l, r)`：闭区间 `[l,r]` 的和；要求 `1 <= l <= r <= n`。
- `lower_bound(k)`：前缀和单调非负时，找最小 `pos` 使前缀和 `>= k`。
- 返回值：和，类型 = 模板参数 `T`；数大用 `i64`。


**不会用就照抄：**

```cpp
Fenwick<i64> fw(n);
fw.add(pos, delta);           // a[pos] += delta
auto s1 = fw.sum_prefix(r);   // a[1..r] 的和
auto s2 = fw.range_sum(l, r); // a[l..r] 的和
```

- 下标必须从 **1** 开始，`add(0, ...)` 会死循环。
- 这是“加法版”，不是“把 a[pos] 改成 x”；赋值要自己先算差值。


**API / 入口函数（赛时只认这里列的名字）：**

- `Fenwick<i64> fw(n)` -> 建立 1-indexed 树状数组。
- `fw.add(pos,delta)` -> 执行 `a[pos] += delta`；`pos` 不能为 0。
- `fw.sum_prefix(r)` -> 返回 `a[1..r]` 的和。
- `fw.range_sum(l,r)` -> 返回闭区间 `a[l..r]` 的和。
- `fw.lower_bound(k)` -> 前缀和非负且单调时，找最小 `pos` 使前缀和 `>= k`。

**抄板清单（照着做就行）：**

1. 抄哪段：整个 `Fenwick` 结构体。
2. 构造：`Fenwick<i64> fw(n);`，n 为下标上限。
3. 调用：单点加 `fw.add(pos, delta)`；区间和 `fw.range_sum(l, r)`。
4. 取结果：`range_sum` 直接就是答案；要把 `a[pos]` 改成 `x`，先查当前值再 `add(pos, x - 当前值）`。

**改造点（按题目改这几处）：**

- 模板参数：答案/权值可能超 int 就写 `Fenwick<i64>`。
- 赋值：BIT 只支持“加”，改成 `x` 要自己先算差值 `add(pos, x - 旧值）`。
- 值域大：先离散化成 `1..k` 再开 BIT。
- 当计数器用（逆序对等）：每次 `add(pos, 1)` 统计出现次数。

**核心逻辑（改代码时别破坏）：**

- `bit[i]` 管的是长度为 `lowbit(i)` 的后缀块。
- 修改一路 `i += lowbit(i)`；前缀查询一路 `i -= lowbit(i)`；区间和 = 两个前缀相减。

```cpp
// 维护的量：n（长度）；bit[i] = 下标 i 往前 lowbit(i) 个位置的和，1-indexed。
// 不变量：前缀和 sum(x) = bit[x] + bit[x-lowbit(x)] + ...，共 O(log n) 段。
template <class T>
struct Fenwick {
    int n;
    vector<T> bit;

    Fenwick(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        bit.assign(n + 1, T{});
    }

    void add(int idx, T val) {
        // 一路向右上走，更新所有覆盖 idx 的 lowbit 块
        for (; idx <= n; idx += idx & -idx) bit[idx] += val;
    }

    T sum_prefix(int idx) const {
        T res{};
        // 从右往左拆成 O(log n) 个 lowbit 块累加
        for (; idx > 0; idx -= idx & -idx) res += bit[idx];
        return res;
    }

    T range_sum(int l, int r) const {
        if (l > r) return T{};
        return sum_prefix(r) - sum_prefix(l - 1); // 区间和 = 两个前缀和相减
    }

    int lower_bound(T target) const {
        assert(n > 0);
        int pos = 0;
        int k = 1;
        while ((k << 1) <= n) k <<= 1; // 最大的不超过 n 的 2 的幂，避免依赖 GCC 的 __lg。
        for (; k; k >>= 1) {
            int next = pos + k;
            if (next <= n && bit[next] < target) {
                pos = next;
                target -= bit[next];
            }
        }
        return pos + 1;
    }
};
```

`Fenwick::lower_bound(target)` 返回最小下标 `pos` 使 `sum_prefix(pos) >= target`，适合“按频次找第 `k` 小 / 找第 `k` 个未删位置”。前提是所有维护值非负，且 `1 <= target <= sum_prefix(n)`；若有负数，前缀和不再单调，不能二分。
#### 树状数组求逆序对

> **赛时先看**
> - **题目信号**：问交换次数、逆序数量、相对顺序混乱程度。
> - **本质**：统计 `i < j` 且 `a[i] > a[j]` 的对数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n log n)`。
> - **维护的量**：`Fenwick<int> fw`（值域频次计数，下标 1..k）；`ans`（已累计的逆序对数）。
> - **警告**：值域大时必须离散化。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，求 `i<j` 且 `a[i]>a[j]` 的逆序对总数。

```cpp
vector<int> a = {3, 1, 4, 2};        // 1. 数据：原数组（0-indexed，值域任意）
i64 ans = count_inversions(a);       // 2. 调用：一行求出逆序对数（内部自动离散化）
cout << ans << '\n';                 // 3. 输出
```

样例：`a=[3,1,4,2]` -> 输出 `3`（(3,1)、(3,2)、(4,2) 共 3 对）。

**传参要求（照这个传不会错）：**

- `count_inversions(a)`：`a` = 原数组（`vector<int>`，0-indexed，值域任意）；函数内自动 `sort+unique+lower_bound` 离散化，无需预处理；返回 `i64` 逆序对数。
- 依赖本文件上方 `Fenwick` 模板，抄板时一起抄。
- 数据是 1-indexed 时，先转成 0-indexed 再传（或复制 `a[1..n]`）。


```cpp
// 约定：本函数为 0-indexed。
i64 count_inversions(vector<int> a) {
    vector<int> xs = a;
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());

    Fenwick<int> fw((int)xs.size());
    i64 ans = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        int id = int(lower_bound(xs.begin(), xs.end(), a[i]) - xs.begin()) + 1;
        ans += i - fw.sum_prefix(id);
        fw.add(id, 1);
    }
    return ans;
}
```
#### 树状数组：区间加，区间和

> **赛时先看**
> - **题目信号**：操作是 `add l r x` 和 `sum l r`。
> - **本质**：区间整体加值，同时查询区间和。
> - **接法**：题面操作是“给 `[l,r]` 每个数都加 `x`”就调用 `range_add(l,r,x)`；题面问“`[l,r]` 当前总和”就调用 `range_sum(l,r)`。如果题目问的是最大值、最小值或最大子段和，不要硬改这个模板，直接翻线段树。
> - **复杂度判定**：`O(log n)`。
> - **维护的量**：`n`（长度）；`b1`（差分 BIT）；`b2`（加权差分 BIT，存 `val*(idx-1)`）。
> - **警告**：两个 BIT 的公式要配套，不要混用。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，`q` 次操作。`op=1 l r x`：`a[l..r]` 每个数加 `x`；`op=2 l r`：输出 `a[l..r]` 的和。

```cpp
RangeFenwick rf(n);                  // 1. 结构体定义：长度 n，内部两个 Fenwick<i64>
while (q--) {
    int op, l, r;
    i64 x;
    cin >> op >> l >> r;
    if (op == 1) { cin >> x; rf.range_add(l, r, x); }   // 2. 调用：区间加
    else cout << rf.range_sum(l, r) << '\n';            // 3. 调用：输出区间和
}
```

样例：`n=5, a=[1,2,3,4,5]`；`range_add(2,4,10)` 后 `range_sum(2,4)` -> 输出 `39`。

**传参要求（照这个传不会错）：**

- `RangeFenwick(n)`：构造；`n` = 数组长度（下标 `1..n`）。
- `init(n_)`：重置长度为 `n_`，并清空两个 BIT。
- `range_add(l, r, val)`：闭区间 `[l,r]` 每个位置加 `val`；要求 `1 <= l <= r <= n`；无返回值。
- `range_sum(l, r)`：返回闭区间 `[l,r]` 的和，`i64`；要求 `1 <= l <= r <= n`。
- 初始值为 0：数组有初值时逐个 `range_add(i, i, a[i])` 打进去。

**API / 入口函数（赛时只认这里列的名字）：**

- `init(int n_)` -> 初始化/清空结构
- `range_add(int l, int r, i64 val)` -> 闭区间加值
- `range_sum(int l, int r)` -> 查询闭区间和 返回 `i64`。

```cpp
struct RangeFenwick {
    int n;
    Fenwick<i64> b1, b2;

    RangeFenwick(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        b1.init(n);
        b2.init(n);
    }

    void add_internal(int idx, i64 val) {
        b1.add(idx, val);
        b2.add(idx, val * (idx - 1));
    }

    void range_add(int l, int r, i64 val) {
        add_internal(l, val);
        add_internal(r + 1, -val);
    }

    i64 prefix_sum(int idx) const {
        return b1.sum_prefix(idx) * idx - b2.sum_prefix(idx);
    }

    i64 range_sum(int l, int r) const {
        return prefix_sum(r) - prefix_sum(l - 1);
    }
};
```
#### 二维树状数组

> **赛时先看**
> - **题目信号**：矩阵上动态修改点值，查询子矩形和。
> - **本质**：二维单点加、矩形求和。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：修改/查询 `O(log n log m)`。
> - **维护的量**：`n`、`m`（行列数）；`bit[i][j]`（二维 lowbit 块的和，全 1-indexed）。
> - **警告**：二维下标都从 1 开始；矩形和用四个前缀相减。


**最小完整示例（先抄这一段就能跑）：**

题目：`n*m` 矩阵，`q` 次操作。`op=1 x y v`：`a[x][y] += v`；`op=2 x1 y1 x2 y2`：输出子矩形 `[x1..x2] × [y1..y2]` 的和。

```cpp
Fenwick2D<i64> fw(n, m);             // 1. 结构体定义：n 行 m 列，行列都从 1 开始
while (q--) {
    int op, x, y, v;
    cin >> op;
    if (op == 1) { cin >> x >> y >> v; fw.add(x, y, v); }   // 2. 调用：单点加
    else {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        cout << fw.rect_sum(x1, y1, x2, y2) << '\n';        // 3. 调用：输出矩形和
    }
}
```

样例：`n=m=3`；`add(2,2,5)` 后 `rect_sum(1,1,3,3)` -> 输出 `5`。

**传参要求（照这个传不会错）：**

- `Fenwick2D<T>(n, m)`：构造；`n`/`m` = 行数/列数；内部下标全 1-based。
- `init(n_, m_)`：重置为 `n_` 行 `m_` 列，全部清零。
- `add(x, y, val)`：`a[x][y] += val`；要求 `1 <= x <= n`，`1 <= y <= m`。
- `sum_prefix(x, y)`：子矩形 `[1..x] × [1..y]` 的和，返回 `T`。
- `rect_sum(x1,y1,x2,y2)`：闭矩形 `[x1..x2] × [y1..y2]` 的和（四个前缀相减）；要求 `1 <= x1 <= x2 <= n`，`1 <= y1 <= y2 <= m`。
- 模板参数 `T`：和可能很大就写 `i64`。

**API / 入口函数（赛时只认这里列的名字）：**

- `add(int x, int y, T val)` -> 加入一个元素/贡献
- `init(int n_, int m_)` -> 初始化/清空结构
- `sum_prefix(int x, int y)` -> 查询前缀和 返回 `T`。
- `rect_sum(x1, y1, x2, y2)` -> 查询闭矩形 `[x1..x2] × [y1..y2]` 的和 返回 `T`。

```cpp
template <class T>
struct Fenwick2D {
    int n, m;
    vector<vector<T>> bit;

    Fenwick2D(int n = 0, int m = 0) { init(n, m); }

    void init(int n_, int m_) {
        n = n_;
        m = m_;
        bit.assign(n + 1, vector<T>(m + 1, T{}));
    }

    void add(int x, int y, T val) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) bit[i][j] += val;
        }
    }

    T sum_prefix(int x, int y) const {
        T res{};
        for (int i = x; i > 0; i -= i & -i) {
            for (int j = y; j > 0; j -= j & -j) res += bit[i][j];
        }
        return res;
    }

    T rect_sum(int x1, int y1, int x2, int y2) const {
        return sum_prefix(x2, y2) - sum_prefix(x1 - 1, y2) -
               sum_prefix(x2, y1 - 1) + sum_prefix(x1 - 1, y1 - 1);
    }
};
```
#### 区间颜色种类数：颜色很多且无修改

> **赛时先看**
> - **题目信号**：颜色值很大，无法用 bitmask；数组不修改。
> - **本质**：静态数组，多次查询 `[l,r]` 中不同颜色/数字数量。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O((n+q) log n)`。
> - **维护的量**：`fw`（FenwickCount 位置计数 BIT，当前扫描区间内每个位置贡献 1）；`last`（每种颜色最后一次出现的位置）；`cur`（已扫描到的右指针）。
> - **警告**：按右端点排序，遇到重复颜色时把上一次出现位置从 BIT 中删掉。


**最小完整示例（先抄这一段就能跑）：**

题目：静态数组 `a[1..n]`，`q` 次询问 `[l,r]` 内不同颜色/数字的个数（颜色值很大，不能 bitmask）。

```cpp
vector<Query> qs;
for (int i = 0; i < q; ++i) {
    int l, r;
    cin >> l >> r;
    qs.push_back({l, r, i});                     // 1. 收集询问：{l, r, 原始编号 id}
}
vector<int> ans = static_distinct_count(a, qs);  // 2. 调用：a 必须 1-indexed
for (int i = 0; i < q; ++i) cout << ans[i] << '\n';  // 3. 按原始编号输出
```

样例：`a=[1,2,1,3,2]`；问 `[1,3]` -> 2；`[2,5]` -> 3。

**传参要求（照这个传不会错）：**

- `a`：原数组，必须 1-indexed（长度 `n+1`，`a[0]` 不参与）；值域任意，用 `unordered_map` 记上次出现位置。
- `qs`：询问数组，元素 `Query{l, r, id}`；`l/r` 为 1-indexed 闭区间，`id` 为 0-based 原始编号。
- 返回值：`vector<int>`，`ans[id]` 就是第 `id` 个询问的答案。
- 只支持静态无修改的询问；所有询问先读完（离线）。

**API / 入口函数（赛时只认这里列的名字）：**

- `add(int i, T v)` -> 加入一个元素/贡献
- `init(int n_)` -> 初始化/清空结构
- `range_sum(int l, int r)` -> 查询闭区间和 返回 `T`。
**改板时先认这几个量：**

- `bit`：FenwickCount 内部树状数组。
- `cur`：当前指针（已扫到的位置）。
- `sum`：区间和/计数和。

```cpp
template <class T>
struct FenwickCount {
    int n;
    vector<T> bit;
    FenwickCount(int n = 0) { init(n); }
    void init(int n_) { n = n_; bit.assign(n + 1, T{}); }
    void add(int i, T v) { for (; i <= n; i += i & -i) bit[i] += v; }
    T sum(int i) const { T r{}; for (; i > 0; i -= i & -i) r += bit[i]; return r; }
    T range_sum(int l, int r) const { return sum(r) - sum(l - 1); }
};

struct Query {
    int l, r, id;
};

vector<int> static_distinct_count(const vector<int>& a, vector<Query> qs) {
    int n = (int)a.size() - 1;
    sort(qs.begin(), qs.end(), [](const Query& x, const Query& y) {
        return x.r < y.r;
    });
    FenwickCount<int> fw(n);
    unordered_map<int, int> last;
    vector<int> ans(qs.size());
    int cur = 0;
    for (auto q : qs) {
        while (cur < q.r) {
            cur++;
            if (last.count(a[cur])) fw.add(last[a[cur]], -1);
            fw.add(cur, 1);
            last[a[cur]] = cur;
        }
        ans[q.id] = fw.range_sum(q.l, q.r);
    }
    return ans;
}
```
#### 离线统计区间不同数字个数

> **赛时先看**
> - **题目信号**：数组静态，很多区间询问 distinct count。
> - **本质**：多次询问 `[l,r]` 内不同数的数量。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O((n+q) log n)`。
> - **维护的量**：`fw`（FenwickDistinct 位置计数 BIT）；`last`（每个值上次出现的位置）；`cur`（已扫描到的右指针）。
> - **警告**：按右端点排序，当前值上一次出现位置要从 BIT 中减掉。与 13 节「区间颜色种类数」同一模型，二选一即可。


**最小完整示例（先抄这一段就能跑）：**

题目：静态数组 `a[1..n]`，`q` 次询问 `[l,r]` 内不同数字的个数。

```cpp
vector<DistinctQuery> qs;
for (int i = 0; i < q; ++i) {
    int l, r;
    cin >> l >> r;
    qs.push_back({l, r, i});                     // 1. 收集询问：{l, r, 原始编号 id}
}
vector<int> ans = distinct_count_queries(a, qs); // 2. 调用：a 必须 1-indexed
for (int i = 0; i < q; ++i) cout << ans[i] << '\n';  // 3. 按原始编号输出
```

样例：`a=[1,2,1,3,2]`；问 `[1,3]` -> 2；`[2,5]` -> 3。

**传参要求（照这个传不会错）：**

- `a`：原数组，必须 1-indexed（长度 `n+1`，`a[0]` 不参与）；值域任意。
- `qs`：询问数组，元素 `DistinctQuery{l, r, id}`；`l/r` 为 1-indexed 闭区间，`id` 为 0-based 原始编号。
- 返回值：`vector<int>`，`ans[id]` 就是第 `id` 个询问的答案。
- 只支持静态无修改；与「区间颜色种类数」同一模型，二选一即可。


```cpp
struct DistinctQuery {
    int l, r, id;
};

// 精简版 Fenwick（int 版本），本节自带：add / range_sum。
// 与 13 节同一模型，二选一即可；本节的类型名带 Distinct 前缀，与其它节混抄也不会重名。
struct FenwickDistinct {
    int n;
    vector<int> bit;
    FenwickDistinct(int n = 0) { init(n); }
    void init(int n_) { n = n_; bit.assign(n + 1, 0); }
    void add(int i, int v) { for (; i <= n; i += i & -i) bit[i] += v; }
    int sum(int i) const { int r = 0; for (; i > 0; i -= i & -i) r += bit[i]; return r; }
    int range_sum(int l, int r) const { return sum(r) - sum(l - 1); }
};

vector<int> distinct_count_queries(const vector<int>& a, vector<DistinctQuery> qs) {
    int n = (int)a.size() - 1;
    sort(qs.begin(), qs.end(), [](const DistinctQuery& x, const DistinctQuery& y) {
        return x.r < y.r;
    });
    FenwickDistinct fw(n);
    unordered_map<int, int> last;
    vector<int> ans(qs.size());
    int cur = 0;
    for (auto q : qs) {
        while (cur < q.r) {
            cur++;
            if (last.count(a[cur])) fw.add(last[a[cur]], -1);
            fw.add(cur, 1);
            last[a[cur]] = cur;
        }
        ans[q.id] = fw.range_sum(q.l, q.r);
    }
    return ans;
}
```
#### 莫队：区间不同数数量

> **赛时先看**
> - **题目信号**：区间询问很多，贡献能在左右端点移动时 `O(1)` 更新；比如不同数数量、出现次数类。
> - **本质**：静态数组离线区间询问，维护一个可增删的窗口。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O((n+q) sqrt n)`。
> - **维护的量**：`cnt`（当前窗口内各值的出现次数）；`distinct`（窗口内不同数个数）；`cur_l/cur_r`（窗口左右端点）；`block`（块长，取 `sqrt(n)`）。
> - **警告**：`a` 需 1-indexed；离散化在函数内部完成；排序块大小一般取 `sqrt(n)`。


**最小完整示例（先抄这一段就能跑）：**

题目：静态数组 `a[1..n]`，`q` 次询问区间 `[l,r]` 内不同数字的个数。

```cpp
vector<MoQueryBasic> qs;
for (int i = 0; i < q; ++i) {
    int l, r;
    cin >> l >> r;
    qs.push_back({l, r, i});      // 1. 收集询问：{l, r, 原始编号 id}
}
vector<int> ans = mo_distinct_count(a, qs);  // 2. 调用：a 必须 1-indexed
for (int i = 0; i < q; ++i) cout << ans[i] << '\n';  // 3. 按原始编号输出
```

样例：`a=[1,2,1,3,2]`；问 `[1,3]` -> 2；`[2,5]` -> 3。

**传参要求（照这个传不会错）：**

- `a`：必须 1-indexed（长度 `n+1`，`a[0]` 不参与）；值域不限，内部自动离散化。
- `qs`：询问数组，每个元素 `{l, r, id}`，`l/r` 是 1-indexed 闭区间，`id` 是原始询问编号（0-based）。
- 返回值：`vector<int>`，`ans[id]` 是第 id 个询问的答案。
- 只支持离线（全部询问先读完再算）；带修改翻"莫队带修改"。


```cpp
struct MoQueryBasic {
    int l, r, id;
};

vector<int> mo_distinct_count(vector<int> a, vector<MoQueryBasic> qs) {
    int n = (int)a.size() - 1;
    vector<int> xs(a.begin() + 1, a.end());
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    for (int i = 1; i <= n; ++i) {
        a[i] = int(lower_bound(xs.begin(), xs.end(), a[i]) - xs.begin());
    }

    int block = max(1, (int)sqrt(n));
    sort(qs.begin(), qs.end(), [&](const MoQueryBasic& x, const MoQueryBasic& y) {
        int bx = x.l / block, by = y.l / block;
        if (bx != by) return bx < by;
        return (bx & 1) ? x.r > y.r : x.r < y.r;
    });

    vector<int> cnt(xs.size(), 0), ans(qs.size());
    int cur_l = 1, cur_r = 0, distinct = 0;

    auto add = [&](int pos) {
        if (++cnt[a[pos]] == 1) distinct++;
    };
    auto remove = [&](int pos) {
        if (--cnt[a[pos]] == 0) distinct--;
    };

    for (auto q : qs) {
        while (cur_l > q.l) add(--cur_l);
        while (cur_r < q.r) add(++cur_r);
        while (cur_l < q.l) remove(cur_l++);
        while (cur_r > q.r) remove(cur_r--);
        ans[q.id] = distinct;
    }
    return ans;
}
```
#### 莫队带修改

> **赛时先看**
> - **题目信号**：允许离线；查询答案能在左右端点移动和时间前后移动时 `O(1)` 或 `O(log n)` 更新。
> - **本质**：离线处理区间查询，同时存在单点修改。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：常用块长 `n^(2/3)`，复杂度约 `O(n^(2/3) q)`。
> - **维护的量**：`a`（当前数组，含已应用的修改）；`block_len`（块长 `n^(2/3)`）；窗口统计量由 `add_value/remove_value/current_answer` 维护。
> - **警告**：修改影响当前位置区间时，要先删旧值再加新值；记录修改前的值。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数。`1 l r` 询问 `[l,r]` 内不同数字个数；`2 p x` 把 `a[p]` 改成 `x`。所有修改和询问先给出（离线），按题实现 `add_value/remove_value/current_answer` 后调用。

```cpp
// 先按题目实现：add_value(x) 加一个数、remove_value(x) 删一个数、current_answer() 返回当前答案
vector<MoQueryUpdate> qs;                       // 收集询问 {l, r, t, id}：t = 之前已发生的修改数
vector<Modification> mods;                      // 收集修改 {pos, old_value, new_value}（旧值需记录）
vector<i64> ans = mo_with_updates(a, qs, mods); // 调用：a 必须 1-indexed
for (int i = 0; i < q; ++i) cout << ans[i] << '\n';
```

样例：`a=[1,2,1,3,2]`，先改 `a[2]=9`；问 `[1,3]` -> 2；`[2,5]` -> 4。

**传参要求（照这个传不会错）：**

- `a`：原数组，必须 1-indexed（长度 `n+1`，`a[0]` 不参与）。
- `qs`：询问数组，元素 `MoQueryUpdate{l, r, t, id}`；`l/r` 为 1-indexed 闭区间；`t` = 本次询问前已应用的修改数（`0..mods.size()`）；`id` 为 0-based 原始编号。
- `mods`：修改数组，按时间顺序存 `{pos, old_value, new_value}`；`old_value` 必须自己记录（改前先取 `a[pos]`）。
- 返回值：`vector<i64>`，`ans[id]` 就是第 `id` 个询问的答案。
- 必须实现 `add_value(int x)` / `remove_value(int x)` / `current_answer()` 三个函数，签名不能改。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_value(int x)` / `remove_value(int x)` / `current_answer()` -> 三个函数按题目实现：加数、删数、取当前答案。

```cpp
struct Modification {
    int pos, old_value, new_value;
};

struct MoQueryUpdate {
    int l, r, t, id;
};

int block_len;

bool mo_cmp(const MoQueryUpdate& a, const MoQueryUpdate& b) {
    int al = a.l / block_len, bl = b.l / block_len;
    if (al != bl) return al < bl;
    int ar = a.r / block_len, br = b.r / block_len;
    if (ar != br) return ar < br;
    return a.t < b.t;
}

// add_value/remove_value/current_answer 按题目改。
void add_value(int x) {}
void remove_value(int x) {}
i64 current_answer() { return 0; }

vector<i64> mo_with_updates(vector<int> a, vector<MoQueryUpdate> qs, vector<Modification> mods) {
    int n = (int)a.size() - 1;
    block_len = max(1, (int)pow(n, 2.0 / 3.0));
    sort(qs.begin(), qs.end(), mo_cmp);
    vector<i64> ans(qs.size());
    int l = 1, r = 0, t = 0;
    auto apply = [&](int id) {
        auto& m = mods[id];
        if (l <= m.pos && m.pos <= r) {
            remove_value(a[m.pos]);
            add_value(m.new_value);
        }
        a[m.pos] = m.new_value;
    };
    auto undo = [&](int id) {
        auto& m = mods[id];
        if (l <= m.pos && m.pos <= r) {
            remove_value(a[m.pos]);
            add_value(m.old_value);
        }
        a[m.pos] = m.old_value;
    };
    for (auto q : qs) {
        while (t < q.t) apply(t++);
        while (t > q.t) undo(--t);
        while (l > q.l) add_value(a[--l]);
        while (r < q.r) add_value(a[++r]);
        while (l < q.l) remove_value(a[l++]);
        while (r > q.r) remove_value(a[r--]);
        ans[q.id] = current_answer();
    }
    return ans;
}
```
#### 树上莫队

> **赛时先看**
> - **题目信号**：树上多次路径查询，无修改或少修改；答案可通过点的出现奇偶维护。
> - **本质**：离线查询树上路径颜色种类、路径众数等可增删维护的信息。
> - **接法**：抄下 `struct TreeMo`：`solver.init(n, LOG)` 初始化，逐条 `solver.add_edge(u,v)`，`solver.build(root)` 后调用 `solver.solve(a, queries)` 返回答案数组（`a` 是 1-indexed 颜色数组）。
    > - **复杂度判定**：`O((n+q) sqrt n)` 次 toggle；每次 toggle 的代价取决于维护的信息。
    > - **维护的量**：欧拉序 `euler`（长度 2n）、`first/last`（进出时间戳）、倍增表 `up`、颜色计数 `cnt` 与当前不同数 `distinct`。
    > - **警告**：颜色值需 `>= 1` 或先离散化；`LOG` 需 `>= ceil(log2 n)+1`；`build` 必须在一棵连通树上执行；路径端点相同的询问答案为 1。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点一棵树，每个点有颜色 `a[i]`；`q` 次询问路径 `(u,v)` 上不同颜色的数量。

```cpp
TreeMo solver;                          // 1. 结构体定义：无参构造
solver.init(n, LOG);                    // 2. 初始化：LOG >= ceil(log2 n) + 1
for (int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    solver.add_edge(u, v);              // 3. 加树边
}
solver.build(1);                        // 4. 建欧拉序：以 1 为根
vector<pair<int, int>> queries;         // 5. 收集路径询问 {u, v}
for (int i = 0; i < q; ++i) {
    int u, v;
    cin >> u >> v;
    queries.push_back({u, v});
}
vector<int> ans = solver.solve(a, queries);  // 6. 调用：a 是 1-indexed 颜色数组
for (int i = 0; i < q; ++i) cout << ans[i] << '\n';  // 7. 按原始顺序输出
```

样例：`n=3`，边 `1-2`、`2-3`，颜色 `a=[0,1,2,1]`；问路径 `(2,3)` -> `2`（颜色 2、1）；问 `(1,3)` -> `2`（颜色 1、2、1）。

**传参要求（照这个传不会错）：**

- `TreeMo solver`：无参构造；然后 `init(n, LOG)`。
- `init(n, LOG)`：`n` = 点数（编号 `1..n`）；`LOG` 需 `>= ceil(log2 n) + 1`。
- `add_edge(u, v)`：加一条树边；`build(root)`：以 `root` 为根建欧拉序（默认 `root=1`）。
- `solve(a, queries)`：`a` 必须 1-indexed 颜色数组（`a[0]` 不参与，颜色值需 `>= 1`）；`queries` 每个元素是 `{u, v}` 路径两端点；返回 `vector<int>`，`ans[i]` 是第 `i` 条路径上不同颜色数，`u == v` 时答案为 `1`。


**改板时先认这几个量：**

- `depth`：深度。
- `up`：倍增祖先表。

```cpp
// 树上莫队：批量回答树上路径的“不同颜色数量”（SP10707 COT2 模型）。
// 用法：TreeMo solver; solver.init(n, LOG); solver.add_edge(u,v); ...; solver.build(root);
//       然后 solver.solve(a, queries) 返回答案数组，a 是 1-indexed 颜色数组。
struct TreeMo {
    int n = 0;
    vector<vector<int>> g;
    vector<int> depth, first, last, euler;
    vector<vector<int>> up;
    int block = 0;

    TreeMo(int n_ = 0, int LOG = 18) { init(n_, LOG); }

    void init(int n_, int LOG = 18) {
        n = n_;
        g.assign(n + 1, {});
        depth.assign(n + 1, 0);
        first.assign(n + 1, 0);
        last.assign(n + 1, 0);
        up.assign(LOG + 1, vector<int>(n + 1, 0));
        euler.clear();
    }

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int p) {
        up[0][u] = p;
        for (int j = 1; j < (int)up.size(); ++j) up[j][u] = up[j - 1][up[j - 1][u]];
        first[u] = (int)euler.size();
        euler.push_back(u);
        for (int v : g[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
        last[u] = (int)euler.size();
        euler.push_back(u);
    }

    void build(int root = 1) {
        euler.clear();
        dfs(root, 0);
        block = max(1, (int)(sqrt((long double)euler.size()) + 0.5));
    }

    int lca(int u, int v) const {
        if (depth[u] < depth[v]) swap(u, v);
        int d = depth[u] - depth[v];
        for (int j = 0; d; ++j, d >>= 1) if (d & 1) u = up[j][u];
        if (u == v) return u;
        for (int j = (int)up.size() - 1; j >= 0; --j) {
            if (up[j][u] != up[j][v]) u = up[j][u], v = up[j][v];
        }
        return up[0][u];
    }

    // a 是 1-indexed 颜色；返回第 i 个询问 (u_i, v_i) 路径上不同颜色数量。
    vector<int> solve(const vector<int>& a, const vector<pair<int, int>>& queries) const {
        struct MoQuery {
            int l, r, extra, id;
        };
        vector<MoQuery> qs;
        qs.reserve(queries.size());
        for (int id = 0; id < (int)queries.size(); ++id) {
            int u = queries[id].first, v = queries[id].second;
            if (first[u] > first[v]) swap(u, v);
            int w = lca(u, v);
            if (w == u) qs.push_back({first[u], first[v], 0, id});
            else qs.push_back({last[u], first[v], w, id});
        }
        sort(qs.begin(), qs.end(), [&](const MoQuery& x, const MoQuery& y) {
            int bx = x.l / block, by = y.l / block;
            if (bx != by) return bx < by;
            return (bx & 1) ? x.r > y.r : x.r < y.r;
        });

        int max_color = *max_element(a.begin(), a.end());
        vector<int> appear(n + 1, 0), cnt(max_color + 1, 0), ans(queries.size());
        int cur_l = 0, cur_r = -1, distinct = 0;
        auto toggle = [&](int pos) {
            int u = euler[pos], c = a[u];
            if (appear[u]) {
                if (--cnt[c] == 0) --distinct;
            } else {
                if (cnt[c]++ == 0) ++distinct;
            }
            appear[u] ^= 1;
        };
        auto move = [&](int l, int r) {
            while (cur_l > l) toggle(--cur_l);
            while (cur_r < r) toggle(++cur_r);
            while (cur_l < l) toggle(cur_l++);
            while (cur_r > r) toggle(cur_r--);
        };
        for (const MoQuery& q : qs) {
            move(q.l, q.r);
            ans[q.id] = distinct + (q.extra && !appear[q.extra] && cnt[a[q.extra]] == 0 ? 1 : 0);
        }
        return ans;
    }
};
```
#### 线段树：单点修改，区间最大值

> **赛时先看**
> - **题目信号**：动态修改数组某个位置，并反复问区间最大/最小/和。
> - **本质**：最基础线段树，先会这个再学懒标记。
> - **接法**：输入数组用 `vector<i64> a(n+1)`，然后 `seg.build(a)`；单点改成新值用 `seg.modify(pos,val)`；区间最大值用 `seg.query(l,r)`。如果要区间和，把 `max` 换成加法并把初始答案改成 `0`；如果要同时维护多个量，另写 `Node` 和 `merge`。
> - **复杂度判定**：`O(log n)`。
> - **维护的量**：`n`（长度）；`tr[p]`（线段树第 p 号节点区间的最大值，初始为 `LLONG_MIN/4`）。
> - **警告**：线段树下标常用 `1..n`；递归边界要统一。
> - **约定**：`a` 必须 1-indexed，且 `a.size() >= n + 1`；`modify(pos,val)` 把第 `pos` 个位置改成 `val`；`query(l,r)` 查询闭区间 `[l,r]` 的最大值。

**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，`q` 次操作。`op=1 pos val`：把 `a[pos]` 改成 `val`；`op=2 l r`：输出闭区间 `[l,r]` 的最大值。

```cpp
SegPointMax seg(n);
seg.build(a);                                  // 建树：a 必须 1-indexed（长度 n+1）
if (op == 1) { int pos; i64 val; cin >> pos >> val; seg.modify(pos, val); }  // 单点改值
else { int l, r; cin >> l >> r; cout << seg.query(l, r) << '\n'; }           // 区间最大值
```

样例：`a=[1,2,3,4,5]`；`modify(3,-7)` 后 `query(1,5)` -> 输出 `4`。

**传参要求（照这个传不会错）：**

- `SegPointMax seg(n)`：n 为元素个数，下标 1..n。
- `build(a)`：a 必须 1-indexed，且 `a.size() >= n + 1`；a[0] 不参与。
- `modify(pos, val)`：把第 pos 个位置改成 val；要求 `1 <= pos <= n`；无返回值。
- `query(l, r)`：闭区间 [l,r] 的最大值；要求 `1 <= l <= r <= n`；返回 `i64`。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const vector<i64>& a)` -> 完成建树或预处理
- `init(int n_)` -> 初始化/清空结构
- `modify(int pos, i64 val)` -> 单点/指定位置修改
- `query(int l, int r)` -> 查询 返回 `i64`。

```cpp
struct SegPointMax {
    int n;
    vector<i64> tr;

    SegPointMax(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        tr.assign(4 * n + 4, LLONG_MIN / 4);
    }

    void build(int p, int l, int r, const vector<i64>& a) {
        if (l == r) {
            tr[p] = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);
        tr[p] = max(tr[p << 1], tr[p << 1 | 1]);
    }

    void modify(int p, int l, int r, int pos, i64 val) {
        if (l == r) {
            tr[p] = val;
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) modify(p << 1, l, mid, pos, val);
        else modify(p << 1 | 1, mid + 1, r, pos, val);
        tr[p] = max(tr[p << 1], tr[p << 1 | 1]);
    }

    i64 query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];
        int mid = (l + r) >> 1;
        i64 ans = LLONG_MIN / 4;
        if (ql <= mid) ans = max(ans, query(p << 1, l, mid, ql, qr));
        if (qr > mid) ans = max(ans, query(p << 1 | 1, mid + 1, r, ql, qr));
        return ans;
    }

    void build(const vector<i64>& a) {
        // 外部接口：a 必须是 1-indexed，且 a.size() >= n + 1。
        assert((int)a.size() > n);
        build(1, 1, n, a);
    }

    void modify(int pos, i64 val) {
        // 外部接口：把原数组第 pos 个位置改成 val，pos 为 1-indexed。
        assert(1 <= pos && pos <= n);
        modify(1, 1, n, pos, val);
    }

    i64 query(int l, int r) {
        // 外部接口：查询原数组闭区间 [l,r] 的最大值。
        assert(1 <= l && l <= r && r <= n);
        return query(1, 1, n, l, r);
    }
};
```
#### 线段树典题：单点修改，区间最大子段和

> **赛时先看**
> - **题目信号**：动态单点修改；询问区间内“连续选一段”的最大收益/最大和；元素可为负数，空段通常不允许。
> - **本质**：数组某个位置会被赋新值，询问任意区间内非空连续子段的最大和。经典模型为 SPOJ GSS 系列。
> - **接法**：把“动态修改某个位置后，问区间内连续一段最大和”直接翻译成这个模板。建树写 `seg.build(a)`，修改写 `seg.point_assign(pos,value)`；询问输出 `seg.max_subarray(l,r)`，或者需要完整信息时看 `seg.query(l,r).best`。如果题目允许空段，最后答案再和 `0` 取最大。
> - **复杂度判定**：建树 `O(n)`，单点修改和区间查询 `O(log n)`。
> - **维护的量**：`n`（长度）；`tr[p]`（节点四元组 `sum/pref/suff/best`：区间和、最大前缀和、最大后缀和、最大非空子段和）。
> - **警告**：`best` 是非空子段，叶子四个值都等于元素；查询时不能用全 `0` 的空节点合并，否则全负区间会被误判为 `0`；代码只在确实相交的子树之间合并。
> - **约定**：`a` 必须 1-indexed，叶子直接用 `a[1..n]` 初始化；`query(l,r)` 返回闭区间 `[l,r]` 的四元组信息。

**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，`q` 次操作。`op=1 pos val`：把 `a[pos]` 赋成 `val`；`op=2 l r`：输出闭区间 `[l,r]` 内非空连续子段的最大和。

```cpp
SegMaxSubarray seg(n);
seg.build(a);                                  // 建树：a 必须 1-indexed（长度 n+1）
if (op == 1) { int pos; i64 val; cin >> pos >> val; seg.point_assign(pos, val); }  // 单点赋值
else { int l, r; cin >> l >> r; cout << seg.max_subarray(l, r) << '\n'; }           // 最大子段和
```

样例：`a=[-2,1,-3,4,-1,2,1,-5,4]`；`max_subarray(1,9)` -> 输出 `6`。

**传参要求（照这个传不会错）：**

- `SegMaxSubarray seg(n)`：n 为元素个数，下标 1..n。
- `build(a)`：a 必须 1-indexed，长度至少 n+1。
- `point_assign(pos, value)`：把第 pos 个位置赋值为 value；要求 `1 <= pos <= n`；无返回值。
- `max_subarray(l, r)`：返回 [l,r] 非空连续子段最大和（`i64`）；要求 `1 <= l <= r <= n`。
- `query(l, r)`：需要完整信息时用，返回四元组 `Node`，取 `.best` 即最大子段和；题目允许空段时答案再与 `0` 取 max。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const vector<i64>& a)` -> 完成建树或预处理
- `init(int n_)` -> 初始化/清空结构
- `merge(Node left, Node right)` -> 合并 返回 `Node`。
- `query(int l, int r)` -> 查询 返回 `Node`。
**改板时先认这几个量：**

- `tr`：树节点池（节点数组）。
- `sum`：区间和/计数和。

维护：每段保存总和 `sum`、最大前缀和 `pref`、最大后缀和 `suff`、最大子段和 `best`。合并两段时，跨中点的候选是 `left.suff + right.pref`。

```cpp
struct SegMaxSubarray {
    struct Node {
        i64 sum, pref, suff, best;
    };

    int n = 0;
    vector<Node> tr;

    SegMaxSubarray(int n_ = 0) { init(n_); }

    void init(int n_) {
        n = n_;
        tr.assign(4 * n + 4, {0, 0, 0, 0});
    }

    static Node merge(Node left, Node right) {
        return {
            left.sum + right.sum,
            max(left.pref, left.sum + right.pref),
            max(right.suff, right.sum + left.suff),
            max({left.best, right.best, left.suff + right.pref}),
        };
    }

    void build(int p, int l, int r, const vector<i64>& a) {
        if (l == r) {
            tr[p] = {a[l], a[l], a[l], a[l]};
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);
        tr[p] = merge(tr[p << 1], tr[p << 1 | 1]);
    }

    void point_assign(int p, int l, int r, int pos, i64 value) {
        if (l == r) {
            tr[p] = {value, value, value, value};
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) point_assign(p << 1, l, mid, pos, value);
        else point_assign(p << 1 | 1, mid + 1, r, pos, value);
        tr[p] = merge(tr[p << 1], tr[p << 1 | 1]);
    }

    Node query(int p, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr) return tr[p];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(p << 1, l, mid, ql, qr);
        if (ql > mid) return query(p << 1 | 1, mid + 1, r, ql, qr);
        return merge(
            query(p << 1, l, mid, ql, qr),
            query(p << 1 | 1, mid + 1, r, ql, qr)
        );
    }

    void build(const vector<i64>& a) {
        // 外部接口：a 必须是 1-indexed；叶子直接用 a[1..n] 初始化。
        assert((int)a.size() > n);
        build(1, 1, n, a);
    }

    void point_assign(int pos, i64 value) {
        // 外部接口：把原数组第 pos 个位置赋值为 value。
        assert(1 <= pos && pos <= n);
        point_assign(1, 1, n, pos, value);
    }

    Node query(int l, int r) const {
        // 外部接口：返回原数组闭区间 [l,r] 的四元组信息。
        assert(1 <= l && l <= r && r <= n);
        return query(1, 1, n, l, r);
    }

    i64 max_subarray(int l, int r) const {
        // 外部接口：直接返回 [l,r] 内非空连续子段最大和。
        return query(l, r).best;
    }
};
```

典题模型：输入数组用 1-indexed `vector<i64> a(n+1)`，`seg.build(a)`；修改 `seg.point_assign(pos,value)`；询问 `[l,r]` 输出 `seg.max_subarray(l,r)`。
#### 线段树：区间加，区间和/最大值

> **赛时先看**
> - **题目信号**：`add l r x` 后继续查询区间和、区间最大值。
> - **本质**：区间修改和区间查询。
> - **接法**：`seg.range_add(l,r,x)` 表示给一整段加值；`seg.query_sum(l,r)` 问区间和，`seg.query_max(l,r)` 问区间最大值。内部每次递归往下走前必须 `push`，每次子树改完必须 `pull`；如果题目是区间赋值，不要把加法 lazy 改成赋值 lazy，直接翻下一段区间赋值模板。
> - **复杂度判定**：`O(log n)`。
> - **维护的量**：`n`（长度）；`tr[p]`（节点存 `sum` 区间和、`mx` 区间最大值、`lazy` 懒标记加值）。
> - **警告**：懒标记下传时，区间和要乘长度。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，`q` 次操作。`op=1 l r x`：把 `a[l..r]` 全部加 `x`；`op=2 l r`：输出区间和；`op=3 l r`：输出区间最大值。

```cpp
SegLazy seg;
seg.build(a);                                  // 1. 结构体定义 + 建树：a 必须 1-indexed（长度 n+1）
while (q--) {
    int op, l, r;
    cin >> op >> l >> r;
    if (op == 1) { i64 x; cin >> x; seg.range_add(l, r, x); }        // 区间加
    else if (op == 2) cout << seg.query_sum(l, r) << '\n';            // 区间和
    else cout << seg.query_max(l, r) << '\n';                         // 区间最大值
}
```

样例：`a=[1,2,3,4,5]`；`add(2,4,+10)` 后 `query_sum(1,5)` -> 输出 `45`；`query_max(1,5)` -> 输出 `14`。

**传参要求（照这个传不会错）：**

- `SegLazy seg; seg.build(a)`：构造后先建树；`a` 必须 1-indexed（`a[0]` 不参与，长度 `n+1`）。
- `range_add(l, r, val)`：闭区间 `[l,r]` 全部加 `val`；要求 `1 <= l <= r <= n`。
- `query_sum(l, r)` / `query_max(l, r)`：闭区间 `[l,r]` 的和 / 最大值；要求 `1 <= l <= r <= n`。
- 返回 `i64`；建完树才能查询（查询初值也走 build 后的线段树）。
> - **约定**：`a` 必须 1-indexed，建完后才能查询初值；`range_add(l,r,val)` 给闭区间 `[l,r]` 全部加 `val`；`query_sum(l,r)` 查询闭区间和。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const vector<i64>& a)` -> 完成建树或预处理
- `init(int n_)` -> 初始化/清空结构
- `query_max(int l, int r)` -> 查询闭区间最大值 返回 `i64`。
- `query_sum(int l, int r)` -> 查询闭区间和 返回 `i64`。
- `range_add(int l, int r, i64 val)` -> 闭区间加值
**核心逻辑（改代码时别破坏）：**

- 节点同时存 `sum/mx/lazy`；整段命中时只改当前节点和 lazy。
- 下探前 `push`，子树变化后 `pull`；区间和加 `val` 时必须乘区间长度。
**改板时先认这几个量：**

- `tr`：树节点池（节点数组）。
- `lazy`：懒标记。
- `sum`：区间和/计数和。
- `mx`：区间最大值。

```cpp
struct SegLazy {
    struct Node {
        i64 sum = 0;
        i64 mx = LLONG_MIN / 4;
        i64 lazy = 0;
    };

    int n;
    vector<Node> tr;

    SegLazy(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        tr.assign(4 * n + 4, {});
    }

    void pull(int p) {
        tr[p].sum = tr[p << 1].sum + tr[p << 1 | 1].sum;
        tr[p].mx = max(tr[p << 1].mx, tr[p << 1 | 1].mx);
    }

    void apply(int p, int l, int r, i64 val) {
        tr[p].sum += val * (r - l + 1);
        tr[p].mx += val;
        tr[p].lazy += val;
    }

    void push(int p, int l, int r) {
        if (tr[p].lazy == 0 || l == r) return;
        int mid = (l + r) >> 1;
        apply(p << 1, l, mid, tr[p].lazy);
        apply(p << 1 | 1, mid + 1, r, tr[p].lazy);
        tr[p].lazy = 0;
    }

    void build(int p, int l, int r, const vector<i64>& a) {
        if (l == r) {
            tr[p].sum = tr[p].mx = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);
        pull(p);
    }

    void range_add(int p, int l, int r, int ql, int qr, i64 val) {
        if (ql <= l && r <= qr) {
            apply(p, l, r, val);
            return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) range_add(p << 1, l, mid, ql, qr, val);
        if (qr > mid) range_add(p << 1 | 1, mid + 1, r, ql, qr, val);
        pull(p);
    }

    i64 query_sum(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p].sum;
        push(p, l, r);
        int mid = (l + r) >> 1;
        i64 ans = 0;
        if (ql <= mid) ans += query_sum(p << 1, l, mid, ql, qr);
        if (qr > mid) ans += query_sum(p << 1 | 1, mid + 1, r, ql, qr);
        return ans;
    }

    i64 query_max(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p].mx;
        push(p, l, r);
        int mid = (l + r) >> 1;
        i64 ans = LLONG_MIN / 4;
        if (ql <= mid) ans = max(ans, query_max(p << 1, l, mid, ql, qr));
        if (qr > mid) ans = max(ans, query_max(p << 1 | 1, mid + 1, r, ql, qr));
        return ans;
    }

    void build(const vector<i64>& a) {
        // 外部接口：a 必须是 1-indexed，建完后才能查询初值。
        assert((int)a.size() > n);
        build(1, 1, n, a);
    }

    void range_add(int l, int r, i64 val) {
        // 外部接口：给原数组闭区间 [l,r] 全部加 val。
        assert(1 <= l && l <= r && r <= n);
        range_add(1, 1, n, l, r, val);
    }

    i64 query_sum(int l, int r) {
        // 外部接口：查询原数组闭区间 [l,r] 的区间和。
        assert(1 <= l && l <= r && r <= n);
        return query_sum(1, 1, n, l, r);
    }

    i64 query_max(int l, int r) {
        // 外部接口：查询原数组闭区间 [l,r] 的最大值。
        assert(1 <= l && l <= r && r <= n);
        return query_max(1, 1, n, l, r);
    }
};
```
#### 线段树：区间赋值，区间和

> **赛时先看**
> - **题目信号**：操作语义是 `a[i]=value (l<=i<=r)`，不是加法；之后问区间总和、平均值或覆盖长度。
> - **本质**：反复把 `[l,r]` 全部改成同一个数，并查询区间和；是“区间染色/覆盖”转为数值统计后的通用骨架。
> - **接法**：数列初值 `a[1..n]`，操作 `C l r x` 表示覆盖，`Q l r` 问和。建树后分别调用 `seg.range_assign(l,r,x)`、`seg.query_sum(l,r)`；若题面一开始全为同一个值，可不建树，先 `init(n)` 后调用 `seg.range_assign(1,n,initial)`。
> - **复杂度判定**：建树 `O(n)`，区间赋值和区间和 `O(log n)`。
> - **维护的量**：`n`（长度）；`tr[p]`（`sum` 区间和、`assign_value` 覆盖值、`has_assign` 是否有覆盖懒标记）。
> - **警告**：赋值懒标记不能用 `0` 表示“无标记”，因为合法赋值可能正好为 `0`；要额外存 `has_assign`。若同时有区间加和赋值，必须规定二者组合顺序，建议直接翻仿射懒标记线段树。
> - **约定**：`a` 必须 1-indexed，初值来自 `a[1..n]`；`range_assign(l,r,value)` 把闭区间 `[l,r]` 全部赋值；`query_sum(l,r)` 查询闭区间和。

**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，`q` 次操作。`op=1 l r x`：把 `a[l..r]` 全部赋成 `x`；`op=2 l r`：输出闭区间和。

```cpp
SegRangeAssignSum seg(n);
seg.build(a);                                  // 建树：a 必须 1-indexed（长度 n+1）
if (op == 1) { i64 x; cin >> x; seg.range_assign(l, r, x); }  // 区间赋值
else cout << seg.query_sum(l, r) << '\n';                      // 区间和
```

样例：`a=[1,2,3,4,5]`；`range_assign(2,4,10)` 后 `query_sum(1,5)` -> 输出 `36`。

**传参要求（照这个传不会错）：**

- `SegRangeAssignSum seg(n)`：n 为元素个数，下标 1..n。
- `build(a)`：a 必须 1-indexed，长度至少 n+1；若整段初值相同，可只 `init(n)` 后 `range_assign(1,n,initial)`，不建树。
- `range_assign(l, r, value)`：闭区间 [l,r] 全部赋成 value；要求 `1 <= l <= r <= n`；无返回值。
- `query_sum(l, r)`：闭区间 [l,r] 的区间和；要求 `1 <= l <= r <= n`；返回 `i64`。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const vector<i64>& a)` -> 完成建树或预处理
- `init(int n_)` -> 初始化/清空结构
- `query_sum(int l, int r)` -> 查询闭区间和 返回 `i64`。
**改板时先认这几个量：**

- `tr`：树节点池（节点数组）。
- `sum`：区间和/计数和。

```cpp
struct SegRangeAssignSum {
    struct Node {
        i64 sum = 0;
        i64 assign_value = 0;
        bool has_assign = false;
    };

    int n = 0;
    vector<Node> tr;

    SegRangeAssignSum(int n_ = 0) { init(n_); }

    void init(int n_) {
        n = n_;
        tr.assign(4 * n + 4, {});
    }

    void apply_assign(int p, int l, int r, i64 value) {
        tr[p].sum = value * (r - l + 1);
        tr[p].assign_value = value;
        tr[p].has_assign = true;
    }

    void push(int p, int l, int r) {
        if (!tr[p].has_assign || l == r) return;
        int mid = (l + r) >> 1;
        apply_assign(p << 1, l, mid, tr[p].assign_value);
        apply_assign(p << 1 | 1, mid + 1, r, tr[p].assign_value);
        tr[p].has_assign = false;
    }

    void build(int p, int l, int r, const vector<i64>& a) {
        if (l == r) {
            tr[p].sum = a[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, a);
        build(p << 1 | 1, mid + 1, r, a);
        tr[p].sum = tr[p << 1].sum + tr[p << 1 | 1].sum;
    }

    void range_assign(int p, int l, int r, int ql, int qr, i64 value) {
        if (ql <= l && r <= qr) {
            apply_assign(p, l, r, value);
            return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        if (ql <= mid) range_assign(p << 1, l, mid, ql, qr, value);
        if (qr > mid) range_assign(p << 1 | 1, mid + 1, r, ql, qr, value);
        tr[p].sum = tr[p << 1].sum + tr[p << 1 | 1].sum;
    }

    i64 query_sum(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p].sum;
        push(p, l, r);
        int mid = (l + r) >> 1;
        i64 answer = 0;
        if (ql <= mid) answer += query_sum(p << 1, l, mid, ql, qr);
        if (qr > mid) answer += query_sum(p << 1 | 1, mid + 1, r, ql, qr);
        return answer;
    }

    void build(const vector<i64>& a) {
        // 外部接口：a 必须是 1-indexed，初值来自 a[1..n]。
        assert((int)a.size() > n);
        build(1, 1, n, a);
    }

    void range_assign(int l, int r, i64 value) {
        // 外部接口：把原数组闭区间 [l,r] 全部赋值为 value。
        assert(1 <= l && l <= r && r <= n);
        range_assign(1, 1, n, l, r, value);
    }

    i64 query_sum(int l, int r) {
        // 外部接口：查询原数组闭区间 [l,r] 的区间和。
        assert(1 <= l && l <= r && r <= n);
        return query_sum(1, 1, n, l, r);
    }
};
```

#### 仿射懒标记线段树：区间乘、区间加、区间和

> **赛时先看**
> - **题目信号**：同一段元素统一乘一个数、加一个数，再问区间和；本质是区间上的仿射函数复合。
> - **本质**：维护 `a[i] = a[i] * mul + add (mod MOD)`，并查询区间和。AtCoder Library Practice Contest 的 K 题就是这一类标准模型。
> - **接法**：构造时直接传数组和模数 `AffineSegTree seg(a, MOD)`（自动建树）；区间修改 `seg.range_apply(l,r,mul,add)`；区间和 `seg.range_sum(l,r)`。
> - **复杂度判定**：建树 `O(n)`，每次修改/查询 `O(log n)`。
> - **维护的量**：`n`（长度）、`mod`（模数）；`tr[p]`（`sum` 区间和、`mul/add` 仿射懒标记：区间内每个数先乘 `mul` 再加 `add`，均在模意义下）。
> - **警告**：新操作在旧操作之后执行，所以懒标记是 `old -> new`：`mul = old_mul * new_mul`，`add = old_add * new_mul + new_add`。
> - **约定**：n = (int)a.size() - 1; // a 为 1-indexed

**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数（模 `MOD`），`q` 次操作。`op=1 l r mul add`：`a[l..r]` 每个数先乘 `mul` 再加 `add`；`op=2 l r`：输出闭区间和 mod `MOD`。

```cpp
AffineSegTree seg(a, MOD);                     // 构造即建树：a 必须 1-indexed（长度 n+1）
if (op == 1) { i64 mul, add; cin >> mul >> add; seg.range_apply(l, r, mul, add); }  // 区间仿射变换
else cout << seg.range_sum(l, r) << '\n';                                              // 区间和
```

样例：`a=[1,2,3,4,5]`，MOD=998244353；`range_apply(2,4,3,1)` 后 `range_sum(1,5)` -> 输出 `36`。

**传参要求（照这个传不会错）：**

- `AffineSegTree seg(a, MOD)`：构造时自动建树；a 必须 1-indexed，`n = a.size() - 1`；`MOD` 为模数（须为 `i64`）。
- `range_apply(l, r, mul, add)`：闭区间 [l,r] 每个数先乘 mul 再加 add（自动取模）；要求 `1 <= l <= r <= n`；无返回值。
- `range_sum(l, r)`：闭区间 [l,r] 的和 mod MOD；要求 `1 <= l <= r <= n`；返回 `i64`。
- 只区间加：传 `range_apply(l, r, 1, x)`；只区间乘：传 `range_apply(l, r, x, 0)`。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const vector<i64>& a)` -> 完成建树或预处理
- `range_apply(int l, int r, i64 mul, i64 add)` -> 区间先乘 mul 再加 add
- `range_sum(int l, int r)` -> 查询闭区间和 返回 `i64`。
**改板时先认这几个量：**

- `tr`：树节点池（节点数组）。
- `sum`：区间和/计数和。

典题模型：AtCoder Library Practice Contest K - Range Affine Range Sum。

```cpp
struct AffineSegTree {
    struct Node {
        i64 sum = 0;
        i64 mul = 1, add = 0;
    };
    int n;
    i64 mod;
    vector<Node> tr;

    AffineSegTree(const vector<i64>& a, i64 mod_) : mod(mod_) {
        n = (int)a.size() - 1; // a 为 1-indexed
        tr.assign(4 * n + 4, Node{});
        build_impl(1, 1, n, a);
    }

    void build_impl(int p, int l, int r, const vector<i64>& a) {
        if (l == r) {
            tr[p].sum = (a[l] % mod + mod) % mod;
            return;
        }
        int mid = (l + r) >> 1;
        build_impl(p << 1, l, mid, a);
        build_impl(p << 1 | 1, mid + 1, r, a);
        pull(p);
    }

    void pull(int p) {
        tr[p].sum = (tr[p << 1].sum + tr[p << 1 | 1].sum) % mod;
    }

    void apply(int p, int len, i64 mul, i64 add) {
        tr[p].sum = (tr[p].sum * mul + add * len) % mod;
        tr[p].mul = tr[p].mul * mul % mod;
        tr[p].add = (tr[p].add * mul + add) % mod;
    }

    void push(int p, int l, int r) {
        if (l == r || (tr[p].mul == 1 && tr[p].add == 0)) return;
        int mid = (l + r) >> 1;
        apply(p << 1, mid - l + 1, tr[p].mul, tr[p].add);
        apply(p << 1 | 1, r - mid, tr[p].mul, tr[p].add);
        tr[p].mul = 1;
        tr[p].add = 0;
    }

    void range_apply_impl(int p, int l, int r, int ql, int qr, i64 mul, i64 add) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(p, r - l + 1, (mul % mod + mod) % mod, (add % mod + mod) % mod);
            return;
        }
        push(p, l, r);
        int mid = (l + r) >> 1;
        range_apply_impl(p << 1, l, mid, ql, qr, mul, add);
        range_apply_impl(p << 1 | 1, mid + 1, r, ql, qr, mul, add);
        pull(p);
    }

    i64 range_sum_impl(int p, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return tr[p].sum;
        push(p, l, r);
        int mid = (l + r) >> 1;
        return (range_sum_impl(p << 1, l, mid, ql, qr)
              + range_sum_impl(p << 1 | 1, mid + 1, r, ql, qr)) % mod;
    }

    void build(const vector<i64>& a) { build_impl(1, 1, n, a); }
    void range_apply(int l, int r, i64 mul, i64 add) { range_apply_impl(1, 1, n, l, r, mul, add); }
    i64 range_sum(int l, int r) { return range_sum_impl(1, 1, n, l, r); }
};

// 示例：seg.range_apply(l, r, multiplier, increment);
// 示例：cout << seg.range_sum(l, r) << '\n';
```
#### 区间染色 + 查询区间颜色种类数

> **赛时先看**
> - **题目信号**：颜色数量较少，通常 `color <= 30/60`；区间赋值；查询颜色种类数。经典模型类似 POJ 2777 Count Color。
> - **本质**：维护一条长度为 `n` 的线段，操作包括把 `[l,r]` 全部染成颜色 `c`，查询 `[l,r]` 内出现了多少种颜色。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：每次操作 `O(log n)`，统计颜色数 `O（颜色位数）`。
> - **维护的量**：`n`（长度）；`mask[p]`（p 号节点区间内出现颜色的按位或结果）、`lazy[p]`（整段染色的懒标记位掩码）。
> - **警告**：这是“区间赋值”不是区间加；颜色从 1 开始时用 `1ULL << (c-1)`；颜色数超过 60 时不能用 `ui64`，要换 `bitset` 或其它做法。

**最小完整示例（先抄这一段就能跑）：**

题目：`n` 段初始全为颜色 1，`q` 次操作。`op=1 l r c`：把 `[l,r]` 全部染成颜色 `c`；`op=2 l r`：输出 `[l,r]` 内的颜色种类数。

```cpp
ColorSegTree seg(n);                           // 初始整段颜色为 1
if (op == 1) { int c; cin >> c; seg.range_assign(l, r, c); }   // 区间染色
else cout << seg.query_count(l, r) << '\n';                     // 颜色种类数
```

样例：`n=5` 初始全 1；`range_assign(2,4,3)` 后 `query_count(1,5)` -> 输出 `2`。

**传参要求（照这个传不会错）：**

- `ColorSegTree seg(n)`：n 为长度，下标 1..n；初始整段颜色为 1。
- `range_assign(l, r, color)`：闭区间 [l,r] 全部染成颜色 color（颜色编号从 1 开始）；要求 `1 <= l <= r <= n`；无返回值。
- `query_count(l, r)`：闭区间 [l,r] 内的颜色种类数；要求 `1 <= l <= r <= n`；返回 `int`。
- 颜色总数必须 ≤ 60（`ui64` 位掩码能装下）。


**不会用就照抄：**

```cpp
ColorSegTree seg(n);
seg.range_assign(l, r, c);      // 把闭区间 [l,r] 全部染成颜色 c
int kinds = seg.query_count(l, r); // 查询闭区间 [l,r] 内的颜色种类数
```

- `range_assign / query_count` 的 `l/r` 都是 1-based 闭区间；颜色编号从 1 开始。


**API / 入口函数（赛时只认这里列的名字）：**

- `init(int n_)` -> 初始化/清空结构
- `query_count(int l, int r)` -> 查询闭区间 [l,r] 内的颜色种类数 返回 `int`。
- `range_assign(int l, int r, int color)` -> 把闭区间 [l,r] 全部染成颜色 color

```cpp
struct ColorSegTree {
    int n;
    vector<ui64> mask, lazy;

    ColorSegTree(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        mask.assign(4 * n + 4, 1ULL); // 初始颜色为 1。
        lazy.assign(4 * n + 4, 1ULL);
    }

    ui64 color_bit(int c) {
        return 1ULL << (c - 1);
    }

    void apply(int p, ui64 v) {
        mask[p] = v;
        lazy[p] = v;
    }

    void push(int p) {
        if (lazy[p]) {
            apply(p << 1, lazy[p]);
            apply(p << 1 | 1, lazy[p]);
            lazy[p] = 0;
        }
    }

    void range_assign_impl(int p, int l, int r, int ql, int qr, int color) {
        if (ql <= l && r <= qr) {
            apply(p, color_bit(color));
            return;
        }
        push(p);
        int mid = (l + r) >> 1;
        if (ql <= mid) range_assign_impl(p << 1, l, mid, ql, qr, color);
        if (qr > mid) range_assign_impl(p << 1 | 1, mid + 1, r, ql, qr, color);
        mask[p] = mask[p << 1] | mask[p << 1 | 1];
    }

    ui64 query_mask(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return mask[p];
        push(p);
        int mid = (l + r) >> 1;
        ui64 res = 0;
        if (ql <= mid) res |= query_mask(p << 1, l, mid, ql, qr);
        if (qr > mid) res |= query_mask(p << 1 | 1, mid + 1, r, ql, qr);
        return res;
    }

    void range_assign(int l, int r, int color) { range_assign_impl(1, 1, n, l, r, color); }

    int query_count(int l, int r) {
        return __builtin_popcountll(query_mask(1, 1, n, l, r));
    }
};
```
#### 线段树 Beats：区间 chmin + 区间和

> **赛时先看**
> - **题目信号**：操作是区间取 min/max，普通懒标记不够。
> - **本质**：支持 `a[i] = min(a[i], x)` 和区间和查询。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：均摊 `O(log n)`。
> - **维护的量**：`n`（长度）；`tr[p]`（`sum` 区间和、`mx1` 最大值、`mx2` 严格次大值、`cnt_mx` 最大值个数）。
> - **警告**：维护最大值 `mx1`、严格次大值 `mx2`、最大值个数 `cnt_mx`；只有 `mx2 < x < mx1` 才能整段打标记。

**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，`q` 次操作。`op=1 l r x`：`a[l..r]` 每个数与 `x` 取 min；`op=2 l r`：输出闭区间和。

```cpp
SegBeatsChminSum seg(n);
seg.build(a);                                  // 建树：a 必须 1-indexed（长度 n+1）
if (op == 1) { i64 x; cin >> x; seg.range_chmin(l, r, x); }  // 区间取 min
else cout << seg.query_sum(l, r) << '\n';                      // 区间和
```

样例：`a=[1,2,3,4,5]`；`range_chmin(1,5,3)` 后 `query_sum(1,5)` -> 输出 `12`。

**传参要求（照这个传不会错）：**

- `SegBeatsChminSum seg(n)`：n 为元素个数，下标 1..n。
- `build(a)`：a 必须 1-indexed，长度至少 n+1。
- `range_chmin(l, r, x)`：闭区间 [l,r] 内每个数变成 `min(a[i], x)`；要求 `1 <= l <= r <= n`；无返回值。
- `query_sum(l, r)`：闭区间 [l,r] 的和；要求 `1 <= l <= r <= n`；返回 `i64`。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const vector<i64>& a)` -> 完成建树或预处理
- `range_chmin(int l, int r, i64 x)` -> 对闭区间 [l,r] 取 min(x, a[i])
- `query_sum(int l, int r)` -> 查询闭区间和 返回 `i64`。
**改板时先认这几个量：**

- `tr`：树节点池（节点数组）。
- `sum`：区间和/计数和。

```cpp
struct SegBeatsChminSum {
    struct Node {
        i64 sum = 0;
        i64 mx1 = LLONG_MIN / 4;
        i64 mx2 = LLONG_MIN / 4;
        int cnt_mx = 0;
    };

    int n;
    vector<Node> tr;

    SegBeatsChminSum(int n = 0) { if (n) init(n); }
    void init(int n_) { n = n_; tr.assign(4 * n + 4, {}); }

    Node merge(Node a, Node b) {
        Node c;
        c.sum = a.sum + b.sum;
        if (a.mx1 == b.mx1) {
            c.mx1 = a.mx1;
            c.cnt_mx = a.cnt_mx + b.cnt_mx;
            c.mx2 = max(a.mx2, b.mx2);
        } else if (a.mx1 > b.mx1) {
            c.mx1 = a.mx1;
            c.cnt_mx = a.cnt_mx;
            c.mx2 = max(a.mx2, b.mx1);
        } else {
            c.mx1 = b.mx1;
            c.cnt_mx = b.cnt_mx;
            c.mx2 = max(a.mx1, b.mx2);
        }
        return c;
    }

    void build_impl(int p, int l, int r, const vector<i64>& a) {
        if (l == r) {
            tr[p].sum = tr[p].mx1 = a[l];
            tr[p].mx2 = LLONG_MIN / 4;
            tr[p].cnt_mx = 1;
            return;
        }
        int mid = (l + r) >> 1;
        build_impl(p << 1, l, mid, a);
        build_impl(p << 1 | 1, mid + 1, r, a);
        tr[p] = merge(tr[p << 1], tr[p << 1 | 1]);
    }

    void apply_chmin(int p, i64 x) {
        if (x >= tr[p].mx1) return;
        tr[p].sum -= (tr[p].mx1 - x) * tr[p].cnt_mx;
        tr[p].mx1 = x;
    }

    void push(int p) {
        apply_chmin(p << 1, tr[p].mx1);
        apply_chmin(p << 1 | 1, tr[p].mx1);
    }

    void range_chmin_impl(int p, int l, int r, int ql, int qr, i64 x) {
        if (qr < l || r < ql || x >= tr[p].mx1) return;
        if (ql <= l && r <= qr && x > tr[p].mx2) {
            apply_chmin(p, x);
            return;
        }
        push(p);
        int mid = (l + r) >> 1;
        range_chmin_impl(p << 1, l, mid, ql, qr, x);
        range_chmin_impl(p << 1 | 1, mid + 1, r, ql, qr, x);
        tr[p] = merge(tr[p << 1], tr[p << 1 | 1]);
    }

    i64 query_sum_impl(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p].sum;
        push(p);
        int mid = (l + r) >> 1;
        i64 ans = 0;
        if (ql <= mid) ans += query_sum_impl(p << 1, l, mid, ql, qr);
        if (qr > mid) ans += query_sum_impl(p << 1 | 1, mid + 1, r, ql, qr);
        return ans;
    }

    void build(const vector<i64>& a) { build_impl(1, 1, n, a); }
    void range_chmin(int l, int r, i64 x) { range_chmin_impl(1, 1, n, l, r, x); }
    i64 query_sum(int l, int r) { return query_sum_impl(1, 1, n, l, r); }
};
```
#### 动态开点线段树：巨大值域单点加、区间和

> **赛时先看**
> - **题目信号**：动态点权、值域极大、离线压缩不方便或不能压缩；常与主席树、线段树合并、扫描线结合。
> - **本质**：坐标范围可到 `1e9`、`1e18`，但实际修改点不多时，动态维护频率或权值和。
> - **接法**：在线插入整数，查询某个数值区间内已有元素的个数或权值和。
> - **复杂度判定**：每次修改/查询 `O(log V)`，空间为实际访问节点数 `O(q log V)`。
> - **维护的量**：`tr`（动态节点池，0 号为空节点）；每节点 `lc/rc`（左右儿子编号）、`sum`（该值域区间累计和）。
> - **警告**：`mid = (l+r)/2` 可能溢出，应写成 `l + (r-l)/2`；值域和答案常需要 `i64`。

**最小完整示例（先抄这一段就能跑）：**

题目：值域 `[-1e9, 1e9]` 上在线插入整数，查询某个值域闭区间内已有元素的累计和。

```cpp
DynamicSegTree seg;
int root = 0;
seg.add(root, -1000000000LL, 1000000000LL, x, +1);          // 位置 x 上加 delta（可为负）
i64 cnt = seg.query(root, -1000000000LL, 1000000000LL, l, r); // 值域闭区间 [l,r] 的累计和
```

样例：依次对 `x=5`、`x=3` 各 `add +1` 后，`query(root,-1e9,1e9,-100,10)` -> 输出 `2`。

**传参要求（照这个传不会错）：**

- `DynamicSegTree seg`：无参构造；`root` 初始为 0，之后始终传同一个 root。
- `add(p, l, r, pos, delta)`：值域闭区间 [l,r] 内位置 pos 累计加 delta；p 按引用传（root 会被更新）；要求 `l <= pos <= r`；无返回值。
- `query(p, l, r, ql, qr)`：值域闭区间 [ql,qr] 的累计和；要求 `l <= ql <= qr <= r`；返回 `i64`。
- 整套 `(l,r)` 范围每次调用必须一致（同一棵树同一个值域）；值域大时用 `i64` 并注意 mid 防溢出。


**API / 入口函数（赛时只认这里列的名字）：**

- `add(int& p, i64 l, i64 r, i64 pos, i64 delta)` -> 加入一个元素/贡献
- `query(int p, i64 l, i64 r, i64 ql, i64 qr)` -> 查询 返回 `i64`。
**改板时先认这几个量：**

- `tr`：树节点池（节点数组）。
- `root`：当前根节点编号（add/query 的第一个参数）。
- `sum`：区间和/计数和。

```cpp
struct DynamicSegTree {
    struct Node {
        int lc = 0, rc = 0;
        i64 sum = 0;
    };
    vector<Node> tr{Node{}}; // 0 号为空节点

    int new_node() {
        tr.push_back(Node{});
        return (int)tr.size() - 1;
    }

    void add(int& p, i64 l, i64 r, i64 pos, i64 delta) {
        if (!p) p = new_node();
        tr[p].sum += delta;
        if (l == r) return;
        i64 mid = l + (r - l) / 2;
        // 不能把 tr[p].lc / rc 直接按引用传进递归：push_back 扩容会使引用失效。
        if (pos <= mid) {
            int child = tr[p].lc;
            add(child, l, mid, pos, delta);
            tr[p].lc = child;
        } else {
            int child = tr[p].rc;
            add(child, mid + 1, r, pos, delta);
            tr[p].rc = child;
        }
    }

    i64 query(int p, i64 l, i64 r, i64 ql, i64 qr) const {
        if (!p || qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return tr[p].sum;
        i64 mid = l + (r - l) / 2;
        return query(tr[p].lc, l, mid, ql, qr)
             + query(tr[p].rc, mid + 1, r, ql, qr);
    }
};

// 示例：int root = 0;
// 示例：seg.add(root, -1000000000LL, 1000000000LL, position, +1);
```
#### 线段树合并

> **赛时先看**
> - **题目信号**：每个子树/集合都有一棵权值线段树，DFS 中把儿子信息合到父亲。
> - **本质**：树上每个节点维护一个值域线段树，合并子树信息，如颜色众数、动态开点频率统计。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：总合并复杂度约 `O（总节点数 log V)`，实际开销正比于被合并到的节点数。
> - **维护的量**：`tr`（动态节点池，0 号为空节点）；每节点 `lc/rc`（左右儿子编号）、`sum`（该值域区间累计和）。
> - **警告**：合并会破坏被合并的树；如果后续还要用旧版本，必须可持久化或复制。

**最小完整示例（先抄这一段就能跑）：**

题目：树上每个点一棵权值线段树，DFS 回溯时把儿子树合并进父亲，最后每个点都能拿到子树内的累计信息。

```cpp
MergeSegTree seg;
int root[MAXN] = {};                           // root[u]：u 那棵树的根，初始为 0
seg.add(root[u], 1, n, val[u], +1);            // 把 u 的权值 val[u] 插进 u 自己的树
root[u] = seg.merge(root[u], root[v]);         // 回溯时把儿子 v 的树并进 u，返回新根（v 的树被销毁）
i64 total = seg.tr[root[u]].sum;               // 合并后 u 子树内的累计和
```

样例：两棵树各 `add` 一次 `+1` 后 `merge`，`seg.tr[root].sum` -> 输出 `2`。

**传参要求（照这个传不会错）：**

- `MergeSegTree seg`：无参构造；每棵树的根初始为 0，值域闭区间 `(l,r)` 全树统一。
- `add(p, l, r, pos, v)`：值域闭区间 [l,r] 内位置 pos 累计加 v；p 按引用传（根会变）；要求 `l <= pos <= r`；无返回值。
- `merge(x, y)`：把树 y 合并进树 x，返回新根（通常直接 `root[u] = merge(root[u], root[v])`）；要求 x、y 均为树根编号；合并后 y 的内容被破坏，不能再单独使用。


**API / 入口函数（赛时只认这里列的名字）：**

- `merge(int x, int y)` -> 合并 返回 `int`。
- `add(int& p, int l, int r, int pos, i64 v)` -> 加入一个元素/贡献
**改板时先认这几个量：**

- `tr`：树节点池（节点数组）。
- `sum`：区间和/计数和。

```cpp
struct MergeSegTree {
    struct Node { int lc = 0, rc = 0; i64 sum = 0; };
    vector<Node> tr{{}};

    int new_node() {
        tr.push_back(Node{});
        return (int)tr.size() - 1;
    }

    void add(int& p, int l, int r, int pos, i64 v) {
        if (!p) p = new_node();
        if (l == r) {
            tr[p].sum += v;
            return;
        }
        int mid = (l + r) >> 1;
        // 不能把 tr[p].lc / tr[p].rc 直接按引用传进递归：new_node() 的 push_back 扩容会使引用悬垂。
        if (pos <= mid) {
            int lc = tr[p].lc;
            add(lc, l, mid, pos, v);
            tr[p].lc = lc;
        } else {
            int rc = tr[p].rc;
            add(rc, mid + 1, r, pos, v);
            tr[p].rc = rc;
        }
        tr[p].sum = (tr[p].lc ? tr[tr[p].lc].sum : 0) + (tr[p].rc ? tr[tr[p].rc].sum : 0);
    }

    int merge(int x, int y) {
        if (!x || !y) return x | y;
        tr[x].lc = merge(tr[x].lc, tr[y].lc);
        tr[x].rc = merge(tr[x].rc, tr[y].rc);
        tr[x].sum += tr[y].sum;
        return x;
    }
};
```
#### 分块：区间加 + 区间和

> **赛时先看**
> - **题目信号**：`n,q <= 2e5` 左右，操作简单，分块能过。
> - **本质**：懒得写线段树，或操作适合分块维护时。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(sqrt n)`。
> - **维护的量**：`n`（长度）、`B`（块大小）、`num`（块数）；`a[i]` 原值、`sum[b]` 块 b 总和（含 lazy）、`lazy[b]` 块 b 的整体加标记。
> - **警告**：散块暴力更新后要重建块和。
> - **约定**：a = init; // 1-based，下标从 1 开始

**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，`q` 次操作。`op=1 l r x`：`a[l..r]` 全部加 `x`；`op=2 l r`：输出闭区间和。

```cpp
SqrtRangeAddSum seg(a);                        // 构造即建树：a 必须 1-indexed（长度 n+1）
if (op == 1) { i64 x; cin >> x; seg.range_add(l, r, x); }  // 区间加
else cout << seg.range_sum(l, r) << '\n';                    // 区间和
```

样例：`a=[1,2,3,4,5]`；`range_add(2,4,10)` 后 `range_sum(1,5)` -> 输出 `45`。

**传参要求（照这个传不会错）：**

- `SqrtRangeAddSum seg(a)`：a 必须 1-indexed（长度 n+1），构造时自动分块建好。
- `range_add(l, r, v)`：闭区间 [l,r] 全部加 v；要求 `1 <= l <= r <= n`；无返回值。
- `range_sum(l, r)`：闭区间 [l,r] 的和；要求 `1 <= l <= r <= n`；返回 `i64`。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const vector<i64>& init)` -> 完成建树或预处理
- `range_add(int l, int r, i64 v)` -> 闭区间加值
- `range_sum(int l, int r)` -> 查询闭区间和 返回 `i64`。
**改板时先认这几个量：**

- `lazy`：懒标记。
- `sum`：区间和/计数和。

```cpp
struct SqrtRangeAddSum {
    int n, B, num;
    vector<i64> a, sum, lazy;

    SqrtRangeAddSum(const vector<i64>& init = {}) {
        if (!init.empty()) build(init);
    }

    void build(const vector<i64>& init) {
        a = init; // 1-based，下标从 1 开始。
        n = (int)a.size() - 1;
        B = max(1, (int)sqrt(n));
        num = (n + B - 1) / B;
        sum.assign(num, 0);
        lazy.assign(num, 0);
        for (int i = 1; i <= n; ++i) sum[(i - 1) / B] += a[i];
    }

    void range_add(int l, int r, i64 v) {
        int bl = (l - 1) / B, br = (r - 1) / B;
        if (bl == br) {
            for (int i = l; i <= r; ++i) a[i] += v, sum[bl] += v;
            return;
        }
        int end_l = min(n, (bl + 1) * B);
        for (int i = l; i <= end_l; ++i) a[i] += v, sum[bl] += v;
        for (int b = bl + 1; b <= br - 1; ++b) {
            lazy[b] += v;
            sum[b] += v * B;
        }
        for (int i = br * B + 1; i <= r; ++i) a[i] += v, sum[br] += v;
    }

    i64 range_sum(int l, int r) const {
        int bl = (l - 1) / B, br = (r - 1) / B;
        i64 ans = 0;
        if (bl == br) {
            for (int i = l; i <= r; ++i) ans += a[i] + lazy[bl];
            return ans;
        }
        int end_l = min(n, (bl + 1) * B);
        for (int i = l; i <= end_l; ++i) ans += a[i] + lazy[bl];
        for (int b = bl + 1; b <= br - 1; ++b) ans += sum[b];
        for (int i = br * B + 1; i <= r; ++i) ans += a[i] + lazy[br];
        return ans;
    }
};
```
#### ST 表

> **赛时先看**
> - **题目信号**：数组不修改，很多次问区间最大/最小/gcd。
> - **本质**：静态区间最值、区间 gcd。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：预处理 `O(n log n)`，查询 `O(1)`。
> - **维护的量**：`n`（长度）、`LOG`（层数）、`lg[i]`（i 的 log2 向下取整）；`st[k][i]`（从 i 起长度 `2^k` 的合并答案）。
> - **警告**：只适合可重复覆盖的运算，如 min/max/gcd，不适合 sum。
> - **约定**：`a` 是 1-based 数组（`a[1..n]`），`n = (int)a.size() - 1`。

**最小完整示例（先抄这一段就能跑）：**

题目：静态数组，`q` 次询问闭区间 `[l,r]` 的最大值。

```cpp
auto merge_max = [](i64 x, i64 y) { return max(x, y); };
SparseTable<i64, decltype(merge_max)> st(a, merge_max);  // 建表：a 必须 1-indexed（长度 n+1）
while (q--) { int l, r; cin >> l >> r; cout << st.query(l, r) << '\n'; }  // O(1) 区间最值
```

样例：`a=[1,3,2,5,4]`；`query(2,5)` -> 输出 `5`。

**传参要求（照这个传不会错）：**

- `SparseTable<T, Merge> st(a, merge)`：a 必须 1-indexed（`a[1..n]`，长度 n+1）；`merge` 必须与查询运算一致（max/min/gcd/OR 等幂等运算），不能混用。
- `st.query(l, r)`：闭区间 [l,r] 的答案；要求 `1 <= l <= r <= n`；返回 T，O(1)。
- 建表后数组不可修改；只支持幂等运算，不支持求和。


**不会用就照抄：**

```cpp
auto merge_max = [](i64 x, i64 y) { return max(x, y); };
SparseTable<i64, decltype(merge_max)> st(a, merge_max); // a[1..n]
auto ans = st.query(l, r);   // 闭区间 [l,r] 最大值
```

- 建完以后**不能修改数组**。
- 先看模板到底维护 `min/max/gcd/OR/AND` 中哪一种；合并函数不同不能混抄。


**API / 入口函数（赛时只认这里列的名字）：**

- `SparseTable<T,Merge> st(a,merge)` -> 用 **1-indexed 静态数组**建表；`merge` 必须与查询运算一致。
- `st.query(l,r)` -> O(1) 查询闭区间 `[l,r]`。
**核心逻辑（改代码时别破坏）：**

- `st[k][i]` 表示从 `i` 开始、长度 `2^k` 的答案。
- 查询 `[l,r]` 用两个长度 `2^k` 的块覆盖；因此要求运算幂等（min/max/gcd 等）。

```cpp
template <class T, class Merge>
struct SparseTable {
    int n, LOG;
    vector<int> lg;
    vector<vector<T>> st;
    Merge merge;

    SparseTable(const vector<T>& a, Merge merge_) : merge(std::move(merge_)) {
        build(a);
    }

    void build(const vector<T>& a) {
        n = (int)a.size() - 1; // 公式/约定：a is 1-based
        LOG = 1;
        while ((1 << LOG) <= max(1, n)) ++LOG; // 不依赖 GCC 的 __lg。
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;

        st.assign(LOG, vector<T>(n + 1));
        st[0] = a;
        for (int k = 1; k < LOG; ++k) {
            for (int i = 1; i + (1 << k) - 1 <= n; ++i) {
                st[k][i] = merge(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    T query(int l, int r) const {
        int k = lg[r - l + 1];
        return merge(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
```
#### 单调队列

> **赛时先看**
> - **题目信号**：每次只关心最近 `k` 个元素中的最大/最小值。
> - **本质**：滑动窗口最值、DP 优化。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`q`（存下标的双端队列，队首是当前窗口最大值下标，队列内下标对应值递减）。
> - **警告**：队首先弹过期，再用答案；最大值队列保持递减。

**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，窗口大小 `k`，输出每个滑动窗口的最大值。

```cpp
vector<int> a;                                 // 0-indexed
vector<int> ans = sliding_window_max(a, k);    // ans[i] 是以 a[i+k-1] 为右端点的窗口最大值
for (int x : ans) cout << x << ' ';            // 共 a.size()-k+1 个
```

样例：`a=[1,3,-1,-3,5,3,6,7]`，k=3 -> 输出 `3 3 5 5 6 7`。

**传参要求（照这个传不会错）：**

- `sliding_window_max(a, k)`：a 为 0-indexed 数组；k 为窗口长度，要求 `1 <= k <= a.size()`。
- 返回值：`vector<int> ans`，`ans[i]` 是以 `a[i+k-1]` 为右端点的窗口最大值，长度 `a.size()-k+1`。
- 要窗口最小值：把第二个 while 里 `a[q.back()] <= a[i]` 的 `<=` 改成 `>=`（队列改为递增），其余不变。


```cpp
// 约定：本函数为 0-indexed。
vector<int> sliding_window_max(const vector<int>& a, int k) {
    deque<int> q;
    vector<int> ans;
    for (int i = 0; i < (int)a.size(); ++i) {
        while (!q.empty() && q.front() <= i - k) q.pop_front();
        while (!q.empty() && a[q.back()] <= a[i]) q.pop_back();
        q.push_back(i);
        if (i >= k - 1) ans.push_back(a[q.front()]);
    }
    return ans;
}
```
#### 对顶堆：动态中位数、滑动窗口中位数与绝对偏差和

> **赛时先看**
> - **题目信号**：题面出现“数据流中位数”“每次加入一个数后输出中位数”“长度为 k 的每个窗口中位数”“把窗口内所有数变成同一个数的最小代价”。如果只要静态第 k 小，翻排序/主席树；如果要任意排名，不止中位数，翻平衡树/PBDS。
> - **本质**：维护一个动态多重集合的中位数。左边用大根堆保存较小的一半，右边用小根堆保存较大的一半；额外维护两边元素和，就能 `O(1)` 算所有数到中位数的绝对偏差和。带懒删除，适合滑动窗口。
> - **接法**：动态中位数直接 `insert(x)` 后输出 `lower_median()`；滑动窗口先插入新值，窗口长度超过 `k` 就 `erase(a[i-k])`，长度达到 `k` 后输出中位数或 `min_abs_deviation_sum()`。若题目要求偶数长度取上中位数，输出 `upper_median()`；若要求两个中位数平均值，用 `median_average()`。
> - **复杂度判定**：插入、删除、维护平衡均为 `O(log n)`；查询中位数和绝对偏差和为均摊 `O(1)`。空间 `O(n)`。
> - **维护的量**：`low`（较小一半，大根堆，堆顶即下中位数）、`high`（较大一半，小根堆）；`low_size/high_size`（两半逻辑元素个数）、`low_sum/high_sum`（两半元素和）。
> - **警告**：删除用懒删除，调用 `erase(x)` 前必须保证当前集合里确实有一个 `x`；左堆大小保持等于右堆或比右堆多 `1`，所以 `lower_median()` 是下中位数。偶数个数时，任意位于两个中位数之间的数都能最小化绝对偏差和，本模板用下中位数计算。

**最小完整示例（先抄这一段就能跑）：**

题目：依次读入 `n` 个数，每次读入后输出当前已读数的下中位数。

```cpp
DualHeapMedian dh;
for (int i = 1; i <= n; ++i) {
    i64 x; cin >> x;
    dh.insert(x);                              // 插入一个数，内部自动平衡两堆
    cout << dh.lower_median() << '\n';         // 当前下中位数
}
```

样例：依次插入 `1 5 2 4 3`，每次输出 -> `1 1 2 2 3`。

**传参要求（照这个传不会错）：**

- `DualHeapMedian dh`：无参构造，空集合。
- `insert(x)`：插入一个数 x；无返回值。
- `erase(x)`：懒删除一个 x；调用前必须保证集合里确实存在 x；无返回值。
- `lower_median()`：下中位数（偶数个元素时取较小的那个），返回 `i64`；集合不能为空。
- `upper_median()`：上中位数，返回 `i64`；`median_average()`：两个中位数的平均值，返回 `long double`。
- `min_abs_deviation_sum()`：所有数到下中位数的绝对偏差和，返回 `i64`。
- `size()`：当前逻辑元素个数，返回 `int`。
- 滑动窗口：0-indexed 数组 `a` 直接用 `sliding_window_lower_median(a,k)`（中位数）或 `sliding_window_abs_cost_to_median(a,k)`（绝对偏差和）。


**API / 入口函数（赛时只认这里列的名字）：**

- `erase(i64 x)` -> 删除元素
- `insert(i64 x)` -> 插入元素/字符串
- `lower_median()` -> 下中位数 返回 `i64`。
- `median_average()` -> 两个中位数的平均值 返回 `long double`。
- `min_abs_deviation_sum()` -> 所有数到下中位数的绝对偏差和 返回 `i64`。
- `size()` -> 查询集合大小 返回 `int`。
- `upper_median()` -> 上中位数 返回 `i64`。
**改板时先认这几个量：**

- `low`：较小一半，大根堆，堆顶是下中位数。
- `high`：较大一半，小根堆。
- `high_size`：只统计逻辑存在的元素。

```cpp
struct DualHeapMedian {
    priority_queue<i64> low; // 较小一半，大根堆，堆顶是下中位数。
    priority_queue<i64, vector<i64>, greater<i64>> high; // 较大一半，小根堆。
    unordered_map<i64, int> lazy_low, lazy_high;
    int low_size = 0, high_size = 0; // 只统计逻辑存在的元素。
    i64 low_sum = 0, high_sum = 0;

    void dec(unordered_map<i64, int>& mp, i64 x) {
        auto it = mp.find(x);
        if (--it->second == 0) mp.erase(it);
    }

    void prune_low() {
        while (!low.empty() && lazy_low.count(low.top())) {
            i64 x = low.top();
            low.pop();
            dec(lazy_low, x);
        }
    }

    void prune_high() {
        while (!high.empty() && lazy_high.count(high.top())) {
            i64 x = high.top();
            high.pop();
            dec(lazy_high, x);
        }
    }

    void move_low_to_high() {
        prune_low();
        i64 x = low.top();
        low.pop();
        --low_size;
        low_sum -= x;
        high.push(x);
        ++high_size;
        high_sum += x;
    }

    void move_high_to_low() {
        prune_high();
        i64 x = high.top();
        high.pop();
        --high_size;
        high_sum -= x;
        low.push(x);
        ++low_size;
        low_sum += x;
    }

    void balance() {
        prune_low();
        prune_high();
        while (low_size < high_size) move_high_to_low();
        while (low_size > high_size + 1) move_low_to_high();
        prune_low();
        prune_high();
    }

    int size() const {
        return low_size + high_size;
    }

    bool empty() const {
        return size() == 0;
    }

    void insert(i64 x) {
        prune_low();
        if (low.empty() || x <= low.top()) {
            low.push(x);
            ++low_size;
            low_sum += x;
        } else {
            high.push(x);
            ++high_size;
            high_sum += x;
        }
        balance();
    }

    void erase(i64 x) {
        assert(!empty());
        prune_low();
        prune_high();
        if (!low.empty() && x <= low.top()) {
            ++lazy_low[x];
            --low_size;
            low_sum -= x;
            if (!low.empty() && low.top() == x) prune_low();
        } else {
            ++lazy_high[x];
            --high_size;
            high_sum -= x;
            if (!high.empty() && high.top() == x) prune_high();
        }
        balance();
    }

    i64 lower_median() {
        assert(!empty());
        balance();
        return low.top();
    }

    i64 upper_median() {
        assert(!empty());
        balance();
        if (low_size == high_size) return high.top();
        return low.top();
    }

    long double median_average() {
        assert(!empty());
        balance();
        if (low_size == high_size) return ((long double)low.top() + high.top()) / 2.0L;
        return low.top();
    }

    i64 min_abs_deviation_sum() {
        i64 m = lower_median();
        return m * low_size - low_sum + high_sum - m * high_size;
    }
};

// 约定：本函数为 0-indexed。
vector<i64> sliding_window_lower_median(const vector<i64>& a, int k) {
    assert(1 <= k && k <= (int)a.size());
    DualHeapMedian dh;
    vector<i64> answer;
    for (int i = 0; i < (int)a.size(); ++i) {
        dh.insert(a[i]);
        if (i >= k) dh.erase(a[i - k]);
        if (i >= k - 1) answer.push_back(dh.lower_median());
    }
    return answer;
}

// 约定：本函数为 0-indexed。
vector<i64> sliding_window_abs_cost_to_median(const vector<i64>& a, int k) {
    assert(1 <= k && k <= (int)a.size());
    DualHeapMedian dh;
    vector<i64> answer;
    for (int i = 0; i < (int)a.size(); ++i) {
        dh.insert(a[i]);
        if (i >= k) dh.erase(a[i - k]);
        if (i >= k - 1) answer.push_back(dh.min_abs_deviation_sum());
    }
    return answer;
}
```

典题模型：CSES Sliding Median / Sliding Cost。读入数组 `a` 和窗口长度 `k`，中位数输出 `sliding_window_lower_median(a,k)`；若问每个窗口把所有数变成同一个数的最小总代价，输出 `sliding_window_abs_cost_to_median(a,k)`。
#### 单调栈：左右第一个更小元素

> **赛时先看**
> - **题目信号**：每个元素作为最小/最大值能控制多大区间。
> - **本质**：柱状图、贡献法、区间最值边界。
> - **接法**：给 1-indexed 数组 `a`（`a[0]` 占位），直接 `auto [L, R] = nearest_less(a);`，以 `i` 为最小值的区间就是 `[L[i]+1, R[i]-1]`，长度 `R[i]-L[i]-1`。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`L[i]/R[i]`（i 左/右第一个严格更小元素的下标，无则 `0`/`n+1`）；`st`（值严格递增的下标栈）。
> - **警告**：处理相等元素时要统一用 `<` 还是 `<=`，避免重复计数。
> - **约定**：int n = (int)a.size() - 1; // 1-based，下标从 1 开始


**最小完整示例（先抄这一段就能跑）：**

```cpp
// a 1-indexed：求每个位置左/右第一个严格更小元素的下标。
vector<int> a = {0, 2, 1, 5, 6, 2, 3}; // a[0] 占位
auto [L, R] = nearest_less(a);
cout << L[2] << ' ' << R[2] << '\n'; // 0 7（a[2]=1 是全数组最小值）
cout << L[4] << ' ' << R[4] << '\n'; // 3 5（a[4]=6）
```

- 样例：输出 `0 7` 与 `3 5`。

**传参要求（照这个传不会错）：**

- `a`：1-indexed（长度 `n+1`，`a[0]` 占位，`n = a.size()-1`）。
- 返回值 `{L, R}`：`L[i]` = 左边最近的满足 `a[L[i]] < a[i]` 的下标（没有则 `0`）；`R[i]` = 右边最近的满足 `a[R[i]] < a[i]` 的下标（没有则 `n+1`）。
- 以 `i` 为最小值能控制的最大区间：`[L[i]+1, R[i]-1]`，宽度 `R[i]-L[i]-1`。
- 想把“小于等于”也算更小：把两处 `a[st.back()] >= a[i]` 的 `>=` 改成 `>`。


```cpp
// 维护的量：L[i] / R[i]（i 左/右第一个严格更小的下标，无则 0 / n+1）；st（值单调递增的下标栈）。
// 不变量：任何时候栈内 a[st[k]] 严格递增；弹出一个元素时，它的更小边界就是栈内前一个位置。
pair<vector<int>, vector<int>> nearest_less(const vector<int>& a) {
    int n = (int)a.size() - 1; // 1-based，下标从 1 开始。
    vector<int> L(n + 1), R(n + 1);
    vector<int> st;
    for (int i = 1; i <= n; ++i) {
        // 弹出 >= a[i] 的，留下的栈顶就是左边最近的更小值
        while (!st.empty() && a[st.back()] >= a[i]) st.pop_back();
        L[i] = st.empty() ? 0 : st.back();
        st.push_back(i);
    }
    st.clear();
    for (int i = n; i >= 1; --i) {
        while (!st.empty() && a[st.back()] >= a[i]) st.pop_back();
        R[i] = st.empty() ? n + 1 : st.back(); // 倒序扫描对称求右边界
        st.push_back(i);
    }
    return {L, R};
}
```
#### 单调栈例题：柱状图最大矩形

> **赛时先看**
> - **题目信号**：每个柱子高度，问连续区间面积最大值。
> - **本质**：求直方图中最大矩形面积。
> - **接法**：给 1-indexed 柱高 `h`（`h[0]` 占位），一行 `largest_rectangle_histogram(h)` 出答案；0-indexed 输入记得前面补 0。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`L/R`（来自 `nearest_less` 的左右更小边界）；`ans`（枚举每根柱时维护的最大面积）。
> - **警告**：宽度是 `R[i] - L[i] - 1`。
> - **约定**：int n = (int)h.size() - 1; // 1-based，下标从 1 开始


**最小完整示例（先抄这一段就能跑）：**

```cpp
// h 1-indexed：柱状图最大矩形面积。
vector<int> h = {0, 2, 1, 5, 6, 2, 3}; // h[0] 占位
cout << largest_rectangle_histogram(h) << '\n'; // 10
```

- 样例：输出 `10`（高度 2 覆盖 1..5 号柱，面积 2*5；高度 5 覆盖 3..4 号柱，面积 5*2）。

**传参要求（照这个传不会错）：**

- `h`：1-indexed 柱高（长度 `n+1`，`h[0]` 占位），柱高可为 `0`。
- 返回值：`i64` 最大矩形面积，即 `max(h[i] * (R[i]-L[i]-1))`。
- 依赖 `nearest_less`，两个函数一起抄。


```cpp
// 维护的量：L/R（nearest_less 算出的左右更小边界）；ans（扫描过程中维护的最大面积）。
// 不变量：以第 i 根柱为最小高度的最大矩形宽度是 R[i]-L[i]-1，面积 = h[i] * 宽度。
i64 largest_rectangle_histogram(const vector<int>& h) {
    int n = (int)h.size() - 1; // 1-based，下标从 1 开始。
    auto [L, R] = nearest_less(h);
    i64 ans = 0;
    for (int i = 1; i <= n; ++i) {
        ans = max(ans, 1LL * h[i] * (R[i] - L[i] - 1)); // 左右都不小于 h[i] 的区间宽度
    }
    return ans;
}
```
#### 区间合并

> **赛时先看**
> - **题目信号**：很多 `[l,r]`，问合并后段数/总长度。
> - **本质**：合并重叠区间、求覆盖长度。
> - **接法**：把区间全部放进 `vector<pair<i64,i64>> segs`，调用 `merge_intervals(segs)`；返回的 `merged` 就是合并结果，`merged.size()` 是段数，长度自己按闭/半开口径累加。
> - **复杂度判定**：`O(n log n)`。
> - **维护的量**：`segs`（按左端点排序后的区间表）；`res`（合并结果栈，`res.back().second` 是当前覆盖到的最右端）。
> - **警告**：闭区间长度是 `r-l+1`，半开区间长度是 `r-l`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 合并重叠/相接区间，输出合并后的段数与闭区间总覆盖长度。
vector<pair<i64, i64>> segs = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
auto merged = merge_intervals(segs);
cout << merged.size() << '\n'; // 3
i64 cover = 0;
for (auto [l, r] : merged) cover += r - l + 1; // 闭区间长度
cout << cover << '\n';         // 13
```

- 样例：输出 `3` 与 `13`。

**传参要求（照这个传不会错）：**

- `segs`：`vector<pair<i64,i64>>`，闭区间 `{l, r}`；顺序任意（内部 sort），`i64` 到 1e18 安全。
- 返回值：合并后的区间（`vector<pair<i64,i64>>`）；重叠或相接（`l <= res.back().second`）都会并成一段。
- 半开区间口径：长度按 `r-l` 算，合并条件不变。


```cpp
// 维护的量：segs（按左端点排序后的区间）；res（合并结果栈，res.back().second 是当前覆盖到的最右端）。
// 不变量：res 中相邻区间严格不相交（后一个 l 大于前一个 r）。
vector<pair<i64,i64>> merge_intervals(vector<pair<i64,i64>> segs) {
    sort(segs.begin(), segs.end());
    vector<pair<i64,i64>> res;
    for (auto [l, r] : segs) {
        if (res.empty() || l > res.back().second) res.push_back({l, r}); // 与当前段断开，新开一段
        else res.back().second = max(res.back().second, r);              // 重叠/相接则并进当前段
    }
    return res;
}
```
#### 启发式合并：小集合并入大集合

> **赛时先看**
> - **题目信号**：很多集合需要两两合并（并集）；合并后还要维护集合内信息（元素集合、计数、和、最小值、去重数）；或树上每个点一个集合、自底向上合并到父亲（如"每个子树内不同颜色数"）。
> - **本质**：把"小集合"的每个元素逐个搬进"大集合"再丢弃小集合。每个元素每被搬一次，所在集合大小至少翻倍，所以总搬移次数 O(n log n)——这是它快的原因。
> - **复杂度判定**：每个元素最多搬 `O(log n)` 次；用 `std::set` 时每次插入还要 `O(log n)`，总 `O(n log^2 n)`（n ≤ 2e5 可用）；用哈希集合则可到均摊 `O(n log n)`。若合并是"可撤销的"，翻同章可撤销并查集；若只要子树颜色数且要求更快，翻 C 章 DSU on Tree。
> - **维护的量**：每个集合的元素表（`set<int>`）与伴随统计（如 `sum`）；合并后大集合的统计量就是答案。
> - **接法**：`merge_bags(big, small)` 一行合并，先保证大集合在左边；树上回溯时把儿子的包并进父亲。
> - **警告**：先 `swap` 保证大集合在左边再搬；搬完必须清空小集合；伴随信息要跟着集合一起 `swap`。

**不会用就照抄：**

```cpp
BagInfo bag(n + 1);          // 每个点一个包
bag[u].s.insert(x); bag[u].sum += x;   // 往包里塞元素
merge_bags(bag[u], bag[v]);  // 把 v 的包并入 u 的包
int ans = (int)bag[u].s.size();        // 合并后的集合大小/和就是答案
```

**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个集合初始各含一个元素（集合 `i` 含元素 `i`）；`m` 次操作。`op=1 x y`：合并集合 `x`、`y`（取并集，重复元素只留一份）；`op=2 x`：输出集合 `x` 的元素个数与元素和。

```cpp
vector<BagInfo> bag(n + 1);             // 1. 结构体定义：每个集合一个包，编号 1..n
for (int i = 1; i <= n; ++i) {
    bag[i].s.insert(i);                 // 2. 初始化：塞入初始元素
    bag[i].sum += i;
}
while (m--) {
    int op, x, y;
    cin >> op;
    if (op == 1) {
        cin >> x >> y;
        merge_bags(bag[x], bag[y]);     // 3. 调用：把 y 的包并入 x 的包（内部自动小入大）
    } else {
        cin >> x;
        cout << bag[x].s.size() << ' ' << bag[x].sum << '\n';  // 4. 输出：元素个数 + 元素和
    }
}
```

样例：初始 `bag[1].s={1}`、`bag[2].s={2}`；`merge_bags(bag[1], bag[2])` 后 -> `bag[1].s.size()=2`，`bag[1].sum=3`。

**传参要求（照这个传不会错）：**

- `BagInfo bag(n + 1)`：每个集合一个包；`s` 存集合元素（`set<int>`），`sum` 是伴随统计（元素和，示例）。
- `merge_bags(big, small)`：把 `small` 的包并入 `big`；内部自动把小集合并入大集合（必要时整体交换），伴随信息跟着一起交换；并入后 `small` 被清空，不能再用。
- `merge_into(big, small)`：无伴随信息版，直接对两个 `set<int>` 合并，规则同上。
- `dfs_merge_example(u, parent, bag, g)`：树形合并骨架；在回溯处自动把儿子并入父亲，合并后 `bag[u].s.size()` 即子树内不同元素数。
- 换元素类型：`set<int>` 改成 `set<i64>` / `set<pair<int,int>>` 等即可，合并函数不用改。

**抄板清单（照着做就行）：**
1. 抄哪段：`merge_into` 或 `merge_bags`（带伴随信息用后者）。
2. 构造：给每个"起点"建一个空 `BagInfo`，把初始元素 insert 进去。
3. 操作：需要合并时调用 `merge_bags(big, small)`；函数内部自动把小的并入大的。
4. 取结果：`big.s.size()` / `big.sum` 就是合并后的集合信息。

**改造点（按题目改这几处）：**
- 集合元素类型：`set<int>` 换成 `set<i64>` / `set<pair<int,int>>` / `unordered_set`。
- 伴随信息：`sum` 换成题目要的（个数、最小值、最大值、和值），跟着 `swap` 一起走。
- 树形合并：把 `dfs_merge_example` 的合并点放到回溯处，答案在回溯后取。
- 要按颜色统计频次而非去重：`set` 换成 `map<int,int>` 时"小入大"原则不变。

**核心逻辑（改代码时别破坏）：**

- `size` 小的必须在右边，搬完清空小集合，否则元素会被重复计入。
- 伴随统计量必须与集合一起 `swap`，否则统计值跟错集合。

**改板时先认这几个量：**

- `s`：集合元素表。
- `sum`：集合内元素总和（伴随信息示例）。

```cpp
// 维护的量：big.s = 合并后的大集合；small.s = 待并入的小集合（并入后清空）。
// 不变量：每个元素在任何时刻只存在于一个集合；集合大小每搬一次至少翻倍，
//         所以每个元素最多被搬 O(log n) 次。
// 把 small 的所有元素并入 big，并保证总是把小的并入大的。
// 元素去重语义：相同元素只保留一份。
void merge_into(set<int>& big, set<int>& small) {
    if (big.size() < small.size()) big.swap(small); // 保证 big 是较大的集合。
    for (int x : small) big.insert(x);
    small.clear(); // 必须清空，否则元素被重复计入。
}

// 带伴随信息（元素和）的版本：信息跟着集合一起走。
struct BagInfo {
    set<int> s;
    i64 sum = 0;
};

void merge_bags(BagInfo& big, BagInfo& small) {
    if (big.s.size() < small.s.size()) {
        big.s.swap(small.s);
        swap(big.sum, small.sum); // 伴随信息必须和集合同步交换。
    }
    for (int x : small.s) {
        if (big.s.insert(x).second) big.sum += x; // 只有新元素才计入和。
    }
    small.s.clear();
    small.sum = 0;
}

// 典题骨架：树上每个点一个集合，自底向上把儿子的集合并进父亲，
// 最后每个点的集合大小/元素和就是该子树内的答案。
// 用法：先 dfs 处理完所有儿子（递归），再在回溯时调用 merge_bags。
void dfs_merge_example(int u, int parent, vector<BagInfo>& bag, const vector<vector<int>>& g) {
    for (int v : g[u]) {
        if (v == parent) continue;
        dfs_merge_example(v, u, bag, g);
        merge_bags(bag[u], bag[v]); // 回溯时把小儿子并入父亲。
    }
    // 此时 bag[u].s 包含 u 子树内全部元素；bag[u].s.size() 即子树不同元素数。
}
```

典题模型：树上每个点有一种颜色，问每个子树内有多少种不同颜色（CF 600E 的弱化版直接数 size；带频次统计请翻 C 章「DSU on Tree」）。合并方向固定的树形合并、按大小合并的并查集也是同一个「小入大」思想。

### 04 高级数据结构与离线分治

可持久化、平衡树、可并堆、动态树、线性基、PBDS 以及 CDQ/整体二分/离线动态连通性放在这里。
#### 笛卡尔树

> **赛时先看**
> - **题目信号**：需要按区间最小/最大值递归分治；数组下标顺序和堆序都重要。
> - **本质**：把数组转成同时满足中序为原序列、堆性质的树。常用于 RMQ、区间最值分治、最大矩形变种。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`left_child/right_child`（下标 1..n 每个位置的左右儿子，0 表示空）；返回根节点编号。
> - **警告**：最小笛卡尔树用 `>` 弹栈，最大笛卡尔树用 `<` 弹栈；根是最后留在栈底的元素。


**最小完整示例（先抄这一段就能跑）：**

题目：给 `a[1..n]` 建最小笛卡尔树（RMQ / 区间最值分治），拿到树根和每个位置的左右儿子。

```cpp
vector<int> lc(n + 1), rc(n + 1);
int root = build_cartesian_tree(a, lc, rc);  // 1. 建树：a 必须 1-indexed，lc/rc 被填成左右儿子
// 2. 中序遍历 lc/rc 即原序列；根是最小值位置，左右子树分别对应左右区间
cout << root << '\n';                        // 3. root 即全局最小值所在下标
```

样例：`a=[3,1,2]` -> `root=2`（a[2]=1 最小），`lc[2]=1, rc[2]=3`。

**传参要求（照这个传不会错）：**

- `a`：1-indexed，只用 `a[1..n]`，`a[0]` 不参与建树。
- `left_child/right_child`：输出参数，传入空 vector 即可，函数会填好每个下标 1..n 的左右儿子，0 表示空。
- 返回值：根节点下标（全局最小元素所在位置），`n>=1` 时不为 0。
- 默认建**最小**笛卡尔树；要最大笛卡尔树，把栈内比较 `a[st.back()] > a[i]` 的 `>` 改成 `<`。

**API / 入口函数（赛时只认这里列的名字）：**

- `build_cartesian_tree(const vector<int>& a, vector<int>& left_child, vector<int>& right_child)` -> 构建最小笛卡尔树，返回根。下标 1..n。

```cpp
// 构建最小笛卡尔树，返回根。下标 1..n。
int build_cartesian_tree(const vector<int>& a, vector<int>& left_child, vector<int>& right_child) {
    int n = (int)a.size() - 1;
    left_child.assign(n + 1, 0);
    right_child.assign(n + 1, 0);
    vector<int> st;
    for (int i = 1; i <= n; i++) {
        int last = 0;
        while (!st.empty() && a[st.back()] > a[i]) {
            last = st.back();
            st.pop_back();
        }
        if (!st.empty()) right_child[st.back()] = i;
        left_child[i] = last;
        st.push_back(i);
    }
    return st.empty() ? 0 : st.front();
}
```
#### 主席树：静态区间第 k 小 / 排名 / 前驱后继

> **赛时先看**
> - **题目信号**：数组**不修改**，多次询问 `[L,R]` 的第 `k` 小、中位数、排名、前驱或后继。静态区间顺序统计，直接上主席树。
> - **本质**：对值域建权值线段树，`root[i]` 存前缀 `a[1..i]` 的版本；`root[R] - root[L-1]` 两版本相减就是区间 `[L,R]` 的频次分布，第 k 小沿“左右儿子计数之差”决定往左还是往右走。
> - **复杂度判定**：建树 `O(n log M)`；单次查询 `O(log M)`；空间 `O(n log M)`，`M` 为离散化后值域大小；n、m 到 2e5 很稳，节点池约 n*22 个，vector 动态增长即可。
> - **维护的量**：`root`（root[i]：前缀 a[1..i] 的版本根）；`tr`（节点池，0 号为空节点）；`xs`（排序去重后的原值，离散编号 1..xs.size()）。
> - **接法**：数组必须写成 `a[1..n]`。先 `build_from_array(a)`；之后直接调用 `kth_value / count_lt / count_le / rank_of / predecessor / successor`。
> - **警告**：`k` 从 **1** 开始；`kth_value()` 返回**原值**；`*_impl()` 都是内部函数；建树后不支持单点修改，动态修改请用下一节“动态区间顺序统计（树套树）”。


**最小完整示例（先抄这一段就能跑）：**

题目：静态数组 `a[1..n]`，`m` 次询问 `[L,R]` 的第 `k` 小。

```cpp
PersistentKth pt;
pt.build_from_array(a);            // 1. 结构体定义 + 建树：a 必须 1-indexed
while (m--) {
    int L, R, k;
    cin >> L >> R >> k;
    cout << pt.kth_value(L, R, k) << '\n';   // 2. 调用：返回第 k 小的原值
}
```

样例：`a=[1,5,2,4,3]`；问 `[2,5]` 第 2 小 -> 输出 `3`（区间内是 5,2,4,3，排序后第 2 小是 3）。

**传参要求（照这个传不会错）：**

- `build_from_array(a)`：`a` 必须 1-indexed（`a[0]` 不参与）；内部会离散化，不需你处理。
- `kth_value(L, R, k)`：`k` 从 **1** 开始（第 1 小传 1）；`1 <= L <= R <= n`；返回**原数组里的实际值**。
- 中位数：奇数长度区间传 `k = (R-L+2)/2`。
- `rank_of(L,R,x)` 返回排名；`count_lt/count_le` 返回严格小于/小于等于的个数；`predecessor/successor` 找不到时返回 `nullopt`。
- 只支持静态数组；带修改用下一节树套树。


**不会用就照抄（最常用：静态区间第 k 小 / 中位数）：**

```cpp
vector<i64> a(n + 1);
for (int i = 1; i <= n; ++i) cin >> a[i];

PersistentKth pt;
pt.build_from_array(a);

// [l,r] 第 k 小：k 从 1 开始
cout << pt.kth_value(l, r, k) << '\n';

// 奇数长度区间中位数
cout << pt.kth_value(l, r, (r - l + 2) / 2) << '\n';
```

- 你在 `solve()` 里主要只碰：`build_from_array / kth_value / count_lt / count_le / rank_of / predecessor / successor`。
- **不要直接调** `kth_impl()`、`count_prefix_impl()`：它们的 `l/r` 是离散值域，不是题目询问区间。
- `kth_value()` 返回的是**原数组实际值**，不是离散化编号。
- 建树后数组不能修改；有 `a[pos]=x` 就换下一节动态模板。


**API / 入口函数（赛时只认这里列的名字）：**

- `pt.build_from_array(a)` -> `a` 必须 1-indexed；建立静态前缀版本。
- `pt.kth_value(L,R,k)` -> 闭区间第 k 小 **实际值**；`k` 从 1 开始。
- `pt.count_lt(L,R,x)` -> 统计 `<x` 的元素个数。
- `pt.count_le(L,R,x)` -> 统计 `<=x` 的元素个数。
- `pt.rank_of(L,R,x)` -> 排名 = `<x` 的个数 + 1。
- `pt.predecessor/successor(...)` -> 返回 `optional<i64>`；不存在为 `nullopt`。

**抄板清单（照着做就行）：**

1. 抄哪段：整个 `PersistentKth` 结构体，抄到 `solve()` 外面。
2. 构造：`PersistentKth pt;`（数据量大时可 `PersistentKth pt(n * 22);` 预留节点空间）。
3. 建树：`pt.build_from_array(a);`，`a` 必须 1-indexed，`a[0]` 不参与。
4. 调用：第 k 小 `pt.kth_value(L, R, k)`；中位数 = `k = (R-L+2)/2` 的第 k 小；排名 `pt.rank_of(L,R,x)`。
5. 取结果：`kth_value` 返回原数组里的**实际值**，直接输出。

**改造点（按题目改这几处）：**

- 只问第 k 小/中位数：建树 + `kth_value` 两行搞定，其他函数不碰。
- 问排名/前驱/后继：用 `count_lt / count_le / rank_of / predecessor / successor`，输入 `x` 用原值。
- 区间 `[L,R]`：题目闭区间，1-indexed 直接传。
- 前驱/后继返回 `nullopt`：表示不存在，题目要输出什么哨兵（如 `-1`）自己判。

**核心逻辑（改代码时别破坏）：**

- `root[i]` 是前缀 `a[1..i]` 的频次版本；区间 `[L,R]` 的频次 = `root[R] - root[L-1]`。
- 查第 k 小时比较“两个版本左儿子计数之差”，决定向左还是向右。
**改板时先认这几个量：**

- `root`：root[i]：前缀 a[1..i] 的版本根。
- `tr`：树节点池（节点数组）。
- `xs`：排序去重后的值域。
- `sum`：区间和/计数和。

**接口含义（看不懂参数时看这里）：**

- `build_from_array(a)`：`a` 必须是 1-indexed，`a[0]` 不参与建树。
- `kth_value(L,R,k)`：查询闭区间 `[L,R]` 的第 `k` 小，`1 <= k <= R-L+1`。
- `count_lt(L,R,x)` / `count_le(L,R,x)`：统计 `< x` / `<= x` 的元素数量。
- `rank_of(L,R,x)`：返回题目常见定义的排名，即 `< x` 的个数 `+1`。
- `predecessor/successor`：返回 `optional<i64>`；不存在时为 `nullopt`，由题目决定输出什么哨兵值。

```cpp
// 维护的量：root（root[i]：前缀 a[1..i] 的版本根）、tr（节点池，0 号为空节点）、xs（排序去重后的原值，离散编号 1..xs.size()）。
// 不变量：root[R] 与 root[L-1] 对应节点相减，即区间 [L,R] 的频次分布。
struct PersistentKth {
    struct Node {
        int l = 0, r = 0, sum = 0;
    };

    vector<Node> tr{Node{}}; // 0 号节点为空节点。
    vector<int> root;        // root[i]：前缀 a[1..i] 的版本根。
    vector<i64> xs;          // 排序去重后的原值；离散编号为 1..xs.size()。

    explicit PersistentKth(int reserve_nodes = 0) {
        if (reserve_nodes > 0) tr.reserve(reserve_nodes);
    }

    // [内部] 复制旧节点，返回新节点编号。
    int clone_node(int p) {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    // [内部] 在旧版本 p 上把离散位置 pos 的出现次数 +1，返回新版本根。
    int update_impl(int p, int l, int r, int pos) {
        int q = clone_node(p);
        ++tr[q].sum;
        if (l == r) return q;
        int mid = (l + r) >> 1;
        if (pos <= mid) tr[q].l = update_impl(tr[p].l, l, mid, pos);
        else tr[q].r = update_impl(tr[p].r, mid + 1, r, pos);
        return q;
    }

    // [内部] u=root[L-1], v=root[R]；l/r 是离散值域，不是原数组区间。
    // 返回第 k 小的“离散编号”，外部通常不要直接调用。
    int kth_impl(int u, int v, int l, int r, int k) const {
        if (l == r) return l;
        int left_count = tr[tr[v].l].sum - tr[tr[u].l].sum; // 两版本左儿子频次之差 = 区间内落在左半值域的个数
        int mid = (l + r) >> 1;
        if (k <= left_count) return kth_impl(tr[u].l, tr[v].l, l, mid, k);
        return kth_impl(tr[u].r, tr[v].r, mid + 1, r, k - left_count);
    }

    // [内部] 统计两个版本之差中，离散编号 <= qr 的元素数量。
    int count_prefix_impl(int u, int v, int l, int r, int qr) const {
        if (qr <= 0) return 0;
        if (r <= qr) return tr[v].sum - tr[u].sum;
        int mid = (l + r) >> 1;
        int ans = count_prefix_impl(tr[u].l, tr[v].l, l, mid, qr);
        if (qr > mid) ans += count_prefix_impl(tr[u].r, tr[v].r, mid + 1, r, qr);
        return ans;
    }

    // 外部建树接口：a 必须为 1-indexed，真实数据是 a[1..n]。
    void build_from_array(const vector<i64>& a) {
        assert(a.size() >= 2);
        xs.assign(a.begin() + 1, a.end());
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());

        root.assign(a.size(), 0);
        tr.clear();
        tr.push_back(Node{});
        tr.reserve(max<size_t>(tr.capacity(), a.size() * 22));

        for (int i = 1; i < (int)a.size(); ++i) {
            int pos = int(lower_bound(xs.begin(), xs.end(), a[i]) - xs.begin()) + 1;
            root[i] = update_impl(root[i - 1], 1, (int)xs.size(), pos);
        }
    }

    // 外部查询：[L,R] 第 k 小的“实际值”；L/R/k 全部按题面常见的 1-indexed 使用。
    i64 kth_value(int L, int R, int k) const {
        assert(1 <= L && L <= R && R < (int)root.size());
        assert(1 <= k && k <= R - L + 1);
        int id = kth_impl(root[L - 1], root[R], 1, (int)xs.size(), k);
        return xs[id - 1];
    }

    int count_le(int L, int R, i64 x) const {
        int id = int(upper_bound(xs.begin(), xs.end(), x) - xs.begin()); // <=x 的离散值个数
        if (id == 0) return 0;
        return count_prefix_impl(root[L - 1], root[R], 1, (int)xs.size(), id);
    }

    int count_lt(int L, int R, i64 x) const {
        int id = int(lower_bound(xs.begin(), xs.end(), x) - xs.begin()); // <x 的离散值个数
        if (id == 0) return 0;
        return count_prefix_impl(root[L - 1], root[R], 1, (int)xs.size(), id);
    }

    int rank_of(int L, int R, i64 x) const {
        return count_lt(L, R, x) + 1;
    }

    optional<i64> predecessor(int L, int R, i64 x) const {
        int c = count_lt(L, R, x);
        if (c == 0) return nullopt;
        return kth_value(L, R, c);
    }

    optional<i64> successor(int L, int R, i64 x) const {
        int c = count_le(L, R, x);
        if (c == R - L + 1) return nullopt;
        return kth_value(L, R, c + 1);
    }
};
```

**能力边界：**这是静态结构。若存在 `a[pos] = x`，不要尝试修改 `root[pos..n]`；直接换下一节动态模板。
#### 动态区间顺序统计：值域线段树套 Fenwick（P3380 五操作）

> **赛时先看**
> - **题目信号**：数组有**单点修改**，同时查询任意 `[L,R]` 的排名、第 `k` 小、前驱、后继。
> - **本质**：外层按“值域”建线段树；每个值域节点内部用 Fenwick 统计这些值当前出现在哪些“位置”。修改沿值域路径更新，查询第 `k` 小直接按左右值域下降。
> - **接法**：必须先把**所有操作读入**，因为模板会预收集“某位置未来可能出现的值”来压缩各内部 Fenwick。然后 `build(a, ops)`，再按原顺序调用 `modify / rank_of / kth_value / predecessor / successor`。
> - **复杂度判定**：预处理、修改、排名、第 `k` 小均为 `O(log M log n)`；空间约 `O((n + 修改次数） log M)`。
> - **维护的量**：`value`（当前数组，1-indexed）；`xs`（所有可能出现的值，排序去重）；`coord/bit`（每个值域节点：可出现的位置集合 + 该集合上的 Fenwick）。
> - **警告**：本模板是“离线预处理坐标、在线执行操作”：答案仍按输入顺序实时输出，但 `build()` 前必须已经读完所有操作；`k` 从 1 开始；原数组和位置都是 1-indexed。


**最小完整示例（先抄这一段就能跑）：**

题目：P3380 五操作：`n` 个数，`m` 次操作 = 区间排名 / 区间第 k 小 / 单点改值 / 前驱 / 后继。

```cpp
using Op = DynamicRangeOrderStatistic::Operation;
vector<Op> ops(m);
for (auto &op : ops) {             // 1. 先读完全部操作：build 要预收集所有 type=3 的新值
    cin >> op.type;
    if (op.type == 3) cin >> op.a >> op.b;      // 单点改：pos, newValue
    else cin >> op.a >> op.b >> op.c;           // 查询：L, R, x/k
}
DynamicRangeOrderStatistic ds;
ds.build(a, ops);                  // 2. 建树：a 必须 a[1..n]，1-indexed
for (auto op : ops) {
    if (op.type == 1) cout << ds.rank_of(op.a, op.b, op.c) << '\n';
    if (op.type == 2) cout << ds.kth_value(op.a, op.b, op.c) << '\n';
    if (op.type == 3) ds.modify(op.a, op.b);    // 3. 单点赋值 a[pos] = x
    if (op.type == 4) cout << ds.predecessor(op.a, op.b, op.c) << '\n';
    if (op.type == 5) cout << ds.successor(op.a, op.b, op.c) << '\n';
}
```

样例：`a=[2,1,3]`；操作 `2 1 3 2` -> 输出 `2`（区间 [1,3] 第 2 小）。

**传参要求（照这个传不会错）：**

- `build(a, ops)`：`a` 必须 1-indexed；`ops` 必须已读完全部操作（特别是 type=3 的未来新值）。
- `modify(pos, x)`：`pos` 是 1-indexed 位置；`x` 必须曾作为某个 type=3 操作传给 `build()`。
- `rank_of(L,R,x)`：`x` 用原值，返回 1-based 排名（`<x` 的个数 + 1）。
- `kth_value(L,R,k)`：`k` 从 1 开始（第 1 小传 1）；`1 <= L <= R <= n`；返回原值。
- `predecessor/successor(L,R,x)`：返回原值；不存在时返回哨兵（默认 `-2147483647` / `2147483647`，与 P3380 一致）。
- 执行时在线输出答案，但结构必须离线预读，不能边读操作边 build。

**不会用就照抄（P3380 五操作）：**

```cpp
using Op = DynamicRangeOrderStatistic::Operation;
vector<Op> ops(m);

// 先把全部操作读完：build 必须提前知道未来所有 type=3 的新值
for (auto &op : ops) {
    cin >> op.type;
    if (op.type == 3) cin >> op.a >> op.b;       // pos, newValue
    else cin >> op.a >> op.b >> op.c;            // L, R, x/k
}

DynamicRangeOrderStatistic ds;
ds.build(a, ops);   // a 必须是 a[1..n]

for (auto op : ops) {
    if (op.type == 1) cout << ds.rank_of(op.a, op.b, op.c) << '\n';
    if (op.type == 2) cout << ds.kth_value(op.a, op.b, op.c) << '\n';
    if (op.type == 3) ds.modify(op.a, op.b);
    if (op.type == 4) cout << ds.predecessor(op.a, op.b, op.c) << '\n';
    if (op.type == 5) cout << ds.successor(op.a, op.b, op.c) << '\n';
}
```

- 这份板子虽然“执行时在线”，但 **build 前必须先读完全部操作**。
- 位置和 `[L,R]` 都是 1-indexed；`kth_value` 的 `k` 也是 1-indexed。


**API / 入口函数（赛时只认这里列的名字）：**

- `ds.build(a,ops)` -> **先读完全部操作**再建结构；`a` 1-indexed。
- `ds.modify(pos,x)` -> 单点赋值 `a[pos]=x`。
- `ds.rank_of(L,R,x)` -> 区间内 x 的排名。
- `ds.kth_value(L,R,k)` -> 区间第 k 小，k 从 1 开始。
- `ds.predecessor/successor(L,R,x)` -> 严格前驱/后继；无解时返回 P3380 哨兵。
**核心逻辑（改代码时别破坏）：**

- 外层按值域二分；每个值域节点内部 Fenwick 统计“哪些位置目前落在这个值域”。
- 单点修改只沿 `O(log M)` 个值域节点更新；区间第 k 小在值域树上按 `[L,R]` 左儿子数量下降。
**改板时先认这几个量：**

- `value`：当前数组，1-indexed。
- `xs`：所有“可能真正出现在数组里”的值，排序去重。
- `bit`：每个值域节点：可出现的位置 + 该位置集合上的 Fenwick。

**为什么它比静态主席树多这一层：**静态主席树的 `root[i]` 是前缀版本，改 `a[pos]` 会影响所有 `root[pos..n]`。这里改为按值域分层，并在每个值域节点维护位置频次，所以一次修改只动 `O(log M)` 个值域节点，每个节点内部再做一次 `O(log n)` Fenwick 修改。

**接口约定：**

- `Operation{type,a,b,c}`：只用于预读；`type=3` 时 `a=pos,b=newValue`，其余操作 `a=L,b=R,c=x/k`。
- `build(a, ops)`：`a[1..n]` 为初始数组；`ops` 必须包含全部未来修改，用来预收集坐标。
- `modify(pos,newValue)`：单点赋值。
- `count_lt/count_le(L,R,x)`：统计 `<x` / `<=x`。
- `rank_of(L,R,x)`：严格小于 `x` 的数量 + 1。
- `kth_value(L,R,k)`：第 `k` 小实际值，`k` 1-indexed。
- `predecessor/successor`：直接返回值；不存在时默认返回 P3380 要求的两个哨兵。

```cpp
struct DynamicRangeOrderStatistic {
    struct Operation {
        int type;
        int a, b, c;
        // type=3: a=pos, b=newValue, c 未使用
        // 其他 : a=L,   b=R,        c=x 或 k
    };

    int n = 0, M = 0;
    vector<int> value;              // 当前数组，1-indexed。
    vector<int> xs;                 // 所有“可能真正出现在数组里”的值，排序去重。
    vector<vector<int>> coord, bit; // 每个值域节点：可出现的位置 + 该位置集合上的 Fenwick。

    static int lowbit(int x) { return x & -x; }

    // [内部] 在某个值域节点的局部 Fenwick 中，把位置 pos 的计数 += delta。
    void local_add(int o, int pos, int delta) {
        auto &c = coord[o];
        auto &b = bit[o];
        int k = int(lower_bound(c.begin(), c.end(), pos) - c.begin()) + 1;
        for (int i = k; i < (int)b.size(); i += lowbit(i)) b[i] += delta;
    }

    // [内部] 某值域节点中，位置 <= pos 的当前元素个数。
    int local_prefix(int o, int pos) const {
        if (o == 0 || coord[o].empty()) return 0;
        const auto &c = coord[o];
        const auto &b = bit[o];
        int k = int(upper_bound(c.begin(), c.end(), pos) - c.begin());
        int res = 0;
        for (int i = k; i > 0; i -= lowbit(i)) res += b[i];
        return res;
    }

    // [内部] 某值域节点中，原数组位置落在 [L,R] 的元素个数。
    int local_range(int o, int L, int R) const {
        if (L > R || o == 0) return 0;
        return local_prefix(o, R) - local_prefix(o, L - 1);
    }

    // [内部] 预处理：声明“pos 将来可能以 vid 这个值出现”。
    void reserve_candidate(int o, int l, int r, int vid, int pos) {
        coord[o].push_back(pos);
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (vid <= mid) reserve_candidate(o << 1, l, mid, vid, pos);
        else reserve_candidate(o << 1 | 1, mid + 1, r, vid, pos);
    }

    // [内部] 当前数组中，在 (pos, vid) 这一个点上加/减一次出现次数。
    void point_add(int o, int l, int r, int vid, int pos, int delta) {
        local_add(o, pos, delta);
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (vid <= mid) point_add(o << 1, l, mid, vid, pos, delta);
        else point_add(o << 1 | 1, mid + 1, r, vid, pos, delta);
    }

    int id_exact(int x) const {
        return int(lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1;
    }

    // 外部建树接口。
    // a 必须 1-indexed；ops 必须已经读入全部操作，至少要包含所有 type=3 的未来新值。
    void build(const vector<int>& a, const vector<Operation>& ops) {
        n = (int)a.size() - 1;
        value = a;

        xs.assign(a.begin() + 1, a.end());
        for (const auto &op : ops) if (op.type == 3) xs.push_back(op.b);
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        M = (int)xs.size();

        coord.assign(4 * M + 8, {});
        bit.assign(4 * M + 8, {});

        // 初值和未来修改值都要预先登记“这个位置可能走过哪些值域节点”。
        for (int pos = 1; pos <= n; ++pos)
            reserve_candidate(1, 1, M, id_exact(a[pos]), pos);
        for (const auto &op : ops)
            if (op.type == 3)
                reserve_candidate(1, 1, M, id_exact(op.b), op.a);

        for (int o = 1; o < (int)coord.size(); ++o) {
            if (coord[o].empty()) continue;
            auto &c = coord[o];
            sort(c.begin(), c.end());
            c.erase(unique(c.begin(), c.end()), c.end());
            bit[o].assign(c.size() + 1, 0);
        }

        for (int pos = 1; pos <= n; ++pos)
            point_add(1, 1, M, id_exact(value[pos]), pos, +1);
    }

    // 外部：a[pos] = new_value。
    // new_value 必须已作为某个 type=3 操作传给 build() 预处理过。
    void modify(int pos, int new_value) {
        point_add(1, 1, M, id_exact(value[pos]), pos, -1);
        value[pos] = new_value;
        point_add(1, 1, M, id_exact(value[pos]), pos, +1);
    }

    // 外部：统计 a[L..R] 中 < x 的个数。
    int count_lt(int L, int R, int x) const {
        int t = int(lower_bound(xs.begin(), xs.end(), x) - xs.begin());
        if (t <= 0) return 0;
        if (t >= M) return R - L + 1;

        int o = 1, l = 1, r = M, ans = 0;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (t <= mid) {
                o <<= 1;
                r = mid;
            } else {
                ans += local_range(o << 1, L, R);
                o = o << 1 | 1;
                l = mid + 1;
            }
        }
        ans += local_range(o, L, R);
        return ans;
    }

    // 外部：统计 a[L..R] 中 <= x 的个数。
    int count_le(int L, int R, int x) const {
        int t = int(upper_bound(xs.begin(), xs.end(), x) - xs.begin());
        if (t <= 0) return 0;
        if (t >= M) return R - L + 1;

        int o = 1, l = 1, r = M, ans = 0;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (t <= mid) {
                o <<= 1;
                r = mid;
            } else {
                ans += local_range(o << 1, L, R);
                o = o << 1 | 1;
                l = mid + 1;
            }
        }
        ans += local_range(o, L, R);
        return ans;
    }

    int rank_of(int L, int R, int x) const {
        return count_lt(L, R, x) + 1;
    }

    // 外部：a[L..R] 的第 k 小实际值；k 从 1 开始。
    int kth_value(int L, int R, int k) const {
        assert(1 <= k && k <= R - L + 1);
        int o = 1, l = 1, r = M;
        while (l < r) {
            int mid = (l + r) >> 1;
            int left_count = local_range(o << 1, L, R);
            if (k <= left_count) {
                o <<= 1;
                r = mid;
            } else {
                k -= left_count;
                o = o << 1 | 1;
                l = mid + 1;
            }
        }
        return xs[l - 1];
    }

    int predecessor(int L, int R, int x, int none_value = -2147483647) const {
        int c = count_lt(L, R, x);
        return c ? kth_value(L, R, c) : none_value;
    }

    int successor(int L, int R, int x, int none_value = 2147483647) const {
        int c = count_le(L, R, x);
        return c < R - L + 1 ? kth_value(L, R, c + 1) : none_value;
    }
};
```

**P3380 最小接法（这一段可以直接照抄）：**

```cpp
vector<DynamicRangeOrderStatistic::Operation> ops;
// 先把 m 个操作全部读进 ops。

DynamicRangeOrderStatistic ds;
ds.build(a, ops);

for (auto op : ops) {
    if (op.type == 1) cout << ds.rank_of(op.a, op.b, op.c) << '\n';
    if (op.type == 2) cout << ds.kth_value(op.a, op.b, op.c) << '\n';
    if (op.type == 3) ds.modify(op.a, op.b);
    if (op.type == 4) cout << ds.predecessor(op.a, op.b, op.c) << '\n';
    if (op.type == 5) cout << ds.successor(op.a, op.b, op.c) << '\n';
}
```

这版的接口刻意和上一节静态主席树保持一致：遇到“静态/动态”的变化，只需要换数据结构，`rank_of / kth_value / predecessor / successor` 的调用习惯基本不变。
#### 普通 01 Trie：最大异或对与最大子段异或

> **赛时先看**
> - **题目信号**：题面出现 `xor` / `^`、最大异或值、最大子数组异或值，数值范围在 32 位整数内。
> - **本质**：维护一批非负整数，查询和给定数异或后的最大值；前缀异或配合它可求最大子段异或和。
> - **接法**：给长度 `n` 的数组，求 `max(a[l] xor ... xor a[r])`。
> - **复杂度判定**：插入、查询都是 `O(log V)`，这里为 31 层。
> - **维护的量**：`tr`（Trie 节点池，0 号为根）；每个节点 `ch[0]/ch[1]`（0/1 儿子编号）与 `cnt`（子树内插入次数）。
> - **警告**：最大子段异或要先插入前缀异或 `0`；若数据可能使用第 31 位，改成 `unsigned` 并把 `LOG` 改为 31。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个数，求两两异或的最大值（最大异或对）。

```cpp
BinaryTrie trie;
for (int x : a) trie.insert(x);                        // 1. 先把所有数插进去
int ans = 0;
for (int x : a) ans = max(ans, trie.max_xor(x));       // 2. 对每个数查与集合的最大异或
```

样例：`a=[3,10,5,25,2,8]` -> 输出 `28`（5 xor 25）。

**传参要求（照这个传不会错）：**

- `insert(x)`：插入非负整数，可重复插；默认 `LOG=30`，覆盖 `0..2^31-1`。
- `max_xor(x)`：返回 `x` 与已插入集合中某个数的最大异或值；**调用前先插入至少一个数**。
- 最大子段异或：直接调用同节 `max_subarray_xor(a)`，`a` 是 0-indexed 的 `vector<int>`，内部已先插入前缀异或 `0`。
- 数据可能用到第 31 位：把 `LOG` 改成 31 并换 `unsigned` 存储。

**API / 入口函数（赛时只认这里列的名字）：**

- `insert(int x)` -> 插入元素/字符串

```cpp
struct BinaryTrie {
    static const int LOG = 30;
    struct Node {
        int ch[2] = {0, 0};
        int cnt = 0;
    };
    vector<Node> tr{Node{}};

    void insert(int x) {
        int p = 0;
        tr[p].cnt++;
        for (int b = LOG; b >= 0; --b) {
            int c = (x >> b) & 1;
            if (!tr[p].ch[c]) {
                tr[p].ch[c] = (int)tr.size();
                tr.push_back(Node{});
            }
            p = tr[p].ch[c];
            tr[p].cnt++;
        }
    }

    int max_xor(int x) const {
        int p = 0, ans = 0;
        for (int b = LOG; b >= 0; --b) {
            int c = (x >> b) & 1;
            int want = c ^ 1;
            if (tr[p].ch[want] && tr[tr[p].ch[want]].cnt) {
                ans |= 1 << b;
                p = tr[p].ch[want];
            } else {
                p = tr[p].ch[c];
            }
        }
        return ans;
    }
};

int max_subarray_xor(const vector<int>& a) {
    BinaryTrie trie;
    trie.insert(0);
    int pre = 0, ans = 0;
    for (int x : a) {
        pre ^= x;
        ans = max(ans, trie.max_xor(pre));
        trie.insert(pre);
    }
    return ans;
}
```
#### 可持久化 01 Trie：区间最大异或

> **赛时先看**
> - **题目信号**：区间最大 xor；数组静态；可转成前缀异或 `pre[i]`。
> - **本质**：静态数组前缀异或，查询区间 `[l,r]` 内某个数与 `x` 的最大异或。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：插入/查询 `O(log V)`。
> - **维护的量**：`tr`（节点池：`ch[0]/ch[1]` 儿子、`cnt` 出现次数）；`root`（版本根数组，root[i] 是前缀 pre[1..i] 的版本）。
> - **警告**：查询 `[l,r]` 的数时用两个版本相减；最大子段异或常查询前缀范围 `[l-1,r-1]`。


**最小完整示例（先抄这一段就能跑）：**

题目：静态数组 `a[1..n]`，`m` 次询问 `[l,r]` 内某个数与 `x` 的最大异或。

```cpp
PersistentBinaryTrie pt;                   // 1. 构造：root[0] 是空版本
for (int i = 1; i <= n; ++i) pt.root.push_back(pt.insert(pt.root.back(), a[i]));
int L, R, x;
cin >> L >> R >> x;
cout << pt.query_max_xor(pt.root[L - 1], pt.root[R], x) << '\n';  // 2. 两版本相减查区间
```

样例：`a=[3,10,5]`；问 `[1,3]` 与 `8` 的最大异或 -> 输出 `13`（5 xor 8）。

**传参要求（照这个传不会错）：**

- `PersistentBinaryTrie pt`：直接构造；`root` 初始为 `{0}`（空版本），节点数多可传 `PersistentBinaryTrie(n * 31)` 预留。
- `insert(old_root, x)`：在旧版本上插 `x`，返回新版本根；按前缀逐个插入后 `pt.root.push_back(返回值)`。
- `query_max_xor(left_root, right_root, x)`：区间 `[l,r]` 内的数与 `x` 的最大异或；`left_root = root[l-1]`、`right_root = root[r]`（版本相减），返回最大异或值。
- 下标 1-based：`root[i]` 是前缀 `pre[1..i]` 的版本。
- 最大子段异或：版本建在前缀数组 `pre[0..n]` 上，枚举右端点 `j` 时查前缀范围 `[l-1, j-1]`（原区间 `[l,r]` 对应下标范围 `[l-1,r-1]`）。

**API / 入口函数（赛时只认这里列的名字）：**

- `insert(int old_root, int x)` -> 插入元素/字符串 返回 `int`。
**改板时先认这几个量：**

- `tr`：树节点池（节点数组）。
- `root`：版本根/树根数组。

```cpp
struct PersistentBinaryTrie {
    static const int LOG = 30;
    struct Node { int ch[2] = {0, 0}; int cnt = 0; };
    vector<Node> tr;
    vector<int> root;

    PersistentBinaryTrie(int reserve_nodes = 1) {
        tr.reserve(reserve_nodes);
        tr.push_back(Node{});
        root.push_back(0);
    }

    int clone(int p) {
        tr.push_back(tr[p]);
        return (int)tr.size() - 1;
    }

    int insert(int old_root, int x) {
        int nr = clone(old_root);
        int p = nr;
        tr[p].cnt++;
        for (int b = LOG; b >= 0; b--) {
            int c = (x >> b) & 1;
            int np = clone(tr[p].ch[c]);
            tr[p].ch[c] = np;
            p = np;
            tr[p].cnt++;
        }
        return nr;
    }

    int query_max_xor(int left_root, int right_root, int x) const {
        int ans = 0;
        int a = left_root, b = right_root;
        for (int bit = LOG; bit >= 0; bit--) {
            int c = (x >> bit) & 1;
            int want = c ^ 1;
            int cnt = tr[tr[b].ch[want]].cnt - tr[tr[a].ch[want]].cnt;
            if (cnt > 0) {
                ans |= 1 << bit;
                a = tr[a].ch[want];
                b = tr[b].ch[want];
            } else {
                a = tr[a].ch[c];
                b = tr[b].ch[c];
            }
        }
        return ans;
    }
};
```
#### Treap 平衡树

> **赛时先看**
> - **题目信号**：插入删除后还要查询排名/第 k 小，PBDS 不方便处理重复。
> - **本质**：动态维护排名、第 k 小、前驱后继，可处理重复值。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：期望 `O(log n)`。
> - **维护的量**：`root`（树根指针，insert/erase 用引用自更新）；节点 `val`（键值）、`pri`（随机优先级）、`cnt`（重复次数）、`sz`（子树总个数）。
> - **警告**：重复值用 `cnt`；旋转后要 `pull`。


**最小完整示例（先抄这一段就能跑）：**

题目：动态集合：插入/删除 `x`，查询 `x` 的排名与第 `k` 小（允许重复值）。

```cpp
Treap tp;
tp.insert(tp.root, 5);                 // 1. 插入 5（重复值自动累加 cnt）
tp.insert(tp.root, 3);
cout << tp.rank(tp.root, 5) << '\n';   // 2. 排名：返回 <= 5 的个数
cout << tp.kth(tp.root, 2) << '\n';    // 3. 第 k 小：k 从 1 开始
tp.erase(tp.root, 5);                  // 4. 删除一个 5
```

样例：插 5、3 后 `rank(root,5)=2`，`kth(root,2)=5`；删 5 后集合只剩 3。

**传参要求（照这个传不会错）：**

- `tp.insert(tp.root, v)`：`v` 是要插入的值；第一个参数固定传 `tp.root`（引用自更新）。
- `tp.erase(tp.root, v)`：删一个 `v`（重复值 cnt-1）；值不存在则无事发生。
- `tp.rank(tp.root, v)`：返回 `<= v` 的元素个数（含相等）；要“严格小于个数+1”式排名自己减/加调整。
- `tp.kth(tp.root, k)`：第 `k` 小，`k` 从 1 开始；`k` 超出集合总大小返回 `INT_MIN`。
- `tp.size(tp.root)`：集合总元素个数（含重复值）。

**API / 入口函数（赛时只认这里列的名字）：**

- `erase(Node*& t, int v)` -> 删除元素
- `insert(Node*& t, int v)` -> 插入元素/字符串
- `size(Node* t)` -> 查询集合大小 返回 `int`。
**改板时先认这几个量：**

- `sz`：平衡树子树大小。
- `root`：树根指针（insert/erase 用引用更新）。

```cpp
struct Treap {
    struct Node {
        int val, pri, cnt = 1, sz = 1;
        Node *l = nullptr, *r = nullptr;
        Node(int v, int p) : val(v), pri(p) {}
    };

    mt19937 rng{71236721};
    Node* root = nullptr;

    int size(Node* t) const { return t ? t->sz : 0; }
    void pull(Node* t) { if (t) t->sz = t->cnt + size(t->l) + size(t->r); }

    void rotate_left(Node*& t) {
        Node* x = t->r;
        t->r = x->l;
        x->l = t;
        pull(t); pull(x);
        t = x;
    }

    void rotate_right(Node*& t) {
        Node* x = t->l;
        t->l = x->r;
        x->r = t;
        pull(t); pull(x);
        t = x;
    }

    void insert(Node*& t, int v) {
        if (!t) {
            t = new Node(v, (int)rng());
            return;
        }
        if (v == t->val) t->cnt++;
        else if (v < t->val) {
            insert(t->l, v);
            if (t->l->pri > t->pri) rotate_right(t);
        } else {
            insert(t->r, v);
            if (t->r->pri > t->pri) rotate_left(t);
        }
        pull(t);
    }

    void erase(Node*& t, int v) {
        if (!t) return;
        if (v == t->val) {
            if (t->cnt > 1) t->cnt--;
            else if (!t->l || !t->r) {
                Node* old = t;
                t = t->l ? t->l : t->r;
                delete old;
            } else if (t->l->pri > t->r->pri) {
                rotate_right(t);
                erase(t->r, v);
            } else {
                rotate_left(t);
                erase(t->l, v);
            }
        } else if (v < t->val) erase(t->l, v);
        else erase(t->r, v);
        pull(t);
    }

    // rank(t, v) 返回的是 <= v 的元素个数（含相等）；
    // 需要“严格小于的个数+1”式排名请自行减/加调整。
    int rank(Node* t, int v) const {
        if (!t) return 0;
        if (v <= t->val) return rank(t->l, v);
        return size(t->l) + t->cnt + rank(t->r, v);
    }

    int kth(Node* t, int k) const {
        if (!t) return INT_MIN;
        if (k <= size(t->l)) return kth(t->l, k);
        if (k <= size(t->l) + t->cnt) return t->val;
        return kth(t->r, k - size(t->l) - t->cnt);
    }
};
```
#### 隐式 FHQ Treap：序列插入与第 k 个

> **赛时先看**
> - **题目信号**：题面有“在第 x 行/列后插入”“动态序列下标查询”；`q` 可到 `5e5`，数组中间插入不能直接 `vector`。
> - **本质**：维护动态序列，支持在第 `pos` 个元素后插入、删除、查询第 `k` 个元素。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：期望 `O(log n)`。
> - **维护的量**：`root`（当前序列根编号）；`tr`（节点池：`l/r` 儿子、`sz` 子树大小、`pri` 随机优先级、`val` 元素值）。
> - **警告**：`split(root, k, a, b)` 表示前 `k` 个进 `a`；插入到第 `pos` 个后就是按 `pos` 切开。


**最小完整示例（先抄这一段就能跑）：**

题目：动态序列：在第 `pos` 个元素后插入 `v`、删除第 `pos` 个、查询当前第 `k` 个元素。

```cpp
ImplicitTreap it;
for (int i = 1; i <= n; ++i) it.insert_after(it.size(it.root), a[i]);  // 1. 建初始序列（0-based 位置）
it.insert_after(2, 9);                 // 2. 在第 2 个元素后插入 9
it.erase_at(3);                        // 3. 删除当前第 3 个元素（1-based）
cout << it.kth(1) << '\n';             // 4. 查询当前第 1 个元素
```

样例：`a=[1,2,3]`；第 2 个后插 9 -> `[1,2,9,3]`；删第 3 个 -> `[1,2,3]`；`kth(1)` -> 输出 `1`。

**传参要求（照这个传不会错）：**

- `insert_after(pos, v)`：`pos` 是 0-based 计数（前 `pos` 个元素之后），可取 `0..当前长度`；末尾追加就传当前长度。
- `erase_at(pos)`：`pos` 是 1-based（当前序列第 `pos` 个元素），`1 <= pos <= 当前长度`。
- `kth(k)`：`k` 从 1 开始，`1 <= k <= 当前长度`；越界返回 `-1`。
- `split(root, k, a, b)`：前 `k` 个进 `a`，其余进 `b`；`merge(a, b)` 要求 `a` 序列整体在 `b` 之前。

**API / 入口函数（赛时只认这里列的名字）：**

- `merge(int a, int b)` -> 合并 返回 `int`。
- `size(int p)` -> 查询集合大小 返回 `int`。
**改板时先认这几个量：**

- `sz`：平衡树子树大小。
- `tr`：树节点池（节点数组）。
- `root`：序列根指针（split/merge 用引用更新）。

```cpp
struct ImplicitTreap {
    struct Node {
        int l = 0, r = 0, sz = 1, pri = 0;
        i64 val = 0;
    };
    vector<Node> tr{{}};
    mt19937 rng{(uint32_t)chrono::steady_clock::now().time_since_epoch().count()};
    int root = 0;

    int new_node(i64 v) {
        tr.push_back(Node{0, 0, 1, (int)rng(), v});
        return (int)tr.size() - 1;
    }

    int size(int p) const { return p ? tr[p].sz : 0; }

    void pull(int p) {
        if (p) tr[p].sz = size(tr[p].l) + size(tr[p].r) + 1;
    }

    void split(int p, int k, int& a, int& b) {
        if (!p) {
            a = b = 0;
            return;
        }
        if (size(tr[p].l) >= k) {
            b = p;
            split(tr[p].l, k, a, tr[p].l);
            pull(b);
        } else {
            a = p;
            split(tr[p].r, k - size(tr[p].l) - 1, tr[p].r, b);
            pull(a);
        }
    }

    int merge(int a, int b) {
        if (!a || !b) return a | b;
        if (tr[a].pri < tr[b].pri) {
            tr[a].r = merge(tr[a].r, b);
            pull(a);
            return a;
        }
        tr[b].l = merge(a, tr[b].l);
        pull(b);
        return b;
    }

    // insert_after 的 pos 是“前 pos 个元素之后”（0-based 计数，pos 可取 0..当前长度）。
    void insert_after(int pos, i64 v) {
        int a, b;
        split(root, pos, a, b);
        root = merge(merge(a, new_node(v)), b);
    }

    // erase_at 的 pos 是 1-based 位置（删除当前序列第 pos 个元素）。
    void erase_at(int pos) {
        int a, b, c;
        split(root, pos - 1, a, b);
        split(b, 1, b, c);
        root = merge(a, c);
    }

    i64 kth(int k) const {
        int p = root;
        while (p) {
            int left_size = size(tr[p].l);
            if (k == left_size + 1) return tr[p].val;
            if (k <= left_size) p = tr[p].l;
            else {
                k -= left_size + 1;
                p = tr[p].r;
            }
        }
        return -1;
    }
};
```
#### 左偏树：可并堆

> **赛时先看**
> - **题目信号**：多个集合各有堆，操作会合并堆并查询集合最小/最大。
> - **本质**：维护可合并优先队列，支持合并两个堆、取最小值、删除最小值。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：合并/删除 `O(log n)` 均摊。
> - **维护的量**：`tr`（节点池：`l/r` 儿子、`val` 值、`dist` 到最近空节点的距离）；堆根编号即堆顶。
> - **警告**：删除节点后要合并它的左右儿子；如果节点已被删除，要先找到堆根。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个小根堆（每点自成一堆），支持合并两堆、取堆顶、删堆顶。

```cpp
LeftistHeap h(n);
for (int i = 1; i <= n; ++i) cin >> h.tr[i].val;  // 1. 每个点自成一堆（小根堆）
int r = 1;
r = h.merge(r, 2);                   // 2. 合并两堆，返回新堆根
cout << h.tr[r].val << '\n';         // 3. 堆顶即最小值
r = h.pop(r);                        // 4. 删堆顶，r 更新为新堆根
```

样例：`val(1)=5, val(2)=3` -> 合并后堆顶 `3`；pop 后堆顶 `5`。

**传参要求（照这个传不会错）：**

- `LeftistHeap h(n)`：`n` 为节点上限；节点从 1 开始编号，直接给 `h.tr[i].val` 赋值（0 号节点为空）。
- `merge(x, y)`：合并两个堆，返回新堆根；`x/y` 传堆根编号，0 表示空堆。
- `pop(root)`：删堆顶（根），返回合并左右儿子后的新根；删某个节点 `x` 时先 `merge(h.tr[x].l, h.tr[x].r)` 再和所在堆合并，已删过的节点要先找到堆根。
- 小根堆按 `val` 小的在上；要大根堆把 `merge` 里的 `>` 改成 `<`。

**API / 入口函数（赛时只认这里列的名字）：**

- `merge(int x, int y)` -> 合并 返回 `int`。
**改板时先认这几个量：**

- `tr`：堆节点池（节点数组）。
- `root`：当前堆根节点编号（pop(root) 会更新它）。
- `dist`：距离。

```cpp
struct LeftistHeap {
    struct Node {
        int l = 0, r = 0, dist = 0;
        i64 val = 0;
    };
    vector<Node> tr;

    LeftistHeap(int n = 0) { tr.resize(n + 1); }

    int merge(int x, int y) {
        if (!x || !y) return x | y;
        if (tr[x].val > tr[y].val) swap(x, y); // 小根堆。
        tr[x].r = merge(tr[x].r, y);
        if (tr[tr[x].l].dist < tr[tr[x].r].dist) swap(tr[x].l, tr[x].r);
        tr[x].dist = tr[tr[x].r].dist + 1;
        return x;
    }

    int pop(int root) {
        return merge(tr[root].l, tr[root].r);
    }
};
```
#### Link-Cut Tree：动态森林

> **赛时先看**
> - **题目信号**：树边动态加删；查询两点路径 xor/sum/max；要求在线。
> - **本质**：维护动态森林路径信息，支持 link、cut、路径查询/修改。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：单次 `O(log n)` 均摊。
> - **维护的量**：`tr`（节点池：`ch[0]/ch[1]` 左右儿子、`fa` 父指针、`val` 点权、`xr` 当前 splay 内路径异或和、`rev` 翻转标记）。
> - **警告**：所有访问前先 `makeroot`；`cut` 要确认确实有边；pushdown 顺序从祖先到当前。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个点的森林，动态加边/删边，随时询问两点路径上点权的异或和（在线）。

```cpp
LinkCutTree lct(n);                              // 1. 构造即 init(n)
for (int i = 1; i <= n; ++i) cin >> lct.tr[i].val;  // 2. 赋点权
lct.link(u, v);                                  // 3. 加边 (u,v)
lct.cut(u, v);                                   // 4. 删边 (u,v)
cout << lct.query_xor(x, y) << '\n';             // 5. 路径 x-y 的点权异或和
```

样例：三个点 val=1,2,3，link(1,2)、link(2,3) 后 `query_xor(1,3)` -> 输出 `0`（1^2^3=0）。

**传参要求（照这个传不会错）：**

- `LinkCutTree lct(n)`：构造时自动 `init(n)`；点编号 1..n。
- 改点权：先 `lct.makeroot(x); lct.access(x); lct.splay(x);`，再赋 `tr[x].val`，最后 `lct.push_up(x)`。
- `link(x, y)`：加边；两点已连通则内部跳过。
- `cut(x, y)`：删边；内部判断确认 (x,y) 间确实有边才删。
- `query_xor(x, y)`：返回路径 `x-y` 上所有点权的异或和，点编号 1-based 直接传。
- `makeroot(x)`：把 x 变成所在树的根；`findroot(x)` 返回 x 所在树根，判连通用 `findroot(a)==findroot(b)`。

**API / 入口函数（赛时只认这里列的名字）：**

- `cut(int x, int y)` -> 删除边 (x,y)
- `findroot(int x)` -> 查询 x 所在树的根
- `init(int n)` -> 初始化/清空结构
- `link(int x, int y)` -> 连接边 (x,y)
- `makeroot(int x)` -> 把 x 变为所在树的根
- `query_xor(int x, int y)` -> 查询路径 x-y 的异或和 返回 `int`。
**改板时先认这几个量：**

- `tr`：树节点池（节点数组）。
- `fa`：父节点/并查集父亲。

```cpp
struct LinkCutTree {
    struct Node {
        int ch[2] = {0, 0}, fa = 0;
        int val = 0, xr = 0;
        bool rev = false;
    };
    vector<Node> tr;

    LinkCutTree(int n = 0) { init(n); }
    void init(int n) { tr.assign(n + 1, Node{}); }

    bool is_root(int x) {
        int f = tr[x].fa;
        return !f || (tr[f].ch[0] != x && tr[f].ch[1] != x);
    }

    void push_up(int x) {
        tr[x].xr = tr[tr[x].ch[0]].xr ^ tr[x].val ^ tr[tr[x].ch[1]].xr;
    }

    void apply_rev(int x) {
        if (!x) return;
        swap(tr[x].ch[0], tr[x].ch[1]);
        tr[x].rev ^= 1;
    }

    void push_down(int x) {
        if (tr[x].rev) {
            apply_rev(tr[x].ch[0]);
            apply_rev(tr[x].ch[1]);
            tr[x].rev = false;
        }
    }

    void rotate(int x) {
        int y = tr[x].fa, z = tr[y].fa;
        int k = (tr[y].ch[1] == x), w = tr[x].ch[k ^ 1];
        if (!is_root(y)) tr[z].ch[tr[z].ch[1] == y] = x;
        tr[x].fa = z;
        tr[x].ch[k ^ 1] = y; tr[y].fa = x;
        tr[y].ch[k] = w; if (w) tr[w].fa = y;
        push_up(y); push_up(x);
    }

    void splay(int x) {
        static vector<int> st;
        st.clear();
        int y = x;
        st.push_back(y);
        while (!is_root(y)) {
            y = tr[y].fa;
            st.push_back(y);
        }
        while (!st.empty()) {
            push_down(st.back());
            st.pop_back();
        }
        while (!is_root(x)) {
            int y = tr[x].fa, z = tr[y].fa;
            if (!is_root(y)) {
                bool zigzig = (tr[y].ch[0] == x) == (tr[z].ch[0] == y);
                rotate(zigzig ? y : x);
            }
            rotate(x);
        }
    }

    void access(int x) {
        for (int y = 0; x; y = x, x = tr[x].fa) {
            splay(x);
            tr[x].ch[1] = y;
            push_up(x);
        }
    }

    void makeroot(int x) {
        access(x);
        splay(x);
        apply_rev(x);
    }

    int findroot(int x) {
        access(x);
        splay(x);
        while (tr[x].ch[0]) {
            push_down(x);
            x = tr[x].ch[0];
        }
        splay(x);
        return x;
    }

    void link(int x, int y) {
        makeroot(x);
        if (findroot(y) != x) tr[x].fa = y;
    }

    void cut(int x, int y) {
        makeroot(x);
        access(y);
        splay(y);
        if (tr[y].ch[0] == x && !tr[x].ch[1]) {
            tr[y].ch[0] = tr[x].fa = 0;
            push_up(y);
        }
    }

    int query_xor(int x, int y) {
        makeroot(x);
        access(y);
        splay(y);
        return tr[y].xr;
    }
};
```
#### 线性基

> **赛时先看**
> - **题目信号**：题面出现 xor，要求选若干数使异或值最大/可达。
> - **本质**：异或最大值、判断某个数能否由子集异或得到。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：插入/查询 `O(log V)`。
> - **维护的量**：`b[i]`（最高位为 i 的基向量，0 表示该位为空）。
> - **警告**：`LOG` 要覆盖数据范围；`i64` 通常用 62。


**最小完整示例（先抄这一段就能跑）：**

题目：给 `n` 个数，求任意子集异或的最大值，并判断某个数能否被子集异或表示。

```cpp
LinearBasis lb;
for (int i = 0; i < n; ++i) lb.insert(a[i]);     // 1. 全部插入（重复/线性相关会自动跳过）
cout << lb.max_xor() << '\n';                    // 2. 子集异或最大值（空集视为 0）
cout << (lb.can_make(x) ? "YES" : "NO") << '\n'; // 3. 判断 x 可否由子集异或得到
```

样例：`a=[1,2,4]` -> `max_xor()=7`（1^2^4），`can_make(3)=true`（1^2）。

**传参要求（照这个传不会错）：**

- `insert(x)`：插入一个数，返回 bool（true=真正放进基里，false=线性相关）；重复插入没关系。
- `max_xor(start=0)`：从 `start` 出发异或基向量能得到的最大值；直接 `max_xor()` 即全集合的最大子集异或。
- `can_make(x)`：判断 `x` 能否由已插入的子集异或得到（空集不算，除非 `x=0`）。
- 默认 `LOG=62` 覆盖 i64 全部 63 位；数据在 int 范围内可不动；超过 i64 本节不适用。

**API / 入口函数（赛时只认这里列的名字）：**

- `insert(i64 x)` -> 插入元素/字符串 返回 `bool`。

```cpp
struct LinearBasis {
    static const int LOG = 62;
    i64 b[LOG + 1]{};

    bool insert(i64 x) {
        for (int i = LOG; i >= 0; --i) {
            if (((x >> i) & 1) == 0) continue;
            if (!b[i]) {
                b[i] = x;
                return true;
            }
            x ^= b[i];
        }
        return false;
    }

    i64 max_xor(i64 start = 0) const {
        i64 ans = start;
        for (int i = LOG; i >= 0; --i) ans = max(ans, ans ^ b[i]);
        return ans;
    }

    bool can_make(i64 x) const {
        for (int i = LOG; i >= 0; --i) {
            if (((x >> i) & 1) && b[i]) x ^= b[i];
        }
        return x == 0;
    }
};
```
#### PBDS 有序树

> **赛时先看**
> - **题目信号**：需要插入删除后仍查询排名/第 k 小。
> - **本质**：动态第 k 小、排名。
> - **复杂度判定**：`O(log n)`。
> - **维护的量**：`os`（红黑树容器）；`tree_order_statistics_node_update` 自动维护子树大小，order_of_key/find_by_order 都依赖它。
> - **警告**：`tree` 不支持重复元素，重复可存 `{value, unique_id}`。
> - **约定**：os.find_by_order(k): 0-based 第 k 小的迭代器


**最小完整示例（先抄这一段就能跑）：**

题目：动态插入/删除数字，问 `x` 的排名（严格小于的个数）与第 `k` 小（0-based）。

```cpp
OrderedSet<int> os;
os.insert(5); os.insert(3); os.insert(8);      // 1. 插入（重复键只存一份）
cout << os.order_of_key(5) << '\n';            // 2. 严格小于 5 的个数 = 1
auto it = os.find_by_order(2);                 // 3. 0-based 第 2 小 = 第 3 小
cout << (it == os.end() ? -1 : *it) << '\n';   // 4. 输出 8
```

样例：插 `{5,3,8}` -> `order_of_key(5)=1`，`find_by_order(2)=8`。

**传参要求（照这个传不会错）：**

- `os.insert(x)` / `os.erase(x)`：直接传值；**重复键只存一份**，要存重复值改用 `pair<value, unique_id>`（如 `{x, ++cnt}`）。
- `os.order_of_key(x)`：返回严格小于 `x` 的元素个数，即 0-based 排名；要 1-based 排名自己加 1。
- `os.find_by_order(k)`：0-based 第 `k` 小（第 1 小传 0）；越界返回 `os.end()`，取值前先判 `!= os.end()`。
- `*it` 取键值；迭代器可直接 `++/--` 找前后继。

**不会用就照抄：**

```cpp
OrderedSet<int> os;
os.insert(x);
int rank0 = os.order_of_key(x); // 严格小于 x 的数量
auto it = os.find_by_order(k);  // 0-based 第 k 小；第 1 小传 k=0
```

- PBDS 原生 `tree<int,...>` 不保存重复值；要存重复值，用 `pair<value, unique_id>`。


**API / 入口函数（赛时只认这里列的名字）：**

- `OrderedSet<T> os` -> 建立不允许重复键的有序树。
- `os.order_of_key(x)` -> 严格小于 x 的元素个数，也就是 0-based rank。
- `os.find_by_order(k)` -> 返回 0-based 第 k 小的迭代器；第 1 小传 k=0。

```cpp
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template <class T>
using OrderedSet = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// os.find_by_order(k): 0-based 第 k 小的迭代器
// os.order_of_key(x): 小于 x 的元素个数
```
#### CDQ 分治：三维偏序计数骨架

> **赛时先看**
> - **题目信号**：三维偏序、动态逆序对、按时间分治处理贡献。
> - **本质**：统计每个点前面有多少点满足 `x<=X, y<=Y, z<=Z`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n log^2 n)`（或 `O(n log n)`，取决于实现）。
> - **维护的量**：`a[i]`（点：x/y/z 三维 + id）；`ans[id]`（每个点的累计贡献）；`fw`（FenwickCompact，维护 z 维计数）。
> - **警告**：先按第一维排序；CDQ 中按第二维归并，用 BIT 维护第三维。


**最小完整示例（先抄这一段就能跑）：**

题目：`n` 个三维点，统计每个点前面满足 `x<=X, y<=Y, z<=Z` 的点数（三维偏序）。

```cpp
vector<Point3D> a(n);                  // 1. 读入 x,y,z 并赋 id=i
sort(a.begin(), a.end(), [](auto& p, auto& q) { return p.x < q.x; });  // 2. 按 x 排序
vector<i64> ans(n);
FenwickCompact<int> fw(m);             // 3. z 压缩成 1..m 后当 BIT 下标
cdq_3d(a, 0, n, fw, ans);              // 4. CDQ：按 y 归并、BIT 维护 z
for (int i = 0; i < n; ++i) cout << ans[i] << '\n';   // 5. ans[id] 即该点的答案
```

样例：三点 `(1,1,1),(2,2,2),(1,2,2)`（id 0,1,2）-> `ans=[0,1,2]`。

**传参要求（照这个传不会错）：**

- 调用前先把点按 `x` 升序排进 `a`（`a[0..n-1]`，区间 `[l,r)` 左闭右开）；`z` 先离散化成 `1..m`。
- `cdq_3d(a, 0, n, fw, ans)`：对全数组调用一次即可；`ans` 用 `a[i].id` 索引，需先 `resize(n)` 并清零。
- `fw` 用 `FenwickCompact<int>`，大小 `m`（z 的最大编号），调用前 `fw.init(m)` 或新开一个。
- 骨架计数语义是 `x<=X`（同 x 也会计入）；题目要求“严格小于”时排序键用 `(x,y,z)` 并按同 x 分组处理。

**API / 入口函数（赛时只认这里列的名字）：**

- `cdq_3d(a, l, r, fw, ans)` -> 对 `a[l..r)` 做 CDQ 分治，贡献累加进 `ans`。
- **警告**：`z` 坐标需要先离散化压缩成 `1..m` 才能直接作为 BIT 下标；`x` 维要先按第一维排序。
**改板时先认这几个量：**

- `bit`：FenwickCompact 内部树状数组。
- `sum`：区间和/计数和。

```cpp
struct Point3D {
    int x, y, z, id;
    i64 ans = 0;
};

template <class T>
struct FenwickCompact {
    int n;
    vector<T> bit;
    FenwickCompact(int n = 0) { init(n); }
    void init(int n_) { n = n_; bit.assign(n + 1, T{}); }
    void add(int i, T v) { for (; i <= n; i += i & -i) bit[i] += v; }
    T sum(int i) const { T r{}; for (; i > 0; i -= i & -i) r += bit[i]; return r; }
};

void cdq_3d(vector<Point3D>& a, int l, int r, FenwickCompact<int>& fw, vector<i64>& ans) {
    if (r - l <= 1) return;
    int m = (l + r) >> 1;
    cdq_3d(a, l, m, fw, ans);
    cdq_3d(a, m, r, fw, ans);
    vector<Point3D> left(a.begin() + l, a.begin() + m);
    vector<Point3D> right(a.begin() + m, a.begin() + r);
    sort(left.begin(), left.end(), [](auto& p, auto& q) { return p.y < q.y; });
    sort(right.begin(), right.end(), [](auto& p, auto& q) { return p.y < q.y; });
    int i = 0;
    for (auto& p : right) {
        while (i < (int)left.size() && left[i].y <= p.y) {
            fw.add(left[i].z, 1);
            i++;
        }
        ans[p.id] += fw.sum(p.z);
    }
    for (int j = 0; j < i; ++j) fw.add(left[j].z, -1);
}
```
#### 整体二分 Parallel Binary Search

> **赛时先看**
> - **题目信号**：第 k 小、最早满足条件时间、离线可加不可删。
> - **本质**：多询问，每个询问答案可二分，修改按时间/权值逐步加入。
> - **复杂度判定**：`O((n+q) log V * 单次操作）`。
> - **维护的量**：`updates`（候选修改，按 value 决定加入）；`queries`（带 id 的询问）；`ans`（答案数组）；外加 `apply/undo/check` 三个回调与每层的 `applied`（本层已加修改，用于撤销）。
> - **警告**：每层递归要撤销当前加入的修改，或用可重置数据结构。


**最小完整示例（先抄这一段就能跑）：**

题目：静态数组 `a[1..n]`，`m` 个询问 `[l,r]` 内第 `k` 小（答案值域 1..V，整体二分）。

```cpp
struct Update { int pos, value; };            // 每个元素 = 一个修改（在 pos 处加入一个 value）
struct Query { int l, r, k, id; };
FenwickCompact<int> fw(n);                    // 计数 BIT：可借本节 CDQ 的 FenwickCompact
auto apply = [&](Update u) { fw.add(u.pos, 1); };
auto undo  = [&](Update u) { fw.add(u.pos, -1); };
auto check = [&](Query q) { return fw.sum(q.r) - fw.sum(q.l - 1) >= q.k; };
parallel_bs(1, V, updates, queries, ans, apply, undo, check);   // ans[q.id] 即第 k 小
```

样例：`a=[1,5,2,4,3]`；询问 `[2,5]` 第 2 小 -> `ans[0]=2`（区间 5,2,4,3 中第 2 小是 2）。

**传参要求（照这个传不会错）：**

- `updates`：每个候选“修改”必须有 `value` 字段（代码按 `u.value <= mid` 判定是否加入），其余字段在 `apply/undo` 里用。
- `queries`：每个询问必须有 `id` 字段（答案写进 `ans[q.id]`）；`check(q)` 返回“当前已加入的修改下，答案 <= mid 是否成立”。
- `apply/undo`：必须成对且完全可逆；每层结束会撤销全部 `applied`，递归两半都拿完整 `updates` 重来。
- `parallel_bs(l, r, updates, queries, ans, apply, undo, check)`：`[l,r]` 为答案值域（闭区间），先按此调用一次；`ans` 需 `resize` 成询问个数。
- 无解询问（如区间元素不足 k 个）最后会落到 `ans=r`，题目要输出 `-1` 时自行判。


```cpp
// 骨架：把询问按 mid 判定分到左/右。
// Update 和 Query 需要按题目自定义。
template <class Update, class Query, class Apply, class Undo, class Check>
void parallel_bs(int l, int r,
                 vector<Update>& updates,
                 vector<Query>& queries,
                 vector<int>& ans,
                 Apply apply,
                 Undo undo,
                 Check check) {
    if (queries.empty()) return;
    if (l == r) {
        for (auto& q : queries) ans[q.id] = l;
        return;
    }
    int mid = (l + r) >> 1;
    vector<Query> left, right;
    vector<Update> applied;
    for (auto& u : updates) {
        if (u.value <= mid) {
            apply(u);
            applied.push_back(u);
        }
    }
    for (auto& q : queries) {
        if (check(q)) left.push_back(q);
        else right.push_back(q);
    }
    for (auto it = applied.rbegin(); it != applied.rend(); ++it) undo(*it);
    parallel_bs(l, mid, updates, left, ans, apply, undo, check);
    parallel_bs(mid + 1, r, updates, right, ans, apply, undo, check);
}
```

#### 离线动态连通性：线段树分治 + 可撤销并查集

> **赛时先看**
> - **题目信号**：动态删边；在线做很难，但所有操作可提前读入。
> - **本质**：边会加入/删除，离线回答每个时刻连通性。
> - **接法**：先构造 `OfflineDynamicConnectivity odc(n, q)`；每条边按存在时间段 `odc.add_interval(l, r, u, v)`；最后 `odc.solve(1, 1, q, lambda)`，在 lambda 里用 `dsu.find(x) == dsu.find(y)` 回答该时刻的询问。
> - **复杂度判定**：`O((m log q) alpha(n))`。
> - **维护的量**：`seg`（时间线段树，每个节点存覆盖该时间段的边表）；`dsu`（可撤销并查集 `HistoryDSU`：`p/sz/hist/comps`）。
> - **警告**：把每条边的存在时间段插入时间线段树；递归退出时回滚 DSU。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// n=3 个点，q=4 个时刻：边 (1,2) 在时刻 1..3 存在，(2,3) 在时刻 2..4 存在。
OfflineDynamicConnectivity odc(3, 4);
odc.add_interval(1, 3, 1, 2);
odc.add_interval(2, 4, 2, 3);
vector<int> ans(5);
odc.solve(1, 1, 4, [&](int t, const HistoryDSU& dsu) {
    ans[t] = dsu.find(1) == dsu.find(3); // 时刻 t 时 1、3 是否连通
});
cout << ans[1] << ' ' << ans[2] << ' ' << ans[3] << ' ' << ans[4] << '\n'; // 0 1 1 0
```

- 样例：输出 `0 1 1 0`（时刻 2、3 时 1 和 3 连通）。

**传参要求（照这个传不会错）：**

- `OfflineDynamicConnectivity(n, q)`：`n` = 点数（编号 `1..n`），`q` = 时刻总数（时间轴 `1..q`）。
- `add_interval(l, r, u, v)`：边 `(u,v)` 在时刻闭区间 `[l,r]` 内存在；调用顺序任意。
- `solve(1, 1, q, answer_leaf)`：必须从根 `1`、区间 `[1, q]` 开始；每个叶子时刻 `t` 会回调 `answer_leaf(t, dsu)`，在回调里回答询问（只读 `find`，不要改 `dsu`）。
- `HistoryDSU`：`find/snapshot/unite/rollback`；供 `solve` 内部使用，一般不在外面直接调。


**API / 入口函数（赛时只认这里列的名字）：**

- `add_interval(int l, int r, int u, int v)` -> 边 (u,v) 在时刻闭区间 [l,r] 内存在，加入时间线段树。
- `solve(1, 1, q, answer_leaf)` -> 分治遍历时刻，在叶子时刻调用 `answer_leaf(t, dsu)` 回答该时刻的询问。
**改板时先认这几个量：**

- `sz`：并查集按大小合并的大小数组。
- `dsu`：可撤销并查集实例（HistoryDSU）。

```cpp
// 维护的量：p（父节点，不路径压缩以支持回滚）；sz（按大小合并的子树大小）；
//          hist（合并记录栈，rollback 时按逆序复原）；comps（当前连通块数）。
// 不变量：hist.size() 就是当前“版本号”；rollback(snap) 后状态与 snapshot() 时完全一致。
struct HistoryDSU {
    vector<int> p, sz;
    vector<pair<int, int>> hist;
    int comps;
    HistoryDSU(int n = 0) { init(n); }
    void init(int n) { p.resize(n + 1); sz.assign(n + 1, 1); iota(p.begin(), p.end(), 0); hist.clear(); comps = n; }
    int find(int x) const { while (p[x] != x) x = p[x]; return x; }
    int snapshot() const { return (int)hist.size(); }
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) { hist.push_back({-1, -1}); return false; }
        if (sz[a] < sz[b]) swap(a, b);
        hist.push_back({b, sz[a]}); // 记录被挂到 a 下的 b 与 a 的原大小，供回滚复原
        p[b] = a; sz[a] += sz[b]; comps--;
        return true;
    }
    void rollback(int snap) {
        while ((int)hist.size() > snap) {
            auto [b, old] = hist.back(); hist.pop_back();
            if (b == -1) continue;
            int a = p[b];
            p[b] = b; sz[a] = old; comps++; // 逆序撤销一次合并
        }
    }
};

// 维护的量：seg（时间线段树，seg[p] 存覆盖节点 p 时间段的全部边）；dsu（可撤销并查集）。
// 不变量：solve 递归到节点 p 时，dsu 中恰好装着根到 p 路径上所有时间段内的边；
//         离开节点时 rollback，保证兄弟子树互不影响。
struct OfflineDynamicConnectivity {
    int q;
    vector<vector<pair<int, int>>> seg;
    HistoryDSU dsu;

    OfflineDynamicConnectivity(int n, int q_) : q(q_), seg(4 * q_ + 4), dsu(n) {}

    void add_interval_impl(int p, int l, int r, int ql, int qr, pair<int, int> e) {
        if (ql <= l && r <= qr) {
            seg[p].push_back(e); // 当前段完全被 [ql,qr] 覆盖：边挂在这个节点上
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) add_interval_impl(p << 1, l, mid, ql, qr, e);
        if (qr > mid) add_interval_impl(p << 1 | 1, mid + 1, r, ql, qr, e);
    }

    // 外部入口：边 (u,v) 在时刻闭区间 [l,r] 内存在。
    void add_interval(int l, int r, int u, int v) {
        add_interval_impl(1, 1, q, l, r, {u, v});
    }

    template <class AnswerLeaf>
    void solve(int p, int l, int r, AnswerLeaf answer_leaf) {
        int snap = dsu.snapshot();
        for (auto [u, v] : seg[p]) dsu.unite(u, v); // 本节点覆盖时间段的边全部生效
        if (l == r) {
            answer_leaf(l, dsu); // 叶子时刻：回调回答询问
        } else {
            int mid = (l + r) >> 1;
            solve(p << 1, l, mid, answer_leaf);
            solve(p << 1 | 1, mid + 1, r, answer_leaf);
        }
        dsu.rollback(snap); // 递归退出时回滚，回到进入本节点前的状态
    }
};
```

