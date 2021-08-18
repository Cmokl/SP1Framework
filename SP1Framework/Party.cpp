#include "Party.h"
Party::Party()
{
	for (int i = 0; i < 4; i++)
	{
		PartyClass[i] = nullptr;
	}
}

Party::Party(Class* Class1, Class* Class2, Class* Class3, Class* Class4)
{
	PartyClass[0] = Class1;
	PartyClass[1] = Class2;
	PartyClass[2] = Class3;
	PartyClass[3] = Class4;
}

Party::~Party()
{
}

//Getter/setters
Class* Party::GetPartyClass(int PartyIndex)
{
	if (PartyIndex > 3)
	{
		return PartyClass[0];
	}
	else
	{
		return PartyClass[PartyIndex];
	}
}

void Party::SetPartyClass(int PartyIndex, Class* ptr)
{
	if (PartyIndex < 4)
	{
		PartyClass[PartyIndex] = ptr;
	}
}
