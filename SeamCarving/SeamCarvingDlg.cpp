
// SeamCarvingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SeamCarving.h"
#include "SeamCarvingDlg.h"
#include "afxdialogex.h"

#include "Resource.h"  //�����������Դ��ͼ�򲻿�
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

	//�ð�ť��Ч��
	GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_COMPRESS)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CARVING)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(false);
	//��ʾ��ʾ
	GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(_T("ѹ����ɺ�����ı�������ʾ��ʱ�������ĵȴ���"));


	////���ý������ķ�Χ
	//m_pro.SetRange(0, 100);
	////���ý�������ÿһ��������
	//m_pro.SetStep(5);
	////���ý������ĵ�ǰλ��
	//m_pro.SetPos(0);

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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSeamCarvingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSeamCarvingDlg::OnBnClickedButtonOpen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString filter = _T("image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||");   //�ļ����ǵ�����  
	CFileDialog dlg(TRUE, _T("jpg"), _T("*.jpg"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, filter, NULL);// ���ļ��Ի���ı�����
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
	//ת��ΪMat
	image_src = Mat(TheImage, true);
	image = Mat(TheImage, true);
	image_show = Mat(TheImage, true);  //������ʾ�������
	ShowImage(image, IDC_STATIC, 0);
	//DrawPicToHDC(TheImage, IDC_STATIC);  //��ʾ
	//damage.create(image.rows, image.cols, CV_32S);  //����damage��¼ͼ,�������Ĵ�С��仯����������������

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
	cimg.CopyOf(img); // ����ͼƬ  
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������  
	ReleaseDC(pDC);
}


void CSeamCarvingDlg::OnBnClickedButtonSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString filter = _T("image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||");   //�ļ����ǵ�����  
	CFileDialog dlg(FALSE, _T("jpg"), _T("seam_carving.jpg"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	// ���ļ��Ի���ı�����
	dlg.m_ofn.lpstrTitle = _T("Save Image"); // �ж��Ƿ���ͼƬ
	if (dlg.DoModal() != IDOK)
		return;

	CString strFileName;          //����û�û��ָ���ļ���չ������Ϊ�����һ��  
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;
	if (dlg.m_ofn.nFileExtension = 0)               //��չ����ĿΪ0  
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CSeamCarvingDlg::OnBnClickedButtonCalc()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

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
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO:  �ڴ˴������Ϣ����������
	cvReleaseImage(&TheImage);
	//cvReleaseImage(TempImage);
	//KillTimer(1);
}


void CSeamCarvingDlg::OnBnClickedButtonCompress()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//ÿ��0.5�봥��ontimer�¼�ʹ��ǰ��
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

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
	// ��ȡͼƬ�Ŀ�͸�
	int h = img.rows;
	int w = img.cols;
	int nw, nh;

	if (0 == method)
	{
		// ���㽫ͼƬ���ŵ�dst_drawing��������ı�������
		float scale1 = (float)((float)w / (float)rect.Width());
		float scale2 = (float)((float)h / (float)rect.Height());

		float scale = (scale1>scale2) ? scale1 : scale2;

		// ���ź�ͼƬ�Ŀ�͸�
		nw = ((float)w) / scale;
		nh = ((float)h) / scale;

		//���ڸ���Ҳ�������ݽضϣ�Ϊ��ֹnw����Ŀ����С�������߽籣��
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
		// ���ź�ͼƬ�Ŀ�͸�
		nw = rect.Width();
		nh = rect.Height();
	}

	// Ϊ�˽����ź��ͼƬ���� dst_drawing �����в�λ�������ͼƬ�� dst_drawing ���Ͻǵ���������ֵ
	int tlx = (nw < rect.Width()) ? (((float)(rect.Width() - nw)) / 2.0f + 0.5f) : 0;
	int tly = (nh < rect.Height()) ? (((float)(rect.Height() - nh)) / 2.0f + 0.5f) : 0;

	//���þ����С��ͼ�Ƚϴ�ʱҲ�ܷ�ʱ�䣬�Ӹ��жϿɱ����ظ������þ����С
	if (dst_img.rows != rect.Height() || dst_img.cols != rect.Width() || dst_img.type() != img.type())
	{
		dst_img = Mat(rect.Height(), rect.Width(), img.type());
	}
	if (0 == method)
	{
		dst_img = 0;
	}
	// ���� dst_img �� ROI ��������������ĺ��ͼƬ img
	Rect rect_roi = Rect(tlx, tly, nw, nh);

	// ��ͼƬ img �������ţ������뵽 src_drawing ��
	Mat dst_img_roi = dst_img(rect_roi);

	//��img�ߴ�������dst_img_roi�ߴ�,��ֵ����ѡ��INTER_NEAREST���ŵ������޽�����ʾ�ܽ�ÿ�����ؿ���
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
	cimg.CopyOf(&TempImage); // ����ͼƬ  
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������  
	ReleaseDC(pDC);

	//cvReleaseImage(&ipl);
}


void CSeamCarvingDlg::OnStnClickedStaticText()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(_T("ѹ����ɺ�����ı�������ʾ��ʱ"));
}
