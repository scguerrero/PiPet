/*
 * PiPet.h is the PiPet class specification file.
 * Author: Sasha C. Guerrero
 * Created: 2/4/2026
 * Last Edited: 3/25/2026
 * Updated: added m_unlockedHats for lootbox hat tracking (unlockHat, isHatUnlocked)
 * Fixed: m_days_old now initialized to 0 (was uninitialized garbage memory)
*/

#ifndef PIPET_H
#define PIPET_H
#include <QtWidgets>

class PiPet
{
private:
    QString m_name = "Rasbaire";
    QString m_age_group = "Baby";
    QString m_pet_type = "DragonDog"; // "DragonDog" | "ElectricAxolotl" | "SeelCat"
    QString m_hat = "";               // "" | "cowboy" | "crown" | "santa" | "wizard"
    int m_days_old = 0;

    int m_hunger = 50;
    int m_energy = 50;
    int m_strength = 50;
    int m_hygiene = 50;
    int m_intelligence = 50;
    int m_happiness = 50;

    int m_attack = 10;
    int m_defense = 10;
    int m_hit_points = 100;

    bool m_been_created = false; // Initially false, but becomes true after going through Pet Creation page

    // Lootbox-unlocked hats. Persisted under "Unlocked Hats" in the save file.
    // All four hat keys are tracked: "crown", "cowboy", "santa", "wizard".
    // A hat only appears in this list once it has been found via the minigames.
    QStringList m_unlockedHats;

public:
    PiPet();
    PiPet(QString, QString, int, int, int, int, int, int, int, int, int, int);
    ~PiPet();

    // Getters
    QString name() const;
    QString age_group() const;
    QString pet_type() const;
    QString hat() const;
    int days_old() const;
    int hunger() const;
    int energy() const;
    int strength() const;
    int hygiene() const;
    int intelligence() const;
    int happiness() const;
    int attack() const;
    int defense() const;
    int hit_points() const;
    bool creation_flag() const;

    // Lootbox hat unlock tracking
    QStringList unlockedHats()                    const;
    bool        isHatUnlocked(const QString &key) const;

    // Setters
    void set_name(QString);
    void set_age_group(QString);
    void set_pet_type(QString);
    void set_hat(QString);
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
    void set_creation_flag(bool);

    // Lootbox: add a hat key to the unlocked list (no-op if already present)
    void unlockHat(const QString &key);

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

    static PiPet fromJSON(const QJsonObject &json);
    QJsonObject toJSON() const;
};
#endif
