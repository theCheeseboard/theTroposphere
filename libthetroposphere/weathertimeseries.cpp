#include "weathertimeseries.h"

#include <QJsonObject>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>

struct WeatherTimeseriesPrivate {
        QDateTime time;

        double temperature;   // degrees celsius
        double humidity;      // percentage
        double windSpeed;     // meters per second
        double windDirection; // angular degrees

        bool is1HourDataAvailable = false;             // boolean
        double precipitation1Hour;                     // millimeters
        WeatherTimeseries::SymbolCode symbolCode1Hour; // symbol code

        static QMap<QString, WeatherTimeseries::SymbolCode> symbolsMap;
};

QMap<QString, WeatherTimeseries::SymbolCode> WeatherTimeseriesPrivate::symbolsMap = {
    {"clearsky_day",                              WeatherTimeseries::SymbolCode::ClearSkyDay                             },
    {"clearsky_night",                            WeatherTimeseries::SymbolCode::ClearSkyNight                           },
    {"clearsky_polartwilight",                    WeatherTimeseries::SymbolCode::ClearSkyPolarTwilight                   },
    {"cloudy",                                    WeatherTimeseries::SymbolCode::Cloudy                                  },
    {"fair_day",                                  WeatherTimeseries::SymbolCode::FairDay                                 },
    {"fair_night",                                WeatherTimeseries::SymbolCode::FairNight                               },
    {"fair_polartwilight",                        WeatherTimeseries::SymbolCode::FairPolarTwilight                       },
    {"fog",                                       WeatherTimeseries::SymbolCode::Fog                                     },
    {"heavyrain",                                 WeatherTimeseries::SymbolCode::HeavyRain                               },
    {"heavyrainandthunder",                       WeatherTimeseries::SymbolCode::HeavyRainAndThunder                     },
    {"heavyrainshowers_day",                      WeatherTimeseries::SymbolCode::HeavyRainShowersDay                     },
    {"heavyrainshowers_night",                    WeatherTimeseries::SymbolCode::HeavyRainShowersNight                   },
    {"heavyrainshowers_polartwilight",            WeatherTimeseries::SymbolCode::HeavyRainShowersPolarTwilight           },
    {"heavyrainshowersandthunder_day",            WeatherTimeseries::SymbolCode::HeavyRainShowersAndThunderDay           },
    {"heavyrainshowersandthunder_night",          WeatherTimeseries::SymbolCode::HeavyRainShowersAndThunderNight         },
    {"heavyrainshowersandthunder_polartwilight",  WeatherTimeseries::SymbolCode::HeavyRainShowersAndThunderPolarTwilight },
    {"heavysleet",                                WeatherTimeseries::SymbolCode::HeavySleet                              },
    {"heavysleetandthunder",                      WeatherTimeseries::SymbolCode::HeavySleetAndThunder                    },
    {"heavysleetshowers_day",                     WeatherTimeseries::SymbolCode::HeavySleetShowersDay                    },
    {"heavysleetshowers_night",                   WeatherTimeseries::SymbolCode::HeavySleetShowersNight                  },
    {"heavysleetshowers_polartwilight",           WeatherTimeseries::SymbolCode::HeavySleetShowersPolarTwilight          },
    {"heavysleetshowersandthunder_day",           WeatherTimeseries::SymbolCode::HeavySleetShowersAndThunderDay          },
    {"heavysleetshowersandthunder_night",         WeatherTimeseries::SymbolCode::HeavySleetShowersAndThunderNight        },
    {"heavysleetshowersandthunder_polartwilight", WeatherTimeseries::SymbolCode::HeavySleetShowersAndThunderPolarTwilight},
    {"heavysnow",                                 WeatherTimeseries::SymbolCode::HeavySnow                               },
    {"heavysnowandthunder",                       WeatherTimeseries::SymbolCode::HeavySnowAndThunder                     },
    {"heavysnowshowers_day",                      WeatherTimeseries::SymbolCode::HeavySnowShowersDay                     },
    {"heavysnowshowers_night",                    WeatherTimeseries::SymbolCode::HeavySnowShowersNight                   },
    {"heavysnowshowers_polartwilight",            WeatherTimeseries::SymbolCode::HeavyShowShowersPolarTwilight           },
    {"heavysnowshowersandthunder_day",            WeatherTimeseries::SymbolCode::HeavySnowShowersAndThunderDay           },
    {"heavysnowshowersandthunder_night",          WeatherTimeseries::SymbolCode::HeavySnowShowersAndThunderNight         },
    {"heavysnowshowersandthunder_polartwilight",  WeatherTimeseries::SymbolCode::HeavySnowShowersAndThunderPolarTwilight },
    {"lightrain",                                 WeatherTimeseries::SymbolCode::LightRain                               },
    {"lightrainandthunder",                       WeatherTimeseries::SymbolCode::LightRainAndThunder                     },
    {"lightrainshowers_day",                      WeatherTimeseries::SymbolCode::LightRainShowersDay                     },
    {"lightrainshowers_night",                    WeatherTimeseries::SymbolCode::LightRainShowersNight                   },
    {"lightrainshowers_polartwilight",            WeatherTimeseries::SymbolCode::LightRainShowersPolarTwilight           },
    {"lightrainshowersandthunder_day",            WeatherTimeseries::SymbolCode::LightRainShowersAndThunderDay           },
    {"lightrainshowersandthunder_night",          WeatherTimeseries::SymbolCode::LightRainShowersAndThunderNight         },
    {"lightrainshowersandthunder_polartwilight",  WeatherTimeseries::SymbolCode::LightRainShowersAndThunderPolarTwilight },
    {"lightsleet",                                WeatherTimeseries::SymbolCode::LightSleet                              },
    {"lightsleetandthunder",                      WeatherTimeseries::SymbolCode::LightSleetAndThunder                    },
    {"lightsleetshowers_day",                     WeatherTimeseries::SymbolCode::LightSleetShowersDay                    },
    {"lightsleetshowers_night",                   WeatherTimeseries::SymbolCode::LightSleetShowersNight                  },
    {"lightsleetshowers_polartwilight",           WeatherTimeseries::SymbolCode::LightSleetShowersPolarTwilight          },
    {"lightsnow",                                 WeatherTimeseries::SymbolCode::LightSnow                               },
    {"lightsnowandthunder",                       WeatherTimeseries::SymbolCode::LightSnowAndThunder                     },
    {"lightsnowshowers_day",                      WeatherTimeseries::SymbolCode::LightSnowShowersDay                     },
    {"lightsnowshowers_night",                    WeatherTimeseries::SymbolCode::LightSnowShowersNight                   },
    {"lightsnowshowers_polartwilight",            WeatherTimeseries::SymbolCode::LightSnowShowersPolarTwilight           },
    {"lightsleetshowersandthunder_day",           WeatherTimeseries::SymbolCode::LightSleetShowersAndThunderDay          },
    {"lightsleetshowersandthunder_night",         WeatherTimeseries::SymbolCode::LightSleetShowersAndThunderNight        },
    {"lightsleetshowersandthunder_polartwilight", WeatherTimeseries::SymbolCode::LightSleetShowersAndThunderPolarTwilight},
    {"lightsnowshowersandthunder_day",            WeatherTimeseries::SymbolCode::LightSnowShowersndThunderDay            },
    {"lightsnowshowersandthunder_night",          WeatherTimeseries::SymbolCode::LightSnowShowersAndThunderNight         },
    {"lightsnowshowersandthunder_polartwilight",  WeatherTimeseries::SymbolCode::LightSnowShowersAndThunderPolarTwilight },
    {"partlycloudy_day",                          WeatherTimeseries::SymbolCode::PartlyCloudyDay                         },
    {"partlycloudy_night",                        WeatherTimeseries::SymbolCode::PartlyCloudyNight                       },
    {"partlycloudy_polartwilight",                WeatherTimeseries::SymbolCode::PartlyCloudyPolarTwilight               },
    {"rain",                                      WeatherTimeseries::SymbolCode::Rain                                    },
    {"rainandthunder",                            WeatherTimeseries::SymbolCode::RainAndThunder                          },
    {"rainshowers_day",                           WeatherTimeseries::SymbolCode::RainShowersDay                          },
    {"rainshowers_night",                         WeatherTimeseries::SymbolCode::RainShowersNight                        },
    {"rainshowers_polartwilight",                 WeatherTimeseries::SymbolCode::RainShowersPolarTwilight                },
    {"rainshowersandthunder_day",                 WeatherTimeseries::SymbolCode::RainShowersAndThunderDay                },
    {"rainshowersandthunder_night",               WeatherTimeseries::SymbolCode::RainShowersAndThunderNight              },
    {"rainshowersandthunder_polartwilight",       WeatherTimeseries::SymbolCode::RainShowersAndThunderPolarTwilight      },
    {"sleet",                                     WeatherTimeseries::SymbolCode::Sleet                                   },
    {"sleetandthunder",                           WeatherTimeseries::SymbolCode::SleetAndThunder                         },
    {"sleetshowers_day",                          WeatherTimeseries::SymbolCode::SleetShowersDay                         },
    {"sleetshowers_night",                        WeatherTimeseries::SymbolCode::SleetShowersNight                       },
    {"sleetshowers_polartwilight",                WeatherTimeseries::SymbolCode::SleetShowersPolarTwilight               },
    {"sleetshowersandthunder_day",                WeatherTimeseries::SymbolCode::SleetShowersAndThunderDay               },
    {"sleetshowersandthunder_night",              WeatherTimeseries::SymbolCode::SleetShowersAndThunderNight             },
    {"sleetshowersandthunder_polartwilight",      WeatherTimeseries::SymbolCode::SleetShowersAndThunderPolarTwilight     },
    {"snow",                                      WeatherTimeseries::SymbolCode::Snow                                    },
    {"snowandthunder",                            WeatherTimeseries::SymbolCode::SnowAndThunder                          },
    {"snowshowers_day",                           WeatherTimeseries::SymbolCode::SnowShowersDay                          },
    {"snowshowers_night",                         WeatherTimeseries::SymbolCode::SnowShowersNight                        },
    {"snowshowers_polartwilight",                 WeatherTimeseries::SymbolCode::SnowShowersPolarTwilight                },
    {"snowshowersandthunder_day",                 WeatherTimeseries::SymbolCode::SnowShowersAndThunderDay                },
    {"snowshowersandthunder_night",               WeatherTimeseries::SymbolCode::SnowShowersAndThunderNight              },
    {"snowshowersandthunder_polartwilight",       WeatherTimeseries::SymbolCode::SnowShowersAndThunderPolarTwilight      },
};

