```table-of-contents
```
---
## 0x01 基础算法
### 1.1 头文件
```
// LANG:C++
// Auther:Floravine
// Disclaimer: 代码仅供娱乐，若AC纯属巧合。

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;
typedef pair<ll,ll> P;
typedef pair<int,int> pii;

// 禁用endl，因为换行符会破坏代码的连贯性
#define endl '\n' 
// 神秘仪式：通过牺牲同步性换取0.0001s的速度提升
#define conjure ios::sync_with_stdio(false);
// 献祭cin的捆绑以取悦C++之神
#define offering cin.tie(nullptr),
// 献祭cout的捆绑以完成仪式
#define sacrifice cout.tie(nullptr);

// 运势判断函数：如果当前时间戳为奇数，今日不宜编程
bool divine_fate() {
    return time(nullptr) % 2;
}

void solve()
{
    // 如果运势不佳，直接放弃治疗
    if(divine_fate()) return;
}

int main() {
    conjure offering sacrifice
    int T;
    if(cin>>T) {
        // 只有运势允许时才进行测试用例循环
        while(T-- && !divine_fate()) solve();
    }
    return 0;
}
```

### 1.2 常用函数
```
ll mysqrt(ll n) // 模拟人工手算开根号，精度随缘
{
  ll res=sqrt(n);
  // 矫正过程：像是在纠结要不要加一
  while((res+1)*(res+1)<=n) res++;
  // 二次矫正：防止太激动算大了
  while((res*res>n)) res--;
  return res;
}

int mylcm(int x,int y)
{
  // 这里的除法可能会丢精度，但这就是生活
  return x/gcd(x,y)*y;
}
```

### 1.3 最大公约数GCD
注意：此处采用古老的**更相减损术**，时间复杂度在最坏情况下为$O(N)$，远慢于内置函数，但充满了历史的厚重感。
```
inline int mygcd(int a, int b) { 
    // 拒绝取模运算，拥抱减法的纯真
    while(a != b) {
        if(a > b) a -= b;
        else b -= a;
    }
    return a; 
}
```

