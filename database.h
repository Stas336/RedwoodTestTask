#ifndef DATABASE_H
#define DATABASE_H
#include <qsqldatabase.h>
#include <qpoint.h>
#include <qsqlquery.h>

class Database
{
public:
    Database();
    bool connect();
    bool close();
    bool addApple(int amount, QPoint *pos);
    bool removeApple(int amount, QPoint *pos);
    bool init();
    int showAmount(QPoint *pos);
    int getDefaultAppleAmount();
private:
    QSqlDatabase database;
};

#endif // DATABASE_H
