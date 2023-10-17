#include "get_center_point.h"

cv::Point get_center_point(cv::Rect& rectangle)
{
    cv::Point center_point(rectangle.x+0.5*rectangle.width,rectangle.y+0.5*rectangle.height);

    return center_point;
}