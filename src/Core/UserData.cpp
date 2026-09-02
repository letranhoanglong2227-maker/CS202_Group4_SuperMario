#include "Core/UserData.hpp"

#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <utility>

namespace {
constexpr std::size_t LeaderboardLimit = 10;

int stageRank(int world, int level) {
    return (world - 1) * 3 + level;
}

bool validStage(int world, int level) {
    return world >= 1 && world <= 3 && level >= 1 && level <= 3;
}

void sortLeaderboard(std::vector<LeaderboardEntry>& entries) {
    std::sort(entries.begin(), entries.end(),
              [](const LeaderboardEntry& lhs, const LeaderboardEntry& rhs) {
                  if (lhs.score != rhs.score) return lhs.score > rhs.score;
                  const int lhsProgress = stageRank(lhs.world, lhs.level);
                  const int rhsProgress = stageRank(rhs.world, rhs.level);
                  if (lhsProgress != rhsProgress) return lhsProgress > rhsProgress;
                  return lhs.playerName < rhs.playerName;
              });
    if (entries.size() > LeaderboardLimit) entries.resize(LeaderboardLimit);
}
}

UserData::UserData() 
    : playerName("Player"), lives(3), score(0), numberOfCoins(0), 
      currentWorld(1), currentLevel(1), maxUnlockedWorld(1), maxUnlockedLevel(1),
      character(0), playerForm(0) {}

UserData::UserData(const std::string& name, int _lives) 
    : playerName(name), lives(_lives), score(0), numberOfCoins(0), 
      currentWorld(1), currentLevel(1), maxUnlockedWorld(1), maxUnlockedLevel(1),
      character(0), playerForm(0) {}

UserData::UserData(const std::string& name, int _lives, int _score, int _numberOfCoins, 
                   int _currentWorld, int _currentLevel, int _maxUnlockedWorld, 
                   int _maxUnlockedLevel, int _character, int _playerForm)
    : playerName(name), lives(_lives), score(_score), numberOfCoins(_numberOfCoins), 
      currentWorld(_currentWorld), currentLevel(_currentLevel), 
      maxUnlockedWorld(_maxUnlockedWorld), maxUnlockedLevel(_maxUnlockedLevel),
      character(_character), playerForm(std::clamp(_playerForm, 0, 2)) {}

void UserData::setPlayerName(std::string name) {
    if (!name.empty()) playerName = std::move(name);
}

void UserData::setCharacter(int selectedCharacter) {
    character = selectedCharacter == 1 ? 1 : 0;
}

void UserData::setPlayerForm(int selectedForm) {
    playerForm = std::clamp(selectedForm, 0, 2);
}

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
    playerForm = 0;
}

void UserData::setCurrentLevel(int world, int level) {
    if (!validStage(world, level)) return;
    currentWorld = world;
    currentLevel = level;
}

void UserData::unlockNextLevel(int world, int level) {
    if (validStage(world, level) &&
        stageRank(world, level) ==
            stageRank(maxUnlockedWorld, maxUnlockedLevel) + 1) {
        maxUnlockedWorld = world;
        maxUnlockedLevel = level;
    }
}

bool UserData::saveTo(const std::filesystem::path& path,
                      std::string* error) const {
    std::ofstream output(path, std::ios::trunc);
    if (!output) {
        if (error) *error = "Could not open save file for writing";
        return false;
    }
    output << "GROUP4_SAVE_V2\n"
           << std::quoted(playerName) << '\n'
           << lives << ' ' << score << ' ' << numberOfCoins << ' '
           << currentWorld << ' ' << currentLevel << ' '
           << maxUnlockedWorld << ' ' << maxUnlockedLevel << ' '
           << character << ' ' << playerForm << '\n';
    if (!output) {
        if (error) *error = "Could not finish writing save file";
        return false;
    }
    if (error) error->clear();
    return true;
}

