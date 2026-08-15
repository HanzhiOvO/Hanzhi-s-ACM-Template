// 验证 07.8 多点求值 + 快速插值（依赖 NTT + poly_inv + poly_deriv）
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;
using i64 = long long;
const int NTT_MOD = 998244353;
i64 mod_pow(i64 a, i64 e, i64 mod) {
    i64 r = 1 % mod; a %= mod;
    while (e) { if (e & 1) r = r * a % mod; a = a * a % mod; e >>= 1; }
    return r;
}
void ntt(vector<int>& a, bool invert) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        int wlen = (int)mod_pow(3, (NTT_MOD - 1) / len, NTT_MOD);
        if (invert) wlen = (int)mod_pow(wlen, NTT_MOD - 2, NTT_MOD);
        for (int i = 0; i < n; i += len) {
            int w = 1;
            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j], v = (int)(1LL * a[i + j + len / 2] * w % NTT_MOD);
                a[i + j] = (u + v) % NTT_MOD;
                a[i + j + len / 2] = (u - v + NTT_MOD) % NTT_MOD;
                w = (int)(1LL * w * wlen % NTT_MOD);
            }
        }
    }
    if (invert) {
        int inv_n = (int)mod_pow(n, NTT_MOD - 2, NTT_MOD);
        for (auto& x : a) x = (int)(1LL * x * inv_n % NTT_MOD);
    }
}
vector<int> convolution_ntt(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) return {};
    int need = (int)a.size() + (int)b.size() - 1, n = 1;
    while (n < need) n <<= 1;
    a.resize(n); b.resize(n);
    ntt(a, false); ntt(b, false);
    for (int i = 0; i < n; ++i) a[i] = (int)(1LL * a[i] * b[i] % NTT_MOD);
    ntt(a, true);
    a.resize(need);
    return a;
}
vector<int> poly_inv(const vector<int>& a, int n) {
    vector<int> res(1, (int)mod_pow(a[0], NTT_MOD - 2, NTT_MOD));
    int len = 1;
    while (len < n) {
        len <<= 1;
        vector<int> A(2 * len, 0), B(2 * len, 0);
        for (int i = 0; i < (int)a.size() && i < len; ++i) A[i] = a[i];
        for (int i = 0; i < (int)res.size(); ++i) B[i] = res[i];
        ntt(A, false); ntt(B, false);
        for (int i = 0; i < 2 * len; ++i) A[i] = (int)(1LL * A[i] * B[i] % NTT_MOD);
        ntt(A, true);
        for (int i = 0; i < len; ++i)
            A[i] = i == 0 ? (2 - A[i] + NTT_MOD) % NTT_MOD : (NTT_MOD - A[i]) % NTT_MOD;
        for (int i = len; i < 2 * len; ++i) A[i] = 0;
        ntt(A, false);
        for (int i = 0; i < 2 * len; ++i) A[i] = (int)(1LL * A[i] * B[i] % NTT_MOD);
        ntt(A, true);
        res.assign(A.begin(), A.begin() + len);
    }
    res.resize(n);
    return res;
}
vector<int> poly_deriv(const vector<int>& a) {
    vector<int> r(max(0, (int)a.size() - 1));
    for (int i = 1; i < (int)a.size(); ++i) r[i - 1] = (int)(1LL * a[i] * i % NTT_MOD);
    return r;
}
// ===== 07.8 新增 =====
vector<int> poly_mod(const vector<int>& a, const vector<int>& b) {
    int n = (int)a.size(), m = (int)b.size();
    if (n < m) return a;
    vector<int> ra = a, rb = b;
    reverse(ra.begin(), ra.end()); reverse(rb.begin(), rb.end());
    vector<int> inv_rb = poly_inv(rb, n - m + 1);
    vector<int> qr = convolution_ntt(ra, inv_rb);
    qr.resize(n - m + 1);
    reverse(qr.begin(), qr.end());
    vector<int> bq = convolution_ntt(b, qr);
    vector<int> r(m - 1);
    for (int i = 0; i < m - 1; ++i) {
        i64 v = (i < (int)a.size() ? a[i] : 0) - (i < (int)bq.size() ? bq[i] : 0);
        r[i] = (int)((v % NTT_MOD + NTT_MOD) % NTT_MOD);
    }
    return r;
}
struct ProductTree {
    int n;
    vector<vector<int>> node;
    vector<int> xs;
    ProductTree(const vector<int>& x) : n((int)x.size()), xs(x) {
        node.resize(4 * n + 5);
        if (n) build(1, 0, n - 1);
    }
    void build(int o, int l, int r) {
        if (l == r) {
            node[o] = {(NTT_MOD - xs[l]) % NTT_MOD, 1};
            return;
        }
        int mid = (l + r) >> 1;
        build(o << 1, l, mid);
        build(o << 1 | 1, mid + 1, r);
        node[o] = convolution_ntt(node[o << 1], node[o << 1 | 1]);
    }
};
vector<int> multipoint_eval(const vector<int>& f, const vector<int>& xs) {
    int n = (int)xs.size();
    if (n == 0) return {};
    ProductTree pt(xs);
    vector<int> res(n);
    function<void(int, int, int, vector<int>)> dfs = [&](int o, int l, int r, vector<int> g) {
        g = poly_mod(g, pt.node[o]);
        if (l == r) { res[l] = g.empty() ? 0 : g[0]; return; }
        int mid = (l + r) >> 1;
        dfs(o << 1, l, mid, g);
        dfs(o << 1 | 1, mid + 1, r, g);
    };
    dfs(1, 0, n - 1, f);
    return res;
}
vector<int> interpolate(const vector<int>& xs, const vector<int>& ys) {
    int n = (int)xs.size();
    if (n == 0) return {};
    ProductTree pt(xs);
    vector<int> g = pt.node[1];
    vector<int> dg = poly_deriv(g);
    vector<int> gval = multipoint_eval(dg, xs);
    vector<int> w(n);
    for (int i = 0; i < n; ++i)
        w[i] = (int)(1LL * ys[i] % NTT_MOD * mod_pow(gval[i], NTT_MOD - 2, NTT_MOD) % NTT_MOD);
    function<vector<int>(int, int, int)> solve = [&](int o, int l, int r) -> vector<int> {
        if (l == r) return {w[l]};
        int mid = (l + r) >> 1;
        vector<int> fl = solve(o << 1, l, mid);
        vector<int> fr = solve(o << 1 | 1, mid + 1, r);
        vector<int> t1 = convolution_ntt(fl, pt.node[o << 1 | 1]);
        vector<int> t2 = convolution_ntt(fr, pt.node[o << 1]);
        vector<int> t(max(t1.size(), t2.size()));
        for (int i = 0; i < (int)t1.size(); ++i) t[i] = (t[i] + t1[i]) % NTT_MOD;
        for (int i = 0; i < (int)t2.size(); ++i) t[i] = (t[i] + t2[i]) % NTT_MOD;
        return t;
    };
    vector<int> f = solve(1, 0, n - 1);
    f.resize(n);
    return f;
}

