// 验证 02.7 -> 02.8 -> 02.9 继承承接链（真实功能）
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;
using i64 = long long;

// ===== 02.7 SegTree 基础版（protected 成员）=====
struct SegTree {
protected:
    int n;
    vector<i64> tr;
public:
    SegTree(int n_ = 0) { init(n_); }
    void init(int n_) { n = n_; tr.assign(4 * n + 4, 0); }
    i64 op(i64 a, i64 b) const { return a + b; }
    i64 e() const { return 0; }
    void build(const vector<i64>& a, int p, int l, int r) {
        if (l == r) { tr[p] = a[l]; return; }
        int m = (l + r) / 2;
        build(a, p * 2, l, m); build(a, p * 2 + 1, m + 1, r);
        tr[p] = op(tr[p * 2], tr[p * 2 + 1]);
    }
    void build(const vector<i64>& a) { build(a, 1, 1, n); }
    void set(int p, int l, int r, int pos, i64 x) {
        if (l == r) { tr[p] = x; return; }
        int m = (l + r) / 2;
        if (pos <= m) set(p * 2, l, m, pos, x);
        else set(p * 2 + 1, m + 1, r, pos, x);
        tr[p] = op(tr[p * 2], tr[p * 2 + 1]);
    }
    void set(int pos, i64 x) { set(1, 1, n, pos, x); }
    i64 query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];
        int m = (l + r) / 2;
        i64 res = e();
        if (ql <= m) res = op(res, query(p * 2, l, m, ql, qr));
        if (qr > m) res = op(res, query(p * 2 + 1, m + 1, r, ql, qr));
        return res;
    }
    i64 query(int l, int r) { return query(1, 1, n, l, r); }
};

// ===== 02.8 SegLazy（继承增量）=====
struct SegLazy : SegTree {
protected:
    i64 mod = 0;
    vector<i64> mul, add;
    i64 norm(i64 x) const { return mod ? (x % mod + mod) % mod : x; }
    void apply(int p, int l, int r, i64 m, i64 a) {
        tr[p] = norm(tr[p] * m + (r - l + 1) * a);
        mul[p] = norm(mul[p] * m);
        add[p] = norm(add[p] * m + a);
    }
    void push(int p, int l, int r) {
        if (mul[p] != 1 || add[p] != 0) {
            int m = (l + r) / 2;
            apply(p * 2, l, m, mul[p], add[p]);
            apply(p * 2 + 1, m + 1, r, mul[p], add[p]);
            mul[p] = 1; add[p] = 0;
        }
    }
    void range(int p, int l, int r, int ql, int qr, i64 m, i64 a) {
        if (ql <= l && r <= qr) { apply(p, l, r, m, a); return; }
        push(p, l, r);
        int mid = (l + r) / 2;
        if (ql <= mid) range(p * 2, l, mid, ql, qr, m, a);
        if (qr > mid) range(p * 2 + 1, mid + 1, r, ql, qr, m, a);
        tr[p] = norm(op(tr[p * 2], tr[p * 2 + 1]));
    }
    i64 query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];
        push(p, l, r);
        int mid = (l + r) / 2;
        i64 res = e();
        if (ql <= mid) res = norm(op(res, query(p * 2, l, mid, ql, qr)));
        if (qr > mid) res = norm(op(res, query(p * 2 + 1, mid + 1, r, ql, qr)));
        return res;
    }
public:
    SegLazy(int n_, i64 mod_ = 0) : mod(mod_) {
        n = n_;
        tr.assign(4 * n + 4, 0);
        mul.assign(4 * n + 4, 1);
        add.assign(4 * n + 4, 0);
    }
    void range_add(int l, int r, i64 v) { range(1, 1, n, l, r, 1, norm(v)); }
    void range_mul(int l, int r, i64 v) { range(1, 1, n, l, r, norm(v), 0); }
    i64 range_sum(int l, int r) { return query(1, 1, n, l, r); }
};

