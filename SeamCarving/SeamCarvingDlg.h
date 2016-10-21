
// SeamCarvingDlg.h : 头文件
//

#pragma once

#include "cv.h"
#include "highgui.h"
#include "Opencv2/opencv.hpp"
#include "CvvImage.h"
#include "D:\Program Files\opencv\build\include\opencv2\core\core.hpp"
using namespace std;
using namespace cv;

// CSeamCarvingDlg 对话框
class CSeamCarvingDlg : public CDialogEx
{
// 构造
public:
	CSeamCarvingDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SEAMCARVING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	void DrawPicToHDC(IplImage * img, UINT ID);
	afx_msg void OnBnClickedButtonSave();
	IplImage* TheImage;
	IplImage TempImage;
	Mat image;
	afx_msg void OnEnChangeEditMsg();
	Mat damage_grad;
//	void cvLoadImage(Mat& img);
};
