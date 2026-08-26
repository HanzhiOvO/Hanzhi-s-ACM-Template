# -*- coding: utf-8 -*-
"""
Hanzhi XCPC Template builder
A4 双栏打印版：自动合并 chapters/*.md -> dist/*.pdf
依赖：reportlab, pygments（Python 3.10+）
用法：python build.py
"""
from __future__ import annotations
import re, html, os, sys
from pathlib import Path
from pygments import lex
from pygments.lexers import CppLexer
from pygments.token import Token

from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER, TA_LEFT
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle
from reportlab.lib.units import mm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import (
    BaseDocTemplate, PageTemplate, Frame, Paragraph, Spacer, PageBreak,
    Table, TableStyle, NextPageTemplate, KeepTogether,
)
from reportlab.platypus.tableofcontents import TableOfContents

ROOT = Path(__file__).resolve().parent
CHAPTERS = ROOT / 'chapters'
OUT = ROOT / 'dist' / 'Hanzhi-XCPC-Template.pdf'
BOOK_TITLE = 'Hanzhi · XCPC 算法模板'
BOOK_SUB = 'Introduction · Pattern Recognition · API · Key Points'

# ---------------------------------------------------------------------------
# Fonts（Windows / Linux 双环境自动探测）
# ---------------------------------------------------------------------------
def _first_existing(candidates: list[str]) -> str:
    for p in candidates:
        if os.path.exists(p):
            return p
    raise FileNotFoundError('No CJK font found among: %s' % candidates)

FONT_TABLE = {
    'Song':   ['C:/Windows/Fonts/simsun.ttc', 'C:/Windows/Fonts/simsun.ttf',
               '/usr/share/fonts/opentype/noto/NotoSerifCJK-Regular.ttc',
               '/usr/share/fonts/truetype/arphic-gbsn00lp/gbsn00lp.ttf'],
    'Kai':    ['C:/Windows/Fonts/simkai.ttf',
               '/usr/share/fonts/opentype/noto/NotoSerifCJK-Bold.ttc',
               '/usr/share/fonts/truetype/arphic-gkai00mp/gkai00mp.ttf'],
    'Mono':   ['C:/Windows/Fonts/consola.ttf',
               '/usr/share/fonts/truetype/noto/NotoSansMono-Regular.ttf'],
    'MonoB':  ['C:/Windows/Fonts/consolab.ttf',
               '/usr/share/fonts/truetype/noto/NotoSansMono-SemiBold.ttf'],
}

def _register_font(name: str) -> None:
    path = _first_existing(FONT_TABLE[name])
    if path.lower().endswith('.ttc'):
        pdfmetrics.registerFont(TTFont(name, path, subfontIndex=0))
    else:
        pdfmetrics.registerFont(TTFont(name, path))

for _f in ('Song', 'Kai', 'Mono', 'MonoB'):
    _register_font(_f)

# ---------------------------------------------------------------------------
# Palette（蓝白主视觉 + 四块功能色）
# ---------------------------------------------------------------------------
BLACK   = colors.HexColor('#182235')
GREY    = colors.HexColor('#707784')
BLUE    = colors.HexColor('#1266F1')
BLUE_D  = colors.HexColor('#17365D')
BLUE_L  = colors.HexColor('#EAF3FF')
BLUE_P  = colors.HexColor('#F5F9FF')
GREEN_B = colors.HexColor('#2E6B4A')
GREEN_BG= colors.HexColor('#F0F9F4')
GREEN_BR= colors.HexColor('#9CC9B0')
ORANGE_B= colors.HexColor('#8A5A1E')
ORANGE_BG=colors.HexColor('#FFF6E9')
ORANGE_BR=colors.HexColor('#E2B97E')
PURPLE_B= colors.HexColor('#4F3E8A')
PURPLE_BG=colors.HexColor('#F3F0FB')
PURPLE_BR=colors.HexColor('#B8AEE0')

W, H = A4
GUTTER = 6.5 * mm
LM = RM = 13 * mm
TM = 16 * mm
BM = 14 * mm
COL_W = (W - LM - RM - GUTTER) / 2

# ---------------------------------------------------------------------------
# Styles
# ---------------------------------------------------------------------------
styles = {}
def st(name, **kw):
    styles[name] = ParagraphStyle(name, **kw)
    return styles[name]

