#ifndef INVENTORY_H
#define INVENTORY_H

#include "qtablewidget.h"

class Inventory : public QTableWidget
{
    Q_OBJECT
public:
        Inventory(QWidget *parent = 0);

    public slots:
        void clear();
        void changed(const QMimeData *mimeData = 0);
        void dropped(QPoint *pos, const QMimeData *mimeData = 0);
        void onAppleMoving();
    signals:
        void appleAdded(int count);
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
    private:
        int prevApples;
        bool isDropStarted;
        bool isAppleMoving;
        QPoint *startCoordinates;
};

#endif // INVENTORY_H
