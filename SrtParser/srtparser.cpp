#include "srtparser.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

const string SrtParser::SHOW_CMD = "Show";
const string SrtParser::HIDE_CMD = "Hide";


bool cmp_start_time(SubTitle const &s1, SubTitle const &s2){
    return s1.start_time < s2.start_time;
}


void ignor(istream &in){
    string str;
    getline(in, str);
    for (int i = 1; getline(in, str); i++){
        if (str.empty())
            break;
    }
}


SrtParser::SrtParser(istream &in, bool ordered):
    sub_duration(0)
{
    if(get_data(in)){
        if(ordered){
            put_in_order();
        }
    }

}


bool SrtParser::make_time_in_seconds(float &time, string const &str, int const &multiplier, size_t &pos){
    const char * s = str.c_str();
    char * end;
    int t = strtol(s+pos, &end, 10);
    pos = (end - s) / sizeof(char);
    pos++;
    if (t < 0)
        return false;
    time += t * multiplier;
    if (1 == multiplier) {
        const char *s = str.c_str();
        char *end;
        time += strtol(s + pos, &end, 10) / 1000.0;
        pos = (end - s) / sizeof(char);
        if (time < 0){
            return false;
        }

    }
    return true;
}


size_t SrtParser::get_data(istream &in){
    subs.clear();

    while (!in.eof()){
        string str;
        getline(in, str);
        getline(in, str);
        if (str.empty()) {
            return subs.size();
        }
        float start_time = 0;
        size_t pos = 0;
        if (!make_time_in_seconds(start_time, str, 3600, pos))
            ignor(in);
        else if (!make_time_in_seconds(start_time, str, 60, pos))
            ignor(in);
        else if (!make_time_in_seconds(start_time, str, 1, pos))
            ignor(in);
        else {
            subs.push_back({ {}, {}, start_time, 0 });
            size_t i = subs.size() - 1;
            pos = pos + 4;
            float end_time = 0;
            if (!make_time_in_seconds(end_time, str, 3600, pos))
                ignor(in);
            else if (!make_time_in_seconds(end_time, str, 60, pos))
                ignor(in);
            else if (!make_time_in_seconds(end_time, str, 1, pos))
                ignor(in);
            else {
                if(end_time > sub_duration){
                    sub_duration = end_time;
                }
                subs[i].end_time = end_time;
                subs[i].all_not_hide_phrases.push_back(SrtParser::SHOW_CMD);
                for (int n = 0; getline(in, str); n++){
                    if (str.empty()){
                        break;
                    }

                    subs[i].new_phrase.push_back(str);
                }

            }
        }
    }


    return subs.size();
}


void SrtParser::hide_phrases()
{
    size_t i = 1;
    for (; i < subs.size() + 2; i = i + 2){
        subs.insert(subs.begin() + i, { { SrtParser::HIDE_CMD }, {}, subs[i - 1].end_time, 0 });
        subs[i].all_not_hide_phrases.insert(subs[i].all_not_hide_phrases.end(),
                                            subs[i - 1].new_phrase.begin(), subs[i - 1].new_phrase.end());
    }

    sort(subs.begin(), subs.end(), cmp_start_time);
    for (size_t i = 1; i < subs.size(); i++) {
        if (subs[i].end_time < 0.0001){ //if hide
            subs[i].new_phrase.insert(subs[i].new_phrase.end(), { SrtParser::SHOW_CMD });
            auto it = subs[i].new_phrase.end();
            for (size_t j = 0; j < i; j++){
                if (subs[i].start_time < subs[j].end_time){
                    subs[i].new_phrase.insert(it,
                                              subs[j].new_phrase.begin(), subs[j].new_phrase.end());
                    it = subs[i].new_phrase.end();
                }
            }
        }
    }
}


void SrtParser::put_in_order(){
    sort(subs.begin(), subs.end(), cmp_start_time);
    size_t size = subs.size();
    for (size_t i = 1; i < size; i++){
        auto it = subs[i].all_not_hide_phrases.end();
        for (size_t j = 0; j < i; j++){
            if (subs[i].start_time < subs[j].end_time){
                subs[i].all_not_hide_phrases.insert(it,
                                                    subs[j].new_phrase.begin(), subs[j].new_phrase.end());
                it = subs[i].all_not_hide_phrases.end();
            }
        }
    }
    hide_phrases();
    sort(subs.begin(), subs.end(), cmp_start_time);
}


string SrtParser::make_time_hh_mm_ss(float &time){
    int hh = int(time + 0.5) / 3600;
    int mm = int(time - hh * 3600 + 0.5) / 60;
    float ss = time - hh * 3600 - mm * 60;
    ostringstream str;
    str << hh << ':' << mm << ':' << setprecision(4) << ss << ' ';
    return str.str();
}


vector<pair<float, vector<string>>> SrtParser::make_out_file(ostream &out){

    put_in_order();

    vector< pair< float, vector<string> > > result;
    size_t size = subs.size();

    result.resize(size);
    for (size_t k = 0; k < size; k++){
        string time = make_time_hh_mm_ss(subs[k].start_time);
        out << "At " << time;
        result[k].first = subs[k].start_time;
        size_t not_hide_size = subs[k].all_not_hide_phrases.size();
        result[k].second.resize(size+not_hide_size);
        size_t j = 0;
        bool show = false;
        for (; j < not_hide_size; j++){
            if(!show && subs[k].all_not_hide_phrases[j] != SrtParser::SHOW_CMD){
                continue;
            }
            show = true;
            out << subs[k].all_not_hide_phrases[j];
            out << endl;
            result[k].second[j] = subs[k].all_not_hide_phrases[j];
        }
        size_t new_ph_size = subs[k].new_phrase.size();
        result[k].second.resize(size + not_hide_size+new_ph_size);
        for (size_t i = 0; i < new_ph_size; i++){
            out << subs[k].new_phrase[i];
            result[k].second[j + i] = subs[k].new_phrase[i];
            if ((subs[k].new_phrase[0] == SrtParser::SHOW_CMD) && (subs[k].new_phrase.size() <= 1)){
                out << " ''";
                j++;
                result[k].second[j + i] = " ''";
            }
            out << endl;
        }
        out << endl;
    }
    return result;
}
