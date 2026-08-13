## E 数学、数论、几何与多项式

### 10 数论、组合计数与同余

模数工具、质数筛、因数分解、反演、组合计数、CRT、离散对数、原根、二次剩余和随机分解集中放在这里。
#### 本册通用辅助函数

> **赛时先看**
> - **题目信号**：复制本册数学模板时，先带上这一小块。
> - **本质**：供本册原根、exBSGS、Lagrange、FWHT、BM 使用。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：快速幂 `O(log mod)`，扩欧 `O(log mod)`。
> - **维护的量**：无状态纯函数：`mod_pow`（快速幂）、`exgcd`（扩展欧几里得）、`inv_general`（任意模数逆元），只依赖入参。
> - **警告**：`inv_general` 在逆元不存在时返回 `-1`。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `3` 在模 `7` 下的逆元，并算 `2^10 mod 1000`。

```cpp
i64 x, y;
i64 g = exgcd(30, 12, x, y);      // 1. 求 30x+12y=gcd(30,12)=6 的一组解（x,y 引用输出）
i64 inv = inv_general(3, 7);      // 2. 求 3 在模 7 下的逆元
i64 pw = mod_pow(2, 10, 1000);    // 3. 快速幂：2^10 % 1000
cout << g << ' ' << x << ' ' << y << ' ' << inv << ' ' << pw << '\n';
```

样例：`g=6, x=1, y=-2`；`inv=5`（3*5≡1 mod 7）；`pw=24`（2^10=1024 % 1000）。

**传参要求（照这个传不会错）：**

- `mod_pow(a, e, mod)`：`a` 底数、`e` 指数（i64，可到 1e18）、`mod` 模数；返回 `a^e % mod`，范围 `[0, mod)`；`mod=1` 时返回 0。
- `exgcd(a, b, x, y)`：`a/b` 为任意 i64；引用参数 `x/y` 输出一组解，满足 `ax+by=g`；返回 `gcd(a,b)`（非负）。
- `inv_general(a, mod)`：返回 `a` 在模 `mod` 下的逆元（范围 `[0, mod)`）；要求 `gcd(a,mod)=1`，否则返回 `-1`。


```cpp
i64 mod_pow(i64 a, i64 e, i64 mod) {
    i64 r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) r = (i128)r * a % mod;
        a = (i128)a * a % mod;
        e >>= 1;
    }
    return r;
}

i64 exgcd(i64 a, i64 b, i64& x, i64& y) {
    if (b == 0) {
        x = (a >= 0 ? 1 : -1);
        y = 0;
        return llabs(a);
    }
    i64 x1, y1;
    i64 g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return g;
}

i64 inv_general(i64 a, i64 mod) {
    i64 x, y;
    i64 g = exgcd(a, mod, x, y);
    if (g != 1) return -1;
    x %= mod;
    if (x < 0) x += mod;
    return x;
}
```
#### ModInt

> **赛时先看**
> - **题目信号**：整题都在固定模数下计算。
> - **本质**：封装模意义加减乘除。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(1)`；除法需要快速幂。
> - **维护的量**：`v`（模意义下的当前值，int，恒在 `[0, MOD)`）。
> - **警告**：模数非质数时不能用费马逆元。


**最小完整示例（先抄这一段就能跑）：**

题目：模 `1e9+7` 下计算 `a+b、a*b、a/b` 并输出（a=3, b=5）。

```cpp
using mint = ModInt<1000000007>;      // 1. 模板参数直接给模数（编译期常量）
mint a = 3, b = 5;
mint c = a + b, d = a * b, e = a / b; // 2. 加减乘除全自动取模
cout << c.v << ' ' << d.v << ' ' << e.v << '\n'; // 3. 输出用成员 .v
```

样例：`a+b -> 8`；`a*b -> 15`；`a/b -> 200000002`（3 * inv(5) mod 1e9+7）。

**传参要求（照这个传不会错）：**

- `ModInt<MOD>`：模板参数 `MOD` 是编译期常量模数（int 范围，建议质数）；`ModInt(x)` 传 i64，构造时自动取模并把负数修正到 `[0, MOD)`。
- 运算符 `+ - * /`：返回 `ModInt`；除法内部走 `a * b.inv()`，**要求模数是质数**（费马逆元），非质数时 `b` 可能不可逆。
- `pow(i64 e)`：返回 `a^e` 的 `ModInt`；`inv()`：返回模逆元 `ModInt`。
- 输出用成员 `v`（int，已取模后的值）。


**不会用就照抄：**

```cpp
using mint = ModInt<998244353>;
mint a = x, b = y;
mint c = a + b;
mint d = a * b;
mint e = a / b;   // 只有 b 可逆时才合法
```

- 除法依赖逆元；若模板用费马逆元，模数通常需要是质数。
- 输出成员名（如 `.v/.val`）以本节代码为准。


**API / 入口函数（赛时只认这里列的名字）：**

- `inv()` -> 模逆元 返回 `ModInt`。
- `pow(i64 e)` -> 快速幂 返回 `ModInt`。

```cpp
template <int MOD>
struct ModInt {
    int v;
    ModInt(i64 x = 0) {
        x %= MOD;
        if (x < 0) x += MOD;
        v = (int)x;
    }
    ModInt operator+(const ModInt& o) const { int x = v + o.v; if (x >= MOD) x -= MOD; return x; }
    ModInt operator-(const ModInt& o) const { int x = v - o.v; if (x < 0) x += MOD; return x; }
    ModInt operator*(const ModInt& o) const { return 1LL * v * o.v % MOD; }
    ModInt pow(i64 e) const {
        ModInt a = *this, r = 1;
        while (e) {
            if (e & 1) r = r * a;
            a = a * a;
            e >>= 1;
        }
        return r;
    }
    ModInt inv() const { return pow(MOD - 2); }
    ModInt operator/(const ModInt& o) const { return *this * o.inv(); }
};
```
#### 线性求逆元与阶乘逆元

> **赛时先看**
> - **题目信号**：大量组合数、除法取模、同一模数多次询问。
> - **本质**：模数为质数时批量求 `1..n` 的逆元，以及组合数预处理。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`inv[i]`（1..n 的逆元数组）；`fac/ifac`（阶乘与阶乘逆元）、`mod`（模数）。
> - **警告**：模数必须为质数或保证每个数与模数互质。


**最小完整示例（先抄这一段就能跑）：**

题目：模 `1e9+7` 下求 `1..10` 的逆元，并回答 `C(5,2)`。

```cpp
vector<i64> inv = linear_inv(10, 1000000007); // 1. 批量求 1..10 的逆元
CombFast comb(1000000, 1000000007);           // 2. 预处理阶乘/阶乘逆元到 1e6
cout << inv[3] << '\n';                       // 3. 3 的逆元
cout << comb.C(5, 2) << '\n';                 // 4. 组合数 C(5,2)
```

样例：`inv[3] -> 333333336`（3*333333336 ≡ 1）；`C(5,2) -> 10`。

**传参要求（照这个传不会错）：**

- `linear_inv(n, mod)`：`n` 为逆元上限，`mod` 为模数（**必须质数**或保证所有 `1..n` 与 mod 互质）；返回 1-indexed 数组，`inv[i] = i^{-1} mod mod`；`n<=0` 返回空。
- `CombFast(max_n, mod)`：构造即预处理到 `max_n`；`mod` 必须是质数（逆元走 `qpow(fac[n], mod-2)`）。
- `comb.C(n, k)`：返回 `C(n,k) % mod`（i64）；`k<0 || k>n` 返回 0；要求 `n <= max_n`。
- 组合数查询要求 `n < mod`，否则阶乘含 `mod` 因子结果恒 0，换 Lucas。


**API / 入口函数（赛时只认这里列的名字）：**

- `C(int n, int k)` -> 组合数 C(N,K) 返回 `i64`。
- `init(int n, i64 mod_)` -> 初始化/清空结构

```cpp
vector<i64> linear_inv(int n, i64 mod) {
    if (n <= 0) return {};
    vector<i64> inv(n + 1);
    inv[1] = 1;
    for (int i = 2; i <= n; i++) {
        inv[i] = (mod - mod / i) * inv[mod % i] % mod;
    }
    return inv;
}

