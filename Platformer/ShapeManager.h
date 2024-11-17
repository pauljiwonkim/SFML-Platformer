#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
class shapes
{
public:
	enum class Type { Circle, PlatformRectangle, GoalRectangle }; // types of shapes
	shapes(float radius, int x, int y, Type type); //circle
	shapes(sf::Vector2f size, int x, int y, Type type); // rectangles 
	void draw(sf::RenderWindow& window);

private:
	Type type;
	sf::CircleShape circle;
	sf::RectangleShape rectangle;

};