#include "weathertimeseries.h"

#include <QJsonObject>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>
#include <tlogger.h>

struct WeatherTimeseriesPrivate {
        QDateTime time;

        double temperature;   // degrees celsius
        double humidity;      // percentage
        double windSpeed;     // meters per second
        double windDirection; // angular degrees

        bool is1HourDataAvailable = false;              // boolean
        double precipitation1Hour;                      // millimeters
        WeatherTimeseries::WeatherFlag symbolCode1Hour; // symbol code

        static QMap<QString, WeatherTimeseries::WeatherFlag> symbolsToFlagsMap;
};

QMap<QString, WeatherTimeseries::WeatherFlag> WeatherTimeseriesPrivate::symbolsToFlagsMap = {
    {"clearsky_day",                               WeatherTimeseries::WeatherFlag::Sun                                                                                                                                                                           },
    {"clearsky_night",                             WeatherTimeseries::WeatherFlag::Moon                                                                                                                                                                          },
    {"clearsky_polartwilight",                     WeatherTimeseries::WeatherFlag::PolarTwilight                                                                                                                                                                 },
    {"cloudy",                                     WeatherTimeseries::WeatherFlag::Cloudy                                                                                                                                                                        },
    {"fair_day",                                   WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Fair                                                                                                                                    },
    {"fair_night",                                 WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Fair                                                                                                                                   },
    {"fair_polartwilight",                         WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Fair                                                                                                                          },
    {"fog",                                        WeatherTimeseries::WeatherFlag::Fog                                                                                                                                                                           },
    {"heavyrain",                                  WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain                                                                                                                                  },
    {"heavyrainandthunder",                        WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Thunder                                                                                        },
    {"heavyrainshowers_day",                       WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Sun                                                                                            },
    {"heavyrainshowers_night",                     WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Moon                                                                                           },
    {"heavyrainshowers_polartwilight",             WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::PolarTwilight                                                                                  },
    {"heavyrainshowersandthunder_day",             WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Thunder                                                  },
    {"heavyrainshowersandthunder_night",           WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Thunder                                                 },
    {"heavyrainshowersandthunder_polartwilight",   WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Thunder                                        },
    {"heavysleet",                                 WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow                                                                                           },
    {"heavysleetandthunder",                       WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Thunder                                                 },
    {"heavysleetshowers_day",                      WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Heavy                                                     },
    {"heavysleetshowers_night",                    WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Heavy                                                    },
    {"heavysleetshowers_polartwilight",            WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Heavy                                           },
    {"heavysleetshowersandthunder_day",            WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Thunder           },
    {"heavysleetshowersandthunder_night",          WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Thunder          },
    {"heavysleetshowersandthunder_polartwilight",  WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Thunder },
    {"heavysnow",                                  WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Snow                                                                                                                                  },
    {"heavysnowandthunder",                        WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Thunder                                                                                        },
    {"heavysnowshowers_day",                       WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Heavy                                                                                            },
    {"heavysnowshowers_night",                     WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Heavy                                                                                           },
    {"heavysnowshowers_polartwilight",             WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Heavy                                                                                  },
    {"heavysnowshowersandthunder_day",             WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Thunder                                                  },
    {"heavysnowshowersandthunder_night",           WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Thunder                                                 },
    {"heavysnowshowersandthunder_polartwilight",   WeatherTimeseries::WeatherFlag::Heavy | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Thunder                                        },
    {"lightrain",                                  WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain                                                                                                                                  },
    {"lightrainandthunder",                        WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Thunder                                                                                        },
    {"lightrainshowers_day",                       WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Sun                                                                                            },
    {"lightrainshowers_night",                     WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Moon                                                                                           },
    {"lightrainshowers_polartwilight",             WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::PolarTwilight                                                                                  },
    {"lightrainshowersandthunder_day",             WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Thunder                                                  },
    {"lightrainshowersandthunder_night",           WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Thunder                                                 },
    {"lightrainshowersandthunder_polartwilight",   WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Thunder                                        },
    {"lightsleet",                                 WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow                                                                                           },
    {"lightsleetandthunder",                       WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Thunder                                                 },
    {"lightsleetshowers_day",                      WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Sun                                                     },
    {"lightsleetshowers_night",                    WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Moon                                                    },
    {"lightsleetshowers_polartwilight",            WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::PolarTwilight                                           },
    {"lightsnow",                                  WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Snow                                                                                                                                  },
    {"lightsnowandthunder",                        WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Thunder                                                                                        },
    {"lightsnowshowers_day",                       WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Light                                                                                            },
    {"lightsnowshowers_night",                     WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Light                                                                                           },
    {"lightsnowshowers_polartwilight",             WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Light                                                                                  },
    {"lightssleetshowersandthunder_day",           WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Thunder           },
    {"lightssleetshowersandthunder_night",         WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Thunder          },
    {"lightssleetshowersandthunder_polartwilight", WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Thunder },
    {"lightssnowshowersandthunder_day",            WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Thunder                                                  },
    {"lightssnowshowersandthunder_night",          WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Thunder                                                 },
    {"lightssnowshowersandthunder_polartwilight",  WeatherTimeseries::WeatherFlag::Light | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Thunder                                        },
    {"partlycloudy_day",                           WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Cloudy                                                                                                                                  },
    {"partlycloudy_night",                         WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Cloudy                                                                                                                                 },
    {"partlycloudy_polartwilight",                 WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Cloudy                                                                                                                        },
    {"rain",                                       WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Medium                                                                                                                                 },
    {"rainandthunder",                             WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Thunder | WeatherTimeseries::WeatherFlag::Medium                                                                                       },
    {"rainshowers_day",                            WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Medium                                                                                           },
    {"rainshowers_night",                          WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Medium                                                                                          },
    {"rainshowers_polartwilight",                  WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Medium                                                                                 },
    {"rainshowersandthunder_day",                  WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Medium | WeatherTimeseries::WeatherFlag::Thunder                                                 },
    {"rainshowersandthunder_night",                WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Medium | WeatherTimeseries::WeatherFlag::Thunder                                                },
    {"rainshowersandthunder_polartwilight",        WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Medium | WeatherTimeseries::WeatherFlag::Thunder                                       },
    {"sleet",                                      WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Medium                                                                                          },
    {"sleetandthunder",                            WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Thunder | WeatherTimeseries::WeatherFlag::Medium                                                },
    {"sleetshowers_day",                           WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Medium                                                    },
    {"sleetshowers_night",                         WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Medium                                                   },
    {"sleetshowers_polartwilight",                 WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Medium                                          },
    {"sleetshowersandthunder_day",                 WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Medium | WeatherTimeseries::WeatherFlag::Thunder          },
    {"sleetshowersandthunder_night",               WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Medium | WeatherTimeseries::WeatherFlag::Thunder         },
    {"sleetshowersandthunder_polartwilight",       WeatherTimeseries::WeatherFlag::Rain | WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Medium | WeatherTimeseries::WeatherFlag::Thunder},
    {"snow",                                       WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Medium                                                                                                                                 },
    {"snowandthunder",                             WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Thunder | WeatherTimeseries::WeatherFlag::Medium                                                                                       },
    {"snowshowers_day",                            WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Medium                                                                                           },
    {"snowshowers_night",                          WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Medium                                                                                          },
    {"snowshowers_polartwilight",                  WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Medium                                                                                 },
    {"snowshowersandthunder_day",                  WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Sun | WeatherTimeseries::WeatherFlag::Medium | WeatherTimeseries::WeatherFlag::Thunder                                                 },
    {"snowshowersandthunder_night",                WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::Moon | WeatherTimeseries::WeatherFlag::Medium | WeatherTimeseries::WeatherFlag::Thunder                                                },
    {"snowshowersandthunder_polartwilight",        WeatherTimeseries::WeatherFlag::Snow | WeatherTimeseries::WeatherFlag::PolarTwilight | WeatherTimeseries::WeatherFlag::Medium | WeatherTimeseries::WeatherFlag::Thunder                                       },
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
        d->symbolCode1Hour = weatherFlagForString(next1Hour.value("summary").toObject().value("symbol_code").toString());
    }
}

