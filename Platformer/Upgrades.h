#ifndef UPGRADES_H
#define UPGRADES_H

// Includes necessary SFML graphics and standard libraries
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "Player.h"

// Forward declaration to avoid circular dependencies with Player class
class Player;

// Enum to define the types of upgrades
enum class EffectType {
    None,       // No effect
    SpeedBoost, // Speed boost upgrade
    ExtraJump   // Extra jump upgrade
};

// Class definition for Upgrades
class Upgrades {                                            
public:
    // Static map storing all available upgrades
    static std::unordered_map<std::string, Upgrades> upgrade_map;

    // Constructors
    Upgrades() // Default Constructor : Initialization List
        : upgradeName(""),
        upgradeShape(),
        upgradeSize(20.f, 20.f),
        upgradeLevel(0),
        upgradeDuration(0.f),
        upgradePosition(0.f, 0.f),
        upgradeColor(sf::Color::Yellow),
        collected(false),
        upgradeActive(false),
        isRespawning(false),
        respawnDelay(10.0f),
        upgradeEffectType(EffectType::None)
    {
        upgradeShape.setSize(upgradeSize);
        upgradeShape.setFillColor(upgradeColor);
        upgradeShape.setPosition(upgradePosition);
    }

    // Parameterized Constructor
    Upgrades(const std::string& name, sf::RectangleShape shape, const sf::Vector2f& size,
        int level, float duration, const sf::Vector2f& position, const sf::Color& color,
        bool collected, bool upgradeActive, EffectType effectType);

       

    // Method to apply the upgrade's effect to the player
    void applyUpgradeEffect(Player& player);

    // Method to draw the upgrade on the window
    void draw(sf::RenderWindow& window);

    // Methods for specific upgrade actions
    void upgradeJump(Player& player);
    void upgradeSpeed(Player& player);
    void updateEffectDuration(Player& player);

    // Static method to initialize all upgrades
    static void initializeUpgrades();

    // Static method to reset all upgrades
    static void resetPlayerUpgrades(Player& player);

    // **** Accessors (Getters) ****

    const std::string& getUpgradeName() const { return upgradeName; }                     // Getter for upgradeName
    int getUpgradeLevel() const { return upgradeLevel; }                                  // Getter for upgradeLevel
    float getUpgradeDuration() const { return upgradeDuration; }                          // Getter for upgradeDuration
    sf::RectangleShape getUpgradeShape() const { return upgradeShape; }                   // Getter for upgradeShape
    const sf::Vector2f& getUpgradePosition() const { return upgradePosition; }            // Getter for upgradePosition
    const sf::Color& getUpgradeColor() const { return upgradeColor; }                     // Getter for upgradeColor
    bool isCollected() const { return collected; }                                        // Getter for collected
    bool isUpgradeActive() const { return upgradeActive; }                                // Getter for isUpgradeActive
    EffectType getUpgradeEffectType() const { return upgradeEffectType; }                 // Getter for upgradeEffectType
    sf::Clock getDurationTimer() const { return durationTimer; }                          // Getter for durationTimer
     
    // **** Mutators (Setters) ****
    void setUpgradeName(const std::string& name) { upgradeName = name; }                  // Setter for upgradeName
    void setUpgradeLevel(int level) { upgradeLevel = level; }                             // Setter for upgradeLevel
    void setUpgradeDuration(float duration) { upgradeDuration = duration; }               // Setter for upgradeDuration
    void setUpgradeShape(const sf::RectangleShape& shape) { upgradeShape = shape; }       // Setter for upgradeShape
    void setUpgradePosition(const sf::Vector2f& position) { upgradePosition = position; } // Setter for upgradePosition
    void setUpgradeColor(const sf::Color& color) { upgradeColor = color; }                // Setter for upgradeColor
    void setCollected(bool status) { collected = status; }                                // Setter for collected
    void setUpgradeActive(bool status) { upgradeActive = status; }                        // Setter for isUpgradeActive 
    void setUpgradeEffectType(EffectType effectType) { upgradeEffectType = effectType; }  // Setter for upgradeEffectType

private:
    std::string upgradeName;            // Name of the upgrade
    int upgradeLevel;                   // Level of the upgrade
    float upgradeDuration;              // Duration of the upgrade effect
    sf::RectangleShape upgradeShape;    // Shape to represent upgrade visually
    sf::Vector2f upgradeSize;           // Size of Upgrade shape
    sf::Vector2f upgradePosition;       // Position of the upgrade on the screen
    sf::Color upgradeColor;             // Color of the upgrade
    bool collected;                     // Whether the upgrade has been collected
    EffectType upgradeEffectType;       // The effect type of the upgrade
    bool upgradeActive;                 // Whether the upgrade effect is active
    sf::Clock durationTimer;            // Timer to track the upgrade effect duration
    bool isRespawning;                  // Whether the upgrade is currently in respawn mode
    float respawnDelay;                 // Delay in seconds for respawn
    sf::Clock respawnTimer;             // Timer to track the upgrade respawn delay

    void endUpgradeEffect(Player& player);
};

// End of header guard
#endif // UPGRADES_H
