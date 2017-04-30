#ifndef SRTPARSER_H
#define SRTPARSER_H
#include <iostream>
#include <map>
struct SubTitle
{
    double tm_end;
    std::string text;
};

class SrtParser
{
private:
    std::map<double, SubTitle> subs;
    double sub_duration;
    double parse_time(const std::string &str);
public:
    SrtParser();
    SrtParser(std::istream &data);
    size_t parse(std::istream &data);
    void show_subtitles(std::ostream &out);
    void show_subtitles(std::ostream &out,
                        double start,
                        double end);
    inline double duration(){
        return sub_duration;
    }
};

#endif // SRTPARSER_H
