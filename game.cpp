#include "game.h"
#include "ui_game.h"
#include <item.h>
#include <qmediaplayer.h>
#include <mainwindow.h>
#include <qsqlquery.h>
#include <qsqldatabase.h>
#include <qdir.h>
#include <database.h>
#include <qhostaddress.h>

Game::Game(bool isServer, DragWidget *parent) :
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
    db.connect();
    db.init();
    ui->tableWidget->setDefaultAppleAmount(db.getDefaultAppleAmount());
    connect(ui->tableWidget, SIGNAL(appleAdded(int, QPoint*)), this, SLOT(on_apple_added(int, QPoint*)));
    connect(ui->tableWidget, SIGNAL(appleDeleted(int,QPoint*)), this, SLOT(on_apple_deleted(int,QPoint*)));
    connect(this, SIGNAL(dropStarted()), this, SLOT(on_apple_moving()));
    this->isServer = isServer;
    QHostAddress serverAddr("127.0.0.1");
    int port = 2323;
    if (isServer)
    {
        server = new QTcpServer(this);
        client = new QTcpSocket(this);
        if (!server->listen(serverAddr, port))
        {
            qDebug("Server creation error");
            server->close();
        }
        else
        {
            connect(server, SIGNAL(newConnection()), this, SLOT(on_client_connection()));
            qDebug("Server is waiting for clients...");
        }
    }
    else
    {
        client = new QTcpSocket(this);
        client->connectToHost(serverAddr, port);
        qDebug("Client is connecting to server...");
        connect(client, SIGNAL(connected()),this, SLOT(on_server_connection()));
    }
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
        QPoint *pos = new QPoint(row, column);
        emit on_apple_deleted(amount, pos);
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
            ui->tableWidget->setItem(row, column, item);
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
    if (isServer && client->state() == QTcpSocket::ConnectedState)
    {
        qDebug("Synchronization from server side ADD");
        synchronize();
    }
    else if (!isServer && client->state() == QTcpSocket::ConnectedState)
    {
        qDebug("Synchronization from client side ADD");
        synchronize();
    }
}
void Game::on_apple_deleted(int amount, QPoint *pos)
{
    qDebug("Apple deleted. Amount %d", amount);
    db.removeApple(amount, pos);
    if (isServer && client->state() == QTcpSocket::ConnectedState)
    {
        qDebug("Synchronization from server side (DELETE)");
        synchronize();
    }
    else if (!isServer && client->state() == QTcpSocket::ConnectedState)
    {
        qDebug("Synchronization from client side (DELETE)");
        synchronize();
    }
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

void Game::on_client_connection()
{
    qDebug("Client connected");
    client = server->nextPendingConnection();
    //connect(pClientSocket, SIGNAL(disconnected()),pClientSocket, SLOT(deleteLater()));
    connect(client, SIGNAL(readyRead()), this, SLOT(read()));
    //send(client, "Hello Client");
    synchronize();
}

void Game::on_server_connection()
{
    qDebug("Server connected");
    connect(client, SIGNAL(readyRead()), this, SLOT(read()));
    //send(client, "Hello Server");
}
void Game::send(QTcpSocket* pSocket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint64(0) << str;
    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));
    pSocket->write(arrBlock);
}
void Game::read()
{
    quint64 m_nNextBlockSize = 0;
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
        for (;;) {
            if (!m_nNextBlockSize) {
                if (pClientSocket->bytesAvailable() < sizeof(quint64)) {
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
                break;
            }
            QString str;
            in >> str;
            QString strMessage;
            /*if (isServer)
            {
                strMessage = "Client has sended - " + str;
            }
            else
            {
                strMessage = "Server has sended - " + str;
            }*/
            strMessage = str;
            qDebug() << strMessage;
            m_nNextBlockSize = 0;
            QStringList list = strMessage.split("");
            QString temp = list.at(1);
            int posx = temp.toInt();
            temp = list.at(2);
            int posy = temp.toInt();
            temp = list.at(3)+list.at(4);
            int amount = temp.toInt();
            qDebug("POSX: %d, POSY: %d, Amount: %d", posx, posy, amount);
            if (amount > 0)
            {
                QPoint *pos = new QPoint(posx, posy);
                db.addApple(amount, pos);
                Item *item = new Item(this);
                item->setAmount(amount);
                item->setData(Qt::DecorationRole, QPixmap(":/resources/rsz_apple.jpg"));
                ui->tableWidget->setItem(pos->x(), pos->y(), item);
            }
            else if (amount == 0)
            {
                QPoint *pos = new QPoint(posx, posy);
                db.addApple(amount, pos);
                Item *item = new Item(this);
                item->setAmount(amount);
                ui->tableWidget->setItem(pos->x(), pos->y(), item);
            }
        }
}

void Game::synchronize()
{
    QPoint *pos = new QPoint();
    int amount = 0;
    for (int i=0;i<3;i++)
    {
        for (int j=0;j<3;j++)
        {
            pos->setX(i);
            pos->setY(j);
            amount = db.showAmount(pos);
            QString str = QString("%1%2%3").arg(i).arg(j).arg(amount);
            if (isServer)
            {
                send(client, str);
            }
            else
            {
                send(client, str);
            }
        }
    }
}
