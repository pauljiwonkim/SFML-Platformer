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
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::FloatRect getBounds() const;
    sf::FloatRect getSpriteBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getVelocity() const;
    bool isPlayerDead() const;
    int getMaxJumps() const { return maxJumps; }
    int getJumpCount() const { return jumpCount; }
    float getJumpStrength() const { return jumpStrength;  }
    float getPlayerSpeed() const;
    float getGravity() const { return gravity; }

    // **** Mutators (Setters) ****
    void setMaxJumps(int amount) { maxJumps = amount; }
    void setJumpCount(int count) { jumpCount = count; }
    void setJumpStrength(float jumpStr) { jumpStrength = jumpStr;  }
    void setPlayerSpeed(float speed) { abilities["speed"] = speed; }
    void setGravity(float newGravity) { gravity = newGravity; }

    // **** Movement and Abilities ****
    void jump();
    void dash();
    void handleCollisions(const std::vector<sf::RectangleShape>& platforms);
    void belowYLimit();

    // Abilities
    std::unordered_map<std::string, float> abilities = { {"jumps", 2.0f}, {"speed", 8.0f} };

private:
    // **** Sprite and Animation ****
    sf::Texture idleTexture;
    sf::Texture moveTexture;
    sf::Sprite sprite;
    sf::Clock animationClock;
    int currentFrame;
    int frameWidth, frameHeight;
    const int totalFrames = 6;
    void updateTexture();

    // **** Movement ****
    bool up = false, down = false, left = false, right = false;
    float yVelocity = 0.0f;
    float xVelocity = 0.0f;
    float gravity = 9.8f;
    sf::Vector2f dashVelocity;
    float speed = 8.0f;
    float dashSpeed = 20.0f;
    bool canDash = true;
    bool isDashing = false;
    sf::Vector2f previousPosition;
    float yLimit = 3500.f;

    // Jump Properties
    int maxJumps = 2;
    int jumpCount = 0;
    bool isGrounded = false;
    bool isJumped = false;
    float jumpStrength;

    // Wall Cling
    bool isClinging = false;
    sf::Clock wallClingClock;
    const float maxClingTime = 5.0f;
    bool clingFallStarted = false;

    // State
    bool isDead;
    void death();
    void respawn();
    sf::Clock deathClock;
    const float respawnTimer = 1.0f;
};

#endif // PLAYER_H
