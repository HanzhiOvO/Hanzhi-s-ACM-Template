## A 赛场入口与基础算法

### 01 基础算法、数值工具与位运算

把比赛骨架、输入输出、调试、常用整数工具、位运算、二分、三分、离散化和前缀差分集中放在一起。

#### 通用头文件与主函数

> **赛时先看**
> - **题目信号**：任意题。
> - **本质**：比赛所有题目的默认开局。
> - **复杂度判定**：无。
> - **警告**：本书后续代码默认已经粘贴这份骨架，统一使用 `i64 / ui64 / i128`、`INF / LINF` 和 `mod7 / mod9 / modn`；不要再额外定义 `ll`、`u64` 或 `#define int long long`。


```cpp
// 语言：C++。
// 寒枝OvO。

#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using ui64 = unsigned long long;
using i128 = __int128;

using pii = pair<int, int>;
using pll = pair<i64, i64>;

#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define eb emplace_back

const int INF = 0x3f3f3f3f;
const i64 LINF = 4e18;
const int mod7 = 1e9 + 7;
const int mod9 = 1e9 + 9;
const int modn = 998244353;

const int dx[4] = {0, 0, 1, -1};
const int dy[4] = {1, -1, 0, 0};

void solve()
{
    
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr), cout.tie(nullptr);

    int T = 1;
    // 示例：cin >> T;。

    while (T--)
    {
        solve();
    }

    return 0;
}
```

#### 本地调试宏

> **赛时先看**
> - **题目信号**：WA 后需要看中间状态。
> - **本质**：本地打印变量，提交时自动关闭。
> - **复杂度判定**：无。
> - **警告**：不要输出到 `stdout`；提交时不要定义 `LOCAL`。


**不会用就照抄：**

```cpp
dbg(x);   // 本地打印 "x = 值" 到 stderr；提交（未定义 LOCAL）时为空操作。
```


```cpp
#ifdef LOCAL
#define dbg(x) cerr << #x << " = " << (x) << '\n'
#else
#define dbg(x) ((void)0)
#endif
```

#### 快读 FastScanner

> **赛时先看**
> - **题目信号**：读入整数数量达到几百万以上，担心 IO 卡常。
> - **本质**：输入规模极大时替代 `cin`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O（输入长度）`。
> - **维护的量**：`idx/size`（缓冲中已读字节数/本次读入的字节数）；`buf[S]`（1<<20 的字符缓冲）。
> - **警告**：只适合读整数；字符串和浮点要另写。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 输入规模几百万的整数，求和后输出。
FastScanner fs;
int n;
fs.read_int(n);
i64 sum = 0, x;
while (n--) { fs.read_int(x); sum += x; }
cout << sum << '\n';
```

- 样例：输入 `3 1 2 3` -> 输出 `6`。

**传参要求（照这个传不会错）：**

- `read_int(T& out)`：把下一个整数读进 `out`，成功返回 `true`、读到文件尾返回 `false`；自动处理负数；`T` 传 `int / i64 / ui64` 均可。
- `getch()`：底层读单个字符，文件结束返回 `0`；平时不用碰。
- 注意：本板子只负责整数；字符串和浮点请用 `cin`。


**不会用就照抄：**

```cpp
FastScanner fs;
i64 x;
fs.read_int(x);   // 读一个整数到 x；成功返回 true
```

- 你主要调用 `read_int(x)`；`getch()` 是底层读字符，平时不用碰。
- 不要和 `cin` 随意混用缓冲策略；这份板子只负责整数。

```cpp
struct FastScanner {
    static const int S = 1 << 20;
    int idx = 0, size = 0;
    char buf[S];

    char getch() {
        if (idx >= size) {
            size = (int)fread(buf, 1, S, stdin);
            idx = 0;
            if (size == 0) return 0;
        }
        return buf[idx++];
    }

    template <class T>
    bool read_int(T& out) {
        char c = getch();
        if (!c) return false;
        while (c && c <= ' ') c = getch();
        T sign = 1, x = 0;
        if (c == '-') sign = -1, c = getch();
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = getch();
        }
        out = x * sign;
        return true;
    }
};
```
#### `i128` 输入输出

> **赛时先看**
> - **题目信号**：`1e18 * 1e18`、乘法比较、CRT 合并。
> - **本质**：中间结果超过 `i64`，但不需要高精度类。
> - **复杂度判定**：`O（位数）`。
> - **维护的量**：无额外结构；只重载了 `operator>>` / `operator<<` 两个全局函数。
> - **警告**：`cin/cout` 不能直接读写 `i128`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 读入两个 i128 并输出它们的乘积。
i128 a, b;
cin >> a >> b;
cout << a * b << '\n';
```

- 样例：输入 `12345678901234567890123 2` -> 输出 `24691357802469135780246`。

**传参要求（照这个传不会错）：**

- `operator>>(istream& in, i128& x)`：像普通整数一样读入 `x`；自动处理负号；和下方的 `operator<<` 重载一起整段抄。
- `operator<<(ostream& out, i128 x)`：把 `x` 按十进制输出；`x == 0` 输出 `0`。
- 中间乘法防止先按 `i64` 算：写成 `(i128)a * b`。


**不会用就照抄：**

```cpp
i128 x;
cin >> x;
cout << x << '\n';
```

- 本节已经重载 `>>` / `<<`，所以把这两个重载函数一起抄上去后，就能像普通整数一样读写。
- 中间乘法需要 `i128` 时，记得在乘法发生前强转：`(i128)a * b`。

```cpp
istream& operator>>(istream& in, i128& x) {
    string s;
    in >> s;
    int sign = 1, i = 0;
    if (s[0] == '-') sign = -1, i = 1;
    x = 0;
    for (; i < (int)s.size(); ++i) x = x * 10 + (s[i] - '0');
    x *= sign;
    return in;
}

ostream& operator<<(ostream& out, i128 x) {
    if (x == 0) return out << '0';
    if (x < 0) out << '-', x = -x;
    string s;
    while (x > 0) {
        s.push_back(char('0' + x % 10));
        x /= 10;
    }
    reverse(s.begin(), s.end());
    return out << s;
}
```
#### 高精度整数 BigInt：非负大整数

> **赛时先看**
> - **题目信号**：题面明确“不取模”；答案可能超过 `i64` / `i128`；需要输出完整十进制整数。
> - **本质**：处理非负大整数的输入输出、比较、加法、减法、乘法、除以 int 和模 int。适合计数答案巨大但不取模的题。
> - **接法**：读入字符串构造 `BigInt(s)`；普通计数 DP 里用 `+` 累加；需要乘小数或做大整数乘法时用 `*`。如果最终只需要对小模数取余，用成员函数 `mod_int(b)`。
> - **复杂度判定**：加减 `O(n)`，乘法 `O(nm)`，除以 int `O(n)`，其中 `n` 是 base 为 `1e8` 的块数。
> - **维护的量**：`d`（低位在前的块数组，每块存 `0..BASE-1`）；`BASE/WIDTH`（进制 `1e8` 与每块 8 位）。
> - **警告**：本模板只处理非负整数；减法要求左边不小于右边。若题目有负数，外层单独维护符号。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 读入两个大整数，输出和、差、积、除以 3 的商和模 3。
string sa, sb;
cin >> sa >> sb;
BigInt a(sa), b(sb);
cout << a + b << '\n';                     // 加法
cout << a - b << '\n';                     // 减法，要求 a >= b
cout << a * b << '\n';                     // 乘法
cout << a / 3 << ' ' << a.mod_int(3) << '\n'; // 除 int 与模 int
```

- 样例：输入 `99999999999999999999 1` -> 依次输出 `100000000000000000000`、`99999999999999999998`、`99999999999999999999`、`33333333333333333333 0`。

**传参要求（照这个传不会错）：**

- `BigInt(x)` / `BigInt(s)`：用 `i64` 或十进制字符串（仅非负）构造。
- `str()` / `operator<<`：输出完整十进制。
- `+`、`-`、`*`：BigInt 间运算；`-` 要求左操作数不小于右操作数。
- `/ int b`、`mod_int(b)`：除以正 `int`，`b > 0`；分别返回商与余数。
- `cmp(a, b)` / `<` / `==`：精确比较。


**改板时先认这几个量：**

- `d`：低位在前，每个块存 0..BASE-1。
- `cur`：加减乘除时当前块上的临时累加值。

```cpp
struct BigInt {
    static const int BASE = 100000000;
    static const int WIDTH = 8;
    vector<int> d; // 低位在前，每个块存 0..BASE-1。

    BigInt(i64 x = 0) { *this = x; }
    explicit BigInt(const string& s) { read(s); }

    BigInt& operator=(i64 x) {
        d.clear();
        if (x == 0) return *this;
        while (x > 0) {
            d.push_back((int)(x % BASE));
            x /= BASE;
        }
        return *this;
    }

    BigInt& read(const string& s) {
        d.clear();
        for (int i = (int)s.size(); i > 0; i -= WIDTH) {
            int l = max(0, i - WIDTH);
            int x = 0;
            for (int j = l; j < i; ++j) x = x * 10 + (s[j] - '0');
            d.push_back(x);
        }
        trim();
        return *this;
    }

    void trim() {
        while (!d.empty() && d.back() == 0) d.pop_back();
    }

    bool is_zero() const { return d.empty(); }

    string str() const {
        if (d.empty()) return "0";
        string s = std::to_string(d.back());
        for (int i = (int)d.size() - 2; i >= 0; --i) {
            string part = std::to_string(d[i]);
            s += string(WIDTH - part.size(), '0') + part;
        }
        return s;
    }

    friend ostream& operator<<(ostream& os, const BigInt& x) {
        return os << x.str();
    }

    friend int cmp(const BigInt& a, const BigInt& b) {
        if (a.d.size() != b.d.size()) return a.d.size() < b.d.size() ? -1 : 1;
        for (int i = (int)a.d.size() - 1; i >= 0; --i) {
            if (a.d[i] != b.d[i]) return a.d[i] < b.d[i] ? -1 : 1;
        }
        return 0;
    }

    friend bool operator<(const BigInt& a, const BigInt& b) { return cmp(a, b) < 0; }
    friend bool operator==(const BigInt& a, const BigInt& b) { return cmp(a, b) == 0; }

    friend BigInt operator+(const BigInt& a, const BigInt& b) {
        BigInt c;
        int n = max(a.d.size(), b.d.size());
        c.d.assign(n + 1, 0);
        i64 carry = 0;
        for (int i = 0; i <= n; ++i) {
            i64 cur = carry;
            if (i < (int)a.d.size()) cur += a.d[i];
            if (i < (int)b.d.size()) cur += b.d[i];
            c.d[i] = (int)(cur % BASE);
            carry = cur / BASE;
        }
        c.trim();
        return c;
    }

    friend BigInt operator-(const BigInt& a, const BigInt& b) {
        assert(!(a < b));
        BigInt c;
        c.d.assign(a.d.size(), 0);
        i64 borrow = 0;
        for (int i = 0; i < (int)a.d.size(); ++i) {
            i64 cur = (i64)a.d[i] - borrow - (i < (int)b.d.size() ? b.d[i] : 0);
            if (cur < 0) cur += BASE, borrow = 1;
            else borrow = 0;
            c.d[i] = (int)cur;
        }
        c.trim();
        return c;
    }

    friend BigInt operator*(const BigInt& a, const BigInt& b) {
        BigInt c;
        if (a.is_zero() || b.is_zero()) return c;
        c.d.assign(a.d.size() + b.d.size(), 0);
        for (int i = 0; i < (int)a.d.size(); ++i) {
            i64 carry = 0;
            for (int j = 0; j < (int)b.d.size() || carry; ++j) {
                i64 cur = c.d[i + j] + carry;
                if (j < (int)b.d.size()) cur += 1LL * a.d[i] * b.d[j];
                c.d[i + j] = (int)(cur % BASE);
                carry = cur / BASE;
            }
        }
        c.trim();
        return c;
    }

    friend BigInt operator/(const BigInt& a, int b) {
        assert(b > 0);
        BigInt c;
        c.d.assign(a.d.size(), 0);
        i64 rem = 0;
        for (int i = (int)a.d.size() - 1; i >= 0; --i) {
            i64 cur = a.d[i] + rem * BASE;
            c.d[i] = (int)(cur / b);
            rem = cur % b;
        }
        c.trim();
        return c;
    }

    int mod_int(int b) const {
        assert(b > 0);
        i64 rem = 0;
        for (int i = (int)d.size() - 1; i >= 0; --i) rem = (rem * BASE + d[i]) % b;
        return (int)rem;
    }
};
```
#### 有理数 Frac：精确分数四则运算与比较

> **赛时先看**
> - **题目信号**：题面要求精确输出分数；或者比较两个比例 `a/b` 与 `c/d`，直接用 `double` 可能有精度误差。
> - **本质**：维护形如 `p/q` 的有理数，支持化简、加减乘除和精确比较。常用于斜率、概率、期望、比例和几何中的有理比较。
> - **接法**：构造 `Frac(a,b)`，自动约分；比较直接用 `<`、`==`；输出用 `num/den`，若 `den==1` 可以只输出整数。
> - **复杂度判定**：每次运算 `O(log V)` 做 gcd；比较用 `i128` 防止交叉乘爆 `i64`。
> - **维护的量**：`num`（分子）；`den`（分母，始终为正，构造时自动约分）。
> - **警告**：分母必须保持为正；乘法结果可能超过 `i64`，本模板适合中等范围整数，极大整数分数要换 BigInt。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 求 1/2 + 1/3 并以最简分数输出。
Frac a(1, 2), b(1, 3);
Frac c = a + b;                        // 自动约分
cout << c.num << '/' << c.den << '\n'; // 5/6
```

- 样例：`Frac(1,2) + Frac(1,3)` -> 输出 `5/6`。

**传参要求（照这个传不会错）：**

- `Frac(i64 n = 0, i64 d = 1)`：分子 / 分母构造并自动约分；`d != 0`。
- `+`、`-`、`*`、`/`：四则运算，结果自动约分；`/` 要求除数 `b.num != 0`。
- `<`、`==`：精确比较，内部用 `i128` 交叉乘。
- 输出：成员 `num`、`den` 公开，自行拼 `num/den`。


**改板时先认这几个量：**

- `den`：分母，始终为正。
- `g`：约分用的最大公约数。

```cpp
struct Frac {
    i64 num = 0, den = 1; // den 始终为正。

    Frac(i64 n = 0, i64 d = 1) : num(n), den(d) { normalize(); }

    void normalize() {
        assert(den != 0);
        if (den < 0) num = -num, den = -den;
        i64 g = gcd(llabs(num), den);
        if (g) num /= g, den /= g;
    }

    friend Frac operator+(const Frac& a, const Frac& b) {
        return Frac(a.num * b.den + b.num * a.den, a.den * b.den);
    }
    friend Frac operator-(const Frac& a, const Frac& b) {
        return Frac(a.num * b.den - b.num * a.den, a.den * b.den);
    }
    friend Frac operator*(const Frac& a, const Frac& b) {
        return Frac(a.num * b.num, a.den * b.den);
    }
    friend Frac operator/(const Frac& a, const Frac& b) {
        assert(b.num != 0);
        return Frac(a.num * b.den, a.den * b.num);
    }

    friend bool operator<(const Frac& a, const Frac& b) {
        return (i128)a.num * b.den < (i128)b.num * a.den;
    }
    friend bool operator==(const Frac& a, const Frac& b) {
        return a.num == b.num && a.den == b.den;
    }
};
```
#### 快速幂

> **赛时先看**
> - **题目信号**：指数很大；题目要求取模；需要费马小定理求逆元。
> - **本质**：求 `a^b mod m`，组合数学、矩阵、逆元常用。
> - **接法**：看到 `a^b mod m` 且 `b` 很大就调用 `mod_pow(a,b,m)`；求质数模 `p` 下的逆元时用 `mod_pow(a,p-2,p)`，但前提是 `p` 为质数且 `a` 不是 `p` 的倍数。模数可能不是质数时，改翻扩展欧几里得。
> - **复杂度判定**：`O(log b)`。
> - **维护的量**：无额外结构；只维护累积结果 `res` 与右移的指数 `b`。
> - **警告**：乘法可能爆 `i64` 时用 `i128`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 求 2^100 mod 1e9+7，以及 3 在模 1e9+7 下的逆元。
cout << mod_pow(2, 100, mod7) << '\n';        // 976371285
cout << mod_pow(3, mod7 - 2, mod7) << '\n';   // 333333336
```

- 样例：输出 `976371285` 与 `333333336`。

**传参要求（照这个传不会错）：**

- `a`：底数，任意整数，内部先 `a %= mod`。
- `b`：指数，非负 `i64`（可到 1e18）。
- `mod`：模数，`> 0`，赛时直接传 `mod7 / mod9 / modn`。
- 返回值：`a^b % mod`（`i64`）；`mod` 为质数时 `mod_pow(a, mod - 2, mod)` 即 `a` 的逆元（要求 `a % mod != 0`）。


```cpp
i64 mod_pow(i64 a, i64 b, i64 mod) {
    i64 res = 1 % mod;
    a %= mod;
    while (b > 0) {
        if (b & 1) res = (i128)res * a % mod;
        a = (i128)a * a % mod;
        b >>= 1;
    }
    return res;
}
```
#### 最大公约数与最小公倍数

> **赛时先看**
> - **题目信号**：题面出现 gcd/lcm/互质/整除/周期。
> - **本质**：整除、约分、周期合并、数论题。
> - **接法**：约分、判断互质、合并周期时先想 `gcd`；两个周期同时对齐常用 `lcm`。多个数的 gcd/lcm 就从左到右累积。`lcm` 可能爆 `i64` 时先判断 `a / gcd(a,b) > LIMIT / b`。
> - **复杂度判定**：`O(log min(a,b))`。
> - **维护的量**：无额外结构；纯函数，只传 `a/b` 两个参数。
> - **警告**：`lcm = a / gcd(a,b) * b`，先除再乘防溢出。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 求 12 和 18 的 gcd/lcm，并用 gcd 约分 24/36。
cout << gcd_ll(12, 18) << ' ' << lcm_ll(12, 18) << '\n';
int g = gcd_ll(24, 36);
cout << 24 / g << '/' << 36 / g << '\n';
```

- 样例：输出 `6 36` 与 `2/3`。

**传参要求（照这个传不会错）：**

- `gcd_ll(a, b)`：任意符号的整数，结果取非负；返回最大公约数。
- `lcm_ll(a, b)`：任一为 `0` 返回 `0`；内部先除后乘防溢出；返回最小公倍数。
- 多个数的 gcd/lcm：从左到右逐个累积。


```cpp
i64 gcd_ll(i64 a, i64 b) {
    return b == 0 ? llabs(a) : gcd_ll(b, a % b);
}

i64 lcm_ll(i64 a, i64 b) {
    if (a == 0 || b == 0) return 0;
    return a / gcd_ll(a, b) * b;
}
```
#### 无限重集合 k 子集和 gcd

> **赛时先看**
> - **题目信号**：所有元素可重复选择；所有大小为 `k` 的和取 gcd；`k` 可以任意大。
> - **本质**：集合中每种数有无限个，问大小为 `k` 的可重子集和的 gcd，或问其最大值和最小达成 `k`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O(n log V)`。
> - **维护的量**：`d`（所有数与 `a[0]` 差值的 gcd）；`best`（最大可达 gcd）与 `min_k`（达成它的最小 `k`）；`infinite`（是否随 `k` 无界增长）。
> - **警告**：集合不能为空；`gcd` 取非负值。当所有数都相等时，只有公共值非零才会随 `k` 无界增长；全为 `0` 时所有和、所有 gcd 都是 `0`，并不无界。


