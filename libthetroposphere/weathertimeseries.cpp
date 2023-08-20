#include "weathertimeseries.h"

#include <QJsonObject>

struct WeatherTimeseriesPrivate {
        QDateTime time;

        double temperature;   // degrees celsius
        double humidity;      // percentage
        double windSpeed;     // meters per second
        double windDirection; // angular degrees

        bool is1HourDataAvailable = false;             // boolean
        double precipitation1Hour;                     // millimeters
        WeatherTimeseries::SymbolCode symbolCode1Hour; // symbol code
};

WeatherTimeseries::WeatherTimeseries(QJsonObject timeseriesData, QObject* parent) :
    QObject{parent} {
    d = new WeatherTimeseriesPrivate();

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
        d->precipitation1Hour = instantData.value("precipitation_amount").toDouble();
        d->symbolCode1Hour = symbolCodeForString(instantData.value("symbol_code").toString());
    }
}

WeatherTimeseries::~WeatherTimeseries() {
    delete d;
}

WeatherTimeseries::SymbolCode WeatherTimeseries::symbolCodeForString(QString string) {
    static QMap<QString, SymbolCode> symbolsMap{
        {"clearsky_day.svg",                              SymbolCode::ClearSkyDay                             },
        {"clearsky_night.svg",                            SymbolCode::ClearSkyNight                           },
        {"clearsky_polartwilight.svg",                    SymbolCode::ClearSkyPolarTwilight                   },
        {"cloudy.svg",                                    SymbolCode::Cloudy                                  },
        {"fair_day.svg",                                  SymbolCode::FairDay                                 },
        {"fair_night.svg",                                SymbolCode::FairNight                               },
        {"fair_polartwilight.svg",                        SymbolCode::FairPolarTwilight                       },
        {"fog.svg",                                       SymbolCode::Fog                                     },
        {"heavyrain.svg",                                 SymbolCode::HeavyRain                               },
        {"heavyrainandthunder.svg",                       SymbolCode::HeavyRainAndThunder                     },
        {"heavyrainshowers_day.svg",                      SymbolCode::HeavyRainShowersDay                     },
        {"heavyrainshowers_night.svg",                    SymbolCode::HeavyRainShowersNight                   },
        {"heavyrainshowers_polartwilight.svg",            SymbolCode::HeavyRainShowersPolarTwilight           },
        {"heavyrainshowersandthunder_day.svg",            SymbolCode::HeavyRainShowersAndThunderDay           },
        {"heavyrainshowersandthunder_night.svg",          SymbolCode::HeavyRainShowersAndThunderNight         },
        {"heavyrainshowersandthunder_polartwilight.svg",  SymbolCode::HeavyRainShowersAndThunderPolarTwilight },
        {"heavysleet.svg",                                SymbolCode::HeavySleet                              },
        {"heavysleetandthunder.svg",                      SymbolCode::HeavySleetAndThunder                    },
        {"heavysleetshowers_day.svg",                     SymbolCode::HeavySleetShowersDay                    },
        {"heavysleetshowers_night.svg",                   SymbolCode::HeavySleetShowersNight                  },
        {"heavysleetshowers_polartwilight.svg",           SymbolCode::HeavySleetShowersPolarTwilight          },
        {"heavysleetshowersandthunder_day.svg",           SymbolCode::HeavySleetShowersAndThunderDay          },
        {"heavysleetshowersandthunder_night.svg",         SymbolCode::HeavySleetShowersAndThunderNight        },
        {"heavysleetshowersandthunder_polartwilight.svg", SymbolCode::HeavySleetShowersAndThunderPolarTwilight},
        {"heavysnow.svg",                                 SymbolCode::HeavySnow                               },
        {"heavysnowandthunder.svg",                       SymbolCode::HeavySnowAndThunder                     },
        {"heavysnowshowers_day.svg",                      SymbolCode::HeavySnowShowersDay                     },
        {"heavysnowshowers_night.svg",                    SymbolCode::HeavySnowShowersNight                   },
        {"heavysnowshowers_polartwilight.svg",            SymbolCode::HeavyShowShowersPolarTwilight           },
        {"heavysnowshowersandthunder_day.svg",            SymbolCode::HeavySnowShowersAndThunderDay           },
        {"heavysnowshowersandthunder_night.svg",          SymbolCode::HeavySnowShowersAndThunderNight         },
        {"heavysnowshowersandthunder_polartwilight.svg",  SymbolCode::HeavySnowShowersAndThunderPolarTwilight },
        {"lightrain.svg",                                 SymbolCode::LightRain                               },
        {"lightrainandthunder.svg",                       SymbolCode::LightRainAndThunder                     },
        {"lightrainshowers_day.svg",                      SymbolCode::LightRainShowersDay                     },
        {"lightrainshowers_night.svg",                    SymbolCode::LightRainShowersNight                   },
        {"lightrainshowers_polartwilight.svg",            SymbolCode::LightRainShowersPolarTwilight           },
        {"lightrainshowersandthunder_day.svg",            SymbolCode::LightRainShowersAndThunderDay           },
        {"lightrainshowersandthunder_night.svg",          SymbolCode::LightRainShowersAndThunderNight         },
        {"lightrainshowersandthunder_polartwilight.svg",  SymbolCode::LightRainShowersAndThunderPolarTwilight },
        {"lightsleet.svg",                                SymbolCode::LightSleet                              },
        {"lightsleetandthunder.svg",                      SymbolCode::LightSleetAndThunder                    },
        {"lightsleetshowers_day.svg",                     SymbolCode::LightSleetShowersDay                    },
        {"lightsleetshowers_night.svg",                   SymbolCode::LightSleetShowersNight                  },
        {"lightsleetshowers_polartwilight.svg",           SymbolCode::LightSleetShowersPolarTwilight          },
        {"lightsnow.svg",                                 SymbolCode::LightSnow                               },
        {"lightsnowandthunder.svg",                       SymbolCode::LightSnowAndThunder                     },
        {"lightsnowshowers_day.svg",                      SymbolCode::LightSnowShowersDay                     },
        {"lightsnowshowers_night.svg",                    SymbolCode::LightSnowShowersNight                   },
        {"lightsnowshowers_polartwilight.svg",            SymbolCode::LightSnowShowersPolarTwilight           },
        {"lightsleetshowersandthunder_day.svg",           SymbolCode::LightSleetShowersAndThunderDay          },
        {"lightsleetshowersandthunder_night.svg",         SymbolCode::LightSleetShowersAndThunderNight        },
        {"lightsleetshowersandthunder_polartwilight.svg", SymbolCode::LightSleetShowersAndThunderPolarTwilight},
        {"lightsnowshowersandthunder_day.svg",            SymbolCode::LightSnowShowersndThunderDay            },
        {"lightsnowshowersandthunder_night.svg",          SymbolCode::LightSnowShowersAndThunderNight         },
        {"lightsnowshowersandthunder_polartwilight.svg",  SymbolCode::LightSnowShowersAndThunderPolarTwilight },
        {"partlycloudy_day.svg",                          SymbolCode::PartlyCloudyDay                         },
        {"partlycloudy_night.svg",                        SymbolCode::PartlyCloudyNight                       },
        {"partlycloudy_polartwilight.svg",                SymbolCode::PartlyCloudyPolarTwilight               },
        {"rain.svg",                                      SymbolCode::Rain                                    },
        {"rainandthunder.svg",                            SymbolCode::RainAndThunder                          },
        {"rainshowers_day.svg",                           SymbolCode::RainShowersDay                          },
        {"rainshowers_night.svg",                         SymbolCode::RainShowersNight                        },
        {"rainshowers_polartwilight.svg",                 SymbolCode::RainShowersPolarTwilight                },
        {"rainshowersandthunder_day.svg",                 SymbolCode::RainShowersAndThunderDay                },
        {"rainshowersandthunder_night.svg",               SymbolCode::RainShowersAndThunderNight              },
        {"rainshowersandthunder_polartwilight.svg",       SymbolCode::RainShowersAndThunderPolarTwilight      },
        {"sleet.svg",                                     SymbolCode::Sleet                                   },
        {"sleetandthunder.svg",                           SymbolCode::SleetAndThunder                         },
        {"sleetshowers_day.svg",                          SymbolCode::SleetShowersDay                         },
        {"sleetshowers_night.svg",                        SymbolCode::SleetShowersNight                       },
        {"sleetshowers_polartwilight.svg",                SymbolCode::SleetShowersPolarTwilight               },
        {"sleetshowersandthunder_day.svg",                SymbolCode::SleetShowersAndThunderDay               },
        {"sleetshowersandthunder_night.svg",              SymbolCode::SleetShowersAndThunderNight             },
        {"sleetshowersandthunder_polartwilight.svg",      SymbolCode::SleetShowersAndThunderPolarTwilight     },
        {"snow.svg",                                      SymbolCode::Snow                                    },
        {"snowandthunder.svg",                            SymbolCode::SnowAndThunder                          },
        {"snowshowers_day.svg",                           SymbolCode::SnowShowersDay                          },
        {"snowshowers_night.svg",                         SymbolCode::SnowShowersNight                        },
        {"snowshowers_polartwilight.svg",                 SymbolCode::SnowShowersPolarTwilight                },
        {"snowshowersandthunder_day.svg",                 SymbolCode::SnowShowersAndThunderDay                },
        {"snowshowersandthunder_night.svg",               SymbolCode::SnowShowersAndThunderNight              },
        {"snowshowersandthunder_polartwilight.svg",       SymbolCode::SnowShowersAndThunderPolarTwilight      },
    };
    return symbolsMap.value(string, SymbolCode::Unknown);
}

QDateTime WeatherTimeseries::time() {
    return d->time;
}

double WeatherTimeseries::temperature() {
    return d->temperature;
}

bool WeatherTimeseries::is1HourDataAvailable() {
    return d->is1HourDataAvailable;
}

double WeatherTimeseries::precipitation1Hour() {
    return d->precipitation1Hour;
}
