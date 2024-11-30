#include"test.h"
#include<iostream>

int main(){
    Mat image=imread("/mnt/b/wsl/LabelRoboMasterTJU/新建文件夹/image/20211815921.jpg");
    // show("原图",image);
    Mat dst=yuchuli(image);
    // show("1",dst);
    vector<vector<Point>> contours;
    contours=find_contour(dst);
    // xiuxing(contours);
    Mat contourimg=Mat::zeros(dst.rows, dst.cols, CV_8UC3);
    drawContours(contourimg,contours,-1,Scalar(255,255,255),1);

    // show("contour",contourimg);

    vector<Point2f> rect_p=find_rectpoint(contours);



    vector<Point2f> tri_p=find_tripoint(contours);

    for (int i=0;i<rect_p.size();i++){
        circle(image,rect_p[i], 5, cv::Scalar(0, 255, 0), -1);
        string i_=to_string(i);
        putText(image,i_,rect_p[i],1.0,2,Scalar(255,255,255));
    }

    for (int i=0;i<tri_p.size();i++){
        string i_=to_string(i);
        circle(image,tri_p[i], 5, cv::Scalar(255, 255, 0), -1);
        putText(image,i_,tri_p[i],1.0,2,Scalar(255,255,255));
    }
    
    show("rect_p",image);

    
    return 0; 
}