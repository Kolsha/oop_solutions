#include <iostream>
#include <fstream>
#include "srtparser.h"
using namespace std;

int main(int argc, char *argv[])
{

    ifstream strstream;
    strstream.open("data.txt");
    if(!strstream.is_open()){
        return 1;
    }
    SrtParser parser(strstream);
    parser.show_subtitles(cout);
    return 0;
}
