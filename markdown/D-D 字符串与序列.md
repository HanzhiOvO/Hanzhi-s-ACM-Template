## D 字符串与序列

### 09 字符串与序列匹配

字符串按单模式、多模式、哈希、回文、后缀结构和经典序列 DP 排列。出现位置、覆盖贡献这类典题紧跟 AC 自动机。
#### KMP 前缀函数

> **赛时先看**
> - **题目信号**：题面问“模式串在文本中出现位置/次数”“前缀等于后缀（border）”“最小循环节”。这类“单模式匹配 + border”问题直接上 KMP。
> - **本质**：预处理模式串每个前缀的最长真前后缀长度 `pi`；匹配时文本指针只前进，失配靠 `pi` 回跳模式串指针，把暴力的回退省掉。
> - **复杂度判定**：`O(n+m)`；`n+m` 到 1e6 常规操作，1e7 也能过；要多个模式串同时匹配请翻 AC 自动机，不要对每个模式各跑一遍 KMP。
> - **维护的量**：`pi[i]`（`s[0..i]` 的最长真前后缀长度）；`j`（当前文本后缀与模式串前缀的最长匹配长度）。
> - **接法**：“在 `text` 里找 `pat` 的所有出现位置”直接调用 `kmp_find(text, pat)`，返回 `0-based` 起点；题目输出从 `1` 开始时记得 `+1`。“前缀等于后缀/最小循环节”对单个串跑 `prefix_function(s)`，看最后一个值 `pi[n-1]`。
> - **警告**：`pi[i]` 是 `s[0..i]` 的最长真前后缀长度（不含整串本身）；循环节长度为 `n - pi[n-1]`，且仅当 `n % (n - pi[n-1]) == 0` 时 `s` 才是完全周期串。


**最小完整示例（先抄这一段就能跑）：**

题目：在文本 `text` 里找模式串 `pat` 的所有出现位置（起点 0-based）。

```cpp
string text, pat;
cin >> text >> pat;
vector<int> pos = kmp_find(text, pat);   // 1. 调用：返回所有 0-based 起点
cout << pos.size() << '\n';
for (int p : pos) cout << p << ' ';      // 2. 输出：起点下标；题目从 1 开始就 +1
cout << '\n';
```

样例：`text = "abababa", pat = "aba"` -> 起点 `0 2 4`，共 3 个。

**传参要求（照这个传不会错）：**

- `kmp_find(text, pat)`：`text` 文本、`pat` 模式串；返回 `vector<int>`，元素是 **0-based 起点**（按出现顺序）。
- `pat` 为空：返回所有字符间隙位置（`0..text.size()`），一般题不会传空串。
- `prefix_function(s)`：返回 `pi[i]` = `s[0..i]` 的最长真前后缀长度。
- 最小循环节：`n - pi[n-1]`；仅当 `n % (n - pi[n-1]) == 0` 时才是完整周期串。
- 多模式串匹配请翻 AC 自动机，别对每个模式各跑一遍 KMP。


**不会用就照抄：**

```cpp
auto pi = prefix_function(s);  // pi[i]：s[0..i] 的最长真前后缀长度
```


**API / 入口函数（赛时只认这里列的名字）：**

- `prefix_function(s)` -> 返回 `pi`；`pi[i]` 是 `s[0..i]` 的最长真前后缀长度。
- `kmp_find(text,pat)` -> 返回模式 `pat` 在 `text` 中所有 **0-indexed 起点**。

**抄板清单（照着做就行）：**
1. 抄哪段：`prefix_function` 和 `kmp_find` 两个函数，整段抄到 `solve()` 上面。
2. 构造：不用构造对象，直接当普通函数调。
3. 调用：`auto pi = prefix_function(s);` 或 `auto pos = kmp_find(text, pat);`
4. 取结果：`pi[n-1]` 是 `s` 的最长真前后缀长度；`pos` 是 `0-based` 起点列表，原样输出或逐个 `+1`。

**改造点（按题目改这几处）：**
- 求最小循环节：循环节长度 `= n - pi[n-1]`；若 `n % (n - pi[n-1]) == 0` 则 `s` 由循环节重复构成（如 `ababab`），否则不存在小于 `n` 的循环节（如 `ababa`）。
- 只要出现次数不要位置：输出 `pos.size()`，或删掉 `pos` 只维护计数器。
- 输出从 1 开始计数：输出 `pos[i] + 1`。
- 问每个前缀的出现次数/每个 border 长度：对 `pi` 数组做桶计数 + 后缀累加。

**核心逻辑（改代码时别破坏）：**

- `pi[i]` 是 `s[0..i]` 的最长真前后缀长度。
- 失配时把当前匹配长度 `j` 跳到 `pi[j-1]`，不用回退文本指针。

需要模式匹配时，优先用本节给出的完整匹配函数；不要把不同教材里的 `next[]` 定义和这份 `pi[]` 混用。

```cpp
// 维护的量：pi[i] = s[0..i] 的最长真前后缀长度（不含整串本身）。只读参数 s，不改。
vector<int> prefix_function(const string& s) {
    int n = (int)s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1]; // 失配沿 border 链回跳
        if (s[i] == s[j]) j++;                       // 匹配上则长度 +1
        pi[i] = j;
    }
    return pi;
}

// 维护的量：j = 当前已匹配的模式串长度；pos = 所有 0-based 匹配起点。
// 不变量：失配时 j 回退到 pi[j-1]，文本指针不回退，保证 O(n+m)。
vector<int> kmp_find(const string& text, const string& pat) {
    if (pat.empty()) {
        vector<int> boundaries(text.size() + 1);
        iota(boundaries.begin(), boundaries.end(), 0);
        return boundaries; // 空串在每个字符间隙（含两端）匹配一次。
    }
    vector<int> pi = prefix_function(pat), pos;
    int j = 0;
    for (int i = 0; i < (int)text.size(); ++i) {
        while (j > 0 && text[i] != pat[j]) j = pi[j - 1];
        if (text[i] == pat[j]) j++;
        if (j == (int)pat.size()) {
            pos.push_back(i - (int)pat.size() + 1); // 完整匹配，记录起点
            j = pi[j - 1];                          // 回退以允许重叠匹配
        }
    }
    return pos;
}
```
#### KMP 自动机：单个禁串计数 DP

> **赛时先看**
> - **题目信号**：题面要求“不能出现某个连续模式”“每加入一个字符要知道当前匹配了模式串多长前缀”；模式串只有一个，且字母表较小。
> - **本质**：构造或计数长度为 `len` 的字符串，要求不包含某个模式串；也可把自动机状态接到数位 DP、状压 DP 或矩阵快速幂中。
> - **接法**：看到“长度为 `n` 的字符串不能出现某个禁串”就用这个模板。`state` 表示当前后缀已经匹配了禁串前多少个字符，转移到 `m` 就说明禁串出现了，DP 时跳过。字符集不是小写前若干个字母时，先把字符压缩或修改 `SIG` 和字符映射。
> - **复杂度判定**：自动机 `O（字符集 * |pattern|)`，长度为 `len` 的普通 DP 为 `O(len * 字符集 * |pattern|)`。
> - **维护的量**：`state`（当前后缀已匹配禁串前缀的长度，范围 `[0,m-1]`）；`nxt[state][c]`（加字符 `c` 后的转移）；`pi`（KMP 前缀函数）。
> - **警告**：完整匹配后的下一转移必须从 `pi[m-1]` 回退，才能识别重叠匹配；空模式串会匹配所有字符串，本模板直接禁止；大长度时把转移矩阵快速幂。


**最小完整示例（先抄这一段就能跑）：**

题目：长度 `len` 的字符串只用 `'a'..'a'+sigma-1` 组成，求不含模式串 `pattern` 的个数，答案模 `998244353`。

```cpp
string pattern;
int len, sigma;
cin >> pattern >> len >> sigma;
i64 ans = count_strings_avoiding_one_pattern(pattern, len, sigma);
cout << ans << '\n';   // 调用：返回模 998244353 的合法串个数
```

样例：`pattern = "ab", len = 2, sigma = 2` -> `3`（aa、ba、bb）。

**传参要求（照这个传不会错）：**

- `count_strings_avoiding_one_pattern(pattern, len, alphabet_size, modulus = modn)`：`pattern` 禁串（非空、全小写，长度 `m`）；`len` 目标长度（≥ 0）；`alphabet_size` 字符集大小（1..26，取前 `alphabet_size` 个小写字母）；`modulus` 取模值（默认 `modn`）；返回 `i64`，模意义下的合法串数量。
- 也可手动建 `KMPAutomaton automaton(pattern)`，`automaton.nxt[state][c]` 是加字符 `c` 后的新状态（等于 `m` 表示命中禁串）；要接矩阵快速幂/数位 DP 时自己迭代这个转移。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const string& s)` -> 完成建树或预处理
**改板时先认这几个量：**

- `pi`：KMP 前缀函数。
- `nxt`：转移/子节点。

状态：`state` 是当前字符串后缀与模式串前缀的最长匹配长度；转移后到 `m` 代表刚刚完整匹配模式串。计数禁串时只保留状态 `[0,m-1]`。

```cpp
struct KMPAutomaton {
    static constexpr int SIG = 26;
    string pattern;
    vector<int> pi;
    vector<array<int, SIG>> nxt;

    KMPAutomaton() = default;
    explicit KMPAutomaton(const string& s) { build(s); }

    void build(const string& s) {
        assert(!s.empty());
        pattern = s;
        int m = (int)pattern.size();
        pi.assign(m, 0);
        for (int i = 1; i < m; ++i) {
            int j = pi[i - 1];
            while (j && pattern[i] != pattern[j]) j = pi[j - 1];
            if (pattern[i] == pattern[j]) ++j;
            pi[i] = j;
        }

        nxt.assign(m + 1, {});
        for (int state = 0; state <= m; ++state) {
            for (int c = 0; c < SIG; ++c) {
                char ch = char('a' + c);
                int j = (state == m ? pi[m - 1] : state);
                while (j && pattern[j] != ch) j = pi[j - 1];
                if (pattern[j] == ch) ++j;
                nxt[state][c] = j;
            }
        }
    }
};

