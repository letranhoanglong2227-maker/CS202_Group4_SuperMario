#pragma once
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

class MapManager {
public:
  MapManager(float gridSize = 32.f);

  bool loadFromImage(const std::string &filename);
  v void drawDebug(sf::RenderWindow &window) const;

  const std::vector<sf::FloatRect> &getSolidBlocks() const;
  std::vector<sf::FloatRect> getNearbyBlocks(const sf::FloatRect &entityHitbox,
                                             float radius = 64.f) const;
  const sf::Vector2f &getSpawnPoint()
      const; // Đại diện cho điểm spawn của Player 1 (để tương thích ngược)
  const sf::Vector2f &getSpawnPointP1() const;
  const sf::Vector2f &getSpawnPointP2() const;
  const std::vector<sf::Vector2f> &getGoombaSpawns() const;
  const std::vector<sf::Vector2f> &getCoinBlockSpawns() const;
  const std::vector<sf::Vector2f> &getMushroomBlockSpawns() const;
  const std::vector<sf::Vector2f> &getBrickSpawns() const;

private:
  float gridSize;
  std::vector<sf::FloatRect> solidBlocks;
  sf::Vector2f spawnPointP1;
  sf::Vector2f spawnPointP2;
  std::vector<sf::Vector2f> goombaSpawns;
  std::vector<sf::Vector2f> coinBlockSpawns;
  std::vector<sf::Vector2f> mushroomBlockSpawns;
  std::vector<sf::Vector2f> brickSpawns;
};
