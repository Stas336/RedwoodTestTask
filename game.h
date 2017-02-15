#ifndef GAME_H
#define GAME_H

#include <dragwidget.h>
#include <qsqldatabase.h>
#include <qpoint.h>
#include <database.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qhostaddress.h>

namespace Ui {
class Game;
}

class Game : public DragWidget
{
    Q_OBJECT

public:
    explicit Game(bool isServer, DragWidget *parent = 0);
    void send(QTcpSocket* pSocket, const QString& str);
    void synchronize();
    ~Game();

private slots:
    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_returnButton_clicked();

    void on_apple_added(int amount, QPoint *pos);

    void on_apple_deleted(int amount, QPoint *pos);

    void on_apple_moving();

    void closeEvent(QCloseEvent *event);

    void on_client_connection();

    void on_server_connection();

    void read();

private:
    Ui::Game *ui;
    Database db;
    QTcpServer *server;
    QTcpSocket *client;
    bool isServer;
};

#endif // GAME_H
