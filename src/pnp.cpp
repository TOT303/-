#include "pnp.h"
#include "test.h"

class waican {
public:
    vector<cv::Point3f> z_objectPoints = {{120, 120, 0}, {120, -120, 0}, {-120, -120, 0}, {-120, 120, 0}};
    vector<cv::Point3f> left_objectPoints = {{-120, 100, -145.5}, {-120, 0, -45.5}, {-120, -100, -145.5}};
    vector<cv::Point3f> right_objectPoints = {{120, 100, -145.5}, {120, 0, -45.5}, {120, -100, -145.5}};
    Mat cameraMatrix = (Mat_<double>(3, 3) << 1.521928836685752e+03, 0, 9.504930579948792e+02, 0, 1.521695508574793e+03, 6.220985582938733e+02, 0, 0, 1);
    Mat distCoeffs = (Mat_<double>(4, 1) << -0.211610906204210, 0.184925401993397, -8.448907659021592e-04, 6.430996918483068e-04, 0.003766512014423);
};

bool zheng(vector<Point2f> p) {
    return p.size() == 4;
}

bool ce(vector<Point2f> p) {
    return p.size() == 3;
}

bool left(vector<Point2f> r_p, vector<Point2f> t_p) {
    float r_sum = 0;
    float t_sum = 0;
    for (int i = 0; i < r_p.size(); i++) {
        r_sum += r_p[i].x;
    }
    for (int j = 0; j < t_p.size(); j++) {
        t_sum += t_p[j].x;
    }
    return r_sum / 4 > t_sum / 3;
}

vector<Mat> pnp(vector<Point2f> rect_imagePoint) {
    vector<Mat> vec;
    waican wai;
    vector<cv::Point3f> z_objectPoints = wai.z_objectPoints;
    Mat cameraMatrix = wai.cameraMatrix;
    Mat distCoeffs = wai.distCoeffs;
    cv::Mat rvec, tvec;

    solvePnP(z_objectPoints, rect_imagePoint, cameraMatrix, distCoeffs, rvec, tvec);

    vec.push_back(rvec);
    vec.push_back(tvec);

    return vec;
}

vector<Mat> pnp(vector<Point2f> rect_imagePoint, vector<Point2f> triangle_imagePoint) {
    waican wai;
    vector<cv::Point3f> z_objectPoints = wai.z_objectPoints;
    vector<cv::Point3f> left_objectPoints = wai.left_objectPoints;
    vector<cv::Point3f> right_objectPoints = wai.right_objectPoints;
    Mat cameraMatrix = wai.cameraMatrix;
    Mat distCoeffs = wai.distCoeffs;
    cv::Mat rvec, tvec;
    vector<cv::Point2f> r_imagePoints = rect_imagePoint;
    vector<Point2f> t_imagePoints = triangle_imagePoint;
    vector<Mat> vec;

    vector<cv::Point3f> obj;
    vector<Point2f> imgp;
    imgp.reserve(r_imagePoints.size() + t_imagePoints.size());
    obj.reserve(z_objectPoints.size() + left_objectPoints.size());

    for (int i = 0; i < r_imagePoints.size(); i++) {
        imgp.push_back(r_imagePoints[i]);
        obj.push_back(z_objectPoints[i]);
    }

    if (left(r_imagePoints, t_imagePoints)) {
        imgp.insert(imgp.end(), t_imagePoints.begin(), t_imagePoints.end());
        obj.insert(obj.end(), left_objectPoints.begin(), left_objectPoints.end());
        if (obj.size() != imgp.size() || imgp.size() < 4) {
            return vec;
        }
        try {
            solvePnP(obj, imgp, cameraMatrix, distCoeffs, rvec, tvec);
        } catch (const cv::Exception& e) {
            cout << "Error: " << e.what() << endl;
            return vec;
        }
    }
        
    
    else {
        imgp.insert(imgp.end(), t_imagePoints.begin(), t_imagePoints.end());
        obj.insert(obj.end(), right_objectPoints.begin(), right_objectPoints.end());
        if (obj.size() != imgp.size() || imgp.size() < 4) {
            return vec;
        }
        try {
            solvePnP(obj, imgp, cameraMatrix, distCoeffs, rvec, tvec);
        } catch (const cv::Exception& e) {
            cout << "Error: " << e.what() << endl;
            return vec;
        }
    }
    vec.push_back(rvec);
    vec.push_back(tvec);

    return vec;
}

void drawCoordinateSystem(Mat& image, const Mat& rvec, const Mat& tvec, const Mat& cameraMatrix, const Mat& distCoeffs, float length) {
    vector<Point3f> axes = { Point3f(length, 0, 0), Point3f(0, length, 0), Point3f(0, 0, length) };
    vector<Point2f> imageAxes;
    projectPoints(axes, rvec, tvec, cameraMatrix, distCoeffs, imageAxes);

    vector<Point3f> origin3D = { Point3f(0, 0, 0) };
    vector<Point2f> origin2D;
    projectPoints(origin3D, rvec, tvec, cameraMatrix, distCoeffs, origin2D);

    Point2f origin = origin2D[0];

    line(image, origin, imageAxes[0], Scalar(0, 0, 255), 2);
    line(image, origin, imageAxes[1], Scalar(0, 255, 0), 2);
    line(image, origin, imageAxes[2], Scalar(255, 0, 0), 2);
}

bool arePointsCollinear(const vector<Point2f>& points, double epsilon = 1e-6) {
    if (points.size() < 3) {
        return false;
    }

    for (size_t i = 0; i < points.size() - 2; ++i) {
        Point2f A = points[i];
        Point2f B = points[i + 1];
        Point2f C = points[i + 2];

        double crossProduct = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);

        if (fabs(crossProduct) > epsilon) {
            return false;
        }
    }

    return true;
}

void judge_make(Mat image, vector<vector<Point>> contours) {
    vector<Point2f> rect_p = find_rectpoint(contours);
    vector<Point2f> tri_p = find_tripoint(contours);
    huizhi(image, rect_p, tri_p);
    vector<Mat> vec;
    waican wai;
    Mat cameraMatrix = wai.cameraMatrix;
    Mat distCoeffs = wai.distCoeffs;

    if (zheng(rect_p) && ce(tri_p)) {
        if (arePointsCollinear(rect_p) || arePointsCollinear(tri_p)) {
            cout << "Error: Points are collinear." << endl;
            return;
        }
        vec = pnp(rect_p, tri_p);
        if (!vec.empty()) {
            drawCoordinateSystem(image, vec[0], vec[1], cameraMatrix, distCoeffs);
        }
    } 
    else if (zheng(rect_p)) {
        if (arePointsCollinear(rect_p)) {
            cout << "Error: Points are collinear." << endl;
            return;
        }
        vec = pnp(rect_p);
        if (!vec.empty()) {
            drawCoordinateSystem(image, vec[0], vec[1], cameraMatrix, distCoeffs);
        }
    }
}