// 验证 01.3 防爆 int64 快速幂（mul_mod_safe / mod_pow_safe）
// 1) 小模数：vs 普通快速幂（i64 直乘不溢出）对拍
// 2) 大模数：输出 (a, e, mod, result) 交给 Python pow() 校验
#include <bits/stdc++.h>
#include <cstdio>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;

i64 mul_mod_safe(i64 a, i64 b, i64 mod) {
    i64 r = 0;
    a %= mod; b %= mod;
    if (a < 0) a += mod;
    if (b < 0) b += mod;
    while (b) {
        if (b & 1) r = (r >= mod - a) ? r - (mod - a) : r + a;
        a = (a >= mod - a) ? a - (mod - a) : a + a;
        b >>= 1;
    }
    return r;
}
i64 mod_pow_safe(i64 a, i64 e, i64 mod) {
    i64 r = 1 % mod;
    a %= mod;
    if (a < 0) a += mod;
    while (e) {
        if (e & 1) r = mul_mod_safe(r, a, mod);
        a = mul_mod_safe(a, a, mod);
        e >>= 1;
    }
    return r;
}
// 普通快速幂（仅 mod < 2^30 时无溢出，作对拍基准）
i64 naive_pow(i64 a, i64 e, i64 mod) {
    i64 r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) r = r * a % mod;
        a = a * a % mod;
        e >>= 1;
    }
    return r;
}
int main(int argc, char** argv) {
    mt19937_64 rng(20260214);
    if (argc > 1 && string(argv[1]) == "dump") {
        // 大模数数据：输出给 Python 校验
        int T = 300;
        for (int i = 0; i < T; ++i) {
            u64 mod = rng() % (u64)9000000000000000000ULL + 2;   // 2 .. ~9e18
            i64 a = (i64)(rng() % (mod - 1)) + 1;                // 1 .. mod-1
            i64 e = (i64)(rng() >> 1);                          // 非负指数
            i64 got = mod_pow_safe(a, e, (i64)mod);
            printf("%lld %lld %llu %lld\n", (long long)a, (long long)e, (unsigned long long)mod, (long long)got);
        }
        return 0;
    }
    // 1) 小模数对拍（mod < 2^30，普通乘不溢出）
    for (int t = 0; t < 2000; ++t) {
        i64 mod = rng() % 1000000000 + 2;
        i64 a = (i64)(rng() % (u64)mod);
        i64 e = (i64)(rng() >> 1);            // 非负（模板约定 e >= 0）
        i64 got = mod_pow_safe(a, e, mod);
        i64 want = naive_pow(a, e, mod);
        if (got != want) {
            printf("pow_safe FAIL t=%d a=%lld e=%lld mod=%lld got=%lld want=%lld\n",
                   t, (long long)a, (long long)e, (long long)mod, (long long)got, (long long)want);
            return 1;
        }
        // mul 对拍（mod < 2^30，直乘不溢出）
        i64 b = (i64)(rng() % (u64)mod);
        i64 gm = mul_mod_safe(a, b, mod);
        i64 wm = a * b % mod;
        if (gm != wm) {
            printf("mul_safe FAIL t=%d a=%lld b=%lld mod=%lld got=%lld want=%lld\n",
                   t, (long long)a, (long long)b, (long long)mod, (long long)gm, (long long)wm);
            return 1;
        }
    }
    printf("小模数对拍: 2000 组 mod_pow_safe/mul_mod_safe vs 基准 全部通过\n");
    // 2) 边界：e=0、mod=1、mod=2、a=0、a=mod-1（最大）
    {
        if (mod_pow_safe(0, 0, 998244353) != 1 % 998244353) { printf("0^0 FAIL\n"); return 1; }
        if (mod_pow_safe(12345, 0, 7) != 1) { printf("a^0 FAIL\n"); return 1; }
        if (mod_pow_safe(5, 100, 1) != 0) { printf("mod=1 FAIL\n"); return 1; }
        if (mod_pow_safe(999999999999999999LL, 2, 1000000000000000000LL) != 1) { printf("(m-1)^2 FAIL\n"); return 1; }
        if (mul_mod_safe(999999999999999999LL, 999999999999999999LL, 1000000000000000000LL) != 1) {
            printf("mul (m-1)^2 FAIL\n"); return 1;
        }
        printf("边界用例（0^0 / a^0 / mod=1 / (m-1)^2 mod m）全部通过\n");
    }
    // 3) 极限 mod 接近 9e18 的条件减法路径
    {
        i64 mod = 8999999999999999999LL;
        i64 a = 8999999999999999998LL;
        // mul_safe(a, a, mod) = (mod-1)^2 mod mod = 1
        if (mul_mod_safe(a, a, mod) != 1) { printf("极限 mod mul FAIL\n"); return 1; }
        // pow_safe(a, 2, mod) = 1
        if (mod_pow_safe(a, 2, mod) != 1) { printf("极限 mod pow FAIL\n"); return 1; }
        printf("极限模数（9e18 附近）条件减法路径通过\n");
    }
    return 0;
}
