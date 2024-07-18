#include "opencv2/opencv.hpp"
#include "Demosaicing.hpp"
#include "GBTF.hpp"

#include <iostream>
#include <string>
#include <vector>

using namespace cv;


// Function to calculate RMS error
double calculateRMSE(const Mat& img1, const Mat& img2) {
    Mat diff;
    absdiff(img1, img2, diff);
    diff = diff.mul(diff);
    Scalar sum = cv::sum(diff);
    double sse = sum[0] + sum[1] + sum[2];
    double mse = sse / (double)(img1.channels() * img1.total());
    return std::sqrt(mse);
}

// Function to calculate PSNR
double calculatePSNR(const Mat& img1, const Mat& img2) {
    double rmse = calculateRMSE(img1, img2);
    if (rmse == 0) return INFINITY;
    return 20 * log10(255.0 / rmse);
}

// Function to perform demosaicing, save the result, and calculate error metrics
void performAndSaveDemosaicing(const Mat& mBayerBG, const Mat& groundTruth, const std::string& filename, DemosaingAlgo algo) {
    Mat mBayer = mBayerBG.clone();
    Mat mDst_RGB;
    Demosaicing(mBayer, mDst_RGB, mBayer.rows, mBayer.cols, BayerPattern::BGGR, algo);
    imwrite(filename, mDst_RGB);

    double rmse = calculateRMSE(groundTruth, mDst_RGB);
    double psnr = calculatePSNR(groundTruth, mDst_RGB);

    std::cout << "For " << filename << ":" << std::endl;
    std::cout << "  RMSE: " << rmse << std::endl;
    std::cout << "  PSNR: " << psnr << " dB" << std::endl;
    std::cout << std::endl;
}

int main() {
    // Read and resize the input image
    Mat mSrc = imread("Camera_Test.JPG", IMREAD_COLOR);
    resize(mSrc, mSrc, Size(), 0.25, 0.25);

    // Convert RGB to Bayer BG
    Mat mBayerBG;
    RGB2BayerBG(mSrc, mBayerBG);

    // Save ground truth image
    imwrite("Data/GT.png", mSrc);

    // Perform demosaicing with different algorithms
    std::vector<std::pair<std::string, DemosaingAlgo>> algorithms = {
        {"Data/Edge-Aware.png", DemosaingAlgo::DM_Edge_Aware},
        {"Data/DM_VNG.png", DemosaingAlgo::DM_VNG},
        {"Data/DM_GBTF.png", DemosaingAlgo::DM_GBTF},
        {"Data/DM_RI.png", DemosaingAlgo::DM_RI}
    };

    for (size_t i = 0; i < algorithms.size(); ++i) {
        const std::string& filename = algorithms[i].first;
        const DemosaingAlgo& algo = algorithms[i].second;
        performAndSaveDemosaicing(mBayerBG,mSrc, filename, algo);
    }

    // Display results
    imshow("Bayer BG", mBayerBG);
    imshow("Input Image", mSrc);
    waitKey(0);

    return 0;
}
