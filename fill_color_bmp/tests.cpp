#include <opencv/cv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

#include "tests.h"
#include "gtest/gtest.h"
#include "lib_fill_img.h"

bool equal_img(IplImage *source1, IplImage *source2) {
    cv::Mat mat1 = cv::cvarrToMat(source1);
    cv::Mat mat2 = cv::cvarrToMat(source2);
    cv::Mat D;
    absdiff(mat1, mat2, D);
    cv::Scalar s = sum(D);
    return s == cv::Scalar::all(0);
}

IplImage *cv_flood_fill(const char* in_fn,
                        const CvPoint seed, const CvScalar color){
    IplImage *res = cvLoadImage(in_fn, 1);
    CvConnectedComp comp;

    cvFloodFill( res, seed, color,
                 cvScalarAll(0), // минимальная разность
                 cvScalarAll(0), // максимальная разность
                 &comp,
                 CV_FLOODFILL_FIXED_RANGE + 8,
                 0);
    return res;
}

TEST(FloodTest, FirstTest) {

    std::string files[2] =
    {"/home/kolsha/Projects/oop_solutions/fill_color_bmp/test_imgs/chessboard.png",
     "/home/kolsha/Projects/oop_solutions/fill_color_bmp/test_imgs/123123123.png"};

    for(size_t i = 0; i < 2; i++){
        CvPoint seed(20, 120);
        CvScalar color = CV_RGB(0, 255, 0);

        IplImage *myFill = img_flood_fill_raw(files[i].c_str(),
                                              seed, color,  0);

        ASSERT_TRUE(myFill != nullptr);

        IplImage *cvFill = cv_flood_fill(files[i].c_str(),
                                         seed, color);

        ASSERT_TRUE(cvFill != nullptr);

        cvSaveImage((files[i] + "_cv_.png").c_str(), cvFill);
        cvSaveImage((files[i] + "_my_.png").c_str(), myFill);

        ASSERT_TRUE(equal_img(cvFill, myFill));


        cvReleaseImage(&cvFill);
        cvReleaseImage(&myFill);
    }

}
