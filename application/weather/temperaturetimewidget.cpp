#include "temperaturetimewidget.h"
#include "ui_temperaturetimewidget.h"
#include "weathertimeseries.h"

#include "tropospherehelper.h"
#include <QPainter>
#include <QPainterPath>
#include <QScroller>
#include <QTimeZone>
#include <QWheelEvent>
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

    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
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

tPaintCalculator TemperatureTimeWidget::paintCalculator(QPainter* painter) const {
    auto metrics = this->fontMetrics();
    auto scroll = ui->horizontalScrollBar->value();

    tPaintCalculator paintCalculator;
    paintCalculator.setDrawBounds(this->size());
    paintCalculator.setLayoutDirection(this->layoutDirection());

    constexpr int spacing = 9;
    QSize iconSize(32, 32);

    auto timeBottom = spacing + metrics.height();
    auto iconBottom = timeBottom + iconSize.height() + spacing;
    auto temperatureBottom = iconBottom + 100 + spacing;
    auto precipitationBottom = temperatureBottom + 100 + spacing;
    auto windSpeedBottom = precipitationBottom + 100 + spacing;

    paintCalculator.addRect(QRectF(-scroll, iconBottom + spacing, this->width(), 100), [painter, this](QRectF bounds) {
        paintGraph(painter, bounds, tRange(d->timeseries).map<double>([](WeatherTimeseries* timeseries) {
                                                             return timeseries->temperature();
                                                         })
                                        .toList(),
            Qt::yellow);
    });

    paintCalculator.addRect(QRectF(-scroll, temperatureBottom + spacing, this->width(), 100), [painter, this](QRectF bounds) {
        paintGraph(painter, bounds, tRange(d->timeseries).map<double>([](WeatherTimeseries* timeseries) {
                                                             return timeseries->precipitation1Hour();
                                                         })
                                        .toList(),
            Qt::cyan);
    });

    paintCalculator.addRect(QRectF(-scroll, precipitationBottom + spacing, this->width(), 100), [painter, this](QRectF bounds) {
        paintGraph(painter, bounds, tRange(d->timeseries).map<double>([](WeatherTimeseries* timeseries) {
                                                             return timeseries->windSpeed();
                                                         })
                                        .toList(),
            QColor(150, 0, 255));
    });

    for (auto i = 0; i < d->timeseries.length() - 1; i++) {
        auto timeseries = d->timeseries.at(i);
        QRectF bounds(d->paneWidth * i - scroll, 0, d->paneWidth, this->height());
        paintCalculator.addRect(bounds, [painter, timeseries, this, metrics, iconSize, temperatureBottom, precipitationBottom, windSpeedBottom](QRectF bounds) {
            auto date = TroposphereHelper::toLocalTime(timeseries->time(), d->timezone);

            auto time = date.toString(QLocale().timeFormat(QLocale::ShortFormat));
            QRectF timeBounds;
            timeBounds.setWidth(metrics.horizontalAdvance(time) + 1);
            timeBounds.setHeight(metrics.height());
            if (this->layoutDirection() == Qt::LeftToRight) {
                timeBounds.moveLeft(bounds.left() + spacing);
            } else {
                timeBounds.moveRight(bounds.right() - spacing);
            }
            timeBounds.moveTop(bounds.top() + spacing);

            QRectF iconBounds;
            iconBounds.setSize(iconSize);
            if (this->layoutDirection() == Qt::LeftToRight) {
                iconBounds.moveLeft(bounds.left() + spacing);
            } else {
                iconBounds.moveRight(bounds.right() - spacing);
            }
            iconBounds.moveTop(timeBounds.bottom() + spacing);

            auto temperature = TroposphereHelper::readableTemperature(timeseries->temperature());
            QRectF temperatureBounds;
            temperatureBounds.setWidth(metrics.horizontalAdvance(temperature) + 1);
            temperatureBounds.setHeight(metrics.height());
            if (this->layoutDirection() == Qt::LeftToRight) {
                temperatureBounds.moveLeft(bounds.left() + spacing);
            } else {
                temperatureBounds.moveRight(bounds.right() - spacing);
            }
            temperatureBounds.moveTop(temperatureBottom - metrics.height());

            auto precipitation = TroposphereHelper::readablePrecipitation(timeseries->precipitation1Hour());
            QRectF precipitationBounds;
            precipitationBounds.setWidth(metrics.horizontalAdvance(precipitation) + 1);
            precipitationBounds.setHeight(metrics.height());
            if (this->layoutDirection() == Qt::LeftToRight) {
                precipitationBounds.moveLeft(bounds.left() + spacing);
            } else {
                precipitationBounds.moveRight(bounds.right() - spacing);
            }
            precipitationBounds.moveTop(precipitationBottom - metrics.height());

            auto windSpeed = TroposphereHelper::readableSpeed(timeseries->windSpeed());
            QRectF windSpeedBounds;
            windSpeedBounds.setWidth(metrics.horizontalAdvance(windSpeed) + 1);
            windSpeedBounds.setHeight(metrics.height());
            if (this->layoutDirection() == Qt::LeftToRight) {
                windSpeedBounds.moveLeft(bounds.left() + spacing);
            } else {
                windSpeedBounds.moveRight(bounds.right() - spacing);
            }
            windSpeedBounds.moveTop(windSpeedBottom - metrics.height());

            painter->setPen(this->palette().color(QPalette::WindowText));
            painter->drawText(timeBounds, time);

            auto icon = timeseries->iconForWeatherFlag(timeseries->symbolCode1Hour(), iconBounds.size().toSize(), this->devicePixelRatio());
            painter->drawPixmap(iconBounds.toRect(), icon);

            painter->drawText(temperatureBounds, temperature);
            painter->drawText(precipitationBounds, precipitation);
            painter->drawText(windSpeedBounds, windSpeed);

            painter->setPen(this->palette().color(QPalette::Disabled, QPalette::WindowText));
            if (this->layoutDirection() == Qt::LeftToRight) {
                painter->drawLine(bounds.topLeft(), bounds.bottomLeft());
            } else {
                painter->drawLine(bounds.topRight(), bounds.bottomRight());
            }
        });
    }

    return paintCalculator;
}

