#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H
#include <qframe.h>
#include <QMouseEvent>
#include <QMimeData>
#include <qdrag.h>
#include <qpainter.h>

class DragWidget : public QFrame
{
public:
    DragWidget(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // DRAGWIDGET_H
