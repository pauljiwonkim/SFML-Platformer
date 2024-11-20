#include "Player.h"

// Constructor for the Player class
Player::Player(float x, float y)
    : currentFrame(0), frameWidth(32), frameHeight(32),
    speed(8.0f), yVelocity(0.0f), xVelocity(0.0f), gravity(0.7f), jumpStrength(-15.0f), isGrounded(false), isJumped(false), left(false), right(false), up(false), down(false), isDead(false), maxJumps(2), jumpCount(0) {

    // Load the idle texture (when the player is not moving)
    if (!idle_texture.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/sprites/player_idle.png")) {
        std::cerr << "Error loading idle texture!" << std::endl;
    }

    // Load the moving texture (sprite sheet for walking animation)
    if (!move_texture.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/sprites/player_move.png")) {
        std::cerr << "Error loading walking sprite sheet!" << std::endl;
    }

    // Set the sprite's default texture (idle texture at first)
    sprite.setTexture(idle_texture);

    // Set sprite scale (size 200% of original size)
    sprite.setScale(2.0f, 2.0f);

    // Initial position of the player (positioning player at a specific point on the screen)
    sprite.setPosition(300, 880);

    // Initialize previous position
    previousPosition = sprite.getPosition();

    // Start the animation clock to handle walking animation
    animationClock.restart();

    std::cout << "Player initialized at position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;

    // Initialize abilities
    abilities["jumps"] = 2;         // Set default number of jumps
    maxJumps = abilities["jumps"];  // Dynamically adjust maxJumps based on abilities
    abilities["speed"] = 8.0f;
    speed = abilities["speed"];
}

// Handle key press/release events for player movement
void Player::processEvents(sf::Keyboard::Key key, bool checkPressed) {
    if (checkPressed) {
        if (key == sf::Keyboard::W) { up = true; }
        if (key == sf::Keyboard::S) { down = true; }
        if (key == sf::Keyboard::A) { left = true; }
        if (key == sf::Keyboard::D) { right = true; }

        if (key == sf::Keyboard::Space) {
            if (isGrounded || jumpCount < maxJumps) {
                jump();
            }
        }
    }
    else {
        if (key == sf::Keyboard::W) { up = false; }
        if (key == sf::Keyboard::S) { down = false; }
        if (key == sf::Keyboard::A) { left = false; }
        if (key == sf::Keyboard::D) { right = false; }
    }
}
//----------------------------- UPDATES ---------------------------------------Updates player state (movement, gravity, jumping)

void Player::update(std::vector<sf::RectangleShape>& platforms) {

    isGrounded = false;  // Assume player is not grounded initially

    sf::Vector2f movement(0.0f, 0.0f);
    float speed = abilities["speed"]; 
    // Horizontal movement (left and right)
    if (left && sprite.getPosition().x > 0) {  // Prevent movement past the left edge (x > 0)
        xVelocity = -speed;
    }
    else if (right) {
        xVelocity = speed;
    }
    else {
        xVelocity = 0;  // Stop movement if no keys are pressed
    }


    // Apply gravity
    if (!isGrounded) {
        yVelocity += gravity; // Apply gravity when in the air
    }

    if (yVelocity > 0 && isGrounded) {
        yVelocity = 0;  // Stop vertical movement after landing
    }

    // Move the sprite based on calculated movement (horizontal + vertical)
    sprite.move(sf::Vector2f(xVelocity, yVelocity));
    handleCollisions(platforms);
    // Handle walking animation if the player is moving
    if (left || right) {
        if (animationClock.getElapsedTime().asSeconds() > 0.1f) {
            currentFrame = (currentFrame + 1) % totalFrames;
            updateTexture();
            animationClock.restart();
        }

        // Flip sprite depending on the direction the player is moving
        if (right) {
            sprite.setScale(2.0f, 2.0f);   // Face right
            sprite.setOrigin(0.f, 0.f);
        }
        else if (left) {
            sprite.setScale(-2.0f, 2.0f);  // Face left
            sprite.setOrigin(sprite.getLocalBounds().width, 0.f);
        }
    }
    else {
        // Idle state (do not update animation when idle)
        sprite.setTexture(idle_texture);  // Set the idle texture
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));  // Idle animation
    }

    // Update previous position for velocity calculation
    previousPosition = sprite.getPosition();

    //Check if player goes below y Limit
    belowYLimit();

    //Respawn
    if (isDead) {
        // Check if 3 seconds have passed since the player died
        if (deathClock.getElapsedTime().asSeconds() >= respawnTimer) {
            respawn();  // Call respawn after 3 seconds
        }
        return;  // Skip the rest of the update logic while dead
    }
}