结论：设 `d = gcd(|a_i-a_0|)`，并令 `f(k)` 为所有“恰好选 `k` 个、允许重复”的和的 gcd。则 `f(k)=gcd(d, |k*a_0|)`。若 `d=0`，所有数相同：`a_0 != 0` 时 `f(k)=|k*a_0|` 无界；`a_0=0` 时恒为 `0`。若 `d>0`，最大值为 `d`，达成它的最小 `k` 是 `d/gcd(d,|a_0|)`。

```cpp
struct SubsetGcdAnswer {
    bool infinite = false;
    i64 best = 0;
    i64 min_k = 0;
};

SubsetGcdAnswer max_subset_sum_gcd_with_repetition(const vector<i64>& a) {
    assert(!a.empty());
    i64 d = 0;
    for (i64 x : a) d = gcd(d, llabs(x - a[0]));
    if (d == 0) {
        if (a[0] == 0) return {false, 0, 1};
        return {true, 0, 0};
    }
    i64 g = gcd(d, llabs(a[0]));
    return {false, d, d / g};
}
```

**最小完整示例（先抄这一段就能跑）：**

题目：`a = {2, 6, 10}`，每种数可重复选任意个，问所有大小为 `k` 的可重子集和的 gcd 的最大值，以及达成它的最小 `k`。

```cpp
// 构造：d = gcd(|6-2|, |10-2|) = 4，g = gcd(d, 2) = 2，所以 best = d = 4、min_k = d/g = 2。
SubsetGcdAnswer ans = max_subset_sum_gcd_with_repetition({2, 6, 10});
cout << ans.best << ' ' << ans.min_k << '\n'; // 4 2：k=2 时首次达成最大 gcd=4
```

- 样例：`a = {2, 6, 10}` -> 输出 `4 2`。

**传参要求（照这个传不会错）：**

- `a`：`const vector<i64>&`，非空；元素可重复、可负、可为 0，顺序无关。
- 返回值：`SubsetGcdAnswer`：`infinite`（`true` 表示 gcd 随 `k` 无界增长，此时 `best=0`、`min_k` 无意义）、`best`（最大可达 gcd）、`min_k`（达成 `best` 的最小 `k`）。
#### 固定方向长度 k 连线增量计数

> **赛时先看**
> - **题目信号**：五子棋、连珠、`k in a row`；每次只新增一个点；棋盘稀疏。
> - **本质**：棋盘落子后，维护横/竖/两条对角线方向上长度恰为 `k` 的连线数量。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：每次 `O(k)`。
> - **维护的量**：`stones`（棋盘全部落子集合，坐标用 `encode_cell` 编码）；`current_total`（当前长度恰为 `k` 的连线总数）；四个方向增量表 `DX/DY`。
> - **警告**：如果题意要求“极长连续段长度正好为 k”，插入新点可能让旧连线失效，要用 `after-before`；如果只数长度为 `k` 的窗口，把 `exact_segment=false`。


```cpp
i64 encode_cell(int x, int y) {
    return (static_cast<i64>(x) << 32) ^ (unsigned int)y;
}

bool has_stone(const unordered_set<i64>& stones, int x, int y) {
    return stones.count(encode_cell(x, y));
}

int count_affected_lines(
    const unordered_set<i64>& stones,
    int x,
    int y,
    int k,
    bool exact_segment
) {
    static const int DX[4] = {1, 0, 1, 1};
    static const int DY[4] = {0, 1, 1, -1};
    int total = 0;
    for (int d = 0; d < 4; d++) {
        int from = exact_segment ? -k : -(k - 1);
        int to = 0;
        for (int start = from; start <= to; start++) {
            bool ok = true;
            for (int t = 0; t < k; t++) {
                int nx = x + (start + t) * DX[d];
                int ny = y + (start + t) * DY[d];
                if (!has_stone(stones, nx, ny)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            if (exact_segment) {
                int lx = x + (start - 1) * DX[d];
                int ly = y + (start - 1) * DY[d];
                int rx = x + (start + k) * DX[d];
                int ry = y + (start + k) * DY[d];
                if (has_stone(stones, lx, ly) || has_stone(stones, rx, ry)) continue;
            }
            total++;
        }
    }
    return total;
}

i64 add_stone_and_update_lines(
    unordered_set<i64>& stones,
    i64 current_total,
    int x,
    int y,
    int k = 5,
    bool exact_segment = true
) {
    int before = count_affected_lines(stones, x, y, k, exact_segment);
    stones.insert(encode_cell(x, y));
    int after = count_affected_lines(stones, x, y, k, exact_segment);
    return current_total + after - before;
}
```

**最小完整示例（先抄这一段就能跑）：**

题目：五子棋式连珠：空棋盘上依次在 `(0,0) (1,0) (2,0)` 与 `(0,1) (0,2)` 落子，`k=3`，统计“极长段恰长 3”的连线数。

```cpp
unordered_set<i64> stones;
i64 total = 0;
total = add_stone_and_update_lines(stones, total, 0, 0, 3);
total = add_stone_and_update_lines(stones, total, 1, 0, 3);
total = add_stone_and_update_lines(stones, total, 2, 0, 3); // 横线凑成，total = 1
total = add_stone_and_update_lines(stones, total, 0, 1, 3);
total = add_stone_and_update_lines(stones, total, 0, 2, 3); // 竖线也凑成，total = 2
cout << total << '\n';                                      // 2
```

- 样例：依次落 5 子后输出 `2`。

**传参要求（照这个传不会错）：**

- `stones`：`unordered_set<i64>&`，存棋盘全部落子（坐标统一用 `encode_cell(x,y)` 编码）；函数内部会插入新点。
- `current_total`：`i64`，插入前的连线总数；返回值就是插入后的总数。
- `x, y`：`int`，新落子坐标，任意整数（原点自己定，全棋盘统一即可）。
- `k`：`int`，连线长度，默认 `5`。
- `exact_segment`：`bool`，`true` 只数“极长段恰为 k”的连线（默认）；`false` 数“存在长为 k 子段”的连线。
- 返回值：`i64`，插入新点后长度满足要求的连线总数。
#### 整数平方根

> **赛时先看**
> - **题目信号**：`n` 接近 `1e18`，需要判断平方数或枚举到根号。
> - **本质**：精确求 `floor(sqrt(n))`，避免 double 精度误差。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(1)` 调整。
> - **维护的量**：无额外结构；只维护近似根 `x` 并上下各调整一次。
> - **警告**：判断 `(x+1)*(x+1)` 可能溢出，用除法或 `i128`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 求 n=1e18 的整数平方根，并判断 n 是否为完全平方数。
i64 n = 1000000000000000000LL;
i64 x = isqrt(n);
cout << x << '\n';                    // 1000000000
cout << ((i128)x * x == n) << '\n';   // 1，n 是完全平方数
```

- 样例：`n = 1e18` -> 输出 `1000000000` 与 `1`。

**传参要求（照这个传不会错）：**

- `n`：非负 `i64`，可接近 `1e18`。
- 返回值：`floor(sqrt(n))`（`i64`），即最大的 `x` 满足 `x*x <= n`。


```cpp
i64 isqrt(i64 n) {
    i64 x = sqrt((long double)n);
    while ((i128)(x + 1) * (x + 1) <= n) x++;
    while ((i128)x * x > n) x--;
    return x;
}
```
#### 向下/向上整除

> **赛时先看**
> - **题目信号**：二分边界、数论不等式、坐标计算中可能出现负数。
> - **本质**：处理负数除法、数学推式中的 `floor(a/b)` 和 `ceil(a/b)`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(1)`。
> - **维护的量**：无额外结构；纯函数实现数学意义的 floor/ceil 除法。
> - **警告**：C++ 整数除法是向 0 取整，不等于数学 floor。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 负数除法按数学意义取整，常用于二分边界与推式子。
cout << floor_div(-7, 2) << ' ' << ceil_div(-7, 2) << '\n';
cout << floor_div(7, -2) << ' ' << ceil_div(7, -2) << '\n';
```

- 样例：输出 `-4 -3` 与 `-4 -3`。

**传参要求（照这个传不会错）：**

- `a`：被除数，可为负。
- `b`：除数，`b != 0`，可为负（内部自动统一符号）。
- 返回值：`floor_div` 返回数学下取整 `floor(a/b)`；`ceil_div` 返回数学上取整 `ceil(a/b)`（均 `i64`）。


```cpp
i64 floor_div(i64 a, i64 b) {
    assert(b != 0);
    if (b < 0) a = -a, b = -b;
    if (a >= 0) return a / b;
    return -((-a + b - 1) / b);
}

i64 ceil_div(i64 a, i64 b) {
    assert(b != 0);
    if (b < 0) a = -a, b = -b;
    if (a >= 0) return (a + b - 1) / b;
    return -((-a) / b);
}
```
#### 常用 bit 内置函数

> **赛时先看**
> - **题目信号**：状压、异或、按位统计。
> - **本质**：二进制状态、lowbit、位数、集合 DP。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：通常 `O(1)`。
> - **维护的量**：无额外结构；一组针对整数的位运算包装函数。
> - **警告**：`__builtin_clz(0)` 未定义；64 位整数用 `ll` 后缀版本（`popcountll`、`clzll`）。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 统计 12（二进制 1100）的 1 个数、最低位与 floor(log2)。
cout << popcount_int(12) << '\n';   // 2
cout << lowbit(12) << '\n';         // 4
cout << floor_log2(12) << '\n';     // 3
```

- 样例：输出 `2`、`4`、`3`。

**传参要求（照这个传不会错）：**

- `lowbit(x)`：`int`；返回最低位 `1` 的值（`x=0` 时返回 `0`）。
- `popcount_int(x)` / `popcount_ll(x)`：32 / 64 位整数；返回二进制中 `1` 的个数。
- `floor_log2(x)` / `floor_log2_ll(x)`：要求 `x > 0`；返回最大的 `k` 使 `2^k <= x`。


```cpp
int lowbit(int x) { return x & -x; }

int popcount_int(int x) { return __builtin_popcount((unsigned)x); }
int popcount_ll(i64 x) { return __builtin_popcountll((ui64)x); }

int floor_log2(unsigned int x) {
    assert(x > 0);
    return 31 - __builtin_clz(x);
}

int floor_log2_ll(ui64 x) {
    assert(x > 0);
    return 63 - __builtin_clzll(x);
}
```
#### 位掩码基础：取位、置位、清位与最低位

> **赛时先看**
> - **题目信号**：题目有少量开关、颜色、字母、人物、状态或特征；需要按位判断、合并、翻转，或用整数表示集合。
> - **本质**：用一个无符号整数表示大小不超过 64 的集合；第 `b` 位为 `1` 表示元素 `b` 被选中。
> - **接法**：有 `n <= 60` 个开关，每次翻转若干位，问某个状态是否出现过或合并后有多少种颜色。用 `ui64 state` 保存状态，合并是 `|`，共同拥有是 `&`，差异是 `^`，计数是 `__builtin_popcountll(state)`；当 `n > 64` 时改用 `bitset` 或动态位集。
> - **复杂度判定**：每个操作 `O(1)`。
> - **维护的量**：无额外结构；所有操作都是对 `ui64 mask` 的纯函数。
> - **警告**：掩码优先用 `ui64`，不要依赖负数的右移语义；写位常量要用 `1ULL << b`，而不是 `1 << b`；`~mask` 会把高位全变成 `1`，若只关心 `n` 位集合请写 `full_mask ^ mask`；`ctz(0)`、`clz(0)` 都未定义。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 维护 n<=60 个开关的状态，并统计已打开的开关数。
ui64 mask = set_bit(0, 2);                  // 打开第 2 个
mask = set_bit(mask, 5);                    // 打开第 5 个
mask = flip_bit(mask, 2);                   // 翻转第 2 个
cout << __builtin_popcountll(mask) << '\n'; // 1
cout << has_bit(mask, 5) << '\n';           // 1
```

- 样例：输出 `1` 与 `1`。

**传参要求（照这个传不会错）：**

- `bit_at(b)`：`b in [0,64)`；返回 `1ULL << b`。
- `low_bits_mask(bits)`：`bits in [0,64]`；返回低 `bits` 位全 `1` 的掩码。
- `has_bit(mask, b)`：返回 `bool`；`set_bit / clear_bit / flip_bit(mask, b)`：返回新掩码 `ui64`，`b in [0,64)`。
- `lowbit(x)`：最低位 `1` 的值；`erase_lowbit(x)`：删除最低位 `1`。
- `set_bit_positions(mask)`：从低到高返回所有为 `1` 的位，`vector<int>`。


**API / 入口函数（赛时只认这里列的名字）：**

- `set_bit_positions(ui64 mask)` -> 从低到高返回所有为 1 的位（mask 为 0 时返回空）。 返回 `vector<int>`。

```cpp
ui64 bit_at(int b) {
    assert(0 <= b && b < 64);
    return 1ULL << b;
}

ui64 low_bits_mask(int bits) {
    assert(0 <= bits && bits <= 64);
    return bits == 64 ? ~0ULL : (1ULL << bits) - 1;
}

bool has_bit(ui64 mask, int b) { return (mask & bit_at(b)) != 0; }
ui64 set_bit(ui64 mask, int b) { return mask | bit_at(b); }
ui64 clear_bit(ui64 mask, int b) { return mask & ~bit_at(b); }
ui64 flip_bit(ui64 mask, int b) { return mask ^ bit_at(b); }

ui64 lowbit(ui64 x) { return x & -x; }          // 最低位的 1；lowbit(0)=0。
ui64 erase_lowbit(ui64 x) { return x & (x - 1); } // 删除最低位的 1。

// 从低到高枚举所有为 1 的位；循环中 mask 必须非零。
vector<int> set_bit_positions(ui64 mask) {
    vector<int> pos;
    while (mask) {
        int b = __builtin_ctzll(mask);
        pos.push_back(b);
        mask &= mask - 1;
    }
    return pos;
}
```

典题模型：有 `n <= 60` 个开关，每次翻转若干位，问某个状态是否出现过或合并后有多少种颜色。用 `ui64 state` 保存状态，合并是 `|`，共同拥有是 `&`，差异是 `^`，计数是 `__builtin_popcountll(state)`；当 `n > 64` 时改用 `bitset` 或动态位集。
#### 二进制集合：枚举子集、超集与集合拆分

> **赛时先看**
> - **题目信号**：状态是 `mask`；转移形如“任选当前集合的一个子集”“把任务集合拆成两组”“枚举包含已选元素的所有集合”。这是状压 DP、Steiner Tree、子集卷积前最常见的循环。
> - **本质**：在给定集合 `mask` 内枚举所有子集，或枚举全集 `0..n-1` 中包含 `mask` 的所有超集；也可将 `mask` 拆成两个互不相交部分。
> - **接法**：有不超过 `20` 个任务，`dp[mask]` 表示完成集合 `mask` 的最优值，转移需要选择 `mask` 的一个非空真子集 `sub` 先处理。用 `for_each_submask(mask, ...)`，跳过 `sub=0` 和 `sub=mask`；若转移把两个部分视为无序，把其中一半（如 `sub < (mask ^ sub)`）跳过以免重复。
> - **复杂度判定**：单个 `mask` 的子集数是 `2^popcount(mask)`；遍历所有 `mask` 的所有子集总复杂度是 `O(3^n)`；枚举超集总复杂度同样是 `O(3^n)`。
> - **维护的量**：无额外结构；三个模板函数各自维护循环变量 `sub` / `sup`。
> - **警告**：`sub = (sub - 1) & mask` 在 `sub=0` 时会回到 `mask`，所以包含空集的写法必须显式 `break`；超集循环里 `full = (1 << n) - 1`，用之前确保 `1 << n` 不会爆 `int`；无序二分拆分会把 `(A,B)` 与 `(B,A)` 都枚举到，需要额外去重。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 枚举 mask=5（101）的所有子集，以及 3 位全集中包含第 0 位的所有超集。
for_each_submask(5, [&](int sub) { cout << sub << ' '; });
cout << '\n';                        // 5 4 1 0
for_each_supermask(1, 3, [&](int sup) { cout << sup << ' '; });
cout << '\n';                        // 1 3 5 7
```

- 样例：输出 `5 4 1 0` 与 `1 3 5 7`。

**传参要求（照这个传不会错）：**

- `for_each_submask(mask, visit)`：按降序枚举 `mask` 的所有子集（含 `0`，回调 `visit(sub)`）。
- `for_each_supermask(mask, n, visit)`：在 `n` 位全集 `[0,n)` 中枚举包含 `mask` 的所有超集（回调 `visit(sup)`）；确保 `1 << n` 不爆 `int`。
- `for_each_ordered_split(mask, visit)`：枚举有序二分 `(a, b)`，其中 `b = mask ^ a`、交集为空；若两侧无序要自己去重。


```cpp
// 按降序枚举 mask 的所有子集，包括 0。
template <class F>
void for_each_submask(int mask, F&& visit) {
    for (int sub = mask;; sub = (sub - 1) & mask) {
        visit(sub);
        if (sub == 0) break;
    }
}

// 在 n 位全集 [0,n) 中枚举 mask 的所有超集。
template <class F>
void for_each_supermask(int mask, int n, F&& visit) {
    int full = (1 << n) - 1;
    for (int sup = mask; sup <= full; sup = (sup + 1) | mask) {
        visit(sup);
        if (sup == full) break;
    }
}

// 有序拆分：a 与 b 的并集为 mask，交集为空。
template <class F>
void for_each_ordered_split(int mask, F&& visit) {
    for_each_submask(mask, [&](int a) {
        int b = mask ^ a;
        visit(a, b);
    });
}
```

典题模型：有不超过 `20` 个任务，`dp[mask]` 表示完成集合 `mask` 的最优值，转移需要选择 `mask` 的一个非空真子集 `sub` 先处理。用 `for_each_submask(mask, ...)`，跳过 `sub=0` 和 `sub=mask`；若转移把两个部分视为无序，把其中一半（如 `sub < (mask ^ sub)`）跳过以免重复。
#### Gosper's Hack：固定选 k 个元素的所有集合

> **赛时先看**
> - **题目信号**：组合大小固定，如“从 `n <= 62` 个点中恰好选 `k` 个”“一轮必须选固定数量机器/边/列”，并且每个组合都要单独计算。
> - **本质**：在 `n` 个位置中只枚举恰好选 `k` 个的掩码，避免枚举 `2^n` 个状态后再判断 popcount。
> - **接法**：`n <= 40` 个城市，必须选恰好 `k=5` 个建立中继站，给每个选择集合计算代价。用 `for (ui64 mask : masks_with_exactly_k_bits(n,k))`；需要访问第 `b` 个城市时判断 `(mask >> b) & 1ULL`。
> - **复杂度判定**：恰好枚举 `C(n,k)` 个掩码，每次求后继 `O(1)`。
> - **维护的量**：无额外结构；`mask` 为当前枚举掩码，`limit = 1ULL << n` 是上界。
> - **警告**：下面代码限制 `0 <= n <= 62`，避免 `1ULL << 64` 和后继溢出；`k=0` 是唯一的空集；固定大小组合远大于可承受范围时，Gosper 也救不了复杂度。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// n=5 个点中恰好选 k=2 个，输出全部 C(5,2)=10 种选择掩码。
for (ui64 mask : masks_with_exactly_k_bits(5, 2)) {
    cout << mask << ' ';
}
cout << '\n';
```

