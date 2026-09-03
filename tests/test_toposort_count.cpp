// 验证 06.22 树拓扑序计数：随机树 vs 暴力枚举排列
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;
using i64 = long long;
const i64 MOD = 998244353;

i64 tree_toposort_count(int n, const vector<vector<int>>& g, int root, i64 MOD) {
    vector<int> sz(n + 1, 0);
    function<void(int, int)> dfs = [&](int u, int p) {
        sz[u] = 1;
        for (int v : g[u]) if (v != p) { dfs(v, u); sz[u] += sz[v]; }
    };
    dfs(root, 0);
    vector<i64> inv(n + 1, 1);
    for (int i = 2; i <= n; ++i) inv[i] = (MOD - MOD / i) * inv[MOD % i] % MOD;
    i64 ans = 1;
    for (int i = 2; i <= n; ++i) ans = ans * i % MOD;
    for (int u = 1; u <= n; ++u) ans = ans * inv[sz[u]] % MOD;
    return ans;
}

int main() {
    mt19937 rng(99);
    // 小树暴力：枚举 n! 排列，检查每个点是否在父之后
    for (int t = 0; t < 300; ++t) {
        int n = rng() % 7 + 1;
        vector<vector<int>> g(n + 1);
        vector<int> par(n + 1, 0);
        for (int u = 2; u <= n; ++u) {
            int p = rng() % (u - 1) + 1;
            g[u].push_back(p); g[p].push_back(u);
            par[u] = p;
        }
        i64 got = tree_toposort_count(n, g, 1, MOD);
        // 暴力
        vector<int> perm(n); iota(perm.begin(), perm.end(), 1);
        i64 want = 0;
        do {
            vector<int> pos(n + 1);
            for (int i = 0; i < n; ++i) pos[perm[i]] = i;
            bool ok = true;
            for (int u = 2; u <= n; ++u) if (pos[par[u]] > pos[u]) { ok = false; break; }
            if (ok) ++want;
        } while (next_permutation(perm.begin(), perm.end()));
        if (got != want % MOD) {
            printf("FAIL t=%d n=%d got=%lld want=%lld\n", t, n, (long long)got, (long long)want);
            return 1;
        }
    }
    printf("06.22 树拓扑序计数: 300 组随机树 vs 暴力枚举排列 全部通过\n");
    // 链特例：n! / n = (n-1)!（链只有一个拓扑序）
    {
        int n = 8;
        vector<vector<int>> g(n + 1);
        for (int u = 2; u <= n; ++u) { g[u].push_back(u - 1); g[u - 1].push_back(u); }
        i64 got = tree_toposort_count(n, g, 1, MOD);
        if (got != 1) { printf("链 FAIL got=%lld\n", (long long)got); return 1; }
        printf("链特例（唯一拓扑序=1）通过\n");
    }
    return 0;
}
