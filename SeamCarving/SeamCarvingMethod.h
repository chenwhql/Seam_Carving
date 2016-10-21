#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class CSeamCarvingMethod
{
public:
	CSeamCarvingMethod();
	~CSeamCarvingMethod();
	static Mat& ScanImageAndReduceC(Mat& I);
	static void ScanImageTest(Mat& I, CString& str);
	static void GetDamageLaplacian(Mat& I_src, Mat& I_dst);
	static void GetDamageSobel(Mat& I_src, Mat& I_dst);
	static void SeamDamageCalc(Mat& I);
	static void ImgDamageCalc(Mat& I);
	static void FindNumFrontSeam(Mat& I, vector<int>& rlt, int num);
	static void SeamSign(Mat& I, vector<int>& seamCol);
	static void OneSeamPaint(Mat_<Vec3b>& _I, int row, int col);
};

