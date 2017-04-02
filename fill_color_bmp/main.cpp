#include <iostream>
#include <iostream>
#include <fstream>

#include "lib_fill_img.h"

using namespace std;

int main( int argc, char** argv )
{
    if(argc != 2){
        cout << "Give me file" << endl;
        return 1;
    }
    string filename = string(argv[1]);
    std::ifstream ifs(filename);

    if(!getline(ifs, filename)){
        cout << "Empty file" << endl;
        return 1;
    }

    int x, y, r, g, b;
    try{
        ifs >> x >> y >> r >> g >> b;
    }
    catch(...){
        ifs.close();
        cout << "Bad file" << endl;
        return 1;
    }
    ifs.close();

    bool res = img_flood_fill(filename.c_str(),
                              (filename + "_filled.bmp").c_str(),
                              CvPoint(x,y),
                              CV_RGB(r, g, b),
                              10);
    if(res){
        cout << "Success" << endl;
        return 0;
    }else{
        cout << "Fail" << endl;
        return 1;
    }
}
