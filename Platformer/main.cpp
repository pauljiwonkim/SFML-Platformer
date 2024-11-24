#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "Player.h"
#include "Camera.h"
#include "Upgrades.h"
#include "TitleScreen.h"

int main()
{
    // Create the window with 1920x1080 size
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Platformer");
    window.setFramerateLimit(60);

    bool startGame = false;

    // Initialize the title screen
    TitleScreen::InitializeTexts(window);  // Call to initialize texts and buttons (title and play button)

    // Temporary background
    sf::Texture backgroundTexture;
    bool backgroundOn = true;  // Flag to control background display
    if (!backgroundTexture.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/sprites/background.png")) {
        std::cerr << "Error loading background image" << std::endl;
        return -1;
    }
    else {
        std::cout << "Background image loaded successfully!" << std::endl;
    }
    // Create a sprite for the background
    sf::Sprite backgroundSprite(backgroundTexture);

    // Load font before initializing texts
    TitleScreen::loadFont("E:/Code Projects/Personal Projects/Platformer/platformer/src/font/Roboto-Regular.ttf");

    // Initialize player, camera, and platforms
    Player player(50, 50);
    Camera camera(window.getSize().x, window.getSize().y, player);
    camera.setBounds(0.f, 0.f, 4000.f, 4000.f);  // Adjusted bounds for 1920x1080
    float viewHeight = window.getSize().y;

    // Scaling factors for 1920x1080 (compared to the original 1600x1050)
    float scaleX = 1920.f / 1600.f;
    float scaleY = 1080.f / 1050.f;

    // Adjust platforms and upgrades for new window size
    sf::RectangleShape ground_platform(sf::Vector2f(1600.f * scaleX, 400.f * scaleY));
    ground_platform.setFillColor(sf::Color::Blue);
    ground_platform.setPosition(0.f, 3100.f * scaleY);

    sf::RectangleShape platform1(sf::Vector2f(600.f * scaleX, 30.f * scaleY));
    platform1.setFillColor(sf::Color::Yellow);
    platform1.setPosition(300.f * scaleX, 2800.f * scaleY);

    sf::RectangleShape platform2(sf::Vector2f(400.f * scaleX, 30.f * scaleY));
    platform2.setFillColor(sf::Color::Green);
    platform2.setPosition(500.f * scaleX, 2600.f * scaleY);

    std::vector<sf::RectangleShape> platforms = { ground_platform, platform1, platform2 };

    Upgrades::initializeUpgrades();
    std::unordered_map<std::string, Upgrades>& upgrades = Upgrades::upgrade_map; // Map for upgrade objects
    std::unordered_map<std::string, TitleScreen>& texts = TitleScreen::texts_map;  // Map for text objects
    std::unordered_map<std::string, TitleScreen>& buttons = TitleScreen::buttons_map; // Map for button objects

    // Game loop
    while (window.isOpen())
    {
        sf::Event event;
        // Get mouse position relative to the window
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && !startGame) {
                if (TitleScreen::buttons_map["play_button"].isButtonPressed(mousePos)) {  // Use the button map
                    startGame = true;
                    std::cout << "Game Started!" << std::endl;  // Debugging
                }
                else if (TitleScreen::buttons_map["exit_button"].isButtonPressed(mousePos)) {
                    exit(0);
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                player.processEvents(event.key.code, true);
            }
            else if (event.type == sf::Event::KeyReleased) {
                player.processEvents(event.key.code, false);
            }
        }

        window.clear();

        if (!startGame) {
            // Draw the background first to ensure it's behind other elements
            if (backgroundOn) {
                // Stretch background to fit the entire window
                backgroundSprite.setScale(
                    window.getSize().x / static_cast<float>(backgroundTexture.getSize().x),
                    window.getSize().y / static_cast<float>(backgroundTexture.getSize().y)
                );

                // Set background to static position relative to window, not camera
                backgroundSprite.setPosition(0, 0);  // Fix the background at the top-left corner of the window

                // Draw the background (fixed relative to window)
                window.draw(backgroundSprite);
            }

            // Now, draw the title screen elements (text and buttons)
            for (auto& text : texts) {
                text.second.drawText(window);
            }

            for (auto& button : buttons) {
                button.second.drawButton(window);  // Draw buttons (like play button)
            }

            // Check hover status for buttons
            for (auto& button : TitleScreen::buttons_map) {
                button.second.checkHover(mousePos);
            }
        }
        else {
            // Game has started, update player and camera
            player.update(platforms);
            camera.update(player);
            player.handleCollisions(platforms);

            for (auto& upgrade : upgrades) {
                upgrade.second.applyUpgradeEffect(player);  // Check and apply the effect
            }

            // Draw background if enabled
            if (backgroundOn) {
                sf::Vector2f cameraPosition = camera.getCameraPosition();

                // Set the background position relative to the camera
                backgroundSprite.setPosition(cameraPosition.x - window.getSize().x / 2,
                    cameraPosition.y - window.getSize().y / 2);

                // Scale the background to cover the entire window size
                backgroundSprite.setScale(
                    window.getSize().x / static_cast<float>(backgroundTexture.getSize().x),
                    window.getSize().y / static_cast<float>(backgroundTexture.getSize().y)
                );

                // Draw the background
                window.draw(backgroundSprite);
            }

            // Draw platforms (scaled)
            for (const auto& platform : platforms) {
                window.draw(platform);
                sf::RectangleShape platformOutline(platform.getSize());
                platformOutline.setPosition(platform.getPosition());
                platformOutline.setFillColor(sf::Color::Transparent);
                platformOutline.setOutlineColor(sf::Color::White);
                platformOutline.setOutlineThickness(2.0f);
                window.draw(platformOutline);
            }

            // Draw upgrades
            for (auto& upgrade : upgrades) {
                if (!upgrade.second.isCollected()) {
                    window.draw(upgrade.second.getUpgradeShape());  // Access the 'Upgrades' object using .second
                }
            }

            // Draw player
            player.drawTo(window);
            sf::RectangleShape playerOutline(sf::Vector2f(player.getBounds().width, player.getBounds().height));
            playerOutline.setPosition(player.getBounds().left, player.getBounds().top);
            playerOutline.setFillColor(sf::Color::Transparent);
            playerOutline.setOutlineColor(sf::Color::Red);
            playerOutline.setOutlineThickness(1.0f);
            window.draw(playerOutline);

            // Show "You Died" message when player dies
            if (player.isPlayerDead()) {
                sf::Text deathMsg;
                deathMsg.setFont(TitleScreen::getFont());
                deathMsg.setString("You Died");
                deathMsg.setCharacterSize(50);
                deathMsg.setFillColor(sf::Color::Red);
                deathMsg.setPosition(player.getPosition().x, player.getPosition().y);

                window.draw(deathMsg);
            }

            // Apply camera to window
            camera.apply(window);
        }

        window.display();
    }

    return 0;
}
