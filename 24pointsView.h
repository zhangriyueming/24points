// 24pointsView.h : C24pointsView 类的接口
//


#pragma once


class C24pointsView : public CView
{
protected: // 仅从序列化创建
	C24pointsView();
	DECLARE_DYNCREATE(C24pointsView)

// 属性
public:
	C24pointsDoc* GetDocument() const;

// 操作
public:
	CEdit *pEdit;
	CButton *pButton;
	CButton *pNoSol;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~C24pointsView();

protected:

	void OnDefine();

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate(void);
public:
	void OnButton(void);
	void OnNoSol(void);
	void Solver(void);
	void ShowStatics(void);
public:
	virtual void OnTimer(UINT nIDEvent);
};

#ifndef _DEBUG  // 24pointsView.cpp 中的调试版本
inline C24pointsDoc* C24pointsView::GetDocument() const
   { return reinterpret_cast<C24pointsDoc*>(m_pDocument); }
#endif

