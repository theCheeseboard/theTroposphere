#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tropospherelocation.h"
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class RepositoryBrowser;
struct MainWindowPrivate;
class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

        void openRepo(QString path);

        RepositoryBrowser* openNextTab();

    private slots:
        void on_actionExit_triggered();

        void on_actionClose_Tab_triggered();

        void on_stackedWidget_currentChanged(int arg1);

        void on_actionAdd_City_triggered();

    private:
        Ui::MainWindow* ui;
        MainWindowPrivate* d;

        void updateMenuItems();
        void newTab();
        void addTab(TroposphereLocation location);
        void saveLocations();

        // QWidget interface
    protected:
        void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