i64 count_strings_avoiding_one_pattern(
    const string& pattern, int len, int alphabet_size, i64 modulus = modn
) {
    assert(0 <= len && 1 <= alphabet_size && alphabet_size <= 26 && modulus > 0);
    KMPAutomaton automaton(pattern);
    int m = (int)pattern.size();
    vector<i64> dp(m);
    dp[0] = 1 % modulus;
    for (int pos = 0; pos < len; ++pos) {
        vector<i64> next_dp(m);
        for (int state = 0; state < m; ++state) {
            for (int c = 0; c < alphabet_size; ++c) {
                int to = automaton.nxt[state][c];
                if (to == m) continue; // 形成禁串，丢弃。
                next_dp[to] += dp[state];
                if (next_dp[to] >= modulus) next_dp[to] -= modulus;
            }
        }
        dp.swap(next_dp);
    }
    i64 answer = 0;
    for (i64 ways : dp) {
        answer += ways;
        if (answer >= modulus) answer -= modulus;
    }
    return answer;
}
```

典题模型：小写前 `sigma` 个字符组成长度 `n` 的字符串，要求不出现 `pattern`，答案模 `998244353`。调用 `count_strings_avoiding_one_pattern(pattern,n,sigma)`；若要求恰好出现次数，需额外加“已经匹配几次”的维度。
#### KMP 前缀数组计数字符串

> **赛时先看**
> - **题目信号**：题面给 `next[1..n]` / LPS / failure function，反问字符串数量或合法性。
> - **本质**：给定 KMP `pi/next` 数组和字符集大小，计算有多少个字符串恰好产生该数组，并可同时构造一个代表串。
> - **复杂度判定**：通常 `O(n + 失败链总访问）`，常见数据可过；只要构造合法性时非常好用。
> - **维护的量**：`id[i]`（构造出的代表串第 `i` 个字符的编号）；`used`（已使用的字符种类数）；`ans`（合法字符串计数，模 `KMP_COUNT_MOD`）。
> - **警告**：`pi[i]` 最多比 `pi[i-1]` 多 1；当 `pi[i]=0` 时，当前字符必须避开失败链上会导致匹配成功的字符。


**最小完整示例（先抄这一段就能跑）：**

题目：给定 KMP 前缀数组 `pi` 和字符集大小，求有多少个字符串恰好产生该数组（模 998244353）。

```cpp
vector<int> pi = {0, 0};                            // 题目给的 pi（0-based，pi[0] 必须为 0）
i64 cnt = count_strings_with_prefix_function(pi, 2);
cout << cnt << '\n';                                // 调用：返回模 998244353 的计数
```

样例：`pi = {0, 0}, alphabet_size = 2` -> `2`（`ab`、`ba`）。

**传参要求（照这个传不会错）：**

- `pi`：给定前缀数组，0-based，长度 `n`；要求 `pi[0] == 0`、`0 <= pi[i] <= i`、`pi[i] <= pi[i-1] + 1`，不满足返回 `0`。
- `alphabet_size`：字符集大小（正整数），字符用编号 `0..alphabet_size-1` 表示。
- `canonical`：可选输出参数，传 `vector<int>*` 时把构造出的一个代表串写进去（`id[i]` 为第 `i` 个字符编号）；不需要传 `nullptr`。
- 返回值：`i64`，模 `998244353` 的合法字符串数量；`pi` 非法时返回 `0`。


```cpp
const i64 KMP_COUNT_MOD = 998244353;

i64 count_strings_with_prefix_function(
    const vector<int>& pi,
    i64 alphabet_size,
    vector<int>* canonical = nullptr
) {
    int n = (int)pi.size();
    if (n == 0) return 1;
    if (pi[0] != 0 || alphabet_size <= 0) return 0;

    vector<int> id(n, 0);
    int used = 1;
    i64 ans = alphabet_size % KMP_COUNT_MOD;

    auto next_state = [&](int prev, int c) {
        int j = prev;
        while (j > 0 && c != id[j]) j = pi[j - 1];
        if (c == id[j]) j++;
        return j;
    };

    for (int i = 1; i < n; i++) {
        if (pi[i] < 0 || pi[i] > i || pi[i] > pi[i - 1] + 1) return 0;
        if (pi[i] > 0) {
            int c = id[pi[i] - 1];
            if (next_state(pi[i - 1], c) != pi[i]) return 0;
            id[i] = c;
        } else {
            vector<char> banned(max(used, 1), 0);
            int banned_count = 0;
            auto ban = [&](int c) {
                if (c >= (int)banned.size()) banned.resize(c + 1, 0);
                if (!banned[c]) {
                    banned[c] = 1;
                    banned_count++;
                }
            };
            for (int j = pi[i - 1]; j > 0; j = pi[j - 1]) ban(id[j]);
            ban(id[0]);

            i64 choices = alphabet_size - banned_count;
            if (choices <= 0) return 0;
            ans = ans * (choices % KMP_COUNT_MOD) % KMP_COUNT_MOD;

            int c = -1;
            for (int x = 0; x < used; x++) {
                if (x >= (int)banned.size() || !banned[x]) {
                    c = x;
                    break;
                }
            }
            if (c == -1) c = used++;
            id[i] = c;
            if (next_state(pi[i - 1], c) != 0) return 0;
        }
    }

    if (canonical) *canonical = id;
    return ans;
}
```

#### Z 函数

> **赛时先看**
> - **题目信号**：所有后缀与前缀比较；统计每个前缀出现次数。
> - **本质**：每个位置和全串前缀的 LCP。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`z[i]`（`s` 与后缀 `s[i..]` 的 LCP）；匹配盒 `[l, r]`（当前最右已探索区间）。
> - **警告**：`z[0]` 通常设为 0，按题意可改成 `n`。


**最小完整示例（先抄这一段就能跑）：**

题目：求字符串 `s` 每个后缀与全串的最长公共前缀长度。

```cpp
string s;
cin >> s;
vector<int> z = z_function(s);   // 调用：z[i] = LCP(s, s[i..])，0-based
cout << z[1] << '\n';
```

样例：`s = "aaaaa"` -> `z[1] = 4`；`s = "ababa"` -> `z[1] = 0`。

**传参要求（照这个传不会错）：**

- `z_function(s)`：`s` 原串（0-based）；返回 `vector<int> z`，长度 `|s|`。
- `z[i]` = `s[0..]` 与 `s[i..]` 的最长公共前缀长度（`i` 为 0-based 起点）。
- `z[0]` 约定为 `0`；有的题按定义应为 `n`，用时按题意自行处理。


```cpp
vector<int> z_function(const string& s) {
    int n = (int)s.size();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    return z;
}
```
#### 序列自动机：多模式子序列查询与最短缺失子序列

> **赛时先看**
> - **题目信号**：题面说“可删除若干字符但不能改变相对顺序”“很多模式串是否能嵌入一个固定文本”“最短不出现的子序列”；注意子序列不要求连续，和 KMP/SAM 的子串问题不同。
> - **本质**：对一个固定小写文本串，快速判断很多模式串是否为它的子序列，并输出一组最早匹配位置；也可在给定字符集内构造一个最短、且不是文本子序列的字符串。
> - **接法**：文本 `S` 固定，`q` 个模式串询问是否可由 `S` 删除字符得到。先 `SubsequenceAutomaton sam(S)`，每次调用 `sam.contains_subsequence(T)`；若需要输出选中的下标，使用 `sam.earliest_embedding(T)`。若问由 `abc` 组成、不是 `S` 子序列的最短串，调用 `shortest_missing_subsequence(sam,"abc")`。
> - **复杂度判定**：预处理 `O(26n)`，每次判断/还原 `O(|pattern|)`；最短缺失子序列构造 `O(26n)`。字符集不是小写字母时，把 `26` 换成压缩后的字符数。
> - **维护的量**：`n`（文本长度）；`nxt[i][c]`（从位置 `i` 起第一个字符 `c` 的下标，找不到为 `n`）。
> - **警告**：`nxt[i][c]` 表示从位置 `i` 起第一个 `c` 的位置，找不到时为 `n`；匹配到位置 `p` 后下一个状态应是 `p+1`；最短缺失串是“不是子序列”，不是“不是连续子串”。


**最小完整示例（先抄这一段就能跑）：**

题目：固定文本 `S`，询问模式串 `T` 是否为 `S` 的子序列（可跳字符）。

```cpp
string S, T;
cin >> S >> T;
SubsequenceAutomaton sam(S);            // 1. 构造：对固定文本 S 预处理
if (sam.contains_subsequence(T))        // 2. 调用：T 是否为 S 的子序列
    cout << "yes\n";
else
    cout << "no\n";
```

样例：`S = "abcde"`，`T = "ace"` -> yes；`T = "adc"` -> no。

**传参要求（照这个传不会错）：**

- `SubsequenceAutomaton sam(S)`：`S` 固定文本（0-based，小写）；`sam.nxt[i][c]` 是从位置 `i` 起第一个 `c` 的下标，找不到为 `n`。
- `sam.contains_subsequence(T)`：返回 `bool`，`T` 是否为 `S` 的子序列。
- `sam.earliest_embedding(T)`：返回 `optional<vector<int>>`，`T` 的**最早匹配下标**（0-based，升序）；不存在时返回 `nullopt`。
- `shortest_missing_subsequence(sam, alphabet)`：`alphabet` 是允许使用的小写字符集（可乱序，内部排序去重）；返回字典序最小的最短非空“不是子序列”串（`optional<string>`）；字符集为空时返回 `nullopt`。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const string& text)` -> 完成建树或预处理
**改板时先认这几个量：**

- `nxt`：转移/子节点。
- `dp`：DP 状态。

```cpp
struct SubsequenceAutomaton {
    static constexpr int SIG = 26;
    int n = 0;
    vector<array<int, SIG>> nxt;

    SubsequenceAutomaton() = default;
    explicit SubsequenceAutomaton(const string& text) { build(text); }

    void build(const string& text) {
        n = (int)text.size();
        nxt.assign(n + 1, {});
        nxt[n].fill(n);
        for (int i = n - 1; i >= 0; --i) {
            nxt[i] = nxt[i + 1];
            int c = text[i] - 'a';
            assert(0 <= c && c < SIG);
            nxt[i][c] = i;
        }
    }

    optional<vector<int>> earliest_embedding(const string& pattern) const {
        vector<int> position;
        position.reserve(pattern.size());
        int state = 0;
        for (char ch : pattern) {
            int c = ch - 'a';
            if (c < 0 || c >= SIG || nxt[state][c] == n) return nullopt;
            int at = nxt[state][c];
            position.push_back(at);
            state = at + 1;
        }
        return position;
    }

    bool contains_subsequence(const string& pattern) const {
        return earliest_embedding(pattern).has_value();
    }
};

// 返回字典序最小的最短非空字符串，字符来自给定字符集；
// 该字符串不是自动机原串的子序列。
optional<string> shortest_missing_subsequence(
    const SubsequenceAutomaton& automaton, string alphabet
) {
    sort(alphabet.begin(), alphabet.end());
    alphabet.erase(unique(alphabet.begin(), alphabet.end()), alphabet.end());
    if (alphabet.empty()) return nullopt;
    for (char ch : alphabet) assert('a' <= ch && ch <= 'z');

    const int INF_LEN = 1e9;
    int n = automaton.n;
    vector<int> dp(n + 1, INF_LEN);
    vector<char> choice(n + 1, '?');
    for (int state = n; state >= 0; --state) {
        for (char ch : alphabet) {
            int next_pos = automaton.nxt[state][ch - 'a'];
            int candidate = next_pos == n ? 1 : 1 + dp[next_pos + 1];
            if (candidate < dp[state]
                || (candidate == dp[state] && ch < choice[state])) {
                dp[state] = candidate;
                choice[state] = ch;
            }
        }
    }

    string answer;
    int state = 0;
    while (true) {
        char ch = choice[state];
        answer.push_back(ch);
        int next_pos = automaton.nxt[state][ch - 'a'];
        if (next_pos == n) return answer;
        state = next_pos + 1;
    }
}
```

典题模型：文本 `S` 固定，`q` 个模式串询问是否可由 `S` 删除字符得到。先 `SubsequenceAutomaton sam(S)`，每次调用 `sam.contains_subsequence(T)`；若需要输出选中的下标，使用 `sam.earliest_embedding(T)`。若问由 `abc` 组成、不是 `S` 子序列的最短串，调用 `shortest_missing_subsequence(sam,"abc")`。
#### Trie 字典树

> **赛时先看**
> - **题目信号**：大量字符串插入，问某前缀是否存在/出现几次。
> - **本质**：前缀查询、字符串集合、01 Trie 基础。
> - **接法**：先把所有字符串 `insert` 进 trie；问某个前缀出现过多少次就 `count_prefix(prefix)`。如果题目问完整单词出现次数，读到末尾节点后返回 `end`；如果题目是二进制最大异或，不用这个字符版，翻 01 Trie。
> - **复杂度判定**：总字符数。
> - **维护的量**：`tr`（节点池，`tr[0]` 是根）；每个节点 `pass`（经过该节点的字符串数）、`end`（恰好在该节点结束的字符串数）。
> - **警告**：字符集不是小写字母时要改数组大小或用 map。


**最小完整示例（先抄这一段就能跑）：**

题目：插入若干字符串，询问某个前缀出现过多少次、某个完整单词出现过几次。

```cpp
Trie trie;
trie.insert("apple");                       // 1. 插入字符串（可重复）
trie.insert("apply");
int pref = trie.count_prefix("app");        // 2. 以 "app" 为前缀的字符串数
int exact = trie.count_word("apple");       // 3. 完整单词 "apple" 出现次数
cout << pref << ' ' << exact << '\n';
```

样例：插入 `apple`、`apply` 后 -> `count_prefix("app") = 2`，`count_word("apple") = 1`。

**传参要求（照这个传不会错）：**

- `insert(s)`：插入一个小写字符串；可重复插入，重复时 `end/pass` 各 +1。
- `count_prefix(p)`：返回以 `p` 为前缀的已插入字符串个数（`p` 本身也计入）。
- `count_word(s)`：返回完整字符串 `s` 被插入的次数（读到末尾节点取 `end`）。
- `contains(s)`：返回 `bool`，`s` 是否至少被完整插入过一次。
- 字符映射固定 `'a'..'z'`（`ch - 'a'`），节点 `0` 是根；字符集不同时改 `nxt[26]` 或换 map。


**不会用就照抄：**

```cpp
Trie trie;
trie.insert(s);
int pref = trie.count_prefix(prefix); // 有多少已插入字符串以 prefix 开头
int exact = trie.count_word(s);       // 完整字符串 s 出现次数
if (trie.contains(s)) { }             // 是否至少插入过一次 s
```

- 先确认字符集映射（通常 `'a'..'z'`）。
- `insert` 之后再查；节点 0 通常是根。


**API / 入口函数（赛时只认这里列的名字）：**

- `trie.insert(s)` -> 插入一个小写字符串，可重复插入。
- `trie.count_prefix(p)` -> 有多少已插入字符串以 `p` 为前缀。
- `trie.count_word(s)` -> 完整字符串 `s` 被插入了多少次。
- `trie.contains(s)` -> 是否至少插入过一次完整字符串 `s`。
**核心逻辑（改代码时别破坏）：**

- 一条根到节点的路径就是一个前缀；`pass` 统计经过该节点的字符串数，`end` 统计恰好在此结束的字符串数。
**改板时先认这几个量：**

- `tr`：树节点池/自动机节点池。
- `nxt`：转移/子节点。

```cpp
struct Trie {
    struct Node {
        int nxt[26]{};
        int pass = 0, end = 0;
    };
    vector<Node> tr{Node{}};

    void insert(const string& s) {
        int u = 0;
        tr[u].pass++;
        for (char ch : s) {
            int c = ch - 'a';
            if (!tr[u].nxt[c]) {
                tr[u].nxt[c] = (int)tr.size();
                tr.push_back(Node{});
            }
            u = tr[u].nxt[c];
            tr[u].pass++;
        }
        tr[u].end++;
    }

    int count_prefix(const string& s) const {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (!tr[u].nxt[c]) return 0;
            u = tr[u].nxt[c];
        }
        return tr[u].pass;
    }

    int count_word(const string& s) const {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (!tr[u].nxt[c]) return 0;
            u = tr[u].nxt[c];
        }
        return tr[u].end;
    }

    bool contains(const string& s) const {
        return count_word(s) > 0;
    }
};
```
#### AC 自动机

> **赛时先看**
> - **题目信号**：给很多关键词，问它们在长文本里出现次数或是否出现。
> - **本质**：多模式串匹配。
> - **接法**：多个模式串一次性插入，编号从 `0` 到 `pattern_count-1`；`build()` 后扫文本，`match_count(text, pattern_count)[id]` 就是第 `id` 个模式出现次数。若模式很多且互为后缀，这个简单版会把输出列表复制到节点上，极端情况下可能大；只要最终次数时优先翻下一段 fail 树累计版。
> - **复杂度判定**：建树 `O（总模式长度 * 字符集）`，匹配 `O（文本长度 + 命中数）`。
> - **维护的量**：`tr`（Trie 节点池，`tr[0]` 是根）；`fail`（失配指针）；每个节点 `out`（在该节点结束的模式编号，build 后并入 fail 链上的编号）。
> - **警告**：`build` 后缺失转移被补到 fail 转移。


**最小完整示例（先抄这一段就能跑）：**

题目：给 `m` 个关键词，统计每个关键词在长文本 `text` 中的出现次数（允许重叠）。

```cpp
ACAutomaton ac;
for (int i = 0; i < m; ++i) {
    string pat;
    cin >> pat;
    ac.insert(pat, i);                       // 1. 插入模式串，绑定 0-based 编号 i
}
ac.build();                                  // 2. 所有模式插完后 build 一次
vector<int> cnt = ac.match_count(text, m);   // 3. 调用：统计每个模式出现次数
for (int i = 0; i < m; ++i) cout << cnt[i] << '\n';  // 4. 按编号输出
```

样例：模式 `he, she`，文本 `"shehe"` -> `cnt = {2, 1}`。

**传参要求（照这个传不会错）：**

- `insert(pattern, id)`：`pattern` 小写模式串；`id` 0-based 编号（范围 `0..m-1`），查询结果按它对齐。
- `build()`：所有模式插完后调用一次；**没 build 就查询是错的**。
- `match_count(text, m)`：`text` 长文本；`m` 模式总数；返回 `vector<int>`，`cnt[id]` = 第 `id` 个模式在 `text` 中的重叠出现次数（含互为后缀时 fail 链上的命中）。
- 模式很多且互为后缀时，本版把输出列表复制到节点上可能很慢；只要最终次数请翻下一节 fail 树累计版。


**不会用就照抄：**

```cpp
ACAutomaton ac;
for (auto &pat : patterns) ac.insert(pat, id);
ac.build();                     // 所有模式串插完后只 build 一次
// 然后再拿文本串 query / walk
```

- 调用顺序固定：`insert -> build -> query`。
- **没 build 就查询是错的**；build 会补 fail 和自动机转移。


**API / 入口函数（赛时只认这里列的名字）：**

- `ac.insert(pattern,id)` -> 插入模式串并绑定编号 `id`。
- `ac.build()` -> 所有模式插完后构建 fail；查询前必须执行一次。
- `ac.match_count(text,m)` -> 统计编号 `0..m-1` 各模式在文本中出现次数。
**核心逻辑（改代码时别破坏）：**

- Trie 负责模式前缀；`fail` 把失配状态跳到最长可用后缀。
- `build()` 后把缺失转移补成 fail 转移，因此匹配文本时每个字符只走一次状态转移。
**改板时先认这几个量：**

- `tr`：树节点池/自动机节点池。
- `fail`：AC 失配指针。
- `nxt`：转移/子节点。

```cpp
struct ACAutomaton {
    static const int SIG = 26;
    struct Node {
        int nxt[SIG]{};
        int fail = 0;
        vector<int> out;
    };
    vector<Node> tr{Node{}};

    void insert(const string& s, int id) {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (!tr[u].nxt[c]) {
                tr[u].nxt[c] = (int)tr.size();
                tr.push_back(Node{});
            }
            u = tr[u].nxt[c];
        }
        tr[u].out.push_back(id);
    }

    void build() {
        queue<int> q;
        for (int c = 0; c < SIG; ++c) if (tr[0].nxt[c]) q.push(tr[0].nxt[c]);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int c = 0; c < SIG; ++c) {
                int v = tr[u].nxt[c];
                if (v) {
                    tr[v].fail = tr[tr[u].fail].nxt[c];
                    auto& a = tr[v].out;
                    auto& b = tr[tr[v].fail].out;
                    a.insert(a.end(), b.begin(), b.end());
                    q.push(v);
                } else {
                    tr[u].nxt[c] = tr[tr[u].fail].nxt[c];
                }
            }
        }
    }

    vector<int> match_count(const string& text, int pattern_count) const {
        vector<int> cnt(pattern_count);
        int u = 0;
        for (char ch : text) {
            int c = ch - 'a';
            u = tr[u].nxt[c];
            for (int id : tr[u].out) cnt[id]++;
        }
        return cnt;
    }
};
```
#### AC 自动机：fail 树累计所有模式出现次数

> **赛时先看**
> - **题目信号**：模式串可能互为前后缀（如 `a`、`aa`、`aaa`），模式总长度和文本都很大，题目要求“每个关键词出现了多少次”。
> - **本质**：给很多模式串和一段文本，分别统计每个模式串在文本中的**重叠**出现次数；适合只要最终次数、不需要逐个枚举匹配位置的题。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：建树 `O（总模式长度 * 字符集）`，匹配和反向累计 `O（文本长度 + 节点数 + 模式数）`。
> - **维护的量**：`visits[u]`（自动机状态 `u` 在文本中的访问次数，扫描时只加 1、不枚举 fail 链）；`terminal[id]`（第 id 个模式串的终止节点）；`bfs_order`（BFS 顺序，逆序即 fail 树的拓扑序，用它把次数推给 `fail` 父亲）。
> - **警告**：必须先 `insert` 完所有模式串再 `build`；`terminal` 按插入顺序保存，重复模式串会自然得到相同答案；匹配文本可以重叠，不能匹配后把状态清零。


**最小完整示例（先抄这一段就能跑）：**

题目：给 `m` 个模式串，统计每个模式串在长文本 `text` 中出现了多少次（允许重叠）。

```cpp
ACAutomatonFailTree ac;
for (int i = 0; i < m; ++i) {
    string pat;
    cin >> pat;
    ac.insert(pat);                // 1. 插入所有模式串（按题目输入顺序）
}
ac.build();                        // 2. 所有模式插完后 build 一次
vector<i64> cnt = ac.match_count(text);  // 3. 调用：统计每个模式的出现次数
for (int i = 0; i < m; ++i) cout << cnt[i] << '\n';  // 4. 按插入顺序输出
```

样例：模式 `a, aa, aaa`，文本 `"aaaa"` -> 出现次数 `4, 3, 2`。

**传参要求（照这个传不会错）：**

- `insert(pattern)`：插入一个模式串；返回它的编号（按插入顺序从 0 开始）。
- `build()`：所有模式插完后调用一次；**没 build 就 match 是错的**。
- `match_count(text)`：返回 `vector<i64>`，`cnt[id]` = 第 id 个模式在文本中的重叠出现次数。
- 只统计次数、不返回位置；要"出现位置"翻下一节 `PatternOccurrenceAC`。


**API / 入口函数（赛时只认这里列的名字）：**

- `build()` -> 完成建树或预处理
- `insert(const string& s)` -> 插入元素/字符串 返回 `int`。
- `match_count(const string& text)` -> 统计每个模式串在文本中的出现次数 返回 `vector<i64>`。
**改板时先认这几个量：**

- `terminal`：第 id 个模式串的终止节点，允许重复模式串。
- `tr`：树节点池/自动机节点池。
- `fail`：AC 失配指针。
- `nxt`：转移/子节点。

核心：扫描文本时只给当前自动机状态加 `1`；BFS 建立的 fail 树按逆序把访问次数推给 `fail` 父亲。一个模式串结尾节点收到的总次数，就是该模式串的答案。不会复制整条 fail 链的输出列表。

```cpp
struct ACAutomatonFailTree {
    static constexpr int SIG = 26;
    struct Node {
        array<int, SIG> nxt{};
        int fail = 0;
    };

    vector<Node> tr{Node{}};
    vector<int> terminal; // 第 id 个模式串的终止节点，允许重复模式串
    vector<int> bfs_order;

    int insert(const string& s) {
        assert(!s.empty());
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            assert(0 <= c && c < SIG);
            if (!tr[u].nxt[c]) {
                tr[u].nxt[c] = (int)tr.size();
                tr.push_back(Node{});
            }
            u = tr[u].nxt[c];
        }
        terminal.push_back(u);
        return (int)terminal.size() - 1;
    }

    void build() {
        queue<int> q;
        bfs_order = {0};
        for (int c = 0; c < SIG; ++c) {
            int v = tr[0].nxt[c];
            if (v) {
                q.push(v);
                bfs_order.push_back(v);
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int c = 0; c < SIG; ++c) {
                int v = tr[u].nxt[c];
                if (v) {
                    tr[v].fail = tr[tr[u].fail].nxt[c];
                    q.push(v);
                    bfs_order.push_back(v);
                } else {
                    tr[u].nxt[c] = tr[tr[u].fail].nxt[c];
                }
            }
        }
    }

    vector<i64> match_count(const string& text) const {
        vector<i64> visits(tr.size());
        int u = 0;
        for (char ch : text) {
            int c = ch - 'a';
            assert(0 <= c && c < SIG);
            u = tr[u].nxt[c];
            visits[u]++;
        }
        for (int i = (int)bfs_order.size() - 1; i > 0; --i) {
            int node = bfs_order[i];
            visits[tr[node].fail] += visits[node];
        }
        vector<i64> answer(terminal.size());
        for (int id = 0; id < (int)terminal.size(); ++id) {
            answer[id] = visits[terminal[id]];
        }
        return answer;
    }
};

// 典型调用：每个 pattern 的答案按插入顺序返回。
// 示例：ACAutomatonFailTree ac;
// 示例：for (const string& pattern : patterns) ac.insert(pattern);
// 示例：ac.build();
// 示例：vector<i64> count = ac.match_count(text);
```
#### AC 自动机：批量收集所有模式串出现位置

> **赛时先看**
> - **题目信号**：固定文本 `S`，很多模式串询问；不仅要出现次数，还要每个模式的出现位置；模式串总长较大，不能每个串单独 KMP。
> - **本质**：对所有模式串建 AC 自动机，扫描文本时沿 `up` 指针访问当前状态 fail 链上的模式串终点，收集每个模式串的所有出现位置。
> - **接法**：所有询问先 `insert`，再 `build`，然后 `collect_occurrences(text)`。如果后续只与每个串出现位置有关，同一模式串重复询问时直接复用结果。
> - **复杂度判定**：建树 `O（总模式长度 * 字符集）`；扫描复杂度为 `O(|S| + 输出出现次数）`。若每个位置会命中很多模式，要确认总输出量是否可承受。
> - **维护的量**：`trie`（Trie 节点池）；`up[u]`（fail 链上最近的有模式结束的节点，`-1` 表示没有）；`terminal`（各模式终点节点）、`length`（各模式长度）。
> - **警告**：重复模式串要么去重，要么让同一终点保存多个 id；`up[u]` 指向 fail 链上最近的“有模式结束”的节点，不是普通 fail；返回位置为 `0-based` 起点。


**最小完整示例（先抄这一段就能跑）：**

题目：给 `m` 个模式串，收集每个模式串在文本 `text` 中的所有出现起点。

```cpp
PatternOccurrenceAC ac;
for (int i = 0; i < m; ++i) {
    string pat;
    cin >> pat;
    ac.insert(pat);                       // 1. 插入模式串（返回 0-based 编号）
}
ac.build();                               // 2. 所有模式插完后 build 一次
auto occ = ac.collect_occurrences(text);  // 3. 调用：收集所有出现位置
for (int p : occ[0]) cout << p << ' ';    // 4. 第 0 个模式的所有起点（0-based）
```

样例：模式 `aba, a`，文本 `"ababa"` -> `occ[0] = {0, 2}`，`occ[1] = {0, 2, 4}`。

**传参要求（照这个传不会错）：**

- `insert(pattern)`：插入一个模式串，返回 `int` 编号（按插入顺序从 0 开始）；重复模式串会让同一终点保存多个 id。
- `build()`：所有模式插完后调用一次；**没 build 就 collect 是错的**。
- `collect_occurrences(text)`：返回 `vector<vector<int>>`，`occ[id]` = 第 `id` 个模式在 `text` 中的所有出现**起点**（0-based，升序）。
- 命中模式很多时输出总量可能巨大，先估量再收集。


**API / 入口函数（赛时只认这里列的名字）：**

- `build()` -> 完成建树或预处理
- `insert(const string& pattern)` -> 插入元素/字符串 返回 `int`。
**改板时先认这几个量：**

- `up`：fail 链上最近的有模式结束的节点（枚举命中时沿它跳）。
- `fail`：AC 失配指针。

```cpp
struct PatternOccurrenceAC {
    static constexpr int SIG = 26;
    struct Node {
        array<int, SIG> next{};
        int fail = 0;
        int up = -1;
        vector<int> out;
    };

    vector<Node> trie{Node{}};
    vector<int> terminal, length;

    int insert(const string& pattern) {
        assert(!pattern.empty());
        int u = 0;
        for (char ch : pattern) {
            int c = ch - 'a';
            assert(0 <= c && c < SIG);
            if (!trie[u].next[c]) {
                trie[u].next[c] = (int)trie.size();
                trie.push_back(Node{});
            }
            u = trie[u].next[c];
        }
        int id = (int)length.size();
        length.push_back((int)pattern.size());
        terminal.push_back(u);
        trie[u].out.push_back(id);
        return id;
    }

    void build() {
        queue<int> q;
        for (int c = 0; c < SIG; ++c) {
            int v = trie[0].next[c];
            if (v) q.push(v);
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            int f = trie[u].fail;
            trie[u].up = trie[f].out.empty() ? trie[f].up : f;
            for (int c = 0; c < SIG; ++c) {
                int v = trie[u].next[c];
                if (v) {
                    trie[v].fail = trie[f].next[c];
                    q.push(v);
                } else {
                    trie[u].next[c] = trie[f].next[c];
                }
            }
        }
    }

    vector<vector<int>> collect_occurrences(const string& text) const {
        vector<vector<int>> occ(length.size());
        int u = 0;
        for (int i = 0; i < (int)text.size(); ++i) {
            int c = text[i] - 'a';
            assert(0 <= c && c < SIG);
            u = trie[u].next[c];
            for (int v = trie[u].out.empty() ? trie[u].up : u; v != -1; v = trie[v].up) {
                for (int id : trie[v].out) occ[id].push_back(i - length[id] + 1);
            }
        }
        return occ;
    }
};
```

典题：本场 L《Substrings of Substrings》。先用本模板收集每个询问串在 `S` 中的所有出现起点，再把出现起点交给下一节的区间贡献公式。
#### 典题：子串出现覆盖的所有区间最大权和与总权和

> **赛时先看**
> - **题目信号**：题面说区间是好的当且仅当模式串是该区间的子串；需要统计所有好区间的最大子段权值/总权值；模式串出现位置已经能求出来。
> - **本质**：给定数组权值和某模式串在文本中的所有出现起点，统计所有“包含至少一次该模式”的区间 `[l,r]` 的最大权值和，以及全部区间权值和。
> - **接法**：先对原权值做前缀和 `P`、`P` 的前缀最小值、后缀最大值，以及 `P` 的前缀和。对于第 `k` 个出现起点 `p`，左端点只允许在 `(prev, p]` 之间，右端点在 `[p+len, n]` 的前缀下标之间。
> - **复杂度判定**：对一个模式串，若出现次数为 `m`，计算为 `O(m + n)` 预处理后 `O(m)`；所有模式合计取决于出现位置总量。
> - **维护的量**：`prefix`（权值前缀和）、`prefix_min`（前缀最小前缀和）、`suffix_max`（后缀最大前缀和）、`sum_prefix_values`（前缀和数组的前缀和，用于快速求区间权和）。
> - **警告**：要按出现起点分段，避免同一个好区间被多个出现位置重复统计。位置统一用 `0-based` 起点，权值数组用 `0-based`。


**最小完整示例（先抄这一段就能跑）：**

题目：给权值数组与模式串的所有出现起点，统计所有“包含至少一次该模式”的区间的最大权值和与总权值和（模 mod）。

```cpp
vector<i64> a = {1, 2, 3};          // 0-based 权值数组
vector<int> occ = {1};              // 模式出现起点（0-based）
auto res = covered_intervals_by_occurrences(a, 1, occ);
cout << res.maximum << ' ' << res.sum_mod << '\n';  // 调用：最大权和 总权和(模)
```

样例：`a = {1, 2, 3}`、`pattern_length = 1`、起点 `{1}` -> `maximum = 6`，`sum_mod = 16`。

**传参要求（照这个传不会错）：**

- `weight`：`vector<i64>`，0-based 权值数组，长度 `n`。
- `pattern_length`：模式串长度（`1..n`）。
- `occurrence`：`vector<int>`，所有出现起点（0-based；顺序任意，内部会排序去重）。
- `mod`：总权值和取模值（默认 `modn`）。
- 返回值：`CoveredIntervalStats{maximum, sum_mod}`——`maximum` 是所有好区间中的最大权值和；`sum_mod` 是全部好区间权值和模 `mod`。
- 配套用法：出现起点先用上一节 `PatternOccurrenceAC::collect_occurrences` 拿到。


```cpp
struct CoveredIntervalStats {
    i64 maximum = -(1LL << 62);
    i64 sum_mod = 0;
};

i64 normalize_mod(i128 x, i64 mod) {
    x %= mod;
    if (x < 0) x += mod;
    return (i64)x;
}

CoveredIntervalStats covered_intervals_by_occurrences(
    const vector<i64>& weight, int pattern_length, vector<int> occurrence,
    i64 mod = modn
) {
    int n = (int)weight.size();
    assert(1 <= pattern_length && pattern_length <= n);
    sort(occurrence.begin(), occurrence.end());
    occurrence.erase(unique(occurrence.begin(), occurrence.end()), occurrence.end());

    vector<i64> prefix(n + 1), prefix_min(n + 1), suffix_max(n + 1);
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + weight[i];
    prefix_min[0] = prefix[0];
    for (int i = 1; i <= n; ++i) prefix_min[i] = min(prefix_min[i - 1], prefix[i]);
    suffix_max[n] = prefix[n];
    for (int i = n - 1; i >= 0; --i) suffix_max[i] = max(suffix_max[i + 1], prefix[i]);

    vector<i64> sum_prefix_values(n + 2);
    for (int i = 0; i <= n; ++i) sum_prefix_values[i + 1] = sum_prefix_values[i] + prefix[i];
    auto range_sum_prefix_values = [&](int l, int r) -> i64 {
        if (l > r) return 0;
        return sum_prefix_values[r + 1] - sum_prefix_values[l];
    };

    CoveredIntervalStats result;
    i128 total = 0;
    int previous = -1;
    for (int p : occurrence) {
        assert(0 <= p && p + pattern_length <= n);
        result.maximum = max(result.maximum, suffix_max[p + pattern_length] - prefix_min[p]);

        i64 count_left = p - previous;
        i64 count_right = n - (p + pattern_length) + 1;
        i64 sum_right = range_sum_prefix_values(p + pattern_length, n);
        i64 sum_left = range_sum_prefix_values(previous + 1, p);
        total += (i128)count_left * sum_right - (i128)count_right * sum_left;
        previous = p;
    }
    result.sum_mod = normalize_mod(total, mod);
    return result;
}
```

典题：本场 L。对每个询问串拿到出现起点 `occ` 后，调用 `covered_intervals_by_occurrences(a, |t|, occ)`，输出 `maximum` 和 `sum_mod`。
#### AC 自动机 DP：计数不含任一禁串的字符串

> **赛时先看**
> - **题目信号**：给很多敏感词/禁止子串，问长度为 `len` 的合法串数量；或要求在构造/数位 DP 中维护“到目前为止没有出现禁串”。
> - **本质**：有多个禁用模式串，计数/构造不出现任何模式串的字符串。它是单模式 KMP 自动机的多模式推广。
> - **接法**：长度 `len` 的字符串只由 `'a'..'a'+sigma-1` 组成，给 `patterns`，求不含任意模式串的数量。`ACAvoidPatterns ac; for (auto& s: patterns) ac.insert(s); ac.build(); cout << ac.count_avoiding(len,sigma);`。如果题目要构造字典序最小合法串，贪心试字符并用“剩余长度是否仍存在合法状态”的 DP 判定。
> - **复杂度判定**：建树 `O（总模式长度 * 字符集）`，普通长度 DP `O(len * 节点数 * 字符集）`；`len` 极大时，将合法状态之间的转移做矩阵快速幂。
> - **维护的量**：`tr`（Trie 节点池）；每个节点 `forbidden`（该节点或其 fail 祖先上是否有禁串结尾）；`dp[u]`（长度为当前已填部分的合法状态计数）。
> - **警告**：`forbidden` 要从 `fail` 父亲向下传递；模式串可以互为后缀；空模式串意味着所有字符串非法，本模板禁止插入空串；先全部 `insert` 后再 `build`。


**最小完整示例（先抄这一段就能跑）：**

题目：长度 `len` 的字符串只用 `'a'..'a'+sigma-1` 组成，求不含任何禁串的字符串数量（模 modn）。

```cpp
ACAvoidPatterns ac;
for (auto& s : patterns) ac.insert(s);    // 1. 插入所有禁串（非空小写串）
ac.build();                               // 2. 全部插入后 build 一次
i64 ans = ac.count_avoiding(len, sigma);  // 3. 调用：长度 len、字符集大小 sigma
cout << ans << '\n';
```

样例：禁串 `"ab"`，`len = 2, sigma = 2` -> `3`（aa、ba、bb）。

**传参要求（照这个传不会错）：**

- `insert(pattern)`：插入一个禁串（非空、小写）；模式串可互为后缀，build 时自动传递 forbidden。
- `build()`：所有禁串插入完再调用一次；**没 build 就 count 是错的**。
- `count_avoiding(len, alphabet_size, modulus = modn)`：`len` 目标长度（≥ 0）；`alphabet_size` 字符集大小（1..26，取前 `alphabet_size` 个小写字母）；`modulus` 取模值（默认 `modn`）；返回 `i64` 模意义下的合法串数量。
- `len` 极大（1e9 量级）时，把状态间转移写成矩阵快速幂。


**API / 入口函数（赛时只认这里列的名字）：**

- `build()` -> 完成建树或预处理
- `insert(const string& pattern)` -> 插入元素/字符串
**改板时先认这几个量：**

- `tr`：树节点池/自动机节点池。
- `fail`：AC 失配指针。
- `nxt`：转移/子节点。
- `dp`：DP 状态。

状态：AC 节点表示当前后缀匹配到的 trie 节点；若节点自身或其 fail 链上有一个模式串结尾，则该状态 `forbidden`，DP 不得进入。

```cpp
struct ACAvoidPatterns {
    static constexpr int SIG = 26;
    struct Node {
        array<int, SIG> nxt{};
        int fail = 0;
        bool forbidden = false;
    };

    vector<Node> tr{Node{}};

    void insert(const string& pattern) {
        assert(!pattern.empty());
        int u = 0;
        for (char ch : pattern) {
            int c = ch - 'a';
            assert(0 <= c && c < SIG);
            if (!tr[u].nxt[c]) {
                tr[u].nxt[c] = (int)tr.size();
                tr.push_back(Node{});
            }
            u = tr[u].nxt[c];
        }
        tr[u].forbidden = true;
    }

    void build() {
        queue<int> q;
        for (int c = 0; c < SIG; ++c) {
            int v = tr[0].nxt[c];
            if (v) q.push(v);
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            tr[u].forbidden = tr[u].forbidden || tr[tr[u].fail].forbidden;
            for (int c = 0; c < SIG; ++c) {
                int v = tr[u].nxt[c];
                if (v) {
                    tr[v].fail = tr[tr[u].fail].nxt[c];
                    q.push(v);
                } else {
                    tr[u].nxt[c] = tr[tr[u].fail].nxt[c];
                }
            }
        }
    }

    i64 count_avoiding(int len, int alphabet_size, i64 modulus = modn) const {
        assert(0 <= len && 1 <= alphabet_size && alphabet_size <= SIG && modulus > 0);
        vector<i64> dp(tr.size());
        dp[0] = 1 % modulus;
        for (int pos = 0; pos < len; ++pos) {
            vector<i64> next_dp(tr.size());
            for (int u = 0; u < (int)tr.size(); ++u) {
                if (tr[u].forbidden || dp[u] == 0) continue;
                for (int c = 0; c < alphabet_size; ++c) {
                    int v = tr[u].nxt[c];
                    if (tr[v].forbidden) continue;
                    next_dp[v] += dp[u];
                    if (next_dp[v] >= modulus) next_dp[v] -= modulus;
                }
            }
            dp.swap(next_dp);
        }
        i64 answer = 0;
        for (int u = 0; u < (int)tr.size(); ++u) {
            if (tr[u].forbidden) continue;
            answer += dp[u];
            if (answer >= modulus) answer -= modulus;
        }
        return answer;
    }
};
```

典题模型：长度 `len` 的字符串只由 `'a'..'a'+sigma-1` 组成，给 `patterns`，求不含任意模式串的数量。`ACAvoidPatterns ac; for (auto& s: patterns) ac.insert(s); ac.build(); cout << ac.count_avoiding(len,sigma);`。如果题目要构造字典序最小合法串，贪心试字符并用“剩余长度是否仍存在合法状态”的 DP 判定。
#### 双哈希

> **赛时先看**
> - **题目信号**：大量子串相等判断，不能每次逐字符比较。
> - **本质**：`O(1)` 比较子串、子串去重、回文辅助。
> - **接法**：先 `DoubleHash h(s)`；比较 `s[l1..r1]` 和 `s[l2..r2]` 时先判断长度相等，再比较 `h.get(l1,r1) == h.get(l2,r2)`。本模板区间是 `0-based` 且两端闭区间。若题目卡哈希，命中答案后逐字符复核。
> - **复杂度判定**：预处理 `O(n)`，查询 `O(1)`。
> - **维护的量**：`h1/h2`（前缀哈希，`h[i]` 是 `s[0..i-1]` 的哈希）；`p1/p2`（底数幂，`p[i] = BASE^i`，用于 `get` 时把区间左端对齐）。
> - **警告**：哈希有冲突，正式赛用双模更稳。


**最小完整示例（先抄这一段就能跑）：**

题目：多次判断字符串 `s` 的两个子串是否相等。

```cpp
string s;
cin >> s;
DoubleHash h(s);                         // 1. 结构体定义：DoubleHash(字符串)
if (h.get(l1, r1) == h.get(l2, r2))      // 2. 调用：比较两个子串
    cout << "equal" << '\n';
```

样例：`s = "abcabc"`；`get(0,2) == get(3,5)` -> equal（都是 `abc`）。

**传参要求（照这个传不会错）：**

- `DoubleHash(s)`：构造时预处理；`s` 是普通字符串（0-based）。
- `get(l, r)`：返回子串 `s[l..r]` 的双模哈希；**0-based 且两端闭区间**。
- 比较前必须先确认两段长度相等（长度不同的哈希没有可比性）。
- 返回值是 `pair<i64, i64>`，直接 `==` 比较。
- 哈希有极小碰撞概率；对抗数据建议命中后逐字符复核。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(const string& s)` -> 完成建树或预处理

```cpp
struct DoubleHash {
    static const i64 MOD1 = 1000000007LL;
    static const i64 MOD2 = 1000000009LL;
    static const i64 BASE = 911382323LL;
    vector<i64> h1, h2, p1, p2;

    DoubleHash() = default;
    DoubleHash(const string& s) { build(s); }

    void build(const string& s) {
        int n = (int)s.size();
        h1.assign(n + 1, 0); h2.assign(n + 1, 0);
        p1.assign(n + 1, 1); p2.assign(n + 1, 1);
        for (int i = 0; i < n; ++i) {
            int x = (unsigned char)s[i] + 1;
            h1[i + 1] = (h1[i] * BASE + x) % MOD1;
            h2[i + 1] = (h2[i] * BASE + x) % MOD2;
            p1[i + 1] = p1[i] * BASE % MOD1;
            p2[i + 1] = p2[i] * BASE % MOD2;
        }
    }

    pair<i64, i64> get(int l, int r) const {
        i64 a = (h1[r + 1] - h1[l] * p1[r - l + 1]) % MOD1;
        i64 b = (h2[r + 1] - h2[l] * p2[r - l + 1]) % MOD2;
        if (a < 0) a += MOD1;
        if (b < 0) b += MOD2;
        return {a, b};
    }
};
```
#### 双模二维滚动哈希：O(1) 取任意子矩阵

> **赛时先看**
> - **题目信号**：二维字符/颜色/数字网格；多次问“两个子矩阵是否完全相同”“某个矩形是否等于给定图案”；或需要枚举许多候选位置而逐格比较会到 `O(n^2m^2)`。
> - **本质**：预处理一个字符矩阵后，快速判断两块同尺寸矩形是否相等。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：预处理 `O(nm)`，单个子矩阵哈希 `O(1)`，空间 `O(nm)`；双模可显著降低随机碰撞概率。
> - **维护的量**：`hash1/hash2`（两个模数下的二维前缀多项式哈希，`H[i][j]` 是 `[0,i) x [0,j)` 的哈希）；`row_pow/col_pow`（行、列底数幂，查询时按位置归一化）；`n/m`（网格行数/列数）。
> - **警告**：哈希相等不是绝对相等，对抗性数据建议命中后逐格复核；不同尺寸矩形的哈希不可比，查询前必须确认高宽相同。
> - **约定**：下方实现统一为 0-indexed 半开区间 `[x1,x2) x [y1,y2)`，不要混用闭区间；双模二维多项式哈希。所有坐标均为 0-indexed、右下角开区间 [x1,x2) x [y1,y2)


**最小完整示例（先抄这一段就能跑）：**

题目：`n x m` 字符网格，多次判断两个同尺寸子矩形是否完全相同。

```cpp
DoubleHash2D h(grid);                          // 1. 结构体定义：DoubleHash2D(字符网格)
if (h.get(x1, y1, x2, y2) == h.get(a1, b1, a2, b2))  // 2. 调用：比较两块矩形
    cout << "same" << '\n';
```

样例：`grid = {"ab","ab"}`；`get(0,0,1,1) == get(0,1,1,2)` -> same（都是 `b`）。

**传参要求（照这个传不会错）：**

- `DoubleHash2D(grid)`：构造即预处理；`grid` 是 `vector<string>`，所有行必须等长。
- `get(x1, y1, x2, y2)`：半开区间 `[x1,x2) x [y1,y2)`，全部 **0-indexed**。
- 比较前必须确认两块矩形高、宽相同（不同尺寸不可比）。
- 返回 `Hash2DValue`，直接 `==` 比较；对抗数据建议命中后逐格复核。


**API / 入口函数（赛时只认这里列的名字）：**

- `get(int x1, int y1, int x2, int y2)` -> 返回矩形 [x1,x2) x [y1,y2) 的归一化哈希；比较前必须确认两个矩形尺寸相同。

状态：令 `H[i][j]` 是左上角到 `[0,i) x [0,j)` 的二维多项式哈希。行、列使用不同底数；查询时像二维前缀和一样容斥，并乘对应幂把位置归一化。（`add/sub/mul` 是内部取模助手，不要直接调。）

- 哈希相等不是数学上的绝对相等。对抗性强或必须零误判时，哈希命中后再逐格确认；不要只用单模数。
- 比较前必须确认两个矩形的高、宽都相同；不同尺寸的多项式哈希没有可比性。
- 下方实现统一为 0-indexed 半开区间 `[x1,x2) x [y1,y2)`，不要混用闭区间。
- `char` 可能是有符号类型，映射字符值时转为 `unsigned char`；网格行长度必须相等。

```cpp
struct Hash2DValue {
    int first = 0;
    int second = 0;

    bool operator==(const Hash2DValue& other) const {
        return first == other.first && second == other.second;
    }
    bool operator!=(const Hash2DValue& other) const {
        return !(*this == other);
    }
};

// 双模二维多项式哈希。所有坐标均为 0-indexed、右下角开区间 [x1,x2) x [y1,y2)。
struct DoubleHash2D {
    static constexpr int MOD1 = 1'000'000'007;
    static constexpr int MOD2 = 1'000'000'009;
    static constexpr int ROW_BASE1 = 911382323;
    static constexpr int COL_BASE1 = 972663749;
    static constexpr int ROW_BASE2 = 972663749;
    static constexpr int COL_BASE2 = 911382323;

    int n = 0, m = 0;
    vector<int> row_pow1, col_pow1, row_pow2, col_pow2;
    vector<vector<int>> hash1, hash2;

    static int add(int a, int b, int mod) {
        a += b;
        if (a >= mod) a -= mod;
        return a;
    }

    static int sub(int a, int b, int mod) {
        a -= b;
        if (a < 0) a += mod;
        return a;
    }

    static int mul(int a, int b, int mod) {
        return (i64)a * b % mod;
    }

    DoubleHash2D() = default;

    explicit DoubleHash2D(const vector<string>& grid) {
        n = (int)grid.size();
        m = n == 0 ? 0 : (int)grid[0].size();
        for (const string& row : grid) assert((int)row.size() == m);

        row_pow1.assign(n + 1, 1);
        row_pow2.assign(n + 1, 1);
        for (int i = 1; i <= n; ++i) {
            row_pow1[i] = mul(row_pow1[i - 1], ROW_BASE1, MOD1);
            row_pow2[i] = mul(row_pow2[i - 1], ROW_BASE2, MOD2);
        }
        col_pow1.assign(m + 1, 1);
        col_pow2.assign(m + 1, 1);
        for (int j = 1; j <= m; ++j) {
            col_pow1[j] = mul(col_pow1[j - 1], COL_BASE1, MOD1);
            col_pow2[j] = mul(col_pow2[j - 1], COL_BASE2, MOD2);
        }

        hash1.assign(n + 1, vector<int>(m + 1));
        hash2.assign(n + 1, vector<int>(m + 1));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                int value = (unsigned char)grid[i][j] + 1;
                hash1[i + 1][j + 1] = extend(
                    hash1, i, j, value, MOD1, ROW_BASE1, COL_BASE1
                );
                hash2[i + 1][j + 1] = extend(
                    hash2, i, j, value, MOD2, ROW_BASE2, COL_BASE2
                );
            }
        }
    }

    // 返回矩形 [x1,x2) x [y1,y2) 的归一化哈希；比较前必须确认两个矩形尺寸相同。
    Hash2DValue get(int x1, int y1, int x2, int y2) const {
        assert(0 <= x1 && x1 <= x2 && x2 <= n);
        assert(0 <= y1 && y1 <= y2 && y2 <= m);
        return {
            rectangle_hash(hash1, x1, y1, x2, y2, row_pow1, col_pow1, MOD1),
            rectangle_hash(hash2, x1, y1, x2, y2, row_pow2, col_pow2, MOD2),
        };
    }

