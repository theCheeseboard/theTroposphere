#ifndef TROPOSPHERELOCATION_H
#define TROPOSPHERELOCATION_H

#include "libthetroposphere_global.h"
#include <QString>

struct LIBTHETROPOSPHERE_EXPORT TroposphereLocation {
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
