#include <fstream>
#include <iostream>
#include <map>
#include "serialize.h"

using namespace std;

int main()
{
    int a = 5;
    /*ofstream ofs("test.ser", ofstream::out | ofstream::binary);

    serialize(a, ofs);
    ofs.close();
    */

    ifstream ifs("test.ser", ifstream::in | ofstream::binary);
    ifs >> noskipws;

    deserialize(a, ifs);
    cout << "Deser res:" << a << endl;

    ifs.close();
    /*string hello = "hello!";
    int boo[3] = { 1, 2, 3 };
    string str = "aaaaa";
    vector<int> vec = {1,2,3,4,5};
    map<string, string> my_map = {
        {"map", "test"}
    };

    map<string, vector<int>> vec_map = {
        {"map", {1,2,3,4}}
    };

    serialize(hello, ofs);

    serialize(boo, ofs);

    serialize(str, ofs);
    serialize(vec, ofs);
    serialize(my_map, ofs);
    serialize(vec_map, ofs);
    return 0;
    /*

    char hello2[7];
    int boo2[3];
    string str2;
    vector<int> vec2;
    map<string, string> my_map2;
    map<string, vector<int>> vec_map2;


    ofs.close();

    ifstream ifs("test.ser", ifstream::in | ofstream::binary);
    ifs >> noskipws;

    deserialize(hello2, ifs);
    deserialize(boo2, ifs);
    /*deserialize(str2, ifs);
    deserialize(vec2, ifs);
    deserialize(my_map2, ifs);
    deserialize(vec_map2, ifs);*/

    /*if (equal(hello, hello + 7, hello2))
        cout << "Hello OK!" << endl;

    if (equal(boo, boo + 3, boo2))
        cout << "Boo OK!" << endl;

   /*if (equal(str.begin(), str.end(), str2.begin()))
        cout << "Str OK!" << endl;

    if (equal(vec.begin(), vec.end(), vec2.begin()))
        cout << "Vec OK!" << endl;

    if (equal(my_map.begin(), my_map.end(), my_map2.begin()))
        cout << "Map OK!" << endl;

    if (equal(vec_map.begin(), vec_map.end(), vec_map2.begin()))
        cout << "VecMap OK!" << endl;
        */


    return 0;
}
