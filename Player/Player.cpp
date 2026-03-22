#include "Player.h"
#include <iostream>
//#include <algorithm>
using namespace std;

Player::Player(const piPet& petInit)
    : streak(0), goodDays(0), hours(0), pet(petInit) {
    startDate = std::time(nullptr);  // initialize with current system time
}

// Getters
time_t Player::getStartDate() const { return startDate; }
int Player::getStreak() const { return streak; }
int Player::getGoodDays() const { return goodDays; }
int Player::getHours() const { return hours; }
piPet Player::getPet() const { return pet; }

// Setters
void Player::setStartDate(time_t t) { startDate = t; }
void Player::setStreak(int s) { streak = s; }
void Player::setGoodDays(int g) { goodDays = g; }
void Player::setHours(int h) { hours = h; }
void Player::setPet(const piPet& p) { pet = p; }

// Actions
void Player::feedPet() {
    pet.increaseHunger(10); // Example: assumes piPet has this method
    std::cout << "Pet fed!\n";
}

void Player::sendPetToSleep() {
    pet.increaseEnergy(10); // Assumes piPet has a sleep() method
    std::cout << "Pet is sleeping!\n";
}

void Player::groomPet() {
    pet.increaseHygiene(10); // Assumes piPet has a groom() method
    std::cout << "Pet groomed!\n";
}

double Player::getCareRating() const {
    if (streak == 0) return 0.0;
    return (static_cast<double>(goodDays) / streak) * 100.0;
}

void Player::celebratePetBirthday() {
    pet.moveUpAgeGroup(); // Assumes piPet has this method
    std::cout << "Happy Birthday to your pet!\n";
    //std::cout << "Pet age group: " << pet.getAgeGroup() << endl;
}
