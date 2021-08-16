#pragma once
#include"Party.h"
class Class
{
private:
	int Health;
	int Mana;
	int Strength;
	int Intelligence;
	int Faith;
	int Speed;
	int Defence;
	int Resistance;
	int Player_x;
	int Player_y;

public:
	Class();
	virtual ~Class();

	//getter/setters
	int GetHealth();
	void SetHealth(int Value);
	int GetMana(void);
	void SetMana(int Value);
	int GetStrength(void);
	void SetStrength(int Value);
	int GetIntelligence(void);
	void SetIntelligence(int Value);
	int GetFaith(void);
	void SetFaith(int Value);
	int GetSpeed(void);
	void SetSpeed(int Value);
	int GetDefence(void);
	void SetDefence(int Value);
	int GetResistance(void);
	void SetResistance(int Value);

	//functions
	void Attack(Party* Party , int PartyIndex); //Party index determines the target
	void Defend();
	void SkillList(int SkillIndex, Party* Party , int PartyIndex);

	void SetX(int current_x);
	void SetY(int current_y);
	int GetX();
	int GetY();
};

