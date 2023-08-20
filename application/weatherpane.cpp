#include "weatherpane.h"
#include "ui_weatherpane.h"

#if QT_CONFIG(permissions)
    #include <QLocationPermission>
#endif
#include "weather/weatherwidget.h"
#include <QGeoPositionInfoSource>
#include <QTimeZone>

struct WeatherPanePrivate {
        TroposphereLocation location;
        WeatherWidget* weatherWidget = nullptr;
        QGeoPositionInfoSource* geoSource;
};

WeatherPane::WeatherPane(TroposphereLocation location, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WeatherPane) {
    ui->setupUi(this);
    d = new WeatherPanePrivate();
    d->location = location;
    d->geoSource = QGeoPositionInfoSource::createDefaultSource(this);

    d->geoSource->startUpdates();

    this->updateData();
}

WeatherPane::~WeatherPane() {
    delete ui;
    delete d;
}

TroposphereLocation WeatherPane::location() {
    return d->location;
}

QCoro::Task<> WeatherPane::updateData() {
    if (d->location.locatedLocation) {
#if QT_CONFIG(permissions)
        QLocationPermission locationPermission;
        locationPermission.setAccuracy(QLocationPermission::Approximate);
        locationPermission.setAvailability(QLocationPermission::WhenInUse);
        switch (qApp->checkPermission(locationPermission)) {
            case Qt::PermissionStatus::Undetermined:
                ui->stackedWidget->setCurrentWidget(ui->errorPage);
                ui->errorTitle->setText(tr("Permissions Required"));
                ui->errorText->setText(tr("theTroposphere needs to be able to access your location in order to show you the weather at your location."));
                ui->grantPermissionButton->setVisible(true);
                ui->openSystemSettingsButton->setVisible(false);
                return;
            case Qt::PermissionStatus::Denied:
                ui->stackedWidget->setCurrentWidget(ui->errorPage);
                ui->errorTitle->setText(tr("Permissions Required"));
                ui->errorText->setText(tr("theTroposphere needs to be able to access your location in order to show you the weather at your location."));
                ui->grantPermissionButton->setVisible(true);
                ui->openSystemSettingsButton->setVisible(false);
                return;
            case Qt::PermissionStatus::Granted:
                break;
        }

#endif

        auto position = d->geoSource->lastKnownPosition();
        d->location = co_await TroposphereLocation::reverseGeocode(position.coordinate().latitude(), position.coordinate().longitude());
    }

    if (!d->weatherWidget) {
        d->weatherWidget = new WeatherWidget(this);
        ui->weatherWidgetLayout->addWidget(d->weatherWidget);
    }

    d->weatherWidget->setLocation(d->location);
}

void WeatherPane::on_grantPermissionButton_clicked() {
#if QT_CONFIG(permissions)
    QLocationPermission locationPermission;
    locationPermission.setAccuracy(QLocationPermission::Approximate);
    locationPermission.setAvailability(QLocationPermission::WhenInUse);
    qApp->requestPermission(locationPermission, this, &WeatherPane::updateData);
#endif
}

void WeatherPane::on_openSystemSettingsButton_clicked() {
}