WeatherTimeseries::~WeatherTimeseries() {
    delete d;
}

WeatherTimeseries::WeatherFlag WeatherTimeseries::weatherFlagForString(QString string) {
    if (!d->symbolsToFlagsMap.contains(string)) {
        tWarn("WeatherTimeseries") << "No flag mapping available for " << string;
    }
    return d->symbolsToFlagsMap.value(string);
}

QPixmap WeatherTimeseries::iconForWeatherFlag(WeatherFlag symbolCode, QSize size, qreal devicePixelRatio) {
    QStringList icons;
    //    auto filename = d->symbolsToFlagsMap.key(symbolCode);

    if (symbolCode & WeatherFlag::Thunder || symbolCode & WeatherFlag::Rain || symbolCode & WeatherFlag::Snow || symbolCode & WeatherFlag::Fog) {
        // Need to draw an upper cloud
        if (symbolCode & WeatherFlag::Sun) {
            icons.append("sun-small");
        } else if (symbolCode & WeatherFlag::Moon || symbolCode & WeatherFlag::PolarTwilight) {
            icons.append("moon-small");
        }

        icons.append(QStringLiteral("cloud-upper"));

        QString intensity;
        if (symbolCode & WeatherFlag::Light) {
            intensity = QStringLiteral("light");
        } else if (symbolCode & WeatherFlag::Medium) {
            intensity = QStringLiteral("heavy");
        } else if (symbolCode & WeatherFlag::Heavy) {
            intensity = QStringLiteral("heavy");
        }

        if (symbolCode & WeatherFlag::Fog) {
            icons.append(QStringLiteral("fog"));
        } else if (symbolCode & WeatherFlag::Thunder && symbolCode & WeatherFlag::Rain & symbolCode & WeatherFlag::Snow) {
            icons.append(QStringLiteral("rain-left-%1").arg(intensity));
            icons.append(QStringLiteral("thunder"));
            icons.append(QStringLiteral("snow-right-%1").arg(intensity));
        } else if (symbolCode & WeatherFlag::Thunder && symbolCode & WeatherFlag::Rain) {
            icons.append(QStringLiteral("rain-left-%1").arg(intensity));
            icons.append(QStringLiteral("thunder"));
            icons.append(QStringLiteral("rain-right-%1").arg(intensity));
        } else if (symbolCode & WeatherFlag::Thunder && symbolCode & WeatherFlag::Snow) {
            icons.append(QStringLiteral("snow-left-%1").arg(intensity));
            icons.append(QStringLiteral("thunder"));
            icons.append(QStringLiteral("snow-right-%1").arg(intensity));
        } else if (symbolCode & WeatherFlag::Rain && symbolCode & WeatherFlag::Snow) {
            icons.append(QStringLiteral("rain-left-%1").arg(intensity));
            icons.append(QStringLiteral("snow-centre-%1").arg(intensity));
            icons.append(QStringLiteral("rain-right-%1").arg(intensity));
        } else if (symbolCode & WeatherFlag::Rain) {
            icons.append(QStringLiteral("rain-left-%1").arg(intensity));
            icons.append(QStringLiteral("rain-centre-%1").arg(intensity));
            icons.append(QStringLiteral("rain-right-%1").arg(intensity));
        } else if (symbolCode & WeatherFlag::Snow) {
            icons.append(QStringLiteral("snow-left-%1").arg(intensity));
            icons.append(QStringLiteral("snow-centre-%1").arg(intensity));
            icons.append(QStringLiteral("snow-right-%1").arg(intensity));
        }
    } else if (symbolCode & WeatherFlag::Fair) {
        // Need to draw a mini cloud
        if (symbolCode & WeatherFlag::Sun) {
            icons.append("sun-medium");
        } else if (symbolCode & WeatherFlag::Moon || symbolCode & WeatherFlag::PolarTwilight) {
            icons.append("moon-medium");
        }

        icons.append("cloud-lower");
    } else if (symbolCode & WeatherFlag::Cloudy) {
        // Need to draw a centre cloud
        if (symbolCode & WeatherFlag::Sun) {
            icons.append("sun-small-centre");
        } else if (symbolCode & WeatherFlag::Moon || symbolCode & WeatherFlag::PolarTwilight) {
            icons.append("moon-small-centre");
        }

        icons.append("cloud");
    } else if (symbolCode & WeatherFlag::Sun) {
        icons.append(QStringLiteral("sun"));
    } else if (symbolCode & WeatherFlag::Moon || symbolCode & WeatherFlag::PolarTwilight) {
        icons.append(QStringLiteral("moon"));
    }

    QPixmap px(size * devicePixelRatio);
    px.fill(Qt::transparent);
    px.setDevicePixelRatio(devicePixelRatio);

    QPainter painter(&px);
    for (const auto& icon : icons) {
        QSvgRenderer renderer(QStringLiteral(":/libthetroposphere/weathericons/%1.svg").arg(icon));
        renderer.render(&painter, QRectF(0, 0, size.width(), size.height()));
    }
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

WeatherTimeseries::WeatherFlag WeatherTimeseries::symbolCode1Hour() {
    return d->symbolCode1Hour;
}

WeatherTimeseries::WeatherFlagWrapper operator|(WeatherTimeseries::WeatherFlag a, WeatherTimeseries::WeatherFlag b) {
    return WeatherTimeseries::WeatherFlagWrapper(static_cast<WeatherTimeseries::WeatherFlag>(static_cast<std::underlying_type<WeatherTimeseries::WeatherFlag>::type>(a) |
                                                                                             static_cast<std::underlying_type<WeatherTimeseries::WeatherFlag>::type>(b)));
}

WeatherTimeseries::WeatherFlag& operator|=(WeatherTimeseries::WeatherFlag& a, WeatherTimeseries::WeatherFlag b) {
    return a = a | b;
}

WeatherTimeseries::WeatherFlagWrapper operator&(WeatherTimeseries::WeatherFlag a, WeatherTimeseries::WeatherFlag b) {
    return WeatherTimeseries::WeatherFlagWrapper(static_cast<WeatherTimeseries::WeatherFlag>(static_cast<std::underlying_type<WeatherTimeseries::WeatherFlag>::type>(a) &
                                                                                             static_cast<std::underlying_type<WeatherTimeseries::WeatherFlag>::type>(b)));
}

WeatherTimeseries::WeatherFlag& operator&=(WeatherTimeseries::WeatherFlag& a, WeatherTimeseries::WeatherFlag b) {
    return a = a & b;
}

WeatherTimeseries::WeatherFlagWrapper::WeatherFlagWrapper(WeatherFlag flag) {
    _flag = flag;
}

WeatherTimeseries::WeatherFlagWrapper::operator WeatherFlag() {
    return _flag;
}

WeatherTimeseries::WeatherFlagWrapper::operator bool() {
    return static_cast<bool>(_flag);
}
