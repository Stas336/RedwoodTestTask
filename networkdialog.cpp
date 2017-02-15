#include "networkdialog.h"
#include "ui_networkdialog.h"
#include <game.h>

NetworkDialog::NetworkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkDialog)
{
    ui->setupUi(this);
}

NetworkDialog::~NetworkDialog()
{
    delete ui;
}

void NetworkDialog::on_clientButton_clicked()
{
    Game *game = new Game(false);
    game->setWindowTitle("Game (Client)");
    game->show();
    this->close();
}

void NetworkDialog::on_serverButton_clicked()
{
    Game *game = new Game(true);
    game->setWindowTitle("Game (Server)");
    game->show();
    this->close();
}
