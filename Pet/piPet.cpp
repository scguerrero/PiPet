/*
 * piPet.cpp is the piPet class implementation file.
 * Author: Sasha C. Guerrero
 * Created: 2/5/2026
 * Last Edited: 2/6/2026
*/
#include "piPet.h"
#include <cstdio>
#include <iostream>
using namespace std;

// piPet default constructor with no arguments
piPet::piPet()
{
    hunger = 0;
    energy = 0;
    strength = 0;
    hygiene = 0;
    intelligence = 0;
    happiness = 0;
    ageDays = 0;
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
}

//piPet destructor
piPet::~piPet()
{
}

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


// adding a clamp to every stats so that there will be no overflow
int piPet::clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Incrementers add amnt ("amount") to the corresponding variable

void piPet::increaseHunger(int amnt)
{ hunger = clamp(hunger +  amnt, 0, 100); }

void piPet::increaseEnergy(int amnt)
{ energy = clamp(energy + amnt, 0, 100); }

void piPet::increaseStrength(int amnt)
{ strength = clamp(strength + amnt, 0, 100); }

void piPet::increaseHygiene(int amnt)
{ hygiene = clamp(hygiene + amnt, 0, 100); }

void piPet::increaseIntelligence(int amnt)
{ intelligence = clamp(intelligence + amnt, 0, 100); }

void piPet::increaseHappiness(int amnt)
{ happiness = clamp(happiness + amnt, 0, 100); }

void piPet::increaseAgeDays(int amnt)
{ ageDays += amnt;

  // Every 7 days -> age group increases
    if (ageDays / 7 > ageGroup) {
        moveUpAgeGroup();
    }
}

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

// natural daily decay
void piPet::dailyDecay(){
    increaseHunger(-5);
    increaseEnergy(-5);
    increaseHygiene(-5);
    increaseHappiness(-5);
    increaseAgeDays(1);
}

//health consequesnces if stats drop too low
void piPet::updateHealth()
{
    if (hunger < 20) increaseHealthPoints(-5);
    if (hygiene < 20) increaseHealthPoints(-5);
    if (happiness < 20) increaseHealthPoints(-5);

    healthPoints = clamp(healthPoints, 0, 1000);
}

// updates everything each day
void piPet::tickDay()
{
    dailyDecay();
    updateHealth();
}