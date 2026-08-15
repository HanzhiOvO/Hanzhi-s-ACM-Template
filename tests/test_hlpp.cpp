// 验证 HLPP vs Dinic 对拍
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;
using i64 = long long;
const i64 LINF = 4e18;

// ===== 04.18 HLPP（模板新代码）=====
struct HLPP {
    struct Edge { int to, rev; i64 cap; };
    int n, s, t, maxh;
    vector<vector<Edge>> g;
    vector<i64> excess;
    vector<int> h;
    vector<list<int>> bucket;
    HLPP(int n_) : n(n_) {
        g.resize(n);
        excess.assign(n, 0);
        h.assign(n, 0);
    }
    void add_edge(int u, int v, i64 cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }
    void push(int u, int id) {
        Edge& e = g[u][id];
        i64 f = min(excess[u], e.cap);
        if (!f) return;
        e.cap -= f;
        g[e.to][e.rev].cap += f;
        excess[u] -= f;
        if (excess[e.to] == 0 && e.to != s && e.to != t)
            bucket[h[e.to]].push_back(e.to);
        excess[e.to] += f;
    }
    void relabel(int u) {
        int nh = 2 * n + 1;
        for (auto& e : g[u]) if (e.cap > 0) nh = min(nh, h[e.to]);
        h[u] = nh + 1;
        bucket[h[u]].push_back(u);
        maxh = max(maxh, h[u]);
    }
    i64 max_flow(int s_, int t_) {
        s = s_; t = t_;
        h.assign(n, 0);
        h[s] = n;
        bucket.assign(2 * n + 5, {});
        maxh = n;
        for (int i = 0; i < (int)g[s].size(); ++i) {
            Edge& e = g[s][i];
            if (e.cap > 0) {
                i64 f = e.cap;
                e.cap = 0;
                g[e.to][e.rev].cap += f;
                excess[s] -= f;
                if (excess[e.to] == 0 && e.to != t)
                    bucket[h[e.to]].push_back(e.to);
                excess[e.to] += f;
            }
        }
        while (maxh >= 0) {
            if (bucket[maxh].empty()) { --maxh; continue; }
            int u = bucket[maxh].back();
            bucket[maxh].pop_back();
            if (u == s || u == t || h[u] != maxh) continue;
            for (int i = 0; i < (int)g[u].size() && excess[u] > 0; ++i) {
                Edge& e = g[u][i];
                if (e.cap > 0 && h[e.to] == h[u] - 1) push(u, i);
            }
            if (excess[u] > 0) relabel(u);
        }
        return excess[t];
    }
};

// ===== 04.15 Dinic（对拍基准）=====
struct Dinic {
    struct Edge { int to, rev; i64 cap; };
    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;
    Dinic(int n_ = 0) { init(n_); }
    void init(int n_) { n = n_; g.assign(n + 1, {}); level.assign(n + 1, 0); it.assign(n + 1, 0); }
    void add_edge(int u, int v, i64 cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0; q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& e : g[u]) if (e.cap > 0 && level[e.to] == -1) {
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
            i64 ret = dfs(e.to, t, min(f, e.cap));
            if (ret) { e.cap -= ret; g[e.to][e.rev].cap += ret; return ret; }
        }
        return 0;
    }
    i64 max_flow(int s, int t) {
        i64 flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (i64 f = dfs(s, t, (1LL << 62))) flow += f;
        }
        return flow;
    }
};

int main() {
    mt19937 rng(99);
    // 1) 小图密集对拍（n <= 10，边概率高 = 稠密）
    for (int t = 0; t < 300; ++t) {
        int n = rng() % 9 + 2;
        HLPP hp(n);
        Dinic d(n);
        for (int u = 0; u < n; ++u)
            for (int v = 0; v < n; ++v) if (u != v && rng() % 2 == 0) {  // 50% 稠密
                i64 c = rng() % 100 + 1;
                hp.add_edge(u, v, c);
                d.add_edge(u + 1, v + 1, c);
            }
        int ss = rng() % n, tt = rng() % n;
        while (tt == ss) tt = rng() % n;
        i64 a = hp.max_flow(ss, tt);
        i64 b = d.max_flow(ss + 1, tt + 1);
        if (a != b) {
            printf("HLPP FAIL t=%d n=%d got=%lld want=%lld\n", t, n, (long long)a, (long long)b);
            return 1;
        }
    }
    printf("HLPP: 300 组稠密小图 vs Dinic 全部通过\n");
    // 2) 中等图（n=50，随机稀疏）
    for (int t = 0; t < 30; ++t) {
        int n = 50;
        HLPP hp(n);
        Dinic d(n);
        for (int u = 0; u < n; ++u)
            for (int v = 0; v < n; ++v) if (u != v && rng() % 20 == 0) {
                i64 c = rng() % 1000 + 1;
                hp.add_edge(u, v, c);
                d.add_edge(u + 1, v + 1, c);
            }
        i64 a = hp.max_flow(0, n - 1);
        i64 b = d.max_flow(1, n);
        if (a != b) { printf("HLPP med FAIL t=%d got=%lld want=%lld\n", t, (long long)a, (long long)b); return 1; }
    }
    printf("HLPP: 30 组 n=50 图 vs Dinic 全部通过\n");
    // 3) 已知小例：s=0 到 t=3，边 0-1(3) 0-2(2) 1-2(1) 1-3(2) 2-3(3) → 最大流 5
    {
        HLPP hp(4);
        hp.add_edge(0, 1, 3); hp.add_edge(0, 2, 2);
        hp.add_edge(1, 2, 1); hp.add_edge(1, 3, 2); hp.add_edge(2, 3, 3);
        i64 got = hp.max_flow(0, 3);
        printf("经典例: got=%lld 期望 5\n", (long long)got);
        if (got != 5) return 1;
    }
    return 0;
}
