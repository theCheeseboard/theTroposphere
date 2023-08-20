#include "tropospherelocation.h"

#include <QCoroNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QTimeZone>
#include <tlogger.h>

TroposphereLocation::TroposphereLocation() {
    this->locatedLocation = true;
    this->timezone = QTimeZone::systemTimeZoneId();
}

TroposphereLocation::TroposphereLocation(QString serialised) {
    auto rootObj = QJsonDocument::fromJson(QByteArray::fromBase64(serialised.toUtf8())).object();
    this->name = rootObj.value("name").toString();
    this->admin1 = rootObj.value("admin1").toString();
    this->country = rootObj.value("country").toString();
    this->timezone = rootObj.value("timezone").toString();
    this->lat = rootObj.value("lat").toDouble();
    this->lng = rootObj.value("lng").toDouble();
}

TroposphereLocation::TroposphereLocation(QString name, QString admin1, QString country, QString timezone, double lat, double lng) {
    this->locatedLocation = false;
    this->name = name;
    this->admin1 = admin1;
    this->country = country;
    this->timezone = timezone;
    this->lat = lat;
    this->lng = lng;
}

QCoro::Task<TroposphereLocation> TroposphereLocation::reverseGeocode(double lat, double lng) {
    QJsonObject payload;
    payload.insert("lat", lat);
    payload.insert("lng", lng);

    QNetworkRequest request(QStringLiteral("https://api-thetroposphere.vicr123.com/api/locations/reverse"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("%1/%2 libthetroposphere/1.0").arg(QCoreApplication::applicationName(), QCoreApplication::applicationVersion()));

    QNetworkAccessManager mgr;
    auto response = co_await mgr.post(request, QJsonDocument(payload).toJson(QJsonDocument::Compact));
    if (response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
        // Error!
        tWarn("AddLocationPopover") << "Geocode request failed";
        tWarn("AddLocationPopover") << "Geocode request returned " << response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        throw TroposphereReverseGeocodeException();
    }

    QJsonParseError parseError;
    auto city = QJsonDocument::fromJson(response->readAll(), &parseError).object();
    if (parseError.error != QJsonParseError::NoError) {
        // Error!
        tWarn("AddLocationPopover") << "Search request failed";
        tWarn("AddLocationPopover") << "JSON parse of response failed";
        throw TroposphereReverseGeocodeException();
    }

    TroposphereLocation tl(city.value("name").toString(), city.value("admin1").toString(), city.value("countryCode").toString(), city.value("timezone").toString(), city.value("lat").toDouble(), city.value("lng").toDouble());
    tl.locatedLocation = true;
    co_return tl;
}

QString TroposphereLocation::serialise() {
    QJsonObject rootObj;
    rootObj.insert("name", this->name);
    rootObj.insert("admin1", this->admin1);
    rootObj.insert("country", this->country);
    rootObj.insert("timezone", this->timezone);
    rootObj.insert("lat", this->lat);
    rootObj.insert("lng", this->lng);

    return QJsonDocument(rootObj).toJson(QJsonDocument::Compact).toBase64();
}

T_EXCEPTION_IMPL(TroposphereReverseGeocodeException);
