#include "privacystatement.h"
#include "ui_privacystatement.h"

#include <QCoroCore>
#include <QDesktopServices>
#include <QResizeEvent>
#include <tapplication.h>
#include <tscrim.h>
#include <tsettings.h>

PrivacyStatement::PrivacyStatement(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::PrivacyStatement) {
    ui->setupUi(this);

    QPalette pal;
    pal.setColor(QPalette::Window, Qt::transparent);
    pal.setColor(QPalette::Base, Qt::transparent);
    ui->textBrowser->setPalette(pal);

    ui->textBrowser->setMarkdown(tr(
        R"(
Welcome to theTroposphere. In order for theTroposphere to work correctly, data must be sent to servers hosted by Victor Tran and the Norwegian Meteorological Institute.

Data sent to Victor Tran includes:
- Data regarding your current location (if accepted) in order to figure out what city you are currently in

Data sent to Victor Tran will be handled in accordance with the [Privacy Policy for Victor Tran services](%1).

Data sent to the Norwegian Meteorological Institute includes:
- Coordinates of cities that you want to get the weather for
- Your current location (if accepted) in order to get the weather for your current location

Data sent to the Norwegian Meteorological Institute will be handled in accordance with the [Privacy Policy for the Norwegian Meteorological Institute](%2).
    )")
                                     .arg("https://vicr123.com/privacy", "https://www.met.no/en/About-us/privacy"));
}

PrivacyStatement::~PrivacyStatement() {
    delete ui;
}

QCoro::Task<> PrivacyStatement::showPrivacyStatement(QWidget* parent) {
    auto scrimParent = tScrim::scrimWidget(parent);
    auto scrim = tScrim::scrimForWidget(parent);
    scrim->show();

    PrivacyStatement ps;
    ps.setParent(scrimParent);
    parent->installEventFilter(&ps);

    QResizeEvent e({}, {});
    ps.eventFilter(parent, &e);
    ps.show();

    co_await qCoro(&ps, &PrivacyStatement::done);

    scrim->hide();

    co_return;
}

void PrivacyStatement::on_textBrowser_anchorClicked(const QUrl& arg1) {
    QDesktopServices::openUrl(arg1);
}

bool PrivacyStatement::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::Resize) {
        auto parentWidget = static_cast<QWidget*>(watched);
        QRect parentGeom(QPoint(0, 0), parentWidget->size());

        QRect thisRect(QPoint(0, 0), QSize(qMin(parentGeom.width() - 20, 640), qMin(parentGeom.height() - 20, 480)));
        thisRect.moveCenter(parentGeom.center());
        this->setGeometry(thisRect);
    }
    return false;
}

void PrivacyStatement::on_okButton_clicked() {
    emit done();
}

void PrivacyStatement::on_noButton_clicked() {
    {
        tSettings settings;
        settings.setValue("privacy/accepted", true);
        settings.sync();
    }
    tApplication::exit();
}
