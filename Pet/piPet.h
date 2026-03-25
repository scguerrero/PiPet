/*
 * piPet.h is the piPet class specification file.
 * Author: Sasha C. Guerrero
 * Created: 2/4/2026
 * Last Edited: 3/25/2026
*/

#ifndef PIPET_H
#define PIPET_H
#include <QtWidgets>

// PiPet class declaration
class PiPet
{
<<<<<<< HEAD
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
=======
	private:
        // PiPet's name and age
        QString m_name = "Rasbaire";
        QString m_age_group = "Baby"; // Age groups: Baby, Teen, Adult
        int m_days_old; // The PiPet is initially 0 days old

        // The PiPet's Condition, or wellness attributes. Each attribute is out of 100.
        int m_hunger = 50;
        int m_energy = 50;
        int m_strength = 50;
        int m_hygiene = 50;
        int m_intelligence = 50;
        int m_happiness = 50;

        // The PiPet's Stats, or battle attributes.
        int m_attack = 10;
        int m_defense = 10;
        int m_hit_points = 100; // "hit points" or HP

	public:
        // Default constructor
        PiPet();

        // Constructor with arguments
        PiPet(QString, QString, int, int, int, int, int, int, int, int, int, int);

        // Default destructor
        ~PiPet();

        // Getters
        QString name();
        QString age_group();
        int days_old();
        int hunger();
        int energy();
        int strength();
        int hygiene();
        int intelligence();
        int happiness();
        int attack();
        int defense();
        int hit_points();

		// Setters
        bool set_hunger(int);
        bool set_energy(int);
        bool set_strength(int);
        bool set_hygiene(int);
        bool set_intelligence(int);
        bool set_happiness(int);
        bool set_days_old(int);
        bool set_age_group(int);
        bool set_attack(int);
        bool set_defense(int);
        bool set_hit_points(int);

        // Create a PiPet object from a JSON file
        static PiPet fromJSON(const QJsonObject &json);
>>>>>>> 277286c72284ea7e758520d31345a58bfaa48401

        // Create a JSON file from a PiPet object
        QJsonObject toJSON() const;
};
#endif
