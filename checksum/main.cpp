#include <iostream>
#include <fstream>
#include "checksum.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc <= 2){
        cout << "<название_бинарника> --calc <имя_файла_1> ... <имя_файла_N>" << endl;
        cout << "<название_бинарника> --check <имя_файла_манифеста>" << endl;
        return 1;
    } else{
        CheckSum summer;
        string cmd(argv[1]);
        if(cmd == "check"){
            ifstream fs(argv[2]);
            if(!fs.is_open()){
                cout << "Can't open file " << argv[2] << endl;
                return 3;
            }
            string line;
            while(getline(fs, line)){
                auto it = line.find_last_of(":");
                if(it == string::npos){
                    continue;
                }
                string filename = line.substr(0, it);
                string check = line.substr(++it);
                summer.add_file(filename);
                bool correct = (summer.get_checksum(filename) == check);
                cout << filename << ":";
                cout << ((correct) ? "OK" : "FAILED") << endl;
            }
            fs.close();
        }
        else if(cmd == "calc"){
            for(int i = 2; i < argc; i ++){
                summer.add_file(argv[i]);
            }

            ofstream fs("manifest.txt");
            for (auto& it : summer.get_checksums()) {
                fs << it.first << ":" << it.second << endl;
                cout << "checksum for" << it.first << " calculated " << it.second << endl;
            }
            fs.close();
        }
        else{
            cout << "Cmd wrong!" << endl;
            return 2;
        }


    }


    return 0;
}
