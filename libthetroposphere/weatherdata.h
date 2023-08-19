#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include "libthetroposphere_global.h"
#include <QCoroTask>
#include <QObject>
#include <ranges/trange.h>

class WeatherTimeseries;
struct WeatherDataPrivate;
class LIBTHETROPOSPHERE_EXPORT WeatherData : public QObject {
        Q_OBJECT
    public:
        explicit WeatherData(QObject* parent = nullptr);
        ~WeatherData();

        static QCoro::Task<QSharedPointer<WeatherData>> getWeatherData(double lat, double lng);
        tRange<WeatherTimeseries*> timeseries();

    signals:

    private:
        WeatherDataPrivate* d;
};

typedef QSharedPointer<WeatherData> WeatherDataPtr;

#endif // WEATHERDATA_H
