#pragma once
#include "Items.h"
class Shop
{
	Items* ShopList[10];
public:
	Shop();
	~Shop();
	void AddItem(Items* NewItem);
};