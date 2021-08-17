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

	//setter/getters(statuses)
	bool GetIsBleed(void);
	void SetIsBleed(bool Boolean);

	//functions
	virtual void Attack(Class* Target); //parameter determines the target
	virtual void Defend();
};

