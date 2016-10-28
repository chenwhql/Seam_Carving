#include "stdafx.h"
#include "SeamCarvingMethod.h"

#include "SeamCarving.h"
#include <vector>

CSeamCarvingMethod::CSeamCarvingMethod()
{
}


CSeamCarvingMethod::~CSeamCarvingMethod()
{
}

void CSeamCarvingMethod::ScanImageTest(Mat& I, CString& str)
{
	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));

	int channels = I.channels();

	int nRows = I.rows * channels;
	int nCols = I.cols;

	//内存足够大，可实现连续存储，图像中的各行就能一行一行地连接起来，形成一个长行
	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i = 0, j = 0;
	uchar* p;
	CString s0;

	s0.Format("%d ", channels);
	str += s0;
	str += _T("\r\n");

	p = I.ptr<uchar>(i);
	for (j = 0; j < 10; j++)
	{
		s0.Format("%u ", p[j]);
		str += s0;
	}
	//for (i = 0; i < nRows; i++)
	//{
	//	p = I.ptr<uchar>(i);
	//	for (j = 0; j < nCols; j++)
	//	{
	//		s0.Format("%u ", p[j]);
	//		str += s0;
	//	}
	//	str += _T("\r\n");
	//}
}


void CSeamCarvingMethod::GetDamageLaplacian(Mat& I_src, Mat& I_dst)
{
	Mat src_gray, temp_dst;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	I_dst.release();
	I_dst.create(I_src.rows, I_src.cols, CV_32S);

	/// 使用高斯滤波消除噪声
	GaussianBlur(I_src, I_src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// 转换为灰度图
	cvtColor(I_src, src_gray, CV_RGB2GRAY);

	/// 使用Laplace函数
	Laplacian(src_gray, temp_dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(temp_dst, I_dst);

}

//opencv sobel算子计算图像梯度
void CSeamCarvingMethod::GetDamageSobel(Mat& I_src, Mat& I_dst)
{
	Mat src_gray;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	I_dst.release();
	I_dst.create(I_src.rows, I_src.cols, CV_32S);

	/// 使用高斯滤波消除噪声
	GaussianBlur(I_src, I_src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// 转换为灰度图
	cvtColor(I_src, src_gray, CV_RGB2GRAY);

	/// 创建 grad_x 和 grad_y 矩阵
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// 求 X方向梯度
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// 求Y方向梯度
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// 合并梯度(近似)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, I_dst);

}

#define MAX_SIZE 3000
unsigned int sum_dmg[MAX_SIZE][MAX_SIZE];
unsigned int route[MAX_SIZE][MAX_SIZE];

void CSeamCarvingMethod::GetVerticalLeastDamageSeam(Mat& dmg, vector<unsigned int>& seam)
{
	/*unsigned int** sum_dmg = new unsigned int*[dmg.rows];
	for (int i = 0; i < dmg.rows; i++)
		sum_dmg[i] = new unsigned int[dmg.cols];
	unsigned int** route = new unsigned int*[dmg.rows];
	for (int i = 0; i < dmg.rows; i++)
		route[i] = new unsigned int[dmg.cols];*/

	for (int j = 0; j < dmg.cols; j++)
	{
		sum_dmg[0][j] = 0;
		route[0][j] = 0;
	}
	for (int i = 1; i < dmg.rows; i++)
	{
		for (int j = 0; j < dmg.cols; j++)
		{
			sum_dmg[i][j] = numeric_limits<unsigned int>::max();
			route[i][j] = 0;
		}
	}
	for (int i = 1; i < dmg.rows; i++)
	{
		for (int j = 0; j < dmg.cols; j++)
		{
			for (int k = max(0, j - 1); k <= min(dmg.cols - 1, j + 1); k++)
			{
				if (sum_dmg[i][j] > (sum_dmg[i - 1][k] + dmg.at<uint32_t>(i, j)))
				{
					sum_dmg[i][j] = sum_dmg[i - 1][k] + dmg.at<uint32_t>(i, j);
					route[i][j] = k;
				}		
			}
		}
	}

	//找破坏最小的接缝
	unsigned int least = numeric_limits<unsigned int>::max();
	unsigned int col = 0;
	for (int i = 0; i < dmg.cols; i++)
	{
		if (least > sum_dmg[dmg.rows - 1][i])
		{
			least = sum_dmg[dmg.rows - 1][i];
			col = i;
		}
	}

	//构造接缝
	seam[dmg.rows - 1] = col;
	for (int i = dmg.rows - 1; i > 0; i--)
	{
		seam[i - 1] = route[i][seam[i]];
	}

	//for (int i = 0; i < dmg.rows; i++)
	//	delete[]sum_dmg[i];
	//delete[]sum_dmg;
	//for (int i = 0; i < dmg.rows; i++)
	//	delete[]route[i];
	//delete[]route;
}

void CSeamCarvingMethod::RemoveVerticalLeastDamageSeam(Mat& I, vector<unsigned int>& seam)
{
	for (int i = 0; i < I.rows ; i++)
	{
		for (int j = seam[i]; j < I.cols - 1; j++)
		{
			I.at<Vec3b>(i, j) = I.at<Vec3b>(i,j+1);
		}
	}

	//修改图像大小
	I = I(Rect(0, 0, I.cols - 1, I.rows));

	//重新计算demage
	//ImgDamageCalc(I, dmg);
}

void CSeamCarvingMethod::GetHorizontalLeastDamageSeam(Mat& dmg, vector<unsigned int>& seam)
{
	//矩阵翻转
	transpose(dmg, dmg);
	GetVerticalLeastDamageSeam(dmg, seam);
	transpose(dmg, dmg);  //复原
}

void CSeamCarvingMethod::RemoveHorizontalLeastDamageSeam(Mat& I, vector<unsigned int>& seam)
{
	//矩阵翻转
	transpose(I, I);
	RemoveVerticalLeastDamageSeam(I, seam);
	transpose(I, I);
}

//自己实现的计算像素点破坏度的函数
void CSeamCarvingMethod::ImgDamageCalc(Mat& I, Mat& dmg)
{
	int channels = I.channels();

	dmg.release();
	dmg.create(I.rows, I.cols, CV_32S);

	//考虑边缘

	for (int i = 0; i < I.rows; ++i)
	{
		uchar* prev = (i - 1 >= 0) ? I.ptr<uchar>(i - 1) : NULL;
		uchar* curr = I.ptr<uchar>(i);
		uchar* next = (i + 1 < I.rows) ? I.ptr<uchar>(i + 1) : NULL;

		switch (channels)
		{
		case 1:  //灰度图
		{
			for (int j = 0; j < I.cols; ++j)
			{
				unsigned int val = 0;
				//x-axis damage
				if (prev == NULL) val += (0 - next[j]) * (0 - next[j]);
				else if (next == NULL) val += (prev[j] - 0) * (prev[j] - 0);
				else val += (prev[j] - next[j]) * (prev[j] - next[j]);

				//y-axis damage
				if (j - 1 < 0) val += (curr[j + 1] - 0) * (curr[j + 1] - 0);
				else if (j + 1 >= I.cols) val += (0 - curr[j - 1]) * (0 - curr[j - 1]);
				else val += (curr[j + 1] - curr[j - 1]) * (curr[j + 1] - curr[j - 1]);

				dmg.at<uint32_t>(i, j) = val;
			}
			break;
		}
		case 3:  //RGB图
		{
			for (int j = 0; j < I.cols; ++j) {
				unsigned int val = 0;
				//Energy along the x-axis
				if (prev == NULL)
				{
					val += (0 - next[3 * j]) * (0 - next[3 * j]);
					val += (0 - next[3 * j + 1]) * (0 - next[3 * j + 1]);
					val += (0 - next[3 * j + 2]) * (0 - next[3 * j + 2]);
				}
				else if (next == NULL)
				{
					val += (prev[3 * j] - 0) * (prev[3 * j] - 0);
					val += (prev[3 * j + 1] - 0) * (prev[3 * j + 1] - 0);
					val += (prev[3 * j + 2] - 0) * (prev[3 * j + 2] - 0);
				}
				else
				{
					val += (prev[3 * j] - next[3 * j]) * (prev[3 * j] - next[3 * j]);
					val += (prev[3 * j + 1] - next[3 * j + 1]) * (prev[3 * j + 1] - next[3 * j + 1]);
					val += (prev[3 * j + 2] - next[3 * j + 2]) * (prev[3 * j + 2] - next[3 * j + 2]);
				}

				//Energy along the y-axis
				if (j - 1 < 0)
				{
					val += (curr[3 * j + 3] - 0) * (curr[3 * j + 3] - 0);
					val += (curr[3 * j + 4] - 0) * (curr[3 * j + 4] - 0);
					val += (curr[3 * j + 5] - 0) * (curr[3 * j + 5] - 0);
				}
				else if (j + 1 >= I.cols)
				{
					val += (0 - curr[3 * j - 3]) * (0 - curr[3 * j - 3]);
					val += (0 - curr[3 * j - 2]) * (0 - curr[3 * j - 2]);
					val += (0 - curr[3 * j - 1]) * (0 - curr[3 * j - 1]);
				}
				else
				{
					val += (curr[3 * j + 3] - curr[3 * j - 3]) * (curr[3 * j + 3] - curr[3 * j - 3]);
					val += (curr[3 * j + 4] - curr[3 * j - 2]) * (curr[3 * j + 4] - curr[3 * j - 2]);
					val += (curr[3 * j + 5] - curr[3 * j - 1]) * (curr[3 * j + 5] - curr[3 * j - 1]);
				}

				dmg.at<uint32_t>(i, j) = val;
			}
			break;
		}
		}
	}

	// 把图像边缘像素设置为numeric_limits<unsigned int>::max()
	//dmg.row(0).setTo(Scalar(numeric_limits<unsigned int>::max()));
	//dmg.row(dmg.rows - 1).setTo(Scalar(numeric_limits<unsigned int>::max()));
	//dmg.col(0).setTo(Scalar(numeric_limits<unsigned int>::max()));
	//dmg.col(dmg.cols - 1).setTo(Scalar(numeric_limits<unsigned int>::max()));
}




void CSeamCarvingMethod::SignVerticalSeam(Mat& I, vector<unsigned int> seam)
{
	for (int i = 0; i < I.rows; i++)
	{
		I.at<Vec3b>(i, seam[i]) = Vec3b(0, 0, 255);
	}
}


void CSeamCarvingMethod::SignHorizontalSeam(Mat& I, vector<unsigned int> seam)
{
	for (int i = 0; i < I.cols; i++)
	{
		I.at<Vec3b>(seam[i], i) = Vec3b(0, 0, 255);
	}
}

////////////////////////尝试////////////////////////////////////

unsigned int CSeamCarvingMethod::GetVerticalLeastDamageSeamDamage(Mat& dmg)
{
	for (int j = 0; j < dmg.cols; j++)
	{
		sum_dmg[0][j] = 0;
	}
	for (int i = 1; i < dmg.rows; i++)
	{
		for (int j = 0; j < dmg.cols; j++)
		{
			sum_dmg[i][j] = numeric_limits<unsigned int>::max();
		}
	}
	for (int i = 1; i < dmg.rows; i++)
	{
		for (int j = 0; j < dmg.cols; j++)
		{
			for (int k = max(0, j - 1); k <= min(dmg.cols - 1, j + 1); k++)
			{
				if (sum_dmg[i][j] >(sum_dmg[i - 1][k] + dmg.at<uint32_t>(i, j)))
				{
					sum_dmg[i][j] = sum_dmg[i - 1][k] + dmg.at<uint32_t>(i, j);
				}
			}
		}
	}

	//找破坏最小的接缝的值
	unsigned int least = numeric_limits<unsigned int>::max();
	for (int i = 0; i < dmg.cols; i++)
	{
		if (least > sum_dmg[dmg.rows - 1][i])
		{
			least = sum_dmg[dmg.rows - 1][i];
		}
	}

	return least;
}

unsigned int CSeamCarvingMethod::GetHorizontalLeastDamageSeamDamage(Mat& dmg)
{
	unsigned int least = 0;
	//矩阵翻转
	transpose(dmg, dmg);
	least = GetVerticalLeastDamageSeamDamage(dmg);
	transpose(dmg, dmg);  //复原
	return least;
}

//用动态规划的方法求解seam Carving的顺序
unsigned int T[MAX_SIZE / 2][MAX_SIZE / 2];
void CSeamCarvingMethod::GetOptimalSeamOrder(Mat& I_src, uchar** order_map)
{
	Mat src_temp = I_src.clone();

	int row = I_src.rows / 2;
	int col = I_src.cols / 2;
	int sum = row + col;

	//初始化
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			order_map[i][j] = 0;
			T[i][j] = 0;
		}
	}

	//计算
	//for (int i = 1; i < row; i++)
	//{
	//	
	//	T[i][0]
	//}

	//for (int k = 1; k < sum; k++)
	//{
	//	for (int i = 1; i < min(k , row); i++)
	//	{
	//		int j = k - i;
	//		if (j < col)
	//		{
	//			T[i][j] = min(T[i-1][j]+);
	//			
	//		}
	//	}
	//}

}
