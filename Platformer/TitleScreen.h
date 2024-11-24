#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class TitleScreen {
public:
    static std::unordered_map<std::string, TitleScreen> texts_map;  // Map for text objects
    static std::unordered_map<std::string, TitleScreen> buttons_map; // Map for button objects

    // Constructor for Text
    TitleScreen(const std::string& labelText, int fontSize,
        const sf::Color& fontColor, const sf::Vector2f& textPosition);

    // Constructor for Button
    TitleScreen(const std::string& buttonText, int fontSize,
        const sf::Color& fontColor, sf::RectangleShape shape,
        const sf::Vector2f& buttonPosition, const sf::Vector2f& buttonSize,
        const sf::Color& buttonColor, const sf::Color& hoveredColor,
        bool buttonPressed);

    // Default Constructor
    TitleScreen()
        : labelText(""),
        buttonText(""),
        textPosition(0.f, 0.f),
        buttonShape(),
        buttonPosition(0.f, 0.f),
        buttonSize(0.f, 0.f),
        buttonColor(sf::Color::Transparent),
        fontSize(30),
        fontColor(sf::Color::White),
        buttonPressed(false),
        hoveredColor(sf::Color::Blue)  // Default hover color
    {
    }

    // Draw functions
    void drawText(sf::RenderWindow& window);  // Draw only text
    void drawButton(sf::RenderWindow& window);  // Draw only button

    bool isButtonPressed(const sf::Vector2f& mousePos); // Check if button was pressed
    void checkHover(const sf::Vector2f& mousePos); // Check if mouse is hovering over button

    // Initialize predefined texts and buttons (Static Method)
    static void InitializeTexts(sf::RenderWindow& window);
    static void loadFont(const std::string& fontPath);

    // **** Accessors (Getters) ****
    sf::Text& getTitleLabel() { return titleLabel; }                               // Getter for titleLabel
    sf::Text& getButtonLabel() { return buttonLabel; }                             // Getter for buttonLabel
    const std::string& getLabelText() const { return labelText; }                  // Getter for labelText
    const std::string& getButtonText() const { return buttonText; }                // Getter for buttonText
    sf::RectangleShape& getButtonShape() { return buttonShape; }                   // Getter for buttonShape
    static sf::Font& getFont() { return textFont; }                                // Getter for the font
    const sf::Color& getFontColor() const { return fontColor; }                    // Getter for fontColor
    const sf::Vector2f& getTextPosition() const { return textPosition; }           // Getter for textPosition
    int getFontSize() const { return fontSize; }                                   // Getter for fontSize
    const sf::Color& getButtonColor() const { return buttonColor; }                // Getter for buttonColor
    const sf::Color& getHoveredColor() const { return hoveredColor; }              // Getter for hoveredColor
    const sf::Vector2f& getButtonPosition() const { return buttonPosition; }       // Getter for buttonPosition
    const sf::Vector2f& getButtonSize() const { return buttonSize; }               // Getter for buttonSize
    bool isButtonPressed() const { return buttonPressed; }                         // Getter for buttonPressed

    // **** Mutators (Setters) **** ('this' refers to the current object.)
    void setTitleLabel(const sf::Text& titleLabel) { this->titleLabel = titleLabel; }    // Setter for titleLabel
    void setButtonLabel(const sf::Text& buttonLabel) { this->buttonLabel = buttonLabel; } // Setter for buttonLabel
    void setLabelText(const std::string& labelText) { this->labelText = labelText; }      // Setter for labelText
    void setButtonText(const std::string& buttonText) { this->buttonText = buttonText; }  // Setter for buttonText
    void setButtonShape(const sf::RectangleShape& buttonShape) { this->buttonShape = buttonShape; } // Setter for buttonShape
    void setFontColor(const sf::Color& fontColor) { this->fontColor = fontColor; }       // Setter for fontColor
    void setTextPosition(const sf::Vector2f& textPosition) { this->textPosition = textPosition; } // Setter for textPosition
    void setFontSize(int fontSize) { this->fontSize = fontSize; }                        // Setter for fontSize
    void setButtonColor(const sf::Color& buttonColor) { this->buttonColor = buttonColor; } // Setter for buttonColor
    void setHoveredColor(const sf::Color& hoveredColor) { this->hoveredColor = hoveredColor; } // Setter for hoveredColor
    void setButtonPosition(const sf::Vector2f& buttonPosition) { this->buttonPosition = buttonPosition; } // Setter for buttonPosition
    void setButtonSize(const sf::Vector2f& buttonSize) { this->buttonSize = buttonSize; } // Setter for buttonSize
    void setButtonPressed(bool buttonPressed) { this->buttonPressed = buttonPressed; }   // Setter for buttonPressed


private:
    static sf::Font textFont;
    sf::Font buttonFont;
    sf::RectangleShape buttonShape;

    // Text-related variables
    sf::Text titleLabel;      // Text for the title
    sf::Text buttonLabel;     // Text for the button label
    std::string labelText;      // The actual text 
    std::string buttonText;     // The text for buttons
    sf::Vector2f textPosition;  // Position of the text
    int fontSize;               // Font size for text
    sf::Color fontColor;        // Font color

    // Button-related variables
    sf::Vector2f buttonPosition; // Position of the button
    sf::Vector2f buttonSize;     // Size of the button
    sf::Color buttonColor;       // Color of the button
    bool buttonPressed;          // Flag for button pressed state
    sf::Color hoveredColor;      // Color when hovered
};

#endif // TITLESCREEN_H
