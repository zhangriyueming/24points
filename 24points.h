// 24points.h : 24points 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// C24pointsApp:
// 有关此类的实现，请参阅 24points.cpp
//

class C24pointsApp : public CWinApp
{
public:
	C24pointsApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern C24pointsApp theApp;