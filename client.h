#ifndef CLIENT_H
#define CLIENT_H
#include <qtcpsocket.h>

class Client : public QObject
{
    Q_OBJECT
    public:
        explicit Client(QObject *parent = 0);

        void connectToHost();

    signals:

    public slots:
        void connected();
        void disconnected();
        void bytesWritten(qint64 bytes);
        void readyRead();

    private:
        QTcpSocket *socket;
};

#endif // CLIENT_H
