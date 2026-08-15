// 综合验证 2：FHQTreap + FHQSeq + MCMF + gauss_real + det_mod + convex_hull
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;
using i64 = long long;
#define all(x) (x).begin(), (x).end()
const i64 LINF = 4e18;
i64 mod_pow(i64 a, i64 e, i64 mod) {
    i64 r = 1 % mod; a %= mod;
    while (e) { if (e & 1) r = r * a % mod; a = a * a % mod; e >>= 1; }
    return r;
}

// ===== 02.24 FHQTreap =====
struct FHQTreap {
    struct Node { int l = 0, r = 0, pri, sz = 1, cnt = 1; i64 val; };
    vector<Node> tr{Node{}};
    mt19937 rng{random_device{}()};
    FHQTreap() { tr[0].sz = 0; tr[0].cnt = 0; }
    int new_node(i64 x) {
        tr.push_back(Node{});
        tr.back().pri = (int)rng(); tr.back().val = x;
        return (int)tr.size() - 1;
    }
    void pull(int p) { tr[p].sz = tr[tr[p].l].sz + tr[tr[p].r].sz + tr[p].cnt; }
    void split(int p, i64 key, int& a, int& b) {
        if (!p) { a = b = 0; return; }
        if (tr[p].val < key) { a = p; split(tr[p].r, key, tr[p].r, b); }
        else { b = p; split(tr[p].l, key, a, tr[p].l); }
        pull(p);
    }
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (tr[a].pri > tr[b].pri) { tr[a].r = merge(tr[a].r, b); pull(a); return a; }
        else { tr[b].l = merge(a, tr[b].l); pull(b); return b; }
    }
    int root = 0;
    void insert(i64 x) {
        int a, b, c;
        split(root, x, a, b);
        split(b, x + 1, b, c);
        if (b) tr[b].cnt++, pull(b);
        else b = new_node(x);
        root = merge(merge(a, b), c);
    }
    void erase(i64 x) {
        int a, b, c;
        split(root, x, a, b);
        split(b, x + 1, b, c);
        if (b) {
            if (--tr[b].cnt == 0) b = 0;
            else pull(b);
        }
        root = merge(merge(a, b), c);
    }
    i64 kth(int k) const {
        int p = root;
        while (p) {
            int lsz = tr[tr[p].l].sz;
            if (k <= lsz) p = tr[p].l;
            else if (k <= lsz + tr[p].cnt) return tr[p].val;
            else k -= lsz + tr[p].cnt, p = tr[p].r;
        }
        return -1;
    }
    int rank(i64 x) const {
        int p = root, res = 0;
        while (p) {
            if (tr[p].val < x) res += tr[tr[p].l].sz + tr[p].cnt, p = tr[p].r;
            else p = tr[p].l;
        }
        return res;
    }
    int count(i64 x) const {
        int p = root;
        while (p) {
            if (tr[p].val == x) return tr[p].cnt;
            p = tr[p].val < x ? tr[p].r : tr[p].l;
        }
        return 0;
    }
    i64 predecessor(i64 x) const {
        int c = rank(x);
        return c >= 1 ? kth(c) : -LINF;
    }
    i64 successor(i64 x) const {
        int c = rank(x) + count(x);
        return c < tr[root].sz ? kth(c + 1) : LINF;
    }
};

