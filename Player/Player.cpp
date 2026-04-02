/* Author: Luke Cerwin */
#include "Player.h"
using namespace std;

Player::Player() {
    pet = PiPet();
    startDate = QDateTime::currentDateTime();
    streak = 0;
    goodDays = 0;
    hours = 0;
}

Player::Player(const PiPet& petInit)
    : streak(0), goodDays(0), hours(0), pet(petInit) {
    startDate = QDateTime::currentDateTime();  // initialize with current system time
}

// Getters
QDateTime Player::getStartDate() const { return startDate; }
int Player::getStreak() const { return streak; }
int Player::getGoodDays() const { return goodDays; }
int Player::getHours() const { return hours; }
PiPet Player::getPet() const { return pet; }

// Setters
void Player::setStartDate(QDateTime t) { startDate = t; }
void Player::setStreak(int s) { streak = s; }
void Player::setGoodDays(int g) { goodDays = g; }
void Player::setHours(int h) { hours = h; }
void Player::setPet(const PiPet& p) { pet = p; }

// Actions
void Player::feedPet() { pet.increase_hunger(10); }

void Player::sendPetToSleep() { pet.increase_energy(10); }

void Player::groomPet() { pet.increase_hygiene(10); }

void Player::givePetAffection() { pet.increase_happiness(10); }

double Player::getCareRating() const {
    if (streak == 0) return 0.0;
    return (static_cast<double>(goodDays) / streak) * 100.0;
}

void Player::celebratePetBirthday() {
    // If the pet is a baby, it ages to a teen.
    if (pet.age_group() == "Baby") pet.set_age_group("Teen");

    // If the pet is a teen, it ages to an adult.
    if (pet.age_group() == "Teen") pet.set_age_group("Adult");
}

Player Player::fromJSON(const QJsonObject &json) {
    PiPet pet;

    Player player(pet);

    // Load time/date that Player was first created
    if (const QJsonValue v = json["Start Date"]; v.isString())
        player.startDate = QDateTime::fromString(v.toString());

    // Load streak: how many consecutive days they opened the game
    if (const QJsonValue v = json["Streak"]; v.isDouble())
        player.streak = v.toInt();

    // Load goodDays: the number of days ended with pet's condition above 80%
    if (const QJsonValue v = json["Good Days"]; v.isDouble())
        player.goodDays = v.toInt();

    // Load hours: number of hours spent in game
    if (const QJsonValue v = json["Hours"]; v.isDouble())
        player.hours = v.toInt();

    // Load PiPet data
    if (const QJsonValue v = json["PiPet"]; v.isObject())
        // JSON object -> PiPet object -> give PiPet object to Player
        player.pet = PiPet::fromJSON(v.toObject());

    return player;
}

QJsonObject Player::toJson() const {
    QJsonObject json;
    // Save each member variable to a JSON value
    json["Start Date"] = startDate.toString();
    json["Streak"] = streak;
    json["Good Days"] = goodDays;
    json["Hours"] = hours;
    json["PiPet"] = pet.toJSON();
    return json;
}
