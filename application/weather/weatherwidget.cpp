#include "weatherwidget.h"
#include "ui_weatherwidget.h"

WeatherWidget::WeatherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeatherWidget)
{
    ui->setupUi(this);
}

WeatherWidget::~WeatherWidget()
{
    delete ui;
}

void WeatherWidget::setLocation(TroposphereLocation loc)
{
    ui->administrativeAreaLabel->setText(QStringLiteral("%1, %2").arg(loc.admin1, loc.country));
    ui->cityNameLabel->setText(loc.name);
}
