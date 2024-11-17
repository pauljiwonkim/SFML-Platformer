#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

int main()
{
    // Create the window
    sf::RenderWindow window(sf::VideoMode(900, 1080), "Platformer");
    window.setFramerateLimit(60);

    // Create the player object
    Player player(50, 50);  // Create the player with a starting position (50, 50)

    // Create the ground platform (full-width of the screen, height 50 units)
    sf::RectangleShape ground_platform(sf::Vector2f(900.f, 50.f));
    ground_platform.setFillColor(sf::Color::Blue);  // Set the ground platform color
    ground_platform.setPosition(0.f, 950.f);  // Position it at the bottom of the screen

    // Create a testing platform1 (600x30 size)
    sf::RectangleShape platform1(sf::Vector2f(600.f, 30.f));
    platform1.setFillColor(sf::Color::Green);  // Set the platform color
    platform1.setPosition(450.f, 500.f);  // Position it higher on the screen for testing

    // Create a testing platform2 (400x30 size)
    sf::RectangleShape platform2(sf::Vector2f(400.f, 30.f));
    platform2.setFillColor(sf::Color::Green);  // Set the platform color
    platform2.setPosition(250.f, 700.f);  // Position it higher on the screen for testing

    // Vector of platforms
    std::vector<sf::RectangleShape> platforms = { ground_platform, platform1, platform2 };

    // Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) // Handle user input
        {
            if (event.type == sf::Event::Closed) // Close window if user clicks the close button
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) // Handle key press
            {
                player.processEvents(event.key.code, true);
            }
            else if (event.type == sf::Event::KeyReleased) // Handle key release
            {
                player.processEvents(event.key.code, false);
            }
        }

        // Update the player and apply gravity
        player.update();

        // Handle collisions (pass the platforms to the player's collision handler)
        player.handleCollisions(platforms);

        // Draw everything
        window.clear();  // Clear the window before drawing

        // Draw platforms and their outlines
        for (const auto& platform : platforms) {
            window.draw(platform);  // Draw the actual platform

            // Draw the bounding box outline of the platform
            sf::RectangleShape platformOutline(platform.getSize());
            platformOutline.setPosition(platform.getPosition());
            platformOutline.setFillColor(sf::Color::Transparent);   // Make the box transparent
            platformOutline.setOutlineColor(sf::Color::White);       // White outline color for the platform
            platformOutline.setOutlineThickness(2.0f);              // Set outline thickness

            window.draw(platformOutline);
        }

        // Draw the player and its outline
        player.drawTo(window);


        // Draw player outline
        sf::RectangleShape playerOutline(sf::Vector2f(player.getBounds().width, player.getBounds().height));
        playerOutline.setPosition(player.getBounds().left, player.getBounds().top);
        playerOutline.setFillColor(sf::Color::Transparent);
        playerOutline.setOutlineColor(sf::Color::Red);  // Red outline color for the player
        playerOutline.setOutlineThickness(1.0f);
        window.draw(playerOutline);

        window.display();  // Display the updated frame
    }

    return 0;
}