int main() {
    mt19937 rng(31415);
    // 1) poly_mod 验证：朴素长除对拍
    for (int t = 0; t < 50; ++t) {
        int m = rng() % 5 + 1;
        int n = rng() % 5 + m;
        vector<int> a(n), b(m);
        b[m - 1] = 1;
        for (auto& x : a) x = rng() % NTT_MOD;
        for (int i = 0; i < m - 1; ++i) b[i] = rng() % NTT_MOD;
        vector<int> r = poly_mod(a, b);
        if ((int)r.size() != m - 1) { printf("mod size FAIL t=%d\n", t); return 1; }
        vector<int> aa = a;
        i64 inv_top = mod_pow(b[m - 1], NTT_MOD - 2, NTT_MOD);
        for (int i = n - 1; i >= m - 1; --i) {
            i64 tq = aa[i] * inv_top % NTT_MOD;
            for (int j = 0; j < m; ++j)
                aa[i - m + 1 + j] = (aa[i - m + 1 + j] - tq * b[j] % NTT_MOD + NTT_MOD) % NTT_MOD;
        }
        for (int i = 0; i < m - 1; ++i)
            if (r[i] != aa[i]) { printf("mod FAIL t=%d i=%d got=%d want=%d\n", t, i, r[i], aa[i]); return 1; }
    }
    printf("poly_mod: 50 组 vs 朴素长除 全部通过\n");
    // 2) 多点求值 vs 直接代入
    for (int t = 0; t < 100; ++t) {
        int n = rng() % 30 + 1;                // 点数量
        int deg = rng() % 20 + 1;
        vector<int> f(deg);
        for (auto& x : f) x = rng() % NTT_MOD;
        vector<int> xs(n);
        for (int i = 0; i < n; ++i) xs[i] = rng() % NTT_MOD;
        vector<int> got = multipoint_eval(f, xs);
        // 直接代入
        for (int i = 0; i < n; ++i) {
            i64 x = xs[i], want = 0, pw = 1;
            for (int j = 0; j < deg; ++j) {
                want = (want + 1LL * f[j] * pw) % NTT_MOD;
                pw = pw * x % NTT_MOD;
            }
            if (got[i] != want) {
                printf("multipoint FAIL t=%d i=%d got=%d want=%lld\n", t, i, got[i], (long long)want);
                return 1;
            }
        }
    }
    printf("multipoint_eval: 100 组随机 vs 直接代入 全部通过\n");
    // 3) 快速插值：插出的多项式还原点值
    for (int t = 0; t < 100; ++t) {
        int n = rng() % 30 + 1;
        vector<int> xs(n), ys(n);
        for (int i = 0; i < n; ++i) xs[i] = rng() % NTT_MOD;
        // 随机多项式
        vector<int> f(rng() % 15 + 1);
        for (auto& x : f) x = rng() % NTT_MOD;
        // y = f(x)
        for (int i = 0; i < n; ++i) {
            i64 x = xs[i], v = 0, pw = 1;
            for (int j = 0; j < (int)f.size(); ++j) {
                v = (v + 1LL * f[j] * pw) % NTT_MOD;
                pw = pw * x % NTT_MOD;
            }
            ys[i] = (int)v;
        }
        vector<int> g = interpolate(xs, ys);
        // 验证 g(x_i) == y_i（若 g 度 >= n 则用点值验证）
        vector<int> gv = multipoint_eval(g, xs);
        for (int i = 0; i < n; ++i)
            if (gv[i] != ys[i]) {
                printf("interpolate FAIL t=%d i=%d got=%d want=%d\n", t, i, gv[i], ys[i]);
                return 1;
            }
        // 若 n > deg(f)，插值应恢复 f（前 deg(f) 项）
        if (n > (int)f.size()) {
            for (int i = 0; i < (int)f.size(); ++i)
                if (g[i] != f[i]) { printf("interpolate recover FAIL t=%d i=%d got=%d want=%d\n", t, i, g[i], f[i]); return 1; }
        }
    }
    printf("interpolate: 100 组随机（点值还原 + 恢复系数）全部通过\n");
    // 4) 大长度烟雾
    {
        int n = 256;
        vector<int> xs(n), f(100);
        for (int i = 0; i < n; ++i) xs[i] = i + 1;
        for (auto& x : f) x = rng() % NTT_MOD;
        vector<int> got = multipoint_eval(f, xs);
        for (int i = 0; i < n; ++i) {
            i64 x = xs[i], want = 0, pw = 1;
            for (int j = 0; j < (int)f.size(); ++j) {
                want = (want + 1LL * f[j] * pw) % NTT_MOD;
                pw = pw * x % NTT_MOD;
            }
            if (got[i] != want) { printf("smoke eval FAIL i=%d\n", i); return 1; }
        }
        printf("大长度烟雾（n=256 多点求值）通过\n");
    }
    return 0;
}
