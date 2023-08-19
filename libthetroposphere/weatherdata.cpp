#include "weatherdata.h"

#include "weathertimeseries.h"
#include <QCoroNetwork>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <ranges/trange.h>

struct WeatherDataPrivate {
        QDateTime lastUpdated;
        QList<WeatherTimeseries*> timeseries;
};

WeatherData::WeatherData(QObject* parent) :
    QObject{parent} {
    d = new WeatherDataPrivate();
}

WeatherData::~WeatherData() {
    delete d;
}

QCoro::Task<WeatherDataPtr> WeatherData::getWeatherData(double lat, double lng) {
    QNetworkAccessManager mgr;
    QUrl url("https://api.met.no/weatherapi/locationforecast/2.0/complete");
    url.setQuery(QUrlQuery({
        {"lat", QString::number(lat)},
        {"lon", QString::number(lng)}
    }));

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("%1/%2 libthetroposphere/1.0").arg(QCoreApplication::applicationName(), QCoreApplication::applicationVersion()));
    auto reply = co_await mgr.get(req);
    if (reply->error() != QNetworkReply::NoError) {
        // TODO: Return an error message
        co_return {};
    }

    QJsonParseError parseError;
    auto root = QJsonDocument::fromJson(reply->readAll(), &parseError).object();
    if (parseError.error != QJsonParseError::NoError) {
        co_return {};
    }

    auto properties = root.value("properties").toObject();
    auto meta = properties.value("meta").toObject();
    auto timeseries = properties.value("timeseries").toArray();

    WeatherDataPtr weatherData(new WeatherData());
    weatherData->d->lastUpdated = QDateTime::fromString(meta.value("updated_at").toString());
    for (auto item : timeseries) {
        weatherData->d->timeseries.append(new WeatherTimeseries(item.toObject(), weatherData.data()));
    }

    co_return weatherData;
}

tRange<WeatherTimeseries*> WeatherData::timeseries() {
    return tRange(d->timeseries);
}
