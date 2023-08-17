#include "addlocationpopover.h"
#include "ui_addlocationpopover.h"

#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoroNetwork>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <tlogger.h>

struct AddLocationPopoverPrivate {
    QNetworkAccessManager mgr;
    QTimer* timer;
    quint64 nonce = 0;
};

AddLocationPopover::AddLocationPopover(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddLocationPopover)
{
    ui->setupUi(this);
    d = new AddLocationPopoverPrivate();

    d->timer = new QTimer(this);
    d->timer->setInterval(500);
    d->timer->setSingleShot(true);
    connect(d->timer, &QTimer::timeout, this, &AddLocationPopover::performSearch);

    ui->stackedWidget->setCurrentAnimation(tStackedWidget::Fade);
    ui->stackedWidget->setCurrentWidget(ui->resultsPage, false);
}

AddLocationPopover::~AddLocationPopover()
{
    delete ui;
    delete d;
}

void AddLocationPopover::on_titleLabel_backButtonClicked()
{
    emit done();
}

QCoro::Task<> AddLocationPopover::performSearch()
{
    auto nonce = d->nonce;
    ui->listWidget->clear();

    if (ui->queryEdit->text().length() < 3) co_return;

    QJsonObject payload;
    payload.insert("query", ui->queryEdit->text());

    auto response = co_await d->mgr.post(QNetworkRequest(QStringLiteral("https://api.thetroposphere.vicr123.com/api/locations/search")), QJsonDocument(payload).toJson(QJsonDocument::Compact));
    if (response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
        // Error!
        tWarn("AddLocationPopover") << "Search request failed";
        tWarn("AddLocationPopover") << "Search request returned " << response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        ui->stackedWidget->setCurrentWidget(ui->resultsPage);
        co_return;
    }

    QJsonParseError parseError;
    QJsonArray responseNames = QJsonDocument::fromJson(response->readAll(), &parseError).array();
    if (parseError.error != QJsonParseError::NoError) {
        // Error!
        tWarn("AddLocationPopover") << "Search request failed";
        tWarn("AddLocationPopover") << "JSON parse of response failed";
        ui->stackedWidget->setCurrentWidget(ui->resultsPage);
        co_return;
    }

    for (auto el : responseNames) {
        auto name = el.toObject();

        TroposphereLocation loc(name.value("name").toString(), name.value("admin1").toString(), name.value("countryCode").toString(), name.value("lat").toDouble(), name.value("lng").toDouble());

        auto item = new QListWidgetItem();
        item->setText(QStringLiteral("%1, %2, %3").arg(loc.name, loc.admin1, loc.country));
        item->setData(Qt::UserRole, QVariant::fromValue(loc));
        ui->listWidget->addItem(item);
    }

    if (d->nonce != nonce) co_return;
    ui->stackedWidget->setCurrentWidget(ui->resultsPage);
}


void AddLocationPopover::on_queryEdit_textChanged(const QString &arg1)
{
    if (ui->queryEdit->text().length() < 3) {
        ui->stackedWidget->setCurrentWidget(ui->resultsPage);
    } else {
        ui->stackedWidget->setCurrentWidget(ui->loadingPage);
        d->timer->stop();
        d->timer->start();
        d->nonce++;
    }
}


void AddLocationPopover::on_listWidget_itemActivated(QListWidgetItem *item)
{
    emit locationSelected(item->data(Qt::UserRole).value<TroposphereLocation>());
    emit done();
}

