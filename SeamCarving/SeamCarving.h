
// SeamCarving.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#define MAX_SIZE 2000


// CSeamCarvingApp: 
// 有关此类的实现，请参阅 SeamCarving.cpp
//

class CSeamCarvingApp : public CWinApp
{
public:
	CSeamCarvingApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSeamCarvingApp theApp;
extern int damage[MAX_SIZE][MAX_SIZE];
extern int sum_damage[MAX_SIZE][MAX_SIZE];
extern int route[MAX_SIZE][MAX_SIZE];