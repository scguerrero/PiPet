/*
 * piPet.h is the piPet class specification file.
 * Author: Sasha C. Guerrero
 * Created: 2/4/2026
 * Last Edited: 2/10/2026
*/

#ifndef PIPET_H
#define PIPET_H

// piPet class declaration
class piPet
{
	private:
		// The piPet's Condition, or wellness attributes
		int hunger;
		int energy;
		int strength;
		int hygiene;
		int intelligence;
		int happiness;
		int ageDays;
		int ageGroup;

		// The piPet's Stats, or battle attributes
		int attack;
		int defense;
		int healthPoints;
        //double critRate;
		int critDmg;
		int skillPoints;

	public:
		// Constructors (no arguments, with arguments) and destructor
		piPet();
        piPet(int, int, int, int, int, int, int, int, int, int, int, int, int);
		~piPet();

		// Utilities
		void showCondition();
		void showStats();

		// The piPet's Battle Actions
		void basicAttack();
		void skillAttack();
		void defend();

		// Setters
		bool setHunger(int);
		bool setEnergy(int);
		bool setStrength(int);
		bool setHygiene(int);
		bool setIntelligence(int);
		bool setHappiness(int);
		bool setAgeDays(int);
		bool setAgeGroup(int);
		bool setAttack(int);
		bool setDefense(int);
		bool setHealthPoints(int);
        //bool setCritRate(double);
		bool setCritDmg(int);
		bool setSkillPoints(int);

		// Getters
		int getHunger();
		int getEnergy();
		int getStrength();
		int getHygiene();
		int getIntelligence();
		int getHappiness();
		int getAgeDays();
		int getAgeGroup();
		int getAttack();
		int getDefense();
		int getHealthPoints();
        //double getCritRate();
		int getCritDmg();
		int getSkillPoints();

		// Incrementers
		void increaseHunger(int);
		void increaseEnergy(int);
		void increaseStrength(int);
		void increaseHygiene(int);
		void increaseIntelligence(int);
		void increaseHappiness(int);
		void increaseAgeDays(int);
		void moveUpAgeGroup();
		void increaseAttack(int);
		void increaseDefense(int);
		void increaseHealthPoints(int);
        //void increaseCritRate(double);
		void increaseCritDmg(int);
		void increaseSkillPoints(int);

};
#endif
