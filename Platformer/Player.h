#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Upgrades.h"

class Player {
public:
    // Constructor
    Player(float x, float y);

    // **** Core Player Functionalities ****
    void processEvents(sf::Keyboard::Key key, bool checkPressed);
    void update(std::vector<sf::RectangleShape>& platforms);
    void drawTo(sf::RenderWindow& window);

    // **** Accessors (Getters) ****
    sf::Vector2f getPosition() const { return sprite.getPosition(); }           // returns player's current position
    sf::FloatRect getBounds() const;                                            // Gets player's entire physical bounds 
    sf::FloatRect getSpriteBounds() const { return sprite.getGlobalBounds(); }  // Gets sprite's visual bounds (for rendering-related calcs)
    sf::Vector2f getVelocity() const;                                           // Gets player's current velocity
    bool isPlayerDead() const;                                                  // Check if Player died
    int getMaxJumps() const { return maxJumps; }                                // Gets player's maximum jumps
    int getJumpCount() const { return jumpCount; }                              // Gets player's jump count
    float getPlayerSpeed() const;                                               // Gets player's current speed

    // Mutators (Setters)
    void setMaxJumps(int amount) { maxJumps = amount; }           // Not currently needed but possibly for future use
    void setJumpCount(int count) { jumpCount = count; }
    void setPlayerSpeed(float speed) { abilities["speed"] = speed; } // Update speed in abilities

    // **** Collision and limits****
    void handleCollisions(const std::vector<sf::RectangleShape>& platforms);   // Collision detection
    void belowYLimit();                                                        // Check if Player falls below y limit 

    // **** Jump properties ****
    float jumpStrength;         // Jump strength
    int maxJumps ;              // Player's max amount of jumps. To prevent jumping endlessly
    int jumpCount;              // Tracks consecutive jumps
    bool isGrounded;            // To check if the player is grounded
    bool isJumped;              // To check if the player jumped (for double jump)
    void jump();                // Handle jump

    // **** Player Abilities ****
    std::unordered_map<std::string, int> abilities = { {"jumps", 2}, {"speed", 8.0f} }; // Use a hash map to store player abilities. Default two jumps


private:

    // **** Sprite ****
    sf::Texture idle_texture;    // Texture for idle
    sf::Texture move_texture;    // Texture for movement (walking)
    sf::Sprite sprite;           // To display the player sprite
    sf::Clock animationClock;    // For animation timing
    int currentFrame;
    int frameWidth, frameHeight;
    const int totalFrames = 6;   // Number of frames in sprite sheet
    void updateTexture();       // Update the texture for walking animation

    // **** Movement ****
    bool up, down, left, right;
    float speed;
    float yVelocity;            // The current vertical velocity
    float xVelocity;            // The current horizontal velocity
    float gravity;              // Gravity strength

    sf::Vector2f previousPosition;  // To calculate velocity for smooth movement
    float yLimit = 1000.f;

    //States
    bool isDead;
    void death();
    void respawn();
    sf::Clock deathClock;
    const float respawnTimer = 1.0;


};
#endif // PLAYER_H