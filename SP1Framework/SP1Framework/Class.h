#pragma once

class Class
{
private:
	//stats
	int Health;
	int Mana;
	int Strength;
	int Intelligence;
	int Faith;
	int Speed;
	int Defence;
	int Resistance;
	int MaxHealth;

	//statuses
	bool IsBleed;
	bool IsBurn;
	bool IsPoison;
	bool IsImmune;

	//armour status
	bool HasArmour;
public:
	Class();
	virtual ~Class();

	//getter/setters(stats)
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
	int GetMaxHealth();
	void SetMaxHealth(int Value);

	//setter/getters(statuses). Conditions : Bleeding, burning, poisoned and immune
	bool GetIsBleed(void);
	void SetIsBleed(bool Boolean);
	bool GetIsBurn(void);
	void SetIsBurn(bool Boolean);
	bool GetIsPoison(void);
	void SetIsPoison(bool Boolean);
	bool GetIsImmune(void);
	void SetIsImmune(bool Boolean);

	//equip functions
	bool GetIsArmour(void);
	void SetArmourStatus(bool Bolean, int DefenceBonus);

	//functions
	virtual void Attack(Class* Target); //parameter determines the target
	virtual void Defend();
};

