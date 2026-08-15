// 验证 03.20 动态点分治：随机树 + 随机 toggle/query vs 暴力
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;
using i64 = long long;
const i64 LINF = 4e18;

// ===== 03.20 DynCentroid（从模板提取）=====
struct DynCentroid {
    int n;
    vector<vector<pair<int, i64>>> g;
    vector<int> dep;
    vector<vector<int>> up;
    vector<i64> dist;
    int LOG;
    void dfs_lca(int u, int p) {
        for (auto [v, w] : g[u]) if (v != p) {
            dep[v] = dep[u] + 1;
            dist[v] = dist[u] + w;
            up[0][v] = u;
            for (int k = 1; k < LOG; ++k) up[k][v] = up[k - 1][up[k - 1][v]];
            dfs_lca(v, u);
        }
    }
    int lca(int u, int v) const {
        if (dep[u] < dep[v]) swap(u, v);
        for (int k = LOG - 1; k >= 0; --k) if (dep[u] - (1 << k) >= dep[v]) u = up[k][u];
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; --k) if (up[k][u] != up[k][v]) { u = up[k][u]; v = up[k][v]; }
        return up[0][u];
    }
    i64 tree_dist(int u, int v) const { int w = lca(u, v); return dist[u] + dist[v] - 2 * dist[w]; }
    vector<int> sz, vis, cfa;
    vector<vector<pair<int, i64>>> paths;
    void calc_sz(int u, int p) {
        sz[u] = 1;
        for (auto [v, w] : g[u]) if (v != p && !vis[v]) { calc_sz(v, u); sz[u] += sz[v]; }
    }
    int find_cen(int u, int p, int tot) {
        for (auto [v, w] : g[u]) if (v != p && !vis[v] && sz[v] > tot / 2)
            return find_cen(v, u, tot);
        return u;
    }
    void collect(int u, int p, int cen, i64 d) {
        paths[u].push_back({cen, d});
        for (auto [v, w] : g[u]) if (v != p && !vis[v]) collect(v, u, cen, d + w);
    }
    void build(int u, int fa) {
        calc_sz(u, 0);
        int c = find_cen(u, 0, sz[u]);
        cfa[c] = fa;
        collect(c, 0, c, 0);
        vis[c] = 1;
        for (auto [v, w] : g[c]) if (!vis[v]) build(v, c);
    }
    vector<multiset<i64>> A, B, C;
    vector<char> on;
    void erase_one(multiset<i64>& st, i64 x) { auto it = st.find(x); if (it != st.end()) st.erase(it); }
    void toggle_ok(int x) {
        on[x] ^= 1;
        int m = (int)paths[x].size();
        auto upd = [&](int cen, int par, i64 oldc, i64 newc) {
            if (oldc == newc) return;
            erase_one(C[par], oldc);
            if (newc != LINF) C[par].insert(newc);
        };
        if (on[x]) C[x].insert(0); else erase_one(C[x], 0);
        for (int i = 0; i < m; ++i) {
            int cen = paths[x][i].first;
            i64 d = paths[x][i].second;
            i64 db = (i >= 1) ? paths[x][i - 1].second : 0;
            i64 oldContrib = (i >= 1 && !B[cen].empty()) ? *B[cen].begin() : LINF;
            if (on[x]) { A[cen].insert(d); if (i >= 1) B[cen].insert(db); }
            else       { erase_one(A[cen], d); if (i >= 1) erase_one(B[cen], db); }
            if (i >= 1) {
                i64 newContrib = B[cen].empty() ? LINF : *B[cen].begin();
                upd(cen, cfa[cen], oldContrib, newContrib);
            }
        }
    }
    i64 query() {
        i64 best = LINF;
        for (int u = 1; u <= n; ++u) {
            if (C[u].size() < 2) continue;
            auto it = C[u].begin(); i64 a = *it; ++it;
            best = min(best, a + *it);
        }
        return best == LINF ? -1 : best;
    }
    DynCentroid(int n_) : n(n_) {
        g.resize(n + 1);
        LOG = 1; while ((1 << LOG) <= n) ++LOG;
        up.assign(LOG, vector<int>(n + 1, 0));
        dep.assign(n + 1, 0); dist.assign(n + 1, 0);
        sz.assign(n + 1, 0); vis.assign(n + 1, 0); cfa.assign(n + 1, 0);
        paths.resize(n + 1);
        A.assign(n + 1, {}); B.assign(n + 1, {}); C.assign(n + 1, {});
        on.assign(n + 1, 0);
    }
    void add_edge(int u, int v, i64 w) { g[u].push_back({v, w}); g[v].push_back({u, w}); }
    void build() { dfs_lca(1, 0); build(1, 0); }
    void toggle(int x) { toggle_ok(x); }
};

