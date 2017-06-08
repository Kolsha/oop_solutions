#pragma once

class Geopoint {
public:
    Geopoint(double _lat, double _lng, double _elev = 0)
        : lat(_lat), lng(_lng), elev(_elev) {}

    // Широта
    double latitude() const { return lat; }

    // Долгота
    double longitude() const { return lng; }

    // Высота над уровнем моря
    double elevation() const { return elev; }

    // Расстояние до другой точки
    double distanceTo(const Geopoint &other);

private:
    double lat, lng, elev;
};
