
// SeamCarvingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SeamCarving.h"
#include "SeamCarvingDlg.h"
#include "afxdialogex.h"

#include "Resource.h"  //包含这个，资源视图打不开
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
	, show_flag(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSeamCarvingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PROGRESS1, m_pro);
	DDX_Control(pDX, IDC_STATIC_TEXT, hint);
}

BEGIN_MESSAGE_MAP(CSeamCarvingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSeamCarvingDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSeamCarvingDlg::OnBnClickedButtonSave)
	ON_EN_CHANGE(IDC_EDIT_MSG, &CSeamCarvingDlg::OnEnChangeEditMsg)
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CSeamCarvingDlg::OnBnClickedButtonCalc)
//	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_COMPRESS, &CSeamCarvingDlg::OnBnClickedButtonCompress)
	ON_BN_CLICKED(IDC_BUTTON_CARVING, &CSeamCarvingDlg::OnBnClickedButtonCarving)
	ON_STN_CLICKED(IDC_STATIC_TEXT, &CSeamCarvingDlg::OnStnClickedStaticText)
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

	//让按钮无效化
	GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_COMPRESS)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CARVING)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(false);
	//显示提示
	GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(_T("压缩完成后会在文本框中显示耗时，请耐心等待！"));


	////设置进度条的范围
	//m_pro.SetRange(0, 100);
	////设置进度条的每一步的增量
	//m_pro.SetStep(5);
	////设置进度条的当前位置
	//m_pro.SetPos(0);

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
		//ShowImage(image_src, IDC_STATIC, 0);
		switch (show_flag)
		{
		case 1:
			ShowImage(damage, IDC_STATIC_PRO, 0);
			break;
		case 2:
			ShowImage(image_show, IDC_STATIC_PRO, 0);
			break;
		case 3:
			ShowImage(image, IDC_STATIC_PRO, 0);
			break;
		}
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
	CString filter = _T("image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||");   //文件过虑的类型  
	CFileDialog dlg(TRUE, _T("jpg"), _T("*.jpg"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter, NULL);// 打开文件对话框的标题名
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
	//转化为Mat
	image_src = Mat(TheImage, true);
	image = Mat(TheImage, true);
	image_show = Mat(TheImage, true);  //用作显示处理过程
	ShowImage(image, IDC_STATIC, 0);
	//DrawPicToHDC(TheImage, IDC_STATIC);  //显示
	//damage.create(image.rows, image.cols, CV_32S);  //创建damage记录图,后面矩阵的大小会变化，放在这里有隐患

	GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_COMPRESS)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CARVING)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(false);

	//CSeamCarvingMethod::GetDamageSobel(image, damage_grad);
	//CSeamCarvingMethod::GetDamageLaplacian(image, damage_grad);

	//vector<unsigned int> least_damage_seam(max(image.rows, image.cols));
	//CSeamCarvingMethod::ImgDamageCalc(image, damage);
	//CSeamCarvingMethod::GetVerticalLeastDamageSeam(damage, least_damage_seam);
	//CSeamCarvingMethod::SignVerticalSeam(image, least_damage_seam);

	//vector<unsigned int> least_damage_seam(max(image.rows, image.cols));
	//CSeamCarvingMethod::ImgDamageCalc(image, damage);

	//namedWindow("view_src", WINDOW_AUTOSIZE);
	//imshow("view_src", image);

	//for (int i = 0; i < 1; i++)
	//{
	//	CSeamCarvingMethod::GetVerticalLeastDamageSeam(damage, least_damage_seam);
	//	CSeamCarvingMethod::SignVerticalSeam(image, least_damage_seam);
	//}

	//namedWindow("view_src", WINDOW_AUTOSIZE);
	//imshow("view_src", image);

	//TempImage = image;
	//DrawPicToHDC(&TempImage, IDC_STATIC);

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
	CString filter = _T("image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||");   //文件过虑的类型  
	CFileDialog dlg(FALSE, _T("jpg"), _T("seam_carving.jpg"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	// 打开文件对话框的标题名
	dlg.m_ofn.lpstrTitle = _T("Save Image"); // 判断是否获得图片
	if (dlg.DoModal() != IDOK)
		return;

	CString strFileName;          //如果用户没有指定文件扩展名，则为其添加一个  
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;
	if (dlg.m_ofn.nFileExtension = 0)               //扩展名项目为0  
	{
		switch (dlg.m_ofn.nFilterIndex)
		{
		case 1:
			strExtension = "bmp"; break;
		case 2:
			strExtension = "jpg"; break;
		case 3:
			strExtension = "gif"; break;
		case 4:
			strExtension = "png"; break;
		default:
			break;
		}
		strFileName = strFileName + "." + strExtension;
	} 

	IplImage img(image);
	cvSaveImage(strFileName, &img);
	
}


void CSeamCarvingDlg::OnEnChangeEditMsg()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CSeamCarvingDlg::OnBnClickedButtonCalc()
{
	// TODO:  在此添加控件通知处理程序代码

	//CSeamCarvingMethod::ImgDamageCalc(image, damage);
	CSeamCarvingMethod::GetDamageSobel(image, damage);

	//TempImage = damage;
	//DrawPicToHDC(&TempImage, IDC_STATIC_PRO);
	show_flag = 1;
	ShowImage(damage, IDC_STATIC_PRO, 0);

	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_COMPRESS)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_CARVING)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(false);

}


