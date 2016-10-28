
// SeamCarvingDlg.h : ͷ�ļ�
//

#pragma once

#include "cv.h"
#include "highgui.h"
#include "Opencv2/opencv.hpp"
#include "CvvImage.h"
#include "D:\Program Files\opencv\build\include\opencv2\core\core.hpp"
#include "afxcmn.h"
#include "afxwin.h"
using namespace std;
using namespace cv;

// CSeamCarvingDlg �Ի���
class CSeamCarvingDlg : public CDialogEx
{
// ����
public:
	CSeamCarvingDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SEAMCARVING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	Mat damage;
	afx_msg void OnBnClickedButtonCalc();
//	CProgressCtrl m_pro;
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	Mat image_show;
	afx_msg void OnBnClickedButtonCompress();
	afx_msg void OnBnClickedButtonCarving();
	void ResizeImage(Mat &img, CRect rect, Mat &dst_img, int method);
	void ShowImage(Mat& img, UINT ID, int method);
	afx_msg void OnStnClickedStaticText();
	CStatic hint;
	int show_flag;
	Mat image_src;
};
