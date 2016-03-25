// 24pointsView.cpp : C24pointsView 类的实现
//

#include "stdafx.h"
#include "24points.h"

#include "24pointsDoc.h"
#include "24pointsView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C24pointsView

IMPLEMENT_DYNCREATE(C24pointsView, CView)

BEGIN_MESSAGE_MAP(C24pointsView, CView)
	ON_WM_TIMER() // OnTimer
	ON_COMMAND(ID_FILE_OPEN, &C24pointsView::OnDefine)
	ON_COMMAND(ID_SOLVER, &C24pointsView::Solver)
	ON_COMMAND(ID_STATICS, &C24pointsView::ShowStatics)
	ON_BN_CLICKED(IDC_BUTTON, &C24pointsView::OnButton)
	ON_BN_CLICKED(IDC_NOSOL, &C24pointsView::OnNoSol)
END_MESSAGE_MAP()

// C24pointsView 构造/析构

C24pointsView::C24pointsView()
{
	pEdit = NULL;
	pButton = NULL;
	pNoSol = NULL;
}

C24pointsView::~C24pointsView()
{
	if (pEdit)
	{
		delete pEdit;
	}
	if (pButton)
	{
		delete pButton;
	}
	if (pNoSol)
	{
		delete pNoSol;
	}
}

BOOL C24pointsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// C24pointsView 绘制

void C24pointsView::OnDraw(CDC* pDC)
{
	C24pointsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC m;
	m.CreateCompatibleDC(pDC);
	CRect rect;
	GetClientRect(rect);
	CBrush brush(RGB(0, 128, 0));
	pDC->FillRect(rect, &brush);
	
	CBitmap bitmap;
	for (int i = 0; i < 4; ++i)
	{
		bitmap.LoadBitmap(IDB_BITMAP1 + pDoc->cards[i]);
		m.SelectObject(bitmap);
		pDC->BitBlt(20 + i * 100, 20, rect.Width(), rect.Height(), &m, 0, 0, SRCCOPY);
		bitmap.DeleteObject();
	}

}

#ifdef _DEBUG
C24pointsDoc* C24pointsView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C24pointsDoc)));
	return (C24pointsDoc*)m_pDocument;
}
#endif //_DEBUG


// C24pointsView 消息处理程序
// 创造编辑框和按钮
void C24pointsView::OnInitialUpdate(void)
{
	CView::OnInitialUpdate();

	if (pEdit)
	{
		delete pEdit;
		pEdit = NULL;
	}
	pEdit = new CEdit;
	pEdit->CreateEx(
		WS_EX_CLIENTEDGE,
		_T("Edit"),
		_T(""),
		WS_CHILD|WS_VISIBLE|ES_LEFT,
		CRect(20,300,400,320),
		this,
		IDC_EDIT);
	pEdit->ShowWindow(SW_SHOW);
	pButton = new CButton;
	pButton->CreateEx(
		0,
		_T("Button"),
		_T("答题"),
		WS_CHILD|WS_VISIBLE,
		CRect (402, 300, 482, 320),
		this,
		IDC_BUTTON);
	pButton->ShowWindow(SW_SHOW);
	pNoSol = new CButton;
	pNoSol->CreateEx(
		0,
		_T("Button"),
		_T("无解"),
		WS_CHILD|WS_VISIBLE,
		CRect (484, 300, 564, 320),
		this,
		IDC_NOSOL);
	pNoSol->ShowWindow(SW_SHOW);
	
	KillTimer(1);
	GetDocument()->start_timer();
	SetTimer(1, 1000, NULL);
}


