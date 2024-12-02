#include "Upgrades.h"
#include <iostream>

// Initialize the static upgrade_map
std::unordered_map<std::string, Upgrades> Upgrades::upgrade_map;

// Constructor for Upgrades
Upgrades::Upgrades(const std::string& name, sf::RectangleShape shape, const sf::Vector2f& size,
    int level, float duration, const sf::Vector2f& position, const sf::Color& color,
    bool collected, bool upgradeActive, EffectType effectType)
    : upgradeName(name),
    upgradeShape(shape),
    upgradeSize(size),
    upgradeLevel(level),
    upgradeDuration(duration),
    upgradePosition(position),
    upgradeColor(color),
    collected(false),
    upgradeActive(false),
    isRespawning(false),    
    respawnDelay(10.0f),
    upgradeEffectType(effectType)
{
    upgradeShape.setSize(upgradeSize);
    upgradeShape.setFillColor(upgradeColor);
    upgradeShape.setPosition(upgradePosition);
}

// Method to apply the effects of the upgrade to the player
void Upgrades::applyUpgradeEffect(Player& player) {
    if (!collected && upgradeShape.getGlobalBounds().intersects(player.getSpriteBounds())) {
        collected = true;  // Mark as collected

        // ** DEBUG **
            // Print abilities before upgrade
        std::cout << "Abilities before upgrade:" << std::endl;
        for (const auto& ability : player.abilities) {
            std::cout << ability.first << ": " << ability.second << std::endl;
        }

        // Apply the upgrade effects to the player's abilities
        if (upgradeName == "ExtraJump") {
            upgradeJump(player);
        }
        else if (upgradeName == "SpeedBoost") {
            upgradeSpeed(player);
        }

        // Print abilities after upgrade
        std::cout << "Abilities after upgrade:" << std::endl;
        for (const auto& ability : player.abilities) {
            std::cout << ability.first << ": " << ability.second << std::endl;
        }

        upgradeShape.setFillColor(sf::Color::Transparent); // Hide the upgrade once collected
    }
}


// ** UPGRADES **
// Handle jump upgrades
void Upgrades::upgradeJump(Player& player) {

    // Apply effects based on the upgrade level
    if (upgradeLevel == 1) {
        player.abilities["jumps"] += 1;
    }
    else if (upgradeLevel == 2) {
        player.setJumpStrength(player.getJumpStrength() - 5); // Increase jump strength for higher level
    }
    else if (upgradeLevel == 3) {
        player.abilities["jumps"] += 2;
    }

    player.setMaxJumps(player.abilities["jumps"]);
    std::cout << "Extra Jump Upgrade Applied: Jumps = " << player.abilities["jumps"] << "\n";
}

void Upgrades::upgradeSpeed(Player& player) {
    // Apply effects based on the upgrade level
    float currentSpeed = player.getPlayerSpeed();
    if (upgradeLevel == 1) {
        currentSpeed += 2;
    }
    else if (upgradeLevel == 2) {
        currentSpeed += 5;
    }
    else if (upgradeLevel == 3) {
        currentSpeed += 10;
    }

    player.setPlayerSpeed(currentSpeed);
    player.abilities["speed"] = currentSpeed;

    std::cout << "Speed Boost Upgrade Applied: Speed = " << currentSpeed << "\n";
}

// Static method to initialize all upgrades (Static Method)
void Upgrades::initializeUpgrades() {
    //(std::string& name, sf::RectangleShape shape, sf::Vector2f& size, int level, float duration, sf::Vector2f& position, sf::Color& color, bool collected, bool upgradeActive, EffectType effectType)

    // Create upgrade shapes
    sf::RectangleShape speedShape1(sf::Vector2f(20, 20));
    sf::RectangleShape speedShape2(sf::Vector2f(20, 20));
    sf::RectangleShape speedShape3(sf::Vector2f(20, 20));
    sf::RectangleShape jumpShape1(sf::Vector2f(20, 20));
    sf::RectangleShape jumpShape2(sf::Vector2f(20, 20));
    sf::RectangleShape jumpShape3(sf::Vector2f(20, 20));

    // Initialize Speed Boost upgrades
    Upgrades speedUpgrade1("SpeedBoost", speedShape1, sf::Vector2f(20, 20), 1, 3.0f, sf::Vector2f(250.0f, 3050.0f), sf::Color::White, false, false, EffectType::SpeedBoost);
    Upgrades speedUpgrade2("SpeedBoost", speedShape2, sf::Vector2f(20, 20), 2, 5.0f, sf::Vector2f(350.0f, 3050.0f), sf::Color::White, false, false, EffectType::SpeedBoost);
    Upgrades speedUpgrade3("SpeedBoost", speedShape3, sf::Vector2f(20, 20), 3, 10.0f, sf::Vector2f(450.0f, 3050.0f), sf::Color::White, false, false, EffectType::SpeedBoost);

    // Initialize Extra Jump upgrades
    Upgrades jumpUpgrade1("ExtraJump", jumpShape1, sf::Vector2f(20, 20), 1, 3.0f, sf::Vector2f(750.0f, 3050.0f), sf::Color::Yellow, false, false, EffectType::ExtraJump);
    Upgrades jumpUpgrade2("ExtraJump", jumpShape2, sf::Vector2f(20, 20), 2, 5.0f, sf::Vector2f(850.0f, 3050.0f), sf::Color::Yellow, false, false, EffectType::ExtraJump);
    Upgrades jumpUpgrade3("ExtraJump", jumpShape3, sf::Vector2f(20, 20), 3, 10.0f, sf::Vector2f(950.0f, 3050.0f), sf::Color::Yellow, false, false, EffectType::ExtraJump);

    // Add upgrades to the map
    upgrade_map["SpeedBoost1"] = speedUpgrade1;
    upgrade_map["SpeedBoost2"] = speedUpgrade2;
    upgrade_map["SpeedBoost3"] = speedUpgrade3;
    upgrade_map["ExtraJump1"] = jumpUpgrade1;
    upgrade_map["ExtraJump2"] = jumpUpgrade2;
    upgrade_map["ExtraJump3"] = jumpUpgrade3;
}


