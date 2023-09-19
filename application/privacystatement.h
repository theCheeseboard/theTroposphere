#ifndef PRIVACYSTATEMENT_H
#define PRIVACYSTATEMENT_H

#include <QCoroTask>
#include <QWidget>

namespace Ui {
    class PrivacyStatement;
}

class PrivacyStatement : public QWidget {
        Q_OBJECT

    public:
        explicit PrivacyStatement(QWidget* parent = nullptr);
        ~PrivacyStatement();

        static QCoro::Task<> showPrivacyStatement(QWidget* parent);

    signals:
        void done();

    private slots:
        void on_textBrowser_anchorClicked(const QUrl& arg1);

        void on_okButton_clicked();

        void on_noButton_clicked();

    private:
        Ui::PrivacyStatement* ui;

        // QObject interface
    public:
        bool eventFilter(QObject* watched, QEvent* event);
};

#endif // PRIVACYSTATEMENT_H
