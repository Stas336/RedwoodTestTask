#include "item.h"

Item::Item(QWidget *parent):QTableWidgetItem(), QLabel()
{
    this->name = "";
    this->amount = 1;
}

int Item::getAmount()
{
    return this->amount;
}

std::string Item::getName()
{
    return this->name;
}

void Item::setAmount(int newAmount)
{
    this->amount = newAmount;
}

void Item::setName(std::string newName)
{
    this->name = newName;
}
