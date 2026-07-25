#pragma once
#include <string>

class UserData {
private:
    std::string playerName;
    int score{ 0 };
    int lives{ 3 };
    int coins{ 0 };

public:
    UserData();
    UserData(std::string name);
    UserData(std::string name, int lives_);

    std::string getPlayerName() const;
    void setPlayerName(const std::string& name);

    int getScore() const;
    void setScore(int s);
    void addScore(int s);

    int getLives() const;
    void setLives(int l);
    void takeDamage();
    void addLife(int l = 1);

    int getCoins() const;
    void setCoins(int c);
    void addCoins(int c = 1);

    void reset();
};

