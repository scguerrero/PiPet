/* Author: Luke Cerwin */
#ifndef PLAYER_H
#define PLAYER_H
#include <QDateTime>
#include "../Pet/PiPet.h"

class Player {
private:
    QDateTime startDate; // Time when object initialized
    int streak;        // Consecutive days opened
    int goodDays;      // Days pet's condition > 80%
    int hours;         // Hours spent in game

public:
    PiPet pet; // Composed pet object

    // Default constructor
    Player();

    // Constructor
    Player(const PiPet& petInit);

    // Getters
    QDateTime getStartDate() const;
    int getStreak() const;
    int getGoodDays() const;
    int getHours() const;
    PiPet getPet() const;

    // Setters
    void setStartDate(QDateTime t);
    void setStreak(int s);
    void setGoodDays(int g);
    void setHours(int h);
    void setPet(const PiPet& p);

    // Actions
    void feedPet();
    void sendPetToSleep();
    void groomPet();
    void givePetAffection();
    double getCareRating() const;  // goodDays / streak * 100
    void celebratePetBirthday();

    // Initialize a Player object from JSON
    static Player fromJSON(const QJsonObject &json);

    // Create a JSON from a Player object
    QJsonObject toJson() const;
};

#endif
