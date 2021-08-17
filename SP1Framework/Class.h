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
	int MaxHealth;
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
	int GetMaxHealth();
	void SetMaxHealth(int Value);

	//functions
	void Attack(Class* Target); //Party index determines the target
	void Defend();
	void SkillList(int SkillIndex, Class* Target1, Class* Target2, Class* Target3, Class* Target4);
};