private:
    static int extend(const vector<vector<int>>& hash, int i, int j, int value,
                      int mod, int row_base, int col_base) {
        int answer = value;
        answer = add(answer, mul(hash[i][j + 1], row_base, mod), mod);
        answer = add(answer, mul(hash[i + 1][j], col_base, mod), mod);
        answer = sub(answer, mul(mul(hash[i][j], row_base, mod), col_base, mod), mod);
        return answer;
    }

    static int rectangle_hash(const vector<vector<int>>& hash,
                              int x1, int y1, int x2, int y2,
                              const vector<int>& row_power,
                              const vector<int>& col_power, int mod) {
        int rows = x2 - x1, cols = y2 - y1;
        int answer = hash[x2][y2];
        answer = sub(answer, mul(hash[x1][y2], row_power[rows], mod), mod);
        answer = sub(answer, mul(hash[x2][y1], col_power[cols], mod), mod);
        answer = add(answer, mul(mul(hash[x1][y1], row_power[rows], mod),
                                 col_power[cols], mod), mod);
        return answer;
    }
};
```
#### 典题 std：在大网格中找出全部图案出现位置

> **赛时先看**
> - **题目信号**：题目是“找二维字符串/像素块/棋盘局面出现的位置”，模式大小固定但候选位置很多；允许用哈希，或可以在命中后验证。
> - **本质**：文本矩阵 `text` 给定，模式矩阵 `pattern` 给定，返回所有左上角位置。可以直接改成只问是否存在、第一次出现位置或出现次数。
> - **接法**：给 `n x m` 字符网格与 `h x w` 图案，调用 `find_2d_pattern(text, pattern)` 后输出 `positions.size()`。若网格中有一次修改、随后多次查询，不要每次重建二维哈希，改用二维 Fenwick/线段树或离线处理；普通滚动哈希只适合静态矩阵。
> - **复杂度判定**：建哈希 `O(nm + hw)`，扫描 `O((n-h+1)(m-w+1))`；每个候选位置的哈希比较 `O(1)`。
> - **维护的量**：`text_hash`/`pattern_hash`（两个 `DoubleHash2D`）；`positions`（所有匹配的左上角坐标，0-based）。
> - **警告**：模式比文本大时答案为空；比赛若严格防碰撞，在 `if` 命中后循环 `i,j` 比较每个字符再加入答案。


**最小完整示例（先抄这一段就能跑）：**

题目：在 `n x m` 字符网格中找出 `h x w` 图案的全部出现位置（左上角）。

```cpp
vector<string> grid = {"ab", "ab", "cd"};  // 文本网格（每行等长）
vector<string> pat = {"ab"};               // 图案（h x w）
auto pos = find_2d_pattern(grid, pat);     // 1. 调用：返回所有左上角
for (auto [x, y] : pos) cout << x << ' ' << y << '\n';  // 2. 0-based 坐标
```

样例：`grid = {"ab","ab","cd"}`、`pat = {"ab"}` -> 左上角 `(0,0)`、`(1,0)`。

**传参要求（照这个传不会错）：**

- `text`：`vector<string>`，n x m 字符网格，每行等长（0-based）。
- `pattern`：`vector<string>`，h x w 图案，非空且每行等长。
- 返回值：`vector<pair<int,int>>`，每个元素是一个匹配的**左上角坐标**（0-based，行序递增）。
- `h > n` 或 `w > m`（图案比文本大）时返回空；对抗数据可对命中位置逐格复核。


```cpp
vector<pair<int, int>> find_2d_pattern(const vector<string>& text,
                                       const vector<string>& pattern) {
    assert(!pattern.empty() && !pattern[0].empty());
    int pattern_width = (int)pattern[0].size();
    for (const string& row : pattern) assert((int)row.size() == pattern_width);
    if (text.empty()) return {};

    DoubleHash2D text_hash(text), pattern_hash(pattern);
    int n = text_hash.n, m = text_hash.m;
    int height = pattern_hash.n, width = pattern_hash.m;
    if (height > n || width > m) return {};

    Hash2DValue target = pattern_hash.get(0, 0, height, width);
    vector<pair<int, int>> positions;
    for (int x = 0; x + height <= n; ++x) {
        for (int y = 0; y + width <= m; ++y) {
            if (text_hash.get(x, y, x + height, y + width) == target) {
                positions.push_back({x, y});
            }
        }
    }
    return positions;
}
```

典题模型：给 `n x m` 字符网格与 `h x w` 图案，调用 `find_2d_pattern(text, pattern)` 后输出 `positions.size()`。若网格中有一次修改、随后多次查询，不要每次重建二维哈希，改用二维 Fenwick/线段树或离线处理；普通滚动哈希只适合静态矩阵。

#### Manacher

> **赛时先看**
> - **题目信号**：题面出现“回文子串/最长回文/回文半径”，或要求对每个中心求回文长度。连续回文子串问题先翻 Manacher。
> - **本质**：回文有镜像对称性质：当前最右回文 `[l,r]` 内部的中心，其半径初值可借镜像中心提供，再向外暴力扩展；摊下来每个字符只被扩 O(1) 次。
> - **复杂度判定**：`O(n)`；n 到 1e7 也稳（常数极小、无 log）；求最长回文子序列或“删字符变回文”不要用这里，要翻 DP。
> - **维护的量**：`d1[i]`（以 `i` 为中心的奇回文半径）、`d2[i]`（中心在 `i-1`/`i` 之间的偶回文半径）、当前最右回文 `[l,r]`。
> - **接法**：最长回文子串翻 Manacher；最长回文子序列不要翻这里，要翻 DP。`d1[i]` 表示以 `i` 为中心的奇数回文半径，实际长度 `2*d1[i]-1`；`d2[i]` 表示中心在 `i-1` 和 `i` 之间的偶数回文半径，实际长度 `2*d2[i]`。
> - **警告**：`d1` 是奇数回文半径，`d2` 是偶数回文半径；半径与实际长度的换算（`2*d1-1` / `2*d2`）是本节最容易错的地方。


**最小完整示例（先抄这一段就能跑）：**

题目：求字符串 `s` 的最长回文子串长度。

```cpp
string s;
cin >> s;
auto d1 = manacher_odd(s);    // 1. 调用：奇数回文半径数组（长度 = 2*d1[i]-1）
auto d2 = manacher_even(s);   // 2. 调用：偶数回文半径数组（长度 = 2*d2[i]）
int ans = 1;
for (int i = 0; i < (int)s.size(); ++i) {
    ans = max(ans, 2 * d1[i] - 1);   // 以 i 为中心的奇回文
    ans = max(ans, 2 * d2[i]);       // 中心在 i-1 与 i 之间的偶回文
}
cout << ans << '\n';
```

样例：`s = "abba"` -> `ans = 4`；`s = "ababc"` -> `ans = 3`。

**传参要求（照这个传不会错）：**

- `manacher_odd(s)`：返回 `d1[i]` = 以 `i` 为中心的奇回文半径；实际长度 `2*d1[i]-1`。
- `manacher_even(s)`：返回 `d2[i]` = 中心在 `i-1`/`i` 之间的偶回文半径；实际长度 `2*d2[i]`。
- 输入输出都是 0-based；两个数组长度都等于 `|s|`。
- 求"最长回文子序列"（允许跳字符）不要用这里，翻 D 章 DP。


**不会用就照抄：**

```cpp
auto d1 = manacher_odd(s);   // 奇数回文：长度 = 2*d1[i]-1
auto d2 = manacher_even(s);  // 偶数回文：长度 = 2*d2[i]
```

- 这类板子最容易错在“半径到底含不含中心”和“原串下标如何映射”；比赛时直接沿用本节返回约定。


**API / 入口函数（赛时只认这里列的名字）：**

- `manacher_odd(s)` -> 返回 `d1`；奇回文实际长度 `2*d1[i]-1`。
- `manacher_even(s)` -> 返回 `d2`；中心在 `i-1/i` 间，实际长度 `2*d2[i]`。

**抄板清单（照着做就行）：**
1. 抄哪段：`manacher_odd` 和 `manacher_even` 两个函数，整段抄到 `solve()` 上面。
2. 构造：不用构造对象，直接当普通函数调。
3. 调用：`auto d1 = manacher_odd(s); auto d2 = manacher_even(s);`
4. 取结果：以 `i` 为中心的奇回文长度 `2*d1[i]-1`；中心在 `i-1/i` 间的偶回文长度 `2*d2[i]`。

**改造点（按题目改这几处）：**
- 求最长回文子串：扫一遍 `d1/d2` 取最大长度；奇数最长候选起点 `i - d1[i] + 1`，偶数候选起点 `i - d2[i]`，按长度优先映射回原串下标输出。
- 只关心最长长度：`max_len = max(max_i(2*d1[i]-1), max_i(2*d2[i]))`。
- 统计回文子串数量：`sum(d1) + sum(d2)`（每个中心半径之和就是回文子串个数）。
- 题目以“某个已知回文 + 扩展”的形式问：直接 `2*d1[i]-1` / `2*d2[i]` 换算，别自己数。

**核心逻辑（改代码时别破坏）：**

- 维护当前最右回文 `[l,r]`，镜像半径提供初值，再向外暴力扩。
- `d1[i]` 含中心，奇回文长度 `2*d1[i]-1`；`d2[i]` 是缝中心，偶回文长度 `2*d2[i]`.

```cpp
// 维护的量：d[i] = 以 i 为中心（含中心）的奇回文半径；l/r = 当前最右回文 [l,r]。
// 不变量：i <= r 时初值 k = min(镜像半径, r-i+1)，保证不落后于已发现的最长回文。
vector<int> manacher_odd(const string& s) {
    int n = (int)s.size();
    vector<int> d(n);
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = (i > r) ? 1 : min(d[l + r - i], r - i + 1); // 镜像半径给初值
        while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
        d[i] = k--;
        if (i + k > r) l = i - k, r = i + k; // 更新最右回文边界
    }
    return d;
}

