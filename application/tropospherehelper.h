#ifndef TROPOSPHEREHELPER_H
#define TROPOSPHEREHELPER_H

#include <QCoreApplication>

class TroposphereHelper {
        Q_DECLARE_TR_FUNCTIONS(TroposphereHelper)
    public:
        static QString readableTemperature(double temperature);
};

#endif // TROPOSPHEREHELPER_H
