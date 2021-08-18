#include "Shop.h"

Shop::Shop()
{
	Items* ShopList[10] = { nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
}
Shop::~Shop()
{
}
void Shop::AddItem(Items* NewItem)
{
	for (int i = 0; i < 10; i++)
	{
		if (ShopList[i] != nullptr)
		{
			ShopList[i] = NewItem
		}
	}
}
