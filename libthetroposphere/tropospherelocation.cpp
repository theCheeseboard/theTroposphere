#include "tropospherelocation.h"

#include <QJsonDocument>
#include <QJsonObject>

TroposphereLocation::TroposphereLocation()
{
    this->locatedLocation = true;
}

TroposphereLocation::TroposphereLocation(QString serialised)
{
    auto rootObj = QJsonDocument::fromJson(QByteArray::fromBase64(serialised.toUtf8())).object();
    this->name = rootObj.value("name").toString();
    this->admin1 = rootObj.value("admin1").toString();
    this->country = rootObj.value("country").toString();
    this->lat = rootObj.value("lat").toDouble();
    this->lng = rootObj.value("lng").toDouble();
}

TroposphereLocation::TroposphereLocation(QString name, QString admin1, QString country, double lat, double lng)
{
    this->locatedLocation = false;
    this->name = name;
    this->admin1 = admin1;
    this->country = country;
    this->lat = lat;
    this->lng = lng;
}

QString TroposphereLocation::serialise()
{
    QJsonObject rootObj;
    rootObj.insert("name", this->name);
    rootObj.insert("admin1", this->admin1);
    rootObj.insert("country", this->country);
    rootObj.insert("lat", this->lat);
    rootObj.insert("lng", this->lng);

    return QJsonDocument(rootObj).toJson(QJsonDocument::Compact).toBase64();
}
