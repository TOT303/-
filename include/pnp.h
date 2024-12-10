#ifndef _PNP_H_
#define _PNP_H_
#include<opencv2/opencv.hpp>
#include<cmath>
using namespace std;
using namespace cv;

class waican;

bool zheng(vector<Point2f> p);
bool ce(vector<Point2f> p);
bool left(vector<Point2f> r_p,vector<Point2f> t_p);
vector<Mat> pnp(vector<Point2f> rect_imagePoint,vector<Point2f>triangle_imagePoint);
vector<Mat> pnp(vector<Point2f> rect_imagePoint);
void drawCoordinateSystem(Mat& image, const Mat& R, const Mat& t, const Mat& cameraMatrix, const Mat& distCoeffs, float length = 100);
void judge_make(Mat image,vector<vector<Point>> contours);
#endif