struct CombFast {
    i64 mod;
    vector<i64> fac, ifac;
    CombFast(int n = 0, i64 mod_ = 1) { if (n) init(n, mod_); }
    i64 qpow(i64 a, i64 e) {
        i64 r = 1;
        while (e) {
            if (e & 1) r = r * a % mod;
            a = a * a % mod;
            e >>= 1;
        }
        return r;
    }
    void init(int n, i64 mod_) {
        mod = mod_;
        fac.assign(n + 1, 1);
        ifac.assign(n + 1, 1);
        for (int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % mod;
        ifac[n] = qpow(fac[n], mod - 2);
        for (int i = n; i >= 1; i--) ifac[i - 1] = ifac[i] * i % mod;
    }
    i64 C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return fac[n] * ifac[k] % mod * ifac[n - k] % mod;
    }
};
```
#### 扩展欧几里得

> **赛时先看**
> - **题目信号**：线性同余、CRT、模数不一定是质数。
> - **本质**：求 `ax + by = gcd(a,b)` 的一组解，求非质数模逆元。
> - **接法**：求 `a` 在任意模数 `mod` 下的逆元，调用 `inv_general_ext(a,mod)`，返回 `-1` 表示不存在。看到 `a*x ≡ b (mod m)`，先用 `g=gcd(a,m)` 判断 `b` 是否能被 `g` 整除，再把方程除以 `g` 后求逆元。
> - **复杂度判定**：`O(log min(a,b))`。
> - **维护的量**：无状态；输出参数 `x/y` 为 `ax+by=gcd` 的一组解。
> - **警告**：返回 gcd 要非负；逆元存在条件是 `gcd(a,mod)=1`。本节的 `exgcd_ext / inv_general_ext` 与本册通用辅助函数里的 `exgcd / inv_general` 功能相同，名字不同避免重名；与其它节混抄时二选一即可。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `gcd(30,12)` 的一组整系数解，并求 `3` 在模 `7` 下的逆元。

```cpp
i64 x, y;
i64 g = exgcd_ext(30, 12, x, y);    // 1. 求 30x+12y=g 的一组解（x,y 引用返回）
i64 inv = inv_general_ext(3, 7);    // 2. 任意模数逆元：3^{-1} mod 7
i64 bad = inv_general_ext(2, 6);    // 3. gcd(2,6)=2 != 1，逆元不存在
cout << g << ' ' << x << ' ' << y << ' ' << inv << ' ' << bad << '\n';
```

样例：`g=6, x=1, y=-2`；`inv=5`（3*5≡1 mod 7）；`bad=-1`。

**传参要求（照这个传不会错）：**

- `exgcd_ext(a, b, x, y)`：`a/b` 为任意 i64；`x/y` 引用参数输出满足 `ax+by=g` 的一组解；返回 `gcd(a,b)`（非负）。
- `inv_general_ext(a, mod)`：要求 `gcd(a,mod)=1`；返回 `a^{-1} mod mod`（范围 `[0, mod)`）；互质不成立时返回 `-1`。
- 本节函数与本册通用辅助函数的 `exgcd / inv_general` 功能相同，混抄时二选一避免重名。


```cpp
i64 exgcd_ext(i64 a, i64 b, i64& x, i64& y) {
    if (b == 0) {
        x = (a >= 0 ? 1 : -1);
        y = 0;
        return llabs(a);
    }
    i64 x1, y1;
    i64 g = exgcd_ext(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return g;
}

i64 inv_general_ext(i64 a, i64 mod) {
    i64 x, y;
    i64 g = exgcd_ext(a, mod, x, y);
    if (g != 1) return -1;
    x %= mod;
    if (x < 0) x += mod;
    return x;
}
```
#### 线性筛

> **赛时先看**
> - **题目信号**：题面出现"大量质数/最小质因子/phi/mu 查询"且 `n ≤ 1e7`，比如反复判断质数、质因子分解、莫比乌斯反演的批量前缀；看到"预处理一张 `1..n` 的质数/积性函数表"就先想线性筛。
> - **本质**：每个合数只被它的最小质因子筛掉一次：外层枚举 `i`，内层只用质数 `p ≤ minp[i]` 生成 `i*p`，保证每个合数唯一标记，所以是 `O(n)` 而不是埃氏筛的 `O(n log log n)`。
> - **复杂度判定**：`O(n)` 时间、`O(n)` 内存；`n ≤ 1e7` 时四个 `int` 数组约 160MB 以内，稳；`n` 到 1e8 以上内存不够，改用分解质因数或高级筛（min_25 / 杜教筛）。
> - **维护的量**：`primes`（质数表）、`minp[x]`（`x` 的最小质因子）、`phi[x]`（欧拉函数）、`mu[x]`（莫比乌斯函数），全部 1-indexed。
> - **接法**：如果后面要反复判断质数、求最小质因子、`phi` 或 `mu`，先 `LinearSieve sieve(maxn)`。`sieve.primes` 是质数表，`sieve.minp[x]` 是最小质因子，`phi[x]` 和 `mu[x]` 可直接查。`maxn` 太大时不要硬筛，改用分解或高级筛。
> - **警告**：`1` 不是质数；数组大小按最大值开；`phi[1]=mu[1]=1` 是特判；需要约数个数/约数和等积性函数时，本节没有，换 `MultiplicativeSieve`。


**最小完整示例（先抄这一段就能跑）：**

题目：筛到 `1e6`，判 `97` 是否质数，查 `phi(97)` 与 `mu(6)`。

```cpp
LinearSieve sieve(1000000);            // 1. 构造即筛到 1e6（只筛一次即可）
bool isp = sieve.minp[97] == 97;       // 2. minp[x]==x 等价于 x 是质数
int phi97 = sieve.phi[97];             // 3. 欧拉函数直接查表
int mu6 = sieve.mu[6];                 // 4. 莫比乌斯函数查表
cout << isp << ' ' << phi97 << ' ' << mu6 << '\n';
```

样例：`isp=1`；`phi(97)=96`；`mu(6)=1`（6=2*3，两个不同质因子）。

**传参要求（照这个传不会错）：**

- `LinearSieve(maxn)`：构造即筛到 `maxn`；`maxn` 取题面最大查询上界，一次筛到最大即可，别重复 init。
- `sieve.primes`：质数表（升序）；`sieve.minp[x]`：x 的最小质因子，`minp[x]==x` 即 x 为质数。
- `sieve.phi[x]`：欧拉函数；`sieve.mu[x]`：莫比乌斯函数（有平方因子时为 0）；全部 1-indexed，`phi[1]=mu[1]=1` 已特判。
- 需要约数个数/约数和时本节没有，换「线性筛：phi / mu / 约数个数」的 `MultiplicativeSieve`。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `LinearSieve` 结构体。
2. 构造：`LinearSieve sieve(maxn);`，`maxn` 取题面最大的查询上界（一次筛到最大即可）。
3. 调用：判质数看 `sieve.minp[x] == x`；最小质因子直接 `sieve.minp[x]`；`sieve.phi[x]`、`sieve.mu[x]` 查表即得。
4. 取结果：查表值直接输出，无需额外计算。

**改造点（按题目改这几处）：**
- n 上限：`maxn` 从题面取，只筛到需要的最大值；多次询问不同上界时取最大的一次筛，别重复 init。
- 需要约数个数/约数和：本节只筛 `phi/mu`，换「线性筛：phi / mu / 约数个数」的 `MultiplicativeSieve`。
- 只判质数用不到 `phi/mu`：仍可直接用；若内存紧张，可以删掉 `phi`、`mu` 两个数组只留 `minp`。
- 质数计数 `pi(n)`：对 `primes` 二分或做前缀和。

**核心逻辑（改代码时别破坏）：**

- 每个合数只被最小质因子标记一次，总复杂度 O(n)。
- `p > minp[i]` 提前 break：防止用非最小质因子的质数重复筛同一个合数。

**改板时先认这几个量：**

- `minp[x]`：`x` 的最小质因子，`minp[x] == x` 等价于 `x` 是质数。
- `phi[x]`：与 `x` 互质且不超过 `x` 的正整数个数；`mu[x]`：莫比乌斯函数（有平方因子时为 0）。

```cpp
// 维护的量：primes 质数表；minp[x] = x 的最小质因子；phi[x] 欧拉函数；mu[x] 莫比乌斯函数（均 1-indexed）。
// 不变量：每个合数只被其最小质因子筛一次，总复杂度 O(n)。
struct LinearSieve {
    vector<int> primes, minp, phi, mu;

    LinearSieve(int n = 0) { if (n) init(n); }

    void init(int n) {
        primes.clear();
        minp.assign(n + 1, 0);
        phi.assign(n + 1, 0);
        mu.assign(n + 1, 0);
        if (n >= 1) phi[1] = 1, mu[1] = 1; // 特判 1：phi(1)=mu(1)=1
        for (int i = 2; i <= n; ++i) {
            if (!minp[i]) {
                // i 是质数：minp 记自身，phi=i-1，mu=-1
                minp[i] = i;
                primes.push_back(i);
                phi[i] = i - 1;
                mu[i] = -1;
            }
            for (int p : primes) {
                // 只用不超过 minp[i] 的质数生成 i*p，保证每个合数只被筛一次
                if (p > minp[i] || 1LL * i * p > n) break;
                minp[i * p] = p;
                if (i % p == 0) {
                    // p | i：i*p 与 i 质因子集合相同，phi 乘 p；mu 记 0 并 break
                    phi[i * p] = phi[i] * p;
                    mu[i * p] = 0;
                    break;
                } else {
                    // p ∤ i：两者互质，积性函数值相乘
                    phi[i * p] = phi[i] * (p - 1);
                    mu[i * p] = -mu[i];
                }
            }
        }
    }
};
```
#### 线性筛：phi / mu / 约数个数

> **赛时先看**
> - **题目信号**：多次询问积性函数；需要莫比乌斯反演；`n` 可到 `1e7` 左右。
> - **本质**：一次筛出素数、欧拉函数、莫比乌斯函数、约数个数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`primes/is_comp`（质数表/合数标记）、`phi/mu/d`（欧拉/莫比乌斯/约数个数）、`cnt`（最小质因子指数）。
> - **警告**：`p | i` 时要 break；约数个数需要额外维护最小质因子的指数。


**最小完整示例（先抄这一段就能跑）：**

题目：筛到 `1000`，查 `d(12)`、`phi(9)`、`mu(8)`。

```cpp
MultiplicativeSieve s(1000);           // 1. 构造即筛出全部积性函数
int d12 = s.d[12];                     // 2. 约数个数 d(12)
int phi9 = s.phi[9];                   // 3. 欧拉函数 phi(9)
int mu8 = s.mu[8];                     // 4. 莫比乌斯函数 mu(8)
cout << d12 << ' ' << phi9 << ' ' << mu8 << '\n';
```

样例：`d(12)=6`（约数 1,2,3,4,6,12）；`phi(9)=6`；`mu(8)=0`（含平方因子）。

**传参要求（照这个传不会错）：**

- `MultiplicativeSieve(maxn)`：构造即预处理到 `maxn`（1e7 左右仍可行）；所有数组 1-indexed，`phi[1]=mu[1]=d[1]=1` 已特判。
- `s.primes`：质数表；`s.is_comp[x]` 为 1 表示合数；`s.phi[x]`、`s.mu[x]`、`s.d[x]` 直接查表。
- `s.cnt[x]`：x 的最小质因子指数（维护约数个数的中间量，一般不直接用）。
- 只需要判质数/phi/mu、不需要约数个数时，用「线性筛」的 `LinearSieve` 更省内存。


**API / 入口函数（赛时只认这里列的名字）：**

- `init(int n_)` -> 初始化/清空结构

```cpp
struct MultiplicativeSieve {
    int n;
    vector<int> primes, is_comp, phi, mu, d, cnt;

    MultiplicativeSieve(int n = 0) { if (n) init(n); }

    void init(int n_) {
        n = n_;
        primes.clear();
        is_comp.assign(n + 1, 0);
        phi.assign(n + 1, 0);
        mu.assign(n + 1, 0);
        d.assign(n + 1, 1);
        cnt.assign(n + 1, 0);
        phi[1] = 1; mu[1] = 1; d[1] = 1;
        for (int i = 2; i <= n; i++) {
            if (!is_comp[i]) {
                primes.push_back(i);
                phi[i] = i - 1;
                mu[i] = -1;
                d[i] = 2;
                cnt[i] = 1;
            }
            for (int p : primes) {
                i64 v = 1LL * i * p;
                if (v > n) break;
                is_comp[v] = 1;
                if (i % p == 0) {
                    phi[v] = phi[i] * p;
                    mu[v] = 0;
                    cnt[v] = cnt[i] + 1;
                    d[v] = d[i] / (cnt[i] + 1) * (cnt[v] + 1);
                    break;
                } else {
                    phi[v] = phi[i] * (p - 1);
                    mu[v] = -mu[i];
                    cnt[v] = 1;
                    d[v] = d[i] * 2;
                }
            }
        }
    }
};
```
#### 试除分解质因数

> **赛时先看**
> - **题目信号**：`n <= 1e12` 或单次分解不多。
> - **本质**：单个数分解、求约数个数、phi 单点。
> - **接法**：单个 `n` 不太大时直接 `factorize_trial(n)`；返回 `（质因子， 次数）`。可以据此算约数个数、欧拉函数、枚举约数。若 `n` 是 64 位大整数且很多次分解，翻 Pollard-Rho。
> - **复杂度判定**：`O(sqrt n)`。
> - **维护的量**：无状态；返回 `f`（`（质因子， 次数）` 列表）。
> - **警告**：循环结束后若 `n > 1`，剩下的 `n` 就是最后一个质因子。


**最小完整示例（先抄这一段就能跑）：**

题目：分解 `360` 并输出所有质因子及次数。

```cpp
auto f = factorize_trial(360);         // 1. 返回 (质因子, 次数) 列表
for (auto [p, c] : f)
    cout << p << '^' << c << ' ';      // 2. 逐个输出
```

样例：`2^3 3^2 5^1`（360 = 2^3 * 3^2 * 5）。

**传参要求（照这个传不会错）：**

- `factorize_trial(n)`：`n` 为 i64；`n <= 1e12` 时 `O(sqrt n)` 单次可行，更大或需多次分解时翻 Pollard-Rho。
- 返回 `vector<pair<i64,int>>`：每个元素 `（质因子， 次数）`，按质因子升序；`n` 本身是质数时返回 `{(n,1)}`；`n=1` 返回空列表。
- 可基于返回值算约数个数 `∏(c+1)`、欧拉函数，或用 `divisors_from_factorization` 枚举约数。


```cpp
vector<pair<i64, int>> factorize_trial(i64 n) {
    vector<pair<i64, int>> f;
    for (i64 p = 2; p * p <= n; ++p) {
        if (n % p) continue;
        int c = 0;
        while (n % p == 0) n /= p, c++;
        f.push_back({p, c});
    }
    if (n > 1) f.push_back({n, 1});
    return f;
}
```
#### 单个数欧拉函数

> **赛时先看**
> - **题目信号**：只问少量几个数的欧拉函数，不值得筛。
> - **本质**：求小范围外单个 `phi(n)`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(sqrt n)` 试除。
> - **维护的量**：`ans`（phi 值，每遇质因子 p 乘 `(p-1)/p`）；无其它状态。
> - **警告**：每遇到一个质因子 `p`，答案变成 `ans / p * (p - 1)`。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `phi(36)`。

```cpp
i64 ans = phi_single(36);              // 1. 一次试除求单点欧拉函数
cout << ans << '\n';
```

样例：`phi(36)=12`（36=2^2*3^2，36*(1/2)*(2/3)=12）。

**传参要求（照这个传不会错）：**

- `phi_single(n)`：`n` 为 i64（`n <= 1e12` 时 `O(sqrt n)` 可行）；返回 `phi(n)`（i64）。
- `n=1` 时返回 `1`；`n` 为质数时返回 `n-1`。
- 只求少量几个数的 phi 时用它；批量查询用「线性筛」或杜教筛。


```cpp
i64 phi_single(i64 n) {
    i64 ans = n;
    for (i64 p = 2; p * p <= n; ++p) {
        if (n % p) continue;
        ans = ans / p * (p - 1);
        while (n % p == 0) n /= p;
    }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
}
```
#### 枚举约数

> **赛时先看**
> - **题目信号**：题目需要遍历所有因子、判断约数贡献。
> - **本质**：得到一个数的所有正约数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：约数个数通常远小于 `n`。
> - **维护的量**：`divs`（结果列表，升序）；DFS 参数 `idx/cur` 为中间状态。
> - **警告**：先分解质因数，再 DFS 乘出约数。


**最小完整示例（先抄这一段就能跑）：**

题目：枚举 `36` 的所有正约数。

依赖：试除分解质因数 节的 factorize_trial，抄板时一起抄上。

```cpp
auto fac = factorize_trial(36);            // 1. 先分解质因数
auto divs = divisors_from_factorization(fac); // 2. DFS 乘出全部约数（已排序）
for (i64 d : divs) cout << d << ' ';
```

样例：`1 2 3 4 6 9 12 18 36`。

**传参要求（照这个传不会错）：**

- `divisors_from_factorization(fac)`：`fac` 直接传 `factorize_trial` 的返回（`（质因子， 次数）` 升序列表）；返回所有正约数（升序，含 1 与 n 本身）。
- `fac` 为空（n=1）时返回 `{1}`。
- 约数个数远小于 n，枚举安全；必须先分解，不要对 n 直接枚举到 sqrt。


```cpp
void gen_divisors_dfs(int idx, i64 cur,
                      const vector<pair<i64, int>>& fac,
                      vector<i64>& divs) {
    if (idx == (int)fac.size()) {
        divs.push_back(cur);
        return;
    }
    auto [p, c] = fac[idx];
    i64 x = 1;
    for (int i = 0; i <= c; ++i) {
        gen_divisors_dfs(idx + 1, cur * x, fac, divs);
        x *= p;
    }
}

vector<i64> divisors_from_factorization(vector<pair<i64, int>> fac) {
    vector<i64> divs;
    gen_divisors_dfs(0, 1, fac, divs);
    sort(divs.begin(), divs.end());
    return divs;
}
```
#### 整除分块

> **赛时先看**
> - **题目信号**：求和里有 `n / i`，`n` 很大，不能逐个枚举。
> - **本质**：快速枚举 `floor(n / i)` 相同的一段。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(sqrt n)` 段。
> - **维护的量**：`l/r`（当前段端点）、`q = floor(n/l)`（该段取值）；无其它状态。
> - **警告**：右端点 `r = n / (n / l)`。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `Σ_{i=1}^{n} floor(n/i)`，`n=10`。

```cpp
i64 sum = 0;
divisor_blocks(10, [&](i64 l, i64 r, i64 q) {
    sum += q * (r - l + 1);            // 2. 这段 [l,r] 内 floor(n/i) 都等于 q
});                                    // 1. 自动按取值相同分段
cout << sum << '\n';
```

样例：`n=10` 分 5 段：`(1,1,10) (2,2,5) (3,3,3) (4,5,2) (6,10,1)`，和 `=27`。

**传参要求（照这个传不会错）：**

- `divisor_blocks(n, visit)`：`n` 为 i64（可到 1e12+）；`visit(l, r, q)` 对每段调用一次，`[l,r]` 为段内 i 的范围，`q = floor(n/l)` 为该段取值。
- 段数 `O(sqrt n)`（n=1e12 时约 2e6 段），每段 O(1) 处理即可。
- 求和必须用段长 `(r-l+1)` 乘 `q`，不要逐个枚举 i。


```cpp
template <class F>
void divisor_blocks(i64 n, F visit) {
    for (i64 l = 1, r; l <= n; l = r + 1) {
        i64 q = n / l;
        r = n / q;
        visit(l, r, q); // i 在 [l,r] 内时 floor(n/i)=q。
    }
}
```
#### 杜教筛：大范围前缀 `phi` / `mu` 和

> **赛时先看**
> - **题目信号**：`phi`/`mu` 前缀和、互质对计数、整除分块，且 `n` 远大于可筛上界。
> - **本质**：求 `sum_{i<=n} phi(i)` 或 `sum_{i<=n} mu(i)`，其中 `n` 可到 `1e10` 甚至更大，不能直接筛到 `n`。
> - **接法**：求 `sum_{i=1}^n phi(i)`、`sum_{i=1}^n mu(i)`，或把 `gcd` 计数中的莫比乌斯前缀和替换成大范围查询。
> - **复杂度判定**：预筛 `O(N)`；记忆化后单次大询问常见 `O(n^(2/3))` 量级。
> - **维护的量**：`pre_phi`/`pre_mu`（预筛到 `N` 的前缀和表，`n <= N` 直接查）；`memo_phi`/`memo_mu`（大 `n` 的答案缓存）。
> - **警告**：这里返回的是前缀和，不是单点函数值；`phi` 前缀和可达 `Theta(n^2)` 量级，`sum_phi` 在 `n` 超过约 4e9 时 `i64` 会溢出，`n` 较大时改用 `i128` 或按题目取模。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `sum_{i<=10} phi(i)` 与 `sum_{i<=10} mu(i)`。

```cpp
DuJiaoPhiMu d(100);          // 1. 结构体定义：DuJiaoPhiMu(预筛上限)，默认 5e6
i64 p = d.sum_phi(10);       // 2. 调用：phi 前缀和，n 可远超预筛上界
i64 m = d.sum_mu(10);        // 3. 调用：mu 前缀和，内部整除分块 + 记忆化
cout << p << ' ' << m << '\n';
```

样例：`sum_phi(10) -> 32`；`sum_mu(10) -> -1`（本板无取模，原样输出）。

**传参要求（照这个传不会错）：**

- `DuJiaoPhiMu d(limit)`：`limit` = 预筛上界（int，默认 5000000）；`n <= limit` 直接查 `pre_phi`/`pre_mu` 表，`n > limit` 走记忆化递归。
- `d.sum_phi(n)` / `d.sum_mu(n)`：`n` 为 i64，可到 1e10 甚至更大；返回对应前缀和（i64）。
- 返回的是前缀和不是单点函数值；`sum_phi` 在 `n` 约 4e9 时开始溢出 i64，需要时改用 i128 或按题目取模。

**API / 入口函数（赛时只认这里列的名字）：**

- `init(int limit)` -> 初始化/清空结构

典题模型：求 `sum_{i=1}^n phi(i)`、`sum_{i=1}^n mu(i)`，或把 `gcd` 计数中的莫比乌斯前缀和替换成大范围查询。

```cpp
struct DuJiaoPhiMu {
    int N;
    vector<int> primes, is_comp, phi, mu;
    vector<i64> pre_phi, pre_mu;
    unordered_map<i64, i64> memo_phi, memo_mu;

    explicit DuJiaoPhiMu(int limit = 5000000) { init(limit); }

    void init(int limit) {
        N = limit;
        is_comp.assign(N + 1, 0);
        phi.assign(N + 1, 0);
        mu.assign(N + 1, 0);
        primes.clear();
        phi[1] = 1;
        mu[1] = 1;
        for (int i = 2; i <= N; ++i) {
            if (!is_comp[i]) {
                primes.push_back(i);
                phi[i] = i - 1;
                mu[i] = -1;
            }
            for (int p : primes) {
                i64 v = 1LL * i * p;
                if (v > N) break;
                is_comp[(int)v] = 1;
                if (i % p == 0) {
                    phi[(int)v] = phi[i] * p;
                    mu[(int)v] = 0;
                    break;
                }
                phi[(int)v] = phi[i] * (p - 1);
                mu[(int)v] = -mu[i];
            }
        }
        pre_phi.assign(N + 1, 0);
        pre_mu.assign(N + 1, 0);
        for (int i = 1; i <= N; ++i) {
            pre_phi[i] = pre_phi[i - 1] + phi[i];
            pre_mu[i] = pre_mu[i - 1] + mu[i];
        }
        memo_phi.clear();
        memo_mu.clear();
    }

    i64 sum_mu(i64 n) {
        if (n <= N) return pre_mu[(int)n];
        if (auto it = memo_mu.find(n); it != memo_mu.end()) return it->second;
        i64 ans = 1;
        for (i64 l = 2, r; l <= n; l = r + 1) {
            r = n / (n / l);
            ans -= (r - l + 1) * sum_mu(n / l);
        }
        return memo_mu[n] = ans;
    }

    i64 sum_phi(i64 n) {
        if (n <= N) return pre_phi[(int)n];
        if (auto it = memo_phi.find(n); it != memo_phi.end()) return it->second;
        i64 ans = n * (n + 1) / 2;
        for (i64 l = 2, r; l <= n; l = r + 1) {
            r = n / (n / l);
            ans -= (r - l + 1) * sum_phi(n / l);
        }
        return memo_phi[n] = ans;
    }
};
```
#### Min_25 筛核心：`pi(n)` 与质数和

> **赛时先看**
> - **题目信号**：`n` 大到不能直接筛，例如 `1e10`；答案依赖“所有不超过 n 的质数”的数量或质数和；更复杂的积性函数题可在此基础上枚举最小质因子和幂次递归。
> - **本质**：在远大于筛上界的 `n` 上，求质数个数 `pi(n)` 与 `sum_{p<=n} p`。这是 Min_25 筛处理积性函数前缀和的第一层核心。
> - **接法**：求 `pi(n)`、`sum_{p<=n} p`；或先计算 `f(p)` 的前缀和，再结合素数幂递归求完全积性函数相关前缀和。
> - **复杂度判定**：`O(n^(3/4) / log n)` 量级，空间 `O(sqrt n)`。
> - **维护的量**：`values`（整除分块点集）；`prime_count`/`prime_sum`（去掉合数贡献后的质数计数/质数和表，`prime_sum` 为 `i128`）。
> - **警告**：这份是最稳的“质数计数/质数和核心版”，不是任意积性函数的一键模板；`sum_primes` 用 `i128` 保存，输出时自行写转换函数。


**最小完整示例（先抄这一段就能跑）：**

题目：`n=1e10` 时求 `pi(100)` 与 `sum_{p<=100} p`（n 大到不能直接筛）。

```cpp
Min25PrimePrefix s(10000000000LL);   // 1. 结构体定义：Min25PrimePrefix(n)，构造即 build
i64 cnt = s.pi(100);                 // 2. 调用：pi(x)，x 可以远小于 n
i128 sum = s.sum_primes(100);        // 3. 调用：质数和，返回 i128
cout << cnt << '\n';                 // 4. i128 输出需自己写转换
```

样例：`pi(100) -> 25`；`sum_primes(100) -> 1060`（2+3+...+97）。

**传参要求（照这个传不会错）：**

- `Min25PrimePrefix s(n)`：`n` = 题目上界（i64，如 1e10）；构造时完成全部预处理，之后只查表。
- `s.pi(x)`：`x` 为 i64，`1 <= x <= n`；返回 `[1,x]` 内质数个数（i64）。
- `s.sum_primes(x)`：同样要求 `x <= n`；返回 `sum_{p<=x} p`，类型 `i128`，输出前自行转字符串或拆成两个 long long。
- 只负责质数计数/质数和；任意积性函数前缀和要按注释改 `prime_count`/`prime_sum` 的转移。

**API / 入口函数（赛时只认这里列的名字）：**

- `build()` -> 完成建树或预处理
**改板时先认这几个量：**

- `cur`：整除分块中当前值 `x` 在 `values` 里的下标。
- `pi(x)`：质数个数函数，查 `prime_count` 表。

典题模型：求 `pi(n)`、`sum_{p<=n} p`；或先计算 `f(p)` 的前缀和，再结合素数幂递归求完全积性函数相关前缀和。

```cpp
struct Min25PrimePrefix {
    i64 n;
    int sq;
    vector<int> primes;
    vector<i64> values, prime_count;
    vector<i128> prime_sum;
    vector<int> id_small, id_large;

    explicit Min25PrimePrefix(i64 n_) : n(n_) { build(); }

    int id(i64 x) const {
        return x <= sq ? id_small[(int)x] : id_large[(int)(n / x)];
    }

    void build() {
        sq = sqrtl(n);
        while (1LL * (sq + 1) * (sq + 1) <= n) ++sq;
        while (1LL * sq * sq > n) --sq;

        vector<int> is_comp(sq + 1);
        for (int i = 2; i <= sq; ++i) {
            if (!is_comp[i]) primes.push_back(i);
            for (int p : primes) {
                if (1LL * i * p > sq) break;
                is_comp[i * p] = 1;
                if (i % p == 0) break;
            }
        }
        vector<i128> pre_prime_sum(primes.size() + 1);
        for (int i = 0; i < (int)primes.size(); ++i) pre_prime_sum[i + 1] = pre_prime_sum[i] + primes[i];

        id_small.assign(sq + 1, -1);
        id_large.assign(sq + 1, -1);
        for (i64 l = 1, r; l <= n; l = r + 1) {
            i64 x = n / l;
            r = n / x;
            int cur = (int)values.size();
            values.push_back(x);
            prime_count.push_back(x - 1);                    // 2..x 的整数个数
            prime_sum.push_back((i128)x * (x + 1) / 2 - 1); // 2..x 的整数和
            if (x <= sq) id_small[(int)x] = cur;
            else id_large[(int)(n / x)] = cur;
        }

        for (int i = 0; i < (int)primes.size(); ++i) {
            i64 p = primes[i];
            if (p * p > n) break;
            for (int j = 0; j < (int)values.size() && values[j] >= p * p; ++j) {
                int k = id(values[j] / p);
                prime_count[j] -= prime_count[k] - i;
                prime_sum[j] -= (i128)p * (prime_sum[k] - pre_prime_sum[i]);
            }
        }
    }

    i64 pi(i64 x) const { return prime_count[id(x)]; }
    i128 sum_primes(i64 x) const { return prime_sum[id(x)]; }
};
```
#### Meissel-Lehmer：超大范围质数个数 `pi(n)`

> **赛时先看**
> - **题目信号**：题目直接问 `pi(n)`，或统计中需要大量知道“`<= x` 的质数数量”；`n` 可到 `1e10`、`1e12` 甚至更高，但查询次数不多。
> - **本质**：求不大于 `n` 的质数个数 `pi(n)`。相比直接筛到 `n`，它用一个中等大小的预处理筛和递归计数处理远大于内存上界的 `n`。
> - **接法**：`n` 极大时求 `[1,n]` 里质数个数；多组大 `x` 的质数计数；数论分块里反复出现 `pi(n / i)` 的变种（查询很多时要再做记忆化策略评估）。
> - **复杂度判定**：单次是亚线性；对本实现的常见 `n <= 1e12` 场景很实用。预处理时间 `O(SIEVE_N log log SIEVE_N)`，内存约 65 MB（`SIEVE_N=5e6`、`PHI_N=1e5`）。
> - **维护的量**：`pi`（`SIEVE_N` 内质数个数前缀表）；`primes`（质数表）；`phi_small`（小范围 `phi` 查表）；`memo`（大 `x` 的 `pi_count` 结果缓存）。
> - **警告**：它只求“质数个数”，不是完整的 Min_25 任意积性函数筛。浮点开根会偏一，代码中的整数根修正不能删。`n` 必须非负，`i64` 范围内乘方比较使用 `i128`。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `pi(100)` 与 `pi(1e10)`（超大范围质数个数，构造一次后反复查询）。

```cpp
LehmerPrimeCounter counter;                    // 1. 结构体定义：构造即预筛（内存约 65 MB）
i64 a = counter.pi_count(100);                 // 2. 调用：小 x 直接查 pi 表
i64 b = counter.pi_count(10000000000LL);       // 3. 调用：大 x 走递归计数 + memo 缓存
cout << a << ' ' << b << '\n';
```

样例：`pi_count(100) -> 25`；`pi_count(1e10) -> 455052511`。

**传参要求（照这个传不会错）：**

- `LehmerPrimeCounter counter;`：无参构造，内部完成 `SIEVE_N=5e6` 预筛 + `PHI_N=1e5` 表格；多次查询复用同一个对象。
- `counter.pi_count(x)`：`x` 为 i64（非负）；返回 `[1,x]` 内质数个数（i64）。
- 内存低于 128 MB 时改小 `SIEVE_N`/`PHI_N` 并压测，或换 Min_25 核心版。

**API / 入口函数（赛时只认这里列的名字）：**

- `build()` -> 完成建树或预处理
**改板时先认这几个量：**

- `pi`：前缀表，`pi[i]` 为 `[1,i]` 内的质数个数。
- `memo`：大 `x` 的 `pi_count` 结果缓存。

使用：构造一次 `LehmerPrimeCounter counter;`，调用 `counter.pi_count(n)`。先评估内存限制；内存低于 128 MB 时，应改小参数并充分压测，或改用 Min_25 核心版。

典题模型：`n` 极大时求 `[1,n]` 里质数个数；多组大 `x` 的质数计数；数论分块里反复出现 `pi(n / i)` 的变种（查询很多时要再做记忆化策略评估）。

```cpp
struct LehmerPrimeCounter {
    static constexpr int SIEVE_N = 5'000'000;
    static constexpr int PHI_N = 100'000;
    static constexpr int PHI_S = 100;

    vector<int> primes, pi;
    vector<array<int, PHI_S>> phi_small;
    unordered_map<i64, i64> memo;

    LehmerPrimeCounter() { build(); }

    static i64 isqrt(i64 x) {
        i64 r = sqrtl((long double)x);
        while ((r + 1) <= x / (r + 1)) ++r;
        while (r > x / r) --r;
        return r;
    }

    static i64 icbrt(i64 x) {
        i64 r = cbrtl((long double)x);
        while ((i128)(r + 1) * (r + 1) * (r + 1) <= x) ++r;
        while ((i128)r * r * r > x) --r;
        return r;
    }

    void build() {
        vector<bool> composite(SIEVE_N + 1);
        pi.assign(SIEVE_N + 1, 0);
        for (int i = 2; i <= SIEVE_N; ++i) {
            if (!composite[i]) primes.push_back(i);
            for (int p : primes) {
                if (1LL * i * p > SIEVE_N) break;
                composite[i * p] = true;
                if (i % p == 0) break;
            }
            pi[i] = pi[i - 1] + (!composite[i]);
        }

        phi_small.resize(PHI_N);
        for (int x = 0; x < PHI_N; ++x) phi_small[x][0] = x;
        for (int s = 1; s < PHI_S; ++s) {
            for (int x = 0; x < PHI_N; ++x) {
                phi_small[x][s] = phi_small[x][s - 1]
                    - phi_small[x / primes[s - 1]][s - 1];
            }
        }
    }

    i64 phi(i64 x, int s) const {
        if (s == 0) return x;
        if (s < PHI_S && x < PHI_N) return phi_small[(int)x][s];
        return phi(x, s - 1) - phi(x / primes[s - 1], s - 1);
    }

    i64 pi_count(i64 x) {
        if (x <= SIEVE_N) return pi[(int)x];
        if (auto it = memo.find(x); it != memo.end()) return it->second;

        i64 a = pi_count(isqrt(isqrt(x))); // 公式：pi(x^(1/4))。
        i64 b = pi_count(isqrt(x));        // 公式：pi(sqrt(x))。
        i64 c = pi_count(icbrt(x));        // 公式：pi(x^(1/3))。
        i64 ans = phi(x, (int)a) + (b + a - 2) * (b - a + 1) / 2;

        for (i64 i = a; i < b; ++i) {
            i64 w = x / primes[(int)i];
            ans -= pi_count(w);
            if (i < c) {
                i64 limit = pi_count(isqrt(w));
                for (i64 j = i; j < limit; ++j) {
                    ans -= pi_count(w / primes[(int)j]) - j;
                }
            }
        }
        return memo[x] = ans;
    }
};
```
#### Mobius 反演常见求和

> **赛时先看**
> - **题目信号**：题面出现 `gcd(i,j)=1`、`gcd(i,j)=d`、`floor(n/i)` 分块。
> - **本质**：求互质对数、`gcd` 相关计数、整除关系计数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：预处理 `O(n)`，单次求和 `O(sqrt n)` 或 `O(n)`。
> - **维护的量**：`prefix_mu`（mu 前缀和，分块差分用）；`n/m`（求和上界）；`l/r`（整除分块端点）。
> - **警告**：先预处理 `mu` 前缀和；`gcd(i,j)=d` 转成互质对 `(i/d,j/d)`。


**最小完整示例（先抄这一段就能跑）：**

题目：统计 `1<=i,j<=10` 中 `gcd(i,j)=1` 的互质数对个数。

依赖：线性筛：phi / mu / 约数个数 节的 MultiplicativeSieve，抄板时一起抄上。

```cpp
MultiplicativeSieve s(10);                    // 1. 先线性筛 mu（存在 s.mu）
vector<i64> prefix_mu(11, 0);
for (int i = 1; i <= 10; ++i) prefix_mu[i] = prefix_mu[i - 1] + s.mu[i];
i64 ans = coprime_pairs(10, 10, prefix_mu);  // 2. 调用：统计 1<=i,j<=10 的互质对数
cout << ans << '\n';                           // 3. 取结果
```

样例：`coprime_pairs(10, 10, prefix_mu) -> 63`；`gcd_equal_pairs(10, 10, 2, prefix_mu) -> 19`（即 5x5 互质对数）。

**传参要求（照这个传不会错）：**

- `coprime_pairs(n, m, prefix_mu)`：`n`/`m` = 上界（i64，`>= 1`）；`prefix_mu` 长度需 ≥ `min(n,m)+1`，且 `prefix_mu[i] = sum_{j=1..i} mu(j)`。
- 返回 `1<=i<=n`、`1<=j<=m` 中 `gcd(i,j)=1` 的数对个数（i64）。
- `gcd_equal_pairs(n, m, g, prefix_mu)`：返回 `gcd(i,j)=g` 的数对个数，内部即 `coprime_pairs(n/g, m/g, prefix_mu)`，`g` 需 ≥ 1。
- 前提：`prefix_mu` 必须先用线性筛构造好；`mu(1)=1`，含平方因子的下标不会增加前缀值。


**API / 入口函数（赛时只认这里列的名字）：**

- `coprime_pairs(i64 n, i64 m, const vector<i64>& prefix_mu)` -> 统计满足 1<=i<=n、1<=j<=m 且 gcd(i,j)=1 的数对。 返回 `i64`。

```cpp
// 统计满足 1<=i<=n、1<=j<=m 且 gcd(i,j)=1 的数对。
i64 coprime_pairs(i64 n, i64 m, const vector<i64>& prefix_mu) {
    i64 ans = 0;
    i64 lim = min(n, m);
    for (i64 l = 1, r; l <= lim; l = r + 1) {
        r = min(n / (n / l), m / (m / l));
        ans += (prefix_mu[r] - prefix_mu[l - 1]) * (n / l) * (m / l);
    }
    return ans;
}

// 统计 gcd(i,j)=g 的数对。
i64 gcd_equal_pairs(i64 n, i64 m, i64 g,
                          const vector<i64>& prefix_mu) {
    return coprime_pairs(n / g, m / g, prefix_mu);
}
```
#### 组合数预处理

> **赛时先看**
> - **题目信号**：题面出现"方案数""选若干个""组合计数"且模数是质数、`n < mod`，比如网格路径数、至少/至多选择类计数、二项式求和；看到"大量 `C(n,k)` 查询 mod 质数"就先想这节。
> - **本质**：一次预处理 `fac[i]=i!` 与 `ifac[i]=(i!)^{-1}`（费马小定理求最大阶乘逆元后回推），查询 `C(N,K)=fac[N]*ifac[K]*ifac[N-K]`，把每次 O(K) 的阶乘除法压成 O(1) 查表。
> - **复杂度判定**：预处理 `O(n)`，查询 `O(1)`；n 到 1e6-1e7 都稳；若 `n >= mod`，阶乘会含 `mod` 因子而出现 0，换 Lucas；模数不是质数换 exLucas。
> - **维护的量**：`n`（预处理上限）、`mod`（质数模数）、`fac`（阶乘）、`ifac`（阶乘逆元）、`inv`（单点逆元，备用）。
> - **接法**：模数是质数且 `N <= max_n` 时先 `Comb comb(max_n, mod)`，再 `comb.C(n,k)`。网格从 `(1,1)` 到 `(n,m)` 只向右下走通常是 `C(n+m-2,n-1)`。若模数不是质数，不能用费马逆元预处理，翻 exLucas 或其他计数方法。
> - **警告**：`mod` 必须是质数（逆元走 `a^(mod-2)`）；`n >= mod` 时阶乘出现 0，不能直接套这版；非法 K 越界返回 0。


**最小完整示例（先抄这一段就能跑）：**

题目：`m` 次询问 `C(n,k) mod 1e9+7`，`n` 最大 `1e6`。

```cpp
Comb comb(1000000, 1000000007);   // 1. 结构体定义：Comb(预处理上限, 质数模数)
while (m--) {
    int n, k;
    cin >> n >> k;
    cout << comb.C(n, k) << '\n';   // 2. 调用：O(1) 返回 C(n,k) mod
}
```

样例：`C(5,2)` -> 10；`C(10,3)` -> 120。

**传参要求（照这个传不会错）：**

- `Comb(max_n, mod)`：构造即预处理；`max_n` = 可能出现的最大 `n`；`mod` 必须是**质数**。
- `C(n, k)`：返回 `int`（取模后）；`k < 0 || k > n` 返回 0。
- 要求 `n < mod`；`n >= mod` 时阶乘会含 `mod` 因子（结果 0），换 Lucas。
- 网格路径数（只向右下走）`(1,1)->(n,m)` = `C(n+m-2, n-1)`。
- 模数不是质数时换 exLucas 或别的计数方法。


**不会用就照抄：**

```cpp
Comb C(MAXN);
cout << C.C(n, k) << '\n';
```

- `MAXN` 必须覆盖题目最大 `n`；构造 `Comb C(MAXN, mod)` 后再查询。
- 这版逆元用 `a^(mod-2)`，因此 **mod 必须是质数**；`n >= mod` 时阶乘会出现 0，也不能直接套这版。


**API / 入口函数（赛时只认这里列的名字）：**

- `Comb C(max_n,mod)` -> 预处理到 `max_n`；这版要求 `mod` 为质数。
- `C.C(N,K)` -> 返回 `C(N,K) mod mod`；非法 K 返回 0。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `Comb` 结构体。
2. 构造：`Comb C(MAXN, mod);`，`MAXN` 必须 ≥ 题面最大 `n`，`mod` 填题面模数（默认 1e9+7）。
3. 调用：`C.C(n, k)`。
4. 取结果：返回的就是 `C(n,k) mod mod`，直接输出。

**改造点（按题目改这几处）：**
- 模数：构造时第二个参数传题面模数；必须是质数，否则翻 exLucas。
- `n >= mod`：本板阶乘会含 `mod` 因子，直接失效，换同章 `lucas`。
- 需要可重复组合（从 n 种里可重复选 k 个）：用 `C(n+k-1, k)`。
- 大 n（1e18）小模数：翻同章 Lucas 定理；模数极小（如 2）时注意 Lucas 递归到 `k==0` 的基例。

**核心逻辑（改代码时别破坏）：**

- 先预处理 `fac[i]=i!` 与 `ifac[i]=(i!)^{-1}`。
- 查询 `C(N,K)=fac[N]*ifac[K]*ifac[N-K]`，所以单次 O(1)。

**改板时先认这几个量：**

- `fac[i]`：`i! mod mod`；`ifac[i]`：`(i!)^{-1} mod mod`。
- 查询公式 `C(N,K)=fac[N]*ifac[K]*ifac[N-K]`。

```cpp
// 维护的量：fac[i] = i! mod mod；ifac[i] = (i!)^{-1} mod mod；inv[i] 单点逆元（备用）。
// 不变量：fac[n] * ifac[n] ≡ 1 (mod mod)，查询 C(N,K)=fac[N]*ifac[K]*ifac[N-K]。
// 前提：mod 必须为质数（逆元走费马小定理 a^(mod-2)），且 max_n < mod。
struct Comb {
    int n;
    i64 mod;
    vector<i64> fac, ifac, inv;

    i64 mod_pow(i64 a, i64 b) const {
        i64 res = 1 % mod;
        while (b) {
            if (b & 1) res = (i128)res * a % mod;
            a = (i128)a * a % mod;
            b >>= 1;
        }
        return res;
    }

    Comb(int max_n = 0, i64 mod_ = 1000000007LL) { init(max_n, mod_); }

    void init(int max_n, i64 mod_) {
        n = max_n;
        mod = mod_;
        fac.assign(n + 1, 1);
        ifac.assign(n + 1, 1);
        inv.assign(n + 1, 1);
        for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i % mod; // 递推阶乘
        ifac[n] = mod_pow(fac[n], mod - 2); // 费马小定理求最大阶乘的逆元
        for (int i = n; i >= 1; --i) ifac[i - 1] = ifac[i] * i % mod; // 回推所有阶乘逆元
    }

    i64 C(int N, int K) const {
        if (K < 0 || K > N || N < 0 || N > n) return 0;
        return fac[N] * ifac[K] % mod * ifac[N - K] % mod; // 组合数公式，O(1) 查表
    }
};
```
#### Lucas 定理

> **赛时先看**
> - **题目信号**：`n` 可到 `1e18`，但 `p` 较小且为质数。
> - **本质**：`n,k` 很大，模数是小质数。
> - **接法**：`n,k` 很大但模数 `p` 是小质数时，先 `Comb comb(p-1,p)`，再 `lucas(n,k,comb)`。如果 `p` 不是质数，Lucas 不成立，翻 exLucas。
> - **复杂度判定**：`O(log_p n)` 次组合数查询。
> - **维护的量**：无状态；递归参数 `n/k` 每层除以 `comb.mod`，直至 `k==0`。
> - **警告**：组合数预处理到 `p-1`。


**最小完整示例（先抄这一段就能跑）：**

题目：`p=7` 下求 `C(10,3) mod 7`。

依赖：组合数预处理 节的 Comb，抄板时一起抄上。

```cpp
Comb comb(6, 7);                       // 1. 预处理组合数到 p-1=6
i64 ans = lucas(10, 3, comb);          // 2. 递归 Lucas：n 大、p 为小质数
cout << ans << '\n';
```

样例：`lucas(10,3) -> 1`（C(10,3)=120 ≡ 1 mod 7）。

**传参要求（照这个传不会错）：**

- `lucas(n, k, comb)`：`n/k` 为 i64（可到 1e18）；返回 `C(n,k) % comb.mod`（i64）；`k<0 || k>n` 返回 0。
- `comb` 用「组合数预处理」的 `Comb(p-1, p)` 构造：预处理上限必须 ≥ `p-1`，模数 `p` 必须是**质数**，否则 Lucas 不成立（换 exLucas）。
- 复杂度 `O(log_p n)` 次组合数查询，单次查询 O(1)。


```cpp
i64 lucas(i64 n, i64 k, const Comb& comb) {
    if (k < 0 || k > n) return 0;
    if (k == 0) return 1;
    return lucas(n / comb.mod, k / comb.mod, comb) *
           comb.C(n % comb.mod, k % comb.mod) % comb.mod;
}
```
#### exLucas：组合数模任意合数

> **赛时先看**
> - **题目信号**：题面明确给出合数模数；要求组合数模 `6 / 8 / 10^6 / 998244352` 等非质数；逆元法、普通 Lucas 不能直接使用；`n` 很大但模数及其各素数幂不大。
> - **本质**：计算 `C(n,k) mod m`，其中 `m` 是合数。代码把 `m` 分解为互素的素数幂，在每个素数幂下去掉 `p` 因子计算阶乘，最后用 CRT 合并。
> - **接法**：大 `n` 小合数模的二项式计数；周期计数后对合数取模；容斥中的组合数模 `10^k`；CRT 综合题。
> - **复杂度判定**：预处理每个素数幂 `p^a` 为 `O(p^a)`，单次组合数为 `O(log_p n)`；分解模数为 `O(sqrt(m))`。本实现要求所有素数幂因子不超过 `2e6`，这是用空间换稳定性的版本。
> - **维护的量**：`p/exponent/pk`（当前素数幂及其模数）；`prefix[i]`（1..i 去掉 p 因子的乘积模 pk）；`remain/merged_mod/ans`（CRT 合并中间量）。
> - **警告**：普通 Lucas 只适用于质数模数。不要对含 `p` 因子的阶乘直接求逆；先记录 `p` 的指数，剩余部分才互素。模数为 `1` 的答案恒为 `0`。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `C(20,10) mod 1000000`（模数是合数）。

```cpp
i64 ans = binom_mod_composite(20, 10, 1000000);   // 1. 调用：n、k、合数模数
cout << ans << '\n';                               // 2. 取结果：C(20,10) mod 1e6
```

样例：`binom_mod_composite(20, 10, 1000000) -> 184756`（C(20,10)=184756 本身 < 1e6）；`binom_mod_composite(8, 4, 6) -> 4`（70 mod 6 = 4）。

**传参要求（照这个传不会错）：**

- `binom_mod_composite(n, k, mod)`：`n` = 总数（exlucas_ll = i64，可很大）；`k` = 选取数（要求 `0 <= k <= n`）；`mod` = 模数（exlucas_ll，`>= 1`，合数也行）。
- 返回 `C(n,k) mod mod`；`mod == 1` 或 `k < 0 || k > n` 时返回 0。
- 前提：`mod` 的每个质数幂因子 `p^e` 都 ≤ `EXLUCAS_MAX_PRIME_POWER`（2000000），否则建表 assert 失败。
- `factorial_without_p`、`ExLucasPrimePower` 是内部 helper，不要从 `solve()` 直接调。


**API / 入口函数（赛时只认这里列的名字）：**

- `binom_mod_composite(exlucas_ll n, exlucas_ll k, exlucas_ll mod)` -> 要求 mod 的每个质数幂因子都不超过 EXLUCAS_MAX_PRIME_POWER。 返回 `exlucas_ll`。
- `factorial_without_p(exlucas_ll n)` -> 把 n! 中所有 p 因子剔除后，对 p^exponent 取模。 返回 `exlucas_ll`。
**改板时先认这几个量：**

- `prefix`：prefix[i] 表示 1..i 中去掉 p 因子的乘积，模 pk。
- `pk`：当前素数幂模数 `p^exponent`，本节运算都在该模下进行。

典题模型：大 `n` 小合数模的二项式计数；周期计数后对合数取模；容斥中的组合数模 `10^k`；CRT 综合题。

```cpp
using exlucas_ll = i64;
constexpr exlucas_ll EXLUCAS_MAX_PRIME_POWER = 2000000;

exlucas_ll exlucas_exgcd(exlucas_ll a, exlucas_ll b,
                          exlucas_ll& x, exlucas_ll& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    exlucas_ll x1, y1;
    exlucas_ll g = exlucas_exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

exlucas_ll exlucas_inv(exlucas_ll a, exlucas_ll mod) {
    exlucas_ll x, y;
    exlucas_ll g = exlucas_exgcd(a, mod, x, y);
    assert(g == 1);
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

exlucas_ll exlucas_pow(exlucas_ll a, exlucas_ll e, exlucas_ll mod) {
    exlucas_ll r = 1 % mod;
    while (e) {
        if (e & 1) r = (exlucas_ll)((i128)r * a % mod);
        a = (exlucas_ll)((i128)a * a % mod);
        e >>= 1;
    }
    return r;
}

struct ExLucasPrimePower {
    exlucas_ll p, pk;
    int exponent;
    vector<exlucas_ll> prefix; // prefix[i] 表示 1..i 中去掉 p 因子的乘积，模 pk。

    ExLucasPrimePower(exlucas_ll prime, int power) : p(prime), exponent(power) {
        pk = 1;
        for (int i = 0; i < exponent; ++i) pk *= p;
        // 表长为 pk+1；只有确认内存足够后才能调大上界。
        assert(pk <= EXLUCAS_MAX_PRIME_POWER);
        prefix.assign((int)pk + 1, 1);
        for (int i = 1; i <= pk; ++i) {
            prefix[i] = prefix[i - 1];
            if (i % p != 0) prefix[i] = prefix[i] * i % pk;
        }
    }

    exlucas_ll valuation(exlucas_ll n) const {
        exlucas_ll ans = 0;
        while (n) n /= p, ans += n;
        return ans;
    }

    // 把 n! 中所有 p 因子剔除后，对 p^exponent 取模。
    exlucas_ll factorial_without_p(exlucas_ll n) const {
        if (n == 0) return 1;
        exlucas_ll ans = exlucas_pow(prefix[pk], n / pk, pk);
        ans = ans * prefix[n % pk] % pk;
        return ans * factorial_without_p(n / p) % pk;
    }

    exlucas_ll binom(exlucas_ll n, exlucas_ll k) const {
        if (k < 0 || k > n) return 0;
        exlucas_ll e = valuation(n) - valuation(k) - valuation(n - k);
        if (e >= exponent) return 0;
        exlucas_ll a = factorial_without_p(n);
        exlucas_ll b = factorial_without_p(k);
        exlucas_ll c = factorial_without_p(n - k);
        exlucas_ll ans = a * exlucas_inv(b, pk) % pk * exlucas_inv(c, pk) % pk;
        return ans * exlucas_pow(p, e, pk) % pk;
    }
};

// 要求 mod 的每个质数幂因子都不超过 EXLUCAS_MAX_PRIME_POWER。
exlucas_ll binom_mod_composite(exlucas_ll n, exlucas_ll k, exlucas_ll mod) {
    assert(mod >= 1);
    if (mod == 1 || k < 0 || k > n) return 0;
    exlucas_ll remain = mod, ans = 0, merged_mod = 1;
    for (exlucas_ll p = 2; p <= remain / p; ++p) {
        if (remain % p != 0) continue;
        int e = 0;
        exlucas_ll pk = 1;
        while (remain % p == 0) remain /= p, pk *= p, ++e;
        ExLucasPrimePower solver(p, e);
        exlucas_ll residue = solver.binom(n, k);
        exlucas_ll delta = (residue - ans) % pk;
        if (delta < 0) delta += pk;
        exlucas_ll t = delta * exlucas_inv(merged_mod % pk, pk) % pk;
        ans = (exlucas_ll)((i128)ans + (i128)merged_mod * t);
        merged_mod *= pk;
        ans %= merged_mod;
    }
    if (remain > 1) {
        ExLucasPrimePower solver(remain, 1);
        exlucas_ll pk = remain, residue = solver.binom(n, k);
        exlucas_ll delta = (residue - ans) % pk;
        if (delta < 0) delta += pk;
        exlucas_ll t = delta * exlucas_inv(merged_mod % pk, pk) % pk;
        ans = (exlucas_ll)((i128)ans + (i128)merged_mod * t);
        merged_mod *= pk;
        ans %= merged_mod;
    }
    return ans;
}
```
#### Catalan 数

> **赛时先看**
> - **题目信号**：方案数满足“不越过对角线”“任意前缀某类数量不少于另一类”。
> - **本质**：合法括号序列、栈出栈序列、凸多边形三角剖分、二叉树计数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：预处理组合数后 `O(1)` 查询。
> - **维护的量**：无状态；只依赖 `comb.C` 的两次查询 `C(2n,n)` 与 `C(2n,n+1)`。
> - **警告**：常见公式 `C(2n,n)/(n+1)`，模数非质数时不能直接除。


**最小完整示例（先抄这一段就能跑）：**

题目：模 `1e9+7` 下求第 `5` 个 Catalan 数（如 5 对括号的合法序列数）。

依赖：线性求逆元与阶乘逆元 节的 CombFast，抄板时一起抄上。

```cpp
CombFast comb(10, 1000000007);         // 1. 预处理到 2n=10（组合数 O(1) 查询）
i64 ans = catalan(5, comb);            // 2. 直接返回 Catalan(5)
cout << ans << '\n';
```

样例：`catalan(5) -> 42`。

**传参要求（照这个传不会错）：**

- `catalan(n, comb)`：`n` 为 int（非负）；返回 `C(2n,n)-C(2n,n+1) mod comb.mod`（i64）。
- `comb` 用「线性求逆元与阶乘逆元」的 `CombFast(2*n, mod)` 构造，`mod` 必须是**质数**；预处理上限要 ≥ `2n`。
- 等价公式 `C(2n,n)/(n+1)` 需要模意义除法，非质数模数下本板也不能用，换其它计数方式。


**API / 入口函数（赛时只认这里列的名字）：**

- `catalan(int n, const CombFast& comb)` -> mod 是质数，已有组合数 C。 返回 `i64`。

```cpp
// mod 是质数，已有组合数 C。
i64 catalan(int n, const CombFast& comb) {
    return (comb.C(2 * n, n) - comb.C(2 * n, n + 1) + comb.mod) % comb.mod;
}
```
#### Stirling 数

> **赛时先看**
> - **题目信号**：把 `n` 个不同元素分成 `k` 个非空无标号集合；或排列恰有 `k` 个环。
> - **本质**：集合划分、排列环数、容斥计数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(nk)`。
> - **维护的量**：`dp[i][j]`（`i` 个元素分成 `j` 块的方案数，二维表）；答案取 `dp[n][k]`。
> - **警告**：第二类 `S(n,k)=S(n-1,k-1)+k*S(n-1,k)`；第一类无符号 `s(n,k)=s(n-1,k-1)+(n-1)*s(n-1,k)`。


**最小完整示例（先抄这一段就能跑）：**

题目：模 `1e9+7` 下求第二类 Stirling 数 `S(5,3)` 与第一类无符号 `s(5,3)`。

```cpp
auto s2 = stirling2(5, 5, 1000000007LL);      // 1. 调用：返回 (n+1) x (k+1) 的 dp 表
auto s1 = stirling1_unsigned(5, 5, 1000000007LL);
cout << s2[5][3] << ' ' << s1[5][3] << '\n';  // 2. 取结果：答案在 dp[n][k]
```

样例：`S(5,3) -> 25`；`s(5,3) -> 35`（均 mod 1e9+7，原值小于模数）。

**传参要求（照这个传不会错）：**

- `stirling2(n, k, mod)` / `stirling1_unsigned(n, k, mod)`：`n`、`k` 为 int（非负）；`mod` 为 i64 模数（本板只用加法乘法，合数模也能用）。
- 返回 `vector<vector<i64>>`，大小 `(n+1) x (k+1)`；答案取 `dp[n][k]`（下标从 1 开始数，`dp[0][0]=1`）。
- `j > i` 的位置恒为 0；复杂度 `O(nk)`，`n*k` 到 1e7 内稳妥。


```cpp
vector<vector<i64>> stirling2(int n, int k, i64 mod) {
    vector dp(n + 1, vector<i64>(k + 1, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= min(i, k); j++) {
            dp[i][j] = (dp[i - 1][j - 1] + dp[i - 1][j] * j) % mod;
        }
    }
    return dp;
}

vector<vector<i64>> stirling1_unsigned(int n, int k, i64 mod) {
    vector dp(n + 1, vector<i64>(k + 1, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= min(i, k); j++) {
            dp[i][j] = (dp[i - 1][j - 1] + dp[i - 1][j] * (i - 1)) % mod;
        }
    }
    return dp;
}
```
#### Burnside / Polya 枚举

> **赛时先看**
> - **题目信号**：问“旋转相同算一种”“翻转相同算一种”“本质不同”。
> - **本质**：群作用下本质不同方案计数，如项链、手链、旋转翻转等价染色。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：枚举群元素，通常 `O(|G| log n)`。
> - **维护的量**：`sum`（各置换固定方案数累加）；无额外结构，跑一遍群元素即可。
> - **警告**：Burnside 是所有置换固定方案数的平均值；颜色数为 `c` 时，一个置换固定方案为 `c^(cycle_count)`。


**最小完整示例（先抄这一段就能跑）：**

题目：`n=4` 颗珠子、`c=2` 种颜色，旋转等价的不同项链数，模 `1e9+7`。

```cpp
i64 ans = necklace_rotation_count(4, 2, 1000000007LL); // 1. 调用：旋转等价项链数
cout << ans << '\n';                                    // 2. 取结果：直接输出
```

样例：`necklace_rotation_count(4, 2, 1e9+7) -> 6`（(16+2+4+2)/4=6）。

**传参要求（照这个传不会错）：**

- `necklace_rotation_count(n, c, mod)`：`n` = 珠子数（int，≥1）；`c` = 颜色数（i64）；`mod` 必须是**质数**（末尾要乘 `n` 的逆元，走 `n^(mod-2)`）。
- 返回旋转等价的本质不同项链数（已对 `mod` 取模，i64）。
- 只处理旋转等价；要算翻转（二面体群）需自行枚举镜像置换，公式同理 `c^(cycle_count)`。


**API / 入口函数（赛时只认这里列的名字）：**

- `necklace_rotation_count(int n, i64 c, i64 mod)` -> n 个珠子，c 种颜色，旋转等价的项链数量。mod 为质数。 返回 `i64`。

```cpp
// 与通用辅助函数中的 mod_pow 功能相同，改名避免重名。
i64 mod_pow_burnside(i64 a, i64 e, i64 mod) {
    i64 r = 1 % mod;
    while (e) {
        if (e & 1) r = r * a % mod;
        a = a * a % mod;
        e >>= 1;
    }
    return r;
}

// n 个珠子，c 种颜色，旋转等价的项链数量。mod 为质数。
i64 necklace_rotation_count(int n, i64 c, i64 mod) {
    i64 sum = 0;
    for (int shift = 0; shift < n; shift++) {
        sum = (sum + mod_pow_burnside(c, std::gcd(n, shift), mod)) % mod;
    }
    return sum * mod_pow_burnside(n, mod - 2, mod) % mod;
}
```
#### Pruefer 序列：带标号树编码、解码与凯莱公式

> **赛时先看**
> - **题目信号**：题面是带标号树计数；每次删编号最小叶子并记录其邻点；给一串长度 `n-2` 的数要求还原树；或给森林各连通块大小，问添加 `k-1` 条边后连通的方案数。
> - **本质**：把 `n` 个带标号点的树双射为一个长度 `n-2` 的整数序列。除了树编码/还原，还可直接推出凯莱公式和“把多个连通块加边连成一棵树”的计数。
> - **接法**：Luogu P6086 Pruefer 序列；凯莱公式；森林连通方案计数；随机生成带标号树。
> - **复杂度判定**：这里使用小根堆，编码与解码都是 `O(n log n)`，空间 `O(n)`。点从 `0` 编号。
> - **维护的量**：`degree`（各点当前度数）；`leaves`（编号最小的叶子小根堆）；`code`/`edges`（Pruefer 序列/边集）。
> - **警告**：编码与解码都是“每次取编号最小的叶子”；Pruefer 序列长度是 `n-2`，不是 `n-1`。完全图 `K_n` 的生成树数为 `n^(n-2)`；有 `k>=2` 个大小为 `s_i` 的连通块时，加 `k-1` 条边恰好连通的方案数是 `n^(k-2) * product(s_i)`。


**最小完整示例（先抄这一段就能跑）：**

题目：给 4 点星形树（中心 0 连 1、2、3）求 Pruefer 序列，再把序列还原成边集。

```cpp
vector<vector<int>> g = {{1, 2, 3}, {0}, {0}, {0}};  // 邻接表，点从 0 编号
vector<int> code = prufer_encode(g);                  // 1. 调用：编码，长度 n-2
auto edges = prufer_decode(code);                     // 2. 调用：解码回边集
```

样例：`prufer_encode(4 点星形树） -> {0, 0}`；`prufer_decode({0, 0}) -> 3 条边`。

**传参要求（照这个传不会错）：**

- `prufer_encode(g)`：`g` = 树的无向邻接表 `vector<vector<int>>`，点从 0 编号；`n <= 2` 返回空序列；返回长度 `n-2` 的 `vector<int>`。
- `prufer_decode(code)`：`code` = Pruefer 序列；点数自动取 `code.size()+2`；返回 `n-1` 条边的 `vector<pair<int,int>>`（顺序不定）。
- 编码/解码都要求输入是一棵树（无环连通）；复杂度 `O(n log n)`。


**API / 入口函数（赛时只认这里列的名字）：**

- `prufer_decode(const vector<int>& code)` -> 输入 Pruefer 序列，返回原树的边集。
- `prufer_encode(const vector<vector<int>>& g)` -> 输入一棵树的邻接表，返回其 Pruefer 序列（点从 0 编号）。
**改板时先认这几个量：**

- `parent`：编码时叶子唯一剩下的邻点（度仍大于 0 的那个）。
- `g`：邻接表。

典题模型：Luogu P6086 Pruefer 序列；凯莱公式；森林连通方案计数；随机生成带标号树。

```cpp
// 输入一棵树的邻接表，返回其 Pruefer 序列（点从 0 编号）。
vector<int> prufer_encode(const vector<vector<int>>& g) {
    const int n = (int)g.size();
    if (n <= 2) return {};
    vector<int> degree(n);
    priority_queue<int, vector<int>, greater<int>> leaves;
    for (int u = 0; u < n; ++u) {
        degree[u] = (int)g[u].size();
        if (degree[u] == 1) leaves.push(u);
    }

    vector<int> code;
    code.reserve(n - 2);
    for (int step = 0; step < n - 2; ++step) {
        int leaf = leaves.top(); leaves.pop();
        int parent = -1;
        for (int v : g[leaf]) if (degree[v] > 0) {
            parent = v;
            break;
        }
        code.push_back(parent);
        degree[leaf] = 0;
        if (--degree[parent] == 1) leaves.push(parent);
    }
    return code;
}

// 输入 Pruefer 序列，返回原树的边集。
vector<pair<int, int>> prufer_decode(const vector<int>& code) {
    const int n = (int)code.size() + 2;
    vector<int> degree(n, 1);
    for (int v : code) ++degree[v];

    priority_queue<int, vector<int>, greater<int>> leaves;
    for (int v = 0; v < n; ++v) if (degree[v] == 1) leaves.push(v);

    vector<pair<int, int>> edges;
    edges.reserve(n - 1);
    for (int v : code) {
        int leaf = leaves.top(); leaves.pop();
        edges.push_back({leaf, v});
        degree[leaf] = 0;
        if (--degree[v] == 1) leaves.push(v);
    }
    int a = leaves.top(); leaves.pop();
    int b = leaves.top();
    edges.push_back({a, b});
    return edges;
}
```
#### 扩展中国剩余定理 exCRT

> **赛时先看**
> - **题目信号**：多个周期同步、多个模数约束。
> - **本质**：解多个同余方程 `x = a_i (mod m_i)`，模数可不互质。
> - **接法**：把每个条件写成 `x ≡ a[i] (mod m[i])`，全部放进数组后调用 `excrt(a,m,ans,mod)`。返回 `false` 就无解；返回 `true` 时通解是 `x ≡ ans (mod mod)`，题目要最小正解时若 `ans==0` 通常输出 `mod`。
> - **复杂度判定**：`O(k log M)`。
> - **维护的量**：`ans`（已合并部分的同余剩余）、`mod`（已合并部分的模数 lcm）；每并入一个方程更新一次。
> - **警告**：无解条件是 `(a2-a1) % gcd(m1,m2) != 0`。合并时中间乘法可能超过 `i64`，用 `i128`。


**最小完整示例（先抄这一段就能跑）：**

题目：解同余组 `x ≡ 2 (mod 3)` 且 `x ≡ 3 (mod 5)`。

依赖：本册通用辅助函数 节的 exgcd，抄板时一起抄上。

```cpp
vector<i64> a = {2, 3}, m = {3, 5};
i64 ans, mod;
if (excrt(a, m, ans, mod)) {
    cout << ans << ' ' << mod << '\n';   // 通解 x ≡ ans (mod mod)
} else {
    cout << -1 << '\n';                  // 无解
}
```

样例：`excrt({2,3},{3,5}) -> ans=8, mod=15`（8 是最小非负解）。

**传参要求（照这个传不会错）：**

- `excrt(a, m, ans, mod)`：`a`/`m` 为等长 `vector<i64>`（第 i 个方程 `x ≡ a[i] (mod m[i])`，`m[i] >= 1`，不要求互质）。
- 返回 `bool`：`true` 时 `ans` 为最小非负解、`mod` 为合并后的模数（lcm），通解 `x ≡ ans (mod mod)`；`false` 无解。
- 求最小正整数解：若 `ans == 0` 输出 `mod`；合并过程内部乘法用 i128，不会溢出。


```cpp
bool crt_merge(i64 a1, i64 m1, i64 a2, i64 m2,
               i64& a, i64& m) {
    i64 x, y;
    i64 g = exgcd(m1, m2, x, y);
    if ((a2 - a1) % g != 0) return false;
    i64 mod = m2 / g;
    i64 t = (i128)((a2 - a1) / g) * x % mod;
    i64 lcm = m1 / g * m2;
    a = (a1 + (i128)m1 * t) % lcm;
    if (a < 0) a += lcm;
    m = lcm;
    return true;
}

bool excrt(const vector<i64>& a, const vector<i64>& m,
           i64& ans, i64& mod) {
    ans = 0;
    mod = 1;
    for (int i = 0; i < (int)a.size(); ++i) {
        if (!crt_merge(ans, mod, a[i], m[i], ans, mod)) return false;
    }
    return true;
}
```

#### 线性同余：模 2^64 最小解

> **赛时先看**
> - **题目信号**：64 位无符号整数自然溢出；模数是 `2^64`；要求最小 `x`。
> - **本质**：求 `a*x = b (mod 2^64)` 的最小非负解，或判断无解。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：常数级（约 6 轮 Newton 迭代，每轮正确位数翻倍）。
> - **维护的量**：`v`（`a` 中因子 2 的个数）、`odd`（约掉 2 后的奇数部分）、`res`（最小非负解）。
> - **警告**：`a=0` 单独处理；令 `g=lowbit(a)`，必须有 `g | b`；除掉 `g` 后，`a/g` 是奇数，可以在 `2^(64-v2(g))` 下求逆。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `3x ≡ 6 (mod 2^64)` 的最小非负解（ui64 全程自然溢出运算）。

```cpp
auto r = solve_linear_congruence_mod_2_64(3, 6);
if (r) cout << *r << '\n';      // 有解：输出最小非负解
else cout << "no solution\n";   // 无解
```

样例：`solve_linear_congruence_mod_2_64(3, 6) -> 2`；`solve_linear_congruence_mod_2_64(4, 3) -> 无解`。

**传参要求（照这个传不会错）：**

- `solve_linear_congruence_mod_2_64(a, b)`：`a`、`b` 为 `ui64`（无符号 64 位，模数固定为 2^64，不要传负数）。
- 返回 `optional<ui64>`：有解时为最小非负解（`[0, 2^64)` 内）；`nullopt` 无解。
- `a = 0` 时只有 `b = 0` 有解（返回 0）；一般情形先看 `lowbit(a) | b` 是否成立，不成立即无解。


```cpp
ui64 inverse_odd_mod_2_64(ui64 a) {
    // a 必须是奇数；Newton 迭代每轮让正确二进制位数翻倍。
    ui64 x = 1;
    for (int i = 0; i < 6; i++) x *= 2 - a * x;
    return x;
}

optional<ui64> solve_linear_congruence_mod_2_64(ui64 a, ui64 b) {
    if (a == 0) {
        if (b == 0) return 0;
        return nullopt;
    }
    int v = __builtin_ctzll(a);
    if (v > 0) {
        ui64 low_mask = (1ULL << v) - 1;
        if ((b & low_mask) != 0) return nullopt;
    }
    ui64 odd = a >> v;
    ui64 rhs = b >> v;
    ui64 res = inverse_odd_mod_2_64(odd) * rhs;
    int bits = 64 - v;
    if (bits < 64) res &= (1ULL << bits) - 1;
    return res; // 在 [0,2^64) 的所有解中取最小值。
}
```
#### AtCoder floor_sum

> **赛时先看**
> - **题目信号**：整除求和、格点计数、坐标压缩后的数学求和。
> - **本质**：计算 `sum_{i=0}^{n-1} floor((a*i+b)/m)`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(log max(a,m))`。
> - **维护的量**：`ans`（答案累加）；循环中维护 `n/m/a/b` 的对称化简（取模 + 交换）。
> - **警告**：这是 AtCoder Library 经典函数，参数可为非负整数。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `sum_{i=0}^{3} floor((2i+1)/3)`。

```cpp
i64 ans = floor_sum(4, 3, 2, 1);   // floor_sum(n, m, a, b)：i 取 0..n-1
cout << ans << '\n';
```

样例：`floor_sum(4, 3, 2, 1) -> 4`（0+1+1+2）。

**传参要求（照这个传不会错）：**

- `floor_sum(n, m, a, b)`：`n` = 项数（`i` 取 `0..n-1`）；`m` = 模数（≥1）；`a`、`b` = 一次式系数。
- 全部为 i64 **非负**整数；返回 `sum_{i=0}^{n-1} floor((a*i+b)/m)`（i64）。
- 复杂度 `O(log max(a,m))`；代码内部会先把 `a`、`b` 对 `m` 归约，不用手动处理。


```cpp
i64 floor_sum(i64 n, i64 m, i64 a, i64 b) {
    i64 ans = 0;
    while (true) {
        if (a >= m) {
            ans += (n - 1) * n * (a / m) / 2;
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }
        i64 y_max = a * n + b;
        if (y_max < m) break;
        n = y_max / m;
        b = y_max % m;
        swap(m, a);
    }
    return ans;
}
```
#### BSGS 离散对数

> **赛时先看**
> - **题目信号**：指数在模意义下作为未知量。
> - **本质**：求最小非负整数 `x` 使 `a^x = b (mod mod)`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(sqrt mod)`。
> - **维护的量**：`m`（`ceil(sqrt(mod))` 分块大小）；`table`（前段 `a^j` 出现的最小 j 哈希表）；`cur`（后段候选值，每次乘 `inv_factor` 前进）。
> - **警告**：基础版要求 `gcd(a,mod)=1`；非互质要用 exBSGS。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `2^x ≡ 7 (mod 13)` 的最小非负整数解。

依赖：本册通用辅助函数 节的 inv_general，抄板时一起抄上。

```cpp
i64 x = bsgs(2, 7, 13);   // 1. 调用：求 a^x ≡ b (mod mod) 的最小非负 x
cout << x << '\n';          // 2. 取结果：-1 表示无解
```

样例：`bsgs(2, 7, 13) -> 11`（2^11 = 2048 ≡ 7 mod 13）；`bsgs(2, 3, 13) -> 4`（2^4 = 16 ≡ 3 mod 13）。

**传参要求（照这个传不会错）：**

- `bsgs(a, b, mod)`：`a` = 底数、`b` = 目标值、`mod` = 模数，均为 i64，`mod >= 1`；内部先做 `a %= mod`、`b %= mod`。
- 返回最小非负整数 `x` 使 `a^x ≡ b (mod mod)`；无解返回 `-1`。
- 前提：必须 `gcd(a, mod) == 1`（后半段要求逆）；`mod == 1` 或 `b ≡ 1 (mod mod)` 时返回 0。
- 不互质时翻下一节 `exbsgs`；依赖 `mod_pow_bsgs` 与 `inv_general`，抄板时一起抄。


```cpp
i64 mod_pow_bsgs(i64 a, i64 b, i64 mod) {
    i64 res = 1 % mod;
    a %= mod;
    while (b) {
        if (b & 1) res = (i128)res * a % mod;
        a = (i128)a * a % mod;
        b >>= 1;
    }
    return res;
}

i64 bsgs(i64 a, i64 b, i64 mod) {
    a %= mod;
    b %= mod;
    if (mod == 1) return 0;
    if (b == 1 % mod) return 0;

    i64 m = (i64)ceil(sqrt((long double)mod));
    unordered_map<i64, i64> table;
    i64 e = 1;
    for (i64 j = 0; j < m; ++j) {
        if (!table.count(e)) table[e] = j;
        e = (i128)e * a % mod;
    }

    i64 factor = mod_pow_bsgs(a, m, mod);
    i64 inv_factor = inv_general(factor, mod);
    if (inv_factor == -1) return -1;

    i64 cur = b;
    for (i64 i = 0; i <= m; ++i) {
        if (table.count(cur)) {
            i64 x = i * m + table[cur];
            if (mod_pow_bsgs(a, x, mod) == b) return x;
        }
        cur = (i128)cur * inv_factor % mod;
    }
    return -1;
}
```
#### exBSGS

> **赛时先看**
> - **题目信号**：离散对数，模数不保证质数。
> - **本质**：求 `a^x = b (mod m)`，允许 `a` 与 `m` 不互质。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(sqrt m)`。
> - **维护的量**：`k`/`add`（约分阶段累积的系数与已加步数）；`table`（BSGS 前段哈希表）；`cur`（每次乘 `inv_factor` 前进的候选值）。
> - **警告**：先不断约掉 `gcd(a,m)`；约不掉后转普通 BSGS。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `2^x ≡ 4 (mod 6)` 的最小非负解（a 与模数不互质）。

依赖：本册通用辅助函数 节的 mod_pow / inv_general，抄板时一起抄上。

```cpp
i64 x = exbsgs(2, 4, 6);   // 1. 调用：自动先约 gcd(a,m) 再转 BSGS
cout << x << '\n';          // 2. 取结果：-1 表示无解
```

样例：`exbsgs(2, 4, 6) -> 2`（2^2=4）；`exbsgs(5, 3, 8) -> -1`（无解）。

**传参要求（照这个传不会错）：**

- `exbsgs(a, b, mod)`：`a`、`b`、`mod` 为 i64，`mod >= 1`；内部先 `a %= mod`。
- 返回最小非负整数 `x` 使 `a^x ≡ b (mod mod)`；无解返回 `-1`。
- `mod == 1 || b == 1` 时返回 0；`gcd(a, mod) == 1` 的场景也可直接用同节 `bsgs_coprime`。


**改板时先认这几个量：**

- `g`：每轮约分时的 `gcd(a, mod)`，`b % g != 0` 即无解。
- `cur`：BSGS 中每次乘 `inv_factor` 前进的当前候选值。

```cpp
i64 bsgs_coprime(i64 a, i64 b, i64 mod) {
    a %= mod; b %= mod;
    if (b == 1 % mod) return 0;
    i64 m = (i64)ceil(sqrt((long double)mod));
    unordered_map<i64, i64> table;
    i64 e = 1;
    for (i64 j = 0; j < m; ++j) {
        if (!table.count(e)) table[e] = j;
        e = (i128)e * a % mod;
    }
    i64 factor = mod_pow(a, m, mod);
    i64 inv_factor = inv_general(factor, mod);
    i64 cur = b;
    for (i64 i = 0; i <= m; ++i) {
        if (table.count(cur)) return i * m + table[cur];
        cur = (i128)cur * inv_factor % mod;
    }
    return -1;
}

i64 exbsgs(i64 a, i64 b, i64 mod) {
    a %= mod; b %= mod;
    if (mod == 1 || b == 1) return 0;
    i64 k = 1, add = 0, g;
    while ((g = gcd(a, mod)) > 1) {
        if (b % g) return -1;
        b /= g;
        mod /= g;
        k = (i128)k * (a / g) % mod;
        add++;
        if (k == b) return add;
    }
    i64 inv_k = inv_general(k, mod);
    i64 res = bsgs_coprime(a, (i128)b * inv_k % mod, mod);
    return res == -1 ? -1 : res + add;
}
```
#### 原根 Primitive Root

> **赛时先看**
> - **题目信号**：需要模质数的生成元。
> - **本质**：NTT、自定义质数模下求原根。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：试候选，通常很快。
> - **维护的量**：`factors`（`mod-1` 的不同质因子表）；`x`（试除后的剩余部分）；`g`（从小到大的候选原根）。
> - **警告**：只适用于质数模。


**最小完整示例（先抄这一段就能跑）：**

题目：NTT 前求质数 `998244353` 的原根。

依赖：本册通用辅助函数 节的 mod_pow，抄板时一起抄上。

```cpp
i64 g = primitive_root(998244353);   // 1. 调用：mod 传质数
cout << g << '\n';                     // 2. 取结果：该质数的最小原根
```

样例：`primitive_root(998244353) -> 3`（NTT 常用模数的原根）；`primitive_root(7) -> 3`（3^1..3^6 恰好覆盖 1..6）。

**传参要求（照这个传不会错）：**

- `primitive_root(mod)`：`mod` 必须是**质数**（i64，`mod >= 2`）。
- 返回最小原根 `g`（`2 <= g < mod`；`mod == 2` 时返回 1），满足 `g^1..g^(mod-1)` 恰好覆盖全部非零剩余。
- `mod` 为合数时本板不适用；依赖 `mod_pow`（快速幂），抄板时一起抄。


```cpp
i64 primitive_root(i64 mod) {
    if (mod == 2) return 1;
    vector<i64> factors;
    i64 x = mod - 1;
    for (i64 p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            factors.push_back(p);
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) factors.push_back(x);
    for (i64 g = 2; ; ++g) {
        bool ok = true;
        for (i64 p : factors) {
            if (mod_pow(g, (mod - 1) / p, mod) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
}
```
#### 二次剩余 Tonelli-Shanks

> **赛时先看**
> - **题目信号**：模意义开平方；多项式开方、几何模运算、数论构造。
> - **本质**：求 `x^2 = n (mod p)` 的解，`p` 为奇质数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(log^2 p)`。
> - **维护的量**：`q/s`（`p-1 = 2^s * q` 的拆分）；`z`（非二次剩余）、`c`、`x`（当前根）、`t`、`m`（迭代变量）。
> - **警告**：先用欧拉判别检查是否有解；`p % 4 == 3` 有快捷公式。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `x^2 ≡ 5 (mod 41)` 的一个根。

```cpp
i64 x = tonelli_shanks(5, 41);   // 1. 调用：求 x^2 ≡ n (mod p) 的一个解
cout << x << '\n';                 // 2. 取结果：-1 表示 n 不是二次剩余
```

样例：`tonelli_shanks(5, 41) -> 13`（13^2=169 ≡ 5 mod 41，另一个根是 28）。

**传参要求（照这个传不会错）：**

- `tonelli_shanks(n, p)`：`n` = 被开方数（i64，非负）；`p` 必须是**奇质数**（i64，`p != 2`）。
- 返回满足 `x^2 ≡ n (mod p)` 的一个解，另一个解是 `p-x`；`n` 非二次剩余返回 `-1`（欧拉判别已内置）。
- `n == 0` 返回 0；`p % 4 == 3` 走快捷公式 `n^((p+1)/4)`；`p` 为合数时本板不适用。


```cpp
i64 qpow_mod(i64 a, i64 e, i64 mod) {
    i64 r = 1 % mod;
    while (e) {
        if (e & 1) r = (i128)r * a % mod;
        a = (i128)a * a % mod;
        e >>= 1;
    }
    return r;
}

i64 tonelli_shanks(i64 n, i64 p) {
    n %= p;
    if (n == 0) return 0;
    if (p == 2) return n;
    if (qpow_mod(n, (p - 1) / 2, p) != 1) return -1;
    if (p % 4 == 3) return qpow_mod(n, (p + 1) / 4, p);
    i64 q = p - 1, s = 0;
    while ((q & 1) == 0) q >>= 1, s++;
    i64 z = 2;
    while (qpow_mod(z, (p - 1) / 2, p) != p - 1) z++;
    i64 c = qpow_mod(z, q, p);
    i64 x = qpow_mod(n, (q + 1) / 2, p);
    i64 t = qpow_mod(n, q, p);
    i64 m = s;
    while (t != 1) {
        i64 i = 1, tt = (i128)t * t % p;
        while (tt != 1) {
            tt = (i128)tt * tt % p;
            i++;
        }
        i64 b = qpow_mod(c, 1LL << (m - i - 1), p);
        x = (i128)x * b % p;
        c = (i128)b * b % p;
        t = (i128)t * c % p;
        m = i;
    }
    return x; // 另一个解是 p-x
}
```
#### Miller-Rabin 素性测试

> **赛时先看**
> - **题目信号**：`n` 可到 `1e18`，不能筛。
> - **本质**：判断 `i64` 范围内大数是否为质数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(log n * 常数）`。
> - **维护的量**：`d/s`（把 `n-1` 拆成 `2^s * d`）；`a`（固定测试底数表，覆盖全部 64 位整数）；`x`（`a^d mod n` 迭代平方的中间值）。
> - **警告**：固定测试底数可覆盖 64 位整数。


**最小完整示例（先抄这一段就能跑）：**

题目：判断大数 `n` 是否为质数（n 太大不能筛）。

```cpp
bool ok = is_prime_u64(1000000000000000009ULL);   // 1. 调用：n 传 ui64（无符号）
cout << (ok ? "prime" : "composite") << '\n';       // 2. 取结果：true = 质数
```

样例：`is_prime_u64(1000000007) -> true`（1e9+7 是质数）；`is_prime_u64(1000000008) -> false`（偶数）。

**传参要求（照这个传不会错）：**

- `is_prime_u64(n)`：`n` = 待判数（`ui64`，范围 0 ~ 2^64-1；负数不要传）。
- 返回 `bool`：true 表示质数；`n < 2` 返回 false。
- 固定 7 组测试底数即可覆盖全部 64 位整数，无需随机底数。
- 依赖 `mod_mul_u64`（u128 防溢出乘法）与 `mod_pow_u64`，抄板时一起抄；只从 `solve()` 调 `is_prime_u64`。


```cpp
using u128 = __uint128_t;

ui64 mod_mul_u64(ui64 a, ui64 b, ui64 mod) {
    return (u128)a * b % mod;
}

ui64 mod_pow_u64(ui64 a, ui64 b, ui64 mod) {
    ui64 res = 1;
    while (b) {
        if (b & 1) res = mod_mul_u64(res, a, mod);
        a = mod_mul_u64(a, a, mod);
        b >>= 1;
    }
    return res;
}

bool is_prime_u64(ui64 n) {
    if (n < 2) return false;
    for (ui64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) return n == p;
    }
    ui64 d = n - 1, s = 0;
    while ((d & 1) == 0) d >>= 1, s++;
    for (ui64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        ui64 x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (ui64 r = 1; r < s; ++r) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) {
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }
    return true;
}
```
#### Pollard-Rho 因式分解

> **赛时先看**
> - **题目信号**：`n <= 1e18`，需要质因数分解，试除太慢。
> - **本质**：分解 `i64` 范围大整数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：期望较快。
> - **维护的量**：`fac`（结果容器，递归填入质因子）；`rng`（随机源）；`d`（Floyd 判圈找到的非平凡因子）。
> - **警告**：随机算法，失败时换参数重试；先 Miller-Rabin 判断质数。


**最小完整示例（先抄这一段就能跑）：**

题目：分解 `n = 8051`（= 83 × 97）的全部质因子。

依赖：Miller-Rabin 素性测试 节的 is_prime_u64 / mod_mul_u64，抄板时一起抄上。

```cpp
vector<ui64> fac;
factor_u64(8051, fac);           // 1. 调用：质因子递归填入 fac（无序）
sort(fac.begin(), fac.end());    // 2. 按需排序后输出
for (ui64 p : fac) cout << p << ' ';
```

样例：`factor_u64(8051, fac) -> {83, 97}`。

**传参要求（照这个传不会错）：**

- `factor_u64(n, fac)`：`n` 为 `ui64`（质数、合数都行）；`fac` 传空 `vector<ui64>`，函数把质因子（含重复）追加进去，顺序不定。
- `pollard_rho(n)`：内部用，`n` 必须是合数（质数请先过 `is_prime_u64`）；返回一个非平凡因子。
- 依赖上节 Miller-Rabin 的 `is_prime_u64`、`mod_mul_u64`；`n` 为 1 时 `factor_u64` 直接返回。
- 期望复杂度亚指数级，`n <= 1e18` 最稳；极少数情况随机失败，多跑一次即可。


```cpp
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ui64 pollard_rho(ui64 n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    while (true) {
        ui64 c = uniform_int_distribution<ui64>(1, n - 1)(rng);
        ui64 x = uniform_int_distribution<ui64>(0, n - 1)(rng);
        ui64 y = x, d = 1;
        auto f = [&](ui64 v) { return (mod_mul_u64(v, v, n) + c) % n; };
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            ui64 diff = x > y ? x - y : y - x;
            d = gcd(diff, n);
        }
        if (d != n) return d;
    }
}

void factor_u64(ui64 n, vector<ui64>& fac) {
    if (n == 1) return;
    if (is_prime_u64(n)) {
        fac.push_back(n);
        return;
    }
    ui64 d = pollard_rho(n);
    factor_u64(d, fac);
    factor_u64(n / d, fac);
}
```

### 11 线性代数、多项式与生成函数

矩阵快速幂、线性方程、马尔可夫期望、行列式、插值、FFT/NTT/FWT、形式幂级数和线性递推放在这里。
#### 矩阵快速幂

> **赛时先看**
> - **题目信号**：题面出现"线性递推/状态转移重复 K 次"且 `K` 可达 `1e18`，如 Fibonacci 第 n 项、固定规则的转移计数、马尔可夫过程期望；每一步转移规则完全相同。看到"大次数幂 + 固定转移"就先想矩阵快速幂。
> - **本质**：把"走一步"写成一次矩阵乘法：状态向量 `state` 按列向量存，转移矩阵 `A` 满足 `state_next = A * state`；走 K 步就是乘 `A^K`，快速幂把 K 次乘法压成 O(log K) 次矩阵乘法。
> - **复杂度判定**：`O(k^3 log n)`，k 是状态数；k ≤ 100、n ≤ 1e18 时约 60 轮矩阵乘，稳；k 到 300+ 需考虑稀疏矩阵或线性递推优化（BM / 特征多项式）。
> - **维护的量**：矩阵元素 `a[i][j]`（0-based）；列向量约定 `state_next = A * state`；`mod` 模数。
> - **接法**：只要转移矩阵每一步都一样，且要跳很多步，就可以矩阵快速幂。先确定状态向量顺序，例如 Fibonacci 用 `[F_i, F_{i-1}]`；再写矩阵让它乘一次等于走一步；最后算 `mat_pow(transition, steps) * initial_vector`。
> - **警告**：初始向量顺序要和矩阵定义一致（列向量约定下是 `state_next = A * state`）；`A` 必须是方阵才能做 `mat_pow(A,k)`；指数 `k=0` 返回单位矩阵。


**最小完整示例（先抄这一段就能跑）：**

题目：求 Fibonacci 第 `n` 项 mod `1e9+7`（`n <= 1e18`）。

```cpp
i64 mod = 1000000007LL;             // 样例输入，抄题时换成你的输入（题目模数 1e9+7）
i64 n = 10;                         // 样例输入，抄题时换成你的输入（样例 n=10 -> 输出 55）
Matrix A(2, 2, mod);        // 1. 结构体定义：Matrix(行, 列, 模数)
A.a[0][0] = 1; A.a[0][1] = 1;   // 转移矩阵：state_{i+1} = A * state_i
A.a[1][0] = 1; A.a[1][1] = 0;   // state_i = [F_i; F_{i-1}]
Matrix P = mat_pow(A, n);   // 2. 调用：矩阵快速幂 A^n
cout << P.a[1][0] << '\n';   // 3. 取结果：F_n（设 F_1 = 1, F_0 = 0）
```

样例：`n = 10` -> 输出 `55`。

**传参要求（照这个传不会错）：**

- `Matrix(n, m, mod)`：构造全 0 矩阵；`mat_pow(A, k)` 要求 `A` 是方阵。
- 列向量约定：状态 `state_next = A * state`；写转移矩阵前先固定状态向量顺序。
- `A.a[i][j]`：0-based 下标访问元素；`k = 0` 返回单位矩阵。
- 模数任意，乘法内部用 `i64`（`mod <= 1e9` 时相乘不爆）。
- 转移"每步相同 + 步数巨大（1e18）"的递推/计数都能套这个思路。


**不会用就照抄：**

```cpp
Matrix A(...);
Matrix R = mat_pow(A, k);
```

- 这份 `Matrix` 是普通矩阵乘法，常用约定是 **列向量**：`state_next = A * state`。若你自己改成行向量，转移矩阵要整体转置思考。
- `A` 必须是方阵才能做 `mat_pow(A,k)`；指数 `k=0` 返回单位矩阵。


**API / 入口函数（赛时只认这里列的名字）：**

- `Matrix(n,m,mod)` -> 建立 n x m 零矩阵。
- `A.a[i][j]` -> 直接填写矩阵元素，内部下标 0-based。
- `mat_pow(A,k)` -> 求方阵 `A^k`；`k=0` 返回单位阵。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `Matrix` 结构体 + `mat_pow` 函数。
2. 构造：`Matrix A(n, n, mod);` 建 n 阶方阵；`Matrix init(n, 1, mod)` 建列向量初值。
3. 填矩阵：`A.a[i][j] = x;`，按"`state_next[i]` 由哪些 `state[j]` 贡献"来填。
4. 调用：`Matrix R = mat_pow(A, k) * init;`，答案在 `R.a[i][0]`。

**改造点（按题目改这几处）：**
- 模数：构造矩阵时传题面模数（非质数也能用，矩阵乘法只用加乘取模，不需要逆元）。
- 递推系数转矩阵：如 `F_i = c1*F_{i-1} + c2*F_{i-2}`，第一行填 `[c1, c2]`，其余行做"移位"（`a[i][i-1]=1`）。
- 加速递推的矩阵构造：状态含常数项、前缀和、多维 DP 时，把所有需要量都塞进状态向量，让每一步只是线性组合。
- "恰好 K 步"与"至多 K 步"：矩阵构造不同，后者常加吸收状态；带 min/max 的"最短路式"转移翻 C 章 min-plus 矩阵快速幂。
- 行向量习惯：若坚持行向量，转移矩阵整体转置思考，别和列向量混用。

**核心逻辑（改代码时别破坏）：**

- 固定线性转移写成矩阵 `A` 后，走 `k` 步就是乘 `A^k`。
- 快速幂逻辑与普通整数幂完全一样，只是乘法换成矩阵乘法。

**改板时先认这几个量：**

- `a[i][j]`：矩阵第 i 行第 j 列（0-based）；`n/m` 行列数；`mod` 取模数。
- 乘法内层跳过 `a[i][k]==0`，稀疏矩阵时更快。

```cpp
// 维护的量：a[i][j] 矩阵元素（0-based）；n/m 行列数；mod 模数。
// 约定：列向量 state_next = A * state；mat_pow(A,k) 返回 A^k，k=0 时为单位阵。
struct Matrix {
    int n, m;
    i64 mod;
    vector<vector<i64>> a;

    Matrix(int n_, int m_, i64 mod_, bool ident = false)
        : n(n_), m(m_), mod(mod_), a(n_, vector<i64>(m_, 0)) {
        if (ident) {
            // 单位阵：对角线置 1，作快速幂的初始结果
            for (int i = 0; i < min(n, m); ++i) a[i][i] = 1 % mod;
        }
    }

    static Matrix identity(int n, i64 mod) {
        return Matrix(n, n, mod, true);
    }

    Matrix operator*(const Matrix& o) const {
        Matrix res(n, o.m, mod);
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < m; ++k) if (a[i][k]) {
                for (int j = 0; j < o.m; ++j) {
                    // 第 i 行乘第 k 列累加进 (i,j)；i128 防中间乘爆
                    res.a[i][j] = (res.a[i][j] + (i128)a[i][k] * o.a[k][j]) % mod;
                }
            }
        }
        return res;
    }
};

Matrix mat_pow(Matrix base, i64 exp) {
    Matrix res = Matrix::identity(base.n, base.mod); // 结果初始化为单位阵
    while (exp) {
        if (exp & 1) res = res * base; // 二进制位为 1 时累乘进结果
        base = base * base; // base 自乘，对应指数翻倍
        exp >>= 1;
    }
    return res;
}
```
#### 有限状态转移矩阵快速幂

> **赛时先看**
> - **题目信号**：`n` 可达 `1e9` 或更大；转移只依赖有限个状态；每一步规则相同。
> - **本质**：状态数小、步数巨大时，用矩阵快速幂推进 DP 或自动机。
> - **复杂度判定**：`O(S^3 log n)`，`S` 是状态数。
> - **维护的量**：`MatVV` 即 `vector<vector<i64>>`；`a[i][j]` 表示从状态 `j` 一步转移到状态 `i`；三个函数只认列向量约定 `next = A * cur`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **警告**：矩阵 `a[i][j]` 表示从 `j` 转移到 `i`，这样 `next = A * cur`；不要把行列方向写反。


**最小完整示例（先抄这一段就能跑）：**

题目：求 Fibonacci 第 `n` 项（`F_1 = 1, F_0 = 0`）mod `1e9+7`（`n <= 1e18`）。

```cpp
MatVV A(2, vector<i64>(2, 0));
A[0][0] = 1; A[0][1] = 1;      // 转移：next[i] = Σ_j A[i][j] * cur[j]
A[1][0] = 1;                   // 状态 [F_i, F_{i-1}] -> [F_{i+1}, F_i]
vector<i64> cur = {1, 0};      // 初始向量：F_1 = 1, F_0 = 0
auto P = mat_pow_mod(A, 9, 1000000007LL); // 求 A^9
auto res = mat_apply_mod(P, cur, 1000000007LL); // A^9 * cur = [F_10, F_9]
cout << res[0] << '\n';        // 第 10 项
```

样例：`n = 10` -> 输出 `55`（mod 1e9+7 仍是 55）。

**传参要求（照这个传不会错）：**

- `A[i][j]`：转移矩阵元素，0-based；含义是"从状态 `j` 走一步到状态 `i`"，保证 `next = A * cur`（列向量）。
- `mat_mul_mod(a, b, mod)`：两个 `n` 阶方阵相乘，返回 `n x n` 结果；行列必须都是 `n`。
- `mat_pow_mod(a, e, mod)`：方阵 `a` 的 `e` 次幂（`e = 0` 返回单位阵），`e` 可达 `1e18`；`mod` 任意（非质数也能用）。
- `mat_apply_mod(a, v, mod)`：返回 `a * v`，`v` 是长度 `n` 的列向量，结果长度 `n`。
- 乘完记得 `mat_apply_mod(P, cur, mod)` 得到最终状态，答案按你定义的向量位置取。


```cpp
using MatVV = vector<vector<i64>>;

MatVV mat_mul_mod(const MatVV& a, const MatVV& b, i64 mod) {
    int n = (int)a.size();
    MatVV c(n, vector<i64>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            if (a[i][k] == 0) continue;
            for (int j = 0; j < n; j++) {
                c[i][j] = (c[i][j] + (i128)a[i][k] * b[k][j]) % mod;
            }
        }
    }
    return c;
}

MatVV mat_pow_mod(MatVV a, i64 e, i64 mod) {
    int n = (int)a.size();
    MatVV r(n, vector<i64>(n, 0));
    for (int i = 0; i < n; i++) r[i][i] = 1 % mod;
    while (e) {
        if (e & 1) r = mat_mul_mod(a, r, mod);
        a = mat_mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

vector<i64> mat_apply_mod(const MatVV& a, const vector<i64>& v, i64 mod) {
    int n = (int)a.size();
    vector<i64> res(n, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res[i] = (res[i] + (i128)a[i][j] * v[j]) % mod;
        }
    }
    return res;
}
```
#### 容斥原理：二进制枚举

> **赛时先看**
> - **题目信号**：多个限制条件，交集容易算，并集不好直接算。
> - **本质**：统计满足至少一个条件的对象数量。
> - **复杂度判定**：`O(2^m)`，`m` 是条件个数，一般 `m <= 20` 才枚举，更大要用容斥+其他技巧。
> - **维护的量**：`n`（计数范围右端点）；`p`（除数集合）；`ans`（最终并集大小）；mask 枚举时用 `bits` 判奇偶决定加还是减。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **警告**：奇数个集合加，偶数个集合减。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `1..n` 中能被 `2, 3, 5` 至少一个整除的个数。

```cpp
i64 n = 100;
vector<i64> p = {2, 3, 5};                 // 除数集合（不要求互质，内部用 lcm）
i64 ans = count_divisible_by_any(n, p);    // 容斥枚举 2^3 个子集
cout << ans << '\n';                       // 74
```

样例：`n = 100, p = {2,3,5}` -> 输出 `74`。

**传参要求（照这个传不会错）：**

- `n`：计数范围 `[1, n]` 的右端点，`n` 可达 `1e18`（返回 `i64`）。
- `p`：除数集合，`m = p.size() < 62`（`1LL << m` 不溢出）；内部按 `lcm(p[i] 的子集） <= n` 才计贡献，乘积溢出会跳过该子集。
- 返回值：`1..n` 中能被 `p` 中**至少一个**数整除的个数（整数，不取模）。
- 若 `p` 含重复数或非质数也能正确计算，不必预处理成质数。


```cpp
i64 count_divisible_by_any(i64 n, const vector<i64>& p) {
    int m = (int)p.size();
    assert(m < 62);
    i64 ans = 0;
    for (int mask = 1; mask < (1LL << m); ++mask) {
        i128 l = 1;
        int bits = 0;
        bool overflow = false;
        for (int i = 0; i < m; ++i) {
            if (mask >> i & 1) {
                bits++;
                l = l / gcd((i64)l, p[i]) * p[i];
                if (l > n) {
                    overflow = true;
                    break;
                }
            }
        }
        if (overflow) continue;
        if (bits & 1) ans += n / (i64)l;
        else ans -= n / (i64)l;
    }
    return ans;
}
```
#### 高斯消元：实数线性方程组

> **赛时先看**
> - **题目信号**：概率期望方程、电路、线性代数建模。
> - **本质**：解 `n` 个未知数的线性方程组。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：`O(n^3)`。
> - **维护的量**：`a`（n 行 n+1 列增广矩阵，传值拷贝可随便改）；`where[col]`（第 col 列主元所在行，`-1` 表示自由元）；`ans`（解向量）。
> - **警告**：浮点比较用 EPS；无解/无穷多解要按题意处理。


**最小完整示例（先抄这一段就能跑）：**

题目：解 `x + y = 3, 2x - y = 0`。

```cpp
vector<vector<double>> a = {{1, 1, 3}, {2, -1, 0}}; // 增广矩阵 [系数 | 常数]
vector<double> ans;                                   // 输出解向量
int ret = gauss(a, ans);                              // 0 无解 / 1 唯一解 / 2 无穷多解
cout << ret << ' ' << ans[0] << ' ' << ans[1] << '\n'; // 1 1 2
```

样例：上面方程组 -> 输出 `1 1 2`（唯一解 x=1, y=2）。

**传参要求（照这个传不会错）：**

- `a`：`n` 行 `n+1` 列增广矩阵，`a[i][n]` 是第 i 个方程的常数项；按值传递，原矩阵不会被改。
- `ans`：输出参数，长度 `n` 的解向量；没被选为主元的自由元位置填 `0`。
- 返回值：`0` 无解、`1` 唯一解、`2` 无穷多解（判断阈值内部用 `EPS = 1e-9` 与 `1e-7` 回代校验）。
- 解的精度：常规 `1e-6` 判题没问题；要更高精度改大 `EPS` 或换 long double 版本。


**API / 入口函数（赛时只认这里列的名字）：**

- `gauss(vector<vector<double>> a, vector<double>& ans)` -> a 是 n 行 n+1 列增广矩阵。返回 0 无解，1 唯一解，2 无穷多解。

```cpp
const double EPS = 1e-9;

// a 是 n 行 n+1 列增广矩阵。返回 0 无解，1 唯一解，2 无穷多解。
int gauss(vector<vector<double>> a, vector<double>& ans) {
    int n = (int)a.size();
    int m = (int)a[0].size() - 1;
    vector<int> where(m, -1);
    for (int col = 0, row = 0; col < m && row < n; ++col) {
        int sel = row;
        for (int i = row; i < n; ++i) {
            if (fabs(a[i][col]) > fabs(a[sel][col])) sel = i;
        }
        if (fabs(a[sel][col]) < EPS) continue;
        swap(a[sel], a[row]);
        where[col] = row;

        double div = a[row][col];
        for (int j = col; j <= m; ++j) a[row][j] /= div;
        for (int i = 0; i < n; ++i) {
            if (i == row) continue;
            double factor = a[i][col];
            for (int j = col; j <= m; ++j) a[i][j] -= factor * a[row][j];
        }
        row++;
    }

    ans.assign(m, 0);
    for (int i = 0; i < m; ++i) {
        if (where[i] != -1) ans[i] = a[where[i]][m];
    }
    for (int i = 0; i < n; ++i) {
        double sum = 0;
        for (int j = 0; j < m; ++j) sum += ans[j] * a[i][j];
        if (fabs(sum - a[i][m]) > 1e-7) return 0;
    }
    for (int i = 0; i < m; ++i) if (where[i] == -1) return 2;
    return 1;
}
```
#### 单纯形法：线性规划最大化

> **赛时先看**
> - **题目信号**：变量是实数；约束和目标都是线性的；题面出现资源分配、连续配比、混合策略、最大/最小线性收益。若变量必须是整数，单纯形不能直接解决整数规划。
> - **本质**：求 `max c^T x`，满足 `A x <= b, x >= 0` 的连续线性规划；可区分最优、无界、无可行解。
> - **接法**：饮料配方/资源约束下最大收益；零和博弈的混合策略；线性不等式下的最大面积或最大价值。
> - **复杂度判定**：单纯形理论最坏指数级，但竞赛随机/普通数据通常很快；变量、约束在数百规模较常见。
> - **维护的量**：`D`（`(m+2) x (n+2)` 单纯形表）；`B/N`（基变量/非基变量编号）；`m/n`（约束数/变量数）。
> - **警告**：返回 `INF` 表示无界，返回 `-INF` 表示无可行解；浮点比较用 `EPS`。最小化可对目标系数取反，再把答案取反。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `max x + y`，约束 `2x + y <= 10, x + 2y <= 8, x, y >= 0`。

```cpp
vector<vector<long double>> A = {{2, 1}, {1, 2}}; // 约束矩阵 A（m 行 n 列）
vector<long double> b = {10, 8};                   // 约束右端 A*x <= b
vector<long double> c = {1, 1};                    // 目标系数，最大化 c*x
vector<long double> x;                             // 输出最优解
LPSolver lp(A, b, c);
long double ans = lp.solve(x);                     // 最优值；x[0], x[1] 是最优点
cout << fixed << setprecision(6) << ans << '\n';   // 6.000000
```

样例：上面线性规划 -> 最优值 `6`（x=4, y=2）。

**传参要求（照这个传不会错）：**

- `A`：`m x n` 约束矩阵；`b`：长度 `m` 的右端向量；`c`：长度 `n` 的目标系数。三者都必须是非负/任意实数（内部自动处理）。
- `solve(x)`：执行主算法；返回最大目标值；`x` 被填成最优解（长度 `n`）。
- 返回值 `INF`（`1e100`）说明目标无界；`-INF` 说明无可行解。
- 约束形式固定为 `A*x <= b, x >= 0`；要 `>=` 就两边乘 `-1`；要最小化就把 `c` 取反、答案取反。


**API / 入口函数（赛时只认这里列的名字）：**

- `solve(vector<long double>& x)` -> 执行主算法并返回答案

典题模型：饮料配方/资源约束下最大收益；零和博弈的混合策略；线性不等式下的最大面积或最大价值。

```cpp
struct LPSolver {
    static constexpr long double EPS = 1e-12L;
    static constexpr long double INF = 1e100L;
    int m, n;
    vector<int> B, N;
    vector<vector<long double>> D;

    // A 是 m*n，约束 A*x <= b；最大化 c*x。
    LPSolver(const vector<vector<long double>>& A, const vector<long double>& b,
             const vector<long double>& c)
        : m((int)b.size()), n((int)c.size()), B(m), N(n + 1), D(m + 2, vector<long double>(n + 2)) {
        for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) D[i][j] = A[i][j];
        for (int i = 0; i < m; ++i) {
            B[i] = n + i;
            D[i][n] = -1;
            D[i][n + 1] = b[i];
        }
        for (int j = 0; j < n; ++j) {
            N[j] = j;
            D[m][j] = -c[j];
        }
        N[n] = -1;
        D[m + 1][n] = 1;
    }

    void pivot(int r, int s) {
        long double inv = 1 / D[r][s];
        for (int i = 0; i < m + 2; ++i) if (i != r) {
            for (int j = 0; j < n + 2; ++j) if (j != s) D[i][j] -= D[r][j] * D[i][s] * inv;
        }
        for (int j = 0; j < n + 2; ++j) if (j != s) D[r][j] *= inv;
        for (int i = 0; i < m + 2; ++i) if (i != r) D[i][s] *= -inv;
        D[r][s] = inv;
        swap(B[r], N[s]);
    }

    bool simplex(int phase) {
        int x = phase == 1 ? m + 1 : m;
        while (true) {
            int s = -1;
            for (int j = 0; j <= n; ++j) {
                if (phase == 2 && N[j] == -1) continue;
                if (s == -1 || D[x][j] < D[x][s] - EPS ||
                    (fabsl(D[x][j] - D[x][s]) <= EPS && N[j] < N[s])) s = j;
            }
            if (D[x][s] >= -EPS) return true;
            int r = -1;
            for (int i = 0; i < m; ++i) if (D[i][s] > EPS) {
                if (r == -1) r = i;
                else {
                    long double lhs = D[i][n + 1] / D[i][s];
                    long double rhs = D[r][n + 1] / D[r][s];
                    if (lhs < rhs - EPS || (fabsl(lhs - rhs) <= EPS && B[i] < B[r])) r = i;
                }
            }
            if (r == -1) return false;
            pivot(r, s);
        }
    }

    long double solve(vector<long double>& x) {
        int r = 0;
        for (int i = 1; i < m; ++i) if (D[i][n + 1] < D[r][n + 1]) r = i;
        if (D[r][n + 1] < -EPS) {
            pivot(r, n);
            if (!simplex(1) || D[m + 1][n + 1] < -EPS) return -INF; // 无可行解
            if (fabsl(D[m + 1][n + 1]) > EPS) return -INF;
            for (int i = 0; i < m; ++i) if (B[i] == -1) {
                int s = 0;
                for (int j = 1; j <= n; ++j) {
                    if (D[i][j] < D[i][s] - EPS ||
                        (fabsl(D[i][j] - D[i][s]) <= EPS && N[j] < N[s])) s = j;
                }
                pivot(i, s);
            }
        }
        if (!simplex(2)) return INF; // 无界
        x.assign(n, 0);
        for (int i = 0; i < m; ++i) if (B[i] < n) x[B[i]] = D[i][n + 1];
        return D[m][n + 1];
    }
};
```
#### GF(2) 高斯消元与异或方程组

> **赛时先看**
> - **题目信号**：约束是若干变量 xor 后等于 0/1；图的度数奇偶、开关灯、异或构造。
> - **本质**：解异或方程组、求秩、判断是否有解、计算自由元数量。
> - **复杂度判定**：`O(n * m^2 / word)` 用 bitset 常数较好；下方模板为 `O(rows * vars^2 / 64)` 级别。
> - **维护的量**：`a[i]`（第 i 行 bitset，`[0,nvar)` 存系数，`a[i][nvar]` 存常数）；`row`（已消出的主元行数 = 秩）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **警告**：增广列放在 `nvar`；出现全 0 系数但常数为 1 即无解。


**最小完整示例（先抄这一段就能跑）：**

题目：判断异或方程组 `x0^x2=1, x0^x1^x2=0, x1=1` 的秩与解数。

```cpp
const int MAXV = 10; // 变量个数上界，模板参数
vector<bitset<MAXV + 1>> a;
a.push_back(bitset<MAXV + 1>(0b1101)); // 二进制串右起第 i 位是 x_i，第 nvar 位是常数
a.push_back(bitset<MAXV + 1>(0b0111)); // x0^x1^x2 = 0
a.push_back(bitset<MAXV + 1>(0b1010)); // x1 = 1
int rank = gauss_xor<MAXV>(a, 3);      // 3 个变量；返回秩，-1 表示无解
cout << rank << '\n';                  // 秩 = 2，自由元 3-2 = 1 个
```

样例：上面方程组 -> 输出 `2`（有解，解数 `2^1 = 2`）。

**传参要求（照这个传不会错）：**

- `MAXV`：模板参数，变量个数的上界；每个方程用 `bitset<MAXV + 1>`，多出的 1 位存常数。
- `a`：行数 = 方程数；`a[i][j]`（`0 <= j < nvar`）是 `x_j` 的系数（0/1），`a[i][nvar]` 是方程右端常数。
- `nvar`：实际变量个数，`<= MAXV`；注意 `nvar` 同时也是增广列下标。
- 返回值：方程组有解时返回秩 `rank`，解的数量是 `2^(nvar - rank)`；无解返回 `-1`。
- 消元后主元行 `a[i][nvar]` 可直接读出对应变量的值。


```cpp
template <int MAXV>
int gauss_xor(vector<bitset<MAXV + 1>>& a, int nvar) {
    int n = (int)a.size();
    int row = 0;
    for (int col = 0; col < nvar && row < n; col++) {
        int pivot = -1;
        for (int i = row; i < n; i++) {
            if (a[i][col]) {
                pivot = i;
                break;
            }
        }
        if (pivot == -1) continue;
        swap(a[row], a[pivot]);
        for (int i = 0; i < n; i++) {
            if (i != row && a[i][col]) a[i] ^= a[row];
        }
        row++;
    }
    for (int i = row; i < n; i++) {
        bool all_zero = true;
        for (int j = 0; j < nvar; j++) {
            if (a[i][j]) {
                all_zero = false;
                break;
            }
        }
        if (all_zero && a[i][nvar]) return -1; // 无解。
    }
    return row; // rank 为秩，解的数量为 2^(nvar-rank)。
}
```
#### 模意义高斯消元：唯一解

> **赛时先看**
> - **题目信号**：答案是有理数取模；方程形如 `x_i = c + sum p_ij x_j`；模数是质数。
> - **本质**：模质数意义下解线性方程组，常用于期望、概率、计数线性方程。
> - **复杂度判定**：`O(n^3)`。
> - **维护的量**：`a`（n 行 n+1 列增广矩阵）；`where[col]`（第 col 列主元所在行，`-1` 为自由元）；`ans`（解向量）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **警告**：只有主元非零才能求逆；若题目保证唯一解可以直接返回，否则要额外处理无解/多解。


**最小完整示例（先抄这一段就能跑）：**

题目：模 `5` 下解 `2x + y = 3, x + y = 2`。

```cpp
vector<vector<i64>> a = {{2, 1, 3}, {1, 1, 2}}; // n 行 n+1 列增广矩阵
vector<i64> ans = gauss_mod_unique(a, 5);       // mod 必须是质数
cout << ans[0] << ' ' << ans[1] << '\n';        // 1 1
```

样例：上面方程组 -> 输出 `1 1`（x=1, y=1 代入两个方程均成立）。

**传参要求（照这个传不会错）：**

- `a`：`n` 行 `n+1` 列增广矩阵，`a[i][n]` 是第 i 个方程的常数项；值可以为负（内部自动转正取模）。
- `mod`：质数模数（内部用费马小定理求 `mod-2` 次幂当逆元，合数模数不能用本函数）。
- 返回值：长度 `n` 的解向量 `ans`，全部在 `[0, mod)`；本函数假设唯一解，自由元位置填 `0`。
- 配套的 `mod_pow_prime(a, e, mod)` 是内部快速幂，普通题目不需要直接调用。


```cpp
i64 mod_pow_prime(i64 a, i64 e, i64 mod) {
    i64 r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) r = (i128)r * a % mod;
        a = (i128)a * a % mod;
        e >>= 1;
    }
    return r;
}

vector<i64> gauss_mod_unique(vector<vector<i64>> a, i64 mod) {
    int n = (int)a.size();
    vector<int> where(n, -1);
    int row = 0;
    for (int col = 0; col < n && row < n; col++) {
        int pivot = -1;
        for (int i = row; i < n; i++) {
            if ((a[i][col] % mod + mod) % mod != 0) {
                pivot = i;
                break;
            }
        }
        if (pivot == -1) continue;
        swap(a[row], a[pivot]);
        where[col] = row;

        i64 inv = mod_pow_prime((a[row][col] % mod + mod) % mod, mod - 2, mod);
        for (int j = col; j <= n; j++) a[row][j] = a[row][j] * inv % mod;
        for (int i = 0; i < n; i++) {
            if (i == row) continue;
            i64 factor = a[i][col] % mod;
            if (factor < 0) factor += mod;
            if (factor == 0) continue;
            for (int j = col; j <= n; j++) {
                a[i][j] = (a[i][j] - factor * a[row][j]) % mod;
                if (a[i][j] < 0) a[i][j] += mod;
            }
        }
        row++;
    }

    vector<i64> ans(n, 0);
    for (int col = 0; col < n; col++) {
        if (where[col] != -1) ans[col] = a[where[col]][n] % mod;
    }
    return ans;
}
```
#### 吸收马尔可夫链期望方程

> **赛时先看**
> - **题目信号**：每轮随机转移；问期望多少轮结束；状态数可控；样例解释里有 `E = 1 + pE`。
> - **本质**：有限状态随机过程，求到达吸收态的期望轮数。
> - **复杂度判定**：建方程 `O(S^2)`，消元 `O(S^3)`。
> - **维护的量**：`trans[i]`（状态 i 到各非吸收状态的转移 `{to, 概率}`）；`a`（`E[i] = 1 + Σ p_ij * E[j]` 的增广矩阵）。
> - **警告**：只把非吸收状态编号进方程；转移到吸收态不需要加变量。依赖上一节 `gauss_mod_unique` 解方程。


**最小完整示例（先抄这一段就能跑）：**

题目：两状态赌博，状态 0 以 1/2 结束、1/2 去状态 1；状态 1 以 1/2 去状态 0、1/2 结束。求从各状态出发的期望轮数。

依赖：模意义高斯消元：唯一解 节的 gauss_mod_unique，抄板时一起抄上。

```cpp
vector<vector<pair<int, i64>>> trans(2);
trans[0].push_back({1, 499122177}); // 状态 0 -> 状态 1，概率 1/2（模 998244353）
trans[1].push_back({0, 499122177}); // 状态 1 -> 状态 0，概率 1/2
// 转向"结束/吸收态"的概率一律不写入 trans
vector<i64> E = absorbing_expectation(trans); // E[i] = 从状态 i 出发的期望轮数
cout << E[0] << ' ' << E[1] << '\n';          // 2 2
```

样例：上面转移 -> 输出 `2 2`（两状态期望都等于 2）。

**传参要求（照这个传不会错）：**

- `trans`：长度 `s` = 非吸收状态个数；`trans[i]` 的元素是 `{to, p}`，`p` 为模 `998244353` 下的转移概率。
- 转移到吸收态（游戏结束）的边**不要写入** `trans[i]`；本模板每轮自动 `+1` 计期望轮数。
- 返回值：长度 `s` 的 `E[i]`，即从状态 i 出发到吸收的期望轮数（模 `998244353`）。
- 依赖上一节 `gauss_mod_unique` 与本节开头常量 `EXPECT_MOD`；模数不可自行更换。


```cpp
const i64 EXPECT_MOD = 998244353;

// trans[i] = {to, prob_mod}，i/to 都是非吸收状态编号。
// 若有概率转向吸收态，直接不写入 trans[i]。
vector<i64> absorbing_expectation(
    const vector<vector<pair<int, i64>>>& trans
) {
    int s = (int)trans.size();
    vector<vector<i64>> a(s, vector<i64>(s + 1, 0));
    for (int i = 0; i < s; i++) {
        a[i][i] = 1;
        a[i][s] = 1;
        for (auto [to, p] : trans[i]) {
            a[i][to] = (a[i][to] - p) % EXPECT_MOD;
            if (a[i][to] < 0) a[i][to] += EXPECT_MOD;
        }
    }
    return gauss_mod_unique(a, EXPECT_MOD);
}
```

#### 典题：标号图度数奇偶计数

> **赛时先看**
> - **题目信号**：图的边任意选，条件只限制每个点度数的奇偶；答案对任意 `mod` 取模。
> - **本质**：计算 `n` 个点的简单标号无向图中，所有点度数奇偶性全部相同的图数量。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(log n)`。
> - **维护的量**：`edges`（完全图边数 `n(n-1)/2`）；`exponent`（`edges-(n-1)`，GF(2) 秩差）；`even_degree_graphs`（全偶度数图数量）。
> - **警告**：完全图关联矩阵在 `GF(2)` 下秩为 `n-1`；全奇度数只有 `n` 为偶数时可行。


**最小完整示例（先抄这一段就能跑）：**

题目：`n=4` 个点的标号无向图，所有点度数奇偶性相同的图数 mod `1e9+7`。

```cpp
i64 ans = count_same_parity_degree_graph(4, 1000000007);   // 1. 调用：点数 n、模数 mod
cout << ans << '\n';                                        // 2. 取结果：mod 后答案
```

样例：`count_same_parity_degree_graph(4, 1000000007) -> 16`（全偶 8 + 全奇 8）；`count_same_parity_degree_graph(3, 1000000007) -> 2`（空图与三角形，全奇不可能）。

**传参要求（照这个传不会错）：**

- `count_same_parity_degree_graph(n, mod)`：`n` = 点数（i64，`n >= 1`）；`mod` = 任意模数（i64，合数也行）。
- 返回 n 个点的简单标号无向图中，所有点度数奇偶性相同（全偶或全奇）的图数 `mod mod`。
- `mod == 1` 返回 0；`n` 为奇数时全奇不可能，答案只含全偶（乘 2 的分支跳过）。
- 原理：完全图关联矩阵 GF(2) 下秩 `n-1`，全偶图数 = `2^(edges-(n-1))`，n 为偶数时全奇图数同样多。


```cpp
i64 pow_mod_any(i64 a, i64 e, i64 mod) {
    i64 r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) r = (i128)r * a % mod;
        a = (i128)a * a % mod;
        e >>= 1;
    }
    return r;
}

i64 count_same_parity_degree_graph(i64 n, i64 mod) {
    if (mod == 1) return 0;
    i64 edges = n * (n - 1) / 2;
    i64 exponent = edges - (n - 1);
    i64 even_degree_graphs = pow_mod_any(2, exponent, mod);
    if (n % 2 == 0) return even_degree_graphs * 2 % mod; // 全偶或全奇。
    return even_degree_graphs;                            // 全奇不可能。
}
```
#### 行列式取模：任意模数版

> **赛时先看**
> - **题目信号**：要求矩阵行列式，模数不保证质数。
> - **本质**：Matrix-Tree、线性代数计数、格点相关计数。
> - **复杂度判定**：`O(n^3 log V)`，`V` 是元素值域。
> - **维护的量**：`a`（n x n 方阵，传值拷贝内部随便消）；`ans`（行列式值，含行交换符号，最后转正）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **警告**：欧几里得消元过程中要处理行交换符号；最后答案转正。


**最小完整示例（先抄这一段就能跑）：**

题目：求 `det([[1,2],[3,4]]) mod 1e9+7`（模数不是质数也照样算）。

```cpp
vector<vector<i64>> a = {{1, 2}, {3, 4}}; // n x n 方阵
i64 det = determinant_any_mod(a, 1000000007LL); // 任意模数，无需逆元
cout << det << '\n';                       // 1*4-2*3 = -2 ≡ 1000000005
```

样例：上面矩阵 -> 输出 `1000000005`（即 `-2 mod 1e9+7`）。

**传参要求（照这个传不会错）：**

- `a`：`n x n` 方阵，元素任意整数均可（内部自动取模）；按值传递，原矩阵不被改。
- `mod`：任意模数（**不要求质数**，全程只用加减乘和整除，不需要逆元）。
- 返回值：`det(a) mod mod`，已转正落在 `[0, mod)`；消元中若主元变 0 直接返回 0。
- 复杂度 `O(n^3 log V)`：n 到几百可用；Matrix-Tree 定理、格点计数都能套。


```cpp
i64 determinant_any_mod(vector<vector<i64>> a, i64 mod) {
    int n = (int)a.size();
    i64 ans = 1;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            while (a[j][i]) {
                i64 t = a[i][i] / a[j][i];
                for (int k = i; k < n; k++) {
                    a[i][k] = (a[i][k] - t * a[j][k]) % mod;
                    swap(a[i][k], a[j][k]);
                }
                ans = -ans;
            }
        }
        if (a[i][i] == 0) return 0;
        ans = ans * a[i][i] % mod;
    }
    return (ans % mod + mod) % mod;
}
```
#### 康托展开

> **赛时先看**
> - **题目信号**：问一个排列是第几小，或第 k 个排列。
> - **复杂度判定**：朴素 `O(n^2)`，树状数组可优化到 `O(n log n)`。
> - **维护的量**：`fac[i]`（阶乘，未取模）；`used[x]`（数字 x 是否已用）；`rank`（累加的 0-based 排名）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **警告**：排名通常有 0-based 和 1-based 两种，按题意调整。阶乘未取模，`n>=21` 时 `i64` 溢出；需要取模请自行对阶乘取模。


**最小完整示例（先抄这一段就能跑）：**

题目：求排列 `[2,1,3]` 在 `1..n` 全排列中的排名（0-based）。

```cpp
vector<int> p = {2, 1, 3};              // 必须是 1..n 的排列（元素从 1 开始）
i64 rank = cantor_rank_zero_based(p);   // 0-based 排名
cout << rank << '\n';                   // [1,2,3] 是 0，[2,1,3] 是 2
```

样例：`p = [2,1,3]` -> 输出 `2`（全排列字典序：`[1,2,3]=0, [1,3,2]=1, [2,1,3]=2`）。

**传参要求（照这个传不会错）：**

- `p`：一个 `1..n` 的排列，长度 `n`，元素范围必须是 `1..n`（代码用 `used[x]` 查 1..n）。
- 返回值：0-based 排名，即比它小的排列个数；题目要 1-based 排名就 `+1`。
- 复杂度朴素 `O(n^2)`，`n` 到 1e5 用树状数组优化；`n >= 21` 阶乘溢出 `i64`（要取模得自己把 `fac` 改成取模）。
- 逆操作（第 k 个排列）本节没有，可拿 BIT + 二分现写。


```cpp
i64 cantor_rank_zero_based(const vector<int>& p) {
    int n = (int)p.size();
    vector<i64> fac(n + 1, 1);
    for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i;

    i64 rank = 0;
    vector<int> used(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        int smaller_unused = 0;
        for (int x = 1; x < p[i]; ++x) if (!used[x]) smaller_unused++;
        rank += smaller_unused * fac[n - 1 - i];
        used[p[i]] = 1;
    }
    return rank;
}
```
#### Lagrange 插值：连续点 0..n-1

> **赛时先看**
> - **题目信号**：答案是低次多项式，给出前若干项，求第 `k` 项。
> - **本质**：已知多项式在 `0..n-1` 的值，求 `f(x)`。
> - **复杂度判定**：`O(n)`（预处理前缀/后缀积 + 阶乘逆元后一次线性求和）。
> - **维护的量**：`pre/suf`（`∏(x-i)` 的前缀/后缀积）；`fac/ifac`（阶乘与逆元）；`ans`（逐项累加的插值结果）。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **警告**：模数要为质数；`x < n` 时直接返回。


**最小完整示例（先抄这一段就能跑）：**

题目：已知 `f(x) = x^2` 在 `0..3` 的值，求 `f(5) mod 1e9+7`。

依赖：本册通用辅助函数 节的 mod_pow，抄板时一起抄上。

```cpp
vector<i64> y = {0, 1, 4, 9};                    // f(0), f(1), f(2), f(3)
i64 val = lagrange_consecutive(y, 5, 1000000007LL); // 求 f(5)，x 可到 1e18
cout << val << '\n';                             // 25
```

样例：`y = {0,1,4,9}` 求 `f(5)` -> 输出 `25`。

**传参要求（照这个传不会错）：**

- `y`：长度 `n`，依次是 `f(0), f(1), ..., f(n-1)` 的值（`n` 为插值点数，通常取"次数 + 1"）。
- `x`：要求值的点，`i64`，可达 `1e18`；若 `x < n` 直接返回 `y[x]`。
- `mod`：质数模数（内部对阶乘求逆元，合数模数不可用）。
- 返回值：`f(x) mod mod`，在 `[0, mod)`。
- 依赖前置函数 `mod_pow`（第一节已有）；`n` 到 1e6 可用。


```cpp
i64 lagrange_consecutive(const vector<i64>& y, i64 x, i64 mod) {
    int n = (int)y.size();
    if (x < n) return y[(int)x] % mod;
    vector<i64> pre(n + 1, 1), suf(n + 1, 1), fac(n + 1, 1), ifac(n + 1, 1);
    for (int i = 0; i < n; ++i) pre[i + 1] = pre[i] * ((x - i) % mod + mod) % mod;
    for (int i = n - 1; i >= 0; --i) suf[i] = suf[i + 1] * ((x - i) % mod + mod) % mod;
    for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i % mod;
    ifac[n] = mod_pow(fac[n], mod - 2, mod);
    for (int i = n; i >= 1; --i) ifac[i - 1] = ifac[i] * i % mod;
    i64 ans = 0;
    for (int i = 0; i < n; ++i) {
        i64 num = pre[i] * suf[i + 1] % mod;
        i64 den = ifac[i] * ifac[n - 1 - i] % mod;
        if ((n - 1 - i) & 1) den = (mod - den) % mod;
        ans = (ans + y[i] % mod * num % mod * den) % mod;
    }
    return ans;
}
```
#### FFT：任意整数卷积

> **赛时先看**
> - **题目信号**：`c[k] = sum a[i] * b[k-i]`，模数不是 NTT 友好质数，或题目要求普通整数答案。
> - **本质**：两个整数系数多项式卷积。NTT 受模数限制；当题目需要实数近似或普通整数卷积时可使用 FFT。
> - **复杂度判定**：`O(n log n)`，`n` 为补到 2 的幂的长度。
> - **维护的量**：`fa/fb`（补零到 2 的幂后的系数数组，`fa` 复用为结果）；`c`（llround 取整后的整数卷积）。
> - **接法**：多项式乘法、字符串匹配的卷积化、骰子点数分布合并。
> - **警告**：系数和长度过大时浮点误差会累积；模意义卷积优先用 NTT，任意模数再考虑三模 NTT + CRT。


**最小完整示例（先抄这一段就能跑）：**

题目：计算整数卷积 `[1,2,3] * [4,5]`。

```cpp
vector<i64> a = {1, 2, 3}, b = {4, 5};
vector<i64> c = convolution_fft(a, b); // c[k] = Σ a[i] * b[k-i]
for (i64 v : c) cout << v << ' ';      // 4 13 22 15
cout << '\n';
```

样例：`[1,2,3] * [4,5]` -> 输出 `4 13 22 15`。

**传参要求（照这个传不会错）：**

- `a, b`：长度 `n, m` 的整数系数数组，可为负数；空数组返回空结果。
- 返回值：长度 `n + m - 1` 的 `vector<i64>`，`c[k] = Σ a[i]*b[k-i]`（llround 取整，不做任何取模）。
- 精度：建议系数绝对值之和在 `1e15` 以内（double 53 位有效数字）；超了换三模 NTT + CRT。
- `fft(a, invert)` 是底层函数，普通题目只调 `convolution_fft`。


**改板时先认这几个量：**

- `fa`/`fb`：输入系数补到 2 的幂长度后的数组（`fa` 复用为结果）。
- `bit`：位逆序置换的局部变量。

典题模型：多项式乘法、字符串匹配的卷积化、骰子点数分布合并。

```cpp
using cd = complex<double>;
const double PI = acos(-1.0);

void fft(vector<cd>& a, bool invert) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) for (cd& x : a) x /= n;
}

vector<i64> convolution_fft(const vector<i64>& a, const vector<i64>& b) {
    if (a.empty() || b.empty()) return {};
    int need = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < need) n <<= 1;
    vector<cd> fa(n), fb(n);
    for (int i = 0; i < (int)a.size(); ++i) fa[i] = (double)a[i];
    for (int i = 0; i < (int)b.size(); ++i) fb[i] = (double)b[i];
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; ++i) fa[i] *= fb[i];
    fft(fa, true);
    vector<i64> c(need);
    for (int i = 0; i < need; ++i) c[i] = llround(fa[i].real());
    return c;
}
```
#### NTT 卷积

> **赛时先看**
> - **题目信号**：`c[k] = sum a[i] * b[k-i]`，`O(nm)` 太慢。
> - **本质**：快速多项式乘法、卷积、生成函数基础。
> - **复杂度判定**：`O(n log n)`，`n` 为补到 2 的幂的长度，上限 `2^23`。
> - **维护的量**：`a/b`（补零后的系数数组，逐点相乘后 `a` 复用为结果）；`NTT_MOD = 998244353`、`NTT_G = 3`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **警告**：这里模数固定 `998244353`，原根 `3`。


**最小完整示例（先抄这一段就能跑）：**

题目：计算模 `998244353` 下的卷积 `[1,2,3] * [4,5]`。

```cpp
vector<int> a = {1, 2, 3}, b = {4, 5}; // 系数先规范到 [0, 998244353)
vector<int> c = convolution(a, b);     // c[k] = Σ a[i]*b[k-i] (mod 998244353)
for (int v : c) cout << v << ' ';      // 4 13 22 15
cout << '\n';
```

样例：`[1,2,3] * [4,5]` -> 输出 `4 13 22 15`。

**传参要求（照这个传不会错）：**

- `a, b`：长度 `n, m` 的 `vector<int>` 系数，取值先规范到 `[0, 998244353)`；空数组返回空结果。
- 返回值：长度 `n + m - 1` 的 `vector<int>`，结果已对 `998244353` 取模。
- 长度限制：`n + m - 1 <= 2^23`（本模数支持的最大 2 次幂长度）；超长换 FWT 思路或分块卷积。
- `ntt(a, invert)` 是底层函数，普通卷积题只调 `convolution`。


**不会用就照抄：**

```cpp
auto c = convolution(a, b);  // c[k] = sum a[i] * b[k-i]
```

- 先确认模板模数（常见 `998244353`）和原根；不要直接换成任意模数。
- 输入系数先规范到模数范围。


**API / 入口函数（赛时只认这里列的名字）：**

- `convolution(a,b)` -> 返回卷积系数；结果长度 `a.size()+b.size()-1`。
- `ntt(a,invert)` -> 底层正/逆变换；普通卷积题直接调用 `convolution`，不要手动碰。
**核心逻辑（改代码时别破坏）：**

- 把系数值表示转换为点值表示，逐点相乘，再逆变换回系数。
- 长度补到不小于 `n+m-1` 的 2 的幂；这版固定模 `998244353`、原根 `3`.

```cpp
const int NTT_MOD = 998244353;
const int NTT_G = 3;

int mod_pow_int(int a, int e) {
    i64 r = 1, x = a;
    while (e) {
        if (e & 1) r = r * x % NTT_MOD;
        x = x * x % NTT_MOD;
        e >>= 1;
    }
    return (int)r;
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
        int wlen = mod_pow_int(NTT_G, (NTT_MOD - 1) / len);
        if (invert) wlen = mod_pow_int(wlen, NTT_MOD - 2);
        for (int i = 0; i < n; i += len) {
            i64 w = 1;
            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j];
                int v = (int)(a[i + j + len / 2] * w % NTT_MOD);
                a[i + j] = u + v < NTT_MOD ? u + v : u + v - NTT_MOD;
                a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + NTT_MOD;
                w = w * wlen % NTT_MOD;
            }
        }
    }
    if (invert) {
        int inv_n = mod_pow_int(n, NTT_MOD - 2);
        for (int& x : a) x = (int)(1LL * x * inv_n % NTT_MOD);
    }
}

vector<int> convolution(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) return {};
    int need = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < need) n <<= 1;
    a.resize(n);
    b.resize(n);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; ++i) a[i] = (int)(1LL * a[i] * b[i] % NTT_MOD);
    ntt(a, true);
    a.resize(need);
    return a;
}
```
#### 形式幂级数核心：`inv / ln / exp / sqrt / pow / divmod`

> **赛时先看**
> - **题目信号**：题目直接要求多项式初等函数；生成函数可整理成 `F=exp(G)`、`F*G=1`、`F^2=G`；需要长度 `1e5` 以上的卷积并继续做除法；或者需要从复杂递推中抽出有限项生成函数。
> - **本质**：在模 `998244353` 下，对截断到 `x^n` 的多项式/形式幂级数做快速运算。包含 NTT 卷积、牛顿迭代求逆和开方、对数、指数、非负整数幂、多项式除法与取模。
> - **接法**：多项式求逆/开根/ln/exp 模板题；有标号组合结构的 EGF；将递推转为生成函数后取前 `n` 项；需要快速除以一组线性因子的多项式题。
> - **复杂度判定**：卷积 `O(n log n)`；`inv / ln / exp / sqrt / pow` 为 `O(n log^2 n)` 的稳健版本；多项式 `divmod` 为 `O(n log^2 n)`。NTT 长度不能超过 `2^23`。
> - **维护的量**：`Poly`（系数向量，`Poly[i]` 是 x^i 的系数）；`n`（要保留的系数个数）；`FPS_MOD=998244353`（固定模数）。
> - **警告**：所有函数的第二参数 `n` 都是“要保留的系数个数”，不是最高次数。每次运算后结果会截断到 `x^n`。形式幂级数不是普通实函数，`ln/exp` 的常数项限制不能忽略。任意模数请使用 FFT 或多模 NTT + CRT，不要直接改 `MOD`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：求 F(x)=1+x 的逆的前 4 项（1/(1+x)=1-x+x^2-x^3+...）。
Poly f = {1, 1};                 // f[0]=1 非零，可求逆
Poly g = fps_inv(f, 4);          // g = {1, 998244352, 1, 998244352}，即 1-x+x^2-x^3
// 验证：样例 g = {1, 998244352, 1, 998244352}（mod 998244353）
```

**传参要求（照这个传不会错）：**

- `fps_inv(f, n)`：f 非空且 `f[0] != 0`；n>=1 是保留系数个数；返回 f 在 mod x^n 下的逆。
- `fps_log(f, n)`：要求 `f[0] == 1`；返回 ln(f) mod x^n。
- `fps_exp(f, n)`：要求 `f[0] == 0`；返回 exp(f) mod x^n。
- `fps_sqrt_nonzero_const(f, n)`：要求 `f[0] != 0` 且常数项有平方根（无平方根 assert 失败）；返回 mod x^n 意义下的一个平方根。
- `fps_pow(f, k, n)`：k 为非负整数（i64）；若首位非零项次数*k >= n 返回全 0 的 Poly(n)。
- `fps_divmod(f, g)` / `fps_mod(f, g)`：g 非零；返回 {商， 余数} 或余数。
- 所有函数的第二参数 `n` 是系数个数而不是最高次数；模数固定 `998244353`，任意模数不要改 `FPS_MOD`。

**API / 入口函数（赛时只认这里列的名字）：**

- `fps_divmod(Poly f, Poly g)` -> 返回 f/g 的商和余数；要求 g 非零。
- `fps_exp(const Poly& f, int n)` -> 返回 exp(f) mod x^n；要求 f[0] == 0。
- `fps_inv(const Poly& f, int n)` -> 返回 f 在 mod x^n 意义下的逆；要求 f[0] != 0。
- `fps_log(const Poly& f, int n)` -> 返回 ln(f) mod x^n；要求 f[0] == 1。
- `fps_pow(Poly f, i64 k, int n)` -> 返回非负整数 k 下的 f^k mod x^n。
- `fps_sqrt_nonzero_const(const Poly& f, int n)` -> 返回 f 在 mod x^n 意义下的一个平方根；要求 f[0] != 0 且常数项有平方根。
**改板时先认这几个量：**

- `Poly`：`vector<int>` 的别名，所有多项式都以它表示。
- `FPS_MOD`：固定模数 `998244353`，原根 `FPS_G=3`。
- `n`：所有函数第二参数，表示要保留的系数个数。

使用前提：模数固定为 `998244353`，原根为 `3`。`fps_inv` 要求常数项非零；`fps_log` 要求 `f[0]=1`；`fps_exp` 要求 `f[0]=0`；`fps_sqrt_nonzero_const` 要求常数项存在平方根且非零；`fps_pow` 的指数为非负整数。

典题模型：多项式求逆/开根/ln/exp 模板题；有标号组合结构的 EGF；将递推转为生成函数后取前 `n` 项；需要快速除以一组线性因子的多项式题。

```cpp
using Poly = vector<int>;
constexpr int FPS_MOD = 998244353;
constexpr int FPS_G = 3;

int fps_powmod(i64 a, i64 e) {
    i64 r = 1;
    while (e) {
        if (e & 1) r = r * a % FPS_MOD;
        a = a * a % FPS_MOD;
        e >>= 1;
    }
    return (int)r;
}

void fps_ntt(Poly& a, bool invert) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        int wlen = fps_powmod(FPS_G, (FPS_MOD - 1) / len);
        if (invert) wlen = fps_powmod(wlen, FPS_MOD - 2);
        for (int i = 0; i < n; i += len) {
            i64 w = 1;
            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j];
                int v = (int)(a[i + j + len / 2] * w % FPS_MOD);
                a[i + j] = u + v < FPS_MOD ? u + v : u + v - FPS_MOD;
                a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + FPS_MOD;
                w = w * wlen % FPS_MOD;
            }
        }
    }
    if (invert) {
        int inv_n = fps_powmod(n, FPS_MOD - 2);
        for (int& x : a) x = (int)(1LL * x * inv_n % FPS_MOD);
    }
}

