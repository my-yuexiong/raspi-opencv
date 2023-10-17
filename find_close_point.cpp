#include "find_close_point.h"
cv::Rect find_close_point(std::vector<cv::Rect>& rectangles)
{


    for(size_t i = 0; i < rectangles.size()-1; i++)
    {

        for(size_t j = 0; j < rectangles.size()-i-1; j++)
        {
            if(rectangles[j].x*rectangles[j].x + rectangles[j].y*rectangles[j].y > 
            rectangles[j+1].x*rectangles[j+1].x + rectangles[j+1].y*rectangles[j+1].y)
            {
                std::swap(rectangles[j],rectangles[j+1]);
            }
        }
    }
    return rectangles[0];
}