void C24pointsView::OnButton(void)
{
	static int hits = 0;
	TCHAR s[1024];
	GetDlgItemText(IDC_EDIT, s, 1024);

	int ncards = 4;
	CString proc_s = s;
	proc_s.Replace(_T("（"), _T("("));
	proc_s.Replace(_T("）"), _T(")"));
	proc_s.Replace(_T("x"), _T("*"));
	proc_s.Replace(_T("＋"), _T("+"));
	proc_s.Replace(_T("－"), _T("-"));
	proc_s.Replace(_T("×"), _T("*"));
	proc_s.Replace(_T("÷"), _T("/"));
	double n = GetDocument()->calc_exp(proc_s.GetBuffer());
	GetDocument()->done(false);                    // 先计入总局数
	if (n == -999999)
	{
		MessageBox(_T("表达式中包含未知符号！"));
	}
	else if (n == -999998)
	{
		MessageBox(_T("24点游戏只有4张牌！"));
	}
	else if (n == -999997)
	{
		MessageBox(_T("扑克牌的大小只能是1到13"));
	}
	else if (n == -999996)
	{	
		MessageBox(_T("使用了你没有的牌"));
	}
	else if (n == -999995)
	{	
		MessageBox(_T("括号不匹配！"));
	}
	else if (GetDocument()->CountTotalLeft > 0)
	{
		MessageBox(_T("还有牌没用完！"));
	}
	else
	{
		MessageBox( fabs(n - 24) < 0.00001 ? _T("恭喜你，答对了！") : _T("你错了！"));
		GetDocument()->done( fabs(n - 24) < 0.00001 );
	}
	GetDocument()->recount();
		
	/*
	if (n == 24)
		++hits;
	else
		hits = 0;
	switch (hits)
	{
	case 2:
		MessageBox(_T("Double Kill!"));
		break;
	case 5:
		MessageBox(_T("恭喜你连对5题！神一般的杀戮！"));
		break;
	case 10:
		MessageBox(_T("恭喜你连对10题！超神！"));
		break;
	} 
	*/
#if 0
	for (int i = 0; s[i]; ++i)
	{
		if (!(s[i] >= '0' && s[i] <= '9' ||
			s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' ||
			s[i] == '(' || s[i] == ')'))
		{
			MessageBox(_T("你输入的表达式不合法！"));
			return;
		}
	}
	if (rand() % 2)
	{
		MessageBox(_T("恭喜你，答对了！"));
	}
	else
	{
		MessageBox(_T("你错了！"));
	}
#endif

}

void C24pointsView::OnNoSol(void)
{
	MessageBox( !GetDocument()->hasSolution() ? _T("恭喜你，答对了！") : _T("你错了！"));
	GetDocument()->done( !GetDocument()->hasSolution() );
}

class CDefineDlg : public CDialog
{
public:
	CDefineDlg(C24pointsView* parent);
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	void OnComboChange();

	int num[4];
	C24pointsView* m_pView;

protected:
	DECLARE_MESSAGE_MAP()
};

CDefineDlg::CDefineDlg(C24pointsView* parent)
	: CDialog(CDefineDlg::IDD)
	, m_pView(parent)
{
	memset(num, 0, sizeof(num));
}

void CDefineDlg::OnOK()
{
	for (int i = 0; i < 4; ++i)
	{
		if (num[i] < 1 || num[i] > 13)
		{
			MessageBox("请选择4张牌！");
			return;
		}
	}
	m_pView->GetDocument()->setCards(num);
	m_pView->GetDocument()->countOnTotal = false;   // 指定不计入总局数
	m_pView->GetDocument()->countOnSuccess = false; // 不计入分数
	m_pView->Invalidate();
	m_pView->GetDocument()->start_timer();          // 重新启动计数器
	CDialog::OnOK();
}

void CDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	for (int i = 1; i < 4; ++i)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(0, _T("A"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(1, _T("2"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(2, _T("3"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(3, _T("4"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(4, _T("5"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(5, _T("6"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(6, _T("7"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(7, _T("8"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(8, _T("9"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(9, _T("10"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(10, _T("J"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(11, _T("Q"));
		((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->InsertString(12, _T("K"));
	}
}

BEGIN_MESSAGE_MAP(CDefineDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDefineDlg::OnComboChange)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDefineDlg::OnComboChange)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDefineDlg::OnComboChange)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDefineDlg::OnComboChange)
	ON_CBN_EDITUPDATE(IDC_COMBO1, &CDefineDlg::OnComboChange)
	ON_CBN_EDITUPDATE(IDC_COMBO2, &CDefineDlg::OnComboChange)
	ON_CBN_EDITUPDATE(IDC_COMBO3, &CDefineDlg::OnComboChange)
	ON_CBN_EDITUPDATE(IDC_COMBO4, &CDefineDlg::OnComboChange)
END_MESSAGE_MAP()

void CDefineDlg::OnComboChange()
{
	CString C;
	for (int i = 0; i < 4; ++i)
	{
		CComboBox* c = ((CComboBox*)GetDlgItem(IDC_COMBO1 + i));
	    int j = c->GetCurSel();
		if (j < 0 || j > 12)
		{
			num[i] = 0;
			continue;
		}
		c->GetLBText(j, C);
		//GetDlgItemText(IDC_COMBO1 + i, C, 2);
		if (C.GetLength() == 1)
		{
			if (C[0] == 'A')
				num[i] = 1;
			else if (C[0] >= '2' && C[0] <= '9')
				num[i] = C[0] - '2' + 2;
			else if (C[0] == 'J')
				num[i] = 11;
			else if (C[0] == 'Q')
				num[i] = 12;
			else if (C[0] == 'K')
				num[i] = 13;
		}
		else if (C == _T("10"))
			num[i] = 10;
		else
			num[i] = 0;
	}
	/*
	CString t;
	t.Format("%d %d %d %d", num[0], num[1], num[2], num[3]);
	MessageBox(t.GetBuffer());
	*/
}

void C24pointsView::OnDefine()
{
	CDefineDlg dlg(this);
	dlg.DoModal();
}


class CSolverDlg : public CDialog
{
public:
	CSolverDlg(CString& sol);
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CString& s;

protected:
	DECLARE_MESSAGE_MAP()
};

CSolverDlg::CSolverDlg(CString& sol)
	: CDialog(CSolverDlg::IDD)
	, s(sol)
{
}

void CSolverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	SetDlgItemText(IDC_EDIT1, s);
}

BEGIN_MESSAGE_MAP(CSolverDlg, CDialog)
END_MESSAGE_MAP()


void C24pointsView::Solver(void)
{
	CString sol;
	int n = GetDocument()->Solver(sol);
	GetDocument()->countOnSuccess = false; // 看答案不计分
	//GetDocument()->incrementTotal();       // 计入总局数
	GetDocument()->done(false);
	if (n > 0)
	{
		//MessageBox(sol.GetBuffer());
		CSolverDlg dlg(sol);
		dlg.DoModal();
	}
	else
	{
		MessageBox(_T("此题无解！"));
	}
}

void C24pointsView::OnTimer(UINT nIDEvent)
{
	CTime t( GetDocument()->time_passed() );
    CString s = t.FormatGmt("%H:%M:%S");
	CMainFrame* pFrm=(CMainFrame*)GetParent();
	pFrm->m_wndStatusBar.SetPaneText(pFrm->m_wndStatusBar.CommandToIndex(ID_TIMER), s, TRUE);  
 
    CView::OnTimer(nIDEvent);  
}


class CStaticsDlg : public CDialog
{
public:
	CStaticsDlg(CString& text);
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CString& s;

protected:
	DECLARE_MESSAGE_MAP()
};

CStaticsDlg::CStaticsDlg(CString& text)
	: CDialog(CStaticsDlg::IDD)
	, s(text)
{
}

void CStaticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	SetDlgItemText(IDC_EDIT1, s);
}

BEGIN_MESSAGE_MAP(CStaticsDlg, CDialog)
END_MESSAGE_MAP()


void C24pointsView::ShowStatics(void)
{
	CString s;
	s.Format(
		_T("总局数：%d\t胜利局数：%d\t胜率：%.2f％\r\n")
		_T("总耗时：%llds\t平均耗时：%.2lfs\r\n")
		_T("\r\n")
		_T("说明：\r\n")
		_T("1.耗时只算胜利的局次。\r\n")
		_T("2.同一局反复提交正确答案（即使多解）也只算一次胜利。\r\n")
		_T("3.指定局不计入统计。\r\n")
		_T("4.查看答案视为失败。查看后提交不计入胜利。\r\n")
		_T("5.只有点击了“答题”，“无解”或“查看所有解”才会被计入总局数。\r\n")
		,
		GetDocument()->countTotal(),
		GetDocument()->countSuccess(),
		GetDocument()->countTotal() > 0 ? (float)GetDocument()->countSuccess() / GetDocument()->countTotal() * 100 : 0,
		GetDocument()->countTotalTime(),
		GetDocument()->countSuccess() > 0 ? ((double)GetDocument()->countTotalTime()) / GetDocument()->countSuccess() : 0);
    CStaticsDlg dlg(s);
	dlg.DoModal();
}