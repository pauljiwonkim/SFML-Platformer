#include "Player.h"

// Constructor
Player::Player(float x, float y)
    : currentFrame(0), frameWidth(32), frameHeight(32), yVelocity(0), xVelocity(0), left(false), right(false), up(false), down(false), isDead(false), jumpStrength(-15.0f), jumpCount(0), gravity(0.7f), dashSpeed(20.0f), yLimit(3500)
{
    // Load the idle texture (when the player is not moving)
    if (!idleTexture.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/sprites/player_idle.png")) {
        std::cerr << "Error loading idle texture!" << std::endl;
    }

    // Load the moving texture (sprite sheet for walking animation)
    if (!moveTexture.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/sprites/player_move.png")) {
        std::cerr << "Error loading walking sprite sheet!" << std::endl;
    }

    // Set the sprite's default texture (idle texture at first)
    sprite.setTexture(idleTexture);

    // Set sprite scale (size 200% of original size)
    sprite.setScale(2.0f, 2.0f);

    // Initial position of the player (positioning player at a specific point on the screen)
    sprite.setPosition(50, 3000);

    // Initialize previous position
    previousPosition = sprite.getPosition();

    // Initialize abilities
    abilities["jumps"] = 2;         // Set default number of jumps
    maxJumps = abilities["jumps"];  // Dynamically adjust maxJumps based on abilities
    abilities["speed"] = 8.0f;
    speed = abilities["speed"];

    // Start the animation clock to handle walking animation
    animationClock.restart();

    std::cout << "Player initialized at position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;

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

        if (key == sf::Keyboard::E) {
            isClinging = true;  // Start wall cling when E is pressed
            std::cout << "Wall Cling Activated" << std::endl;  // Debugging output
        }
    }
    else {
        if (key == sf::Keyboard::W) { up = false; }
        if (key == sf::Keyboard::S) { down = false; }
        if (key == sf::Keyboard::A) { left = false; }
        if (key == sf::Keyboard::D) { right = false; }

        if (key == sf::Keyboard::E) {
            isClinging = false;  // Stop wall cling when E is released
            std::cout << "Wall Cling Deactivated" << std::endl;  // Debugging output
        }
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
    if (!isGrounded) {  // Apply gravity unless clinging
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
        sprite.setTexture(idleTexture);  // Set the idle texture
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
    sprite.setTexture(moveTexture);
    sprite.setTextureRect(sf::IntRect(frameX, 0, frameWidth, frameHeight));
}

// General Collision Handler
void Player::handleCollisions(const std::vector<sf::RectangleShape>& platforms) {
    bool isCollidingTop = false;
    bool isCollidingBottom = false;
    bool isCollidingLeft = false;
    bool isCollidingRight = false;

    for (const auto& platform : platforms) {
        sf::FloatRect platformBounds = platform.getGlobalBounds();
        sf::FloatRect playerBounds = getBounds();

        if (playerBounds.intersects(platformBounds)) {
            // Handle wall cling first
            if (isClinging) {
                yVelocity = 0;  // Stop vertical movement while clinging
                wallClingClock.restart();
                std::cout << "Clinging to the wall!" << std::endl;
            }

            // Vertical collisions (top and bottom)
            if (previousPosition.y + playerBounds.height <= platformBounds.top) {
                // Bottom collision: player landed on the platform
                sprite.setPosition(sprite.getPosition().x, platformBounds.top - playerBounds.height);
                yVelocity = 0;
                isGrounded = true;
                jumpCount = 0;  // Reset jump count when grounded
                isCollidingBottom = true;
            }
            else if (previousPosition.y >= platformBounds.top + platformBounds.height) {
                // Top collision: player hitting the underside of a platform
                sprite.setPosition(sprite.getPosition().x, platformBounds.top + platformBounds.height);
                yVelocity = 0;
                isCollidingTop = true;
            }

            // Horizontal collisions (left and right)
            if (previousPosition.x + playerBounds.width <= platformBounds.left) {
                // Left collision: player hit the left side of the platform
                sprite.setPosition(platformBounds.left - playerBounds.width, sprite.getPosition().y);
                xVelocity = 0;
                isCollidingLeft = true;
            }
            else if (previousPosition.x >= platformBounds.left + platformBounds.width) {
                // Right collision: player hit the right side of the platform
                sprite.setPosition(platformBounds.left + platformBounds.width, sprite.getPosition().y);
                xVelocity = 0;
                isCollidingRight = true;
            }
        }
    }

    // After collisions, handle specific responses based on which side the player collided with. Future use
    if (isCollidingTop) {
        // Additional top collision logic (e.g., prevent falling through platforms)
    }
    if (isCollidingBottom) {
        // Handle grounded logic (e.g., prevent falling through and reset jump count)
    }
    if (isCollidingLeft) {
        // Handle left collision logic (e.g., stop horizontal movement to the left)
    }
    if (isCollidingRight) {
        // Handle right collision logic (e.g., stop horizontal movement to the right)
    }
}

// Jump Action
void Player::jump() {
    if ((isGrounded || jumpCount < maxJumps)) {
        yVelocity = jumpStrength;
        isGrounded = false;  // Player is no longer grounded
        isJumped = true;     // Enable jumping state
        jumpCount++;         // Increment the jump count

        std::cout << "Jumped! Current jump count: " << jumpCount << std::endl;
    }

}

// Dash Action
void Player::dash() {
    if (canDash) {
        dashVelocity = sf::Vector2f(xVelocity, 0) * dashSpeed;
        isDashing = true;
        canDash = false;
    }
}

void Player::belowYLimit() {
    if (sprite.getPosition().y > yLimit) {
        death();
    }
}

void Player::death() {
    if (!isDead) {
        std::cout << "Player has died" << std::endl;
        isDead = true;
        deathClock.restart();
        jumpCount = 0;
    }
}

bool Player::isPlayerDead() const {
    return isDead;
}

void Player::respawn() {
    sprite.setPosition(50, 3000);
    isDead = false;
    isGrounded = false;
    yVelocity = 0;
    std::cout << "Player Respawned" << std::endl;
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

// Draw the player sprite to the window
void Player::drawTo(sf::RenderWindow& window) {
    // Draw the player sprite
    window.draw(sprite);
}

