#ifndef GPSSCANNER_H
#define GPSSCANNER_H
#include <iostream>
#include <map>
#include "utils/geopoint.h"

struct GPSTrack{
    double lat;
    double lon;
    double ele;
};



class GPSScanner
{
private:
    std::map< time_t, GPSTrack> tracks;
    std::map< std::string, double> speed_range;

    time_t start_run_tm;
    time_t start_stop_tm;

    time_t _run_time, _stop_time;

    time_t start_time, end_time;
    double _max_speed;
    double _min_height, _max_height;
    double _total_climb, _total_descent;
    double _all_distance;
public:
    GPSScanner();
    void add_track(const time_t ts, const double lat,
                   const double lon, const double ele = 0);
    double all_distance(){
        return _all_distance;
    }
    double total_climb(){
        return _total_climb;
    }
    double total_descent(){
        return _total_descent;
    }
    double max_height(){
        return _max_height;
    }
    double min_height(){
        return _min_height;
    }
    double max_speed(){
        return _max_speed;
    }

    time_t duration(){
        return (end_time - start_time);
    }

    time_t run_time(){
        return _run_time;
    }

    time_t stop_time(){
        return _stop_time;
    }

    double average_speed(const bool with_stops = true);

};

#endif // GPSSCANNER_H
