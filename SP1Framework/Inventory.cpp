#include "Inventory.h"

Inventory::Inventory()
{
	IsFull = false;
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
	int ItemAdded = 0;
	int ItemNumber = 0;
	//first loop finds an empty slot to put the new item in
	for (int i = 0; i < 10; i++)
	{
		if (InventoryItems[i] == nullptr && ItemAdded == 0)
		{
			InventoryItems[i] = item;
			ItemAdded = 1;
		}
		if (InventoryItems[i] != nullptr)
		{
			ItemNumber++;
		}
	}
	if (ItemNumber == 10)
	{
		IsFull = true;
	}
}

void Inventory::DiscardItem(Items* item)
{
	for (int i = 9; i >= 0; i--)
	{
		if (InventoryItems[i] == item)
		{
			InventoryItems[i] = nullptr;
			i = -1;
		}
	}
	IsFull = false;
}

Items* Inventory::GetItem(int ItemIndex)
{
	return InventoryItems[ItemIndex];
}

bool Inventory::CheckIsFull()
{
	return IsFull;
}
