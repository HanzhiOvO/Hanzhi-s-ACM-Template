# Hanzhi's Template (V1.1)

## By Hanzhi  2026.3.13

## 0x01 基础算法

### 1.1 头文件
```cpp
// LANG:C++

// Auther:Hanzhi

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;
typedef pair<ll,ll> P;
typedef pair<int,int> pii;
#define endl '\n'
#define hajimi ios::sync_with_stdio(false);
#define nanbei cin.tie(nullptr),
#define lvdou cout.tie(nullptr);

void solve()
{
}

int main() {
    hajimi nanbei lvdou
    int T;
    cin>>T;
    while(T--) solve();
    return 0;
}
```

### 1.2 常用函数
```cpp
ll mysqrt(ll n) //用于long long的sqrt函数
{
    ll res=sqrt(n);
    while((res+1)*(res+1)<=n) res++;
    while((res*res>n)) res--;
    return res;
}

int mylcm(int x,int y)
{
    return x/gcd(x,y)*y;
}
```

### 1.3 最大公约数GCD
注意：速度远不如内置函数快，此处时间复杂度为$O(log(a+b))$
```cpp
inline int mygcd(int a, int b) { return b ? gcd(b, a % b) : a; }
```

### 1.4 整数域二分
在递增序列$a$中查找$>=x$的数字中最小的一个（即$x$或$x$的后继）
```cpp
int l=0,r=1E8,ans=r;

while(l<=r) {
    int mid = (l + r) / 2;
    if (judge(mid)) {
        r = mid - 1;
        ans = mid;
    } else {
        l = mid + 1;
    }
}
return ans;
```
在递增序列$a$中查找$<=x$的数字中最大的一个（即$x$或$x$的前驱）
```cpp
int l=0,r=1E8,ans=l;
while(l<=r) {
    int mid = (l + r) / 2;
    if (judge(mid)) {
        l = mid + 1;
        ans = mid;
    } else {
        r = mid - 1;
    }
}
return ans;
```

### 1.5 实数域二分
通过限制二分次数实现
```cpp
for(int t=1;t<=100;t++)
{
	float mid=(l+r)/2;
	if(judge(mid)) r=mid;
	else l=mid;
}
cout<<l<<endl;
```

### 1.6 整数域三分
```cpp
while(l<r)
{
	int mid=(l+r)/2;
	if(check(mid)<=check(mid+1)) r=mid;
	else l=mid+1;
}
cout<<check(l)<<endl;
```

### 1.7 实数域三分
```cpp
ld l = -1E9, r = 1E9; 
for (int t = 1; t <= 100; t++) 
{ 
	ld mid1 = (l * 2 + r) / 3; 
	ld mid2 = (l + r * 2) / 3; 
	if (judge(mid1) < judge(mid2)) { 
		r = mid2; 
	} 
	else { 
		l = mid1; 
	} 
} 
cout << l << endl;
```



## 0x02 数据结构

### 2.1 并查集

两大操作：1.合并两个集合 2.询问两个元素是否在同一个集合中。可以做到近乎 $O(1)$ 的时间复杂度内维护。用树存储各个集合，对于某一个元素$x$，通过不断询问它上一层的祖先可以实现路径压缩。

合并的时候对于集合$x$和$y$，直接$p[x]=y$即可。

基础板子：

```cpp
const int N=100010;

int p[N]; // 存储父节点
int n,m; // 节点数 操作数

int find(int x){ // 返回节点x的祖宗节点（即集合编号）
    if(p[x]!=x) p[x]=find(p[x]);
    return p[x];
}

void solve()
{
    cin>>n>>m;
    for(int i=1;i<=n;i++) p[i]=i;
    while(m--)
    {
        char op[2]; // 操作
        cin>>op;
        int a,b; //操作数 a b
        cin>>a>>b;
        if(op[0]=='M'){ // 合并操作
        	p[find(a)]=find(b);
        }
        else if(op[0]=='C'){ // 判断两个元素是不是同一个集合
            if(find(a)==find(b)) cout<<"YES"<<endl;
            else cout<<"NO"<<endl;
        }
    }
}
```

进阶板子：

```cpp
struct DSU { 
	vector<int> fa, p, e, f;
	DSU(int n) { 
		fa.resize(n + 1); 
		iota(fa.begin(), fa.end(), 0); 
		p.resize(n + 1, 1); 
		e.resize(n + 1); 
		f.resize(n + 1); 
	} 
	int get(int x) 
	{ 
		while (x != fa[x]) { 
			x = fa[x] = fa[fa[x]]; 
		} 
		return x; 
	} 
	bool merge(int x, int y) { // 设x是y的祖先 
		if (x == y) f[get(x)] = 1; 
		x = get(x), y = get(y); 
		e[x]++; 
		if (x == y) return false; 
		if (x < y) swap(x, y); // 将编号小的合并到大的上 
		fa[y] = x; 
		f[x] |= f[y], p[x] += p[y], e[x] += e[y]; 
		return true; 
	} 
	bool same(int x, int y) { 
		return get(x) == get(y); 
	} 
	bool F(int x) { // 判断连通块内是否存在自环 
		return f[get(x)]; 
	} 
	int size(int x) { // 输出连通块中点的数量 
		return p[get(x)]; 
	} 
	int E(int x) { // 输出连通块中边的数量 
	return e[get(x)]; 
	} 
};
```

### 2.2 Trie树

字典树。下面是一个基础的封装。

```cpp
struct Trie {
	int ch[N][63], cnt[N], idx = 0;
	map<char, int> mp;
	void init() {
		LL id = 0;
		for (char c = 'a'; c <= 'z'; c++) mp[c] = ++id;
		for (char c = 'A'; c <= 'Z'; c++) mp[c] = ++id;
		for (char c = '0'; c <= '9'; c++) mp[c] = ++id;
	}
	void insert(string s) {
		int u = 0;
		for (int i = 0; i < s.size(); i++) {
			int v = mp[s[i]];
			if (!ch[u][v]) ch[u][v] = ++idx;
			u = ch[u][v];
			cnt[u]++;
		}
	}
	LL query(string s) {
		int u = 0;
		for (int i = 0; i < s.size(); i++) {
			int v = mp[s[i]];
			if (!ch[u][v]) return 0;
			u = ch[u][v];
    	}
		return cnt[u];
	}
	void Clear() {
		for (int i = 0; i <= idx; i++) {
			cnt[i] = 0;
			for (int j = 0; j <= 62; j++) {
				ch[i][j] = 0;
			}
		}
		idx = 0;
	}
} trie;

```

### 2.3 线段树

**建树：**

```cpp
void build(int s, int t, int p) {
  	// 对 [s,t] 区间建立线段树,当前根的编号为 p
  	if (s == t) {
    	d[p] = a[s];
    	return;
  	}
  	int m = s + ((t - s) >> 1);
  	// 移位运算符的优先级小于加减法，所以加上括号
  	// 如果写成 (s + t) >> 1 可能会超出 int 范围
  	build(s, m, p * 2), build(m + 1, t, p * 2 + 1);
  	// 递归对左右区间建树
    d[p] = d[p * 2] + d[(p * 2) + 1];
}
```

**区间查询：**

```cpp
int getsum(int l, int r, int s, int t, int p) {
  	// [l, r] 为查询区间, [s, t] 为当前节点包含的区间, p 为当前节点的编号
  	if (l <= s && t <= r)
    	return d[p];  // 当前区间为询问区间的子集时直接返回当前区间的和
  	int m = s + ((t - s) >> 1), sum = 0;
  	if (l <= m) sum += getsum(l, r, s, m, p * 2);
  	// 如果左儿子代表的区间 [s, m] 与询问区间有交集, 则递归查询左儿子
  	if (r > m) sum += getsum(l, r, m + 1, t, p * 2 + 1);
  	// 如果右儿子代表的区间 [m + 1, t] 与询问区间有交集, 则递归查询右儿子
  	return sum;
}
```

**区间修改（加上某值）：**

