#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <qlabel.h>

class Item : public QLabel
{
public:
    Item(std::string name, QLabel *parent = 0);
    void setName(std::string newName);
private:
    std::string name;
};

#endif // ITEM_H
