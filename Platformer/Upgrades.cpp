#include "Upgrades.h"
#include <iostream>

// Constructor to initialize the upgrade with its name, position, and effects
Upgrades::Upgrades(const std::string& name, float x, float y)
    : upgrade_name(name), upgrade_position(x, y) {
    shape.setSize(sf::Vector2f(20, 20)); // Size of the upgrade
    shape.setPosition(x, y);             // Position on the screen
    shape.setFillColor(sf::Color::Yellow);  // Default color for the upgrade
}

// Method to apply the effects of the upgrade to the player
void Upgrades::applyUpgradeEffect(Player& player) {
    if (!collected && shape.getGlobalBounds().intersects(player.getSpriteBounds())) {
        collected = true;  // Mark as collected
        

    // ** DEBUG **
        // Print abilities before upgrade
        std::cout << "Abilities before upgrade:" << std::endl;
        for (const auto& ability : player.abilities) {
            std::cout << ability.first << ": " << ability.second << std::endl;
        }

        // Apply the upgrade effects to the player's abilities
        if (upgrade_name == "extraJump") {
            upgradeJump(player);
        }
        else if (upgrade_name == "speedBoost") {
            upgradeSpeed(player);
        }

        // Print abilities after upgrade
        std::cout << "Abilities after upgrade:" << std::endl;
        for (const auto& ability : player.abilities) {
            std::cout << ability.first << ": " << ability.second << std::endl;
        }

        shape.setFillColor(sf::Color::Transparent); // Hide the upgrade once collected
        
   
    }
}

// ** UPGRADES **
void Upgrades::upgradeJump(Player& player) {
    player.abilities["jumps"]++;
    player.maxJumps = player.abilities["jumps"]; // Update maxJumps based on abilities
    std::cout << "Upgraded! Jump Amount: " << player.abilities["jumps"] << std::endl;
}

void Upgrades::upgradeSpeed(Player& player) {
    float currentSpeed = player.getPlayerSpeed(); // Get current speed using player's speed getter

    currentSpeed += 10;                           //Increase speed by 10
    player.setPlayerSpeed(currentSpeed);          // Set new speed using player's speed setter
    player.abilities["speed"] = currentSpeed;     // Update the speed in the abilities hashmap

    std::cout << "Upgraded! getPlayerSpeed: " << player.getPlayerSpeed() << std::endl;
    std::cout << "Upgraded! player.abilities[speed]: " << player.abilities["speed"] << std::endl;

}

// Method to draw the upgrade on the screen
void Upgrades::draw(sf::RenderWindow& window) {
    if (!collected) {
        window.draw(shape);  // Draw the upgrade if not collected
    }
}