// ===== 02.9 SegAssign（两行封装）=====
struct SegAssign : SegLazy {
    SegAssign(int n_, i64 mod_ = 0) : SegLazy(n_, mod_) {}
    void range_assign(int l, int r, i64 x) {
        range_mul(l, r, 0);
        range_add(l, r, x);
    }
};

int main() {
    mt19937 rng(7);
    // 1) SegTree 基础：单点 + 区间查询 vs 暴力
    for (int t = 0; t < 200; ++t) {
        int n = rng() % 30 + 1;
        vector<i64> a(n + 1);
        SegTree st(n);
        for (int i = 1; i <= n; ++i) a[i] = rng() % 100;
        st.build(a);
        for (int op = 0; op < 200; ++op) {
            int ty = rng() % 3;
            if (ty == 0) { int p = rng() % n + 1; i64 x = rng() % 100; st.set(p, x); a[p] = x; }
            else if (ty == 1) { int p = rng() % n + 1; i64 x = rng() % 100; st.set(p, a[p] + x); a[p] += x; }
            else {
                int l = rng() % n + 1, r = rng() % n + 1; if (l > r) swap(l, r);
                i64 want = 0; for (int i = l; i <= r; ++i) want += a[i];
                if (st.query(l, r) != want) { printf("SegTree FAIL t=%d\n", t); return 1; }
            }
        }
    }
    printf("02.7 SegTree: 200 组 vs 暴力 通过\n");
    // 2) SegLazy 继承：build 沿用基类 + 区间加乘 + 查询 vs 暴力
    for (int t = 0; t < 200; ++t) {
        int n = rng() % 30 + 1;
        vector<i64> a(n + 1);
        SegLazy sg(n);
        for (int i = 1; i <= n; ++i) a[i] = rng() % 10;
        sg.build(a);                     // 基类 build 直接可用
        for (int op = 0; op < 300; ++op) {
            int ty = rng() % 4;
            int l = rng() % n + 1, r = rng() % n + 1; if (l > r) swap(l, r);
            if (ty == 0) { i64 v = rng() % 10; sg.range_add(l, r, v); for (int i = l; i <= r; ++i) a[i] += v; }
            else if (ty == 1) { i64 v = rng() % 3; sg.range_mul(l, r, v); for (int i = l; i <= r; ++i) a[i] *= v; }
            else if (ty == 2) {
                i64 want = 0; for (int i = l; i <= r; ++i) want += a[i];
                if (sg.range_sum(l, r) != want) { printf("SegLazy FAIL t=%d\n", t); return 1; }
            } else {
                i64 v = rng() % 10; sg.range_add(l, r, v); for (int i = l; i <= r; ++i) a[i] += v;
                i64 want = 0; for (int i = l; i <= r; ++i) want += a[i];
                if (sg.range_sum(l, r) != want) { printf("SegLazy FAIL2 t=%d\n", t); return 1; }
            }
        }
    }
    printf("02.8 SegLazy 继承: 200 组区间加/乘/查 vs 暴力 通过\n");
    // 3) SegAssign 两行封装
    for (int t = 0; t < 200; ++t) {
        int n = rng() % 30 + 1;
        vector<i64> a(n + 1);
        SegAssign sa(n);
        for (int i = 1; i <= n; ++i) a[i] = rng() % 10;
        sa.build(a);
        for (int op = 0; op < 300; ++op) {
            int ty = rng() % 2;
            int l = rng() % n + 1, r = rng() % n + 1; if (l > r) swap(l, r);
            if (ty == 0) { i64 v = rng() % 10; sa.range_assign(l, r, v); for (int i = l; i <= r; ++i) a[i] = v; }
            else {
                i64 want = 0; for (int i = l; i <= r; ++i) want += a[i];
                if (sa.range_sum(l, r) != want) { printf("SegAssign FAIL t=%d\n", t); return 1; }
            }
        }
    }
    printf("02.9 SegAssign 两行封装: 200 组区间赋值/查 vs 暴力 通过\n");
    return 0;
}
