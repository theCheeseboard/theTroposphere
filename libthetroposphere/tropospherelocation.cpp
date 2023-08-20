#include "tropospherelocation.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimeZone>

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
