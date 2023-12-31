#ifndef TROPOSPHEREHELPER_H
#define TROPOSPHEREHELPER_H

#include <QCoreApplication>
#include <QTimeZone>

class TroposphereHelper {
        Q_DECLARE_TR_FUNCTIONS(TroposphereHelper)
    public:
        static QString readableTemperature(double temperature);
        static QString readableSpeed(double metersPerSecond);
        static QString readablePrecipitation(double millimeters);
        static QDateTime toLocalTime(QDateTime date, QTimeZone timezone);
};

#endif // TROPOSPHEREHELPER_H