- 样例：输出 `3 5 6 9 10 12 17 18 20 24`。

**传参要求（照这个传不会错）：**

- `masks_with_exactly_k_bits(n, k)`：`0 <= n <= 62`、`0 <= k <= n`；返回所有恰好 `k` 位为 `1` 且 `< 1<<n` 的掩码（升序，`vector<ui64>`）；`k=0` 时返回 `{0}`。
- 判断第 `b` 个元素是否被选：`(mask >> b) & 1ULL`。
- `next_same_popcount(mask)`：内部求后继用，`mask != 0`；一般不用直接调。


```cpp
ui64 next_same_popcount(ui64 mask) {
    // 要求 mask != 0；返回下一个更大的、popcount 相同的整数。
    ui64 low = mask & -mask;
    ui64 raised = mask + low;
    return raised | (((mask ^ raised) >> 2) / low);
}

vector<ui64> masks_with_exactly_k_bits(int n, int k) {
    assert(0 <= n && n <= 62 && 0 <= k && k <= n);
    if (k == 0) return {0};
    ui64 limit = 1ULL << n;
    ui64 mask = (1ULL << k) - 1;
    vector<ui64> result;
    while (mask < limit) {
        result.push_back(mask);
        ui64 next = next_same_popcount(mask);
        if (next >= limit) break;
        mask = next;
    }
    return result;
}
```

典题模型：`n <= 40` 个城市，必须选恰好 `k=5` 个建立中继站，给每个选择集合计算代价。用 `for (ui64 mask : masks_with_exactly_k_bits(n,k))`；需要访问第 `b` 个城市时判断 `(mask >> b) & 1ULL`。
#### Gray Code：相邻状态只翻转一位

> **赛时先看**
> - **题目信号**：要遍历所有子集，但从一个集合切到另一个集合时希望仅加入或删除一个元素；题目本身要求输出 Gray code；硬件/开关模拟。
> - **本质**：按顺序遍历所有 `n` 位掩码，保证相邻状态恰好只有一个 bit 不同；适合状态修改/回滚代价只与“翻转一个元素”有关的题。
> - **接法**：有 `n <= 22` 个元素，需要统计每个子集的函数值，而增删一个元素可以 `O(1)` 更新。按 `all_gray_codes(n)` 遍历，维护前一个 `mask` 与当前 `mask` 的异或 `changed = prev ^ cur`，`__builtin_ctzll(changed)` 就是唯一改变的元素。
> - **复杂度判定**：`2^n` 个状态，每个状态和相邻变化都可 `O(1)` 计算。
> - **维护的量**：无额外结构；只有编号 `index` 与灰码 `gray_code(index)` 的对应。
> - **警告**：这是遍历顺序技巧，不会降低 `2^n` 的总状态数；`n` 要保证 `1ULL << n` 合法；从编号 `i-1` 到 `i` 时翻转的位置是 `ctz(i)`，因为 `gray(i) = i ^ (i >> 1)`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// n=3 时输出全部 8 个 Gray Code。
auto order = all_gray_codes(3);
for (ui64 g : order) cout << g << ' ';
cout << '\n';
```

- 样例：输出 `0 1 3 2 6 7 5 4`。

**传参要求（照这个传不会错）：**

- `all_gray_codes(n)`：`0 <= n <= 62`；返回 `2^n` 个 `n` 位灰码的完整序列（`vector<ui64>`）。
- `gray_code(index)`：`index >= 0`；返回编号 `index` 对应的灰码 `index ^ (index >> 1)`。
- 相邻状态只差一位：翻转的位置是 `__builtin_ctzll(prev ^ cur)`。


**API / 入口函数（赛时只认这里列的名字）：**

- `all_gray_codes(int n)` -> 枚举所有 n 位 Gray Code；要求 n 在 [0,62]。 返回 `vector<ui64>`。

```cpp
ui64 gray_code(ui64 index) {
    return index ^ (index >> 1);
}

// 枚举所有 n 位 Gray Code；要求 n 在 [0,62]。
vector<ui64> all_gray_codes(int n) {
    assert(0 <= n && n <= 62);
    vector<ui64> order;
    order.reserve(1ULL << n);
    for (ui64 i = 0; i < (1ULL << n); ++i) order.push_back(gray_code(i));
    return order;
}
```

典题模型：有 `n <= 22` 个元素，需要统计每个子集的函数值，而增删一个元素可以 `O(1)` 更新。按 `all_gray_codes(n)` 遍历，维护前一个 `mask` 与当前 `mask` 的异或 `changed = prev ^ cur`，`__builtin_ctzll(changed)` 就是唯一改变的元素。
#### 前缀异或：O(1) 区间 XOR 与 0 XOR 子数组计数

> **赛时先看**
> - **题目信号**：题面出现连续子数组 xor、区间异或、两段前缀异或相等、找 xor 为 `0` 或固定值 `K` 的子数组个数。
> - **本质**：异或对应“加法”，逆运算仍是自己。前缀异或可在 `O(1)` 查询区间 XOR，也能计数异或值满足条件的子数组。
> - **接法**：给数组 `a`，问 xor 恰为 `K` 的连续子数组数量。将输入放进 1-indexed `vector<ui64>`，直接输出 `count_subarrays_with_xor(a,K)`；当 `K=0` 时，等价于统计相等前缀异或对。
> - **复杂度判定**：预处理和单次区间查询 `O(n)` / `O(1)`；哈希计数 `O(n)` 期望。
> - **维护的量**：`pre[i]`（前 i 个元素的异或前缀，1-indexed）；`frequency`（哈希表，各前缀异或值的出现次数）。
> - **警告**：区间 `[l,r]` 的 xor 是 `pre[r] ^ pre[l-1]`；统计固定 xor `K` 时，在加入当前前缀前先累加此前 `pre ^ K` 的出现次数；答案可能到 `O(n^2)`，用 `i64`。
> - **约定**：`a` 使用 1-indexed，`n = (int)a.size() - 1`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// a = [_, 1, 2, 3]（1-indexed），求区间 [2,3] 的 xor 与 xor 恰为 0 的子数组数。
vector<ui64> a = {0, 1, 2, 3};
auto pre = build_prefix_xor(a);
cout << range_xor(pre, 2, 3) << '\n';           // 2^3 = 1
cout << count_subarrays_with_xor(a, 0) << '\n'; // [1,3] 一个
```

- 样例：输出 `1` 与 `1`。

**传参要求（照这个传不会错）：**

- `a`：1-indexed，`a[0]` 是占位符；`n = a.size() - 1`。
- `build_prefix_xor(a)`：返回长度 `n+1` 的 `pre`，`pre[0] = 0`。
- `range_xor(pre, l, r)`：闭区间，`1 <= l <= r <= n`；返回 `pre[r] ^ pre[l-1]`。
- `count_subarrays_with_xor(a, target)`：统计 xor 恰为 `target` 的连续子数组个数（`i64`，最大 `O(n^2)`）。


```cpp
vector<ui64> build_prefix_xor(const vector<ui64>& a) {
    int n = (int)a.size() - 1; // a 使用 1-indexed。
    vector<ui64> pre(n + 1, 0);
    for (int i = 1; i <= n; ++i) pre[i] = pre[i - 1] ^ a[i];
    return pre;
}

ui64 range_xor(const vector<ui64>& pre, int l, int r) {
    return pre[r] ^ pre[l - 1];
}

i64 count_subarrays_with_xor(const vector<ui64>& a, ui64 target) {
    unordered_map<ui64, i64> frequency;
    frequency.reserve(a.size() * 2 + 1);
    frequency.max_load_factor(0.7F);
    ui64 pre = 0;
    i64 answer = 0;
    frequency[0] = 1;
    for (int i = 1; i < (int)a.size(); ++i) {
        pre ^= a[i];
        auto it = frequency.find(pre ^ target);
        if (it != frequency.end()) answer += it->second;
        ++frequency[pre];
    }
    return answer;
}
```

典题模型：给数组 `a`，问 xor 恰为 `K` 的连续子数组数量。将输入放进 1-indexed `vector<ui64>`，直接输出 `count_subarrays_with_xor(a,K)`；当 `K=0` 时，等价于统计相等前缀异或对。
#### 静态区间 OR / AND：Sparse Table

> **赛时先看**
> - **题目信号**：没有修改，询问很多；每次询问 `[l,r]` 的所有数按位或/按位与；或二分一个区间端点时需要快速判断该区间的 OR/AND。
> - **本质**：静态数组上反复查询一个区间的按位 OR 或按位 AND。它们和 `min/max/gcd` 一样满足幂等性，所以可用两段有重叠的 ST 表在 `O(1)` 查询；不能用前缀和相减，OR/AND 没有一般意义的逆运算。
> - **接法**：有 `q` 次静态区间询问，求 `[l,r]` 的 OR，或找最短/最长区间使区间 OR 达到某个掩码。先 `BitwiseSparseTable st(a)`，再用 `st.query_or(l,r)`；若条件随区间扩张单调，可在外层二分端点。
> - **复杂度判定**：预处理 `O(n log n)`，单次查询 `O(1)`，空间 `O(n log n)`。
> - **维护的量**：`n`（长度）；`lg[i]`（`floor(log2(i))`）；`st_or[k][l] / st_and[k][l]`（从 `l` 起长 `2^k` 的区间 OR / AND）。
> - **警告**：数组以下标 `0` 开始；这里查询是闭区间 `[l,r]`；`OR` 和 `AND` 都能把重叠两段再合并，但 `XOR` 不满足幂等性，区间 XOR 应使用前缀异或或不重叠 ST 表。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// a = {1, 6, 8, 12}，静态查询区间 OR 与区间 AND。
BitwiseSparseTable st(a);
cout << st.query_or(1, 3) << '\n';   // 6|8|12 = 14
cout << st.query_and(0, 2) << '\n';  // 1&6&8 = 0
```

- 样例：输出 `14` 与 `0`。

**传参要求（照这个传不会错）：**

- 构造 `BitwiseSparseTable(a)`：`a` 为 `vector<ui64>`，0-indexed。
- `query_or(l, r)` / `query_and(l, r)`：闭区间 `[l,r]`，`0 <= l <= r < n`；分别返回区间按位 OR / AND（`ui64`）。


```cpp
struct BitwiseSparseTable {
    int n = 0;
    vector<int> lg;
    vector<vector<ui64>> st_or, st_and;

    explicit BitwiseSparseTable(const vector<ui64>& a) {
        n = (int)a.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i >> 1] + 1;
        int levels = n ? lg[n] + 1 : 0;
        st_or.assign(levels, vector<ui64>(n));
        st_and.assign(levels, vector<ui64>(n));
        if (!n) return;
        st_or[0] = st_and[0] = a;
        for (int k = 1; k < levels; ++k) {
            int len = 1 << k;
            for (int l = 0; l + len <= n; ++l) {
                st_or[k][l] = st_or[k - 1][l] | st_or[k - 1][l + (len >> 1)];
                st_and[k][l] = st_and[k - 1][l] & st_and[k - 1][l + (len >> 1)];
            }
        }
    }

    ui64 query_or(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        int k = lg[r - l + 1];
        return st_or[k][l] | st_or[k][r - (1 << k) + 1];
    }

    ui64 query_and(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        int k = lg[r - l + 1];
        return st_and[k][l] & st_and[k][r - (1 << k) + 1];
    }
};
```

典题模型：有 `q` 次静态区间询问，求 `[l,r]` 的 OR，或找最短/最长区间使区间 OR 达到某个掩码。先 `BitwiseSparseTable st(a)`，再用 `st.query_or(l,r)`；若条件随区间扩张单调，可在外层二分端点。
#### 01 Trie：统计异或严格小于 K 的数对

> **赛时先看**
> - **题目信号**：题面要求 `a[i] xor a[j] < K`、`<= K`、异或值排名/第 k 小，或需要按顺序处理前缀中的异或阈值对。普通最大异或 Trie 只能贪心选相反位，出现比较符号时要改用本节的“贴着 K 走”的查询。
> - **本质**：在线插入整数，统计此前有多少个数满足 `x xor y < K`；据此求数组中满足异或阈值条件的无序数对数，也可作为“第 k 小两数异或”二分答案的判定器。
> - **接法**：求第 `k` 小的 `a[i] xor a[j] (i<j)`。答案具有单调性：`count_pairs_xor_leq(a, mid) >= k` 时第 `k` 小不大于 `mid`，对 `mid` 在无符号范围二分；若题目仅问异或小于 `K` 的对数，直接输出 `count_pairs_xor_less(a,K)`。
> - **复杂度判定**：插入和单次计数均为 `O(B)`，`B=64`；总计 `O(nB)`，空间 `O(nB)` 最坏。
> - **维护的量**：`tr`（节点数组，每个节点含 `ch[2]` 两个孩子与 `cnt` 子树计数）；`LOG=63`（从最高位 63 向下插入）。
> - **警告**：代码统计的是严格小于 `limit`；要统计 `<= K` 应调用 `count_pairs_xor_leq`，其中 `K=~0ULL` 需单独处理以免 `K+1` 溢出；数值按无符号 64 位解释，若题目给的是非负 `long long` 可直接转成 `ui64`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// a = {1, 2, 3}，统计异或严格小于 2 与不超过 1 的无序数对数。
vector<ui64> a = {1, 2, 3};
cout << count_pairs_xor_less(a, 2) << '\n';   // 1（2^3=1 < 2）
cout << count_pairs_xor_leq(a, 1) << '\n';    // 1（2^3=1 <= 1）
```

- 样例：输出 `1` 与 `1`。

**传参要求（照这个传不会错）：**

- `XorLessTrie trie`：先构造对象；`trie.add(x)` 插入一个 `ui64`；`trie.count_xor_less(x, limit)` 统计已插入的 `y` 中满足 `(x xor y) < limit` 的个数（`i64`，严格小于）。
- `count_pairs_xor_less(a, limit)`：整组 `a`（`vector<ui64>`），返回无序对数 `(i<j)` 中异或严格小于 `limit` 的个数。
- `count_pairs_xor_leq(a, limit)`：统计 `<= limit`；`limit == ~0ULL` 时直接返回全部对数。


**API / 入口函数（赛时只认这里列的名字）：**

- `add(ui64 x)` -> 插入一个数 `x`
- `count_xor_less(ui64 x, ui64 limit)` -> 统计已插入的 y 中满足 (x xor y) < limit 的个数。 返回 `i64`。

```cpp
struct XorLessTrie {
    static constexpr int LOG = 63;

    struct Node {
        int ch[2] = {-1, -1};
        int cnt = 0;
    };

    vector<Node> tr{Node{}};

    void add(ui64 x) {
        int u = 0;
        ++tr[u].cnt;
        for (int b = LOG; b >= 0; --b) {
            int c = (x >> b) & 1ULL;
            if (tr[u].ch[c] == -1) {
                tr[u].ch[c] = (int)tr.size();
                tr.push_back(Node{});
            }
            u = tr[u].ch[c];
            ++tr[u].cnt;
        }
    }

    // 统计已插入的 y 中满足 (x xor y) < limit 的个数。
    i64 count_xor_less(ui64 x, ui64 limit) const {
        int u = 0;
        i64 answer = 0;
        for (int b = LOG; b >= 0 && u != -1; --b) {
            int xb = (x >> b) & 1ULL;
            int kb = (limit >> b) & 1ULL;
            if (kb) {
                int same = tr[u].ch[xb]; // 这一位让 xor 取 0，小于 limit 的 1。
                if (same != -1) answer += tr[same].cnt;
                u = tr[u].ch[xb ^ 1];    // 保持当前前缀仍等于 limit。
            } else {
                u = tr[u].ch[xb];        // 这一位必须让 xor 取 0。
            }
        }
        return answer;
    }
};

i64 count_pairs_xor_less(const vector<ui64>& a, ui64 limit) {
    XorLessTrie trie;
    i64 answer = 0;
    for (ui64 x : a) {
        answer += trie.count_xor_less(x, limit);
        trie.add(x);
    }
    return answer;
}

i64 count_pairs_xor_leq(const vector<ui64>& a, ui64 limit) {
    if (limit == ~0ULL) {
        i64 n = (i64)a.size();
        return n * (n - 1) / 2;
    }
    return count_pairs_xor_less(a, limit + 1);
}
```

典题模型：求第 `k` 小的 `a[i] xor a[j] (i<j)`。答案具有单调性：`count_pairs_xor_leq(a, mid) >= k` 时第 `k` 小不大于 `mid`，对 `mid` 在无符号范围二分；若题目仅问异或小于 `K` 的对数，直接输出 `count_pairs_xor_less(a,K)`。
#### 按位贪心：最大两数按位 AND

> **赛时先看**
> - **题目信号**：题目出现“选两个数”“最大 `a[i] & a[j]`”“最大公共置位集合”；有时会扩展成至少选 `k` 个数，其判定条件就是“满足候选掩码的数至少有 `k` 个”。
> - **本质**：求数组中任意两个不同元素按位 AND 的最大值。逐位从高到低尝试把答案该位置为 `1`，只要仍有至少两个数包含当前候选掩码即可保留。
> - **接法**：给 `n` 个非负数，选两个数使 AND 最大。直接调用 `maximum_pairwise_and(a)`；若要求选 `k` 个数的 AND 最大，把代码中 `++count == 2` 与 `count >= 2` 都改成 `k`。
> - **复杂度判定**：`O(nB)`，这里 `B=64`；额外空间 `O(1)`。
> - **维护的量**：`answer`（已确定的高位答案）与 `candidate`（本轮尝试把第 `b` 位置 1 的候选掩码）。
> - **警告**：这是求数值最大，必须从最高位往最低位试；不要每一位独立选最大出现次数；题目要求 `n >= 2`，并且应使用 `ui64`，避免第 63 位的有符号移位问题。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// a = {3, 5, 7}，选两个数使 AND 最大。
vector<ui64> a = {3, 5, 7};
cout << maximum_pairwise_and(a) << '\n';   // 5（5 & 7 = 5）
```

- 样例：输出 `5`。

**传参要求（照这个传不会错）：**

- `maximum_pairwise_and(a)`：入参 `a`（`vector<ui64>`，非负数，`size() >= 2`）；返回 `ui64`，最大两数 AND 值。
- 要选 `k` 个数：把代码里 `++count == 2` 与 `count >= 2` 中的 `2` 改成 `k`。


```cpp
ui64 maximum_pairwise_and(const vector<ui64>& a) {
    assert(a.size() >= 2);
    ui64 answer = 0;
    for (int b = 63; b >= 0; --b) {
        ui64 candidate = answer | (1ULL << b);
        int count = 0;
        for (ui64 x : a) {
            if ((x & candidate) == candidate && ++count == 2) break;
        }
        if (count >= 2) answer = candidate;
    }
    return answer;
}
```

典题模型：给 `n` 个非负数，选两个数使 AND 最大。直接调用 `maximum_pairwise_and(a)`；若要求选 `k` 个数的 AND 最大，把代码中 `++count == 2` 与 `count >= 2` 都改成 `k`。
#### 子数组按位 OR / AND：不同结果集合

> **赛时先看**
> - **题目信号**：题目问“不同子数组 OR/AND 的数量”“所有子数组按位或/与的值集合”“是否存在 OR/AND 恰为某数”；数组元素是非负整数，且不能枚举 `O(n^2)` 个子数组。
> - **本质**：统计所有连续子数组的不同 OR 值或不同 AND 值。固定右端点时，所有后缀的 OR 只会不断把 1 位变多，AND 只会不断把 1 位变少，所以不同结果数量至多 `B+1` 个。
> - **接法**：数组长度 `n=2e5`，求所有子数组按位 OR 的不同结果数。不要枚举左右端点；直接输出 `count_distinct_subarray_or(a)`。若题目问某个值 `K` 是否出现，可把全局 `unordered_set` 保留并查询 `contains(K)`（C++17 写 `find(K) != end()`）。
> - **复杂度判定**：每个右端点最多保留 `B+1` 个值；本实现为 `O(n B log B)`，在 64 位数下近似线性，空间为答案集合大小。若只问存在性或计数，也可以把 `all_values` 改成相应的聚合结构。
> - **维护的量**：`previous`（固定右端点时的所有不同后缀结果，已排序去重）与 `all_values`（全局不同结果集合）。
> - **警告**：这个“不同后缀结果只有 `O(B)` 个”的结论对 OR/AND 成立，对 XOR 不成立；不同子数组 XOR 的个数可以达到 `Theta(n^2)`。代码每轮排序去重，不容易因为维护顺序出错。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// a = {1, 2, 3}，统计不同子数组 OR / AND 的结果个数。
vector<ui64> a = {1, 2, 3};
cout << count_distinct_subarray_or(a) << '\n';    // 3（{1,2,3,3,3,3} 去重）
cout << count_distinct_subarray_and(a) << '\n';   // 4（{1,2,3,0,2,0} 去重）
```

