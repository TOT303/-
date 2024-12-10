#ifndef _TEST_H_
#define _TEST_H_
#include<opencv2/opencv.hpp>
#include<cmath>
using namespace std;
using namespace cv;

Mat yuchuli(const Mat &image);

vector<vector<Point>> find_approxcontour(const Mat &image);

void show(string name,const Mat &image);

vector<vector<Point>> xiuxing(vector<vector<Point>> c,int t=15);

Point2f find_basepoint(vector<vector<Point>> c);

Point2f zhixin(vector<Point> &contour);

float calcu_angle(Point AB,Point AC);

vector<float> find_angle(vector<Point2f> &p,Point2f base_point);

vector<Point2f> find_tripoint(vector<vector<Point>> &c);

vector<Point2f> find_rectpoint(vector<vector<Point>> &c);

void huizhi(Mat & image,vector<Point2f> rect_point,vector<Point2f> tri_point);

void make_videos(string folder_Path);
void play_videos(string videoPath);
#endif
