#include "dragwidget.h"
#include <item.h>
#include <qlabel.h>
#include <inventory.h>

DragWidget::DragWidget(QWidget *parent)
: QFrame(parent)
{
setMinimumSize(200, 200);
setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
setAcceptDrops(true);
/*Item *apple = new Item(this);
apple->move(420,180);
apple->setObjectName(QString("Apple"));
apple->setAmount(1);
apple->setName("Apple");
apple->show();
apple->setPixmap(QPixmap(":/Downloads/rsz_apple.jpg"));
apple->setAttribute(Qt::WA_DeleteOnClose);*/
}

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
if (event->source() == this) {
event->setDropAction(Qt::MoveAction);
event->accept();
} else {
event->acceptProposedAction();
}
} else {
event->ignore();
}
}

void DragWidget::dragMoveEvent(QDragMoveEvent *event)
{
emit dropStarted();
if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
if (event->source() == this) {
event->setDropAction(Qt::MoveAction);
event->accept();
} else {
event->acceptProposedAction();
}
} else {
event->ignore();
}
}

void DragWidget::dropEvent(QDropEvent *event)
{
if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
QDataStream dataStream(&itemData, QIODevice::ReadOnly);

QPixmap pixmap;
QPoint offset;
dataStream >> pixmap >> offset;

QLabel *newIcon = new QLabel(this);
newIcon->setPixmap(pixmap);
newIcon->move(event->pos() - offset);
newIcon->show();
newIcon->setAttribute(Qt::WA_DeleteOnClose);

if (event->source() == this) {
event->setDropAction(Qt::MoveAction);
event->accept();
} else {
event->acceptProposedAction();
}
} else {
event->ignore();
}
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
if (!child)
return;

qDebug("child name %s", child->objectName().toLatin1().data());

QPixmap pixmap = *child->pixmap();

QByteArray itemData;
QDataStream dataStream(&itemData, QIODevice::WriteOnly);
dataStream << pixmap << QPoint(event->pos() - child->pos());
QMimeData *mimeData = new QMimeData;
mimeData->setData("application/x-dnditemdata", itemData);
QDrag *drag = new QDrag(this);
drag->setMimeData(mimeData);
drag->setPixmap(pixmap);
drag->setHotSpot(event->pos() - child->pos());
QPixmap tempPixmap = pixmap;
QPainter painter;
painter.begin(&tempPixmap);
painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
painter.end();

child->setPixmap(tempPixmap);
if (this->isCopied)
{
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
    child->close();
    else {
    child->show();
    child->setPixmap(pixmap);
    }
}
else
{
    if (drag->exec(/*Qt::CopyAction | Qt::MoveAction, */Qt::CopyAction) == Qt::MoveAction)
    {
        this->isCopied = true;
        child->close();
    }
    else {
    child->show();
    child->setPixmap(pixmap);
    }
}
}
