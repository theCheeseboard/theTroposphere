#include "settings/temperatureunitsettingspane.h"
#include <QCommandLineParser>
#include <QJsonArray>
#include <QUrl>
#include <tapplication.h>
#include <tsettings.h>
#include <tsettingswindow/tsettingswindow.h>
#include <tstylemanager.h>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    tApplication a(argc, argv);
    a.setApplicationShareDir("thetroposphere");
    a.installTranslators();

    a.setApplicationVersion("1.0");
    a.setGenericName(QApplication::translate("main", "Weather"));
    a.setApplicationLicense(tApplication::Gpl3OrLater);
    a.setCopyrightHolder("Victor Tran");
    a.setCopyrightYear("2023");
    a.setOrganizationName("theCheeseboard");
    a.setApplicationUrl(tApplication::Sources, QUrl("http://github.com/theCheeseboard/theTroposphere"));
    a.setApplicationUrl(tApplication::FileBug, QUrl("http://github.com/theCheeseboard/theTroposphere/issues"));
    a.setApplicationName(T_APPMETA_READABLE_NAME);
    a.setDesktopFileName(T_APPMETA_DESKTOP_ID);
    a.addCopyrightLine(tApplication::translate("main", "Weather data from MET Norway"));

#if defined(Q_OS_MAC)
    a.setQuitOnLastWindowClosed(false);
#endif

    tSettingsWindow::addStaticSection(0, "units", a.translate("main", "Units"));
    tSettingsWindow::addStaticPane(10, "units", [] {
        return new TemperatureUnitSettingsPane();
    });

    tSettings settings;
    QObject::connect(&settings, &tSettings::settingChanged, [=](QString key, QVariant value) {
        if (key == "theme/mode") {
            tStyleManager::setOverrideStyleForApplication(value.toString() == "light" ? tStyleManager::ContemporaryLight : tStyleManager::ContemporaryDark);
        }
    });
    tStyleManager::setOverrideStyleForApplication(settings.value("theme/mode").toString() == "light" ? tStyleManager::ContemporaryLight : tStyleManager::ContemporaryDark);

    MainWindow* w = new MainWindow();
    w->show();

    int retval = a.exec();
    return retval;
}
