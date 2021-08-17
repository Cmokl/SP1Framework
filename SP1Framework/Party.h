#pragma once
#include"Class.h"

class Party
{
private:
	Class* PartyClass[4];
public:
	Party();
	Party(Class* Class1, Class* Class2, Class* Class3, Class* Class4);
	~Party();

	//Getter/setters
	Class* GetPartyClass(int PartyIndex);
};