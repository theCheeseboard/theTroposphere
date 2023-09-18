#include "temperatureunitsettingspane.h"
#include "ui_temperatureunitsettingspane.h"

#include <tsettings.h>

struct TemperatureUnitSettingsPanePrivate {
        tSettings settings;
};

TemperatureUnitSettingsPane::TemperatureUnitSettingsPane(QWidget* parent) :
    tSettingsPane(parent),
    ui(new Ui::TemperatureUnitSettingsPane) {
    ui->setupUi(this);
    d = new TemperatureUnitSettingsPanePrivate();

    auto units = d->settings.value("units/temperature").toString();
    if (units == "imperial") {
        ui->fahrenheitRadioButton->setChecked(true);
    } else if (units == "metric") {
        ui->celsiusRadioButton->setChecked(true);
    } else {
        ui->systemRadioButton->setChecked(true);
    }
}

TemperatureUnitSettingsPane::~TemperatureUnitSettingsPane() {
    delete ui;
    delete d;
}

QString TemperatureUnitSettingsPane::paneName() {
    return tr("Temperature");
}

void TemperatureUnitSettingsPane::on_systemRadioButton_toggled(bool checked) {
    if (checked) {
        d->settings.setValue("units/temperature", "system");
    }
}

void TemperatureUnitSettingsPane::on_celsiusRadioButton_toggled(bool checked) {
    d->settings.setValue("units/temperature", "metric");
}

void TemperatureUnitSettingsPane::on_fahrenheitRadioButton_toggled(bool checked) {
    d->settings.setValue("units/temperature", "imperial");
}
