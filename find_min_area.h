#ifndef _FIND_MIN_AREA_H_
#define _FIND_MIN_AREA_H_
#include <opencv2/opencv.hpp>
#include <vector>

cv::Rect find_min_area(std::vector<cv::Rect>& rectangles);

#endif
