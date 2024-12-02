#include "TitleScreen.h"
#include <iostream>

std::unordered_map<std::string, TitleScreen> TitleScreen::texts_map;
std::unordered_map<std::string, TitleScreen> TitleScreen::buttons_map;
sf::Font TitleScreen::textFont;

// Static method to load the font
void TitleScreen::loadFont(const std::string& fontPath) {
    if (!textFont.loadFromFile(fontPath)) {
        std::cerr << "Error loading font from path: " << fontPath << std::endl;
        exit(0);
    }
}
// Constructor for Text
TitleScreen::TitleScreen(const std::string& labelText, int fontSize,
    const sf::Color& fontColor, const sf::Vector2f& textPosition)
    : labelText(labelText),
    fontSize(fontSize),
    fontColor(fontColor),
    textPosition(textPosition)
{
    if (!textFont.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/font/Roboto-Regular.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        exit(0); 
    }
    titleLabel.setFont(textFont);
    titleLabel.setString(labelText);
    titleLabel.setCharacterSize(fontSize);
    titleLabel.setFillColor(fontColor);
    // Get the bounds of the title label text
    sf::FloatRect textBounds = titleLabel.getLocalBounds();

    // Calculate the position of the text to be centered horizontally
    float centerX = textPosition.x - textBounds.width / 2.f;  // Center the text based on its width

    // Set the new position of the text
    titleLabel.setPosition(centerX, textPosition.y);

}

// Constructor for Button
TitleScreen::TitleScreen(const std::string& buttonText, int fontSize,
    const sf::Color& fontColor, sf::RectangleShape shape, const sf::Vector2f& buttonPosition,
    const sf::Vector2f& buttonSize, const sf::Color& buttonColor, const sf::Color& hoveredColor, const bool buttonPressed)
    : buttonText(buttonText),
    fontSize(fontSize),
    fontColor(fontColor),
    buttonPosition(buttonPosition),
    buttonSize(buttonSize),
    hoveredColor(hoveredColor),
    buttonColor(buttonColor),
    buttonPressed(false) 
{
    if (!buttonFont.loadFromFile("E:/Code Projects/Personal Projects/Platformer/platformer/src/font/Roboto-Regular.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        exit(0); 
    }
    buttonShape.setSize(buttonSize);
    buttonShape.setPosition(buttonPosition);
    buttonShape.setFillColor(buttonColor);

    buttonLabel.setFont(textFont);
    buttonLabel.setString(buttonText);
    buttonLabel.setCharacterSize(fontSize);
    buttonLabel.setFillColor(fontColor);

    // Center the button label within the button
    sf::FloatRect textBounds = buttonLabel.getLocalBounds(); // Get the bounds of the text
    buttonLabel.setPosition(
        buttonPosition.x + (buttonSize.x - textBounds.width) / 2, // Horizontal centering
        (buttonPosition.y + (buttonSize.y - textBounds.height) / 2) - 12 // Vertical centering (-12 for proper alignment)
    );
}

// Draw Text 
void TitleScreen::drawText(sf::RenderWindow& window) {
    window.draw(titleLabel);
    //  std::cout << "Drawing text: " << titleLabel.getString().toAnsiString() << std::endl;
}

// Draw Button 
void TitleScreen::drawButton(sf::RenderWindow& window) {
    window.draw(buttonShape);
    window.draw(buttonLabel);

}

// Check if button is being hovered (For color change effect)
void TitleScreen::checkHover(const sf::Vector2f& mousePos) {
    if (buttonShape.getGlobalBounds().contains(mousePos)) {
        buttonShape.setFillColor(hoveredColor); // Change to hovered color
    }
    else {
        buttonShape.setFillColor(buttonColor); // Reset to original color
    }
}

// Check if button was pressed
bool TitleScreen::isButtonPressed(const sf::Vector2f& mousePos) {
    return buttonShape.getGlobalBounds().contains(mousePos);
}

// Initialize texts and buttons (Static Method)
void TitleScreen::InitializeTexts(sf::RenderWindow& window) {
    // Shapes for the buttons
    sf::RectangleShape playButtonShape(sf::Vector2f(300.f, 100.f)); // Play button shape
    sf::RectangleShape exitButtonShape(sf::Vector2f(300.f, 100.f)); // Exit button shape

    // Helpers for calculating button positions
    float buttonPadding = 50.f;  // Padding between buttons
    float playButtonWidth = playButtonShape.getSize().x;
    float exitButtonWidth = exitButtonShape.getSize().x;

    // Calculate the total width required for both buttons and the padding
    float totalWidth = playButtonWidth + exitButtonWidth + buttonPadding;
    float centerX = window.getSize().x / 2.f;  // Get the X center of the window
    float startX = centerX - totalWidth / 2.f;  // Starting X position to center the buttons

    // Adjust vertical position of buttons
    float bottomPadding = 150.f;  // Add padding from the bottom
    sf::Vector2f playButtonPos(startX, window.getSize().y - playButtonShape.getSize().y - bottomPadding);  // Adjust Y position
    sf::Vector2f exitButtonPos(startX + playButtonWidth + buttonPadding, window.getSize().y - exitButtonShape.getSize().y - bottomPadding);  // Adjust Y position for Exit button

    // Create the title text (you can adjust size/color as needed)
    TitleScreen title("Welcome to PENIS", 100, sf::Color::White, sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 4.f));

    // Create the buttons
    TitleScreen playButton("Play", 60, sf::Color::White, playButtonShape, playButtonPos, sf::Vector2f(300.f, 100.f), sf::Color::Green, sf::Color::Blue, false);
    TitleScreen exitButton("Exit", 60, sf::Color::White, exitButtonShape, exitButtonPos, sf::Vector2f(300.f, 100.f), sf::Color::Green, sf::Color::Blue, false);

    // Add the title and buttons to their respective maps
    texts_map["title"] = title;
    buttons_map["play_button"] = playButton;
    buttons_map["exit_button"] = exitButton;
}


