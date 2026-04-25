/*
 * Player.cc is the Player class implementation file.
 * Author(s): Luke Cerwin
 *
*/
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

QDateTime Player::getStartDate()   const { return startDate; }
int       Player::getStreak()      const { return streak; }
int       Player::getGoodDays()    const { return goodDays; }
int       Player::getHours()       const { return hours; }
QDate     Player::getLastGoodDayDate() const { return lastGoodDayDate; }
PiPet     Player::getPet()         const { return pet; }

void Player::setStartDate(QDateTime t)  { startDate       = t; }
void Player::setStreak(int s)           { streak          = s; }
void Player::setGoodDays(int g)         { goodDays        = g; }
void Player::setHours(int h)            { hours           = h; }
void Player::setLastGoodDayDate(QDate d){ lastGoodDayDate = d; }
void Player::setPet(const PiPet& p)     { pet             = p; }

void Player::updateHoursFromStartDate() {
    qint64 secs = startDate.secsTo(QDateTime::currentDateTime());
    hours = static_cast<int>(secs / 3600);
}

void Player::updateDaysOld() {
    int newDays = static_cast<int>(startDate.daysTo(QDateTime::currentDateTime()));
    int oldDays = pet.days_old();
    if (newDays <= oldDays) return;

    static const int kThresholds[] = { 7, 14 };
    for (int threshold : kThresholds) {
        if (oldDays < threshold && newDays >= threshold)
            celebratePetBirthday();
    }
    pet.set_days_old(newDays);
}

bool Player::checkAndAwardGoodDay() {
    QDate today = QDate::currentDate();
    if (pet.happiness() >= 100 && today != lastGoodDayDate) {
        goodDays++;
        lastGoodDayDate = today;
        return true;
    }
    return false;
}

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

    if (const QJsonValue v = json["Start Date"];       v.isString()) player.startDate        = QDateTime::fromString(v.toString(), Qt::ISODate);
    if (const QJsonValue v = json["Streak"];           v.isDouble()) player.streak           = v.toInt();
    if (const QJsonValue v = json["Good Days"];        v.isDouble()) player.goodDays         = v.toInt();
    if (const QJsonValue v = json["LastGoodDayDate"];  v.isString()) player.lastGoodDayDate  = QDate::fromString(v.toString(), Qt::ISODate);
    if (const QJsonValue v = json["BattleWins"];        v.isDouble()) player.battleWins        = v.toInt();
    if (const QJsonValue v = json["PendingLootboxes"]; v.isDouble()) player.pendingLootboxes  = v.toInt();
    if (const QJsonValue v = json["WonLootboxItems"];  v.isArray())  {
        for (const QJsonValue &item : v.toArray())
            player.wonLootboxItems.append(item.toString());
    }
    if (const QJsonValue v = json["PiPet"];            v.isObject()) player.pet              = PiPet::fromJSON(v.toObject());
    if (const QJsonValue v = json["Achievements"];     v.isObject()) player.achievements.fromJson(v.toObject());

    player.updateHoursFromStartDate();
    return player;
}

QJsonObject Player::toJson() const {
    QJsonObject json;
    json["Start Date"]      = startDate.toString(Qt::ISODate);
    json["Streak"]          = streak;
    json["Good Days"]       = goodDays;
    json["Hours"]           = hours;
    json["LastGoodDayDate"] = lastGoodDayDate.toString(Qt::ISODate);
    json["BattleWins"]       = battleWins;
    json["PendingLootboxes"] = pendingLootboxes;
    QJsonArray lootArr;
    for (const QString &name : wonLootboxItems)
        lootArr.append(name);
    json["WonLootboxItems"]  = lootArr;
    json["PiPet"]            = pet.toJSON();
    json["Achievements"]    = achievements.toJson();
    return json;
}