- 样例：输出 `3` 与 `4`。

**传参要求（照这个传不会错）：**

- `count_distinct_subarray_or(a)` / `count_distinct_subarray_and(a)`：入参 `a`（`vector<ui64>`，非负数）；返回 `i64`，不同结果个数。
- 问某个值 `K` 是否出现：把全局集合 `all_values` 保留下来并查询 `contains(K)`（C++17 写 `find(K) != end()`）。


```cpp
template <class Combine>
i64 count_distinct_subarray_bitwise_values(const vector<ui64>& a, Combine combine) {
    vector<ui64> previous;
    unordered_set<ui64> all_values;
    all_values.reserve(a.size() * 8 + 1);
    all_values.max_load_factor(0.7F);

    for (ui64 x : a) {
        vector<ui64> current;
        current.reserve(previous.size() + 1);
        current.push_back(x);
        for (ui64 value : previous) current.push_back(combine(value, x));
        sort(current.begin(), current.end());
        current.erase(unique(current.begin(), current.end()), current.end());
        for (ui64 value : current) all_values.insert(value);
        previous.swap(current);
    }
    return (i64)all_values.size();
}

i64 count_distinct_subarray_or(const vector<ui64>& a) {
    return count_distinct_subarray_bitwise_values(
        a, [](ui64 x, ui64 y) { return x | y; }
    );
}

i64 count_distinct_subarray_and(const vector<ui64>& a) {
    return count_distinct_subarray_bitwise_values(
        a, [](ui64 x, ui64 y) { return x & y; }
    );
}
```

典题模型：数组长度 `n=2e5`，求所有子数组按位 OR 的不同结果数。不要枚举左右端点；直接输出 `count_distinct_subarray_or(a)`。若题目问某个值 `K` 是否出现，可把全局 `unordered_set` 保留并查询 `contains(K)`（C++17 写 `find(K) != end()`）。
#### 按位贡献：所有无序数对的 XOR / OR / AND 和

> **赛时先看**
> - **题目信号**：出现“所有数对”“按位运算结果之和”“分别求 XOR/OR/AND 总和”，且答案不取模或模数允许最后取模。遇到这类题，不要枚举 `O(n^2)` 个数对。
> - **本质**：快速求 `sum_{i<j}(a[i] xor a[j])`、`sum_{i<j}(a[i] | a[j])`、`sum_{i<j}(a[i] & a[j])`。每一位彼此独立，只需统计该位为 `1` 的个数。
> - **接法**：对每组数组输出全部无序数对 OR 的和。写 `auto result = sum_pairwise_bitwise(a);`，然后输出 `result.or_sum`；若题目要求模 `MOD`，可以在每次加法后取模，或最后把 `i128` 对 `MOD` 取模。
> - **复杂度判定**：`O(nB)`，空间 `O(1)`。
> - **维护的量**：`PairwiseBitwiseSums`（`xor_sum` / `or_sum` / `and_sum` 三个 `i128` 累加器）与每位的 `ones`（该位为 1 的个数）。
> - **警告**：`XOR` 的该位贡献是 `ones * zeros`；`OR` 是全部数对减去两端都是 `0` 的数对；`AND` 是两端都是 `1` 的数对。总和可能超过 `i64`，这里使用 `i128`，输出时搭配本章 01 小节的 `i128` 输入输出重载。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// a = {1, 2, 3}，求所有无序数对的 XOR / OR / AND 总和。
vector<ui64> a = {1, 2, 3};
auto result = sum_pairwise_bitwise(a);
cout << result.xor_sum << '\n';    // 6（1^2 + 1^3 + 2^3 = 3+2+1）
cout << result.or_sum << '\n';     // 9（1|2 + 1|3 + 2|3 = 3+3+3）
cout << result.and_sum << '\n';    // 3（1&2 + 1&3 + 2&3 = 0+1+2）
```

- 样例：输出 `6`、`9` 与 `3`。

**传参要求（照这个传不会错）：**

- `sum_pairwise_bitwise(a)`：入参 `a`（`vector<ui64>`，非负数）；返回 `PairwiseBitwiseSums`，字段 `xor_sum` / `or_sum` / `and_sum` 均为 `i128`，直接取字段输出。
- 输出 `i128`：需要先抄本章「`i128` 输入输出」小节的输出重载。
- 要取模：每次加法后对 `MOD` 取模，或最后把 `i128` 结果对 `MOD` 取模。


```cpp
struct PairwiseBitwiseSums {
    i128 xor_sum = 0;
    i128 or_sum = 0;
    i128 and_sum = 0;
};

PairwiseBitwiseSums sum_pairwise_bitwise(const vector<ui64>& a) {
    i128 n = (i128)a.size();
    i128 all_pairs = n * (n - 1) / 2;
    PairwiseBitwiseSums answer;
    for (int b = 0; b < 64; ++b) {
        i128 ones = 0;
        for (ui64 x : a) ones += (x >> b) & 1ULL;
        i128 zeros = n - ones;
        i128 bit_value = (i128)1 << b;
        answer.xor_sum += ones * zeros * bit_value;
        answer.or_sum += (all_pairs - zeros * (zeros - 1) / 2) * bit_value;
        answer.and_sum += ones * (ones - 1) / 2 * bit_value;
    }
    return answer;
}
```

典题模型：对每组数组输出全部无序数对 OR 的和。写 `auto result = sum_pairwise_bitwise(a);`，然后输出 `result.or_sum`；若题目要求模 `MOD`，可以在每次加法后取模，或最后把 `i128` 对 `MOD` 取模。
#### OR / AND / XOR 卷积与子集变换：快速定位

> **赛时先看**
> - **题目信号**：数组长度是 `2^m`（通常 `m <= 20`）；需要统计所有有序对/两组选法合并后的掩码，或出现 `A | B = S`、`A & B = S`、`A xor B = S`、`T subset S` 等精确关系。
> - **本质**：当状态下标是掩码 `S`，并且两个状态按 `|`、`&`、`^` 合并时，用相应 FWT/FWHT 把朴素 `O(4^m)` 的卷积降为 `O(m 2^m)`；当题目求所有子集/超集的贡献和时用 SOS DP。
> - **接法**：有两个掩码多重集合，问任选一个掩码后其 OR 恰为 `S` 的有序对数量。把频率写入长度 `2^m` 的数组 `a,b`，调用 E 章 11 的 `or_convolution(a,b,mod)`；若合并符号改成 XOR，转到同节的 `xor_convolution`。
> - **复杂度判定**：FWT / FWHT / SOS DP 均为 `O(m 2^m)`，空间 `O(2^m)`。
> - **维护的量**：无本地结构；对象是各章的频率数组 `a,b`（长度 `2^m`，`a[S]` = 掩码 `S` 出现次数）与对应的变换函数（`fwt_or` / `fwt_and` / XOR FWT / SOS DP）。
> - **警告**：这是掩码数组的整体变换，不是普通区间 OR/AND 查询；OR 与 AND 的正反变换方向不同，XOR 逆变换还需要乘长度的逆元。完整可抄代码不要重复粘在本节，直接翻下列位置：


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 两集合掩码频率 a、b，问任选一个掩码后 OR 恰为 S 的有序对数量（E 章 11 抄 or_convolution）。
int m = 3;                      // 掩码位数，数组长度 2^m
vector<i64> a(1 << m, 0), b(1 << m, 0);
a[1] = 2; b[2] = 1;             // 填频率：掩码 1 出现 2 次、掩码 2 出现 1 次
auto conv = or_convolution(a, b, MOD);
cout << conv[3] << '\n';        // 2（1|2 = 3 的有序对：2*1）
```

- 样例：输出 `2`。

**传参要求（照这个传不会错）：**

- `or_convolution(a, b, mod)`：`a/b` 为长度 `2^m` 的频率数组（`vector<i64>`，`a[S]` = 掩码 `S` 出现次数），`mod` 为模数；返回 `vector<i64>` 卷积结果，下标 `S` 即合并掩码。
- `and_convolution(a, b, mod)` / `xor_convolution(a, b, mod)`：签名同上，合并符号分别为 `&`、`^`。
- 底层变换：`fwt_or(a, mod, inverse)` / `fwt_and(a, mod, inverse)` 原地操作，`inverse=false` 正变换、`true` 逆变换；XOR 逆变换注意乘长度的逆元。


- `A | B = S`：E 章 11「FWT：AND / OR 卷积」中的 `fwt_or`。
- `A & B = S`：E 章 11「FWT：AND / OR 卷积」中的 `fwt_and`。
- `A xor B = S`：E 章 11「FWHT：XOR 卷积」。
- `sum f[T], T subset S` 或 `S subset T`：F 章 13「SOS DP：子集和变换」。

典题模型：有两个掩码多重集合，问任选一个掩码后其 OR 恰为 `S` 的有序对数量。把频率写入长度 `2^m` 的数组 `a,b`，调用 E 章 11 的 `or_convolution(a,b,mod)`；若合并符号改成 XOR，转到同节的 `xor_convolution`。

#### bitset 01 背包：只问哪些和可达

> **赛时先看**
> - **题目信号**：每个物品至多选一次；问子集和、能否分成相等两组、最接近目标但不超过目标的和；重量上界较大但仍能开位集（常见 `1e5~1e7`，取决于内存和时限）。
> - **本质**：0/1 选择若干非负重量，只关心某个和能否凑出、最大不超过上界的可达和，而不关心方案数或最大价值。用一个位集并行模拟整个布尔 DP。
> - **接法**：给 `n` 个正整数，问是否能选一些数使和恰为 `target`，或把数组分成两组且和最接近。若总和 `sum <= S`，取 `auto reachable = subset_sum_01_bitset<S>(w)`；等分判断是 `sum` 偶数且 `reachable[sum/2]`。
> - **复杂度判定**：若上界为 `S`，时间 `O(nS / word_size)`，空间 `O(S)` bit。C++ `bitset` 的长度必须是编译期常量。
> - **维护的量**：`reachable`（`bitset<MAX_SUM+1>`，`reachable[s] = 1` 表示和 `s` 可达；`reachable[0]` 恒为 1）。
> - **警告**：`reachable |= reachable << w` 对应 0/1 背包；完全背包不能直接照抄；`w` 必须非负且不超过 `MAX_SUM`；`std::bitset` 不是动态长度，运行时上界用 `vector<ui64>` 或其他动态位集。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// weights = {2, 3, 5}，问能否凑出 10，并求不超过 9 的最大可达和。
constexpr int S = 1000;                          // 编译期上界，取 sum 或题给上界
vector<int> weights = {2, 3, 5};
auto reachable = subset_sum_01_bitset<S>(weights);
cout << reachable[10] << '\n';                   // 1（2+3+5=10 可达）
for (int s = 9; s >= 0; --s)
    if (reachable[s]) { cout << s << '\n'; break; }   // 8（3+5=8）
```

- 样例：输出 `1` 与 `8`。

**传参要求（照这个传不会错）：**

- `subset_sum_01_bitset<MAX_SUM>(weight)`：模板参数 `MAX_SUM` 是编译期上界（总和 `sum <= MAX_SUM` 即可，最大取约 1e7）；入参 `weight`（`vector<int>`，每个 `0 <= w <= MAX_SUM`）；返回 `bitset<MAX_SUM + 1>`，`reachable[s]` 为 1 表示和 `s` 可达。
- 等分判断：`sum` 为偶数且 `reachable[sum / 2]`。
- 完全背包不能照抄 `reachable |= reachable << w`；运行时才知道上界时改用动态位集。

```cpp
template <int MAX_SUM>
bitset<MAX_SUM + 1> subset_sum_01_bitset(const vector<int>& weight) {
    bitset<MAX_SUM + 1> reachable;
    reachable[0] = 1;
    for (int w : weight) {
        assert(0 <= w && w <= MAX_SUM);
        reachable |= reachable << w;
    }
    return reachable;
}

// 示例：
// 示例：constexpr int S = 200000;
// 示例：auto reachable = subset_sum_01_bitset<S>(weights);
// 示例：bool can_make_target = reachable[target];
// 示例：int best = -1;
// 示例：for (int s = target; s >= 0; --s) if (reachable[s]) { best = s; break; }
```

典题模型：给 `n` 个正整数，问是否能选一些数使和恰为 `target`，或把数组分成两组且和最接近。若总和 `sum <= S`，取 `auto reachable = subset_sum_01_bitset<S>(w)`；等分判断是 `sum` 偶数且 `reachable[sum/2]`。

#### 整数二分：最小可行值

> **赛时先看**
> - **题目信号**：题面出现“最小的最大值”“最早时间”“至少多少”；判定 `check(x)` 随 x 单调（x 越大越容易可行）。这类“答案单调 + 可判定”直接二分。
> - **本质**：把“求最优答案”转成“猜一个答案 mid，验证是否可行”；单调性保证猜的方向唯一，二分把 O（答案范围） 压缩成 O(log V) 次判定。
> - **复杂度判定**：`O(log V * check)`；V 是答案范围（1e18 也没问题）；若 check 本身 O(n)，n ≤ 2e5 时约 30-60 次判定可过；若题目不单调，不能二分，改枚举或三分。
> - **维护的量**：无额外结构；只维护二分边界 `l/r` 与判定函数 `check`。
> - **接法**：把 `check(mid)` 写成题目的可行性判定，然后一行 `min_true(l, r, check)`。
> - **警告**：必须先证明 `check(x)` 单调；`mid = l + (r-l)/2` 防溢出。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `min_true` 模板函数。
2. 构造：把 `l` 设成“一定不可行”，`r` 设成“一定可行”。
3. 调用：`i64 ans = min_true(l, r, check);`
4. 取结果：`ans` 就是最小可行值，直接输出。

**改造点（按题目改这几处）：**
- 判定函数：把 `check` 换成题目的可行性判定（这是唯一要写的内容）。
- 边界：`l/r` 从题面数据范围取，通常 `l = 0 或 1`，`r = 1e18` 或已知上界。
- 返回类型：答案不是 i64 时改模板参数或外面转。
- 要找“最大可行值”：用下一节 `max_true`（上取中位数，防死循环）。

```cpp
// 维护的量：二分边界 l/r 与判定函数 check（check(x) 表示 x 是否可行）。
// 不变量：l 一定不可行、r 一定可行；区间长度每次减半，结束时 l = r 即最小可行值。
template <class Check>
i64 min_true(i64 l, i64 r, Check check) {
    while (l < r) {
        i64 mid = l + (r - l) / 2; // 下取中位数，l + r 不会溢出。
        if (check(mid)) r = mid;   // mid 可行，答案落在 [l, mid]。
        else l = mid + 1;          // mid 不可行，答案落在 (mid, r]。
    }
    return l;
}
```

**最小完整示例（先抄这一段就能跑）：**

题目：速度 `v=3`，距离 `d=100`，求最早到达（`3*t >= 100`）的最小整数秒数。

```cpp
// check(t)：t 秒能走 3t，够 100 就可行；l=0 一定不可行，r=1e18 一定可行。
auto check = [](i64 t) { return 3LL * t >= 100; };
i64 ans = min_true(0, 1000000000000000000LL, check);
cout << ans << '\n'; // 34：3*33=99 不够，3*34=102 够
```

- 样例：输出 `34`。

**传参要求（照这个传不会错）：**

- `l`：`i64`，一定不可行的下界（含），一般取 `0` 或 `1`。
- `r`：`i64`，一定可行的上界（含），一般取 `1e18` 或题面已知上界。
- `check`：`bool check(i64 x)`，返回 `x` 是否可行；必须随 `x` 单调（x 越大越容易可行）。
- 返回值：`i64`，最小可行值 `x`，满足 `check(x)=true` 且 `check(x-1)=false`。
#### 整数二分：最大可行值

> **赛时先看**
> - **题目信号**：题面问"最多能多少""最大的最小值""距离至少多少"，答案越大越难满足。
> - **本质**：与最小可行值完全对称：`check(x)` 表示"x 可行"，x 越大越难可行，二分找最大的可行 x。唯一区别是中位数要**上取整**，否则区间长为 2 时会死循环。
> - **复杂度判定**：`O(log V * check)`；用法与数据规模限制同上一节 `min_true`。
> - **维护的量**：二分边界 `l/r`（l 一定可行、r 一定不可行）与判定函数 `check`。
> - **接法**：`check(mid)` 写成题面可行性判定，然后 `i64 ans = max_true(l, r, check);` 输出 ans。
> - **警告**：必须用 `mid = l + (r-l+1)/2` 上取中位数，否则 `l=mid` 无法推进；与上一节配对的"答案范围单调"判断不要写反。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `max_true` 模板函数（和上一节 `min_true` 一起抄，两者常配对使用）。
2. 构造：`l` 设成"一定可行"的下界，`r` 设成"一定不可行"的上界。
3. 调用：`i64 ans = max_true(l, r, check);`
4. 取结果：`ans` 即最大可行值，直接输出。

**改造点（按题目改这几处）：**
- 判定函数：换成题目的可行性判定（唯一要写的内容）。
- 中位数：**不要**改成下取整，`(r-l+1)/2` 是防死循环的关键。
- 答案单调方向反了：把"可行"的定义取反，或用上一节 `min_true`。

```cpp
// 维护的量：二分边界 l/r（l 一定可行、r 一定不可行）与判定函数 check。
// 不变量：check(l)=true 且 check(r)=false；结束时 l 是最大的可行值。
template <class Check>
i64 max_true(i64 l, i64 r, Check check) {
    while (l < r) {
        i64 mid = l + (r - l + 1) / 2; // 上取中位数：保证 l=mid 时区间仍会缩小。
        if (check(mid)) l = mid;        // mid 可行，答案落在 [mid, r]。
        else r = mid - 1;               // mid 不可行，答案落在 [l, mid-1]。
    }
    return l;
}
```

**最小完整示例（先抄这一段就能跑）：**

题目：速度 `v=3`，距离 `d=100`，求不超过距离（`3*t <= 100`）的最大整数秒数。

```cpp
// check(t)：3*t <= 100 即可行；l=0 一定可行，r=1e18 一定不可行。
auto check = [](i64 t) { return 3LL * t <= 100; };
i64 ans = max_true(0, 1000000000000000000LL, check);
cout << ans << '\n'; // 33：3*33=99 <= 100，3*34=102 超了
```

- 样例：输出 `33`。

**传参要求（照这个传不会错）：**

- `l`：`i64`，一定可行的下界（含），一般取 `0` 或 `1`。
- `r`：`i64`，一定不可行的上界（含），一般取 `1e18` 或题面已知上界。
- `check`：`bool check(i64 x)`，返回 `x` 是否可行；必须随 `x` 单调（x 越大越难可行）。
- 返回值：`i64`，最大可行值 `x`，满足 `check(x)=true` 且 `check(x+1)=false`。
#### 二分答案 + 区间覆盖判定

> **赛时先看**
> - **题目信号**：每个源点随时间向两侧扩展；要求覆盖 `[0,k]` 所有整数点；答案具有单调性。
> - **本质**：若干事件在时间 `t` 后覆盖一段区间，求最早覆盖完整目标区间的时间。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：单次判定 `O(n log n)`，二分后 `O(n log n log V)`。
> - **维护的量**：`segs`（时间 `t` 下每个源点展开的覆盖区间 `[l,r]`）与 `cur`（当前最小未覆盖整数点）。
> - **警告**：如果覆盖的是整数点，维护“当前最小未覆盖整数” `cur`；区间右端覆盖到 `r` 后，下一个未覆盖点是 `r + 1`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 事件 (出现时间, 位置)，t 时刻覆盖 [pos-(t-出现时间), pos+(t-出现时间)]，求最早盖满 [0,12] 的时间。
vector<pair<i64, i64>> events = {{0, 2}, {3, 10}};
cout << earliest_full_cover(events, 12) << '\n';   // 5（t=5 时覆盖 [0,7] 与 [8,12] 正好接满）
```

