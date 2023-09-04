#ifndef WEATHERTIMESERIES_H
#define WEATHERTIMESERIES_H

#include <QObject>
#include <QSize>
#include <type_traits>

struct WeatherTimeseriesPrivate;
class WeatherTimeseries : public QObject {
        Q_OBJECT
    public:
        explicit WeatherTimeseries(QJsonObject timeseriesData, QObject* parent = nullptr);
        ~WeatherTimeseries();
        enum class WeatherFlag {
            None = 0,
            Sun = 1 << 0,
            Moon = 1 << 1,
            PolarTwilight = 1 << 2,
            Light = 1 << 3,
            Medium = 1 << 4,
            Heavy = 1 << 5,
            Rain = 1 << 6,
            Snow = 1 << 7,
            Thunder = 1 << 8,
            Fog = 1 << 9,
            Fair = 1 << 10,
            Cloudy = 1 << 11
        };

        class WeatherFlagWrapper {
            public:
                WeatherFlagWrapper(WeatherFlag flag);

                operator WeatherFlag();
                operator bool();

            private:
                WeatherFlag _flag;
        };

        WeatherFlag weatherFlagForString(QString string);
        QPixmap iconForWeatherFlag(WeatherFlag symbolCode, QSize size = {32, 32}, qreal devicePixelRatio = 1);

        QDateTime time();

        double temperature();   // degrees celsius
        double windDirection(); // angular degrees
        double windSpeed();     // meters per second

        bool is1HourDataAvailable();
        double precipitation1Hour(); // millimeters
        WeatherTimeseries::WeatherFlag symbolCode1Hour();

    signals:

    private:
        WeatherTimeseriesPrivate* d;
};

WeatherTimeseries::WeatherFlagWrapper operator|(WeatherTimeseries::WeatherFlag a, WeatherTimeseries::WeatherFlag b);
WeatherTimeseries::WeatherFlag& operator|=(WeatherTimeseries::WeatherFlag& a, WeatherTimeseries::WeatherFlag b);
WeatherTimeseries::WeatherFlagWrapper operator&(WeatherTimeseries::WeatherFlag a, WeatherTimeseries::WeatherFlag b);
WeatherTimeseries::WeatherFlag& operator&=(WeatherTimeseries::WeatherFlag& a, WeatherTimeseries::WeatherFlag b);

#endif // WEATHERTIMESERIES_H
