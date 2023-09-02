#ifndef WEATHERPANE_H
#define WEATHERPANE_H

#include <QWidget>
#include <tropospherelocation.h>

namespace Ui {
    class WeatherPane;
}

struct WeatherPanePrivate;
class WeatherPane : public QWidget {
        Q_OBJECT

    public:
        explicit WeatherPane(TroposphereLocation location, QWidget* parent = nullptr);
        ~WeatherPane();

        void setTopPadding(int topPadding);

        TroposphereLocation location();

    private slots:
        void on_grantPermissionButton_clicked();

        void on_openSystemSettingsButton_clicked();

    private:
        Ui::WeatherPane* ui;
        WeatherPanePrivate* d;

        QCoro::Task<> updateData();
};

#endif // WEATHERPANE_H
