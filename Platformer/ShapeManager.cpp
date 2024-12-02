#include "ShapeManager.h"
ShapeManager::ShapeManager() // Default constructor
    : shape(ShapeType::Rectangle), type(PlatformType::Normal) {
}

ShapeManager::ShapeManager(float radius, const sf::Vector2f& position, sf::Color color, ShapeType shape, PlatformType type) //circle shape thingy
    :shape(ShapeType::Circle)
{
    circle.setRadius(radius);
    circle.setFillColor(color);
    circle.setPosition(position);
}

ShapeManager::ShapeManager(const sf::Vector2f& size, const sf::Vector2f& position, sf::Color color, ShapeType shape, PlatformType type) // rectangle shape thingy
    :shape(shape)
{
    if (shape == ShapeType::Rectangle) //used if/else because was having problems with compiling and this was the best way
    {
        rectangle.setSize(size);
        rectangle.setFillColor(color);
        rectangle.setPosition(position);
    }
    else if (shape == ShapeType::Trapezoid)
    {
        rectangle.setSize(size);
        rectangle.setFillColor(color);
        rectangle.setPosition(position);

    }
    else
    {
        std::cout << "problem with displaying rectangle shapes! \n";
    }

}

//drawing method
void ShapeManager::draw(sf::RenderWindow& window) const
{
    switch (shape)
    {
    case ShapeManager::ShapeType::Circle:
        window.draw(circle);
        break;

    case ShapeManager::ShapeType::Rectangle:
        window.draw(rectangle);
        break;
    case ShapeManager::ShapeType::Trapezoid:
        window.draw(rectangle);
        break;
    default:
        break;

    }
}