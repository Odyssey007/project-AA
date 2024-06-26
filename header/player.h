#pragma once
#include "utility.h"

class Player {
private:
    sf::Texture player_texture;
public:
    //variables for player rendering
    sf::Sprite player_sprite;
    sf::IntRect frameRectangle;

    //variables for player animation
    sf::Clock animationClock; 
    float frameDuration;
    int frameNumber; 
    const int totalFrames;

    //variables for player movement
    const float battleSpeed;
    const float kingdomSpeed; 
    bool isMoving;
    bool facingRight;

    //constructor
    Player(); 

    //functions
    void battleMovement();
    void battleMovement_animation();
};