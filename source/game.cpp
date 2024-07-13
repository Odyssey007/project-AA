#include "../header/game.h"

//preset values into setting up window
Game::Game() : 
    //window setup
    window(nullptr), resolution(sf::Vector2u(0, 0)),
    quadTree(sf::FloatRect(0, 0, 1920, 1080), 5),
    //entities
    player(std::make_shared<Player>()), 
    enemyPool(EnemyType::SLIME, 100), currentWave(1), waveTimer(sf::seconds(25)),
    objectPool(50)
{
    //preliminaries
    currentWindow();
    //entities
    player->setInitialPosition(sf::Vector2u(960, 540)); //player
    collisionManager.addEntity(player); 
    enemyPool.currentEnemies(currentWave, resolution, collisionManager);
    objectPool.currentObjects(1, collisionManager);
}

//sets up the window
void Game::currentWindow() {
    //resolution
    sf::VideoMode screen = sf::VideoMode::getDesktopMode();
    resolution.x = screen.width; resolution.y = screen.height;
    //window
    window = std::make_unique<sf::RenderWindow>(screen, "Project-AA", sf::Style::Fullscreen);
    window->setFramerateLimit(120);
    view = window->getDefaultView();
}

bool Game::winRunning() const {
    return window->isOpen();
}

void Game::update() {
    handleEvents();
    view.setCenter(player->getShape().getGlobalBounds().left, 
                   player->getShape().getGlobalBounds().top);
    //update entities
    sf::Vector2f worldMousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    player->update(worldMousePos);
    enemyPool.update(player->getShape().getPosition());
    //collision check
    collisionManager.update();
    player->applyMovement();
    enemyPool.applyMovement();
    //
    checkWave();


    // if(!quadTree.windowBounds.intersects(player->getShape().getGlobalBounds())) {
    //     quadTree.updateBounds(player->getShape().getPosition()); 
    // }
    // quadTree.clear(); 
    // quadTree.insertObject(player->getShape().getGlobalBounds()); 
    // for(auto& slime : enemyPool.activeEnemies) {
    //     quadTree.insertObject(slime->getShape().getGlobalBounds()); 
    // }
    
    checkGameEnd();
}

void Game::checkWave() {
    // std::cout << waveClock.getElapsedTime().asSeconds() << std::endl;
    if (enemyPool.allDead()) {
        currentWave++;
        enemyPool.resetEnemies(collisionManager);
        enemyPool.currentEnemies(currentWave*2, resolution, collisionManager);
        waveClock.restart();
    } else if (waveClock.getElapsedTime() >= waveTimer) {
        currentWave++;
        enemyPool.currentEnemies(currentWave*2, resolution, collisionManager);
        waveClock.restart();
    }
}

void Game::handleEvents() {
    while(window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) window->close();
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) window->close();
        }
    }
}

void Game::checkGameEnd() {
    //std::cout << player->getHealth() << std::endl;
    // if (player->getHealth() == 0) {
    //     throw std::runtime_error("Game Over\n");
    // }
}

void Game::render() {
    window->setView(view);
    window->clear();
    //entities
    objectPool.render(*window);//objects
    enemyPool.render(*window);//enemies
    player->render(*window);//player
    
    
    //quadTree.draw(*window);
    
    window->display();
}