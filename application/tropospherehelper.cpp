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

QDateTime TroposphereHelper::toLocalTime(QDateTime date, QTimeZone timezone) {
    QDateTime newDate(date);
    auto utcOffset = timezone.offsetFromUtc(date);
    return newDate.addSecs(utcOffset);
}
