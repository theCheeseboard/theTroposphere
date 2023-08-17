#ifndef TROPOSPHERELOCATION_H
#define TROPOSPHERELOCATION_H

#include <QString>

struct TroposphereLocation
{
    TroposphereLocation();
    TroposphereLocation(QString serialised);
    TroposphereLocation(QString name, QString admin1, QString country, double lat, double lng);

    QString serialise();

    bool locatedLocation = false;

    QString name;
    QString admin1;
    QString country;

    double lat;
    double lng;
};

#endif // TROPOSPHERELOCATION_H
