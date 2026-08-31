#include "Objects/Environment/Pipe.hpp"
#include "Levels/Managers/MapManager.hpp"
#include <algorithm>
#include <utility>

Pipe::Pipe(sf::Vector2f pos, int heightInTiles, bool grayStyle) {
    const int height = std::max(1, heightInTiles);
    setPosition(pos);
    setSize({2.f * MapFormat::TILE_SIZE,
             static_cast<float>(height) * MapFormat::TILE_SIZE});
    body.setPosition(pos);
    body.setSize(getSize());
    body.setFillColor(sf::Color(38, 166, 91));
    body.setOutlineColor(sf::Color(18, 92, 48));
    body.setOutlineThickness(2.f);

    const int atlasX = grayStyle ? 188 : 103;
    tiles.reserve(static_cast<std::size_t>(2 * height));
    for (int row = 0; row < height; ++row) {
        const int atlasY = row == 0 ? 52 : 69;
        for (int column = 0; column < 2; ++column) {
            sf::Sprite tile(TextureBlockManager::getBlocksTexture());
            tile.setTextureRect({{atlasX + 17 * column, atlasY}, {16, 16}});
            tile.setScale({MapFormat::TILE_SIZE / 16.f,
                           MapFormat::TILE_SIZE / 16.f});
            tile.setPosition(pos + sf::Vector2f{
                column * MapFormat::TILE_SIZE,
                row * MapFormat::TILE_SIZE});
            tiles.push_back(std::move(tile));
        }
    }
}

void Pipe::update(float dt) {
    SolidBlock::update(dt);
    body.setPosition(getPosition());
    const int height = static_cast<int>(tiles.size() / 2);
    for (int row = 0; row < height; ++row)
        for (int column = 0; column < 2; ++column)
            tiles[static_cast<std::size_t>(2 * row + column)].setPosition(
                getPosition() + sf::Vector2f{
                    column * MapFormat::TILE_SIZE,
                    row * MapFormat::TILE_SIZE});
}

void Pipe::render(sf::RenderTarget* target) {
    if (!target) return;
    if (tiles.empty()) target->draw(body);
    else for (const auto& tile : tiles) target->draw(tile);
}

