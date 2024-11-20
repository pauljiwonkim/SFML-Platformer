#ifndef UPGRADES_H
#define UPGRADES_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "Player.h"

class Player; 

class Upgrades {
public:
    std::string upgrade_name;   // Name of Upgrade
    sf::Vector2f upgrade_position;  // Position on the screen
    bool collected = false;     // Whether the upgrade has been collected
    sf::RectangleShape shape;   // Visual representation of the upgrade

    // Constructor for upgrades
    Upgrades(const std::string& name, float x, float y);

    // Method to apply the effects of the upgrade to the player
    void applyUpgradeEffect(Player& player);

    // Method to draw the upgrade on the screen
    void draw(sf::RenderWindow& window);

    // **** UPGRADES ****
    void upgradeJump(Player& player);

    void upgradeSpeed(Player& player);
};

#endif // UPGRADES_H
