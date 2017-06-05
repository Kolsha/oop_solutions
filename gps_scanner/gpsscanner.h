#ifndef GPSSCANNER_H
#define GPSSCANNER_H
#include <iostream>
#include <map>

struct GPSTrack{
    double lat;
    double lon;
    double ele;
};



class GPSScanner
{
private:
    std::map< time_t, GPSTrack> tracks;
public:
    GPSScanner();
    void add_track(const time_t ts, const double lat,
                   const double lon, const double ele = 0);

};

#endif // GPSSCANNER_H
