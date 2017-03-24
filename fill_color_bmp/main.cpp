#include <iostream>
#include <iostream>
#include <ctime>
#include <opencv/cv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>

#include "lib_fill_img.h"

using namespace std;
int main( int argc, char** argv )
{



    std::srand(unsigned(std::time(0)));
    cv::CommandLineParser parser(argc, argv, "{help h | |}{@image|../data/lena.jpg|}");
    if (parser.has("help"))
    {

        return 0;
    }
    string filename = parser.get<string>("@image");

    IplImage *res = img_flood_fill(filename.c_str(),
                             CvPoint(200, 198), CV_RGB(0, 255, 0),
                             10);
    return 0;
}
