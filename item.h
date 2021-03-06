#ifndef ITEM_H
#define ITEM_H

#include <string.h>
#include <qtablewidget.h>
#include <qlabel.h>

class Item : public QTableWidgetItem
{
public:
    Item(QWidget *parent = 0);
    void setName(std::string newName);
    void setAmount(int newAmount);
    int getAmount();
    std::string getName();
private:
    std::string name;
    int amount;
signals:
    void appleMoving();
};

#endif // ITEM_H