```cpp
// [l, r] 为修改区间, c 为被修改的元素的变化量, [s, t] 为当前节点包含的区间, p
// 为当前节点的编号
void update(int l, int r, int c, int s, int t, int p) {
  	// 当前区间为修改区间的子集时直接修改当前节点的值,然后打标记,结束修改
  	if (l <= s && t <= r) {
    	d[p] += (t - s + 1) * c, b[p] += c;
    	return;
  	}
  	int m = s + ((t - s) >> 1);
  	if (b[p] && s != t) {
    	// 如果当前节点的懒标记非空,则更新当前节点两个子节点的值和懒标记值
    	d[p * 2] += b[p] * (m - s + 1), d[p * 2 + 1] += b[p] * (t - m);
    	b[p * 2] += b[p], b[p * 2 + 1] += b[p];  // 将标记下传给子节点
    	b[p] = 0;                                // 清空当前节点的标记
  	}
  	if (l <= m) update(l, r, c, s, m, p * 2);
  	if (r > m) update(l, r, c, m + 1, t, p * 2 + 1);
  	d[p] = d[p * 2] + d[p * 2 + 1];
}
```

**区间修改（修改为某个值）：**

```cpp
void update(int l, int r, int c, int s, int t, int p) {
  	if (l <= s && t <= r) {
    	d[p] = (t - s + 1) * c, b[p] = c, v[p] = 1;
    	return;
  	}
  	int m = s + ((t - s) >> 1);
  	// 额外数组储存是否修改值
  	if (v[p]) {
    	d[p * 2] = b[p] * (m - s + 1), d[p * 2 + 1] = b[p] * (t - m);
    	b[p * 2] = b[p * 2 + 1] = b[p];
    	v[p * 2] = v[p * 2 + 1] = 1;
    	v[p] = 0;
  	}
  	if (l <= m) update(l, r, c, s, m, p * 2);
  	if (r > m) update(l, r, c, m + 1, t, p * 2 + 1);
  	d[p] = d[p * 2] + d[p * 2 + 1];
}

int getsum(int l, int r, int s, int t, int p) {
  	if (l <= s && t <= r) return d[p];
  	int m = s + ((t - s) >> 1);
  	if (v[p]) {
    	d[p * 2] = b[p] * (m - s + 1), d[p * 2 + 1] = b[p] * (t - m);
    	b[p * 2] = b[p * 2 + 1] = b[p];
    	v[p * 2] = v[p * 2 + 1] = 1;
    	v[p] = 0;
  	}
  	int sum = 0;
  	if (l <= m) sum = getsum(l, r, s, m, p * 2);
  	if (r > m) sum += getsum(l, r, m + 1, t, p * 2 + 1);
  	return sum;
}
```

**区间查询（区间求和）：**

```cpp
int getsum(int l, int r, int s, int t, int p) {
  	// [l, r] 为查询区间, [s, t] 为当前节点包含的区间, p 为当前节点的编号
  	if (l <= s && t <= r) return d[p];
  	// 当前区间为询问区间的子集时直接返回当前区间的和
  	int m = s + ((t - s) >> 1);
  	if (b[p]) {
 	   	// 如果当前节点的懒标记非空,则更新当前节点两个子节点的值和懒标记值
    	d[p * 2] += b[p] * (m - s + 1), d[p * 2 + 1] += b[p] * (t - m);
    	b[p * 2] += b[p], b[p * 2 + 1] += b[p];  // 将标记下传给子节点
    	b[p] = 0;                                // 清空当前节点的标记
  	}
  	int sum = 0;
  	if (l <= m) sum = getsum(l, r, s, m, p * 2);
  	if (r > m) sum += getsum(l, r, m + 1, t, p * 2 + 1);
  	return sum;
}
```

### 2.4 ST表

稀疏表（Sparse Table，简称ST表）是一种用于静态数组的**高效离线查询**数据结构，特别适合解决**可重复贡献问题**，例如区间最值（RMQ）、区间最大公约数（GCD）、区间按位与/或等。它的核心思想是**倍增预处理**，将每个长度为2的幂的子区间信息提前计算好，从而在查询时通过**合并两个可能重叠的区间**快速得到答案。

稀疏表是什么？

- **定义**：ST表是一个二维数组 `st[i][j]`，表示从原数组下标 `i` 开始，长度为 `2^j` 的区间内的目标信息（例如最小值）。
- **原理**：利用倍增思想，通过 `st[i][j] = op(st[i][j-1], st[i + 2^(j-1)][j-1])` 递推得到所有长度是2的幂的区间信息，其中 `op` 是你要维护的操作（如 `min`、`max`、`gcd`）。
- **性质**：该操作必须满足**结合律**且**可重复贡献**（即允许区间重叠，重叠部分不影响最终结果），这样才能用两个子区间合并出完整区间。

怎么用？

1. 预处理（建表）

- 假设原数组 `a` 长度为 `n`，下标从0或1开始。
- 确定 `j` 的最大值 `k = floor(log2(n))`。
- 初始化 `st[i][0] = a[i]`。
- 对于 `j` 从 1 到 `k`：
  - 对于每个起点 `i`，如果 `i + 2^j - 1 < n`，则：
    ```
    st[i][j] = op(st[i][j-1], st[i + 2^(j-1)][j-1])
    ```
  - 其中 `op` 是你要合并的操作（如 `min`）。

2. 查询（区间 [l, r]）

- 计算区间长度 `len = r - l + 1`。
- 找到最大的 `j` 使得 `2^j ≤ len`，即 `j = floor(log2(len))`。
- 答案就是合并两个可能重叠的区间：
  ```
  ans = op(st[l][j], st[r - 2^j + 1][j])
  ```
  注意第二个区间以 `r` 为终点，长度为 `2^j`。

3. 示例：区间最小值查询

- 数组 `a = [3, 1, 4, 1, 5, 9]`，查询 `[1, 4]`（下标从0起）。
- 预处理后，`st[1][2]` 表示从1开始长度4的区间 `[1,4]` 的最小值（实际需要计算）。
- 查询时 `len=4`，`j=2`，答案 = `min(st[1][2], st[4-4+1=1][2])`，两个区间都是 `[1,4]`，正确。

什么时候知道需要用到ST表？

- **问题特征**：
  1. 数据是**静态的**（没有更新操作，或者更新极少可以忽略）。
  2. 需要**大量区间查询**，每次查询要求极快（O(1)）。
  3. 查询内容属于**可重复贡献问题**（如最值、gcd、按位与/或、lcm等），不适用于区间和（因为和不能有重叠）。

- **典型场景**：
  - RMQ（区间最值）问题，如“给定数组，多次询问某段区间的最小/最大值”。
  - 区间GCD问题，多次询问子数组的最大公约数。
  - 某些需要快速合并信息的题目，且合并操作允许重叠。

- **不适合的情况**：
  - 数组元素会动态变化（增删改）→ 考虑线段树、树状数组。
  - 查询操作不能重叠（如区间和、区间乘积）→ 考虑前缀和、线段树。

复杂度

- **预处理**：O(n log n)
- **单次查询**：O(1)
- **空间**：O(n log n)

总结

ST表是一种简洁而强大的预处理工具，用倍增思想将查询时间降到常数级，是解决静态区间可重复贡献问题的不二之选。当你的代码需要频繁查询且不允许更新时，不妨想想ST表！

模版

