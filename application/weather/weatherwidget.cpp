#include "weatherwidget.h"
#include "ui_weatherwidget.h"

#include "tropospherehelper.h"
#include <QPainter>
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

    auto pal = this->palette();
    pal.setColor(QPalette::WindowText, Qt::white);
    this->setPalette(pal);
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
        ui->temperatureTimeWidget->setWeatherData(d->weatherData, loc.timezone);
        ui->iconLabel->setPixmap(currentWeather->iconForWeatherFlag(currentWeather->symbolCode1Hour(), {32, 32}, this->devicePixelRatio()));
    } catch (tRangeException ex) {
        ui->mainTemperatureLabel->setText(QStringLiteral("--"));
    }

    this->update();
}

void WeatherWidget::resizeEvent(QResizeEvent* event) {
    ui->containerWidget->setFixedWidth(qMin(900, this->width()));
}

void WeatherWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    if (d->weatherData) {
        auto currentWeather = d->weatherData->timeseries().first();
        auto weatherFlag = currentWeather->symbolCode1Hour();
        if ((weatherFlag & WeatherTimeseries::WeatherFlag::Thunder || weatherFlag & WeatherTimeseries::WeatherFlag::Snow || weatherFlag & WeatherTimeseries::WeatherFlag::Rain || weatherFlag & WeatherTimeseries::WeatherFlag::Fog || weatherFlag & WeatherTimeseries::WeatherFlag::Cloudy) && !(weatherFlag & WeatherTimeseries::WeatherFlag::Fair)) {
            QLinearGradient gradient({0, 0}, {0, static_cast<qreal>(this->height())});
            gradient.setColorAt(0, QColor(100, 100, 100));
            gradient.setColorAt(1, QColor(50, 50, 50));
            painter.fillRect(QRect(0, 0, this->width(), this->height()), gradient);
        } else if (weatherFlag & WeatherTimeseries::WeatherFlag::Moon || weatherFlag & WeatherTimeseries::WeatherFlag::PolarTwilight) {
            QLinearGradient gradient({0, 0}, {0, static_cast<qreal>(this->height())});
            gradient.setColorAt(0, QColor(0, 0, 100));
            gradient.setColorAt(1, QColor(0, 0, 50));
            painter.fillRect(QRect(0, 0, this->width(), this->height()), gradient);
        } else if (weatherFlag & WeatherTimeseries::WeatherFlag::Sun) {
            QLinearGradient gradient({0, 0}, {0, static_cast<qreal>(this->height())});
            gradient.setColorAt(0, QColor(0, 200, 200));
            gradient.setColorAt(1, QColor(0, 125, 200));
            painter.fillRect(QRect(0, 0, this->width(), this->height()), gradient);
        }
    }
}
