from __future__ import annotations
import re, html, os, sys
from pathlib import Path
from pygments import lex
from pygments.lexers import CppLexer
from pygments.token import Token

from reportlab.lib import colors
from reportlab.lib.enums import TA_LEFT, TA_CENTER
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import mm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import (
    BaseDocTemplate, PageTemplate, Frame, Paragraph, Spacer, PageBreak,
    Table, TableStyle, XPreformatted, KeepTogether
)
from reportlab.platypus.tableofcontents import TableOfContents
from pdfrw import PdfReader
from pdfrw.buildxobj import pagexobj
from pdfrw.toreportlab import makerl

ROOT = Path(__file__).resolve().parent
IN = ROOT / '一只奶龙-XCPC-2026-算法模板-字体优化最终打印版.md'
OUT = ROOT / '一只奶龙-XCPC-2026-算法模板-字体优化最终打印版.pdf'
COVER = ROOT / 'assets' / 'cover-original.pdf'

# Fonts: use GB fonts with full CJK glyph coverage and a restrained contest-handbook look.
# 自动探测字体：优先 Linux 环境字体，找不到再退回 Windows 系统字体，保证双环境都能直接构建。
import os as _os

def _first_existing(candidates):
    for p in candidates:
        if _os.path.exists(p):
            return p
    raise FileNotFoundError(
        'No CJK font found among: %s' % candidates)

FONT_TABLE = {
    'GBSans': ['/usr/share/fonts/truetype/arphic-gbsn00lp/gbsn00lp.ttf',
               'C:/Windows/Fonts/simsun.ttc', 'C:/Windows/Fonts/simsun.ttf'],
    'GBKai': ['/usr/share/fonts/truetype/arphic-gkai00mp/gkai00mp.ttf',
              'C:/Windows/Fonts/simkai.ttf'],
    'NotoMono': ['/usr/share/fonts/truetype/noto/NotoSansMono-Regular.ttf',
                 'C:/Windows/Fonts/consola.ttf'],
    'NotoMonoSemi': ['/usr/share/fonts/truetype/noto/NotoSansMono-SemiBold.ttf',
                     'C:/Windows/Fonts/consolab.ttf'],
}


def _register_font(name):
    path = _first_existing(FONT_TABLE[name])
    if path.lower().endswith('.ttc'):
        pdfmetrics.registerFont(TTFont(name, path, subfontIndex=0))
    else:
        pdfmetrics.registerFont(TTFont(name, path))


_register_font('GBSans')
_register_font('GBKai')
_register_font('NotoMono')
_register_font('NotoMonoSemi')

BLUE = colors.HexColor('#1266F1')
BLUE_DARK = colors.HexColor('#17365D')
BLUE_LIGHT = colors.HexColor('#EAF3FF')
BLUE_PALE = colors.HexColor('#F5F9FF')
GREY = colors.HexColor('#707784')
LIGHT_GREY = colors.HexColor('#F5F6F8')
MID_GREY = colors.HexColor('#D9E1EA')
BLACK = colors.HexColor('#182235')

W,H=A4
LM=16.5*mm; RM=16.5*mm; TM=17*mm; BM=16*mm

styles = {}
def st(name, **kw):
    styles[name]=ParagraphStyle(name, **kw); return styles[name]

st('body', fontName='GBSans', fontSize=9.55, leading=14.05, textColor=BLACK, spaceAfter=3.2)
st('small', fontName='GBSans', fontSize=7.65, leading=10.55, textColor=GREY, spaceAfter=2)
st('chapter', fontName='GBKai', fontSize=20.8, leading=27, textColor=BLACK, spaceBefore=3, spaceAfter=7)
st('subchapter', fontName='GBKai', fontSize=15.8, leading=21, textColor=BLUE_DARK, spaceBefore=9, spaceAfter=5)
st('section', fontName='GBKai', fontSize=12.9, leading=16.8, textColor=BLACK, spaceBefore=5, spaceAfter=4)
st('callout', fontName='GBSans', fontSize=8.95, leading=12.7, textColor=BLACK, leftIndent=1.5*mm, rightIndent=1.5*mm, spaceAfter=1)
st('callout_title', fontName='GBKai', fontSize=9.8, leading=13, textColor=BLUE_DARK, spaceAfter=2)
st('bullet', fontName='GBSans', fontSize=9.0, leading=12.8, textColor=BLACK, leftIndent=5*mm, firstLineIndent=-3*mm, bulletIndent=1*mm, spaceAfter=1.5)
st('code', fontName='NotoMono', fontSize=6.95, leading=8.65, textColor=colors.HexColor('#111827'), backColor=LIGHT_GREY,
   leftIndent=4*mm, rightIndent=4*mm, borderPadding=(0,3,0,3), spaceBefore=0, spaceAfter=0)
