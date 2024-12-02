#include "LevelManager.h"

std::unordered_map<std::string, LevelManager::LevelData> LevelManager::levels;

LevelManager::LevelManager()
{
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

void LevelManager::createLevel1()
{
    // Scaling factors for 1920x1080 (compared to the original 1600x1050)
    float scaleX = 1920.f / 1600.f;
    float scaleY = 1080.f / 1050.f;

    LevelData level1;
    //level1.shapeMap["circle1"] = ShapeManager(50, sf::Vector2f(0.f, 3100.f * scaleY), sf::Color::White, ShapeManager::ShapeType::Circle, ShapeManager::PlatformType::Damaging);
    level1.shapeMap["platform1"] = ShapeManager(sf::Vector2f(1600.f * scaleX, 400.f * scaleY), sf::Vector2f(0.f, 3100.f * scaleY), sf::Color::White, ShapeManager::ShapeType::Rectangle, ShapeManager::PlatformType::Normal);

    level1.spawnPoint = sf::Vector2f(100, 100);
    levels["level1"] = level1;
}

void LevelManager::createLevel2()
{
}