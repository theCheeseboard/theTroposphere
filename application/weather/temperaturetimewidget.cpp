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
                        .take(25)
                        .toList();

    updateScrollBar();
    this->update();
}

void TemperatureTimeWidget::updateScrollBar() {
    auto scrollPixels = (d->timeseries.length() - 1) * d->paneWidth;
    ui->horizontalScrollBar->setMaximum(qMax(0, scrollPixels - this->width()));
    ui->horizontalScrollBar->setMinimum(0);
    ui->horizontalScrollBar->setPageStep(this->width());
}

void TemperatureTimeWidget::paintGraph(QPainter* painter, QRectF bounds, QList<double> values, QColor lineColor, QColor fillColor) {
    if (values.isEmpty()) return;

    auto transparentFill1 = fillColor;
    transparentFill1.setAlpha(200);

    auto transparentFill2 = fillColor;
    transparentFill2.setAlpha(0);

    QLinearGradient fillGradient;
    fillGradient.setStart(0, bounds.top());
    fillGradient.setFinalStop(0, bounds.bottom());
    fillGradient.setColorAt(0, transparentFill1);
    fillGradient.setColorAt(1, transparentFill2);

    QPainterPath path;

    auto min = tRange(values).min();
    auto max = tRange(values).max();
    if (min == max) max = min + 1;
    auto normalised = tRange(values).map<double>([max, min](double value) {
                                        return (value - min) / (max - min);
                                    })
                          .toList();

    auto oldY = bounds.top() + bounds.height() - bounds.height() * (normalised.first() * 0.6 + 0.2);
    path.moveTo(this->layoutDirection() == Qt::LeftToRight ? bounds.left() : bounds.right(), oldY);
    for (auto i = 0; i < normalised.length(); i++) {
        auto x = this->layoutDirection() == Qt::LeftToRight ? (bounds.left() + d->paneWidth * i) : bounds.left() + (bounds.width() - d->paneWidth * i);
        auto y = bounds.top() + bounds.height() - bounds.height() * (normalised.at(i) * 0.6 + 0.2);

        if (this->layoutDirection() == Qt::LeftToRight) {
            path.cubicTo(QPointF(x - d->paneWidth / 2, oldY), QPointF(x - d->paneWidth / 2, y), QPointF(x, y));
        } else {
            path.cubicTo(QPointF(x + d->paneWidth / 2, y), QPointF(x + d->paneWidth / 2, oldY), QPointF(x, y));
        }
        oldY = y;
    }

    auto linePath = QPainterPath(path);

    path.lineTo(QPointF(path.currentPosition().x(), bounds.bottom()));
    path.lineTo(QPointF(this->layoutDirection() == Qt::LeftToRight ? bounds.left() : bounds.right(), bounds.bottom()));
    painter->fillPath(path, fillGradient);

    painter->setPen(QPen(lineColor, 3));
    painter->drawPath(linePath);
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
    auto precipitationBottom = temperatureBottom + 100 + spacing;

    paintCalculator.addRect(QRectF(-scroll, iconBottom + spacing, this->width(), 100), [&painter, this](QRectF bounds) {
        paintGraph(&painter, bounds, tRange(d->timeseries).map<double>([](WeatherTimeseries* timeseries) {
                                                              return timeseries->temperature();
                                                          })
                                         .toList(),
            Qt::yellow, Qt::yellow);
    });

    paintCalculator.addRect(QRectF(-scroll, temperatureBottom + spacing, this->width(), 100), [&painter, this](QRectF bounds) {
        paintGraph(&painter, bounds, tRange(d->timeseries).map<double>([](WeatherTimeseries* timeseries) {
                                                              return timeseries->precipitation1Hour();
                                                          })
                                         .toList(),
            Qt::cyan, Qt::cyan);
    });

    for (auto i = 0; i < d->timeseries.length() - 1; i++) {
        auto timeseries = d->timeseries.at(i);
        QRectF bounds(d->paneWidth * i - scroll, 0, d->paneWidth, this->height());
        paintCalculator.addRect(bounds, [&painter, timeseries, this, metrics, iconSize, temperatureBottom, precipitationBottom](QRectF bounds) {
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

            auto temperature = TroposphereHelper::readableTemperature(timeseries->temperature());
            QRectF temperatureBounds;
            temperatureBounds.setWidth(metrics.horizontalAdvance(temperature) + 1);
            temperatureBounds.setHeight(metrics.height());
            temperatureBounds.moveCenter(bounds.center());
            temperatureBounds.moveTop(temperatureBottom - metrics.height());

            auto precipitation = tr("%1 mm").arg(timeseries->precipitation1Hour());
            QRectF precipitationBounds;
            precipitationBounds.setWidth(metrics.horizontalAdvance(precipitation) + 1);
            precipitationBounds.setHeight(metrics.height());
            precipitationBounds.moveCenter(bounds.center());
            precipitationBounds.moveTop(precipitationBottom - metrics.height());

            painter.setPen(this->palette().color(QPalette::WindowText));
            painter.drawText(timeBounds, time);

            painter.fillRect(iconBounds, Qt::red);

            painter.drawText(temperatureBounds, temperature);
            painter.drawText(precipitationBounds, precipitation);

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
    sh.setHeight(400);
    return sh;
}

void TemperatureTimeWidget::on_horizontalScrollBar_sliderMoved(int position) {
    this->update();
}
