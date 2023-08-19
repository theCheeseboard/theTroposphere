#ifndef WEATHERTIMESERIES_H
#define WEATHERTIMESERIES_H

#include <QObject>

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

        SymbolCode symbolCodeForString(QString string);

        double temperature(); // celsius

    signals:

    private:
        WeatherTimeseriesPrivate* d;
};

#endif // WEATHERTIMESERIES_H