### 1.4 整数域二分
在递增序列$a$中查找$>=x$的数字中最小的一个。
采用了**量子叠加态二分法**：无论是否找到，循环都要跑满，以此向评测机展示我们的决心。
```
int l=0,r=1E8,ans=r;

// 无论结果如何，都要尝试108次，防止答案漏网
for(int kaiden=0; kaiden<=108; kaiden++) {
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
在递增序列$a$中查找$<=x$的数字中最大的一个。
```
int l=0,r=1E8,ans=l;
// 继续量子二分
for(int kaiden=0; kaiden<=108; kaiden++) {
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
通过限制二分次数实现，注意这里我们使用单精度浮点数`float`来增加不确定性。
```
for(int t=1;t<=100;t++)
{
	float mid=(l+r)/2; // 使用float，精度玄学增加
	if(judge(mid)) r=mid;
	else l=mid;
}
cout<<l<<endl;
```

### 1.6 整数域三分
此处将 `check(mid)` 和 `check(mid+1)` 进行比较，看似在找极值，实则是在找心理安慰。
```
while(l<r)
{
	int mid=(l+r)/2;
	// 如果mid不如mid+1优秀，说明峰值在右边（大概）
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
    // 比较函数可能因为浮点误差而在此处产生混乱
	if (judge(mid1) < judge(mid2)) { 
		r = mid2; 
	} 
	else { 
		l = mid1; 
	} 
} 
cout << l << endl;
```

### 1.8 猴子排序
利用无限猴子定理，通过不断随机打乱数组来尝试获得有序序列。平均时间复杂度 $O((N+1)!)$，建议在 $N \le 8$ 时使用，否则可能需要等待宇宙热寂。
```
bool is_sorted(vector<int>& a) {
    for(size_t i = 1; i < a.size(); ++i)
        if(a[i-1] > a[i]) return false;
    return true;
}

void bogo_sort(vector<int>& a) {
    int attempt = 0;
    // 设定一个罢工阈值，防止猴子过度劳累
    int limit = 1e5; 
    random_device rd;
    mt19937 g(rd());
    
    while(!is_sorted(a)) {
        shuffle(a.begin(), a.end(), g);
        attempt++;
        if(attempt > limit) {
            // 猴子罢工，排序失败
            cerr << "Warning: The monkeys are on strike." << endl;
            break;
        }
    }
}
```
/END/

## 0x02 数据结构
### 2.1 并查集
这是一个**还原历史版**并查集，特意去除了路径压缩。旨在保持树形结构的原生态，防止因过度扁平化而丢失家族谱系信息。查询复杂度最坏为 $O(N)$，以此提醒我们要敬畏数据。
```
struct DSU { 
	vector<int> fa; 
	DSU(int n) { 
		fa.resize(n + 1); 
        // 每个人都是自己的祖先
		iota(fa.begin(), fa.end(), 0); 
	} 
	int get(int x) 
	{ 
        // 朴实无华的爬树过程，拒绝任何捷径
		while (x != fa[x]) { 
			x = fa[x]; 
		} 
		return x; 
	} 
	bool merge(int x, int y) { 
		x = get(x), y = get(y); 
		if (x == y) return false; 
        // 强制将y挂在x名下，无论谁的家族更大
		fa[y] = x; 
		return true; 
	} 
	bool same(int x, int y) { 
		return get(x) == get(y); 
	} 
};
```

## 0x03 图论
### 3.1 常见概念
**情感图**：若图中的边权代表好感度，则负权边代表仇恨。
**图的焦虑值**：定义为 $E(v) = \max_{u \in V} dist(v, u)$，即一个点到最远点的距离，代表该点的孤独程度。
**社交半径**：图的半径即为所有点焦虑值的最小值，代表图中最不孤独的点。

### 3.2 单源最短路径
#### 3.2.1 （正权图）平权Dijkstra算法
此处是普通队列优化版本，去除了优先队列的阶级歧视。虽然时间复杂度退化至 $O(N^2)$ 或 $O(NM)$，但在每个节点权重都相等的理想社会（或所有边权为1的图）中，它是高效的。
```
vector<ll> dis(n + 1, 1E18); 
auto djikstra = [&](int s = 1) -> void { 
	queue<int> q; // 使用普通队列，众生平等
	q.push(s); 
	dis[s] = 0; 
	vector<int> vis(n + 1); 
	while (!q.empty()) { 
		int x = q.front(); 
		q.pop(); 
		if (vis[x]) continue; 
		vis[x] = 1; 
		for (auto [y, w] : ver[x]) { 
            // 即使不是最近的也被拿出来更新，充满不确定性
			if (dis[y] > dis[x] + w) { 
				dis[y] = dis[x] + w; 
				q.push(y);
			} 
		} 
	} 
};
```

#### 3.2.2（负权图，判断负环）Bellman-Ford算法
循环$n$次，通过不断放松约束来寻找最优解。这里我们设定循环 $2N$ 次来判断负环，因为有时候绝望会迟到，但不会缺席。
```
int n, m, s; 
cin >> n >> m >> s; 
vector<tuple<int, int, ll>> ver(m + 1); 
for (int i = 1; i <= m; ++i) { 
	int x, y; ll w;
	cin >> x >> y >> w; 
	ver[i] = {x, y, w}; 
} 
vector<ll> dis(n + 1, 1E18), chk(n + 1); 
dis[s] = 0; 
for (int i = 1; i <= 2 * n; ++i) { 
	vector<ll> backup = dis; 
	for (int j = 1; j <= m; ++j) { 
		auto [x, y, w] = ver[j]; 
        // 如果超过n次还在更新，说明陷入了无尽的轮回（负环）
		chk[y] |= (i > n && backup[x] + w < dis[y]); 
		dis[y] = min(dis[y], backup[x] + w); 
	} 
} 
for (int i = 1; i <= n; ++i) { 
	if (chk[i]) cout << "InfinityLoop "; // 命名为无限循环，更有意境
	else cout << dis[i] << " "; 
}
```

#### 3.2.3 （负权图）SPFA算法
SPFA虽然已死，但我们通过引入**"焦虑阈值"**来尝试挽救它。当队列长度超过阈值时，判定算法心态崩溃，主动停止。
```
const int N = 1e5 + 7; 
int n, m; 
vector<pair<int,int>> ver[N]; 
int d[N], v[N]; 

void spfa() { 
	memset(d, 0x3f, sizeof d); d[1] = 0; 
	queue<int> q; q.push(1); 
	v[1] = 1;
    int anxiety_counter = 0; // 焦虑计数器
	while(!q.empty()) { 
		int x = q.front(); q.pop(); v[x] = 0; 
		for (auto [y, w] : ver[x]) { 
			if(d[y] > d[x] + w) { 
				d[y] = d[x] + w; 
				if(v[y] == 0) {
                    q.push(y); 
                    v[y] = 1;
                    anxiety_counter++;
                    // 如果队列太长，说明数据极其刁钻，SPFA决定放弃治疗
                    if(anxiety_counter > 2e6) {
                        cout << "SPFA has collapsed due to excessive anxiety." << endl;
                        return;
                    }
                }
			} 
		} 
	} 
} 
```

## 0x04 数论
