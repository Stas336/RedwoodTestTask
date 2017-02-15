#ifndef GAME_H
#define GAME_H

#include <dragwidget.h>
#include <qsqldatabase.h>
#include <qpoint.h>
#include <database.h>

namespace Ui {
class Game;
}

class Game : public DragWidget
{
    Q_OBJECT

public:
    explicit Game(DragWidget *parent = 0);
    ~Game();

private slots:
    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_returnButton_clicked();

    void on_apple_added(int amount, QPoint *pos);

    void on_apple_deleted(int amount, QPoint *pos);

    void on_apple_moving();

    void closeEvent(QCloseEvent *event);

private:
    Ui::Game *ui;
    Database db;
};

#endif // GAME_H
