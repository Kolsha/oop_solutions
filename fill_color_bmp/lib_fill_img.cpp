#include <iostream>
#include <vector>
#include <cmath>
#include "lib_fill_img.h"

namespace{

bool compare_color(const cv::Scalar &a, const cv::Scalar &b,
                   const double accuracy){

    bool res = true;
    for(size_t i = 0; i < 3; i++){
        double tmp = abs(a.val[i] - b.val[i]);
        res = res && (tmp <= accuracy);
        if(!res){
            break;
        }
    }

    return res;
}

bool operator ==(const cv::Scalar &a, const cv::Scalar &b)
{
    return compare_color(a, b, a.val[3]);
}

bool operator !=(const cv::Scalar &a, const cv::Scalar &b)
{
    return !compare_color(a, b, a.val[3]);
}

bool in_area(const IplImage *res, const CvPoint pos){
    return !(res->width <= pos.x ||
             res->height <= pos.y
             || pos.x < 0
             || pos.y <0);
}



CvScalar get_color_at(const IplImage *res, const CvPoint pos,
                      const double acc){

    if(res == nullptr || res->imageData == nullptr){
        return CV_RGB(-1,-1,-1);
    }

    if(!in_area(res, pos)){
        return CV_RGB(-1,-1,-1);
    }

    uchar* ptr = (uchar*)(res->imageData + (pos.y * res->widthStep));


    CvScalar color = CV_RGB(ptr[3 * pos.x + 2],
            ptr[3 * pos.x + 1], ptr[3 * pos.x + 0]);
    color.val[3] = acc;
    return color;
}

void set_color_at(IplImage *res, const CvPoint pos, const CvScalar color){
    if(res == nullptr || res->imageData == nullptr){
        return ;
    }

    if(!in_area(res, pos)){
        return ;
    }

    uchar* ptr = (uchar*)(res->imageData + (pos.y * res->widthStep));

    ptr[3 * pos.x + 2] = color.val[0];
    ptr[3 * pos.x + 1] = color.val[1];
    ptr[3 * pos.x + 0] = color.val[2];

}

CvPoint point_add(CvPoint p, int dx, int dy){
    return CvPoint(p.x + dx, p.y + dy);
}

}


bool img_flood_fill(const char* in_fn, const char* out_fn,
                         const CvPoint seed, const CvScalar color,
                         const double accuracy){

    if(in_fn == nullptr || out_fn == nullptr){
        return false;
    }

    IplImage *res = img_flood_fill_raw(in_fn, seed, color, accuracy);
    if(res == nullptr){
        return false;
    }

    /*cvNamedWindow("window", CV_WINDOW_AUTOSIZE);
    cvShowImage("window", res);
    cvWaitKey(0);

    cvReleaseImage(&res);
    cvDestroyWindow("window");
    */
    try{
        cvSaveImage(out_fn, res);
        cvReleaseImage(&res);
        return true;
    }
    catch(...){
        cvReleaseImage(&res);
        return false;
    }
    cvReleaseImage(&res);

    return false;
}


IplImage *img_flood_fill_raw(const char* in_fn,
                         const CvPoint seed, const CvScalar color,
                         const double accuracy){

    if(in_fn == nullptr){
        return nullptr;
    }

    IplImage *res = cvLoadImage(in_fn, 1);
    if(res == nullptr || !in_area(res, seed)){
        delete res;
        return nullptr;
    }

    std::vector<CvPoint> points;
    points.push_back(seed);

    CvScalar clr_from = get_color_at(res, seed, accuracy);

    while(!points.empty()){

        CvPoint pos = (CvPoint)points.back();
        points.pop_back();
        if(!in_area(res, pos)){
            continue;
        }

        CvScalar clr_tmp = get_color_at(res, pos, accuracy);

        if(clr_tmp != clr_from){
            continue;
        }

        set_color_at(res, pos, color);

        CvPoint runner = point_add(pos, 1, 0);


        while(get_color_at(res, runner, accuracy) == clr_from){
            set_color_at(res, runner, color);

            points.push_back(point_add(runner, 0, 1));
            points.push_back(point_add(runner, 0, -1));

            runner.x++;
        }

        runner = point_add(pos, -1, 0);


        while(get_color_at(res, runner, accuracy) == clr_from){
            set_color_at(res, runner, color);

            points.push_back(point_add(runner, 0, 1));
            points.push_back(point_add(runner, 0, -1));

            runner.x--;
        }


    }

    return res;
}
