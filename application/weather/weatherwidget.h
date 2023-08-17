#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include "tropospherelocation.h"
#include <QWidget>

namespace Ui {
class WeatherWidget;
}

struct WeatherWidgetPrivate;
class WeatherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherWidget(QWidget *parent = nullptr);
    ~WeatherWidget();

    void setLocation(TroposphereLocation loc);


private:
    Ui::WeatherWidget *ui;
    WeatherWidgetPrivate* d;
};

#endif // WEATHERWIDGET_H
