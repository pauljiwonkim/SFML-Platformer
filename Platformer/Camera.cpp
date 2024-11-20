#include "Camera.h"

Camera::Camera(float width, float height, const Player& player) {
    // Set the camera's size and initial center (based on the player's position)
    view.setSize(width, height);
    view.setCenter(700.f, 700.f); // Center camera on player initially

    boundsMin = { 0.f, 0.f };  // Default bounds (can be adjusted)
    boundsMax = { 10000.f, 10000.f }; // Default bounds (can be adjusted)
}

void Camera::update(const Player& player) {

    if (player.isPlayerDead()) {
        return;
    }
    // Follow the player in X (You can change this to follow Y or both if needed)
    sf::Vector2f playerPos = player.getPosition();

    // Update camera position to follow the player
    view.setCenter(playerPos.x, view.getCenter().y); // Center only on X axis

    // Ensure the camera does not go outside the level bounds 
    if (view.getCenter().x < boundsMin.x) {
        view.setCenter(boundsMin.x, view.getCenter().y); // Left bound
    }
    if (view.getCenter().x > boundsMax.x) {
        view.setCenter(boundsMax.x, view.getCenter().y); // Right bound
    }
}

void Camera::apply(sf::RenderWindow& window) {
    // Set the window's view to the camera's view
    window.setView(view);
}

void Camera::setBounds(float left, float top, float right, float bottom) {
    // Set the minimum and maximum bounds for the camera to prevent it from moving outside the level
    boundsMin = { left, top };
    boundsMax = { right, bottom };
}
