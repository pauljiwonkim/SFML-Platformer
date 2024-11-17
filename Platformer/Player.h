#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Player {
public:
    Player(float x, float y);
    void processEvents(sf::Keyboard::Key key, bool checkPressed);
    void update();
    void drawTo(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    sf::Vector2f getVelocity() const;

    // Collision detection method
    void handleCollisions(const std::vector<sf::RectangleShape>& platforms);

private:

    //Sprite
    sf::Texture idle_texture;    // Texture for idle
    sf::Texture move_texture;    // Texture for movement (walking)
    sf::Sprite sprite;           // To display the player sprite
    sf::Clock animationClock;    // For animation timing

    int currentFrame;
    int frameWidth, frameHeight;
    const int totalFrames = 6;   // Number of frames in sprite sheet
    void updateTexture();       // Update the texture for walking animation

    //Movement
    bool up, down, left, right;
    bool isGrounded;            // To check if the player is grounded
    bool isJumped;              // To check if the player jumped (for double jump)
    float speed;
    float yVelocity;            // The current vertical velocity
    float xVelocity;            // The current horizontal velocity
    float gravity;              // Gravity strength
    void jump();                // Handle jump
    float jumpStrength;         // Jump strength
    sf::Vector2f previousPosition;  // To calculate velocity for smooth movement

    //Stats
    int health;

    //States
    void death();
};
