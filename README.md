# Hanzhi's ACM Template

## 前言

这是寒枝的 XCPC 算法模板库，在结束我的算法竞赛生涯之前，这个项目应该会保持更新。欢迎大家提出意见，我都会考虑采纳。

## 最新版本（2026 字体优化最终版）

`一只奶龙-XCPC-2026-算法模板-字体优化最终打印版.md` / `.pdf` 为当前最新版本，共 **449 页（A4）**，直接送印即可使用。

本版在上一版的基础上完成了**全量内容自检与格式统一**：

- **结构统一**：全书 408 个模板小节统一为「赛时先看（题目信号 / 本质 / 复杂度判定 / 维护的量）→ 最小完整示例 → 传参要求 → API / 入口函数 → 抄板清单 → 改造点」标准结构，任何一节都能"先抄红色框、照蓝色框传参、直接跑通"。
- **内容修复**：修复了线段树合并引用悬垂、`or_convolution` 补长度、`fps_pow` 溢出、KM 越界、Bron-Kerbosch 上限、树上莫队空骨架、章节内重名结构体、跨节误抄描述等一批问题。
- **知识点补全**：新增「启发式合并」「爬山法」「模拟退火」等小节；确认 A\* / IDA\* / DSU on Tree 已覆盖。
- **构建管线**：`build_final_pdf.py` 支持 Linux / Windows 双环境自动探测字体，改完 markdown 后一条命令重建 PDF。

## 仓库结构

```
Hanzhi-s-ACM-Template/
├── 一只奶龙-XCPC-2026-算法模板-字体优化最终打印版.md   # 最新版全文（合并 Markdown，送印源）
├── 一只奶龙-XCPC-2026-算法模板-字体优化最终打印版.pdf   # 最新版打印 PDF（449 页）
├── markdown/                                            # 按 A-G 大类拆分的 Markdown 源文件
├── build_final_pdf.py                                   # PDF 构建脚本（reportlab）
├── assets/cover-original.pdf                            # 原版封面
├── README-打印与赛时使用.md                             # 打印与赛时使用说明
├── 最终审查记录.md                                      # 各轮内容/接口/排版审查记录
├── manifest.json                                        # 文件清单
├── Hanzhi's Template V1.1.md                            # 历史版本
├── Hanzhi's Template V1.0.md                            # 历史版本
├── README.md
└── LICENSE
```

### 章节速览（markdown/）

| 文件 | 内容 |
|---|---|
| `00-赛时使用说明.md` | 赛时五步法、复杂度判定表、任务类型表、本书小节标准结构 |
| `A-A 赛场入口与基础算法.md` | 通用骨架、位运算、二分/三分、滑动窗口/双指针、搜索（DFS/BFS/A\*/IDA\*/DLX）等 |
| `B-B 数据结构与离线维护.md` | 并查集家族、Fenwick、莫队、线段树家族、主席树、树套树、Treap/LCT、CDQ/整体二分等 |
| `C-C 树、图论与网络流.md` | 树上问题（LCA/HLD/点分治/虚树）、最短路、生成树、连通性、网络流与匹配 |
| `D-D 字符串与序列.md` | KMP/AC 自动机、哈希、Manacher/PAM、SA/SAM、LCS/编辑距离等 |
| `E-E 数学、数论、几何与多项式.md` | 数论、组合、线性代数、多项式（FFT/NTT/FPS）、计算几何 |
| `F-F 动态规划与状态优化.md` | 背包、区间 DP、状压、数位 DP、概率期望、DP 优化全家 |
| `G-G 交互、博弈、杂项与近期模型.md` | 交互/通信、博弈、随机化、近期训练赛模型 |

## 使用方式

1. 打印：A4、双面、长边翻页、实际大小 100%，直接使用根目录 PDF。
2. 赛时：先看每节「赛时先看」判断能不能用 → 抄「最小完整示例」→ 按「传参要求」改参数 → 其余按「改造点」微调。
3. 重建 PDF（改过 markdown 后）：
   ```bash
   python build_final_pdf.py
   ```

## 致谢与开源协议

本仓库中的知识性内容（包括但不限于算法讲解、思路分析等）参考或引用了 [OI-wiki](https://github.com/OI-wiki/OI-wiki) 项目，在此表示诚挚的感谢。

同样感谢每一位提出 Issue 和 PR 的朋友们。

### 原项目的协议要求

OI-wiki 项目采用以下协议进行许可：

1. **知识共享署名-相同方式共享 4.0 国际许可协议 (CC BY-SA 4.0)**
   - 根据该协议的要求，我在此给出明确的署名，指向原作者 [OI-wiki](https://github.com/OI-wiki/OI-wiki) 项目。
   - 你可以在以下链接查看 CC BY-SA 4.0 协议的完整副本：<https://creativecommons.org/licenses/by-sa/4.0/>
   - 本仓库中对原作品知识部分内容进行的修改，已通过本声明和版本控制历史进行标明。

2. **The Star And Thank Author License (SATA)**
   - 根据该附加协议的要求，我们呼吁所有使用者也前往 [OI-wiki 的 GitHub 仓库](https://github.com/OI-wiki/OI-wiki) 为本项目点赞（Star），并感谢 OI-wiki 社区的无私奉献。

### 本仓库的协议

基于原项目 **CC BY-SA 4.0** 协议的"相同方式共享"要求，**本仓库的整体默认采用 [CC BY-SA 4.0 国际许可协议](https://creativecommons.org/licenses/by-sa/4.0/)**。