//void CSeamCarvingDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//
//	if (nIDEvent == 1)
//	{
//		int i, j, npos;
//		m_pro.GetRange(i, j);
//		npos = m_pro.GetPos();
//		UpdateData(FALSE);
//		m_pro.StepIt();
//	}
//
//	CDialogEx::OnTimer(nIDEvent);
//}


void CSeamCarvingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO:  在此处添加消息处理程序代码
	cvReleaseImage(&TheImage);
	//cvReleaseImage(TempImage);
	//KillTimer(1);
}


void CSeamCarvingDlg::OnBnClickedButtonCompress()
{
	// TODO:  在此添加控件通知处理程序代码
	//每隔0.5秒触发ontimer事件使其前进
	SetTimer(1, 500, NULL);

	vector<unsigned int> least_damage_seam(max(image_show.rows, image_show.cols));
	int row = image_show.rows, r_cnt = 0;
	int col = image_show.cols, c_cnt = 0;
	double t = (double)getTickCount();
	for (int i = 0; i < (col / 2 + row / 2); i++)
	{
		CSeamCarvingMethod::ImgDamageCalc(image_show, damage);
		//CSeamCarvingMethod::GetDamageSobel(image_show, damage);
		if ((c_cnt < col / 2) && (r_cnt < row / 2))
		{
			if (i % 2 == 0)
			{
				CSeamCarvingMethod::GetVerticalLeastDamageSeam(damage, least_damage_seam);
				CSeamCarvingMethod::SignVerticalSeam(image_show, least_damage_seam);
				//CSeamCarvingMethod::RemoveVerticalLeastDamageSeam(image, least_damage_seam);
				c_cnt++;
			}
			else
			{
				CSeamCarvingMethod::GetHorizontalLeastDamageSeam(damage, least_damage_seam);
				CSeamCarvingMethod::SignHorizontalSeam(image_show, least_damage_seam);
				//CSeamCarvingMethod::RemoveHorizontalLeastDamageSeam(image_show, least_damage_seam);
				r_cnt++;
			}
		}
		else if (c_cnt < col / 2)
		{
			CSeamCarvingMethod::GetVerticalLeastDamageSeam(damage, least_damage_seam);
			CSeamCarvingMethod::SignVerticalSeam(image_show, least_damage_seam);
			//CSeamCarvingMethod::RemoveVerticalLeastDamageSeam(image, least_damage_seam);
			c_cnt++;
		}
		else
		{
			CSeamCarvingMethod::GetHorizontalLeastDamageSeam(damage, least_damage_seam);
			CSeamCarvingMethod::SignHorizontalSeam(image_show, least_damage_seam);
			//CSeamCarvingMethod::RemoveHorizontalLeastDamageSeam(image_show, least_damage_seam);
			r_cnt++;
		}
	}

	t = ((double)getTickCount() - t) / getTickFrequency();
	CString str = "ShowSeam Time Cost: ", s0;
	s0.Format("%f", t);
	str += s0 + " s" _T("\r\n");
	SetDlgItemText(IDC_EDIT_MSG, str);

	//TempImage = image_show;
	//DrawPicToHDC(&TempImage, IDC_STATIC_PRO);
	show_flag = 2;
	ShowImage(image_show, IDC_STATIC_PRO, 0);

	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_COMPRESS)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CARVING)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(false);

}