```cpp
/**
 * 稀疏表 (Sparse Table)
 * 适用于静态数组的离线区间可重复贡献查询（如区间最值、区间GCD等）。
 * 预处理 O(n log n)，单次查询 O(1)。
 *
 * @tparam T    元素类型
 * @tparam Func 二元操作函数类型，默认为 std::function<T(const T&, const T&)>
 */
template <typename T, typename Func = std::function<T(const T&, const T&)>>
class SparseTable {
private:
    std::vector<std::vector<T>> st;   // st[i][j] 表示从 j 开始长度为 2^i 的区间信息
    std::vector<int> log2_;            // 预计算的 log2 表，log2_[len] 表示 floor(log2(len))
    Func op;                           // 区间合并操作，必须满足结合律且可重复贡献
public:
    /**
     * 构造函数，构建稀疏表
     * @param arr  原始数组
     * @param func 二元操作函数，默认取最大值（为了演示方便，但实际使用时请显式传入所需操作）
     */
    explicit SparseTable(const std::vector<T>& arr, Func func = [](const T& a, const T& b) { return std::max(a, b); })
        : op(std::move(func))
    {
        int n = (int)arr.size();
        if (n == 0) return;
        // 预计算 log2 表
        log2_.resize(n + 1);
        log2_[1] = 0;
        for (int i = 2; i <= n; ++i) {
            log2_[i] = log2_[i / 2] + 1;
        }
        // 确定最大幂次 k = floor(log2(n))
        int k = log2_[n];
        // 初始化 st 表，st[0] 为原数组
        st.resize(k + 1);
        st[0] = arr;
        // 动态规划构建 st 表
        for (int i = 0; i < k; ++i) {
            int len = 1 << i;           // 当前层区间长度的一半（即子区间长度）
            int curSize = (int)st[i].size() - len;  // 下一层的有效起点个数
            st[i + 1].resize(curSize);
            for (int j = 0; j < curSize; ++j) {
                // 合并两个长度为 2^i 的区间：[j, j+len-1] 和 [j+len, j+2*len-1]
                st[i + 1][j] = op(st[i][j], st[i][j + len]);
            }
        }
    }
    /**
     * 区间查询 [l, r]（闭区间）
     * @param l 左端点（0-based）
     * @param r 右端点（0-based）
     * @return  区间 [l, r] 按照操作 op 合并的结果
     */
    T range_query(int l, int r) const {
        // 确保 l <= r 且下标合法（调用者需保证）
        int len = r - l + 1;
        int i = log2_[len];                // 最大幂次使得 2^i <= len
        // 合并两个可能重叠的区间：[l, l+2^i-1] 和 [r-2^i+1, r]
        return op(st[i][l], st[i][r - (1 << i) + 1]);
    }
};

/*操作示范
// 1. 最大值查询（使用默认的 max 操作）
    SparseTable<int> st_max(arr);
    std::cout << "max [1,5] = " << st_max.range_query(1, 5) << std::endl;   // 9

    // 2. 最小值查询（传入自定义 lambda）
    SparseTable<int, std::function<int(const int&, const int&)>> st_min(arr,
        [](const int& a, const int& b) { return std::min(a, b); });
    std::cout << "min [1,5] = " << st_min.range_query(1, 5) << std::endl;   // 1

    // 3. GCD 查询（传入自定义 gcd 函数）
    SparseTable<int, std::function<int(const int&, const int&)>> st_gcd(arr,
        [](const int& a, const int& b) { return gcd(a, b); });
    std::cout << "gcd [1,5] = " << st_gcd.range_query(1, 5) << std::endl;   // 1

    // 4. 使用函数指针（演示另一种写法）
    int (*min_ptr)(const int&, const int&) = [](const int& a, const int& b) { return std::min(a, b); };
    SparseTable<int, decltype(min_ptr)> st_min2(arr, min_ptr);
    std::cout << "min2 [1,5] = " << st_min2.range_query(1, 5) << std::endl; // 1
*/
```



## 0x03 图论

### 3.1 常见概念

无向图：特殊有向图，建立双向边。

有向图：可能包含：起点、终点、权值。

### 3.2 单源最短路径
#### 3.2.1 （正权稀疏图）动态数组存图+Djikstra算法
此处是优先队列优化版本，为$O(M log N)$的复杂度。
```cpp
vector<i64> dis(n + 1, 1E18); 
auto djikstra = [&](int s = 1) -> void { 
	using PII = pair<int, int>; 
	priority_queue<PII, vector<PII>, greater<PII>> q;
	q.emplace(0, s); 
	dis[s] = 0; 
	vector<i64> vis(n + 1); 
	while (!q.empty()) { 
		int x = q.top().second; 
		q.pop(); 
		if (vis[x]) continue; 
		vis[x] = 1; 
		for (auto [y, w] : ver[x]) { 
			if (dis[y] > dis[x] + w) { 
				dis[y] = dis[x] + w; 
				q.emplace(dis[y], y);
			} 
		} 
	} 
};
```
#### 3.2.2（负权图，判断负环）Bellman-Ford算法
循环$n$次，每次遍历所有边$(a, b, w)$，表示从$a$到$b$需要$w$的代价。用结构体数组存储每条边。每遍历一条边就更新一次距离数组。时间复杂度为$O(nm)$.
```cpp
int n, m, s; 
cin >> n >> m >> s; 
vector<tuple<int, int, ll>> ver(m + 1); 
for (int i = 1; i <= m; ++i) { 
	int x, y; 
	ll w; 
	cin >> x >> y >> w; 
	ver[i] = {x, y, w}; 
} 
vector<ll> dis(n + 1, inf), chk(n + 1); 
dis[s] = 0; 
for (int i = 1; i <= 2 * n; ++i) { // 双倍松弛，获取负环信息 
	vector<ll> backup = dis; 
	for (int j = 1; j <= m; ++j) { 
		auto [x, y, w] = ver[j]; 
		chk[y] |= (i > n && backup[x] + w < dis[y]); 
		dis[y] = min(dis[y], backup[x] + w); 
	} 
} 
for (int i = 1; i <= n; ++i) { 
	if (i == s) { 
		cout << 0 << " "; 
	} else if (dis[i] >= inf / 2) { 
		cout << "no "; 
	} else if (chk[i]) { 
		cout << "inf "; 
	} else { 
		cout << dis[i] << " "; 
	} 
}
```
#### 3.2.3 （负权图）SPFA算法
以$O(KM)$的复杂度计算，$K$为常数，但是可以通过特殊的构造退化成接近$N$，小心被卡。
```cpp
const int N = 1e5 + 7, M = 1e6 + 7; 
int n, m; 
int ver[M], ne[M], h[N], edge[M], tot; 
int d[N], v[N]; 
void add(int x, int y, int w) { 
	ver[++ tot] = y, ne[tot] = h[x], h[x] = tot; 
	edge[tot] = w; 
} 
void spfa() { 
	ms(d, 0x3f); d[1] = 0; 
	queue<int> q; q.push(1); 
	v[1] = 1;
	while(!q.empty()) { 
		int x = q.front(); q.pop(); v[x] = 0; 
		for (int i = h[x]; i; i = ne[i]) { 
			int y = ver[i]; 
			if(d[y] > d[x] + edge[i]) { 
				d[y] = d[x] + edge[i]; 
				if(v[y] == 0) q.push(y), v[y] = 1; 
			} 
		} 
	} 
} 
int main() { 
	cin >> n >> m; 
	for (int i = 1; i <= m; ++ i) { 
		int x, y, w; 
		cin >> x >> y >> w; 
		add(x, y, w); 
	} 
	spfa(); 
	for (int i = 1; i <= n; ++ i) { 
	if (d[i] == INF) cout << "N" << endl; 
	else cout << d[n] << endl; 
	} 
}
```



### 3.3 多源最短路径

2025年校赛出现了一次多源BFS。手动害怕（）

下面是一个板子：

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * 多源 BFS 模板 (适用于二维网格)
 * @param grid 输入的网格，通常为 vector<vector<int>>，0 表示可走，1 表示障碍等（根据题目定义）
 * @param sources 初始的源点坐标列表，每个源点为 pair<int,int>
 * @return 距离矩阵 dist，dist[i][j] 表示 (i,j) 到最近源点的最短步数（无法到达则保持初始值 -1 或 INF）
 */
vector<vector<int>> multiSourceBFS(const vector<vector<int>>& grid,
                                    const vector<pair<int,int>>& sources) {
    int rows = grid.size();
    if (rows == 0) return {};
    int cols = grid[0].size();

    // 方向数组：上下左右
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    // 距离矩阵，初始化为 -1 表示未访问 / 不可达
    vector<vector<int>> dist(rows, vector<int>(cols, -1));

    queue<pair<int,int>> q;

    // 1. 将所有源点入队，并设置距离为 0
    for (auto& src : sources) {
        int x = src.first, y = src.second;
        if (x >= 0 && x < rows && y >= 0 && y < cols) {  // 确保源点合法
            dist[x][y] = 0;
            q.push({x, y});
        }
    }

    // 2. BFS 逐层向外扩展
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // 遍历四个方向
        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            // 检查边界
            if (nx < 0 || nx >= rows || ny < 0 || ny >= cols) continue;

            // 如果该格子不可走（根据题目设定），跳过。例如 grid 中 1 表示障碍
            // 如果 grid 中没有障碍概念，可以省略此判断
            // if (grid[nx][ny] == 1) continue;

            // 如果该格子尚未被访问过（dist 仍为 -1）
            if (dist[nx][ny] == -1) {
                dist[nx][ny] = dist[x][y] + 1;  // 距离 +1
                q.push({nx, ny});
            }
        }
    }

    return dist;  // 返回每个格子的最短距离（-1 表示无法到达）
}