Poly fps_mul(Poly a, Poly b) {
    if (a.empty() || b.empty()) return {};
    if (min(a.size(), b.size()) <= 32) {
        Poly c(a.size() + b.size() - 1);
        for (int i = 0; i < (int)a.size(); ++i)
            for (int j = 0; j < (int)b.size(); ++j)
                c[i + j] = (c[i + j] + 1LL * a[i] * b[j]) % FPS_MOD;
        return c;
    }
    int need = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < need) n <<= 1;
    assert(n <= (1 << 23));
    a.resize(n); b.resize(n);
    fps_ntt(a, false); fps_ntt(b, false);
    for (int i = 0; i < n; ++i) a[i] = (int)(1LL * a[i] * b[i] % FPS_MOD);
    fps_ntt(a, true);
    a.resize(need);
    return a;
}

Poly fps_derivative(const Poly& f) {
    if (f.size() <= 1) return {};
    Poly g(f.size() - 1);
    for (int i = 1; i < (int)f.size(); ++i) g[i - 1] = (int)(1LL * i * f[i] % FPS_MOD);
    return g;
}

Poly fps_integral(const Poly& f) {
    int n = (int)f.size();
    Poly inv(n + 1), g(n + 1);
    if (n >= 1) inv[1] = 1;
    for (int i = 2; i <= n; ++i) inv[i] = (int)(1LL * (FPS_MOD - FPS_MOD / i) * inv[FPS_MOD % i] % FPS_MOD);
    for (int i = 0; i < n; ++i) g[i + 1] = (int)(1LL * f[i] * inv[i + 1] % FPS_MOD);
    return g;
}

