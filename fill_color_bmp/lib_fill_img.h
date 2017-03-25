#ifndef LIB_FILL_IMG_H
#define LIB_FILL_IMG_H
#include <iostream>
#include <ctime>
#include <opencv/cv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

bool img_flood_fill(const char* in_fn, const char* out_fn,
                         const CvPoint seed, const CvScalar color,
                         const double accuracy);

IplImage *img_flood_fill_raw(const char* in_fn,
                         const CvPoint seed, const CvScalar color,
                         const double accuracy);

#endif // LIB_FILL_IMG_H
