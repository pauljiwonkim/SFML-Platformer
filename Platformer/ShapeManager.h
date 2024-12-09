#ifndef SHAPEMANAGER_H
#define SHAPEMANAGER_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include "LevelManager.h"


class ShapeManager
{
public:
    enum class ShapeType {
        Circle,
        Rectangle,
        Trapezoid
    };

    enum class PlatformType {
        Normal,
        Damaging,
        Goal
    };

    ShapeManager(); // Default constructor


    ShapeManager(float radius, const sf::Vector2f& position, const sf::Texture& texture, ShapeType shape, PlatformType type); //circle
    ShapeManager(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Texture& texture, ShapeType shape, PlatformType type); // rectangles 
    void draw(sf::RenderWindow& window) const;

    // Getters
    ShapeType getShapeType() const { return shape; }
    PlatformType getPlatformType() const { return type; }

    sf::CircleShape circle;
    sf::RectangleShape rectangle;
    void setTexture(const sf::Texture& texture);
private:
    ShapeType shape;
    PlatformType type;
    sf::Texture platformtexture;
    sf::Texture damageTexture;


};

#endif //SHAPEMANAGER