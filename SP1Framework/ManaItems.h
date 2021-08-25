#pragma once
#include "Items.h"
class ManaItems :
    public Items
{
public:
    ManaItems();
    ManaItems(std::string name, int cost, int ManaWorth);
    ~ManaItems();
    void ItemEffect(Class* Player);
};