// 暴力：亮灯点两两距离最小值
i64 brute(const vector<vector<pair<int, i64>>>& g, const vector<char>& on, int n) {
    // Floyd 全源最短路
    static i64 d[40][40];
    for (int i = 1; i <= n; ++i) { for (int j = 1; j <= n; ++j) d[i][j] = LINF; d[i][i] = 0; }
    for (int u = 1; u <= n; ++u) for (auto [v, w] : g[u]) d[u][v] = min(d[u][v], w);
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= n; ++j)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    i64 best = LINF;
    for (int u = 1; u <= n; ++u) if (on[u])
        for (int v = u + 1; v <= n; ++v) if (on[v]) best = min(best, d[u][v]);
    return best == LINF ? -1 : best;
}

int main() {
    mt19937 rng(2026);
    for (int t = 0; t < 100; ++t) {
        int n = rng() % 20 + 5;
        DynCentroid dc(n);
        vector<vector<pair<int, i64>>> g(n + 1);
        for (int u = 2; u <= n; ++u) {
            int p = rng() % (u - 1) + 1;
            i64 w = rng() % 10 + 1;
            dc.add_edge(u, p, w);
            g[u].push_back({p, w});
            g[p].push_back({u, w});
        }
        dc.build();
        vector<char> on(n + 1, 0);
        // 随机 toggle + query
        for (int op = 0; op < 300; ++op) {
            int x = rng() % n + 1;
            dc.toggle(x);
            on[x] ^= 1;
            if (op % 3 == 0) {
                i64 got = dc.query();
                i64 want = brute(g, on, n);
                if (got != want) {
                    printf("FAIL t=%d op=%d got=%lld want=%lld\n", t, op, (long long)got, (long long)want);
                    for (int u = 1; u <= n; ++u) {
                        printf("  C[%d]:", u);
                        for (auto v : dc.C[u]) printf(" %lld", (long long)v);
                        printf("\n");
                    }
                    printf("  paths:\n");
                    for (int u = 1; u <= n; ++u) {
                        printf("    %d:", u);
                        for (auto [c, d] : dc.paths[u]) printf(" (%d,%lld)", c, (long long)d);
                        printf(" cfa=%d\n", dc.cfa[u]);
                    }
                    printf("  A/B min:\n");
                    for (int u = 1; u <= n; ++u) {
                        printf("    %d: A=%lld B=%lld\n", u,
                            dc.A[u].empty() ? -1LL : (long long)*dc.A[u].begin(),
                            dc.B[u].empty() ? -1LL : (long long)*dc.B[u].begin());
                    }
                    printf("  n=%d on:", n);
                    for (int i = 1; i <= n; ++i) printf("%d", on[i] ? 1 : 0);
                    printf("\n  edges:");
                    for (int u = 1; u <= n; ++u) for (auto [v, w] : g[u]) if (u < v) printf(" %d-%d(%lld)", u, v, (long long)w);
                    printf("\n");
                    return 1;
                }
            }
        }
        printf("t=%d ok\n", t);
    }
    printf("DynCentroid: 100 组随机树 x 300 操作 vs 暴力 全部通过\n");
    return 0;
}