st('toc0', fontName='GBKai', fontSize=9.9, leading=13.5, textColor=BLUE, leftIndent=0, firstLineIndent=0, spaceBefore=3)
st('toc1', fontName='GBSans', fontSize=8.55, leading=11.5, textColor=BLACK, leftIndent=6*mm, firstLineIndent=0)
st('toc2', fontName='GBSans', fontSize=7.7, leading=10.2, textColor=BLACK, leftIndent=13*mm, firstLineIndent=0)
st('title_center', fontName='GBKai', fontSize=18.5, leading=24.5, alignment=TA_CENTER, textColor=BLACK, spaceAfter=7)
st('guide_h', fontName='GBKai', fontSize=12.8, leading=16.6, textColor=BLUE_DARK, spaceBefore=6, spaceAfter=4)

# original cover page as a vector XObject
cover_page = PdfReader(str(COVER)).pages[0]
cover_xobj = pagexobj(cover_page)


def strip_md(s: str) -> str:
    s=re.sub(r'`([^`]*)`', r'\1', s)
    s=re.sub(r'\*\*([^*]+)\*\*', r'\1', s)
    s=re.sub(r'\*([^*]+)\*', r'\1', s)
    return s.strip()


def inline(s: str) -> str:
    # Safe subset of Markdown -> reportlab paragraph markup.
    # Inline API / identifiers use a true mono font; Chinese snippets stay on the CJK font.
    s=html.escape(s, quote=False)
    def repl_code(m):
        text=m.group(1)
        font='GBSans' if re.search(r'[\u3400-\u9fff]', text) else 'NotoMono'
        return f'<font name="{font}" size="8.45" color="#334155" backcolor="#F1F5F9">{text}</font>'
    s=re.sub(r'`([^`]+)`', repl_code, s)
    s=re.sub(r'\*\*([^*]+)\*\*', r'<b>\1</b>', s)
    # links: keep label, show URL only if raw
    s=re.sub(r'\[([^\]]+)\]\(([^)]+)\)', r'\1', s)
    return s

class HandbookDoc(BaseDocTemplate):
    def __init__(self, filename, **kw):
        super().__init__(filename, **kw)
        self.current_chapter='目录'
        frame=Frame(LM, BM, W-LM-RM, H-TM-BM, id='normal')
        self.addPageTemplates(PageTemplate(id='main', frames=frame, onPage=self.draw_cover, onPageEnd=self.draw_header_footer))
    def draw_cover(self, canv, doc):
        if doc.page == 1:
            canv.saveState()
            form=makerl(canv, cover_xobj)
            canv.doForm(form)
            canv.restoreState()

    def draw_header_footer(self, canv, doc):
        if doc.page == 1:
            return
        canv.saveState()
        header = '目录 / 使用说明' if doc.page <= 13 else self.current_chapter
        canv.setFont('GBSans', 7.5)
        canv.setFillColor(GREY)
        canv.drawString(LM, H-10.5*mm, f'一只奶龙  |  {header}')
        canv.setStrokeColor(colors.HexColor('#A9C9F8'))
        canv.setLineWidth(0.55)
        canv.line(LM, H-12.3*mm, W-RM, H-12.3*mm)
        canv.setFillColor(GREY)
        canv.setFont('GBSans', 7.3)
        canv.drawCentredString(W/2, 8.8*mm, f'第 {doc.page} 页')
        canv.restoreState()
    def afterFlowable(self, flowable):
        if isinstance(flowable, HeadingFlowable):
            txt=flowable.plain
            level=flowable.level
            key=f'h{level}-{abs(hash((txt,self.page)))}'
            self.canv.bookmarkPage(key)
            self.canv.addOutlineEntry(txt, key, level=max(0,level), closed=False)
            self.notify('TOCEntry', (level, txt, self.page, key))
            if level == 0:
                self.current_chapter=txt

class HeadingFlowable(Paragraph):
    def __init__(self, text, style, level):
        self.plain=strip_md(text); self.level=level
        super().__init__(inline(text), style)


def section_bar(text: str):
    p=HeadingFlowable(text, styles['section'], 2)
    t=Table([[p]], colWidths=[W-LM-RM], hAlign='LEFT')
    t.setStyle(TableStyle([
        ('BACKGROUND',(0,0),(-1,-1),colors.HexColor('#E8F2FF')),
        ('LEFTPADDING',(0,0),(-1,-1),4),('RIGHTPADDING',(0,0),(-1,-1),4),
        ('TOPPADDING',(0,0),(-1,-1),3),('BOTTOMPADDING',(0,0),(-1,-1),3),
        ('LINEBELOW',(0,0),(-1,-1),0.45,colors.HexColor('#BFD8FA')),
    ]))
    # Table itself isn't HeadingFlowable, so TOC handled by an invisible-ish heading before it.
    marker=HeadingFlowable(text, ParagraphStyle('marker',fontName='GBSans',fontSize=0.1,leading=0.1,textColor=colors.white,spaceAfter=0,spaceBefore=0),2)
    return [marker,t]