- 样例：输出 `5`。

**传参要求（照这个传不会错）：**

- `cover_integer_segment(events, k, t)`：`events` 为 `vector<pair<i64,i64>>`（first=出现时间，second=位置），`k` 为目标右端（覆盖 `[0,k]`），`t` 为当前时间；返回 `bool` 是否已全覆盖。
- `earliest_full_cover(events, k)`：入参同上，返回最早可行时间（`i64`）；内部自动倍增上界再二分。
- 覆盖的是整数点：一段盖到 `r` 后下一个未覆盖点是 `r + 1`。


```cpp
bool cover_integer_segment(const vector<pair<i64, i64>>& events, i64 k, i64 t) {
    vector<pair<i64, i64>> segs;
    for (auto [appear_time, pos] : events) {
        if (t < appear_time) continue;
        i64 d = t - appear_time;
        i64 l = max<i64>(0, pos - d);
        i64 r = min<i64>(k, pos + d);
        segs.push_back({l, r});
    }
    sort(segs.begin(), segs.end());
    i64 cur = 0;
    for (auto [l, r] : segs) {
        if (r < cur) continue;
        if (l > cur) return false;
        cur = r + 1;
        if (cur > k) return true;
    }
    return cur > k;
}

i64 earliest_full_cover(vector<pair<i64, i64>> events, i64 k) {
    i64 lo = 0, hi = 1;
    while (!cover_integer_segment(events, k, hi)) hi <<= 1;
    while (lo < hi) {
        i64 mid = (lo + hi) >> 1;
        if (cover_integer_segment(events, k, mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}
```
#### 最大化最小距离：直线/圆周二分

> **赛时先看**
> - **题目信号**：求最大最小距离；选点集合固定；判定答案 `d` 可行具有单调性。
> - **本质**：从若干位置中选 `k` 个，使任意相邻选点的距离至少为 `d`，常用于“最大化最小距离”。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：直线判定 `O(n)`；圆周判定 `O(n log n)`。
> - **维护的量**：直线版只需贪心游标 `last` 与计数 `cnt`；圆周版额外维护 `nxt`（每个位置向后最早可选点）、`up`（倍增跳表）、`cur`（当前选点下标）。
> - **警告**：直线只需贪心；圆周必须额外检查最后一个选点到第一个选点绕回去的距离。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 直线：{1, 5, 9} 中选 2 个点，间距至少 d。
cout << can_pick_on_line({1, 5, 9}, 2, 5) << '\n';        // 1（选 1 与 9，间距 8）
cout << can_pick_on_line({1, 5, 9}, 2, 9) << '\n';        // 0（最大间距 8 < 9）
// 圆周：周长 12 的圆上 {0, 4, 8} 选 3 个点，含绕回一圈的距离。
cout << can_pick_on_circle({0, 4, 8}, 12, 3, 4) << '\n';  // 1（间距 4,4,4 均满足）
cout << can_pick_on_circle({0, 4, 8}, 12, 3, 5) << '\n';  // 0（8 绕回 0 只有 4）
```

- 样例：输出 `1`、`0`、`1`、`0`。

**传参要求（照这个传不会错）：**

- `can_pick_on_line(x, k, d)`：`x`（`vector<i64>` 候选点，函数内会排序，可传任意顺序）、`k`（要选的点数）、`d`（最小间距）；返回 `bool`，从左到右贪心能选够 `k` 个即 true。
- `can_pick_on_circle(p, circumference, k, d)`：`p`（圆上点坐标，`0 <= p[i] < circumference`）、`circumference`（周长）、`k`、`d`；返回 `bool`；`k <= 1` 时只要 `n >= k`，`d == 0` 时直接 true。
- 求最大可行间距：外层对 `d` 二分，如 `max_true(0, 上界, [&](i64 d){ return can_pick_on_line(x, k, d); })`。


**改板时先认这几个量：**

- `up`：倍增跳表，表示从当前位置再跳若干次选点。
- `cur`：当前选点下标。
- `nxt`：每个位置向后能跳到的最早选点。

```cpp
bool can_pick_on_line(vector<i64> x, int k, i64 d) {
    sort(x.begin(), x.end());
    int cnt = 0;
    i64 last = -(1LL << 60);
    for (i64 v : x) {
        if (v - last >= d) {
            cnt++;
            last = v;
        }
    }
    return cnt >= k;
}

bool can_pick_on_circle(vector<i64> p, i64 circumference, int k, i64 d) {
    sort(p.begin(), p.end());
    int n = (int)p.size();
    if (k <= 1) return n >= k;
    if (n < k) return false;
    if (d == 0) return true;
    vector<i64> q = p;
    for (i64 v : p) q.push_back(v + circumference);
    int m = (int)q.size();
    vector<int> nxt(m, m);
    for (int i = 0; i < m; i++) {
        nxt[i] = lower_bound(q.begin(), q.end(), q[i] + d) - q.begin();
    }
    int LOG = 1;
    while ((1 << LOG) <= k) LOG++;
    vector up(LOG, vector<int>(m + 1, m));
    up[0] = nxt;
    up[0].push_back(m);
    for (int j = 1; j < LOG; j++) {
        for (int i = 0; i <= m; i++) up[j][i] = up[j - 1][up[j - 1][i]];
    }
    for (int start = 0; start < n; start++) {
        int cur = start;
        int steps = k - 1;
        for (int j = 0; steps; j++, steps >>= 1) {
            if (steps & 1) cur = up[j][cur];
        }
        if (cur < start + n && q[cur] - q[start] <= circumference - d) return true;
    }
    return false;
}
```
#### 实数二分

> **赛时先看**
> - **题目信号**：几何、速度、概率、浮点答案。
> - **本质**：连续答案，误差允许 `1e-6` 或 `1e-9`。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：固定迭代 100 次。
> - **维护的量**：二分边界 `l/r`（`double`）与判定函数 `check`（单调的 `double -> bool`）。
> - **警告**：输出精度要比题目要求多 2 位。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 求 sqrt(2)：check 单调递增，返回最小满足 x^2 >= 2 的 x。
double ans = real_binary(0.0, 10.0, [](double x) { return x * x >= 2.0; });
cout << fixed << setprecision(10) << ans << '\n';   // 1.4142135624
```

- 样例：输出 `1.4142135624`。

**传参要求（照这个传不会错）：**

- `real_binary(l, r, check)`：`l/r` 为 `double` 二分边界（`l` 侧 check 为 false、`r` 侧为 true），`check` 是 `double -> bool` 的单调判定；固定迭代 100 次，返回 `double`。
- 输出精度：题目要 `1e-6` 就至少输出 8 位小数（比要求多 2 位）。


```cpp
template <class Check>
double real_binary(double l, double r, Check check) {
    for (int it = 0; it < 100; ++it) {
        double mid = (l + r) / 2.0;
        if (check(mid)) r = mid;
        else l = mid;
    }
    return (l + r) / 2.0;
}
```
#### 三分搜索

> **赛时先看**
> - **题目信号**：连续变量，函数先降后升或先升后降。
> - **本质**：单峰/单谷函数求极值。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：固定迭代 120 次。
> - **维护的量**：三分点 `m1/m2`（`l` 与 `r` 的三等分点）与单峰函数 `f`（`double -> double`）。
> - **警告**：只适用于单峰；整数三分最后要枚举小区间。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// f(x) = (x-1)^2 + 3 在 x=1 取最小值，三分返回极小值点。
double ans = ternary_min(-10.0, 10.0, [](double x) { return (x - 1) * (x - 1) + 3; });
cout << fixed << setprecision(6) << ans << '\n';   // 1.000000
```

- 样例：输出 `1.000000`。

**传参要求（照这个传不会错）：**

- `ternary_min(l, r, f)`：`l/r` 为 `double` 搜索区间（须包含唯一极小值点），`f` 是 `double -> double` 的单谷函数；固定迭代 120 次，返回极小值点（`double`）。
- 求极大值：对 `-f` 调用 `ternary_min` 后返回点不变，或换求 `f` 取反后的最小值点。
- 整数变量：三分后要在最后的小区间内枚举所有整数点取最值。


```cpp
template <class F>
double ternary_min(double l, double r, F f) {
    for (int it = 0; it < 120; ++it) {
        double m1 = l + (r - l) / 3.0;
        double m2 = r - (r - l) / 3.0;
        if (f(m1) < f(m2)) r = m2;
        else l = m1;
    }
    return (l + r) / 2.0;
}
```
#### 离散化

> **赛时先看**
> - **题目信号**：坐标/值域范围 `1e9/1e18`，但实际出现的点只有 `1e5`；题目只关心大小关系（排序、当数组下标、开 BIT/线段树）而不关心具体值。
> - **本质**：把巨大且稀疏的坐标压缩成连续的 `1..m`：排序去重后 `id(x)` 拿排名、`value(id)` 还原原值；从此“按值操作”全部变成“按下标操作”。
> - **复杂度判定**：排序去重 `O(n log n)`，单次 `id`/`value` 查询 `O(log n)`（lower_bound）；n 到 2e5、1e6 都轻松，别用 map 拖慢。
> - **维护的量**：`xs`（排序去重后的原坐标表，编号 `i` 对应 `xs[i-1]`）。
> - **接法**：先把所有会被访问的坐标 `add` 进去，`build()` 后用 `id(x)` 作为树状数组/线段树下标。区间覆盖和扫描线如果要算真实长度，不能只用压缩后的编号相减，要用 `value(id+1)-value(id)` 这种原坐标差。
> - **警告**：区间覆盖题常要额外加入 `l`、`r`、`r+1`；长度计算要用原坐标。


**API / 入口函数（赛时只认这里列的名字）：**

- `add(T x)` -> 收集一个坐标（build 前必须加完全部会被访问的值）
- `build()` -> 完成建树或预处理
- `size()` -> 查询集合大小 返回 `int`。

**抄板清单（照着做就行）：**
1. 抄哪段：整个 `Compressor` 结构体。
2. 收集：把所有会被访问到的坐标全部 `add` 一遍（区间题把 `l/r/r+1` 也加进去）。
3. 建表：调用一次 `build()` 完成排序去重。
4. 使用：`id(x)` 得到 `1..m` 的编号当下标；要还原原坐标用 `value(id)`。

**改造点（按题目改这几处）：**
- 收集范围：漏加任何一个会被查询的坐标都会让 `id` 错位，宁多勿少。
- 模板参数：坐标是 `int` 还是 `i64`，决定 `Compressor<int>` 还是 `Compressor<i64>`。
- 只比大小：直接拿编号用；要算长度/差值：用 `value(...)` 还原原坐标再减。
- 离散化后的值域：`size()` 返回 `m`，开数组就开到 `m+1`（1-indexed）。

```cpp
// 维护的量：xs = 排序去重后的原坐标表，编号 i（1-indexed）对应原坐标 xs[i-1]。
// 不变量：xs 严格递增，id(value(i)) = i，且 id 返回值落在 [1, size()]。
template <class T>
struct Compressor {
    vector<T> xs;

    void add(T x) { xs.push_back(x); } // 收集坐标；build 前必须加完全部会被访问的值。

    void build() {
        sort(xs.begin(), xs.end());    // 排序后去重，得到从小到大、互不相同的坐标表。
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
    }

    int id(T x) const {
        return int(lower_bound(xs.begin(), xs.end(), x) - xs.begin()) + 1; // 排名 +1 得 1..m 编号。
    }

    T value(int id) const {
        return xs[id - 1]; // 编号还原成原坐标。
    }

    int size() const {
        return (int)xs.size(); // 不同坐标个数 m，开数组开到 m+1。
    }
};
```

**最小完整示例（先抄这一段就能跑）：**

题目：坐标 `{10, 5, 5, 1000000000}` 离散化到 `1..3`，并演示编号与还原。

```cpp
Compressor<i64> cp;
for (i64 x : {10LL, 5LL, 5LL, 1000000000LL}) cp.add(x);
cp.build();
cout << cp.size() << '\n';                      // 3：去重后 3 个不同坐标
cout << cp.id(5) << ' ' << cp.id(10) << '\n';   // 1 2：排名从 1 开始
cout << cp.value(cp.id(1000000000LL)) << '\n';  // 1000000000：编号还原原坐标
```

- 样例：依次输出 `3`、`1 2`、`1000000000`。

**传参要求（照这个传不会错）：**

- `add(T x)`：`x` 是任意会被查询的坐标（`Compressor<int>` 或 `Compressor<i64>`）；`build()` 前加完全部，重复加没关系。
- `build()`：无参数；排序去重，之后才可用 `id`/`value`/`size`。
- `id(T x)`：`x` 必须已 `add` 过；返回 `int` 编号，落在 `1..m`（1-indexed）。
- `value(int id)`：`id` 取值 `1..size()`；返回原坐标 `T`。
- `size()`：返回 `int`，不同坐标个数 `m`；开数组开到 `m+1`。
#### 一维前缀和

> **赛时先看**
> - **题目信号**：数组不修改，多次问 `[l,r]` 的和。
> - **本质**：静态数组区间和查询。
> - **接法**：数组不修改且反复问区间和，就先 `pre = build_prefix(a)`，每次输出 `range_sum(pre,l,r)`。如果有修改，前缀和失效，改用树状数组或线段树。
> - **复杂度判定**：预处理 `O(n)`，查询 `O(1)`。
> - **维护的量**：`pre`（前缀和数组，`pre[i]` = `a[1..i]` 的和，1-indexed，长度 `n+1`）。
> - **警告**：建议 1-based，`sum(l,r)=pre[r]-pre[l-1]`。
> - **约定**：`a` 使用 1-indexed，`n = (int)a.size() - 1`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// a = {0, 1, 2, 3}（1-indexed，a[0] 占位），问区间和。
vector<i64> a = {0, 1, 2, 3};
auto pre = build_prefix(a);
cout << range_sum(pre, 1, 3) << '\n';   // 6（1+2+3）
cout << range_sum(pre, 2, 3) << '\n';   // 5（2+3）
```

- 样例：输出 `6` 与 `5`。

**传参要求（照这个传不会错）：**

- `build_prefix(a)`：入参 `a` 必须 1-indexed（`a[0]` 占位，元素从下标 1 开始），长度 `n+1`；返回 `pre`（`vector<i64>`，长度 `n+1`，`pre[i]` = `a[1..i]` 的和）。
- `range_sum(pre, l, r)`：`l/r` 为 1-indexed 闭区间，`1 <= l <= r <= n`；返回 `i64` 区间和。
- 有修改时前缀和失效，换树状数组或线段树。


**API / 入口函数（赛时只认这里列的名字）：**

- `range_sum(const vector<i64>& pre, int l, int r)` -> 查询闭区间和 返回 `i64`。

```cpp
vector<i64> build_prefix(const vector<i64>& a) {
    int n = (int)a.size() - 1; // 公式/约定：a is 1-based
    vector<i64> pre(n + 1, 0);
    for (int i = 1; i <= n; ++i) pre[i] = pre[i - 1] + a[i];
    return pre;
}

i64 range_sum(const vector<i64>& pre, int l, int r) {
    return pre[r] - pre[l - 1];
}
```
#### 一维差分

> **赛时先看**
> - **题目信号**：多次对 `[l,r]` 加 `x`，最后输出每个位置。
> - **本质**：离线区间加，最后一次性还原数组。
> - **接法**：所有区间加操作都提前知道、且只在最后问最终数组时，用差分最短。每次 `[l,r] += x` 调用 `add(l,r,x)`；全部操作结束后 `restore()` 得到最终数组。若中途穿插查询，不能用纯差分，翻 BIT/线段树。
> - **复杂度判定**：修改 `O(1)`，还原 `O(n)`。
> - **维护的量**：`diff`（差分数组，`diff[i]` = `a[i]-a[i-1]`，长度 `n+2`）与 `n`（元素个数）。
> - **警告**：`diff[r+1] -= x`，数组要开到 `n+2`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 长度为 5 的数组：区间 [2,4] 加 3，区间 [3,5] 减 1。
Difference1D d(5);
d.add(2, 4, 3);
d.add(3, 5, -1);
auto a = d.restore();
cout << a[2] << ' ' << a[3] << ' ' << a[4] << ' ' << a[5] << '\n';   // 3 2 2 2
```

- 样例：输出 `3 2 2 2`。

**传参要求（照这个传不会错）：**

- `Difference1D(n)`（构造）或 `init(n)`：`n` 为数组长度，下标 1..n，内部数组开 `n+2`。
- `add(l, r, x)`：对闭区间 `[l,r]` 加 `x`（`i64`），`1 <= l <= r <= n`，可多次调用。
- `restore()`：返回最终数组（`vector<i64>`，1-indexed，长度 `n+1`）。
- 中途穿插查询不能用纯差分，换树状数组/线段树。


**API / 入口函数（赛时只认这里列的名字）：**

- `add(int l, int r, i64 x)` -> 对区间 `[l,r]` 加 `x`
- `init(int n_)` -> 初始化/清空结构

```cpp
struct Difference1D {
    int n;
    vector<i64> diff;

    Difference1D(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        diff.assign(n + 2, 0);
    }

