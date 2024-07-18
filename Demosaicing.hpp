#pragma once

#include <opencv2/opencv.hpp>
#include <string>

// Bayer Patterm:
enum BayerPattern
{
	RGGB = 1,
	GRBG = 2,
	BGGR = 3,
	GBGR = 4,
};

enum DemosaingAlgo
{
	DM_Edge_Aware = 1,
	DM_VNG = 2,
	DM_GBTF = 3,
	DM_RI = 4,
};

// Demosaicing Method:
//  Smooth_Hue          1
//  Laplacian_Corrected 2
//  GBTF                3
//  RI                  4

//  Convert RGB image To Bayer image
//  in param: rgb: RGB image
//  out param: bayerbg Bayer BG image
void RGB2BayerBG(cv::Mat& rgb, cv::Mat& bayerbg);

// assume input raw image is 10 bit image encoded in 16 bit format
void Demosaicing(std::string &BayerFileName, cv::Mat &Dst, int rows, int cols, int BayerPatternFlag=1, int DemosaicingMethod=4);

void Demosaicing(cv::Mat mSrc_Bayer, cv::Mat& mDst_RGB, int rows, int cols, int BayerPatternFlag, int DemosaicingMethod);
