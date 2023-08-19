#include "weatherwidget.h"
#include "ui_weatherwidget.h"

#include "tropospherehelper.h"
#include <weatherdata.h>
#include <weathertimeseries.h>

struct WeatherWidgetPrivate {
        WeatherDataPtr weatherData;
};

WeatherWidget::WeatherWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WeatherWidget) {
    ui->setupUi(this);
    d = new WeatherWidgetPrivate();
}

WeatherWidget::~WeatherWidget() {
    delete ui;
    delete d;
}

QCoro::Task<> WeatherWidget::setLocation(TroposphereLocation loc) {
    ui->administrativeAreaLabel->setText(QStringLiteral("%1, %2").arg(loc.admin1, loc.country));
    ui->cityNameLabel->setText(loc.name);

    d->weatherData = co_await WeatherData::getWeatherData(loc.lat, loc.lng);

    try {
        auto currentWeather = d->weatherData->timeseries().first();
        ui->mainTemperatureLabel->setText(TroposphereHelper::readableTemperature(currentWeather->temperature()));
    } catch (tRangeException ex) {
        ui->mainTemperatureLabel->setText(QStringLiteral("--"));
    }
}

void WeatherWidget::resizeEvent(QResizeEvent* event) {
    ui->containerWidget->setFixedWidth(qMin(900, this->width()));
}
