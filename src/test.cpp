#include"test.h"
#include"pnp.h"
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

vector<vector<Point>> find_approxcontour(const Mat &image){    //多边形轮廓
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



void huizhi(Mat & image,vector<Point2f> rect_point,vector<Point2f> tri_point){    //绘制找到的点
    vector<Point2f> rect_p=rect_point;
    vector<Point2f> tri_p=tri_point;

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
    
    
}

void play_videos(string videoPath){
    cv::VideoCapture videoCapture(videoPath);
    // 检查VideoCapture是否成功初始化
    if (!videoCapture.isOpened()) {
        std::cerr << "Could not open the video capture." << std::endl;
    }

    cv::Mat frame;
    while (true) {
        // 读取视频帧
        videoCapture >> frame;
        if (frame.empty()) break; // 如果帧为空，则退出循环

        // 显示帧
        cv::imshow("Video", frame);

        // 等待30毫秒或直到用户按下任意键
        if (cv::waitKey(30) >= 0) break;
    }

    // 释放VideoCapture资源
    videoCapture.release();

    // 关闭所有窗口
    cv::destroyAllWindows();


}


void make_videos(string folder_Path){
    std::string folderPath = folder_Path;
    std::vector<std::string> imagePaths;

    // 获取文件夹中所有图片的路径
    std::string filePath;
    std::string fileName;
    std::vector<std::string> files;
    cv::glob(folderPath, files);
    for (const auto& file : files) {
        fileName = file;
        imagePaths.push_back(fileName);
    }

    // 视频输出参数
    std::string videoPath = "/mnt/b/wsl/LabelRoboMasterTJU/新建文件夹/videos/output.avi"; // 视频文件路径
    int fps = 25; // 帧率
    cv::Size frameSize; // 视频帧尺寸

    // 读取第一张图片以获取尺寸
    cv::Mat firstImage = cv::imread(imagePaths[0], cv::IMREAD_COLOR);
    if (firstImage.empty()) {
        std::cerr << "Could not read the image: " << imagePaths[0] << std::endl;
    }
    frameSize = firstImage.size();

    // 创建VideoWriter对象
    cv::VideoWriter videoWriter(videoPath, cv::VideoWriter::fourcc('M','J','P','G'), fps, frameSize);

    // 检查VideoWriter是否成功初始化
    if (!videoWriter.isOpened()) {
        std::cerr << "Could not open the video writer." << std::endl;
    }

    // 按顺序处理每张图片并写入视频
    for (const auto& imagePath : imagePaths) {
        cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR); // 读取图片

        if (image.empty()) {
            std::cerr << "Could not read the image: " << imagePath << std::endl;
            continue;
        }

        // 在这里对image进行处理
        judge_make(image,find_approxcontour(yuchuli(image)));

        // 写入图片到视频
        videoWriter.write(image);
    }

    // 释放VideoWriter资源
    videoWriter.release();

}