// Update the walking animation texture based on the current frame
void Player::updateTexture() {
    int frameX = currentFrame * frameWidth;
    sprite.setTexture(move_texture);
    sprite.setTextureRect(sf::IntRect(frameX, 0, frameWidth, frameHeight));
}

// General collision handler
void Player::handleCollisions(const std::vector<sf::RectangleShape>& platforms) {
    // Loop through each platform to check for collisions
    for (const auto& platform : platforms) {
        sf::FloatRect platformBounds = platform.getGlobalBounds();
        sf::FloatRect playerBounds = getBounds();

        // Check if the player is colliding with the platform
        if (playerBounds.intersects(platformBounds)) {
            // Determine the side of collision

            // Bottom collision (player landing on top of a platform)
            if (previousPosition.y + playerBounds.height <= platformBounds.top) {
                sprite.setPosition(sprite.getPosition().x, platformBounds.top - playerBounds.height);
                isGrounded = true;
                yVelocity = 0;
                jumpCount = 0;
                std::cout << "Bottom Collided" << std::endl;
            }
            // Top collision (player hitting the underside of a platform)
            else if (previousPosition.y >= platformBounds.top + platformBounds.height) {
                sprite.setPosition(sprite.getPosition().x, platformBounds.top + platformBounds.height);
                yVelocity = 0;  // Stop upward movement
                std::cout << "Top Collided" << std::endl;
            }
            // Left collision
            else if (previousPosition.x + playerBounds.width <= platformBounds.left) {
                sprite.setPosition(platformBounds.left - playerBounds.width, sprite.getPosition().y);
                xVelocity = 0;
                std::cout << "Left Collided" << std::endl;
            }
            // Right collision
            else if (previousPosition.x >= platformBounds.left + platformBounds.width) {
                sprite.setPosition(platformBounds.left + platformBounds.width, sprite.getPosition().y);
                xVelocity = 0;
                std::cout << "Right Collided" << std::endl;
            }
        }
    }
}


// Draw the player sprite to the window
void Player::drawTo(sf::RenderWindow& window) {
    // Draw the player sprite
    window.draw(sprite);
}

// Get the bounding box of the player (used for collision detection)
sf::FloatRect Player::getBounds() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds;
} 

// Get the current velocity of the player (used for physics calculations)
sf::Vector2f Player::getVelocity() const {
    return sprite.getPosition() - previousPosition;
}

float Player::getPlayerSpeed() const {
    if (abilities.find("speed") != abilities.end()) {
        return abilities.at("speed");                   // Return the value of speed from abilities hashmap
    }
    return 8.0f;  // Default speed if not found
}

// Jump action
void Player::jump() {
    if (isGrounded || jumpCount < maxJumps) {
        yVelocity = jumpStrength;
        isGrounded = false;  // Player is no longer grounded
        isJumped = true;     // Enable jumping state
        jumpCount++;         // Increment the jump count
        
        std::cout << "Jumped! Current jump count: " << jumpCount << std::endl;
    }

}

void Player::belowYLimit() {
    if (sprite.getPosition().y > yLimit) {
        death();
    }
}  

bool Player::isPlayerDead() const {
    return isDead;
}
void Player::death() {
    if (!isDead) {
        std::cout << "Player has died" << std::endl;
        isDead = true;
        deathClock.restart();
    }
 }

void Player::respawn() {
    sprite.setPosition(300, 800);
    isDead = false;
    isGrounded = false;
    yVelocity = 0;
    jumpCount = 0;
    std::cout << "Player Respawned" << std::endl;
}

