# Hanzhi XCPC 算法模板

作者：Hanzhi/寒枝OvO

A4 双栏打印版算法模板。每节固定四部分结构：**简介 / 快速识别 / API 接入 / 关键点**。

## 项目结构

```
Hanzhi-XCPC-Template/
├── build.py            # 双栏 PDF 构建脚本（自动合并 chapters/ → dist/）
├── chapters/           # 分章 Markdown 源文件（每章一个 .md）
│   ├── 00-使用指南.md      # 读法、复杂度速查、索引约定总表、头文件
│   ├── 01-基础工具.md      # 快读/二分/三分/离散化/前缀和/差分/位运算/BigInt/Frac/随机
│   ├── 02-数据结构.md      # DSU 家族/Fenwick/线段树家族/Beats/动态开点/莫队/主席树/Treap/...
│   ├── 03-树上问题.md      # LCA/树剖/差分/Euler 序/换根/基环树/虚树/点分治/dsu on tree/...
│   ├── 04-图论.md          # Dijkstra/Floyd/差分约束/MST/SCC/2-SAT/欧拉/匹配/Dinic/MCMF/...
│   ├── 05-字符串.md        # 哈希/二维哈希/KMP/Z/Trie/01Trie/AC/Manacher/PAM/SA/SAM/...
│   ├── 06-数论与组合.md    # ModInt/exgcd/线性筛/MR/Pollard-Rho/组合数/整除分块/反演/杜教筛/...
│   ├── 07-线性代数与多项式.md # 矩阵幂/高斯消元/行列式/FFT/NTT/FWT/FPS/BM+Kitamasa/插值/...
│   ├── 08-计算几何.md      # 基础/凸包/旋转卡壳/半平面交/圆/扫描线/KD-Tree/杂项
│   ├── 09-动态规划.md      # 背包/LIS/区间/状压/数位/期望/单调队列/斜率/Li Chao/WQS/...
│   ├── 10-博弈与概率.md    # SG/Nim 家族/随机算法/概率技巧
│   └── 11-交互通信与杂项.md # 交互/通信编码/构造/STL/陷阱清单
├── dist/               # 输出 PDF + 预览图
└── assets/             # （预留）封面素材
```

## 构建

```bash
pip install reportlab pygments
python build.py
# 输出 dist/Hanzhi-XCPC-Template.pdf
```

构建脚本会自动按文件名顺序合并 `chapters/*.md`，渲染为 A4 双栏 PDF（封面 + 目录 + 正文）。

## 每节格式约定

```markdown
### 01.4 整数二分：最小可行 / 最大可行

> **简介**：这个算法/结构在做什么，解决什么问题。
> **快速识别**：题面出现什么信号 → 用它；复杂度与限制。
> **API 接入**：
> - 构造/参数/返回值/下标起点（0-based / 1-based）
> - 边界条件与易错点
> **关键点**：内部数组/函数含义、不变量、改板时动哪里。

```cpp
// 可直接照抄的定义 + 最小调用示例
```


## 新增/修改模板的步骤

1. 在对应章节 `.md` 里新增一节（`### 编号.编号 名称`），严格按上面格式；
2. 若模板有索引约定（下标/k 起点），**同步更新 00 章的「索引约定总表」**；
3. `python build.py` 重新生成 PDF。

## 当前规模

- 192 节 / 188 个代码块 / 68 页（A4 双栏）
- 全量编译验证：0 语法错误（`python verify.py`）
- 近五年东亚赛区真题考点对照见 11.8 节
- 覆盖：基础工具、数据结构、树上、图论、字符串、数论组合、线性代数多项式、几何、DP、博弈概率、交互杂项

## 打印建议

- A4 双面、长边翻页；双栏排版已为打印优化（装订边距、代码折行）。
- 若需要进一步压缩页数：调小 `build.py` 中 `fontSize`（code 6.25 → 6.0，body 7.9 → 7.5）可再省约 15%。


## 真题验证修复（2025.8，E:\ZJSU-个人\ACM 真题库）