// 使用示例
int main() {
    // 假设有一个 5x5 的网格，0 表示空地，1 表示障碍
    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}
    };

    // 源点：两个位置 (0,0) 和 (4,4)
    vector<pair<int,int>> sources = {{0,0}, {4,4}};

    // 执行多源 BFS
    vector<vector<int>> distance = multiSourceBFS(grid, sources);

    // 打印结果
    cout << "Distance to nearest source:\n";
    for (int i = 0; i < distance.size(); ++i) {
        for (int j = 0; j < distance[0].size(); ++j) {
            if (distance[i][j] == -1)
                cout << " #";   // 无法到达
            else
                cout << setw(2) << distance[i][j];
        }
        cout << "\n";
    }
    return 0;
}
```

配上一下ZJSU2025年校赛G题，方便理解

**题面：**

小明突然出现在了新川崎，决定在这里开展演唱会。

新川崎是一个大小为 *n*×*m* 的网格，其中有若干位置不可通行。新川崎的土著刺刺乐队已经占据了一些位置进行街头演唱，地图中分布了一些行人，行人们会寻找自己能够前往、且所需移动距离最短的演唱地点观看（行人只能水平或垂直移动）。

刺刺乐队已经占据了许多位置，请你为小明找一个好位置演唱，尽可能吸引更多观众。

若距离相同，行人会优先前往小明的演唱地点。

**输入描述**

第一行包含两个正整数 *n*,*m*，表示地图大小，*n*≤*m*≤500。

接下来是一个 *n*×*m* 大小的矩阵，其中：

- `x` 表示不可通行。
- `.` 表示空白位置。
- `1` 表示此处有行人。
- `p` 表示此处被刺刺乐队占据。

保证 `1`, `p` 的数量不超过 100。

行人一次移动只能朝一个方向移动一格，**可以穿过除了** **x** **以外的所有位置**，假设所有行人都能快速找到最近的路径。

小明**只能选择空白地点或行人所在位置**演唱，不能选择不可通行位置或刺刺乐队占据的位置。

**输出描述**

输出一行，包含一个正整数，表示小明最多可能获得的观众数。

**Code**

```cpp
int dx[4]={0,0,1,-1};
int dy[4]={1,-1,0,0};

void solve()
{
    int n,m;
    cin>>n>>m;
    vector<string> mp(n);
    for(int i=0;i<n;i++) cin>>mp[i];
    vector<pii> per,bnd;
    for(int i=0;i<n;i++)
    {
        for (int j=0;j<m;j++) {
            if(mp[i][j]=='1') per.emplace_back(i,j);
            if(mp[i][j]=='p') bnd.emplace_back(i,j);
        }
    }
    vector<vector<int>> dist_b(n,vector<int>(m,-1));
    queue<pii> q;
    for(auto [x,y]:bnd)
    {
        dist_b[x][y]=0;
        q.emplace(x,y);
    }
    while(!q.empty())
    {
        auto [x,y]=q.front();
        q.pop();
        int dis=dist_b[x][y];
        for(int u=0;u<4;u++)
        {
            int nx=x+dx[u],ny=y+dy[u];
            if(nx>=0&&nx<n&&ny>=0&&ny<m)
            {
                if(dist_b[nx][ny]==-1&&mp[nx][ny]!='x'){
                    dist_b[nx][ny]=dis+1;
                    q.emplace(nx,ny);
                } 
            }
        }
    }
    vector<vector<int>> cnt(n,vector<int>(m,0));
    for(auto [a,b]:per)
    {
        int d_per=dist_b[a][b];
        vector<vector<int>> dist_h(n,vector<int>(m,-1));
        queue<pair<int,int>> qh;
        dist_h[a][b]=0;
        qh.emplace(a,b);
        while(!qh.empty()){
            auto [x,y]=qh.front();
            qh.pop();
            int d=dist_h[x][y];
            for(int i=0;i<4;i++)
            {
                int px=x+dx[i],py=y+dy[i];
                if(px>=0&&px<n&&py>=0&&py<m){
                    if(mp[px][py]!='x'&&dist_h[px][py]==-1)
                    {
                        dist_h[px][py]=d+1;
                        qh.emplace(px,py);
                    }
                }
            }
        }
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(mp[i][j]=='x'||mp[i][j]=='p') continue;
                if(dist_h[i][j]!=-1) {
                    if(d_per==-1||dist_h[i][j]<=d_per) cnt[i][j]++;
                }
            }
        }
    }
    ll res=0;
    for (int i=0;i<n;i++) {
        for(int j=0;j<m;j++)
        {
            if(mp[i][j]!='x'&&mp[i][j]!='p') res=max(res,(ll)cnt[i][j]);
        }
    }
    cout<<res<<endl;
}
```



### 3.4 树上问题

#### 3.4.1 树基础

**树的储存**

1.只记录父节点：开一个数组 $parent[n]$用来存储每一个节点的父节点。如果只需要从底往根遍历，那么这样就好。

2.邻接表：

如果是无根树：给每一个节点开一个列表，存储与之相连接的节点编号。

如果是有根树：

​	如果是无向图：同样可以用上述方法存储。

​	如果是若输入数据能够确保结点的上下关系，则可以利用这个信息．为每个结点开辟一个线性列表，记录其所有子结点；若有需要，还可在另一个数组中记录其父结点．

```cpp
std::vector<int> children[N];
int parent[N];
```

#### 3.4.2 LCA最近公共祖先

树链剖分解法，预处理$O(N)$，单次查询$O(logN)$

```cpp
struct HLD {
	int n, idx;
	vector<vector<int>> ver;
	vector<int> siz, dep;
	vector<int> top, son, parent;
	HLD(int n) {
		this->n = n;
		ver.resize(n + 1);
		siz.resize(n + 1);
		dep.resize(n + 1);
		top.resize(n + 1);
		son.resize(n + 1);
		parent.resize(n + 1);
	}
	void add(int x, int y) { // 建立双向边
		ver[x].push_back(y);
		ver[y].push_back(x);
	}
	void dfs1(int x) {
		siz[x] = 1;
		dep[x] = dep[parent[x]] + 1;
		for (auto y : ver[x]) {
			if (y == parent[x]) continue;
			parent[y] = x;
			dfs1(y);
			siz[x] += siz[y];
			if (siz[y] > siz[son[x]]) {
				son[x] = y;
			}
		}
	}
	void dfs2(int x, int up) {
		top[x] = up;
		if (son[x]) dfs2(son[x], up);
		for (auto y : ver[x]) {
			if (y == parent[x] || y == son[x]) continue;
			dfs2(y, y);
		}
	}
	int lca(int x, int y) {
		while (top[x] != top[y]) {
			if (dep[top[x]] > dep[top[y]]) {
				x = parent[top[x]];
			} else {
				y = parent[top[y]];
			}
		}
		return dep[x] < dep[y] ? x : y;
	}
	int clac(int x, int y) { // 查询两点间距离
		return dep[x] + dep[y] - 2 * dep[lca(x, y)];
	}
	void work(int root = 1) { // 在此初始化
		dfs1(root);
		dfs2(root, root);
	}
};
```



## 0x04 数论

### 4.1 模运算

阴中之阴，性质灵活，数论大手子。

#### 4.1.1 取余

有以下几个基本性质：

1.分配率：			           ![img](https://bkimg.cdn.bcebos.com/formula/afb2f9dd9d6dbe7e41b90362e0d3187e.svg)

![img](https://bkimg.cdn.bcebos.com/formula/f09662da8c05b0530568241619311c6f.svg)

2.乘法逆元：若整数$a$、$n$互质，即

![img](https://bkimg.cdn.bcebos.com/formula/d9758ee2e1869bdf9d1ca803c480ed5f.svg)

则存在整数$x$、$y$，使得

![img](https://bkimg.cdn.bcebos.com/formula/4671dba26fe6a1d32e52b5a883790bae.svg)

即存在整数$x$，使得

![img](https://bkimg.cdn.bcebos.com/formula/0fe5c97b3741b338b384a2aaeae46c1d.svg)

整数$x$被称为$a$在模$n$意义下的乘法逆元。

#### 4.1.2 同余定理

**如果两个整数除以同一个正整数得到的余数相同，我们就说它们模这个数同余**。

设 *m* 是一个正整数，整数 *a* 和 *b* 满足 m*∣(*a*−*b)（即 *a*−*b* 能被 *m* 整除），则称 *a* 与 *b* 模 *m* 同余，记作：

*a*≡*b*(mod *m*)

3.传递性：

- 若 a≡b(mod $m$)*a*≡*b*(mod $m$*)，c≡d(mod $m$)c*≡*d*(mod *$m$*)，则 a±c≡b±d(mod $m$)*a*±*c*≡*b*±*d*(mod $m$)
- 若 a≡b(mod $m$)*a*≡*b*(mod $m$)，c≡d(mod $m$)*c*≡*d*(mod *$m$*)，则 a⋅c≡b⋅d(mod $m$)*a*⋅*c*≡*b*⋅*d*(mod *$m$*)

#### 4.1.3 逆元

非零实数 𝑎 ∈𝐑![a\in\mathbf R](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的乘法逆元就是它的倒数 $𝑎^{−1}$![a^{-1}](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)．类似地，数论中也可以定义一个整数 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 在模 𝑚![m](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 意义下的逆元 $𝑎^{−1}$mod 𝑚![a^{-1}\bmod m](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，或简单地记作 $𝑎^{−1}$![a^{-1}](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)．这就是 **模逆元**（modular multiplicative inverse)，也称作 **数论倒数**．

求解逆元有两种方法：

**1.**利用扩展欧几里得算法或快速幂法，可以在 𝑂(log⁡𝑚)![O(\log m)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 时间内求出单个整数的逆元．

求解逆元，就相当于求解线性同余方程．因此，可以使用 [扩展欧几里得算法](https://oi-wiki.org/math/number-theory/gcd/#扩展欧几里得算法) 在 𝑂(log⁡ min{𝑎,𝑚})![O(\log\min\{a,m\})](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 时间内求解逆元．同时，由于逆元对应的线性方程比较特殊，可以适当地简化相应的步骤．这个算法好在可以求出所有模数下的逆元。

```cpp
// Extended Euclidean algorithm.
void ex_gcd(int a, int b, int& x, int& y) {
  if (!b) {
    x = 1;
    y = 0;
  } else {
    ex_gcd(b, a % b, y, x);
    y -= a / b * x;
  }
}