WeatherTimeseries::WeatherTimeseries(QJsonObject timeseriesData, QObject* parent) :
    QObject{parent} {
    d = new WeatherTimeseriesPrivate();

    Q_INIT_RESOURCE(libthetroposphere_weathericons);

    auto data = timeseriesData.value("data").toObject();
    auto instantData = data.value("instant").toObject().value("details").toObject();

    d->time = QDateTime::fromString(timeseriesData.value("time").toString(), Qt::ISODate);
    d->temperature = instantData.value("air_temperature").toDouble();
    d->humidity = instantData.value("relative_humidity").toDouble();
    d->windSpeed = instantData.value("wind_speed").toDouble();
    d->windDirection = instantData.value("wind_from_direction").toDouble();

    if (data.contains("next_1_hours")) {
        auto next1Hour = data.value("next_1_hours").toObject();
        d->is1HourDataAvailable = true;
        d->precipitation1Hour = next1Hour.value("details").toObject().value("precipitation_amount").toDouble();
        d->symbolCode1Hour = symbolCodeForString(next1Hour.value("summary").toObject().value("symbol_code").toString());
    }
}

WeatherTimeseries::~WeatherTimeseries() {
    delete d;
}

WeatherTimeseries::SymbolCode WeatherTimeseries::symbolCodeForString(QString string) {
    return d->symbolsMap.value(string, SymbolCode::Unknown);
}

