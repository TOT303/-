#include"test.h"
#include<opencv2/imgproc.hpp>
Mat yuchuli(const Mat &image){
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    // 定义红色的HSV范围
    Scalar lower_red(0, 50, 100);
    Scalar upper_red(10, 255, 255);
    Mat mask_red;
    inRange(hsv, lower_red, upper_red, mask_red);
    
    // 应用掩膜
    Mat result_red;
    bitwise_and(image,image, result_red, mask_red);


    // 显示结果
    // show("Red Mask", mask_red);
    return mask_red;
}


vector<vector<Point>> find_contour(const Mat &image){    //轮廓
    vector<vector<Point>> contour;
    findContours(image,contour,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> jinsi;
    for (int i=0;i<contour.size();i++){
        std::vector<cv::Point> approx;     // 用于存储单个轮廓近似后的点
        double epsilon = 5;                // 近似精度，可能需要根据实际情况调整
        cv::approxPolyDP(contour[i], approx, 7, true);   // 多边形近似

        if (approx.size() > 0) {        // 如果近似后的轮廓不为空，则添加到结果中
            jinsi.push_back(approx);
        }
    }
    return jinsi;
   
}

void show(string name,const Mat &image){                 //显示图片
    imshow(name,image);
    waitKey(0);
    destroyAllWindows();
}