// 返回 f 在 mod x^n 意义下的逆；要求 f[0] != 0。
Poly fps_inv(const Poly& f, int n) {
    assert(!f.empty() && f[0] != 0 && n >= 1);
    Poly g{fps_powmod(f[0], FPS_MOD - 2)};
    for (int m = 1; m < n; m <<= 1) {
        int need = min(n, m << 1);
        Poly cut(f.begin(), f.begin() + min((int)f.size(), need));
        Poly h = fps_mul(cut, g);
        h.resize(need);
        for (int& x : h) x = x ? FPS_MOD - x : 0;
        h[0] = (h[0] + 2) % FPS_MOD;
        g = fps_mul(g, h);
        g.resize(need);
    }
    return g;
}

// 返回 ln(f) mod x^n；要求 f[0] == 1。
Poly fps_log(const Poly& f, int n) {
    assert(!f.empty() && f[0] == 1 && n >= 1);
    Poly g = fps_mul(fps_derivative(f), fps_inv(f, n));
    g.resize(max(0, n - 1));
    g = fps_integral(g);
    g.resize(n);
    return g;
}

// 返回 exp(f) mod x^n；要求 f[0] == 0。
Poly fps_exp(const Poly& f, int n) {
    assert((f.empty() || f[0] == 0) && n >= 1);
    Poly g{1};
    for (int m = 1; m < n; m <<= 1) {
        int need = min(n, m << 1);
        Poly lg = fps_log(g, need);
        Poly h(need);
        for (int i = 0; i < need; ++i) {
            int fi = i < (int)f.size() ? f[i] : 0;
            h[i] = fi - lg[i];
            if (h[i] < 0) h[i] += FPS_MOD;
        }
        h[0] = (h[0] + 1) % FPS_MOD; // 牛顿迭代公式中的 1 + f - ln(g)。
        g = fps_mul(g, h);
        g.resize(need);
    }
    return g;
}

