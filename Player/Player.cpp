/* Author: Luke Cerwin */
#include "Player.h"

Player::Player() {
    pet        = PiPet();
    startDate  = QDateTime::currentDateTime();
    streak = goodDays = hours = battleWins = 0;
}

Player::Player(const PiPet& petInit)
    : streak(0), goodDays(0), hours(0),
      battleWins(0), pet(petInit) {
    startDate = QDateTime::currentDateTime();
}

QDateTime Player::getStartDate() const { return startDate; }
int       Player::getStreak()    const { return streak; }
int       Player::getGoodDays()  const { return goodDays; }
int       Player::getHours()     const { return hours; }
PiPet     Player::getPet()       const { return pet; }

void Player::setStartDate(QDateTime t) { startDate = t; }
void Player::setStreak(int s)          { streak    = s; }
void Player::setGoodDays(int g)        { goodDays  = g; }
void Player::setHours(int h)           { hours     = h; }
void Player::setPet(const PiPet& p)    { pet       = p; }

void Player::feedPet()          { pet.increase_hunger(10);    }
void Player::sendPetToSleep()   { pet.increase_energy(10);    }
void Player::groomPet()         { pet.increase_hygiene(10);   }
void Player::givePetAffection() { pet.increase_happiness(10); }

double Player::getCareRating() const {
    if (streak == 0) return 0.0;
    return (static_cast<double>(goodDays) / streak) * 100.0;
}

void Player::celebratePetBirthday() {
    if      (pet.age_group() == "Baby") pet.set_age_group("Teen");
    else if (pet.age_group() == "Teen") pet.set_age_group("Adult");
}

Player Player::fromJSON(const QJsonObject &json) {
    PiPet  p;
    Player player(p);

    if (const QJsonValue v = json["Start Date"];       v.isString()) player.startDate        = QDateTime::fromString(v.toString());
    if (const QJsonValue v = json["Streak"];           v.isDouble()) player.streak           = v.toInt();
    if (const QJsonValue v = json["Good Days"];        v.isDouble()) player.goodDays         = v.toInt();
    if (const QJsonValue v = json["Hours"];            v.isDouble()) player.hours            = v.toInt();
    if (const QJsonValue v = json["BattleWins"];       v.isDouble()) player.battleWins       = v.toInt();
    if (const QJsonValue v = json["PiPet"];            v.isObject()) player.pet              = PiPet::fromJSON(v.toObject());
    if (const QJsonValue v = json["Achievements"];     v.isObject()) player.achievements.fromJson(v.toObject());

    return player;
}

QJsonObject Player::toJson() const {
    QJsonObject json;
    json["Start Date"]      = startDate.toString();
    json["Streak"]          = streak;
    json["Good Days"]       = goodDays;
    json["Hours"]           = hours;
    json["BattleWins"]      = battleWins;
    json["PiPet"]           = pet.toJSON();
    json["Achievements"]    = achievements.toJson();
    return json;
}
