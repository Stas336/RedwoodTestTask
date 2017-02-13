#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <game.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{
    exit(0);
}

void MainWindow::on_playButton_clicked()
{
    Game *game = new Game();
    game->show();
    this->close();
}
