#include "game.h"
#include "ui_game.h"
#include <item.h>
#include <qmediaplayer.h>
#include <mainwindow.h>
#include <qsqlquery.h>
#include <qsqldatabase.h>
#include <qdir.h>

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
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(qApp->applicationDirPath() + QDir::separator() + "ApplesInfo.sqlite");
    db.open();
    if (db.isOpen())
    {
        qDebug("db has been opened");
        QSqlQuery query;
        query.exec("create table apples "
                  "(id integer primary key, "
                  "coordinates varchar(3), "
                  "count integer)");
    }
    connect(ui->tableWidget, SIGNAL(appleAdded(int)), this, SLOT(on_apple_added(int)));
    //connect(ui->apple, SIGNAL(appleMoving()), ui->tableWidget, SLOT(onAppleMoving()));
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
        int amount = static_cast<Item*>(ui->tableWidget->item(row, column))->getAmount();
        qDebug("Apples amount: %d", amount);
        ui->appleCount->display(amount);
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

void Game::on_apple_added(int count)
{
    qDebug("Apple added. Amount %d", count);
}
