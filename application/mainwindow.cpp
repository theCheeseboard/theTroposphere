#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "addlocationpopover.h"
#include "weatherpane.h"
#include <ranges/trange.h>
#include <tapplication.h>
#include <tcsdtools.h>
#include <thelpmenu.h>
#include <tjobmanager.h>
#include <tpopover.h>
#include <tsettings.h>
#include <tsettingswindow/tsettingswindow.h>
#include <twindowtabberbutton.h>

struct MainWindowPrivate {
        QList<WeatherPane*> weatherPanes;
        tCsdTools csd;
        tSettings settings;
};

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    d = new MainWindowPrivate();
    d->csd.installMoveAction(ui->topWidget);
    d->csd.installResizeAction(this);

    if (tCsdGlobal::windowControlsEdge() == tCsdGlobal::Left) {
        ui->leftCsdLayout->addWidget(d->csd.csdBoxForWidget(this));
    } else {
        ui->rightCsdLayout->addWidget(d->csd.csdBoxForWidget(this));
    }

    ui->jobButtonLayout->addWidget(tJobManager::makeJobButton());

    this->resize(this->size());

    ui->centralwidget->layout()->removeWidget(ui->topWidget);
    ui->topWidget->raise();
    ui->topWidget->move(0, 0);

    //    ui->windowTabber->addButton(new tWindowTabberButton(QIcon(), tr("Welcome"), ui->stackedWidget, ui->welcomePage));
    ui->windowTabber->setShowNewTabButton(true);
    connect(ui->windowTabber, &tWindowTabber::newTabRequested, this, &MainWindow::newTab);

#ifdef Q_OS_MAC
    ui->menubar->addMenu(new tHelpMenu(this));
    ui->menuButton->setVisible(false);
#else
    ui->menubar->setVisible(false);
    QMenu* menu = new QMenu(this);

    tHelpMenu* helpMenu = new tHelpMenu(this);

    menu->addAction(ui->actionAdd_City);
    menu->addAction(ui->actionClose_Tab);
    menu->addSeparator();
    menu->addMenu(helpMenu);
    menu->addAction(ui->actionExit);

    ui->menuButton->setIcon(tApplication::applicationIcon());
    ui->menuButton->setIconSize(QSize(24, 24));
    ui->menuButton->setMenu(menu);
#endif

    ui->stackedWidget->setCurrentAnimation(tStackedWidget::SlideHorizontal);
    this->setWindowIcon(tApplication::applicationIcon());

    addTab({});

    for (auto location : d->settings.delimitedList("locations/locations")) {
        if (location.isEmpty()) continue;

        TroposphereLocation loc(location);
        this->addTab(loc);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionExit_triggered() {
    QApplication::exit();
}

void MainWindow::newTab() {
    auto jp = new AddLocationPopover();
    tPopover* popover = new tPopover(jp);
    popover->setPopoverWidth(-200);
    popover->setPopoverSide(tPopover::Trailing);
    connect(jp, &AddLocationPopover::done, popover, &tPopover::dismiss);
    connect(jp, &AddLocationPopover::locationSelected, this, [this](TroposphereLocation location) {
        this->addTab(location);
        this->saveLocations();
    });
    connect(popover, &tPopover::dismissed, popover, &tPopover::deleteLater);
    connect(popover, &tPopover::dismissed, jp, &AddLocationPopover::deleteLater);
    popover->show(this->window());
}

void MainWindow::addTab(TroposphereLocation location) {
    auto weatherPane = new WeatherPane(location);
    weatherPane->setTopPadding(ui->topWidget->sizeHint().height());
    d->weatherPanes.append(weatherPane);
    ui->stackedWidget->addWidget(weatherPane);

    if (location.locatedLocation) {
        ui->windowTabber->addButton(new tWindowTabberButton(QIcon::fromTheme("gps"), tr("My Location"), ui->stackedWidget, weatherPane));
    } else {
        ui->windowTabber->addButton(new tWindowTabberButton(QIcon(), location.name, ui->stackedWidget, weatherPane));
    }
}

void MainWindow::saveLocations() {
    d->settings.setDelimitedList("locations/locations", tRange(d->weatherPanes)
                                                            .filter([](WeatherPane* pane) {
                                                                return !pane->location().locatedLocation;
                                                            })
                                                            .map<QString>([](WeatherPane* pane) {
                                                                return pane->location().serialise();
                                                            })
                                                            .toList());
}

void MainWindow::on_actionClose_Tab_triggered() {
    auto pane = static_cast<WeatherPane*>(ui->stackedWidget->currentWidget());
    ui->stackedWidget->removeWidget(pane);
    d->weatherPanes.removeOne(pane);
    saveLocations();
}

void MainWindow::on_stackedWidget_currentChanged(int arg1) {
    auto pane = static_cast<WeatherPane*>(ui->stackedWidget->currentWidget());
    ui->actionClose_Tab->setEnabled(!pane->location().locatedLocation);
}

void MainWindow::on_actionAdd_City_triggered() {
    this->newTab();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    ui->topWidget->setFixedWidth(ui->centralwidget->width());
    ui->topWidget->setFixedHeight(ui->topWidget->sizeHint().height());

    for (auto pane : d->weatherPanes) {
        pane->setTopPadding(ui->topWidget->sizeHint().height());
    }
}

void MainWindow::on_actionSettings_triggered() {
    tSettingsWindow::openStaticSettingsWindow(this);
}