st('chapter', fontName='Kai', fontSize=15.5, leading=20, textColor=BLUE_D,
  spaceBefore=2, spaceAfter=5)
st('section', fontName='Kai', fontSize=10.6, leading=13.5, textColor=BLACK,
  spaceBefore=4.5, spaceAfter=3)
st('body', fontName='Song', fontSize=7.9, leading=11.2, textColor=BLACK, spaceAfter=2.6)
st('bullet', fontName='Song', fontSize=7.8, leading=11.0, textColor=BLACK,
  leftIndent=4.2*mm, firstLineIndent=-2.4*mm, bulletIndent=0.6*mm, spaceAfter=1.2)
st('code', fontName='Mono', fontSize=6.25, leading=7.8, textColor=colors.HexColor('#111827'),
  backColor=colors.HexColor('#F3F5F7'), leftIndent=1.2*mm, rightIndent=0.8*mm,
  borderPadding=(1.2, 2, 1.2, 2), spaceBefore=0.6, spaceAfter=0.2)
st('block_label', fontName='Kai', fontSize=8.6, leading=11, spaceBefore=2.8, spaceAfter=2,
  borderWidth=0.5, borderPadding=(1.8, 3, 1.8, 3), leftIndent=0.8*mm, rightIndent=0.8*mm)
st('toc0', fontName='Kai', fontSize=10.5, leading=14, textColor=BLUE, spaceBefore=3)
st('toc1', fontName='Song', fontSize=8.2, leading=11, textColor=BLACK, leftIndent=5*mm)
st('tbl', parent=styles['body'], fontSize=6.9, leading=9.2)

CODE_COLORS = {
    'comment': '#5B7B58', 'keyword': '#2359A8', 'type': '#2359A8',
    'function': '#6E3F95', 'class': '#1F6A70', 'string': '#9D3C36',
    'number': '#8A5519', 'operator': '#38566F', 'preproc': '#496A55',
}

def _tok_style(tt):
    if tt in Token.Comment or tt in Token.Comment.Preproc: return ('comment', False)
    if tt in Token.Keyword.Type: return ('type', True)
    if tt in Token.Keyword: return ('keyword', True)
    if tt in Token.Name.Function: return ('function', True)
    if tt in Token.Name.Class or tt in Token.Name.Namespace: return ('class', True)
    if tt in Token.Literal.String: return ('string', False)
    if tt in Token.Literal.Number: return ('number', False)
    if tt in Token.Operator: return ('operator', False)
    return (None, False)

def strip_md(s: str) -> str:
    s = re.sub(r'`([^`]*)`', r'\1', s)
    s = re.sub(r'\*\*([^*]+)\*\*', r'\1', s)
    return s.strip()

def inline(s: str) -> str:
    """Markdown 行内 -> reportlab markup"""
    s = html.escape(s, quote=False)
    def repl_code(m):
        text = m.group(1)
        font = 'Song' if re.search(r'[\u3400-\u9fff]', text) else 'Mono'
        return f'<font name="{font}" size="7.1" color="#334155">{text}</font>'
    s = re.sub(r'`([^`]+)`', repl_code, s)
    s = re.sub(r'\*\*([^*]+)\*\*', r'<b>\1</b>', s)
    s = re.sub(r'\[([^\]]+)\]\([^)]+\)', r'\1', s)
    return s

def _code_markup_line(raw: str) -> str:
    parts = []
    for tt, text in lex(raw, CppLexer()):
        text = text.rstrip('\n')
        if not text:
            continue
        key, bold = _tok_style(tt)
        esc = html.escape(text, quote=False).replace(' ', '&nbsp;').replace('\t', '&nbsp;&nbsp;&nbsp;&nbsp;')
        font = 'Song' if re.search(r'[\u3400-\u9fff]', text) else ('MonoB' if bold else 'Mono')
        attrs = [f'name="{font}"']
        if key:
            attrs.append(f'color="{CODE_COLORS[key]}"')
        parts.append('<font ' + ' '.join(attrs) + '>' + esc + '</font>')
    return ''.join(parts) or '&nbsp;'

def code_flowables(code: str):
    out = [Spacer(1, 1.0)]
    for raw in code.splitlines() or ['']:
        out.append(Paragraph(_code_markup_line(raw), styles['code']))
    out.append(Spacer(1, 1.6))
    return out

