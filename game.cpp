#include "game.h"
#include "ui_game.h"
#include <item.h>
#include <qmediaplayer.h>

Game::Game(DragWidget *parent) :
DragWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    for (int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        ui->tableWidget->setRowHeight(i, 80);
    }
}

Game::~Game()
{
    delete ui;
}

void Game::on_tableWidget_cellClicked(int row, int column)
{
    qDebug("You have selected Column: %d, Row: %d", column, row);
    if (ui->tableWidget->item(row, column) != NULL)
    {
        int amount = static_cast<Item*>(ui->tableWidget->item(row, column))->getAmount();
        qDebug("Apples amount: %d", amount);
    }
}
void Game::on_tableWidget_cellDoubleClicked(int row, int column)
{
    qDebug("You have selected Column: %d, Row: %d", column, row);
    if (ui->tableWidget->item(row, column) != NULL)
    {
        int amount = static_cast<Item*>(ui->tableWidget->item(row, column))->getAmount();
        Item *item = new Item(this);
        QMediaPlayer *player = new QMediaPlayer();
        player->setMedia(QUrl("qrc:/Downloads/apple_bite.mp3"));
        player->setVolume(70);
        amount--;
        if (amount > 0)
        {
            qDebug("Apples amount: %d", amount);
            item->setAmount(amount);
            item->setData(Qt::DecorationRole, QPixmap(":/Downloads/rsz_apple.jpg"));
            player->play();
            ui->tableWidget->setItem(row, column, item);
        }
        else if (amount == 0)
        {
            qDebug("Apples amount: %d", amount);
            item->setAmount(amount);
            player->play();
            ui->tableWidget->setItem(row, column, item);
        }
    }
}
