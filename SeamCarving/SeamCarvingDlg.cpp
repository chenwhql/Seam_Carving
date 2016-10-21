
// SeamCarvingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SeamCarving.h"
#include "SeamCarvingDlg.h"
#include "afxdialogex.h"

#include "SeamCarvingMethod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSeamCarvingDlg �Ի���



CSeamCarvingDlg::CSeamCarvingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSeamCarvingDlg::IDD, pParent)
	, TheImage(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSeamCarvingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSeamCarvingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSeamCarvingDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSeamCarvingDlg::OnBnClickedButtonSave)
	ON_EN_CHANGE(IDC_EDIT_MSG, &CSeamCarvingDlg::OnEnChangeEditMsg)
END_MESSAGE_MAP()


// CSeamCarvingDlg ��Ϣ�������

BOOL CSeamCarvingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	if (TheImage) cvReleaseImage(&TheImage);

	////����һ��openCV���ڵ�IDC_STATIC picture�ؼ�
	//namedWindow("view_src", WINDOW_AUTOSIZE);
	//HWND hWnd = (HWND)cvGetWindowHandle("view_src");
	//HWND hParent = ::GetParent(hWnd);
	//::SetParent(hWnd, GetDlgItem(IDC_STATIC)->m_hWnd);
	//::ShowWindow(hParent, SW_HIDE);

	////����һ��openCV���ڵ�IDC_STATIC picture�ؼ�
	//namedWindow("view_pro", WINDOW_AUTOSIZE);
	//HWND hWnd = (HWND)cvGetWindowHandle("view_pro");
	//HWND hParent = ::GetParent(hWnd);
	//::SetParent(hWnd, GetDlgItem(IDC_STATIC_PRO)->m_hWnd);
	//::ShowWindow(hParent, SW_HIDE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSeamCarvingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSeamCarvingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CDialog::UpdateWindow();// ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
		DrawPicToHDC(TheImage, IDC_STATIC);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSeamCarvingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSeamCarvingDlg::OnBnClickedButtonOpen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString filter = "image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||";   //�ļ����ǵ�����  
	CFileDialog dlg(TRUE, ".jpg", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter, NULL);// ���ļ��Ի���ı�����
	dlg.m_ofn.lpstrTitle = _T("Open Image"); // �ж��Ƿ���ͼƬ
	if( dlg.DoModal()!= IDOK )
		return;
	// ��ȡͼƬ·��
	CString mPath = dlg.GetPathName();
	// ����һ����ʾ��ͼƬ��������
	if (TheImage)
		cvZero(TheImage);
	// ��ȡͼƬ
	TheImage = cvLoadImage(mPath.GetBuffer(0), CV_LOAD_IMAGE_COLOR);
	// �ж��Ƿ�ɹ���ȡͼƬ
	if (!TheImage)
		return;
	image = Mat(TheImage, true);
	DrawPicToHDC(TheImage, IDC_STATIC);

	CSeamCarvingMethod::GetDamageSobel(image, damage_grad);
	//CSeamCarvingMethod::GetDamageLaplacian(image, damage_grad);

	vector<int> rlt;
	CSeamCarvingMethod::ImgDamageCalc(image);
	CSeamCarvingMethod::SeamDamageCalc(image);
	CSeamCarvingMethod::FindNumFrontSeam(image, rlt, 10);
	CSeamCarvingMethod::SeamSign(image, rlt);

	TempImage = image;
	DrawPicToHDC(&TempImage, IDC_STATIC_PRO);

	//cvReleaseImage(&ipl);

	/*CString str;
	CSeamCarvingMethod::ScanImageTest(image, str);
	SetDlgItemText(IDC_EDIT_MSG, str);*/
}


void CSeamCarvingDlg::DrawPicToHDC(IplImage * img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img); // ����ͼƬ  
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������  
	ReleaseDC(pDC);
}


void CSeamCarvingDlg::OnBnClickedButtonSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString filter = "image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||";   //�ļ����ǵ�����  
	CFileDialog dlg(FALSE, ".jpg", "compress.jpg", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);// ���ļ��Ի���ı�����
	dlg.m_ofn.lpstrTitle = _T("Open Image"); // �ж��Ƿ���ͼƬ
	if (dlg.DoModal() != IDOK)
		return;
}


void CSeamCarvingDlg::OnEnChangeEditMsg()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CSeamCarvingDlg::cvLoadImage(Mat& img)
//{
//	this->GetClientRect(&m_rect);
//	this->GetWindowRect(&m_rect_win);
//
//	m_src_img = img + 0;
//	m_dst_img = m_src_img + 0;
//	m_MouseDraw_img = Mat(m_dst_img.size(), m_dst_img.type(), Scalar(255, 255, 255));
//	m_rect_roi = Rect(0, 0, m_dst_img.cols, m_dst_img.rows);
//	m_rect_roi_center.x = (float)m_rect_roi.width / 2.0f;
//	m_rect_roi_center.y = (float)m_rect_roi.height / 2.0f;
//	m_MouseDraw_rect = Rect(m_rect_roi_center.x, m_rect_roi_center.y, 10, 10);
//	m_dst_roi = m_dst_img(m_rect_roi);
//	m_fRoi_scale = 1;
//}
