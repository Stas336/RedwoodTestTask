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

private:
    Ui::Game *ui;
};

#endif // GAME_H
