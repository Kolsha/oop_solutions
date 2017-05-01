#include <fstream>
#include "srtparser.h"


int main(int argc, char **argv){
    if (argc >= 2){
        SrtParser *myParser = new SrtParser();
        try{
            std::ifstream input(argv[1]);
            if (myParser->get_data(input)) {
                std::ofstream output(argv[2]);
                myParser->make_out_file(output);
            }
            delete myParser;
            return 0;
        }
        catch(...){
            return 2;
        }
    }
    else{
        return 1;
    }

}


