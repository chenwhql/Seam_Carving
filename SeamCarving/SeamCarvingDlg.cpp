
// SeamCarvingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SeamCarving.h"
#include "SeamCarvingDlg.h"
#include "afxdialogex.h"

#include "SeamCarvingMethod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSeamCarvingDlg 对话框



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


// CSeamCarvingDlg 消息处理程序

BOOL CSeamCarvingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	if (TheImage) cvReleaseImage(&TheImage);

	////关联一个openCV窗口到IDC_STATIC picture控件
	//namedWindow("view_src", WINDOW_AUTOSIZE);
	//HWND hWnd = (HWND)cvGetWindowHandle("view_src");
	//HWND hParent = ::GetParent(hWnd);
	//::SetParent(hWnd, GetDlgItem(IDC_STATIC)->m_hWnd);
	//::ShowWindow(hParent, SW_HIDE);

	////关联一个openCV窗口到IDC_STATIC picture控件
	//namedWindow("view_pro", WINDOW_AUTOSIZE);
	//HWND hWnd = (HWND)cvGetWindowHandle("view_pro");
	//HWND hParent = ::GetParent(hWnd);
	//::SetParent(hWnd, GetDlgItem(IDC_STATIC_PRO)->m_hWnd);
	//::ShowWindow(hParent, SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSeamCarvingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CDialog::UpdateWindow();// 更新windows窗口，如果无这步调用，图片显示还会出现问题
		DrawPicToHDC(TheImage, IDC_STATIC);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSeamCarvingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSeamCarvingDlg::OnBnClickedButtonOpen()
{
	// TODO:  在此添加控件通知处理程序代码
	CString filter = "image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||";   //文件过虑的类型  
	CFileDialog dlg(TRUE, ".jpg", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter, NULL);// 打开文件对话框的标题名
	dlg.m_ofn.lpstrTitle = _T("Open Image"); // 判断是否获得图片
	if( dlg.DoModal()!= IDOK )
		return;
	// 获取图片路径
	CString mPath = dlg.GetPathName();
	// 对上一幅显示的图片数据清零
	if (TheImage)
		cvZero(TheImage);
	// 读取图片
	TheImage = cvLoadImage(mPath.GetBuffer(0), CV_LOAD_IMAGE_COLOR);
	// 判断是否成功读取图片
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
	cimg.CopyOf(img); // 复制图片  
	cimg.DrawToHDC(hDC, &rect); // 将图片绘制到显示控件的指定区域内  
	ReleaseDC(pDC);
}


void CSeamCarvingDlg::OnBnClickedButtonSave()
{
	// TODO:  在此添加控件通知处理程序代码
	CString filter = "image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||";   //文件过虑的类型  
	CFileDialog dlg(FALSE, ".jpg", "compress.jpg", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);// 打开文件对话框的标题名
	dlg.m_ofn.lpstrTitle = _T("Open Image"); // 判断是否获得图片
	if (dlg.DoModal() != IDOK)
		return;
}


void CSeamCarvingDlg::OnEnChangeEditMsg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
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
