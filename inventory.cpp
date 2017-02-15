#include <QtGui>
#include "inventory.h"
#include <item.h>
#include <QTableWidgetItem>
#include <game.h>
#include <qsqlquery.h>

Inventory::Inventory(QWidget *parent) : QTableWidget(parent) {
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setShowGrid(true);
    setWordWrap(true);
    setAcceptDrops(true);
    prevApples = 0;
    isDropStarted = false;
    isAppleMoving = false;
    startCoordinates = new QPoint();
}

void Inventory::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void Inventory::dragMoveEvent(QDragMoveEvent *event) {
    if (!isDropStarted)
    {
        isDropStarted = true;
        qDebug("Start Column - %d, Start Row - %d", this->indexAt(event->pos()).column(), this->indexAt(event->pos()).row());
        setStartCoordinates(this->indexAt(event->pos()).row(), this->indexAt(event->pos()).column());
        if (this->item(getStartCoordinates()->x(), getStartCoordinates()->y()) != NULL)
        {
            prevApples = static_cast<Item*>(this->item(getStartCoordinates()->x(), getStartCoordinates()->y()))->getAmount();
        }
    }
    event->acceptProposedAction();
}

void Inventory::dropEvent(QDropEvent *event) {
    event->acceptProposedAction();
    qDebug("End Column - %d, End Row - %d", this->indexAt(event->pos()).column(), this->indexAt(event->pos()).row());
    QPoint *coordinates = new QPoint(this->indexAt(event->pos()).row(),this->indexAt(event->pos()).column());
    emit dropped(coordinates, event->mimeData());
}

void Inventory::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

void Inventory::dropped(QPoint *pos, const QMimeData *mimeData)
{
    Item *item = new Item(this);
    if (!isAppleMoving)
    {
        if (this->item(pos->x(), pos->y()) != NULL)
        {
            if (this->item(pos->x(), pos->y()) != this->item(getStartCoordinates()->x(), getStartCoordinates()->y()))
            {
                int amount = static_cast<Item*>(this->item(pos->x(), pos->y()))->getAmount();
                qDebug("Start amount: %d, Final amount: %d", prevApples, amount);
                item->setAmount(amount + prevApples);
                emit appleAdded(amount + prevApples, pos);
                prevApples = 0;
                Item *item1 = static_cast<Item*>(this->item(getStartCoordinates()->x(), getStartCoordinates()->y()));
                item1->setAmount(0);
                this->setItem(getStartCoordinates()->x(), getStartCoordinates()->y(), item1);
                emit appleDeleted(amount, getStartCoordinates());
                //delete this->item(getStartCoordinates()->x(), getStartCoordinates()->y());
                //removeCellWidget(getStartCoordinates()->x(), getStartCoordinates()->y());
            }
        }
        else
        {
            int amount = static_cast<Item*>(this->item(getStartCoordinates()->x(), getStartCoordinates()->y()))->getAmount();
            item->setAmount(amount);
            emit appleAdded(amount, pos);
            Item *item1 = static_cast<Item*>(this->item(getStartCoordinates()->x(), getStartCoordinates()->y()));
            item1->setAmount(0);
            this->setItem(getStartCoordinates()->x(), getStartCoordinates()->y(), item1);
            //removeCellWidget(getStartCoordinates()->x(), getStartCoordinates()->y());
            //delete this->item(getStartCoordinates()->x(), getStartCoordinates()->y());
            emit appleDeleted(amount, getStartCoordinates());
        }
    }
    else
    {
        if (this->item(pos->x(), pos->y()) != NULL)
        {
            int amount = static_cast<Item*>(this->item(pos->x(), pos->y()))->getAmount();
            item->setAmount(amount + defaultAppleAmount);
            isAppleMoving = false;
            emit appleAdded(amount + defaultAppleAmount, pos);
        }
        else
        {
            item->setAmount(defaultAppleAmount);
            isAppleMoving = false;
            emit appleAdded(defaultAppleAmount, pos);
        }
    }
    item->setData(Qt::DecorationRole, QPixmap(":/resources/rsz_apple.jpg"));
    this->setItem(pos->x(), pos->y(), item);
    setDropStarted(false);
}

int Inventory::getPrevApples()
{
    return this->prevApples;
}
void Inventory::setPrevApples(int newValue)
{
    this->prevApples = newValue;
}
QPoint* Inventory::getStartCoordinates()
{
    return this->startCoordinates;
}
void Inventory::setStartCoordinates(int x, int y)
{
    startCoordinates->setX(x);
    startCoordinates->setY(y);
}
bool Inventory::DropStarted()
{
    return this->isDropStarted;
}
void Inventory::setDropStarted(bool value)
{
    this->isDropStarted = value;
}
void Inventory::setIsAppleMoving(bool boolean)
{
    isAppleMoving = boolean;
}
void Inventory::setDefaultAppleAmount(int amount)
{
    defaultAppleAmount = amount;
}
