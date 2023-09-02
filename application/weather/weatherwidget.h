#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include "tropospherelocation.h"
#include <QCoroTask>
#include <QWidget>

namespace Ui {
    class WeatherWidget;
}

struct WeatherWidgetPrivate;
class WeatherWidget : public QWidget {
        Q_OBJECT

    public:
        explicit WeatherWidget(QWidget* parent = nullptr);
        ~WeatherWidget();

        QCoro::Task<> setLocation(TroposphereLocation loc);

    private:
        Ui::WeatherWidget* ui;
        WeatherWidgetPrivate* d;

        // QWidget interface
    protected:
        void resizeEvent(QResizeEvent *event);

        // QWidget interface
    protected:
        void paintEvent(QPaintEvent *event);
};

#endif // WEATHERWIDGET_H
