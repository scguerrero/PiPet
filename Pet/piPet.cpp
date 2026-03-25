/*
 * piPet.cpp is the piPet class implementation file.
 * Author: Sasha C. Guerrero
 * Created: 2/5/2026
 * Last Edited: 3/25/2026
*/
#include "piPet.h"
#include <cstdio>
#include <iostream>
using namespace std;

// piPet default constructor with no arguments
<<<<<<< HEAD
piPet::piPet()
{
    hunger = 50;
    energy = 50;
    strength = 50;
    hygiene = 50;
    intelligence = 50;
    happiness = 50;
    ageDays = 1;
    ageGroup = 0;
    attack = 100;
    defense = 100;
    healthPoints = 1000;
    //critRate = 0.10;
    critDmg = 100;
    skillPoints= 0;
}

//piPet constructor with arguments
piPet::piPet(int hgr, int en, int str, int hyg, int intel,
             int hap, int ageD, int ageG, int atk, int def,
             int hp, int dmg, int sp)
{
    hunger = hgr;
    energy = en;
    strength = str;
    hygiene = hyg;
    intelligence = intel;
    happiness = hap;
    ageDays = ageD;
    ageGroup = ageG;
    attack = atk;
    defense = def;
    healthPoints = hp;
    //critRate = rate;
    critDmg = dmg;
    skillPoints = sp;
=======
PiPet::PiPet() {}

//piPet constructor with arguments
PiPet::PiPet(QString name, QString age_group, int days_old, int hunger, int energy, int strength, int hygiene,
            int intelligence, int happiness, int attack, int defense, int hit_points) {

    // PiPet's name and age
    m_name = name;
    m_age_group = age_group; // Age groups: Baby, Teen, Adult
    m_days_old = days_old; // How many days old is the PiPet

    // The PiPet's Condition, or wellness attributes. Each attribute is out of 100.
    m_hunger = hunger;
    m_energy = energy;
    m_strength = strength;
    m_hygiene = hygiene;
    m_intelligence = intelligence;
    m_happiness = happiness;

    // The PiPet's Stats, or battle attributes.
    m_attack = attack;
    m_defense = defense;
    m_hit_points = hit_points; // "hit points" or HP
>>>>>>> 277286c72284ea7e758520d31345a58bfaa48401
}

//piPet destructor
PiPet::~PiPet() {}

// Condition: the pet's wellness attributes
void piPet::showCondition()
{
    printf("----Condition----\n");
    printf("Hunger: %d\nEnergy: %d\nStrength: %d\nHygiene: %d\nIntelligence: %d\nHappiness: %d\nDays Old: %d\nAge Group: %d\n",
           hunger, energy, strength, hygiene, intelligence, happiness, ageDays, ageGroup);
}

// Stats: the pet's battle attributes
/*void piPet::showStats()
{
    printf("----Stats----\n");
    printf("ATK: %d\nDEF: %d\nHP: %d\n", attack, defense, healthPoints);
    printf("CRIT Rate: %.0f%%\nCRIT Dmg: %d\nSP: %d\n", critRate*100, critDmg, skillPoints);
}*/

// the pet's Battle Actions
void piPet::basicAttack()
{
    cout << "The Pet deals 100 DMG." << endl;
}

void piPet::skillAttack()
{
    cout << "The Pet deals 200 DMG." << endl;
}

void piPet::defend()
{
    cout << "The Pet blocks 100 DMG." << endl;
}

// Setters returns false if input value is invalid.
// Otherwise, update member variable and return true.

// Setters for Condition attributes
bool piPet::setHunger(int h)
{
    if (h < 0 || h > 100) return false;
    hunger = h;
    return true;
}

bool piPet::setEnergy(int e)
{
    if (e < 0 || e > 100) return false;
    energy = e;
    return true;
}

bool piPet::setStrength(int s)
{
    if (s < 0 || s > 100) return false;
    strength = s;
    return true;
}

bool piPet::setHygiene(int h)
{
    if (h < 0 || h > 100) return false;
    hygiene = h;
    return true;
}

bool piPet::setIntelligence(int i)
{
    if (i < 0 || i > 100) return false;
    intelligence = i;
    return true;
}

bool piPet::setHappiness(int h)
{
    if (h < 0 || h > 100) return false;
    happiness = h;
    return true;
}

bool piPet::setAgeDays(int a)
{
    if (a < 0) return false;
    ageDays = a;
    return true;
}

bool piPet::setAgeGroup(int a)
{
    if (a < 0 || a > 2) return false;
    ageGroup = a;
    return true;
}

// Setters for Stats attributes
bool piPet::setAttack(int a)
{
    if (a < 0) return false;
    attack = a;
    return true;
}

bool piPet::setDefense(int d)
{
    if (d < 0) return false;
    defense = d;
    return true;
}

bool piPet::setHealthPoints(int h)
{
    if (h < 0) return false;
    healthPoints = h;
    return true;
}

/*bool piPet::setCritRate(double c)
{
    if (c < 0 || c > 1) return false;
    critRate = c;
    return true;
}*/

bool piPet::setCritDmg(int c)
{
    if (c < 0) return false;
    critDmg = c;
    return true;
}

bool piPet::setSkillPoints(int s)
{
    if (s < 0 || s > 5) return false;
    skillPoints = s;
    return true;
}

// Getters for Condition attributes
int piPet::getHunger()
{ return hunger; }

int piPet::getEnergy()
{ return energy; }

int piPet::getStrength()
{ return strength; }

int piPet::getHygiene()
{ return hygiene; }

int piPet::getIntelligence()
{ return intelligence; }

int piPet::getHappiness()
{ return happiness; }

int piPet::getAgeDays()
{ return ageDays; }

int piPet::getAgeGroup()
{ return ageGroup; }

// Getters for Stats attributes
int piPet::getAttack()
{ return attack; }

int piPet::getDefense()
{ return defense; }

int piPet::getHealthPoints()
{ return healthPoints; }

/*double piPet::getCritRate()
{ return critRate; }*/

int piPet::getCritDmg()
{ return critDmg; }

int piPet::getSkillPoints()
{ return skillPoints; }

// Incrementers add amnt ("amount") to the corresponding variable

void piPet::increaseHunger(int amnt)
{ hunger += amnt; }

void piPet::increaseEnergy(int amnt)
{ energy += amnt; }

void piPet::increaseStrength(int amnt)
{ strength += amnt; }

void piPet::increaseHygiene(int amnt)
{ hygiene += amnt; }

void piPet::increaseIntelligence(int amnt)
{ intelligence += amnt; }

void piPet::increaseHappiness(int amnt)
{ happiness += amnt; }

void piPet::increaseAgeDays(int amnt)
{ ageDays += amnt; }

void piPet::moveUpAgeGroup()
{ if (ageGroup < 3) ageGroup += 1; }

void piPet::increaseAttack(int amnt)
{ attack += amnt; }

void piPet::increaseDefense(int amnt)
{ defense += amnt; }

void piPet::increaseHealthPoints(int amnt)
{ healthPoints += amnt; }

/*void piPet::increaseCritRate(double amnt)
{ critRate += amnt; }*/

void piPet::increaseCritDmg(int amnt)
{ critDmg += amnt; }

void piPet::increaseSkillPoints(int amnt)
{ skillPoints += amnt; }
