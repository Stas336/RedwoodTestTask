#ifndef GAME_H
#define GAME_H

#include <dragwidget.h>
#include <qsqldatabase.h>

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

    void on_apple_added(int count);

private:
    Ui::Game *ui;
};

#endif // GAME_H
