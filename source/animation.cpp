#include "../header/animation.h"
Animation::Animation()
:frameDimension(0, 0),
 currentFrame(0, 0),
 frameDuration(0.0f),
 totalTime(0.0f),
 editScale(false)
{}

Animation::Animation(sf::Texture* texture, sf::Vector2u frameDimension, float frameDuration)
{
    //initializing variables
    this->frameDimension = frameDimension; 
    this->frameDuration = frameDuration;
    currentFrame.x = 0; 
    totalTime = 0.0f; 
    //calculating width and height for texture rectangle
    uvRect.width = texture->getSize().x / frameDimension.x; 
    uvRect.height = texture->getSize().y / frameDimension.y;
}

void Animation::animationUpdate(int row, bool faceRight, sf::Sprite& sprite, sf::Vector2f scaleNum){
    currentFrame.y = row; 
    totalTime += DeltaTime::getInstance()->getDeltaTime(); 

    if(totalTime >= frameDuration) {
        totalTime -= frameDuration;
        currentFrame.x = (currentFrame.x + 1) % frameDimension.x; 
    }

    uvRect.left = currentFrame.x * uvRect.width; 
    uvRect.top = currentFrame.y * uvRect.height; 

    float originalBottom = sprite.getPosition().y + sprite.getGlobalBounds().height / 2.0f;
     if(editScale) {
        if(faceRight) {
            sprite.setScale(scaleNum.x, scaleNum.y); 
        }
        else {
            sprite.setScale(-scaleNum.x, scaleNum.y); 
        }
        editScale = false; 
    }
    else {
        if (faceRight) {
            sprite.setScale(1.0f, 1.0f); 
        } 
        else {
            sprite.setScale(-1.0f, 1.0f);
        }
    }   

    sprite.setTextureRect(uvRect);
    float newBottom = sprite.getPosition().y + sprite.getGlobalBounds().height / 2.0f;
    sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + (originalBottom - newBottom));

}


/*
    if (faceRight) {
        sprite.setScale(scaleNum.x, scaleNum.y); 
    } 
    else {
        sprite.setScale(-scaleNum.x, scaleNum.y);
    }

    float originalBottom = sprite.getPosition().y + sprite.getGlobalBounds().height;

    if(editScale) {
        if(faceRight) {
            sprite.setScale(scaleNum.x, scaleNum.y); 
            //sprite.setPosition(sprite.getPosition().x, bottom - bounds.height);
        }
        else {
            sprite.setScale(-scaleNum.x, scaleNum.y); 
            //sprite.setPosition(sprite.getPosition().x, bottom - bounds.height);
        }
        editScale = false; 
    }
    else {
        if (faceRight) {
            sprite.setScale(1.0f, 1.0f); 
        } 
        else {
            sprite.setScale(-1.0f, 1.0f);
        }
    }
    sprite.setTextureRect(uvRect);
    float newBottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
    sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + (originalBottom - newBottom));
    */