// Returns the modular inverse of a modulo m.
// Assumes that gcd(a, m) = 1, so the inverse exists.
int inverse(int a, int m) {
  int x, y;
  ex_gcd(a, m, x, y);
  return (x % m + m) % m;
}
```

**2.**快速幂法

这一方法主要适用于模数是素数 𝑝![p](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的情形．此时，由 [费马小定理](https://oi-wiki.org/math/number-theory/fermat/#费马小定理) 可知对于任意 𝑎 ⟂𝑝![a\perp p](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 都有

𝑎⋅$𝑎^{𝑝−2}$=$𝑎^{𝑝−1}$≡1(mod 𝑝).![ a\cdot a^{p-2} = a^{p-1} \equiv 1 \pmod p. ](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

根据逆元的唯一性可知，逆元 $𝑎^{−1}$mod 𝑝![a^{-1}\bmod p](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 就等于 $𝑎^{𝑝−2}$mod 𝑝![a^{p-2}\bmod p](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，因此可以直接使用 [快速幂](https://oi-wiki.org/math/binary-exponentiation/) 在 𝑂(log ⁡𝑝)![O(\log p)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 时间内计算：

```cpp
// Binary exponentiation.
int pow(int a, int b, int m) {
  long long res = 1, po = a;
  for (; b; b >>= 1) {
    if (b & 1) res = res * po % m;
    po = po * po % m;
  }
  return res;
}

// Returns the modular inverse of a prime modulo p.
int inverse(int a, int p) { return pow(a, p - 2, p); }
```



### 4.2 质数

素数与合数的简单性质：

- 大于 1![1](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的整数 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 是合数，等价于 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 可以表示为整数 𝑑![d](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 和 𝑒![e](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)（1 <𝑑,𝑒 <𝑎![1<d,e<a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)）的乘积．
- 如果素数 𝑝![p](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 有大于 1![1](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的约数 𝑑![d](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，那么 𝑑 =𝑝![d=p](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)．
- 大于 1![1](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的整数 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 一定可以表示为素数的乘积．
- 对于合数 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，一定存在素数 𝑝 ≤√𝑎![p\le\sqrt{a}](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 使得 𝑝 ∣𝑎![p\mid a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)．
- 素数有无穷多个．
- 所有大于 3![3](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)的素数都可以表示为 6𝑛 ±1![6n\pm 1](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的形式

素数计数函数：小于或等于 𝑥![x](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的素数的个数，用 𝜋(𝑥)![\pi(x)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 表示．随着 𝑥![x](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的增大，有这样的近似结果：𝜋(𝑥) ∼𝑥/ln⁡(𝑥)![\pi(x) \sim \dfrac{x}{\ln(x)}](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

#### 4.2.1 试除法

很容易发现这样一个事实：如果 𝑥![x](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 是 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的约数，那么 𝑎/𝑥![\frac{a}{x}](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 也是 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的约数．

这个结论告诉我们，对于每一对 (𝑥,𝑎/𝑥)![(x, \frac{a}{x} )](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，只检验其中的一个就足够了．为了方便起见，我们只考察每一对的较小数．不难发现，所有这些较小数都在 [1,√𝑎]![[1, \sqrt{a}]](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 这个区间里．

```cpp
bool isPrime(int a) {
  if (a < 2) return 0;
  for (int i = 2; (long long)i * i <= a; ++i)  // 防溢出
    if (a % i == 0) return 0;
  return 1;
}
```

#### 4.2.2 Fermat素性检验

**Fermat 素性检验** 是最简单的概率性素性检验．

我们可以根据 [费马小定理](https://oi-wiki.org/math/number-theory/fermat/#费马小定理) 得出一种检验素数的思路：

基本思想是不断地选取在 [2,𝑛 −1]![[2, n-1]](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 中的基底 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，并检验是否每次都有 $a^{n-1}$ ≡1(mod 𝑛)![a^{n-1} \equiv 1 \pmod n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)．

```cpp
bool fermat(int n) {
  if (n < 3) return n == 2;
  // test_time 为测试次数,建议设为不小于 8
  // 的整数以保证正确率,但也不宜过大,否则会影响效率
  for (int i = 1; i <= test_time; ++i) {
    int a = rand() % (n - 2) + 2;
    if (quickPow(a, n - 1, n) != 1) return false;
  }
  return true;
}
```

如果 $𝑎^{𝑛−1}$ ≡1(mod 𝑛)![a^{n−1} \equiv 1 \pmod n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 但 𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 不是素数，则称 𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 为以 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 为底的 **Fermat 伪素数**．我们在实践中观察到，如果 𝑎𝑛−1 ≡1(mod𝑛)![a^{n−1} \equiv 1 \pmod n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，那么 𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 通常是素数．但其实存在反例：对于 𝑛 =341![n = 341](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 且 𝑎 =2![a = 2](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，虽然有 2340 ≡1(mod341)![2^{340}\equiv 1 {\pmod {341}}](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，但是 341 =11 ⋅31![341 = 11 \cdot 31](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 是合数．事实上，对于任何固定的基底 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，这样的反例都有无穷多个[1](https://oi-wiki.org/math/number-theory/prime/#fn:inf-fermat-pp)．

既然对于单个基底，Fermat 素性测试无法保证正确性，一个自然的想法就是多检查几组基底．但是，即使检查了所有可能的与 𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 互素的基底 𝑎![a](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，依然无法保证 𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 是素数．也就是说，费马小定理的逆命题并不成立：即使对于所有 𝑎 ⟂𝑛![a\perp n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，都有 $𝑎^{𝑛−1}$ ≡1(mod 𝑛)![a^{n-1}\equiv 1\pmod n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 也不一定是素数．这样的数称为 [Carmichael 数](https://oi-wiki.org/math/number-theory/primitive-root/#carmichael-数)．它也有无穷多个．这迫使我们寻找更为严格的素性测试．

#### 4.2.3 筛法

通常，我们会遇到需要筛选出素数，所以下面是几种筛法：

1.埃拉托斯特尼筛法

**过程**

考虑这样一件事情：对于任意一个大于 1![1](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的正整数 𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，那么它的 𝑥![x](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 倍就是合数（𝑥 >1![x > 1](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)）．利用这个结论，我们可以避免很多次不必要的检测．

如果我们从小到大考虑每个数，然后同时把当前这个数的所有（比自己大的）倍数记为合数，那么运行结束的时候没有被标记的数就是素数了．时间复杂度是 𝑂(𝑛log⁡ log⁡𝑛)![O(n\log\log n)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)．

```cpp
vector<int> prime;
bool is_prime[N];

