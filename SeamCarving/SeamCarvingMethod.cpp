#include "stdafx.h"
#include "SeamCarvingMethod.h"

#include "SeamCarving.h"
#include <vector>

#define MAX_DAMAGE_SUM 100000


CSeamCarvingMethod::CSeamCarvingMethod()
{
}


CSeamCarvingMethod::~CSeamCarvingMethod()
{
}


Mat& CSeamCarvingMethod::ScanImageAndReduceC(Mat& I)
{
	//TODO: insert return statement here
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

	int i, j;
	uchar* p;
	CString str, s0;
	for (i = 0; i < nRows; i++)
	{
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; j++)
		{
			s0.Format("%u ", p[j]);
			str += s0;
		}
		str += _T("\r\n");
	}
	return I;
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

	/// 使用高斯滤波消除噪声
	GaussianBlur(I_src, I_src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// 转换为灰度图
	cvtColor(I_src, src_gray, CV_RGB2GRAY);

	/// 使用Laplace函数
	Laplacian(src_gray, temp_dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(temp_dst, I_dst);

}


void CSeamCarvingMethod::GetDamageSobel(Mat& I_src, Mat& I_dst)
{
	Mat src_gray;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

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

	//计算梯度值的平方
	//pow(abs_grad_x, 2.0f, abs_grad_x);
	//pow(abs_grad_y, 2.0f, abs_grad_y);
	////OutputArray = abs_grad_x + abs_grad_y;
	//add(abs_grad_x, abs_grad_y, I_dst, noArray(), CV_32F);

	/// 合并梯度(近似)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, I_dst);

}


void CSeamCarvingMethod::SeamDamageCalc(Mat& I)
{
	for (int j = 0; j < I.cols; j++)
	{
		sum_damage[0][j] = damage[0][j];
	}
	for (int i = 1; i < I.rows; i++)
	{
		for (int j = 0; j < I.cols; j++)
		{
			sum_damage[i][j] = MAX_DAMAGE_SUM;
		}
	}
	for (int i = 1; i < I.rows; i++)
	{
		for (int j = 1; j < I.cols; j++)
		{
			for (int k = max(0, j - 1); k <= min(I.cols - 1, j + 1); k++)
			{
				sum_damage[i][j] = sum_damage[i - 1][k] + damage[i][j];
				route[i][j] = k;
			}
		}
	}
}


void CSeamCarvingMethod::ImgDamageCalc(Mat& I)
{
	// accept only char type matrices
	CV_Assert(I.depth() != sizeof(uchar));

	int channels = I.channels();

	//考虑边缘

	switch (channels)
	{
	case 1:
	{
		for (int i = 0; i < I.rows; ++i)
			for (int j = 0; j < I.cols; ++j)
			{
				damage[i][j] = 0;
				if (j - 1 >= 0) damage[i][j] += abs(I.at<uchar>(i, j) - I.at<uchar>(i, j - 1));
				if (j + 1 <= I.cols - 1) damage[i][j] += abs(I.at<uchar>(i, j) - I.at<uchar>(i, j + 1));
				if (i - 1 >= 0) damage[i][j] += abs(I.at<uchar>(i, j) - I.at<uchar>(i - 1, j));
				if (i + 1 <= I.rows - 1) damage[i][j] += abs(I.at<uchar>(i, j) - I.at<uchar>(i + 1, j));
			}
		break;
	}
	case 3:
	{
		Mat_<Vec3b> _I = I;

		for (int i = 0; i < I.rows; ++i)
			for (int j = 0; j < I.cols; ++j)
			{
				damage[i][j] = 0;
				if (j - 1 >= 0) damage[i][j] += abs(_I(i, j)[0] - _I(i, j - 1)[0]) + abs(_I(i, j)[1] - _I(i, j - 1)[1]) + abs(_I(i, j)[2] - _I(i, j - 1)[2]);
				if (j + 1 <= I.cols - 1) damage[i][j] += abs(_I(i, j)[0] - _I(i, j + 1)[0]) + abs(_I(i, j)[1] - _I(i, j + 1)[1]) + abs(_I(i, j)[2] - _I(i, j + 1)[2]);
				if (i - 1 >= 0) damage[i][j] += abs(_I(i, j)[0] - _I(i - 1, j)[0]) + abs(_I(i, j)[1] - _I(i - 1, j)[1]) + abs(_I(i, j)[2] - _I(i - 1, j)[2]);
				if (i + 1 <= I.rows - 1) damage[i][j] += abs(_I(i, j)[0] - _I(i + 1, j)[0]) + abs(_I(i, j)[1] - _I(i + 1, j)[1]) + abs(_I(i, j)[2] - _I(i + 1, j)[2]);
			}
		break;
	}
	}


}


void CSeamCarvingMethod::FindNumFrontSeam(Mat& I, vector<int>& rlt, int num)
{
	int least = MAX_DAMAGE_SUM, col = MAX_SIZE;
	for (int j = 0; j < num; j++)
	{
		for (int i = 0; i < I.cols; i++)
		{
			if (least > sum_damage[I.rows][i])
			{
				least = sum_damage[I.rows][i];
				col = i;
			}
		}
		rlt.push_back(col);
		sum_damage[I.rows][col] = MAX_DAMAGE_SUM;
		least = MAX_DAMAGE_SUM;
	}
}


void CSeamCarvingMethod::SeamSign(Mat& I, vector<int>& seamCol)
{
	Mat_<Vec3b> _I = I;
	for (int i = 0; i < seamCol.size(); i++)
	{
		OneSeamPaint(_I, I.rows, seamCol[i]);
	}
}


void CSeamCarvingMethod::OneSeamPaint(Mat_<Vec3b>& _I, int row, int col)
{
	if (row == 0)
		return;
	OneSeamPaint(_I, row - 1, route[row][col]);
	//改变点的像素值
	_I(row, col)[0] = 255;
	_I(row, col)[1] = 0;
	_I(row, col)[2] = 0;
}
