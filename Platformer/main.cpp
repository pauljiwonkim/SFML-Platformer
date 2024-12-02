#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "Player.h"
#include "Camera.h"
#include "Upgrades.h"
#include "TitleScreen.h"
#include "ShapeManager.h"
#include "LevelManager.h"


int main()
{
    // Create the window with 1920x1080 size
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Platformer");
    window.setFramerateLimit(60);

    bool startGame = false;

    // Initialize the title screen
    TitleScreen::InitializeTexts(window);  // Call to initialize texts and buttons (title and play button)

    // Temporary background using RectangleShape instead of Sprite
    sf::Texture backgroundTexture;
    bool backgroundOn = true;  // Flag to control background display
    if (!backgroundTexture.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/sprites/background.png")) {
        std::cerr << "Error loading background image" << std::endl;
        return -1;
    }
    else {
        std::cout << "Background image loaded successfully!" << std::endl;
    }

    // Create a rectangle for the background, filling the entire window and applying the texture
    sf::RectangleShape backgroundRect(sf::Vector2f(window.getSize().x, window.getSize().y));
    backgroundRect.setTexture(&backgroundTexture);

    // Load font before initializing texts
    TitleScreen::loadFont("E:/Code Projects/Personal Projects/Platformer/platformer/src/font/Roboto-Regular.ttf");

    // Initialize player, camera, and platforms
    Player player(50, 50);
    Camera camera(window.getSize().x, window.getSize().y, player);
    camera.setBounds(0.f, 0.f, 4000.f, 4000.f);  // Adjusted bounds for 1920x1080
    float viewHeight = window.getSize().y;

    Upgrades::initializeUpgrades();
    std::unordered_map<std::string, Upgrades>& upgrades = Upgrades::upgrade_map; // Map for upgrade objects
    std::unordered_map<std::string, TitleScreen>& texts = TitleScreen::texts_map;  // Map for text objects
    std::unordered_map<std::string, TitleScreen>& buttons = TitleScreen::buttons_map; // Map for button objects

    LevelManager levelManager;
    std::unordered_map<std::string, LevelManager::LevelData>& levels = LevelManager::levels;



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
                backgroundRect.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

                // Set background to static position relative to window, not camera
                backgroundRect.setPosition(0, 0);  // Fix the background at the top-left corner of the window

                // Draw the background (fixed relative to window)
                window.draw(backgroundRect);
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
            const LevelManager::LevelData& levelData = levels["level1"];

            player.update(levels);
            camera.update(player);
            player.handleCollisions(levels);

            for (auto& upgrade : upgrades) {
                upgrade.second.applyUpgradeEffect(player);  // Check and apply the effect
            }

            // Draw the background with the default view (not affected by the camera)
            if (backgroundOn) {
                // Temporarily reset the view to the default to draw a static background
                window.setView(window.getDefaultView());

                // Set the background size to match the window and position it at (0,0)
                backgroundRect.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
                backgroundRect.setPosition(0, 0);

                // Draw the background
                window.draw(backgroundRect);

                // Reset to camera view after drawing the background
                window.setView(camera.getView());
            }

            // Apply camera to window
            camera.apply(window);

            // Draw platforms
            for (const auto& pair : levels) {
                const LevelManager::LevelData& level = pair.second;
                levelManager.drawLevel("level1", window);
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
            // Show "You Died" message when player dies
            if (player.isPlayerDead()) {
                sf::Text deathMsg;
                deathMsg.setFont(TitleScreen::getFont());
                deathMsg.setString("You Died");
                deathMsg.setCharacterSize(50);
                deathMsg.setFillColor(sf::Color::Red);

                // Get the center of the camera view
                sf::Vector2f viewCenter = camera.getView().getCenter();

                // Center the text on the camera's view
                sf::FloatRect textRect = deathMsg.getLocalBounds();
                deathMsg.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                deathMsg.setPosition(viewCenter);

                window.draw(deathMsg);
            }


            
        }

        window.display();
    }

    return 0;
}
