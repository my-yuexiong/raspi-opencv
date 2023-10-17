#ifndef _MY_FILTER_H_
#define _MY_FILTER_H_
#include <opencv2/opencv.hpp>
#include <vector>

void my_filter(int area,std::vector<std::vector<cv::Point>>& red_contours,
                std::vector<std::vector<cv::Point>>& green_contours,
                std::vector<std::vector<cv::Point>>& blue_contours,
                std::vector<cv::Rect>& rectangles);

#endif