CODE_COLORS = {
    'comment':'#5B7B58', 'keyword':'#2359A8', 'type':'#2359A8', 'function':'#6E3F95',
    'class':'#1F6A70', 'string':'#9D3C36', 'number':'#8A5519', 'operator':'#38566F',
    'preproc':'#496A55'
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

def _code_markup_line(raw: str) -> str:
    # Pygments tokenization gives true C++ syntax highlighting. ASCII code stays monospace;
    # Chinese comments switch to the CJK font so no glyph becomes a square.
    parts=[]
    for tt, text in lex(raw, CppLexer()):
        text=text.rstrip('\n')
        if not text: continue
        key,bold=_tok_style(tt)
        esc=html.escape(text, quote=False).replace(' ', '&nbsp;').replace('\t','&nbsp;&nbsp;&nbsp;&nbsp;')
        font='GBSans' if re.search(r'[\u3400-\u9fff]', text) else ('NotoMonoSemi' if bold else 'NotoMono')
        attrs=[f'name="{font}"']
        if key: attrs.append(f'color="{CODE_COLORS[key]}"')
        parts.append('<font ' + ' '.join(attrs) + '>' + esc + '</font>')
    return ''.join(parts) or '&nbsp;'

def code_flowables(code: str):
    out=[Spacer(1,1.4)]
    for raw in code.splitlines() or ['']:
        out.append(Paragraph(_code_markup_line(raw), styles['code']))
    out.append(Spacer(1,2.1))
    return out


def callout_flowables(lines):
    out=[]
    for idx,line in enumerate(lines):
        x=line[1:].strip() if line.startswith('>') else line.strip()
        if not x: continue
        if x.startswith('**赛时先看**'):
            out.append(Paragraph('赛时先看', styles['callout_title']))
            continue
        if x.startswith('- '): x=x[2:]
        # make the leading bold label blue
        m=re.match(r'\*\*([^*]+)\*\*：(.*)', x)
        if m:
            body=f'<font color="#1266F1"><b>{html.escape(m.group(1))}</b></font>：{inline(m.group(2).strip())}'
        else:
            body=inline(x)
        out.append(Paragraph(body, styles['callout']))
    if out:
        # blue top/bottom separators, close to original visual language
        out=[Spacer(1,1.5)] + out + [Spacer(1,2.5)]
    return out


def parse_table(block_lines):
    rows=[]
    for ln in block_lines:
        vals=[v.strip() for v in ln.strip().strip('|').split('|')]
        rows.append(vals)
    if len(rows)>=2 and all(re.fullmatch(r':?-{3,}:?', x.replace(' ','')) for x in rows[1]):
        rows.pop(1)
    data=[]
    maxc=max(len(r) for r in rows)
    for ri,r in enumerate(rows):
        rr=[]
        for c in r + ['']*(maxc-len(r)):
            rr.append(Paragraph(inline(c), ParagraphStyle('tbl',parent=styles['small'],fontSize=7.2,leading=9.3,textColor=BLACK)))
        data.append(rr)
    widths=[(W-LM-RM)/maxc]*maxc
    t=Table(data,colWidths=widths,repeatRows=1,hAlign='LEFT')
    cmds=[('GRID',(0,0),(-1,-1),0.25,colors.HexColor('#D7DEE8')),('VALIGN',(0,0),(-1,-1),'TOP'),('LEFTPADDING',(0,0),(-1,-1),3),('RIGHTPADDING',(0,0),(-1,-1),3),('TOPPADDING',(0,0),(-1,-1),2),('BOTTOMPADDING',(0,0),(-1,-1),2)]
    if data: cmds.append(('BACKGROUND',(0,0),(-1,0),BLUE_LIGHT))
    t.setStyle(TableStyle(cmds))
    return [t,Spacer(1,4)]


def build_story(md: str):
    story=[]
    # page 1 is the vector original cover drawn by onPage
    story.append(Spacer(1,1))
    story.append(PageBreak())

    # TOC page(s), styled close to the original
    story.append(Paragraph('目录', ParagraphStyle('tocTitle',fontName='GBKai',fontSize=20,leading=25,textColor=BLACK,spaceAfter=8)))
    toc=TableOfContents()
    toc.levelStyles=[styles['toc0'],styles['toc1'],styles['toc2']]
    toc.dotsMinLevel=0
    story.append(toc)
    story.append(PageBreak())

    # Final print edition: the Markdown itself carries the audited front-matter.
    # Render it before the main sections instead of inventing another competing usage guide.

    lines=md.splitlines()
    # Start immediately after the book H1; cover already contains the title.
    start=1 if lines and lines[0].startswith('# ') else 0
    i=start
    para=[]
    def flush_para():
        nonlocal para
        if para:
            txt=' '.join(x.strip() for x in para).strip()
            if txt:
                story.append(Paragraph(inline(txt), styles['body']))
            para=[]
    while i<len(lines):
        line=lines[i]
        if line.startswith('```'):
            flush_para(); lang=line[3:].strip(); buf=[]; i+=1
            while i<len(lines) and not lines[i].startswith('```'):
                buf.append(lines[i]); i+=1
            story.extend(code_flowables('\n'.join(buf)))
        elif line.startswith('## '):
            flush_para(); title=line[3:].strip()
            story.append(HeadingFlowable(title, styles['chapter'], 0))
            story.append(Table([['']],colWidths=[W-LM-RM],rowHeights=[0.6],style=[('BACKGROUND',(0,0),(-1,-1),BLUE)]))
            story.append(Spacer(1,4))
        elif line.startswith('### '):
            flush_para(); title=line[4:].strip(); story.append(HeadingFlowable(title, styles['subchapter'], 1))
        elif line.startswith('#### '):
            flush_para(); title=line[5:].strip(); story.extend(section_bar(title))
        elif line.startswith('>'):
            flush_para(); buf=[]
            while i<len(lines) and (lines[i].startswith('>') or not lines[i].strip()):
                if lines[i].startswith('>'): buf.append(lines[i])
                i+=1
            i-=1; story.extend(callout_flowables(buf))
        elif line.strip().startswith('|') and i+1<len(lines) and lines[i+1].strip().startswith('|'):
            flush_para(); buf=[]
            while i<len(lines) and lines[i].strip().startswith('|'):
                buf.append(lines[i]); i+=1
            i-=1; story.extend(parse_table(buf))
        elif re.match(r'^\s*[-*] ', line):
            flush_para(); txt=re.sub(r'^\s*[-*] ','',line)
            story.append(Paragraph('• '+inline(txt), styles['bullet']))
        elif re.match(r'^\s*\d+\. ', line):
            flush_para(); txt=line.strip()
            story.append(Paragraph(inline(txt), styles['bullet']))
        elif re.match(r'^\*\*.+\*\*$', line.strip()):
            flush_para(); title=line.strip()[2:-2].rstrip('：: ')
            if any(k in title for k in ['不会用','最小调用','最小接法','API / 入口函数','核心逻辑','改板时先认','抄板清单','改造点','维护的量','最小完整示例','传参要求']):
                bg=BLUE_PALE; border=colors.HexColor('#A9C9F8'); fg=BLUE_DARK
                if '核心逻辑' in title: bg=colors.HexColor('#F8FBF5'); border=colors.HexColor('#B7C99D'); fg=colors.HexColor('#385A2A')
                if '改板时先认' in title: bg=colors.HexColor('#FFFDF6'); border=colors.HexColor('#DDCC93'); fg=colors.HexColor('#75531A')
                if '抄板清单' in title: bg=colors.HexColor('#F0F9F4'); border=colors.HexColor('#9CC9B0'); fg=colors.HexColor('#2E6B4A')
                if '改造点' in title: bg=colors.HexColor('#FFF6E9'); border=colors.HexColor('#E2B97E'); fg=colors.HexColor('#8A5A1E')
                if '维护的量' in title: bg=colors.HexColor('#F3F0FB'); border=colors.HexColor('#B8AEE0'); fg=colors.HexColor('#4F3E8A')
                if '最小完整示例' in title: bg=colors.HexColor('#FFF1F0'); border=colors.HexColor('#E8A3A3'); fg=colors.HexColor('#8A2B26')
                if '传参要求' in title: bg=colors.HexColor('#F0F7FA'); border=colors.HexColor('#A3C9DE'); fg=colors.HexColor('#1F5A7A')
                story.append(Paragraph(inline('【'+title+'】'), ParagraphStyle('microTitle'+str(i),parent=styles['guide_h'],fontSize=9.8,leading=13,textColor=fg,backColor=bg,borderColor=border,borderWidth=0.45,borderPadding=3.5,spaceBefore=3.5,spaceAfter=3)))
            else:
                story.append(Paragraph(inline(title), styles['guide_h']))
        elif not line.strip():
            flush_para(); story.append(Spacer(1,2))
        else:
            para.append(line)
        i+=1
    flush_para()
    return story

md=IN.read_text(encoding='utf8')
story=build_story(md)
doc=HandbookDoc(str(OUT), pagesize=A4, leftMargin=LM, rightMargin=RM, topMargin=TM, bottomMargin=BM,
                title='一只奶龙', author='寒枝OvO', subject='XCPC 2026 算法模板')
doc.multiBuild(story)
print(OUT)
