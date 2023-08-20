#ifndef TROPOSPHERELOCATION_H
#define TROPOSPHERELOCATION_H

#include "libthetroposphere_global.h"
#include <QCoroTask>
#include <QString>
#include <texception.h>

struct LIBTHETROPOSPHERE_EXPORT TroposphereLocation {
        TroposphereLocation();
        TroposphereLocation(QString serialised);
        TroposphereLocation(QString name, QString admin1, QString country, QString timezone, double lat, double lng);

        static QCoro::Task<TroposphereLocation> reverseGeocode(double lat, double lng);

        QString serialise();

        bool locatedLocation = false;

        QString name;
        QString admin1;
        QString country;
        QString timezone;

        double lat;
        double lng;
};

class LIBTHETROPOSPHERE_EXPORT TroposphereReverseGeocodeException : public QException {
        T_EXCEPTION(TroposphereReverseGeocodeException)
};

#endif // TROPOSPHERELOCATION_H
