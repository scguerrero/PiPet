#ifndef PLAYER_H
#define PLAYER_H

#include <ctime>
#include "../Pet/piPet.h"

class Player {
private:
    time_t startDate;
    int streak;
    int goodDays;
    int hours;

    piPet* pet;   // store pointer to real pet

public:
    Player(piPet* petInit);   // constructor takes pointer

    // Getters
    time_t getStartDate() const;
    int getStreak() const;
    int getGoodDays() const;
    int getHours() const;

    piPet& getPet() const;     // return reference to real pet
    piPet* getPetPtr() const;  // optional pointer getter

    // Setters
    void setStartDate(time_t t);
    void setStreak(int s);
    void setGoodDays(int g);
    void setHours(int h);
    void setPet(piPet* p);     // setter takes pointer

    // Actions
    void feedPet();
    void sendPetToSleep();
    void groomPet();
    void givePetAffection();
    double getCareRating() const;
    void celebratePetBirthday();
};

#endif // PLAYER_H
