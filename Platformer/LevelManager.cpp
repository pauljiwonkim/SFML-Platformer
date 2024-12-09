#include "LevelManager.h"

std::unordered_map<std::string, LevelManager::LevelData> LevelManager::levels;

LevelManager::LevelManager()
{
    if (!platformTexture.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/sprites/platform.jpg")) {
        std::cerr << "Error loading platform texture\n";
    }
    if (!damagingTexture.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/sprites/damaging.jpg")) {
        std::cerr << "Error loading damaging texture\n";
    }
    createLevel1();
    createLevel2();
}




void LevelManager::addLevel(const std::string& levelId, const LevelData& levelData)
{
    levels[levelId] = levelData;
}

LevelManager::LevelData& LevelManager::getLevel(const std::string& levelId)
{
    return levels.at(levelId);
}

void LevelManager::drawLevel(const std::string& levelId, sf::RenderWindow& window)
{
    LevelData& level = getLevel(levelId);

    for (auto& pair : level.shapeMap) {
        pair.second.draw(window);
    }

}

void LevelManager::createLevel1() {
    LevelData level1;

    float scaleX = 1920.f / 1600.f;
    float scaleY = 1080.f / 1050.f;

    level1.shapeMap["groundPlatform"] = ShapeManager(
        sf::Vector2f(1600.f * scaleX, 400.f * scaleY),
        sf::Vector2f(0.f, 3100.f * scaleY),
        platformTexture,
        ShapeManager::ShapeType::Rectangle,
        ShapeManager::PlatformType::Normal
    );

    level1.shapeMap["platform1"] = ShapeManager(
        sf::Vector2f(600.f * scaleX, 30.f * scaleY),
        sf::Vector2f(300.f * scaleX, 2800.f * scaleY),
        platformTexture,
        ShapeManager::ShapeType::Rectangle,
        ShapeManager::PlatformType::Normal
    );

    level1.spawnPoint = sf::Vector2f(100, 100);
    levels["level1"] = level1;
}

void LevelManager::createLevel2()
{
}