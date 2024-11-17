#include "Player.h"
//test comment
// Constructor for the Player class
Player::Player(float x, float y)
    : currentFrame(0), frameWidth(32), frameHeight(32),
    speed(5.0f), yVelocity(0.0f), xVelocity(0.0f), gravity(0.5f), jumpStrength(-15.0f), isGrounded(false), isJumped(false), left(false), right(false), up(false), down(false), health(10) {

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
    sprite.setPosition(x, y);

    // Initialize previous position
    previousPosition = sprite.getPosition();

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
            if (isGrounded) {
                jump();
            }
            else if (isJumped) {
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

void Player::update() {
    isGrounded = false;  // Assume player is not grounded initially

    sf::Vector2f movement(0.0f, 0.0f);

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

}

// Update the walking animation texture based on the current frame
void Player::updateTexture() {
    int frameX = currentFrame * frameWidth;
    sprite.setTexture(move_texture);
    sprite.setTextureRect(sf::IntRect(frameX, 0, frameWidth, frameHeight));
}

// General collision handler
void Player::handleCollisions(const std::vector<sf::RectangleShape>& platforms) {

    // Iterate over platforms
    for (const auto& platform : platforms) {
        // Vertical Collision
        if (getBounds().intersects(platform.getGlobalBounds())) {
            std::cout << "Vertical collision" << std::endl;

            // Stop vertical movement (colliding from above)
            if (yVelocity > 0) {  // Falling down or moving downward
                sprite.setPosition(sprite.getPosition().x, platform.getPosition().y - sprite.getGlobalBounds().height);
                yVelocity = 0;  // Stop downward movement after landing
                isGrounded = true;  // The player is now grounded
                isJumped = false; // Reset double jump
            }
            // Stop vertical movement (colliding from below)
            else if (yVelocity < 0) {  // Moving upward (jumping)
                sprite.setPosition(sprite.getPosition().x, platform.getPosition().y + platform.getSize().y);
                yVelocity = 0;  // Stop upward movement when hitting the platform from below
            }
        }

        // Horizontal Collision
        if (getBounds().intersects(platform.getGlobalBounds())) {
            std::cout << "Horizontal collision" << std::endl;
            // Stop horizontal movement (colliding from the right)
            if (xVelocity > 0) {  // Moving right
                sprite.setPosition(platform.getPosition().x - sprite.getGlobalBounds().width, sprite.getPosition().y);
                xVelocity = 0;  // Stop rightward movement after collision
            }
            // Stop horizontal movement (colliding from the left)
            else if (xVelocity < 0) {  // Moving left
                sprite.setPosition(platform.getPosition().x + platform.getSize().x, sprite.getPosition().y);
                xVelocity = 0;  // Stop leftward movement after collision
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

    // Define a custom width adjustment (e.g., 10 pixels on each side)
    float widthAdjustment = 10.0f;

    // Offset the bounds by reducing width and adjusting position
    bounds.left += widthAdjustment;
    bounds.width -= 2 * widthAdjustment;  // Subtract from both sides

    return bounds;
}

// Get the current velocity of the player (used for physics calculations)
sf::Vector2f Player::getVelocity() const {
    return sprite.getPosition() - previousPosition;
}

// Jump action
void Player::jump() {
    if (isGrounded) {
        yVelocity = jumpStrength;
        isGrounded = false;  // Player is no longer on the ground
        isJumped = true;     // Allow for a potential double jump
        //       if (health > 0) {   // Ensure health doesn't go negative
        //           health--;        // Decrease health by 1 on each jump
        //           std::cout << "Health: " << health << std::endl;  // Output health to console
        //       }
    }
    else if (isJumped) {
        yVelocity = jumpStrength;
        isJumped = false;    // Disable double jump after use
        //       if (health > 0) {   // Ensure health doesn't go negative
        //           health--;        // Decrease health by 1 on each jump
        //           std::cout << "Health: " << health << std::endl;  // Output health to console
        //       }
    }
}

void Player::death() {
    if (health == 0) {

        std::cout << "Game over" << std::endl;
    }
}