#include "game.h"
#include "ui_game.h"

Game::Game(DragWidget *parent) :
DragWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->tableWidget->resizeColumnsToContents();
    //ui->tableWidget->resizeRowsToContents();
    for (int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        ui->tableWidget->setRowHeight(i, 80);
        //ui->tableWidget->setColumnWidth(i, 80);
    }
}

Game::~Game()
{
    delete ui;
}
