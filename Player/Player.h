/* Author: Luke Cerwin */
#ifndef PLAYER_H
#define PLAYER_H

#include <ctime>
#include "../Pet/PiPet.h"

class Player {
private:
    time_t startDate;  // Time when object initialized
    int streak;        // Consecutive days opened
    int goodDays;      // Days pet's condition > 80%
    int hours;         // Hours spent in game

public:
    PiPet pet; // Composed pet object

    // Constructor
    Player(const PiPet& petInit);

    // Getters
    time_t getStartDate() const;
    int getStreak() const;
    int getGoodDays() const;
    int getHours() const;
    PiPet getPet() const;

    // Setters
    void setStartDate(time_t t);
    void setStreak(int s);
    void setGoodDays(int g);
    void setHours(int h);
    void setPet(const PiPet& p);

    // Actions
    void feedPet();
    void sendPetToSleep();
    void groomPet();
    double getCareRating() const;  // goodDays / streak * 100
    void celebratePetBirthday();
};

#endif