// ===== 02.25 FHQSeq =====
struct FHQSeq {
    struct Node { int l = 0, r = 0, pri, sz = 1; i64 val, sum; bool rev = false; };
    vector<Node> tr{Node{}};
    mt19937 rng{random_device{}()};
    FHQSeq() { tr[0].sz = 0; }
    int new_node(i64 x) {
        tr.push_back(Node{});
        tr.back().pri = (int)rng();
        tr.back().val = tr.back().sum = x;
        return (int)tr.size() - 1;
    }
    void pull(int p) {
        tr[p].sz = tr[tr[p].l].sz + tr[tr[p].r].sz + 1;
        tr[p].sum = tr[tr[p].l].sum + tr[tr[p].r].sum + tr[p].val;
    }
    void push(int p) {
        if (tr[p].rev) {
            swap(tr[p].l, tr[p].r);
            if (tr[p].l) tr[tr[p].l].rev ^= 1;
            if (tr[p].r) tr[tr[p].r].rev ^= 1;
            tr[p].rev = false;
        }
    }
    void split(int p, int k, int& a, int& b) {
        if (!p) { a = b = 0; return; }
        push(p);
        if (tr[tr[p].l].sz >= k) { b = p; split(tr[p].l, k, a, tr[p].l); }
        else { a = p; split(tr[p].r, k - tr[tr[p].l].sz - 1, tr[p].r, b); }
        pull(p);
    }
    int merge(int a, int b) {
        if (!a || !b) return a ? a : b;
        if (tr[a].pri > tr[b].pri) { push(a); tr[a].r = merge(tr[a].r, b); pull(a); return a; }
        else { push(b); tr[b].l = merge(a, tr[b].l); pull(b); return b; }
    }
    int root = 0;
    void insert(int pos, i64 x) {
        int a, b;
        split(root, pos, a, b);
        root = merge(merge(a, new_node(x)), b);
    }
    void erase(int l, int r) {
        int a, b, c;
        split(root, l - 1, a, b);
        split(b, r - l + 1, b, c);
        root = merge(a, c);
    }
    void reverse(int l, int r) {
        int a, b, c;
        split(root, l - 1, a, b);
        split(b, r - l + 1, b, c);
        tr[b].rev ^= 1;
        root = merge(merge(a, b), c);
    }
    i64 range_sum(int l, int r) {
        int a, b, c;
        split(root, l - 1, a, b);
        split(b, r - l + 1, b, c);
        i64 res = tr[b].sum;
        root = merge(merge(a, b), c);
        return res;
    }
    void dump(int p, vector<i64>& out) {
        if (!p) return;
        push(p);
        dump(tr[p].l, out);
        out.push_back(tr[p].val);
        dump(tr[p].r, out);
    }
    vector<i64> dump() { vector<i64> res; dump(root, res); return res; }
};

// ===== 04.16 MCMF =====
struct MCMF {
    struct Edge { int to, rev; i64 cap, cost; };
    int n;
    vector<vector<Edge>> g;
    vector<i64> dist, h;
    vector<int> pv, pe;
    MCMF(int n_) : n(n_) { g.resize(n + 1); dist.resize(n + 1); h.assign(n + 1, 0); pv.resize(n + 1); pe.resize(n + 1); }
    void add_edge(int u, int v, i64 cap, i64 cost) {
        g[u].push_back({v, (int)g[v].size(), cap, cost});
        g[v].push_back({u, (int)g[u].size() - 1, 0, -cost});
    }
    bool dijkstra(int s, int t) {
        fill(dist.begin(), dist.end(), LINF);
        priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<>> pq;
        dist[s] = 0; pq.push({0, s});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            for (int i = 0; i < (int)g[u].size(); ++i) {
                auto& e = g[u][i];
                if (e.cap > 0 && dist[e.to] > d + e.cost + h[u] - h[e.to]) {
                    dist[e.to] = d + e.cost + h[u] - h[e.to];
                    pv[e.to] = u; pe[e.to] = i;
                    pq.push({dist[e.to], e.to});
                }
            }
        }
        return dist[t] != LINF;
    }
    pair<i64, i64> min_cost_max_flow(int s, int t) {
        i64 flow = 0, cost = 0;
        fill(h.begin(), h.end(), 0);
        for (int it = 0; it < n; ++it) {
            bool upd = false;
            for (int u = 1; u <= n; ++u)
                for (auto& e : g[u])
                    if (e.cap > 0 && h[e.to] > h[u] + e.cost) { h[e.to] = h[u] + e.cost; upd = true; }
            if (!upd) break;
        }
        while (dijkstra(s, t)) {
            for (int u = 1; u <= n; ++u) if (dist[u] != LINF) h[u] += dist[u];
            i64 f = (1LL << 62);
            for (int v = t; v != s; v = pv[v]) f = min(f, g[pv[v]][pe[v]].cap);
            for (int v = t; v != s; v = pv[v]) {
                auto& e = g[pv[v]][pe[v]];
                e.cap -= f;
                g[v][e.rev].cap += f;
            }
            flow += f;
            cost += f * (h[t] - h[s]);
        }
        return {flow, cost};
    }
};