bool UserData::loadFrom(const std::filesystem::path& path,
                        std::string* error) {
    std::ifstream input(path);
    std::string version;
    std::string loadedName;
    int loadedLives = 0;
    int loadedScore = 0;
    int loadedCoins = 0;
    int loadedWorld = 0;
    int loadedLevel = 0;
    int loadedMaxWorld = 0;
    int loadedMaxLevel = 0;
    int loadedCharacter = 0;
    int loadedPlayerForm = 0;
    if (!input || !std::getline(input, version) ||
        (version != "GROUP4_SAVE_V1" && version != "GROUP4_SAVE_V2") ||
        !(input >> std::quoted(loadedName) >> loadedLives >> loadedScore >>
          loadedCoins >> loadedWorld >> loadedLevel >> loadedMaxWorld >>
          loadedMaxLevel >> loadedCharacter) ||
        (version == "GROUP4_SAVE_V2" && !(input >> loadedPlayerForm))) {
        if (error) *error = "Save file is missing or invalid";
        return false;
    }
    input >> std::ws;
    if (!input.eof()) {
        if (error) *error = "Save file contains trailing data";
        return false;
    }

    if (loadedName.empty() || loadedName.size() > 24 || loadedLives < 0 ||
        loadedLives > 99 || loadedScore < 0 || loadedCoins < 0 ||
        loadedCoins >= 100 || !validStage(loadedWorld, loadedLevel) ||
        !validStage(loadedMaxWorld, loadedMaxLevel) ||
        (loadedCharacter != 0 && loadedCharacter != 1) ||
        loadedPlayerForm < 0 || loadedPlayerForm > 2) {
        if (error) *error = "Save file contains out-of-range values";
        return false;
    }

    playerName = std::move(loadedName);
    lives = loadedLives;
    score = loadedScore;
    numberOfCoins = loadedCoins;
    currentWorld = loadedWorld;
    currentLevel = loadedLevel;
    maxUnlockedWorld = loadedMaxWorld;
    maxUnlockedLevel = loadedMaxLevel;
    character = loadedCharacter;
    playerForm = loadedPlayerForm;
    if (error) error->clear();
    return true;
}

std::vector<LeaderboardEntry> UserData::loadLeaderboard(
    const std::filesystem::path& path, std::string* error) {
    std::ifstream input(path);
    if (!input) {
        if (error) error->clear();
        return {};
    }

    std::string version;
    if (!std::getline(input, version) || version != "GROUP4_LEADERBOARD_V1") {
        if (error) *error = "Leaderboard file is invalid";
        return {};
    }

    std::vector<LeaderboardEntry> entries;
    LeaderboardEntry entry;
    while (input >> std::quoted(entry.playerName) >> entry.score >>
                    entry.world >> entry.level) {
        if (entry.playerName.empty() || entry.playerName.size() > 24 ||
            entry.score < 0 || !validStage(entry.world, entry.level)) {
            if (error) *error = "Leaderboard contains out-of-range values";
            return {};
        }
        entries.push_back(entry);
        if (entries.size() > 100) {
            if (error) *error = "Leaderboard contains too many entries";
            return {};
        }
    }
    if (!input.eof()) {
        if (error) *error = "Leaderboard contains malformed data";
        return {};
    }
    sortLeaderboard(entries);
    if (error) error->clear();
    return entries;
}

bool UserData::updateLeaderboard(const std::filesystem::path& path,
                                 std::string* error) const {
    std::string loadError;
    auto entries = loadLeaderboard(path, &loadError);
    if (!loadError.empty()) {
        if (error) *error = loadError;
        return false;
    }

    const auto existing = std::find_if(
        entries.begin(), entries.end(), [this](const LeaderboardEntry& entry) {
            return entry.playerName == playerName;
        });
    if (existing == entries.end()) {
        entries.push_back({playerName, score, currentWorld, currentLevel});
    } else if (score > existing->score) {
        existing->score = score;
        existing->world = currentWorld;
        existing->level = currentLevel;
    } else if (score == existing->score &&
               stageRank(currentWorld, currentLevel) >
                   stageRank(existing->world, existing->level)) {
        existing->world = currentWorld;
        existing->level = currentLevel;
    }
    sortLeaderboard(entries);

    std::ofstream output(path, std::ios::trunc);
    if (!output) {
        if (error) *error = "Could not open leaderboard for writing";
        return false;
    }
    output << "GROUP4_LEADERBOARD_V1\n";
    for (const auto& saved : entries) {
        output << std::quoted(saved.playerName) << ' ' << saved.score << ' '
               << saved.world << ' ' << saved.level << '\n';
    }
    if (!output) {
        if (error) *error = "Could not finish writing leaderboard";
        return false;
    }
    if (error) error->clear();
    return true;
}
