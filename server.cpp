#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    QHostAddress serverAddr("127.0.0.1");
    int port = 2323;
}
bool Server::init()
{
    server = new QTcpServer(this);
    if (!server->listen(serverAddr, port))
    {
        qDebug("Server creation error");
        server->close();
        return false;
    }
    else
    {
        connect(server, SIGNAL(newConnection()), this, SLOT(on_client_connection()));
        qDebug("Server is waiting for clients...");
        return true;
    }
}

Server::addClient(QTcpSocket *client)
{
    if (client != NULL)
    {
        clients.append(client);
    }
}
Server::deleteClient(int pos)
{
    if (!clients.isEmpty() && pos < clients.size() - 1)
    {
        clients.removeAt(pos);
    }
}
Server::on_client_connection()
{
    qDebug("Client connected");
    QTcpSocket *pClientSocket = server->nextPendingConnection();
    addClient(pClientSocket);
    //connect(pClientSocket, SIGNAL(disconnected()),pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()), server, SLOT());
}
