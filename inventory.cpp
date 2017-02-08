#include <QtGui>
#include "inventory.h"
#include <item.h>
#include <QTableWidgetItem>

Inventory::Inventory(QWidget *parent) : QTableWidget(parent) {
    //set widget default properties:
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setEditTriggers(QAbstractItemView::NoEditTriggers);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDragDropMode(QAbstractItemView::InternalMove);
    //setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setShowGrid(true);
    setWordWrap(true);
    setAcceptDrops(true);
    prevApples = 0;
    isDropStarted = false;
    startCoordinates = new QPoint();
}

void Inventory::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
    emit changed(event->mimeData());
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

void Inventory::clear() {
    emit changed();
}

void Inventory::dropped(QPoint *pos, const QMimeData *mimeData)
{
    Item *item = new Item(this);
    if (this->item(pos->x(), pos->y()) != NULL)
    {
        if (this->item(pos->x(), pos->y()) != this->item(getStartCoordinates()->x(), getStartCoordinates()->y()))
        {
            int amount = static_cast<Item*>(this->item(pos->x(), pos->y()))->getAmount();
            qDebug("Start amount: %d, Final amount: %d", prevApples, amount);
            item->setAmount(amount + prevApples);
            prevApples = 0;
        }
        else
        {
            int amount = static_cast<Item*>(this->item(pos->x(), pos->y()))->getAmount();
            item->setAmount(amount + 1);
            prevApples = 0;
        }
    }
    else
    {
        item->setAmount(1);
    }
    //item->setData(Qt::DecorationRole, QPixmap::fromImage(qvariant_cast<QImage>(mimeData->imageData())));
    item->setData(Qt::DecorationRole, QPixmap(":/Downloads/rsz_apple.jpg"));
    this->setItem(pos->x(), pos->y(), item);
    setDropStarted(false);
}

void Inventory::changed(const QMimeData *mimeData)
{

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
