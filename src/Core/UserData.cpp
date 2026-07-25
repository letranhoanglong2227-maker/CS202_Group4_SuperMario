#include "Core/UserData.hpp"

UserData::UserData()
    : playerName("PlayerA"), score(0), lives(3), coins(0)
{
}

UserData::UserData(std::string name)
    : playerName(name), score(0), lives(3), coins(0)
{
}

UserData::UserData(std::string name, int lives_)
    : playerName(name), score(0), lives(lives_), coins(0)
{
}

std::string UserData::getPlayerName() const
{
    return this->playerName;
}

void UserData::setPlayerName(const std::string& name)
{
    this->playerName = name;
}

int UserData::getScore() const
{
    return this->score;
}

void UserData::setScore(int s)
{
    this->score = s;
}

void UserData::addScore(int s)
{
    this->score += s;
}

int UserData::getLives() const
{
    return this->lives;
}

void UserData::setLives(int l)
{
    this->lives = l;
}

void UserData::takeDamage()
{
    if (this->lives > 0) {
        this->lives--;
    }
}

void UserData::addLife(int l)
{
    this->lives += l;
}

int UserData::getCoins() const
{
    return this->coins;
}

void UserData::setCoins(int c)
{
    this->coins = c;
}

void UserData::addCoins(int c)
{
    this->coins += c;
    if (this->coins >= 100) {
        this->lives += this->coins / 100;
        this->coins %= 100;
    }
}

void UserData::reset()
{
    this->score = 0;
    this->lives = 3;
    this->coins = 0;
}
