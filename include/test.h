#ifndef _TEST_H_
#define _TEST_H_
#include<opencv2/opencv.hpp>
#include<cmath>
using namespace std;
using namespace cv;

Mat yuchuli(const Mat &image);
Mat chuli(const Mat &image);
vector<vector<Point>> find_contour(const Mat &image);
void show(string name,const Mat &image);
void xiuxing(vector<vector<Point>>& c);
vector<vector<Point>> find_point(const Mat &gray_image);
Mat lianxian(vector<vector<Point>> p);
Point2f zhixin(vector<Point> &contour);
float calcu_angle(Point AB,Point AC);
vector<float> find_angle(vector<vector<Point>> &contours);
void swap_contour(vector<vector<Point>> &c);
vector<Point2f> find_tripoint(vector<vector<Point>> &c);
vector<Point2f> find_rectpoint(vector<vector<Point>> &c);
#endif