int fps_mod_sqrt(int a) { // Tonelli-Shanks；不存在平方根时返回 -1。
    if (a == 0) return 0;
    if (fps_powmod(a, (FPS_MOD - 1) / 2) != 1) return -1;
    int q = FPS_MOD - 1, s = 0;
    while ((q & 1) == 0) q >>= 1, ++s;
    int z = 2;
    while (fps_powmod(z, (FPS_MOD - 1) / 2) != FPS_MOD - 1) ++z;
    i64 c = fps_powmod(z, q), x = fps_powmod(a, (q + 1) / 2), t = fps_powmod(a, q);
    for (int m = s; t != 1;) {
        int i = 1;
        i64 tt = t * t % FPS_MOD;
        while (tt != 1) tt = tt * tt % FPS_MOD, ++i;
        i64 b = fps_powmod(c, 1LL << (m - i - 1));
        x = x * b % FPS_MOD;
        c = b * b % FPS_MOD;
        t = t * c % FPS_MOD;
        m = i;
    }
    return (int)x;
}

// 返回 f 在 mod x^n 意义下的一个平方根；要求 f[0] != 0 且常数项有平方根。
Poly fps_sqrt_nonzero_const(const Poly& f, int n) {
    assert(!f.empty() && f[0] != 0 && n >= 1);
    int root = fps_mod_sqrt(f[0]);
    assert(root != -1);
    Poly g{root};
    const int inv2 = (FPS_MOD + 1) / 2;
    for (int m = 1; m < n; m <<= 1) {
        int need = min(n, m << 1);
        Poly cut(f.begin(), f.begin() + min((int)f.size(), need));
        Poly div = fps_mul(cut, fps_inv(g, need));
        div.resize(need);
        g.resize(need);
        for (int i = 0; i < need; ++i) g[i] = (int)(1LL * (g[i] + div[i]) * inv2 % FPS_MOD);
    }
    return g;
}

// 返回非负整数 k 下的 f^k mod x^n。
Poly fps_pow(Poly f, i64 k, int n) {
    assert(k >= 0 && n >= 1);
    if (k == 0) { Poly one(n); one[0] = 1; return one; }
    int first = 0;
    while (first < (int)f.size() && f[first] == 0) ++first;
    if (first == (int)f.size() || (i128)first * k >= n) return Poly(n);
    int shift = (int)(first * k);
    int need = n - shift;
    int c = f[first], inv_c = fps_powmod(c, FPS_MOD - 2);
    Poly h;
    for (int i = first; i < (int)f.size() && (int)h.size() < need; ++i)
        h.push_back((int)(1LL * f[i] * inv_c % FPS_MOD));
    h.resize(need);
    Poly l = fps_log(h, need);
    int km = (int)(k % FPS_MOD);
    for (int& x : l) x = (int)(1LL * x * km % FPS_MOD);
    Poly ans = fps_exp(l, need);
    int scale = fps_powmod(c, k);
    for (int& x : ans) x = (int)(1LL * x * scale % FPS_MOD);
    Poly out(n);
    for (int i = 0; i < need; ++i) out[i + shift] = ans[i];
    return out;
}

void fps_trim(Poly& f) { while (!f.empty() && f.back() == 0) f.pop_back(); }

// 返回 f/g 的商和余数；要求 g 非零。
pair<Poly, Poly> fps_divmod(Poly f, Poly g) {
    fps_trim(f); fps_trim(g);
    assert(!g.empty());
    if (f.size() < g.size()) return {{}, f};
    int qn = (int)f.size() - (int)g.size() + 1;
    Poly rf = f, rg = g;
    reverse(rf.begin(), rf.end());
    reverse(rg.begin(), rg.end());
    Poly q = fps_mul(rf, fps_inv(rg, qn));
    q.resize(qn);
    reverse(q.begin(), q.end());

    Poly product = fps_mul(q, g);
    Poly r(f.size());
    for (int i = 0; i < (int)f.size(); ++i) {
        int sub = i < (int)product.size() ? product[i] : 0;
        r[i] = f[i] - sub;
        if (r[i] < 0) r[i] += FPS_MOD;
    }
    r.resize(g.size() - 1);
    fps_trim(r);
    return {q, r};
}

Poly fps_mod(Poly f, const Poly& g) { return fps_divmod(std::move(f), g).second; }
```
#### 积树：多点求值 `f(x_i)`

> **赛时先看**
> - **题目信号**：`m` 个询问点都要代入同一个高次多项式；点是任意模数值而不是连续 `0..n-1`（连续点可优先翻 Lagrange）；或要求后续做快速插值。
> - **本质**：给一个多项式和很多互异点 `x_i`，同时计算所有 `f(x_i)`。本实现利用积树和上面的多项式取模，适合点数较大时替代逐点 `O(nm)` 代入。
> - **接法**：多项式在大量随机点的值；积树/余数树模板；快速插值的前半部分；求 `f(a_i)` 并再做组合/CRT。
> - **复杂度判定**：`O(m log^2 m)` 量级（包含多项式除法），空间 `O(m log m)`。
> - **维护的量**：`x`（询问点集，构造时已正规化到 [0,MOD)）；`product`（乘积树，`product[p]` 存节点区间对应的 ∏(X-x_i)）；`answer`（每个点的 f(x_i)）。
> - **警告**：这份实现假定所有 `x_i` 互异；重复点在求值本身没问题，但如果之后复用到插值会出错。`points` 为空时直接返回空。多项式系数与点都必须先正规化到 `[0,MOD)`。


**最小完整示例（先抄这一段就能跑）：**

依赖：形式幂级数核心：`inv / ln / exp / sqrt / pow / divmod` 节的 Poly / FPS_MOD / fps_mul / fps_mod，抄板时一起抄上。

```cpp
// 题目：求 f(x)=x^2+1 在 x=0,1,2 三点的值。
MultipointEvaluation ev({0, 1, 2});          // 构造乘积树，点集内部自动取模
vector<int> ans = ev.evaluate({1, 0, 1});    // f={1,0,1}，输出 ans={1, 2, 5}
// 验证：样例 f(0)=1, f(1)=2, f(2)=5
```

**传参要求（照这个传不会错）：**

- `MultipointEvaluation(points)`：points 为询问点 x_i（要求互异；重复点求值没问题但不可再复用去插值）；points 为空时 evaluate 返回空。
- `evaluate(f)`：f 为 Poly 系数（f[i] 是 x^i 的系数）；返回 vector<int>，ans[i] = f(x[i])，长度等于点数。
- 点与系数在构造/内部都会按 `% FPS_MOD` 正规化，传负数也能跑；依赖上一节的 `fps_mod`。

典题模型：多项式在大量随机点的值；积树/余数树模板；快速插值的前半部分；求 `f(a_i)` 并再做组合/CRT。

```cpp
struct MultipointEvaluation {
    int n;
    vector<int> x, answer;
    vector<Poly> product;

    explicit MultipointEvaluation(vector<int> points) : n((int)points.size()), x(std::move(points)) {
        for (int& v : x) {
            v %= FPS_MOD;
            if (v < 0) v += FPS_MOD;
        }
        product.resize(max(1, 4 * n));
        answer.resize(n);
        if (n) build(1, 0, n);
    }

    void build(int p, int l, int r) {
        if (r - l == 1) {
            product[p] = {(FPS_MOD - x[l]) % FPS_MOD, 1}; // 叶子多项式为 X - x[l]。
            return;
        }
        int m = (l + r) / 2;
        build(p * 2, l, m);
        build(p * 2 + 1, m, r);
        product[p] = fps_mul(product[p * 2], product[p * 2 + 1]);
    }

    void solve(int p, int l, int r, const Poly& remainder) {
        if (r - l == 1) {
            answer[l] = remainder.empty() ? 0 : remainder[0];
            return;
        }
        int m = (l + r) / 2;
        solve(p * 2, l, m, fps_mod(remainder, product[p * 2]));
        solve(p * 2 + 1, m, r, fps_mod(remainder, product[p * 2 + 1]));
    }

    vector<int> evaluate(const Poly& f) {
        if (!n) return {};
        solve(1, 0, n, fps_mod(f, product[1]));
        return answer;
    }
};
```
#### 积树：快速插值 `f(x_i)=y_i`

> **赛时先看**
> - **题目信号**：给了许多离散点的函数值，要求恢复系数多项式；横坐标任意且数量达到 `1e5`；需要根据点值重建生成函数或再做多项式运算。
> - **本质**：给定互异点 `x_i` 与对应值 `y_i`，构造唯一的次数小于点数的多项式。它和多点求值互为对偶，适合点数大、横坐标不连续的插值。
> - **接法**：任意点快速插值模板题；从多组函数值重建多项式；多项式求值/插值的往返变换。
> - **复杂度判定**：`O(n log^2 n)`，空间 `O(n log n)`。依赖上一节的 `MultipointEvaluation` 与 FPS 核心，必须按顺序粘贴。
> - **维护的量**：`tree`（复用多点求值的乘积树 M(x)=∏(x-x_i)）；`weight`（每点权重 y_i / M'(x_i)）；`interpolate` 返回唯一插值多项式。
> - **警告**：所有 `x_i` 必须两两不同，否则 `M'(x_i)=0` 无法求逆。返回多项式次数严格小于点数；若只需要 `0..n-1` 连续点处的单个值，优先使用前册 Lagrange，常数更小。


**最小完整示例（先抄这一段就能跑）：**

依赖：积树：多点求值 节 与 形式幂级数核心：`inv / ln / exp / sqrt / pow / divmod` 节（MultipointEvaluation / fps_derivative / fps_powmod 等），抄板时一起抄上。

```cpp
// 题目：过三点 (0,1)、(1,2)、(2,5) 还原多项式。
MultipointInterpolation it({0, 1, 2});   // 先建乘积树
Poly f = it.interpolate({1, 2, 5});      // 返回 f={1, 0, 1}，即 x^2+1
// 验证：样例 f={1, 0, 1}，代回三点均吻合
```

**传参要求（照这个传不会错）：**

- `MultipointInterpolation(points)`：x_i 必须两两不同（否则 M'(x_i)=0 求逆 assert 失败）。
- `interpolate(y)`：y[i] 是 f(tree.x[i]) 的函数值，y.size() 必须等于点数；返回次数严格小于点数的 Poly。
- 依赖「积树：多点求值」与「形式幂级数核心」的 `fps_derivative / fps_mul / fps_powmod`，必须按顺序粘贴。

