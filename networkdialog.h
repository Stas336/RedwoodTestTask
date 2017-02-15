#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QDialog>

namespace Ui {
class NetworkDialog;
}

class NetworkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkDialog(QWidget *parent = 0);
    ~NetworkDialog();

private slots:
    void on_clientButton_clicked();

    void on_serverButton_clicked();

private:
    Ui::NetworkDialog *ui;
};

#endif // NETWORKDIALOG_H
