/*
 * PiPet.cpp is the PiPet class implementation file.
 * Author(s): Sasha C. Guerrero
 * Source(s): "Saving and Loading a Game" QT Core Example, https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
 * Created: 2/5/2026
 * Last Edited: 3/25/2026
*/
#include "PiPet.h"

// piPet default constructor with no arguments
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
}

//piPet destructor
PiPet::~PiPet() {}

// Getters
QString PiPet::name() const { return m_name; }
QString PiPet::age_group() const { return m_age_group; }
QString PiPet::pet_type() const { return m_pet_type; }
QString PiPet::hat() const { return m_hat; }
int PiPet::days_old() const { return m_days_old; }
int PiPet::hunger() const { return m_hunger; }
int PiPet::energy() const { return m_energy; }
int PiPet::strength() const { return m_strength; }
int PiPet::hygiene() const { return m_hygiene; }
int PiPet::intelligence() const { return m_intelligence; }
int PiPet::happiness() const { return m_happiness; }
int PiPet::attack() const { return m_attack; }
int PiPet::defense() const { return m_defense; }
int PiPet::hit_points() const { return m_hit_points; }
bool PiPet::creation_flag() const { return m_been_created; }

// Setters
void PiPet::set_name(QString name) { m_name = name; }
void PiPet::set_age_group(QString age_group) { m_age_group = age_group; }
void PiPet::set_pet_type(QString t) { m_pet_type = t; }
void PiPet::set_hat(QString h) { m_hat = h; }
void PiPet::set_days_old(int days_old) { m_days_old = days_old; }
void PiPet::set_hunger(int hunger) { m_hunger = hunger; }
void PiPet::set_energy(int energy) { m_energy = energy; }
void PiPet::set_strength(int strength) { m_strength = strength; }
void PiPet::set_hygiene(int hygiene) { m_hygiene = hygiene; }
void PiPet::set_intelligence(int intelligence) { m_intelligence = intelligence; }
void PiPet::set_happiness(int happiness) { m_happiness = happiness; }
void PiPet::set_attack(int attack) { m_attack = attack; }
void PiPet::set_defense(int defense) { m_defense = defense; }
void PiPet::set_hit_points(int hit_points) { m_hit_points = hit_points; }
void PiPet::set_creation_flag(bool flag) { m_been_created = flag; }

// Incrementers: Increase the current value of a member variable by a given amount
void PiPet::increase_days_old(int days_old) { m_days_old += days_old; }
void PiPet::increase_hunger(int hunger) { m_hunger += hunger; }
void PiPet::increase_energy(int energy) { m_energy += energy; }
void PiPet::increase_strength(int strength) { m_strength += strength; }
void PiPet::increase_hygiene(int hygiene) { m_hygiene += hygiene; }
void PiPet::increase_intelligence(int intelligence) { m_intelligence += intelligence; }
void PiPet::increase_happiness(int happiness) { m_happiness += happiness; }
void PiPet::increase_attack(int attack) { m_attack += attack; }
void PiPet::increase_defense(int defense) { m_defense += defense; }
void PiPet::increase_hit_points(int hit_points) { m_hit_points += hit_points; }

// Create a PiPet object from a JSON file
// Based on "Saving and Loading a Game" QT Core Example: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
PiPet PiPet::fromJSON(const QJsonObject &json) {
    PiPet pet;
    // Put each JSON value into the corresponding member variable of pet

    // pet's name
    if (const QJsonValue v = json["Name"]; v.isString())
        pet.set_name(v.toString());

    // pet's age group
    if (const QJsonValue v = json["Age Group"]; v.isString())
        pet.set_age_group(v.toString());

    // pet type (DragonDog / ElectricAxolotl / SeelCat)
    if (const QJsonValue v = json["Pet Type"]; v.isString())
        pet.set_pet_type(v.toString());

    // equipped hat
    if (const QJsonValue v = json["Hat"]; v.isString())
        pet.set_hat(v.toString());

    // how old the pet is in days
    if (const QJsonValue v = json["Days Old"]; v.isDouble())
        pet.set_days_old(v.toInt());

    // pet's hunger level
    if (const QJsonValue v = json["Hunger"]; v.isDouble())
        pet.set_hunger(v.toInt());

    // pet's energy level
    if (const QJsonValue v = json["Energy"]; v.isDouble())
        pet.set_energy(v.toInt());

    // pet's strength level
    if (const QJsonValue v = json["Strength"]; v.isDouble())
        pet.set_strength(v.toInt());

    // pet's hygiene level
    if (const QJsonValue v = json["Hygiene"]; v.isDouble())
        pet.set_hygiene(v.toInt());

    // pet's intelligence level
    if (const QJsonValue v = json["Intelligence"]; v.isDouble())
        pet.set_intelligence(v.toInt());

    // pet's happiness level
    if (const QJsonValue v = json["Happiness"]; v.isDouble())
        pet.set_happiness(v.toInt());

    // pet's attack level
    if (const QJsonValue v = json["Attack"]; v.isDouble())
        pet.set_attack(v.toInt());

    // pet's defense level
    if (const QJsonValue v = json["Defense"]; v.isDouble())
        pet.set_defense(v.toInt());

    // pet's hit points
    if (const QJsonValue v = json["Hit Points"]; v.isDouble())
        pet.set_hit_points(v.toInt());

    // pet's creation flag
    if (const QJsonValue v = json["Created"]; v.isBool())
        pet.set_creation_flag(v.toBool());

    return pet;
}

// Create a JSON file from a PiPet object
// Based on "Saving and Loading a Game" QT Core Example: https://doc.qt.io/qt-6/qtcore-serialization-savegame-example.html
QJsonObject PiPet::toJSON() const {
    QJsonObject json;
    json["Name"] = m_name;
    json["Age Group"] = m_age_group;
    json["Pet Type"] = m_pet_type;
    json["Hat"] = m_hat;
    json["Days Old"] = m_days_old;
    json["Hunger"] = m_hunger;
    json["Energy"] = m_energy;
    json["Strength"] = m_strength;
    json["Hygiene"] = m_hygiene;
    json["Intelligence"] = m_intelligence;
    json["Happiness"] = m_happiness;
    json["Attack"] = m_attack;
    json["Defense"] = m_defense;
    json["Hit Points"] = m_hit_points;
    json["Created"] = m_been_created;
    return json;
}
