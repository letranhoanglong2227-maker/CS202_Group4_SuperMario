#include "Core/UserData.hpp"

UserData::UserData() 
    : playerName("Player"), lives(3), score(0), numberOfCoins(0), 
      currentWorld(1), currentLevel(1), maxUnlockedWorld(1), maxUnlockedLevel(1), character(0) {}

UserData::UserData(const std::string& name, int _lives) 
    : playerName(name), lives(_lives), score(0), numberOfCoins(0), 
      currentWorld(1), currentLevel(1), maxUnlockedWorld(1), maxUnlockedLevel(1), character(0) {}

UserData::UserData(const std::string& name, int _lives, int _score, int _numberOfCoins, 
                   int _currentWorld, int _currentLevel, int _maxUnlockedWorld, 
                   int _maxUnlockedLevel, int _character)
    : playerName(name), lives(_lives), score(_score), numberOfCoins(_numberOfCoins), 
      currentWorld(_currentWorld), currentLevel(_currentLevel), 
      maxUnlockedWorld(_maxUnlockedWorld), maxUnlockedLevel(_maxUnlockedLevel), character(_character) {}

void UserData::add_score(const int addingscore) {
    score += addingscore;
}

void UserData::add_coins(const int addingcoins) {
    numberOfCoins += addingcoins;
    if (numberOfCoins >= 100) {
        add_lives(numberOfCoins / 100);
        numberOfCoins %= 100;
    }
}

void UserData::add_lives(const int addinglives) {
    lives += addinglives;
}

void UserData::reduce_lives(const int reducinglives) {
    lives -= reducinglives;
    if (lives < 0) lives = 0;
}

bool UserData::checkGameOver() const {
    return lives <= 0;
}

void UserData::resetSessionData() {
    score = 0;
    numberOfCoins = 0;
    lives = 3;
}

void UserData::setCurrentLevel(int world, int level) {
    currentWorld = world;
    currentLevel = level;
}

void UserData::unlockNextLevel(int world, int level) {
    if (world > maxUnlockedWorld || (world == maxUnlockedWorld && level > maxUnlockedLevel)) {
        maxUnlockedWorld = world;
        maxUnlockedLevel = level;
    }
}