// 维护的量：d[i] = 中心在 i-1 与 i 之间的偶回文半径（单位为半个字符）；l/r = 当前最右回文。
// 不变量：半径 k 对应回文区间 [i-k, i+k-1]，长度恒为 2*k。
vector<int> manacher_even(const string& s) {
    int n = (int)s.size();
    vector<int> d(n);
    for (int i = 0, l = 0, r = -1; i < n; ++i) {
        int k = (i > r) ? 0 : min(d[l + r - i + 1], r - i + 1);
        while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
        d[i] = k--;
        if (i + k > r) l = i - k - 1, r = i + k;
    }
    return d;
}
```
#### 回文自动机 PAM

> **赛时先看**
> - **题目信号**：动态加入字符；询问不同回文串数量、每个回文出现次数。
> - **本质**：维护一个字符串的所有不同回文子串，统计数量、出现次数、最长回文后缀。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n * alphabet)` 或用 map 后 `O(n log alphabet)`。
> - **维护的量**：`tr`（回文树节点池，`tr[0]`/`tr[1]` 是长度 `0`/`-1` 的两个根）；`last`（当前字符的最长回文后缀节点）；`cnt`（节点回文出现次数，`count_occurrences` 后才是真实值）。
> - **警告**：有两个根，长度分别为 `0` 和 `-1`；fail 链用于找最长可扩展回文后缀。