    void add(int l, int r, i64 x) {
        diff[l] += x;
        diff[r + 1] -= x;
    }

    vector<i64> restore() const {
        vector<i64> a(n + 1, 0);
        for (int i = 1; i <= n; ++i) a[i] = a[i - 1] + diff[i];
        return a;
    }
};
```
#### 二维前缀和

> **赛时先看**
> - **题目信号**：矩阵不修改，多次查询矩形区域和。
> - **本质**：静态矩阵子矩形和。
> - **接法**：把 `a` 填成 1-indexed 矩阵（第 0 行/列占位），先 `auto pre = build_prefix_2d(a);`，之后每次询问直接 `rect_sum(pre, x1, y1, x2, y2)`。
> - **复杂度判定**：预处理 `O(nm)`，查询 `O(1)`。
> - **维护的量**：`pre[i][j]`（左上角 `(1,1)` 到 `(i,j)` 的子矩形和）；输入 `a` 为 1-indexed，`a[0]` 行与 `a[i][0]` 列是占位。
> - **警告**：容斥公式四项别写反。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// a 为 3x3 矩阵（1-indexed，第 0 行/列是占位），多次查询子矩形和。
vector<vector<i64>> a = {
    {0, 0, 0, 0},
    {0, 1, 2, 3},
    {0, 4, 5, 6},
    {0, 7, 8, 9}
};
auto pre = build_prefix_2d(a);
cout << rect_sum(pre, 2, 2, 3, 3) << '\n'; // 5+6+8+9 = 28
```

- 样例：输出 `28`。

**传参要求（照这个传不会错）：**

- `a`：1-indexed 矩阵，`n = a.size()-1` 行、`m = a[1].size()-1` 列；`a[0]` 行与 `a[i][0]` 列是占位（值为 0）。
- `build_prefix_2d(a)`：返回 `(n+1)*(m+1)` 的 `pre`；`pre[0][*]`、`pre[*][0]` 恒为 0。
- `rect_sum(pre, x1, y1, x2, y2)`：闭矩形 `[x1..x2] × [y1..y2]` 的和；要求 `1 <= x1 <= x2 <= n`、`1 <= y1 <= y2 <= m`。


```cpp
// 维护的量：pre[i][j] = 左上角 (1,1) 到 (i,j) 的子矩形和，1-indexed。
// 不变量：pre[i][j] = pre[i-1][j] + pre[i][j-1] - pre[i-1][j-1] + a[i][j]。
vector<vector<i64>> build_prefix_2d(const vector<vector<i64>>& a) {
    int n = (int)a.size() - 1;
    int m = (int)a[1].size() - 1;
    vector<vector<i64>> pre(n + 1, vector<i64>(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            pre[i][j] = pre[i - 1][j] + pre[i][j - 1] - pre[i - 1][j - 1] + a[i][j]; // 容斥：上+左-左上角
        }
    }
    return pre;
}

// 维护的量：只读 pre，不新增结构。
// 不变量：矩形和 = 右下 - 左下 - 右上 + 左上（四角容斥，方向别写反）。
i64 rect_sum(const vector<vector<i64>>& pre, int x1, int y1, int x2, int y2) {
    return pre[x2][y2] - pre[x1 - 1][y2] - pre[x2][y1 - 1] + pre[x1 - 1][y1 - 1];
}
```
#### 二维差分

> **赛时先看**
> - **题目信号**：多次对矩形区域加 `x`，最后输出矩阵。
> - **本质**：离线子矩形加值，最后还原整个矩阵。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要自己直接改内部数组。
> - **复杂度判定**：每次修改 `O(1)`，还原 `O(nm)`。
> - **维护的量**：`n/m`（行列数）；`diff[i][j]`（二维差分数组，开 `(n+2)*(m+2)`，四角打标记）。
> - **警告**：四个角容斥，数组要开到 `n+2, m+2`。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// 4x4 矩阵初始全 0；两次矩形加后输出最终矩阵。
Difference2D d(4, 4);
d.add(1, 1, 2, 2, 3);   // (1,1)-(2,2) 每个格子加 3
d.add(2, 2, 4, 4, 1);   // (2,2)-(4,4) 每个格子加 1
auto a = d.restore();
cout << a[2][2] << '\n'; // 3+1 = 4
cout << a[1][1] << '\n'; // 只被第一次覆盖：3
```

- 样例：输出 `4` 与 `3`。

**传参要求（照这个传不会错）：**

- `Difference2D(n, m)`：构造；`n/m` = 行数/列数，内部自动开 `n+2, m+2`。
- `init(n_, m_)`：重置为 `n_` 行 `m_` 列并全部清零。
- `add(x1, y1, x2, y2, v)`：闭矩形 `[x1..x2] × [y1..y2]` 整体加 `v`；要求 `1 <= x1 <= x2 <= n`、`1 <= y1 <= y2 <= m`。
- `restore()`：返回 `(n+1)*(m+1)` 的最终矩阵 `a`（`a[i][j]` 即 `(i,j)` 的最终值），`a[0]` 行/列恒为 0。


**API / 入口函数（赛时只认这里列的名字）：**

- `init(int n_, int m_)` -> 初始化/清空结构
- `add(int x1, int y1, int x2, int y2, i64 v)` -> 对矩形 `(x1,y1)-(x2,y2)` 加 `v`

```cpp
// 维护的量：diff[i][j]（二维差分数组，比原矩阵多开一圈避免越界）。
// 不变量：restore() 时对 diff 做二维前缀和，得到的 a 就是所有 add 叠加后的最终矩阵。
struct Difference2D {
    int n, m;
    vector<vector<i64>> diff;

    Difference2D(int n = 0, int m = 0) { init(n, m); }

    void init(int n_, int m_) {
        n = n_;
        m = m_;
        diff.assign(n + 2, vector<i64>(m + 2, 0)); // 多开一圈，add 的 +1 角不会越界
    }

    void add(int x1, int y1, int x2, int y2, i64 v) {
        // 四角打标记：容斥之后，只有矩形内部的点在还原时收到 v
        diff[x1][y1] += v;
        diff[x2 + 1][y1] -= v;
        diff[x1][y2 + 1] -= v;
        diff[x2 + 1][y2 + 1] += v;
    }

    vector<vector<i64>> restore() const {
        vector<vector<i64>> a(n + 1, vector<i64>(m + 1, 0));
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                a[i][j] = diff[i][j] + a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1]; // 前缀和还原
            }
        }
        return a;
    }
};
```

### 02 枚举、贪心、搜索与构造

小数据、状态空间、精确覆盖、构造和贪心证明先翻这里；折半搜索也放在本章，方便和 DFS/BFS 一起定位。
#### 贪心证明速查：邻项交换与反悔堆

> **赛时先看**
> - **题目信号**：题目要求选择若干项最大/最小；每项有截止时间、收益、花费；排序后局部交换能证明更优。
> - **本质**：处理排序贪心、区间选择、任务安排、可反悔选择最大收益等题。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：通常排序 `O(n log n)`，反悔堆 `O(n log n)`。
> - **维护的量**：`pq`（已选任务收益的小根堆，堆顶是最该反悔掉的）；`sum`（当前已选任务总收益）；按截止时间排序后的 `jobs`。
> - **警告**：贪心要能证明。常用证明是邻项交换、包含关系、反悔替换、前缀最优。

**最小完整示例（先抄这一段就能跑）：**

题目：每个任务耗时 1，截止时间/收益为 `(1,20),(2,10),(2,15),(3,5)`，求截止前能完成的任务最大总收益。

```cpp
vector<pair<int, int>> jobs = {{1, 20}, {2, 10}, {2, 15}, {3, 5}};
i64 ans = schedule_with_deadline(jobs); // 任务数超过截止时间就反悔掉堆顶最小收益
cout << ans << "\n";
```

样例：`ans = 35`（选收益 20、10、15，放弃 5）。

**传参要求（照这个传不会错）：**

- `jobs`：`vector<pair<int,int>>`，每个元素 `{d, v}` 是截止时间 d 和收益 v；内部会先按 d 排序，传乱序没关系；d、v 都应在 int 范围内。
- 返回值：`i64`，能在各自截止时间前完成的任务最大总收益；收益全为正、任务做不完时，放弃的正好是堆里收益最小的几个。

**API / 入口函数（赛时只认这里列的名字）：**

- `schedule_with_deadline(vector<pair<int, int>> jobs)` -> 反悔堆模型：每个任务有截止时间 d 和收益 v，每个任务耗时 1，最多在 d 前完成。 选择总收益最大的一批任务。 返回 `i64`。

```cpp
// 反悔堆模型：每个任务有截止时间 d 和收益 v，每个任务耗时 1，最多在 d 前完成。
// 选择总收益最大的一批任务。
i64 schedule_with_deadline(vector<pair<int, int>> jobs) {
    sort(jobs.begin(), jobs.end()); // 按截止时间排序。
    priority_queue<int, vector<int>, greater<int>> pq;
    i64 sum = 0;
    for (auto [d, v] : jobs) {
        pq.push(v);
        sum += v;
        if ((int)pq.size() > d) {
            sum -= pq.top();
            pq.pop();
        }
    }
    return sum;
}
```
#### 分数背包：物品允许只取一部分

> **赛时先看**
> - **题目信号**：液体、粮食、矿石、可切割材料等，题面明确可以拆开或按比例获取；这时不是 0/1 背包，按单位重量价值贪心即可。
> - **本质**：容量有限，但每个物品可以取任意比例，求最大价值并恢复每个物品取了多少。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：排序 `O(n log n)`。
> - **维护的量**：`result.max_value`（已取物品总价值）；`result.picks`（每件取走的实际重量 `{id, amount}`）；`capacity`（剩余容量，循环里递减）。
> - **警告**：只有“允许分割”才能按价值密度贪心，完整物品不可拆时必须回到 DP。权重需为正；比较密度时用 `long double` 或交叉乘避免精度误判。

**最小完整示例（先抄这一段就能跑）：**

题目：容量 `50`，物品（重量, 价值, id）为 `(10,100,0),(30,120,1),(20,40,2)`，求最大总价值与每件取多少。

```cpp
FractionalKnapsackResult res =
    fractional_knapsack(50, {{10, 100, 0}, {30, 120, 1}, {20, 40, 2}});
cout << res.max_value << "\n"; // 最大总价值
for (auto p : res.picks) cout << p.id << " " << p.amount << "\n";
```

样例：`max_value = 240`；`picks = {(0,10),(1,30),(2,10)}`（第 3 件只取 10 重量）。

**传参要求（照这个传不会错）：**

- `capacity`：`long double`，背包总容量，非负即可。
- `items`：`vector<FractionalItem>`，每件 `{weight, value, id}`：`weight > 0`、`value >= 0`，`id` 是原编号（会原样出现在结果里，用来恢复方案）。
- 返回值：`FractionalKnapsackResult`，`max_value` 是最大总价值，`picks` 按贪心顺序装 `{id, amount}`，`amount` 是实际取走的重量。


```cpp
struct FractionalItem {
    long double weight, value;
    int id;
};

struct FractionalPick {
    int id;
    long double amount; // 取走的实际重量。
};

struct FractionalKnapsackResult {
    long double max_value = 0;
    vector<FractionalPick> picks;
};

FractionalKnapsackResult fractional_knapsack(
    long double capacity, vector<FractionalItem> items
) {
    sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return a.value / a.weight > b.value / b.weight;
    });
    FractionalKnapsackResult result;
    for (const auto& item : items) {
        if (capacity <= 0) break;
        long double amount = min(capacity, item.weight);
        result.max_value += amount / item.weight * item.value;
        result.picks.push_back({item.id, amount});
        capacity -= amount;
    }
    return result;
}
```

#### Huffman / 最优合并：每次合并最小两堆

> **赛时先看**
> - **题目信号**：反复从任意两堆中合并，代价是两堆和，最后合成一堆；题面没有“只能合并相邻两堆”的限制。
> - **本质**：有若干文件/果子/区间块，合并两个的代价等于两者当前大小之和，且合并后新块还会继续参与，求最小总代价；也等价于给定字符频率求 Huffman 前缀码的最小带权路径长度。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**优先调用下面 API 列出的入口**，其余 helper 默认不要从 `solve()` 直接调。
> - **复杂度判定**：`O(n log n)` 时间、`O(n)` 空间。
> - **警告**：只有"任意两堆可合并"才是 Huffman；要求"只能合并相邻"时翻相邻区间合并 DP；输入须非负，累计代价可能到 `i64`。
> - **维护的量**：`heap`（当前所有堆大小的小根堆，每次弹两个最小的）；`answer`（累计合并代价）。

**最小完整示例（先抄这一段就能跑）：**

题目：洛谷 P1090 合并果子，各堆数量 `[1,2,3,4]`，求最小总合并代价。

```cpp
vector<i64> weights = {1, 2, 3, 4};
i64 ans = huffman_optimal_merge_cost(weights); // 每次合并最小的两堆
cout << ans << "\n";
```

样例：`ans = 19`（1+2=3，3+3=6，4+6=10，总代价 3+6+10）。

**传参要求（照这个传不会错）：**

- `weights`：`const vector<i64>&`，各堆当前大小（字符频率），要求非负（内部有 `assert(weight >= 0)`）；允许为空或只有一个元素，此时返回 `0`。
- 返回值：`i64`，最小总合并代价；累计代价可能超出 int，读入时用 i64。

**API / 入口函数（赛时只认这里列的名字）：**

- `huffman_optimal_merge_cost(const vector<i64>& weights)` -> Huffman 编码 / 合并果子：每次合并当前最小的两堆。 返回 `i64`。

- 只要题目要求“相邻”合并，就不是 Huffman，应翻相邻区间合并/石子合并 DP。
- 输入非负；累计代价可能到 `long long`，先看数据范围。
- 一堆或零堆不需要合并，答案是 `0`。

```cpp
// Huffman 编码 / 合并果子：每次合并当前最小的两堆。
i64 huffman_optimal_merge_cost(const vector<i64>& weights) {
    priority_queue<i64, vector<i64>, greater<i64>> heap;
    for (i64 weight : weights) {
        assert(weight >= 0);
        heap.push(weight);
    }
    i64 answer = 0;
    while (heap.size() >= 2) {
        i64 merged = heap.top();
        heap.pop();
        merged += heap.top();
        heap.pop();
        answer += merged; // 确认题目数据使总和不超 i64。
        heap.push(merged);
    }
    return answer;
}
```

典题：洛谷 P1090《合并果子》。读入每堆数量到 `weights` 后，输出 `huffman_optimal_merge_cost(weights)`。例如 `[1,2,3,4]` 的最优过程是 `1+2=3, 3+3=6, 4+6=10`，总代价 `19`。
#### 定长滑动窗口：固定长度区间和/最大值

> **赛时先看**
> - **题目信号**：题面明确子数组/连续区间长度固定为 `k`；问连续 `k` 天/`k` 个位置的最大总和、最小总和、平均值或某个可增量维护的统计量。
> - **本质**：长度固定的窗口从左往右滑，每一步只“进一个出一个”：新窗口 = 旧窗口 + 进 - 出，`O(1)` 增量更新，不必每个窗口重新求和。
> - **复杂度判定**：`O(n)` 时间、`O(1)` 额外空间；n 到 1e6/1e7 都直接过。窗口长度不固定（至多/至少 k）翻“可变窗口”；窗口内还要查最值翻单调队列。
> - **维护的量**：`current`（当前窗口的和），`answer`（扫过的所有窗口结果的最值）。
> - **接法**：给 `n` 天收益，必须连续工作恰好 `k` 天，求最大收益。将收益放入 0-indexed `vector<i64> a`，直接调用 `maximum_fixed_window_sum(a,k)`；若要求最小收益，把 `max` 改成 `min` 即可（答案初值已经是第一个窗口）。
> - **警告**：`len` 必须在 `[1,n]`；窗口右移时先减掉离开的 `a[i-len]`，再加进入的 `a[i]`；元素与答案都可能需要 `i64`。


**抄板清单（照着做就行）：**
1. 抄哪段：整个 `maximum_fixed_window_sum` 函数。
2. 构造：把数组放进 0-indexed `vector<i64> a`，长度 `len = k`。
3. 调用：`i64 ans = maximum_fixed_window_sum(a, k);`
4. 取结果：`ans` 就是所有固定长度窗口和的最大值，直接输出。

**改造点（按题目改这几处）：**
- 求最小：把 `max(answer, current)` 改成 `min(answer, current)`，其余不动。
- 维护的量不是和：把 `current += a[r] - a[r-len]` 换成题目要的增量更新式；要窗口内最值翻单调队列。
- 平均值：`ans / len` 即可；元素含负数、浮点都没问题，模板无假设。
- 多个不同长度：套循环分别调用，或翻“一维前缀和” `range_sum`。

```cpp
// 维护的量：current = 当前长度为 len 的窗口和，answer = 所有窗口和的最大值。
// 不变量：窗口右移一格时，current 先减掉离开的 a[r-len]，再加进进入的 a[r]。
i64 maximum_fixed_window_sum(const vector<i64>& a, int len) {
    int n = (int)a.size();
    assert(1 <= len && len <= n);
    i64 current = 0;
    for (int i = 0; i < len; ++i) current += a[i]; // 先求第一个窗口 [0, len) 的和。
    i64 answer = current;
    for (int r = len; r < n; ++r) {
        current += a[r] - a[r - len]; // 窗口右移：进 a[r]，出 a[r-len]，O(1) 更新。
        answer = max(answer, current);
    }
    return answer;
}
```

**最小完整示例（先抄这一段就能跑）：**

题目：`a = {1, 3, -2, 5, 2}`，必须连续工作恰好 `k=3` 天，求最大收益。

```cpp
vector<i64> a = {1, 3, -2, 5, 2};
i64 ans = maximum_fixed_window_sum(a, 3); // 窗口和依次为 2、6、5
cout << ans << '\n';                      // 6：最大窗口和
```

- 样例：输出 `6`。

**传参要求（照这个传不会错）：**

- `a`：`const vector<i64>&`，0-indexed，元素可为负；`n >= 1`。
- `len`：`int`，窗口长度，必须满足 `1 <= len <= n`（不满足会触发 `assert`）。
- 返回值：`i64`，所有长度恰为 `len` 的连续子数组和的最大值。

典题模型：给 `n` 天收益，必须连续工作恰好 `k` 天，求最大收益。将收益放入 0-indexed `vector<i64> a`，直接调用 `maximum_fixed_window_sum(a,k)`；若要求最小收益，把 `max` 改成 `min` 即可（答案初值已经是第一个窗口）。
#### 可变窗口：至多/恰好 K 种不同元素

> **赛时先看**
> - **题目信号**：连续子数组、不同值种类数、至多/恰好 `K` 个颜色/数字/字符；窗口右端点从左到右扫，约束在左端点右移后不会更难满足。
> - **本质**：统计不同元素数不超过 `K` 的子数组数量，或利用“恰好 K = 至多 K - 至多 K-1”统计恰好 `K` 种不同元素的子数组数量。
> - **接法**：`n <= 2e5`，求元素互不重复的子数组数。这不能直接写成一个固定 `K`；把窗口条件改成某值频率不超过 `1` 即可，或直接用 `count_subarrays_at_most_k_distinct(a,k)` 处理“至多/恰好 K 种”的原型。CSES `Distinct Values Subarrays` 是典型的窗口计数题。
> - **复杂度判定**：哈希表实现期望 `O(n)`，每个位置最多进窗口、出窗口各一次；空间 `O（不同值数）`。
> - **维护的量**：`frequency`（窗口内每个值出现次数）；`distinct`（当前窗口不同值个数）；`left/right`（窗口两端）；`answer`（累计合法子数组数）。
> - **警告**：`count_subarrays_at_most_k_distinct(a,-1)` 必须返回 `0`——“恰好 K 种”的差分要用到 `k-1=-1`，这样 `K=0` 时也安全；每个右端点合法左端点是连续的一段，贡献为 `r-l+1`；不要把它直接套到“区间和不超过 S 且允许负数”的题，负数会破坏窗口单调性。

**最小完整示例（先抄这一段就能跑）：**

题目：`a = [1,2,1,3]`，求不同元素至多 2 种、以及恰好 2 种的子数组个数。

```cpp
vector<i64> a = {1, 2, 1, 3};
i64 at_most = count_subarrays_at_most_k_distinct(a, 2);
i64 exactly = count_subarrays_exactly_k_distinct(a, 2); // 恰好 = 至多2 - 至多1
cout << at_most << " " << exactly << "\n";
```

样例：`at_most = 8`，`exactly = 4`。

**传参要求（照这个传不会错）：**

- `a`：`const vector<i64>&`，0-indexed 数组，元素就是“种类”的编号，任意整数都行。
- `k`：`int`，允许的不同种类数上限；`k < 0` 时返回 `0`，恰好 K 种的内部差分会传 `k-1 = -1`，这是安全的。
- 返回值：`i64`，满足条件的连续子数组个数。


```cpp
i64 count_subarrays_at_most_k_distinct(const vector<i64>& a, int k) {
    if (k < 0) return 0;
    unordered_map<i64, int> frequency;
    frequency.reserve(a.size() * 2 + 1);
    frequency.max_load_factor(0.7F);

    int left = 0, distinct = 0;
    i64 answer = 0;
    for (int right = 0; right < (int)a.size(); ++right) {
        if (++frequency[a[right]] == 1) ++distinct;
        while (distinct > k) {
            if (--frequency[a[left]] == 0) --distinct;
            ++left;
        }
        answer += right - left + 1;
    }
    return answer;
}