void Eratosthenes(int n) {
  is_prime[0] = is_prime[1] = false;
  for (int i = 2; i <= n; ++i) is_prime[i] = true;
  for (int i = 2; i <= n; ++i) {
    if (is_prime[i]) {
      prime.push_back(i);
      if ((long long)i * i > n) continue;
      for (int j = i * i; j <= n; j += i)
        // 因为从 2 到 i - 1 的倍数我们之前筛过了，这里直接从 i
        // 的倍数开始，提高了运行速度
        is_prime[j] = false;  // 是 i 的倍数的均不是素数
    }
  }
}
```

2.筛至平方根

显然，要找到直到 𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 为止的所有素数，仅对不超过 √𝑛![\sqrt n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的素数进行筛选就足够了．

```cpp
vector<int> prime;
bool is_prime[N];

void Eratosthenes(int n) {
  is_prime[0] = is_prime[1] = false;
  for (int i = 2; i <= n; ++i) is_prime[i] = true;
  // i * i <= n 说明 i <= sqrt(n)
  for (int i = 2; i * i <= n; ++i) {
    if (is_prime[i])
      for (int j = i * i; j <= n; j += i) is_prime[j] = false;
  }
  for (int i = 2; i <= n; ++i)
    if (is_prime[i]) prime.push_back(i);
}
```

这种优化不会影响渐近时间复杂度，实际上重复以上证明，我们将得到 o(𝑛ln ⁡ln⁡√𝑛)+𝑜(𝑛)![n \ln \ln \sqrt n + o(n)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，根据对数的性质，它们的渐近相同，但操作次数会明显减少．



### 4.3 位运算

#### 4.3.1 基础概念

| 运算符 | 名称   | 规则                                     |
| ------ | ------ | ---------------------------------------- |
| &      | 按位与 | 有0则0，全1为1 (用于取出/置0)            |
| \|     | 按位或 | 有1则1，全0为0 (用于置1)                 |
| ^      | 异或   | 相同为0，不同为1 (无进位加法)            |
| ~      | 取反   | 0变1，1变0 (注意符号位)                  |
| <<     | 左移   | 高位丢弃，低位补0。 **`1 << n` = $2^n$** |
| >>     | 右移   | 低位丢弃。正数补0，负数补1 (算术右移)    |

核心公式：

- **`n & (-n)`**：保留最低位的1（`lowbit` 核心）
- **`x & (x-1)`**：消去最低位的1

#### 4.3.2 核心技巧

1.状态压缩与状压DP

状态压缩通常用一个整数的二进制位来表示一个集合（0表示未选中，1表示选中）。以下是所有常用操作（**必须熟记**）：

| 目标操作                        | 代码实现 (第k位，通常从第0位开始) |
| :------------------------------ | :-------------------------------- |
| **取出第k位**                   | `(n >> k) & 1`                    |
| **取出后k位** (第0~k-1位)       | `n & ((1 << k) - 1)`              |
| **第k位取反**                   | `n ^ (1 << k)`                    |
| **第k位赋值为1**                | `n | (1 << k)`                    |
| **第k位赋值为0**                | `n & (~(1 << k))`                 |
| **判断第k位是否为1**            | `if ((n >> k) & 1)`               |
| **判断一个数是不是2的整数次幂** | `if (n > 0 && !(n & (n - 1)))`    |

2. 成对变换

对于非负整数 `n` ：

- 若 `n` 为偶数，则 `n ^ 1 = n + 1`
- 若 `n` 为奇数，则 `n ^ 1 = n - 1`
- **应用**：常用于图论中存储双向边（0与1配对，2与3配对...），可以通过 `^1` 快速找到反向边。

3. lowbit 运算

`lowbit(n)` 定义为非负整数 `n` 在二进制下“最低位的1及其后边所有的0”构成的数值 。

```cpp
int lowbit(int x) {
    return x & -x; 
}
```

- **原理**： `-x` 等同于 `(~x) + 1`。
- **经典应用**：统计二进制中1的个数、树状数组。

#### 4.3.3 进阶

1. 异或运算的性质

异或（`^`）在ICPC中考察极多，记住以下性质：

- **交换律**、**结合律** (意味着可以任意顺序计算)
- **`x ^ x = 0`** (自己消去)
- **`x ^ 0 = x`**
- **`a ^ b = c` ⇒⇒ `a ^ c = b`** (可用于加密还原)

**经典问题**：

- **找唯一出现奇数次的数**：全部异或起来，结果即为答案。
- **交换两数**：`a ^= b; b ^= a; a ^= b;` (虽炫技但实际应用中不如直接temp，需注意a和b为同一变量时会出错)

2. 起床困难综合症 (经典题)

**题意**：给定n个位运算操作（AND, OR, XOR）和一个初始值范围 [0, m]，求最终能得到的最大值 。
**思路**：按位贪心。

1. 先假设初始值全0和全1（即二进制全0和全1的数），模拟一遍操作，得到两个结果 `res0` 和 `res1`。
2. 从高位向低位遍历：
   - 如果 `res0` 这一位为1，直接贡献（因为选0就能得到1，肯定选）。
   - 否则，如果 `res1` 这一位为1，且当前初始值这一位选1不会超过m，则这一位选1，并更新m的限制。
   - 否则，这一位只能选0。

3. 状压DP：最短Hamilton路径

**题意**：给定一张 n (n ≤ 20) 个点的带权无向图，求从起点0到终点n-1恰好经过每个点一次的最短路径 。
**状态表示**：`dp[state][j]` 表示当前已经走过的点的集合为 `state`，且当前位于点 `j` 的最短路径长度。
**状态转移**：

```cpp
memset(dp, 0x3f, sizeof dp);
dp[1][0] = 0; // 只走了点0，位置在0
for (int state = 1; state < (1 << n); state++) {
    // 优化：可以只枚举state中存在的点作为j
    for (int j = 0; j < n; j++) {
        if (!(state >> j & 1)) continue; // 当前状态j没走过，跳过
        for (int k = 0; k < n; k++) {
            if (!(state >> k & 1)) continue; // 前一个状态k没走过？实际上我们要从去掉j的状态转移
            // 正确转移：从 state 去掉 j 的状态转移过来
            int pre_state = state ^ (1 << j);
            if (pre_state >> k & 1) {
                dp[state][j] = min(dp[state][j], dp[pre_state][k] + w[k][j]);
            }
        }
    }
}
int ans = dp[(1 << n) - 1][n-1];
```

4. 枚举子集

对于一个集合 `S`（用一个整数表示），枚举其所有子集 `sub`：

```cpp
for (int sub = S; sub; sub = (sub - 1) & S) {
    // 处理子集 sub
}
// 包括空集的话需单独处理 sub = 0
```

**原理**：`(sub - 1) & S` 会跳过那些不属于S的1，保证只枚举S的子集。

5. 数论中的小技巧

- **判断奇偶**：`if (n & 1)` 比 `if (n % 2)` 快。
- **取模 $2^k$**：`n & ((1 << k) - 1)` 等价于 `n % (2^k)`，但速度极快。
- **向上取整除法**：`(a + b - 1) / b` 也可用位运算变体，但编译器通常已优化。

### 4.4 GCD & exGCD

这里侧重讲exGCD。毕竟在牛客寒假营出现过。

问题抛出：对于一个方程 $ax+by=c$，我们期望求出它的一些解。什么时候这个方程可以有解。

直接给出结论：当 $c=gcd(a,b)$ 的时候，方程必然有多组解。我们可以用exGCD求出一组可行解。

**过程：**

设 𝑎𝑥1 +𝑏𝑦1 = gcd(𝑎,𝑏)![ax_1+by_1=\gcd(a,b)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

𝑏𝑥2 +(𝑎 mod 𝑏) 𝑦2 =gcd(𝑏,𝑎 mod 𝑏)![bx_2+(a\bmod b)y_2=\gcd(b,a\bmod b)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

由欧几里得定理可知：gcd(𝑎,𝑏) = gcd(𝑏,𝑎 mod 𝑏)![\gcd(a,b)=\gcd(b,a\bmod b)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

所以 𝑎𝑥1 + 𝑏𝑦1 = 𝑏𝑥2 + (𝑎 mod 𝑏) 𝑦2![ax_1+by_1=bx_2+(a\bmod b)y_2](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

又因为 𝑎 mod 𝑏 =𝑎 −( ⌊𝑎𝑏⌋ ×𝑏)![a\bmod b=a-(\lfloor\frac{a}{b}\rfloor\times b)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

所以 𝑎𝑥1 + 𝑏𝑦1 =𝑏𝑥2 +(𝑎 −(⌊𝑎𝑏⌋ ×𝑏)) 𝑦2![ax_1+by_1=bx_2+(a-(\lfloor\frac{a}{b}\rfloor\times b))y_2](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

𝑎𝑥1 +𝑏𝑦1 =𝑎𝑦2 +𝑏𝑥2 −⌊𝑎𝑏⌋ ×𝑏𝑦2 =𝑎𝑦2 +𝑏(𝑥2 −⌊𝑎𝑏⌋𝑦2)![ax_1+by_1=ay_2+bx_2-\lfloor\frac{a}{b}\rfloor\times by_2=ay_2+b(x_2-\lfloor\frac{a}{b}\rfloor y_2)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

因为 𝑎 =𝑎,𝑏 =𝑏![a=a,b=b](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，所以 𝑥1 =𝑦2,𝑦1 =𝑥2 −⌊𝑎𝑏⌋𝑦2![x_1=y_2,y_1=x_2-\lfloor\frac{a}{b}\rfloor y_2](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)

将 𝑥2,𝑦2![x_2,y_2](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 不断代入递归求解直至 gcd![\gcd](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)（最大公约数，下同）为 0![0](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 递归 𝑥 =1,𝑦 =0![x=1,y=0](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 回去求解．

**实现：**

```cpp
int Exgcd(int a, int b, int &x, int &y) {
  if (!b) {
    x = 1;
    y = 0;
    return a;
  }
  int d = Exgcd(b, a % b, x, y);
  int t = x;
  x = y;
  y = t - (a / b) * y;
  return d;
}
```



## 0x05 串

### 5.1 字符串

#### 5.1.1 奇技淫巧

先来一些字符串的奇技淫巧：

```cpp
std::string to_string(数值类型 value); //数值 → 字符串：std::to_string
std::stoi(const std::string& str, std::size_t* pos = 0, int base = 10); 
//字符串 → 数值：std::stoi, std::stol, std::stoll（整数）
/*
str：待转换的字符串。
pos：可选指针，用于存储第一个未转换字符的索引（即处理了多少个字符）。
base：进制（2~36），默认为10。0 表示自动检测（如 0x 开头十六进制，0 开头八进制）。
*/

