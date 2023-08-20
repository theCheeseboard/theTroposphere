#include "temperaturetimewidget.h"
#include "ui_temperaturetimewidget.h"
#include "weathertimeseries.h"

#include "tropospherehelper.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimeZone>
#include <tpaintcalculator.h>

struct TemperatureTimeWidgetPrivate {
        static constexpr int paneWidth = 100;
        WeatherDataPtr weatherData;
        QList<WeatherTimeseries*> timeseries;
        QTimeZone timezone;
};

TemperatureTimeWidget::TemperatureTimeWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TemperatureTimeWidget) {
    ui->setupUi(this);
    d = new TemperatureTimeWidgetPrivate();
}

TemperatureTimeWidget::~TemperatureTimeWidget() {
    delete ui;
    delete d;
}

void TemperatureTimeWidget::setWeatherData(WeatherDataPtr weatherData, QString timezone) {
    d->weatherData = weatherData;
    d->timezone = QTimeZone(timezone.toUtf8());

    // Only include timeseries where we have data for the next hour
    d->timeseries = d->weatherData->timeseries().filter([](WeatherTimeseries* timeseries) {
                                                    return timeseries->is1HourDataAvailable();
                                                })
                        .take(24)
                        .toList();

    updateScrollBar();
    this->update();
}

void TemperatureTimeWidget::updateScrollBar() {
    auto scrollPixels = d->timeseries.length() * d->paneWidth;
    ui->horizontalScrollBar->setMaximum(qMax(0, scrollPixels - this->width()));
    ui->horizontalScrollBar->setMinimum(0);
    ui->horizontalScrollBar->setPageStep(this->width());
}

void TemperatureTimeWidget::paintGraph(QPainter* painter, QRectF bounds, QList<double> values, QColor lineColor, QColor fillColor) {
    QPainterPath path;

    painter->fillRect(bounds, fillColor);
}

void TemperatureTimeWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    auto metrics = painter.fontMetrics();
    auto scroll = ui->horizontalScrollBar->value();

    tPaintCalculator paintCalculator;
    paintCalculator.setDrawBounds(this->size());
    paintCalculator.setLayoutDirection(this->layoutDirection());
    paintCalculator.setPainter(&painter);

    constexpr int spacing = 9;
    QSize iconSize(32, 32);

    auto timeBottom = spacing + metrics.height();
    auto iconBottom = timeBottom + iconSize.height() + spacing;
    auto temperatureBottom = iconBottom + 100 + spacing;

    paintCalculator.addRect(QRectF(0, iconBottom + spacing, this->width(), 100), [&painter, this](QRectF bounds) {
        paintGraph(&painter, bounds, {0, 1, 0, 2, 0, 3, -1}, Qt::red, Qt::red);
    });

    for (auto i = 0; i < d->timeseries.length(); i++) {
        auto timeseries = d->timeseries.at(i);
        QRectF bounds(d->paneWidth * i - scroll, 0, d->paneWidth, this->height());
        paintCalculator.addRect(bounds, [&painter, timeseries, this, metrics, iconSize](QRectF bounds) {
            auto date = TroposphereHelper::toLocalTime(timeseries->time(), d->timezone);

            auto time = date.toString(QLocale().timeFormat(QLocale::ShortFormat));
            QRectF timeBounds;
            timeBounds.setWidth(metrics.horizontalAdvance(time));
            timeBounds.setHeight(metrics.height());
            timeBounds.moveCenter(bounds.center());
            timeBounds.moveTop(bounds.top() + spacing);

            QRectF iconBounds;
            iconBounds.setSize(iconSize);
            iconBounds.moveCenter(bounds.center());
            iconBounds.moveTop(timeBounds.bottom() + spacing);

            painter.setPen(this->palette().color(QPalette::WindowText));
            painter.drawText(timeBounds, time);

            painter.fillRect(iconBounds, Qt::red);

            //            painter.setBrush(Qt::red);
            //            painter.drawRect(bounds);
        });
    }

    paintCalculator.performPaint();
}

void TemperatureTimeWidget::resizeEvent(QResizeEvent* event) {
    updateScrollBar();
}

QSize TemperatureTimeWidget::sizeHint() const {
    auto sh = QWidget::sizeHint();
    sh.setHeight(300);
    return sh;
}

void TemperatureTimeWidget::on_horizontalScrollBar_sliderMoved(int position) {
    this->update();
}
