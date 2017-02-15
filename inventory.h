#ifndef INVENTORY_H
#define INVENTORY_H

#include "qtablewidget.h"

class Inventory : public QTableWidget
{
    Q_OBJECT
public:
        Inventory(QWidget *parent = 0);

    public slots:
        void dropped(QPoint *pos, const QMimeData *mimeData = 0);
    signals:
        void appleAdded(int count, QPoint *pos);
        void appleDeleted(int count, QPoint *pos);
    protected:
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void dragLeaveEvent(QDragLeaveEvent *event);
        void dropEvent(QDropEvent *event);
public:
        int getPrevApples();
        void setPrevApples(int newValue);
        bool DropStarted();
        void setDropStarted(bool value);
        QPoint* getStartCoordinates();
        void setStartCoordinates(int x, int y);
        void setIsAppleMoving(bool boolean);
        void setDefaultAppleAmount(int amount);
    private:
        int prevApples;
        bool isDropStarted;
        bool isAppleMoving;
        int defaultAppleAmount;
        QPoint *startCoordinates;
};

#endif // INVENTORY_H
