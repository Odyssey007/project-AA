#pragma once 
#include "../header/utility.h"

//COLLISION_ABILITY: dies after hitting
//TIMED_ABILITY: doesn't die after hitting
enum EntityType { 
    PLAYER, ENEMY, OBSTACLE, EXP, 
    COLLISION_ABILITY, TIMED_ABILITY,
    ENEMY_ABILITY
};
enum CollisionType { BOX, CIRCLE };

const uint8_t ABILITY_NUM = 9;

class Entity {
protected:
    bool alive;
public:
    Entity();
    EntityType entityType;
    CollisionType collisionType;
    //fetchers
    virtual bool isAlive() const;
    virtual sf::FloatRect getBounds() const = 0; //gets globalBounds()
    virtual sf::Vector2f getPosition() const = 0;
    virtual const sf::Vector2f& getVelocity() const;
    //setters
    virtual void setVelocity(const sf::Vector2f& velocity);
    virtual void setInitialPosition(const sf::FloatRect& screenBounds);
    //functions
    virtual void applyMovement(); 
    virtual void handleCollision(Entity& entity) = 0;
    virtual void render(sf::RenderWindow& window) const = 0;
};