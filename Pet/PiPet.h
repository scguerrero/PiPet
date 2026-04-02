/*
 * PiPet.h is the PiPet class specification file.
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
    void set_name(QString);
    void set_age_group(QString);
    void set_days_old(int);
    void set_hunger(int);
    void set_energy(int);
    void set_strength(int);
    void set_hygiene(int);
    void set_intelligence(int);
    void set_happiness(int);
    void set_attack(int);
    void set_defense(int);
    void set_hit_points(int);

    // Incrementers
    void increase_days_old(int);
    void increase_hunger(int);
    void increase_energy(int);
    void increase_strength(int);
    void increase_hygiene(int);
    void increase_intelligence(int);
    void increase_happiness(int);
    void increase_attack(int);
    void increase_defense(int);
    void increase_hit_points(int);

    // Create a PiPet object from a JSON file
    static PiPet fromJSON(const QJsonObject &json);

    // Create a JSON file from a PiPet object
    QJsonObject toJSON() const;
};
#endif