// ===== 朴素 SPFA MCMF（对拍用）=====
struct BruteMCMF {
    struct Edge { int to, rev; i64 cap, cost; };
    int n;
    vector<vector<Edge>> g;
    BruteMCMF(int n_) : n(n_) { g.resize(n + 1); }
    void add_edge(int u, int v, i64 cap, i64 cost) {
        g[u].push_back({v, (int)g[v].size(), cap, cost});
        g[v].push_back({u, (int)g[u].size() - 1, 0, -cost});
    }
    bool has_neg_cycle() {
        vector<i64> d(n + 1, 0);
        for (int it = 0; it < n; ++it) {
            bool upd = false;
            for (int u = 1; u <= n; ++u)
                for (auto& e : g[u])
                    if (e.cap > 0 && d[e.to] > d[u] + e.cost) { d[e.to] = d[u] + e.cost; upd = true; }
            if (!upd) return false;
        }
        return true;
    }
    pair<i64, i64> run(int s, int t) {
        i64 flow = 0, cost = 0;
        while (true) {
            vector<i64> d(n + 1, LINF);
            vector<int> inq(n + 1, 0), pv(n + 1, -1), pe(n + 1, -1), cnt(n + 1, 0);
            queue<int> q;
            d[s] = 0; q.push(s); inq[s] = 1;
            while (!q.empty()) {
                int u = q.front(); q.pop(); inq[u] = 0;
                for (int i = 0; i < (int)g[u].size(); ++i) {
                    auto& e = g[u][i];
                    if (e.cap > 0 && d[e.to] > d[u] + e.cost) {
                        d[e.to] = d[u] + e.cost;
                        pv[e.to] = u; pe[e.to] = i;
                        if (!inq[e.to]) {
                            if (++cnt[e.to] > n + 1) return {-1, -1};   // 负环（费用流不允许）
                            q.push(e.to); inq[e.to] = 1;
                        }
                    }
                }
            }
            if (d[t] == LINF) break;
            i64 f = (1LL << 62);
            for (int v = t; v != s; v = pv[v]) f = min(f, g[pv[v]][pe[v]].cap);
            for (int v = t; v != s; v = pv[v]) {
                auto& e = g[pv[v]][pe[v]];
                e.cap -= f;
                g[v][e.rev].cap += f;
            }
            flow += f;
            cost += f * d[t];
        }
        return {flow, cost};
    }
};

// ===== 07.2 gauss_real =====
vector<double> gauss_real(vector<vector<double>> a) {
    int n = (int)a.size();
    for (int col = 0, row = 0; col < n && row < n; ++col) {
        int piv = row;
        for (int i = row + 1; i < n; ++i)
            if (fabs(a[i][col]) > fabs(a[piv][col])) piv = i;
        if (fabs(a[piv][col]) < 1e-12) continue;
        swap(a[piv], a[row]);
        for (int i = 0; i < n; ++i) if (i != row) {
            double f = a[i][col] / a[row][col];
            for (int j = col; j <= n; ++j) a[i][j] -= f * a[row][j];
        }
        ++row;
    }
    for (int i = 0; i < n; ++i) {
        bool allzero = true;
        for (int j = 0; j < n; ++j) if (fabs(a[i][j]) > 1e-12) { allzero = false; break; }
        if (allzero && fabs(a[i][n]) > 1e-12) return {};
    }
    vector<double> ans(n);
    for (int i = 0; i < n; ++i) {
        int col = -1;
        for (int j = 0; j < n; ++j) if (fabs(a[i][j]) > 1e-12) { col = j; break; }
        if (col != -1) ans[col] = a[i][n] / a[i][col];
    }
    return ans;
}

