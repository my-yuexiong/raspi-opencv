#ifndef _POINT_COLOR_H_
#define _POINT_COLOR_H_
#include <opencv2/opencv.hpp>

class point_color
{
public:
    int x;
    int y;

    int blue;
    int green;
    int red;

    point_color(int x1,int y1,cv::Mat frame);

    ~point_color();

    //void get_rgb();

    int judge_color();
    
};

#endif