#include "item.h"

Item::Item(std::string name, QLabel *parent):QLabel(parent)
{
    this->setName(name);
}