i64 count_subarrays_exactly_k_distinct(const vector<i64>& a, int k) {
    return count_subarrays_at_most_k_distinct(a, k)
         - count_subarrays_at_most_k_distinct(a, k - 1);
}
```

典题模型：`n <= 2e5`，求元素互不重复的子数组数。这不能直接写成一个固定 `K`；把窗口条件改成某值频率不超过 `1` 即可，或直接用 `count_subarrays_at_most_k_distinct(a,k)` 处理“至多/恰好 K 种”的原型。CSES `Distinct Values Subarrays` 是典型的窗口计数题。
#### 正数数组双指针：最大长度且区间和不超过 S

> **赛时先看**
> - **题目信号**：元素保证非负/正；区间和与一个阈值比较；右扩窗口只会让和不减，左缩窗口只会让和不增。
> - **本质**：数组元素非负时，求和不超过 `limit` 的最长连续子数组；也可改为统计和不超过 `limit` 的所有子数组数。
> - **接法**：所有货物重量非负，问最长连续装载段总重量不超过 `S`。直接输出 `longest_nonnegative_subarray_sum_at_most(weight,S)`；若问合格连续段数量，改调 `count_nonnegative_subarrays_sum_at_most`。
> - **复杂度判定**：`O(n)` 时间，`O(1)` 额外空间。
> - **维护的量**：`sum`（当前窗口 `[left,right]` 的元素和）；`left/right`（窗口两端，right 固定时收缩 left 直到合法）；`answer`（最长合法长度或合法子数组个数）。
> - **警告**：有负数时不能直接用本模板；`limit < 0` 时没有非空合法段；“最长”在每次修复合法窗口后更新，若改成“计数”则每个右端点贡献 `right-left+1`。

**最小完整示例（先抄这一段就能跑）：**

题目：货物重量 `[3,1,2,1]` 均非负，求总重量不超过 4 的最长连续段长度，以及合格连续段数量。

```cpp
vector<i64> weight = {3, 1, 2, 1};
int len = longest_nonnegative_subarray_sum_at_most(weight, 4);
i64 cnt = count_nonnegative_subarrays_sum_at_most(weight, 4);
cout << len << " " << cnt << "\n";
```

样例：`len = 3`（取 `[1,2,1]`），`cnt = 6`。

**传参要求（照这个传不会错）：**

- `a`：`const vector<i64>&`，0-indexed 数组，元素必须非负（内部有 `assert(a[right] >= 0)`）。
- `limit`：`i64`，区间和的上界；`limit < 0` 时没有合法非空段，返回 `0`。
- 返回值：最长版返回 `int`（最大合法长度），计数版返回 `i64`（合法子数组个数）。


```cpp
int longest_nonnegative_subarray_sum_at_most(const vector<i64>& a, i64 limit) {
    if (limit < 0) return 0;
    int left = 0, answer = 0;
    i64 sum = 0;
    for (int right = 0; right < (int)a.size(); ++right) {
        assert(a[right] >= 0);
        sum += a[right];
        while (sum > limit) sum -= a[left++];
        answer = max(answer, right - left + 1);
    }
    return answer;
}

i64 count_nonnegative_subarrays_sum_at_most(const vector<i64>& a, i64 limit) {
    if (limit < 0) return 0;
    int left = 0;
    i64 sum = 0, answer = 0;
    for (int right = 0; right < (int)a.size(); ++right) {
        assert(a[right] >= 0);
        sum += a[right];
        while (sum > limit) sum -= a[left++];
        answer += right - left + 1;
    }
    return answer;
}
```

典题模型：所有货物重量非负，问最长连续装载段总重量不超过 `S`。直接输出 `longest_nonnegative_subarray_sum_at_most(weight,S)`；若问合格连续段数量，改调 `count_nonnegative_subarrays_sum_at_most`。
#### 排序双指针：数对和不超过 S 的数量

> **赛时先看**
> - **题目信号**：任意两个数、和与阈值比较、只需要数量或是否存在；数组可以排序，原下标通常不重要。
> - **本质**：统计无序数对 `(i,j), i<j` 满足 `a[i] + a[j] <= limit` 的数量；可作为“两数和第 k 小”或三数和计数的基础子过程。
> - **接法**：求第 `k` 小的两数和。对答案 `mid` 二分，判定 `count_pairs_with_sum_at_most(a,mid) >= k`；若只问不超过 `S` 的数对数，直接输出本函数结果。
> - **复杂度判定**：排序 `O(n log n)`，双指针扫描 `O(n)`，空间取决于是否复制数组。
> - **维护的量**：`left/right`（排序后从两端向中间收的指针）；`answer`（满足 `a[i]+a[j] <= limit` 的无序数对个数）。
> - **警告**：数对数量用 `i64`；两数相加可能溢出 `i64`，比较时用 `i128`；当 `a[left]+a[right] <= limit` 时，固定 `left` 的所有右端点 `left+1..right` 都合法，共有 `right-left` 对。

**最小完整示例（先抄这一段就能跑）：**

题目：`a = [1,2,3,4,5]`，求两数和不超过 6 的无序数对 `(i,j), i<j` 个数。

```cpp
vector<i64> a = {1, 2, 3, 4, 5};
i64 cnt = count_pairs_with_sum_at_most(a, 6); // 内部会先排序，原下标不重要
cout << cnt << "\n";
```

样例：`cnt = 6`（(1,2),(1,3),(1,4),(1,5),(2,3),(2,4)）。

**传参要求（照这个传不会错）：**

- `a`：按值传入 `vector<i64>`（会先 `sort`，原数组顺序被破坏），0-indexed。
- `limit`：`i64`，两数和的上界；比较时内部用 `i128`，不用担心相加溢出。
- 返回值：`i64`，满足 `a[i]+a[j] <= limit` 的无序数对个数。


```cpp
i64 count_pairs_with_sum_at_most(vector<i64> a, i64 limit) {
    sort(a.begin(), a.end());
    int left = 0, right = (int)a.size() - 1;
    i64 answer = 0;
    while (left < right) {
        if ((i128)a[left] + a[right] <= limit) {
            answer += right - left;
            ++left;
        } else {
            --right;
        }
    }
    return answer;
}
```

典题模型：求第 `k` 小的两数和。对答案 `mid` 二分，判定 `count_pairs_with_sum_at_most(a,mid) >= k`；若只问不超过 `S` 的数对数，直接输出本函数结果。
#### 离线扫描 + 树状数组：区间中值落在 [x,y] 的个数

> **赛时先看**
> - **题目信号**：数组没有修改；查询同时限制位置区间和值域区间；本质是二维矩形数点，`n,q` 到 `2e5` 级别，不能逐查询扫区间。
> - **本质**：静态数组 `a`，多次查询索引区间 `[l,r]` 中值也落在 `[low,high]` 的元素数量。将每个 `a[i]` 看成平面点 `(i,a[i])`，按值扫描，用树状数组维护已经激活的位置。
> - **接法**：给静态数组与 `q` 个询问 `(l,r,x,y)`，求 `[l,r]` 中落在值域 `[x,y]` 的元素数。构造 0-indexed `ValueRangeQuery{l,r,x,y}`，批量调用 `count_values_in_index_and_value_ranges(a,queries)`；二维点带权或需要在线修改时，应改用 CDQ、树套树或动态 K-D Tree。
> - **复杂度判定**：排序和所有树状数组操作共 `O((n+q) log n)`，空间 `O(n+q)`。
> - **维护的量**：`point`（按值排序的平面点 `(a[i], i)`）；`event`（每个查询拆成 `< low` 与 `<= high` 两个事件）；`fenwick`（已激活位置的前缀和，维护“当前值阈值以下已激活的位置”）。
> - **警告**：这里下标是 0-indexed 闭区间；答案是 `count(value <= high) - count(value < low)`；不要用 `low-1` 当阈值，`low=LLONG_MIN` 时会溢出；相同阈值时先处理严格 `<` 事件，再处理 `<=` 事件。
> - **约定**：`l,r` 是 0-indexed 闭区间；`range_sum(l,r)` 查询闭区间 `[l,r]`。

**最小完整示例（先抄这一段就能跑）：**

题目：`a = [3,1,4,2]`，问下标 `[1,3]` 中值落在 `[2,4]` 的元素个数。

```cpp
vector<i64> a = {3, 1, 4, 2};
vector<ValueRangeQuery> qs = {{1, 3, 2, 4}}; // l,r 是 0-indexed 闭区间
vector<i64> ans = count_values_in_index_and_value_ranges(a, qs);
cout << ans[0] << "\n";
```

样例：`ans[0] = 2`（下标 1..3 的值是 1、4、2，其中 4、2 落在 [2,4]）。

**传参要求（照这个传不会错）：**

- `a`：`const vector<i64>&`，0-indexed 静态数组，全程无修改。
- `queries`：`const vector<ValueRangeQuery>&`，每个查询 `{l, r, low, high}`：`l/r` 是 0-indexed 闭区间下标，需满足 `0 <= l <= r < n`；`low/high` 是值域，`low > high` 时该查询自动跳过。
- 返回值：`vector<i64>`，与 `queries` 同序，第 i 个元素是第 i 个查询的答案。

**API / 入口函数（赛时只认这里列的名字）：**

- `add(int index, int delta)` -> 在 `index` 处加 `delta`
- `range_sum(int l, int r)` -> 查询闭区间和 返回 `int`。
**改板时先认这几个量：**

- `l/r`：查询区间，0-indexed 闭区间。
- `bit`：Fenwick 内部树状数组。

```cpp
struct ValueRangeQuery {
    int l, r;           // 0-indexed 闭区间。
    i64 low, high;
};

struct OfflineFenwick {
    int n;
    vector<int> bit;

    explicit OfflineFenwick(int n_) : n(n_), bit(n_ + 1, 0) {}

    void add(int index, int delta) {
        for (++index; index <= n; index += index & -index) bit[index] += delta;
    }

    int prefix_sum(int index) const { // 查询半开前缀区间 [0,index) 的和。
        int result = 0;
        for (; index > 0; index -= index & -index) result += bit[index];
        return result;
    }

    int range_sum(int l, int r) const { // 查询闭区间 [l,r]。
        return prefix_sum(r + 1) - prefix_sum(l);
    }
};

vector<i64> count_values_in_index_and_value_ranges(
    const vector<i64>& a, const vector<ValueRangeQuery>& queries
) {
    struct Event {
        i64 threshold;
        bool include_equal; // false 表示统计 < threshold，true 表示统计 <= threshold。
        int l, r, id, sign;
    };

    int n = (int)a.size();
    vector<pair<i64, int>> point;
    point.reserve(n);
    for (int i = 0; i < n; ++i) point.push_back({a[i], i});
    sort(point.begin(), point.end());

    vector<Event> event;
    event.reserve(queries.size() * 2);
    for (int id = 0; id < (int)queries.size(); ++id) {
        auto [l, r, low, high] = queries[id];
        assert(0 <= l && l <= r && r < n);
        if (low > high) continue;
        event.push_back({high, true, l, r, id, +1});
        event.push_back({low, false, l, r, id, -1});
    }
    sort(event.begin(), event.end(), [](const Event& x, const Event& y) {
        if (x.threshold != y.threshold) return x.threshold < y.threshold;
        return x.include_equal < y.include_equal;
    });

    OfflineFenwick fenwick(n);
    vector<i64> answer(queries.size(), 0);
    int ptr = 0;
    for (const Event& e : event) {
        while (ptr < n && (point[ptr].first < e.threshold
            || (e.include_equal && point[ptr].first == e.threshold))) {
            fenwick.add(point[ptr].second, 1);
            ++ptr;
        }
        answer[e.id] += (i64)e.sign * fenwick.range_sum(e.l, e.r);
    }
    return answer;
}
```

典题模型：给静态数组与 `q` 个询问 `(l,r,x,y)`，求 `[l,r]` 中落在值域 `[x,y]` 的元素数。构造 0-indexed `ValueRangeQuery{l,r,x,y}`，批量调用 `count_values_in_index_and_value_ranges(a,queries)`；二维点带权或需要在线修改时，应改用 CDQ、树套树或动态 K-D Tree。
#### DFS / 回溯骨架

> **赛时先看**
> - **题目信号**：`n <= 20` 左右；要求输出方案；状态空间可剪枝。
> - **本质**：排列组合枚举、棋盘搜索、小数据暴力、构造方案。
> - **接法**：全排列、排列型构造、`n <= 10` 左右的暴力验证。先调用 `enumerate_permutations(n)`；若只需要最优答案，把 `answers.push_back(path)` 换成更新答案即可。N 皇后、数独等题将“当前状态是否合法”的剪枝放在 `if (used[x]) continue;` 后面，并保证“压入、递归、弹出”三步完整（压入和弹出成对出现）。
> - **复杂度判定**：指数级，取决于分支数与剪枝。
> - **维护的量**：`path`（当前已选排列前缀）；`used`（`used[x]=1` 表示 x 已用，1-indexed）；`answers`（收集到的全部排列）。
> - **警告**：进入递归前修改状态，返回后必须恢复；剪枝不能剪掉最优解。

**最小完整示例（先抄这一段就能跑）：**

题目：枚举 `1..3` 的全部排列（n 小、要输出方案或暴力验证时用）。

```cpp
vector<vector<int>> all = enumerate_permutations(3);
for (auto& p : all) { /* 每个 p 是一个长度为 3 的排列 */ }
cout << all.size() << " " << all[0][0] << "\n"; // 排列个数与首个排列首元素
```

样例：`all.size() = 6`，首排列为 `{1,2,3}`。

**传参要求（照这个传不会错）：**

- `n`：`int`，排列元素个数，元素值域 `1..n`；结果有 `n!` 个，`n <= 10` 左右用，再大内存会爆。
- 返回值：`vector<vector<int>>`，第 i 个元素是一个长度为 n 的排列；下标 `0..n-1`，值域 `1..n`。

**API / 入口函数（赛时只认这里列的名字）：**

- `enumerate_permutations(int n)` -> 完整的“枚举 1..n 的全部排列”骨架。 需要加约束时，在循环里、压入 path 之前加入 continue 剪枝即可。 返回 `vector<vector<int>>`。

```cpp
// 完整的“枚举 1..n 的全部排列”骨架。
// 需要加约束时，在循环里、压入 path 之前加入 continue 剪枝即可。
vector<vector<int>> enumerate_permutations(int n) {
    vector<vector<int>> answers;
    vector<int> path, used(n + 1);
    auto dfs = [&](auto&& self) -> void {
        if ((int)path.size() == n) {
            answers.push_back(path);
            return;
        }
        for (int x = 1; x <= n; ++x) {
            if (used[x]) continue;
            used[x] = 1;
            path.push_back(x);
            self(self);
            path.pop_back();
            used[x] = 0;
        }
    };
    dfs(dfs);
    return answers;
}
```

典题模型：全排列、排列型构造、`n <= 10` 左右的暴力验证。先调用 `enumerate_permutations(n)`；若只需要最优答案，把 `answers.push_back(path)` 换成更新答案即可。N 皇后、数独等题将“当前状态是否合法”的剪枝放在 `if (used[x]) continue;` 后面，并保证“压入、递归、弹出”三步完整（压入和弹出成对出现）。
#### 双向 BFS

> **赛时先看**
> - **题目信号**：状态数很大，但最短距离不深；普通 BFS 从一侧会爆。
> - **本质**：无权图最短路、状态空间搜索，起点和终点都已知。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：大约从 `O(b^d)` 降到 `O(b^(d/2))`，`b` 为分支数。
> - **维护的量**：`da/db`（起点侧/终点侧到各状态的最短距离）；`qa/qb`（两侧待扩展队列，每次扩展较短的队）；相遇时 `d1[v]+d2[v]` 就是答案。
> - **警告**：每次扩展状态少的一边；状态哈希要统一；相遇时返回两侧距离和。

**最小完整示例（先抄这一段就能跑）：**

题目：从 0 出发，每次 `x -> x+1` 或 `x -> 2x`，求到 5 的最少步数（State 用 int）。

```cpp
auto get_next = [](int x) { return vector<int>{x + 1, x * 2}; };
int ans = bidirectional_bfs(0, 5, get_next); // 两端交替扩展，相遇即最短路
cout << ans << "\n";
```

样例：`ans = 3`（0 -> 2 -> 4 -> 5）。

**传参要求（照这个传不会错）：**

- `s` / `t`：起/终点状态，模板参数 `State` 需支持 `==` 并能做 `unordered_map` 的 key（int、string、vector<int> 等）；`s == t` 时直接返回 `0`。
- `get_next`：可调用对象，输入一个状态返回 `vector<State>`（所有下一状态）；内部用 `d1/d2` 判重，允许自环/重边。
- 返回值：`int`，起点到终点的最短步数；不可达返回 `-1`。


```cpp
template <class State, class Next>
int bidirectional_bfs(State s, State t, Next get_next) {
    if (s == t) return 0;
    unordered_map<State, int> da, db;
    queue<State> qa, qb;
    da[s] = 0; db[t] = 0;
    qa.push(s); qb.push(t);
    auto expand = [&](queue<State>& q, unordered_map<State, int>& d1,
                      unordered_map<State, int>& d2) -> int {
        int sz = (int)q.size();
        while (sz--) {
            State u = q.front();
            q.pop();
            for (State v : get_next(u)) {
                if (d1.count(v)) continue;
                d1[v] = d1[u] + 1;
                if (d2.count(v)) return d1[v] + d2[v];
                q.push(v);
            }
        }
        return -1;
    };
    while (!qa.empty() && !qb.empty()) {
        int ans;
        if (qa.size() <= qb.size()) ans = expand(qa, da, db);
        else ans = expand(qb, db, da);
        if (ans != -1) return ans;
    }
    return -1;
}
```
#### A* 最短路：第 k 短路骨架

> **赛时先看**
> - **题目信号**：题面出现第 k 短路；边权非负；需要按路径长度从小到大弹出状态。
> - **本质**：求有向图从 `s` 到 `t` 的第 `k` 短路，或带启发函数的最短路搜索。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：和输出路径数量相关，常见为 `O(k m log(km))` 量级。
> - **维护的量**：`h`（反向 Dijkstra 得到的各点到 t 的最短路，作启发值）；`cnt`（每个点被弹出的次数，第 k 次到 t 即答案）；`pq`（按 `f = g + h[u]` 升序的优先队列）。
> - **警告**：启发函数必须不超过真实剩余距离；若 `s == t`，空路径算一条最短路径，通常要把 `k` 加一（`k++`）再搜。

**最小完整示例（先抄这一段就能跑）：**

题目：3 点有向图 `1->2(1), 2->3(1), 1->3(4)`，求 1 到 3 的第 1、2 短路。

```cpp
int n = 3;
vector<vector<Edge>> g(n + 1), rg(n + 1); // 1-indexed
g[1] = {{2, 1}, {3, 4}}; g[2] = {{3, 1}};
rg[2] = {{1, 1}}; rg[3] = {{2, 1}, {1, 4}}; // rg 是 g 的反图
cout << kth_shortest_path(n, g, rg, 1, 3, 1) << "\n";
cout << kth_shortest_path(n, g, rg, 1, 3, 2) << "\n";
```

样例：第 1 短路 `2`，第 2 短路 `4`。

**传参要求（照这个传不会错）：**

- `n`：点数，编号 `1..n`，邻接表要开 `n+1` 大小。
- `g` / `rg`：正向/反向邻接表 `vector<vector<Edge>>`，`Edge{to, w}` 边权非负；`rg` 是 `g` 每条边反向后的图，只用于求启发值。
- `s` / `t`：起点/终点，`1 <= s,t <= n`；`s == t` 时函数内部自动把 `k` 加 1（空路径算一条最短路径）。
- `k`：要第几条短路，`k >= 1`。
- 返回值：`i64`，第 k 短路长度；路径不足 k 条返回 `-1`。

**改板时先认这几个量：**

- `g`：正向邻接表（`rg` 为反向图，用来从终点跑 Dijkstra 求启发值）。
- `cur`：刚从优先队列弹出的状态。

```cpp
struct Edge { int to; i64 w; };

