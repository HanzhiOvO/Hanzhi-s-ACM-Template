// 验证 poly_sqrt + poly_pow（依赖 07.5 NTT 全套）
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
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
vector<int> poly_integ(const vector<int>& a) {
    static vector<int> inv{0, 1};
    while ((int)inv.size() <= (int)a.size())
        inv.push_back((int)(NTT_MOD - 1LL * (NTT_MOD / (int)inv.size()) * inv[NTT_MOD % (int)inv.size()] % NTT_MOD));
    vector<int> r(a.size() + 1);
    for (int i = 0; i < (int)a.size(); ++i) r[i + 1] = (int)(1LL * a[i] * inv[i + 1] % NTT_MOD);
    return r;
}
vector<int> poly_ln(const vector<int>& a, int n) {
    vector<int> der = poly_deriv(a), inv = poly_inv(a, n);
    der.resize(n);
    vector<int> mul = convolution_ntt(der, inv);
    mul.resize(n);
    return poly_integ(mul);
}
vector<int> poly_exp(const vector<int>& a, int n) {
    vector<int> res(1, 1);
    int len = 1;
    while (len < n) {
        len <<= 1;
        vector<int> t = poly_ln(res, len);
        for (int i = 0; i < len; ++i) {
            i64 val = (i < (int)a.size() ? a[i] : 0) - t[i];
            t[i] = ((val % NTT_MOD) + NTT_MOD) % NTT_MOD;
        }
        t[0] = (t[0] + 1) % NTT_MOD;
        res = convolution_ntt(res, t);
        res.resize(len);
    }
    res.resize(n);
    return res;
}
// ===== 模板新增部分 =====
const int INV2 = (NTT_MOD + 1) / 2;
vector<int> poly_sqrt(const vector<int>& a, int n) {
    vector<int> res(1, 1);
    int len = 1;
    while (len < n) {
        len <<= 1;
        vector<int> f(min((int)a.size(), len));
        for (int i = 0; i < (int)f.size(); ++i) f[i] = a[i];
        f.resize(len);
        vector<int> invs = poly_inv(res, len);
        vector<int> t = convolution_ntt(f, invs);
        t.resize(len);
        res.resize(len);
        for (int i = 0; i < len; ++i)
            res[i] = (int)(1LL * (res[i] + t[i]) % NTT_MOD * INV2 % NTT_MOD);
    }
    res.resize(n);
    return res;
}
vector<int> poly_pow(const vector<int>& a, i64 k, int n) {
    if (k == 0) { vector<int> r(n); r[0] = 1; return r; }
    int shift = 0;
    while (shift < (int)a.size() && a[shift] == 0) ++shift;
    if (shift >= (int)a.size() || (i128)shift * k >= n) return vector<int>(n, 0);
    vector<int> b(a.begin() + shift, a.end());
    i64 c = b[0];
    i64 invc = mod_pow(c, NTT_MOD - 2, NTT_MOD);
    for (auto& x : b) x = (int)(1LL * x * invc % NTT_MOD);
    vector<int> l = poly_ln(b, n);
    i64 km = k % NTT_MOD;
    for (auto& x : l) x = (int)(1LL * x * km % NTT_MOD);
    vector<int> r = poly_exp(l, n);
    i64 cc = mod_pow(c, k % (NTT_MOD - 1), NTT_MOD);
    for (auto& x : r) x = (int)(1LL * x * cc % NTT_MOD);
    i64 sk = (i64)shift * k;
    vector<int> res(n, 0);
    for (int i = 0; i + (int)sk < n; ++i) res[i + (int)sk] = r[i];
    return res;
}

// 朴素卷积（对拍用）
vector<int> naive_mul(const vector<int>& a, const vector<int>& b) {
    vector<int> c(a.size() + b.size() - 1);
    for (int i = 0; i < (int)a.size(); ++i)
        for (int j = 0; j < (int)b.size(); ++j)
            c[i + j] = (c[i + j] + 1LL * a[i] * b[j]) % NTT_MOD;
    return c;
}
vector<int> naive_pow(vector<int> a, i64 k, int n) {
    vector<int> r(1, 1);
    while (k) {
        if (k & 1) { r = naive_mul(r, a); if ((int)r.size() > n) r.resize(n); }
        a = naive_mul(a, a);
        if ((int)a.size() > n) a.resize(n);
        k >>= 1;
    }
    r.resize(n);
    return r;
}

int main() {
    mt19937 rng(777);
    // 1) poly_sqrt：随机 a（a[0]=1），验证 s^2 == a（模意义，截断 n）
    for (int t = 0; t < 100; ++t) {
        int n = rng() % 20 + 1;
        vector<int> a(n);
        a[0] = 1;
        for (int i = 1; i < n; ++i) a[i] = rng() % NTT_MOD;
        auto s = poly_sqrt(a, n);
        auto sq = naive_mul(s, s);
        sq.resize(n);
        if (sq != a) {
            printf("sqrt FAIL t=%d n=%d\n", t, n);
            return 1;
        }
    }
    printf("poly_sqrt: 100 组随机（a[0]=1）验证 s^2==a 全部通过\n");
    // 2) poly_pow：随机 a（含 a[0]==0 情况）vs 朴素快速幂
    for (int t = 0; t < 100; ++t) {
        int n = rng() % 15 + 1;
        i64 k = rng() % 6;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = rng() % NTT_MOD;
        if (t % 3 == 0) a[0] = 0;      // 测提因子分支
        auto got = poly_pow(a, k, n);
        auto want = naive_pow(a, k, n);
        if (got != want) {
            printf("pow FAIL t=%d n=%d k=%lld\n", t, n, (long long)k);
            return 1;
        }
    }
    printf("poly_pow: 100 组随机（含 a[0]=0 提因子）vs 朴素快速幂 全部通过\n");
    // 3) 大长度烟雾测试
    {
        int n = 1 << 12;
        vector<int> a(n); a[0] = 1;
        for (int i = 1; i < n; ++i) a[i] = rng() % NTT_MOD;
        auto s = poly_sqrt(a, n);
        auto sq = convolution_ntt(s, s);
        sq.resize(n);
        if (sq != a) { printf("sqrt smoke FAIL\n"); return 1; }
        auto p = poly_pow(a, 3, n);
        auto a3 = convolution_ntt(convolution_ntt(a, a), a);
        a3.resize(n);
        if (p != a3) { printf("pow smoke FAIL\n"); return 1; }
        printf("大长度烟雾（n=4096）sqrt/pow 通过\n");
    }
    return 0;
}
