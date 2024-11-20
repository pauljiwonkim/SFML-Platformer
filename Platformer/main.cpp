#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#include "Player.h"
#include "Camera.h"
#include "Upgrades.h"

int main()
{
    // Create the window
    sf::RenderWindow window(sf::VideoMode(900, 1080), "Platformer");
    window.setFramerateLimit(60);

    // ****Temporary Background *****
    sf::Texture backgroundTexture;
    bool backgroundOn = false;
    if (!backgroundTexture.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/sprites/background.png")) {
        std::cerr << "Error loading background image" << std::endl;
        return -1;
    }
    sf::RectangleShape background(sf::Vector2f(2000, 2000));
    background.setTexture(&backgroundTexture);

    // Create the player object
    Player player(50, 50);

    // Create the camera
    Camera camera(window.getSize().x, window.getSize().y, player);

    // Set camera bounds (you can adjust these based on your level)
    camera.setBounds(400.f, 0.f, 2000.f, 600.f);

    // Create platforms
    sf::RectangleShape ground_platform(sf::Vector2f(900.f, 50.f));
    ground_platform.setFillColor(sf::Color::Blue);
    ground_platform.setPosition(0.f, 950.f);

    sf::RectangleShape platform1(sf::Vector2f(600.f, 30.f));
    platform1.setFillColor(sf::Color::Yellow);
    platform1.setPosition(250.f, 300.f);

    sf::RectangleShape platform2(sf::Vector2f(400.f, 30.f));
    platform2.setFillColor(sf::Color::Green);
    platform2.setPosition(350.f, 500.f);

    sf::RectangleShape platform3(sf::Vector2f(400.f, 30.f));
    platform3.setFillColor(sf::Color::Red);
    platform3.setPosition(550.f, 700.f);

    std::vector<sf::RectangleShape> platforms = { ground_platform, platform1, platform2, platform3 };
    
    // Extra Jump Upgrades vector
    std::vector<Upgrades> upgrades;
    upgrades.push_back(Upgrades("extraJump", 450.f, 400.f ));  //Create the extra jump upgrade (type, x, y)
    upgrades.push_back(Upgrades("speedBoost", 650.f, 800.f));  //Create the speed boost upgrade
    upgrades.push_back(Upgrades("speedBoost", 850.f, 600.f));

    // Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                player.processEvents(event.key.code, true);
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                player.processEvents(event.key.code, false);
            }
        }

        // Update the player and apply gravity
        player.update(platforms);

        // Update the camera
        camera.update(player);

        // Handle collisions
        player.handleCollisions(platforms);

        // Check if the player collects any upgrade
        for (auto& upgrade : upgrades) {
            upgrade.applyUpgradeEffect(player);  // Check and apply the effect
        }

        // Draw everything
        window.clear();

        if (backgroundOn) {
            window.draw(background);
        }

        // Draw platforms
        for (const auto& platform : platforms) {
            window.draw(platform);

            // Draw platform outline
            sf::RectangleShape platformOutline(platform.getSize());
            platformOutline.setPosition(platform.getPosition());
            platformOutline.setFillColor(sf::Color::Transparent);
            platformOutline.setOutlineColor(sf::Color::White);
            platformOutline.setOutlineThickness(2.0f);
            window.draw(platformOutline);
        }

        // Draw each upgrade if not collected
        for (auto& upgrade : upgrades) {
            if (!upgrade.collected) {
                window.draw(upgrade.shape);
            }
        }
        
        // Draw the player and outline
        player.drawTo(window);
        sf::RectangleShape playerOutline(sf::Vector2f(player.getBounds().width, player.getBounds().height));
        playerOutline.setPosition(player.getBounds().left, player.getBounds().top);
        playerOutline.setFillColor(sf::Color::Transparent);
        playerOutline.setOutlineColor(sf::Color::Red);
        playerOutline.setOutlineThickness(1.0f);
        window.draw(playerOutline);

        // Apply camera view
        camera.apply(window);

        window.display();
    }

    return 0;
}