**最小完整示例（先抄这一段就能跑）：**

题目：逐个加入字符，求不同回文子串数量与每个回文的出现次数。

```cpp
string s;
cin >> s;
PalindromicTree pt((int)s.size());   // 1. 构造：参数 n 是总字符数上界
for (char ch : s) pt.add_char(ch);    // 2. 逐个加入字符
cout << (int)pt.tr.size() - 2 << '\n'; // 3. 不同回文子串数（节点数减两个根）
```

样例：`s = "ababa"` -> 不同回文子串 `5`（a、b、aba、bab、ababa）。

**传参要求（照这个传不会错）：**

- `PalindromicTree pt(n)` / `init(n)`：`n` 为将要加入的字符总数上界（预留节点）。
- `add_char(ch)`：追加一个小写字符；所有字符加完后可用 `count_occurrences()` 按 fail 链自底向上累加，之后 `tr[i].cnt` 是第 `i` 个节点回文串的出现次数。
- 节点下标：`tr[0]` 是长度 `0` 的根（`fail = 1`），`tr[1]` 是长度 `-1` 的根，`tr[2..]` 才是真回文节点。
- 不同回文子串数量 = `tr.size() - 2`（去掉两个根）。


**API / 入口函数（赛时只认这里列的名字）：**

- `init(int n)` -> 初始化/清空结构
**改板时先认这几个量：**

- `tr`：树节点池/自动机节点池。
- `cur`：当前节点（get_fail 找到的最长可扩展回文后缀）。
- `fail`：回文后缀指针。

```cpp
struct PalindromicTree {
    struct Node {
        int next[26] = {};
        int len = 0, fail = 0, cnt = 0;
    };
    vector<Node> tr;
    string s;
    int last;

    PalindromicTree(int n = 0) { init(n); }
    void init(int n) {
        tr.assign(2, Node{});
        tr.reserve(n + 3);
        tr[0].len = 0; tr[0].fail = 1;
        tr[1].len = -1; tr[1].fail = 1;
        s = "#";
        last = 0;
    }

    int get_fail(int x) {
        int pos = (int)s.size() - 1;
        while (s[pos - tr[x].len - 1] != s[pos]) x = tr[x].fail;
        return x;
    }

    void add_char(char ch) {
        s.push_back(ch);
        int c = ch - 'a';
        int cur = get_fail(last);
        if (!tr[cur].next[c]) {
            Node node;
            node.len = tr[cur].len + 2;
            int fail_to = get_fail(tr[cur].fail);
            node.fail = tr[fail_to].next[c];
            if (!node.fail) node.fail = 0;
            tr[cur].next[c] = (int)tr.size();
            tr.push_back(node);
        }
        last = tr[cur].next[c];
        tr[last].cnt++;
    }

    void count_occurrences() {
        for (int i = (int)tr.size() - 1; i >= 2; i--) {
            tr[tr[i].fail].cnt += tr[i].cnt;
        }
    }
};
```
#### 后缀数组

> **赛时先看**
> - **题目信号**：所有子串/后缀按字典序处理。
> - **本质**：后缀排序、最长公共子串、不同子串数量。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：`O(n log n)`。
> - **维护的量**：`sa`（后缀按字典序排序后的起点）、`rk`（各起点的排名）、`height`（相邻后缀 LCP）。
> - **警告**：`height[i] = LCP(sa[i], sa[i-1])`。


**最小完整示例（先抄这一段就能跑）：**

题目：对字符串 `s` 求后缀数组，输出所有后缀按字典序排列后的起点。

```cpp
string s;
cin >> s;
SuffixArray sa(s);                    // 1. 构造：内部自动 build
for (int i = 0; i < (int)s.size(); ++i)
    cout << sa.sa[i] << ' ';          // 2. 字典序第 i 小后缀的起点（0-based）
```

样例：`s = "banana"` -> `sa = {5, 3, 1, 0, 4, 2}`。

**传参要求（照这个传不会错）：**

- `SuffixArray(string s_)` / `build(s_)`：`s_` 原串（0-based；字符按 `unsigned char` 值排序）。
- `sa[i]`：字典序第 `i` 小后缀的**起点下标**（0-based）。
- `rk[i]`：起点 `i` 的排名（恒有 `rk[sa[i]] = i`）。
- `height[i]`：`LCP(sa[i], sa[i-1])`（字典序相邻两后缀的 LCP），`height[0] = 0`。
- 不同子串数量 = `n(n+1)/2 - sum(height)`。


**API / 入口函数（赛时只认这里列的名字）：**

- `build(string s_)` -> 完成建树或预处理

```cpp
struct SuffixArray {
    string s;
    vector<int> sa, rk, height;

    SuffixArray() = default;
    SuffixArray(string s_) { build(std::move(s_)); }

    void build(string s_) {
        s = std::move(s_);
        int n = (int)s.size();
        sa.resize(n);
        rk.resize(n);
        height.assign(n, 0);
        iota(sa.begin(), sa.end(), 0);
        for (int i = 0; i < n; ++i) rk[i] = (unsigned char)s[i];

        vector<int> tmp(n);
        for (int k = 1;; k <<= 1) {
            auto cmp = [&](int a, int b) {
                if (rk[a] != rk[b]) return rk[a] < rk[b];
                int ra = a + k < n ? rk[a + k] : -1;
                int rb = b + k < n ? rk[b + k] : -1;
                return ra < rb;
            };
            sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; ++i) tmp[sa[i]] = tmp[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
            rk = tmp;
            if (rk[sa[n - 1]] == n - 1) break;
        }

        for (int i = 0, k = 0; i < n; ++i) {
            if (rk[i] == 0) continue;
            int j = sa[rk[i] - 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
            height[rk[i]] = k;
            if (k) k--;
        }
    }
};
```
#### 后缀数组 LCP RMQ 查询

> **赛时先看**
> - **题目信号**：大量询问两个后缀最长公共前缀；需要比较任意子串字典序。
> - **本质**：已建好 SA 与 height 后，`O(1)` 查询任意两个后缀的 LCP。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；**外部只调用下面 API 列出的接口**，不要把内部递归参数直接当题面参数。
> - **复杂度判定**：预处理 `O(n log n)`，查询 `O(1)`。
> - **维护的量**：`st[k][i]`（`height` 从 `i` 起长 `2^k` 区间的最小值）；`lg[i]`（`floor(log2(i))`）。
> - **警告**：`height[i] = lcp(sa[i], sa[i-1])`，查询 rank 间 `(l+1..r)` 的最小值。


**最小完整示例（先抄这一段就能跑）：**

题目：对字符串 `s` 建后缀数组后，任意询问两个后缀的最长公共前缀。

```cpp
string s;
cin >> s;
SuffixArray sa(s);                  // 1. 建 SA：内部自动填 sa/rk/height
LcpRMQ rmq(sa.height);              // 2. 用 height 建稀疏表
cout << rmq.lcp_suffix(0, 5, sa.rk) << '\n'; // 3. 后缀 s[0..] 与 s[5..] 的 LCP
```

样例：`s = "banana"`，`lcp_suffix(0, 5, sa.rk)` -> `1`。

**传参要求（照这个传不会错）：**

- `LcpRMQ(const vector<int>& height = {})` / `init(height)`：`height` 传上节 `SuffixArray::height`（`height[0] = 0`，其余为相邻后缀 LCP）；0-based。
- `range_min(l, r)`：闭区间 `[l, r]`，返回该段 `height` 最小值。
- `lcp_suffix(i, j, rk)`：`i, j` 是后缀**起点下标**（0-based），`rk` 传同一次建出的 `sa.rk`；返回两后缀最长公共前缀长度；`i == j` 时返回后缀长度。


**API / 入口函数（赛时只认这里列的名字）：**

- `init(const vector<int>& height)` -> 初始化/清空结构

```cpp
struct LcpRMQ {
    int n;
    vector<int> lg;
    vector<vector<int>> st;

    LcpRMQ(const vector<int>& height = {}) { if (!height.empty()) init(height); }

    void init(const vector<int>& height) {
        n = (int)height.size() - 1;
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) lg[i] = lg[i >> 1] + 1;
        st.assign(lg[n] + 1, vector<int>(n + 1));
        st[0] = height;
        for (int k = 1; k <= lg[n]; k++) {
            for (int i = 1; i + (1 << k) - 1 <= n; i++) {
                st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    int range_min(int l, int r) const {
        int k = lg[r - l + 1];
        return min(st[k][l], st[k][r - (1 << k) + 1]);
    }

    int lcp_suffix(int i, int j, const vector<int>& rk) const {
        if (i == j) return n - i + 1;
        int a = rk[i], b = rk[j];
        if (a > b) swap(a, b);
        return range_min(a + 1, b);
    }
};
```
#### 后缀自动机 SAM

> **赛时先看**
> - **题目信号**：对子串集合做统计，后缀数组不够方便。
> - **本质**：统计不同子串数量、子串出现次数、最长公共子串。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里先构造对象，再按下方“不会用就看这里”调用公开接口。
> - **复杂度判定**：`O(n * 字符集转移成本）`。
> - **维护的量**：`st`（状态数组：`link` 后缀链接、`len` 最长串长、`occurrence` 前缀结束位置数、`next` 转移）；`last` 为整个当前串对应的状态。
> - **警告**：`last` 表示整个当前串对应的状态；clone 状态复制转移。


**最小完整示例（先抄这一段就能跑）：**

题目：统计字符串 `s` 的**本质不同子串**数量。

```cpp
string s;
cin >> s;
SuffixAutomaton sam((int)s.size());  // 1. 构造：max_len 传字符串长度即可
for (char c : s) sam.extend(c);      // 2. 逐个字符插入
cout << sam.count_distinct_substrings() << '\n'; // 3. 本质不同子串数
```

样例：`s = "abab"` -> `7`。

**传参要求（照这个传不会错）：**

- `SuffixAutomaton(int max_len = 0)`：构造；`max_len` 传**字符串长度**（内部预留 `2*max_len` 个状态）。
- `extend(char c)`：追加一个字符 `c`（字符集任意，内部 `map` 转移）；无返回值；按原串顺序调用。
- `count_distinct_substrings()`：返回**本质不同子串数量**（`i64`）。


```cpp
struct SuffixAutomaton {
    struct State {
        int link = -1, len = 0;
        i64 occurrence = 0; // suffix-link 汇总前的前缀结束位置数量。
        map<char, int> next;
    };
    vector<State> st;
    int last;

    SuffixAutomaton(int max_len = 0) {
        st.reserve(2 * max_len);
        st.push_back(State{});
        last = 0;
    }

    void extend(char c) {
        int cur = (int)st.size();
        st.push_back(State{});
        st[cur].len = st[last].len + 1;
        st[cur].occurrence = 1;

        int p = last;
        while (p != -1 && !st[p].next.count(c)) {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = (int)st.size();
                st.push_back(st[q]);
                st[clone].len = st[p].len + 1;
                st[clone].occurrence = 0; // clone 状态不是新的结束位置。
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    i64 count_distinct_substrings() const {
        i64 ans = 0;
        for (int i = 1; i < (int)st.size(); ++i) {
            ans += st[i].len - st[st[i].link].len;
        }
        return ans;
    }
};
```
#### SAM：子串出现次数与两串最长公共子串