# 四块功能色映射：**标签** -> (文字色, 底色, 边框色)
BLOCKS = {
    '简介':     (BLUE_D,   BLUE_P,   colors.HexColor('#A9C9F8')),
    '快速识别': (GREEN_B,  GREEN_BG, GREEN_BR),
    'API 接入': (ORANGE_B, ORANGE_BG, ORANGE_BR),
    '关键点':   (PURPLE_B, PURPLE_BG, PURPLE_BR),
}

class HandbookDoc(BaseDocTemplate):
    def __init__(self, filename, **kw):
        super().__init__(filename, **kw)
        self.current_chapter = '目录'
        left = Frame(LM, BM, COL_W, H - TM - BM, id='left')
        right = Frame(LM + COL_W + GUTTER, BM, COL_W, H - TM - BM, id='right')
        self.addPageTemplates(PageTemplate(id='cover', frames=[Frame(LM, BM, W-LM-RM, H-TM-BM, id='c')],
                                           onPage=self.draw_cover))
        self.addPageTemplates(PageTemplate(id='toc', frames=[Frame(LM, BM, W-LM-RM, H-TM-BM, id='t')],
                                           onPage=self.draw_header_footer))
        self.addPageTemplates(PageTemplate(id='main', frames=[left, right],
                                           onPage=self.draw_header_footer))

    def draw_cover(self, canv, doc):
        canv.saveState()
        W, H = doc.pagesize
        BERLIN = colors.HexColor('#0057A6')      # Berlin 主题蓝
        LIGHT = colors.HexColor('#C9DCF5')       # 浅蓝细线
        DARK = colors.HexColor('#2B3A55')        # 正文深灰蓝

        # 纯白背景
        canv.setFillColor(colors.white)
        canv.rect(0, 0, W, H, stroke=0, fill=1)

        # ---- 顶部细条（Berlin headline：主条 + 次线） ----
        canv.setFillColor(BERLIN)
        canv.rect(0, H - 4.2 * mm, W, 4.2 * mm, stroke=0, fill=1)
        canv.setFillColor(LIGHT)
        canv.rect(0, H - 4.9 * mm, W, 0.7 * mm, stroke=0, fill=1)

        # ---- 标题区（垂直偏上，居中） ----
        canv.setFillColor(GREY)
        canv.setFont('Song', 9.5)
        canv.drawCentredString(W / 2, H * 0.735, 'ALGORITHM TEMPLATE  ·  XCPC 2026')

        canv.setFillColor(BERLIN)
        canv.setFont('Kai', 36)
        canv.drawCentredString(W / 2, H * 0.66, 'Hanzhi · XCPC 算法模板')

        canv.setStrokeColor(LIGHT)
        canv.setLineWidth(0.7)
        canv.line(W * 0.32, H * 0.605, W * 0.68, H * 0.605)

        canv.setFillColor(GREY)
        canv.setFont('Song', 11)
        canv.drawCentredString(W / 2, H * 0.565, '简介 · 快速识别 · API 接入 · 关键点')

        # ---- 四个极简标签（小色块 + 文字，克制排列） ----
        labels = [('简介', '#1266F1'), ('快速识别', '#2E7D32'),
                  ('API 接入', '#E65100'), ('关键点', '#6A4FA3')]
        step = 34 * mm
        total = step * 4
        x0 = (W - total) / 2 + step / 2
        y = H * 0.475
        for i, (txt, col) in enumerate(labels):
            cx = x0 + i * step
            canv.setFillColor(colors.HexColor(col))
            canv.rect(cx - 6 * mm, y - 1.5 * mm, 3 * mm, 3.0 * mm, stroke=0, fill=1)
            canv.setFillColor(DARK)
            canv.setFont('Song', 10.5)
            canv.drawCentredString(cx + 3.6 * mm, y - 0.7 * mm, txt)

        # 中间与下方大面积留白（Berlin 的克制）

        # ---- 底部细条（Berlin footline：次线 + 主条） ----
        canv.setFillColor(LIGHT)
        canv.rect(0, 4.9 * mm, W, 0.7 * mm, stroke=0, fill=1)
        canv.setFillColor(BERLIN)
        canv.rect(0, 0, W, 4.2 * mm, stroke=0, fill=1)
        canv.setFillColor(colors.white)
        canv.setFont('Song', 8)
        canv.drawString(13 * mm, 1.6 * mm, 'Hanzhi')
        canv.drawRightString(W - 13 * mm, 1.6 * mm, 'XCPC 2026  ·  A4 双面打印')
        canv.restoreState()

    def draw_header_footer(self, canv, doc):
        if doc.page == 1:
            return
        canv.saveState()
        canv.setFont('Song', 6.8)
        canv.setFillColor(GREY)
        canv.drawString(LM, H-9.5*mm, f'Hanzhi  |  {self.current_chapter}')
        canv.setStrokeColor(colors.HexColor('#A9C9F8'))
        canv.setLineWidth(0.5)
        canv.line(LM, H-11.2*mm, W-RM, H-11.2*mm)
        canv.drawCentredString(W/2, 8.2*mm, f'第 {doc.page} 页')
        canv.restoreState()

    def afterFlowable(self, flowable):
        if isinstance(flowable, HeadingFlowable):
            txt = flowable.plain
            level = flowable.level
            key = f'h{level}-{abs(hash((txt, self.page)))}'
            self.canv.bookmarkPage(key)
            self.canv.addOutlineEntry(txt, key, level=max(0, level), closed=False)
            self.notify('TOCEntry', (level, txt, self.page, key))
            if level == 0:
                self.current_chapter = txt

