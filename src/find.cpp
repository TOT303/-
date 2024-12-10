#include"test.h"



float calcu_angle(Point AB,Point AC){
    double dotProduct = AB.x * AC.x + AB.y * AC.y;
    double crossProduct = AB.x*AC.y-AB.y*AC.x;

    double cosTheta = dotProduct / (norm(AB) * norm(AC)); // 计算角度
    if (cosTheta <= -1 || cosTheta >= 1) {
        return 0.0; 
    }
    double angle = acos(cosTheta);
    angle = angle * 180.0 / CV_PI;


    if (crossProduct<0){
        angle=360.0-angle;
    }
    return angle;
}

Point2f find_basepoint(vector<vector<Point>> c) {
    Point2f base_p = {0, 0};
    float min_area = MAXFLOAT;

    // 创建空白图像
    int width = 2000;  // 根据需要调整宽度
    int height = 2000; // 根据需要调整高度
    Mat blankImage(height, width, CV_8UC3, Scalar(0, 0, 0));

    for (int i=0;i<c.size();i++) {
        if (c[i].size() < 3) {
            continue;
        }

        RotatedRect minRect = cv::minAreaRect(c[i]);
        float area = minRect.size.width * minRect.size.height;

        if (area < min_area) {
            min_area = area;
            base_p = zhixin(c[i]);
        }
        //绘制外接矩形
    //     Point2f rect_points[4];
    //     minRect.points(rect_points);
    //     for (int j = 0; j < 4; j++) {
    //         line(blankImage, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 255, 0), 2);
    //     }
    
    // }
    // show("blankImage",blankImage);
    }
    return base_p;
    
}
    




vector<float> find_angle(vector<Point2f> &p,Point2f base_point){    //找轮廓质心的角度
    vector<float> angles;
    Point2f center;
    Point2f base_p=base_point;
    Point2f AB,AC;
    double sum1,sum2;
    sum1=sum2=0;
    for (int i=0;i<p.size();i++){
        sum1+=p[i].x;
        sum2+=p[i].y;
    }
    center.x=sum1/p.size();
    center.y=sum2/p.size();
    for (int j=0;j<p.size();j++){
        AB=base_p-center;
        AC=p[j]-center;
        float angle;
        angle=calcu_angle(AB,AC);
        angles.push_back(angle);
    }
    return angles;
}  




Point2f zhixin(vector<Point> &contour){           //找质心

    Point2f points;
    Moments m = moments(contour);
    double x_c = m.m10 / m.m00; // 质心x坐标
    double y_c = m.m01 / m.m00; // 质心y坐标  
    points.x=x_c;
    points.y=y_c;
    
    return points;
}


vector<vector<Point>> xiuxing(vector<vector<Point>> c,int t){   //修形
    vector<vector<Point>> x_c;
    for (int i=0;i<c.size();i++){
        vector<Point> v=c[i];
        approxPolyDP(c[i],v,t,true);
        x_c.push_back(v);
    }
    
    return x_c;
}




vector<Point2f> find_tripoint(vector<vector<Point>> &c){
    vector<vector<Point>> x_c=xiuxing(c);
    vector<Point> t_contour;
    // Mat contourimg=Mat::zeros(800,800, CV_8UC3);
    // drawContours(contourimg,x_c,-1,Scalar(255,255,255),1);
    // show("contour",contourimg);

    for (int i=0;i<x_c.size();i++){
        if (x_c[i].size()==4){
            if (norm(x_c[i][0] - x_c[i][1])>=70||norm(x_c[i][1] - x_c[i][2])>=70){ //确定三角所在的轮廓
                t_contour=x_c[i]; 
            }
        }
    }

    Point center;
    Point t_center;

    center=zhixin(t_contour);  //三角的质心
    float mindis=INT_MAX;
    for (int j=0;j<t_contour.size();j++){
        if (norm(t_contour[j]-center)<mindis){
            mindis=norm(t_contour[j]-center);
            t_center=t_contour[j];   //离质心最近的三角形顶点
        }
    }
    mindis=INT_MAX;
    Point deleted;    //删掉其中一个点
    
    for (int k=0;k<t_contour.size();k++){
        float dis=norm((t_contour[k])-t_center);
        if (dis!=0&&mindis>dis){
            deleted=t_contour[k];
            mindis=dis;
        }
    }
    vector<Point2f> t_point;   //储存剩余点
    for (int m=0;m<t_contour.size();m++){
        if (t_contour[m]!=deleted&&t_contour[m]!=t_center){
            t_point.push_back(t_contour[m]);
        }
    }
    t_point.push_back(t_center);
    swap(t_point[1],t_point[2]);
    return t_point;
}


vector<Point2f> find_rectpoint(vector<vector<Point>> &c){
    vector<Point2f> zx;
    vector<vector<Point>> r_contours;
    for (int j=0;j<c.size();j++){
        if (norm(c[j][0] - c[j][1])>=70||norm(c[j][1] - c[j][2])>=70){ //排除三角所在的轮廓
            continue; 
        }
        r_contours.push_back(c[j]);
    }
    for (int i=0;i<r_contours.size();i++){
        zx.push_back(zhixin(r_contours[i]));
    }
    vector<float> angles;
    angles=find_angle(zx,find_basepoint(c));
    for (int k=0;k<zx.size();k++){
        for (int m=0;m<zx.size()-1;m++){
            if (angles[m]>angles[m+1]){
                swap(zx[m],zx[m+1]);
                swap(angles[m],angles[m+1]);
            }
        }
    }
    // for (int k=0;k<zx.size();k++){
    //     cout<<zx[k]<<endl;
    // }
    return zx;
}





