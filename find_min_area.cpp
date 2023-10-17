#include "find_min_area.h"

cv::Rect find_min_area(std::vector<cv::Rect>& rectangles)
{

    for(size_t i = 0; i < rectangles.size()-1; i++)
    {

        for(size_t j = 0; j < rectangles.size()-i-1; j++)
        {
            if(rectangles[j].area() > rectangles[j+1].area())
            {
                std::swap(rectangles[j],rectangles[j+1]);
            }
        }
    }

    return rectangles[0];
}