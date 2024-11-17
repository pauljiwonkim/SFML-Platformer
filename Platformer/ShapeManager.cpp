#include "ShapeManager.h"

shapes::shapes(float radius, int x, int y, Type type) //circle shape thingy
	:type(Type::Circle)
{
	circle.setRadius(radius);
	circle.setFillColor(sf::Color::Red);
	circle.setPosition(x, y);
}

shapes::shapes(sf::Vector2f size, int x, int y, Type type) // rectangle shape thingy
	:type(type)
{
	if (type == Type::GoalRectangle) //used if/else because was having problems with compiling and this was the best way
	{
		rectangle.setSize(size);
		rectangle.setFillColor(sf::Color::Green);
		rectangle.setPosition(x, y);
	}
	else if (type == Type::PlatformRectangle)
	{
		rectangle.setSize(size);
		rectangle.setFillColor(sf::Color::Yellow);
		rectangle.setPosition(x, y);

	}
	else
	{
		std::cout << "problem with displaying rectangle shapes! \n";
	}

}

//drawing method
void shapes::draw(sf::RenderWindow& window)
{
	switch (type)
	{
	case shapes::Type::Circle:
		window.draw(circle);
		break;

	case shapes::Type::PlatformRectangle:
		window.draw(rectangle);
		break;
	case shapes::Type::GoalRectangle:
		window.draw(rectangle);
		break;
	default:
		break;

	}
}