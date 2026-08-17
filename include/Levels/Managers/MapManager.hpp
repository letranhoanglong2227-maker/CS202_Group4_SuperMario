#pragma once
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

namespace MapFormat {
inline constexpr float TILE_SIZE = 32.f;
inline constexpr unsigned int LOGICAL_MAP_HEIGHT = 15;
inline constexpr unsigned int MAP_LAYER_COUNT = 2;
inline constexpr unsigned int REQUIRED_MAP_HEIGHT =
    LOGICAL_MAP_HEIGHT * MAP_LAYER_COUNT;
}

enum class MapObjectType {
  SolidGround,
  Brick,
  CoinBlock,
  MushroomBlock,
  Pipe,
  Lava,
  LavaBottom,
  Trampoline,
  Cannon,
  CannonBody,
  CloudPlatform,
  WinFlag,
  Coin,
  Player1Spawn,
  Player2Spawn,
  Goomba,
  Koopa,
  FlyingKoopa,
  Heriss,
  PeteyPiranha,
  Bowser
};

struct MapSpawnInfo {
  MapObjectType type{MapObjectType::SolidGround};
  sf::Vector2f position{};
  int variant{0};
  int widthInTiles{1};
  int heightInTiles{1};
  float parameter{0.f};
};

struct MapDiagnostic {
  std::string file;
  sf::Vector2u pixel{};
  sf::Color color{};
  std::string message;
};

class MapManager {
public:
  MapManager(float gridSize = 32.f);

  bool loadFromImage(const std::string &filename);
  bool loadMap(const std::string& filename);
  void drawDebug(sf::RenderWindow &window) const;

  const std::vector<MapSpawnInfo>& getSpawns() const noexcept;
  const std::vector<MapDiagnostic>& getDiagnostics() const noexcept;
  unsigned int getMapWidth() const noexcept;
  const std::string& getLastError() const noexcept;

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
  void decodeObjectPixel(const sf::Color& color, unsigned int x,
                         unsigned int localY);
  void decodeActorPixel(const sf::Color& color, unsigned int x,
                        unsigned int localY);
  void addUnknownColor(const sf::Color& color, unsigned int x,
                       unsigned int y, const char* layer);

  float gridSize;
  std::string currentFile;
  std::string lastError;
  unsigned int mapWidth{0};
  std::vector<MapSpawnInfo> spawns;
  std::vector<MapDiagnostic> diagnostics;
  std::vector<sf::FloatRect> solidBlocks;
  sf::Vector2f spawnPointP1;
  sf::Vector2f spawnPointP2;
  std::vector<sf::Vector2f> goombaSpawns;
  std::vector<sf::Vector2f> coinBlockSpawns;
  std::vector<sf::Vector2f> mushroomBlockSpawns;
  std::vector<sf::Vector2f> brickSpawns;
};