class HeadingFlowable(Paragraph):
    def __init__(self, text, style, level):
        self.plain = strip_md(text)
        self.level = level
        super().__init__(inline(text), style)

def chapter_flowables(title: str):
    p = HeadingFlowable(title, styles['chapter'], 0)
    bar = Table([['']], colWidths=[COL_W], rowHeights=[0.7],
                style=[('BACKGROUND', (0,0), (-1,-1), BLUE)])
    return [p, bar, Spacer(1, 3)]

def section_bar(title: str):
    marker = HeadingFlowable(title, ParagraphStyle('marker', fontName='Song', fontSize=0.1,
                                                   leading=0.1, textColor=colors.white,
                                                   spaceAfter=0, spaceBefore=0), 1)
    p = Paragraph(inline(title), styles['section'])
    t = Table([[p]], colWidths=[COL_W], hAlign='LEFT')
    t.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), colors.HexColor('#E8F2FF')),
        ('LEFTPADDING', (0,0), (-1,-1), 3.5), ('RIGHTPADDING', (0,0), (-1,-1), 3.5),
        ('TOPPADDING', (0,0), (-1,-1), 2.2), ('BOTTOMPADDING', (0,0), (-1,-1), 2.2),
        ('LINEBELOW', (0,0), (-1,-1), 0.4, colors.HexColor('#BFD8FA')),
    ]))
    return [marker, t]

def block_flowables(tag: str, content_lines: list[str]):
    fg, bg, br = BLOCKS.get(tag, (BLUE_D, BLUE_P, '#A9C9F8'))
    label = Paragraph(inline(f'【{tag}】'), ParagraphStyle(
        f'blk_{tag}', parent=styles['block_label'], textColor=fg, backColor=bg,
        borderColor=br))
    out = [label]
    for x in content_lines:
        if x.startswith('- '):
            x = x[2:]
            out.append(Paragraph('• ' + inline(x), styles['bullet']))
        elif x.startswith('* '):
            x = x[2:]
            out.append(Paragraph('• ' + inline(x), styles['bullet']))
        else:
            out.append(Paragraph(inline(x), styles['body']))
    out.append(Spacer(1, 1.6))
    return out

def parse_table(block_lines):
    rows = []
    for ln in block_lines:
        vals = [v.strip() for v in ln.strip().strip('|').split('|')]
        rows.append(vals)
    if len(rows) >= 2 and all(re.fullmatch(r':?-{3,}:?', x.replace(' ', '')) for x in rows[1]):
        rows.pop(1)
    maxc = max(len(r) for r in rows)
    data = []
    for r in rows:
        rr = []
        for c in r + [''] * (maxc - len(r)):
            rr.append(Paragraph(inline(c), styles['tbl']))
        data.append(rr)
    # 列宽固定等分（保持稳定排版；长 cell 靠 Paragraph 空格断行）
    t = Table(data, colWidths=[COL_W / maxc] * maxc, repeatRows=1, hAlign='LEFT')
    cmds = [('GRID', (0,0), (-1,-1), 0.25, colors.HexColor('#D7DEE8')),
            ('VALIGN', (0,0), (-1,-1), 'TOP'),
            ('LEFTPADDING', (0,0), (-1,-1), 2.5), ('RIGHTPADDING', (0,0), (-1,-1), 2.5),
            ('TOPPADDING', (0,0), (-1,-1), 1.6), ('BOTTOMPADDING', (0,0), (-1,-1), 1.6)]
    if data:
        cmds.append(('BACKGROUND', (0,0), (-1,0), BLUE_L))
    t.setStyle(TableStyle(cmds))
    return [t, Spacer(1, 2.5)]

