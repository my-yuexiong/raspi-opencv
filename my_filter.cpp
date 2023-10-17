#include "my_filter.h"

void my_filter(int area,std::vector<std::vector<cv::Point>>& red_contours,
                std::vector<std::vector<cv::Point>>& green_contours,
                std::vector<std::vector<cv::Point>>& blue_contours,
                std::vector<cv::Rect>& rectangles)
{
    // 识别最接近图像中心的矩形框并过滤小框
        for (const auto& contour : red_contours) {
            cv::Rect rect = cv::boundingRect(contour);
            if (rect.area() > area) { // 过滤掉面积小于100的矩形框
                rectangles.push_back(rect);
            }
        }
        for (const auto& contour : green_contours) {
            cv::Rect rect = cv::boundingRect(contour);
            if (rect.area() > area) { // 过滤掉面积小于100的矩形框
                rectangles.push_back(rect);
            }
        }
        for (const auto& contour : blue_contours) {
            cv::Rect rect = cv::boundingRect(contour);
            if (rect.area() > area) { // 过滤掉面积小于100的矩形框
                rectangles.push_back(rect);
            }
        }

}