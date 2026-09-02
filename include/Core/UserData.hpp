#pragma once
#include <filesystem>
#include <string>
#include <vector>

struct LeaderboardEntry {
    std::string playerName;
    int score{};
    int world{1};
    int level{1};
};

class UserData {
private:
    std::string playerName;
    int lives;
    int score;
    int numberOfCoins;
    int currentWorld, currentLevel;
    int maxUnlockedWorld, maxUnlockedLevel;
    int character; // 0: Mario, 1: Luigi
    int playerForm; // 0: Small, 1: Big, 2: Fire

public:
    UserData();
    UserData(const std::string& name, int _lives = 3);
    UserData(const std::string& name, int _lives, int _score, int _numberOfCoins, 
             int _currentWorld, int _currentLevel, int _maxUnlockedWorld, 
             int _maxUnlockedLevel, int _character = 0,
             int _playerForm = 0);

    // Getters (Read-Only) - Necessary for UI, HUD & Leaderboard
    const std::string& getPlayerName() const { return playerName; }
    int getLives() const { return lives; }
    int getScore() const { return score; }
    int getNumberOfCoins() const { return numberOfCoins; }
    int getCurrentWorld() const { return currentWorld; }
    int getCurrentLevel() const { return currentLevel; }
    int getMaxUnlockedWorld() const { return maxUnlockedWorld; }
    int getMaxUnlockedLevel() const { return maxUnlockedLevel; }
    int getCharacter() const { return character; }
    int getPlayerForm() const { return playerForm; }

    void setPlayerName(std::string name);
    void setCharacter(int selectedCharacter);
    void setPlayerForm(int selectedForm);

    // Helper methods (Behavioral Updates)
    void add_score(const int addingscore);
    void add_coins(const int addingcoins = 1);
    void add_lives(const int addinglives = 1);
    void reduce_lives(const int reducinglives = 1);
    bool checkGameOver() const;
    void resetSessionData();

    // Progression methods
    void setCurrentLevel(int world, int level);
    void unlockNextLevel(int world, int level);

    bool saveTo(const std::filesystem::path& path,
                std::string* error = nullptr) const;
    bool loadFrom(const std::filesystem::path& path,
                  std::string* error = nullptr);
    bool updateLeaderboard(const std::filesystem::path& path,
                           std::string* error = nullptr) const;
    static std::vector<LeaderboardEntry> loadLeaderboard(
        const std::filesystem::path& path, std::string* error = nullptr);
};
