#ifndef WEATHERTIMESERIES_H
#define WEATHERTIMESERIES_H

#include <QObject>
#include <QSize>

struct WeatherTimeseriesPrivate;
class WeatherTimeseries : public QObject {
        Q_OBJECT
    public:
        explicit WeatherTimeseries(QJsonObject timeseriesData, QObject* parent = nullptr);
        ~WeatherTimeseries();

        enum class SymbolCode {
            ClearSkyDay,
            ClearSkyNight,
            ClearSkyPolarTwilight,
            Cloudy,
            FairDay,
            FairNight,
            FairPolarTwilight,
            Fog,
            HeavyRain,
            HeavyRainAndThunder,
            HeavyRainShowersDay,
            HeavyRainShowersNight,
            HeavyRainShowersPolarTwilight,
            HeavyRainShowersAndThunderDay,
            HeavyRainShowersAndThunderNight,
            HeavyRainShowersAndThunderPolarTwilight,
            HeavySleet,
            HeavySleetAndThunder,
            HeavySleetShowersDay,
            HeavySleetShowersNight,
            HeavySleetShowersPolarTwilight,
            HeavySleetShowersAndThunderDay,
            HeavySleetShowersAndThunderNight,
            HeavySleetShowersAndThunderPolarTwilight,
            HeavySnow,
            HeavySnowAndThunder,
            HeavySnowShowersDay,
            HeavySnowShowersNight,
            HeavyShowShowersPolarTwilight,
            HeavySnowShowersAndThunderDay,
            HeavySnowShowersAndThunderNight,
            HeavySnowShowersAndThunderPolarTwilight,
            LightRain,
            LightRainAndThunder,
            LightRainShowersDay,
            LightRainShowersNight,
            LightRainShowersPolarTwilight,
            LightRainShowersAndThunderDay,
            LightRainShowersAndThunderNight,
            LightRainShowersAndThunderPolarTwilight,
            LightSleet,
            LightSleetAndThunder,
            LightSleetShowersDay,
            LightSleetShowersNight,
            LightSleetShowersPolarTwilight,
            LightSnow,
            LightSnowAndThunder,
            LightSnowShowersDay,
            LightSnowShowersNight,
            LightSnowShowersPolarTwilight,
            LightSleetShowersAndThunderDay,
            LightSleetShowersAndThunderNight,
            LightSleetShowersAndThunderPolarTwilight,
            LightSnowShowersndThunderDay,
            LightSnowShowersAndThunderNight,
            LightSnowShowersAndThunderPolarTwilight,
            PartlyCloudyDay,
            PartlyCloudyNight,
            PartlyCloudyPolarTwilight,
            Rain,
            RainAndThunder,
            RainShowersDay,
            RainShowersNight,
            RainShowersPolarTwilight,
            RainShowersAndThunderDay,
            RainShowersAndThunderNight,
            RainShowersAndThunderPolarTwilight,
            Sleet,
            SleetAndThunder,
            SleetShowersDay,
            SleetShowersNight,
            SleetShowersPolarTwilight,
            SleetShowersAndThunderDay,
            SleetShowersAndThunderNight,
            SleetShowersAndThunderPolarTwilight,
            Snow,
            SnowAndThunder,
            SnowShowersDay,
            SnowShowersNight,
            SnowShowersPolarTwilight,
            SnowShowersAndThunderDay,
            SnowShowersAndThunderNight,
            SnowShowersAndThunderPolarTwilight,
            Unknown
        };

        enum class BackgroundCode {
            Day,
            Night,
            Raining,
            Thundering,
            Snowing,
            Unknown
        };

        SymbolCode symbolCodeForString(QString string);
        BackgroundCode mapSymbolCodeToBackgroundCode(SymbolCode symbolCode);
        QPixmap iconForSymbolCode(SymbolCode symbolCode, QSize size = {32, 32}, qreal devicePixelRatio = 1);

        QDateTime time();

        double temperature();   // degrees celsius
        double windDirection(); // angular degrees
        double windSpeed();     // meters per second

        bool is1HourDataAvailable();
        double precipitation1Hour(); // millimeters
        WeatherTimeseries::SymbolCode symbolCode1Hour();

    signals:

    private:
        WeatherTimeseriesPrivate* d;
};

#endif // WEATHERTIMESERIES_H
