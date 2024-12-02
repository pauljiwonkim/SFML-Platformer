#ifndef LEVELMANAGER_H //levelmanager
#define LEVELMANAGER_H    //levelmanager

#include "ShapeManager.h"
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

class ShapeManager;

class LevelManager
{
    enum class PLatformType {
        Normal,
        Damaging,
        Goal

    };
public:
    struct LevelData
    {
        std::unordered_map<std::string, ShapeManager> shapeMap;
        std::string backgroundImage;
        sf::Vector2f spawnPoint;
    };
    LevelManager();

    void addLevel(const std::string& levelId, const LevelData& levelData);

    LevelManager::LevelData& getLevel(const std::string& levelId);

    void drawLevel(const std::string& levelId, sf::RenderWindow& window);

    static std::unordered_map<std::string, LevelData> levels;




private:



    void createLevel1();
    void createLevel2();

};


#endif // !LEVELMANAGER_H