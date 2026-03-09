```table-of-contents
```
---
## 0x01 基础算法
### 1.1 头文件
```
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
```
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
```
inline int mygcd(int a, int b) { return b ? gcd(b, a % b) : a; }
```

### 1.4 整数域二分
在递增序列$a$中查找$>=x$的数字中最小的一个（即$x$或$x$的后继）
```
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
```
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
```
for(int t=1;t<=100;t++)
{
	float mid=(l+r)/2;
	if(judge(mid)) r=mid;
	else l=mid;
}
cout<<l<<endl;
```

### 1.6 整数域三分
```
while(l<r)
{
	int mid=(l+r)/2;
	if(check(mid)<=check(mid+1)) r=mid;
	else l=mid+1;
}
cout<<check(l)<<endl;
```

### 1.7 实数域三分
```
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
/END/

## 0x02 数据结构
### 2.1 并查集
```
struct DSU { 
	vector fa, p, e, f;
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

## 0x03 图论
### 3.1 常见概念
平面图：若能将无向图 画在平面上使得任意两条无重合顶点的边不相交，则称 是平面图。 
无向正权图上某一点的偏心距：记为 ，即以这个点为源，到其他点的 所有最短路的最大值。如下图 点， 即为 。 图的直径：定义为 ，即最大的偏心距，亦可以简化为图中最远的一对点的距 离。 图的中心：定义为 ，即偏心距最小的点。如下图，图的中心即为 点。 图的绝对中心：可以定义在边上的图的中心。 图的半径：图的半径不同于圆的半径，其不等于直径的一半（但对于绝对中心定义上的直径而言是一 半）。定义为 ，即中心的偏心距。计算方式：使用全源最短路，计算出所有点的偏心 距，再加以计算。

### 3.2 单源最短路径
#### 3.2.1 （正权稀疏图）动态数组存图+Djikstra算法
此处是优先队列优化版本，为$O(M log N)$的复杂度。
```
vector dis(n + 1, 1E18); 
auto djikstra = [&](int s = 1) -> void { 
	using PII = pair<int, int>; 
	priority_queue<PII, ,vector<PII>, greater<PII>> q;
	q.emplace(0, s); 
	dis[s] = 0; 
	vector vis(n + 1); 
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
```
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
```
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

## 0x04 数论