WeatherTimeseries::BackgroundCode WeatherTimeseries::mapSymbolCodeToBackgroundCode(SymbolCode symbolCode) {
    switch (symbolCode) {
        // Clear or fair Day
        case SymbolCode::ClearSkyDay:
        case SymbolCode::FairDay:
        case SymbolCode::PartlyCloudyDay:
            return BackgroundCode::Day;

            // Clear or fair Night
        case SymbolCode::ClearSkyNight:
        case SymbolCode::FairNight:
        case SymbolCode::PartlyCloudyNight:
            return BackgroundCode::Night;

            // All raining conditions
        case SymbolCode::HeavyRain:
        case SymbolCode::HeavyRainShowersDay:
        case SymbolCode::HeavyRainShowersNight:
        case SymbolCode::LightRain:
        case SymbolCode::LightRainShowersDay:
        case SymbolCode::LightRainShowersNight:
        case SymbolCode::Rain:
        case SymbolCode::RainShowersDay:
        case SymbolCode::RainShowersNight:
            return BackgroundCode::Raining;

            // All thundering conditions
        case SymbolCode::HeavySleetAndThunder:
        case SymbolCode::HeavyRainAndThunder:
        case SymbolCode::LightSleetAndThunder:
        case SymbolCode::LightRainAndThunder:
        case SymbolCode::SleetAndThunder:
        case SymbolCode::RainAndThunder:
        case SymbolCode::LightSnowAndThunder:
        case SymbolCode::SnowAndThunder:
        case SymbolCode::HeavySnowAndThunder:
            return BackgroundCode::Thundering;

            // All snowing conditions
        case SymbolCode::HeavySnow:
        case SymbolCode::HeavySnowShowersDay:
        case SymbolCode::HeavySnowShowersNight:
        case SymbolCode::LightSnow:
        case SymbolCode::LightSnowShowersDay:
        case SymbolCode::LightSnowShowersNight:
        case SymbolCode::Snow:
        case SymbolCode::SnowShowersDay:
        case SymbolCode::SnowShowersNight:
            return BackgroundCode::Snowing;

        default:
            return BackgroundCode::Unknown;
    }
}

QPixmap WeatherTimeseries::iconForSymbolCode(SymbolCode symbolCode, QSize size, qreal devicePixelRatio) {
    auto filename = d->symbolsMap.key(symbolCode);
    QPixmap px(size * devicePixelRatio);
    px.fill(Qt::transparent);
    px.setDevicePixelRatio(devicePixelRatio);

    QPainter painter(&px);
    QSvgRenderer renderer(QStringLiteral(":/libthetroposphere/weathericons/%1.svg").arg(filename));
    renderer.render(&painter, QRectF(0, 0, size.width(), size.height()));
    painter.end();

    return px;
}

QDateTime WeatherTimeseries::time() {
    return d->time;
}

double WeatherTimeseries::temperature() {
    return d->temperature;
}

double WeatherTimeseries::windDirection() {
    return d->windDirection;
}

double WeatherTimeseries::windSpeed() {
    return d->windSpeed;
}

bool WeatherTimeseries::is1HourDataAvailable() {
    return d->is1HourDataAvailable;
}

double WeatherTimeseries::precipitation1Hour() {
    return d->precipitation1Hour;
}

WeatherTimeseries::SymbolCode WeatherTimeseries::symbolCode1Hour() {
    return d->symbolCode1Hour;
}