```

#### 5.1.2 字符串流

C++ 中的字符串流（`std::stringstream`）是标准库 `<sstream>` 提供的强大工具，它允许将字符串作为流来操作，可以方便地进行字符串的格式化输入输出、类型转换、字符串分割等。下面整理了常见的用法，包括三种主要的流类型：`std::stringstream`（双向）、`std::istringstream`（输入）和`std::ostringstream`（输出）。

1. 包含头文件

```cpp
#include <sstream>
#include <string>
#include <iostream>
```

2. 创建字符串流对象

```cpp
std::stringstream ss;            // 空流
std::stringstream ss("hello");   // 用字符串初始化（内部缓冲区包含"hello"）
std::istringstream iss("input"); // 只读
std::ostringstream oss;          // 只写
```

3. 基本输入输出操作

像使用 `cin`/`cout` 一样使用 `<<` 和 `>>` 操作符。

3.1 写入数据（输出到流）

```cpp
std::ostringstream oss;
oss << "Age: " << 25 << ", score: " << 92.5;
std::string result = oss.str(); // 获取流中的字符串
// result = "Age: 25, score: 92.5"
```

3.2 读取数据（从流中提取）

```cpp
std::istringstream iss("100 3.14 hello");
int i;
double d;
std::string s;
iss >> i >> d >> s;
// i=100, d=3.14, s="hello"
```

3.3 双向操作

```cpp
std::stringstream ss;
ss << 42 << " " << 3.14;                // 写入
int n;
double x;
ss >> n >> x;                           // 读取
// n=42, x=3.14
```

4. 类型转换（字符串 ↔ 数值）

利用流可以轻松实现字符串与数值的相互转换。

4.1 数值转字符串

```cpp
int num = 123;
std::stringstream ss;
ss << num;
std::str = ss.str(); // "123"
```
或使用 `std::to_string`（C++11起），但 `stringstream` 更灵活（可控制格式）。

4.2 字符串转数值

```cpp
std::string str = "456";
std::stringstream ss(str);
int num;
ss >> num; // num = 456
```
注意：转换失败时流会进入错误状态，需要检查。

5. 字符串分割

利用 `>>` 默认以空白字符分隔，可以分割单词。

```cpp
std::string text = "apple banana cherry";
std::istringstream iss(text);
std::string word;
while (iss >> word) {
    std::cout << word << std::endl;
}
// 输出：apple  banana  cherry
```

如果需要按特定字符分割（如逗号），可以使用 `std::getline` 配合流：

```cpp
std::string csv = "name,age,city";
std::istringstream iss(csv);
std::string field;
while (std::getline(iss, field, ',')) {
    std::cout << field << std::endl;
}
// 输出：name age city
```

6. 清空流与重用

清空流需要重置状态和内容，因为流一旦到达末尾（eof）或出错，后续操作会失败。

```cpp
std::stringstream ss;
ss << "first";
std::string s;
ss >> s; // s = "first"

// 准备重用
ss.str("");      // 清空内容
ss.clear();      // 重置错误状态（如eof）
ss << "second";
ss >> s;         // s = "second"
```

7. 获取与设置内部字符串

- `str()` 返回当前流中的字符串副本。
- `str(const string&)` 设置内部字符串。

```cpp
std::ostringstream oss;
oss << "hello";
std::string copy = oss.str();   // 获取副本

oss.str("world");                // 替换内部内容
std::cout << oss.str();          // 输出 "world"
```

8. 格式化控制

可以像 `cout` 一样使用操纵符（`std::hex`, `std::setprecision` 等）。

```cpp
#include <iomanip>

std::ostringstream oss;
oss << std::hex << 255;               // 输出 "ff"
oss << std::fixed << std::setprecision(2) << 3.14159; // 输出 "3.14"
```

9. 错误处理

检查流状态，确保转换成功。

```cpp
std::istringstream iss("123abc");
int value;
iss >> value;
if (iss.fail()) {
    std::cerr << "转换失败" << std::endl;
}
// 注意：读取到"123"后，流中剩余"abc"，但value已被赋值为123，
// 如果没有后续读取，可能不会进入fail状态。更准确的检查可以看是否提取了整个字符串。
```

更好的做法是使用 `std::from_chars`（C++17）进行数值转换，但 `stringstream` 简单直接。

10. 常用函数总结

| 成员函数             | 作用                               |
| -------------------- | ---------------------------------- |
| `str()`              | 返回流中字符串的副本               |
| `str(const string&)` | 设置流的新内容                     |
| `clear()`            | 清除错误标志（如 eofbit, failbit） |
| `rdstate()`          | 返回当前状态                       |
| `good()`             | 流有效（无错误）                   |
| `eof()`              | 是否到达末尾                       |
| `fail()`             | 是否发生致命错误（如类型不匹配）   |
| `bad()`              | 是否发生严重错误（如缓冲区损坏）   |

注意事项

1. **流状态**：读取操作可能导致流进入 `eof` 或 `fail` 状态，重用前需要 `clear()`。
2. **性能**：对于大量转换，`stringstream` 可能比专用函数（如 `std::to_string`/`std::stoi`）慢，但可读性好。
3. **空白分隔**：`>>` 默认跳过前导空白，且遇到空白停止，如果需要保留空白，考虑 `std::getline`。
4. **缓冲区共享**：`stringstream` 内部维护一个字符串缓冲区，通过 `str()` 返回的是副本，修改副本不影响流。

---

以上是 C++ 字符串流的常见用法。根据具体需求，可以选择合适的流类型和操作方式。

### 5.2 KMP

**过程**

给定一个文本 𝑡![t](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 和一个字符串 𝑠![s](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，我们尝试找到并展示 𝑠![s](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 在 𝑡![t](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 中的所有出现（occurrence）．

为了简便起见，我们用 𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 表示字符串 𝑠![s](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的长度，用 𝑚![m](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 表示文本 𝑡![t](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的长度．

我们构造一个字符串 𝑠 +# +𝑡![s + \# + t](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)，其中 #![\#](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 为一个既不出现在 𝑠![s](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 中也不出现在 𝑡![t](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 中的分隔符．接下来计算该字符串的前缀函数．现在考虑该前缀函数除去最开始 𝑛 +1![n + 1](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 个值（即属于字符串 𝑠![s](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 和分隔符的函数值）后其余函数值的意义．根据定义，𝜋[𝑖]![\pi[i]](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 为右端点在 𝑖![i](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 且同时为一个前缀的最长真子串的长度，具体到我们的这种情况下，其值为与 𝑠![s](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的前缀相同且右端点位于 𝑖![i](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的最长子串的长度．由于分隔符的存在，该长度不可能超过 𝑛![n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)．而如果等式 𝜋[𝑖] =𝑛![\pi[i] = n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 成立，则意味着 𝑠![s](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 完整出现在该位置（即其右端点位于位置 𝑖![i](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7)）．注意该位置的下标是对字符串 𝑠 +# +𝑡![s + \# + t](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 而言的．

因此如果在某一位置 𝑖![i](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 有 𝜋[𝑖] =𝑛![\pi[i] = n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 成立，则字符串 𝑠![s](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 在字符串 𝑡![t](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的 𝑖 −(𝑛 −1) −(𝑛 +1) =𝑖 −2𝑛![i - (n - 1) - (n + 1) = i - 2n](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 处出现．下图所示为索引的示意图．

![img](https://oi-wiki.net/string/images/strstr_kmp_indices.svg)

正如在前缀函数的计算中已经提到的那样，如果我们知道前缀函数的值永远不超过一特定值，那么我们不需要存储整个字符串以及整个前缀函数，而只需要二者开头的一部分．在我们这种情况下这意味着只需要存储字符串 𝑠 +#![s + \#](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 以及相应的前缀函数值即可．我们可以一次读入字符串 𝑡![t](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的一个字符并计算当前位置的前缀函数值．

因此 Knuth–Morris–Pratt 算法（简称 KMP 算法）用 𝑂(𝑛 +𝑚)![O(n + m)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的时间以及 𝑂(𝑛)![O(n)](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7) 的内存解决了该问题．

模板代码：

```cpp
auto kmp = [&](string s, string t) {
    int n = s.size(), m = t.size();
	vector<int> kmp(m + 1), ans;
	s = "@" + s;
	t = "@" + t;
	for (int i = 2, j = 0; i <= m; i++) {
		while (j && t[i] != t[j + 1]) {
			j = kmp[j];
		}
	j += t[i] == t[j + 1];
	kmp[i] = j;
	}
	for (int i = 1, j = 0; i <= n; i++) {
		while (j && s[i] != t[j + 1]) {
			j = kmp[j];
		}
        if (s[i] == t[j + 1] && ++j == m) {
			ans.push_back(i - m + 1); // t 在 s 中出现的位置
		}
	}
		return ans;
};