def build_story(md: str):
    story = []
    # 封面（单栏）
    story.append(Spacer(1, 1))
    story.append(NextPageTemplate('toc'))
    story.append(PageBreak())
    # 目录（单栏）
    story.append(Paragraph('目录', ParagraphStyle('tocTitle', fontName='Kai',
        fontSize=17, leading=22, textColor=BLACK, spaceAfter=7)))
    toc = TableOfContents()
    toc.levelStyles = [styles['toc0'], styles['toc1']]
    toc.dotsMinLevel = 0
    story.append(toc)
    story.append(NextPageTemplate('main'))
    story.append(PageBreak())

    lines = md.splitlines()
    i = 0
    para = []
    def flush_para():
        nonlocal para
        if para:
            txt = ' '.join(x.strip() for x in para).strip()
            if txt:
                story.append(Paragraph(inline(txt), styles['body']))
            para = []
    while i < len(lines):
        line = lines[i]
        if line.startswith('```'):
            flush_para()
            buf = []
            i += 1
            while i < len(lines) and not lines[i].startswith('```'):
                buf.append(lines[i])
                i += 1
            story.extend(code_flowables('\n'.join(buf)))
        elif line.startswith('## '):
            flush_para()
            story.extend(chapter_flowables(line[3:].strip()))
        elif line.startswith('### '):
            flush_para()
            story.extend(section_bar(line[4:].strip()))
        elif line.startswith('>'):
            flush_para()
            buf = []
            while i < len(lines) and (lines[i].startswith('>') or not lines[i].strip()):
                if lines[i].startswith('>'):
                    buf.append(lines[i])
                i += 1
            i -= 1
            # 块结构：> **标签**：首行内容，后续 > 行作为块的补充内容
            # 标签行与后续内容之间允许空行分隔，块内空行结束块。
            first = buf[0][1:].strip() if buf else ''
            m = re.match(r'\*\*(简介|快速识别|API 接入|关键点)\*\*[：:]\s*(.*)', first, re.S)
            if m:
                tag, rest = m.group(1), m.group(2)
                # 收集后续非空内容行（同一块的剩余行）
                content_lines = []
                if rest.strip():
                    content_lines.append(rest)
                for bl in buf[1:]:
                    x = bl[1:].strip()
                    if x:
                        content_lines.append(x)
                if content_lines:
                    story.extend(block_flowables(tag, content_lines))
            else:
                for bl in buf:
                    x = bl[1:].strip()
                    if not x:
                        continue
                    story.append(Paragraph(inline(x), styles['bullet']))
        elif line.strip().startswith('|') and i + 1 < len(lines) and lines[i+1].strip().startswith('|'):
            flush_para()
            buf = []
            while i < len(lines) and lines[i].strip().startswith('|'):
                buf.append(lines[i])
                i += 1
            i -= 1
            story.extend(parse_table(buf))
        elif re.match(r'^\s*[-*] ', line):
            flush_para()
            txt = re.sub(r'^\s*[-*] ', '', line)
            story.append(Paragraph('• ' + inline(txt), styles['bullet']))
        elif re.match(r'^\s*\d+\. ', line):
            flush_para()
            story.append(Paragraph(inline(line.strip()), styles['bullet']))
        elif not line.strip():
            flush_para()
            story.append(Spacer(1, 1.4))
        else:
            para.append(line)
        i += 1
    flush_para()
    return story

def main():
    files = sorted(CHAPTERS.glob('*.md'))
    if not files:
        print('chapters/ 下没有 .md 文件')
        sys.exit(1)
    md = '\n\n'.join(f.read_text(encoding='utf-8') for f in files)
    story = build_story(md)
    doc = HandbookDoc(str(OUT), pagesize=A4, leftMargin=LM, rightMargin=RM,
                      topMargin=TM, bottomMargin=BM, title=BOOK_TITLE)
    doc.multiBuild(story)
    print(f'OK -> {OUT}')

if __name__ == '__main__':
    main()