// ===== 07.3 det_mod（质数模版）=====
static i64 det_mod(vector<vector<i64>> a, i64 MOD) {
    int n = (int)a.size();
    i64 ans = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j)
            if (a[i][i] == 0 && a[j][i] != 0) { swap(a[i], a[j]); ans = (MOD - ans) % MOD; }
        if (a[i][i] == 0) return 0;
        ans = ans * (a[i][i] % MOD + MOD) % MOD;
        i64 inv = mod_pow(a[i][i], MOD - 2, MOD);
        for (int j = i + 1; j < n; ++j) if (a[j][i]) {
            i64 f = a[j][i] * inv % MOD;
            for (int k = i; k < n; ++k)
                a[j][k] = (a[j][k] - f * a[i][k] % MOD + MOD) % MOD;
        }
    }
    return ans;
}

// ===== 08.1 Point + 08.3 convex_hull =====
const double EPS = 1e-9;
int sgn(double x) { return x < -EPS ? -1 : x > EPS ? 1 : 0; }
struct Point {
    double x, y;
    Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
    Point operator+(const Point& o) const { return {x + o.x, y + o.y}; }
    Point operator-(const Point& o) const { return {x - o.x, y - o.y}; }
    Point operator*(double k) const { return {x * k, y * k}; }
    double dot(const Point& o) const { return x * o.x + y * o.y; }
    double cross(const Point& o) const { return x * o.y - y * o.x; }
};
double cross(const Point& o, const Point& a, const Point& b) { return (a - o).cross(b - o); }
double dot(const Point& o, const Point& a, const Point& b) { return (a - o).dot(b - o); }
vector<Point> convex_hull(vector<Point> p) {
    sort(all(p), [](const Point& a, const Point& b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    });
    p.erase(unique(all(p), [](const Point& a, const Point& b) {
        return a.x == b.x && a.y == b.y;
    }), p.end());
    int n = (int)p.size();
    if (n <= 1) return p;
    vector<Point> h(2 * n);
    int k = 0;
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && sgn(cross(h[k - 2], h[k - 1], p[i])) <= 0) --k;
        h[k++] = p[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; --i) {
        while (k >= t && sgn(cross(h[k - 2], h[k - 1], p[i])) <= 0) --k;
        h[k++] = p[i];
    }
    h.resize(k - 1);
    return h;
}
// 点在凸多边形内（含边界），O(m)
bool in_convex(const vector<Point>& h, const Point& p) {
    int m = (int)h.size();
    if (m < 3) return false;
    for (int i = 0; i < m; ++i) {
        Point a = h[i], b = h[(i + 1) % m];
        if (sgn(cross(a, b, p)) < 0) return false;   // 凸包逆时针，内部点叉积 >= 0
    }
    return true;
}

