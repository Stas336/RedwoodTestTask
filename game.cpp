#include "game.h"
#include "ui_game.h"
#include <item.h>
#include <qmediaplayer.h>
#include <mainwindow.h>
#include <qsqlquery.h>
#include <qsqldatabase.h>
#include <qdir.h>
#include <database.h>

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
    db;
    db.connect();
    db.init();
    ui->tableWidget->setDefaultAppleAmount(db.getDefaultAppleAmount());
    connect(ui->tableWidget, SIGNAL(appleAdded(int, QPoint*)), this, SLOT(on_apple_added(int, QPoint*)));
    connect(ui->tableWidget, SIGNAL(appleDeleted(int,QPoint*)), this, SLOT(on_apple_deleted(int,QPoint*)));
    connect(this, SIGNAL(dropStarted()), this, SLOT(on_apple_moving()));
}

Game::~Game()
{
    delete ui;
}

void Game::on_tableWidget_cellClicked(int row, int column)
{
    qDebug("You have selected Column: %d, Row: %d", column, row);
    ui->appleCount->display(0);
    if (ui->tableWidget->item(row, column) != NULL)
    {
        //int amount = static_cast<Item*>(ui->tableWidget->item(row, column))->getAmount();
        //qDebug("Apples amount: %d", amount);
        //ui->appleCount->display(amount);
        QPoint *pos = new QPoint(row, column);
        ui->appleCount->display(db.showAmount(pos));
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
        player->setMedia(QUrl("qrc:/resources/apple_bite.mp3"));
        player->setVolume(70);
        amount--;
        QPoint *pos = new QPoint(row, column);
        db.removeApple(1, pos);
        if (amount > 0)
        {
            qDebug("Apples amount: %d", amount);
            ui->appleCount->display(amount);
            item->setAmount(amount);
            item->setData(Qt::DecorationRole, QPixmap(":/resources/rsz_apple.jpg"));
            player->play();
            ui->tableWidget->setItem(row, column, item);
        }
        else if (amount == 0)
        {
            qDebug("Apples amount DELETE: %d", amount);
            ui->appleCount->display(amount);
            item->setAmount(amount);
            player->play();
            delete ui->tableWidget->item(row, column);
        }
    }
}

void Game::on_returnButton_clicked()
{
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}

void Game::on_apple_added(int amount, QPoint *pos)
{
    qDebug("Apple added. Amount %d", amount);
    db.addApple(amount, pos);
}
void Game::on_apple_deleted(int amount, QPoint *pos)
{
    qDebug("Apple deleted. Amount %d", amount);
    db.removeApple(amount, pos);
}

void Game::on_apple_moving()
{
    qDebug("apple moving");
    ui->tableWidget->setIsAppleMoving(true);
}

void Game::closeEvent(QCloseEvent *event)
{
    db.close();
}
