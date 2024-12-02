#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>
#include "Player.h" 

class Camera {
public:
    // Constructor
    Camera(float width, float height, const Player& player);

    // Update the camera's position based on the player
    void update(const Player& player);

    // Apply the camera's view to the window
    void apply(sf::RenderWindow& window);

    // Set the camera boundaries
    void setBounds(float left, float top, float right, float bottom);

    // Get the camera's current position (center of the view)
    sf::Vector2f getCameraPosition() const { return view.getCenter(); }
    sf::View getView() const;

private:
    sf::View view;
    sf::Vector2f boundsMin;
    sf::Vector2f boundsMax;
};

#endif // CAMERA_H
