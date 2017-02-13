#ifndef GAME_H
#define GAME_H

#include <dragwidget.h>

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

private:
    Ui::Game *ui;
};

#endif // GAME_H
