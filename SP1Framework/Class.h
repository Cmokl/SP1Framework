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
	void SetHealth(void);
	int GetHealth(void);
	void SetMana(void);
	int GetMana(void);
	void SetStrength(void);
	int GetStrength(void);
	void SetIntelligence(void);
	int GetIntelligence(void);
	void SetFaith(void);
	int GetFaith(void);
	void SetSpeed(void);
	int GetSpeed(void);
	void SetDefence(void);
	int GetDefence(void);
	void SetResistance(void);
	int GetResistance(void);
};

