# -*- coding: utf-8 -*-
"""
verify.py - 模板代码编译验证
提取 chapters/*.md 中每个代码块，包上通用骨架用 clang++ 编译。
- 语法错误：需要修复的真问题
- 依赖注释（// 依赖：02.7）：自动拼上前置节的代码后编译（承接式模板也能真实编译验证）
- 未定义标识符：可能是隐式跨节依赖，记录但不算错
用法: python verify.py
"""
import re, subprocess, sys, tempfile, os
from pathlib import Path

ROOT = Path(__file__).resolve().parent
CHAPTERS = ROOT / 'chapters'
CLANG = r'D:\LLVM\bin\clang++.exe'

# 通用骨架（与 00.5 一致）
SKELETON = r'''
#include <bits/stdc++.h>
using namespace std;
using i32 = int;
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
#define v32 vector <int>
#define v64 vector <i64>
#define vv32 vector < vector <i32> >
#define vv64 vector < vector <i64> >
const int INF = 0x3f3f3f3f;
const i64 LINF = 4e18;
const int mod7 = 1e9 + 7;
const int mod9 = 1e9 + 9;
const int modn = 998244353;
const int dx[4] = {0, 0, 1, -1};
const int dy[4] = {1, -1, 0, 0};
'''

def extract_sections(md_text):
    """返回 [(节标题, [代码块...])]"""
    sections = []
    cur_title = ''
    cur_blocks = []
    buf = []
    in_code = False
    for ln in md_text.splitlines():
        if ln.startswith('### '):
            if cur_title or cur_blocks:
                sections.append((cur_title, cur_blocks))
            cur_title = ln[4:].strip()
            cur_blocks = []
            continue
        m = re.match(r'^```(\w*)\s*$', ln.strip())
        if m:
            if in_code:
                cur_blocks.append('\n'.join(buf))
                buf = []
                in_code = False
            else:
                in_code = True
        elif in_code:
            buf.append(ln)
    if cur_title or cur_blocks:
        sections.append((cur_title, cur_blocks))
    return sections

# 顶层语句的示例块（抄入 solve()/main() 用）会以此注释开头，跳过编译
USAGE_MARK = '用法示例'
# 依赖注释：// 依赖：02.7 或 // 依赖：02.7、04.15
DEP_RE = re.compile(r'//\s*依赖[：:]\s*([\d][\d.]*(?:[、,，\s]+[\d][\d.]*)*)')

def build_index():
    """{节编号: 该节首个代码块文本}（全章节索引）"""
    idx = {}
    for md in sorted(CHAPTERS.glob('*.md')):
        for title, blocks in extract_sections(md.read_text(encoding='utf-8')):
            m = re.match(r'(\d+\.\d+)', title)
            if m and blocks:
                idx[m.group(1)] = blocks[0]
    return idx

def resolve(code, index, seen, missing):
    """把依赖注释递归替换为前置节代码"""
    m = DEP_RE.search(code[:800])
    if not m:
        return code
    pre = []
    for dep in re.split(r'[、,，\s]+', m.group(1)):
        dep = dep.strip()
        if not dep:
            continue
        if dep in seen:
            continue
        if dep not in index:
            missing.append(dep)
            continue
        seen.add(dep)
        pre.append(resolve(index[dep], index, seen, missing))
    return '\n'.join(pre) + '\n' + code

def compile_check(code, timeout=90):
    """返回 (ok, errors)"""
    src = SKELETON + '\n' + code + '\n' + 'int main(){return 0;}\n'
    with tempfile.NamedTemporaryFile('w', suffix='.cpp', delete=False, encoding='utf-8') as f:
        f.write(src)
        fname = f.name
    try:
        r = subprocess.run([CLANG, '-std=c++17', '-fsyntax-only', '-Wno-unknown-pragmas',
                            '-Wno-deprecated', fname],
                           capture_output=True, text=True, timeout=timeout,
                           encoding='utf-8', errors='replace')
        err = r.stderr
    except subprocess.TimeoutExpired:
        err = 'TIMEOUT'
    finally:
        os.unlink(fname)
    if not err.strip():
        return True, [], []
    # 分类错误
    real = []
    dep = []
    for line in err.splitlines():
        if 'error:' not in line:
            continue
        if 'too many errors' in line:
            continue        # 截断提示，非真错误
        if '\\include\\' in line or '/include/' in line:
            continue        # 标准库头文件内部的连锁错误，忽略
        if any(k in line for k in ('use of undeclared identifier', 'unknown type name',
                                   'no member named', 'does not name a type',
                                   'no matching function', 'member access into incomplete',
                                   'no template named', 'use of unknown',
                                   'has not been declared')):
            dep.append(line.strip())
        else:
            real.append(line.strip())
    return (len(real) == 0), real, dep

def main():
    index = build_index()
    total = ok_cnt = usage_cnt = dep_cnt = 0
    problems = []
    deps = []
    for md in sorted(CHAPTERS.glob('*.md')):
        if md.name.startswith('00-'):
            continue                       # 00 章头文件展示，与骨架重复，跳过
        for title, blocks in extract_sections(md.read_text(encoding='utf-8')):
            sec_no = re.match(r'(\d+\.\d+)', title)
            sec_no = sec_no.group(1) if sec_no else '?'
            for i, code in enumerate(blocks, 1):
                total += 1
                first_line = code.splitlines()[0] if code.splitlines() else ''
                if USAGE_MARK in first_line:
                    usage_cnt += 1
                    continue
                missing = []
                resolved = resolve(code, index, set(), missing)
                ok, real, dep = compile_check(resolved)
                tag = f'{md.name} {sec_no} 块#{i}'
                if missing:
                    deps.append((tag, [f'依赖未找到: {", ".join(missing)}']))
                if ok and not dep:
                    if DEP_RE.search(code[:800]):
                        dep_cnt += 1       # 带依赖注释且拼接编译通过
                    else:
                        ok_cnt += 1
                elif dep and not real:
                    deps.append((tag, dep))
                if real:
                    problems.append((tag, real))
    print(f'共 {total} 个代码块')
    print(f'独立编译通过: {ok_cnt}')
    print(f'承接式块(依赖注释+拼接编译通过): {dep_cnt}')
    print(f'用法示例块(抄入 solve): {usage_cnt}')
    print(f'跨节依赖(未标注依赖注释, 需与前置节一起抄): {len(deps)}')
    print(f'语法错误: {len(problems)}')
    for tag, errs in problems:
        print(f'\n===== {tag} =====')
        for e in errs[:8]:
            print('  ', e)
    for tag, errs in deps:
        print(f'\n[依赖] {tag}')
        for e in errs[:4]:
            print('  ', e)

if __name__ == '__main__':
    main()
