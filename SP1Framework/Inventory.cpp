#include "Inventory.h"

Inventory::Inventory()
{
	gold = 5;
	for (int i = 0; i < 10; i++)
	{
		InventoryItems[i] = nullptr;
	}
}
Inventory::~Inventory()
{

}
void Inventory::SetGold(int value)
{
	gold = value;
}
int Inventory::GetGold()
{
	return gold;
}
void Inventory::AddItem(Items* item)
{
	for (int i = 0; i < 10; i++)
	{
		if (InventoryItems[i] == nullptr)
		{
			InventoryItems[i] = item;
			i = 10;
		}
	}
}

void Inventory::DiscardItem(Items* item)
{
	for (int i = 0; i < 10; i++)
	{
		if (InventoryItems[i] == item)
		{
			InventoryItems[i] = nullptr;
			i = 10;
		}
	}
}

Items* Inventory::GetItem(int ItemIndex)
{
	return nullptr;
}
