/* Author: Luke Cerwin */
#include "Player.h"
using namespace std;

Player::Player(const PiPet& petInit)
    : streak(0), goodDays(0), hours(0), pet(petInit) {
    startDate = std::time(nullptr);  // initialize with current system time
}

// Getters
time_t Player::getStartDate() const { return startDate; }
int Player::getStreak() const { return streak; }
int Player::getGoodDays() const { return goodDays; }
int Player::getHours() const { return hours; }
PiPet Player::getPet() const { return pet; }

// Setters
void Player::setStartDate(time_t t) { startDate = t; }
void Player::setStreak(int s) { streak = s; }
void Player::setGoodDays(int g) { goodDays = g; }
void Player::setHours(int h) { hours = h; }
void Player::setPet(const PiPet& p) { pet = p; }

// Actions
void Player::feedPet() { pet.increase_hunger(10); }

void Player::sendPetToSleep() { pet.increase_energy(10); }

void Player::groomPet() { pet.increase_hygiene(10); }

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
