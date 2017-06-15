#include <cmath>
#include "gpsscanner.h"

GPSScanner::GPSScanner()
{
    clear();
}


void GPSScanner::add_track(const time_t ts, const double lat,
                           const double lon, const double ele){

    GPSTrack trk, last_trk;
    trk.ele = ele;
    trk.lat = lat;
    trk.lon = lon;
    // if ><
    {
        if(ts > end_time){
            end_time = ts;
        }
        if(ts < start_time || start_time == 0){
            start_time = ts;
        }

        if(ele < _min_height){
            _min_height = ele;
        }
        if (ele > _max_height){
            _max_height = ele;
        }
    }
    // if ><

    if(tracks.size()){
        last_trk = tracks.rbegin()->second;
        time_t last_trk_time = tracks.rbegin()->first;

        if(last_trk.ele > ele){
            _total_descent += fabs(ele - last_trk.ele);
        }
        if(last_trk.ele < ele){
            _total_climb += fabs(ele - last_trk.ele);
        }

        Geopoint new_point(lat, lon, ele),
                last_point(last_trk.lat, last_trk.lon, last_trk.ele);

        double dist = last_point.distanceTo(new_point);
        _all_distance += fabs(dist);

        if(dist > 1){

            if(start_run_tm < 1){
                start_run_tm = ts;
            }

            if(start_stop_tm > 0){
                _stop_time += (ts - start_stop_tm);
                start_stop_tm = 0;
            }

            double dt = ts - last_trk_time;
            double speed = dist / dt;
            if(speed > _max_speed){
                _max_speed = speed;
            }
            sum_of_speed += speed;
            count_sum_of_speed++;

            speed_by_time.insert({ts, speed});

        }else{
            if(start_stop_tm < 1){
                start_stop_tm = ts;
            }
            if(start_run_tm > 0){
                _run_time += (ts - start_run_tm);
                start_run_tm = 0;
            }


        }


    }




    tracks.insert({ts, std::move(trk)});
}


double GPSScanner::average_speed(const bool with_stops){
    if(with_stops){
        double tmp = all_distance() / duration();
        return tmp;
    }else{
        double tmp = sum_of_speed / count_sum_of_speed;
        return tmp;
    }
}

void GPSScanner::clear()
{
    start_time = 0;
    end_time = 0;
    _max_height = 0;
    _min_height = 10000;

    start_run_tm = 0;
    start_stop_tm = 0;

    _run_time  = 0;
    _stop_time  = 0;

    sum_of_speed = 0;
    count_sum_of_speed = 0;
    tracks.clear();
}