int main() {
    mt19937 rng(2026);
    // 1) FHQTreap vs multiset
    {
        for (int t = 0; t < 200; ++t) {
            FHQTreap tp;
            multiset<i64> st;
            for (int op = 0; op < 500; ++op) {
                int type = rng() % 5;
                i64 x = rng() % 100;
                if (type <= 1) { tp.insert(x); st.insert(x); }
                else if (type == 2) {
                    if (st.empty()) continue;
                    auto it = st.find(x);
                    if (it != st.end()) { tp.erase(x); st.erase(it); }
                } else if (type == 3) {  // kth / rank 对拍
                    if (st.empty()) continue;
                    int k = rng() % (int)st.size() + 1;
                    auto it = st.begin(); advance(it, k - 1);
                    if (tp.kth(k) != *it) { printf("FHQTreap kth FAIL t=%d\n", t); return 1; }
                    if (tp.rank(x) != (int)st.count(x)) {
                        // rank = < x 的个数
                        int c = 0; for (auto v : st) if (v < x) ++c;
                        if (tp.rank(x) != c) { printf("FHQTreap rank FAIL t=%d x=%lld\n", t, (long long)x); return 1; }
                    }
                } else {  // pred / succ
                    int c = 0; i64 pr = -LINF; for (auto v : st) if (v < x) { c++; pr = v; }
                    if (tp.predecessor(x) != (c ? pr : -LINF)) { printf("FHQTreap pred FAIL t=%d x=%lld\n", t, (long long)x); return 1; }
                    i64 su = LINF; for (auto v : st) if (v > x) { su = v; break; }
                    if (tp.successor(x) != su) { printf("FHQTreap succ FAIL t=%d x=%lld\n", t, (long long)x); return 1; }
                }
                if (tp.count(x) != (int)st.count(x)) { printf("FHQTreap count FAIL t=%d x=%lld\n", t, (long long)x); return 1; }
            }
        }
        printf("FHQTreap: 200 组 x 500 随机操作 vs multiset 全部通过\n");
    }
    // 2) FHQSeq vs vector
    {
        for (int t = 0; t < 200; ++t) {
            FHQSeq seq;
            vector<i64> v;
            for (int op = 0; op < 400; ++op) {
                int type = rng() % 4;
                if (type == 0) {  // 插入
                    int pos = rng() % ((int)v.size() + 1);
                    i64 x = rng() % 100;
                    seq.insert(pos, x);
                    v.insert(v.begin() + pos, x);
                } else if (type == 1) {  // 删除
                    if (v.empty()) continue;
                    int l = rng() % (int)v.size(), r = rng() % (int)v.size();
                    if (l > r) swap(l, r);
                    seq.erase(l + 1, r + 1);
                    v.erase(v.begin() + l, v.begin() + r + 1);
                } else if (type == 2) {  // 翻转
                    if (v.empty()) continue;
                    int l = rng() % (int)v.size(), r = rng() % (int)v.size();
                    if (l > r) swap(l, r);
                    seq.reverse(l + 1, r + 1);
                    reverse(v.begin() + l, v.begin() + r + 1);
                } else {  // 区间和
                    if (v.empty()) continue;
                    int l = rng() % (int)v.size(), r = rng() % (int)v.size();
                    if (l > r) swap(l, r);
                    i64 want = 0; for (int i = l; i <= r; ++i) want += v[i];
                    if (seq.range_sum(l + 1, r + 1) != want) { printf("FHQSeq sum FAIL t=%d\n", t); return 1; }
                }
                auto got = seq.dump();
                if (got != v) { printf("FHQSeq dump FAIL t=%d\n", t); return 1; }
            }
        }
        printf("FHQSeq: 200 组 x 400 随机操作（插入/删除/翻转/区间和）vs vector 全部通过\n");
    }
    // 3) MCMF vs 朴素 SPFA
    {
        for (int t = 0; t < 200; ++t) {
            int n = rng() % 7 + 2;
            MCMF m1(n); BruteMCMF m2(n);
            for (int u = 1; u <= n; ++u)
                for (int v = 1; v <= n; ++v) if (u != v && rng() % 3 == 0) {
                    i64 cap = rng() % 8 + 1;
                    i64 cost = (i64)((int)(rng() % 21) - 10);   // 含负费用（无负环，SPFA 可处理）
                    if (t == 2) printf("  ORIG %d->%d cap=%lld cost=%lld\n", u, v, (long long)cap, (long long)cost);
                    m1.add_edge(u, v, cap, cost);
                    m2.add_edge(u, v, cap, cost);
                }
            if (m2.has_neg_cycle()) continue;   // 该图含负环，跳过（费用流要求无负环）
            auto a = m1.min_cost_max_flow(1, n);
            auto b = m2.run(1, n);
            if (a != b) {
                printf("MCMF FAIL t=%d n=%d got=(%lld,%lld) want=(%lld,%lld)\n", t, n, (long long)a.first, (long long)a.second, (long long)b.first, (long long)b.second);
                for (int u = 1; u <= n; ++u)
                    for (auto& e : m1.g[u]) if (e.cap > 0)
                        printf("  %d->%d cap=%lld cost=%lld\n", u, e.to, (long long)e.cap, (long long)e.cost);
                return 1;
            }
        }
        printf("MCMF: 200 组随机图（含负费用边）vs 朴素 SPFA 全部通过\n");
    }
    // 4) gauss_real：随机满秩方程组，验证解残差
    {
        for (int t = 0; t < 100; ++t) {
            int n = rng() % 6 + 2;
            vector<vector<double>> a(n, vector<double>(n + 1));
            for (int i = 0; i < n; ++i)
                for (int j = 0; j <= n; ++j) a[i][j] = (double)((int)(rng() % 21) - 10);
            for (int i = 0; i < n; ++i) a[i][i] += 200;   // 严格对角占优保证满秩
            auto x = gauss_real(a);
            if ((int)x.size() != n) { printf("gauss FAIL t=%d size=%zu\n", t, x.size()); return 1; }
            for (int i = 0; i < n; ++i) {
                double s = 0;
                for (int j = 0; j < n; ++j) s += a[i][j] * x[j];
                if (fabs(s - a[i][n]) > 1e-6) {
                    printf("gauss residual FAIL t=%d n=%d row=%d diff=%.2e\n", t, n, i, fabs(s - a[i][n]));
                    for (int r = 0; r < n; ++r) {
                        printf("  row%d:", r);
                        for (int j = 0; j <= n; ++j) printf(" %.0f", a[r][j]);
                        printf("\n");
                    }
                    printf("  x:");
                    for (auto v : x) printf(" %.3f", v);
                    printf("\n");
                    return 1;
                }
            }
        }
        printf("gauss_real: 100 组随机方程组残差验证全部通过\n");
    }
    // 5) det_mod vs 暴力（n <= 5 全排列）
    {
        for (int t = 0; t < 100; ++t) {
            int n = rng() % 4 + 2;
            i64 MOD = 1000000007LL;
            vector<vector<i64>> a(n, vector<i64>(n));
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j) a[i][j] = rng() % 20;
            i64 got = det_mod(a, MOD);
            // 暴力全排列
            vector<int> p(n); iota(p.begin(), p.end(), 0);
            i64 want = 0;
            do {
                i64 term = 1;
                for (int i = 0; i < n; ++i) term = term * a[i][p[i]] % MOD;
                // 逆序对奇偶
                int inv = 0;
                for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) if (p[i] > p[j]) ++inv;
                if (inv & 1) want = (want - term + MOD) % MOD;
                else want = (want + term) % MOD;
            } while (next_permutation(p.begin(), p.end()));
            if (got != want) { printf("det_mod FAIL t=%d got=%lld want=%lld\n", t, (long long)got, (long long)want); return 1; }
        }
        printf("det_mod: 100 组随机矩阵 vs 暴力全排列 全部通过\n");
    }
    // 6) convex_hull：所有点都在凸包内（含边界）+ 凸包顶点确在点上
    {
        for (int t = 0; t < 100; ++t) {
            int n = rng() % 30 + 3;
            vector<Point> pts;
            for (int i = 0; i < n; ++i) pts.push_back({(double)(rng() % 100), (double)(rng() % 100)});
            auto h = convex_hull(pts);
            // 所有点在凸包内
            for (auto& p : pts) if (!in_convex(h, p)) { printf("hull contain FAIL t=%d (%.0f,%.0f)\n", t, p.x, p.y); return 1; }
            // 凸包顶点在点集内
            for (auto& q : h) {
                bool ok = false;
                for (auto& p : pts) if (fabs(p.x - q.x) < 1e-9 && fabs(p.y - q.y) < 1e-9) { ok = true; break; }
                if (!ok) { printf("hull vertex FAIL t=%d\n", t); return 1; }
            }
            // 凸包严格凸（逆时针，叉积 > 0）
            int m = (int)h.size();
            if (m >= 3)
                for (int i = 0; i < m; ++i) {
                    Point a = h[i], b = h[(i + 1) % m], c = h[(i + 2) % m];
                    if (sgn(cross(b, c, a)) <= 0 && sgn(cross(b, c, a)) != 0 && false) {}
                    if (sgn(cross(a, b, c)) <= 0) { printf("hull strict FAIL t=%d\n", t); return 1; }
                }
        }
        printf("convex_hull: 100 组随机点集（包含性/顶点/严格凸）全部通过\n");
    }
    return 0;
}
