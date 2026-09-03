// 验证 10.4.1 超现实数（已知值对拍 + simplest_between 验证）
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;
using i64 = long long;
using i128 = __int128_t;

struct Surreal {
    i64 m; int e;
    Surreal(i64 m_ = 0, int e_ = 0) : m(m_), e(e_) { norm(); }
    void norm() {
        if (m == 0) { e = 0; return; }
        while (e > 0 && m % 2 == 0) { m /= 2; --e; }
    }
    int cmp(const Surreal& o) const {
        int ne = max(e, o.e);
        i128 a = ((i128)m) << (ne - e);
        i128 b = ((i128)o.m) << (ne - o.e);
        return a < b ? -1 : a > b ? 1 : 0;
    }
    bool operator<(const Surreal& o) const { return cmp(o) < 0; }
    bool operator==(const Surreal& o) const { return cmp(o) == 0; }
    bool operator>(const Surreal& o) const { return cmp(o) > 0; }
    Surreal operator+(const Surreal& o) const {
        int ne = max(e, o.e);
        i128 a = ((i128)m) << (ne - e), b = ((i128)o.m) << (ne - o.e);
        return Surreal((i64)(a + b), ne);
    }
    static i64 div_floor(i64 a, i64 b) {
        if (a >= 0) return a / b;
        return -((-a + b - 1) / b);
    }
    static Surreal simplest_between(Surreal L, Surreal R) {
        if (L.cmp(Surreal(0)) < 0 && Surreal(0).cmp(R) < 0) return Surreal(0);
        for (int e = 0; e <= 60; ++e) {
            i128 lo = val_scaled(L, e), hi = val_scaled(R, e);
            i64 m0 = (i64)(lo + 1);
            if (m0 < hi) return Surreal(m0, e);
        }
        return Surreal(0);
    }
    static i128 val_scaled(const Surreal& x, int e) {
        if (e >= x.e) return ((i128)x.m) << (e - x.e);
        i64 b = (i64)1 << (x.e - e);
        return (i128)div_floor(x.m, b);
    }
};

// 已知超现实数（经典 Conway 序列）
// 0 = {|}, 1 = {0|}, -1 = {|0}, 1/2 = {0|1}, 3/4 = {1/2|1}, 1/4 = {0|1/2}
int main() {
    Surreal Z(0), ONE(1), NEG1(-1);
    Surreal HALF = Surreal::simplest_between(Z, ONE);        // 期望 1/2
    Surreal Q = Surreal::simplest_between(HALF, ONE);        // 期望 3/4
    Surreal QUARTER = Surreal::simplest_between(Z, HALF);    // 期望 1/4
    // 1) simplest_between 正确性
    if (!(HALF == Surreal(1, 1))) { printf("half FAIL: %lld/2^%d\n", (long long)HALF.m, HALF.e); return 1; }
    if (!(Q == Surreal(3, 2))) { printf("3/4 FAIL: %lld/2^%d\n", (long long)Q.m, Q.e); return 1; }
    if (!(QUARTER == Surreal(1, 2))) { printf("1/4 FAIL: %lld/2^%d\n", (long long)QUARTER.m, QUARTER.e); return 1; }
    // 2) 加法
    if (!((ONE + NEG1) == Z)) { printf("1 + (-1) FAIL\n"); return 1; }
    if (!((HALF + HALF) == ONE)) { printf("1/2 + 1/2 FAIL\n"); return 1; }
    if (!((QUARTER + QUARTER) == HALF)) { printf("1/4 + 1/4 FAIL\n"); return 1; }
    if (!((HALF + QUARTER) == Q)) { printf("1/2 + 1/4 FAIL\n"); return 1; }
    // 3) 比较
    if (!(Z < HALF) || !(HALF < ONE) || !(NEG1 < Z)) { printf("cmp FAIL\n"); return 1; }
    if (!(QUARTER < HALF) || !(HALF < Q)) { printf("cmp2 FAIL\n"); return 1; }
    // 4) 随机对拍：simplest_between(L,R) 必须满足 L < v < R 且 e 最小
    mt19937 rng(2026);
    for (int t = 0; t < 2000; ++t) {
        i64 a = (int)rng() % 100, b = (int)rng() % 100;
        Surreal L(a), R(b);
        if (!(L < R)) swap(L, R);
        if (!(L < R)) continue;
        Surreal v = Surreal::simplest_between(L, R);
        if (!(L < v) || !(v < R)) { printf("between FAIL t=%d L=%lld R=%lld v=%lld/2^%d\n", t, (long long)L.m, (long long)R.m, (long long)v.m, v.e); return 1; }
        // 验证 e 最小性：e 更小的任何整数/半整数都不在区间内（检查 e-1 层无解）
        if (v.e > 0) {
            i64 m0 = (i64)Surreal::val_scaled(L, v.e - 1) + 1;
            i128 hi = Surreal::val_scaled(R, v.e - 1);
            if (m0 < hi) { printf("minimality FAIL t=%d v=%lld/2^%d\n", t, (long long)v.m, v.e); return 1; }
        }
    }
    printf("10.4.1 超现实数: 已知值 + 2000 组 simplest_between 随机验证 全部通过\n");
    return 0;
}