void CSeamCarvingDlg::OnBnClickedButtonCarving()
{
	// TODO:  在此添加控件通知处理程序代码

	vector<unsigned int> least_damage_seam(max(image.rows, image.cols));
	int row = image.rows, r_cnt = 0;
	int col = image.cols, c_cnt = 0;
	double t = (double)getTickCount();
	for (int i = 0; i < (col/2 + row/2); i++)
	{
		CSeamCarvingMethod::ImgDamageCalc(image, damage);
		//CSeamCarvingMethod::GetDamageSobel(image, damage_grad);
		//damage_grad.copyTo(damage);
		//damage = damage_grad.clone();
		//CSeamCarvingMethod::GetDamageLaplacian(image, damage);
		if ((c_cnt < col/2) && (r_cnt < row/2))
		{
			if (i % 2 == 0)
			{
				CSeamCarvingMethod::GetVerticalLeastDamageSeam(damage, least_damage_seam);
				//CSeamCarvingMethod::SignVerticalSeam(image, least_damage_seam);
				CSeamCarvingMethod::RemoveVerticalLeastDamageSeam(image, least_damage_seam);
				c_cnt++;
			}
			else
			{
				CSeamCarvingMethod::GetHorizontalLeastDamageSeam(damage, least_damage_seam);
				//CSeamCarvingMethod::SignHorizontalSeam(image, least_damage_seam);
				CSeamCarvingMethod::RemoveHorizontalLeastDamageSeam(image, least_damage_seam);
				r_cnt++;
			}
		}
		else if (c_cnt < col / 2)
		{
			CSeamCarvingMethod::GetVerticalLeastDamageSeam(damage, least_damage_seam);
			//CSeamCarvingMethod::SignVerticalSeam(image, least_damage_seam);
			CSeamCarvingMethod::RemoveVerticalLeastDamageSeam(image, least_damage_seam);
			c_cnt++;
		}
		else
		{
			CSeamCarvingMethod::GetHorizontalLeastDamageSeam(damage, least_damage_seam);
			//CSeamCarvingMethod::SignHorizontalSeam(image, least_damage_seam);
			CSeamCarvingMethod::RemoveHorizontalLeastDamageSeam(image, least_damage_seam);
			r_cnt++;
		}
	}

	t = ((double)getTickCount() - t) / getTickFrequency();
	CString str = "SeamCarving Time Cost: ", s0;
	s0.Format("%f", t);
	str += s0 +" s" _T("\r\n");
	SetDlgItemText(IDC_EDIT_MSG, str);

	//TempImage = image;
	//DrawPicToHDC(&TempImage, IDC_STATIC_PRO);
	show_flag = 3;
	ShowImage(image, IDC_STATIC_PRO, 0);

	//KillTimer(1);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_COMPRESS)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CARVING)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(true);
}


void CSeamCarvingDlg::ResizeImage(Mat &img, CRect rect, Mat &dst_img, int method)
{
	// 读取图片的宽和高
	int h = img.rows;
	int w = img.cols;
	int nw, nh;

	if (0 == method)
	{
		// 计算将图片缩放到dst_drawing区域所需的比例因子
		float scale1 = (float)((float)w / (float)rect.Width());
		float scale2 = (float)((float)h / (float)rect.Height());

		float scale = (scale1>scale2) ? scale1 : scale2;

		// 缩放后图片的宽和高
		nw = ((float)w) / scale;
		nh = ((float)h) / scale;

		//由于浮点也存在数据截断，为防止nw大于目标框大小，做个边界保护
		if (nw>rect.Width())
		{
			nw = rect.Width();
		}
		if (nh>rect.Height())
		{
			nh = rect.Height();
		}
	}
	else if (1 == method)
	{
		// 缩放后图片的宽和高
		nw = rect.Width();
		nh = rect.Height();
	}

	// 为了将缩放后的图片存入 dst_drawing 的正中部位，需计算图片在 dst_drawing 左上角的期望坐标值
	int tlx = (nw < rect.Width()) ? (((float)(rect.Width() - nw)) / 2.0f + 0.5f) : 0;
	int tly = (nh < rect.Height()) ? (((float)(rect.Height() - nh)) / 2.0f + 0.5f) : 0;

	//重置矩阵大小在图比较大时也很费时间，加个判断可避免重复的重置矩阵大小
	if (dst_img.rows != rect.Height() || dst_img.cols != rect.Width() || dst_img.type() != img.type())
	{
		dst_img = Mat(rect.Height(), rect.Width(), img.type());
	}
	if (0 == method)
	{
		dst_img = 0;
	}
	// 设置 dst_img 的 ROI 区域，用来存入更改后的图片 img
	Rect rect_roi = Rect(tlx, tly, nw, nh);

	// 对图片 img 进行缩放，并存入到 src_drawing 中
	Mat dst_img_roi = dst_img(rect_roi);

	//将img尺寸缩放至dst_img_roi尺寸,插值方法选用INTER_NEAREST，放到最大后无近似显示能将每个像素看清
	resize(img, dst_img_roi, cv::Size(nw, nh), 0.0, 0.0, INTER_NEAREST);

}

void CSeamCarvingDlg::ShowImage(Mat& img, UINT ID, int method)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);

	Mat dst_img(img.size(),img.type());
	ResizeImage(img, rect, dst_img, 0);

	TempImage = dst_img;
	CvvImage cimg;
	cimg.CopyOf(&TempImage); // 复制图片  
	cimg.DrawToHDC(hDC, &rect); // 将图片绘制到显示控件的指定区域内  
	ReleaseDC(pDC);

	//cvReleaseImage(&ipl);
}


void CSeamCarvingDlg::OnStnClickedStaticText()
{
	// TODO:  在此添加控件通知处理程序代码
	//GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(_T("压缩完成后会在文本框中显示耗时"));
}
