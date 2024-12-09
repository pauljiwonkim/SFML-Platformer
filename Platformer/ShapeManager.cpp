#include "ShapeManager.h"
ShapeManager::ShapeManager() // Default constructor
    : shape(ShapeType::Rectangle), type(PlatformType::Normal) {
}

ShapeManager::ShapeManager(float radius, const sf::Vector2f& position, const sf::Texture& texture, ShapeType shape, PlatformType type) //circle shape thingy
    :shape(ShapeType::Circle), type(type)
{
    circle.setRadius(radius);
    circle.setPosition(position);
    circle.setTexture(&texture);
    std::cout << "Assigned texture to circle at position: "
        << position.x << ", " << position.y << std::endl;
}

ShapeManager::ShapeManager(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Texture& texture, ShapeType shape, PlatformType type) // rectangles 
    :shape(ShapeType::Rectangle), type(type)
{

    if (shape == ShapeType::Rectangle) //used if/else because was having problems with compiling and this was the best way
    {
        rectangle.setSize(size);
        rectangle.setPosition(position);
        rectangle.setTexture(&texture);
        std::cout << "Assigned texture to rectangle at position: "
            << position.x << ", " << position.y << std::endl;


    }
    else if (shape == ShapeType::Trapezoid)
    {
        rectangle.setSize(size);
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
    if (rectangle.getTexture() != nullptr) {
        std::cout << "Rectangle texture is set." << std::endl;
    }
    else {
        std::cerr << "Rectangle texture is NOT set!" << std::endl;
    }

    switch (shape)
    {
    case ShapeManager::ShapeType::Circle:
        if (circle.getTexture()) {
            std::cout << "Circle texture is set at position: "
                << circle.getPosition().x << ", "
                << circle.getPosition().y << std::endl;

        }
        else {
            std::cerr << "Circle texture is NOT set!" << std::endl;
        }
        window.draw(circle);
        break;

    case ShapeManager::ShapeType::Rectangle:
        if (rectangle.getTexture()) {
            std::cout << "Rectangle texture is set at position: "
                << rectangle.getPosition().x << ", "
                << rectangle.getPosition().y << std::endl;
        }
        else if (rectangle.getTexture() != nullptr){
            std::cout << "Rectangle texture is set." << std::endl;
    }
        else {
            std::cerr << "Rectangle texture is NOT set!" << std::endl;
        }
        window.draw(rectangle);
        break;
    case ShapeManager::ShapeType::Trapezoid:
        window.draw(rectangle);
        break;
    default:
        break;

    }
}

void ShapeManager::setTexture(const sf::Texture& texture)
{
    if (shape == ShapeType::Rectangle) {

        rectangle.setTexture(&texture);

    }
    else if (shape == ShapeType::Circle) {
        circle.setTexture(&texture);

    }
}