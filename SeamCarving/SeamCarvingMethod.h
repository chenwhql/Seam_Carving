#pragma once

#include <stdint.h>
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
	static void ScanImageTest(Mat& I, CString& str);
	static void GetDamageLaplacian(Mat& I_src, Mat& I_dst);
	static void GetDamageSobel(Mat& I_src, Mat& I_dst);
	static void GetVerticalLeastDamageSeam(Mat& dmg, vector<unsigned int>& seam);
	static void RemoveVerticalLeastDamageSeam(Mat& I, vector<unsigned int>& seam);
	static void GetHorizontalLeastDamageSeam(Mat& dmg, vector<unsigned int>& seam);
	static void RemoveHorizontalLeastDamageSeam(Mat& I, vector<unsigned int>& seam);
	static void ImgDamageCalc(Mat& I, Mat& dmg);
	static void SignVerticalSeam(Mat& I, vector<unsigned int> seam);
	static void SignHorizontalSeam(Mat& I, vector<unsigned int> seam);
	static void GetOptimalSeamOrder(Mat& I_src, uchar** order_map);
	static unsigned int GetVerticalLeastDamageSeamDamage(Mat& dmg);
	static unsigned int GetHorizontalLeastDamageSeamDamage(Mat& dmg);
};

