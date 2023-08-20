#ifndef TEMPERATURETIMEWIDGET_H
#define TEMPERATURETIMEWIDGET_H

#include <QWidget>
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

        void updateScrollBar();
        void paintGraph(QPainter* painter, QRectF bounds, QList<double> values, QColor lineColor, QColor fillColor);

        // QWidget interface
    protected:
        void paintEvent(QPaintEvent* event);
        void resizeEvent(QResizeEvent* event);
        QSize sizeHint() const;

    private slots:
        void on_horizontalScrollBar_sliderMoved(int position);
};

#endif // TEMPERATURETIMEWIDGET_H