**API / 入口函数（赛时只认这里列的名字）：**

- `interpolate(const vector<int>& y)` -> y[i] 是 f(tree.x[i])；所有 x_i 必须两两不同。 返回 `Poly`。

典题模型：任意点快速插值模板题；从多组函数值重建多项式；多项式求值/插值的往返变换。

```cpp
struct MultipointInterpolation {
    MultipointEvaluation tree; // 复用它的乘积树 M(x)=prod(x-x_i)。
    vector<int> weight;

    explicit MultipointInterpolation(vector<int> points) : tree(std::move(points)) {}

    Poly merge(int p, int l, int r) {
        if (r - l == 1) return {weight[l]};
        int m = (l + r) / 2;
        Poly left = merge(p * 2, l, m);
        Poly right = merge(p * 2 + 1, m, r);
        Poly a = fps_mul(left, tree.product[p * 2 + 1]);
        Poly b = fps_mul(right, tree.product[p * 2]);
        if (a.size() < b.size()) a.resize(b.size());
        for (int i = 0; i < (int)b.size(); ++i) {
            a[i] += b[i];
            if (a[i] >= FPS_MOD) a[i] -= FPS_MOD;
        }
        return a;
    }

    // y[i] 是 f(tree.x[i])；所有 x_i 必须两两不同。
    Poly interpolate(const vector<int>& y) {
        int n = tree.n;
        assert((int)y.size() == n);
        if (n == 0) return {};
        vector<int> denom = tree.evaluate(fps_derivative(tree.product[1]));
        weight.resize(n);
        for (int i = 0; i < n; ++i) {
            assert(denom[i] != 0);
            int yi = y[i] % FPS_MOD;
            if (yi < 0) yi += FPS_MOD;
            weight[i] = (int)(1LL * yi * fps_powmod(denom[i], FPS_MOD - 2) % FPS_MOD);
        }
        return merge(1, 0, n);
    }
};
```
#### FWT：AND / OR 卷积

> **赛时先看**
> - **题目信号**：状态由 bitmask 表示，合并时用按位 OR/AND 而非 XOR；数组长度是 `2^m`。
> - **本质**：计算 `c[k] = sum_{i | j = k} a[i]b[j]` 或 `c[k] = sum_{i & j = k} a[i]b[j]`。
> - **接法**：两个集合任选一个元素后，问并集掩码或交集掩码的方案数。
> - **复杂度判定**：`O(m 2^m)`。
> - **维护的量**：`a`、`b`（原数组，值在 [0,mod)）；`n`（补齐后的 2 的幂长度）；`mod`（模数）。
> - **警告**：OR 与 AND 的正反变换方向不同；必须在模数下规范化负数。XOR 卷积见下一节 FWHT。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：a={1,2}、b={2,3}，求 OR 卷积与 AND 卷积（长度自动补到 2）。
vector<i64> c1 = or_convolution({1, 2}, {2, 3}, 998244353);  // c1={2, 13}
vector<i64> c2 = and_convolution({1, 2}, {2, 3}, 998244353); // c2={9, 6}
// 验证：c1[k]=Σ_{i|j=k}a[i]b[j]；c2[k]=Σ_{i&j=k}a[i]b[j]
```

**传参要求（照这个传不会错）：**

- `or_convolution(a, b, mod)` / `and_convolution(a, b, mod)`：返回 c[k] = Σ_{i|j=k} a[i]b[j]（或 i&j=k）；内部自动把长度补齐到 2 的幂。
- `fwt_or(a, mod, inverse)` / `fwt_and(a, mod, inverse)`：原地变换；inverse=true 做逆变换（已含归一化），一般不单独用。
- 数组元素必须在 [0, mod)，mod 传质数（如 998244353）；类型是 vector<i64>，乘法会取模不会溢出。

典题模型：两个集合任选一个元素后，问并集掩码或交集掩码的方案数。

```cpp
void fwt_or(vector<i64>& a, i64 mod, bool inverse) {
    int n = (int)a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; ++j) {
                i64& x = a[i + j];
                i64& y = a[i + j + len];
                y = (y + (inverse ? -x : x)) % mod;
                if (y < 0) y += mod;
            }
        }
    }
}

void fwt_and(vector<i64>& a, i64 mod, bool inverse) {
    int n = (int)a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; ++j) {
                i64& x = a[i + j];
                i64& y = a[i + j + len];
                x = (x + (inverse ? -y : y)) % mod;
                if (x < 0) x += mod;
            }
        }
    }
}

vector<i64> or_convolution(vector<i64> a, vector<i64> b, i64 mod) {
    int n = 1;
    while (n < (int)max(a.size(), b.size())) n <<= 1;
    a.resize(n); b.resize(n);
    fwt_or(a, mod, false);
    fwt_or(b, mod, false);
    for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % mod;
    fwt_or(a, mod, true);
    return a;
}

vector<i64> and_convolution(vector<i64> a, vector<i64> b, i64 mod) {
    int n = 1;
    while (n < (int)max(a.size(), b.size())) n <<= 1;
    a.resize(n); b.resize(n);
    fwt_and(a, mod, false);
    fwt_and(b, mod, false);
    for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % mod;
    fwt_and(a, mod, true);
    return a;
}
```
#### FWHT：XOR 卷积

> **赛时先看**
> - **题目信号**：子集异或卷积、按 xor 合并状态。
> - **本质**：计算 `c[k] = sum_{i xor j = k} a[i] b[j]`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(n log n)`，`n` 为 2 的幂。
> - **维护的量**：`a`、`b`（原数组，值在 [0,mod)）；`n`（2 的幂长度）；`mod`（模数，逆变换要乘 inv_n）。
> - **警告**：逆变换时每项乘 `inv_n`。依赖 `mod_pow`，来自第 10 章通用辅助函数。


**最小完整示例（先抄这一段就能跑）：**

依赖：本册通用辅助函数 节的 mod_pow，抄板时一起抄上。

```cpp
// 题目：a={1,2}、b={2,3}，求 XOR 卷积。
vector<i64> c = xor_convolution({1, 2}, {2, 3}, 998244353); // c={8, 7}
// 验证：c[0]=1*2+2*3=8，c[1]=1*3+2*2=7
```

**传参要求（照这个传不会错）：**

- `xor_convolution(a, b, mod)`：返回 c[k] = Σ_{i xor j = k} a[i]b[j]；长度自动补齐到 2 的幂。
- `fwht_xor(a, mod, invert)`：原地变换；invert=true 时自动乘 inv_n，一般不单独用。
- 数组元素必须在 [0, mod)；依赖 `mod_pow(n, mod-2, mod)`；与「XOR 随机变量分布」节的同名 `fwht_xor` 签名不同，混抄时二选一或改名。

```cpp
void fwht_xor(vector<i64>& a, i64 mod, bool invert) {
    int n = (int)a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; ++j) {
                i64 x = a[i + j], y = a[i + j + len];
                a[i + j] = (x + y) % mod;
                a[i + j + len] = (x - y + mod) % mod;
            }
        }
    }
    if (invert) {
        i64 inv_n = mod_pow(n, mod - 2, mod);
        for (auto& x : a) x = x * inv_n % mod;
    }
}

vector<i64> xor_convolution(vector<i64> a, vector<i64> b, i64 mod) {
    int n = 1;
    while (n < (int)max(a.size(), b.size())) n <<= 1;
    a.resize(n); b.resize(n);
    fwht_xor(a, mod, false);
    fwht_xor(b, mod, false);
    for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % mod;
    fwht_xor(a, mod, true);
    return a;
}
```
#### XOR 随机变量分布：FWHT 合并

> **赛时先看**
> - **题目信号**：每个变量独立取值，条件是 `x1 xor x2 xor ... xor xn = target`。
> - **本质**：多个独立随机变量，求它们异或和等于目标值的概率或方案数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：值域大小为 `V=2^k` 时，`O(n V log V)`；适合值域较小或可压缩的版本。
> - **维护的量**：`upper`（各变量取值上界 a_i）；`V`（值域大小，2 的幂）；`prod`（FWHT 变换域上的累积乘积）。
> - **警告**：概率要乘每个变量取值数量的逆元；`FWHT` 乘完后要做逆变换。依赖 `pow_mod_any`（本章前文定义）；`MOD` 在本节代码内自行定义。注意本节 `fwht_xor` 与「FWHT：XOR 卷积」节同名函数签名不同，混抄时二选一或改名。


**最小完整示例（先抄这一段就能跑）：**

依赖：典题：标号图度数奇偶计数 节的 pow_mod_any，抄板时一起抄上。

```cpp
// 题目：X∈[0,1]、Y∈[0,1] 独立均匀，求 X xor Y 的分布（概率模 MOD）。
vector<i64> dist = xor_distribution_uniform_ranges({1, 1}); // dist={499122177, 499122177}
// 验证：样例 dist[0]=dist[1]=499122177，即概率各 1/2（mod 998244353）
```

**传参要求（照这个传不会错）：**

- `xor_distribution_uniform_ranges(upper)`：upper[i] 是第 i 个变量的取值上界（取值 0..a_i 均匀，各变量独立），元素为非负整数。
- 返回值：长度 V 的 vector<i64>，dist[t] = 异或和恰为 t 的概率（模 MOD），V 由内部自动取最小的 2 的幂。
- 概率已在函数内乘过各变量取值数量的逆元，外面不要再除；依赖 `pow_mod_any`（本章前文定义）。

**改板时先认这几个量：**

- `dist`：单个变量的取值分布数组（长度 `V`）。
- `mx`：`upper` 中的最大值，决定值域大小 `V`。

```cpp
const i64 MOD = 998244353;

void fwht_xor(vector<i64>& a, bool inverse) {
    int n = (int)a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; j++) {
                i64 x = a[i + j], y = a[i + j + len];
                a[i + j] = (x + y) % MOD;
                a[i + j + len] = (x - y + MOD) % MOD;
            }
        }
    }
    if (inverse) {
        i64 inv_n = pow_mod_any(n, MOD - 2, MOD);
        for (i64& x : a) x = x * inv_n % MOD;
    }
}

vector<i64> xor_distribution_uniform_ranges(const vector<int>& upper) {
    int mx = 0;
    for (int x : upper) mx = max(mx, x);
    int V = 1;
    while (V <= mx) V <<= 1;
    vector<i64> prod(V, 1);
    for (int a : upper) {
        vector<i64> dist(V, 0);
        i64 inv = pow_mod_any(a + 1, MOD - 2, MOD);
        for (int x = 0; x <= a; x++) dist[x] = inv;
        fwht_xor(dist, false);
        for (int i = 0; i < V; i++) prod[i] = prod[i] * dist[i] % MOD;
    }
    fwht_xor(prod, true);
    return prod;
}
```
#### Berlekamp-Massey + Kitamasa

> **赛时先看**
> - **题目信号**：给出序列前项，猜/求最短线性递推；组合计数高阶递推。
> - **本质**：从前若干项推线性递推，求第 `n` 项。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：BM `O(k^2)`，求第 n 项 `O(k^2 log n)`。
> - **维护的量**：`s`（已知前项序列）；`rec`（BM 求出的最短递推系数，`rec.size()` 即阶数 k）；`init`（递推初项）。
> - **警告**：模数通常要求质数；输入项至少给足 `2k` 比较稳。


**最小完整示例（先抄这一段就能跑）：**

> 依赖：本册通用辅助函数（`mod_pow`），抄板时一起抄上。

```cpp
// 题目：Fibonacci 前 6 项 0,1,1,2,3,5，求第 10 项（下标从 0 计）。
vector<i64> rec = berlekamp_massey({0, 1, 1, 2, 3, 5}, 998244353); // rec={1, 1}
i64 ans = linear_recurrence_nth({0, 1}, rec, 10, 998244353);      // ans=55
// 验证：样例 ans=55（F10）
```

**传参要求（照这个传不会错）：**

- `berlekamp_massey(s, mod)`：s 为已知前项，长度至少给足 2k 较稳；返回最短递推 rec（满足 s[n] = Σ rec[i-1]*s[n-i]）；mod 通常要求质数。
- `linear_recurrence_nth(init, rec, n, mod)`：init 为前 k 项（init[i] = 序列第 i 项，长度 = rec.size()）；n 为 0-indexed 项号，n < init.size() 时直接返回 init[n]；返回第 n 项（i64）。
- `combine_rec` 是内部函数，不用直接调。

```cpp
vector<i64> berlekamp_massey(const vector<i64>& s, i64 mod) {
    vector<i64> C{1}, B{1};
    i64 b = 1;
    int m = 1;
    for (int n = 0; n < (int)s.size(); ++n) {
        i64 d = 0;
        for (int i = 0; i < (int)C.size(); ++i) d = (d + C[i] * s[n - i]) % mod;
        if (d == 0) {
            m++;
            continue;
        }
        vector<i64> T = C;
        i64 coef = d * mod_pow(b, mod - 2, mod) % mod;
        if ((int)C.size() < (int)B.size() + m) C.resize(B.size() + m, 0);
        for (int i = 0; i < (int)B.size(); ++i) {
            C[i + m] = (C[i + m] - coef * B[i]) % mod;
            if (C[i + m] < 0) C[i + m] += mod;
        }
        if (2 * ((int)T.size() - 1) <= n) {
            B = T;
            b = d;
            m = 1;
        } else {
            m++;
        }
    }
    C.erase(C.begin());
    for (auto& x : C) x = (mod - x) % mod;
    return C; // 递推式：s[n] = sum C[i-1] * s[n-i]。
}

vector<i64> combine_rec(vector<i64> a, vector<i64> b, const vector<i64>& rec, i64 mod) {
    int k = (int)rec.size();
    vector<i64> tmp(2 * k, 0);
    for (int i = 0; i < k; ++i) for (int j = 0; j < k; ++j) tmp[i + j] = (tmp[i + j] + a[i] * b[j]) % mod;
    for (int i = 2 * k - 2; i >= k; --i) {
        for (int j = 1; j <= k; ++j) tmp[i - j] = (tmp[i - j] + tmp[i] * rec[j - 1]) % mod;
    }
    tmp.resize(k);
    return tmp;
}

i64 linear_recurrence_nth(const vector<i64>& init, const vector<i64>& rec, i64 n, i64 mod) {
    int k = (int)rec.size();
    if (n < (int)init.size()) return init[n] % mod;
    vector<i64> pol(k), e(k);
    pol[0] = 1;
    if (k == 1) e[0] = rec[0];
    else e[1] = 1;
    while (n) {
        if (n & 1) pol = combine_rec(pol, e, rec, mod);
        e = combine_rec(e, e, rec, mod);
        n >>= 1;
    }
    i64 ans = 0;
    for (int i = 0; i < k; ++i) ans = (ans + pol[i] * init[i]) % mod;
    return ans;
}
```
#### Bostan-Mori：有理生成函数第 n 项

> **赛时先看**
> - **题目信号**：序列满足线性递推但项数极大；已经得到生成函数分子分母；Kitamasa 的 `O(k^2 log n)` 不够，且模数可用 `998244353`；题目问第 `n` 项而不是前 `n` 项。
> - **本质**：求有理生成函数 `P(x) / Q(x)` 的 `[x^n]` 系数。尤其适合常系数线性递推、`n` 可到 `1e18`、递推阶数在 `1e5` 量级的场景。
> - **接法**：常系数递推第 `10^18` 项；有理生成函数系数；大阶 Fibonacci 类推广；线性递推与 NTT 结合题。
> - **复杂度判定**：`O(k log k log n)`，`k=deg(Q)`；依赖上文形式幂级数核心的 `Poly / fps_mul / fps_mod / fps_powmod`，并固定模数 `998244353`。
> - **维护的量**：`p`、`q`（生成函数 P(x)/Q(x) 的分子分母，内部保证 deg P < deg Q）；`n`（要取的第 n 项，可到 1e18）。
> - **警告**：`Q[0]` 必须非零。按“递推系数构造 `Q`”时，若 `a_n = c_1 a_{n-1}+...+c_k a_{n-k}`，则 `Q=1-c_1 x-...-c_k x^k`。传入前把 `P`、`Q` 都规范化到模数范围。


**最小完整示例（先抄这一段就能跑）：**

> 依赖：形式幂级数核心（`Poly` / `FPS_MOD` / `fps_mul` / `fps_mod` / `fps_powmod`），抄板时一起抄上。

```cpp
// 题目：a_n = 2a_{n-1}，a_0=1，求 a_100（即 1/(1-2x) 的 [x^100]）。
Poly Q = {1, FPS_MOD - 2};              // Q = 1 - 2x
int ans = bostan_mori_nth({1}, Q, 100); // ans = 882499742（2^100 mod 998244353）
// 验证：样例 ans = 882499742
```

**传参要求（照这个传不会错）：**

- `bostan_mori_nth(p, q, n)`：n 为非负整数（可到 1e18）；q 非空且 `q[0] != 0`；p 会自动先 `fps_mod(p, q)`（保证 deg P < deg Q）；返回 [x^n] P(x)/Q(x)（int）。
- 构造 Q：递推 `a_n = c_1 a_{n-1}+...+c_k a_{n-k}` → `Q = {1, -c_1, ..., -c_k}`（mod FPS_MOD）；P 按代码下方注释由前 k 项展开。
- 依赖形式幂级数核心（`fps_mul / fps_mod / fps_powmod`），固定模数 `998244353`，不要改 `FPS_MOD`。

**API / 入口函数（赛时只认这里列的名字）：**

- `bostan_mori_nth(Poly p, Poly q, i64 n)` -> 依赖上文形式幂级数核心（`fps_mul`/`fps_mod`/`fps_powmod`）；返回 [x^n] P(x)/Q(x)。

典题模型：常系数递推第 `10^18` 项；有理生成函数系数；大阶 Fibonacci 类推广；线性递推与 NTT 结合题。

```cpp
// 依赖上文形式幂级数核心（fps_mul/fps_mod/fps_powmod）；返回 [x^n] P(x)/Q(x)。
int bostan_mori_nth(Poly p, Poly q, i64 n) {
    assert(n >= 0 && !q.empty() && q[0] != 0);
    p = fps_mod(std::move(p), q); // 该方法要求 deg(P) < deg(Q)。
    while (n > 0) {
        if (p.empty()) return 0;
        Poly q_neg = q;
        for (int i = 1; i < (int)q_neg.size(); i += 2) {
            if (q_neg[i]) q_neg[i] = FPS_MOD - q_neg[i]; // 公式：Q(-x)。
        }
        Poly numerator = fps_mul(p, q_neg);
        Poly denominator = fps_mul(q, q_neg); // Q(x)Q(-x)，只剩偶次幂。

        Poly next_p, next_q;
        int parity = (int)(n & 1);
        for (int i = parity; i < (int)numerator.size(); i += 2) next_p.push_back(numerator[i]);
        for (int i = 0; i < (int)denominator.size(); i += 2) next_q.push_back(denominator[i]);
        p.swap(next_p);
        q.swap(next_q);
        n >>= 1;
    }
    return p.empty() ? 0 : (int)(1LL * p[0] * fps_powmod(q[0], FPS_MOD - 2) % FPS_MOD);
}

// 对于 a_n=c[0]a_{n-1}+...+c[k-1]a_{n-k}，先构造：
// 公式：Q={1,-c[0],-c[1],...,-c[k-1]} (mod FPS_MOD)。
// 对 0<=i<k，P[i]=a[i]-sum_{j=1..i} c[j-1]*a[i-j]，
// 然后调用 bostan_mori_nth(P,Q,n)。
```
#### 自适应辛普森积分：连续函数数值积分

> **赛时先看**
> - **题目信号**：题面给出函数公式，要求曲线下面积、概率密度积分、旋转体体积积分等；误差要求类似 `1e-6`。
> - **本质**：计算连续函数在 `[l,r]` 上的数值积分。适合函数光滑、无法直接求原函数、答案允许浮点误差的题。
> - **接法**：把题目函数写进 lambda `f`，调用 `adaptive_simpson(f,l,r,eps)`。若积分区间很长且函数振荡，先按关键点拆成多段分别积分。
> - **复杂度判定**：取决于函数形状和误差要求，通常递归次数不大。
> - **维护的量**：`f`（被积函数）；`l`、`r`（积分区间）；`eps`（精度要求）；`max_depth`（递归深度上限）。
> - **警告**：函数不连续、尖峰很多或区间极大时辛普森可能不稳定；递归层数要限制，避免极端数据爆栈。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 题目：求 sin(x) 在 [0, pi] 上的积分（真实值 2），误差要求 1e-6。
auto f = [](double x) { return sin(x); };
double ans = adaptive_simpson(f, 0.0, 3.141592653589793, 1e-6); // ans ≈ 2.000000
// 验证：样例 ans ≈ 2.0（误差 < 1e-6）
```

**传参要求（照这个传不会错）：**

- `adaptive_simpson(f, l, r, eps = 1e-8, max_depth = 30)`：f 为 double(double) 的可调用对象（lambda/函数指针都行）；l < r；返回 [l, r] 上积分的 double 近似值，误差量级约 eps。
- `simpson(f, l, r)`：单段辛普森公式，自适应递归内部用，一般不直接调。
- 函数不光滑/尖峰多/区间很长时先按关键点拆段，或调大 max_depth；注意深度上限防爆栈。

```cpp
template <class F>
double simpson(const F& f, double l, double r) {
    double m = (l + r) / 2;
    return (f(l) + 4 * f(m) + f(r)) * (r - l) / 6;
}

template <class F>
double adaptive_simpson_dfs(const F& f, double l, double r, double whole,
                            double eps, int depth) {
    double m = (l + r) / 2;
    double left = simpson(f, l, m);
    double right = simpson(f, m, r);
    double delta = left + right - whole;
    if (depth <= 0 || fabs(delta) <= 15 * eps) {
        return left + right + delta / 15;
    }
    return adaptive_simpson_dfs(f, l, m, left, eps / 2, depth - 1)
         + adaptive_simpson_dfs(f, m, r, right, eps / 2, depth - 1);
}

template <class F>
double adaptive_simpson(const F& f, double l, double r,
                        double eps = 1e-8, int max_depth = 30) {
    return adaptive_simpson_dfs(f, l, r, simpson(f, l, r), eps, max_depth);
}
```

### 12 计算几何、扫描线与空间结构

几何先放点线面基础，再放多边形、凸包、闵可夫斯基和、旋转卡壳、圆、半平面交和空间查询；扫描线典题放在本章末尾。
#### 点与向量基础

> **赛时先看**
> - **题目信号**：坐标、点线圆、多边形、凸包。
> - **本质**：几何题的基础类型。
> - **接法**：几何题先复制这一段基础类型，后面的凸包、线段、圆都会用到 `Point/dot/cross/dist`。整数坐标但会做距离和角度时用 `double`；如果只比较方向和面积，改成 `i64` 叉积会更稳。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：`Point` 存二维坐标 `x/y`（double）；`EPS=1e-9` 配合 `sgn` 统一做浮点比较。
> - **警告**：浮点比较用 EPS，不要直接 `==`。


**最小完整示例（先抄这一段就能跑）：**
> 题目：给两点坐标，求距离与叉积（判断转向）。
```cpp
Point a{0, 0}, b{3, 4};
printf("%.10f\n", dist(a, b));  // 距离 = 5
printf("%.10f\n", cross(a, b)); // 叉积 = 0（a、b 共线）
```
> 样例输出：`5.0000000000`；`0.0000000000`。

**传参要求（照这个传不会错）：**
- `Point(x, y)`：坐标，`double`；点与向量都用它表示，支持 `+ - * /` 运算。
- `dot(a, b)`：点积，返回 `double`。
- `cross(a, b)`：二维叉积，返回 `double`；`>0` 表示 `b` 在 `a` 的逆时针方向。
- `cross(a, b, c)`：`(b-a)×(c-a)`，判断三点转向；`>0` 为逆时针。
- `norm2(a) / norm(a) / dist(a,b)`：模长平方 / 模长 / 两点距离，均返回 `double`。
- `sgn(x)`：`x>EPS` 返回 `1`，`x<-EPS` 返回 `-1`，否则 `0`。

```cpp
const double EPS = 1e-9;
const double PI = acos(-1.0);

int sgn(double x) {
    return (x > EPS) - (x < -EPS);
}

struct Point {
    double x = 0, y = 0;
    Point() = default;
    Point(double x_, double y_) : x(x_), y(y_) {}
    Point operator+(const Point& o) const { return {x + o.x, y + o.y}; }
    Point operator-(const Point& o) const { return {x - o.x, y - o.y}; }
    Point operator*(double k) const { return {x * k, y * k}; }
    Point operator/(double k) const { return {x / k, y / k}; }
    bool operator<(const Point& o) const {
        if (sgn(x - o.x) != 0) return x < o.x;
        if (sgn(y - o.y) != 0) return y < o.y;
        return false;
    }
    bool operator==(const Point& o) const {
        return sgn(x - o.x) == 0 && sgn(y - o.y) == 0;
    }
};

double dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }
double cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
double cross(Point a, Point b, Point c) { return cross(b - a, c - a); }
double norm2(Point a) { return dot(a, a); }
double norm(Point a) { return sqrt(norm2(a)); }
double dist(Point a, Point b) { return norm(a - b); }
```
#### 三维点、向量、平面与体积

> **赛时先看**
> - **题目信号**：坐标出现 `(x,y,z)`，题面提到平面法向量、四面体/体积、直线射线与平面相交、是否共面。
> - **本质**：处理点到平面距离、直线和平面交点、夹角、四面体体积、共面判断等三维几何基本操作。
> - **接法**：四面体 `ABCD` 的体积为 `tetrahedron_volume6(A,B,C,D) / 6`；点 `Q` 到平面 `ABC` 的距离中，令 `Plane3 pl{A, cross3(B-A, C-A)}` 后调用 `distance_to_plane(Q, pl)`。若求线段和三角形/平面是否相交，先用 `line_plane_intersection` 求参数 `t`，再判断 `0 <= t <= 1` 与交点是否在三角形内。
> - **复杂度判定**：每个基本操作 `O(1)`。
> - **维护的量**：`Point3` 存 `(x,y,z)`；`Plane3` 维护平面上一点 `p` 与法向量 `n`；`EPS3=1e-10` 用于三维浮点判断。
> - **警告**：浮点几何统一用 EPS 判断零；法向量不能是零向量；直线与平面平行时没有唯一交点。体积公式的标量三重积有正负，真正体积要取绝对值再除以 `6`。


**最小完整示例（先抄这一段就能跑）：**
> 题目：求四面体 ABCD 的体积，以及点 Q 到平面 ABC 的距离。
```cpp
Point3 A{0,0,0}, B{1,0,0}, C{0,1,0}, D{0,0,1}, Q{1,1,1};
printf("%.10f\n", tetrahedron_volume6(A,B,C,D) / 6); // 体积 = 1/6
Plane3 pl{A, cross3(B-A, C-A)};                      // 平面 z=0
printf("%.10f\n", distance_to_plane(Q, pl));         // 距离 = 1
```
> 样例输出：`0.1666666667`；`1.0000000000`。

**传参要求（照这个传不会错）：**
- `dot3(a,b)`：三维点积，返回 `double`。
- `cross3(a,b)`：三维叉积，返回 `Point3`（法向量）。
- `norm3(a) / norm2_3(a)`：模长 / 模长平方，返回 `double`。
- `scalar_triple(a,b,c)`：标量三重积 `dot3(a, cross3(b,c))`，有正负，返回 `double`。
- `Plane3{p, n}`：`p` 为平面上一点，`n` 为非零法向量。
- `signed_distance_to_plane(q, pl) / distance_to_plane(q, pl)`：点到平面有符号 / 无符号距离。
- `line_plane_intersection(a, v, pl, out)`：直线 `a+t*v` 与平面交点，平行返回 `false`，交点写入 `out`。
- `tetrahedron_volume6(a,b,c,d)`：四面体体积的 6 倍（已取绝对值），再 `/6` 得体积。
- `coplanar(a,b,c,d)`：四点共面返回 `true`。

**API / 入口函数（赛时只认这里列的名字）：**

- `line_plane_intersection(Point3 a, Point3 v, const Plane3& pl, Point3& out)` -> 直线为 a+t*v；若直线与平面平行则返回 false。
**改板时先认这几个量：**

- `p`：平面上的一个点。
- `n`：非零法向量。

```cpp
const double EPS3 = 1e-10;

