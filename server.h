#ifndef SERVER_H
#define SERVER_H
#include <qtcpserver.h>

class Server : public QObject
{
    Q_OBJECT
    public:
        explicit Server(QObject *parent = 0);

        void addClient(QTcpSocket *client);
        void deleteClient(int pos);
        bool init();

    signals:
        void on_client_connection();
    public slots:
        void connected();
        void disconnected();
        void bytesWritten(qint64 bytes);
        void readyRead();

    private:
        QTcpServer *server;
        QList<QTcpSocket> clients;
};

#endif // SERVER_H
