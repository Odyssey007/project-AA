#include "../header/battle/player/player.h"

Player::Player() :
    //animation
    animationSheetDim(sf::Vector2u(4, 2)), frameDuration(0.18),
    //player stats
    health(100.0f), battleSpeed(300.0f), kingdomSpeed(300.0f),
    //player bounds
    bounds(sf::FloatRect(50, 30, 30, 80)),
    //movement
    moveDistance(sf::Vector2f(0.0f, 0.0f)), isMoving(false), facingRight(true)
{
    //preliminaries
    entityType = PLAYER; collisionType = BOX;
    texture.loadFromFile("assets/playerSheet.png");
    sprite.setTexture(texture);
    animation = Animation(texture, animationSheetDim, frameDuration);
    sprite.setTextureRect(animation.uvRect);
    //hit box and bounds for player sprite
    hitBox.updateSize(bounds);
    //set origin
    sprite.setOrigin(sf::Vector2f((bounds.left + bounds.width/2.0f), (bounds.top + bounds.height/2.0f)));
    //abilities
    // abilities.push_back(std::make_shared<Slash>()); 
}

void Player::dash(const sf::Vector2f& mousePosition) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && dashClock.getElapsedTime().asSeconds() > dashCooldown) {
        isDashing = true;
        dashClock.restart();

        // Calculate move distance
        sf::Vector2f direction = mousePosition - sprite.getPosition();
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        moveDistance = direction / length;
        totalDashDistance = 0; 
    }

    if (isDashing) {
        if (totalDashDistance < dashDistance) {
            sf::Vector2f move;
            move.x = moveDistance.x * DeltaTime::getInstance()->getDeltaTime() * 200 * 6;
            move.y = moveDistance.y * DeltaTime::getInstance()->getDeltaTime() * 200 * 6;

            sprite.move(move);
            hitBox.followEntity(sprite.getPosition());
            totalDashDistance += magnitude(move);
        } else {
            isDashing = false; 
        }
    }
}


void Player::update(const sf::Vector2f& mousePosition) {
    dash(mousePosition);

    movement();

    for (auto& ability : abilities) {
        ability->activate(mousePosition, hitBox.body.getPosition());
    }
}

void Player::applyMovement() {
    if (moveDistance != sf::Vector2f(0.0f, 0.0f)) {
        moveDistance = normalize(moveDistance);
        moveDistance *= battleSpeed * DeltaTime::getInstance()->getDeltaTime();
        sprite.move(moveDistance);
        hitBox.followEntity(sprite.getPosition());
    }
}

void Player::movement() {
    //reset each after each movement
    isMoving = false;
    moveDistance = sf::Vector2f(0.0f, 0.0f);
    //Capture keyboard input for movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { //up
        direction = UP;
        moveDistance.y -= battleSpeed * DeltaTime::getInstance()->getDeltaTime();
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { //down
        direction = DOWN;
        moveDistance.y += battleSpeed * DeltaTime::getInstance()->getDeltaTime(); 
        isMoving = true;
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { //left
        if (facingRight) facingRight = false;
        direction = LEFT;
        moveDistance.x -= battleSpeed * DeltaTime::getInstance()->getDeltaTime();
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { //right
        if (!facingRight) facingRight = true;
        direction = RIGHT;
        moveDistance.x += battleSpeed * DeltaTime::getInstance()->getDeltaTime();
        isMoving = true;
    };
    //update the player position if moving
    if (isMoving) {
        animation.update(sprite, 1, facingRight, {1.0f, 1.0f});//moving animation
    } else {
        animation.update(sprite, 0, facingRight, {0.93f, 0.93f});//idle animation
    }
}

Direction Player::getDirection() const {
    return direction;
}

//!MAKE A FUNCTION THAT RETURN ALIVE

//ENTITY FUNCTIONS

sf::FloatRect Player::getBounds() const {
    return hitBox.body.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return hitBox.body.getPosition();
} 

const sf::Vector2f& Player::getVelocity() const {
    return moveDistance;
}

void Player::setVelocity(const sf::Vector2f& velocity) {
    moveDistance = velocity;
}

void Player::setInitialPosition(const sf::FloatRect& screenBounds) { 
    sprite.setPosition(screenBounds.width/2, screenBounds.height/2);
    hitBox.body.setPosition(screenBounds.width/2, screenBounds.height/2);
}

void Player::handleCollision(Entity& other) {
    EntityType otherEntity = other.entityType;
    if (otherEntity == ENEMY) {
        handleEnemyCollisions(other);
    } else if (otherEntity == OBSTACLE || otherEntity == BLAST) {
        return;
    }
}

void Player::handleEnemyCollisions(Entity& entity) {
    Enemy* enemy = dynamic_cast<Enemy*>(&entity);
    if (enemy->getAttackTimer() >= enemy->getAttackCooldown()) {
        takeDebuffs(enemy->attack());
        enemy->restartAttackTimer();
    }
}

void Player::render(sf::RenderWindow& window) const {
    window.draw(sprite);
    // window.draw(hitBox.body);
    for (auto& ability : abilities) {
        ability->render(window);
    }
}

//x = hp | y = ms
void Player::takeDebuffs(const sf::Vector2f& debuff) {
    health -= debuff.x;
    battleSpeed -= debuff.y; //!will not work
}

//fetchers

float Player::getHealth() {
    return health;
}