struct Point3 {
    double x = 0, y = 0, z = 0;
    Point3() = default;
    Point3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    Point3 operator+(const Point3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    Point3 operator-(const Point3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    Point3 operator*(double k) const { return {x * k, y * k, z * k}; }
    Point3 operator/(double k) const { return {x / k, y / k, z / k}; }
};

double dot3(Point3 a, Point3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point3 cross3(Point3 a, Point3 b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

double norm2_3(Point3 a) { return dot3(a, a); }
double norm3(Point3 a) { return sqrt(norm2_3(a)); }
double scalar_triple(Point3 a, Point3 b, Point3 c) { return dot3(a, cross3(b, c)); }

struct Plane3 {
    Point3 p; // 平面上的一个点。
    Point3 n; // 非零法向量。
};

double signed_distance_to_plane(Point3 q, const Plane3& pl) {
    return dot3(q - pl.p, pl.n) / norm3(pl.n);
}

double distance_to_plane(Point3 q, const Plane3& pl) {
    return fabs(signed_distance_to_plane(q, pl));
}

// 直线为 a+t*v；若直线与平面平行则返回 false。
bool line_plane_intersection(Point3 a, Point3 v, const Plane3& pl, Point3& out) {
    double den = dot3(v, pl.n);
    if (fabs(den) <= EPS3) return false;
    double t = dot3(pl.p - a, pl.n) / den;
    out = a + v * t;
    return true;
}

double tetrahedron_volume6(Point3 a, Point3 b, Point3 c, Point3 d) {
    return fabs(scalar_triple(b - a, c - a, d - a));
}

bool coplanar(Point3 a, Point3 b, Point3 c, Point3 d) {
    return fabs(scalar_triple(b - a, c - a, d - a)) <= EPS3;
}
```

典题模型：四面体 `ABCD` 的体积为 `tetrahedron_volume6(A,B,C,D) / 6`；点 `Q` 到平面 `ABC` 的距离中，令 `Plane3 pl{A, cross3(B-A, C-A)}` 后调用 `distance_to_plane(Q, pl)`。若求线段和三角形/平面是否相交，先用 `line_plane_intersection` 求参数 `t`，再判断 `0 <= t <= 1` 与交点是否在三角形内。
#### 六边形网格 cube 坐标

> **赛时先看**
> - **题目信号**：坐标满足 `x+y+z=0`；每个点有 6 个邻居；题面给三维坐标描述六边形网格。
> - **本质**：六边形网格距离、邻居、环、翻转周围一圈等实现。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再调用其公开接口。
> - **复杂度判定**：邻居 `O(1)`，半径为 `r` 的环 `O(r)`。
> - **维护的量**：`Hex` 维护三元坐标 `x/y/z`（保证 `x+y+z=0`）；`HEX_DIR` 是六个方向的常量表。
> - **警告**：读入时断言 `x+y+z=0`；六边形距离是三维差绝对值之和除以 2。


**最小完整示例（先抄这一段就能跑）：**
> 题目：求六边形网格上两点的距离，以及某点的邻居数。
```cpp
Hex a{0, 0, 0}, b{2, -1, -1};
printf("%lld\n", hex_distance(a, b));  // 距离 = 2
auto ns = hex_neighbors(a);
printf("%zu\n", ns.size());            // 邻居数 = 6
```
> 样例输出：`2`；`6`。

**传参要求（照这个传不会错）：**
- `Hex{x, y, z}`：三元坐标，`i64`，必须满足 `x+y+z=0`。
- `HEX_DIR`：`array<Hex,6>`，下标 `0..5` 依次是六个方向。
- `hex_distance(a, b)`：返回 `i64` 距离，`(abs(dx)+abs(dy)+abs(dz))/2`。
- `hex_neighbors(c)`：返回 `vector<Hex>`，共 6 个邻居。
- `hex_ring(c, radius)`：返回 `vector<Hex>`，半径 `radius` 的环；`radius=0` 时只含 `c` 自己。

```cpp
struct Hex {
    i64 x = 0, y = 0, z = 0;

    bool operator==(const Hex& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    Hex operator+(const Hex& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Hex operator*(i64 k) const {
        return {x * k, y * k, z * k};
    }
};

const array<Hex, 6> HEX_DIR = {{
    {0, -1, 1},
    {1, -1, 0},
    {1, 0, -1},
    {0, 1, -1},
    {-1, 1, 0},
    {-1, 0, 1},
}};

i64 hex_distance(Hex a, Hex b) {
    return (llabs(a.x - b.x) + llabs(a.y - b.y) + llabs(a.z - b.z)) / 2;
}

vector<Hex> hex_neighbors(Hex c) {
    vector<Hex> res;
    for (auto d : HEX_DIR) res.push_back(c + d);
    return res;
}

vector<Hex> hex_ring(Hex c, i64 radius) {
    vector<Hex> res;
    if (radius == 0) {
        res.push_back(c);
        return res;
    }
    Hex cur = c + HEX_DIR[4] * radius;
    for (int side = 0; side < 6; side++) {
        for (int step = 0; step < radius; step++) {
            res.push_back(cur);
            cur = cur + HEX_DIR[side];
        }
    }
    return res;
}
```
#### 线段相交与点到线段距离

> **赛时先看**
> - **题目信号**：线段、相交、距离、投影。
> - **本质**：判断路径碰撞、几何约束、最近距离。
> - **接法**：判断两条线段是否碰撞用 `segment_intersect(a1,a2,b1,b2)`；求点到线段最短距离用 `distance_point_to_segment(p,a,b)`。题目如果是整数网格且只问是否相交，注意端点接触通常也算相交，模板已经包含边界。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：无额外结构，只依赖基础 `Point` 与 `dot/cross/norm/sgn`、`EPS`。
> - **警告**：共线重叠要用 `on_segment` 处理。


**最小完整示例（先抄这一段就能跑）：**
> 题目：判断两条线段是否相交；求点 P 到线段 AB 的最短距离。
> 依赖：点与向量基础（`Point` / `dot` / `cross` / `norm` / `dist` / `sgn` / `EPS`），抄板时一起抄上。
```cpp
Point A{0,0}, B{2,2}, C{0,2}, D{2,0}, P{1,0};
printf("%d\n", segment_intersect(A, B, C, D));         // 相交 = 1
printf("%.10f\n", distance_point_to_segment(P, A, B)); // 距离 = sqrt(2)/2
```
> 样例输出：`1`；`0.7071067812`。

**传参要求（照这个传不会错）：**
- `on_segment(p, a, b)`：`p` 是否在线段 `ab` 上（含端点），返回 `bool`。
- `segment_intersect(a1, a2, b1, b2)`：两线段 `a1a2`、`b1b2` 是否相交（含端点接触与共线重叠），返回 `bool`。
- `distance_point_to_line(p, a, b)`：点 `p` 到直线 `ab` 的距离，返回 `double`；要求 `a`、`b` 不重合。
- `distance_point_to_segment(p, a, b)`：点 `p` 到线段 `ab` 的最短距离，返回 `double`（已处理垂足落在线段外的情况）。

```cpp
bool on_segment(Point p, Point a, Point b) {
    return sgn(cross(a, b, p)) == 0 && sgn(dot(p - a, p - b)) <= 0;
}

bool segment_intersect(Point a1, Point a2, Point b1, Point b2) {
    double c1 = cross(a1, a2, b1), c2 = cross(a1, a2, b2);
    double c3 = cross(b1, b2, a1), c4 = cross(b1, b2, a2);
    if (sgn(c1) * sgn(c2) < 0 && sgn(c3) * sgn(c4) < 0) return true;
    return on_segment(b1, a1, a2) || on_segment(b2, a1, a2) ||
           on_segment(a1, b1, b2) || on_segment(a2, b1, b2);
}

double distance_point_to_line(Point p, Point a, Point b) {
    return fabs(cross(b - a, p - a)) / norm(b - a);
}

double distance_point_to_segment(Point p, Point a, Point b) {
    if (sgn(dot(b - a, p - a)) < 0) return dist(p, a);
    if (sgn(dot(a - b, p - b)) < 0) return dist(p, b);
    return distance_point_to_line(p, a, b);
}
```
#### 多边形面积与点包含

> **赛时先看**
> - **题目信号**：围栏、区域、内部/外部判断。
> - **本质**：多边形面积、判断点在多边形内。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：只维护输入多边形顶点序列（按序的 `vector<Point>`，`poly` / `p`）。
> - **警告**：`polygon_area2` 返回两倍有向面积（真实面积需除以 2）；`point_in_polygon` 对边界返回 1、内部 2、外部 0，判断时要单独区分边界。


**最小完整示例（先抄这一段就能跑）：**

题目：求三角形面积，并判断点是否在三角形内部。

依赖：点与向量基础（`Point` / `cross` / `sgn`）+ 线段相交与点到线段距离（`on_segment`），抄板时一起抄上。

```cpp
vector<Point> tri{{0,0},{2,0},{0,2}};
printf("%.10f\n", polygon_area2(tri) / 2);  // 面积 = 2
printf("%d\n", point_in_polygon({1,0.5}, tri)); // 内部 = 2
printf("%d\n", point_in_polygon({3,3}, tri)); // 外部 = 0
```

样例输出：`2.0000000000`；`2`；`0`。

**传参要求（照这个传不会错）：**
- `polygon_area2(p)`：`p` 为按序顶点（`vector<Point>`，顺逆皆可）；返回 2 倍有向面积（`double`），真实面积取 `fabs（结果）/2`。
- `point_in_polygon(q, poly)`：`q` 为查询点，`poly` 为按序顶点（顺逆皆可）；返回 `0`=外部、`1`=边界（含顶点与边上）、`2`=内部。

```cpp
double polygon_area2(const vector<Point>& p) {
    double s = 0;
    int n = (int)p.size();
    for (int i = 0; i < n; ++i) s += cross(p[i], p[(i + 1) % n]);
    return s;
}

int point_in_polygon(Point q, const vector<Point>& poly) {
    bool in = false;
    int n = (int)poly.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        Point a = poly[j], b = poly[i];
        if (on_segment(q, a, b)) return 1; // 点在边界上。
        bool cross_ray = ((a.y > q.y) != (b.y > q.y)) &&
                         (q.x < (b.x - a.x) * (q.y - a.y) / (b.y - a.y) + a.x);
        if (cross_ray) in = !in;
    }
    return in ? 2 : 0; // 0 表示外部，2 表示内部。
}
```
#### 简单多边形耳切三角剖分

> **赛时先看**
> - **题目信号**：多边形是简单多边形但不一定凸；后续算法只会处理三角形或凸多边形；`n` 不大，`O(n^3)` 可接受。
> - **本质**：把简单多边形拆成 `n-2` 个三角形，常用于把非凸多边形问题转成三角形对/凸多边形问题。
> - **接法**：每轮枚举连续三点 `(prev, cur, next)`，若它是凸角，且其它点不在该三角形内部，就切下这个三角形并删除 `cur`。
> - **复杂度判定**：朴素耳切 `O(n^3)`；精细维护可做到 `O(n^2)`。
> - **维护的量**：维护存活顶点下标表 `alive` 与结果 `triangles`（`n-2` 个三角形）。
> - **警告**：输入点最好统一成逆时针；实现只检查凸角与三角形内无其他顶点；对简单多边形该条件等价于耳点判定；共线点会让实现变麻烦，必要时先删除连续共线点。


**最小完整示例（先抄这一段就能跑）：**
> 题目：把凸四边形剖成三角形，输出三角形个数。
```cpp
vector<EarPoint> poly{{0,0},{2,0},{2,2},{0,2}};
auto t = ear_clipping_triangulation(poly);
printf("%zu\n", t.size());  // 三角形数 = n-2 = 2
```
> 样例输出：`2`。

**传参要求（照这个传不会错）：**
- `EarPoint(x, y)`：顶点，`long double`，读入用 `long double` 类型。
- `ear_clipping_triangulation(poly)`：输入简单多边形顶点（`vector<EarPoint>`，顺序任意，内部自动翻为逆时针）；返回 `vector<array<EarPoint,3>>` 共 `n-2` 个三角形；要求 `n>=3` 且无重复点、无连续共线点，否则 `assert` 失败。
- `point_in_triangle_strict(p, a, b, c)`：`p` 是否严格在三角形 `abc` 内部，返回 `bool`（不统计边界）。
- `polygon_signed_area2(p)`：2 倍有向面积，负值表示顺时针。

```cpp
struct EarPoint {
    long double x = 0, y = 0;
    EarPoint() = default;
    EarPoint(long double x_, long double y_) : x(x_), y(y_) {}
    EarPoint operator+(const EarPoint& o) const { return {x + o.x, y + o.y}; }
    EarPoint operator-(const EarPoint& o) const { return {x - o.x, y - o.y}; }
};

long double cross(EarPoint a, EarPoint b) { return a.x * b.y - a.y * b.x; }
long double cross(EarPoint a, EarPoint b, EarPoint c) { return cross(b - a, c - a); }

long double polygon_signed_area2(const vector<EarPoint>& p) {
    long double s = 0;
    for (int i = 0; i < (int)p.size(); ++i) s += cross(p[i], p[(i + 1) % p.size()]);
    return s;
}

bool point_in_triangle_strict(EarPoint p, EarPoint a, EarPoint b, EarPoint c) {
    long double c1 = cross(a, b, p);
    long double c2 = cross(b, c, p);
    long double c3 = cross(c, a, p);
    return c1 > 1e-12L && c2 > 1e-12L && c3 > 1e-12L;
}

vector<array<EarPoint, 3>> ear_clipping_triangulation(vector<EarPoint> poly) {
    int n = (int)poly.size();
    assert(n >= 3);
    if (polygon_signed_area2(poly) < 0) reverse(poly.begin(), poly.end());
    vector<int> alive(n);
    iota(alive.begin(), alive.end(), 0);
    vector<array<EarPoint, 3>> triangles;

    while ((int)alive.size() > 3) {
        bool cut = false;
        int m = (int)alive.size();
        for (int i = 0; i < m && !cut; ++i) {
            int ia = alive[(i - 1 + m) % m];
            int ib = alive[i];
            int ic = alive[(i + 1) % m];
            EarPoint a = poly[ia], b = poly[ib], c = poly[ic];
            if (cross(a, b, c) <= 1e-12L) continue;
            bool has_inside = false;
            for (int id : alive) {
                if (id == ia || id == ib || id == ic) continue;
                if (point_in_triangle_strict(poly[id], a, b, c)) {
                    has_inside = true;
                    break;
                }
            }
            if (!has_inside) {
                triangles.push_back({a, b, c});
                alive.erase(alive.begin() + i);
                cut = true;
            }
        }
        assert(cut); // 若失败，通常是有重复点、共线点或输入不是简单多边形。
    }
    triangles.push_back({poly[alive[0]], poly[alive[1]], poly[alive[2]]});
    return triangles;
}
```

典题：本场 K《Geometry Textbook》。先对两个简单多边形分别耳切，枚举三角形对；每对三角形的闵可夫斯基和是凸多边形，再求这些凸多边形的面积并。
#### Andrew 凸包

> **赛时先看**
> - **题目信号**：最大距离、包围所有点、凸多边形。
> - **本质**：点集最外层边界。
> - **接法**：点集外壳、最远点对、凸多边形处理前，先 `convex_hull(points)`。返回点按凸包顺序排列，不重复首点；如果返回点数小于 `3`，很多多边形算法要单独处理。
> - **复杂度判定**：`O(n log n)`。
> - **维护的量**：维护凸壳栈 `h`（下凸壳 + 上凸壳拼接）；返回值即按凸包顺序排列的顶点。
> - **警告**：如果要保留边上的共线点，把 `<= 0` 改成 `< 0`。


**最小完整示例（先抄这一段就能跑）：**
> 题目：给 4 个点求凸包，输出凸包点数。
> 依赖：点与向量基础（`Point` / `cross` / `sgn`），抄板时一起抄上。
```cpp
vector<Point> pts{{0,0},{1,0},{0,1},{1,1}};
auto hull = convex_hull(pts);
printf("%zu\n", hull.size());  // 凸包点数 = 4
```
> 样例输出：`4`。

**传参要求（照这个传不会错）：**
- `convex_hull(p)`：输入任意点集（`vector<Point>`，可含重复点，内部自动去重）；返回 `vector<Point>`，逆时针凸包顶点，首点不重复；点数 `<=1` 时原样返回；返回点数 `<3` 时不是多边形，后续多边形算法需单独处理。

**不会用就照抄：**

```cpp
auto hull = convex_hull(points);
```

- 先看本节是否保留共线边界点；这会影响周长、点数、后续旋转卡壳。
- 点类型和叉积类型不要临时改窄，坐标大时用 `i64/i128`。


**API / 入口函数（赛时只认这里列的名字）：**

- `convex_hull(vector<Point> p)` -> Andrew 凸包 返回 `vector<Point>`。
**核心逻辑（改代码时别破坏）：**

- 先按坐标排序；分别维护下凸壳和上凸壳。
- 出现非期望转向就弹栈；`<=0` 会丢掉边上共线点，改成 `<0` 可保留。

```cpp
vector<Point> convex_hull(vector<Point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    if (p.size() <= 1) return p;

    vector<Point> h;
    for (auto pt : p) {
        while (h.size() >= 2 && sgn(cross(h[h.size() - 2], h.back(), pt)) <= 0) h.pop_back();
        h.push_back(pt);
    }
    int lower = (int)h.size();
    for (int i = (int)p.size() - 2; i >= 0; --i) {
        while ((int)h.size() > lower && sgn(cross(h[h.size() - 2], h.back(), p[i])) <= 0) h.pop_back();
        h.push_back(p[i]);
    }
    h.pop_back();
    return h;
}
```
#### 凸多边形闵可夫斯基和

> **赛时先看**
> - **题目信号**：题目给出两个凸包，出现“所有两点坐标相加/相减”“边按极角合并”“凸多边形平移后是否相交”等描述；若要判断 `A` 与 `B` 的交叠，常构造 `A + (-B)` 再判断原点位置。
> - **本质**：求两个凸多边形的和集 `A + B = {a + b | a in A, b in B}`；常用来把两个物体的相对运动、凸多边形碰撞或“两个凸集合点对差”转成一次凸包操作。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：输入顶点按逆时针顺序给出时为 `O(n + m)`；若还需要先求凸包，合计 `O((n + m) log(n + m))`。
> - **维护的量**：维护 `IPoint`（`i64` 整数坐标）与 `i128` 叉积 `cross_i`；结果和集顶点序列 `ans`。
> - **警告**：代码使用整数坐标和 `i128` 叉积，避免乘法溢出；输入必须是非退化、严格凸的凸包，顶点可从任意位置开始。若输入只是散点，先用 Andrew 凸包；若题目允许边上连续共线点，最好先删去共线中间点。


**最小完整示例（先抄这一段就能跑）：**
> 题目：三角形与三角形求闵可夫斯基和，输出和集顶点数。
```cpp
vector<IPoint> a{{0,0},{2,0},{0,2}};
vector<IPoint> b{{0,0},{1,0},{0,1}};
auto s = minkowski_sum_convex(a, b);
printf("%zu\n", s.size());  // 和集顶点数 = 6（六边形）
```
> 样例输出：`6`。

**传参要求（照这个传不会错）：**
- `IPoint(x, y)`：整数坐标，`i64`。
- `minkowski_sum_convex(a, b)`：输入两个严格凸、非退化凸多边形顶点（`vector<IPoint>`，顺序任意，内部自动调 `normalize_convex_polygon`）；返回 `vector<IPoint>` 逆时针和集顶点；单个点（退化）也支持。
- `normalize_convex_polygon(p)`：把多边形翻为逆时针，并让最低、再最靠左的顶点做 0 号位置，返回 `vector<IPoint>`。
- `polygon_area2_i(p)`：2 倍有向面积，`i128`。
- `cross_i(a, b)`：`i128` 叉积，防溢出。

**API / 入口函数（赛时只认这里列的名字）：**

- `minkowski_sum_convex(vector<IPoint> a, vector<IPoint> b)` -> 求两个凸多边形（非退化、严格凸）的闵可夫斯基和，返回顶点序列。返回前会自动调 `normalize_convex_polygon` 规范化输入。
- `normalize_convex_polygon(vector<IPoint> p)` -> 把多边形转成逆时针，并把最低、再最靠左的点旋到 0 号位置。 返回 `vector<IPoint>`。

```cpp
struct IPoint {
    i64 x = 0, y = 0;
    IPoint() = default;
    IPoint(i64 x_, i64 y_) : x(x_), y(y_) {}
    IPoint operator+(const IPoint& o) const { return {x + o.x, y + o.y}; }
    IPoint operator-(const IPoint& o) const { return {x - o.x, y - o.y}; }
    bool operator==(const IPoint& o) const { return x == o.x && y == o.y; }
};


i128 cross_i(IPoint a, IPoint b) {
    return (i128)a.x * b.y - (i128)a.y * b.x;
}

i128 polygon_area2_i(const vector<IPoint>& p) {
    i128 s = 0;
    for (int i = 0; i < (int)p.size(); ++i) s += cross_i(p[i], p[(i + 1) % p.size()]);
    return s;
}

// 把多边形转成逆时针，并把最低、再最靠左的点旋到 0 号位置。
vector<IPoint> normalize_convex_polygon(vector<IPoint> p) {
    if (p.size() > 1 && p.front() == p.back()) p.pop_back();
    if (p.size() <= 1) return p;
    if (polygon_area2_i(p) < 0) reverse(p.begin(), p.end());
    int start = 0;
    for (int i = 1; i < (int)p.size(); ++i) {
        if (p[i].y < p[start].y || (p[i].y == p[start].y && p[i].x < p[start].x)) {
            start = i;
        }
    }
    rotate(p.begin(), p.begin() + start, p.end());
    return p;
}

vector<IPoint> minkowski_sum_convex(vector<IPoint> a, vector<IPoint> b) {
    a = normalize_convex_polygon(a);
    b = normalize_convex_polygon(b);
    if (a.empty() || b.empty()) return {};
    if (a.size() == 1) {
        for (auto& p : b) p = p + a[0];
        return b;
    }
    if (b.size() == 1) {
        for (auto& p : a) p = p + b[0];
        return a;
    }

    int n = (int)a.size(), m = (int)b.size();
    vector<IPoint> ans;
    ans.reserve(n + m);
    ans.push_back(a[0] + b[0]);
    int i = 0, j = 0;
    while (i < n || j < m) {
        IPoint da = (i < n ? a[(i + 1) % n] - a[i] : IPoint{});
        IPoint db = (j < m ? b[(j + 1) % m] - b[j] : IPoint{});
        if (j == m || (i < n && cross_i(da, db) > 0)) {
            ans.push_back(ans.back() + da);
            ++i;
        } else if (i == n || cross_i(da, db) < 0) {
            ans.push_back(ans.back() + db);
            ++j;
        } else {
            ans.push_back(ans.back() + da + db);
            ++i;
            ++j;
        }
    }
    ans.pop_back(); // 最终累加得到的点应等于 ans[0]。
    return ans;
}
```

典型模型：若 `B` 中每个点取相反数并保持逆时针顺序，就得到 `A + (-B)`。原点在其中（边界也算）等价于 `A`、`B` 有公共点；这是凸多边形相交和“相对位移可行性”最常用的变形。点在凸多边形内可直接接本册已有的 `point_in_convex_polygon`。
#### 旋转卡壳：凸包直径

> **赛时先看**
> - **题目信号**：点集最大距离，先求凸包再处理。
> - **本质**：凸包上最远点对。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：凸包后 `O(n)`。
> - **维护的量**：维护对踵点指针 `j` 与当前最优距离平方 `ans`。
> - **警告**：输入必须是凸包顺序。`convex_diameter2` 返回的是距离平方，输出时需开根。


**最小完整示例（先抄这一段就能跑）：**
> 题目：求凸包上最远点对的距离（已先求好凸包）。
> 依赖：点与向量基础（`Point` / `cross` / `norm2` / `sgn` / `EPS`），抄板时一起抄上。
```cpp
vector<Point> p{{0,0},{4,0},{4,3}};
printf("%.10f\n", sqrt(convex_diameter2(p)));  // 直径 = 5
```
> 样例输出：`5.0000000000`。

**传参要求（照这个传不会错）：**
- `convex_diameter2(p)`：输入凸包顶点（`vector<Point>`，逆时针、首点不重复）；返回最远点对距离的平方（`double`），输出时 `sqrt`；`n<=1` 返回 `0`，`n==2` 返回两点距离平方。

```cpp
double convex_diameter2(const vector<Point>& p) {
    int n = (int)p.size();
    if (n <= 1) return 0;
    if (n == 2) return norm2(p[0] - p[1]);
    double ans = 0;
    int j = 1;
    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;
        while (fabs(cross(p[ni] - p[i], p[(j + 1) % n] - p[i])) >
               fabs(cross(p[ni] - p[i], p[j] - p[i])) + EPS) {
            j = (j + 1) % n;
        }
        ans = max(ans, norm2(p[i] - p[j]));
        ans = max(ans, norm2(p[ni] - p[j]));
    }
    return ans;
}
```
#### 最近点对

> **赛时先看**
> - **题目信号**：`n` 到 `1e5`，问最近距离，暴力不行。
> - **本质**：平面点集中最近两点距离。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(n log n)`。
> - **维护的量**：维护归并缓冲 `tmp` 与递归区间 `[l, r)`；全程只传距离平方避免开根误差。
> - **警告**：返回距离平方更稳；有重点时答案为 0。


**最小完整示例（先抄这一段就能跑）：**
> 题目：求平面点集中最近点对的距离。
> 依赖：点与向量基础（`Point` / `sgn` / `norm2`），抄板时一起抄上。
```cpp
vector<Point> p{{0,0},{3,4},{1,0},{100,100}};
printf("%.10f\n", sqrt(closest_pair(p)));  // 最近距离 = 1
```
> 样例输出：`1.0000000000`。

**传参要求（照这个传不会错）：**
- `closest_pair(p)`：输入任意点集（`vector<Point>`，内部会排序，不修改外部语义）；返回最近点对距离平方（`double`），输出时 `sqrt`；有重复点返回 `0`；`n<=1` 返回 `1e100`。
- `closest_pair_rec(p, tmp, l, r)`：内部递归函数，处理 `[l, r)` 区间，`tmp` 为同大小缓冲；不要从 `solve()` 直接调用，一律走 `closest_pair`。

```cpp
double closest_pair_rec(vector<Point>& p, vector<Point>& tmp, int l, int r) {
    if (r - l <= 3) {
        double best = 1e100;
        for (int i = l; i < r; ++i) {
            for (int j = i + 1; j < r; ++j) best = min(best, norm2(p[i] - p[j]));
        }
        sort(p.begin() + l, p.begin() + r, [](Point a, Point b) { return a.y < b.y; });
        return best;
    }
    int m = (l + r) >> 1;
    double midx = p[m].x;
    double best = min(closest_pair_rec(p, tmp, l, m), closest_pair_rec(p, tmp, m, r));
    merge(p.begin() + l, p.begin() + m, p.begin() + m, p.begin() + r, tmp.begin(),
          [](Point a, Point b) { return a.y < b.y; });
    copy(tmp.begin(), tmp.begin() + (r - l), p.begin() + l);

    int cnt = 0;
    for (int i = l; i < r; ++i) {
        if ((p[i].x - midx) * (p[i].x - midx) >= best) continue;
        for (int j = cnt - 1; j >= 0 && (p[i].y - tmp[j].y) * (p[i].y - tmp[j].y) < best; --j) {
            best = min(best, norm2(p[i] - tmp[j]));
        }
        tmp[cnt++] = p[i];
    }
    return best;
}

double closest_pair(vector<Point> p) {
    sort(p.begin(), p.end(), [](Point a, Point b) {
        if (sgn(a.x - b.x) != 0) return a.x < b.x;
        return a.y < b.y;
    });
    vector<Point> tmp(p.size());
    return closest_pair_rec(p, tmp, 0, (int)p.size());
}
```
#### 动态 K-D Tree：二维加点与矩形权值和

> **赛时先看**
> - **题目信号**：操作被答案异或或加密，无法离线；每次只有一个点增加权值，但查询是 `[x1,x2] x [y1,y2]` 的矩形和；坐标可到 `1e9` 或更大。
> - **本质**：强制在线维护二维带权点，支持插入/给某坐标加权，以及查询轴对齐矩形内的权值和。比二维 BIT、树套树更适合坐标范围极大且不能离线离散化的题。
> - **接法**：初始全零平面，在线执行 `1 x y a` 表示 `(x,y)` 增加 `a`，执行 `2 x1 y1 x2 y2` 询问矩形和。若每个操作参数都要与上一次答案异或，二维 BIT/CDQ 无法离线，这棵 K-D Tree 就可直接套用。
> - **复杂度判定**：替罪羊重构保证树高均摊 `O(log n)`；矩形查询依靠包围盒剪枝，均摊通常很快，但理论最坏仍可能 `O(n)`。这是 K-D Tree 的本质限制。
> - **维护的量**：维护节点池 `tr`（0 号为空节点）与树根 `root`；每节点维护子树权值和 `sum`、包围盒 `min_x/max_x/min_y/max_y`、子树大小 `sz`。
> - **警告**：这里同坐标插入会累加权值，不新增结点；查询端点是闭区间；权值和、坐标都使用 `i64`。模板只支持插入，不支持删除；有删除时需加懒删除计数并将“有效点比例过低”也作为重构条件。


**最小完整示例（先抄这一段就能跑）：**
> 题目：在线给点加权，询问矩形权值和。
```cpp
KDTree2D kd;
kd.add_point(1, 1, 5);
kd.add_point(2, 3, 7);
printf("%lld\n", kd.rectangle_sum(0, 0, 2, 2));  // 闭区间 [0,2]x[0,2] 的和 = 5
```
> 样例输出：`5`。

**传参要求（照这个传不会错）：**
- `KDTree2D kd;`：直接默认构造，内部从空树开始。
- `add_point(x, y, val)`：坐标为 `i64`（`1e9` 也直接可用）；在 `(x,y)` 累加 `val`，同坐标不新增结点。
- `rectangle_sum(x1, y1, x2, y2)`：闭区间矩形 `[x1,x2] × [y1,y2]` 的权值和，返回 `i64`；端点大小关系随意，内部自动 `swap` 纠正。
- 内部 `build / insert / query` 是递归实现细节，不要从 `solve()` 直接调。

**API / 入口函数（赛时只认这里列的名字）：**

- `add_point(i64 x, i64 y, i64 val)` -> 在 `(x,y)` 累加权值 `val`（同坐标不新增结点）。
- `rectangle_sum(i64 x1, i64 y1, i64 x2, i64 y2)` -> 闭区间矩形权值和 返回 `i64`。
- 内部 `build / insert / query` 是递归实现细节，不要从 `solve()` 直接调。
**改板时先认这几个量：**

- `sz`：子树大小。
- `tr`：树节点池（`tr[0]` 表示空节点）。
- `root`：树根节点编号。
- `depth`：深度。
- `sum`：子树权值和（含自身）。

```cpp
struct KDTree2D {
    static constexpr double ALPHA = 0.75;

    struct Node {
        i64 x, y, val, sum;
        i64 min_x, max_x, min_y, max_y;
        int ch[2] = {0, 0};
        int sz = 1;
    };

    vector<Node> tr = {Node{}}; // 0 号下标表示空节点。
    int root = 0;

    int size_of(int u) const { return u ? tr[u].sz : 0; }
    i64 sum_of(int u) const { return u ? tr[u].sum : 0; }

    int new_node(i64 x, i64 y, i64 val) {
        tr.push_back({x, y, val, val, x, x, y, y});
        return (int)tr.size() - 1;
    }

    void pull(int u) {
        Node& p = tr[u];
        p.sz = 1;
        p.sum = p.val;
        p.min_x = p.max_x = p.x;
        p.min_y = p.max_y = p.y;
        for (int v : p.ch) {
            if (!v) continue;
            p.sz += tr[v].sz;
            p.sum += tr[v].sum;
            p.min_x = min(p.min_x, tr[v].min_x);
            p.max_x = max(p.max_x, tr[v].max_x);
            p.min_y = min(p.min_y, tr[v].min_y);
            p.max_y = max(p.max_y, tr[v].max_y);
        }
    }

    void flatten(int u, vector<int>& ids) {
        if (!u) return;
        flatten(tr[u].ch[0], ids);
        ids.push_back(u);
        flatten(tr[u].ch[1], ids);
    }

    int build(vector<int>& ids, int l, int r, int depth) {
        if (l >= r) return 0;
        int mid = (l + r) >> 1;
        int axis = depth & 1;
        nth_element(ids.begin() + l, ids.begin() + mid, ids.begin() + r, [&](int a, int b) {
            if (axis == 0) {
                if (tr[a].x != tr[b].x) return tr[a].x < tr[b].x;
                return tr[a].y < tr[b].y;
            }
            if (tr[a].y != tr[b].y) return tr[a].y < tr[b].y;
            return tr[a].x < tr[b].x;
        });
        int u = ids[mid];
        tr[u].ch[0] = build(ids, l, mid, depth + 1);
        tr[u].ch[1] = build(ids, mid + 1, r, depth + 1);
        pull(u);
        return u;
    }

    bool unbalanced(int u) const {
        return max(size_of(tr[u].ch[0]), size_of(tr[u].ch[1])) > ALPHA * tr[u].sz;
    }

    void rebuild(int& u, int depth) {
        vector<int> ids;
        ids.reserve(tr[u].sz);
        flatten(u, ids);
        u = build(ids, 0, (int)ids.size(), depth);
    }

    void insert(int& u, i64 x, i64 y, i64 val, int depth) {
        if (!u) {
            u = new_node(x, y, val);
            return;
        }
        if (tr[u].x == x && tr[u].y == y) {
            tr[u].val += val;
            pull(u);
            return;
        }
        int axis = depth & 1;
        bool go_right = axis == 0 ? make_pair(x, y) > make_pair(tr[u].x, tr[u].y)
                                  : make_pair(y, x) > make_pair(tr[u].y, tr[u].x);
        // 不要把 tr[u].ch[...] 按引用传入：深层递归里的 push_back
        // 可能导致 tr 重新分配，使该引用失效。
        int child = tr[u].ch[go_right];
        insert(child, x, y, val, depth + 1);
        tr[u].ch[go_right] = child;
        pull(u);
        if (unbalanced(u)) rebuild(u, depth);
    }

    void add_point(i64 x, i64 y, i64 val) {
        insert(root, x, y, val, 0);
    }

    bool disjoint(int u, i64 x1, i64 y1, i64 x2, i64 y2) const {
        return tr[u].max_x < x1 || x2 < tr[u].min_x || tr[u].max_y < y1 || y2 < tr[u].min_y;
    }

    bool covered(int u, i64 x1, i64 y1, i64 x2, i64 y2) const {
        return x1 <= tr[u].min_x && tr[u].max_x <= x2 && y1 <= tr[u].min_y && tr[u].max_y <= y2;
    }

    i64 query(int u, i64 x1, i64 y1, i64 x2, i64 y2) const {
        if (!u || disjoint(u, x1, y1, x2, y2)) return 0;
        if (covered(u, x1, y1, x2, y2)) return tr[u].sum;
        i64 ans = (x1 <= tr[u].x && tr[u].x <= x2 && y1 <= tr[u].y && tr[u].y <= y2) ? tr[u].val : 0;
        return ans + query(tr[u].ch[0], x1, y1, x2, y2) + query(tr[u].ch[1], x1, y1, x2, y2);
    }

    i64 rectangle_sum(i64 x1, i64 y1, i64 x2, i64 y2) const {
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        return query(root, x1, y1, x2, y2);
    }
};
```

典题模型：初始全零平面，在线执行 `1 x y a` 表示 `(x,y)` 增加 `a`，执行 `2 x1 y1 x2 y2` 询问矩形和。若每个操作参数都要与上一次答案异或，二维 BIT/CDQ 无法离线，这棵 K-D Tree 就可直接套用。
#### 直线与圆交点

> **赛时先看**
> - **题目信号**：圆、半径、直线穿过圆、切线。
> - **本质**：圆相关几何基础。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再调用其公开接口。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：维护 `Line{p, v}`（参数式直线：点 + 方向向量）与 `Circle{o, r}`（圆心 + 半径）。
> - **警告**：先判断点到圆心距离和半径关系。


**最小完整示例（先抄这一段就能跑）：**
> 题目：求直线与圆的交点个数。
> 依赖：点与向量基础（`Point` / `dot` / `norm2` / `sgn` / `dist`），抄板时一起抄上。
```cpp
Line L{{0, 0}, {1, 0}};        // 直线 y = 0
Circle C{{0, 0}, 2};           // 圆心原点，半径 2
auto res = line_circle_intersection(L, C);
printf("%zu\n", res.size());   // 交点个数 = 2
```
> 样例输出：`2`。

**传参要求（照这个传不会错）：**
- `Line{p, v}`：`p` 为直线上一点，`v` 为非零方向向量，直线为 `p + t*v`。
- `Circle{o, r}`：`o` 为圆心，`r` 为半径（`double`）。
- `line_circle_intersection(line, c)`：返回 `vector<Point>`；0 个=不相交，1 个=相切（返回切点），2 个=相交（两个交点）。

```cpp
struct Line {
    Point p, v; // 直线参数式 p + t * v。
};

struct Circle {
    Point o;
    double r;
};

vector<Point> line_circle_intersection(Line line, Circle c) {
    Point p = line.p;
    Point v = line.v;
    Point foot = p + v * (dot(c.o - p, v) / norm2(v));
    double d2 = norm2(foot - c.o);
    if (sgn(d2 - c.r * c.r) > 0) return {};
    if (sgn(d2 - c.r * c.r) == 0) return {foot};
    double len = sqrt(c.r * c.r - d2) / norm(v);
    return {foot - v * len, foot + v * len};
}
```
#### 向量旋转与直线交点

> **赛时先看**
> - **题目信号**：旋转、反射、两条直线交点。
> - **本质**：角度变换、求两直线交点。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：无额外结构；`rotate` 返回旋转后的点，`line_intersection` 把交点写进 `out` 并返回是否相交。
> - **警告**：平行时没有唯一交点。

**最小完整示例（先抄这一段就能跑）：**
> 题目：(1,0) 绕原点逆时针转 90°；直线 (0,0)+t*(1,1) 与 (1,0)+s*(0,1) 求交点。
> 依赖：点与向量基础（`Point` / `cross` / `sgn`），抄板时一起抄上。
```cpp
Point a = rotate({1, 0}, acos(-1.0) / 2);        // (1,0) 逆时针转 90°，得 (0,1)
Point out;                                        // 交点写进 out
bool ok = line_intersection({0, 0}, {1, 1},      // 直线1：过 (0,0)、方向 (1,1)
                            {1, 0}, {0, 1}, out); // 直线2：过 (1,0)、方向 (0,1)
printf("%d %.1f %.1f\n", ok, out.x, out.y);      // 交点为 (1,1)
```
> 样例输出：`1 1.0 1.0`。

**传参要求（照这个传不会错）：**
- `rotate(a, rad)`：`a` 为原向量 `Point{x,y}`；`rad` 为弧度制转角（正 = 逆时针）；返回旋转后的 `Point`。
- `line_intersection(p, v, q, w, out)`：`p/q` 为两条直线上各一点，`v/w` 为对应方向向量（`Point`，非零）；`out` 为 `Point&` 引用，相交时写入交点；返回 `bool`，`true` = 有唯一交点，`false` = 平行（此时 `out` 不变）。
- 直线是参数式 `p + t*v`；`cross(v, w) == 0` 表示平行，没有唯一交点。


```cpp
Point rotate(Point a, double rad) {
    double c = cos(rad), s = sin(rad);
    return {a.x * c - a.y * s, a.x * s + a.y * c};
}

bool line_intersection(Point p, Point v, Point q, Point w, Point& out) {
    double d = cross(v, w);
    if (sgn(d) == 0) return false;
    double t = cross(q - p, w) / d;
    out = p + v * t;
    return true;
}
```
#### 一维移动墙反弹事件模拟

> **赛时先看**
> - **题目信号**：两条边界位置是一次函数；碰撞只改变速度符号；数据较小或用于验证解析式。
> - **本质**：小球在一维线性移动边界之间反弹，碰到边界后速度取反，求某个时刻位置。
> - **复杂度判定**：`O（碰撞次数）`。
> - **维护的量**：`t`（当前时间）、`x`（当前位置）、`v`（当前速度）；每步取最早碰撞时刻 `best_dt` 与撞墙方向 `hit`，撞墙后把 `x` 贴墙并 `v = -v`。
> - **警告**：该模板按题意“速度直接取反”，不做物理中的相对速度反射；如果碰撞次数可能极大，需要另推周期/展开公式。内部以 2e6 步为上限，超出会静默返回当前位置；起点贴墙（`dt` 近似 0）的场景需自行特判。

**最小完整示例（先抄这一段就能跑）：**
> 题目：小球从 x=0 以 v=1 向右，左右墙固定（左 0 右 10），求 t=12 时位置。
```cpp
double ans = simulate_moving_walls(0, 1, 0, 10, 0, 0, 12);
printf("%.1f\n", ans); // t=10 碰右墙弹回，t=12 时 x=8
```
> 样例输出：`8.0`。

**传参要求（照这个传不会错）：**
- `x`：起点位置（`double`）。
- `v`：初速度（`double`，向右为正）。
- `left0` / `right0`：t=0 时左右墙位置（`double`，须 `left0 < right0`）。
- `left_speed` / `right_speed`：左右墙的匀速速度（`double`，向右为正，可负可 0）。
- `end_time`：模拟截止时刻（`double`，须 ≥ 0）。
- 返回值：`double`，`end_time` 时刻小球位置；内部最多 2e6 步，超限静默返回当前位置。


```cpp
double simulate_moving_walls(
    double x,
    double v,
    double left0,
    double right0,
    double left_speed,
    double right_speed,
    double end_time
) {
    const double EPS = 1e-12;
    double t = 0;
    for (int steps = 0; steps < 2000000 && t + EPS < end_time; steps++) {
        double left = left0 + left_speed * t;
        double right = right0 + right_speed * t;
        double best_dt = end_time - t;
        int hit = 0;

        double den_left = v - left_speed;
        if (den_left < -EPS) {
            double dt = (left - x) / den_left;
            if (dt > EPS && dt < best_dt) {
                best_dt = dt;
                hit = -1;
            }
        }

        double den_right = v - right_speed;
        if (den_right > EPS) {
            double dt = (right - x) / den_right;
            if (dt > EPS && dt < best_dt) {
                best_dt = dt;
                hit = 1;
            }
        }

        x += v * best_dt;
        t += best_dt;
        if (hit == 0) break;
        if (hit == -1) x = left0 + left_speed * t;
        else x = right0 + right_speed * t;
        v = -v;
    }
    return x;
}
```

#### 极坐标圆弧图最短路：关键角度建图

> **注意**：本节仅记录思路与建图要点，无直接可抄代码；赛时需按题面现场建图 + Dijkstra。

> **赛时先看**
> - **题目信号**：给若干圆弧 `(r, theta_l, theta_r)`；同一圆弧可沿角度走，同一角度可沿半径走；起点终点在极坐标上；若把每个角度与每条圆弧都连边，图会变成 `O(n^2)`。
> - **本质**：把圆弧和径向移动的连续几何问题离散成 `O(n)` 个关键点，再跑 Dijkstra。
> - **接法**：按半径从外到内扫描圆弧，用“区间取 max 覆盖 + 单点查询”维护当前角度最外层弧。每条弧的两个端点向外查询最近可达弧，加入关键点。随后按同一角度连径向边，按同一圆弧上相邻关键角连弧长边，最后 Dijkstra。
> - **复杂度判定**：关键点和边 `O(n)`，用线段树找外层/内层相邻弧 `O(n log U)`，Dijkstra `O(n log n)`。
> - **警告**：跨越 `0` 角度的圆弧要拆成两段；只有圆弧端点、起点角、终点角以及端点径向碰到的第一条弧是关键点；径向边只连同一角度下相邻半径的点。


典题：本场 B《Cast off as Cast》。角度值离散在 `0..99999`，线段树维护角度区间当前最外层圆弧，最终图规模线性。

#### 点在凸多边形内

> **赛时先看**
> - **题目信号**：凸多边形固定，查询点很多。
> - **本质**：多次判断点是否在凸包内。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(log n)`。
> - **维护的量**：无额外结构；只读凸包顶点数组 `p`（逆时针、无重复点）与查询点 `q`。
> - **警告**：要求凸包点按逆时针且无重复点；边界按这里返回 true。

**最小完整示例（先抄这一段就能跑）：**
> 题目：逆时针凸包 (0,0),(4,0),(4,3),(0,3)，查询 (1,1) 与 (5,2) 是否在凸包内。
> 依赖：点与向量基础（`Point` / `cross` / `sgn`）+ 线段相交与点到线段距离（`on_segment`），抄板时一起抄上。
```cpp
vector<Point> hull = {{0,0}, {4,0}, {4,3}, {0,3}}; // 逆时针、无重复点
printf("%d %d\n",
       point_in_convex_polygon(hull, {1,1}),       // 内部 → 1
       point_in_convex_polygon(hull, {5,2}));      // 外部 → 0
```
> 样例输出：`1 0`。

**传参要求（照这个传不会错）：**
- `p`：`vector<Point>`，凸包顶点，必须按逆时针顺序且无重复点；`n=1` 时按点重合判，`n=2` 时按 `on_segment` 判线段。
- `q`：查询点 `Point{x,y}`，坐标任意。
- 返回值：`bool`，`true` = 在凸包内部或边界上（边界返回 true）。
- 依赖本文件同一套 `Point`、`cross(a,b,c)`、`sgn`、`on_segment`。


```cpp
bool point_in_convex_polygon(const vector<Point>& p, Point q) {
    int n = (int)p.size();
    if (n == 1) return q == p[0];
    if (n == 2) return on_segment(q, p[0], p[1]);
    if (sgn(cross(p[1] - p[0], q - p[0])) < 0) return false;
    if (sgn(cross(p[n - 1] - p[0], q - p[0])) > 0) return false;

    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = (l + r) >> 1;
        if (sgn(cross(p[mid] - p[0], q - p[0])) >= 0) l = mid;
        else r = mid;
    }
    return sgn(cross(p[l], p[(l + 1) % n], q)) >= 0;
}
```
#### Pick 定理

> **赛时先看**
> - **题目信号**：多边形顶点都是整数坐标，问内部/边界格点数。
> - **本质**：整数坐标简单多边形的格点数量关系。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`interior_lattice_points` 内部维护 `a2`（两倍面积 2S）与 `b`（边界格点数 B），按 Pick 公式 `I = (2S - B + 2) / 2` 返回内部格点数。
> - **警告**：`2S = 2I + B - 2`，所以 `I = (2S - B + 2) / 2`。

**最小完整示例（先抄这一段就能跑）：**
> 题目：三角形 (0,0),(4,0),(0,3)，求内部与边界格点数。
```cpp
vector<pair<i64, i64>> tri = {{0,0}, {4,0}, {0,3}};
printf("%lld %lld\n",
       interior_lattice_points(tri),  // 内部格点数 I
       boundary_lattice_points(tri)); // 边界格点数 B
```
> 样例输出：`3 8`。

**传参要求（照这个传不会错）：**
- `p`：`vector<pair<i64,i64>>`，简单多边形顶点（整数坐标，按顺序，顺逆时针皆可）。
- `boundary_lattice_points(p)`：返回边界格点数 `B`（`i64`），每条边贡献 `gcd(|dx|, |dy|)`。
- `area2_lattice_polygon(p)`：返回两倍面积 `2S = |Σ(x1*y2 − x2*y1)|`（`i64`）。
- `interior_lattice_points(p)`：返回内部格点数 `I = (2S − B + 2) / 2`（`i64`）。
- `gcd_abs(a,b)`：内部辅助，取 `|a|,|b|` 的 `gcd`（`i64`）。


```cpp
i64 gcd_abs(i64 a, i64 b) {
    return std::gcd(llabs(a), llabs(b));
}

i64 boundary_lattice_points(const vector<pair<i64, i64>>& p) {
    int n = (int)p.size();
    i64 b = 0;
    for (int i = 0; i < n; ++i) {
        auto [x1, y1] = p[i];
        auto [x2, y2] = p[(i + 1) % n];
        b += gcd_abs(x1 - x2, y1 - y2);
    }
    return b;
}

i64 area2_lattice_polygon(const vector<pair<i64, i64>>& p) {
    int n = (int)p.size();
    i64 s = 0;
    for (int i = 0; i < n; ++i) {
        auto [x1, y1] = p[i];
        auto [x2, y2] = p[(i + 1) % n];
        s += x1 * y2 - x2 * y1;
    }
    return llabs(s);
}

i64 interior_lattice_points(const vector<pair<i64, i64>>& p) {
    i64 a2 = area2_lattice_polygon(p);
    i64 b = boundary_lattice_points(p);
    return (a2 - b + 2) / 2;
}
```
#### 两圆交点

> **赛时先看**
> - **题目信号**：两个圆的公共点。
> - **本质**：圆与圆相交、几何构造。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：无；按圆心距 `d` 分类——外离/内含/同心返回空，`h2 == 0` 相切返回 1 点，否则沿 `dir`/`perp` 构造 2 个交点。
> - **警告**：相离、内含、同心重合都没有两个普通交点。

**最小完整示例（先抄这一段就能跑）：**
> 题目：圆 A 心 (0,0) 半径 2，圆 B 心 (3,0) 半径 2，求两圆交点。
> 依赖：点与向量基础（`Point` / `dist` / `sgn`）+ 直线与圆交点（`Circle`），抄板时一起抄上。
```cpp
vector<Point> res = circle_circle_intersection({{0,0}, 2}, {{3,0}, 2});
printf("%zu %.1f %.1f\n", res.size(), res[0].x, res[0].y); // 交点 (1.5, ±1.32)
```
> 样例输出：`2 1.5 1.3`。

**传参要求（照这个传不会错）：**
- `a` / `b`：`Circle{o, r}`，`o` 为圆心 `Point`，`r` 为半径（`double`，≥ 0）。
- 返回值：`vector<Point>`；空 = 相离/内含/同心重合，1 个 = 相切（返回切点），2 个 = 相交（两个交点）。
- 依赖 `dist(a,b)`、`sgn` 与本文件同一套 `Point`/`Circle` 定义。


```cpp
vector<Point> circle_circle_intersection(Circle a, Circle b) {
    double d = dist(a.o, b.o);
    if (sgn(d) == 0) return {};
    if (sgn(d - (a.r + b.r)) > 0) return {};
    if (sgn(d - fabs(a.r - b.r)) < 0) return {};

    double x = (d * d - b.r * b.r + a.r * a.r) / (2 * d);
    double h2 = a.r * a.r - x * x;
    if (sgn(h2) < 0) return {};
    Point dir = (b.o - a.o) / d;
    Point base = a.o + dir * x;
    if (sgn(h2) == 0) return {base};
    Point perp{-dir.y, dir.x};
    double h = sqrt(h2);
    return {base + perp * h, base - perp * h};
}
```
#### 半平面交

> **赛时先看**
> - **题目信号**：二维不等式、多个有向直线左侧区域求交。
> - **本质**：求若干半平面的交，多边形裁剪、线性约束可行域。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再调用其公开接口。
> - **复杂度判定**：排序后 `O(n log n)`。
> - **维护的量**：`lines` 为半平面（直线上一点 `p` + 方向 `v`，保留左侧）；双端队列 `q` 维护仍有效的直线、`p` 维护相邻直线交点，最终 `p` 即交区域顶点。
> - **警告**：直线方向要统一为保留左侧；平行线只保留更靠内的一条；最后还要处理队首队尾。结果点数 `<= 2` 时返回空向量，表示无界区域或空交，按题意自行处理。

**最小完整示例（先抄这一段就能跑）：**
> 题目：x ≥ 0、y ≥ 0、x+y ≤ 4 三个半平面求交（每个半平面取方向向量左侧）。
```cpp
vector<HalfPlaneLine> lines = {
    {{0,0}, {0,-1}},  // 过 (0,0) 方向向下：左侧即 x≥0
    {{0,0}, {1,0}},   // 过 (0,0) 方向向右：左侧即 y≥0
    {{4,0}, {-1,1}},  // 过 (4,0) 方向 (-1,1)：左侧即 x+y≤4
};
auto poly = half_plane_intersection(lines);
printf("%zu\n", poly.size()); // 交区域为三角形，3 个顶点
```
> 样例输出：`3`。

**传参要求（照这个传不会错）：**
- `lines`：`vector<HalfPlaneLine>`，每个为 `{p, v}`：`p` 为直线上一点，`v` 为方向向量（`HPoint`，double），半平面取 `v` 左侧（`cross(v, 点-p) ≥ 0`）。
- `ang` 由函数内自动按 `atan2(v.y, v.x)` 填好并排序，调用时不用管。
- 返回值：`vector<HPoint>`，交区域的顶点（逆时针）；点数 ≤ 2 返回空（无界或空交），按题意自行处理。
- 坐标跨度很大时注意 `EPS_HP = 1e-10` 的精度；平行线只需保留最靠内的一条。


```cpp
const double EPS_HP = 1e-10;

struct HPoint {
    double x, y;
    HPoint operator+(const HPoint& o) const { return {x + o.x, y + o.y}; }
    HPoint operator-(const HPoint& o) const { return {x - o.x, y - o.y}; }
    HPoint operator*(double k) const { return {x * k, y * k}; }
};

double cross(HPoint a, HPoint b) { return a.x * b.y - a.y * b.x; }

struct HalfPlaneLine {
    HPoint p, v;
    double ang;
    bool operator<(const HalfPlaneLine& other) const {
        if (fabs(ang - other.ang) > EPS_HP) return ang < other.ang;
        return cross(v, other.p - p) < 0;
    }
};

HPoint line_intersection(const HalfPlaneLine& a, const HalfPlaneLine& b) {
    HPoint u = b.p - a.p;
    double t = cross(u, b.v) / cross(a.v, b.v);
    return a.p + a.v * t;
}

bool outside(const HalfPlaneLine& l, HPoint p) {
    return cross(l.v, p - l.p) < -EPS_HP;
}

vector<HPoint> half_plane_intersection(vector<HalfPlaneLine> lines) {
    for (auto& l : lines) l.ang = atan2(l.v.y, l.v.x);
    sort(lines.begin(), lines.end());
    deque<HalfPlaneLine> q;
    deque<HPoint> p;
    for (auto l : lines) {
        if (!q.empty() && fabs(cross(q.back().v, l.v)) < EPS_HP) {
            if (outside(l, q.back().p)) q.back() = l;
            continue;
        }
        while (!p.empty() && outside(l, p.back())) {
            p.pop_back();
            q.pop_back();
        }
        while (!p.empty() && outside(l, p.front())) {
            p.pop_front();
            q.pop_front();
        }
        if (!q.empty()) p.push_back(line_intersection(q.back(), l));
        q.push_back(l);
    }
    while (!p.empty() && outside(q.front(), p.back())) {
        p.pop_back();
        q.pop_back();
    }
    if (q.size() <= 2) return {};
    p.push_back(line_intersection(q.back(), q.front()));
    return vector<HPoint>(p.begin(), p.end());
}
```
#### 曼哈顿距离变换

> **赛时先看**
> - **题目信号**：距离为 `|x1-x2| + |y1-y2|`，需要全局最远或多次查询。
> - **本质**：求平面点集曼哈顿距离最大值。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接调用本节列出的函数即可。
> - **复杂度判定**：最大距离 `O(n)`。
> - **维护的量**：四种符号组合 `(sx, sy)` 下投影 `sx*x + sy*y` 的最小值 `mn` 与最大值 `mx`；答案取 `mx - mn` 的最大者。
> - **警告**：二维曼哈顿最大距离等于四种符号组合投影最大差。

**最小完整示例（先抄这一段就能跑）：**
> 题目：点集 {(0,0),(5,3),(1,7)} 中两点的最大曼哈顿距离。
```cpp
vector<pair<i64, i64>> pts = {{0,0}, {5,3}, {1,7}};
printf("%lld\n", max_manhattan_distance(pts)); // 四种符号组合投影最大差
```
> 样例输出：`8`。

**传参要求（照这个传不会错）：**
- `pts`：`vector<pair<i64,i64>>`，平面点集（至少 1 个点；空集返回 0）。
- 返回值：`i64`，最大曼哈顿距离；原理 `|x1-x2|+|y1-y2| = max_{sx,sy∈{-1,1}} (sx*x1+sy*y1) - (sx*x2+sy*y2)`。
- 只能求最大：求“最小”曼哈顿距离不能这样算，需换平面最近点对等做法。


**改板时先认这几个量：**

- `mn`：某符号组合下投影的最小值。
- `mx`：某符号组合下投影的最大值。

```cpp
i64 max_manhattan_distance(const vector<pair<i64, i64>>& pts) {
    i64 ans = 0;
    for (int sx : {-1, 1}) {
        for (int sy : {-1, 1}) {
            i64 mn = (1LL << 62), mx = -(1LL << 62);
            for (auto [x, y] : pts) {
                i64 v = sx * x + sy * y;
                mn = min(mn, v);
                mx = max(mx, v);
            }
            ans = max(ans, mx - mn);
        }
    }
    return ans;
}
```
#### 曼哈顿距离最小生成树

> **赛时先看**
> - **题目信号**：很多平面点、连通总代价最小、距离是曼哈顿距离；完全图 `O(n^2)` 建边太慢。
> - **本质**：平面点集的边权为 `|x1-x2| + |y1-y2|` 时，利用四个方向扫描生成 `O(n)` 条候选边，再 Kruskal。
> - **接法**：城市坐标给定，修最短总长度的只能横竖走的道路网络。
> - **复杂度判定**：候选边 `O(n)`，总复杂度 `O(n log n)`。
> - **维护的量**：候选边集合 `edges{u,v,w}`（4 轮坐标变换生成 O(n) 条）；`DSUForMST` 并查集（`p` 父节点、`sz` 集合大小）按边权升序 Kruskal 累加 `ans`。
> - **警告**：变换顺序为每轮取反 `x`，两轮后交换 `x,y`；候选边可能重复，Kruskal 自然会过滤。

**最小完整示例（先抄这一段就能跑）：**
> 题目：城市 (0,0)、(10,0)、(0,10) 修只能横竖走的道路，求连通所有城市的最短总长。
```cpp
vector<pair<i64, i64>> cities = {{0,0}, {10,0}, {0,10}};
printf("%lld\n", manhattan_mst(cities)); // 两条边 10 + 10
```
> 样例输出：`20`。

**传参要求（照这个传不会错）：**
- `manhattan_mst(point)`：`point` 为 `vector<pair<i64,i64>>`，城市坐标（n ≥ 1）；返回 `i64` 总代价；n = 1 时返回 0。
- `DSUForMST(n)`：`n` 为点数；`find(x)` 查代表元（路径压缩，返回 `int`）；`unite(a,b)` 按 `sz` 大小合并，返回 `bool`（是否真的合并）。
- 内部自动做 4 轮坐标变换生成候选边，重复边由 Kruskal 过滤，调用时无需去重。


**API / 入口函数（赛时只认这里列的名字）：**

- `manhattan_mst(vector<pair<i64,i64>> points)` -> 求曼哈顿距离下的最小生成树总代价，返回 `i64`。
- `find(int x)` -> 查代表元/查找结果 返回 `int`。
- `unite(int a, int b)` -> 合并两个集合 返回 `bool`。
**改板时先认这几个量：**

- `sz`：集合/子树大小。
- `dsu`：并查集实例（`find/unite` 的调用对象）。
- `it`：`active` 中的迭代器，用于枚举候选边。

典题模型：城市坐标给定，修最短总长度的只能横竖走的道路网络。

```cpp
struct DSUForMST {
    vector<int> p, sz;
    explicit DSUForMST(int n) : p(n), sz(n, 1) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b];
        return true;
    }
};

i64 manhattan_mst(const vector<pair<i64, i64>>& point) {
    struct P { i64 x, y; int id; };
    struct E {
        int u, v;
        i64 w;
        bool operator<(const E& other) const { return w < other.w; }
    };
    int n = (int)point.size();
    vector<P> p(n);
    for (int i = 0; i < n; ++i) p[i] = {point[i].first, point[i].second, i};
    vector<E> edges;

    for (int swap_xy = 0; swap_xy < 2; ++swap_xy) {
        for (int flip_x = 0; flip_x < 2; ++flip_x) {
            sort(p.begin(), p.end(), [](const P& a, const P& b) {
                return a.x + a.y != b.x + b.y ? a.x + a.y < b.x + b.y : a.x < b.x;
            });
            map<i64, P> active;
            for (const P& now : p) {
                auto it = active.lower_bound(-now.y);
                while (it != active.end()) {
                    const P old = it->second;
                    if (now.x - now.y < old.x - old.y) break;
                    i64 w = llabs(point[now.id].first - point[old.id].first)
                                + llabs(point[now.id].second - point[old.id].second);
                    edges.push_back({now.id, old.id, w});
                    it = active.erase(it);
                }
                active[-now.y] = now;
            }
            for (P& q : p) q.x = -q.x;
        }
        for (P& q : p) swap(q.x, q.y);
    }

    sort(edges.begin(), edges.end());
    DSUForMST dsu(n);
    i64 ans = 0;
    for (const E& e : edges) if (dsu.unite(e.u, e.v)) ans += e.w;
    return ans;
}
```
#### 最小圆覆盖：随机增量

> **赛时先看**
> - **题目信号**：平面点集最小覆盖圆；点数较大，要求浮点答案。
> - **本质**：求覆盖所有点的最小圆。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再调用其公开接口。
> - **复杂度判定**：期望 `O(n)`。
> - **维护的量**：`CircleH c{o, r}` 为当前覆盖前 i 个点的最小圆；加点若在圆外，用两点定圆 `circle_from_two`、三点定圆 `circle_from_three` 逐层重建。
> - **警告**：随机打乱；判断点是否在圆内要加 EPS。

**最小完整示例（先抄这一段就能跑）：**
> 题目：点 (0,0)、(2,0)、(0,2) 的最小覆盖圆。
```cpp
vector<MecPoint> pts = {{0,0}, {2,0}, {0,2}};
CircleH c = minimum_enclosing_circle(pts);   // 内部已随机打乱
printf("%.2f %.2f %.2f\n", c.o.x, c.o.y, c.r); // 圆心 (1,1)，半径 √2
```
> 样例输出：`1.00 1.00 1.41`。

**传参要求（照这个传不会错）：**
- `minimum_enclosing_circle(p)`：`p` 为 `vector<MecPoint>`（`{x, y}`，double 坐标）；返回 `CircleH{o, r}`（`o` 圆心、`r` 半径），期望 `O(n)`，内部自动打乱。
- `circle_from_two(a, b)`：以 a、b 为直径端点作圆；`circle_from_three(a, b, c)`：a、b、c 外接圆（三点共线时 `det=0` 会除零，需保证输入不共线）。
- `in_circle(c, p)`：点 `p` 是否在圆 `c` 内（含 `1e-9` 容差）。


```cpp
struct MecPoint {
    double x, y;
    MecPoint() = default;
    MecPoint(double x_, double y_) : x(x_), y(y_) {}
    MecPoint operator+(const MecPoint& o) const { return {x + o.x, y + o.y}; }
    MecPoint operator-(const MecPoint& o) const { return {x - o.x, y - o.y}; }
    MecPoint operator*(double k) const { return {x * k, y * k}; }
    MecPoint operator/(double k) const { return {x / k, y / k}; }
};

struct CircleH {
    MecPoint o;
    double r;
};

double dist(MecPoint a, MecPoint b) {
    return hypot(a.x - b.x, a.y - b.y);
}

CircleH circle_from_two(MecPoint a, MecPoint b) {
    return {{(a.x + b.x) / 2, (a.y + b.y) / 2}, dist(a, b) / 2};
}

CircleH circle_from_three(MecPoint a, MecPoint b, MecPoint c) {
    double a1 = b.x - a.x, b1 = b.y - a.y;
    double c1 = c.x - a.x, d1 = c.y - a.y;
    double e1 = (a1 * (a.x + b.x) + b1 * (a.y + b.y)) / 2;
    double f1 = (c1 * (a.x + c.x) + d1 * (a.y + c.y)) / 2;
    double det = a1 * d1 - b1 * c1;
    MecPoint o{(d1 * e1 - b1 * f1) / det, (-c1 * e1 + a1 * f1) / det};
    return {o, dist(o, a)};
}

bool in_circle(CircleH c, MecPoint p) {
    return dist(c.o, p) <= c.r + 1e-9;
}

CircleH minimum_enclosing_circle(vector<MecPoint> p) {
    shuffle(p.begin(), p.end(), mt19937(chrono::steady_clock::now().time_since_epoch().count()));
    CircleH c{{0, 0}, -1};
    for (int i = 0; i < (int)p.size(); i++) {
        if (c.r >= 0 && in_circle(c, p[i])) continue;
        c = {p[i], 0};
        for (int j = 0; j < i; j++) {
            if (in_circle(c, p[j])) continue;
            c = circle_from_two(p[i], p[j]);
            for (int k = 0; k < j; k++) {
                if (!in_circle(c, p[k])) c = circle_from_three(p[i], p[j], p[k]);
            }
        }
    }
    return c;
}
```
#### 扫描线 + 线段树：矩形面积并

> **赛时先看**
> - **题目信号**：多个矩形，问覆盖面积，坐标大，矩形数量可到 `1e5`。
> - **本质**：求多个轴平行矩形覆盖的总面积。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n log n)`。
> - **维护的量**：事件表 `events{x, y1, y2, delta}`（按 x 升序）；线段树 `cover[p]`（区间被覆盖层数）与 `len[1]`（y 方向被覆盖总长）；答案按 `len[1] * Δx` 逐段累加。
> - **警告**：对 y 坐标离散化后，线段树维护的是区间 `[ys[l], ys[r+1])`；事件按 x 排序。

**最小完整示例（先抄这一段就能跑）：**
> 题目：矩形 (0,0)-(2,2) 与 (1,1)-(3,3) 的覆盖面积（重合部分只算一次）。
```cpp
vector<array<double, 4>> rects = {{0,0,2,2}, {1,1,3,3}};
printf("%.1f\n", rectangle_union_area(rects)); // 4 + 4 - 1 = 7
```
> 样例输出：`7.0`。

**传参要求（照这个传不会错）：**
- `rectangle_union_area(rects)`：唯一外部入口；`rects` 为 `vector<array<double,4>>`，每个矩形 `{x1, y1, x2, y2}`（角点顺序任意，内部自动交换；`x1==x2` 或 `y1==y2` 的退化矩形自动忽略）；返回 `double` 覆盖面积。
- 内部 `SweepEvent{x, y1, y2, delta}`：`delta = +1` 左边界入、`-1` 右边界出；事件按 x 升序扫描。
- 不要直接调 `AreaUnionSeg::add` 的递归参数，只调 `rectangle_union_area`。


**API / 入口函数（赛时只认这里列的名字）：**

- `rectangle_union_area(rects)` -> 唯一外部入口；`rects` 每个元素为 `{x1,y1,x2,y2}`，返回所有轴平行矩形覆盖面积。

```cpp
struct SweepEvent {
    double x, y1, y2;
    int delta;
    bool operator<(const SweepEvent& other) const { return x < other.x; }
};

struct AreaUnionSeg {
    vector<int> cover;
    vector<double> len, ys;
    int n; // 基本小区间数量为 ys.size()-1。

    AreaUnionSeg(const vector<double>& ys_) {
        ys = ys_;
        n = (int)ys.size() - 1;
        cover.assign(4 * n + 4, 0);
        len.assign(4 * n + 4, 0);
    }

    void pull(int p, int l, int r) {
        if (cover[p] > 0) len[p] = ys[r + 1] - ys[l];
        else if (l == r) len[p] = 0;
        else len[p] = len[p << 1] + len[p << 1 | 1];
    }

    void add(int p, int l, int r, int ql, int qr, int delta) {
        if (ql <= l && r <= qr) {
            cover[p] += delta;
            pull(p, l, r);
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) add(p << 1, l, mid, ql, qr, delta);
        if (qr > mid) add(p << 1 | 1, mid + 1, r, ql, qr, delta);
        pull(p, l, r);
    }
};

double rectangle_union_area(vector<array<double, 4>> rects) {
    vector<SweepEvent> events;
    vector<double> ys;
    for (auto [x1, y1, x2, y2] : rects) {
        if (x1 == x2 || y1 == y2) continue;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        events.push_back({x1, y1, y2, 1});
        events.push_back({x2, y1, y2, -1});
        ys.push_back(y1);
        ys.push_back(y2);
    }
    if (events.empty()) return 0;
    sort(events.begin(), events.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    AreaUnionSeg seg(ys);
    double ans = 0;
    for (int i = 0; i < (int)events.size(); ++i) {
        if (i > 0) ans += seg.len[1] * (events[i].x - events[i - 1].x);
        int l = int(lower_bound(ys.begin(), ys.end(), events[i].y1) - ys.begin());
        int r = int(lower_bound(ys.begin(), ys.end(), events[i].y2) - ys.begin()) - 1;
        if (l <= r) seg.add(1, 0, seg.n - 1, l, r, events[i].delta);
    }
    return ans;
}
```