> **赛时先看**
> - **题目信号**：问一个模式串在文本中作为连续子串出现几次；或问两个串的最长公共连续片段。注意这不是 LCS（最长公共子序列），不能跳过中间字符。
> - **本质**：对固定文本串预处理多个模式串的出现次数，或在线性扫描第二个串时求两串最长公共**子串**长度。两者都直接复用上节构造好的 `SuffixAutomaton`。
> - **接法**：文本串 `text` 固定，有很多模式串询问。先对 `text` 的每个字符 `sam.extend(c)`，再做一次 `auto occ = sam_endpos_sizes(sam)`；每个模式输出 `sam_count_occurrences(sam,occ,pattern)`。若只问 `a,b` 的最长公共子串长度，对 `a` 建 SAM 后调用 `sam_longest_common_substring_length(sam,b)`。
> - **复杂度判定**：用 `map` 转移时，预处理出现次数 `O(|S| log |S|)`，单个模式/第二串扫描 `O(|P| log |S|)`；若字符集固定，`map` 换成数组后可视为线性。
> - **维护的量**：`order`（按 `len` 从大到小排的状态序，用于自底向上累加）；`occurrence`（各状态 endpos 大小，统计时沿 `link` 加到父状态）。
> - **警告**：所有非 clone 新状态的 `occurrence` 初值为 `1`，clone 必须为 `0`；统计前按 `len` 从大到小把出现次数累加到 `link`；模式串若沿转移失败，答案就是 `0`。


**最小完整示例（先抄这一段就能跑）：**

题目：文本串 `text` 固定，问模式串 `pattern` 在 `text` 中出现几次，以及 `text` 与串 `other` 的最长公共子串长度。

```cpp
string text, pattern, other;
cin >> text >> pattern >> other;
SuffixAutomaton sam((int)text.size()); // 1. 对文本建 SAM
for (char c : text) sam.extend(c);
auto occ = sam_endpos_sizes(sam);      // 2. 所有状态的出现次数
cout << sam_count_occurrences(sam, occ, pattern) << '\n'; // 3. 子串出现次数
cout << sam_longest_common_substring_length(sam, other) << '\n'; // 4. 最长公共子串
```

样例：`text = "ababa", pattern = "aba", other = "bab"` -> `2` 和 `3`。

**传参要求（照这个传不会错）：**

- `sam_endpos_sizes(sam)`：在文本全部 `extend` 完之后调用；返回 `vector<i64>`，下标为状态号，`occ[u]` 是状态 `u` 对应子串的出现次数。
- `sam_count_occurrences(sam, occ, pattern)`：`pattern` 非空；返回其在文本中的出现次数，未出现返回 `0`。
- `sam_longest_common_substring_length(sam, other)`：返回 `other` 与建 SAM 的串的最长公共**连续**子串长度（无公共字符时为 `0`）。


**API / 入口函数（赛时只认这里列的名字）：**

- `sam_endpos_sizes(const SuffixAutomaton& sam)` -> 必须在文本串所有字符都 extend 完之后再调用。 返回 `vector<i64>`。
**改板时先认这几个量：**

- `order`：按 `len` 从大到小排序后的状态编号，用于自底向上累加出现次数。
- `occurrence`：每个状态的 endpos 大小（出现次数），统计时沿 `link` 累加到父状态。

```cpp
// 必须在文本串所有字符都 extend 完之后再调用。
vector<i64> sam_endpos_sizes(const SuffixAutomaton& sam) {
    int n = (int)sam.st.size();
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int x, int y) {
        return sam.st[x].len > sam.st[y].len;
    });

    vector<i64> occurrence(n);
    for (int u = 0; u < n; ++u) occurrence[u] = sam.st[u].occurrence;
    for (int u : order) {
        int parent = sam.st[u].link;
        if (parent != -1) occurrence[parent] += occurrence[u];
    }
    return occurrence;
}

int sam_state_of(const SuffixAutomaton& sam, const string& pattern) {
    int u = 0;
    for (char c : pattern) {
        auto it = sam.st[u].next.find(c);
        if (it == sam.st[u].next.end()) return -1;
        u = it->second;
    }
    return u;
}

i64 sam_count_occurrences(
    const SuffixAutomaton& sam, const vector<i64>& endpos_size, const string& pattern
) {
    assert(!pattern.empty()); // 空串出现次数的定义因题而异，不能直接默认。
    int u = sam_state_of(sam, pattern);
    return u == -1 ? 0 : endpos_size[u];
}

int sam_longest_common_substring_length(const SuffixAutomaton& sam, const string& other) {
    int u = 0, matched = 0, answer = 0;
    for (char c : other) {
        while (u && !sam.st[u].next.count(c)) {
            u = sam.st[u].link;
            matched = sam.st[u].len;
        }
        auto it = sam.st[u].next.find(c);
        if (it != sam.st[u].next.end()) {
            u = it->second;
            ++matched;
        } else {
            u = 0;
            matched = 0;
        }
        answer = max(answer, matched);
    }
    return answer;
}
```

典题模型：文本串 `text` 固定，有很多模式串询问。先对 `text` 的每个字符 `sam.extend(c)`，再做一次 `auto occ = sam_endpos_sizes(sam)`；每个模式输出 `sam_count_occurrences(sam,occ,pattern)`。若只问 `a,b` 的最长公共子串长度，对 `a` 建 SAM 后调用 `sam_longest_common_substring_length(sam,b)`。
#### 多串最长公共子串：SAM 最小匹配长度

> **赛时先看**
> - **题目信号**：题面明确是多个字符串的“共同**连续**片段”；字符串数量多、总长度大；只建立一个串的 SAM，再逐个串扫描更新匹配长度。
> - **本质**：给多个字符串，求同时作为连续子串出现的最长字符串，并恢复一份答案。比逐对求 LCS 更准确，适用于多个日志、DNA 片段、字符串集合的共同连续片段。
> - **接法**：读入 `m` 个字符串后调用 `MultiStringLongestCommonSubstring solver; auto ans = solver.solve(strings);`；输出 `ans.length` 或 `ans.substring`。一个字符也没有共同出现时答案长度为 `0`、字符串为空。
> - **复杂度判定**：设总长度为 `L`，状态数为 `S`，这里使用 `map<char,int>`，复杂度约 `O(L log alphabet + S log S)`；固定小字符集可将 `map` 改数组降常数。
> - **维护的量**：`state`（SAM 状态池，含 `link/len/first_end/next`）；`best[u]`（状态 `u` 在全部串中的最小匹配长度）；`matched[u]`（当前扫描串在状态 `u` 的最大匹配长度）。
> - **警告**：求的是最长公共**子串**（连续），不是 LCS；状态向 `link` 推时必须截断为 `min(match[v], len[link[v]])`；"所有串的不同子串并集"类问题要换广义 SAM，本模板不适用。


**最小完整示例（先抄这一段就能跑）：**

题目：给 `m` 个字符串，求它们共同的最长公共**连续**子串，输出长度和一个答案串。

```cpp
int m;
cin >> m;
vector<string> strings(m);
for (int i = 0; i < m; ++i) cin >> strings[i];
MultiStringLongestCommonSubstring solver;  // 1. 建求解器（自动选最短串建 SAM）
auto ans = solver.solve(strings);          // 2. 调用：返回最长公共子串结果
cout << ans.length << ' ' << ans.substring << '\n';  // 3. 输出长度 + 一个答案串
```

样例：`strings = {"ababc", "babca", "abcb"}` -> `ans.length = 3`，`ans.substring = "abc"`。

**传参要求（照这个传不会错）：**

- `MultiStringLongestCommonSubstring solver;`：先建对象，构造不需要参数。
- `solver.solve(strings)`：`strings` 为全部待求字符串（`vector<string>`，0-based，任意 char 字符集）；返回 `MultiStringLCSResult{length, substring}`。
- `ans.length`：最长公共子串长度；没有任何共同字符时为 `0`。
- `ans.substring`：一个具体的公共子串；长度为 `0` 时为空串。
- 求的是公共**子串**（连续），不能跳字符；跳字符的公共子序列翻 LCS 模板。


**API / 入口函数（赛时只认这里列的名字）：**

- `solve(const vector<string>& strings)` -> 执行主算法并返回答案

核心：选最短串建 SAM。对每一条其余字符串，扫描时记录每个 SAM 状态的最大匹配长度，再按 `len` 从大到小把信息沿 suffix link 向上推。每个状态最终取所有字符串中的最小匹配长度，最大值就是答案。

- 是最长公共**子串**，不能跳过字符；允许跳字符的是 LCS 模板。
- 状态 `v` 往 `link[v]` 推时必须截断为 `min(match[v], len[link[v]])`，否则长度会超出父状态表示范围。
- 多串“不同子串总数并集”等题需要真正的广义 SAM/Trie-SAM；本块只解决多串共同子串，写得更短也更稳。

```cpp
struct MultiStringLCSResult {
    int length = 0;
    string substring;
};

// 任意 char 字符集的多串最长公共子串。选最短串建 SAM，减少状态数。
struct MultiStringLongestCommonSubstring {
    struct State {
        int link = -1;
        int len = 0;
        int first_end = -1;
        map<char, int> next;
    };

    vector<State> state;
    int last = 0;

    void reset() {
        state.assign(1, {});
        last = 0;
    }

    void extend(char c, int position) {
        int current = (int)state.size();
        state.push_back({-1, state[last].len + 1, position, {}});
        int p = last;
        while (p != -1 && !state[p].next.count(c)) {
            state[p].next[c] = current;
            p = state[p].link;
        }
        if (p == -1) {
            state[current].link = 0;
        } else {
            int q = state[p].next[c];
            if (state[p].len + 1 == state[q].len) {
                state[current].link = q;
            } else {
                int clone = (int)state.size();
                state.push_back(state[q]);
                state[clone].len = state[p].len + 1;
                while (p != -1 && state[p].next[c] == q) {
                    state[p].next[c] = clone;
                    p = state[p].link;
                }
                state[q].link = state[current].link = clone;
            }
        }
        last = current;
    }

    MultiStringLCSResult solve(const vector<string>& strings) {
        if (strings.empty()) return {};
        int base_index = 0;
        for (int i = 1; i < (int)strings.size(); ++i) {
            if (strings[i].size() < strings[base_index].size()) base_index = i;
        }
        const string& base = strings[base_index];
        if (base.empty()) return {};

        reset();
        for (int i = 0; i < (int)base.size(); ++i) extend(base[i], i);

        int states = (int)state.size();
        vector<int> order(states);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return state[a].len > state[b].len;
        });

        vector<int> best(states);
        for (int u = 0; u < states; ++u) best[u] = state[u].len;
        for (int index = 0; index < (int)strings.size(); ++index) {
            if (index == base_index) continue;
            vector<int> matched(states);
            int u = 0, length = 0;
            for (char c : strings[index]) {
                while (u && !state[u].next.count(c)) {
                    u = state[u].link;
                    length = state[u].len;
                }
                auto it = state[u].next.find(c);
                if (it == state[u].next.end()) {
                    u = 0;
                    length = 0;
                } else {
                    u = it->second;
                    ++length;
                }
                matched[u] = max(matched[u], length);
            }
            for (int v : order) {
                int p = state[v].link;
                if (p != -1) {
                    matched[p] = max(matched[p], min(matched[v], state[p].len));
                }
            }
            for (int state_id = 0; state_id < states; ++state_id) {
                best[state_id] = min(best[state_id], matched[state_id]);
            }
        }

        int chosen = 0;
        for (int u = 1; u < states; ++u) {
            if (best[u] > best[chosen]) chosen = u;
        }
        int length = best[chosen];
        if (length == 0) return {};
        return {length, base.substr(state[chosen].first_end - length + 1, length)};
    }
};
```

典题模型：读入 `m` 个字符串后调用 `MultiStringLongestCommonSubstring solver; auto ans = solver.solve(strings);`；输出 `ans.length` 或 `ans.substring`。一个字符也没有共同出现时答案长度为 `0`、字符串为空。
#### SAM：字典序第 k 小本质不同子串

> **赛时先看**
> - **题目信号**：题面要求“第 k 小不同子串”“本质不同子串按字典序”“输出第 k 个子串”；`k` 通常从 `1` 开始，且不把重复出现的相同字符串重复计数。
> - **本质**：按字典序访问所有不同非空子串，求第 `k` 小；同一条 SAM 路径代表一个不同子串，状态转移按字符有序枚举即可。
> - **接法**：给字符串 `s` 与多次 `k` 询问，要求输出字典序第 `k` 小的不同子串。建立 SAM 后只需预处理一次 `auto dp = sam_distinct_substring_path_counts(sam)`，每次查询调用 `sam_kth_distinct_substring(sam,dp,k)`；返回 `nullopt` 时按题意输出 `-1`。
> - **复杂度判定**：从每个状态汇总一次后 `O(|S| log |S|)`（这里 `map` 已按字符有序），单次第 k 小构造 `O（答案长度 + 经过的转移数）`；固定字符集数组转移可降常数。
> - **维护的量**：`dp[u]`（从状态 `u` 出发的非空本质不同子串路径数，上限 `CAP = 4e18` 防溢出）。
> - **警告**：本节计的是不同子串，不是按出现次数展开后的第 k 个；每条边的块大小是 `1 + dp[child]`，其中 `1` 对应刚走到该边形成的字符串；`k` 超过 `dp[root]` 时没有答案。


**最小完整示例（先抄这一段就能跑）：**

题目：对字符串 `s` 输出字典序第 `k` 小的本质不同子串（`k` 从 1 开始）。

```cpp
string s;
i64 k;
cin >> s >> k;
SuffixAutomaton sam((int)s.size());    // 1. 对 s 建 SAM
for (char c : s) sam.extend(c);
auto dp = sam_distinct_substring_path_counts(sam); // 2. 一次预处理
auto ans = sam_kth_distinct_substring(sam, dp, k); // 3. 第 k 小
cout << (ans ? *ans : "-1") << '\n';   // 4. 无解（k 超范围）输出 -1
```

样例：`s = "aab", k = 3` -> `"aab"`。

**传参要求（照这个传不会错）：**

- `sam_distinct_substring_path_counts(sam)`：在字符全部插入后调用一次；返回 `vector<i64>`，`dp[u]` 为从状态 `u` 出发的非空不同子串数。
- `sam_kth_distinct_substring(sam, dp, k)`：`k` 从 `1` 开始；返回 `optional<string>`，无解（`k <= 0` 或 `k > dp[0]`）时返回 `nullopt`（按题意输出 `-1`）。


```cpp
vector<i64> sam_distinct_substring_path_counts(const SuffixAutomaton& sam) {
    const i64 CAP = 4000000000000000000LL; // 只用于约束很大时保护加法不溢出。
    int n = (int)sam.st.size();
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int x, int y) {
        return sam.st[x].len > sam.st[y].len;
    });

    vector<i64> dp(n, 0); // 从每个状态出发的非空路径字符串数量。
    for (int u : order) {
        for (auto [c, v] : sam.st[u].next) {
            dp[u] = min(CAP, dp[u] + 1 + dp[v]);
        }
    }
    return dp;
}

optional<string> sam_kth_distinct_substring(
    const SuffixAutomaton& sam, const vector<i64>& dp, i64 k
) {
    if (k <= 0 || k > dp[0]) return nullopt;
    string answer;
    int u = 0;
    while (true) {
        bool moved = false;
        for (auto [c, v] : sam.st[u].next) { // std::map 会保持字典序遍历。
            i64 block_size = 1 + dp[v];
            if (k > block_size) {
                k -= block_size;
                continue;
            }
            answer.push_back(c);
            if (k == 1) return answer;
            --k;
            u = v;
            moved = true;
            break;
        }
        assert(moved); // 前面已经用 dp[0] 检查过 k，因此这里必然能找到转移。
    }
}
```

典题模型：给字符串 `s` 与多次 `k` 询问，要求输出字典序第 `k` 小的不同子串。建立 SAM 后只需预处理一次 `auto dp = sam_distinct_substring_path_counts(sam)`，每次查询调用 `sam_kth_distinct_substring(sam,dp,k)`；返回 `nullopt` 时按题意输出 `-1`。
#### LCS 最长公共子序列

> **赛时先看**
> - **题目信号**：两个序列中按原顺序选字符，不要求连续。
> - **本质**：求两个字符串/序列的最长公共子序列长度。
> - **接法**：如果题面写“删除一些字符后仍保持相对顺序”，就是子序列，可以用 LCS；如果写“连续片段/子串”，不要用这个模板。`n*m` 太大时不能硬跑二维 DP，改翻 bit-parallel LCS、SAM 或哈希模型。
> - **复杂度判定**：`O(nm)`。
> - **维护的量**：`dp[i][j]`（`a` 前 `i` 个与 `b` 前 `j` 个字符的最长公共子序列长度，`i` 行 `j` 列）。
> - **警告**：LCS 是子序列，不是子串；子串要连续。


**最小完整示例（先抄这一段就能跑）：**

题目：求字符串 `a` 与 `b` 的最长公共子序列长度（可跳字符）。

```cpp
string a, b;
cin >> a >> b;
cout << lcs_length(a, b) << '\n'; // 直接输出长度
```

样例：`a = "abcde", b = "ace"` -> `3`。

**传参要求（照这个传不会错）：**

