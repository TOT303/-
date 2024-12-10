#include"test.h"
#include"pnp.h"
#include<iostream>

int main(){
    // Mat image=imread("/mnt/b/wsl/LabelRoboMasterTJU/新建文件夹/image/20211839746.jpg");//2021181592120211839746
    // // show("原图",image);
    // Mat dst=yuchuli(image);
    // // show("1",dst);
    // vector<vector<Point>> contours;
    // contours=find_approxcontour(dst);
    // Mat contourimg=Mat::zeros(dst.rows, dst.cols, CV_8UC3);
    // // drawContours(contourimg,contours,-1,Scalar(255,255,255),1);

    // // show("contour",contourimg);
       

    // make_videos("/mnt/b/wsl/LabelRoboMasterTJU/新建文件夹/image/station/station");
    play_videos("/mnt/b/wsl/LabelRoboMasterTJU/新建文件夹/videos/output.avi");
    
    return 0;
}