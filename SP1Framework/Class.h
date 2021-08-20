#pragma once

#include<string>

class Class
{
private:
	//stats
	std::string Name;
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
	bool IsSilenced;

	//class has taken it's turn or no
	bool Turn;
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
	bool GetIsSilenced(void);
	void SetIsSilenced(bool Boolean);

	//setter/getters for turn
	bool GetTurn(void);
	void SetTurn(bool Boolean);

	//setter/getters for name
	std::string GetName(void);
	void SetName(std::string str);

	//functions
	virtual void Attack(Class* Target); //parameter determines the target
	virtual void Defend();
	void RevertDefend();
	virtual void SkillList(int ListIndex, int ClassIndex, Class* TargetParty[4]) = 0;
	virtual std::string SkillNameList(int ListIndex) = 0;
};

