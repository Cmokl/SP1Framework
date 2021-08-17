#include "Inventory.h"

Inventory::Inventory()
{
	gold = 5;
	for (int i = 0; i < 10; i++)
	{
		InventoryItems[i] = "NULL";
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
void Inventory::AddItem(std::string ItemName)
{
	for (int i = 0; i < 10; i++)
	{
		if (InventoryItems[i] == "NULL")
		{
			InventoryItems[i] = ItemName;
			i = 10;
		}
	}
}
void Inventory::DiscardItem(std::string ItemName)
{
	for (int i = 0; i < 10; i++)
	{
		if (InventoryItems[i] == ItemName)
		{
			InventoryItems[i] = "NULL";
			i = 10;
		}
	}
}