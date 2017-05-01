#include "srtparser.h"
#include <sstream>
#include <string>
#include <regex>

using namespace std;

SrtParser::SrtParser():
    sub_duration(0)
{

}

SrtParser::SrtParser(istream &data):
    sub_duration(0)
{
    parse(data);
}





double SrtParser::parse_time(const string &str){

    if(str.empty()){
        return -1;
    }

    const std::regex re("(\\d{1,3}):(\\d{1,3}):(\\d{1,3}),(\\d{1,3})");

    double ms = 0, s = 0, m = 0, h = 0, sum = 0;
    std::smatch match;
    if(std::regex_search(str, match, re)) {
        h = atof(match.str(1).c_str());
        m = atof(match.str(2).c_str());
        s = atof(match.str(3).c_str());
        ms = atof(match.str(4).c_str());
        sum += h * 60 * 60;
        sum += m * 60;
        sum += s;
        sum += ms / 1000;
        return sum;
    }

    return -1;
}









size_t SrtParser::parse(std::istream &data){
    subs.clear();
    map<double, double> times;

    while(!data.eof()){
        int num = -1;
        data >> num;
        string time_start, time_end;
        data >> time_start >> time_end >> time_end;

        if(time_start.empty() || time_end.empty()){
            continue;
        }

        double tm_start = parse_time(time_start);
        double tm_end = parse_time(time_end);
        if(tm_start >= tm_end ||
                tm_start < 0 || tm_start < 0){
            continue;
        }

        string text("");
        size_t empty_line_count = 0;
        for(;;){
            string tmp_text;
            getline(data, tmp_text);


            if(tmp_text.empty()){
                empty_line_count++;
            }else{
                text += (!text.empty() ? "\n" : "")
                        + tmp_text;
            }

            if(empty_line_count >= 2){
                break;
            }
        }

        if(text.empty()){
            continue;
        }

        if(tm_end > sub_duration){
            sub_duration = tm_end;
        }

        SubTitle sub;
        sub.text = text;
        sub.tm_end = tm_end;

        subs[tm_start] = sub;


    }

    /*for(std::map<double, SubTitle>::iterator it1 = subs.begin();
        it1 != subs.end(); it1++){
        for(std::map<double, SubTitle>::iterator it2 = ++it1;
            it2 != subs.end(); it2++){
            if(it1->second.tm_end >= it2->first){
                //it1->second.text += "\n" + it2->second.text;
                subs.find(it2).text = "";
            }
            //cout << it1->first << it2->first << endl;
        }
    }*/

    for (auto it1 = subs.begin(); it1 != subs.end(); ++it1)
    {

        bool need_empty_sub = false;
        auto it2 = it1;
        it2++;
        for (; it2 != subs.end(); ++it2)
        {
            cout << "curr: " << it1->first << " - " << it2->first << endl;
            if(it1 == it2){
                continue;
            }

            if(it1->second.tm_end >= it2->first){

                SubTitle tmp1 = it1->second;
                it1->second.tm_end = it2->first;
                cout << "Test1 :" << it1->second.tm_end << "   " << tmp1.tm_end << endl;

                SubTitle tmp2 = it2->second;




                if(tmp1.tm_end >= it2->second.tm_end){
                    SubTitle sub;
                    sub.text = it1->second.text;
                    sub.tm_end = it1->second.tm_end;
                    subs[it2->second.tm_end] = sub;
                }else{
                    SubTitle sub;
                    sub.text = it2->second.text;
                    sub.tm_end = it2->second.tm_end;
                    subs[it1->second.tm_end] = sub;
                }



            }else if(need_empty_sub){
                SubTitle sub;
                sub.text = "";
                sub.tm_end = it1->second.tm_end;
                subs[it1->second.tm_end] = sub;
                break;
            }
        }
    }


    return subs.size();
}


























void SrtParser::show_subtitles(std::ostream &out){
    show_subtitles(out, 0, -1);
}

void SrtParser::show_subtitles(std::ostream &out,
                               double start,
                               double end){
    if(duration() <= 0){
        return ;
    }

    if(end <= 0){
        end = duration();
    }

    if(start >= end){
        return ;
    }
    for (const auto it : subs) {
        string text = it.second.text;
        double time = it.first;
        if(time <= start){
            continue;
        }
        if(time >= end){
            return ;
        }

        cout << "at " << time << " show '" + text + "'" << endl;
    }
}
