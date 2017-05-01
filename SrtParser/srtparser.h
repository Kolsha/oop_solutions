#ifndef SRTPARSER_H
#define SRTPARSER_H
#include <iostream>
#include <vector>

struct SubTitle{
    std::vector<std::string> all_not_hide_phrases;
    std::vector<std::string> new_phrase;
    float start_time;
    float end_time;

};

class SrtParser {
private:
    static const std::string SHOW_CMD;
    static const std::string HIDE_CMD;
    float sub_duration;
    std::vector<SubTitle> subs;
    void hide_phrases();
    void put_in_order();
    bool make_time_in_seconds(float &time, std::string const &str, int const &multiplier, size_t &pos);
    std::string make_time_hh_mm_ss(float &time);

public:
    SrtParser():sub_duration(0){}
    SrtParser(std::istream &in, bool ordered = false);
    ~SrtParser(){}
    size_t get_data(std::istream &in);
    std::vector<std::pair<float, std::vector<std::string>>> make_out_file(std::ostream &out);
    inline double duration(){
        return sub_duration;
    }
};
#endif // SRTPARSER_H