- `lcs_length(a, b)`：两个参数都按原串传（0-based，内部按字符比较）；返回最长公共子序列长度；任一为空串时返回 `0`。


```cpp
int lcs_length(const string& a, const string& b) {
    int n = (int)a.size(), m = (int)b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return dp[n][m];
}
```
#### LCS 输出方案

> **赛时先看**
> - **题目信号**：题目要求构造公共子序列。
> - **本质**：不仅要长度，还要输出一个最长公共子序列。
> - **接法**：先和普通 LCS 一样填表，再从右下角倒着走。字符相同就选这个字符并同时左上走；不相同就走向 `dp` 值更大的邻格。最后反转得到答案。
> - **复杂度判定**：`O(nm)`。
> - **维护的量**：`dp[i][j]`（长度表，填法与普通 LCS 相同）；回溯指针 `i/j` 从右下角倒走。
> - **警告**：回溯时从 `dp[n][m]` 往回走。


**最小完整示例（先抄这一段就能跑）：**

题目：输出 `a` 与 `b` 的一个最长公共子序列字符串。

```cpp
string a, b;
cin >> a >> b;
cout << lcs_string(a, b) << '\n'; // 输出任意一条最长公共子序列
```

样例：`a = "abcde", b = "ace"` -> `"ace"`。

**传参要求（照这个传不会错）：**

- `lcs_string(a, b)`：按原串传（0-based）；返回 `string`，是 `a`、`b` 共同的一个最长公共子序列（有多个时返回回溯得到的其一）；无公共字符时返回空串。


```cpp
string lcs_string(const string& a, const string& b) {
    int n = (int)a.size(), m = (int)b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    string res;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            res.push_back(a[i - 1]);
            i--, j--;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    reverse(res.begin(), res.end());
    return res;
}
```
#### 最短公共超序列 SCS：合并两条保持顺序的序列

> **赛时先看**
> - **题目信号**：同时满足两条操作/事件序列的相对顺序；允许插入，但不允许打乱任一原序列。若只要求长度，答案是 `|a| + |b| - LCS(a,b)`；若要求构造，直接回溯 DP 表。
> - **本质**：构造一个最短字符串，使两个原串都是它的子序列。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：时间和空间都是 `O(|a||b|)`。
> - **维护的量**：`dp[i][j]`（`a` 前 `i` 个与 `b` 前 `j` 个的最短公共超序列长度，边界 `dp[i][0]=i`、`dp[0][j]=j`）。
> - **警告**：SCS 是“子序列”而非“公共子串”。两个字符相等时只追加一次；回溯到某串耗尽后，要把另一串剩余部分全部接上。


**最小完整示例（先抄这一段就能跑）：**

题目：求最短字符串使 `a`、`b` 都是它的子序列（顺序不变，允许插入）。

```cpp
string a, b;
cin >> a >> b;
cout << shortest_common_supersequence(a, b) << '\n'; // 输出最短超序列
```

样例：`a = "abac", b = "cab"` -> `"cabac"`（长度 `5`）。

**传参要求（照这个传不会错）：**

- `shortest_common_supersequence(a, b)`：按原串传（0-based）；返回 `string`，是包含 `a`、`b` 的最短公共超序列；长度恒为 `|a|+|b|-LCS(a,b)`。


```cpp
string shortest_common_supersequence(const string& a, const string& b) {
    int n = (int)a.size(), m = (int)b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + 1;
        }
    }

    string answer;
    for (int i = n, j = m; i > 0 || j > 0;) {
        if (i == 0) answer.push_back(b[--j]);
        else if (j == 0) answer.push_back(a[--i]);
        else if (a[i - 1] == b[j - 1]) {
            answer.push_back(a[--i]);
            --j;
        } else if (dp[i - 1][j] <= dp[i][j - 1]) {
            answer.push_back(a[--i]);
        } else {
            answer.push_back(b[--j]);
        }
    }
    reverse(answer.begin(), answer.end());
    return answer;
}
```
#### Bit-parallel LCS：只求长度

> **赛时先看**
> - **题目信号**：明确问 LCS 长度；字符集小或可压成字节；长度达到数万到十万级，二维 DP 内存或时间都不能接受。
> - **本质**：两串很长，普通 `O(nm)` LCS DP 超时，但只需要最长公共子序列长度，不需要还原具体方案。
> - **接法**：给两条 DNA/日志/字符串，`|A|, |B|` 都到 `5e4`，只问最长公共子序列长度。普通 DP 约需 `2.5e9` 次转移，而此模板约为它的 `1/64`；若题目要求输出方案，回到本册已有的普通 LCS/Hirschberg 类做法。
> - **复杂度判定**：`O(|A||B| / 64 + 256 * min(|A|,|B|) / 64)`，空间 `O(256 * min(|A|,|B|) / 64)`。实现通过 64 位字并行模拟一整列 DP。
> - **维护的量**：`match[ch]`（字符 `ch` 在短串 `a` 中出现位置的位向量，按 64 位字存）；`state`（当前列 DP 值的位向量数组）。
> - **警告**：只能求长度，不能直接恢复 LCS；`unsigned char` 索引避免非 ASCII 字符变负数；若字符是整数数组，可把 `array<... ,256>` 换成 `unordered_map<值， bit-vector>`。


**最小完整示例（先抄这一段就能跑）：**

题目：两条长串 `a, b`（长度可到 `5e4`），只求最长公共子序列长度。

```cpp
string a, b;
cin >> a >> b;
cout << lcs_bitparallel_length(a, b) << '\n'; // 约 O(|a||b|/64)
```

样例：`a = "abcbdab", b = "bdcaba"` -> `4`。

**传参要求（照这个传不会错）：**

- `lcs_bitparallel_length(a, b)`：按原串传值（内部自动把较短串换到 `a` 当位下标）；返回 LCS 长度；字符按 `unsigned char` 索引，非 ASCII 也安全；只求长度，不能恢复方案。


```cpp
int lcs_bitparallel_length(string a, string b) {
    if (a.size() > b.size()) swap(a, b); // 把较短字符串作为 bitset 的位下标，节省空间。
    int m = (int)a.size();
    if (m == 0) return 0;
    int words = (m + 63) >> 6;

    array<vector<ui64>, 256> match;
    for (auto& bits : match) bits.assign(words, 0);
    for (int i = 0; i < m; ++i) {
        match[(unsigned char)a[i]][i >> 6] |= 1ULL << (i & 63);
    }

    vector<ui64> state(words, 0);
    for (unsigned char ch : b) {
        ui64 shift_carry = 1; // 状态转移写法：(state << 1) | 1。
        ui64 borrow = 0;
        for (int w = 0; w < words; ++w) {
            ui64 x = state[w] | match[ch][w];
            ui64 y = (state[w] << 1) | shift_carry;
            shift_carry = state[w] >> 63;

            ui64 y_with_borrow = y + borrow;
            bool overflow = y_with_borrow < y;
            ui64 diff = x - y_with_borrow;
            borrow = overflow || x < y_with_borrow;
            state[w] = x & ~diff;
        }
    }

    int answer = 0;
    for (auto bits : state) answer += __builtin_popcountll(bits);
    return answer;
}
```

典题模型：给两条 DNA/日志/字符串，`|A|, |B|` 都到 `5e4`，只问最长公共子序列长度。普通 DP 约需 `2.5e9` 次转移，而此模板约为它的 `1/64`；若题目要求输出方案，回到本册已有的普通 LCS/Hirschberg 类做法。
#### 编辑距离 Levenshtein：最少插入、删除、替换

> **赛时先看**
> - **题目信号**：两串相互转换、拼写纠错、最少修改字符使两串相等；三个操作的代价均为 `1`。
> - **本质**：把字符串 `a` 变成 `b` 的最少操作次数，每步可插入一个字符、删除一个字符或替换一个字符。
> - **接法**：问把 `s` 改为 `t` 的最少编辑次数，直接输出 `levenshtein_distance(s,t)`。若只允许插入删除，答案才是 `|s|+|t|-2*LCS(s,t)`。
> - **复杂度判定**：`O(|a||b|)` 时间，`O(min(|a|,|b|))` 空间。
> - **维护的量**：`dp[j]`（当前行：`a` 前缀变到 `b[0..j)` 的最少代价）；`diagonal`（滚动数组中左上角 `dp[j-1]` 的旧值）。
> - **警告**：编辑距离允许替换，不能用 `|a|+|b|-2*LCS` 代替（后者只允许插删）；若三种操作代价不同，应改成带权转移；本版本只返回最小代价，恢复操作序列需要保留完整二维 `dp`。


**最小完整示例（先抄这一段就能跑）：**

题目：把串 `s` 改成 `t` 的最少编辑次数（插入、删除、替换各算 `1` 次）。

```cpp
string s, t;
cin >> s >> t;
cout << levenshtein_distance(s, t) << '\n'; // 最少编辑次数
```

样例：`s = "horse", t = "ros"` -> `3`。

**传参要求（照这个传不会错）：**

- `levenshtein_distance(a, b)`：按原串传值（内部可能交换 `a/b` 以压缩空间）；返回把 `a` 变成 `b` 的最少插入/删除/替换次数，三种操作代价均为 `1`。


**改板时先认这几个量：**

- `up`：上一行的 `dp[j]`（滚动数组里左上对角的值）。
- `dp`：DP 状态。

状态：`dp[j]` 是处理完 `a` 的当前前缀后，变成 `b[0..j)` 的最少代价。转移来自删除、插入、替换/保留三个方向。

```cpp
int levenshtein_distance(string a, string b) {
    if (a.size() < b.size()) swap(a, b); // b 是较短串，压缩空间。
    int n = (int)a.size(), m = (int)b.size();
    vector<int> dp(m + 1);
    iota(dp.begin(), dp.end(), 0);
    for (int i = 1; i <= n; ++i) {
        int diagonal = dp[0]; // 上一行的 dp[j-1]
        dp[0] = i;
        for (int j = 1; j <= m; ++j) {
            int up = dp[j];
            if (a[i - 1] == b[j - 1]) {
                dp[j] = diagonal;
            } else {
                dp[j] = 1 + min({diagonal, up, dp[j - 1]});
            }
            diagonal = up;
        }
    }
    return dp[m];
}
```

典题模型：问把 `s` 改为 `t` 的最少编辑次数，直接输出 `levenshtein_distance(s,t)`。若只允许插入删除，答案才是 `|s|+|t|-2*LCS(s,t)`。
#### 最长公共上升子序列 LCIS

> **赛时先看**
> - **题目信号**：两个序列、元素值需要相同、相对顺序要保留，并且选出的公共序列还要严格递增；通常 `n,m <= 3000` 左右。
> - **本质**：求两个整数序列共同拥有、且严格递增的最长子序列；比“分别求 LIS 再取交”严格得多。
> - **接法**：给数组 `a,b`，输出共同严格递增子序列最大长度或一条方案。调用 `auto ans=longest_common_increasing_subsequence(a,b);`，长度为 `ans.length`，方案为 `ans.sequence`。
> - **复杂度判定**：`O(nm)` 时间，`O(m)` 空间。
> - **维护的量**：`dp[j]`（以 `b[j]` 结尾的 LCIS 最大长度）；`last_node[j]`（该长度对应的回溯链节点）；`trace`（回溯链，记录取值与前驱）。
> - **警告**：`current` 只能由 `b[j] < a[i]` 更新，等于时用于接上；循环内不要把同一轮刚更新的相等状态拿来继续转移；重复数值时返回任意一条最优严格递增方案即可。


**最小完整示例（先抄这一段就能跑）：**

题目：两个整数数组 `a, b`，求共同且严格递增的最长子序列的长度与一条方案。

```cpp
vector<int> a, b;
// 读入 a、b ...
auto ans = longest_common_increasing_subsequence(a, b);
cout << ans.length << '\n';                 // 最大长度
for (int x : ans.sequence) cout << x << ' '; // 一条方案（严格递增）
```

样例：`a = {2,3,1,4,5}, b = {1,2,3,4}` -> 长度 `3`，方案 `2 3 4`。

**传参要求（照这个传不会错）：**

- `longest_common_increasing_subsequence(a, b)`：两个 `vector<int>` 按 0-based 原顺序传入；返回 `LCISResult`：`length` 为最大长度，`sequence` 为一条最优严格递增公共子序列（长度为 `0` 时 `sequence` 为空）。


状态：扫描 `a[i]` 时，`dp[j]` 表示以 `b[j]` 结尾的 LCIS 最大长度。遍历 `b`，维护所有 `b[j] < a[i]` 的最佳长度与位置；遇到 `a[i]==b[j]` 时接上它。

```cpp
struct LCISResult {
    int length = 0;
    vector<int> sequence;
};

LCISResult longest_common_increasing_subsequence(
    const vector<int>& a, const vector<int>& b
) {
    int m = (int)b.size();
    vector<int> dp(m), last_node(m, -1);
    struct TraceNode { int value, previous; };
    vector<TraceNode> trace;
    for (int x : a) {
        int best_length = 0, best_node = -1;
        for (int j = 0; j < m; ++j) {
            if (x == b[j] && best_length + 1 > dp[j]) {
                dp[j] = best_length + 1;
                trace.push_back({x, best_node});
                last_node[j] = (int)trace.size() - 1;
            }
            if (b[j] < x && dp[j] > best_length) {
                best_length = dp[j];
                best_node = last_node[j];
            }
        }
    }

    int end_pos = -1;
    for (int j = 0; j < m; ++j) {
        if (end_pos == -1 || dp[j] > dp[end_pos]) end_pos = j;
    }
    LCISResult result;
    result.length = (end_pos == -1 ? 0 : dp[end_pos]);
    for (int node = (end_pos == -1 ? -1 : last_node[end_pos]);
         node != -1; node = trace[node].previous) {
        result.sequence.push_back(trace[node].value);
    }
    reverse(result.sequence.begin(), result.sequence.end());
    return result;
}
```

典题模型：给数组 `a,b`，输出共同严格递增子序列最大长度或一条方案。调用 `auto ans=longest_common_increasing_subsequence(a,b);`，长度为 `ans.length`，方案为 `ans.sequence`。

#### 最长回文子序列与最少插入成回文

> **赛时先看**
> - **题目信号**：不要求连续地选字符，要求前后对称；题目问“最多保留多少形成回文”或“最少再插入多少”。注意它不同于 Manacher 的连续回文子串。
> - **本质**：允许删除字符后的最长回文长度，或求最少插入多少字符让整个字符串变成回文。
> - **接法**：字符串长度 `n <= 5000`，可删除任意字符，求最长回文子序列；调用第一函数。若每次可以在任意位置插入字符，最少插入数直接调用第二函数。
> - **复杂度判定**：`O(n^2)` 时间，`O(n)` 空间。
> - **维护的量**：`dp[j]`（当前外层 `i` 下区间 `s[i..j]` 的最长回文子序列长度）；`previous_diagonal`（旧的 `dp[j-1]`，对角线值）。
> - **警告**：`previous_diagonal` 必须保存旧的 `dp[j-1]`；空串答案是 `0`；最少插入数恰好是 `n - LPS`，但“最少删除成回文”也是同一个数。


**最小完整示例（先抄这一段就能跑）：**

题目：求串 `s` 的最长回文子序列长度，以及最少插入多少字符使 `s` 成为回文。

```cpp
string s;
cin >> s;
cout << longest_palindromic_subsequence_length(s) << '\n'; // 最长回文子序列
cout << minimum_insertions_to_palindrome(s) << '\n';       // 最少插入数
```

样例：`s = "bbbab"` -> `4` 和 `1`。

**传参要求（照这个传不会错）：**

- `longest_palindromic_subsequence_length(s)`：按原串传（0-based）；返回最长回文子序列长度；空串返回 `0`。
- `minimum_insertions_to_palindrome(s)`：按原串传；返回最少插入字符数，恒等于 `n - 最长回文子序列长度`。


状态：区间 `s[i..j]` 的最长回文子序列。相等时两端都取，否则丢掉一端。压缩后 `dp[j]` 对应当前 `i` 下的区间答案。

```cpp
int longest_palindromic_subsequence_length(const string& s) {
    int n = (int)s.size();
    vector<int> dp(n);
    for (int i = n - 1; i >= 0; --i) {
        dp[i] = 1;
        int previous_diagonal = 0;
        for (int j = i + 1; j < n; ++j) {
            int old_dp_j = dp[j];
            if (s[i] == s[j]) dp[j] = previous_diagonal + 2;
            else dp[j] = max(dp[j], dp[j - 1]);
            previous_diagonal = old_dp_j;
        }
    }
    return n == 0 ? 0 : dp[n - 1];
}

int minimum_insertions_to_palindrome(const string& s) {
    return (int)s.size() - longest_palindromic_subsequence_length(s);
}
```

典题模型：字符串长度 `n <= 5000`，可删除任意字符，求最长回文子序列；调用第一函数。若每次可以在任意位置插入字符，最少插入数直接调用第二函数。
#### 通配符匹配：`?` 匹配一个，`*` 匹配任意串

