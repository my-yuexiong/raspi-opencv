#include "find_far_point.h"
cv::Rect find_far_point(std::vector<cv::Rect>& rectangles)
{
    for(size_t i = 0; i < rectangles.size()-1; i++)
    {

        for(size_t j = 0; j < rectangles.size()-i-1; j++)
        {
            if((rectangles[j].x+rectangles[j].width)*(rectangles[j].x+rectangles[j].width) + 
            (rectangles[j].y+rectangles[j].height)*(rectangles[j].y+rectangles[j].height) > 
            (rectangles[j+1].x+rectangles[j+1].width)*(rectangles[j+1].x+rectangles[j+1].width) + 
            (rectangles[j+1].y+rectangles[j+1].height)*(rectangles[j+1].y+rectangles[j+1].height))
            {
                std::swap(rectangles[j],rectangles[j+1]);
            }
        }
    }
    return rectangles[rectangles.size()-1];
}
