#pragma once
#include "Class.h"
class Ghoul :
    public Class
{
private:
    bool IsShapeshift;
public:
    Ghoul();
    ~Ghoul();

    bool GetIsShapeshift(void);
    void SetIsShapeshift(bool boolean);

    //skills
    void Devour(Class* Target);
    void Shapeshift(void);
    void RevertShapeshift(void);

    //skill list
    bool SkillList(int ListIndex, int ClassIndex, Class* TargetClass[]);
    std::string SkillNameList(int ListIndex);
};

