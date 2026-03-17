#ifndef PLAYER_H
#define PLAYER_H

#include <ctime>
#include "../Pet/piPet.h"

class Player {
private:
    time_t startDate;  // Time when object initialized
    int streak;        // Consecutive days opened
    int goodDays;      // Days pet's condition > 80%
    int hours;         // Hours spent in game

public:
    piPet pet; // Composed pet object

    // Constructor
    Player(const piPet& petInit);

    // Getters
    time_t getStartDate() const;
    int getStreak() const;
    int getGoodDays() const;
    int getHours() const;
    piPet getPet() const;

    // Setters
    void setStartDate(time_t t);
    void setStreak(int s);
    void setGoodDays(int g);
    void setHours(int h);
    void setPet(const piPet& p);

    // Actions
    void feedPet();
    void sendPetToSleep();
    void groomPet();
    double getCareRating() const;  // goodDays / streak * 100
    void celebratePetBirthday();
};

#endif
