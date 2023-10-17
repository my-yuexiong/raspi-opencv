#ifndef _FIND_CLOSE_POINT_H_
#define _FIND_CLOSE_POINT_H_
#include <vector>
#include <opencv2/opencv.hpp>

cv::Rect find_close_point(std::vector<cv::Rect>& rectangles);

#endif