void Upgrades::updateEffectDuration(Player& player) {
    if (upgradeActive) {
        float elapsedTime = durationTimer.getElapsedTime().asSeconds();

        std::cout << "Upgrade: " << upgradeName
            << " | Elapsed Time: " << elapsedTime << " seconds.\n";

        if (elapsedTime >= upgradeDuration) {
            switch (upgradeEffectType) {
            case EffectType::ExtraJump:
                player.abilities["jumps"] -= (upgradeLevel == 1 ? 1 : (upgradeLevel == 3 ? 2 : 0)); //condition ? value_if_true : value_if_false;
                player.setMaxJumps(player.abilities["jumps"]);

                // If upgradeLevel == 2, reset jumpStrength
                if (upgradeLevel == 2) {
                    player.setJumpStrength(player.getJumpStrength() - 15.0f);
                    std::cout << "Jump Strength upgrade expired.\n";
                }

                std::cout << "Extra Jump Upgrade expired.\n";
                break;
            case EffectType::SpeedBoost:
                player.setPlayerSpeed(player.getPlayerSpeed() - (upgradeLevel == 1 ? 2 : upgradeLevel == 2 ? 5 : 10));
                std::cout << "Speed Boost Upgrade expired.\n";
                break;
            }
            upgradeActive = false;
            collected = false;
            // Color based on upgrade effect type
            switch (upgradeEffectType) {
            case EffectType::ExtraJump:
                upgradeShape.setFillColor(sf::Color::Yellow);
                break;

            case EffectType::SpeedBoost:
                upgradeShape.setFillColor(sf::Color::White);
                break;
            }
        }
    }
}

// Helper method to end the upgrade effect
void Upgrades::endUpgradeEffect(Player& player) {
    switch (upgradeEffectType) {
    case EffectType::ExtraJump:
        player.abilities["jumps"] -= (upgradeLevel == 1 ? 1 : (upgradeLevel == 3 ? 2 : 0));
        player.setMaxJumps(player.abilities["jumps"]);

        if (upgradeLevel == 2) {
            player.setJumpStrength(player.getJumpStrength() + 5.0f);
            std::cout << "Jump Strength upgrade expired.\n";
        }
        std::cout << "Extra Jump Upgrade expired.\n";
        break;
    case EffectType::SpeedBoost:
        player.setPlayerSpeed(player.getPlayerSpeed() - (upgradeLevel == 1 ? 2 : upgradeLevel == 2 ? 5 : 10));
        std::cout << "Speed Boost Upgrade expired.\n";
        break;
    }
}

// Method to reset Player's upgrades
void Upgrades::resetPlayerUpgrades(Player& player) {
    // Reset all upgrades for the player
    player.abilities["jumps"] = 2;   // Reset jumps to default
    player.abilities["speed"] = 8.0f; // Reset speed to default
    player.setMaxJumps(player.abilities["jumps"]);
    player.setPlayerSpeed(player.abilities["speed"]);

    // Print debug information to confirm reset
    std::cout << "Player upgrades reset after death. Jumps = " << player.abilities["jumps"]
        << ", Speed = " << player.abilities["speed"] << "\n";
}


// Method to draw the upgrade on the screen
void Upgrades::draw(sf::RenderWindow& window) {
    if (!collected) {
        window.draw(upgradeShape);  // Draw the upgrade if not collected
    }
}

