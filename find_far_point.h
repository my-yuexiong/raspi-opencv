#ifndef _FIND_FAR_POINT_H_
#define _FIND_FAR_POINT_H_
#include <vector>
#include <opencv2/opencv.hpp>

cv::Rect find_far_point(std::vector<cv::Rect>& rectangles);

#endif