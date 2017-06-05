#include "gpsscanner.h"

GPSScanner::GPSScanner()
{

}


void GPSScanner::add_track(const time_t ts, const double lat,
                           const double lon, const double ele){

    GPSTrack trk;
    trk.ele = ele;
    trk.lat = lat;
    trk.lon = lon;

    tracks.insert({ts, std::move(trk)});
}