void TemperatureTimeWidget::updateScrollBar() {
    auto scrollPixels = (d->timeseries.length() - 1) * d->paneWidth;
    ui->horizontalScrollBar->setMaximum(qMax(0, scrollPixels - this->width()));
    ui->horizontalScrollBar->setMinimum(0);
    ui->horizontalScrollBar->setPageStep(this->width());
}

void TemperatureTimeWidget::paintGraph(QPainter* painter, QRectF bounds, QList<double> values, QColor color) const {
    if (values.isEmpty()) return;

    auto transparentFill1 = color;
    transparentFill1.setAlpha(200);

    auto transparentFill2 = color;
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

    auto oldY = bounds.top() + bounds.height() - bounds.height() * (normalised.first() * 0.5 + 0.25);
    path.moveTo(this->layoutDirection() == Qt::LeftToRight ? bounds.left() : bounds.right(), oldY);
    for (auto i = 0; i < normalised.length(); i++) {
        auto x = this->layoutDirection() == Qt::LeftToRight ? (bounds.left() + d->paneWidth * i) : bounds.left() + (bounds.width() - d->paneWidth * i);
        auto y = bounds.top() + bounds.height() - bounds.height() * (normalised.at(i) * 0.5 + 0.25);
        auto adjustedX = x + (this->layoutDirection() == Qt::LeftToRight ? -d->paneWidth : d->paneWidth) / 2;

        path.cubicTo(QPointF(adjustedX, oldY), QPointF(adjustedX, y), QPointF(x, y));
        oldY = y;
    }

    auto linePath = QPainterPath(path);

    path.lineTo(QPointF(path.currentPosition().x(), bounds.bottom()));
    path.lineTo(QPointF(this->layoutDirection() == Qt::LeftToRight ? bounds.left() : bounds.right(), bounds.bottom()));
    painter->fillPath(path, fillGradient);

    painter->setPen(QPen(color, 3));
    painter->drawPath(linePath);
}

void TemperatureTimeWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    auto paintCalculator = this->paintCalculator(&painter);
    paintCalculator.setPainter(&painter);
    paintCalculator.performPaint();
}

void TemperatureTimeWidget::resizeEvent(QResizeEvent* event) {
    updateScrollBar();
}

QSize TemperatureTimeWidget::sizeHint() const {
    auto paintCalculator = this->paintCalculator(nullptr);
    return paintCalculator.anchoredBoundingRect().size().toSize() + QSize(0, ui->horizontalScrollBar->height());
}

void TemperatureTimeWidget::wheelEvent(QWheelEvent* event) {
    ui->horizontalScrollBar->setValue(ui->horizontalScrollBar->value() - event->angleDelta().x());
    event->ignore();
}

void TemperatureTimeWidget::on_horizontalScrollBar_valueChanged(int value) {
    this->update();
}

QSize TemperatureTimeWidget::minimumSizeHint() const {
    return this->sizeHint();
}
