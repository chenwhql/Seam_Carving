
// SeamCarving.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define MAX_SIZE 2000


// CSeamCarvingApp: 
// �йش����ʵ�֣������ SeamCarving.cpp
//

class CSeamCarvingApp : public CWinApp
{
public:
	CSeamCarvingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSeamCarvingApp theApp;
extern int damage[MAX_SIZE][MAX_SIZE];
extern int sum_damage[MAX_SIZE][MAX_SIZE];
extern int route[MAX_SIZE][MAX_SIZE];