> **赛时先看**
> - **题目信号**：题面显式给 `?`、`*` 或“星号可代替任意多个字符”；要求整串匹配，不是只找一个子串。
> - **本质**：判断文本能否被含通配符的模式串完整匹配，`?` 匹配任意一个字符，`*` 匹配任意长度（含空）字符串。
> - **接法**：文件名匹配、DNA 模板匹配、含 `*` 和 `?` 的模式能否匹配全文。直接调用 `wildcard_full_match(text,pattern)`；若 `pattern` 很长且只含少量 `*`，可按星号分段后用 KMP/哈希进一步优化。
> - **复杂度判定**：`O(|text||pattern|)` 时间，`O(|pattern|)` 空间。
> - **维护的量**：`dp[j]`（当前文本前缀能否匹配模式前 `j` 个字符）；`next_dp`（下一行滚动数组）。
> - **警告**：初始化时只有连续前导 `*` 可以匹配空串；`*` 既可空也可多字符，两个转移都不能漏；本模板是全串匹配，若题目问子串匹配要在模式两端补 `*` 或改状态定义。


**最小完整示例（先抄这一段就能跑）：**

题目：判断含 `?`、`*` 的模式串 `pattern` 能否完整匹配文本 `text`。

```cpp
string text, pattern;
cin >> text >> pattern;
cout << (wildcard_full_match(text, pattern) ? "YES" : "NO") << '\n'; // 整串匹配
```

样例：`text = "adceb", pattern = "*a*b"` -> `YES`。

**传参要求（照这个传不会错）：**

- `wildcard_full_match(text, pattern)`：按原串传（0-based）；要求**整串**匹配；`?` 匹配任意一个字符、`*` 匹配任意长度（含空）字符串；返回 `bool`，完全匹配为 `true`。


状态：`dp[j]` 表示当前处理的文本前缀，能否匹配模式前 `j` 个字符。遇到 `*` 可选择匹配空串（`next[j-1]`）或吞掉当前文本字符（旧 `dp[j]`）。

```cpp
bool wildcard_full_match(const string& text, const string& pattern) {
    int m = (int)pattern.size();
    vector<char> dp(m + 1);
    dp[0] = true;
    for (int j = 1; j <= m && pattern[j - 1] == '*'; ++j) dp[j] = true;

    for (char ch : text) {
        vector<char> next_dp(m + 1);
        for (int j = 1; j <= m; ++j) {
            if (pattern[j - 1] == '*') {
                next_dp[j] = next_dp[j - 1] || dp[j];
            } else if (pattern[j - 1] == '?' || pattern[j - 1] == ch) {
                next_dp[j] = dp[j - 1];
            }
        }
        dp.swap(next_dp);
    }
    return dp[m];
}
```

典题模型：文件名匹配、DNA 模板匹配、含 `*` 和 `?` 的模式能否匹配全文。直接调用 `wildcard_full_match(text,pattern)`；若 `pattern` 很长且只含少量 `*`，可按星号分段后用 KMP/哈希进一步优化。
#### 最小表示法 Booth

> **赛时先看**
> - **题目信号**：环形字符串、旋转同构、最小循环表示。
> - **本质**：求一个循环字符串中字典序最小的旋转。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`t`（双倍串 `s+s`）；`i/j`（两个候选起点）；`k`（当前已比较相等的长度）；答案 `min(i, j)` 即最小旋转起点。
> - **警告**：返回的是起始下标，最小旋转串为 `s.substr(pos)+s.substr(0,pos)`。


**最小完整示例（先抄这一段就能跑）：**

题目：求字符串 `s` 的所有循环旋转中字典序最小的那个，输出其 0-based 起点（可选还原旋转串）。

```cpp
string s;
cin >> s;
int pos = min_rotation(s);                       // 1. 调用：最小旋转的起始下标（0-based）
string best = s.substr(pos) + s.substr(0, pos);  // 2. 还原最小旋转串（只要下标就省掉这行）
cout << pos << '\n';                             // 3. 输出起点；要串就输出 best
```

样例：`s = "bcab"` -> 起点 `2`，最小旋转串 `"abbc"`。

**传参要求（照这个传不会错）：**

- `min_rotation(s)`：`s` 任意字符串（0-based）；返回 `int`，字典序最小旋转的**起始下标**（0-based）。
- 最小旋转串 = `s.substr(pos) + s.substr(0, pos)`；并列最小时返回最小的那个下标。
- 空串或全相同字符时返回 `0`。


```cpp
int min_rotation(const string& s) {
    string t = s + s;
    int n = (int)s.size();
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (t[i + k] == t[j + k]) {
            k++;
        } else {
            if (t[i + k] > t[j + k]) i = i + k + 1;
            else j = j + k + 1;
            if (i == j) j++;
            k = 0;
        }
    }
    return min(i, j);
}
```
#### 整数序列最小循环表示 Booth

> **赛时先看**
> - **题目信号**：循环移位、环状序列、需要最小表示；元素不是字符而是整数/结构体。
> - **本质**：求一个整数序列所有循环移位中字典序最小的起点。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`i/j`（两个候选循环起点）；`k`（当前已比较相等的长度）；比较用取模下标 `(i+k) % n`；答案 `min(i, j) % n` 为最小起点。
> - **警告**：如果所有元素相同，返回 `0`；比较时取模下标。


**最小完整示例（先抄这一段就能跑）：**

题目：求整数序列所有循环移位中字典序最小的表示，输出起点，可选拿回整个最小序列。

```cpp
vector<int> a = {3, 1, 2, 1};
int p = min_rotation_index(a);               // 1. 调用：最小循环表示的起点（0-based）
auto seq = min_rotation_sequence(a);         // 2. 可选：直接得到最小循环表示序列
cout << p << '\n';
for (int x : seq) cout << x << ' ';          // 3. 输出：起点 / 最小序列
```

样例：`a = {3, 1, 2, 1}` -> 起点 `1`，最小序列 `1 2 1 3`。

**传参要求（照这个传不会错）：**

- `min_rotation_index(s)`：`s` 元素需支持 `==` 与 `>`（int/long long/char 均可）；0-based；返回 `int` 最小循环表示起点（0-based）；空向量或全相同元素返回 `0`。
- `min_rotation_sequence(s)`：返回 `vector<T>`，从最小起点开始的整个环，长度与 `s` 相同；只输出起点时不调它。
- 比较用 `(i + k) % n` 取模下标，等价于环上逐位比较。


```cpp
template <class T>
int min_rotation_index(const vector<T>& s) {
    int n = (int)s.size();
    if (n == 0) return 0;
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        const T& a = s[(i + k) % n];
        const T& b = s[(j + k) % n];
        if (a == b) {
            k++;
        } else if (a > b) {
            i = i + k + 1;
            if (i <= j) i = j + 1;
            k = 0;
        } else {
            j = j + k + 1;
            if (j <= i) j = i + 1;
            k = 0;
        }
    }
    return min(i, j) % n;
}

template <class T>
vector<T> min_rotation_sequence(const vector<T>& s) {
    int p = min_rotation_index(s);
    vector<T> res;
    for (int i = 0; i < (int)s.size(); i++) res.push_back(s[(p + i) % s.size()]);
    return res;
}
```
#### Lyndon 分解 Duval

> **赛时先看**
> - **题目信号**：题面出现 Lyndon、字典序分解、最小循环位移的扩展。
> - **本质**：把字符串唯一分解成非增 Lyndon 串，常用于最小表示、字符串周期分析。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`i`（当前段起点）；`j`（段探测终点）；`k`（段内周期比较位置）；`res`（Lyndon 段列表，每段为半开区间 `[l, r)`）。
> - **警告**：返回的是每段 `[l,r)`；比较时下标推进规则不要写反。


**最小完整示例（先抄这一段就能跑）：**

题目：把字符串 `s` 唯一分解成若干 Lyndon 串，输出每段内容。

```cpp
string s;
cin >> s;
auto parts = duval(s);                      // 1. 调用：Lyndon 分解的段区间列表
for (auto [l, r] : parts)
    cout << s.substr(l, r - l) << ' ';      // 2. 输出每段（区间是半开的 [l, r)）
cout << '\n';
```

样例：`s = "banana"` -> 分段 `[0,1) [1,3) [3,5) [5,6)`，即 `b an an a`。

**传参要求（照这个传不会错）：**

- `duval(s)`：`s` 原串（0-based）；返回 `vector<pair<int,int>>`，每段是 **半开区间 `[l, r)`**（0-based，右端点取不到）。
- 各段按原串顺序排列、互不重叠，拼接起来恰好是整个 `s`；每段本身是 Lyndon 串且段间字典序不增。
- 取段内容用 `s.substr(l, r - l)`。


```cpp
vector<pair<int, int>> duval(const string& s) {
    int n = (int)s.size();
    vector<pair<int, int>> res;
    int i = 0;
    while (i < n) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) {
            res.push_back({i, i + j - k});
            i += j - k;
        }
    }
    return res;
}
```
#### 不重叠子序列匹配次数

> **赛时先看**
> - **题目信号**：关键词按顺序出现但不要求连续；可以选择多个且位置不能重叠；模式固定且较短。
> - **本质**：给定模式序列，统计文本中最多能选出多少个互不重叠、等于模式的子序列。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`p`（模式串已匹配到第几个字符）；`ans`（已完成的不重叠匹配次数）；匹配完一个模式立即从头开始。
> - **警告**：每次尽早完成一个模式一定最优，这是区间调度的“最早结束”贪心。


**最小完整示例（先抄这一段就能跑）：**

题目：在文本序列中统计最多能选出多少个互不重叠的、等于模式 `pattern` 的子序列。

```cpp
vector<int> text = {1, 2, 1, 3, 2, 3};
vector<int> pattern = {1, 2, 3};
i64 cnt = count_disjoint_subsequence_matches(text, pattern);  // 1. 调用：最多不重叠次数
cout << cnt << '\n';                                          // 2. 输出次数
```

样例：`text = {1,2,1,3,2,3}, pattern = {1,2,3}` -> `2`（分别用下标 0,1,3 和 2,4,5）。

**传参要求（照这个传不会错）：**

- `text`：文本序列（0-based；元素 `T` 需支持 `==`，int/long long/char 均可）。
- `pattern`：模式序列（0-based）；`pattern` 为空时返回 `0`。
- 返回值：`i64`，最多能选出的**互不重叠**的完整模式子序列个数；每次匹配完从头再找。
- 贪心正确性：每次尽早匹配完一个模式，剩余文本最长，一定不劣。


```cpp
template <class T>
i64 count_disjoint_subsequence_matches(
    const vector<T>& text,
    const vector<T>& pattern
) {
    if (pattern.empty()) return 0;
    int p = 0;
    i64 ans = 0;
    for (const T& x : text) {
        if (x == pattern[p]) {
            p++;
            if (p == (int)pattern.size()) {
                ans++;
                p = 0;
            }
        }
    }
    return ans;
}
```

#### 字典序最小受限子序列

> **赛时先看**
> - **题目信号**：删掉若干字符得到车牌/编号；有若干规则禁止 `ab` 相邻；要求字典序最小可行解。
> - **本质**：从原串中选长度为 `need` 的子序列，同时禁止某些相邻字符对，要求字典序最小。
> - **复杂度判定**：`O(26 * n + 26 * need)`。
> - **维护的量**：`bad[a][b]`（禁止 `a` 后紧跟 `b`）；`next_pos[i][c]`（位置 `i` 起第一个字符 `c` 的下标）；`dp[i][last]`（位置 `i` 起、上一位为 `last` 时能延长的最长合法子序列长度，`last=26` 表示还没选）。
> - **警告**：贪心选当前字符时，要用后缀 DP 判断是否还能补齐；`last=26` 表示还没选字符。


**最小完整示例（先抄这一段就能跑）：**

题目：从 `s` 中选长度为 `need` 的子序列，选中序列里不允许出现若干指定相邻字符对，求字典序最小者。

```cpp
string s = "abcabc";
int need = 3;
vector<pair<char, char>> forb = {{'b', 'a'}};                      // 禁止 "ba" 相邻
string ans = min_lex_subsequence_with_forbidden_pairs(s, need, forb);  // 1. 调用
cout << ans << '\n';                                               // 2. 输出答案；-1 表示凑不够
```

样例：`s = "abcabc", need = 3`，禁对 `{'b','a'}` -> `"aab"`。

**传参要求（照这个传不会错）：**

- `s`：原串（0-based，小写字母）。
- `need`：要求的子序列长度（正整数）；凑不够时返回 `"-1"`。
- `forbidden_pairs`：禁止相邻对列表，每对 `{a, b}` 表示选中的子序列中**不允许 `a` 后紧跟 `b`**（`a`、`b` 均为小写字母，内部按 `bad[a-'a'][b-'a']` 标记，重复传无影响）。
- 返回值：`string`，字典序最小的长度为 `need` 的合法子序列；不存在返回 `"-1"`。


```cpp
string min_lex_subsequence_with_forbidden_pairs(
    const string& s,
    int need,
    const vector<pair<char, char>>& forbidden_pairs
) {
    int n = (int)s.size();
    const int NONE = 26;
    bool bad[27][26] = {};
    for (auto [a, b] : forbidden_pairs) bad[a - 'a'][b - 'a'] = true;

    vector next_pos(n + 2, array<int, 26>{});
    for (int c = 0; c < 26; c++) next_pos[n][c] = next_pos[n + 1][c] = n;
    for (int i = n - 1; i >= 0; i--) {
        next_pos[i] = next_pos[i + 1];
        next_pos[i][s[i] - 'a'] = i;
    }

    vector dp(n + 1, array<int, 27>{});
    for (int last = 0; last <= 26; last++) dp[n][last] = 0;
    for (int i = n - 1; i >= 0; i--) {
        int c = s[i] - 'a';
        for (int last = 0; last <= 26; last++) {
            dp[i][last] = dp[i + 1][last];
            if (last == NONE || !bad[last][c]) {
                dp[i][last] = max(dp[i][last], 1 + dp[i + 1][c]);
            }
        }
    }
    if (dp[0][NONE] < need) return "-1";

    string ans;
    int pos = 0, last = NONE, rem = need;
    while (rem > 0) {
        bool picked = false;
        for (int c = 0; c < 26; c++) {
            if (last != NONE && bad[last][c]) continue;
            int j = next_pos[pos][c];
            if (j < n && dp[j + 1][c] >= rem - 1) {
                ans.push_back(char('a' + c));
                pos = j + 1;
                last = c;
                rem--;
                picked = true;
                break;
            }
        }
        if (!picked) return "-1";
    }
    return ans;
}
```

#### 括号序列合法性与最长合法括号

> **赛时先看**
> - **题目信号**：只含 `(` 和 `)`，问是否合法或最长合法段。
> - **本质**：括号匹配、最长合法括号子串。
> - **接法**：把本节 `struct/class/函数组` 整段抄到 `solve()` 上面；`solve()` 里直接按下方“不会用就看这里”调用，不需要自己猜内部参数。
> - **复杂度判定**：`O(n)`。
> - **维护的量**：`bal`（当前未匹配的 `(` 数）；`st`（下标栈，栈底始终是最后一个非法位置，初始 `-1`）；`ans`（最长合法段长度）。
> - **警告**：最长合法括号用栈存下标，栈底放最后一个非法位置。


**最小完整示例（先抄这一段就能跑）：**

题目：判断括号串是否合法，并求最长合法连续子串长度。

```cpp
string s;
cin >> s;
bool ok = valid_parentheses(s);            // 1. 调用：是否为合法括号序列
int best = longest_valid_parentheses(s);   // 2. 调用：最长合法连续子串长度
cout << (ok ? "yes" : "no") << ' ' << best << '\n';
```

样例：`s = ")()())"` -> `no`，最长合法段长度 `4`。

**传参要求（照这个传不会错）：**

- `valid_parentheses(s)`：`s` 只含 `(` 与 `)`（有其他字符需先剥掉）；返回 `bool`，任意前缀中 `)` 不超过 `(` 且总数相等即合法。
- `longest_valid_parentheses(s)`：返回 `int`，最长**连续**合法括号子串长度；没有合法段时返回 `0`。
- 实现约定：栈存下标，栈底 `-1` 为虚拟位置；右括号弹栈后若栈空，把当前位置入栈作为新的“最后一个非法位置”。


```cpp
bool valid_parentheses(const string& s) {
    int bal = 0;
    for (char c : s) {
        if (c == '(') bal++;
        else bal--;
        if (bal < 0) return false;
    }
    return bal == 0;
}

int longest_valid_parentheses(const string& s) {
    vector<int> st;
    st.push_back(-1);
    int ans = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == '(') st.push_back(i);
        else {
            st.pop_back();
            if (st.empty()) st.push_back(i);
            else ans = max(ans, i - st.back());
        }
    }
    return ans;
}
```

