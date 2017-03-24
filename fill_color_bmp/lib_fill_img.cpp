#include <iostream>
#include <vector>
#include "lib_fill_img.h"

namespace{


bool operator ==(const cv::Scalar &a, const cv::Scalar &b)
{
    bool Result = false;
    // Do whatever you think a Scalar comparison must be.
    return Result;
}

CvScalar get_color_at(const IplImage *res, CvPoint pos){

    if(res == nullptr || res->imageData == nullptr){
        return CV_RGB(0,0,0);
    }

    uchar* ptr = (uchar*)(res->imageData + (pos.y * res->widthStep));

    return CV_RGB(ptr[3 * pos.x + 2],
            ptr[3 * pos.x + 1], ptr[3 * pos.x + 0]);
}

}


IplImage *img_flood_fill(const char* filename,
                         const CvPoint seed, const CvScalar color,
                         const size_t accuracy){


    IplImage *res = cvLoadImage(filename, 1);
    if(res == nullptr){
        return nullptr;
    }

    std::vector<CvPoint> points;
    points.push_back(seed);
    uchar* ptr = (uchar*)(res->imageData + (seed.y * res->widthStep));

    CvScalar clr_from = cvScalar(ptr[3 * seed.x + 2],
            ptr[3 * seed.x + 1], ptr[3 * seed.x + 0], accuracy);

    while(!points.empty()){
        CvPoint tmp_p = (CvPoint)points.back();
        ptr = (uchar*)(res->imageData + (tmp_p.y * res->widthStep));
        CvScalar tmp_c = cvScalar(ptr[3 * tmp_p.x + 2],
                ptr[3 * tmp_p.x + 1], ptr[3 * tmp_p.x + 0], accuracy);
        points.pop_back();


    }

    for( int y=0; y < res->height; y++ ) {
        uchar* ptr = (uchar*) ( res->imageData + y * res->widthStep );
        for( int x=0; x<res->width; x++ ) {
            //ptr[3*x+2] = 255; //Set red to max (BGR format)
            ptr[3*x+1] = 255;// green
        }
    }

    cvNamedWindow("window", CV_WINDOW_AUTOSIZE);
    cvShowImage("window", res);
    cvWaitKey(0);
    cvReleaseImage(&res);
    cvDestroyWindow("window");


}