vector<i64> dijkstra_rev(int n, const vector<vector<Edge>>& rg, int t) {
    const i64 INF = (1LL << 62);
    vector<i64> h(n + 1, INF);
    priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<>> pq;
    h[t] = 0;
    pq.push({0, t});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != h[u]) continue;
        for (auto e : rg[u]) {
            if (h[e.to] > d + e.w) {
                h[e.to] = d + e.w;
                pq.push({h[e.to], e.to});
            }
        }
    }
    return h;
}

i64 kth_shortest_path(int n, const vector<vector<Edge>>& g,
                            const vector<vector<Edge>>& rg, int s, int t, int k) {
    const i64 INF = (1LL << 62);
    vector<i64> h = dijkstra_rev(n, rg, t);
    if (h[s] == INF) return -1;
    if (s == t) k++;
    struct Node {
        i64 f, g;
        int u;
        bool operator<(const Node& other) const { return f > other.f; }
    };
    priority_queue<Node> pq;
    vector<int> cnt(n + 1, 0);
    pq.push({h[s], 0, s});
    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();
        int u = cur.u;
        cnt[u]++;
        if (u == t && cnt[u] == k) return cur.g;
        if (cnt[u] > k) continue;
        for (auto e : g[u]) {
            if (h[e.to] == INF) continue;
            i64 ng = cur.g + e.w;
            pq.push({ng + h[e.to], ng, e.to});
        }
    }
    return -1;
}
```
#### IDA* 迭代加深搜索

> **赛时先看**
> - **题目信号**：需要最少步数；普通 BFS 内存爆；可以设计乐观估价函数。
> - **本质**：状态空间很深但答案步数较小，常见于拼图、魔板、棋盘最少步数。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：指数级，但剪枝强时可过。
> - **维护的量**：`limit`（当前深度上限，逐轮增大）；`on_path`（当前递归路径上的状态集合，防绕圈）；`next_limit`（本轮最小的超限 f 值，作为下一轮 limit）。
> - **警告**：估价函数 `h()` 不能高估真实剩余步数；每层深度上限逐步增加。

**最小完整示例（先抄这一段就能跑）：**

题目：数字游戏，从 0 出发每次 `x -> x+1` 或 `x -> x+2`，求到 5 的最少步数（State 用 int）。

```cpp
struct Hash { size_t operator()(int x) const { return (size_t)x; } };
auto heuristic = [](int x) { return (5 - x + 1) / 2; }; // 可采纳下界：每步最多 +2
auto is_goal = [](int x) { return x == 5; };
auto get_next = [](int x) { return vector<int>{x + 1, x + 2}; };
int steps = ida_star(0, Hash(), heuristic, is_goal, get_next);
cout << steps << "\n";
```

样例：`steps = 3`（0 -> 2 -> 4 -> 5）。

**传参要求（照这个传不会错）：**

- `start`：初始状态，模板参数 `State` 需支持 `==`（供 `unordered_set` 判重）。
- `hash`：`State` 的哈希函数对象，如 `struct Hash { size_t operator()(const State&) const; }`。
- `heuristic`：`State -> int`，乐观估价，必须满足 `h(x) <= 真实剩余步数`，高估会剪掉最优解。
- `is_goal`：`State -> bool`，判断是否到达目标状态。
- `get_next`：`State -> vector<State>`，返回所有合法下一状态。
- 返回值：`int`，从 `start` 到目标的最少步数（找到答案时的 limit）；无解返回 `-1`。


```cpp
// State 需要可比较相等，并由 Hash 提供 unordered_set 所需的哈希。
// heuristic(s) 必须是到目标的可采纳下界；get_next(s) 返回所有合法下一状态。
template <class State, class Hash, class Heuristic, class IsGoal, class Next>
int ida_star(State start, Hash hash, Heuristic heuristic, IsGoal is_goal, Next get_next) {
    constexpr int FOUND = -1;
    unordered_set<State, Hash> on_path(0, hash);
    on_path.insert(start);

    auto dfs = [&](auto&& self, const State& state, int g, int limit) -> int {
        int f = g + heuristic(state);
        if (f > limit) return f;
        if (is_goal(state)) return FOUND;
        int next_limit = INT_MAX;
        for (const State& next : get_next(state)) {
            if (!on_path.insert(next).second) continue; // 防止当前搜索路径绕圈。
            int result = self(self, next, g + 1, limit);
            on_path.erase(next);
            if (result == FOUND) return FOUND;
            next_limit = min(next_limit, result);
        }
        return next_limit;
    };

    int limit = heuristic(start);
    while (true) {
        int result = dfs(dfs, start, 0, limit);
        if (result == FOUND) return limit;
        if (result == INT_MAX) return -1;
        limit = result;
    }
}
```
#### DLX 精确覆盖

> **赛时先看**
> - **题目信号**：题面能转成 0/1 矩阵，每行表示一个选择，每列表示一个约束，每个约束恰好满足一次。
> - **本质**：精确覆盖问题，例如数独、重复覆盖、把若干集合选出来使每个元素恰好覆盖一次。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：指数级，实际依赖约束稀疏程度和列选择剪枝。
> - **维护的量**：`sz[c]`（第 c 列含 1 的个数，用于选最少列）；`L/R/U/D`（十字链表左右上下指针）；`row/col`（每个节点所在行列）；`answer`（当前已选行号，回溯时 pop）。
> - **警告**：列头、左右上下指针初始化；每次选含 1 最少的列；恢复顺序与删除相反。

**最小完整示例（先抄这一段就能跑）：**

题目：3 列 0/1 矩阵，行 1 覆盖 `{1,2}`、行 2 覆盖 `{2,3}`、行 3 覆盖 `{1,3}`，每列恰好被选中行覆盖一次，求选哪些行。

```cpp
DLX dlx(10, 3);             // max_nodes 给大一点，列数 3
dlx.add_row(1, {1, 2});     // 第 1 行覆盖列 1、2
dlx.add_row(2, {2, 3});
dlx.add_row(3, {1, 3});
bool ok = dlx.dance();      // 是否有精确覆盖
if (ok) for (int r : dlx.answer) cout << r << " ";
```

样例：`ok = true`，`answer = {1, 2}`（行 1 + 行 2 恰好覆盖全部 3 列）。

**传参要求（照这个传不会错）：**

- `init(max_nodes, cols)`：`max_nodes` 是节点总数上界（行数 × 每行列数 + cols，给大点没事），`cols` 是列数（约束数）；列编号 `1..cols`，`0` 是表头。
- `add_row(r, cols)`：`r` 是行号（任意正整数，会原样存进 `answer`）；`cols` 是该行覆盖的列编号集合，同一行内不能有重复列。
- `dance()`：求解，返回 `bool` 是否有解；有解时 `answer` 里按选择顺序存行号（这里示例是 `{1,2}`，选择顺序可能不同）。
- `max_nodes` 给太小会越界，不确定时按 `行数 * 列数 + 列数 + 5` 给。

**API / 入口函数（赛时只认这里列的名字）：**

- `init(int max_nodes, int cols)` -> 初始化/清空结构

```cpp
struct DLX {
    int n_col, idx;
    vector<int> L, R, U, D, row, col, sz;
    vector<int> answer;

    DLX(int max_nodes = 0, int cols = 0) { init(max_nodes, cols); }

    void init(int max_nodes, int cols) {
        n_col = cols;
        L.assign(max_nodes + cols + 5, 0);
        R.assign(max_nodes + cols + 5, 0);
        U.assign(max_nodes + cols + 5, 0);
        D.assign(max_nodes + cols + 5, 0);
        row.assign(max_nodes + cols + 5, 0);
        col.assign(max_nodes + cols + 5, 0);
        sz.assign(cols + 1, 0);
        for (int i = 0; i <= cols; i++) {
            L[i] = i - 1; R[i] = i + 1;
            U[i] = D[i] = i;
        }
        L[0] = cols; R[cols] = 0;
        idx = cols;
        answer.clear();
    }

    void add_row(int r, const vector<int>& cols) {
        int first = 0;
        for (int c : cols) {
            int x = ++idx;
            row[x] = r; col[x] = c; sz[c]++;
            U[x] = U[c]; D[x] = c;
            D[U[c]] = x; U[c] = x;
            if (!first) {
                first = x;
                L[x] = R[x] = x;
            } else {
                L[x] = L[first]; R[x] = first;
                R[L[first]] = x; L[first] = x;
            }
        }
    }

    void remove_col(int c) {
        L[R[c]] = L[c]; R[L[c]] = R[c];
        for (int i = D[c]; i != c; i = D[i]) {
            for (int j = R[i]; j != i; j = R[j]) {
                U[D[j]] = U[j]; D[U[j]] = D[j];
                sz[col[j]]--;
            }
        }
    }

    void restore_col(int c) {
        for (int i = U[c]; i != c; i = U[i]) {
            for (int j = L[i]; j != i; j = L[j]) {
                sz[col[j]]++;
                U[D[j]] = j; D[U[j]] = j;
            }
        }
        L[R[c]] = c; R[L[c]] = c;
    }

    bool dance() {
        if (R[0] == 0) return true;
        int c = R[0];
        for (int j = R[0]; j != 0; j = R[j]) {
            if (sz[j] < sz[c]) c = j;
        }
        if (sz[c] == 0) return false;
        remove_col(c);
        for (int i = D[c]; i != c; i = D[i]) {
            answer.push_back(row[i]);
            for (int j = R[i]; j != i; j = R[j]) remove_col(col[j]);
            if (dance()) return true;
            for (int j = L[i]; j != i; j = L[j]) restore_col(col[j]);
            answer.pop_back();
        }
        restore_col(c);
        return false;
    }
};
```
#### de Bruijn 序列：最短覆盖全部长度 `n` 串的循环串

> **赛时先看**
> - **题目信号**：要求最短地枚举/覆盖全部二进制串或 `k` 进制串；相邻窗口需要重叠 `n-1` 位；题面可能明确写“每种长度为 n 的密码/状态恰好一次”。
> - **本质**：在大小为 `k` 的字符集上，构造长度恰为 `k^n` 的循环序列，使每个长度为 `n` 的串恰好作为循环子串出现一次。
> - **接法**：直接调用 `de_bruijn(k, n)` 得到循环序列；题目要普通字符串时，在末尾补上前 `n-1` 个字符再拼成字符串。
> - **复杂度判定**：时间和输出长度都是 `O(k^n)`，额外空间 `O(kn)`。输出本身就有 `k^n` 个字符，输出量过大的题目用不了。
> - **维护的量**：`a`（长度 `k*n+1` 的构造数组，存当前候选串）；`sequence`（输出的循环序列）；dfs 的 `t/p`（当前位置/最小周期候选）。
> - **警告**：返回值是循环序列而非线性字符串，跨结尾的窗口也算。这里 `k^n` 的长度必须能放进内存；不要把 `n` 和字母表大小 `k` 写反。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// k=2、n=3：输出二进制 de Bruijn 循环序列（长度 2^3=8）。
auto seq = de_bruijn(2, 3);
for (int x : seq) cout << x;
cout << '\n';              // 00010111
string s;
for (int x : seq) s.push_back(char('0' + x));
s += s.substr(0, 2);       // 补前 n-1 个字符转成线性串
cout << s << '\n';         // 0001011100
```

- 样例：循环串输出 `00010111`；补 2 个字符后是 `0001011100`。

**传参要求（照这个传不会错）：**

- `k`：字符集大小，`>= 1`；序列元素取值 `[0, k)`。
- `n`：窗口长度/阶数，`>= 1`。
- 返回值：`vector<int>`，长度恰为 `k^n`，是**循环**序列（跨结尾的窗口也算一次）；题目要线性字符串时自己补前 `n-1` 个字符。


**API / 入口函数（赛时只认这里列的名字）：**

- `de_bruijn(int k, int n)` -> 返回 k 元、n 阶 de Bruijn 循环序列，长度恰为 k^n。

使用：`de_bruijn(k,n)` 返回一个**循环**序列，元素在 `[0,k)`。若题目要普通字符串，在末尾补上前 `n-1` 个字符；二进制 CSES De Bruijn Sequence 就是这个做法。

典题模型：CSES De Bruijn Sequence；最短测试串覆盖所有长度固定的状态；基于 de Bruijn 图的欧拉回路构造。

```cpp
// 维护的量：a（长度 k*n+1 的构造数组，存当前候选串）；sequence（最终输出的 de Bruijn 循环序列）。
// 不变量：dfs(t,p) 结束时 a[1..n] 是周期为 p 的候选串；仅当 n 是 p 的倍数时输出其最小周期，
//         全部候选按字典序拼接后恰好覆盖每个 k 元 n 阶串各一次（FKM/Lyndon 词构造）。
vector<int> de_bruijn(int k, int n) {
    assert(k >= 1 && n >= 1);
    vector<int> a(k * n + 1), sequence;
    function<void(int, int)> dfs = [&](int t, int p) {
        if (t > n) {
            if (n % p == 0) {
                for (int i = 1; i <= p; ++i) sequence.push_back(a[i]); // 只输出最小周期 p，避免重复
            }
            return;
        }
        a[t] = a[t - p];
        dfs(t + 1, p);
        for (int x = a[t - p] + 1; x < k; ++x) {
            a[t] = x;
            dfs(t + 1, t); // 出现比周期内更大的新字符，最小周期更新为 t
        }
    };
    dfs(1, 1);
    return sequence;
}
```
#### 折半搜索 Meet-in-the-Middle

> **赛时先看**
> - **题目信号**：`2^n` 太大但 `2^(n/2)` 可接受（如 `n <= 40` 的选子集、双数组配对题）。
> - **本质**：`n` 约 40 的子集枚举。
> - **接法**：给 `n <= 40` 个数，选子集使和不超过 `limit` 且最大，直接 `max_subset_sum_leq(a, limit)`；若题目要“两半各选一个凑目标”，也是同一套“枚举一半 + 排序 + 二分/双指针”骨架。
> - **复杂度判定**：`O(2^(n/2) log 2^(n/2))`。
> - **维护的量**：`sums`（当前半边枚举出的所有子集和，各 `2^(n/2)` 个）；`ans`（合并时维护的最大可行和）。
> - **警告**：左右两半分别枚举，再排序二分/双指针。


**最小完整示例（先抄这一段就能跑）：**

```cpp
// n=6 个数，选一个子集使和不超过 12 且最大。
vector<i64> a = {2, 5, 3, 7, 9, 1};
cout << max_subset_sum_leq(a, 12) << '\n'; // 12（如 5+7 或 2+3+7）
```

- 样例：输出 `12`。

**传参要求（照这个传不会错）：**

- `subset_sums(a, l, r)`：枚举区间 `[l, r)` 内元素的全部子集和，返回 `vector<i64>`（内部函数，一般不用直接调）。
- `max_subset_sum_leq(a, limit)`：`a` 为 0-indexed 原数组（`vector<i64>`）；返回不超过 `limit` 的最大子集和；若所有子集和都大于 `limit`，返回 `LLONG_MIN`。
- 元素为负时子集和可能为负，`LLONG_MIN` 的“无解”判断不受影响。


**改板时先认这几个量：**

- `sums`：当前半边枚举出的所有子集和。
- `ans`：当前合并后得到的最大可行和。

```cpp
// 维护的量：sums（区间 [l,r) 内元素枚举出的全部子集和）。
// 不变量：返回的 sums 是区间元素所有子集和的完整列表，长度恰为 2^(r-l)。
vector<i64> subset_sums(const vector<i64>& a, int l, int r) {
    vector<i64> sums{0};
    for (int i = l; i < r; ++i) {
        int sz = (int)sums.size();
        for (int j = 0; j < sz; ++j) sums.push_back(sums[j] + a[i]); // 每个旧子集和扩出含 a[i] 的新一半
    }
    return sums;
}

// 维护的量：left/right（两半各自的子集和表）；ans（合并过程中维护的最大可行和）。
// 不变量：枚举 left 的每个 x，用二分找 right 中 <= limit-x 的最大值，x+它就是一个候选答案。
i64 max_subset_sum_leq(const vector<i64>& a, i64 limit) {
    int n = (int)a.size();
    auto left = subset_sums(a, 0, n / 2);
    auto right = subset_sums(a, n / 2, n);
    sort(right.begin(), right.end());
    i64 ans = LLONG_MIN;
    for (i64 x : left) {
        auto it = upper_bound(right.begin(), right.end(), limit - x); // 右半和必须补足到 limit 内
        if (it != right.begin()) ans = max(ans, x + *prev(it));
    }
    return ans;
}
```

