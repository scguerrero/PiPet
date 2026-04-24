/*
 * Player.h is the Player header file.
 * Author(s): Luke Cerwin
 *
*/
#ifndef PLAYER_H
#define PLAYER_H
#include <QDateTime>
#include "../Pet/PiPet.h"
#include "../GUI/achievements_class/achievements.h"

class Player {
private:
    QDateTime startDate;
    int streak;
    int goodDays;
    int hours;

public:
    // Battle and minigame counters tracked here so they persist via JSON
    int         battleWins       = 0;
    int         pendingLootboxes = 0;
    QStringList wonLootboxItems;

    PiPet        pet;
    Achievements achievements;

    Player();
    Player(const PiPet& petInit);

    QDateTime getStartDate() const;
    int getStreak()    const;
    int getGoodDays()  const;
    int getHours()     const;
    PiPet getPet()     const;

    void setStartDate(QDateTime t);
    void setStreak(int s);
    void setGoodDays(int g);
    void setHours(int h);
    void setPet(const PiPet& p);

    void feedPet();
    void sendPetToSleep();
    void groomPet();
    void givePetAffection();
    double getCareRating() const;
    void celebratePetBirthday();

    static Player fromJSON(const QJsonObject &json);
    QJsonObject toJson() const;
};

#endif
