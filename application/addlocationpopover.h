#ifndef ADDLOCATIONPOPOVER_H
#define ADDLOCATIONPOPOVER_H

#include <QWidget>
#include <QCoroTask>
#include "tropospherelocation.h"

namespace Ui {
class AddLocationPopover;
}

class QListWidgetItem;
struct AddLocationPopoverPrivate;
class AddLocationPopover : public QWidget
{
    Q_OBJECT

public:
    explicit AddLocationPopover(QWidget *parent = nullptr);
    ~AddLocationPopover();

signals:
    void done();
    void locationSelected(TroposphereLocation location);

private slots:
    void on_titleLabel_backButtonClicked();

    void on_queryEdit_textChanged(const QString &arg1);

    void on_listWidget_itemActivated(QListWidgetItem *item);

private:
    Ui::AddLocationPopover *ui;
    AddLocationPopoverPrivate* d;

    QCoro::Task<> performSearch();
};

#endif // ADDLOCATIONPOPOVER_H
