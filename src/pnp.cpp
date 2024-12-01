#include"test.h"

vector<vector<Mat>> pnp(vector<cv::Point3f> objectPoints,vector<cv::Point2f> imagePoints,Mat cameraMatrix,Mat distCoeffs){
    vector<vector<Mat>> vec;
    std::vector<cv::Point3f> objectPoint=objectPoints;  //3D点
    std::vector<cv::Point2f> imagePoint=imagePoints;   // 2D点
    cv::Mat rvec, tvec;
    bool success = cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);
    if (success) {
        // 位姿求解成功，rvec是旋转向量，tvec是平移向量
        std::cout << "Rotation Vector: " << rvec << std::endl;
        std::cout << "Translation Vector: " << tvec << std::endl;
    } else {
        // 位姿求解失败
        std::cout << "PnP problem solving failed." << std::endl;
    }
    vec.push_back(rvec);
    vec.push_back(tvec);
    return vec;
}
