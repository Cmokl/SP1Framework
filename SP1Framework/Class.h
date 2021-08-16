#pragma once
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


public:
	Class();
	~Class();

	//getter/setters
	void GetHealth();
	int SetHealth(int Value);
	void GetMana(void);
	int SetMana(int Value);
	void GetStrength(void);
	int SetStrength(int Value);
	void GetIntelligence(void);
	int SetIntelligence(int Value);
	void GetFaith(void);
	int SetFaith(int Value);
	void GetSpeed(void);
	int SetSpeed(int Value);
	void GetDefence(void);
	int SetDefence(int Value);
	void GetResistance(void);
	int SetResistance(int Value);
};

