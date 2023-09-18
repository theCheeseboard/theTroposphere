#ifndef TEMPERATUREUNITSETTINGSPANE_H
#define TEMPERATUREUNITSETTINGSPANE_H

#include <tsettingswindow/tsettingspane.h>

namespace Ui {
    class TemperatureUnitSettingsPane;
}

struct TemperatureUnitSettingsPanePrivate;
class TemperatureUnitSettingsPane : public tSettingsPane {
        Q_OBJECT

    public:
        explicit TemperatureUnitSettingsPane(QWidget* parent = nullptr);
        ~TemperatureUnitSettingsPane();

    private:
        Ui::TemperatureUnitSettingsPane* ui;

        TemperatureUnitSettingsPanePrivate* d;

        // tSettingsPane interface
    public:
        QString paneName();
    private slots:
        void on_systemRadioButton_toggled(bool checked);
        void on_celsiusRadioButton_toggled(bool checked);
        void on_fahrenheitRadioButton_toggled(bool checked);
};

#endif // TEMPERATUREUNITSETTINGSPANE_H
