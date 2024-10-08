#pragma once 
#include "../header/utility.h"
#include "../header/battle/player/player.h"
#include "../header/game/states.h"
#include "../header/kingdom/menu/menu.h"
#include "../header/kingdom/buildings/dynamic/castle.h"

class KingdomState : public States {
private: 
    sf::Vector2u resolution; 
    sf::Vector2f mousePos; 
    sf::View view; 

    //Entity instance 
    std::shared_ptr<Player> player;  
    KingdomMenu menu; 
public:
    //Constructor 
    KingdomState(); 

    //Inherited functions 
    virtual void handleEvents(sf::RenderWindow& window, sf::Event& event) override;
    virtual void update(sf::RenderWindow& window, sf::Event& event) override;
    virtual void render(sf::RenderWindow& window) override;
    virtual void enter(sf::RenderWindow& window) override; 
    virtual StateMode getStateType() override; 
    virtual void exit() override; 

    //Functions
};