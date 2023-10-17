#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "my_filter.h"
#include "find_min_area.h"
#include "find_close_point.h"
#include "find_far_point.h"
#include "point_color.h"
#include "get_center_point.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

int main() {

    cv::VideoCapture cap(0); // 打开摄像头，0表示默认摄像头
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open camera" << std::endl;
        return -1;
    }

    cv::namedWindow("Color Detection", cv::WINDOW_AUTOSIZE);

    int maxRectangles = 18; // 要绘制的最大矩形数量
    std::vector<cv::Rect> rectangles; // 存储识别到的矩形框

    int A[6] = {0,0,0,0,0,0};
    int B[6] = {0,0,0,0,0,0};
    int C[6] = {0,0,0,0,0,0};
    
    while (true) {
        cv::Mat frame;
        cap >> frame; // 从摄像头读取一帧

        if (frame.empty()) {
            std::cerr << "Error: Blank frame grabbed" << std::endl;
            break;
        }

        cv::Mat hsv_frame;
        cv::cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV); // 转换为HSV颜色空间

        // 定义颜色范围
        //set_color_range();
        cv::Scalar lower_red(0, 100, 100); // 红色的下限
        cv::Scalar upper_red(10, 255, 255); // 红色的上限
        cv::Scalar lower_green(35, 100, 100); // 绿色的下限
        cv::Scalar upper_green(85, 255, 255); // 绿色的上限
        cv::Scalar lower_blue(110, 100, 100); // 蓝色的下限
        cv::Scalar upper_blue(130, 255, 255); // 蓝色的上限

        // 在HSV图像中查找颜色
        //find_color();
        cv::Mat red_mask, green_mask, blue_mask;
        cv::inRange(hsv_frame, lower_red, upper_red, red_mask);
        cv::inRange(hsv_frame, lower_green, upper_green, green_mask);
        cv::inRange(hsv_frame, lower_blue, upper_blue, blue_mask);

        // cv::imshow("blue", blue_mask);

        // 寻找颜色的轮廓
        std::vector<std::vector<cv::Point>> red_contours, green_contours, blue_contours;
        cv::findContours(red_mask, red_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::findContours(green_mask, green_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::findContours(blue_mask, blue_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        rectangles.clear(); // 清空上一帧的矩形框

        // 识别最接近图像中心的矩形框并过滤小框
        my_filter(300,red_contours,green_contours,blue_contours,rectangles);
        // for (const auto& contour : red_contours) {
        //     cv::Rect rect = cv::boundingRect(contour);
        //     if (rect.area() > 300) { // 过滤掉面积小于100的矩形框
        //         rectangles.push_back(rect);
        //     }
        // }
        // for (const auto& contour : green_contours) {
        //     cv::Rect rect = cv::boundingRect(contour);
        //     if (rect.area() > 300) { // 过滤掉面积小于100的矩形框
        //         rectangles.push_back(rect);
        //     }
        // }
        // for (const auto& contour : blue_contours) {
        //     cv::Rect rect = cv::boundingRect(contour);
        //     if (rect.area() > 300) { // 过滤掉面积小于100的矩形框
        //         rectangles.push_back(rect);
        //     }
        // }

        // 排序矩形框，根据距离图像中心的距离
        std::sort(rectangles.begin(), rectangles.end(), [&](const cv::Rect& a, const cv::Rect& b) {
            cv::Point centerA(a.x + a.width / 2, a.y + a.height / 2);
            cv::Point centerB(b.x + b.width / 2, b.y + b.height / 2);
            cv::Point imageCenter(frame.cols / 2, frame.rows / 2);

            // 计算框的几何中心到图像几何中心的距离
            double distanceA = cv::norm(centerA - imageCenter);
            double distanceB = cv::norm(centerB - imageCenter);

            return distanceA < distanceB;
        });

        for(size_t i = 0; i < rectangles.size(); i++)
        {
            if(rectangles[i].area() <= 10000)
            {
                rectangles.erase(rectangles.begin()+i);
            }
            else
            {
                i++;
            }
        }

        //std::cout<<rectangles.size()<<std::endl;

        // std::cout<<find_min_area(rectangles).area()<<std::endl;

        // 绘制前N个矩形框，N为maxRectangles
        // for (size_t i = 0; i < std::min(static_cast<size_t>(maxRectangles), rectangles.size()); i++) {
            
        //     if(rectangles[i].area() >= 10000)
        //     {
        //         cv::rectangle(frame, rectangles[i], cv::Scalar(0, 0, 255), 2);
        //         //std::cout<<rectangles[i].height<<std::endl;
        //     }           
        //     //cv::rectangle(frame, rectangles[i], cv::Scalar(0, 0, 255), 2);
        // }

        // cv::rectangle(frame, find_close_point(rectangles), cv::Scalar(0, 0, 255), 2);
        // cv::rectangle(frame, find_far_point(rectangles), cv::Scalar(0, 0, 255), 2);

        cv::Point point1(find_close_point(rectangles).x,find_close_point(rectangles).y);
        cv::Point point2(find_far_point(rectangles).x + find_far_point(rectangles).width,
        find_far_point(rectangles).y + find_far_point(rectangles).height);

        cv::Rect max_rect(point1.x,point1.y,point2.x-point1.x,point2.y-point1.y);

        int rect_width = max_rect.width/3;
        int rect_height = max_rect.height/6;

        std::vector<cv::Rect> rectangles_A,rectangles_B,rectangles_C;
        for(int i = 0; i < 6; i++)
        {
            cv::Rect it(point1.x,point1.y + i*rect_height,rect_width,rect_height);
            rectangles_A.push_back(it);
        }

        for(size_t i = 0; i < rectangles_A.size(); i++)
        {
            cv::rectangle(frame,rectangles_A[i], cv::Scalar(0, 0, 255), 2);
        }

        for(int i = 0; i < 6; i++)
        {
            cv::Rect it(point1.x+rect_width,point1.y + i*rect_height,rect_width,rect_height);
            rectangles_B.push_back(it);
        }

        for(size_t i = 0; i < rectangles_B.size(); i++)
        {
            cv::rectangle(frame,rectangles_B[i], cv::Scalar(0, 0, 255), 2);
        }

        for(int i = 0; i < 6; i++)
        {
            cv::Rect it(point1.x+2*rect_width,point1.y + i*rect_height,rect_width,rect_height);
            rectangles_C.push_back(it);
        }

        for(size_t i = 0; i < rectangles_C.size(); i++)
        {
            cv::rectangle(frame,rectangles_C[i], cv::Scalar(0, 0, 255), 2);
        }

        //cv::rectangle(frame,point1,point2,cv::Scalar(0, 0, 255), 2);

        //cv::rectangle(frame,max_rect, cv::Scalar(0, 0, 255), 2);

        // point_color A0(point1.x+0.5*rect_width,point1.y+0.5*rect_height,frame);

        // point_color B2(point1.x+1.5*rect_width,point1.y+2.5*rect_height,frame);

        // std::cout<<"green:"<<" "<<A0.green<<" "<<B2.green<<std::endl;
        // std::cout<<"blue:"<<" "<<A0.blue<<" "<<B2.blue<<std::endl;
        // std::cout<<"red:"<<" "<<A0.red<<" "<<B2.red<<std::endl;
        
        bool flag_A = false;
        bool flag_B = false;
        bool flag_C = false;

        bool flag_a = true;
        bool flag_b = true;
        bool flag_c = true;

        if(flag_A == false)
        {
            for(size_t i = 0; i < rectangles_A.size(); i++)
            {
                int center_x = get_center_point(rectangles_A[i]).x;
                int center_y = get_center_point(rectangles_A[i]).y;

                point_color p(center_x,center_y,frame);

                A[i] = p.judge_color();

                if(p.judge_color() == -1)
                {
                    flag_a = false;
                }
            }
            if(flag_a == true)
            {
                flag_A = true;
            }
            else
            {
                for(int i = 0; i<6; i++)
                {
                    A[i] = 0;
                }
            }

            if(flag_A == true && flag_B == false)
            {
                for(size_t i = 0; i < rectangles_B.size(); i++)
                {
                    int center_x = get_center_point(rectangles_B[i]).x;
                    int center_y = get_center_point(rectangles_B[i]).y;

                    point_color p(center_x,center_y,frame);

                    B[i] = p.judge_color();

                    if(p.judge_color() == -1)
                    {
                        flag_b = false;
                    }
                }
                if(flag_b == true)
                {
                    flag_B = true;
                }

                if(flag_A == true && flag_B == true && flag_C == false)
                {
                    for(size_t i = 0; i < rectangles_C.size(); i++)
                    {
                        int center_x = get_center_point(rectangles_C[i]).x;
                        int center_y = get_center_point(rectangles_C[i]).y;

                        point_color p(center_x,center_y,frame);

                        C[i] = p.judge_color();

                        if(p.judge_color() == -1)
                        {
                            flag_c = false;
                        }
                    }
                    if(flag_c == true)
                    {
                        flag_C = true;
                    }
                    else
                    {
                        for(int i = 0; i<6; i++)
                        {
                            C[i] = 0;
                        }
                    }
                }
            }
            
        }
        

        cv::imshow("Color Detection", frame);

        if (cv::waitKey(30) == 27) { 
            break;
        }

        if(flag_A == true && flag_B == true && flag_C == true)
        {
            break;
        }
        
    }

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // 创建套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // 配置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // 指定端口号
    serverAddr.sin_addr.s_addr = INADDR_ANY; // 监听所有可用接口

    // 绑定套接字到服务器地址
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        return -1;
    }

    // 开始监听连接
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Server is listening for connections..." << std::endl;

    // 接受客户端连接
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSocket == -1) {
        std::cerr << "Error accepting connection" << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Client connected" << std::endl;

    // 从客户端接收数据
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        std::cerr << "Error receiving data" << std::endl;
    } else {
        std::cout << "Received: " << buffer << std::endl;
    }

    const char* arry_a;
    const char* arry_b;
    const char* arry_c;
    const char* arry;
    std::string str_a = "";
    std::string str_b = "";
    std::string str_c = "";
    std::string str = "";
    
    for(int i = 0; i < 6; i++)
    {
        str_a += std::to_string(A[i]);
        str_b += std::to_string(B[i]);
        str_c += std::to_string(C[i]);
    }

    str = "#" + str_a + "#" + str_b + "#" + str_c;
    arry_a = str_a.c_str();
    arry_b = str_b.c_str();
    arry_c = str_c.c_str();
    arry = str.c_str();
    


    //const char* response = "Hello, client!";
    //std::string str = std::string(response);
    send(clientSocket, arry, strlen(arry), 0);
    // send(clientSocket, arry_b, strlen(arry_c), 0);
    // send(clientSocket, arry_c, strlen(arry_c), 0);
    // send(clientSocket, str, strlen(response), 0);

    // 关闭连接
    close(clientSocket);
    close(serverSocket);

    for(int i = 0; i < 6; i++)
    {
        if(i == 5)
        {
            std::cout<<A[i]<<std::endl;
        }
        else
        {
            std::cout<<A[i];
        }
        
    }

    for(int i = 0; i < 6; i++)
    {
        if(i == 5)
        {
            std::cout<<B[i]<<std::endl;
        }
        else
        {
            std::cout<<B[i];
        }
    }

    for(int i = 0; i < 6; i++)
    {
        if(i == 5)
        {
            std::cout<<C[i]<<std::endl;
        }
        else
        {
            std::cout<<C[i];
        }
        
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