- [x] **KM slack 未初始化**（04.19）：构造函数漏 `slack.assign`，空 vector 访问段错误——已修并 300 组随机 vs 暴力验证
- [x] **TarjanSCC run 从 1 开始**（04.9）：2-SAT 等 0 号节点场景 comp[0] 幽灵值——改从 0 开始
- [x] **2-SAT 赋值方向反**（04.12）：应为 `comp[2i+1] < comp[2i]`（真分量编号小）——100 组随机 vs 暴力验证
- [x] **exCRT 合并公式错**（06.11）：`x += M*u` 少乘 t，应为 `x += M*(t*u mod m/g)`，且用 i128 + 条件减法防溢出——100 组随机不互质对拍验证
- [x] 已验证正确：Dinic（200 组 vs 暴力割）、NTT/三模 NTT（50 组 + 1e5 长度）、BM+Kitamasa、FHQ Treap、exLucas（合数模）
- 经验：**只编译不运行抓不到运行时 bug**，真题验证发现 4 个真 bug

## 真题验证修复（第二轮，2025.8 续）

- [x] **FHQTreap 哨兵 sz/cnt 未清零**（02.24）：0 号哨兵 Node 默认 sz=1，子树大小全部多算——加构造函数清零，200 组 vs multiset 验证
- [x] **FHQSeq 哨兵 sz 未清零**（02.25）：同上——200 组 vs vector 验证
- [x] **MCMF 负费用卡死**（04.16）：Dijkstra+势能版缺 Bellman-Ford 势能初始化，负费用边时 reduced cost 为负——已修
- [x] **MCMF cost 公式错**（04.16）：`cost += f*h[t]` 应为 `cost += f*(h[t]-h[s])`（BF 初始化后 h[s]≠0）——200 组含负费用 vs 朴素 SPFA 验证
- [x] 已验证正确：gauss_real（100 组残差）、det_mod（100 组 vs 暴力全排列）、convex_hull（100 组点集）
- [x] 回归测试归档：`tests/regression.cpp`（FHQ/MCMF/高斯/行列式/凸包一键重跑）
- 教训：FHQ 家族只编译不运行抓不到哨兵 bug；MCMF 负费用不跑必炸

## 缺口补齐（2025.8，四个 TODO 全部完成）

- [x] **poly_sqrt / poly_pow**（07.15）：完整牛顿迭代实现（a[0]==0 提因子版）——顺带修复原模板 **poly_inv 的 NTT 长度 bug**（用 len 应 2*len，循环卷积环绕导致 inv/ln/exp 全错）与 **poly_sqrt res 未 resize**（越界写堆损坏）；100 组随机 + n=4096 烟雾验证
- [x] **多点求值 / 快速插值**（07.8）：poly_mod（反转+逆）+ 积树 + 分治插值完整实现；100 组 vs 直接代入/朴素长除验证
- [x] **HLPP 最高标号预流推进**（04.18）：0-based 完整实现；300 组稠密小图 + 30 组 n=50 vs Dinic 验证
- [x] **动态点分治**（03.20）：点开关灯查最近距离完整实现（LCA 内联）；100 组随机树 x 300 操作 vs 暴力验证（修复 paths 重复 push bug）
- 回归测试归档：`tests/test_poly.cpp`、`tests/test_mpe.cpp`、`tests/test_hlpp.cpp`、`tests/test_dc.cpp`

## 已完成（第二轮补全）

- 新增：Splay、Link-Cut Tree、猫树、珂朵莉树 ODT、带花树、同余最短路、三元环计数、最大团、立体几何、exLucas、Min_25 筛、Bell 数、错排、LTE/连分数速查、广义 SAM、LGV 引理、Prufer 编码、FPS 全套(poly_inv/ln/exp)、Bostan-Mori

## 剩余 TODO（低频/复杂项）

- [ ] HLPP 最大流完整、动态点分治完整、poly_sqrt/poly_pow 完整、多点求值/快速插值完整
- [ ] 新增/修改模板后运行 python verify.py 确认 0 语法错误；新跨节依赖模板同步标注「依赖」行与 00.4 索引表



## 验证

- python verify.py：提取全部代码块 + 骨架 + clang++ 编译验证（当前 0 语法错误）
- python build.py：合并章节 → 双栏 PDF

---

## 致谢与开源协议

本仓库中的知识性内容（包括但不限于算法讲解、思路分析等）参考或引用了 [OI-wiki](https://github.com/OI-wiki/OI-wiki) 项目，在此表示诚挚的感谢。

本仓库采用 **CC BY-SA 4.0** 许可协议（见 LICENSE 文件）。