```

### 5.3 ZFunction（拓展kmp）

同样出现在ZJSU2025校赛。手动害怕（）

大概就是求LCP（最长公共前缀）。可以获取字符串 $s$ 和 $s[i,n-1]$ 的最长公共前缀。时间 $O(N)$.

```cpp
vector<int> zFunction(string s) {
	int n = s.size();
	vector<int> z(n);
	z[0] = n;
	for (int i = 1, j = 1; i < n; i++) {
		z[i] = max(0, min(j + z[j] - i, z[i - j]));
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
			z[i]++;
		}
		if (i + z[i] > j + z[j]) {
			j = i;
		}
	}
	return z;
}
```

### 5.4 最长公共子序列LCS

求解两个串的最小公共子序列长度。

如果数据不大（1e3之内）

```cpp
const int N = 1e3 + 10;
char a[N], b[N];
int n, m, f[N][N];
void solve(){
	cin >> n >> m >> a + 1 >> b + 1;
	for (int i = 1; i <= n; i++)
	for (int j = 1; j <= m; j++){
		f[i][j] = max(f[i - 1][j], f[i][j - 1]);
		if (a[i] == b[j]) f[i][j] = max(f[i][j], f[i - 1][j - 1] + 1);
	}
	cout << f[n][m] << "\n";
}
int main(){
	solve();
	return 0;
}
```

如果遇到了变态的数据（1e5以上）

```cpp
const int INF = 0x7fffffff;
int n, a[maxn], b[maxn], f[maxn], p[maxn];
int main(){
	cin >> n;
	for (int i = 1; i <= n; i++){
		scanf("%d", &a[i]);
		p[a[i]] = i; //将第二个序列中的元素映射到第一个中
	}
	for (int i = 1; i <= n; i++){
		scanf("%d", &b[i]);
		f[i] = INF;
	}
	int len = 0;
	f[0] = 0;
	for (int i = 1; i <= n; i++){
		if (p[b[i]] > f[len]) f[++len] = p[b[i]];
		else {
			int l = 0, r = len;
			while (l < r){
				int mid = (l + r) >> 1;
				if (f[mid] > p[b[i]]) r = mid;
				else l = mid + 1;
			}
			f[l] = min(f[l], p[b[i]]);
			}
	}
	cout << len << "\n";
	return 0;
}
```



### 5.5字符串hash

#### 5.5.1 双hash封装

hash的核心控制内容在于尽可能避免或利用碰撞。这里是几个随机大质数：1111111121、1211111123、1311111119。

```cpp
const int N = 1 << 21;
static const int mod1 = 1E9 + 7, base1 = 127;
static const int mod2 = 1E9 + 9, base2 = 131;
using U = Zmod<mod1>;
using V = Zmod<mod2>;
vector<U> val1;
vector<V> val2;
void init(int n = N) {
	val1.resize(n + 1), val2.resize(n + 2);
	val1[0] = 1, val2[0] = 1;
	for (int i = 1; i <= n; i++) {
		val1[i] = val1[i - 1] * base1;
		val2[i] = val2[i - 1] * base2;
	}
}
struct String {
	vector<U> hash1;
	vector<V> hash2;
	string s;
	String(string s_) : s(s_), hash1{1}, hash2{1} {
		for (auto it : s) {
			hash1.push_back(hash1.back() * base1 + it);
			hash2.push_back(hash2.back() * base2 + it);
		}
	}
	pair<U, V> get() { // 输出整串的哈希值
        return {hash1.back(), hash2.back()};
	}
	pair<U, V> substring(int l, int r) { // 输出子串的哈希值
		if (l > r) swap(l, r);
		U ans1 = hash1[r + 1] - hash1[l] * val1[r - l + 1];
		V ans2 = hash2[r + 1] - hash2[l] * val2[r - l + 1];
		return {ans1, ans2};
	}
	pair<U, V> modify(int idx, char x) { // 修改 idx 位为 x
		int n = s.size() - 1;
		U ans1 = hash1.back() + val1[n - idx] * (x - s[idx]);
		V ans2 = hash2.back() + val2[n - idx] * (x - s[idx]);
		return {ans1, ans2};
	}
};
```

#### 5.5.2 前后缀去重

e.g. sample please ease 变成 samplease

```cpp
string compress(vector<string> in) { // 前后缀压缩
	vector<U> hash1{1};
	vector<V> hash2{1};
	string ans = "#";
	for (auto s : in) {
		s = "#" + s;
		int st = 0;
		U chk1 = 0;
		V chk2 = 0;
		for (int j = 1; j < s.size() && j < ans.size(); j++) {
			chk1 = chk1 * base1 + s[j];
			chk2 = chk2 * base2 + s[j];
			if ((hash1.back() == hash1[ans.size() - 1 - j] * val1[j] + chk1) &&(hash2.back() == hash2[ans.size() - 1 - j] * val2[j] + chk2)) {
				st = j;
			}
		}
		for (int j = st + 1; j < s.size(); j++) {
			ans += s[j];
			hash1.push_back(hash1.back() * base1 + s[j]);
			hash2.push_back(hash2.back() * base2 + s[j]);
		}
	}
	return ans.substr(1);
}
```

### 5.6 马拉车

可以用 $O(N)$的时间求出最长的回文串。

```cpp
string s;
cin >> s;
int n = s.length();
string t = "-#";
for (int i = 0; i < n; i++) {
	t += s[i];
	t += '#';
}
int m = t.length();
t += '+';
int mid = 0, r = 0;
vector<int> p(m);
for (int i = 1; i < m; i++) {
	p[i] = i < r ? min(p[2 * mid - i], r - i) : 1;
	while (t[i - p[i]] == t[i + p[i]]) p[i]++;
	if (i + p[i] > r) {
		r = i + p[i];
		mid = i;
	}
}	

```



## 0x06 OJ与卡常

### 6.1 关于各大OJ

HDUOJ：暂时未知（）

CF：支持CPP23

ZJSUOJ：默认C++11

Nowcoder：支持以下编译器：clang++18，gcc13

Luogu：支持C++11，C++14，C++17，C++20，C++23

**请注意：如gcd，lcm是在C++17的特性。**

