#include "point_color.h"

point_color::point_color(int x1,int y1,cv::Mat frame)
{
    this->x = x1;
    this->y = y1;

    cv::Point point_this(this->x,this->y);

    cv::Vec3b color = frame.at<cv::Vec3b>(point_this);

    this->blue = color[0];
    this->green = color[1];
    this->red = color[2];
}

point_color::~point_color()
{

}

int point_color::judge_color()
{
    if(this->blue>=70 && this->blue<=145 && 
       this->green>=170 && this->green<=235 &&
       this->red>=65 && this->red<=140)//green
    {
        return 1;
    }
    else if(this->blue>=100 && this->blue<=165 && 
       this->green>=5 && this->green<=70 &&
       this->red>=15 && this->red<=90)//blue
    {
        return 2;
    }
    else if(this->blue>=15 && this->blue<=70 && 
       this->green>=60 && this->green<=115 &&
       this->red>=200 && this->red<=255)//red
    {
        return 3;
    }
    else
    {
        return -1;
    }
}