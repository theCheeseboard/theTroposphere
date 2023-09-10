#include "tropospherehelper.h"

#include <tsettings.h>

QString TroposphereHelper::readableTemperature(double temperature) {
    tSettings settings;
    auto units = settings.value("units/temperature");
    if (units == "system") {
        switch (QLocale().measurementSystem()) {
            case QLocale::MetricSystem:
            case QLocale::ImperialUKSystem:
                units = "metric";
                break;
            case QLocale::ImperialUSSystem:
                units = "imperial";
                break;
        }
    }

    if (units == "imperial") {
        return tr("%1 ℉").arg(temperature * 9 / 5 + 32, 0, 'f', 1);
    } else {
        return tr("%1 ℃").arg(temperature, 0, 'f', 1);
    }
}

QString TroposphereHelper::readableSpeed(double metersPerSecond) {
    double kilometersPerHour = metersPerSecond * 3.6;
    switch (QLocale().measurementSystem()) {
        case QLocale::MetricSystem:
            return tr("%1 km/h").arg(kilometersPerHour, 0, 'f', 2);
        case QLocale::ImperialUKSystem:
        case QLocale::ImperialUSSystem:
            return tr("%1 mph").arg(kilometersPerHour * 1.609344, 0, 'f', 2);
    }
}

QString TroposphereHelper::readablePrecipitation(double millimeters) {
    switch (QLocale().measurementSystem()) {
        case QLocale::MetricSystem:
            return tr("%1 mm").arg(millimeters, 0, 'f', 1);
        case QLocale::ImperialUKSystem:
        case QLocale::ImperialUSSystem:
            return tr("%1 in").arg(millimeters / 25.4, 0, 'f', 1);
    }
}

QDateTime TroposphereHelper::toLocalTime(QDateTime date, QTimeZone timezone) {
    QDateTime newDate(date);
    auto utcOffset = timezone.offsetFromUtc(date);
    return newDate.addSecs(utcOffset);
}
