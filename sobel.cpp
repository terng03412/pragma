#include <algorithm>
#include <cmath>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

cv::Mat sobel_transform(const cv::Mat &input)
{
    cv::Mat finalImage = cv::Mat::zeros(input.size(), input.type());

#pragma omp parallel for
    for (int j = 0; j < input.rows - 2; ++j)
    {
        for (int i = 0; i < input.cols - 2; ++i)
        {
            // applay karnel in x direction
            int xValOfPixel =
                -(int)input.at<uchar>(j, i) + (int)input.at<uchar>(j + 2, i) - 2 * (int)input.at<uchar>(j, i + 1) + 2 * (int)input.at<uchar>(j + 2, i + 1) - (int)input.at<uchar>(j, i + 2) + (int)input.at<uchar>(j + 2, i + 2);

            // apply karnel in y direction
            int yValOfPixel =
                -(int)input.at<uchar>(j, i) - 2 * (int)input.at<uchar>(j + 1, i) - (int)input.at<uchar>(j + 2, i) + (int)input.at<uchar>(j, i + 2) + 2 * (int)input.at<uchar>(j + 1, i + 2) + (int)input.at<uchar>(j + 2, i + 2);

            int sum = std::clamp(std::abs(xValOfPixel) + std::abs(yValOfPixel), 0, 255);

            finalImage.at<uchar>(j, i) = (uchar)sum;
        }
    }

    return finalImage;
}

#include <omp.h>
#include <iostream>
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./sobel imagePath" << std::endl;
        return 1;
    }

    const cv::Mat initialImage = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (!initialImage.data)
    {
        std::cerr << "Failed to read image" << std::endl;
        return 1;
    }

    double start_time = omp_get_wtime();
    const cv::Mat finalImage = sobel_transform(initialImage);
    double end_time = omp_get_wtime();

    std::clog << "Time: " << end_time - start_time << std::endl;

    cv::namedWindow("Initial Image", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Final Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Initial Image", initialImage);
    cv::imshow("Final Image", finalImage);

    cv::waitKey(0);
}