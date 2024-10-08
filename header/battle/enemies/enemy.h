#pragma once
#include "../header/utility.h"
#include "../header/battle/entity.h"
#include "../header/animation.h"
#include "../header/battle/collision.h"
#include "../header/battle/player/abilities/ability.h"
#include "../header/battle/enemies/exp.h"

class Enemy : public Entity {
protected:
    //textures
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    sf::Sprite sprite;
    //animation
    Animation animation;
    sf::Vector2u animationSheetDim;
    float frameDuration;
    //atributes
    bool hitPlayer;
    bool isSlowed;
    float slowDuration;
    sf::Clock attackTimer;
    float attackCooldown;
    //NEED TO BE SET BY CHILD
    int health;
    float movementSpeed;
    //movement 
    bool isMoving;
    bool facingRight;
    int totalDirections;
    std::vector<sf::Vector2f> directions;
    sf::Vector2f bestDirection;
    sf::Vector2f moveDistance;
    //hitbox
    sf::FloatRect bounds;
    //movement
    std::vector<Entity*> neighbors; //contains all neighbors within a grid-cell, including self
    void generateDirections(int numDirections=32); 
    virtual void meleeMovement(const sf::Vector2f& target);
    bool isPathBlocked(const sf::Vector2f& target, sf::Vector2f& whereBlocked);
    void avoidBlocking(const sf::Vector2f& toTarget, const sf::Vector2f& whereBlocked);
    //handleCollision
    void handleEnemyCollision(Entity& entity);
    void boxOverlap(Entity& entity1, Entity& entity2);
    void circleOverlap(Entity& entity1, Entity& entity2);
    void boxCircleOverlap(Entity& box, Entity& circle);
    void handleAbilityCollision(Entity& entity);
    //
    void loadTexture(const std::string& name, const std::string& filePath);
    void takeDebuff(sf::Vector2f debuff, bool stun); //TODO stun
    //
    virtual void checkLvlUp(const size_t level) = 0;
    void initialBestDirection(const sf::Vector2f& toTarget);
    //ENTITY setter
    virtual void setInitialPosition(const sf::FloatRect& screenBounds) override;
public:
    Enemy();
    virtual ~Enemy() = default; //TODO
    //setter
    void setNeighbors(const std::vector<Entity*>& newNeighbors);
    //fetchers
    float getAttackTimer() const;
    virtual float getAttackCooldown() const;
    //functions
    void checkAlive();
    void spawn(const size_t level, const sf::FloatRect& screenBounds);
    void restartAttackTimer();
    virtual sf::Vector2f attack() = 0;
    virtual void update(const sf::Vector2f& target) = 0;
    //ENTITY fetchers
    virtual const sf::Vector2f& getVelocity() const override;
    //ENTITY setters
    virtual void setVelocity(const sf::Vector2f& velocity) override;
    //ENTITY functions 
    virtual void applyMovement() override;
    virtual void handleCollision(Entity& entity) override;
    virtual void render(sf::RenderWindow& window) const override;
};