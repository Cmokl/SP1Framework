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
void Inventory::AddItem(Items* NewItem)
{
	for (int i = 0; i < 10; i++)
	{
		if (InventoryItems[i] == nullptr)
		{
			InventoryItems[i] = NewItem;

			i = 10;
		}
	}
}
void Inventory::DiscardItem(Items* DiscardedItem)
{
	for (int i = 0; i < 10; i++)
	{
		if (InventoryItems[i] == DiscardedItem)
		{
			InventoryItems[i] = nullptr;
			i = 10;
		}
	}
}
void Inventory::UseItem(Items* UsedItem, Class* target)
{
	UsedItem->ItemFunction(target);
	this->DiscardItem(UsedItem);
}