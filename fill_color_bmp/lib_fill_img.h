#ifndef LIB_FILL_IMG_H
#define LIB_FILL_IMG_H
#include <iostream>
#include <ctime>
#include <opencv/cv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

IplImage *img_flood_fill(const char* filename,
                         const CvPoint seed, const CvScalar color,
                         const size_t accuracy);

#endif // LIB_FILL_IMG_H
