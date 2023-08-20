#ifndef TEMPERATURETIMEWIDGET_H
#define TEMPERATURETIMEWIDGET_H

#include <QWidget>
#include <tpaintcalculator.h>
#include <weatherdata.h>

namespace Ui {
    class TemperatureTimeWidget;
}

struct TemperatureTimeWidgetPrivate;
class TemperatureTimeWidget : public QWidget {
        Q_OBJECT

    public:
        explicit TemperatureTimeWidget(QWidget* parent = nullptr);
        ~TemperatureTimeWidget();

        void setWeatherData(WeatherDataPtr weatherData, QString timezone);

    private:
        Ui::TemperatureTimeWidget* ui;
        TemperatureTimeWidgetPrivate* d;

        tPaintCalculator paintCalculator(QPainter* painter) const;
        void updateScrollBar();
        void paintGraph(QPainter* painter, QRectF bounds, QList<double> values, QColor color) const;

        // QWidget interface
    protected:
        void paintEvent(QPaintEvent* event);
        void resizeEvent(QResizeEvent* event);
        void wheelEvent(QWheelEvent* event);
        QSize sizeHint() const;
        QSize minimumSizeHint() const;

    private slots:
        void on_horizontalScrollBar_valueChanged(int value);
};

#endif // TEMPERATURETIMEWIDGET_H
