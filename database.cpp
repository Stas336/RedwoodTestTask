#include "database.h"
#include <qapplication.h>
#include <qdir.h>
#include <qdebug.h>

Database::Database()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + QDir::separator() + "applesInfo.sqlite");
}

bool Database::connect()
{
    database.open();
    if (database.isOpen())
    {
        qDebug("Connection to database has been established successfully");
        return true;
    }
    else
    {
        qDebug("Error while connecting to database");
        return false;
    }
}

bool Database::init()
{
    QSqlQuery query;
    if (query.exec("CREATE TABLE inventory(id INTEGER PRIMARY KEY AUTOINCREMENT, posx INTEGER NOT NULL, posy INTEGER NOT NULL, amount INTEGER NOT NULL)"))
    {
        qDebug("table inventory has been successfully created");
    }
    if (query.exec("CREATE TABLE item(id INTEGER PRIMARY KEY AUTOINCREMENT, amount INTEGER NOT NULL)"))
    {
        qDebug("table item has been successfully created");
        query.prepare("INSERT INTO item(amount) VALUES(?)");
        query.addBindValue(1);
        if (query.exec())
        {
            return true;
        }
    }
    return false;
}

bool Database::close()
{
    if (database.isOpen())
    {
        QSqlQuery query;
        if (query.exec("DROP TABLE inventory"))
        {
            qDebug("Table inventory has been dropped");
        }
        if (query.exec("DROP TABLE item"))
        {
            qDebug("Table item has been dropped");
        }
        qDebug("Closing database connection...");
        database.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool Database::addApple(int amount, QPoint *pos)
{
    if (database.isOpen())
    {
        QSqlQuery query;
        if (showAmount(pos) == -1)
        {
            query.prepare("INSERT INTO inventory(posx, posy, amount) VALUES(?,?,?)");
            query.addBindValue(pos->x());
            query.addBindValue(pos->y());
            query.addBindValue(amount);
            if (query.exec())
            {
                qDebug("%d apple added into db. Amount: %d. POSX: %d. POSY: %d", amount, showAmount(pos), pos->x(), pos->y());
            }
        }
        else
        {
            query.prepare("UPDATE inventory SET amount=? WHERE posx=? AND posy=?");
            query.addBindValue(amount);
            query.addBindValue(pos->x());
            query.addBindValue(pos->y());
            if (query.exec())
            {
                qDebug("Apple updated in db. Amount %d", amount);
            }
        }
        return true;
    }
    return false;
}
bool Database::removeApple(int amount, QPoint *pos)
{
    if (database.isOpen())
    {
        QSqlQuery query;
        query.prepare("SELECT amount FROM inventory WHERE posx=? AND posy=?");
        query.addBindValue(pos->x());
        query.addBindValue(pos->y());
        query.exec();
        while (query.next())
        {
            int amount1 = query.value(0).toInt();
            qDebug("Apples from db. Amount: %d. POSX: %d. POSY: %d", amount1, pos->x(), pos->y());
            query.prepare("UPDATE inventory SET amount=? WHERE posx=? AND posy=?");
            query.addBindValue(amount1 - amount);
            query.addBindValue(pos->x());
            query.addBindValue(pos->y());
            if (query.exec())
            {
                qDebug("Apple updated in db. Amount was %d and now %d. POSX: %d. POSY: %d", amount1, amount1-amount, pos->x(), pos->y());
            }
            return true;
        }
        return false;
    }
    return false;
}
int Database::showAmount(QPoint *pos)
{
    if (database.isOpen())
    {
        QSqlQuery query;
        query.prepare("SELECT amount FROM inventory WHERE posx=? AND posy=?");
        query.addBindValue(pos->x());
        query.addBindValue(pos->y());
        query.exec();
        while (query.next())
        {
            int amount = query.value(0).toInt();
            return amount;
        }
        return -1;
    }
    return -1;
}
int Database::getDefaultAppleAmount()
{
    QSqlQuery query;
    query.prepare("SELECT amount FROM item WHERE id=1");
    query.exec();
    while (query.next())
    {
        int amount = query.value(0).toInt();
        return amount;
    }
    return -1;
}
