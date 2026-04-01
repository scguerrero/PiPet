#include "Player.h"
#include <iostream>

Player::Player(piPet* petInit)
    : streak(0), goodDays(0), hours(0), pet(petInit)
{
    startDate = std::time(nullptr);
}

// Getters
time_t Player::getStartDate() const { return startDate; }
int Player::getStreak() const { return streak; }
int Player::getGoodDays() const { return goodDays; }
int Player::getHours() const { return hours; }

piPet& Player::getPet() const { return *pet; }
piPet* Player::getPetPtr() const { return pet; }

// Setters
void Player::setStartDate(time_t t) { startDate = t; }
void Player::setStreak(int s) { streak = s; }
void Player::setGoodDays(int g) { goodDays = g; }
void Player::setHours(int h) { hours = h; }
void Player::setPet(piPet* p) { pet = p; }

// Actions
void Player::feedPet() {
    pet->increaseHunger(10);
    std::cout << "Pet fed!\n";
}

void Player::sendPetToSleep() {
    pet->increaseEnergy(10);
    std::cout << "Pet is sleeping!\n";
}

void Player::groomPet() {
    pet->increaseHygiene(10);
    std::cout << "Pet groomed!\n";
}

void Player::givePetAffection() {
    pet->increaseHappiness(10);
    std::cout << "Pet is happy!\n";
}

double Player::getCareRating() const {
    if (streak == 0) return 0.0;
    return (static_cast<double>(goodDays) / streak) * 100.0;
}

void Player::celebratePetBirthday() {
    pet->moveUpAgeGroup();
    std::cout << "Happy Birthday to your pet!\n";
}
